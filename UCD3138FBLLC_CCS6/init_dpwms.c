//###########################################################################
//
// FILE:    init_dpwms.c
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

#define PERIOD 20000

void init_dpwm0(void) // DPWM0 is used for primary side drive.
{		
	// --------------- CTRL0
	Dpwm0Regs.DPWMCTRL0.bit.PWM_EN = 1;       
	Dpwm0Regs.DPWMCTRL0.bit.CLA_EN = 1;   
	Dpwm0Regs.DPWMCTRL0.bit.PWM_MODE = NORMAL;
	// Slave  
	Dpwm0Regs.DPWMCTRL0.bit.MSYNC_SLAVE_EN = 1;
	// Adv cnt limit enabled  
	Dpwm0Regs.DPWMCTRL0.bit.CBC_ADV_CNT_EN = 1; 
	// Current limit enabled for AB outputs 
	Dpwm0Regs.DPWMCTRL0.bit.CBC_PWM_AB_EN = 1;  

	Dpwm0Regs.DPWMCTRL0.bit.PWM_A_INTRA_MUX = PWMC; //BELOW_C;
	Dpwm0Regs.DPWMCTRL0.bit.PWM_B_INTRA_MUX = BELOW2_C;
	Dpwm0Regs.DPWMCTRL0.bit.BLANK_A_EN = 1;    
	Dpwm0Regs.DPWMCTRL0.bit.RESON_MODE_FIXED_DUTY_EN = 0;

	// --------------- AUTOMID
    Dpwm0Regs.DPWMAUTOMID.bit.CLA_EN = 1; 
    Dpwm0Regs.DPWMAUTOMID.bit.PWM_MODE = RESONANT;
    // Adv cnt limit enabled  
    Dpwm0Regs.DPWMAUTOMID.bit.CBC_ADV_CNT_EN = 1;
    // Current limit enabled for AB outputs  
    Dpwm0Regs.DPWMAUTOMID.bit.CBC_PWM_AB_EN = 1; 
	Dpwm0Regs.DPWMAUTOMID.bit.PWM_A_INTRA_MUX = PASS_THROUGH; 	
    Dpwm0Regs.DPWMAUTOMID.bit.PWM_B_INTRA_MUX = PASS_THROUGH;   
    Dpwm0Regs.DPWMAUTOMID.bit.RESON_MODE_FIXED_DUTY_EN = 0;

	// --------------- AUTOMAX
    Dpwm0Regs.DPWMAUTOMAX.bit.CLA_EN = 1;    
    Dpwm0Regs.DPWMAUTOMAX.bit.PWM_MODE = RESONANT; 
    // Adv cnt limit enabled  
    Dpwm0Regs.DPWMAUTOMAX.bit.CBC_ADV_CNT_EN = 1; 
    // Current limit enabled for AB outputs 
    Dpwm0Regs.DPWMAUTOMAX.bit.CBC_PWM_AB_EN = 1; 
	Dpwm0Regs.DPWMAUTOMAX.bit.PWM_A_INTRA_MUX = PASS_THROUGH; 	
    Dpwm0Regs.DPWMAUTOMAX.bit.PWM_B_INTRA_MUX = PASS_THROUGH; 
    // Enable fixed duty for SR 
    Dpwm0Regs.DPWMAUTOMAX.bit.RESON_MODE_FIXED_DUTY_EN = 0;  

	// ---------------- CTRL1
    Dpwm0Regs.DPWMCTRL1.bit.HIRES_DIS = 1;      
    Dpwm0Regs.DPWMCTRL1.bit.ALL_PHASE_CLK_ENA = 1;      
    Dpwm0Regs.DPWMCTRL1.bit.CHECK_OVERRIDE = 1; 
    // Update end of period      
    Dpwm0Regs.DPWMCTRL1.bit.EVENT_UP_SEL = 1; 
    // Enable auto mode switching      
    Dpwm0Regs.DPWMCTRL1.bit.AUTO_MODE_SEL = 1;  

	//---------------------------
	// DPWM setting for interrupt
	//---------------------------
	//every 4 cycles
	Dpwm0Regs.DPWMINT.bit.PRD_INT_SCALE =2;
	Dpwm0Regs.DPWMINT.bit.PRD_INT_EN = 1;

//	Dpwm0Regs.DPWMCTRL0.bit.CBC_PWM_C_EN = 1;
//	Dpwm0Regs.DPWMAUTOMID.bit.CBC_PWM_C_EN = 1;
//	Dpwm0Regs.DPWMAUTOMAX.bit.CBC_PWM_C_EN = 1;
    
}

void init_dpwm1(void) //dpwm1 is used for sync FET - programmed for device spec version
{
	// ------------------ CTRL0
    Dpwm1Regs.DPWMCTRL0.bit.PWM_EN = 1;
    Dpwm1Regs.DPWMCTRL0.bit.CLA_EN = 1; 
    Dpwm1Regs.DPWMCTRL0.bit.PWM_MODE = MULTI;
	// Slave
    Dpwm1Regs.DPWMCTRL0.bit.MSYNC_SLAVE_EN = 1;
    // Adv cnt limit enabled  
    Dpwm1Regs.DPWMCTRL0.bit.CBC_ADV_CNT_EN = 1;    
    Dpwm1Regs.DPWMCTRL0.bit.CBC_PWM_AB_EN = 1;

	// Output 2A, for test only
	Dpwm1Regs.DPWMCTRL0.bit.PWM_A_INTRA_MUX = CROSSOVER;
    Dpwm1Regs.DPWMCTRL0.bit.PWM_B_INTRA_MUX = CROSSOVER;
    Dpwm1Regs.DPWMCTRL0.bit.BLANK_A_EN = 1;
    Dpwm1Regs.DPWMCTRL0.bit.BLANK_B_EN = 1;    
    Dpwm1Regs.DPWMCTRL0.bit.RESON_MODE_FIXED_DUTY_EN = 0; 
	
	// ------------------- AUTOMID
    Dpwm1Regs.DPWMAUTOMID.bit.CLA_EN = 1; 
    Dpwm1Regs.DPWMAUTOMID.bit.PWM_MODE = RESONANT;
	// Adv cnt limit enabled
    Dpwm1Regs.DPWMAUTOMID.bit.CBC_ADV_CNT_EN = 1; 
    // Current limit enabled for AB outputs   
    Dpwm1Regs.DPWMAUTOMID.bit.CBC_PWM_AB_EN = 1; 
    // Output 2A, for test only
	Dpwm1Regs.DPWMAUTOMID.bit.PWM_A_INTRA_MUX = CROSSOVER;    
    Dpwm1Regs.DPWMAUTOMID.bit.PWM_B_INTRA_MUX = CROSSOVER; 
    Dpwm1Regs.DPWMAUTOMID.bit.RESON_MODE_FIXED_DUTY_EN = 0;  

	// ------------------- AUTOMAX
    Dpwm1Regs.DPWMAUTOMAX.bit.CLA_EN = 1;        
    Dpwm1Regs.DPWMAUTOMAX.bit.PWM_MODE = RESONANT; 
	// Adv cnt limit enabled 
    Dpwm1Regs.DPWMAUTOMAX.bit.CBC_ADV_CNT_EN = 1; 
    // Current limit enabled for AB outputs  
    Dpwm1Regs.DPWMAUTOMAX.bit.CBC_PWM_AB_EN = 1;   
    // Output 2A, for test only
	Dpwm1Regs.DPWMAUTOMAX.bit.PWM_A_INTRA_MUX = CROSSOVER; 
    Dpwm1Regs.DPWMAUTOMAX.bit.PWM_B_INTRA_MUX = CROSSOVER; 
    // Enable fixed duty for SR 
    Dpwm1Regs.DPWMAUTOMAX.bit.RESON_MODE_FIXED_DUTY_EN = 1; 

	// ------------------- CTRL1
    Dpwm1Regs.DPWMCTRL1.bit.HIRES_DIS = 1;      
    Dpwm1Regs.DPWMCTRL1.bit.ALL_PHASE_CLK_ENA = 1;      
    Dpwm1Regs.DPWMCTRL1.bit.CHECK_OVERRIDE = 1;
    // Update at end of period       
    Dpwm1Regs.DPWMCTRL1.bit.EVENT_UP_SEL = 1; 
    // Enable auto mode switching        
	Dpwm1Regs.DPWMCTRL1.bit.AUTO_MODE_SEL = 1; 

//	Dpwm1Regs.DPWMCTRL0.bit.CBC_PWM_C_EN = 1;
//	Dpwm1Regs.DPWMAUTOMID.bit.CBC_PWM_C_EN = 1;
//	Dpwm1Regs.DPWMAUTOMAX.bit.CBC_PWM_C_EN = 1;
}

//DPWM2 is used in FBLLC, is copied form init_dpwm0(void).
void init_dpwm2(void) 
{
	// --------------- CTRL0
    Dpwm2Regs.DPWMCTRL0.bit.PWM_EN = 1; 
    Dpwm2Regs.DPWMCTRL0.bit.CLA_EN = 1;   
    Dpwm2Regs.DPWMCTRL0.bit.PWM_MODE = NORMAL;
    // Master 
    Dpwm2Regs.DPWMCTRL0.bit.MSYNC_SLAVE_EN = 0;
    // Adv cnt limit enabled    
    Dpwm2Regs.DPWMCTRL0.bit.CBC_ADV_CNT_EN = 1;
    // Current limit enabled for AB outputs    
    Dpwm2Regs.DPWMCTRL0.bit.CBC_PWM_AB_EN  = 1;    
    Dpwm2Regs.DPWMCTRL0.bit.PWM_A_INTRA_MUX = EDGEGEN;
    Dpwm2Regs.DPWMCTRL0.bit.PWM_B_INTRA_MUX = EDGEGEN;
    Dpwm2Regs.DPWMCTRL0.bit.BLANK_A_EN = 1;    

	// ---------------- AUTOMID
    Dpwm2Regs.DPWMAUTOMID.bit.CLA_EN = 1;
    Dpwm2Regs.DPWMAUTOMID.bit.PWM_MODE = RESONANT;
    // Adv cnt limit enabled 
    Dpwm2Regs.DPWMAUTOMID.bit.CBC_ADV_CNT_EN = 1;
    // Current limit enabled for AB outputs    
    Dpwm2Regs.DPWMAUTOMID.bit.CBC_PWM_AB_EN = 1;    
    Dpwm2Regs.DPWMAUTOMID.bit.PWM_A_INTRA_MUX = BELOW_A; 
    Dpwm2Regs.DPWMAUTOMID.bit.PWM_B_INTRA_MUX = BELOW_B; 

	// ---------------- AUTOMAX
	//make mid and max the same
	Dpwm2Regs.DPWMAUTOMAX.bit.CLA_EN = 1;
    Dpwm2Regs.DPWMAUTOMAX.bit.PWM_MODE = RESONANT;
    // Adv cnt limit enabled 
    Dpwm2Regs.DPWMAUTOMAX.bit.CBC_ADV_CNT_EN = 1;

    // Current limit enabled for AB outputs    
    Dpwm2Regs.DPWMAUTOMAX.bit.CBC_PWM_AB_EN = 1;    
    Dpwm2Regs.DPWMAUTOMAX.bit.PWM_A_INTRA_MUX = BELOW_A; 
    Dpwm2Regs.DPWMAUTOMAX.bit.PWM_B_INTRA_MUX = BELOW_B; 

	// ---------------- CTRL1
    Dpwm2Regs.DPWMCTRL1.bit.HIRES_DIS = 1;      
    Dpwm2Regs.DPWMCTRL1.bit.ALL_PHASE_CLK_ENA = 1;      
    Dpwm2Regs.DPWMCTRL1.bit.CHECK_OVERRIDE = 1;
    // Update end of period       
    Dpwm2Regs.DPWMCTRL1.bit.EVENT_UP_SEL = 1; 
    //enable auto mode switching   
  	Dpwm2Regs.DPWMCTRL1.bit.AUTO_MODE_SEL = 1; //
     
    // ---------------- Edge-generation setup
    Dpwm2Regs.DPWMEDGEGEN.bit.A_ON_EDGE = CURRENT_POS_B;
    Dpwm2Regs.DPWMEDGEGEN.bit.A_OFF_EDGE = NEXT_NEG_A;
    Dpwm2Regs.DPWMEDGEGEN.bit.B_ON_EDGE = NEXT_POS_B;
    Dpwm2Regs.DPWMEDGEGEN.bit.B_OFF_EDGE = CURRENT_NEG_A;

    // When using edgegen, may need to enable all other DPWMS 
    // to ensure same path delay, default is passthru on others (1234)
    Dpwm0Regs.DPWMEDGEGEN.bit.EDGE_EN = 1;
    Dpwm1Regs.DPWMEDGEGEN.bit.EDGE_EN = 1;
    Dpwm2Regs.DPWMEDGEGEN.bit.EDGE_EN = 1;
    Dpwm3Regs.DPWMEDGEGEN.bit.EDGE_EN = 1;

//  Dpwm2Regs.DPWMCTRL0.bit.CBC_PWM_C_EN = 1;
//    Dpwm2Regs.DPWMAUTOMID.bit.CBC_PWM_C_EN = 1;
//    Dpwm2Regs.DPWMAUTOMAX.bit.CBC_PWM_C_EN = 1;
}

//DPWM3 is used internally
void init_dpwm3(void)
{
	// --------------- CTRL0
    Dpwm3Regs.DPWMCTRL0.bit.PWM_EN = 1;
    Dpwm3Regs.DPWMCTRL0.bit.CLA_EN = 1;  
    Dpwm3Regs.DPWMCTRL0.bit.PWM_MODE = NORMAL;
	// Slave
    Dpwm3Regs.DPWMCTRL0.bit.MSYNC_SLAVE_EN = 1;
    // Adv cnt limit enabled   
    Dpwm3Regs.DPWMCTRL0.bit.CBC_ADV_CNT_EN = 1;
    // Current limit enabled for AB outputs  
    Dpwm3Regs.DPWMCTRL0.bit.CBC_PWM_AB_EN = 1;    
    Dpwm3Regs.DPWMCTRL0.bit.PWM_A_INTRA_MUX = BELOW2_C; 
    Dpwm3Regs.DPWMCTRL0.bit.PWM_B_INTRA_MUX = BELOW3_C;
    Dpwm3Regs.DPWMCTRL0.bit.BLANK_A_EN = 1;    

	// --------------- AUTOMID
    Dpwm3Regs.DPWMAUTOMID.bit.CLA_EN = 1; 
    Dpwm3Regs.DPWMAUTOMID.bit.PWM_MODE = RESONANT;
	// Adv cnt limit enabled
    Dpwm3Regs.DPWMAUTOMID.bit.CBC_ADV_CNT_EN = 1;
    // Current limit enabled for AB outputs    
    Dpwm3Regs.DPWMAUTOMID.bit.CBC_PWM_AB_EN = 1;


    Dpwm3Regs.DPWMAUTOMID.bit.PWM_A_INTRA_MUX = PASS_THROUGH;
    Dpwm3Regs.DPWMAUTOMID.bit.PWM_B_INTRA_MUX = PASS_THROUGH; 

  	// --------------- AUTOMAX
	// make mid and max the same
	Dpwm3Regs.DPWMAUTOMAX.bit.CLA_EN = 1; 
    Dpwm3Regs.DPWMAUTOMAX.bit.PWM_MODE = RESONANT;
	// Adv cnt limit enabled
    Dpwm3Regs.DPWMAUTOMAX.bit.CBC_ADV_CNT_EN = 1;
    // Current limit enabled for AB outputs    
    Dpwm3Regs.DPWMAUTOMAX.bit.CBC_PWM_AB_EN = 1;    
    Dpwm3Regs.DPWMAUTOMAX.bit.PWM_A_INTRA_MUX = PASS_THROUGH;
    Dpwm3Regs.DPWMAUTOMAX.bit.PWM_B_INTRA_MUX = PASS_THROUGH; 

	// --------------- CTRL1
    Dpwm3Regs.DPWMCTRL1.bit.HIRES_DIS = 1;      
    Dpwm3Regs.DPWMCTRL1.bit.ALL_PHASE_CLK_ENA = 1;      
    Dpwm3Regs.DPWMCTRL1.bit.CHECK_OVERRIDE = 1;
    // Update at end of period       
    Dpwm3Regs.DPWMCTRL1.bit.EVENT_UP_SEL = 1;
    // enable auto mode switching         
	Dpwm3Regs.DPWMCTRL1.bit.AUTO_MODE_SEL = 1; 

//	Dpwm3Regs.DPWMCTRL0.bit.CBC_PWM_C_EN = 1;
//	Dpwm3Regs.DPWMAUTOMID.bit.CBC_PWM_C_EN = 1;
//	Dpwm3Regs.DPWMAUTOMAX.bit.CBC_PWM_C_EN = 1;
}

void init_dpwms(void)
{
	init_dpwm0();
	init_dpwm1();
	init_dpwm2();
	init_dpwm3();
	configure_dpwm_timing();
}

void init_sample_trigger(void)
{
    //------------------------------
    // Sample trigger setting
    //------------------------------

	LoopMuxRegs.SAMPTRIGCTRL.bit.FE0_TRIG_DPWM0_EN = 1;
	LoopMuxRegs.SAMPTRIGCTRL.bit.FE1_TRIG_DPWM0_EN = 1;
	//Enable sample trigger 1
	Dpwm0Regs.DPWMCTRL2.bit.SAMPLE_TRIG_1_EN = 1;
	//Enable sample trigger 2
	Dpwm0Regs.DPWMCTRL2.bit.SAMPLE_TRIG_2_EN = 0;
	//Trigger sample is fixed
	Dpwm0Regs.DPWMCTRL2.bit.SAMPLE_TRIG1_MODE = 0;
	//0 - 1x, 1 - 2x, 2 - 4x, 3 - 8x
	Dpwm0Regs.DPWMCTRL2.bit.SAMPLE_TRIG1_OVERSAMPLE = 0;
	//Sample trigger 1 position
	Dpwm0Regs.DPWMSAMPTRIG1.bit.SAMPLE_TRIGGER = 52;
	//Sample trigger 2 position
	Dpwm0Regs.DPWMSAMPTRIG2.bit.SAMPLE_TRIGGER = 0;
	//Adaptive trigger offset
	Dpwm0Regs.DPWMADAPTIVE.bit.ADAPT_SAMP = 0;
	//Enable sample trigger 1
/*
	Dpwm1Regs.DPWMCTRL2.bit.SAMPLE_TRIG_1_EN = 1;
	//Enable sample trigger 2
	Dpwm1Regs.DPWMCTRL2.bit.SAMPLE_TRIG_2_EN = 0;
	//Trigger sample is fixed
	Dpwm1Regs.DPWMCTRL2.bit.SAMPLE_TRIG1_MODE = 0;
	//0 - 1x, 1 - 2x, 2 - 4x, 3 - 8x
	Dpwm1Regs.DPWMCTRL2.bit.SAMPLE_TRIG1_OVERSAMPLE = 0;
	//Sample trigger 1 position
	Dpwm1Regs.DPWMSAMPTRIG1.bit.SAMPLE_TRIGGER = 52;
	//Sample trigger 2 position
	Dpwm1Regs.DPWMSAMPTRIG2.bit.SAMPLE_TRIGGER = 0;
	//Adaptive trigger offset
	Dpwm1Regs.DPWMADAPTIVE.bit.ADAPT_SAMP = 0;
*/
}

