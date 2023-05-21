//###########################################################################
//
// FILE:    pmbus_other_commands.c
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


//==========================================================================================
Uint8 pmbus_write_vin_on_limit(void)
{
	pmbus_dcdc_config[0].vin_on_limit = pmbus_buffer[1] + (pmbus_buffer[2] << 8);
	configure_vin_on_off_thresholds();
 	return PMBUS_SUCCESS;
}

Uint8 pmbus_read_vin_on_limit(void)
{
    pmbus_read_two_byte_handler(pmbus_dcdc_config[0].vin_on_limit);
 	return PMBUS_SUCCESS;
}

//==========================================================================================
Uint8 pmbus_write_vin_off_limit(void)
{
	pmbus_dcdc_config[0].vin_off_limit = pmbus_buffer[1] + (pmbus_buffer[2] << 8);
	configure_vin_on_off_thresholds();
 	return PMBUS_SUCCESS;
}

Uint8 pmbus_read_vin_off_limit(void)
{
    pmbus_read_two_byte_handler(pmbus_dcdc_config[0].vin_off_limit);
 	return PMBUS_SUCCESS;
}
