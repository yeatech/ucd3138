//###########################################################################
//
// FILE:    init_fault_mux.c
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

void init_fault_mux(void)
{
}

void init_uvp(void)//VOUT UVP
{
	FaultMuxRegs.ACOMPCTRL0.bit.ACOMP_EN = 1;           //Enable analog comparators
	FaultMuxRegs.ACOMPCTRL0.bit.ACOMP_B_SEL = 0;        //Configures reference of analog comparator B to ACOMP_B_THRESH
	FaultMuxRegs.ACOMPCTRL0.bit.ACOMP_B_POL = 0;        //comparator result enabled when input lower than threshold
	
	configure_uvp();
}

void init_ovp(void)
{
	FaultMuxRegs.ACOMPCTRL0.bit.ACOMP_EN = 1;           //Enable analog comparators
	FaultMuxRegs.ACOMPCTRL0.bit.ACOMP_B_SEL = 0;        //Configures reference of analog comparator B to ACOMP_B_THRESH
	FaultMuxRegs.ACOMPCTRL0.bit.ACOMP_B_POL = 1;        //Polarity of comparator when an event is detected.
	FaultMuxRegs.DPWM0FLTABDET.bit.ACOMP_B_EN = 1;     //both DPWMA and DPWMB is turned off when ACOMP B action. is not default option
	FaultMuxRegs.DPWM1FLTABDET.bit.ACOMP_B_EN = 1;
	FaultMuxRegs.DPWM2FLTABDET.bit.ACOMP_B_EN = 1;
	FaultMuxRegs.DPWM3FLTABDET.bit.ACOMP_B_EN = 1;

	Dpwm0Regs.DPWMFLTCTRL.bit.ALL_FAULT_EN = 1;
	Dpwm0Regs.DPWMFLTCTRL.bit.AB_MAX_COUNT = 4;
	Dpwm1Regs.DPWMFLTCTRL.bit.ALL_FAULT_EN = 1;
	Dpwm1Regs.DPWMFLTCTRL.bit.AB_MAX_COUNT = 4;
//add by qys for DPWM2
	Dpwm2Regs.DPWMFLTCTRL.bit.ALL_FAULT_EN = 1;
	Dpwm2Regs.DPWMFLTCTRL.bit.AB_MAX_COUNT = 4;
	
	Dpwm3Regs.DPWMFLTCTRL.bit.ALL_FAULT_EN = 1;
	Dpwm3Regs.DPWMFLTCTRL.bit.AB_MAX_COUNT = 4;

	configure_ovp();
}

void init_sec_ocp(void)
{
	FaultMuxRegs.ACOMPCTRL2.bit.ACOMP_E_SEL = 0;        //Configures reference of analog comparator E to ACOMP_E_THRESH
	FaultMuxRegs.ACOMPCTRL2.bit.ACOMP_E_POL = 1;        //Polarity of comparator when an event is detected.
	//FaultMuxRegs.DPWM0FLTABDET.bit.ACOMP_E_EN = 0;
	//FaultMuxRegs.DPWM1FLTABDET.bit.ACOMP_E_EN = 0;

	configure_iout_ocp();
}

/*
void init_pri_ocp(void)
{
	//Configures reference of analog comparator E to ACOMP_E_THRESH
	FaultMuxRegs.ACOMPCTRL2.bit.ACOMP_E_SEL = 0;        
	//Polarity of comparator when an event is detected.
	FaultMuxRegs.ACOMPCTRL2.bit.ACOMP_E_POL = 1;        
	//DAC value for OCP on the primary
	FaultMuxRegs.ACOMPCTRL2.bit.ACOMP_E_THRESH = (int)DAC_OCP_PRI; 
	FaultMuxRegs.DPWM0FLTABDET.bit.ACOMP_E_EN = 1;
	FaultMuxRegs.DPWM1FLTABDET.bit.ACOMP_E_EN = 1;
}
*/

//Analog comparator D, AD04 pin
void init_ipri_cycle_by_cycle(void)   
{
	FaultMuxRegs.ACOMPCTRL1.bit.ACOMP_D_POL = 1;
	FaultMuxRegs.ACOMPCTRL1.bit.ACOMP_D_SEL = 0;


	Dpwm0Regs.DPWMCTRL0.bit.BLANK_A_EN =1;
	Dpwm0Regs.DPWMBLKABEG.bit.BLANK_A_BEGIN = 0;
	//44 x 4ns =176ns
	Dpwm0Regs.DPWMBLKAEND.bit.BLANK_A_END = 44;

	//Set to 1 for pulse matching
	Dpwm0Regs.DPWMCTRL0.bit.CBC_ADV_CNT_EN = 1;
	Dpwm0Regs.DPWMAUTOMID.bit.CBC_ADV_CNT_EN = 1;
	Dpwm0Regs.DPWMAUTOMAX.bit.CBC_ADV_CNT_EN = 1;
	//Set to 1 for B side current limit
	Dpwm0Regs.DPWMCTRL1.bit.CBC_BSIDE_ACTIVE_EN = 0;
//ADD By qys
	Dpwm2Regs.DPWMCTRL0.bit.BLANK_A_EN =1;
	Dpwm2Regs.DPWMBLKABEG.bit.BLANK_A_BEGIN = 0;
	//44 x 4ns =176ns
	Dpwm2Regs.DPWMBLKAEND.bit.BLANK_A_END = 44;

	//Set to 1 for pulse matching
	Dpwm2Regs.DPWMCTRL0.bit.CBC_ADV_CNT_EN = 1;
	Dpwm2Regs.DPWMAUTOMID.bit.CBC_ADV_CNT_EN = 1;
	Dpwm2Regs.DPWMAUTOMAX.bit.CBC_ADV_CNT_EN = 1;
	//Set to 1 for B side current limit
	Dpwm2Regs.DPWMCTRL1.bit.CBC_BSIDE_ACTIVE_EN = 0;
//
	Dpwm1Regs.DPWMCTRL0.bit.BLANK_A_EN = 1;
	Dpwm1Regs.DPWMCTRL0.bit.BLANK_B_EN = 1;

	Dpwm1Regs.DPWMBLKABEG.bit.BLANK_A_BEGIN = 0;
	//50 x 4ns =200ns
	Dpwm1Regs.DPWMBLKAEND.bit.BLANK_A_END = 50;

	Dpwm1Regs.DPWMBLKBBEG.bit.BLANK_B_BEGIN = MINPER;
	//50 x 4ns =200ns
	Dpwm1Regs.DPWMBLKBEND.bit.BLANK_B_END = MINPER + 50;

	//Set to 1 for pulse matching
	Dpwm1Regs.DPWMCTRL0.bit.CBC_ADV_CNT_EN = 1;
	Dpwm1Regs.DPWMAUTOMID.bit.CBC_ADV_CNT_EN = 1;
	Dpwm1Regs.DPWMAUTOMAX.bit.CBC_ADV_CNT_EN = 1;
	//Set to 1 for B side current limit
	Dpwm1Regs.DPWMCTRL1.bit.CBC_BSIDE_ACTIVE_EN = 0;

	Dpwm3Regs.DPWMCTRL0.bit.BLANK_A_EN =1;
	Dpwm3Regs.DPWMBLKABEG.bit.BLANK_A_BEGIN = 0;
	//44 x 4ns =176ns
	Dpwm3Regs.DPWMBLKAEND.bit.BLANK_A_END = 44;

	//Set to 1 for pulse matching
	Dpwm3Regs.DPWMCTRL0.bit.CBC_ADV_CNT_EN = 1;
	Dpwm3Regs.DPWMAUTOMID.bit.CBC_ADV_CNT_EN = 1;
	Dpwm3Regs.DPWMAUTOMAX.bit.CBC_ADV_CNT_EN = 1;
	//Set to 1 for B side current limit
	Dpwm3Regs.DPWMCTRL1.bit.CBC_BSIDE_ACTIVE_EN = 0;
}



