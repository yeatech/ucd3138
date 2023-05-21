//###########################################################################
//
// FILE:    init_front_ends.c
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

void init_front_end0(void)
{
	//Initialize DAC to 0.
	FeCtrl0Regs.EADCDAC.bit.DAC_VALUE = 0;
	// Initialize Front End Gain
	FeCtrl0Regs.EADCCTRL.bit.AFE_GAIN = 3;
	// 0---16 MHz sampling frequency, 1---8MHz sampling frequency
	FeCtrl0Regs.EADCCTRL.bit.SCFE_CLK_DIV_2 = 0;
	//0 - standard, 1 - averaging mode
	FeCtrl0Regs.EADCCTRL.bit.EADC_MODE = 0;
	//Averaging config 0 - 2x, 1 - 4x, 2 - 8x
	FeCtrl0Regs.EADCCTRL.bit.AVG_MODE_SEL = 2;
	//Sample triggers are configured by the DPWM
	FeCtrl0Regs.EADCCTRL.bit.AVG_SPATIAL_EN = 1;

	// Configure soft start ramp
	FeCtrl0Regs.RAMPCYCLE.bit.DELAY_CYCLES = 0;//no delay before ramp up, delay time can setup before ramp up
	FeCtrl0Regs.RAMPCYCLE.bit.SWITCH_CYC_PER_STEP = 0;//1 switching cycle per DAC step
	FeCtrl0Regs.RAMPCTRL.bit.RAMP_SAT_EN = 0;//0 is default, this command is OK to delete
	FeCtrl0Regs.RAMPCTRL.bit.RAMP_EN = 1;
}

void init_front_end1(void)
{
	// Initialize Front End Gain
	FeCtrl1Regs.EADCCTRL.bit.AFE_GAIN       = 3;
	// Use 16 MHz sampling frequency
	FeCtrl1Regs.EADCCTRL.bit.SCFE_CLK_DIV_2 = 0;
	// Set Front End in Continuous Mode
	FeCtrl1Regs.EADCCTRL.bit.EADC_MODE      = 0;

}

void init_front_end2(void)
{
}

