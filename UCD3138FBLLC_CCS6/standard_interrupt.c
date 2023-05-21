//###########################################################################
//
// FILE:    standard_interrupt.c
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

#define CALIB_TEMPERATURE (23)
#define TEMP_COUNT_AT_CALIB_TEMPERATURE (1856)
#define TEMP_COUNT_PER_DEG_C (104)
#define CBC_RECOVER_TIME_THRESHOLD (100)

void cpcc_adjustment(void)
{
	//constant power calculation.
	v_out = adc_values_avg.vo_sense;
	i_out = adc_values_avg.io_sense;

	//30 is arbitrarily chosen to prevent a divide by 0 in the subsequent
	//division.
	if (i_out < 30)
	{
		i_out = 30;
	}

	vref = (Uint32) (p_out / i_out);
	//(2.5/2^12)/(1.6/2^14) = 25600 * 2^-12; scaler fron adc to dac
	vdac = (vref * 25600) >> 12;    

	if (vdac > pmbus_dcdc_config_translated[0].vout_cmd)
	{
		vdac = (current_share_control_effort >> cs_int_exp);
		if (supply_state == STATE_CPCC)
		{
			configure_new_compensation(&filter0_pmbus_regs);
			supply_state = STATE_REGULATED;
			cpcc_count = 0;
		}
	}
	else if (supply_state == STATE_REGULATED)
	{
//		burst_mode_disable();
		configure_new_compensation(&filter0_cp_pmbus_regs);
		supply_state = STATE_CPCC;
	}
	FeCtrl0Regs.EADCDAC.bit.DAC_VALUE = vdac;
}

void poll_adc(void)
{
	if(AdcRegs.ADCSTAT.bit.ADC_INT == 1)
		//If the conversion is complete
	{
		//Collect raw ADC values
		adc_values.vo_sense 		= AdcRegs.ADCRESULT[0].bit.RESULT;
		adc_values.io_sense 		= AdcRegs.ADCRESULT[1].bit.RESULT;
		adc_values.temp 			= AdcRegs.ADCRESULT[2].bit.RESULT;
		adc_values.vin_mon 			= AdcRegs.ADCRESULT[3].bit.RESULT;
		adc_values.ishare 			= AdcRegs.ADCRESULT[4].bit.RESULT;
		adc_values.vo_ovp 			= AdcRegs.ADCRESULT[5].bit.RESULT;
		adc_values.ips 				= AdcRegs.ADCRESULT[6].bit.RESULT;
		adc_values.address 			= AdcRegs.ADCRESULT[7].bit.RESULT;
		adc_values.device_temp 		= AdcRegs.ADCRESULT[8].bit.RESULT;

		//Collect averaged ADC values
		adc_values_avg.vo_sense 	= AdcRegs.ADCAVGRESULT[0].bit.RESULT; //1752;
		adc_values_avg.io_sense	 	= AdcRegs.ADCAVGRESULT[1].bit.RESULT;
		adc_values_avg.temp 		= AdcRegs.ADCAVGRESULT[2].bit.RESULT;
		adc_values_avg.vin_mon  	= AdcRegs.ADCAVGRESULT[3].bit.RESULT;//1502;
		adc_values_avg.ishare   	= AdcRegs.ADCAVGRESULT[4].bit.RESULT;
		adc_values_avg.vo_ovp   	= AdcRegs.ADCAVGRESULT[5].bit.RESULT;
	}
	//Start a conversion
	AdcRegs.ADCCTRL.bit.SW_START = 1;  
	//Clear trigger, so we don't restart
	AdcRegs.ADCCTRL.bit.SW_START = 0;  
}

void average_adc_readings(void)
{
	//These variables contain additional resolution to improve the 
	//firmware based averaging filter performance.
	//The pole point of this IIR filter is configurable by the global variable
	//"scf."

	//Calculate the high resolution average.
	adc_values_avg.ips_hr 		  = (((adc_values_avg.ips_hr 		 + adc_values.ips        ) << adc_values_avg.adc_scale_factor) - adc_values_avg.ips_hr        ) >> adc_values_avg.adc_scale_factor;
	adc_values_avg.address_hr 	  = (((adc_values_avg.address_hr	 + adc_values.address    ) << adc_values_avg.adc_scale_factor) - adc_values_avg.address_hr    ) >> adc_values_avg.adc_scale_factor;
	adc_values_avg.device_temp_hr = (((adc_values_avg.device_temp_hr + adc_values.device_temp) << adc_values_avg.adc_scale_factor) - adc_values_avg.device_temp_hr) >> adc_values_avg.adc_scale_factor;
	//Calculate the 12 bit version of the average and store the result in the averaged ADC12
	//structure.
	adc_values_avg.ips 	 	   = adc_values_avg.ips_hr 	   	>> adc_values_avg.adc_scale_factor;
	adc_values_avg.address     = adc_values_avg.address_hr 	>> adc_values_avg.adc_scale_factor;
	adc_values_avg.device_temp = adc_values_avg.device_temp_hr >> adc_values_avg.adc_scale_factor;
}

void clear_faults(void)
{
	global_disable();
	global_enable();
	//This is necessary to make sure all interrupt status values are 
	//cleared.
	FAULTMUXINTSTAT_value =	FaultMuxRegs.FAULTMUXINTSTAT.all;
}

void transition_to_idle_state(void)
{
	supply_state = STATE_IDLE;
//	burst_mode_disable();
	//Disable DPWMs
	gpio_dpwm_off();
	Dpwm0Regs.DPWMINT.bit.PRD_INT_EN = 0;
	//Stall filter states so that no integrator wind up occurs while off.
	stop_filter_states();
	//Reset soft start ramp
	start_up_reset();
	//Initialize filter states.
	init_filter0_states();
	init_filter1_states();

	//Disable current sharing
	disable_current_sharing();

	//Clear faults
	clear_faults();
	//Reset the PMBus status word to the idle state values.
	pmbus_status_word = PMBUS_STATUS_WORD_HIGH_BYTE | PMBUS_STATUS_WORD_OFF | PMBUS_STATUS_WORD_POWER_GOOD_NOT;
	//Reset the fault status flags.
	fiq_data.fault_status = 0;
	//Turn off the alert LED.
	PMBusRegs.PMBCTRL3.bit.ALERT_EN = 0;
	//Turn off the failure indicator.
	MiscAnalogRegs.GLBIOVAL.all &= ~((Uint32)MASK_FAILURE);

	//Shut off the current share DPWM current source
	MiscAnalogRegs.CSCTRL.bit.DPWM_DUTY = 0;

	//Reset counter that delays operation of the current share algorithm for
	//a count limit defined by: master_time_limit.
	master_time_count = 0;
	//Reset the CPCC fault timer counter.
	cpcc_count = 0;

	ramp_complete = 0;

	configure_new_compensation(&filter0_start_up_pmbus_regs);
	filter_recover_time = 0;
	cs_recover_time = 0;
//	configure_ipri_cycle_by_cycle(CBC_DISABLE_VALUE);

	FaultMuxRegs.DCOMPCTRL0.bit.INT_EN = 0;
	FaultMuxRegs.DCOMPCTRL1.bit.INT_EN = 0;
	FaultMuxRegs.DCOMPCTRL2.bit.INT_EN = 0;

	FaultMuxRegs.DCOMPCTRL0.bit.COMP_EN = 0;
	FaultMuxRegs.DCOMPCTRL1.bit.COMP_EN = 0;
	FaultMuxRegs.DCOMPCTRL2.bit.COMP_EN = 0;

	fiq_data.burst_mode_en = 0;
	FaultMuxRegs.DCOMPCTRL0.bit.THRESH = /*((pmbus_dcdc_config_translated[0].vout_cmd + 0x8) >> 4) +*/ - fiq_data.burst_mode_v_hys;
	FaultMuxRegs.DCOMPCTRL1.bit.THRESH = /*((pmbus_dcdc_config_translated[0].vout_cmd + 0x8) >> 4)*/ fiq_data.burst_mode_v_hys;
	FaultMuxRegs.DCOMPCTRL2.bit.THRESH = 30;    //under voltage
	LoopMuxRegs.DPWMMUX.all &= ~CC_FILTER_DPWM_SELECT_BITS;
}

void control_sr_on_off(void)
{
	if (llc_sr_enable ^ previous_llc_sr_command)
	{
		if (llc_sr_enable)
		{
			//Turn on SR
			gpio_sr_on();
		}
		else
		{
			//Turn off SR
			gpio_sr_off();
		}
		previous_llc_sr_command = llc_sr_enable;
	}
}

void set_vout(void)
{
	FeCtrl0Regs.EADCDAC.bit.DAC_VALUE = pmbus_dcdc_config_translated[0].vout_cmd;
}

void handle_current_sharing_average(void)
{
	//Configure current souce PWM
	MiscAnalogRegs.CSCTRL.bit.DPWM_DUTY = (adc_values_avg.io_sense * 
		(MiscAnalogRegs.CSCTRL.bit.DPWM_PERIOD + error_zero) + (1 << 11)) >> 12;	
	//Calculate error
	local_error = adc_values_avg.ishare - adc_values_avg.io_sense + error_offset;

	//difference should be proportional to differences between two currents
	if(abs(local_error) > ishare_threshold)
	{
		//if current difference is bigger than threshold
		current_share_int_state = current_share_int_state + current_share_ki * local_error;
		current_share_control_effort = current_share_int_state + current_share_kp * local_error;

		//if result is over permissible maximum voltage
		if( current_share_control_effort >  (eadc_dac_max << cs_int_exp)) 
		{
			//pull it down to maximum
			current_share_control_effort = (eadc_dac_max << cs_int_exp); 
			current_share_int_state = current_share_control_effort;
		}
		//if result is under permissible minimim voltage
		else if(current_share_control_effort < (eadc_dac_min << cs_int_exp)) 
		{
			//pull it up to minimum
			current_share_control_effort = (eadc_dac_min << cs_int_exp); 
			current_share_int_state = current_share_control_effort;
		}
	}
	//if within threshold area, edge it gently back toward center of voltage range
	else 
	{
		//if well above target
		if(((current_share_control_effort >> cs_int_exp)  - eadc_dac_target) > ishare_center_threshold * 16) 
		{
			//pull it down
			current_share_control_effort = current_share_control_effort - 16; 
		}
		//if well below target
		else if(((current_share_control_effort >> cs_int_exp) - eadc_dac_target) < -ishare_center_threshold * 16) 
		{
			//pull it up
			current_share_control_effort = current_share_control_effort + 16; 
		}
		//Configure the integrator state for the next cycle.
		current_share_int_state = current_share_control_effort;
	}

	//Program the DAC value with all 14 bits.
	FeCtrl0Regs.EADCDAC.bit.DAC_VALUE = (current_share_control_effort >> cs_int_exp);
} 

void handle_current_sharing_master_slave(void)
{
	//Configure current souce PWM
	MiscAnalogRegs.CSCTRL.bit.DPWM_DUTY = (adc_values_avg.io_sense * (MiscAnalogRegs.CSCTRL.bit.DPWM_PERIOD + error_zero) + (1 << 11)) >> 12;	
	//Calculate error
	local_error = adc_values_avg.ishare - adc_values_avg.io_sense + error_offset;

	//Wait unit master_time_count is equal to master_time_limit before
	//current sharing. This timer may no longer be necessary.
	if (master_time_count < master_time_limit)
	{
		master_time_count++;
		master_state = 1;
		MiscAnalogRegs.CSCTRL.bit.TEST_MODE = 3;
	}
	else
	{
		//Be the master
		if (local_error < ishare_threshold_master_enable)
		{
			master_state = 1;
			MiscAnalogRegs.CSCTRL.bit.TEST_MODE = 3;
		}
		//Be the slave.
		else if (local_error > ishare_threshold_slave_enable)
		{
			master_state = 0;
			MiscAnalogRegs.CSCTRL.bit.TEST_MODE = 0;
		}
	}

	if((abs(local_error) > ishare_threshold_ms) & (!master_state))
	{
		//if current difference is bigger than threshold
		current_share_int_state = current_share_int_state + current_share_ki * local_error;
		current_share_control_effort = current_share_int_state + current_share_kp * local_error;

		//if result is over permissible maximum voltage
		if(current_share_control_effort >  (eadc_dac_max << cs_int_exp))
		{
			//pull it down to maximum
			current_share_control_effort = (eadc_dac_max << cs_int_exp); 
			current_share_int_state = current_share_control_effort;
		}
		//if result is under permissible minimim voltage
		else if(current_share_control_effort < (eadc_dac_target << cs_int_exp)) 
		{
			//pull it up to minimum
			current_share_control_effort = (eadc_dac_target << cs_int_exp);
			current_share_int_state = current_share_control_effort;
		}
	}
	else if (master_state) //if within threshold area, edge it gently back toward center of voltage range
	{
		//if well above target
		if(((current_share_control_effort >> cs_int_exp)  - eadc_dac_target) > ishare_center_threshold_ms * 16) 
		{
			//pull it down
			current_share_control_effort = current_share_control_effort - 16; 
		}
		//if well below target
		else if(((current_share_control_effort >> cs_int_exp) - eadc_dac_target) < -ishare_center_threshold_ms * 16) 
		{
			//pull it up
			current_share_control_effort = current_share_control_effort + 16; 
		}
		//Configure the integrator state for the next cycle.
		current_share_int_state = current_share_control_effort;
	}

	//Program the DAC value with all 14 bits.
	FeCtrl0Regs.EADCDAC.bit.DAC_VALUE = (current_share_control_effort >> cs_int_exp);
} 

void handle_standard_interrupt_global_tasks(void)
{
	//Collect ADC data
	poll_adc();
	//Average ADC readings;
	average_adc_readings();
	//Receive UART data
	uart_receive_data();
	//call pgood handler
	handle_pgood();
}

void handle_idle_state(void)
{
/*The following start up delay was added since the 12 V bias has a long delay before
presenting a valid 12 V to the gate drive circuitry. This delay insures that there is 
enough time for the bias to power up and stabilize prior to attempting any turn on.*/
	if (!start_up_delay_over)
	{
		if (count == count_end)
		{
			start_up_delay_over = 1;
		}
		else
		{
			count++;
		}
	}
	else if (PSON && ABOVE_VIN_ON_LIMIT)
	{
		FaultMuxRegs.DCOMPCTRL0.bit.INT_EN = 1;
		FaultMuxRegs.DCOMPCTRL1.bit.INT_EN = 1;
		FaultMuxRegs.DCOMPCTRL2.bit.INT_EN = 1;

		FaultMuxRegs.DCOMPCTRL0.bit.COMP_EN = 1;
		FaultMuxRegs.DCOMPCTRL1.bit.COMP_EN = 1;
		FaultMuxRegs.DCOMPCTRL2.bit.COMP_EN = 1;
		configure_ipri_cycle_by_cycle(pmbus_dcdc_config[0].iin_oc_fault_limit);


  		Filter0Regs.FILTERCTRL.bit.KI_STALL = 1;
	   	Filter0Regs.FILTERPRESET.all = 0 + PRESET_EN_MASK + PRESET_REG_INT_MASK;
	   	Filter0Regs.FILTERCTRL.bit.KI_STALL = 0;

		//Configure rise time
		configure_ton_rise();
		//Re-start filter states
		start_filter_states();
		//Enable DPWMs
		gpio_dpwm_on();
		Dpwm0Regs.DPWMINT.bit.PRD_INT_EN = 1;
		//Initiate soft start ramp 
		FeCtrl0Regs.RAMPCTRL.bit.FIRMWARE_START = 1; 
//		MiscAnalogRegs.GLBIOVAL.bit.DPWM2B_IO_VALUE = 1;
		//Change operating state
		supply_state = STATE_RAMP_UP;

	
	}
}

void handle_ramp_up_state(void)
{
	//Check status of the start up ramp
	ramp_complete = FeCtrl0Regs.RAMPSTAT.bit.RAMP_COMP_INT_STATUS;

	if ((!PSON) || BELOW_VIN_OFF_LIMIT)
	{
		//Check to see if PSON is still valid. If no go to idle.
		transition_to_idle_state();
	}
	else if (ramp_complete)
	{
//		MiscAnalogRegs.GLBIOVAL.bit.DPWM2B_IO_VALUE = 0;
		//Set DAC to final ramp value
		set_vout();
		//Clear status word
		pmbus_status_word = 0;
		//Initialize current share compensation
		current_share_int_state = (eadc_dac_target << cs_int_exp);
		current_share_control_effort = current_share_int_state;
		//Enable Current shareing
		enable_current_sharing();
		//Change operating state
		supply_state = STATE_REGULATED;
	}
}

void handle_ramp_down_state(void)
{

}

void handle_regulation_state(void)
{	
	//Control the state of the SR's
	control_sr_on_off();

	if (cs_recover_time < cs_recover_time_threshold)
	{
		cs_recover_time++;
	}

	if (filter_recover_time < filter_recover_time_threshold)
	{
		filter_recover_time++;
	}
	else if (filter_recover_time == filter_recover_time_threshold)
	{
		configure_new_compensation(&filter0_pmbus_regs);
//		configure_ipri_cycle_by_cycle(pmbus_dcdc_config[0].iin_oc_fault_limit);
		filter_recover_time++;
	}

	//Verify that the on command is still valid and that the input voltage
	//is large enough to operate.
	if ((!PSON) || BELOW_VIN_OFF_LIMIT)
	{
		transition_to_idle_state();
	}
	else 
	{
		//Check for and handle any faults
				handle_faults();
				//Check for and handle and warnings
				handle_warnings();
				if (fiq_data.ll_en && (filter_recover_time > filter_recover_time_threshold) && (adc_values.io_sense < fiq_data.burst_mode_i_turn_on))
				{
		            MiscAnalogRegs.GLBIOEN.all |=  (DPWM1A_GLBIO_BIT_MASK | DPWM1B_GLBIO_BIT_MASK);
		            fiq_data.burst_mode_en = 1;
		            if (fiq_data.vloop_filter < 1800000) //threshold PWM and resonant mode
		            {
//		                Filter0Regs.FILTERCTRL.bit.KI_STALL = 1;
//		                Filter0Regs.FILTERPRESET.all = fiq_data.burst_mode_int_jam_value + PRESET_EN_MASK + PRESET_REG_INT_MASK;
//		                Filter0Regs.FILTERCTRL.bit.KI_STALL = 0;
		            }
		        }
		        if((sr_on ==1) && (adc_values.io_sense > fiq_data.burst_mode_i_turn_off))//make sure all DPWMs are turned on.
		        {
		        	MiscAnalogRegs.GLBIOEN.all &= ~(DPWM0A_GLBIO_BIT_MASK | DPWM0B_GLBIO_BIT_MASK  | DPWM1A_GLBIO_BIT_MASK | DPWM1B_GLBIO_BIT_MASK |
		        	        						DPWM2A_GLBIO_BIT_MASK | DPWM2B_GLBIO_BIT_MASK | DPWM3A_GLBIO_BIT_MASK | DPWM3B_GLBIO_BIT_MASK);//
		            sr_on = 0;
					fiq_data.burst_mode_en =0;

		        }
		        //Any time the unit is in the "Regulation State" and burst mode should be off execute the following code.
		        //This includes when the unit has just exited from a constant current or constant power state.
		        if (!fiq_data.ll_en || (adc_values.io_sense > fiq_data.burst_mode_i_turn_off))
		        {

		            Filter0Regs.FILTERCTRL.bit.KI_STALL = 0;
//			        fiq_data.burst_mode_en = 0;
		        }

		//Adjust constant power threshold
		if (pmbus_dcdc_config[0].cpcc_enable)
		{
			cpcc_adjustment();
		}
		//Perform current sharing
		if (pmbus_dcdc_config[0].ishare_enable && (cs_recover_time == cs_recover_time_threshold))
		{
			handle_current_sharing_average();
		}
	}
}

void handle_vout_transition_state(void)
{
	//Read the ramp complete status bit. It is clear on read.
	ramp_complete = FeCtrl0Regs.RAMPSTAT.bit.RAMP_COMP_INT_STATUS;

	//Check to see if the turn on command is still valid.
	//If not transition to the idle state.
	if ((!PSON) || BELOW_VIN_OFF_LIMIT)
	{
		transition_to_idle_state();
	}
	else if (ramp_complete)
	{
		//Set DAC to final ramp value
		set_vout();
		//Initialize current share compensation
		current_share_int_state = (eadc_dac_target << cs_int_exp);
		current_share_control_effort = current_share_int_state;
		supply_state = STATE_REGULATED;
	}
}

void handle_light_load_state(void)
{

}

void handle_cpcc_state(void)
{
	//Check to see if the turn on command is still valid and that there is
	//sufficient input voltage to continue to operate.
	//If not transition to the idle state.
	if ((!PSON) || BELOW_VIN_OFF_LIMIT)
	{
		transition_to_idle_state();
	}
	else{
		//If the constant power constant current fault timer has been enabled
		//increment the CPCC fault counter by 1.
		if (pmbus_dcdc_config[0].cpcc_time_out_en)
		{
			cpcc_count++;
		}
		//Check for and handle any faults
		handle_faults();
		//Check for and handle and warnings
		handle_warnings();
		//Check to see if the counter has exceeded the programmed threshold.
		//If it has turn off the DPWM set the status word, alert pin and change the state.
		if (cpcc_count > cpcc_ton_fault_time_limit)
		{
			gpio_dpwm_off();		
			pmbus_status_word |= PMBUS_STATUS_WORD_IOUT_OC | PMBUS_STATUS_WORD_OFF | PMBUS_STATUS_WORD_IOUT | PMBUS_STATUS_WORD_HIGH_BYTE;	
			PMBusRegs.PMBCTRL3.bit.ALERT_EN = 1;	
			disable_current_sharing();	
			supply_state = STATE_FAULT;
		}
		else
		{
			cpcc_adjustment();
		}
	}
}

void handle_hiccup_state(void)
{
	transition_to_idle_state();
}

void handle_fault_state(void)
{
	//Check to see if the turn on command is still valid.
	//If not transition to the idle state.
	if(!PSON)
	{
		transition_to_idle_state();
	}
}

#pragma INTERRUPT(standard_interrupt,IRQ)
void standard_interrupt(void)
{
	//Set a GPIO to signify the begining of the interrupt
//	MiscAnalogRegs.GLBIOVAL.bit.DPWM2A_IO_VALUE = 1;

	//Perform general tasks.
	handle_standard_interrupt_global_tasks();
	if(fiq_data.ll_en) // firmware burst mode enabled
	{
		LoopMuxRegs.LLCTRL.bit.LL_EN = 0;
	}
	else //hardware burst mode enabled
	{
		LoopMuxRegs.LLCTRL.bit.LL_EN = 1;
	}

	switch (supply_state)
	{
	case STATE_IDLE:
		handle_idle_state();
		break;
	case STATE_RAMP_UP:
		handle_ramp_up_state();
		break;
	case STATE_RAMP_DOWN:
		handle_ramp_down_state();
		break;
	case STATE_REGULATED:
		handle_regulation_state();
		break;
	case STATE_VOUT_TRANSITION:
		handle_vout_transition_state();
		break;
	case STATE_LIGHT_LOAD:
		handle_light_load_state();
		break;
	case STATE_CPCC:
		handle_cpcc_state();
		break;
	case STATE_HICCUP:
		handle_hiccup_state();
		break;
	case STATE_FAULT:
		handle_fault_state();
		break;
	}

	//clear interrupt flag by a read/write to register.
	TimerRegs.T16PWM2CMPCTRL.all = 3; 
	//Clear the GPIO to signify the end of the interrupt
//	MiscAnalogRegs.GLBIOVAL.bit.DPWM2A_IO_VALUE = 0;
}
