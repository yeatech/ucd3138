//###########################################################################
//
// FILE:    init_current_sharing.c
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

void enable_current_sharing(void)
{
	if (pmbus_dcdc_config[0].ishare_enable)
	{
		FeCtrl0Regs.EADCDAC.bit.DAC_DITHER_EN = 1;
		//3 for pass through
		//1 for DPWM current source control 2.5 uA into 100 kOhm at 100% duty cycle.
		MiscAnalogRegs.CSCTRL.bit.TEST_MODE = 3;
		//300 kHz
		MiscAnalogRegs.CSCTRL.bit.DPWM_PERIOD = 108;
	}
}

void disable_current_sharing(void)
{
	FeCtrl0Regs.EADCDAC.bit.DAC_DITHER_EN = 0;
	//3 for pass through, 1 for DPWM current source control 2.5 uA into 100 kOhm at 100% duty cycle.
	MiscAnalogRegs.CSCTRL.bit.TEST_MODE = 0;
}
