//###########################################################################
//
// FILE:    system_defines.h
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
//system_defines.h

#define Cyclone2p0 (1) // Set if working with 2p0 version of the device

#define PRESET_EN_MASK (1 << 27)
#define PRESET_REG_INT_MASK (1 << 24)

#define DCOMP_INT_EN_MASK (1 << 12)
#define DCOMP_COMP_EN_MASK (1 << 11)

#define DPWM_INT_STATUS (1 << 16)

#define KI_STALL_MASK (1 << 6)

#define CBC_DISABLE_VALUE	(100)

#define FILTER_CONFIG_BIN1 (0x99999999)

#define MAX_FILTER_VALUE	(0x7FFFFF)
#define CC_FILTER_DPWM_SELECT_BITS	((1 << 9) | (1 << 6) | (1 << 3) | (1))
#define min(a,b)  (((a)>(b)) ? (b):(a))
#define max(a,b)  (((a)>(b)) ? (a):(b))

#define IOUT_OC_FAULT_RESPONSE_BITS_MASK    (0xC0)

#define TDPWM (250e-12)					//High resolution DPWM step size
#define TSAMP (1 / (100e3))					//EADC sample rate

#define TH (1 / (150e3))				//PWM mode period. This is also the maximum resonant frequency (resonant fre =83K)
#define TL (1 / (35e3))					//Lowest resonant mode period.
#define DT0 (350e-9)					//Primary MOSFET dead time
#define DT0SR (1050e-9)					//SR dead time
#define TSR (5.3e-6 + DT0SR)			//Maximum SR on time

#define DT1 (DT0 / 2)					//Primary switches dead time from start of period to EV1
#define DT2 (DT0)						//Primary switches dead time from EV2 to EV3
#define DT3 (DT0 / 2)					//Primary switches dead time from EV4 to end of period
#define DT1SR (DT0SR / 2)				//SR switches dead time from start of period to EV1
#define DT2SR (DT0SR)					//SR switches dead time from EV2 to EV3
#define DT3SR (DT0SR / 2)				//SR switches dead time from EV4 to end of period
#define DT (DT1 + DT2 + DT3) 			//Primary switches total dead time
#define DTSR (DT1SR + DT2SR + DT3SR)	//SR switches total dead time

#define VOUT (12.0)						//Target output voltage
#define TSS (20e-3)						//Target soft start time
#define VOUT_OVP (14)					//Target OVP level
#define IPRI_OCP (0.8)//0.65			//Target OCP for primary current -- Needs to be translated to Amperes
#define ENABLE (1)
#define DISABLE (0)

#define IMAX (35)

#define MASK_PSON (FAULT3_GLBIO_BIT_MASK )//(1 << 29)
#define MASK_PGOOD (SYNC_GLBIO_BIT_MASK) //(1 << 8)
#define MASK_FAILURE (ADC_EXT_TRIG_GLBIO_BIT_MASK) //(1 << 28)
#define MASK_AC_FAIL (TMR_PWM1_GLBIO_BIT_MASK) //(1 << 18)
#define MASK_ORING_CTRL (FAULT1_GLBIO_BIT_MASK)

#define GLBEN_BIT_MASK_DPWM0 	(1)
#define GLBEN_BIT_MASK_DPWM1 	(1 << 1)
#define GLBEN_BIT_MASK_DPWM2 	(1 << 2)
#define GLBEN_BIT_MASK_DPWM3 	(1 << 3)
#define GLBEN_BIT_MASK_FE0 		(1 << 8)
#define GLBEN_BIT_MASK_FE1 		(1 << 9)
#define GLBEN_BIT_MASK_FE2 		(1 << 10)

#define FILTER_VOLTAGE_LOOP_START_UP	 	(0)
#define FILTER_VOLTAGE_LOOP_STEADY_STATE 	(1)
#define FILTER_VOLTAGE_LOOP_CONSTANT_POWER	(2)
#define FILTER_CURRENT_LOOP_STEADY_STATE 	(3)

#define PSON ((MiscAnalogRegs.GLBIOREAD.all & (CONTROL_GLBIO_BIT_MASK | MASK_PSON)) && enable_turn_on)

#define TON_RISE_EXP (1)
/*
#define DAC_OCP_PRI (IPRI_OCP / 2.5 * ((1<<7) - 1.0) + 1)			//DAC value required to achieve Ipri_OCP
#define PMAX (int)(IMAX * VOUT * (float)(1 << 14) * (float)(1 << 12) * 1.08 * 54e-3 / 12.0 / 1.6 / 2.5)	//Firmware CP value
#define IPMAX (int)(PMAX / 6440)
#define ICCMAX (int)(25.0 * (float)(1 << 12) * 54e-3 / 2.5)
*/

//==========================================================================================
// Memory allocation constants
//==========================================================================================
// ****VOYAGER#define MFBALR2_HALF0_DATA_FLASH_BASE_ADDRESS 0x8800
#define DATA_FLASH_START_ADDRESS 	(0x18800)
#define DATA_FLASH_END_ADDRESS 		(0x18fff)
#define DATA_FLASH_LENGTH			(DATA_FLASH_END_ADDRESS - DATA_FLASH_START_ADDRESS + 1)
#define	DATA_FLASH_SEGMENT_SIZE		(32)
#define	DATA_FLASH_NUM_SEGMENTS		(DATA_FLASH_LENGTH / DATA_FLASH_SEGMENT_SIZE) 

// This limitation check is required because the flash access routines in flash.c make the
// assumption that the size of the segments of the data flash are an integral power of two.
#if (DATA_FLASH_SEGMENT_SIZE & (DATA_FLASH_SEGMENT_SIZE - 1)) != 0
#error "DATA_FLASH_SEGMENT_SIZE must be defined as an integral power of two"
#endif

#define MFBALR2_HALF0_DATA_FLASH_BASE_ADDRESS (0x8800)
#define MFBALRX_BYTE0_BLOCK_SIZE_2K     	(0x20) 		
#define MFBALRX_BYTE0_RONLY					(0x02)		   //Read-only protection
#define MFBALRX_BYTE0_BLOCK_SIZE_32K    	(0x60)
#define UART_RX_SIZE (10) //total number of bytes for received data packet
#define UART_TX_SIZE (10) //total number of bytes for transmitted data packet

// Flash Error codes
#define FLASH_SUCCESS 				(0)
#define FLASH_INVALID_SEGMENT 		(1)
#define	FLASH_INVALID_ADDRESS		(2)
#define	FLASH_MISCOMPARE			(3)

#define NULL						(0)
#define ROM_PASSWORD_BYTES      (4)
#define NUMBER_OF_FAULTY_TRIALS (5) // Number of time wrog password may be entered

#define DCDC_PAGE_NUMS (1)

// 912 = 16 * Round(1.065 / (12 * 1.6) * (1 << 10))
#define VOUT_MODE_EXP	 (9)
#define DAC_VALUE_SCALER (912)
#define VIN_REF (380)
/*
DAC0 * (1 << 11) * TSAMP / TSS calculates the DAC step required to achieve a
soft start time of TSS from 0V. Where TSS is in seconds and DAC0 is the 14 bit version. 
TSAMP is the sample rate of the EADC. For the LLC converter this has been fixed 
to 500ns independant of the switching frequency.

The factor of 1000 below is to convert the units from mV/us to V/s. DAC_VALUE_SCALER is included
to convert the voltage to DAC LSBs. The factor VOUT_MODE_EXP is not needed since 
the original units of the transition rate do not include it.
*/
#define VOUT_TRANSITION_EXP (9)
//#define VOUT_TRANSITION_RATE_SCALER (int32)(TSAMP * 1000 * (1 << (11 + VOUT_TRANSITION_EXP)) * DAC_VALUE_SCALER)

//((1 / 12.0 * 1.065 / 2.5 * ((1 << 7) - 1)) << OVP_EXP)
#define OVP_EXP (6)
#define DAC_OVP (289)

//These two together properly scale the VOUT from the ADC to the Literal value.
//((12 / 1.065) * 2.5 / 4096) << (16 + VOUT_MODE_EXP - VOUT_ADC_TO_LITERAL_SHIFT)
//The factor of 16 appears due to the unsigned_short_q_multiply function
#define VOUT_ADC_TO_LITERAL_SHIFT 	(3)
#define VOUT_ADC_TO_LITERAL_SCALER  (28845)

//Status Word aliases
#define PMBUS_STATUS_WORD_HIGH_BYTE			(0x1)
#define PMBUS_STATUS_WORD_CML 				(0x2)
#define PMBUS_STATUS_WORD_TEMPERATURE 		(0x4)
#define PMBUS_STATUS_WORD_VIN_UV 			(0x8)
#define PMBUS_STATUS_WORD_IOUT_OC 			(0x10)
#define PMBUS_STATUS_WORD_VOUT_OV 			(0x20)
#define PMBUS_STATUS_WORD_OFF 				(0x40)
#define PMBUS_STATUS_WORD_BUSY 				(0x80)
#define PMBUS_STATUS_WORD_UNKNOWN 			(0x100)
#define PMBUS_STATUS_WORD_OTHER 			(0x200)
#define PMBUS_STATUS_WORD_FAN	 			(0x400)
#define PMBUS_STATUS_WORD_POWER_GOOD_NOT 	(0x800)
#define PMBUS_STATUS_WORD_MFR   			(0x1000)
#define PMBUS_STATUS_WORD_INPUT 			(0x2000)
#define PMBUS_STATUS_WORD_IOUT  			(0x4000)
#define PMBUS_STATUS_WORD_VOUT  			(0x8000)

#define DCOMP0_INT_STATUS_MASK 	(1 << 13)
#define DCOMP1_INT_STATUS_MASK 	(1 << 14)
#define DCOMP2_INT_STATUS_MASK 	(1 << 15)
#define DCOMP3_INT_STATUS_MASK 	(1 << 16)

#define ACOMPA_INT_STATUS_MASK 	(0x01)
#define ACOMPB_INT_STATUS_MASK 	(0x02)
#define ACOMPC_INT_STATUS_MASK 	(0x04)
#define ACOMPD_INT_STATUS_MASK 	(0x08)
#define ACOMPE_INT_STATUS_MASK 	(0x10)
#define ACOMPF_INT_STATUS_MASK 	(0x20)
#define ACOMPG_INT_STATUS_MASK 	(0x40)
#define COMP_INT_STATUS_MASK	(ACOMPB_INT_STATUS_MASK | ACOMPE_INT_STATUS_MASK)
#define OVER_VOLTAGE_FAULT 		(ACOMPB_INT_STATUS_MASK & fiq_data.fault_status)
#define OVER_CURRENT_SEC_FAULT	(ACOMPE_INT_STATUS_MASK & fiq_data.fault_status)

#define CS_INT_MAN (1)
#define KI_STALL_MASK (1 << 6)
#define KD_STALL_MASK (1 << 7)
#define DPWM_FAULT_AB_STATUS_MASK (0x2)

#define MAX_VALUE_FIT_5_BITS  (0x0001F)
#define MAX_VALUE_FIT_6_BITS  (0x0003F)
#define MAX_VALUE_FIT_7_BITS  (0x0007F)
#define MAX_VALUE_FIT_8_BITS  (0x000FF)
#define MAX_VALUE_FIT_9_BITS  (0x001FF)
#define MAX_VALUE_FIT_10_BITS (0x003FF)
#define MAX_VALUE_FIT_11_BITS (0x007FF)
#define MAX_VALUE_FIT_12_BITS (0x00FFF)
#define MAX_VALUE_FIT_13_BITS (0x01FFF)
#define MAX_VALUE_FIT_14_BITS (0x03FFF)
#define MAX_VALUE_FIT_15_BITS (0x07FFF)
#define MAX_VALUE_FIT_16_BITS (0x0FFFF)
#define MAX_VALUE_FIT_17_BITS (0x1FFFF)
#define MAX_VALUE_FIT_18_BITS (0x3FFFF)
#define MAX_VALUE_FIT_19_BITS (0x7FFFF)
#define MAX_VALUE_FIT_20_BITS (0xFFFFF)

#define qnote_int16_multiply_fit(x_qnote, var_int16, max_value) qnote_linear16_multiply_fit(x_qnote, var_int16, 0, max_value)

#define ABOVE_VIN_ON_LIMIT   (adc_values_avg.vin_mon > pmbus_dcdc_config_translated[0].vin_on_limit)
#define BELOW_VIN_OFF_LIMIT  (adc_values_avg.vin_mon < pmbus_dcdc_config_translated[0].vin_off_limit)

#define OVOUT_WARN 	 (adc_values_avg.vo_sense  > pmbus_dcdc_config_translated[0].vout_ov_warn_limit)
#define UVOUT_WARN 	 (adc_values_avg.vo_sense  < pmbus_dcdc_config_translated[0].vout_uv_warn_limit)
#define OCOUT_WARN 	 (adc_values_avg.io_sense  > pmbus_dcdc_config_translated[0].iout_oc_warn_limit)
#define OCIN_WARN 	 (adc_values_avg.ips 	   > pmbus_dcdc_config_translated[0].iin_oc_warn_limit)
#define OT_WARN 	 (adc_values_avg.temp 	   < pmbus_dcdc_config_translated[0].temp_ot_warn_limit)
#define OVVIN_WARN 	 (adc_values_avg.vin_mon   > pmbus_dcdc_config_translated[0].vin_ov_warn_limit)
#define UVVIN_WARN 	 (adc_values_avg.vin_mon   < pmbus_dcdc_config_translated[0].vin_uv_warn_limit)

#define UVOUT_FAULT  ((adc_values_avg.vo_sense  < pmbus_dcdc_config_translated[0].vout_uv_fault_limit) && (supply_state == STATE_REGULATED))
#define OT_FAULT 	 (adc_values_avg.temp 	   < pmbus_dcdc_config_translated[0].temp_ot_fault_limit)
#define OVVIN_FAULT  (adc_values_avg.vin_mon   > pmbus_dcdc_config_translated[0].vin_ov_fault_limit)
#define UVVIN_FAULT  (adc_values_avg.vin_mon   < pmbus_dcdc_config_translated[0].vin_uv_fault_limit)

#define PGOOD_ON     (adc_values_avg.vo_sense  > pmbus_dcdc_config_translated[0].pgood_on_limit)
#define PGOOD_OFF    (adc_values_avg.vo_sense  < pmbus_dcdc_config_translated[0].pgood_off_limit)

#define ADC12_VOUT_SCALER_MANTISSA  (18587)
#define	ADC12_VOUT_SCALER_EXPONENT  (-7)
#define ADC12_IOUT_SCALER_MANTISSA  (29947)
#define ADC12_IOUT_SCALER_EXPONENT  (-9)
#define ADC12_IIN_SCALER_MANTISSA   (32716)
#define ADC12_IIN_SCALER_EXPONENT   (-6)
#define ADC12_VIN_SCALER_MANTISSA   (32380)
#define ADC12_VIN_SCALER_EXPONENT   (-12)
#define ADC12_TEMP_SCALER_MANTISSA  (19623)
#define ADC12_TEMP_SCALER_EXPONENT  (-10)
#define ADC12_TEMP_OFFSET 			(3058)

//Add by Jamie
#define AVERAGE_Vout_SHIFT (11)
#define AVERAGE_IOUT_SHIFT (11)
