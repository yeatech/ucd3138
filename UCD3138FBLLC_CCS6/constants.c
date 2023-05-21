//###########################################################################
//
// FILE:    constants.c
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
#include "cyclone_device.h"
#include "system_defines.h"
#include "pmbus_commands.h"
#include "pmbus.h"

#pragma DATA_SECTION(filter0_pmbus_regs_constants, ".CONFIG")
volatile const FILTER_PMBUS_REGS filter0_pmbus_regs_constants = DEFAULT_FILTER_0_PMBUS_REGS;

#pragma DATA_SECTION(filter0_pmbus_regs_constants_b, ".CONFIG_B")
volatile const FILTER_PMBUS_REGS filter0_pmbus_regs_constants_b;

#pragma DATA_SECTION(filter0_start_up_pmbus_regs_constants, ".CONFIG")
volatile const FILTER_PMBUS_REGS filter0_start_up_pmbus_regs_constants = DEFAULT_FILTER_0_START_UP_PMBUS_REGS;

#pragma DATA_SECTION(filter0_start_up_pmbus_regs_constants_b, ".CONFIG_B")
volatile const FILTER_PMBUS_REGS filter0_start_up_pmbus_regs_constants_b;

#pragma DATA_SECTION(filter0_cp_pmbus_regs_constants, ".CONFIG")
volatile const FILTER_PMBUS_REGS filter0_cp_pmbus_regs_constants = DEFAULT_FILTER_0_CP_PMBUS_REGS;

#pragma DATA_SECTION(filter0_cp_pmbus_regs_constants_b, ".CONFIG_B")
volatile const FILTER_PMBUS_REGS filter0_cp_pmbus_regs_constants_b;

#pragma DATA_SECTION(filter1_pmbus_regs_constants, ".CONFIG")
volatile const FILTER_PMBUS_REGS filter1_pmbus_regs_constants = DEFAULT_FILTER_1_PMBUS_REGS;

#pragma DATA_SECTION(filter1_pmbus_regs_constants_b, ".CONFIG_B")
volatile const FILTER_PMBUS_REGS filter1_pmbus_regs_constants_b;

#pragma DATA_SECTION(pmbus_dcdc_cal_constants, ".CONFIG")
volatile const PMBUS_DCDC_CAL pmbus_dcdc_cal_constants[DCDC_PAGE_NUMS] = {DEFAULT_PMBUS_DCDC_1_CAL};

#pragma DATA_SECTION(pmbus_dcdc_cal_constants_b, ".CONFIG_B")
volatile const PMBUS_DCDC_CAL pmbus_dcdc_cal_constants_b[DCDC_PAGE_NUMS]; 

#pragma DATA_SECTION(pmbus_dcdc_cal_nonpaged_constants, ".CONFIG")
volatile const PMBUS_DCDC_CAL_NONPAGED pmbus_dcdc_cal_nonpaged_constants = DEFAULT_PMBUS_DCDC_CAL_NONPAGED;

#pragma DATA_SECTION(pmbus_dcdc_cal_nonpaged_constants_b, ".CONFIG_B")
volatile const PMBUS_DCDC_CAL_NONPAGED pmbus_dcdc_cal_nonpaged_constants_b;

#pragma DATA_SECTION(pmbus_dcdc_config_constants, ".CONFIG")
volatile const PMBUS_DCDC_CONFIG pmbus_dcdc_config_constants[DCDC_PAGE_NUMS] = {DEFAULT_PMBUS_DCDC_1_CONFIG};

#pragma DATA_SECTION(pmbus_dcdc_config_constants_b, ".CONFIG_B")
volatile const PMBUS_DCDC_CONFIG pmbus_dcdc_config_constants_b[DCDC_PAGE_NUMS];// = 

#pragma DATA_SECTION(pmbus_dcdc_config_nonpaged_constants, ".CONFIG")
volatile const PMBUS_DCDC_CONFIG_NONPAGED pmbus_dcdc_config_nonpaged_constants = DEFAULT_PMBUS_DCDC_CONFIG_NONPAGED;

#pragma DATA_SECTION(pmbus_dcdc_config_nonpaged_constants_b, ".CONFIG_B")
volatile const PMBUS_DCDC_CONFIG_NONPAGED pmbus_dcdc_config_nonpaged_constants_b;// = DEFAULT_PMBUS_DCDC_CONFIG_NONPAGED;

//  ------------- DATA FLASH CHECKSUM  -------------
#pragma DATA_SECTION(pmbus_checksum, ".CONFIG");
volatile const Uint32 pmbus_checksum = 0x87654321;

#pragma DATA_SECTION(pmbus_checksum_b, ".CONFIG_B");
volatile const Uint32 pmbus_checksum_b;
