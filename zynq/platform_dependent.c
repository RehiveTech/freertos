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
