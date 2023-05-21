//###########################################################################
//
// FILE:    interrupts.c
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

#pragma INTERRUPT(undefined_instruction_exception,UDEF)
void undefined_instruction_exception(void)
{
}

#pragma INTERRUPT(software_interrupt,SWI)
void software_interrupt(Uint32 arg1, Uint32 arg2, Uint32 arg3, Uint8 swi_number)
//void software_interrupt(Uint32 *address, Uint32 data, Uint32 more_data, Uint8 swi_number)
{
	//make sure interrupts are disabled
	asm(" MRS     r4, cpsr "); 		// get psr
	asm(" ORR     r4, r4, #0xc0 "); // set interrupt disables
	asm(" MSR     cpsr_cf, r4"); 		// restore psr

	switch (swi_number) //handle flash write/erase and ROM backdoor first
	{
	case 0: 
		//--------------------------------------------------------------------------------------
		// SWI ALIAS: erase_data_flash_segment()
		// 	Erases one segment of Data Flash and wait for erase to complete.
		//--------------------------------------------------------------------------------------
	case 1: 	
		//--------------------------------------------------------------------------------------
		// SWI ALIAS: erase_dflash_segment_no_delay()
		// 	Erase one segment of Data Flash and return without waiting for completion.
		//--------------------------------------------------------------------------------------
		{
			union DFLASHCTRL_REG dflashctrl_shadow;	// Shadow copy of control register

			if (arg1 >= DATA_FLASH_NUM_SEGMENTS)	
			{
				return;		// Invalid segment number
			}
			DecRegs.FLASHILOCK.all = 0x42DC157E; //unlock flash write;
			// Set the bits in the Data Flash Control Register to erase the indicated segment
			dflashctrl_shadow.all = DecRegs.DFLASHCTRL.all;	// Read the hardware register
			dflashctrl_shadow.bit.PAGE_ERASE = 1; 			// Erase one segment
			dflashctrl_shadow.bit.PAGE_SEL = arg1;		// Segment number
			DecRegs.DFLASHCTRL.all = dflashctrl_shadow.all;	// Write the hardware register
			if (swi_number == 1)	// 0= Wait for erase to complete, 1= return immediately
			{
				return;
			}
			while(DecRegs.DFLASHCTRL.bit.BUSY != 0)
			{
				; //do nothing while it programs
			}
			return;

		}
	case 3: //write word to data flash

		if(((arg1) < DATA_FLASH_START_ADDRESS) ||
			((arg1) > DATA_FLASH_END_ADDRESS))
		{//if out of data flash range
			return;
		}

		//this clears read only bit to permit writes to data flash.
		DecRegs.FLASHILOCK.all = 0x42DC157E; //unlock flash write

		DecRegs.MFBALR2.bit.BLOCK_SIZE =2;
		DecRegs.MFBALR2.bit.ADDRESS = 0x22;
		DecRegs.MFBALR2.bit.RONLY = 0;

		//put data in word.
		*(Uint32 *)(arg1 & 0xfffffffc) = arg2 ;

		DecRegs.MFBALR2.bit.RONLY = 1;

		while(DecRegs.DFLASHCTRL.bit.BUSY != 0)
		{
			; //do nothing while it programs
		}
		return;

		//handle interrupt enables/disables next
	case 4: //enable fiq
		asm(" MRS     r0, spsr "); //get saved psr
		asm(" BIC     r0, r0, #0x40 "); // clear fiq disable
		asm(" MSR     spsr_cf, r0"); //restore saved psr
		return;
	case 5: //disable fiq
		asm(" MRS     r0, spsr "); //get saved psr
		asm(" ORR     r0, r0, #0x40 "); // set fiq disable
		asm(" MSR     spsr_cf, r0"); //restore saved psr
		return;
	case 6: //enable irq
		asm(" MRS     r0, spsr "); //get saved psr
		asm(" BIC     r0, r0, #0x80 "); // clear irq disable
		asm(" MSR     spsr_cf, r0"); //restore saved psr
		return;
	case 7: //disable irq
		asm(" MRS     r0, spsr "); //get saved psr
		asm(" ORR     r0, r0, #0x80 "); // set irq disable
		asm(" MSR     spsr_cf, r0"); //restore saved psr
		return;
	case 8: //write to fiq/irq program_control_register
		CimRegs.FIRQPR.all = arg1;
		return;
	case 9: //write to fiq/irq program_control_register
		CimRegs.REQMASK.all = arg1;
		return;
	case 10: // switch to supervisor mode
		asm(" MRS     r0, spsr "); //get saved psr
		asm(" BIC	  r0, r0, #0x1F "); // clear 5 lsbs.
		asm(" ORR     r0, r0, #0x13 "); // set mode bits to 13.
		asm(" MSR     spsr_cf, r0"); //restore saved psr
		return;
	case 11: // switch to user mode
		asm(" MRS     r0, spsr "); //get saved psr
		asm(" BIC	  r0, r0, #0x1F "); // clear 5 lsbs.
		asm(" ORR     r0, r0, #0x10 "); // set mode bits to 10.
		asm(" MSR     spsr_cf, r0"); //restore saved psr
		return;
	case 12: // clear integrity word.
		{
			{
				register Uint32 * program_index = (Uint32 *) 0x19000; //store destination address for program
				register Uint32 * source_index = (Uint32 *)zero_out_integrity_word; //Set source address of PFLASH;

				register Uint32 counter;

				for(counter=0; counter < 500; counter++) //Copy program from PFLASH to RAM
				{
					*(program_index++)=*(source_index++);
				}
			}
			{
				register FUNC_PTR func_ptr;
				func_ptr=(FUNC_PTR)0x19000;     //Set function to 0x19000
				func_ptr();
				func_ptr=(FUNC_PTR)0x70000;
				func_ptr(); 
			}        //execute erase checksum

			return;
		}
	case 13: //write block to data flash
		//--------------------------------------------------------------------------------------
		// SWI ALIAS: write_data_flash_block()
		// 	Copies a block of data from a source (typically RAM) to a destination in Data Flash.
		// Handles locking and unlocking the read-only bit.
		// Includes necessary delays while writing.
		// Assumptions:  
		//	Destination address is in Data Flash.
		//	Destination addresses start and end on word boundary.
		//	Source addresses start and end on word boundaries.
		//--------------------------------------------------------------------------------------
		{
			volatile Uint32* dest_ptr = (volatile Uint32*)(arg1 & 0xfffffffc);
			Uint32*	src_ptr =  (Uint32*)(arg2);
			int32	byte_counter = (int32)arg3;	// Use int instead of Uint in case count is not a multiple of 4

			// Validate that destination address is in Data Flash
			if(  ((arg1) < DATA_FLASH_START_ADDRESS)
				||((arg1) > DATA_FLASH_END_ADDRESS)  )
			{//if out of data flash range
				flash_write_status = FLASH_INVALID_ADDRESS;
				return;	// Return without writing to DFlash
			}

			// Clear read-only bit to allow writes to Data Flash.
			DecRegs.MFBALR2.bit.RONLY = 0;	

			// Copy a block of RAM to DFlash						     
			while (byte_counter > 0)
			{
				Uint32	temp_word = *src_ptr++;
				DecRegs.FLASHILOCK.all = 0x42DC157E; //unlock flash write
				// Write the temp word to DFlash.
				*dest_ptr = temp_word;

				// *** Should add value to checksum.
				// checksum += temp.word;

				// Wait for any previous writes to complete.
				while(DecRegs.DFLASHCTRL.bit.BUSY != 0)
				{
					; //do nothing while it programs
				}

				// Read back value from DFlash. Abort if it does not match intended value.
				if (*dest_ptr != temp_word)
				{
					// Set an error flag to indicate write failure.
					flash_write_status = FLASH_MISCOMPARE;
					return;	
				}

				dest_ptr++;
				byte_counter -= 4;
			}

			// Set read-only bit to protect Data Flash
			DecRegs.MFBALR2.bit.RONLY = 1;
			flash_write_status = FLASH_SUCCESS;
			return;
		}
	case 14: // erase entire Program Flash
		{
			{
				register Uint32 * program_index = (Uint32 *) 0x19000; //store destination address for program
				register Uint32 * source_index = (Uint32 *)clear_program_flash; //Set source address of PFLASH;

				register Uint32 counter;

				for(counter=0; counter < 500; counter++) //Copy program from PFLASH to RAM
				{
					*(program_index++)=*(source_index++);
				}
			}
			{
				register FUNC_PTR func_ptr;
				func_ptr=(FUNC_PTR)0x19000;     //Set function to 0x19000
				func_ptr();
				func_ptr=(FUNC_PTR)0x70000;
				func_ptr(); 
			}        //execute mass erase PFLASH

			return;
		}
	default:
		break;
	}
}

#pragma INTERRUPT(abort_prefetch_exception,PABT)
void abort_prefetch_exception(void)
{
}

#pragma INTERRUPT(abort_data_fetch_exception,DABT)
void abort_data_fetch_exception(void)
{
}

//Create a histogram of the xn data. Sample size is number_of_samples
void xn_histogram(void)
{
	//process a sample
	if((loop_counter < number_of_samples) && start_monitor) 
	{
		xn_bins[Filter0Regs.FILTERXNREAD.bit.XN + 32]++;
		loop_counter++;
	}
}
/*
inline void burst_mode_handler(void)
{
	FaultMuxRegs.DCOMPCTRL0.all &= ~(DCOMP_COMP_EN_MASK | DCOMP_INT_EN_MASK);
	FaultMuxRegs.DCOMPCTRL1.all &= ~(DCOMP_COMP_EN_MASK | DCOMP_INT_EN_MASK);

	if ((fiq_data.fault_status_raw & DCOMP0_INT_STATUS_MASK) && (adc_values.io_sense < fiq_data.burst_mode_i_turn_on))
	{
		MiscAnalogRegs.GLBIOEN.all |=  (DPWM0A_GLBIO_BIT_MASK | DPWM0B_GLBIO_BIT_MASK |
			DPWM1A_GLBIO_BIT_MASK | DPWM1B_GLBIO_BIT_MASK | DPWM2A_GLBIO_BIT_MASK | DPWM2B_GLBIO_BIT_MASK);
	}
	else if (fiq_data.fault_status_raw & DCOMP1_INT_STATUS_MASK)
	{
		MiscAnalogRegs.GLBIOEN.all &= ~(DPWM0A_GLBIO_BIT_MASK | DPWM0B_GLBIO_BIT_MASK |
			DPWM1A_GLBIO_BIT_MASK | DPWM1B_GLBIO_BIT_MASK | DPWM2A_GLBIO_BIT_MASK | DPWM2B_GLBIO_BIT_MASK);
		if (fiq_data.burst_mode_en && (adc_values.io_sense > fiq_data.burst_mode_i_turn_off))
		{
			//Unfreeze the integrator.
			Filter0Regs.FILTERCTRL.bit.KI_STALL = 1;
			fiq_data.burst_mode_en = 0;
		}
	}

	fiq_data.fault_status &= ~(DCOMP0_INT_STATUS_MASK | DCOMP1_INT_STATUS_MASK);

	FaultMuxRegs.DCOMPCTRL0.all |= (DCOMP_COMP_EN_MASK | DCOMP_INT_EN_MASK);
	FaultMuxRegs.DCOMPCTRL1.all |= (DCOMP_COMP_EN_MASK | DCOMP_INT_EN_MASK);
}
*/
inline void burst_mode_handler(void)
{

    if (fiq_data.fault_status_raw & DCOMP0_INT_STATUS_MASK) // && (fiq_data.burst_mode_en == 1))//  && (fiq_data.vloop_filter < 1800000)) //2400000 is PWM and resonant threshold

    {
        MiscAnalogRegs.GLBIOEN.all |=  (DPWM0A_GLBIO_BIT_MASK | DPWM0B_GLBIO_BIT_MASK | DPWM1A_GLBIO_BIT_MASK | DPWM1B_GLBIO_BIT_MASK |
            DPWM2A_GLBIO_BIT_MASK | DPWM2B_GLBIO_BIT_MASK | DPWM3A_GLBIO_BIT_MASK | DPWM3B_GLBIO_BIT_MASK);

//        fiq_data.burst_mode_en = 0;

    }
    else if (fiq_data.fault_status_raw & DCOMP1_INT_STATUS_MASK)
    {
        MiscAnalogRegs.GLBIOEN.all &= ~(DPWM0A_GLBIO_BIT_MASK | DPWM0B_GLBIO_BIT_MASK  |
        								DPWM2A_GLBIO_BIT_MASK | DPWM2B_GLBIO_BIT_MASK | DPWM3A_GLBIO_BIT_MASK | DPWM3B_GLBIO_BIT_MASK);//

        {
            //Unfreeze the integrator.
            if (fiq_data.fault_status_raw & DCOMP2_INT_STATUS_MASK)
            {
              Filter0Regs.FILTERCTRL.bit.KI_STALL = 1;
              Filter0Regs.FILTERPRESET.all = fiq_data.burst_mode_int_jam_value + PRESET_EN_MASK + PRESET_REG_INT_MASK;
            }

            fiq_data.burst_mode_en = 1;
            sr_on =1;
            Filter0Regs.FILTERCTRL.bit.KI_STALL = 0;

        }
    }

    fiq_data.fault_status &= ~(DCOMP0_INT_STATUS_MASK | DCOMP1_INT_STATUS_MASK | DCOMP2_INT_STATUS_MASK);

}

inline void cc_handler(void)
{
	fiq_data.vloop_filter = (int32) (Filter0Regs.FILTERYNREAD.bit.YN);
	fiq_data.iloop_filter = (int32) (Filter1Regs.FILTERYNREAD.bit.YN);

	if	(fiq_data.cbc_enabled && (!fiq_data.cbc_detected) && (fiq_data.fault_status & ACOMPD_INT_STATUS_MASK))
	{
		if (fiq_data.cbc_counter < fiq_data.cbc_max)
		{
			fiq_data.cbc_counter++;
		}
		else
		{
			Filter0Regs.FILTERCTRL.bit.KI_STALL = 1;
			Filter0Regs.FILTERPRESET.all = fiq_data.cbc_voltage_loop_integrator_jam_value + PRESET_EN_MASK + PRESET_REG_INT_MASK;
			Filter1Regs.FILTERCTRL.bit.KI_STALL = 1;
			Filter1Regs.FILTERPRESET.all = fiq_data.cbc_current_loop_integrator_jam_value + PRESET_EN_MASK + PRESET_REG_INT_MASK;
			Filter1Regs.FILTERCTRL.bit.KI_STALL = 0;
			fiq_data.cc_detected = 0;
			fiq_data.cbc_detected = 1;
		}
	}
	else if (fiq_data.iloop_filter < (fiq_data.vloop_filter - 0x200))  
	{
		LoopMuxRegs.DPWMMUX.all |= CC_FILTER_DPWM_SELECT_BITS;

		Filter0Regs.FILTERCTRL.bit.KI_STALL = 1;
		fiq_data.cc_detected = 1;
		fiq_data.cbc_detected = 0;
		fiq_data.cbc_counter = 0;
	}
	else if (fiq_data.cc_detected && (fiq_data.iloop_filter > fiq_data.vloop_filter))
	{
		LoopMuxRegs.DPWMMUX.all &= ~CC_FILTER_DPWM_SELECT_BITS;

		fiq_data.cc_detected = 0;
		fiq_data.cbc_detected = 0;
		fiq_data.cbc_counter = 0;
	}
	else
	{
		fiq_data.cbc_counter = 0;
	}

}

#pragma INTERRUPT(fast_interrupt,FIQ)
void fast_interrupt(void)
{

volatile int temp;
	fiq_data.vloop_filter = (int32) (Filter0Regs.FILTERYNREAD.bit.YN);
	fiq_data.iloop_filter = (int32) (Filter1Regs.FILTERYNREAD.bit.YN);


   fiq_data.fault_status |= FaultMuxRegs.FAULTMUXINTSTAT.all;
   fiq_data.fault_status_raw = FaultMuxRegs.FAULTMUXRAWSTAT.all;
   fiq_data.dpwm_status = Dpwm0Regs.DPWMINT.all;
   //MiscAnalogRegs.GLBIOVAL.bit.DPWM2A_IO_VALUE = Filter0Regs.FILTERCTRL.bit.KI_STALL;
//  MiscAnalogRegs.GLBIOVAL.bit.DPWM2B_IO_VALUE = fiq_data.cbc_detected;
//  MiscAnalogRegs.GLBIOVAL.bit.DPWM2A_IO_VALUE = (fiq_data.fault_status & ACOMPD_INT_STATUS_MASK) >> 3;
   //MiscAnalogRegs.GLBIOVAL.bit.DPWM2B_IO_VALUE = (fiq_data.fault_status_raw & ACOMPD_INT_STATUS_MASK) >> 3;


   if (fiq_data.ll_en) // && (fiq_data.fault_status_raw & (DCOMP0_INT_STATUS_MASK | DCOMP1_INT_STATUS_MASK)))
   {
       burst_mode_handler();
      
   }
   else if((fiq_data.fault_status_raw & DCOMP2_INT_STATUS_MASK) && (supply_state == STATE_REGULATED))
   {
	   Filter0Regs.FILTERCTRL.bit.KI_STALL = 1;
	   Filter0Regs.FILTERPRESET.all = fiq_data.burst_mode_int_jam_value + PRESET_EN_MASK + PRESET_REG_INT_MASK;
	   Filter0Regs.FILTERCTRL.bit.KI_STALL = 0;
   }
	
	

   if (fiq_data.cpcc_en && (fiq_data.dpwm_status & DPWM_INT_STATUS))
   {
       cc_handler();
   }

	FaultMuxRegs.DCOMPCTRL0.bit.CNT_CLR=1;
	FaultMuxRegs.DCOMPCTRL0.bit.CNT_CLR=0;
	FaultMuxRegs.DCOMPCTRL1.bit.CNT_CLR=1;
	FaultMuxRegs.DCOMPCTRL1.bit.CNT_CLR=0;
	FaultMuxRegs.DCOMPCTRL2.bit.CNT_CLR=1;
	FaultMuxRegs.DCOMPCTRL2.bit.CNT_CLR=0;

   fiq_data.fault_status &= ~(ACOMPD_INT_STATUS_MASK | DCOMP0_INT_STATUS_MASK | DCOMP1_INT_STATUS_MASK | DCOMP2_INT_STATUS_MASK);
   temp= FaultMuxRegs.FAULTMUXINTSTAT.all;

}

