//###########################################################################
//
// FILE:    pmbus_manuf_specific_commands.c
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

extern const Uint32 parm_mem_start[NUM_MEMORY_SEGMENTS];
extern const Uint16 parm_mem_length[NUM_MEMORY_SEGMENTS];

const Uint8 cmd_pfc[]            = CMD_PFC;
const Uint8 cmd_dcdc_paged[]     = CMD_DCDC_PAGED;
const Uint8 cmd_dcdc_nonpaged[]  = CMD_DCDC_NONPAGED;

// Base address table used by the PARM_INFO command
#if NUM_MEMORY_SEGMENTS == 19
	       const Uint32	parm_mem_start[19] = {	RAM_START_ADDRESS, 
												REG_START_ADDRESS,  
												DFLASH_START_ADDRESS,	
												PFLASH_CONST_START_ADDRESS,
												PFLASH_PROG_START_ADDRESS,
												LOOP_MUX_START_ADDRESS,
												FAULT_MUX_START_ADDRESS,
												ADC_START_ADDRESS,
												DPWM3_START_ADDRESS,
												FILTER2_START_ADDRESS,
												DPWM2_START_ADDRESS,
												FE_CTRL2_START_ADDRESS,
												FILTER1_START_ADDRESS,
												DPWM1_START_ADDRESS,
												FE_CTRL1_START_ADDRESS,
												FILTER0_START_ADDRESS,
												DPWM0_START_ADDRESS,
												FE_CTRL0_START_ADDRESS,
												SYSTEM_REGS_START_ADDRESS	};

	       const Uint16	parm_mem_length[19] = {	RAM_LENGTH, 
												REG_LENGTH,  
												DFLASH_LENGTH,	
												PFLASH_CONST_LENGTH,
												PFLASH_PROG_LENGTH,
												LOOP_MUX_LENGTH,
												FAULT_MUX_LENGTH,
												ADC_LENGTH,
												DPWM3_LENGTH,
												FILTER2_LENGTH,
												DPWM2_LENGTH,
												FE_CTRL2_LENGTH,
												FILTER1_LENGTH,
												DPWM1_LENGTH,
												FE_CTRL1_LENGTH,
												FILTER0_LENGTH,
												DPWM0_LENGTH,
												FE_CTRL0_LENGTH,
												SYSTEM_REGS_LENGTH	};
#elif NUM_MEMORY_SEGMENTS == 5
	       const Uint32	parm_mem_start[5] = {	RAM_START_ADDRESS, 
												REG_START_ADDRESS,  
												DFLASH_START_ADDRESS,	
												PFLASH_CONST_START_ADDRESS,
												PFLASH_PROG_START_ADDRESS	};
	       const Uint16	parm_mem_length[5] = {	RAM_LENGTH, 
												REG_LENGTH,  
												DFLASH_LENGTH,	
												PFLASH_CONST_LENGTH,
												PFLASH_PROG_LENGTH		};
#else // Use only 4 memory segments -- Not Program	
	       const Uint32	parm_mem_start[4] = {	RAM_START_ADDRESS, 
												REG_START_ADDRESS,  
												DFLASH_START_ADDRESS,	
												PFLASH_CONST_START_ADDRESS};
	       const Uint16	parm_mem_length[4] = {	RAM_LENGTH, 
												REG_LENGTH,  
												DFLASH_LENGTH,	
												PFLASH_CONST_LENGTH	};
#endif

const Uint8 status_bit_mask[]    = PFC_STATUS_BIT_MASK;
const Uint8 debug_0_text[] = DEBUG_0_TEXT;
const Uint8 debug_1_text[] = DEBUG_1_TEXT;
const Uint8 debug_2_text[] = DEBUG_2_TEXT;
const Uint8 debug_3_text[] = DEBUG_3_TEXT;
const Uint8 debug_4_text[] = DEBUG_4_TEXT;
const Uint8 debug_5_text[] = DEBUG_5_TEXT;
const Uint8 debug_6_text[] = DEBUG_6_TEXT;
const Uint8 debug_7_text[] = DEBUG_7_TEXT;
const Uint8 vbus_full_range_text[] = VBUS_FULL_RANGE_TEXT;
#define NA_TEXT "N/A"
const Uint8 na_text[] = NA_TEXT;

#define YES_TEXT "Yes"
const Uint8 yes_text[] = YES_TEXT;

Uint8 pmbus_write_ishare_configure(void)
{
	pmbus_dcdc_config[0].ishare_enable = pmbus_buffer[1];

	return PMBUS_SUCCESS;
}
	
Uint8 pmbus_read_ishare_configure(void)
{
	pmbus_buffer[0] = pmbus_dcdc_config[0].ishare_enable;
	pmbus_number_of_bytes = 1;

	return PMBUS_SUCCESS;
}

//==========================================================================================
//pmbus_read_cmd_dcdc_paged
//==========================================================================================
Uint8 pmbus_read_cmd_dcdc_paged(void)
{
	Uint8 byte;
	pmbus_number_of_bytes = 32 + 1;

	for(byte = 0; byte < 32; byte ++)
	{
		pmbus_buffer[byte+1] = (Uint8) cmd_dcdc_paged[byte];
	}	
	pmbus_buffer[0] = 32;
	return 0;
}

//==========================================================================================
//pmbus_read_cmd_dcdc_non_paged
//==========================================================================================
Uint8 pmbus_read_cmd_dcdc_nonpaged(void)
{
	Uint8 byte;
	pmbus_number_of_bytes = 32 + 1;

	for(byte = 0; byte < 32; byte ++)
	{
		pmbus_buffer[byte+1] = (Uint8) cmd_dcdc_nonpaged[byte];
	}	
	pmbus_buffer[0] = 32;
	return 0;
}

Uint8 pmbus_read_cmd_pfc(void)
{
	Uint8 byte;
	pmbus_number_of_bytes = 32 + 1;

	for(byte = 0; byte < 32; byte ++)
	{
		pmbus_buffer[byte+1] = (Uint8) cmd_pfc[byte];
	}	
	pmbus_buffer[0] = 32;
	return 0;
}

Uint8 pmbus_read_debug_buffer(void)
{
	Uint8 byte;
	pmbus_number_of_bytes = 8 + 1;

	for(byte = 0; byte < 8; byte ++)
	{
		pmbus_buffer[byte+1] = debug_buffer[byte];
	}
	pmbus_buffer[0] = 8;

	return PMBUS_SUCCESS;
}

Uint8 pmbus_read_status_bit_mask(void)
{

	Uint8 byte;

	for (byte = 0; byte < 11; byte++)
	{
		pmbus_buffer[byte+1] = (Uint8)status_bit_mask[byte];

	} 
	pmbus_buffer[0] = 11;
	return 0;
}

Uint8 pmbus_write_gui_constant(void)
{
	gui_constant_pointer = pmbus_buffer[2] ;
	return PMBUS_SUCCESS;
}

Uint8 pmbus_write_user_ram_00(void)
{
	user_ram_00 = pmbus_buffer[1];

	return PMBUS_SUCCESS;
}
	
Uint8 pmbus_read_user_ram_00(void)
{
	pmbus_buffer[0] = user_ram_00;
	pmbus_number_of_bytes = 1;

	return PMBUS_SUCCESS;
}

void rom_back_door(void)
{
	// Call a SWI to clear the integrity word.
	clear_integrity_word();		
	// Wait for watchdog to expire and force CPU reset.	
	watchdog_reset();		
}


//==========================================================================================
// pmbus_write_rom_mode() 
//	Erases the program integrity word in FLASH, then waits for watchdog timer to reset the 
// CPU.  There is no return code or return from this function.  
//==========================================================================================
int pmbus_write_rom_mode(void)
{
	rom_back_door();			

	return PMBUS_SUCCESS;			// Note: This line is never reached.  
}

//==========================================================================================
// pmbus_write_parm_info() checks for a valid input then assigns value to
//	parm_index, parm_offset and parm_length.  These values are needed to uniquely identify
//	a variable or register location in the system that will be queried or modified by the
//	parm_value command. 
//
// Global Inputs: 
//	pmbus_buffer[2]		parm_index. 0=RAM, 1=Registers, 2=DFlash, 3=PFlash Consts, {4=PFlash Program}
//	pmbus_buffer[3:4]	signed offset added to base address pointed to by parm_index
//	pmbus_buffer[5]		number of elements to transfer
//  pmbus_buffer[6]		size, in bytes, of the elements to tranfer (1, 2 or 4 bytes)
//
// modified globals
//	parm_index		Index to a memory section base address
//	parm_offset		Offset added to memory section base address, in multiples of parm_size bytes.
//  parm_count		Number of values to be transferred
//	parm_size		Number of bytes for each value (1, 2, or 4)
//==========================================================================================
Uint8 pmbus_write_parm_info(void)
{
	Uint8	temp_index;
	int16	temp_offset;	
	Uint8	temp_count;		
	Uint8	temp_size;	
	Uint8	temp_length;	

	temp_index  = pmbus_buffer[2];
	temp_offset = pmbus_buffer[3] + (pmbus_buffer[4]<<8);
	temp_count	= pmbus_buffer[5];
	temp_size   = pmbus_buffer[6];
	temp_length = temp_count * temp_size; 


	// ----- Validate the incoming arguments -----

	// ----- VALIDATE DATA -----
	// Verify that the specified base is valid
	// 0 (RAM)  and 1 (REGS) are valid for reading or writing.
	// 2 (DFLASH), 3 (PFLASH_CONST), and 4 (PFLASH_PROG) are read-only.
	// They will be flagged in pmbus_write_parm_value() if a write to them is attempted.
	if(temp_index > NUM_MEMORY_SEGMENTS)  // Unsigned.    
	{
		return PMBUS_INVALID_DATA;	// Error: Invalid Index
	}

	// Verify that the message is short enough to fit
	if(temp_length > 32)
	{
		return PMBUS_INVALID_DATA;	// Error: Length greater than SAA capabilities.
	}

	// Verify that the size is 1, 2, or 4 bytes 
	if ((temp_size != 1) && (temp_size != 2) && (temp_size != 4))
	{
		return PMBUS_INVALID_DATA;	// Error: Invalid size
	}


	// ----- EXECUTE COMMAND -----
	// Arguments valid, assign values.
	parm_index	= temp_index;
	parm_offset	= temp_offset;
	parm_count	= temp_count;
	parm_size   = temp_size;

	return PMBUS_SUCCESS;
}

//==========================================================================================
// pmbus_read_parm_info() returns the value of the parm_index variable.
//==========================================================================================
Uint8 pmbus_read_parm_info(void)		                                                               
{                                                                                                                                                          
	// return a block of data with parm_index, parm_offset and parm_length   
	Uint8	num_bytes = 5;                                                                                                
	pmbus_buffer[0] = num_bytes;
	pmbus_buffer[1] = (Uint8)(parm_index);	
	pmbus_buffer[2] = (Uint8)(parm_offset & 0xff); 		//low byte of value
	pmbus_buffer[3] = (Uint8)(parm_offset >> 8); 		//high byte of value    
	pmbus_buffer[4] = (Uint8)(parm_count);
	pmbus_buffer[5] = (Uint8)(parm_size);    
	pmbus_number_of_bytes = num_bytes+1;                                                            
	return PMBUS_SUCCESS;                                                               
}                                                                                                                                                          
 
//==========================================================================================
// pmbus_write_parm_value()  This command gives the host a way to write virtually any
//	memory address in the system.
//	The host issues a parm_info command to set-up the parm_index, parm_offset
//	and parm_length parameters that determine how this command will	handle the incoming 
//	data. 
//
// modified globals
//	The memory pointed to by the combination of parm_index, parm_offset, and parm_size
//==========================================================================================
Uint8 pmbus_write_parm_value(void)
{
	Uint8	buffer_index;	// index into pmbus_buffer
	Uint16	start_offset;	// Byte Offset into selected memory segment
	Uint8	length;			// Total number of bytes to transfer
	Uint32	start_address;	// Starting byte address in selected memory segment
	Uint32	i;				// generic loop counter

	length = parm_count * parm_size;	// Total number of bytes to transfer
	start_offset = parm_offset * parm_size;

	// ----- VALIDATE DATA -----
	// Verify that the message is short enough to fit and that 
	// the number of bytes sent in this PARM_VALUE command matches the number
	// of bytes requested in the previous PARM_INFO command.
	if( (length > 32) || (length != pmbus_buffer[1]) )
	{
		return	PMBUS_INVALID_DATA;		// Error: Length greater than SAA capabilities
		//     or Lengths don't match
	}

	// ----- Validate the parm arguments -----
	// These should have already been checked by the PARM_INFO command, 
	// but because writing to invalid locations could cause so much havoc,
	// we will check them again here.

	// Verify that the index is valid for writing
	if ((parm_index == 2) || (parm_index == 3) || (parm_index == 4) || parm_index == 18)
		// don't allow writes to any flash or to system registers.
	{
		return PMBUS_INVALID_DATA;	// Error: Invalid Index
	}

	// Verify that the starting and ending offsets are both within the valid memory range
	if (  ( start_offset		 > parm_mem_length[parm_index])	
		||((start_offset+length) > parm_mem_length[parm_index])  )
	{
		return PMBUS_INVALID_DATA;	// Error: Starting or ending addr out of range
	}



	// ----- EXECUTE COMMAND -----
	// Else it is a valid address.
	start_address = parm_mem_start[parm_index] + start_offset; 


	buffer_index = 2;	// Data starts in 3rd byte of buffer (for C the third byte is index=2)

	// Verification that the size is 1, 2, or 4 bytes is handled by the 'default' case in
	// the switch statement below.

	// Fill the transmit buffer with the requested data in 1, 2, or 4 byte chunks.
	switch (parm_size)
	{
	case 1:			// Transfer 1 byte at a time
		{	
			Uint8*	dest_ptr = (Uint8*)start_address;	

			for (i=0; i<parm_count; i++)
			{
				// Copy from the pmbus_buffer to the destination 1 byte at a time
				Uint8 val= pmbus_buffer[buffer_index];	
				*dest_ptr++ = val;
				buffer_index += parm_size;
			}
			break;
		}

	case 2:		// Transfer 2 bytes at a time
		{
			Uint16*	dest_ptr = (Uint16*)start_address;	

			for (i=0; i<parm_count; i++)
			{
				// Copy from the pmbus_buffer to the destination 2 bytes at a time
				Uint16 val=   (pmbus_buffer[buffer_index+0] << 0)
					+ (pmbus_buffer[buffer_index+1] << 8);		    
				*dest_ptr++ = val;
				buffer_index += parm_size;
			}
			break;
		}

	case 4:		// Transfer 4 bytes at a time
		{
			Uint32*	dest_ptr = (Uint32*)start_address;	

			for (i=0; i<parm_count; i++)
			{
				// Copy from the pmbus_buffer to the destination 4 bytes at a time
				Uint32 val=  (pmbus_buffer[buffer_index+0] << 0 )
					+ (pmbus_buffer[buffer_index+1] << 8 )		    
					+ (pmbus_buffer[buffer_index+2] << 16)
					+ (pmbus_buffer[buffer_index+3] << 24);
				*dest_ptr++ = val;
				buffer_index += parm_size;
			}
			break;
		}
	default:
		{
			// Should never get here since size should have been checked by parm_info cmd
			return	PMBUS_INVALID_DATA;		// Error: Invalid size
		}
	}	// end switch (parm_size)

	return PMBUS_SUCCESS;
}
                                                                                                                                                           
//==========================================================================================
// pmbus_read_parm_value() returns the value of the parm_index variable.
//
//	Assumptions: This function assumes that write_parm_info has ensured these conditions. 
//		* parm_index is pointing to a valid memory segment
//		* transfer length (==parm_count*parm_size) is <= 32 bytes
//==========================================================================================
Uint8 pmbus_read_parm_value(void)		                                                               
{   
	Uint16	start_offset;	// Byte Offset into selected memory segment
	Uint8 	length;			// Total number of bytes to transfer
	Uint8*	start_address; 	// Starting byte address in selected memory segment
	Uint8   i;

	// Verify that the starting and ending offsets are both within the valid memory range
	start_offset = parm_offset * parm_size;
	length       = parm_count  * parm_size;
	if (  ( start_offset 		 > parm_mem_length[parm_index])	
		||((start_offset+length) > parm_mem_length[parm_index])  )
	{
		return PMBUS_INVALID_DATA;	// Error: Starting or ending addr out of range
	}

	// Else it is a valid address.
	start_address = (Uint8*)parm_mem_start[parm_index] + start_offset; 

		pmbus_number_of_bytes = length + 1;

	for (i = 0; i < length; i++)
	{
		if(start_address == (Uint8*)0xfff7f040) //fake Cyclone 2
		{
			pmbus_buffer[i + 1] = 1;
			start_address++;
		}
		else 
		{                                              
			pmbus_buffer[i + 1] = *start_address++;
		}
	}
	pmbus_buffer[0] = length;

	return PMBUS_SUCCESS;
}                   

int pmbus_write_pfc_phase_2_enable(void)
{
	if(!pmbus_buffer[1])
	{
		// ----- EXECUTE COMMAND -----
		pfc_phase_2_enable = DISABLE;
		return PMBUS_SUCCESS;
	}
	else
	{
		pfc_phase_2_enable = ENABLE;
		return PMBUS_SUCCESS;
	}
}

Uint8 pmbus_read_pfc_phase_2_enable(void)
{
	pmbus_number_of_bytes = 1;
	pmbus_buffer[0] = (Uint8) pfc_phase_2_enable;
	return PMBUS_SUCCESS;
}

int pmbus_write_pfc_zvs_enable(void)
{
	if(!pmbus_buffer[1])
	{
		// ----- EXECUTE COMMAND -----
		pfc_zvs_enable = DISABLE;
		return PMBUS_SUCCESS;
	}
	else
	{
		pfc_zvs_enable = ENABLE;
		return PMBUS_SUCCESS;
	}
}

Uint8 pmbus_read_pfc_zvs_enable(void)
{
	pmbus_number_of_bytes = 1;
	pmbus_buffer[0] = (Uint8) pfc_zvs_enable;
	return PMBUS_SUCCESS;
}

int pmbus_write_pfc_os_enable(void)
{
	if(!pmbus_buffer[1])
	{
		// ----- EXECUTE COMMAND -----
		pfc_os_enable = DISABLE;
		return PMBUS_SUCCESS;
	}
	else
	{
		pfc_os_enable = ENABLE;
		return PMBUS_SUCCESS;
	}
}

Uint8 pmbus_read_pfc_os_enable(void)
{
	pmbus_number_of_bytes = 1;
	pmbus_buffer[0] = (Uint8) pfc_os_enable;
	return PMBUS_SUCCESS;
}

int pmbus_write_llc_sr_enable(void)
{
	if(!pmbus_buffer[1])
	{
		// ----- EXECUTE COMMAND -----
		llc_sr_enable = DISABLE;
		return PMBUS_SUCCESS;
	}
	else
	{
		llc_sr_enable = ENABLE;
		return PMBUS_SUCCESS;
	}
}

Uint8 pmbus_read_llc_sr_enable(void)
{
	pmbus_number_of_bytes = 1;
	pmbus_buffer[0] = (Uint8) llc_sr_enable;
	return PMBUS_SUCCESS;
}

void watchdog_reset(void)
{
	//turn off all PWM outputs
	Dpwm0Regs.DPWMCTRL1.bit.GPIO_A_EN = 1;
	Dpwm0Regs.DPWMCTRL1.bit.GPIO_B_EN = 1;
	Dpwm1Regs.DPWMCTRL1.bit.GPIO_A_EN = 1; 
	Dpwm1Regs.DPWMCTRL1.bit.GPIO_B_EN = 1;
	Dpwm2Regs.DPWMCTRL1.bit.GPIO_A_EN = 1;
	Dpwm2Regs.DPWMCTRL1.bit.GPIO_B_EN = 1;
	Dpwm3Regs.DPWMCTRL1.bit.GPIO_A_EN = 1; 
	Dpwm3Regs.DPWMCTRL1.bit.GPIO_B_EN = 1; 
	//Make sure the watchdog is enabled.
	TimerRegs.WDCTRL.bit.CPU_RESET_EN = 1;
	//Set WD period to timeout faster
	TimerRegs.WDCTRL.bit.WD_PERIOD = 1;
}

Uint8 pmbus_write_deadband_config(void)
{
	pmbus_dcdc_config[0].dead_time_1 = pmbus_buffer[3] + (pmbus_buffer[2] << 8);
	pmbus_dcdc_config[0].dead_time_2 = pmbus_buffer[5] + (pmbus_buffer[4] << 8);
	pmbus_dcdc_config[0].dead_time_3 = pmbus_buffer[7] + (pmbus_buffer[6] << 8);
	pmbus_dcdc_config[0].dead_time_4 = pmbus_buffer[9] + (pmbus_buffer[8] << 8);
	pmbus_dcdc_config[0].sample_rate = pmbus_buffer[11] + (pmbus_buffer[10] << 8);

	configure_dpwm_timing();
	return PMBUS_SUCCESS;
}

Uint8 pmbus_read_deadband_config(void)
{
	pmbus_number_of_bytes = 11;

	pmbus_buffer[0] = 10;

	pmbus_buffer[1] = 	pmbus_dcdc_config[0].dead_time_1 >> 8;
	pmbus_buffer[2] = 	pmbus_dcdc_config[0].dead_time_1 & 0xFF;

	pmbus_buffer[3] = 	pmbus_dcdc_config[0].dead_time_2 >> 8;
	pmbus_buffer[4] = 	pmbus_dcdc_config[0].dead_time_2 & 0xFF;

	pmbus_buffer[5] = 	pmbus_dcdc_config[0].dead_time_3 >> 8;
	pmbus_buffer[6] = 	pmbus_dcdc_config[0].dead_time_3 & 0xFF;

	pmbus_buffer[7] = 	pmbus_dcdc_config[0].dead_time_4 >> 8;
	pmbus_buffer[8] = 	pmbus_dcdc_config[0].dead_time_4 & 0xFF;

	pmbus_buffer[9] = 	pmbus_dcdc_config[0].sample_rate >> 8;
	pmbus_buffer[10] = 	pmbus_dcdc_config[0].sample_rate & 0xFF;

	return PMBUS_SUCCESS;
}

Uint8 pmbus_write_mode_switching_config(void)
{
	pmbus_dcdc_config[0].min_period = pmbus_buffer[3] + (pmbus_buffer[2] << 8);
	pmbus_dcdc_config[0].max_period = pmbus_buffer[5] + (pmbus_buffer[4] << 8);
	pmbus_dcdc_config[0].tsrmax 	= pmbus_buffer[7] + (pmbus_buffer[6] << 8);
	configure_dpwm_timing();
	return PMBUS_SUCCESS;
}

Uint8 pmbus_read_mode_switching_config(void)
{
	pmbus_number_of_bytes = 7;

	pmbus_buffer[0] = 6;

	pmbus_buffer[1] = 	pmbus_dcdc_config[0].min_period  >> 8;
	pmbus_buffer[2] = 	pmbus_dcdc_config[0].min_period & 0xFF;

	pmbus_buffer[3] = 	pmbus_dcdc_config[0].max_period >> 8;
	pmbus_buffer[4] = 	pmbus_dcdc_config[0].max_period & 0xFF;

	pmbus_buffer[5] = 	pmbus_dcdc_config[0].tsrmax >> 8;
	pmbus_buffer[6] = 	pmbus_dcdc_config[0].tsrmax & 0xFF;

	return PMBUS_SUCCESS;
}

Uint8 pmbus_write_light_load_config(void)
{
	pmbus_dcdc_config[0].ll_turn_on_thresh  = pmbus_buffer[4] + (pmbus_buffer[3] << 8) + (pmbus_buffer[2] << 16);
	pmbus_dcdc_config[0].ll_turn_off_thresh = pmbus_buffer[7] + (pmbus_buffer[6] << 8) + (pmbus_buffer[5] << 16);
	pmbus_dcdc_config[0].ll_en 				= pmbus_buffer[8] >> 6;
	fiq_data.ll_en = pmbus_buffer[8] >> 6; 
	return PMBUS_SUCCESS;
}

Uint8 pmbus_read_light_load_config(void)
{
	pmbus_number_of_bytes = 8;

	pmbus_buffer[0] = 7;

	pmbus_buffer[1] =  (pmbus_dcdc_config[0].ll_turn_on_thresh >> 16) & 0xFF;
	pmbus_buffer[2] =  (pmbus_dcdc_config[0].ll_turn_on_thresh >> 8) & 0xFF;
	pmbus_buffer[3] = 	pmbus_dcdc_config[0].ll_turn_on_thresh & 0xFF;

	pmbus_buffer[4] =  (pmbus_dcdc_config[0].ll_turn_off_thresh >> 16) & 0xFF;
	pmbus_buffer[5] =  (pmbus_dcdc_config[0].ll_turn_off_thresh >> 8) & 0xFF;
	pmbus_buffer[6] = 	pmbus_dcdc_config[0].ll_turn_off_thresh & 0xFF;

	pmbus_buffer[7] = 	pmbus_dcdc_config[0].ll_en << 6;

	return PMBUS_SUCCESS;
}

Uint8 pmbus_write_cpcc_config(void)
{
	pmbus_dcdc_config[0].cpcc_pmax   = pmbus_buffer[3] + (pmbus_buffer[2] << 8);
	pmbus_dcdc_config[0].cpcc_imax   = pmbus_buffer[5] + (pmbus_buffer[4] << 8);
	pmbus_dcdc_config[0].cpcc_ton 	 = pmbus_buffer[7] + (pmbus_buffer[6] << 8);
	pmbus_dcdc_config[0].cpcc_enable = pmbus_buffer[8];
	pmbus_dcdc_config[0].cpcc_time_out_en = pmbus_buffer[9];
	fiq_data.cpcc_en = pmbus_dcdc_config[0].cpcc_enable;
	configure_cc_dac_value();
	configure_constant_power();
	return PMBUS_SUCCESS;
}

Uint8 pmbus_read_cpcc_config(void)
{
	pmbus_number_of_bytes = 9;

	pmbus_buffer[0] = 8;

	pmbus_buffer[1] = 	pmbus_dcdc_config[0].cpcc_pmax  >> 8;
	pmbus_buffer[2] = 	pmbus_dcdc_config[0].cpcc_pmax & 0xFF;

	pmbus_buffer[3] = 	pmbus_dcdc_config[0].cpcc_imax >> 8;
	pmbus_buffer[4] = 	pmbus_dcdc_config[0].cpcc_imax & 0xFF;

	pmbus_buffer[5] = 	pmbus_dcdc_config[0].cpcc_ton >> 8;
	pmbus_buffer[6] = 	pmbus_dcdc_config[0].cpcc_ton & 0xFF;

	pmbus_buffer[7] = 	pmbus_dcdc_config[0].cpcc_enable & 0xFF;

	pmbus_buffer[8] = 	pmbus_dcdc_config[0].cpcc_time_out_en & 0xFF;

	return PMBUS_SUCCESS;
}

Uint8 pmbus_read_filter_select(void)
{
	pmbus_number_of_bytes = 1;
	pmbus_buffer[0]	= (filter_activate << 4) + filter_select;

	return PMBUS_SUCCESS;
}

Uint8 pmbus_write_filter_select(void)
{
	filter_select   = pmbus_buffer[1] & 0x0F;
	filter_activate = pmbus_buffer[1] >> 4;

	switch (filter_select)
	{
		case FILTER_VOLTAGE_LOOP_START_UP:
			filter_destination = &filter0_start_up_pmbus_regs;
			break;
		case FILTER_VOLTAGE_LOOP_STEADY_STATE:
			filter_destination = &filter0_pmbus_regs;
			break;
		case FILTER_VOLTAGE_LOOP_CONSTANT_POWER:
			filter_destination = &filter0_cp_pmbus_regs;
			break;
		case FILTER_CURRENT_LOOP_STEADY_STATE:
			filter_destination = &filter1_pmbus_regs;
			break;
		default:
			filter_destination = &filter0_pmbus_regs;
			break;
	};

	if (filter_activate)
	{
		configure_new_compensation(filter_destination);
	}

	return PMBUS_SUCCESS;
}

Uint8 pmbus_write_filter_gains(void)
{
	filter_destination->FILTERKPCOEF0.bit.KP_COEF_0    = (pmbus_buffer[2]  << 8) + pmbus_buffer[3];
	filter_destination->FILTERKPCOEF0.bit.KP_COEF_1    = (pmbus_buffer[4]  << 8) + pmbus_buffer[5];
	filter_destination->FILTERKPCOEF1.bit.KP_COEF_2    = (pmbus_buffer[6]  << 8) + pmbus_buffer[7];

	filter_destination->FILTERKICOEF0.bit.KI_COEF_0    = (pmbus_buffer[8]  << 8) + pmbus_buffer[9];
	filter_destination->FILTERKICOEF0.bit.KI_COEF_1    = (pmbus_buffer[10] << 8) + pmbus_buffer[11];
	filter_destination->FILTERKICOEF1.bit.KI_COEF_2    = (pmbus_buffer[12] << 8) + pmbus_buffer[13];
	filter_destination->FILTERKICOEF1.bit.KI_COEF_3    = (pmbus_buffer[14] << 8) + pmbus_buffer[15];

	filter_destination->FILTERKDCOEF0.bit.KD_COEF_0    = (pmbus_buffer[16] << 8) + pmbus_buffer[17];
	filter_destination->FILTERKDCOEF0.bit.KD_COEF_1    = (pmbus_buffer[18] << 8) + pmbus_buffer[19];
	filter_destination->FILTERKDCOEF1.bit.KD_COEF_2    = (pmbus_buffer[20] << 8) + pmbus_buffer[21];

	filter_destination->FILTERKDALPHA.bit.KD_ALPHA_0   = (pmbus_buffer[22] << 8) + pmbus_buffer[23];
	filter_destination->FILTERKDALPHA.bit.KD_ALPHA_1   = (pmbus_buffer[24] << 8) + pmbus_buffer[25];

	filter_destination->FILTER_MISC_GAIN.bit.KCOMP     = (pmbus_buffer[26] << 8) + pmbus_buffer[27];	
	filter_destination->FILTER_MISC_GAIN.bit.CLA_SCALE = (pmbus_buffer[28] >> 3);
	filter_destination->FILTER_MISC_GAIN.bit.YN_SCALE  = (pmbus_buffer[28] & 0x07);

	filter_destination->FILTERNL0.bit.LIMIT0 = (pmbus_buffer[29] << 8) + pmbus_buffer[30];
	filter_destination->FILTERNL0.bit.LIMIT1 = (pmbus_buffer[31] << 8) + pmbus_buffer[32];
	filter_destination->FILTERNL1.bit.LIMIT2 = (pmbus_buffer[33] << 8) + pmbus_buffer[34];
	filter_destination->FILTERNL1.bit.LIMIT3 = (pmbus_buffer[35] << 8) + pmbus_buffer[36];
	filter_destination->FILTERNL2.bit.LIMIT4 = (pmbus_buffer[37] << 8) + pmbus_buffer[38];
	filter_destination->FILTERNL2.bit.LIMIT5 = (pmbus_buffer[39] << 8) + pmbus_buffer[40];

	filter_destination->COEFCONFIG.bit.BIN0_CONFIG = (pmbus_buffer[41] & 0xE0) >> 5;
	filter_destination->COEFCONFIG.bit.BIN0_ALPHA  = (pmbus_buffer[41] & 0x10) >> 4;
	filter_destination->COEFCONFIG.bit.BIN1_CONFIG = (pmbus_buffer[41] & 0x0E) >> 1;
	filter_destination->COEFCONFIG.bit.BIN1_ALPHA  = (pmbus_buffer[41] & 0x01);
	filter_destination->COEFCONFIG.bit.BIN2_CONFIG = (pmbus_buffer[42] & 0xE0) >> 5;
	filter_destination->COEFCONFIG.bit.BIN2_ALPHA  = (pmbus_buffer[42] & 0x10) >> 4;
	filter_destination->COEFCONFIG.bit.BIN3_CONFIG = (pmbus_buffer[42] & 0x0E) >> 1;
	filter_destination->COEFCONFIG.bit.BIN3_ALPHA  = (pmbus_buffer[42] & 0x01);
	filter_destination->COEFCONFIG.bit.BIN4_CONFIG = (pmbus_buffer[43] & 0xE0) >> 5;
	filter_destination->COEFCONFIG.bit.BIN4_ALPHA  = (pmbus_buffer[43] & 0x10) >> 4;
	filter_destination->COEFCONFIG.bit.BIN5_CONFIG = (pmbus_buffer[43] & 0x0E) >> 1;
	filter_destination->COEFCONFIG.bit.BIN5_ALPHA  = (pmbus_buffer[43] & 0x01);
	filter_destination->COEFCONFIG.bit.BIN6_CONFIG = (pmbus_buffer[44] & 0xE0) >> 5;
	filter_destination->COEFCONFIG.bit.BIN6_ALPHA  = (pmbus_buffer[44] & 0x10) >> 4;
	filter_destination->FILTER_MISC.bit.NL_MODE    = (pmbus_buffer[44] & 0x08) >> 3;
	filter_destination->FILTER_MISC.bit.AUTO_GEAR_SHIFT = (pmbus_buffer[44] & 0x04) >> 2;
	filter_destination->FILTER_MISC.bit.AFE_GAIN   		= (pmbus_buffer[44] & 0x03);

	return PMBUS_SUCCESS;
}


Uint8 pmbus_read_filter_gains(void)
{
	pmbus_number_of_bytes = 44;
	pmbus_buffer[0] = 43;

	pmbus_buffer[1]  = (Uint8)(filter_destination->FILTERKPCOEF0.bit.KP_COEF_0 >> 8);
	pmbus_buffer[2]  = (Uint8)(filter_destination->FILTERKPCOEF0.bit.KP_COEF_0 & 0xFF);
	pmbus_buffer[3]  = (Uint8)(filter_destination->FILTERKPCOEF0.bit.KP_COEF_1 >> 8);
	pmbus_buffer[4]  = (Uint8)(filter_destination->FILTERKPCOEF0.bit.KP_COEF_1 & 0xFF);
	pmbus_buffer[5]  = (Uint8)(filter_destination->FILTERKPCOEF1.bit.KP_COEF_2 >> 8);
	pmbus_buffer[6]  = (Uint8)(filter_destination->FILTERKPCOEF1.bit.KP_COEF_2 & 0xFF);

	pmbus_buffer[7]  = (Uint8)(filter_destination->FILTERKICOEF0.bit.KI_COEF_0 >> 8);
	pmbus_buffer[8]  = (Uint8)(filter_destination->FILTERKICOEF0.bit.KI_COEF_0 & 0xFF);
	pmbus_buffer[9]  = (Uint8)(filter_destination->FILTERKICOEF0.bit.KI_COEF_1 >> 8);
	pmbus_buffer[10] = (Uint8)(filter_destination->FILTERKICOEF0.bit.KI_COEF_1 & 0xFF);
	pmbus_buffer[11] = (Uint8)(filter_destination->FILTERKICOEF1.bit.KI_COEF_2 >> 8);
	pmbus_buffer[12] = (Uint8)(filter_destination->FILTERKICOEF1.bit.KI_COEF_2 & 0xFF);
	pmbus_buffer[13] = (Uint8)(filter_destination->FILTERKICOEF1.bit.KI_COEF_3 >> 8);
	pmbus_buffer[14] = (Uint8)(filter_destination->FILTERKICOEF1.bit.KI_COEF_3 & 0xFF);

	pmbus_buffer[15] = (Uint8)(filter_destination->FILTERKDCOEF0.bit.KD_COEF_0 >> 8);
	pmbus_buffer[16] = (Uint8)(filter_destination->FILTERKDCOEF0.bit.KD_COEF_0 & 0xFF);
	pmbus_buffer[17] = (Uint8)(filter_destination->FILTERKDCOEF0.bit.KD_COEF_1 >> 8);
	pmbus_buffer[18] = (Uint8)(filter_destination->FILTERKDCOEF0.bit.KD_COEF_1 & 0xFF);
	pmbus_buffer[19] = (Uint8)(filter_destination->FILTERKDCOEF1.bit.KD_COEF_2 >> 8);
	pmbus_buffer[20] = (Uint8)(filter_destination->FILTERKDCOEF1.bit.KD_COEF_2 & 0xFF);

	pmbus_buffer[21] = (Uint8)(filter_destination->FILTERKDALPHA.bit.KD_ALPHA_0 >> 8) & 0x01;
	pmbus_buffer[22] = (Uint8)(filter_destination->FILTERKDALPHA.bit.KD_ALPHA_0 & 0xFF);
	pmbus_buffer[23] = (Uint8)(filter_destination->FILTERKDALPHA.bit.KD_ALPHA_1 >> 8) & 0x01;
	pmbus_buffer[24] = (Uint8)(filter_destination->FILTERKDALPHA.bit.KD_ALPHA_1 & 0xFF);

	pmbus_buffer[25] = (Uint8)(filter_destination->FILTER_MISC_GAIN.bit.KCOMP >> 8) & 0x3F;
	pmbus_buffer[26] = (Uint8)(filter_destination->FILTER_MISC_GAIN.bit.KCOMP & 0xFF);
	pmbus_buffer[27] = (Uint8)((filter_destination->FILTER_MISC_GAIN.bit.CLA_SCALE << 3) +
								filter_destination->FILTER_MISC_GAIN.bit.YN_SCALE);

	pmbus_buffer[28] = (Uint8)(filter_destination->FILTERNL0.bit.LIMIT0 >> 8) & 0x01;
	pmbus_buffer[29] = (Uint8)(filter_destination->FILTERNL0.bit.LIMIT0 & 0xFF);
	pmbus_buffer[30] = (Uint8)(filter_destination->FILTERNL0.bit.LIMIT1 >> 8) & 0x01;
	pmbus_buffer[31] = (Uint8)(filter_destination->FILTERNL0.bit.LIMIT1 & 0xFF);
	pmbus_buffer[32] = (Uint8)(filter_destination->FILTERNL1.bit.LIMIT2 >> 8) & 0x01;
	pmbus_buffer[33] = (Uint8)(filter_destination->FILTERNL1.bit.LIMIT2 & 0xFF);
	pmbus_buffer[34] = (Uint8)(filter_destination->FILTERNL1.bit.LIMIT3 >> 8) & 0x01;
	pmbus_buffer[35] = (Uint8)(filter_destination->FILTERNL1.bit.LIMIT3 & 0xFF);
	pmbus_buffer[36] = (Uint8)(filter_destination->FILTERNL2.bit.LIMIT4 >> 8) & 0x01;
	pmbus_buffer[37] = (Uint8)(filter_destination->FILTERNL2.bit.LIMIT4 & 0xFF);
	pmbus_buffer[38] = (Uint8)(filter_destination->FILTERNL2.bit.LIMIT5 >> 8) & 0x01;
	pmbus_buffer[39] = (Uint8)(filter_destination->FILTERNL2.bit.LIMIT5 & 0xFF);

	pmbus_buffer[40] = (filter_destination->COEFCONFIG.bit.BIN0_CONFIG << 5) +
					   (filter_destination->COEFCONFIG.bit.BIN0_ALPHA  << 4) +
					   (filter_destination->COEFCONFIG.bit.BIN1_CONFIG << 1) +
					   (filter_destination->COEFCONFIG.bit.BIN1_ALPHA);
	pmbus_buffer[41] = (filter_destination->COEFCONFIG.bit.BIN2_CONFIG << 5) +
					   (filter_destination->COEFCONFIG.bit.BIN2_ALPHA  << 4) +
					   (filter_destination->COEFCONFIG.bit.BIN3_CONFIG << 1) +
					   (filter_destination->COEFCONFIG.bit.BIN3_ALPHA);
	pmbus_buffer[42] = (filter_destination->COEFCONFIG.bit.BIN4_CONFIG << 5) +
					   (filter_destination->COEFCONFIG.bit.BIN4_ALPHA  << 4) +
					   (filter_destination->COEFCONFIG.bit.BIN5_CONFIG << 1) +
					   (filter_destination->COEFCONFIG.bit.BIN5_ALPHA);
	pmbus_buffer[43] = (filter_destination->COEFCONFIG.bit.BIN6_CONFIG 		<< 5) +
					   (filter_destination->COEFCONFIG.bit.BIN6_ALPHA  		<< 4) +
					   (filter_destination->FILTER_MISC.bit.NL_MODE			<< 3) +
					   (filter_destination->FILTER_MISC.bit.AUTO_GEAR_SHIFT << 2) +
					   (filter_destination->FILTER_MISC.bit.AFE_GAIN);

	return PMBUS_SUCCESS;
}
