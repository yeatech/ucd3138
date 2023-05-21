//###########################################################################
//
// FILE:    flash.c
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

extern volatile const Uint32			            pmbus_checksum;
extern volatile const Uint32			            pmbus_checksum_b;
extern volatile const FILTER_PMBUS_REGS filter0_pmbus_regs_constants;
extern volatile const FILTER_PMBUS_REGS filter0_pmbus_regs_constants_b;
extern volatile const FILTER_PMBUS_REGS filter0_start_up_pmbus_regs_constants;
extern volatile const FILTER_PMBUS_REGS filter0_start_up_pmbus_regs_constants_b;
extern volatile const FILTER_PMBUS_REGS filter0_cp_pmbus_regs_constants;
extern volatile const FILTER_PMBUS_REGS filter0_cp_pmbus_regs_constants_b;
extern volatile const FILTER_PMBUS_REGS filter1_pmbus_regs_constants;
extern volatile const FILTER_PMBUS_REGS filter1_pmbus_regs_constants_b;
//----------------------------------------------------------------------------------
//	FILE:			flash.c
//
//	Description:	Flash erase and write functions.
//
//  Copyright Texas Instruments © 2008
//----------------------------------------------------------------------------------
//#include "include.h"


//==========================================================================================
// look_for_interrupted_dflash_erase() 
// This function is getting called at startup
// It checks the status of both Dflash blocks A & B and erases one if one is not completely balnk
// This ebables the function restore_default_all() to be responsive and eliminate or minimize the need for NACK
//==========================================================================================

void write_program_flash_word(int * address, int data)
{
	while(DecRegs.PFLASHCTRL.bit.BUSY != 0)
	{
		; //wait for any previous programming to finish
	}
	DecRegs.MFBALR1.bit.RONLY = 0; //make it writable
	DecRegs.FLASHILOCK.all = 0x42DC157E; //unlock flash write
	*address = data;
	DecRegs.MFBALR1.bit.RONLY = 1;
	while(DecRegs.PFLASHCTRL.bit.BUSY != 0)
	{
		; //wait for any previous programming to finish
	}
	//string_out(" cleared!   ");
}

void look_for_interrupted_dflash_erase(void)
{
	volatile Uint32	bytes_to_erase, checksum, checksum_b,blank_checksum;
	bytes_to_erase = ((Uint8*)&pmbus_checksum - (Uint8*)&filter0_pmbus_regs);
	blank_checksum = 0xff * ((Uint8*)&pmbus_checksum - (Uint8*)&filter0_pmbus_regs);
	checksum = calculate_dflash_checksum((Uint8*)&filter0_pmbus_regs, 
		(Uint8*)&pmbus_checksum);
	if(checksum != blank_checksum)
	{
		if((checksum!=pmbus_checksum)&&(pmbus_checksum!=0x87654321))
		{	//erase BANK A
			start_erase_task((void*)&filter0_pmbus_regs_constants, (bytes_to_erase + 4));	
			return;
		}
	}
	checksum_b = calculate_dflash_checksum((Uint8*)&filter0_pmbus_regs_constants_b, 
		(Uint8*)&pmbus_checksum_b);
	if((checksum != blank_checksum) && (checksum_b != blank_checksum))	
	{	
		//BANK A is available and BANK B is not BLANK; Erase BANK B, ignor any checksum of BANK B. 
		//and start erasing BANK B
		start_erase_task((void*)&filter0_pmbus_regs_constants_b, (bytes_to_erase + 4));	
		return;
	}
	//BANK B is empty, don't need erase it again.
	erase_segment_counter = 0;
	return;
}

//==========================================================================================
// update_data_flash() 
//	Writes a variable-length block of data to Data Flash.  This function is the one and 
//	only function that should be called by the application code (i.e. STORE_DEFAULT_ALL
//	and WRITE_CLA_GAINS).  This function will take care of writing partial segments or
//	full segments of Data Flash as needed.
//
// Modified Globals:
//==========================================================================================
Uint8 update_data_flash(void* dest_ptr, const void* src_ptr, Uint16 byte_count)                   
{
	// Copy block from RAM to DFlash.
	// The write_data_flash_block function is a SWI, which cannot return a value.
	// Its response code is stored in the global var flash_write_status.
	write_data_flash_block((Uint32)dest_ptr, (Uint32)src_ptr, byte_count);

	return	flash_write_status;
}

//==========================================================================================
// start_erase_task():
// 	Sets up the erase task to erase one or more segments of Data Flash.
// The erase task will be periodically polled and updated from main_loop().  This frees
// the processor to do other operations while the DFlash is being erased rather than 
// hogging the processor for 20-450 milliseconds at a time
//
// // Assumptions: Destination pointer should be word aligned, or it could erase parts of 
// // other blocks.
//  
//==========================================================================================
Uint8 start_erase_task(const void* dest_ptr, Uint16 byte_count)
{
	Uint8	first_segment;	// 
	//	Uint32	segment_number;	// Holds the segment currently being updated
	//	Uint32	num_segments;	// Number of DFlash segments, including any partials
	Uint8	status;

	// Validate destination and calculate which segment(s) of Data Flash to erase
	status = calc_flash_segments(dest_ptr, byte_count, &first_segment);
	if(status != FLASH_SUCCESS)
	{ 
		return status;	// Error: Starting or Ending addr out of range
	}
	// Calculate the number of segments required to store the desired number of bytes.
	// Round up to include any partial segments.
	erase_segment_counter = (byte_count + DATA_FLASH_SEGMENT_SIZE-1) / DATA_FLASH_SEGMENT_SIZE;
	// Set the segment number for the last segment to be erased.
	// This will erase the required segments in reverse order.  
	// The checksum will be the first to go, guaranteeing that the partially erased segment
	// will not be accidentally considered valid when reading DFlash.
	erase_segment_number = first_segment + erase_segment_counter - 1; 

	// Call the SWI to start erasing the indicated segment and return without waiting for
	// the erasure to complete.
	erase_dflash_segment_no_delay(erase_segment_number);
	// Each segment takes ~20ms to erase.  Return and let the erase_task called from 
	// main_loop() handle the delays and the erasing of multiple segments.
	return FLASH_SUCCESS;
}	

//==========================================================================================
// erase_task():
//
//	If there are still DFlash segments that need erasing and the DFlash is not busy
// erasing one of them, start erasing the next segment.
//==========================================================================================
void erase_task(void)
{
	// 	if (erase_segment_counter == 0)
	// 	{
	// 		return; // No erase in progress.  Bail immediately.
	// 	}

	if (DecRegs.DFLASHCTRL.bit.BUSY != 0)
	{
		return;	// Previous segment erase still in progress
	}

	// Decrement the number of segments left to erase.
	erase_segment_counter--;
	if (erase_segment_counter > 0)
	{
		erase_segment_number--;
		// Still segments left to erase. 
		// Call the SWI to start erasing the next segment and return without waiting for
		// the erasure to complete.
		erase_dflash_segment_no_delay(erase_segment_number);
		// Each segment takes ~20ms to erase.  Leave for now.  
		// We will come back later to handle the next segment.
	}
	return;	
}

//==========================================================================================
// calculate_dflash_checksum:
//  Calculates a 32-bit checksum by adding up the bytes starting at the specified address 
//	for the specified number of bytes.
//==========================================================================================
Uint32 calculate_dflash_checksum(Uint8 *start_addr, Uint8 *end_addr)
{
	Uint8 *addr = start_addr;
	Uint32	checksum = 0;

	while(addr < end_addr)
	{
		checksum = checksum + (Uint32)*addr++ ;
	}
	return (checksum);
}

//==========================================================================================
// calc_flash_segments()
// This function validates the destination address range and calculates the Data Flash 
//	segment numbers for the area of flash specified by the DEST_PTR and BYTE_COUNT 
//	and returns a structure containing the first and last segments needed. 
//
// Modified Globals:
//==========================================================================================
inline Uint8 calc_flash_segments(const void* dest_ptr, Uint16 byte_count, Uint8* first_segment)                   
{                                                                                           

	Uint32 dest_addr = (Uint32)dest_ptr;	// Cast away the CONST nature of these pointer
	// but only within this function

	// Verify that the starting address is within the valid memory range
	if(   (dest_addr > DATA_FLASH_END_ADDRESS) 
		|| (dest_addr < DATA_FLASH_START_ADDRESS) )
	{

		return FLASH_INVALID_SEGMENT;	// Error: Starting addr out of range
	}

	// Verify that the ending address is within the valid memory range
	// 16-bit byte_count can't force the end to "wrap".
	if(dest_addr+byte_count-1 > DATA_FLASH_END_ADDRESS)
	{
		return FLASH_INVALID_SEGMENT;	// Error: Ending addr out of range
	}

	// Calculate which segment(s) of Data Flash to erase
	if(first_segment != NULL)
	{
		*first_segment = (Uint8)( (dest_addr - DATA_FLASH_START_ADDRESS) / DATA_FLASH_SEGMENT_SIZE );
	}

	return FLASH_SUCCESS;
}

//==========================================================================================
// erase_one_section(int first_segment, int byte_count)
// This function erases the section starting at the first_segment and the section is
// of byte_count.
// Modified Globals:
//==========================================================================================
void erase_one_section(int first_segment, int byte_count)
{
	while (byte_count > 0)
	{
		erase_data_flash_segment(first_segment);
		byte_count -= 32;
		first_segment++;
	}
}

//==========================================================================================
// Modified Globals:filter register setting
// set filter's CLA gains and NL from the cla_gains_constants[][] array
// before call this function, need set the control bits 
// BNA_ACTIVE, NL_PG_CONTROL, and BNA_READ correctly
//input: 
//  loop_index: which CLA loop to select
//  bank_index: which bank of the given CLA loop
//==========================================================================================
/*
void load_filter_from_flash(Uint8 loop_index, Uint8 bank_index)
{
 
  volatile const  cla_gains_struct*	          cla_gains_ptr;
  Uint32 	checksum;


	// Wait for any erase that might be in progress to complete.
	// IMPORTANT!  This must be done before attempting to access any DFlash location, 
	// even if it is not in the segment being erased.
	while(DecRegs.DFLASHCTRL.bit.BUSY != 0)
	{
		; //do nothing while busy erasing DFlash
	}	

	// Look in Data Flash A for valid values.
	checksum = calculate_dflash_checksum((Uint8*)&cla_gains_constants, 
										 (Uint8*)&pmbus_checksum);
	// A zero checksum only occurs when the segment is all zeroes, which is not valid.
	// If the calculated checksum is nonzero and matches the checksum in the DFlash,  
	// that segment is good, so use it.
	if(   (pmbus_checksum == 0x87654321)	// Hardcoded exception for parms written directly to data flash
												// (GUI download tool does not calculate checksum)
	    ||((checksum != 0) && (checksum == pmbus_checksum)) )	// Checksum is valid and matches.
	{
		// Checksum A Good: Use default values from DFlash A
		cla_gains_ptr	         = &cla_gains_constants[0][0];
	}
	else
	{
		// Look in Data Flash B for valid values
		checksum = calculate_dflash_checksum((Uint8*)&cla_gains_constants_b, 
											 (Uint8*)&pmbus_checksum_b);
		// A zero checksum only occurs when the segment is all zeroes, which is not valid.
		// If the calculated checksum is nonzero and matches the checksum in the DFlash,  
		// that segment is good, so use it.
		if ((checksum != 0) && (checksum == pmbus_checksum_b))
		{
			// Checksum B Good: Use default values from DFlash B
			cla_gains_ptr	         = &cla_gains_constants_b[0][0];
		}	
	}

 
  if((loop_index < NUM_CLA_LOOPS) && (bank_index < NUM_CLA_BANKS))
  {
    FilterRegs[loop_index]->FLTRCOEF1.bit.B01 = (cla_gains_ptr + (loop_index * NUM_CLA_BANKS) + bank_index)->B01;
	FilterRegs[loop_index]->FLTRCOEF1.bit.B11 = (cla_gains_ptr + (loop_index * NUM_CLA_BANKS) + bank_index)->B11;
	FilterRegs[loop_index]->FLTRCOEF2.bit.B21 = (cla_gains_ptr + (loop_index * NUM_CLA_BANKS) + bank_index)->B21;
	FilterRegs[loop_index]->FLTRCOEF2.bit.COEF_SCALER = (cla_gains_ptr + (loop_index * NUM_CLA_BANKS) + bank_index)->COEF_SCALER;
	FilterRegs[loop_index]->FLTRCOEF3.bit.A11 = (cla_gains_ptr + (loop_index * NUM_CLA_BANKS) + bank_index)->A11;
	FilterRegs[loop_index]->FLTRCOEF3.bit.A21 = (cla_gains_ptr + (loop_index * NUM_CLA_BANKS) + bank_index)->A21;
	FilterRegs[loop_index]->FLTRCOEF4.bit.B12 = (cla_gains_ptr + (loop_index * NUM_CLA_BANKS) + bank_index)->B12;
    FilterRegs[loop_index]->FLTRCOEF4.bit.A12 = (cla_gains_ptr + (loop_index * NUM_CLA_BANKS) + bank_index)->A12;

	FilterRegs[loop_index]->FLTRNLR1.bit.AFE_GAIN = (cla_gains_ptr + (loop_index * NUM_CLA_BANKS) + bank_index)->AFE_GAIN;
	FilterRegs[loop_index]->FLTRNLR1.bit.NOM_GAIN_MULT = (cla_gains_ptr + (loop_index * NUM_CLA_BANKS) + bank_index)->NOM_GAIN_MULT;
    FilterRegs[loop_index]->FLTRNLR1.bit.POS_MID_GAIN_MULT = (cla_gains_ptr + (loop_index * NUM_CLA_BANKS) + bank_index)->POS_MID_GAIN_MULT;
    FilterRegs[loop_index]->FLTRNLR1.bit.POS_LRG_GAIN_MULT = (cla_gains_ptr + (loop_index * NUM_CLA_BANKS) + bank_index)->POS_LRG_GAIN_MULT;
    FilterRegs[loop_index]->FLTRNLR1.bit.NEG_MID_GAIN_MULT = (cla_gains_ptr + (loop_index * NUM_CLA_BANKS) + bank_index)->NEG_MID_GAIN_MULT;
    FilterRegs[loop_index]->FLTRNLR1.bit.NEG_LRG_GAIN_MULT = (cla_gains_ptr + (loop_index * NUM_CLA_BANKS) + bank_index)->NEG_LRG_GAIN_MULT; 
    FilterRegs[loop_index]->FLTRNLR2.bit.LIMIT3 = (cla_gains_ptr + (loop_index * NUM_CLA_BANKS) + bank_index)->LIMIT3;
    FilterRegs[loop_index]->FLTRNLR2.bit.LIMIT2 = (cla_gains_ptr + (loop_index * NUM_CLA_BANKS) + bank_index)->LIMIT2;
    FilterRegs[loop_index]->FLTRNLR2.bit.LIMIT1 = (cla_gains_ptr + (loop_index * NUM_CLA_BANKS) + bank_index)->LIMIT1;
    FilterRegs[loop_index]->FLTRNLR2.bit.LIMIT0 = (cla_gains_ptr + (loop_index * NUM_CLA_BANKS) + bank_index)->LIMIT0; 
  }
   
    load_filter_to_cla_gains_in_ram(loop_index, bank_index);
}

//==========================================================================================
// 
//
// Modified Globals:cla_gains
// before call this function, need set the control bits BNA_ACTIVE, NL_PG_CONTROL correctly
//==========================================================================================
void load_filter_to_cla_gains_in_ram(Uint8 loop_index, Uint8 bank_index)
{
   if ( (loop_index < NUM_CLA_LOOPS) && (bank_index < NUM_CLA_BANKS))
   {
    cla_gains[loop_index][bank_index].B01 = FilterRegs[loop_index]->FLTRCOEF1.bit.B01;
	cla_gains[loop_index][bank_index].B11 = FilterRegs[loop_index]->FLTRCOEF1.bit.B11;
	cla_gains[loop_index][bank_index].B21 = FilterRegs[loop_index]->FLTRCOEF2.bit.B21;
	cla_gains[loop_index][bank_index].COEF_SCALER = FilterRegs[loop_index]->FLTRCOEF2.bit.COEF_SCALER;
	cla_gains[loop_index][bank_index].A11 = FilterRegs[loop_index]->FLTRCOEF3.bit.A11;
	cla_gains[loop_index][bank_index].A21 = FilterRegs[loop_index]->FLTRCOEF3.bit.A21;
	cla_gains[loop_index][bank_index].B12 = FilterRegs[loop_index]->FLTRCOEF4.bit.B12;
    cla_gains[loop_index][bank_index].A12 = FilterRegs[loop_index]->FLTRCOEF4.bit.A12;
	cla_gains[loop_index][bank_index].AFE_GAIN = FilterRegs[loop_index]->FLTRNLR1.bit.AFE_GAIN;
	cla_gains[loop_index][bank_index].NOM_GAIN_MULT = FilterRegs[loop_index]->FLTRNLR1.bit.NOM_GAIN_MULT;
    cla_gains[loop_index][bank_index].POS_MID_GAIN_MULT = FilterRegs[loop_index]->FLTRNLR1.bit.POS_MID_GAIN_MULT;
    cla_gains[loop_index][bank_index].POS_LRG_GAIN_MULT = FilterRegs[loop_index]->FLTRNLR1.bit.POS_LRG_GAIN_MULT;
    cla_gains[loop_index][bank_index].NEG_MID_GAIN_MULT = FilterRegs[loop_index]->FLTRNLR1.bit.NEG_MID_GAIN_MULT;
    cla_gains[loop_index][bank_index].NEG_LRG_GAIN_MULT = FilterRegs[loop_index]->FLTRNLR1.bit.NEG_LRG_GAIN_MULT; 
    cla_gains[loop_index][bank_index].LIMIT3 = FilterRegs[loop_index]->FLTRNLR2.bit.LIMIT3;
    cla_gains[loop_index][bank_index].LIMIT2 = FilterRegs[loop_index]->FLTRNLR2.bit.LIMIT2;
    cla_gains[loop_index][bank_index].LIMIT1 = FilterRegs[loop_index]->FLTRNLR2.bit.LIMIT1;
    cla_gains[loop_index][bank_index].LIMIT0 = FilterRegs[loop_index]->FLTRNLR2.bit.LIMIT0;
   }
}

*/
