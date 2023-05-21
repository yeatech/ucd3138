//###########################################################################
//
// FILE:    pmbus_manuf_info_commands.c
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

const Uint8 setup_id[]          = SETUP_ID;
const Uint8 mfr_id[]			= MFR_ID;
const Uint8 mfr_model[]			= MFR_MODEL;
const Uint8 mfr_revision[]		= MFR_REVISION;
const Uint8 mfr_date[]			= MFR_DATE;
const Uint8 mfr_location[]		= MFR_LOCATION;
const Uint8 mfr_serial[]		= MFR_SERIAL;
const Uint8 mfr_ic_device[]		= MFR_IC_DEVICE;
const Uint8 mfr_ic_device_rev[]	= MFR_IC_DEVICE_REV;

void send_string(const Uint8 string_to_send[], Uint8 num_bytes)
{
	Uint8 byte;
	Uint8 string_len;

	string_len = (Uint8) min(num_bytes, PMBUS_BUFFER_SIZE - 1);

	pmbus_number_of_bytes = string_len + 1;

	pmbus_buffer[0] = string_len;

	for(byte = 0; byte < string_len; byte++)
	{
		pmbus_buffer[byte+1] = (Uint8) string_to_send[byte];
	}
}

int pmbus_read_setup_id(void) 
{
	send_string(setup_id, (Uint8) SETUP_ID_LENGTH);
	return PMBUS_SUCCESS;
}

int pmbus_mfr_model_handler(void) 
{ 
	send_string(mfr_model, (Uint8) MFR_MODEL_LENGTH);
	return PMBUS_SUCCESS;
}

int pmbus_mfr_revision_handler(void) 
{ 
	send_string(mfr_revision, (Uint8) MFR_REVISION_LENGTH);
	return PMBUS_SUCCESS;
}

int pmbus_mfr_id_handler(void) 
{ 
	send_string(mfr_id, (Uint8) MFR_ID_LENGTH);
	return PMBUS_SUCCESS;
}

int pmbus_mfr_date_handler(void) 
{ 
	send_string(mfr_date, (Uint8) MFR_DATE_LENGTH);
	return PMBUS_SUCCESS;
}

int pmbus_mfr_location_handler(void) 
{ 
	send_string(mfr_location, (Uint8) MFR_LOCATION_LENGTH);
	return PMBUS_SUCCESS;
}

int pmbus_mfr_serial_handler(void) 
{ 
	send_string(mfr_serial, (Uint8) MFR_SERIAL_LENGTH);
	return PMBUS_SUCCESS;
}

int pmbus_mfr_ic_device_handler(void) 
{ 
	send_string(mfr_ic_device, (Uint8) MFR_IC_DEVICE_LENGTH);
	return PMBUS_SUCCESS;
}

int pmbus_mfr_ic_device_rev_handler(void) 
{ 
	send_string(mfr_ic_device_rev, (Uint8) MFR_IC_DEVICE_REV_LENGTH);
	return PMBUS_SUCCESS;
}
