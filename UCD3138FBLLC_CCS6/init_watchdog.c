//###########################################################################
//
// FILE:    init_watchdog.c
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

void init_watchdog(void)
{
	TimerRegs.WDCTRL.bit.WD_PERIOD = 0;  //set period fast for test purposes
	TimerRegs.WDCTRL.bit.PROTECT = 0;
}
