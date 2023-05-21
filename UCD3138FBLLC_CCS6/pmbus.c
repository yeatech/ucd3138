//###########################################################################
//
// FILE:    pmbus.c
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

int32 pmbus_read_one_byte_handler(Uint8 value) 
{
	pmbus_number_of_bytes = 1;
	pmbus_buffer[0] = value;  
	return 0;
}

int32 pmbus_read_two_byte_handler(Uint16 value) 
{
	pmbus_number_of_bytes = 2;
	pmbus_buffer[1] = value >> 8;
	pmbus_buffer[0] = value & 0xff;
	return 0;
}

//==========================================================================================
// pmbus_write_page() writes the page 
// pmbus_read_page() 
///==========================================================================================
Uint8 pmbus_write_page(void)
{
	// Check range
	if(pmbus_buffer[1] < 2)
	   {
		// ----- EXECUTE COMMAND -----
       	page = pmbus_buffer[1];
		return PMBUS_SUCCESS;
   	   }
	else
	   {
		return PMBUS_INVALID_DATA;	// Error: Invalid bank number
	   }
		

}

Uint8 pmbus_read_page(void)
{
 	pmbus_number_of_bytes = 1;
	pmbus_buffer[0] = (Uint8) page;
	return PMBUS_SUCCESS;
}
//==========================================================================================
//
//
//==========================================================================================
int pmbus_read_pin_handler(unsigned short pin) 
{
                Uint16 temp;

				pin = (pin * 10) >> 6;

                if(pin < 150) //15W
                {
                                temp = 0xD000 + ((pin << 6) / 10);//exponent -6
                }
                else if(pin < 310) //31W
                {
                                temp = 0xD800 + ((pin << 5) / 10);//exponent -5
                }
                else if(pin < 630) //63W
                {
                                temp = 0xE000 + ((pin << 4) / 10);//exponent -4
                }
                else if(pin < 1270) //127W
                {
                                temp = 0xE800 + ((pin << 3) / 10);//exponent -3
                }
                else if(pin < 2550) //255W
                {
                                temp = 0xF000 + ((pin << 2) / 10);//exponent -2
                }
                else 
                {
                                temp = 0xF800 + ((pin << 1) / 10);//exponent -1
                }

                return pmbus_read_two_byte_handler(temp);

}

//==========================================================================================
//
//
//==========================================================================================
#define PFC_IIN_ADC_TO_LITERAL_EXPONENT 0xd8 //gives exponent of -5 when put in high byte of sent word

int pmbus_read_iin_handler(unsigned short iin) 
{
    
                Uint16 temp;

                
                if(iin < 2000)//2A
                {
                                temp = 0xB800 + ((iin << 9) / 1000);//exponent -9
                }
                else
                {
                                temp = 0xC000 + ((iin << 8) / 1000);//exponent -8
                }

                return pmbus_read_two_byte_handler(temp);

}






// look at command byte from a write perspective
int32 pmbus_write_message(void)
{
	switch (pmbus_buffer[0]) 			
	{
		case PMBUS_CMD_MFR_SPECIFIC_02:
			return pmbus_write_light_load_config();
		case PMBUS_CMD_MFR_SPECIFIC_36:
			return pmbus_write_cpcc_config();
		case PMBUS_CMD_MFR_SPECIFIC_01:
			return pmbus_write_mode_switching_config();
		case PMBUS_CMD_MFR_DEADBAND_CONFIG: 
			return pmbus_write_deadband_config();
		case PMBUS_CMD_VOUT_OV_FAULT_LIMIT: 
			return pmbus_write_vout_ov_fault_limit();
		case PMBUS_CMD_VOUT_OV_WARN_LIMIT: 
			return pmbus_write_vout_ov_warn_limit();
		case PMBUS_CMD_VOUT_UV_FAULT_LIMIT: 
			return pmbus_write_vout_uv_fault_limit();
		case PMBUS_CMD_VOUT_UV_WARN_LIMIT: 
			return pmbus_write_vout_uv_warn_limit();
		case PMBUS_CMD_IOUT_OC_FAULT_RESPONSE:
		    return pmbus_write_iout_oc_fault_response();
		case PMBUS_CMD_IOUT_OC_FAULT_LIMIT: 
			return pmbus_write_iout_oc_fault_limit();
		case PMBUS_CMD_IOUT_OC_WARN_LIMIT: 
			return pmbus_write_iout_oc_warn_limit();
		case PMBUS_CMD_OT_FAULT_LIMIT: 
			return pmbus_write_temp_ot_fault_limit();
		case PMBUS_CMD_OT_WARN_LIMIT: 
			return pmbus_write_temp_ot_warn_limit();
		case PMBUS_CMD_VIN_OV_FAULT_LIMIT: 
			return pmbus_write_vin_ov_fault_limit();
		case PMBUS_CMD_VIN_OV_WARN_LIMIT: 
			return pmbus_write_vin_ov_warn_limit();
		case PMBUS_CMD_VIN_UV_FAULT_LIMIT: 
			return pmbus_write_vin_uv_fault_limit();
		case PMBUS_CMD_VIN_UV_WARN_LIMIT: 
			return pmbus_write_vin_uv_warn_limit();
		case PMBUS_CMD_IIN_OC_FAULT_LIMIT: 
			return pmbus_write_iin_oc_fault_limit();
		case PMBUS_CMD_IIN_OC_WARN_LIMIT: 
			return pmbus_write_iin_oc_warn_limit();
		case PMBUS_CMD_POWER_GOOD_ON: 
			return pmbus_write_pgood_on_limit();
		case PMBUS_CMD_POWER_GOOD_OFF: 
			return pmbus_write_pgood_off_limit();
		case PMBUS_CMD_CLEAR_FAULTS: 
			return pmbus_write_clear_faults();
		case PMBUS_CMD_ROM_MODE: 
			return pmbus_write_rom_mode();
		case PMBUS_CMD_TON_RISE: 
			return pmbus_write_ton_rise();
		case PMBUS_CMD_STORE_DEFAULT_ALL: 
			return pmbus_write_store_default_all();
		case PMBUS_CMD_RESTORE_DEFAULT_ALL: 
			return pmbus_write_restore_default_all();
		case PMBUS_CMD_VOUT_COMMAND: 
			return pmbus_write_vout_cmd();
		case PMBUS_CMD_VOUT_TRANSITION_RATE: 
			return pmbus_write_vout_transition_rate();	
		case PMBUS_CMD_USER_RAM_00: 
			return pmbus_write_user_ram_00();
        case PMBUS_CMD_MFR_PARM_INFO: 
        	return pmbus_write_parm_info();
		case PMBUS_CMD_MFR_PARM_VALUE: 
			return pmbus_write_parm_value();	
		case PMBUS_CMD_MFR_LIGHT_LOAD_ENB: 
			return pmbus_write_pfc_phase_2_enable();
		case PMBUS_CMD_MFR_SPECIFIC_42: 
			return pmbus_write_pfc_zvs_enable();
		case PMBUS_CMD_MFR_SPECIFIC_43: 
			return pmbus_write_pfc_os_enable();
		case PMBUS_CMD_MFR_SPECIFIC_44: 
			return pmbus_write_llc_sr_enable();
		case PMBUS_CMD_VIN_ON: 
			return pmbus_write_vin_on_limit();
		case PMBUS_CMD_VIN_OFF: 
			return pmbus_write_vin_off_limit();
		case PMBUS_CMD_MFR_SPECIFIC_39:
			return pmbus_write_ishare_configure();
		case PMBUS_CMD_MFR_SPECIFIC_03:
			return pmbus_write_filter_gains();
		case PMBUS_CMD_MFR_SPECIFIC_05:
			return pmbus_write_filter_select();
		case PMBUS_CMD_PAGE:
			 return pmbus_write_page();
		default:
			break;
	}
	return 0; 
}

// look at command byte from a read perspective
int32 pmbus_read_message(void)
{
	switch (pmbus_buffer[0]) 			
	{
		case PMBUS_CMD_MFR_SPECIFIC_02:
			return pmbus_read_light_load_config();
		case PMBUS_CMD_MFR_SPECIFIC_36:
			return pmbus_read_cpcc_config();
		case PMBUS_CMD_MFR_SPECIFIC_01:
			return pmbus_read_mode_switching_config();
		case PMBUS_CMD_MFR_DEADBAND_CONFIG: 
			return pmbus_read_deadband_config();
		case PMBUS_CMD_VOUT_OV_FAULT_LIMIT: 
			return pmbus_read_vout_ov_fault_limit();
		case PMBUS_CMD_VOUT_OV_WARN_LIMIT: 
			return pmbus_read_vout_ov_warn_limit();
		case PMBUS_CMD_VOUT_UV_FAULT_LIMIT: 
			return pmbus_read_vout_uv_fault_limit();
		case PMBUS_CMD_VOUT_UV_WARN_LIMIT: 
			return pmbus_read_vout_uv_warn_limit();
		case PMBUS_CMD_IOUT_OC_FAULT_RESPONSE:
		    return pmbus_read_iout_oc_fault_response();
		case PMBUS_CMD_IOUT_OC_FAULT_LIMIT: 
			return pmbus_read_iout_oc_fault_limit();
		case PMBUS_CMD_IOUT_OC_WARN_LIMIT: 
			return pmbus_read_iout_oc_warn_limit();
		case PMBUS_CMD_OT_FAULT_LIMIT: 
			return pmbus_read_temp_ot_fault_limit();
		case PMBUS_CMD_OT_WARN_LIMIT: 
			return pmbus_read_temp_ot_warn_limit();
		case PMBUS_CMD_VIN_OV_FAULT_LIMIT: 
			return pmbus_read_vin_ov_fault_limit();
		case PMBUS_CMD_VIN_OV_WARN_LIMIT: 
			return pmbus_read_vin_ov_warn_limit();
		case PMBUS_CMD_VIN_UV_FAULT_LIMIT: 
			return pmbus_read_vin_uv_fault_limit();
		case PMBUS_CMD_VIN_UV_WARN_LIMIT: 
			return pmbus_read_vin_uv_warn_limit();
		case PMBUS_CMD_IIN_OC_FAULT_LIMIT: 
			return pmbus_read_iin_oc_fault_limit();
		case PMBUS_CMD_IIN_OC_WARN_LIMIT: 
			return pmbus_read_iin_oc_warn_limit();
		case PMBUS_CMD_POWER_GOOD_ON: 
			return pmbus_read_pgood_on_limit();
		case PMBUS_CMD_POWER_GOOD_OFF: 
			return pmbus_read_pgood_off_limit();
		case PMBUS_CMD_CAPABILITY: 
			return pmbus_read_one_byte_handler(0xb0); //PEC, 400 KHz, ALERT all supported.
		case PMBUS_CMD_VOUT_MODE: 
			return pmbus_read_one_byte_handler((32 - VOUT_MODE_EXP));
		case PMBUS_CMD_TON_RISE: 
			return pmbus_read_ton_rise();
		case PMBUS_CMD_STATUS_BYTE: 
			return pmbus_read_one_byte_handler((Uint8) (pmbus_status_word & 0xFF));
		case PMBUS_CMD_STATUS_WORD: 
			return pmbus_read_two_byte_handler(pmbus_status_word); 
		case PMBUS_CMD_VOUT_COMMAND: 
			return pmbus_read_vout_cmd();
		case PMBUS_CMD_VOUT_TRANSITION_RATE: 
			return pmbus_read_vout_transition_rate();
		case PMBUS_CMD_READ_VOUT: 
			return pmbus_read_vout();

		case PMBUS_CMD_READ_VIN: 
			return pmbus_read_vin();
		case PMBUS_CMD_READ_IOUT: 
			return pmbus_read_iout();
		case PMBUS_CMD_READ_TEMPERATURE_1: 
			return pmbus_read_temp_device();
		case PMBUS_CMD_READ_TEMPERATURE_2: 
			return pmbus_read_temp_mosfet();
		case PMBUS_CMD_READ_FREQUENCY: 
			return pmbus_read_frequency();

        case PMBUS_CMD_MFR_CMDS_DCDC_PAGED: 
        	return pmbus_read_cmd_dcdc_paged();
        case PMBUS_CMD_MFR_CMDS_DCDC_NONPAGED: 
        	return pmbus_read_cmd_dcdc_nonpaged();
        case PMBUS_CMD_MFR_CMDS_PFC: 
        	return pmbus_read_cmd_pfc();
		case PMBUS_CMD_MFR_SETUP_ID: 
			return pmbus_read_setup_id();
		case PMBUS_CMD_MFR_DEVICE_ID: 
			return pmbus_read_device_id();	
        case PMBUS_CMD_PMBUS_REVISION: 
        	return pmbus_read_one_byte_handler(0x42); //version 1.2 for both		
		case PMBUS_CMD_MFR_ID: 
			return pmbus_mfr_id_handler(); 
		case PMBUS_CMD_MFR_MODEL: 
			return pmbus_mfr_model_handler(); 
		case PMBUS_CMD_MFR_REVISION: 
			return pmbus_mfr_revision_handler(); 
		case PMBUS_CMD_MFR_LOCATION: 
			return pmbus_mfr_location_handler();
		case PMBUS_CMD_MFR_DATE: 
			return pmbus_mfr_date_handler();
		case PMBUS_CMD_MFR_SERIAL: 
			return pmbus_mfr_serial_handler();
		case PMBUS_CMD_MFR_IC_DEVICE: 
			return pmbus_mfr_ic_device_handler();
		case PMBUS_CMD_MFR_IC_DEVICE_REV: 
			return pmbus_mfr_ic_device_rev_handler();
		case PMBUS_CMD_MFR_DEBUG_BUFFER: 
			return pmbus_read_debug_buffer();
		case PMBUS_CMD_MFR_STATUS_BIT_MASK: 
			return pmbus_read_status_bit_mask();
		case PMBUS_CMD_USER_RAM_00: 
			return pmbus_read_user_ram_00();
		case PMBUS_CMD_MFR_PARM_INFO: 
			return pmbus_read_parm_info();
		case PMBUS_CMD_MFR_PARM_VALUE: 
			return pmbus_read_parm_value();
		case PMBUS_CMD_MFR_LIGHT_LOAD_ENB: 
			return pmbus_read_pfc_phase_2_enable();
		case PMBUS_CMD_MFR_SPECIFIC_42: 
			return pmbus_read_pfc_zvs_enable();
		case PMBUS_CMD_MFR_SPECIFIC_43: 
			return pmbus_read_pfc_os_enable();
		case PMBUS_CMD_MFR_SPECIFIC_44: 
			return pmbus_read_llc_sr_enable();
		case PMBUS_CMD_VIN_ON: 
			return pmbus_read_vin_on_limit();
		case PMBUS_CMD_VIN_OFF: 
			return pmbus_read_vin_off_limit();
		case PMBUS_CMD_MFR_SPECIFIC_39:
			return pmbus_read_ishare_configure();
		case PMBUS_CMD_MFR_SPECIFIC_03:
			return pmbus_read_filter_gains();
		case PMBUS_CMD_MFR_SPECIFIC_05:
			return pmbus_read_filter_select();
		case PMBUS_CMD_PAGE:
			 return pmbus_read_page();
		case PMBUS_CMD_READ_PIN:
			return pmbus_read_pin_handler(pfc_out_struct.pfc_pin);//only for PFC
		case PMBUS_CMD_READ_IIN:
			return pmbus_read_iin_handler(pfc_out_struct.pfc_iac);//only for PFC IIN 

		default:
			pmbus_number_of_bytes = 16;
			pmbus_buffer[0] = 0x0f;
			pmbus_buffer[1] = 0x22;
			pmbus_buffer[2] = 0x33;
			pmbus_buffer[3] = 0x44;
			pmbus_buffer[4] = 0x55;
			pmbus_buffer[5] = 0x66;
			pmbus_buffer[6] = 0x77;
			pmbus_buffer[7] = 0x88;
			pmbus_buffer[8] = 0x99;
			pmbus_buffer[9] = 0xaa;
			pmbus_buffer[10] = 0xbb;
			pmbus_buffer[11] = 0xcc;
			pmbus_buffer[12] = 0xdd;
			pmbus_buffer[13] = 0xee;
			pmbus_buffer[14] = 0xff;
			pmbus_buffer[15] = 0x01;
			return 0;
	}
}

