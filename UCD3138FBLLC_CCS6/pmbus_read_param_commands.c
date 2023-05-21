//###########################################################################
//
// FILE:    pmbus_read_param_commands.c
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
#define max(a,b)					(((a)>(b)) ? (a):(b))

//==========================================================================================
// pmbus_read_vout()
//==========================================================================================
Uint8 pmbus_read_vout(void) 
{
	Uint16 vout_adc12_value;
	struct qnote vout_adc_scaler = {VOUT_ADC_TO_LITERAL_SCALER, VOUT_ADC_TO_LITERAL_SHIFT - 16};

	vout_adc12_value = qnote_int16_multiply_fit(vout_adc_scaler, adc_values_avg.vo_sense, MAX_VALUE_FIT_16_BITS);
	pmbus_read_two_byte_handler(vout_adc12_value);

	return 0;
}
/*
The mantissa is calculated by the following relationship: 
400V/1.93V
Round(2.5 / 1.93 * 400) = 518.
If the exponent is 0 the available range of VIN that can be sensed is 
0V to 518V.
*/
Uint8 pmbus_read_vin(void)
{
	if (page == 0)
	{
		Uint16 value;
		struct qnote constant_0_126498 = {16580, -17};
		/*
		int16 vin;
		Uint8  exp;
		exp = 0;

		vin = ((adc_values_avg.vin_mon * 518) >> (12 - exp));
		value = (0x7FF & vin) + ((0x1F & (32 - exp)) << 11);
		*/
		value = qnote_to_linear11(qnote_scale(constant_0_126498, adc_values_avg.vin_mon));
		pmbus_read_two_byte_handler(value);
	}
	else  
	{
		pmbus_read_two_byte_handler(pfc_out_struct.pfc_vac);
	}
	
	return PMBUS_SUCCESS;
}       

/*
The mantissa is calculated by the following relationship: 
35A/1.6V
Round(2.5 / 1.6 * 35 << 4) = 875. (Experiementally tuned to 900)
If the exponent is 0 the available range of IOUT that can be sensed is 
0A to 55A.
0.01733 = 2.5 / 1.6 * 45 / 2^12
*/
Uint8 pmbus_read_iout(void)
{
	Uint16 value;
	//This struct represents the floating point number: 0.01694
	struct qnote constant_0_01694 = {17763 , -20};//	value = qnote_to_linear11(qnote_scale(constant_0_01733, adc_values_avg.io_sense));

	if(adc_values_avg.io_sense > 0)
	{
		value = adc_values_avg.io_sense  + 51; //offset from demo board
	}
	else
	{
		value = 0; 
		//make sure it goes to 0 at currents too low to read - below .85 amps on demo board
	}
	value = qnote_to_linear11(qnote_scale(constant_0_01694, value));
	pmbus_read_two_byte_handler(value);

	return 0;
}

/*
Base temperature equation is 159.6 - VSENSE * 85.5. The ADC12 reading is first converted
into voltage by mulitplying the ADC12 value by 2.5 >> 12 or 5 >> 13. Making 85.5 into an
integer changes this factor to (2 * 85.5 * 5) >> 14 = 855 >> 14. 
The exponent "exp" is chosen such that the maximum temperature that can be read is 159.6 C
and the minimum is ~ -54 C. The temperature resolution is: 0.0522 C.
*/
Uint8 pmbus_read_temp_mosfet(void)
{
	int16 value;

	struct qnote temp_qnote;
	struct qnote temp_qnote_scale;
	struct qnote constant_159_6 = {20429, -7};
	struct qnote constant_0_0521851 = {27360, -19};

	temp_qnote_scale = qnote_scale(constant_0_0521851, adc_values_avg.temp);
	temp_qnote = qnote_subtract(constant_159_6, temp_qnote_scale);
	value = qnote_to_linear11(temp_qnote);

	temp_qnote1=temp_qnote;
	temp_qnote_scale1=temp_qnote_scale;
	temp_qnote_value1=value;

	pmbus_read_two_byte_handler(value);

	return 0;
} 

/*
This equation has not been tuned.
(ADC12_Reading*2.5/4096 - 1.9)/0.004 + 25
ADC12_Reading*0.15259 - 450;
*/

Uint8 pmbus_read_temp_device(void)
{

	struct qnote constant_0_15259 = {20000 , -17};
	struct qnote constant_450 = {450 , 0};
	struct qnote temp_result;
	Uint16 value;

	temp_result = qnote_scale(constant_0_15259, adc_values_avg.device_temp);
	temp_result = qnote_subtract(temp_result, constant_450);
	value = qnote_to_linear11(temp_result);

	pmbus_read_two_byte_handler(value);

	return 0;
}

Uint8 pmbus_read_frequency(void)
{
	Uint16 value;
	Uint16 period;
	Uint16 freq;
	Uint8  exp;
	exp = 2;
	if((supply_state == STATE_IDLE) || (supply_state == STATE_FAULT))
	{
		value =0;
	}
	else if (!LoopMuxRegs.DPWMMUX.bit.DPWM0_FILTER_SEL)
	{
		period = (((long long)Filter0Regs.FILTERYNREAD.bit.YN * (long long)LoopMuxRegs.FILTERKCOMPA.bit.KCOMP0) >> 23);
		freq = min(((Uint16)(1000000 / pmbus_dcdc_config[0].min_period)) << (exp - 2) , (Uint16)((250000 << exp) / period));
		value = (0x7FF & freq) + ((0x1F & (32 - exp)) << 11);
	}
	else
	{
		period = (((long long)Filter1Regs.FILTERYNREAD.bit.YN * (long long)LoopMuxRegs.FILTERKCOMPA.bit.KCOMP0) >> 23);
		freq = min(((Uint16)(1000000 / pmbus_dcdc_config[0].min_period)) << (exp - 2) , (Uint16)((250000 << exp) / period));
		value = (0x7FF & freq) + ((0x1F & (32 - exp)) << 11);
	}

	pmbus_read_two_byte_handler(value);

	return 0;
}
