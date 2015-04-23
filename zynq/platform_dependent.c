/**
 * platform_dependent.c
 * Copyright (C) 2014 Jan Viktorin
 */

#include <limits.h>
#include <FreeRTOS.h>
#include "platform_dependent.h"

XScuWdt xWatchDogInstance;
XScuGic xInterruptController;

extern void vPortInstallFreeRTOSVectorTable(void);

void platSetupHardware(void)
{
	BaseType_t xStatus;
	XScuGic_Config *pxGICConfig;

	/* Ensure no interrupts execute while the scheduler is in an inconsistent
	state.  Interrupts are automatically enabled when the scheduler is
	started. */
	portDISABLE_INTERRUPTS();

	/* Obtain the configuration of the GIC. */
	pxGICConfig = XScuGic_LookupConfig( XPAR_SCUGIC_SINGLE_DEVICE_ID );

	/* Sanity check the FreeRTOSConfig.h settings are correct for the
	hardware. */
	configASSERT( pxGICConfig );
	configASSERT( pxGICConfig->CpuBaseAddress == ( configINTERRUPT_CONTROLLER_BASE_ADDRESS + configINTERRUPT_CONTROLLER_CPU_INTERFACE_OFFSET ) );
	configASSERT( pxGICConfig->DistBaseAddress == configINTERRUPT_CONTROLLER_BASE_ADDRESS );

	/* Install a default handler for each GIC interrupt. */
	xStatus = XScuGic_CfgInitialize( &xInterruptController, pxGICConfig, pxGICConfig->CpuBaseAddress );
	configASSERT( xStatus == XST_SUCCESS );
	( void ) xStatus; /* Remove compiler warning if configASSERT() is not defined. */

	/* The Xilinx projects use a BSP that do not allow the start up code to be
	altered easily. Therefore the vector table used by FreeRTOS is defined in
	asm_vectors.S, which is part of this project. Switch to use the FreeRTOS vector table. */
	vPortInstallFreeRTOSVectorTable();
}

void vInitialiseTimerForRunTimeStats( void )
{
	XScuWdt_Config *pxWatchDogInstance;
	uint32_t ulValue;
	const uint32_t ulMaxDivisor = 0xff, ulDivisorShift = 0x08;

	pxWatchDogInstance = XScuWdt_LookupConfig( XPAR_SCUWDT_0_DEVICE_ID );
	XScuWdt_CfgInitialize( &xWatchDogInstance, pxWatchDogInstance, pxWatchDogInstance->BaseAddr );

	ulValue = XScuWdt_GetControlReg( &xWatchDogInstance );
	ulValue |= ulMaxDivisor << ulDivisorShift;
	XScuWdt_SetControlReg( &xWatchDogInstance, ulValue );

	XScuWdt_LoadWdt( &xWatchDogInstance, UINT_MAX );
	XScuWdt_SetTimerMode( &xWatchDogInstance );
	XScuWdt_Start( &xWatchDogInstance );
}

#define TLB_ATTR_COMMON    0x041e2
#define TLB_ATTR_SHAREABLE 0x10000 /* S = b1 */
#define TLB_ATTR_NONCACHE  0x00000 /* TEX(1:0) = b00, C = b0, B = b0 */
#define TLB_ATTR_FULLACC   0x00C00 /* AP(2) = b0, AP(1:0) = b11 */

int platInitCoherentMemory(uint8_t *m, size_t size)
{
	size_t base = (size_t) m;
	const size_t end  = base + __plat_coherent_align(size);

	configASSERT((base & __PLAT_COHERENT_GRANULARITY_MASK) == 0);
	configASSERT((end  & __PLAT_COHERENT_GRANULARITY_MASK) == 0);

	/* FIXME: what about clearing zeros? */
	const uint32_t attr_coherent = TLB_ATTR_COMMON
		| TLB_ATTR_SHAREABLE | TLB_ATTR_NONCACHE | TLB_ATTR_FULLACC;

	while(base < end) {
		Xil_SetTlbAttributes((uint32_t) base, attr_coherent);
		base += __PLAT_COHERENT_GRANULARITY_SIZE;
	}

	memset(m, 0, __plat_coherent_align(size));
	return 0;
}
