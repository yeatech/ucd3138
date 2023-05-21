//###########################################################################
//
// FILE:    gpio.c
//
// TITLE:
//
// NOTES:
//  1)
//###########################################################################
//
//  Ver  | dd mmm yyyy | Who  		| Description of changes
// ======|=============|============|========================================
//  00 	   06 15 2015 	 HPCS
//
//  Texas Instruments, Inc
//  Copyright Texas Instruments 2008. All rights reserved.
//###########################################################################
#include "system_defines.h"
#include "cyclone_device.h"
#include "pmbus_commands.h"
#include "pmbus.h"
#include "variables.h"
#include "function_definitions.h"
#include "software_interrupts.h"
#include "cyclone_defines.h"

void init_gpio(void)
{
	// Set outputs to 0.
	MiscAnalogRegs.GLBIOVAL.all = 0;
	// 1 - Output, 0 - Input
	MiscAnalogRegs.GLBIOOE.all = MASK_PGOOD | MASK_FAILURE | MASK_AC_FAIL | MASK_ORING_CTRL |
								 TDI_GLBIO_BIT_MASK | TMS_GLBIO_BIT_MASK |
								 DPWM0A_GLBIO_BIT_MASK | DPWM0B_GLBIO_BIT_MASK |
								 DPWM1A_GLBIO_BIT_MASK | DPWM1B_GLBIO_BIT_MASK | 
								 DPWM2A_GLBIO_BIT_MASK | DPWM2B_GLBIO_BIT_MASK | 
								 DPWM3A_GLBIO_BIT_MASK | DPWM3B_GLBIO_BIT_MASK;

	// 1 - Indicates that control is governed by the GPIO module.
	// 0 - Indicates that control is done by the default peripherial.
	MiscAnalogRegs.GLBIOEN.all = MASK_PGOOD | MASK_FAILURE | MASK_AC_FAIL | MASK_ORING_CTRL |
								 TDI_GLBIO_BIT_MASK | TMS_GLBIO_BIT_MASK |
								 MASK_PSON | CONTROL_GLBIO_BIT_MASK | 
								 DPWM2A_GLBIO_BIT_MASK | DPWM2B_GLBIO_BIT_MASK |
								 DPWM3A_GLBIO_BIT_MASK | DPWM3B_GLBIO_BIT_MASK;

	//Setting this to 1 disables the external oring control. The Oring MOSFET will always be off.
	//Set it to 0 and diode oring will operate normally.
	MiscAnalogRegs.GLBIOVAL.bit.FAULT1_IO_VALUE = 0;
	MiscAnalogRegs.GLBIOVAL.bit.DPWM2A_IO_VALUE = 0;
	MiscAnalogRegs.GLBIOVAL.bit.DPWM2B_IO_VALUE = 0;
}

void gpio_sr_on(void)
{
	MiscAnalogRegs.GLBIOEN.all &= ~((Uint32)(DPWM1A_GLBIO_BIT_MASK | DPWM1B_GLBIO_BIT_MASK));
}

void gpio_sr_off(void)
{
	MiscAnalogRegs.GLBIOEN.all |=  ((Uint32)(DPWM1A_GLBIO_BIT_MASK | DPWM1B_GLBIO_BIT_MASK));
}

void gpio_dpwm_on(void)
{
	MiscAnalogRegs.GLBIOEN.all &= ~((Uint32)(DPWM0A_GLBIO_BIT_MASK | DPWM0B_GLBIO_BIT_MASK |
								 	 		 DPWM1A_GLBIO_BIT_MASK | DPWM1B_GLBIO_BIT_MASK | 
								 	 		 DPWM2A_GLBIO_BIT_MASK | DPWM2B_GLBIO_BIT_MASK));// ADD by qys
}

void gpio_dpwm_off(void)
{
	MiscAnalogRegs.GLBIOEN.all |=  ((Uint32)(DPWM0A_GLBIO_BIT_MASK | DPWM0B_GLBIO_BIT_MASK |
								 		     DPWM1A_GLBIO_BIT_MASK | DPWM1B_GLBIO_BIT_MASK |
								 		     DPWM2A_GLBIO_BIT_MASK | DPWM2B_GLBIO_BIT_MASK));// ADD by qys
}
