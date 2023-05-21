//###########################################################################
//
// FILE:    init_cpcc.c
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

void init_cpcc(void)
{
	//For Front End tied to Voltage Sense for CPCC, set DACn_SEL to 3 
	//and set EXT_DACn_EN bit to 1
	//Use Constant Power Module for Voltage DAC source
	LoopMuxRegs.EXTDACCTRL.bit.DAC0_SEL = 3; 
	//Setup connection for each filter in use to KCOMP
	LoopMuxRegs.FILTERMUX.bit.FILTER1_KCOMP_SEL = 0;
	//Setup DPWM switching period input for each filter
	LoopMuxRegs.FILTERMUX.bit.FILTER1_PER_SEL = 0;
	//Setup which Front End drives the filter
	LoopMuxRegs.FILTERMUX.bit.FILTER0_FE_SEL = 0;
	LoopMuxRegs.FILTERMUX.bit.FILTER1_FE_SEL = 1;
	//Setup which filter or CPCC module or Light Load Module drives each DPWM in use
	//constant power module?  want both constant power and light load
//	LoopMuxRegs.DPWMMUX.bit.DPWM0_FILTER_SEL = 3; 
	//constant power module?  want both constant power and light load
//	LoopMuxRegs.DPWMMUX.bit.DPWM1_FILTER_SEL = 3; 
	//Constant Power Control Register
	LoopMuxRegs.CPCTRL.bit.DAC_COMP_EN = 0;
	//Set CPCC_CONFIG to 1 for Loop Oring
	LoopMuxRegs.CPCTRL.bit.CPCC_CONFIG = 1;
	//Set DIVISOR_SEL to Front End used for Current Sensing
	LoopMuxRegs.CPCTRL.bit.FW_DIVISOR_EN = 0;
	LoopMuxRegs.CPCTRL.bit.DIVISOR_SEL = 1;
	//Set THRESH_SEL to Front End used for Current Sensing
	LoopMuxRegs.CPCTRL.bit.THRESH_SEL = 4;
	//Set CLOOP_SEL to Front End used for Current Sensing
	LoopMuxRegs.CPCTRL.bit.CLOOP_SEL = 1;
	//Set VLOOP_SEL to Front End used for Voltage Sensing
	LoopMuxRegs.CPCTRL.bit.VLOOP_SEL = 0;
	//Set VLOOP_FREEZE_EN to 1 if you want to freeze integrators when loop not in control
	LoopMuxRegs.CPCTRL.bit.VLOOP_FREEZE_EN = 1;
	//Set LOWER_COMP_EN to 1 to enable loop oring configuration 
	//(minimum of voltage/current sent to DPWM)
	LoopMuxRegs.CPCTRL.bit.LOWER_COMP_EN = 1;
	LoopMuxRegs.CPCTRL.bit.CPCC_INT_EN = 0;
	//Set INT_THRESH value for offset when attempting to determine when to freeze integrator
	LoopMuxRegs.EXTDACCTRL.bit.EXT_DAC0_EN = 0;

	configure_constant_power();
	configure_cc_dac_value();
}


