//###########################################################################
//
// FILE:    pmbus_commands.h
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
//PMBus Version 1.2 command number constants:

#define PMBUS_CMD_PAGE                                            0x00
#define PMBUS_CMD_OPERATION                                       0x01
#define PMBUS_CMD_ON_OFF_CONFIG                                   0x02
#define PMBUS_CMD_CLEAR_FAULTS                                    0x03
#define PMBUS_CMD_PHASE                                           0x04
#define PMBUS_CMD_PAGE_PLUS_WRITE                                 0x05
#define PMBUS_CMD_PAGE_PLUS_READ                                  0x06
                                                                  // 0x07-0x0F Reserved
#define PMBUS_CMD_WRITE_PROTECT                                   0x10
#define PMBUS_CMD_STORE_DEFAULT_ALL                               0x11
#define PMBUS_CMD_RESTORE_DEFAULT_ALL                             0x12
#define PMBUS_CMD_STORE_DEFAULT_CODE                              0x13
#define PMBUS_CMD_RESTORE_DEFAULT_CODE                            0x14
#define PMBUS_CMD_STORE_USER_ALL                                  0x15
#define PMBUS_CMD_RESTORE_USER_ALL                                0x16
#define PMBUS_CMD_STORE_USER_CODE                                 0x17
#define PMBUS_CMD_RESTORE_USER_CODE                               0x18
#define PMBUS_CMD_CAPABILITY                                      0x19
#define PMBUS_CMD_QUERY                                           0x1A
#define PMBUS_CMD_SMBALERT_MASK                                   0x1B
                                                                  // 0x1C - 0x1F Reserved
#define PMBUS_CMD_VOUT_MODE                                       0x20
#define PMBUS_CMD_VOUT_COMMAND                                    0x21
#define PMBUS_CMD_VOUT_TRIM                                       0x22
#define PMBUS_CMD_VOUT_CAL_OFFSET                                 0x23
#define PMBUS_CMD_VOUT_MAX                                        0x24
#define PMBUS_CMD_VOUT_MARGIN_HIGH                                0x25
#define PMBUS_CMD_VOUT_MARGIN_LOW                                 0x26
#define PMBUS_CMD_VOUT_TRANSITION_RATE                            0x27
#define PMBUS_CMD_VOUT_DROOP                                      0x28
#define PMBUS_CMD_VOUT_SCALE_LOOP                                 0x29
#define PMBUS_CMD_VOUT_SCALE_MONITOR                              0x2A
                                                                  // 0x2B - 0x2F Reserved
#define PMBUS_CMD_COEFFICIENTS                                    0x30
#define PMBUS_CMD_POUT_MAX                                        0x31
#define PMBUS_CMD_MAX_DUTY                                        0x32
#define PMBUS_CMD_FREQUENCY_SWITCH                                0x33
                                                                  // 0x34 Reserved
#define PMBUS_CMD_VIN_ON                                          0x35
#define PMBUS_CMD_VIN_OFF                                         0x36
#define PMBUS_CMD_INTERLEAVE                                      0x37
#define PMBUS_CMD_IOUT_CAL_GAIN                                   0x38
#define PMBUS_CMD_IOUT_CAL_OFFSET                                 0x39
#define PMBUS_CMD_FAN_CONFIG_1_2                                  0x3A
#define PMBUS_CMD_FAN_COMMAND_1                                   0x3B
#define PMBUS_CMD_FAN_COMMAND_2                                   0x3C
#define PMBUS_CMD_FAN_CONFIG_3_4                                  0x3D
#define PMBUS_CMD_FAN_COMMAND_3                                   0x3E
#define PMBUS_CMD_FAN_COMMAND_4                                   0x3F
#define PMBUS_CMD_VOUT_OV_FAULT_LIMIT                             0x40
#define PMBUS_CMD_VOUT_OV_FAULT_RESPONSE                          0x41
#define PMBUS_CMD_VOUT_OV_WARN_LIMIT                              0x42
#define PMBUS_CMD_VOUT_UV_WARN_LIMIT                              0x43
#define PMBUS_CMD_VOUT_UV_FAULT_LIMIT                             0x44
#define PMBUS_CMD_VOUT_UV_FAULT_RESPONSE                          0x45
#define PMBUS_CMD_IOUT_OC_FAULT_LIMIT                             0x46
#define PMBUS_CMD_IOUT_OC_FAULT_RESPONSE                          0x47
#define PMBUS_CMD_IOUT_OC_LV_FAULT_LIMIT                          0x48
#define PMBUS_CMD_IOUT_OC_LV_FAULT_RESPONSE                       0x49
#define PMBUS_CMD_IOUT_OC_WARN_LIMIT                              0x4A
#define PMBUS_CMD_IOUT_UC_FAULT_LIMIT                             0x4B
#define PMBUS_CMD_IOUT_UC_FAULT_RESPONSE                          0x4C
                                                                  // 0x4D � 0x4E Reserved
#define PMBUS_CMD_OT_FAULT_LIMIT                                  0x4F
#define PMBUS_CMD_OT_FAULT_RESPONSE                               0x50
#define PMBUS_CMD_OT_WARN_LIMIT                                   0x51
#define PMBUS_CMD_UT_WARN_LIMIT                                   0x52
#define PMBUS_CMD_UT_FAULT_LIMIT                                  0x53
#define PMBUS_CMD_UT_FAULT_RESPONSE                               0x54
#define PMBUS_CMD_VIN_OV_FAULT_LIMIT                              0x55
#define PMBUS_CMD_VIN_OV_FAULT_RESPONSE                           0x56
#define PMBUS_CMD_VIN_OV_WARN_LIMIT                               0x57
#define PMBUS_CMD_VIN_UV_WARN_LIMIT                               0x58
#define PMBUS_CMD_VIN_UV_FAULT_LIMIT                              0x59
#define PMBUS_CMD_VIN_UV_FAULT_RESPONSE                           0x5A
#define PMBUS_CMD_IIN_OC_FAULT_LIMIT                              0x5B  // (For CBC current limit)
#define PMBUS_CMD_IIN_OC_FAULT_RESPONSE                           0x5C
#define PMBUS_CMD_IIN_OC_WARN_LIMIT                               0x5D
#define PMBUS_CMD_POWER_GOOD_ON                                   0x5E
#define PMBUS_CMD_POWER_GOOD_OFF                                  0x5F
#define PMBUS_CMD_TON_DELAY                                       0x60
#define PMBUS_CMD_TON_RISE                                        0x61
#define PMBUS_CMD_TON_MAX_FAULT_LIMIT                             0x62
#define PMBUS_CMD_TON_MAX_FAULT_RESPONSE                          0x63
#define PMBUS_CMD_TOFF_DELAY                                      0x64
#define PMBUS_CMD_TOFF_FALL                                       0x65
#define PMBUS_CMD_TOFF_MAX_WARN_LIMIT                             0x66
                                                                  //0x67 Reserved   Deleted in PMBus Ver 1.1
#define PMBUS_CMD_POUT_OP_FAULT_LIMIT                             0x68
#define PMBUS_CMD_POUT_OP_FAULT_RESPONSE                          0x69
#define PMBUS_CMD_POUT_OP_WARN_LIMIT                              0x6A
#define PMBUS_CMD_PIN_OP_WARN_LIMIT                               0x6B
                                                                  //  0x6C - 0x77 Reserved
#define PMBUS_CMD_STATUS_BYTE                                     0x78
#define PMBUS_CMD_STATUS_WORD                                     0x79
#define PMBUS_CMD_STATUS_VOUT                                     0x7A
#define PMBUS_CMD_STATUS_IOUT                                     0x7B
#define PMBUS_CMD_STATUS_INPUT                                    0x7C
#define PMBUS_CMD_STATUS_TEMPERATURE                              0x7D
#define PMBUS_CMD_STATUS_CML                                      0x7E
#define PMBUS_CMD_STATUS_OTHER                                    0x7F
#define PMBUS_CMD_STATUS_MFR_SPECIFIC                             0x80
#define PMBUS_CMD_STATUS_FANS_1_2                                 0x81
#define PMBUS_CMD_STATUS_FANS_3_4                                 0x82
                                                                  // 0x83 - 0x85 Reserved
#define PMBUS_CMD_READ_EIN                                        0x86
#define PMBUS_CMD_READ_EOUT                                       0x87
#define PMBUS_CMD_READ_VIN                                        0x88
#define PMBUS_CMD_READ_IIN                                        0x89
#define PMBUS_CMD_READ_VCAP                                       0x8A
#define PMBUS_CMD_READ_VOUT                                       0x8B
#define PMBUS_CMD_READ_IOUT                                       0x8C
#define PMBUS_CMD_READ_TEMPERATURE_1                              0x8D
#define PMBUS_CMD_READ_TEMPERATURE_2                              0x8E
#define PMBUS_CMD_READ_TEMPERATURE_3                              0x8F
#define PMBUS_CMD_READ_FAN_SPEED_1                                0x90
#define PMBUS_CMD_READ_FAN_SPEED_2                                0x91
#define PMBUS_CMD_READ_FAN_SPEED_3                                0x92
#define PMBUS_CMD_READ_FAN_SPEED_4                                0x93
#define PMBUS_CMD_READ_DUTY_CYCLE                                 0x94
#define PMBUS_CMD_READ_FREQUENCY                                  0x95
#define PMBUS_CMD_READ_POUT                                       0x96
#define PMBUS_CMD_READ_PIN                                        0x97
#define PMBUS_CMD_PMBUS_REVISION                                  0x98
#define PMBUS_CMD_MFR_ID                                          0x99
#define PMBUS_CMD_MFR_MODEL                                       0x9A
#define PMBUS_CMD_MFR_REVISION                                    0x9B
#define PMBUS_CMD_MFR_LOCATION                                    0x9C
#define PMBUS_CMD_MFR_DATE                                        0x9D
#define PMBUS_CMD_MFR_SERIAL                                      0x9E
#define PMBUS_CMD APP_PROFILE_SUPPORT                             0x9F 
#define PMBUS_CMD_MFR_VIN_MIN                                     0xA0
#define PMBUS_CMD_MFR_VIN_MAX                                     0xA1
#define PMBUS_CMD_MFR_IIN_MAX                                     0xA2
#define PMBUS_CMD_MFR_PIN_MAX                                     0xA3
#define PMBUS_CMD_MFR_VOUT_MIN                                    0xA4
#define PMBUS_CMD_MFR_VOUT_MAX                                    0xA5
#define PMBUS_CMD_MFR_IOUT_MAX                                    0xA6
#define PMBUS_CMD_MFR_POUT_MAX                                    0xA7
#define PMBUS_CMD_MFR_TAMBIENT_MAX                                0xA8
#define PMBUS_CMD_MFR_TAMBIENT_MIN                                0xA9
#define PMBUS_CMD_MFR_EFFICIENCY_LL                               0xAA
#define PMBUS_CMD_MFR_EFFICIENCY_HL                               0xAB
#define PMBUS_CMD_MFR_PIN_ACURRACY                                0xAC
#define PMBUS_CMD_MFR_IC_DEVICE                                   0xAD
#define PMBUS_CMD_MFR_IC_DEVICE_REV                               0xAE
                                                                  // 0xAF Reserved                                                                                               
#define PMBUS_CMD_USER_DATA_00                                    0xB0     
#define PMBUS_CMD_USER_DATA_01                                    0xB1     
#define PMBUS_CMD_USER_DATA_02                                    0xB2     
#define PMBUS_CMD_USER_DATA_03                                    0xB3     
#define PMBUS_CMD_USER_DATA_04                                    0xB4
#define PMBUS_CMD_USER_DATA_05                                    0xB5
#define PMBUS_CMD_USER_DATA_06                                    0xB6
#define PMBUS_CMD_USER_DATA_07                                    0xB7
#define PMBUS_CMD_USER_DATA_08                                    0xB8
#define PMBUS_CMD_USER_DATA_09                                    0xB9
#define PMBUS_CMD_USER_DATA_10                                    0xBA
#define PMBUS_CMD_USER_DATA_11                                    0xBB
#define PMBUS_CMD_USER_DATA_12                                    0xBC
#define PMBUS_CMD_USER_DATA_13                                    0xBD
#define PMBUS_CMD_USER_DATA_14                                    0xBE
#define PMBUS_CMD_USER_DATA_15                                    0xBF
#define PMBUS_CMD_MFR_MAX_TEMP_1                                  0xC0
#define PMBUS_CMD_MFR_MAX_TEMP_2                                  0xC1 
#define PMBUS_CMD_MFR_MAX_TEMP_3                                  0xC2 
                                                                  // 0xC3-0xCF Reserved                                                                                       
#define PMBUS_CMD_MFR_LIGHT_LOAD_ENB                              0xD0      
#define PMBUS_CMD_MFR_SPECIFIC_01                                 0xD1
#define PMBUS_CMD_MFR_SPECIFIC_02                                 0xD2                                          
#define PMBUS_CMD_MFR_SPECIFIC_03                                 0xD3      
#define PMBUS_CMD_MFR_SPECIFIC_04                                 0xD4      
#define PMBUS_CMD_MFR_SPECIFIC_05                                 0xD5      
#define PMBUS_CMD_MFR_SPECIFIC_06                                 0xD6                              
#define PMBUS_CMD_MFR_SPECIFIC_07                                 0xD7      
#define PMBUS_CMD_MFR_SPECIFIC_08                                 0xD8      
#define PMBUS_CMD_ROM_MODE                                        0xD9      
#define PMBUS_CMD_USER_RAM_00                                     0xDA                             
#define PMBUS_CMD_MFR_PHASE_CONTROL                               0xDB                             
#define PMBUS_CMD_MFR_IOUT_OC_FAULT_LIMIT_LOW                     0xDC                                                                                                    
#define PMBUS_CMD_MFR_VIN_SCALE                                   0xDD    
#define PMBUS_CMD_MFR_VIN_OFFSET                                  0xDE    
#define PMBUS_CMD_MFR_READ_TEMPERATURE_4                          0xDF    
#define PMBUS_CMD_MFR_OT_LIMIT_1                                  0xE0     
#define PMBUS_CMD_MFR_OT_LIMIT_2                                  0xE1     
#define PMBUS_CMD_MFR_PARM_INFO                                   0xE2     
#define PMBUS_CMD_MFR_PARM_VALUE                                  0xE3     
#define PMBUS_CMD_MFR_CMDS_DCDC_PAGED                             0xE4     
#define PMBUS_CMD_MFR_CMDS_DCDC_NONPAGED                          0xE5     
#define PMBUS_CMD_MFR_CMDS_PFC                                    0xE6     
#define PMBUS_CMD_MFR_SETUP_ID                                    0xE7     
#define PMBUS_CMD_MFR_OT_LIMIT_3                                  0xE8     
#define PMBUS_CMD_MFR_OT_LIMIT_4                                  0xE9     
#define PMBUS_CMD_MFR_DEADBAND_CONFIG                             0xEA    
#define PMBUS_CMD_MFR_PIN_CAL_A                                   0xEB    
#define PMBUS_CMD_MFR_PIN_CAL_B                                   0xEC                
#define PMBUS_CMD_MFR_PIN_CAL_C                                   0xED    
#define PMBUS_CMD_MFR_PIN_CAL_D                                   0xEE                                                                            
#define PMBUS_CMD_MFR_TEMP_CAL_OFFSET                             0xEF    
#define PMBUS_CMD_MFR_DEBUG_BUFFER                                0xF0     
#define PMBUS_CMD_MFR_TEMP_CAL_GAIN                               0xF1     
#define PMBUS_CMD_MFR_STATUS_BIT_MASK                             0xF2     
#define PMBUS_CMD_MFR_SPECIFIC_35                                 0xF3                                                                                         
#define PMBUS_CMD_MFR_SPECIFIC_36                                 0xF4     
#define PMBUS_CMD_MFR_SPECIFIC_37                                 0xF5                                                                             
#define PMBUS_CMD_MFR_SPECIFIC_38                                 0xF6                                                                                         
#define PMBUS_CMD_MFR_SPECIFIC_39                                 0xF7                                                                             
#define PMBUS_CMD_MFR_VOUT_CAL_MONITOR                            0xF8                                                                             
#define PMBUS_CMD_ROM_MODE_WITH_PASSWORD                          0xF9                                                                             
#define PMBUS_CMD_MFR_SPECIFIC_42                                 0xFA                                                                            
#define PMBUS_CMD_MFR_SPECIFIC_43                                 0xFB                                                                            
#define PMBUS_CMD_MFR_SPECIFIC_44                                 0xFC                                                                            
#define PMBUS_CMD_MFR_DEVICE_ID                                   0xFD                                       
#define PMBUS_CMD_MFR_SPECIFIC_COMMAND                            0xFE    
#define PMBUS_CMD_PMBUS_COMMAND_EXT                               0xFF
