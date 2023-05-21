//###########################################################################
//
// FILE:    init_adc12.c
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
#include "cyclone_device.h"     // UCD30xx Headers Include File
#include "software_interrupts.h"
#include "system_defines.h"

void init_adc12(void)
{
	MiscAnalogRegs.TEMPSENCTRL.bit.TEMP_SENSE_DIS = 0;//enable internal temp sensor
	AdcRegs.ADCCTRL.bit.SAMPLING_SEL = 0;//select ADC sampling and hold timeing 267ks/s 

	AdcRegs.ADCCTRL.bit.MAX_CONV = 8; //Set up to measure 9 different items

    AdcRegs.ADCCTRL.bit.SINGLE_SWEEP = 1; //Use single sweep mode

	AdcRegs.ADCSEQSEL0.bit.SEQ0  = 3; 	//VO_SENSE, the first sample signal to be converted   
	AdcRegs.ADCSEQSEL0.bit.SEQ1  = 13;	//IO_SENSE
	AdcRegs.ADCSEQSEL0.bit.SEQ2  = 12;	//TEMP
	AdcRegs.ADCSEQSEL0.bit.SEQ3  = 6;	//VIN_MON
	AdcRegs.ADCSEQSEL1.bit.SEQ4  = 2; 	//ISHARE
	AdcRegs.ADCSEQSEL1.bit.SEQ5  = 5; 	//VO OVP
	AdcRegs.ADCSEQSEL1.bit.SEQ6  = 4; 	//IPS0
	AdcRegs.ADCSEQSEL1.bit.SEQ7  = 1; 	//ADDRESS
	AdcRegs.ADCSEQSEL2.bit.SEQ8 = 14;  //DEVICE TEMP

	AdcRegs.ADCAVGCTRL.bit.AVG0_EN = 1;	//Enable averaging
	AdcRegs.ADCAVGCTRL.bit.AVG1_EN = 1;	//Enable averaging 
	AdcRegs.ADCAVGCTRL.bit.AVG2_EN = 1;	//Enable averaging
	AdcRegs.ADCAVGCTRL.bit.AVG3_EN = 1;	//Enable averaging
	AdcRegs.ADCAVGCTRL.bit.AVG4_EN = 1;	//Enable averaging
	AdcRegs.ADCAVGCTRL.bit.AVG5_EN = 1;	//Enable averaging

	AdcRegs.ADCAVGCTRL.bit.AVG0_CONFIG = 0;//0-4x average, 1-8x average, 2-16x average, 3-32x average
	AdcRegs.ADCAVGCTRL.bit.AVG1_CONFIG = 3;//0-4x average, 1-8x average, 2-16x average, 3-32x average
	AdcRegs.ADCAVGCTRL.bit.AVG2_CONFIG = 3;//0-4x average, 1-8x average, 2-16x average, 3-32x average
	AdcRegs.ADCAVGCTRL.bit.AVG3_CONFIG = 3;//0-4x average, 1-8x average, 2-16x average, 3-32x average
	AdcRegs.ADCAVGCTRL.bit.AVG4_CONFIG = 3;//0-4x average, 1-8x average, 2-16x average, 3-32x average
	AdcRegs.ADCAVGCTRL.bit.AVG5_CONFIG = 3;//0-4x average, 1-8x average, 2-16x average, 3-32x average;

	AdcRegs.ADCCTRL.bit.ADC_EN = 1;		//Enable ADC

	AdcRegs.ADCCTRL.bit.SW_START = 1;	//Send start bit high
	AdcRegs.ADCCTRL.bit.SW_START = 0;	//Send start bit low
}
