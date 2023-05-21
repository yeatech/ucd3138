//pmbus_cml_commands.c
#include "system_defines.h"
#include "cyclone_device.h"
#include "pmbus_commands.h"
#include "pmbus.h"
#include "variables.h"
#include "function_definitions.h"
#include "software_interrupts.h"

extern const Uint8 setup_id[];
extern const Uint8 mfr_id[];
extern const Uint8 mfr_model[];
extern const Uint8 mfr_revision[];
extern const Uint8 mfr_date[];
extern const Uint8 mfr_location[];
extern const Uint8 mfr_serial[];
extern const Uint8 mfr_ic_device[];
extern const Uint8 mfr_ic_device_rev[];

extern int eadc_dac_offset_dflash;
extern int second_eadc_dac_offset_dflash;
extern volatile const FILTER_PMBUS_REGS filter0_pmbus_regs_constants;
extern volatile const FILTER_PMBUS_REGS filter0_pmbus_regs_constants_b;
extern volatile const FILTER_PMBUS_REGS filter0_start_up_pmbus_regs_constants;
extern volatile const FILTER_PMBUS_REGS filter0_start_up_pmbus_regs_constants_b;
extern volatile const FILTER_PMBUS_REGS filter0_cp_pmbus_regs_constants;
extern volatile const FILTER_PMBUS_REGS filter0_cp_pmbus_regs_constants_b;
extern volatile const FILTER_PMBUS_REGS filter1_pmbus_regs_constants;
extern volatile const FILTER_PMBUS_REGS filter1_pmbus_regs_constants_b;
extern volatile const Uint32 pmbus_checksum;
extern volatile const Uint32 pmbus_checksum_b;
extern volatile const PMBUS_DCDC_CONFIG pmbus_dcdc_config_constants[DCDC_PAGE_NUMS];
extern volatile const PMBUS_DCDC_CONFIG pmbus_dcdc_config_constants_b[DCDC_PAGE_NUMS];
extern volatile const PMBUS_DCDC_CONFIG_NONPAGED pmbus_dcdc_config_nonpaged_constants;
extern volatile const PMBUS_DCDC_CONFIG_NONPAGED pmbus_dcdc_config_nonpaged_constants_b;
extern volatile const PMBUS_DCDC_CAL pmbus_dcdc_cal_constants[DCDC_PAGE_NUMS];
extern volatile const PMBUS_DCDC_CAL pmbus_dcdc_cal_constants_b[DCDC_PAGE_NUMS];
extern volatile const PMBUS_DCDC_CAL_NONPAGED pmbus_dcdc_cal_nonpaged_constants;
extern volatile const PMBUS_DCDC_CAL_NONPAGED pmbus_dcdc_cal_nonpaged_constants_b;

Uint8 pmbus_write_restore_default_all(void)
{
	/*Setting the enable_turn_on flag to 0 will force the PSON #define to
	a 0 state. This subsequently will force the power supply into the
	idle state until the restore default_all command completes.
	
	After enable_turn_on goes high the power supply will start up at the next
	valid PSON.*/
	enable_turn_on = 0;

	while (supply_state != STATE_IDLE)
	{
		;
	}

	restore_default_all();

	configure_vout_cmd();
	configure_filter_parameters();
	configure_dpwm_timing();
	//configure_burst_mode();
	configure_cc_dac_value();
	configure_ovp();
	configure_iout_ocp();
	configure_constant_power();
	configure_vin_on_off_thresholds();
	configure_fault_levels();
	configure_warning_levels();
	configure_pgood_levels();
	configure_cs_limits();
	configure_vout_timing();

	enable_turn_on = 1;

	return PMBUS_SUCCESS; 
}

//==========================================================================================
// pmbus_write_store_default_all()
//	Store all PMBus-configurable variables from RAM to Data Flash.   
//==========================================================================================
Uint8 pmbus_write_store_default_all(void)
{
	Uint8	status;
	Uint32	checksum;

	volatile const  FILTER_PMBUS_REGS*	   		   dest_address_filter0_pmbus_regs;
	volatile const  FILTER_PMBUS_REGS*	   		   dest_address_filter0_start_up_pmbus_regs;
	volatile const  FILTER_PMBUS_REGS*	   		   dest_address_filter0_cp_pmbus_regs;
	volatile const  FILTER_PMBUS_REGS*	   		   dest_address_filter1_pmbus_regs;
	volatile const  PMBUS_DCDC_CONFIG*             dest_address_dcdc_config;
	volatile const  PMBUS_DCDC_CONFIG_NONPAGED*    dest_address_dcdc_config_nonpaged;
	volatile const  PMBUS_DCDC_CAL*                dest_address_dcdc_cal;
	volatile const  PMBUS_DCDC_CAL_NONPAGED*       dest_address_dcdc_cal_nonpaged;

	volatile const Uint32*						   dest_checksum;

	volatile const FILTER_PMBUS_REGS*	   		   opposite_bank_start;

	// Number of bytes to erase is the total size of all the blocks, including the checksum.
	// Assumes that the _a and _b structures are the same size.
	volatile const	Uint32	bytes_to_erase =    
		  sizeof(filter0_pmbus_regs_constants)
		+ sizeof(filter0_start_up_pmbus_regs_constants)
		+ sizeof(filter0_cp_pmbus_regs_constants)
		+ sizeof(filter1_pmbus_regs_constants)
		+ sizeof(pmbus_dcdc_config_constants)
		+ sizeof(pmbus_dcdc_config_nonpaged_constants)
		+ sizeof(pmbus_dcdc_cal_constants)
		+ sizeof(pmbus_dcdc_cal_nonpaged_constants)
		+ sizeof(pmbus_checksum);

	// NOTE: This is a SEND BYTE command that requires no additional data validation.
	// ----- EXECUTE COMMAND -----
	// Check for DFlash erase operations in progress
	if (erase_segment_counter > 1)
	{
		// There are multiple segments that still need to be erased.  There is no way to 
		// get them erased in time to avoid a PMBus timeout so we will just report an error
		// and return.
		return PMBUS_OTHER_FAULT;	// CML - Other memory or logic fault has occurred

	}

	copy_coefficients_to_ram(&filter0_pmbus_regs, &Filter0Regs);
	copy_coefficients_to_ram(&filter1_pmbus_regs, &Filter1Regs);

	// There are one or zero segments left to erase.
	// Wait for any erase that might be in progress to complete.
	// IMPORTANT!  This must be done before attempting to access any DFlash location, 
	// even if it is not in the segment being erased.
	while(DecRegs.DFLASHCTRL.bit.BUSY != 0)
	{
		; //do nothing while busy erasing DFlash
	}

	// Test that the selected bank is completely erased by checking the first and last word.
	// If both are all F's, the bank is completeley erased so use it.
	// (Assumes that all words in between are erased; usually a reasonable assumption.)
	if ((pmbus_checksum == 0xFFFFFFFF)&&(filter0_pmbus_regs_constants.FILTERKDCOEF0.bit.KD_COEF_0 == (signed short)0xFFFF))	// Test Dflash A 
	{	
		// DFlash A is blank. 
		//Store new values in DFlash A and erase DFlash B when done.
		dest_address_filter0_pmbus_regs   			= &filter0_pmbus_regs_constants;
		dest_address_filter0_start_up_pmbus_regs	= &filter0_start_up_pmbus_regs_constants;
		dest_address_filter0_cp_pmbus_regs 			= &filter0_cp_pmbus_regs_constants;
		dest_address_filter1_pmbus_regs   			= &filter1_pmbus_regs_constants;
		dest_address_dcdc_config          			= &pmbus_dcdc_config_constants[0];
		dest_address_dcdc_config_nonpaged 			= &pmbus_dcdc_config_nonpaged_constants;
		dest_address_dcdc_cal             			= &pmbus_dcdc_cal_constants[0];
		dest_address_dcdc_cal_nonpaged    			= &pmbus_dcdc_cal_nonpaged_constants;
		dest_checksum			          			= &pmbus_checksum;

			opposite_bank_start		= &filter0_pmbus_regs_constants_b;
	}
	else if ((pmbus_checksum_b == 0xFFFFFFFF)&&(filter0_pmbus_regs_constants_b.FILTERKDCOEF0.bit.KD_COEF_0 == (signed short)0xFFFF))	// Test DFlash B 
	{
		// DFlash B is blank.
		// Store new values in DFlash B and erase DFlash A when done.
		dest_address_filter0_pmbus_regs   			= &filter0_pmbus_regs_constants_b;
		dest_address_filter0_start_up_pmbus_regs	= &filter0_start_up_pmbus_regs_constants_b;
		dest_address_filter0_cp_pmbus_regs 			= &filter0_cp_pmbus_regs_constants_b;
		dest_address_filter1_pmbus_regs   			= &filter1_pmbus_regs_constants_b;
		dest_address_dcdc_config          			= &pmbus_dcdc_config_constants_b[0];
		dest_address_dcdc_config_nonpaged 			= &pmbus_dcdc_config_nonpaged_constants_b;
		dest_address_dcdc_cal             			= &pmbus_dcdc_cal_constants_b[0];
		dest_address_dcdc_cal_nonpaged    			= &pmbus_dcdc_cal_nonpaged_constants_b;
		dest_checksum			          			= &pmbus_checksum_b;

			opposite_bank_start		= &filter0_pmbus_regs_constants;
	}
	else
	{

		// Neither bank is blank.  Erase one of them so it will be ready for next time
		// and report a Flash failure.
		if (pmbus_checksum_b == 0xFFFFFFFF)
		{
			// We got here because DFlash B was only partially erased, probably because
			// of a power failure during the erase process.  Re-erase bank B and leave 
			// bank A intact since it might have valid values.
			dest_address_filter0_pmbus_regs	= &filter0_pmbus_regs_constants_b;

		}
		else
		{
			// We got here because DFlash A was partly erased and B was intact,
			// or because both banks were never erased.
			// Either way, we will now erase bank A.
			dest_address_filter0_pmbus_regs	= &filter0_pmbus_regs_constants;
		}
		goto flash_write_failed;	// Fail: Destination bank not erased.
	}


	// --------------- CLA GAINS  ---------------
	// Call Update Flash to write the necessary segments of Data Flash
	status = update_data_flash((void*)dest_address_filter0_pmbus_regs, &filter0_pmbus_regs, sizeof(filter0_pmbus_regs));
	if (status != FLASH_SUCCESS)
	{
		goto flash_write_failed;	// Clean up after flash write failure
	}
	status = update_data_flash((void*)dest_address_filter0_start_up_pmbus_regs, &filter0_start_up_pmbus_regs, sizeof(filter0_start_up_pmbus_regs));
	if (status != FLASH_SUCCESS)
	{
		goto flash_write_failed;	// Clean up after flash write failure
	}
	status = update_data_flash((void*)dest_address_filter0_cp_pmbus_regs, &filter0_cp_pmbus_regs, sizeof(filter0_cp_pmbus_regs));
	if (status != FLASH_SUCCESS)
	{
		goto flash_write_failed;	// Clean up after flash write failure
	}
	status = update_data_flash((void*)dest_address_filter1_pmbus_regs, &filter1_pmbus_regs, sizeof(filter1_pmbus_regs));
	if (status != FLASH_SUCCESS)
	{
		goto flash_write_failed;	// Clean up after flash write failure
	}
	// --------------- PMBUS CONFIG PER PAGE---------------
	// Call Update Flash to write the necessary segments of Data Flash
	status = update_data_flash((void*)dest_address_dcdc_config, &pmbus_dcdc_config[0], sizeof(pmbus_dcdc_config)); 
	if (status != FLASH_SUCCESS)
	{
		goto flash_write_failed;	// Clean up after flash write failure
	}

	// --------------- PMBUS CONFIG NONPAGED -----------------------
	// Call Update Flash to write the necessary segments of Data Flash
	status = update_data_flash((void*)dest_address_dcdc_config_nonpaged, &pmbus_dcdc_config_nonpaged, sizeof(pmbus_dcdc_config_nonpaged)); 
	if (status != FLASH_SUCCESS)
	{
		goto flash_write_failed;	// Clean up after flash write failure
	}

	// --------------- PMBUS CAL PER PAGE ---------------
	// Call Update Flash to write the necessary segments of Data Flash
	status = update_data_flash((void*)dest_address_dcdc_cal, &pmbus_dcdc_cal[0], sizeof(pmbus_dcdc_cal)); 
	if (status != FLASH_SUCCESS)
	{
		goto flash_write_failed;	// Clean up after flash write failure
	}

	// --------------- PMBUS CAL NONPAGED ---------------
	// Call Update Flash to write the necessary segments of Data Flash
	status = update_data_flash((void*)dest_address_dcdc_cal_nonpaged, &pmbus_dcdc_cal_nonpaged, sizeof(pmbus_dcdc_cal_nonpaged)); 
	if (status != FLASH_SUCCESS)
	{
		goto flash_write_failed;	// Clean up after flash write failure
	}

	// --------------- CHECKSUM ---------------
	// Calculate checksum for selected Data Flash segment and write to Data Flash

	checksum = calculate_dflash_checksum((Uint8*)dest_address_filter0_pmbus_regs, (Uint8*)dest_checksum);
	status = update_data_flash((void*)dest_checksum, &checksum, sizeof(checksum));
	if (status != FLASH_SUCCESS)
	{
		goto flash_write_failed;	// Clean up after flash write failure
	}

	{

		// ----- Bank written successfully.  Erase opposite bank. -----
		start_erase_task((void*)opposite_bank_start, bytes_to_erase);	
		// If everything works, return success.
		return PMBUS_SUCCESS;
	}

	flash_write_failed:		// <--- Destination for several goto's above.
	{
		// ----- This bank write failed.  Erase present bank and report the failure.
		start_erase_task((void*)dest_address_filter0_pmbus_regs, bytes_to_erase);	
		return (PMBUS_MEMORY_FAULT);		// Flash write failed
	}
}
