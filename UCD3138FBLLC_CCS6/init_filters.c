//###########################################################################
//
// FILE:    init_filters.c
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
//############################################################################include "system_defines.h"
#include "cyclone_device.h"
#include "pmbus_commands.h"
#include "pmbus.h"
#include "variables.h"
#include "function_definitions.h"
#include "software_interrupts.h"
#include "system_defines.h"


void init_filter0(void)
{
	//Setup connection for max duty/period into Filter.
	//Input to filter for PERIOD max comes from LOOP MUX (KCOMP)
	Filter0Regs.FILTERCTRL.bit.PERIOD_MULT_SEL = 1;  
	//Input to filter for DUTY max comes from DPWM
	Filter0Regs.FILTERCTRL.bit.OUTPUT_MULT_SEL = 3;  
	Filter0Regs.FILTERCTRL.bit.FILTER_EN = 1;
	//Filter0Regs.FILTEROCLPHI.all = 0x3FFFF;
	//Filter0Regs.FILTEROCLPLO.all = 0;
	Filter0Regs.FILTERCTRL.bit.OUTPUT_SCALE = 0;  
	Filter0Regs.FILTERCTRL.bit.USE_CPU_SAMPLE = 0;

	copy_coefficients_to_filter(&Filter0Regs, &filter0_pmbus_regs);
}

void init_filter1(void)
{
	//Setup connection for max duty/period into Filter.
	//Input to filter for PERIOD max comes from LOOP MUX (KCOMP)
	Filter1Regs.FILTERCTRL.bit.PERIOD_MULT_SEL = 1;
	//Input to filter for DUTY max comes from DPWM
	Filter1Regs.FILTERCTRL.bit.OUTPUT_MULT_SEL = 3;
	Filter1Regs.FILTERCTRL.bit.FILTER_EN = 1;
	//Filter1Regs.FILTEROCLPHI.all = 0x3FFFF;
	//Filter1Regs.FILTEROCLPLO.all = 0;
	Filter1Regs.FILTERCTRL.bit.OUTPUT_SCALE = 0;  
	Filter1Regs.FILTERCTRL.bit.USE_CPU_SAMPLE = 0;

	copy_coefficients_to_filter(&Filter1Regs, &filter1_pmbus_regs);
}	

void init_filter2(void)
{
	//Setup connection for max duty/period into Filter.
	//Input to filter for PERIOD max comes from LOOP MUX (KCOMP)
	Filter2Regs.FILTERCTRL.bit.PERIOD_MULT_SEL = 1;
	//Input to filter for DUTY max comes from DPWM
	Filter2Regs.FILTERCTRL.bit.OUTPUT_MULT_SEL = 3;
	Filter2Regs.FILTERCTRL.bit.FILTER_EN = 1;
	//Filter1Regs.FILTEROCLPHI.all = 0x3FFFF;
	//Filter1Regs.FILTEROCLPLO.all = 0;
	Filter2Regs.FILTERCTRL.bit.OUTPUT_SCALE = 0;  
	Filter2Regs.FILTERCTRL.bit.USE_CPU_SAMPLE = 0;

	Filter2Regs.COEFCONFIG.all = 0;
	Filter2Regs.FILTERKPCOEF0.bit.KP_COEF_0 = 1;
	Filter2Regs.FILTERKICOEF0.bit.KI_COEF_0 = 0;
	Filter2Regs.FILTERKDCOEF0.bit.KD_COEF_0 = 0;
	Filter2Regs.FILTERKDALPHA.bit.KD_ALPHA_0 = 0;
}

void init_filter0_states(void)
{
	//Jam XN_M1 to 0
    Filter0Regs.FILTERPRESET.bit.PRESET_REG_SEL = 0; 
	Filter0Regs.FILTERPRESET.bit.PRESET_VALUE = 0;
	Filter0Regs.FILTERPRESET.bit.PRESET_EN = 1;
	//PRESET KI_YN to 0
	Filter0Regs.FILTERPRESET.bit.PRESET_REG_SEL = 1; 
	Filter0Regs.FILTERPRESET.bit.PRESET_VALUE = 0;
	Filter0Regs.FILTERPRESET.bit.PRESET_EN = 1;
	//PRESET KD_YN to 0
	Filter0Regs.FILTERPRESET.bit.PRESET_REG_SEL = 2; 
	Filter0Regs.FILTERPRESET.bit.PRESET_VALUE = 0;
	Filter0Regs.FILTERPRESET.bit.PRESET_EN = 1;
	//PRESET YN to 0
	Filter0Regs.FILTERPRESET.bit.PRESET_REG_SEL = 3; 
	Filter0Regs.FILTERPRESET.bit.PRESET_VALUE = 0;
	Filter0Regs.FILTERPRESET.bit.PRESET_EN = 1;
	//PRESET 18 bit filter duty register (after multiplication) to 0
	Filter0Regs.FILTERPRESET.bit.PRESET_REG_SEL = 4; 
	Filter0Regs.FILTERPRESET.bit.PRESET_VALUE = 0;
	Filter0Regs.FILTERPRESET.bit.PRESET_EN = 1;

	Filter0Regs.FILTERCTRL.bit.USE_CPU_SAMPLE = 1;

	Filter0Regs.FILTERCTRL.bit.FORCE_START = 1;

	Filter0Regs.FILTERCTRL.bit.FORCE_START = 0;

	Filter0Regs.FILTERCTRL.bit.USE_CPU_SAMPLE = 0;
}

void init_filter1_states(void)
{
	//PRESET XN_M1 to 0
    Filter1Regs.FILTERPRESET.bit.PRESET_REG_SEL = 0; 
	Filter1Regs.FILTERPRESET.bit.PRESET_VALUE = 0;
	Filter1Regs.FILTERPRESET.bit.PRESET_EN = 1;
	//PRESET KI_YN to 0
	Filter1Regs.FILTERPRESET.bit.PRESET_REG_SEL = 1; 
	Filter1Regs.FILTERPRESET.bit.PRESET_VALUE = MAX_FILTER_VALUE;
	Filter1Regs.FILTERPRESET.bit.PRESET_EN = 1;
	//PRESET KD_YN to 0
	Filter1Regs.FILTERPRESET.bit.PRESET_REG_SEL = 2; 
	Filter1Regs.FILTERPRESET.bit.PRESET_VALUE = MAX_FILTER_VALUE;
	Filter1Regs.FILTERPRESET.bit.PRESET_EN = 1;
	//PRESET YN to 0
	Filter1Regs.FILTERPRESET.bit.PRESET_REG_SEL = 3; 
	Filter1Regs.FILTERPRESET.bit.PRESET_VALUE = MAX_FILTER_VALUE;
	Filter1Regs.FILTERPRESET.bit.PRESET_EN = 1;
	//PRESET 18 bit filter duty register (after multiplication) to 0
	Filter1Regs.FILTERPRESET.bit.PRESET_REG_SEL = 4; 
	Filter1Regs.FILTERPRESET.bit.PRESET_VALUE = 0;
	Filter1Regs.FILTERPRESET.bit.PRESET_EN = 1;

	Filter1Regs.FILTERCTRL.bit.USE_CPU_SAMPLE = 1;

	Filter1Regs.FILTERCTRL.bit.FORCE_START = 1;

	Filter1Regs.FILTERCTRL.bit.FORCE_START = 0;

	Filter1Regs.FILTERCTRL.bit.USE_CPU_SAMPLE = 0;
}


void stop_filter_states(void)
{
	Dpwm0Regs.DPWMINT.bit.PRD_INT_EN = 0;
	Filter0Regs.FILTERCTRL.all |=  (Uint32)(KD_STALL_MASK | KI_STALL_MASK);
	Filter1Regs.FILTERCTRL.all |=  (Uint32)(KD_STALL_MASK | KI_STALL_MASK);
}

void start_filter_states(void)
{
	Filter0Regs.FILTERCTRL.all &= ~(Uint32)(KD_STALL_MASK | KI_STALL_MASK);
	Filter1Regs.FILTERCTRL.all &= ~(Uint32)(KD_STALL_MASK | KI_STALL_MASK);
	Dpwm0Regs.DPWMINT.bit.PRD_INT_EN = 1;
}


