//###########################################################################
//
// FILE:    variables.h
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
#ifdef MAIN 					// If it is the main routine
	#define EXTERN	 			// If it isn't extern, so define it as nothing
#else 							// If it isn't the main routine
	#define EXTERN extern 		// If it is extern
#endif

typedef void (*FUNC_PTR)(); 	//used for zeroing instruction word.

typedef struct  
{
	Uint32 address;	//Value from adc for device address
	Uint32 vin_mon; //Value from adc for Vin 
	Uint32 ishare; 	//Value from adc for Ishare
	Uint32 vo_sense; //Value from adc for Vout
	Uint32 ips; 	//Value from adc for primary current sense
	Uint32 vo_ovp; 	//Value from adc for Vin  
	Uint32 temp;	//Value from adc for SR MOSFET temperature
	Uint32 io_sense; //Value from adc for Iout
	Uint32 device_temp;	//Value from adc for internal device temperature
	Uint32 ips_hr;
	Uint32 address_hr;
	Uint32 device_temp_hr;
	Uint32 adc_scale_factor;
} ADC_VALUES_STRUCT;

struct PFC_OUT_STRUCT
{
	unsigned char pfc_status0;
    unsigned char pfc_status1;
    unsigned int  pfc_vac;
    unsigned int  pfc_iac;
    unsigned int  pfc_pin;
	unsigned int  pfc_pout;
    unsigned int  pfc_vdc_bus;
    unsigned int  pfc_sw_frequency;
    unsigned char pfc_temperature;
};

typedef struct
{
	Uint16 burst_mode_i_turn_on;
	Uint16 burst_mode_i_turn_off;
	int16 burst_mode_v_hys;
	Uint32 burst_mode_int_jam_value;
	Uint8  burst_mode_en;
	int32 vloop_filter;
	int32 iloop_filter;
	Uint8 cbc_enabled;
	Uint8 cc_detected;
	Uint8 cbc_detected;
	Uint32 cbc_current_loop_integrator_jam_value;
	Uint32 cbc_voltage_loop_integrator_jam_value;
	Uint32 dpwm_status;
	Uint32 fault_status;
	Uint32 fault_status_raw;
	Uint8 ll_en;
	Uint8 cpcc_en;
	Uint8 cbc_counter;
	Uint8 cbc_max;
} FIQ_DATA;

typedef enum
{
	STATE_IDLE,			
	STATE_RAMP_UP,		
	STATE_RAMP_DOWN,	
	STATE_REGULATED,	
	STATE_LIGHT_LOAD,	
	STATE_CPCC,	
	STATE_FAULT,
	STATE_HICCUP,
	STATE_VOUT_TRANSITION		
} SUPPLY_STATE;

EXTERN FIQ_DATA fiq_data; 

EXTERN Uint32 xn_bins[64]; 				//xn histogram bins
EXTERN int loop_counter;				//Counter for tracking the number of samples to collect
EXTERN int number_of_samples;			//Total number of samples to collect
EXTERN int start_monitor;				//RAM variable to start xn data collection
EXTERN int ramp_complete;				//Value stored in FeCtrl0Regs.RAMPSTAT.bit.RAMP_COMP_INT_STATUS
EXTERN ADC_VALUES_STRUCT adc_values; 	//ADC Readings
EXTERN ADC_VALUES_STRUCT adc_values_avg;//ADC Readings Averaged
EXTERN SUPPLY_STATE supply_state; 		//Supply state enum for state machine

EXTERN Uint8 uart_text_rx_buf[UART_RX_SIZE*2 + 2]; //UART receive buffer in text mode
EXTERN Uint8 uart_text_tx_buf[UART_TX_SIZE*2 + 2]; //UART transmit buffer in text mode
EXTERN Uint8 uart_rx_buf[UART_RX_SIZE]; //UART receive buffer
EXTERN Uint8 uart_tx_buf[UART_TX_SIZE]; //UART transmit buffer
EXTERN Uint8 uart_rx_data_rdy; //flag, received a new data packet
EXTERN Uint8 uart_tx_data_rdy; //flag, a new data packet is ready for transmit
EXTERN Uint8 uart_rx_buf_ptr; //point to the buffer which will store the coming byte
EXTERN Uint8 uart_tx_buf_ptr; //point to buffer whose data is going to be sent out 
EXTERN Uint16 uart_rx_timeout; //count IRQ, UART receiver will start over when timeout
EXTERN Uint16 uart_tx_timeout; //count IRQ, UART needs to wait for a certain period before send the next data packet

EXTERN struct PFC_OUT_STRUCT pfc_out_struct;

EXTERN Uint8 pfc_command;//for APEC demo
EXTERN Uint8 pfc_phase_2_enable;//for APEC demo
EXTERN Uint8 pfc_zvs_enable;//for APEC demo
EXTERN Uint8 pfc_os_enable;//for APEC demo
EXTERN Uint8 llc_sr_enable;//for APEC demo
EXTERN Uint8 previous_llc_sr_command;

EXTERN Uint8 erase_segment_counter;	// Number of DFlash segment remaining to be erased
EXTERN Uint8 erase_segment_number;		// DFlash segment number being erased
EXTERN Uint8 flash_write_status;	// Global status while attempting to write to Data Flash.

EXTERN Uint16 period;



struct qnote
{
  int16 mantissa;
  int16 exponent;
};

EXTERN struct qnote temp_qnote1;
EXTERN struct qnote temp_qnote_scale1;
EXTERN struct qnote adc12_vin_scaler;
EXTERN int16 temp_qnote_value1;
EXTERN Uint32 rogue_errors;

EXTERN int32 current_share_int_state; 
EXTERN int32 current_share_control_effort;
EXTERN int16 current_share_kp;
EXTERN int16 current_share_ki;
EXTERN int32 error_zero;
EXTERN int32 local_error;
EXTERN int32 ishare_center_threshold;
EXTERN int32 ishare_threshold;
EXTERN int32 ishare_threshold_ms;
EXTERN int32 ishare_center_threshold_ms;
EXTERN int32 ishare_threshold_master_enable;
EXTERN int32 ishare_threshold_slave_enable;
EXTERN int32 eadc_dac_target;
EXTERN int32 eadc_dac_max;
EXTERN int32 eadc_dac_min;
EXTERN int32 master_state;
EXTERN int32 master_time_count;
EXTERN int32 master_time_limit;
EXTERN int8  enable_turn_on;

EXTERN Uint32 pnom_value;
EXTERN Uint32 cpcc_count;
EXTERN int16  fault_and_warning_delay_count;

EXTERN Uint32 cpcc_ton_fault_time_limit;
EXTERN Uint32 cs_recover_time;
EXTERN Uint32 cs_recover_time_threshold;
EXTERN Uint32 filter_recover_time_threshold;
EXTERN Uint32 filter_recover_time;
EXTERN volatile Uint32 FAULTMUXINTSTAT_value;
EXTERN union CPSTAT_REG cpstat_local;

EXTERN Uint32 restart_counter;

EXTERN Uint64 p_out;
EXTERN Uint32 v_out;
EXTERN Uint32 i_out;
EXTERN Uint32 vref;
EXTERN Uint32 vdac;
EXTERN int32  error_offset;
EXTERN Uint8 cs_int_exp;
EXTERN Uint8 filter_select;
EXTERN Uint8 filter_activate;

EXTERN int16 slope_burst_mode_exp;
EXTERN int16 slope_ioff_mode_exp;
EXTERN int32 turn_on;
EXTERN int32 turn_off;
EXTERN Uint8 table_index;

EXTERN Uint8 burst_mode_enable_flag;
EXTERN int32 ioff_value;


EXTERN Uint32 start_up_delay_over;
EXTERN Uint32 count;
EXTERN Uint32 count_end;
EXTERN Uint32 sr_on;
