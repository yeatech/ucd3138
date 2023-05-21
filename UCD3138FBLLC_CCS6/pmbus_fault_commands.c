//###########################################################################
//
// FILE:    pmbus_fault_commands.c
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

Uint32 dpwm0_status = 0;
Uint32 dpwm1_status = 0;


Uint8 pmbus_write_clear_faults(void)
{
	switch (supply_state)
	{
	case STATE_REGULATED:
	case STATE_CPCC:
		{
			pmbus_status_word = 0;
			fiq_data.fault_status = 0;
			PMBusRegs.PMBCTRL3.bit.ALERT_EN = 0;
			MiscAnalogRegs.GLBIOVAL.all &= ~((Uint32)MASK_FAILURE);
			return PMBUS_SUCCESS;
		}
	case STATE_IDLE:
		{
			pmbus_status_word = PMBUS_STATUS_WORD_HIGH_BYTE | 
								PMBUS_STATUS_WORD_OFF | 
								PMBUS_STATUS_WORD_POWER_GOOD_NOT;
			fiq_data.fault_status = 0;
			PMBusRegs.PMBCTRL3.bit.ALERT_EN = 0;
			MiscAnalogRegs.GLBIOVAL.all &= ~((Uint32)MASK_FAILURE);
			return PMBUS_SUCCESS;
		}
	default: return PMBUS_SUCCESS;
	}
}

//OV Fault
Uint8 pmbus_write_vout_ov_fault_limit(void)
{
	pmbus_dcdc_config[0].vout_ov_fault_limit = pmbus_buffer[1] + (pmbus_buffer[2] << 8);
	configure_ovp();
	configure_fault_levels();
	return PMBUS_SUCCESS;
}
Uint8 pmbus_read_vout_ov_fault_limit(void)
{
	pmbus_read_two_byte_handler(pmbus_dcdc_config[0].vout_ov_fault_limit);
	return PMBUS_SUCCESS;
}

//OV Warn
Uint8 pmbus_write_vout_ov_warn_limit(void)
{
	pmbus_dcdc_config[0].vout_ov_warn_limit = pmbus_buffer[1] + (pmbus_buffer[2] << 8);
	configure_warning_levels();
	return PMBUS_SUCCESS;
}
Uint8 pmbus_read_vout_ov_warn_limit(void)
{
	pmbus_read_two_byte_handler(pmbus_dcdc_config[0].vout_ov_warn_limit);
	return PMBUS_SUCCESS;
}

//UV Fault
Uint8 pmbus_write_vout_uv_fault_limit(void)
{
	pmbus_dcdc_config[0].vout_uv_fault_limit = pmbus_buffer[1] + (pmbus_buffer[2] << 8);
	configure_fault_levels();
	return PMBUS_SUCCESS;
}
Uint8 pmbus_read_vout_uv_fault_limit(void)
{
	pmbus_read_two_byte_handler(pmbus_dcdc_config[0].vout_uv_fault_limit);
	return PMBUS_SUCCESS;
}

//UV Warn
Uint8 pmbus_write_vout_uv_warn_limit(void)
{
	pmbus_dcdc_config[0].vout_uv_warn_limit = pmbus_buffer[1] + (pmbus_buffer[2] << 8);
	configure_warning_levels();
	return PMBUS_SUCCESS;
}
Uint8 pmbus_read_vout_uv_warn_limit(void)
{
	pmbus_read_two_byte_handler(pmbus_dcdc_config[0].vout_uv_warn_limit);
	return PMBUS_SUCCESS;
}

Uint8 pmbus_write_iout_oc_fault_response(void)
{
  pmbus_dcdc_config[0].iout_oc_fault_response = pmbus_buffer[1];
  configure_iout_ocp();
  return PMBUS_SUCCESS;
}

Uint8 pmbus_read_iout_oc_fault_response(void)
{
  pmbus_read_one_byte_handler(pmbus_dcdc_config[0].iout_oc_fault_response);
  return PMBUS_SUCCESS;
}

//OC Fault
Uint8 pmbus_write_iout_oc_fault_limit(void)
{
	pmbus_dcdc_config[0].iout_oc_fault_limit = pmbus_buffer[1] + (pmbus_buffer[2] << 8);
	configure_iout_ocp();
	configure_fault_levels();
	return PMBUS_SUCCESS;
}
Uint8 pmbus_read_iout_oc_fault_limit(void)
{
	pmbus_read_two_byte_handler(pmbus_dcdc_config[0].iout_oc_fault_limit);
	return PMBUS_SUCCESS;
}

//OC Warn
Uint8 pmbus_write_iout_oc_warn_limit(void)
{
	pmbus_dcdc_config[0].iout_oc_warn_limit = pmbus_buffer[1] + (pmbus_buffer[2] << 8);
	configure_warning_levels();
	return PMBUS_SUCCESS;
}
Uint8 pmbus_read_iout_oc_warn_limit(void)
{
	pmbus_read_two_byte_handler(pmbus_dcdc_config[0].iout_oc_warn_limit);
	return PMBUS_SUCCESS;
}

//OT Fault
Uint8 pmbus_write_temp_ot_fault_limit(void)
{
	pmbus_dcdc_config[0].temp_ot_fault_limit = pmbus_buffer[1] + (pmbus_buffer[2] << 8);
	configure_fault_levels();
	return PMBUS_SUCCESS;
}
Uint8 pmbus_read_temp_ot_fault_limit(void)
{
	pmbus_read_two_byte_handler(pmbus_dcdc_config[0].temp_ot_fault_limit);
	return PMBUS_SUCCESS;
}

//OT Warn
Uint8 pmbus_write_temp_ot_warn_limit(void)
{
	pmbus_dcdc_config[0].temp_ot_warn_limit = pmbus_buffer[1] + (pmbus_buffer[2] << 8);
	configure_warning_levels();
	return PMBUS_SUCCESS;
}
Uint8 pmbus_read_temp_ot_warn_limit(void)
{
	pmbus_read_two_byte_handler(pmbus_dcdc_config[0].temp_ot_warn_limit);
	return PMBUS_SUCCESS;
}

//VIN OV Fault
Uint8 pmbus_write_vin_ov_fault_limit(void)
{
	pmbus_dcdc_config[0].vin_ov_fault_limit = pmbus_buffer[1] + (pmbus_buffer[2] << 8);
	configure_fault_levels();
	return PMBUS_SUCCESS;
}
Uint8 pmbus_read_vin_ov_fault_limit(void)
{
	pmbus_read_two_byte_handler(pmbus_dcdc_config[0].vin_ov_fault_limit);
	return PMBUS_SUCCESS;
}

//VIN OV Warn
Uint8 pmbus_write_vin_ov_warn_limit(void)
{
	pmbus_dcdc_config[0].vin_ov_warn_limit = pmbus_buffer[1] + (pmbus_buffer[2] << 8);
	configure_warning_levels();
	return PMBUS_SUCCESS;
}
Uint8 pmbus_read_vin_ov_warn_limit(void)
{
	pmbus_read_two_byte_handler(pmbus_dcdc_config[0].vin_ov_warn_limit);
	return PMBUS_SUCCESS;
}

//VIN UV Fault
Uint8 pmbus_write_vin_uv_fault_limit(void)
{
	pmbus_dcdc_config[0].vin_uv_fault_limit = pmbus_buffer[1] + (pmbus_buffer[2] << 8);
	configure_fault_levels();
	return PMBUS_SUCCESS;
}
Uint8 pmbus_read_vin_uv_fault_limit(void)
{
	pmbus_read_two_byte_handler(pmbus_dcdc_config[0].vin_uv_fault_limit);
	return PMBUS_SUCCESS;
}

//VIN UV Warn
Uint8 pmbus_write_vin_uv_warn_limit(void)
{
	pmbus_dcdc_config[0].vin_uv_warn_limit = pmbus_buffer[1] + (pmbus_buffer[2] << 8);
	configure_warning_levels();
	return PMBUS_SUCCESS;
}
Uint8 pmbus_read_vin_uv_warn_limit(void)
{
	pmbus_read_two_byte_handler(pmbus_dcdc_config[0].vin_uv_warn_limit);
	return PMBUS_SUCCESS;
}

//IIN OC Fault
Uint8 pmbus_write_iin_oc_fault_limit(void)
{
	pmbus_dcdc_config[0].iin_oc_fault_limit = pmbus_buffer[1] + (pmbus_buffer[2] << 8);
	if (supply_state != STATE_IDLE)
	{
		configure_ipri_cycle_by_cycle(pmbus_dcdc_config[0].iin_oc_fault_limit);
	}
	configure_fault_levels();
	return PMBUS_SUCCESS;
}
Uint8 pmbus_read_iin_oc_fault_limit(void)
{
	pmbus_read_two_byte_handler(pmbus_dcdc_config[0].iin_oc_fault_limit);
	return PMBUS_SUCCESS;
}

//IIN OC Warn
Uint8 pmbus_write_iin_oc_warn_limit(void)
{
	pmbus_dcdc_config[0].iin_oc_warn_limit = pmbus_buffer[1] + (pmbus_buffer[2] << 8);
	configure_warning_levels();
	return PMBUS_SUCCESS;
}
Uint8 pmbus_read_iin_oc_warn_limit(void)
{
	pmbus_read_two_byte_handler(pmbus_dcdc_config[0].iin_oc_warn_limit);
	return PMBUS_SUCCESS;
}

//PGOOD ON
Uint8 pmbus_write_pgood_on_limit(void)
{
	pmbus_dcdc_config[0].pgood_on_limit = pmbus_buffer[1] + (pmbus_buffer[2] << 8);
	configure_pgood_levels();
	return PMBUS_SUCCESS;
}
Uint8 pmbus_read_pgood_on_limit(void)
{
	pmbus_read_two_byte_handler(pmbus_dcdc_config[0].pgood_on_limit);
	return PMBUS_SUCCESS;
}

//PGOOD OFF
Uint8 pmbus_write_pgood_off_limit(void)
{
	pmbus_dcdc_config[0].pgood_off_limit = pmbus_buffer[1] + (pmbus_buffer[2] << 8);
	configure_pgood_levels();
	return PMBUS_SUCCESS;
}
Uint8 pmbus_read_pgood_off_limit(void)
{
	pmbus_read_two_byte_handler(pmbus_dcdc_config[0].pgood_off_limit);
	return PMBUS_SUCCESS;
}

void handle_warnings(void)
{
	if(OVOUT_WARN)
	{
		pmbus_status_word |= PMBUS_STATUS_WORD_VOUT | PMBUS_STATUS_WORD_HIGH_BYTE;
		PMBusRegs.PMBCTRL3.bit.ALERT_EN = 1;  //Set SMBus alert ( Pull the pin down)
	}
	if(UVOUT_WARN)
	{
		pmbus_status_word |= PMBUS_STATUS_WORD_VOUT | PMBUS_STATUS_WORD_HIGH_BYTE;
		PMBusRegs.PMBCTRL3.bit.ALERT_EN = 1;  //Set SMBus alert ( Pull the pin down)
	}
	if(OCOUT_WARN)
	{
		pmbus_status_word |= PMBUS_STATUS_WORD_IOUT | PMBUS_STATUS_WORD_IOUT_OC | PMBUS_STATUS_WORD_HIGH_BYTE;
		PMBusRegs.PMBCTRL3.bit.ALERT_EN = 1;  //Set SMBus alert ( Pull the pin down)
	}
	if(OCIN_WARN)
	{
		pmbus_status_word |= PMBUS_STATUS_WORD_INPUT | PMBUS_STATUS_WORD_HIGH_BYTE;
		PMBusRegs.PMBCTRL3.bit.ALERT_EN = 1;  //Set SMBus alert ( Pull the pin down)
	}
	if(OT_WARN)
	{
		pmbus_status_word |= PMBUS_STATUS_WORD_TEMPERATURE;
		PMBusRegs.PMBCTRL3.bit.ALERT_EN = 1;  //Set SMBus alert ( Pull the pin down)
	}
	if(OVVIN_WARN)
	{
		pmbus_status_word |= PMBUS_STATUS_WORD_INPUT | PMBUS_STATUS_WORD_HIGH_BYTE;
		PMBusRegs.PMBCTRL3.bit.ALERT_EN = 1;  //Set SMBus alert ( Pull the pin down)
	}
	if(UVVIN_WARN)
	{
		pmbus_status_word |= PMBUS_STATUS_WORD_INPUT | PMBUS_STATUS_WORD_HIGH_BYTE;
		PMBusRegs.PMBCTRL3.bit.ALERT_EN = 1;  //Set SMBus alert ( Pull the pin down)
	}
}

void handle_pgood(void)
{
	if (PGOOD_ON)
	{
		MiscAnalogRegs.GLBIOVAL.all |= MASK_PGOOD;
		pmbus_status_word &= ~(PMBUS_STATUS_WORD_HIGH_BYTE | PMBUS_STATUS_WORD_POWER_GOOD_NOT);
	}
	else if (PGOOD_OFF)
	{
		MiscAnalogRegs.GLBIOVAL.all &= ~MASK_PGOOD;
		pmbus_status_word |= PMBUS_STATUS_WORD_HIGH_BYTE | PMBUS_STATUS_WORD_POWER_GOOD_NOT;
	}
}

void handle_faults(void) 
{
	Uint8 shut_down_fault_detected = 0;

	//Poll DPWM for fault status and then poll comparator interrupt status registers
	//fault_status |= (FaultMuxRegs.FAULTMUXINTSTAT.all);
	dpwm0_status |= (Dpwm0Regs.DPWMFLTSTAT.all);
	dpwm1_status |= (Dpwm1Regs.DPWMFLTSTAT.all);

	if(OVER_VOLTAGE_FAULT)
	{
		pmbus_status_word |= 	
			PMBUS_STATUS_WORD_VOUT_OV |
			PMBUS_STATUS_WORD_OFF | 
			PMBUS_STATUS_WORD_VOUT |
			PMBUS_STATUS_WORD_HIGH_BYTE;				
		shut_down_fault_detected = 1;
	}
	if(OVER_CURRENT_SEC_FAULT)
	{
		/*pmbus_status_word |=	
			PMBUS_STATUS_WORD_IOUT_OC |
			PMBUS_STATUS_WORD_OFF | 
			PMBUS_STATUS_WORD_IOUT |
			PMBUS_STATUS_WORD_HIGH_BYTE;				
		shut_down_fault_detected = 1;*/
	}
	if(OT_FAULT)
	{
		pmbus_status_word |= PMBUS_STATUS_WORD_TEMPERATURE;					
		shut_down_fault_detected = 1;
	}
	if(OVVIN_FAULT)
	{
		pmbus_status_word |= PMBUS_STATUS_WORD_INPUT | PMBUS_STATUS_WORD_HIGH_BYTE;				
		shut_down_fault_detected = 1;
	}
	if(UVVIN_FAULT)
	{
		pmbus_status_word |= PMBUS_STATUS_WORD_INPUT | PMBUS_STATUS_WORD_HIGH_BYTE;				
		//shut_down_fault_detected = 1;
	}
	if(UVOUT_FAULT)
	{
		pmbus_status_word |= PMBUS_STATUS_WORD_VOUT | PMBUS_STATUS_WORD_HIGH_BYTE;	
		PMBusRegs.PMBCTRL3.bit.ALERT_EN = 1;  //Set SMBus alert (Pull the pin down)	
	}

	if (shut_down_fault_detected)// If any faults detected that should shut down the PSU
	{
		gpio_dpwm_off();
		MiscAnalogRegs.GLBIOVAL.all |= MASK_FAILURE;
		PMBusRegs.PMBCTRL3.bit.ALERT_EN = 1;  //Set SMBus alert (Pull the pin down)
		disable_current_sharing();
		supply_state = STATE_FAULT;
	}
}


