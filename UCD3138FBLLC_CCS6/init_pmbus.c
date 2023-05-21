//###########################################################################
//
// FILE:    init_pmbus.c
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
Uint32 temp_adc_address;     

void init_pmbus(void)
{
	int32 pmbus_address;

	//Start a conversion
	//AdcRegs.ADCCTRL.bit.SW_START = 1;  
	//Clear trigger, so we don't restart
	//AdcRegs.ADCCTRL.bit.SW_START = 0;  

	//poll_adc();
	while(AdcRegs.ADCSTAT.bit.ADC_INT == 0)
	{
		;
	}

	temp_adc_address = AdcRegs.ADCRESULT[7].bit.RESULT;

	if (temp_adc_address > 600)
	{
		pmbus_address = 0x59; //Module 1
	}
	else
	{
		pmbus_address = 0x5A; //Module 2
	}
	PMBusRegs.PMBCTRL3.bit.IBIAS_B_EN = 0;

	//pmbus initialization code.  Much of this is unnecessary after a reset, but is put
	//in for completeness, and in case code is entered after some other program uses
	//PMBus interface

	PMBusRegs.PMBINTM.all = 0x1FF; //disable all PMBus interrupts
	PMBusRegs.PMBCTRL2.all = PMBCTRL2_HALF0_PEC_ENA 
							 + pmbus_address 
							 + PMBCTRL2_HALF0_SLAVE_ADDRESS_MASK_DISABLE
							 + PMBCTRL2_ALL_RX_BYTE_ACK_CNT; 
	pmbus_state = PMBUS_STATE_IDLE;  //initialize state to no message in progress 
}


