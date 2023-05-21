//###########################################################################
//
// FILE:    pmbus_vout_commands.c
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

Uint8 pmbus_write_vout_transition_rate(void)
{
	pmbus_dcdc_config[0].vout_transition_rate = pmbus_buffer[1] + (pmbus_buffer[2] << 8);
	configure_vout_timing();

	return PMBUS_SUCCESS;
}

Uint8 pmbus_read_vout_transition_rate(void)
{
	pmbus_read_two_byte_handler(pmbus_dcdc_config[0].vout_transition_rate);

	return PMBUS_SUCCESS;
}

//==========================================================================================
//pmbus_write_vout_cmd
//pmbus_read_vout_cmd
//==========================================================================================
Uint8 pmbus_write_vout_cmd(void)
{
	//int32 dac_new;
	//int32 dac_old;
	//struct qnote dac_value_scaler = {DAC_VALUE_SCALER, 0};

	pmbus_dcdc_config[0].vout_cmd = pmbus_buffer[1] + (pmbus_buffer[2] << 8);

	//dac_old   = FeCtrl0Regs.EADCDAC.bit.DAC_VALUE;
	//dac_new   = (Uint32)pmbus_dcdc_config[0].vout_cmd * DAC_VALUE_SCALER >> VOUT_MODE_EXP;
	//tran_rate = (Uint32)pmbus_dcdc_config[0].vout_transition_rate * VOUT_TRANSITION_RATE_SCALER >> VOUT_MODE_EXP;	
	configure_vout_cmd();
	//dac_new = qnote_linear16_multiply_fit(dac_value_scaler, pmbus_dcdc_config[0].vout_cmd, -VOUT_MODE_EXP, MAX_VALUE_FIT_14_BITS);
	//FeCtrl0Regs.EADCDAC.bit.DAC_VALUE = dac_old;
	FeCtrl0Regs.RAMPDACEND.bit.RAMP_DAC_VALUE = pmbus_dcdc_config_translated[0].vout_cmd;

	if(supply_state == STATE_REGULATED) 
	{
		configure_vout_ramp_rate();
		supply_state = STATE_VOUT_TRANSITION;
		FeCtrl0Regs.RAMPCTRL.bit.FIRMWARE_START = 1;
	}

	return PMBUS_SUCCESS;
}

Uint8 pmbus_read_vout_cmd(void)
{
    pmbus_read_two_byte_handler(pmbus_dcdc_config[0].vout_cmd);
 
  return PMBUS_SUCCESS;
}
