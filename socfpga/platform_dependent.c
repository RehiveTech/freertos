/**
 * platform_dependent.c
 * Copyright (C) 2014 Jan Viktorin
 */

#include <limits.h>
#include <FreeRTOS.h>
#include "platform_dependent.h"

/* configAPPLICATION_ALLOCATED_HEAP is set to 1 in FreeRTOSConfig.h so the
application can define the array used as the FreeRTOS heap.  This is done so the
heap can be forced into fast internal RAM - useful because the stacks used by
the tasks come from this space. */
uint8_t ucHeap[configTOTAL_HEAP_SIZE] __attribute__((section(".oc_ram")));

/* FreeRTOS uses its own interrupt handler code.  This code cannot use the array
of handlers defined by the Altera drivers because the array is declared static,
and so not accessible outside of the dirver's source file.  Instead declare an
array for use by the FreeRTOS handler.  See:
http://www.freertos.org/Using-FreeRTOS-on-Cortex-A-Embedded-Processors.html. */
static INT_DISPATCH_t xISRHandlers[ALT_INT_PROVISION_INT_COUNT];

void platSetupHardware(void)
{
	extern uint8_t __cs3_interrupt_vector;
	uint32_t ulSCTLR, ulVectorTable = ( uint32_t ) &__cs3_interrupt_vector;
	const uint32_t ulVBit = 13U;

	alt_int_global_init();
	alt_int_cpu_binary_point_set( 0 );

	/* Clear SCTLR.V for low vectors and map the vector table to the beginning
	of the code. */
	__asm( "MRC p15, 0, %0, c1, c0, 0" : "=r" ( ulSCTLR ) );
	ulSCTLR &= ~( 1 << ulVBit );
	__asm( "MCR p15, 0, %0, c1, c0, 0" : : "r" ( ulSCTLR ) );
	__asm( "MCR p15, 0, %0, c12, c0, 0" : : "r" ( ulVectorTable ) );

	cache_init();
	mmu_init();
}

void vConfigureTickInterrupt(void)
{
	alt_freq_t ulTempFrequency;
	const alt_freq_t ulMicroSecondsPerSecond = 1000000UL;
	void FreeRTOS_Tick_Handler( void );

	/* Interrupts are disabled when this function is called. */

	/* Initialise the general purpose timer modules. */
	alt_gpt_all_tmr_init();

	/* ALT_CLK_MPU_PERIPH = mpu_periph_clk */
	alt_clk_freq_get( ALT_CLK_MPU_PERIPH, &ulTempFrequency );

	/* Use the local private timer. */
	alt_gpt_counter_set( ALT_GPT_CPU_PRIVATE_TMR, ulTempFrequency / configTICK_RATE_HZ );

	/* Sanity check. */
	configASSERT( alt_gpt_time_microsecs_get( ALT_GPT_CPU_PRIVATE_TMR ) == ( ulMicroSecondsPerSecond / configTICK_RATE_HZ ) );

	/* Set to periodic mode. */
	alt_gpt_mode_set( ALT_GPT_CPU_PRIVATE_TMR, ALT_GPT_RESTART_MODE_PERIODIC );

	/* The timer can be started here as interrupts are disabled. */
	alt_gpt_tmr_start( ALT_GPT_CPU_PRIVATE_TMR );

	/* Register the standard FreeRTOS Cortex-A tick handler as the timer's
	interrupt handler.  The handler clears the interrupt using the
	configCLEAR_TICK_INTERRUPT() macro, which is defined in FreeRTOSConfig.h. */
	vRegisterIRQHandler( ALT_INT_INTERRUPT_PPI_TIMER_PRIVATE, ( alt_int_callback_t ) FreeRTOS_Tick_Handler, NULL );

	/* This tick interrupt must run at the lowest priority. */
	alt_int_dist_priority_set( ALT_INT_INTERRUPT_PPI_TIMER_PRIVATE, portLOWEST_USABLE_INTERRUPT_PRIORITY << portPRIORITY_SHIFT );

	/* Ensure the interrupt is forwarded to the CPU. */
	alt_int_dist_enable( ALT_INT_INTERRUPT_PPI_TIMER_PRIVATE );

	/* Finally, enable the interrupt. */
	alt_gpt_int_clear_pending( ALT_GPT_CPU_PRIVATE_TMR );
	alt_gpt_int_enable( ALT_GPT_CPU_PRIVATE_TMR );
}

void vRegisterIRQHandler(uint32_t ulID, alt_int_callback_t pxHandlerFunction, void *pvContext)
{
	if( ulID < ALT_INT_PROVISION_INT_COUNT )
	{
		xISRHandlers[ ulID ].pxISR = pxHandlerFunction;
		xISRHandlers[ ulID ].pvContext = pvContext;
	}
}

void vApplicationIRQHandler(uint32_t ulICCIAR)
{
	uint32_t ulInterruptID;
	void *pvContext;
	alt_int_callback_t pxISR;

	/* Re-enable interrupts. */
	__asm ( "cpsie i" );

	/* The ID of the interrupt is obtained by bitwise anding the ICCIAR value
	with 0x3FF. */
	ulInterruptID = ulICCIAR & 0x3FFUL;

	if( ulInterruptID < ALT_INT_PROVISION_INT_COUNT )
	{
		/* Call the function installed in the array of installed handler
		functions. */
		pxISR = xISRHandlers[ ulInterruptID ].pxISR;
		pvContext = xISRHandlers[ ulInterruptID ].pvContext;
		pxISR( ulICCIAR, pvContext );
	}
}
