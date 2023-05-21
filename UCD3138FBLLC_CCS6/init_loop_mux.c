//###########################################################################
//
// FILE:    init_loop_mux.c
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
#include "cyclone_device.h"   // UCD30xx Headers Include File
#include "software_interrupts.h"
#include "system_defines.h"
#include "function_definitions.h"

void init_loop_mux(void)
{
	//Set which DPWMs output connects to the filter as an input
	//DPWM-0 connects to filter for resonant duty
	LoopMuxRegs.FILTERMUX.bit.FILTER0_PER_SEL = 0;    

	//Set filter KCOMP value (which is the maximum PERIOD)
	//Use value in KCOMP-0 register
	LoopMuxRegs.FILTERMUX.bit.FILTER0_KCOMP_SEL = 0;

	//Connect FILTER-0 to DPWM-0
	LoopMuxRegs.DPWMMUX.bit.DPWM0_FILTER_SEL = 0;
	//Connect FILTER-0 to DPWM-1
	LoopMuxRegs.DPWMMUX.bit.DPWM1_FILTER_SEL = 0;
	//Connect FILTER-0 to DPWM-2
	LoopMuxRegs.DPWMMUX.bit.DPWM2_FILTER_SEL = 0;
	//Connect filter 0 to front end 0
	LoopMuxRegs.FILTERMUX.bit.FILTER0_FE_SEL = 0;
	//Connect filter 1 to front end 1
	LoopMuxRegs.FILTERMUX.bit.FILTER1_FE_SEL = 1;

	//Connect filter 2 to front end 2. 
	//This is an experiment to fix burst mode.
	LoopMuxRegs.FILTERMUX.bit.FILTER2_FE_SEL = 0;

	LoopMuxRegs.FECTRL0MUX.bit.DPWM3_FRAME_SYNC_EN = 1;
	LoopMuxRegs.LLCTRL.bit.LL_FILTER_SEL = 0;
	LoopMuxRegs.LLCTRL.bit.DPWM_ON_TIME = 0;
	LoopMuxRegs.LLCTRL.bit.CYCLE_CNT_EN = 0;
	LoopMuxRegs.LLENTHRESH.bit.CYCLE_CNT_THRESH = 0;

	//configure_burst_mode();
	configure_cc_dac_value();
}

void global_enable(void)
{
	//Enable DPWM0, DPWM1,DPWM2, FE_CTRL0 AND FE_CTRL1 simultaneously.
	LoopMuxRegs.GLBEN.all = GLBEN_BIT_MASK_FE0 	 | GLBEN_BIT_MASK_FE1 | 
							GLBEN_BIT_MASK_DPWM0 | GLBEN_BIT_MASK_DPWM1 | GLBEN_BIT_MASK_DPWM2 | GLBEN_BIT_MASK_DPWM3;// GLBEN_BIT_MASK_DPWM2 is added by qys 2013/07/03
}

void global_disable(void)
{
	//Disable DPWM0, DPMW1 simultaneously.
	LoopMuxRegs.GLBEN.all = GLBEN_BIT_MASK_FE0 	 | GLBEN_BIT_MASK_FE1;
}
