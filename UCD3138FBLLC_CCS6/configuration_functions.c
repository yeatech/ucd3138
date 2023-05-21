
//###########################################################################
//
// FILE:    configuration_functions.c
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

//#define EADC_DAC_TARGET ((int32)(pmbus_dcdc_config[0].vout_cmd * DAC_VALUE_SCALER >> VOUT_MODE_EXP)) //used to be (393 << EADC_DAC_SHIFT)
//#define EADC_DAC_MAX ((int32)((float)(pmbus_dcdc_config[0].vout_cmd * DAC_VALUE_SCALER >> VOUT_MODE_EXP)* ((float)1.01)))
//#define EADC_DAC_MIN ((int32)((float)(pmbus_dcdc_config[0].vout_cmd * DAC_VALUE_SCALER >> VOUT_MODE_EXP)* ((float)0.99)))
void configure_vout_cmd(void)
{
	struct qnote dac_value_scaler = {DAC_VALUE_SCALER, 0};
	pmbus_dcdc_config_translated[0].vout_cmd = qnote_linear16_multiply_fit(dac_value_scaler, pmbus_dcdc_config[0].vout_cmd,-VOUT_MODE_EXP,	MAX_VALUE_FIT_14_BITS);
	configure_cs_limits();
}

void configure_vout_timing(void)
{
	struct qnote constant_3_73555 = {30602 , -13};
	struct qnote tran_scaler;
	struct qnote slew_rate_qnote;
	struct qnote ton;
	struct qnote vcmd;
	Uint32 ramp_step;
	Uint16 slew_rate;

	vcmd.mantissa = pmbus_dcdc_config[0].vout_cmd;
	vcmd.exponent = -VOUT_MODE_EXP;

	ton = linear11_to_qnote(pmbus_dcdc_config[0].ton_rise);

	slew_rate_qnote.mantissa = ((Uint32)(vcmd.mantissa << TON_RISE_EXP)) / ((Uint32)ton.mantissa);
	slew_rate_qnote.exponent = vcmd.exponent - ton.exponent - TON_RISE_EXP;

	slew_rate = qnote_to_linear11(slew_rate_qnote);
	tran_scaler = qnote_scale_int32(constant_3_73555, pmbus_dcdc_config[0].sample_rate);

		ramp_step = qnote_linear11_multiply_fit(tran_scaler, slew_rate, MAX_VALUE_FIT_18_BITS);
	pmbus_dcdc_config_translated[0].ton_rise = ramp_step;
	ramp_step = qnote_linear11_multiply_fit(tran_scaler, pmbus_dcdc_config[0].vout_transition_rate,	MAX_VALUE_FIT_18_BITS);
	pmbus_dcdc_config_translated[0].vout_transition_rate = ramp_step;
}
 
void configure_cs_limits(void)
{
	struct qnote dac_value_scaler_max = {DAC_VALUE_SCALER + 9, 0};
	struct qnote dac_value_scaler_min = {DAC_VALUE_SCALER - 9, 0};
	eadc_dac_target = pmbus_dcdc_config_translated[0].vout_cmd;
	eadc_dac_max 	= qnote_linear16_multiply_fit(dac_value_scaler_max, pmbus_dcdc_config[0].vout_cmd, -VOUT_MODE_EXP, MAX_VALUE_FIT_14_BITS);
	eadc_dac_min 	= qnote_linear16_multiply_fit(dac_value_scaler_min, pmbus_dcdc_config[0].vout_cmd, -VOUT_MODE_EXP, MAX_VALUE_FIT_14_BITS);
}

void configure_pgood_levels(void)
{
	struct qnote adc12_vout_scaler = {ADC12_VOUT_SCALER_MANTISSA, ADC12_VOUT_SCALER_EXPONENT};
	pmbus_dcdc_config_translated[0].pgood_on_limit  = qnote_linear16_multiply_fit(adc12_vout_scaler, pmbus_dcdc_config[0].pgood_on_limit, -VOUT_MODE_EXP, MAX_VALUE_FIT_12_BITS);
	pmbus_dcdc_config_translated[0].pgood_off_limit = qnote_linear16_multiply_fit(adc12_vout_scaler, pmbus_dcdc_config[0].pgood_off_limit, -VOUT_MODE_EXP, MAX_VALUE_FIT_12_BITS);
}

void configure_fault_levels(void)
{
	struct qnote adc12_vout_scaler = {ADC12_VOUT_SCALER_MANTISSA, ADC12_VOUT_SCALER_EXPONENT};
	struct qnote adc12_vin_scaler  = {ADC12_VIN_SCALER_MANTISSA,  ADC12_VIN_SCALER_EXPONENT}; 
	struct qnote adc12_temp_scaler = {ADC12_TEMP_SCALER_MANTISSA, ADC12_TEMP_SCALER_EXPONENT}; 
	pmbus_dcdc_config_translated[0].vout_uv_fault_limit = qnote_linear16_multiply_fit(adc12_vout_scaler, pmbus_dcdc_config[0].vout_uv_fault_limit, -VOUT_MODE_EXP, MAX_VALUE_FIT_12_BITS);
	pmbus_dcdc_config_translated[0].temp_ot_fault_limit = ADC12_TEMP_OFFSET - qnote_linear11_multiply_fit(adc12_temp_scaler, pmbus_dcdc_config[0].temp_ot_fault_limit, MAX_VALUE_FIT_12_BITS);
	pmbus_dcdc_config_translated[0].vin_ov_fault_limit  = qnote_linear11_multiply_fit(adc12_vin_scaler, pmbus_dcdc_config[0].vin_ov_fault_limit, MAX_VALUE_FIT_12_BITS);
	pmbus_dcdc_config_translated[0].vin_uv_fault_limit  = qnote_linear11_multiply_fit(adc12_vin_scaler, pmbus_dcdc_config[0].vin_uv_fault_limit, MAX_VALUE_FIT_12_BITS);
}

void configure_warning_levels(void)
{
	struct qnote adc12_vout_scaler = {ADC12_VOUT_SCALER_MANTISSA, ADC12_VOUT_SCALER_EXPONENT};
	struct qnote adc12_iout_scaler = {ADC12_IOUT_SCALER_MANTISSA, ADC12_IOUT_SCALER_EXPONENT};  
	struct qnote adc12_iin_scaler  = {ADC12_IIN_SCALER_MANTISSA,  ADC12_IIN_SCALER_EXPONENT};  
	struct qnote adc12_vin_scaler  = {ADC12_VIN_SCALER_MANTISSA,  ADC12_VIN_SCALER_EXPONENT}; 
	struct qnote adc12_temp_scaler = {ADC12_TEMP_SCALER_MANTISSA, ADC12_TEMP_SCALER_EXPONENT};
	pmbus_dcdc_config_translated[0].vout_ov_warn_limit = qnote_linear16_multiply_fit(adc12_vout_scaler, pmbus_dcdc_config[0].vout_ov_warn_limit, -VOUT_MODE_EXP, MAX_VALUE_FIT_12_BITS);
	pmbus_dcdc_config_translated[0].vout_uv_warn_limit = qnote_linear16_multiply_fit(adc12_vout_scaler, pmbus_dcdc_config[0].vout_uv_warn_limit, -VOUT_MODE_EXP, MAX_VALUE_FIT_12_BITS);
	pmbus_dcdc_config_translated[0].iout_oc_warn_limit = qnote_linear11_multiply_fit(adc12_iout_scaler, pmbus_dcdc_config[0].iout_oc_warn_limit, MAX_VALUE_FIT_12_BITS);
	pmbus_dcdc_config_translated[0].iin_oc_warn_limit  = qnote_linear11_multiply_fit(adc12_iin_scaler, pmbus_dcdc_config[0].iin_oc_warn_limit, MAX_VALUE_FIT_12_BITS);
	pmbus_dcdc_config_translated[0].temp_ot_warn_limit = ADC12_TEMP_OFFSET - qnote_linear11_multiply_fit(adc12_temp_scaler, pmbus_dcdc_config[0].temp_ot_warn_limit, MAX_VALUE_FIT_12_BITS);
	pmbus_dcdc_config_translated[0].vin_ov_warn_limit  = qnote_linear11_multiply_fit(adc12_vin_scaler, pmbus_dcdc_config[0].vin_ov_warn_limit, MAX_VALUE_FIT_12_BITS);
	pmbus_dcdc_config_translated[0].vin_uv_warn_limit  = qnote_linear11_multiply_fit(adc12_vin_scaler, pmbus_dcdc_config[0].vin_uv_warn_limit, MAX_VALUE_FIT_12_BITS);
}

void configure_vin_on_off_thresholds(void)
{
	struct qnote adc12_vin_scaler  = {ADC12_VIN_SCALER_MANTISSA,  ADC12_VIN_SCALER_EXPONENT}; 
	pmbus_dcdc_config_translated[0].vin_on_limit  = qnote_linear11_multiply_fit(adc12_vin_scaler, pmbus_dcdc_config[0].vin_on_limit,  MAX_VALUE_FIT_12_BITS);
	pmbus_dcdc_config_translated[0].vin_off_limit = qnote_linear11_multiply_fit(adc12_vin_scaler, pmbus_dcdc_config[0].vin_off_limit, MAX_VALUE_FIT_12_BITS);
}

void configure_constant_power(void)
{
	struct qnote pcmd;
	struct qnote ton;
	struct qnote pmax_to_adc_scaler = {16781, -1};

	ton  = linear11_to_qnote(pmbus_dcdc_config[0].cpcc_ton);
	pcmd = linear11_to_qnote(pmbus_dcdc_config[0].cpcc_pmax);
	p_out = (Uint64)(((int64)pmax_to_adc_scaler.mantissa * 
		(int64)pcmd.mantissa) >> (-pmax_to_adc_scaler.exponent - pcmd.exponent));

	//Calculate the maximum time to allow operation in CPCC to occur.
	if (ton.exponent > 0)
	{
		cpcc_ton_fault_time_limit = (ton.mantissa * 10) << ton.exponent;
	}
	else if (ton.exponent < 0)
	{
		cpcc_ton_fault_time_limit = (ton.mantissa * 10) >> ton.exponent;
	}
	else
	{
		cpcc_ton_fault_time_limit = ton.mantissa * 10;
	}
	fiq_data.cpcc_en = pmbus_dcdc_config[0].cpcc_enable;
}

void configure_uvp(void)
{
	Uint16 uvp_limit;
	struct qnote vdac_scaler = {DAC_OVP, -OVP_EXP};
	uvp_limit = qnote_linear16_multiply_fit(vdac_scaler , 
		pmbus_dcdc_config[0].vout_ov_fault_limit, -VOUT_MODE_EXP,
		MAX_VALUE_FIT_7_BITS);
	FaultMuxRegs.ACOMPCTRL0.bit.ACOMP_B_THRESH = uvp_limit;
}

void configure_ovp(void)
{
	Uint16 ovp_limit;
	struct qnote vdac_scaler = {DAC_OVP, -OVP_EXP};
	ovp_limit = qnote_linear16_multiply_fit(vdac_scaler , 
		pmbus_dcdc_config[0].vout_ov_fault_limit, -VOUT_MODE_EXP,
		MAX_VALUE_FIT_7_BITS);
	FaultMuxRegs.ACOMPCTRL0.bit.ACOMP_B_THRESH = ovp_limit;
}

void configure_iout_ocp(void)
{
	//This struct represents the floating point number: 1.81356
	struct qnote constant_1_81356 = {29713 , -14}; 
	FaultMuxRegs.ACOMPCTRL2.bit.ACOMP_E_THRESH = 
		qnote_linear11_multiply_fit(constant_1_81356, 
		pmbus_dcdc_config[0].iout_oc_fault_limit,
		MAX_VALUE_FIT_7_BITS);

	if (pmbus_dcdc_config[0].iout_oc_fault_response & IOUT_OC_FAULT_RESPONSE_BITS_MASK)
	{
		FaultMuxRegs.DPWM0FLTABDET.bit.ACOMP_E_EN = 1;
		FaultMuxRegs.DPWM1FLTABDET.bit.ACOMP_E_EN = 1;
		FaultMuxRegs.DPWM2FLTABDET.bit.ACOMP_E_EN = 1;
		FaultMuxRegs.DPWM3FLTABDET.bit.ACOMP_E_EN = 1;
	}
	else
	{
		FaultMuxRegs.DPWM0FLTABDET.bit.ACOMP_E_EN = 0;
		FaultMuxRegs.DPWM1FLTABDET.bit.ACOMP_E_EN = 0;
		FaultMuxRegs.DPWM2FLTABDET.bit.ACOMP_E_EN = 0;
		FaultMuxRegs.DPWM3FLTABDET.bit.ACOMP_E_EN = 0;
	}
}

void configure_ipri_cycle_by_cycle(Uint16 fault_limit) 
{
	//This struct represents the floating point number: 15.9746
	struct qnote constant_15_9746 = {32716, -11}; 

	if(fault_limit == CBC_DISABLE_VALUE)
	{
		FaultMuxRegs.DPWM0CLIM.bit.ACOMP_D_EN = 0;
		FaultMuxRegs.DPWM1CLIM.bit.ACOMP_D_EN = 0;

		Dpwm0Regs.DPWMCTRL0.bit.CBC_PWM_AB_EN = 0;
		Dpwm0Regs.DPWMAUTOMID.bit.CBC_PWM_AB_EN = 0;
		Dpwm0Regs.DPWMAUTOMAX.bit.CBC_PWM_AB_EN = 0;

		Dpwm1Regs.DPWMCTRL0.bit.CBC_PWM_AB_EN = 0;
		Dpwm1Regs.DPWMAUTOMID.bit.CBC_PWM_AB_EN = 0;
		Dpwm1Regs.DPWMAUTOMAX.bit.CBC_PWM_AB_EN = 0;

		fiq_data.cbc_enabled = 0;
	}
	else
	{
		FaultMuxRegs.DPWM0CLIM.bit.ACOMP_D_EN = 1;
		FaultMuxRegs.DPWM1CLIM.bit.ACOMP_D_EN = 1;
		FaultMuxRegs.DPWM2CLIM.bit.ACOMP_D_EN = 1;
		FaultMuxRegs.DPWM3CLIM.bit.ACOMP_D_EN = 1;

		Dpwm0Regs.DPWMCTRL0.bit.CBC_PWM_AB_EN = 1;
		Dpwm0Regs.DPWMAUTOMID.bit.CBC_PWM_AB_EN = 1;
		Dpwm0Regs.DPWMAUTOMAX.bit.CBC_PWM_AB_EN = 1;
	//Add by qys
		Dpwm2Regs.DPWMCTRL0.bit.CBC_PWM_AB_EN = 1;
		Dpwm2Regs.DPWMAUTOMID.bit.CBC_PWM_AB_EN = 1;
		Dpwm2Regs.DPWMAUTOMAX.bit.CBC_PWM_AB_EN = 1;

		Dpwm1Regs.DPWMCTRL0.bit.CBC_PWM_AB_EN = 1;
		Dpwm1Regs.DPWMAUTOMID.bit.CBC_PWM_AB_EN = 1;
		Dpwm1Regs.DPWMAUTOMAX.bit.CBC_PWM_AB_EN = 1;

		Dpwm3Regs.DPWMCTRL0.bit.CBC_PWM_AB_EN = 1;
		Dpwm3Regs.DPWMAUTOMID.bit.CBC_PWM_AB_EN = 1;
		Dpwm3Regs.DPWMAUTOMAX.bit.CBC_PWM_AB_EN = 1;

		fiq_data.cbc_enabled = 1;
	}

	//Analog comparator 3(AD04 pin)
	//Resolution is 312 mV/A, 2500/128 mV/LSB
	//312*128/2500 LSB/A ~ 16 LSB/A
	//Maximum current limit ~ 8 A
	FaultMuxRegs.ACOMPCTRL1.bit.ACOMP_D_THRESH = 
		qnote_linear11_multiply_fit(constant_15_9746, 
		pmbus_dcdc_config[0].iin_oc_fault_limit,
		MAX_VALUE_FIT_7_BITS);
}

void configure_cc_dac_value(void)
{
	int16 imax_value;
	struct qnote constant_22_430 = {22968, -10};
	imax_value = qnote_linear11_multiply_fit(constant_22_430, 
		pmbus_dcdc_config[0].cpcc_imax,
		MAX_VALUE_FIT_10_BITS) << 4;
	// Set Processor DAC to MAX_CURRENT value for Error calculation
	FeCtrl1Regs.EADCDAC.bit.DAC_VALUE = imax_value;
}

void configure_vout_ramp_rate(void)
{
	FeCtrl0Regs.DACSTEP.bit.DAC_STEP = pmbus_dcdc_config_translated[0].vout_transition_rate;
}

void configure_ton_rise(void)
{
	FeCtrl0Regs.EADCDAC.bit.DAC_VALUE = 0;
	FeCtrl0Regs.RAMPDACEND.bit.RAMP_DAC_VALUE = pmbus_dcdc_config_translated[0].vout_cmd;
	FeCtrl0Regs.DACSTEP.bit.DAC_STEP = pmbus_dcdc_config_translated[0].ton_rise;
}

void start_up_reset(void)
{
	FeCtrl0Regs.RAMPCTRL.bit.FIRMWARE_START = 0;
	FeCtrl0Regs.RAMPCTRL.bit.RAMP_EN = 0;
	FeCtrl0Regs.EADCDAC.bit.DAC_VALUE = 0;
	FeCtrl0Regs.RAMPCTRL.bit.RAMP_EN = 1;
}

void configure_dpwm_timing(void) // need to change to pmbus variables later
{
	//-------------------
	// Config DPWM timing
	//-------------------
    Dpwm0Regs.DPWMEV1.bit.EVENT1 = PWM0_EV1;       
    Dpwm0Regs.DPWMEV2.bit.EVENT2 = PWM0_EV2; 
    Dpwm0Regs.DPWMEV3.bit.EVENT3 = PWM0_EV3;       
    Dpwm0Regs.DPWMEV4.bit.EVENT4 = PWM0_EV4;
    Dpwm0Regs.DPWMBLKBBEG.all = PWM1_EV5;
    Dpwm0Regs.DPWMBLKBEND.all = PWM1_EV6;
                  
	Dpwm0Regs.DPWMCYCADJA.bit.CYCLE_ADJUST_A = (int16)(-DEADTIME4);
	Dpwm0Regs.DPWMCYCADJB.bit.CYCLE_ADJUST_B = (int16)(-DEADTIME4);
	Dpwm0Regs.DPWMPRD.bit.PRD = MINPER;

	Dpwm1Regs.DPWMEV1.bit.EVENT1 = PWM1_EV1;       
    Dpwm1Regs.DPWMEV2.bit.EVENT2 = PWM1_EV2;       
    Dpwm1Regs.DPWMEV3.bit.EVENT3 = PWM1_EV3;       
    Dpwm1Regs.DPWMEV4.bit.EVENT4 = PWM1_EV4;       
    Dpwm1Regs.DPWMBLKBBEG.all = PWM1_EV5;
    Dpwm1Regs.DPWMBLKBEND.all = PWM1_EV6;
	Dpwm1Regs.DPWMCYCADJA.bit.CYCLE_ADJUST_A = (int16)(-DEADTIME4);
	Dpwm1Regs.DPWMCYCADJB.bit.CYCLE_ADJUST_B = (int16)(-DEADTIME4);
	Dpwm1Regs.DPWMPRD.bit.PRD = MINPER; 

    Dpwm2Regs.DPWMEV1.bit.EVENT1 = PWM2_EV1;       
    Dpwm2Regs.DPWMEV2.bit.EVENT2 = PWM2_EV2;       
    Dpwm2Regs.DPWMEV3.bit.EVENT3 = PWM2_EV3;       
    Dpwm2Regs.DPWMEV4.bit.EVENT4 = PWM2_EV4;             
    Dpwm2Regs.DPWMBLKBBEG.all = PWM2_EV5;
    Dpwm2Regs.DPWMBLKBEND.all = PWM2_EV6;
	Dpwm2Regs.DPWMCYCADJA.bit.CYCLE_ADJUST_A = (int16)(-DEADTIME2);
	Dpwm2Regs.DPWMCYCADJB.bit.CYCLE_ADJUST_B = (int16)(-DEADTIME2);
	Dpwm2Regs.DPWMPRD.bit.PRD = MINPER;

    Dpwm3Regs.DPWMEV1.bit.EVENT1 = PWM3_EV1;       
    Dpwm3Regs.DPWMEV2.bit.EVENT2 = PWM3_EV2;       
    Dpwm3Regs.DPWMEV3.bit.EVENT3 = PWM3_EV3;       
    Dpwm3Regs.DPWMEV4.bit.EVENT4 = PWM3_EV4;             
	Dpwm3Regs.DPWMCYCADJA.bit.CYCLE_ADJUST_A = (int16)(-DEADTIME2);
	Dpwm3Regs.DPWMCYCADJB.bit.CYCLE_ADJUST_B = (int16)(-DEADTIME2);
	Dpwm3Regs.DPWMPRD.bit.PRD = MINPER; 

	//----------------------
	// Config Mode Switching
 	//----------------------
	Dpwm0Regs.DPWMAUTOSWHIUPTHRESH.bit.AUTO_SWITCH_HIGH_UPPER = TSRMAX + ((DEADTIME3 + 8) >> 4);
	Dpwm0Regs.DPWMAUTOSWHILOWTHRESH.bit.AUTO_SWITCH_HIGH_LOWER = TSRMAX + ((DEADTIME3 + 8) >> 4);
	Dpwm0Regs.DPWMAUTOSWLOUPTHRESH.bit.AUTO_SWITCH_LOW_UPPER = HALF_PERIOD; 
	Dpwm0Regs.DPWMAUTOSWLOLOWTHRESH.bit.AUTO_SWITCH_LOW_LOWER = HALF_PERIOD;

	Dpwm1Regs.DPWMAUTOSWHIUPTHRESH.bit.AUTO_SWITCH_HIGH_UPPER = TSRMAX + ((DEADTIME3 + 8) >> 4);
	Dpwm1Regs.DPWMAUTOSWHILOWTHRESH.bit.AUTO_SWITCH_HIGH_LOWER = TSRMAX + ((DEADTIME3 + 8) >> 4);
	Dpwm1Regs.DPWMAUTOSWLOUPTHRESH.bit.AUTO_SWITCH_LOW_UPPER = HALF_PERIOD; 
	Dpwm1Regs.DPWMAUTOSWLOLOWTHRESH.bit.AUTO_SWITCH_LOW_LOWER = HALF_PERIOD;

	Dpwm2Regs.DPWMAUTOSWHIUPTHRESH.bit.AUTO_SWITCH_HIGH_UPPER = TSRMAX + ((DEADTIME3 + 8) >> 4);
	Dpwm2Regs.DPWMAUTOSWHILOWTHRESH.bit.AUTO_SWITCH_HIGH_LOWER = TSRMAX + ((DEADTIME3 + 8) >> 4);
	Dpwm2Regs.DPWMAUTOSWLOUPTHRESH.bit.AUTO_SWITCH_LOW_UPPER = HALF_PERIOD; 
	Dpwm2Regs.DPWMAUTOSWLOLOWTHRESH.bit.AUTO_SWITCH_LOW_LOWER = HALF_PERIOD;

  	Dpwm3Regs.DPWMAUTOSWHIUPTHRESH.bit.AUTO_SWITCH_HIGH_UPPER = TSRMAX + ((DEADTIME3 + 8) >> 4);
	Dpwm3Regs.DPWMAUTOSWHILOWTHRESH.bit.AUTO_SWITCH_HIGH_LOWER = TSRMAX + ((DEADTIME3 + 8) >> 4);
	Dpwm3Regs.DPWMAUTOSWLOUPTHRESH.bit.AUTO_SWITCH_LOW_UPPER = HALF_PERIOD; 
	Dpwm3Regs.DPWMAUTOSWLOLOWTHRESH.bit.AUTO_SWITCH_LOW_LOWER = HALF_PERIOD;

	//---------------------------
	// Config filter and loop-mux
 	//---------------------------
	LoopMuxRegs.DPWMMUX.bit.DPWM0_FILTER_SEL = 0;
	LoopMuxRegs.DPWMMUX.bit.DPWM1_FILTER_SEL = 0;
	LoopMuxRegs.DPWMMUX.bit.DPWM2_FILTER_SEL = 0;
	LoopMuxRegs.DPWMMUX.bit.DPWM3_FILTER_SEL = 0;

	LoopMuxRegs.FILTERKCOMPA.bit.KCOMP0 = MAXPER;
	
	// Connect to dpwm2
    LoopMuxRegs.DPWMMUX.bit.DPWM0_SYNC_SEL = 2; 
	// Connect to dpwm2
	LoopMuxRegs.DPWMMUX.bit.DPWM1_SYNC_SEL = 2;
	// Connect to dpwm2 
	LoopMuxRegs.DPWMMUX.bit.DPWM3_SYNC_SEL = 2; 

	Dpwm0Regs.DPWMRESDUTY.bit.RESONANT_DUTY = ((MAXPER + 1) >> 1);
	Dpwm1Regs.DPWMRESDUTY.bit.RESONANT_DUTY = ((MAXPER + 1) >> 1);
	Dpwm2Regs.DPWMRESDUTY.bit.RESONANT_DUTY = ((MAXPER + 1) >> 1);
	Dpwm3Regs.DPWMRESDUTY.bit.RESONANT_DUTY = ((MAXPER + 1) >> 1);

	Dpwm0Regs.DPWMCTRL2.bit.FILTER_DUTY_SEL = 2;
	Dpwm1Regs.DPWMCTRL2.bit.FILTER_DUTY_SEL = 2;
	Dpwm2Regs.DPWMCTRL2.bit.FILTER_DUTY_SEL = 2;
	Dpwm3Regs.DPWMCTRL2.bit.FILTER_DUTY_SEL = 2;

	//use dpwm3 period
	LoopMuxRegs.FILTERMUX.bit.FILTER0_PER_SEL = 3; 
	//use dpwm3 period
	LoopMuxRegs.FILTERMUX.bit.FILTER1_PER_SEL = 3; 

	Dpwm0Regs.DPWMCTRL2.bit.RESON_DEADTIME_COMP_EN = 1;
	Dpwm1Regs.DPWMCTRL2.bit.RESON_DEADTIME_COMP_EN = 1;
	Dpwm2Regs.DPWMCTRL2.bit.RESON_DEADTIME_COMP_EN = 1;
	Dpwm3Regs.DPWMCTRL2.bit.RESON_DEADTIME_COMP_EN = 1;

	//configure phase shift between DPWM modules
    Dpwm0Regs.DPWMPHASETRIG.all =  (MINPER * 8 + 32);
    Dpwm2Regs.DPWMPHASETRIG.all =  (MINPER * 8 + 32);

    LoopMuxRegs.FECTRL0MUX.bit.DPWM0_FRAME_SYNC_EN = 1;
    LoopMuxRegs.FECTRL1MUX.bit.DPWM1_FRAME_SYNC_EN = 1;

	Dpwm0Regs.DPWMCTRL0.bit.MIN_DUTY_MODE = 2;
	Dpwm1Regs.DPWMCTRL0.bit.MIN_DUTY_MODE = 2;
	Dpwm2Regs.DPWMCTRL0.bit.MIN_DUTY_MODE = 2;
	Dpwm3Regs.DPWMCTRL0.bit.MIN_DUTY_MODE = 2;

	Dpwm0Regs.DPWMMINDUTYLO.bit.MIN_DUTY_LOW = (DEADTIME3 + 8) >> 4;
	Dpwm0Regs.DPWMMINDUTYHI.bit.MIN_DUTY_HIGH = (DEADTIME3 + 8) >> 4;

	Dpwm1Regs.DPWMMINDUTYLO.bit.MIN_DUTY_LOW = 263; // use 180 for now //(DEADTIME3 + 8) >> 4;  
	Dpwm1Regs.DPWMMINDUTYHI.bit.MIN_DUTY_HIGH = 263; // use 180 for now //(DEADTIME3 + 8) >> 4;
	
	Dpwm2Regs.DPWMMINDUTYLO.bit.MIN_DUTY_LOW = (DEADTIME1 + 8) >> 4;
	Dpwm2Regs.DPWMMINDUTYHI.bit.MIN_DUTY_HIGH = (DEADTIME1 + 8) >> 4;

	Dpwm3Regs.DPWMMINDUTYLO.bit.MIN_DUTY_LOW = (DEADTIME1 + 8) >> 4;
	Dpwm3Regs.DPWMMINDUTYHI.bit.MIN_DUTY_HIGH = (DEADTIME1 + 8) >> 4;

	Dpwm0Regs.DPWMCTRL1.bit.GLOBAL_PERIOD_EN = 1;
	Dpwm1Regs.DPWMCTRL1.bit.GLOBAL_PERIOD_EN = 1;
	Dpwm2Regs.DPWMCTRL1.bit.GLOBAL_PERIOD_EN = 1;
	Dpwm3Regs.DPWMCTRL1.bit.GLOBAL_PERIOD_EN = 1;

	LoopMuxRegs.PWMGLBPER.bit.PRD = MINPER;

	Filter0Regs.FILTERYNCLPLO.all = 0xFFFF;
	Filter1Regs.FILTERYNCLPLO.all = 0xFFFF;

	Filter0Regs.FILTERCTRL.bit.PERIOD_MULT_SEL = 1; 
	Filter0Regs.FILTERCTRL.bit.OUTPUT_MULT_SEL = 3;
}

void burst_mode_enable(void)
{
	LoopMuxRegs.LLCTRL.bit.LL_EN = pmbus_dcdc_config[0].ll_en;
	burst_mode_enable_flag = 1;
}

void burst_mode_disable(void)
{
	LoopMuxRegs.LLCTRL.bit.LL_EN = 0;
	burst_mode_enable_flag = 0;
}

void configure_burst_mode(void)
{
	LoopMuxRegs.LLCTRL.bit.LL_EN = pmbus_dcdc_config[0].ll_en;
	LoopMuxRegs.LLENTHRESH.bit.TURN_ON_THRESH = pmbus_dcdc_config[0].ll_turn_on_thresh;
	LoopMuxRegs.LLDISTHRESH.bit.TURN_OFF_THRESH = pmbus_dcdc_config[0].ll_turn_off_thresh;
}

void copy_coefficients_to_filter(volatile struct FILTER_REGS *dest, const FILTER_PMBUS_REGS *source)
{
	dest->COEFCONFIG.all    	 = source->COEFCONFIG.all;
	dest->FILTERKPCOEF0.all 	 = source->FILTERKPCOEF0.all;
	dest->FILTERKPCOEF1.all 	 = source->FILTERKPCOEF1.all;
	dest->FILTERKICOEF0.all 	 = source->FILTERKICOEF0.all;
	dest->FILTERKICOEF1.all 	 = source->FILTERKICOEF1.all;
	dest->FILTERKDCOEF0.all 	 = source->FILTERKDCOEF0.all;
	dest->FILTERKDCOEF1.all  	 = source->FILTERKDCOEF1.all;
	dest->FILTERKDALPHA.all 	 = source->FILTERKDALPHA.all;
	dest->FILTERNL0.all     	 = source->FILTERNL0.all;
	dest->FILTERNL1.all     	 = source->FILTERNL1.all;
	dest->FILTERNL2.all     	 = source->FILTERNL2.all;
	dest->FILTERKICLPHI.all 	 = source->FILTERKICLPHI.all;
	dest->FILTERKICLPLO.all 	 = source->FILTERKICLPLO.all;
	dest->FILTERYNCLPHI.all 	 = source->FILTERYNCLPHI.all;
	dest->FILTERYNCLPLO.all 	 = source->FILTERYNCLPLO.all;
	dest->FILTEROCLPHI.all		 = source->FILTEROCLPHI.all;
	dest->FILTEROCLPLO.all		 = source->FILTEROCLPLO.all;
	dest->FILTERCTRL.bit.NL_MODE = source->FILTER_MISC.bit.NL_MODE;

	if(source == &filter1_pmbus_regs)
	{
		FeCtrl1Regs.EADCCTRL.bit.AFE_GAIN = source->FILTER_MISC.bit.AFE_GAIN;
	}
	else
	{
		FeCtrl0Regs.EADCCTRL.bit.AFE_GAIN = source->FILTER_MISC.bit.AFE_GAIN;
	}
}

void copy_coefficients_to_ram(volatile FILTER_PMBUS_REGS *dest, volatile struct FILTER_REGS *source)
{
	dest->COEFCONFIG.all 	  	  = source->COEFCONFIG.all;
	dest->FILTERKPCOEF0.all 	  = source->FILTERKPCOEF0.all;
	dest->FILTERKPCOEF1.all 	  = source->FILTERKPCOEF1.all;
	dest->FILTERKICOEF0.all 	  = source->FILTERKICOEF0.all;
	dest->FILTERKICOEF1.all 	  = source->FILTERKICOEF1.all;
	dest->FILTERKDCOEF0.all 	  = source->FILTERKDCOEF0.all;
	dest->FILTERKDCOEF1.all 	  = source->FILTERKDCOEF1.all;
	dest->FILTERKDALPHA.all 	  = source->FILTERKDALPHA.all;
	dest->FILTERNL0.all 		  = source->FILTERNL0.all;
	dest->FILTERNL1.all 	  	  = source->FILTERNL1.all;
	dest->FILTERNL2.all 		  = source->FILTERNL2.all;
	dest->FILTERKICLPHI.all 	  = source->FILTERKICLPHI.all;
	dest->FILTERKICLPLO.all 	  = source->FILTERKICLPLO.all;
	dest->FILTERYNCLPHI.all 	  = source->FILTERYNCLPHI.all;
	dest->FILTERYNCLPLO.all 	  = source->FILTERYNCLPLO.all;
	dest->FILTEROCLPHI.all		  = source->FILTEROCLPHI.all;
	dest->FILTEROCLPLO.all		  = source->FILTEROCLPLO.all;
	dest->FILTER_MISC.bit.NL_MODE = source->FILTERCTRL.bit.NL_MODE;
	
	if(dest == &filter1_pmbus_regs)
	{
		dest->FILTER_MISC.bit.AFE_GAIN = FeCtrl1Regs.EADCCTRL.bit.AFE_GAIN;
	}
	else
	{
		dest->FILTER_MISC.bit.AFE_GAIN = FeCtrl0Regs.EADCCTRL.bit.AFE_GAIN;
	}
}

void configure_new_compensation(volatile FILTER_PMBUS_REGS *source)
{
	volatile struct FILTER_REGS *dest;
	
	if(source == &filter1_pmbus_regs)
	{
		FeCtrl1Regs.EADCCTRL.bit.AFE_GAIN = source->FILTER_MISC.bit.AFE_GAIN;
		dest = &Filter1Regs;
	}
	else
	{
		FeCtrl0Regs.EADCCTRL.bit.AFE_GAIN = source->FILTER_MISC.bit.AFE_GAIN;
		dest = &Filter0Regs;
	}
	/*
	It is assumed that during normal operation:
	bin 0 always uses KP_COEF_0, KI_COEF_0, KD_COEF_0 and KD_ALPHA_0;
	bin 1 always uses KP_COEF_1, KI_COEF_1, KD_COEF_1 and KD_ALPHA_1.
	An exception to this is found in the following code during the transition from
	the old set of coefficients to the new ones.
	*/

	//Configure filter to use bin 0 coefficients only
	dest->COEFCONFIG.all = 0;

	//Configure bin 1 with the new bin 0 coefficients
	dest->FILTERKPCOEF0.bit.KP_COEF_1  = source->FILTERKPCOEF0.bit.KP_COEF_0;
	dest->FILTERKICOEF0.bit.KI_COEF_1  = source->FILTERKICOEF0.bit.KI_COEF_0;
	dest->FILTERKDCOEF0.bit.KD_COEF_1  = source->FILTERKDCOEF0.bit.KD_COEF_0;
	dest->FILTERKDALPHA.bit.KD_ALPHA_1 = source->FILTERKDALPHA.bit.KD_ALPHA_0;

	//Configure filter to use bin 1 coefficients only
	dest->COEFCONFIG.all = FILTER_CONFIG_BIN1;

	//Configure bin 0 with the new bin 0 coefficients
	dest->FILTERKPCOEF0.bit.KP_COEF_0  = source->FILTERKPCOEF0.bit.KP_COEF_0;
	dest->FILTERKICOEF0.bit.KI_COEF_0  = source->FILTERKICOEF0.bit.KI_COEF_0;
	dest->FILTERKDCOEF0.bit.KD_COEF_0  = source->FILTERKDCOEF0.bit.KD_COEF_0;
	dest->FILTERKDALPHA.bit.KD_ALPHA_0 = source->FILTERKDALPHA.bit.KD_ALPHA_0;

	//Configure filter to use bin 0 coefficients only
	dest->COEFCONFIG.all = 0;

	//Reprogram the remaining filter coefficients
	dest->FILTERKPCOEF0.all = source->FILTERKPCOEF0.all;
	dest->FILTERKPCOEF1.all = source->FILTERKPCOEF1.all;
	dest->FILTERKICOEF0.all = source->FILTERKICOEF0.all;
	dest->FILTERKICOEF1.all = source->FILTERKICOEF1.all;
	dest->FILTERKDCOEF0.all = source->FILTERKDCOEF0.all;
	dest->FILTERKDCOEF1.all = source->FILTERKDCOEF1.all;
	dest->FILTERKDALPHA.all = source->FILTERKDALPHA.all;

	//Configure filter to use bins as defined
	dest->COEFCONFIG.all = source->COEFCONFIG.all;
}

void configure_filter_parameters(void)
{
	copy_coefficients_to_filter(&Filter0Regs, &filter0_pmbus_regs);
	copy_coefficients_to_filter(&Filter1Regs, &filter1_pmbus_regs);
}

