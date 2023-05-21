//###########################################################################
//
// FILE:    software_interrupts.h
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
#pragma SWI_ALIAS (swi_single_entry, 0)
void swi_single_entry(Uint32 arg1, Uint32 arg2, Uint32 arg3, Uint8 swi_number);
//software_interrupts.h

void erase_data_flash_segment(Uint8 segment);


void erase_dflash_segment_no_delay(Uint8 segment);


void write_data_flash_word(Uint32 address,unsigned long data);


void enable_fast_interrupt(void);


void disable_fast_interrupt(void);


void enable_interrupt(void);


void disable_interrupt(void);


void write_firqpr(unsigned long value);


void write_reqmask(unsigned long value);


void clear_integrity_word(void);


void write_data_flash_block();


void erase_pflash(void);

