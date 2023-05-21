//###########################################################################
//
// FILE:    pmbus.h
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
//pmbus.h
//===========================================================================
// PMBus Module (PmbusReg) Constant Definitions
//

#include "system_defines.h"

#ifdef MAIN 					// If it is the main routine
	#define EXTERN	 			// If it isn't extern, so define it as nothing
#else 							// If it isn't the main routine
	#define EXTERN extern 		// If it is extern
#endif

#define PMBCTRL1_BYTE2_PRC_CALL 0x10	// 20     Process call message enable
#define PMBCTRL1_BYTE2_GRP_CMD	8		// 19     Group command message enable
#define PMBCTRL1_BYTE2_PEC_ENA	4		// 18     PEC byte enable
#define PMBCTRL1_BYTE2_EXT_CMD	2		// 17     Extended command enable
#define PMBCTRL1_BYTE2_CMD_ENA	1		// 16     Command word enable

#define PMBCTRL1_BYTE0_RW		1		// 0      Read/write indicator

#define PMBCTRL1_ALL_PRC_CALL	0x100000	// 20     Process call message enable
#define PMBCTRL1_ALL_GRP_CMD	0x80000		// 19     Group command message enable
#define PMBCTRL1_ALL_PEC_ENA	0x40000		// 18     PEC byte enable
#define PMBCTRL1_ALL_EXT_CMD	0x20000		// 17     Extended command enable
#define PMBCTRL1_ALL_CMD_ENA	0x10000		// 16     Command word enable

#define PMBACK_BYTE0_ACK 1 //0 pmbus acknowledge enable

#define PMBINTM_BYTE1_LOST_ARB			1		// 8      Lost arbitration interrupt mask

#define PMBINTM_BYTE0_CONTROL			0x80	// 7      Remote module control asserted interrupt mask
#define PMBINTM_BYTE0_ALERT				0x40	// 6      Alert signal detected interrupt mask
#define PMBINTM_BYTE0_EOM				0x20	// 5      End of message interrupt mask
#define PMBINTM_BYTE0_SLAVE_ADDR_READY	0x10	// 4      Slave address ready interrupt mask
#define PMBINTM_BYTE0_DATA_REQUEST		8		// 3      Data request interrupt mask
#define PMBINTM_BYTE0_DATA_READY			4		// 2      Data ready interrupt mask
#define PMBINTM_BYTE0_BUS_LOW_TIMEOUT	2		// 1      Bus low timeout interrupt mask
#define PMBINTM_BYTE0_BUS_FREE			1		// 0      Bus free interrupt mask

#define PMBINTM_HALF0_LOST_ARB			0x10000		// 8      Lost arbitration interrupt mask

#define PMBCTRL2_HALF0_PEC_ENA			0x8000	// 15     Enable PEC processing

#define PMBHSA_BYTE0_SLAVE_ADDR_MASK	0xf7	// 7:1    Stored address acknowledged by slave
#define PMBHSA_BYTE0_SLAVE_RW			1		// 0      Stored R/W bit from slave acknowledge

#define PMBCTRL3_BYTE0_CNTL_INT_EDGE		0x20	// 5      Control interrupt edge select
#define PMBCTRL3_BYTE0_FAST_MODE_PLUS_ENA	0x10	// 4      Fast mode plus enable
#define PMBCTRL3_BYTE0_FAST_MODE_ENA		8		// 3      Fast mode enable
#define PMBCTRL3_BYTE0_BUS_LO_INT_EDGE		4		// 2      Bus low timeout interrupt edge select
#define PMBCTRL3_BYTE0_ALERT_ENA			2		// 1      Alert enable
#define PMBCTRL3_BYTE0_RESET				1		// 0      PMBus Master reset


//=========================================================================================
// PMBus module definitions
//=========================================================================================
	// PMBST Register
#define PMBST_ALL_CONTROL_RAW			(0x00080000)	// 19  Control Pin Raw status
#define PMBST_ALL_ALERT_RAW				(0x00040000)	// 18  Alert Pin Raw status 
#define PMBST_ALL_CONTROL_EDGE			(0x00020000)	// 17  Remote module control asserted flag
#define PMBST_ALL_ALERT_EDGE			(0x00010000)	// 16  Alert signal detected flag
#define PMBST_ALL_MASTER					(0x00008000)	// 15  Master/Slave mode flag
#define PMBST_ALL_LOST_ARB					(0x00004000)	// 14  Lost arbitration flag
#define PMBST_ALL_BUS_FREE					(0x00002000)	// 13  Bus free flag
#define PMBST_ALL_UNIT_BUSY					(0x00001000)	// 12  Unit busy flag
#define PMBST_ALL_RPT_START					(0x00000800)	// 11  Repeated start condition flag
#define PMBST_ALL_SLAVE_ADDR_READY			(0x00000400)	// 10  Slave address ready flag
#define PMBST_ALL_CLK_HIGH_TIMEOUT			(0x00000200)	// 9   Clock high timeout flag
#define PMBST_ALL_CLK_LOW_TIMEOUT			(0x00000100)	// 8   Clock low timeout flag
#define PMBST_ALL_PEC_VALID					(0x00000080)	// 7   PEC valid flag
#define PMBST_ALL_NACK						(0x00000040)	// 6   NACK condition received flag
#define PMBST_ALL_EOM						(0x00000020)	// 5   End of message flag
#define PMBST_ALL_DATA_REQUEST				(0x00000010)	// 4   Data request flag
#define PMBST_ALL_DATA_READY				(0x00000008)	// 3   Data ready flag
#define PMBST_ALL_RD_BYTE_COUNT				(0x00000007)	// 2:0 Read byte count mask

// ***KKN***HEM	It would be nice to get rid of these _BYTE0_ and _HALF0_ values.  
// *** 			Requires change to _ALL_ in multiple places in pmbus.c.
#define PMBST_HALF0_MASTER					(0x8000)		// 15  Master/Slave mode flag
#define PMBST_HALF0_LOST_ARB				(0x4000)		// 14  Lost arbitration flag
#define PMBST_HALF0_BUS_FREE				(0x2000)		// 13  Bus free flag
#define PMBST_HALF0_UNIT_BUSY				(0x1000)		// 12  Unit busy flag
#define PMBST_HALF0_RPT_START				(0x0800)		// 11  Repeated start condition flag
#define PMBST_HALF0_SLAVE_ADDR_READY		(0x0400)		// 10  Slave address ready flag
#define PMBST_HALF0_CLK_HIGH_TIMEOUT		(0x0200)		// 9   Clock high timeout flag
#define PMBST_HALF0_CLK_LOW_TIMEOUT			(0x0100)		// 8   Clock low timeout flag
#define PMBST_HALF0_PEC_VALID				(0x0080)		// 7   PEC valid flag
#define PMBST_HALF0_NACK					(0x0040)		// 6   NACK condition received flag
#define PMBST_HALF0_EOM						(0x0020)		// 5   End of message flag
#define PMBST_HALF0_DATA_REQUEST			(0x0010)		// 4   Data request flag
#define PMBST_HALF0_DATA_READY				(0x0008)		// 3   Data ready flag
#define PMBST_HALF0_RD_BYTE_COUNT			(0x0007)		// 2:0 Read byte count mask

#define PMBST_BYTE0_PEC_VALID				(0x80)			// 7   PEC valid flag
#define PMBST_BYTE0_NACK					(0x40)			// 6   NACK condition received flag
#define PMBST_BYTE0_EOM						(0x20)			// 5   End of message flag
#define PMBST_BYTE0_DATA_REQUEST			(0x10)			// 4   Data request flag
#define PMBST_BYTE0_DATA_READY				(0x08)			// 3   Data ready flag
#define PMBST_BYTE0_RD_BYTE_COUNT			(0x07)			// 2:0 Read byte count mask


	// PMBCTRL2 Register
#define PMBCTRL2_ALL_RX_BYTE_ACK_CNT		(0x00600000)	// 22:21  Received byte acknowledge count	
#define PMBCTRL2_ALL_MAN_CMD				(0x00100000)	// 20     Manual command acknowledge mode
#define PMBCTRL2_ALL_TX_PEC					(0x00080000)	// 19     Transmited PEC flag
#define PMBCTRL2_ALL_TX_COUNT				(0x00070000)	// 18:16  Transmit byte count
#define PMBCTRL2_ALL_PEC_ENA				(0x00008000)	// 15     Enable PEC processing
#define PMBCTRL2_ALL_SLAVE_MASK				(0x00007F00)	// 14:8   Slave mask
#define PMBCTRL2_ALL_MAN_SLAVE_ACK			(0x00000080)	// 7      Manual slave acknowledge mode enable
#define PMBCTRL2_ALL_SLAVE_ADDR				(0x0000007F)	// 6:0    Slave address

#define PMBCTRL2_BYTE2_RX_BYTE_ACK_CNT		(0x60)			// 22:21  Received byte acknowledge count	
#define PMBCTRL2_BYTE2_MAN_CMD				(0x10)			// 20     Manual command acknowledge mode
#define PMBCTRL2_BYTE2_TX_PEC				(0x08)			// 19     Transmited PEC flag
#define PMBCTRL2_BYTE2_TX_COUNT				(0x07)			// 18:16  Transmit byte count
#define PMBCTRL2_BYTE1_PEC_ENA				(0x80)			// 15     Enable PEC processing
#define PMBCTRL2_BYTE1_SLAVE_MASK			(0x7F)			// 14:8   Slave mask
#define PMBCTRL2_BYTE0_MAN_SLAVE_ACK		(0x80)			// 7      Manual slave acknowledge mode enable
#define PMBCTRL2_BYTE0_SLAVE_ADDR			(0x7F)			// 6:0    Slave address


//=========================================================================================

void init_pmbus(void);
void pmbus_handler(void);

void pmbus_idle_handler(void);
void pmbus_read_block_handler(void);
void pmbus_write_block_handler(void);
void pmbus_read_wait_for_eom_handler(void);

int pmbus_read_setup_id(void);
int pmbus_read_device_id(void);

Uint8 pmbus_read_cmd_dcdc_paged(void);

Uint8 pmbus_read_cmd_dcdc_nonpaged(void);

Uint8 pmbus_read_cmd_pfc(void);



/*
Uint8 pmbus_read_pin_cal_A(void);
Uint8 pmbus_write_pin_cal_A(void);
Uint8 pmbus_read_pin_cal_B(void);
Uint8 pmbus_write_pin_cal_B(void);
Uint8 pmbus_read_pin_cal_C(void);
Uint8 pmbus_write_pin_cal_C(void);
Uint8 pmbus_read_pin_cal_D(void);
Uint8 pmbus_write_pin_cal_D(void);
*/
#define	NUM_CLA_BYTES				25		// Bytes in CLA gains structure (pmbus use).

EXTERN char	status_cml;	 // STATUS_CML (Communication, Logic, and Memory) for entire board
#define	CML_INVALID_CMD			(7)		// Invalid or Unsupported Command Received
#define	CML_INVALID_DATA		(6)		// Invalid or Unsupported Data Received
#define	CML_PEC_FAILED			(5)		// Packet Error Check Failed
#define	CML_MEMORY_FAULT	  	(4)		// Memory Fault Detected
#define	CML_PROC_FAULT			(3)		// Processor Fault Detected
#define	CML_RSVD2 				(2)		// Reserved
#define	CML_COMM_OTHER_FAULT	(1)		// Unlisted communication fault
#define	CML_OTHER_FAULT			(0)		// Other Memory or Logic fault has occurred


#define PMBUS_SUCCESS               0

#define PMBUS_INVALID_DATA         (1<<CML_INVALID_DATA)



#define PMBUS_BUFFER_SIZE (50) //40
#define PMBST_HALF0_CHECK_BITS (PMBST_BYTE0_EOM + PMBST_BYTE0_DATA_READY + PMBST_BYTE0_DATA_REQUEST + PMBST_BYTE0_PEC_VALID + PMBST_HALF0_CLK_LOW_TIMEOUT + PMBST_HALF0_CLK_HIGH_TIMEOUT)
#define PMBCTRL2_HALF0_SLAVE_ADDRESS_MASK_DISABLE 0x7f00 //any bits cleared in slave address mask make that bit a don't care

#define PMBUS_STATE_IDLE 0
#define PMBUS_STATE_WRITE_BLOCK 1
#define PMBUS_STATE_READ_BLOCK 2
#define PMBUS_STATE_READ_WAIT_FOR_EOM 3

//#define program_flash_integrity_word *((volatile Uint32 *) 0x7ffc)

#define VBUS_FULL_RANGE_TEXT "572.57" //sent to model for design of voltage loop.


//-----------------------
//setup ID description:
//DCDC PFC | CLA 1           CLA2               CLA 3              CLA 4             |  DCDC rail 1 |   DCDC rail 2 
//-----------------------------------------------------------------------------------------------------------------
// #   #    rail# loop type   rail# loop type   rail# loop type    rail# loop type      Topology      Topology
// R   R     O       R         O       R         O      R           O       R              O            O
// R: means the field is required to have.
// O: means the field is optional to have
//
//DCDC #: required. choose from 0, 1, 2;  # of DCDC rails
//PFC #:  required. choose from 0, 1;  # of PFC 
//rail #: optional. choose from 1, 2 if the associated CLA loop type is either C or V. 
//loop type: required. choose from C, V, N. C: current loop. V: voltage loop. N: N/A.
//           defines each CLA's loop type.
//Topology:  optional.DCDC rail topology. choose from: TTIF, PSHFB, ZVSPWM, LLC, HSFB
//-----------------------------------------------------------------------------------------

#define SETUP_ID            "VERSION1|LLC002"			//Generic setup id
#define DEVICE           	 UCD3100ISO1  				//Device Name
#define MFR_ID				"TI"						//Hardware Manufacturer
#define MFR_MODEL			"UCD3138LLCEVM-028"			//Hardware Model
#define MFR_REVISION    	"E3"						//Hardware revision
#define MFR_DATE			"YYMMDD"					//Manufacturer date
#define MFR_LOCATION		"Dallas, TX"				//Manufacturer location
#define MFR_SERIAL			"XXXXX"						//Serial number
#define MFR_IC_DEVICE		"UCD3138RGC"				//TI device part number
#define MFR_IC_DEVICE_REV	"0"							//TI device revision

#define SETUP_ID_LENGTH 			sizeof(SETUP_ID)
#define MFR_ID_LENGTH 				sizeof(MFR_ID)
#define MFR_MODEL_LENGTH 			sizeof(MFR_MODEL)
#define MFR_REVISION_LENGTH 		sizeof(MFR_REVISION)
#define MFR_DATE_LENGTH				sizeof(MFR_DATE)
#define MFR_LOCATION_LENGTH			sizeof(MFR_LOCATION)
#define MFR_SERIAL_LENGTH			sizeof(MFR_SERIAL)
#define MFR_IC_DEVICE_LENGTH		sizeof(MFR_IC_DEVICE)
#define MFR_IC_DEVICE_REV_LENGTH	sizeof(MFR_IC_DEVICE_REV)

////////////////////////////////////////////////////////////////
//the commands that shows bit masking of which PMBUS commands 
//are supported
//CMD_DCDC_PAGED, CMD_DCDC_NONPAGED, CMD_PFC 
// 
//cmd-->lower nibble  
// #     0123 4567    89AB CDEF
//higher
//nibble
//0    0b0000 0000, 0b0000 0000, command codes from 0x00 to 0x0F
//1    0b0000 0000, 0b0000 0000, command codes from 0x10 to 0x1F
//2    0b0000 0000, 0b0000 0000, command codes from 0x20 to 0x2F
//3    0b0000 0000, 0b0000 0000, command codes from 0x30 to 0x3F
//4    0b0000 0000, 0b0000 0000, command codes from 0x40 to 0x4F
//5    0b0000 0000, 0b0000 0000, command codes from 0x50 to 0x5F
//6    0b0000 0000, 0b0000 0000, command codes from 0x60 to 0x6F
//7    0b0000 0000, 0b0000 0000, command codes from 0x70 to 0x7F
//8    0b0000 0000, 0b0000 0000, command codes from 0x80 to 0x8F
//9    0b0000 0000, 0b0000 0000, command codes from 0x90 to 0x9F
//A    0b0000 0000, 0b0000 0000, command codes from 0xA0 to 0xAF
//B    0b0000 0000, 0b0000 0000, command codes from 0xB0 to 0xBF
//C    0b0000 0000, 0b0000 0000, command codes from 0xC0 to 0xCF
//D    0b0000 0000, 0b0000 0000, command codes from 0xD0 to 0xDF
//E    0b0000 0000, 0b0000 0000, command codes from 0xE0 to 0xEF
//F    0b0000 0000, 0b0000 0000, command codes from 0xF0 to 0xFF

#define CMD_PFC  \
                           {0x00, 0x00, \
                            0x00, 0x00, \
                            0x00, 0x00, \
                            0x00, 0x00, \
                            0x00, 0x00, \
                            0x00, 0x00, \
                            0x00, 0x00, \
                            0x00, 0x00, \
                            0x00, 0x40, \
                            0x01, 0x00, \
                            0x00, 0x00, \
                            0x00, 0x00, \
                            0x00, 0x00, \
                            0x00, 0x00, \
                            0x00, 0x00, \
                            0x00, 0x00 \
                            }          

#define CMD_DCDC_PAGED  \
 						   {0x00, 0x00, \
                            0x00, 0x00, \
							0x00, 0x00, \
							0x00, 0x00, \
							0x00, 0x00, \
							0x00, 0x00, \
							0x00, 0x00, \
							0x00, 0x00, \
							0x00, 0x00, \
							0x00, 0x00, \
							0x00, 0x00, \
							0x00, 0x00, \
							0x00, 0x00, \
							0x00, 0x00, \
							0x00, 0x00, \
							0x00, 0x00  \
							}

#define CMD_DCDC_NONPAGED  \
                           {0x90, 0x00, \
                            0x60, 0x00, \
                            0xC1, 0x00, \
                            0x06, 0x00, \
                            0xB8, 0x21, \
                            0x45, 0xD7, \
                            0x40, 0x00, \
                            0x00, 0xC0, \
                            0x00, 0x9E, \
                            0x04, 0xFE, \
                            0x00, 0x06, \
                            0x00, 0x00, \
                            0x00, 0x00, \
                            0x74, 0x60, \
                            0x3D, 0x20, \
                            0x89, 0x0C \
                            }                  
           
                                                                                   

#define STATUS_BIT_MASK    {0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x80, 0x34, 0xC8, 0x00, 0x00}
#define PFC_STATUS_BIT_MASK    {0x00, 0x00, 0x00, 0x80, 0x90, 0x00, 0x90, 0x2c, 0xa0, 0x00, 0x00}


#define DEBUG_0_TEXT "Debug 0"
#define DEBUG_1_TEXT "Debug 1"
#define DEBUG_2_TEXT "Debug 2"
#define DEBUG_3_TEXT "Debug 3"
#define DEBUG_4_TEXT "Debug 4"
#define DEBUG_5_TEXT "Debug 5"
#define DEBUG_6_TEXT "Debug 6"
#define DEBUG_7_TEXT "Debug 7"

EXTERN Uint8 gui_constant_pointer;
EXTERN Uint8 user_ram_00;

///////////////////////////////////////////////////////////////
//variables for PARM_INFO and PARM_VAR
///////////////////////////////////////////////////////////////

//  Memory limits used by the PARM_INFO and PARM_VALUE commands.
#define	RAM_START_ADDRESS	0x00019000	// Beginning of RAM
#define	RAM_END_ADDRESS		0x00019FFF	// End of RAM
#define RAM_LENGTH			(RAM_END_ADDRESS - RAM_START_ADDRESS + 1)			

// Allow access to peripherals, but not core ARM regs.
#define	REG_START_ADDRESS	0xFFF7d100	// Beginning of Register space
#define	REG_END_ADDRESS		0xFFF7fdff	// End of Register space
#define REG_LENGTH			(REG_END_ADDRESS - REG_START_ADDRESS + 1)

// Allow read-only access to Data Flash
#define	DFLASH_START_ADDRESS	0x00018800	// Beginning of DFLASH
#define	DFLASH_END_ADDRESS		0x00018FFF	// End of DFLASH
#define DFLASH_LENGTH			(DFLASH_END_ADDRESS - DFLASH_START_ADDRESS + 1)			

// Allow read-only access to Constants in Program Flash 
#define	PFLASH_CONST_START_ADDRESS	0x00000000	// Beginning of PFLASH Constants
#define	PFLASH_CONST_END_ADDRESS   	0x00007FFF	// End of PFLASH Constants
#define PFLASH_CONST_LENGTH			(PFLASH_CONST_END_ADDRESS - PFLASH_CONST_START_ADDRESS + 1)			

// Allow read-only access to Program in Program Flash 
#define	PFLASH_PROG_START_ADDRESS	0x00000000	// Beginning of PFLASH Program
#define	PFLASH_PROG_END_ADDRESS   	0x00007FFF	// End of PFLASH Program
#define PFLASH_PROG_LENGTH			(PFLASH_PROG_END_ADDRESS - PFLASH_PROG_START_ADDRESS + 1)			


#ifdef Cyclone2p0

 //Cyclone 2 extra addresses
#define LOOP_MUX_START_ADDRESS  0x00020000
#define LOOP_MUX_LENGTH         0x00000078

#define FAULT_MUX_START_ADDRESS 0x00030000
#define FAULT_MUX_LENGTH        0x00000080

#define ADC_START_ADDRESS       0x00040000
#define ADC_LENGTH              0x00000098

#define DPWM3_START_ADDRESS     0x00050000
#define DPWM3_LENGTH            0x0000008c

#define FILTER2_START_ADDRESS   0x00060000
#define FILTER2_LENGTH          0x00000064

#define DPWM2_START_ADDRESS     0x00070000
#define DPWM2_LENGTH            0x0000008c

#define FE_CTRL2_START_ADDRESS  0x00080000
#define FE_CTRL2_LENGTH         0x00000044

#define FILTER1_START_ADDRESS   0x00090000
#define FILTER1_LENGTH          0x00000064

#define DPWM1_START_ADDRESS     0x000a0000
#define DPWM1_LENGTH            0x0000008c

#define FE_CTRL1_START_ADDRESS  0x000b0000
#define FE_CTRL1_LENGTH         0x00000044

#define FILTER0_START_ADDRESS   0x000c0000
#define FILTER0_LENGTH          0x00000064

#define DPWM0_START_ADDRESS     0x000d0000
#define DPWM0_LENGTH            0x0000008c

#define FE_CTRL0_START_ADDRESS  0x000e0000
#define FE_CTRL0_LENGTH         0x00000044

#define SYSTEM_REGS_START_ADDRESS 0xfffffd00
#define SYSTEM_REGS_LENGTH        0x2d0


#define	NUM_MEMORY_SEGMENTS	19	// 19 memory segments for Cyclone 2

#else

#define	NUM_MEMORY_SEGMENTS	5	// Set this equal to the number of memory segments that can
								// be accessed by the PARM_VALUE read command.
								// 5: allow reading both Program and constants from PFLASH
        						 // 4: allow reading constants from PFLASH, but not Program 
#endif 



//#define PMBUS_BUFFER_SIZE 50

// These error codes are masks based on bits in the CML_STATUS byte.  This allows the
// pmbus_error_handler() function to do a simple 'OR' instead of a big 'switch' statement.
#define	PMBUS_INVALID_CMD		(1<<CML_INVALID_CMD)		// bit7 Invalid or Unsupported Command Received
#define	PMBUS_INVALID_DATA		(1<<CML_INVALID_DATA)		// bit6 Invalid or Unsupported Data Received
#define	PMBUS_PEC_FAILED		(1<<CML_PEC_FAILED)			// bit5 Packet Error Check Failed
#define	PMBUS_MEMORY_FAULT	 	(1<<CML_MEMORY_FAULT)	  	// bit4 Memory Fault Detected
#define	PMBUS_PROC_FAULT		(1<<CML_PROC_FAULT)			// bit3 Processor Fault Detected
#define	PMBUS_RSVD2 			(1<<CML_RSVD2) 				// bit2 Reserved
#define	PMBUS_COMM_OTHER_FAULT	(1<<CML_COMM_OTHER_FAULT)	// bit1 Unlisted communication fault
#define	PMBUS_OTHER_FAULT		(1<<CML_OTHER_FAULT)		// bit0 Other Memory or Logic fault has occurred

EXTERN	Uint8	parm_index;
EXTERN	int16	parm_offset;	
EXTERN	Uint8	parm_count;		
EXTERN	Uint8	parm_size;	

EXTERN Uint8 pmbus_buffer[PMBUS_BUFFER_SIZE] ;
EXTERN Uint8 pmbus_state;
EXTERN Uint8 pmbus_number_of_bytes;
EXTERN Uint8 pmbus_buffer_position;
EXTERN Uint16 pmbus_status_half_word_0_value; //save pmbus status, since cleared on read.
EXTERN Uint16 pmbus_status_half_word_0_value_ored; //for debug

/////////////////////////////////////////////////////////
//non-paged variables
//the same for both PFC and DCDC
////////////////////////////////////////////////////////// 

EXTERN Uint8  				debug_buffer[8];
EXTERN Uint8  				page;
EXTERN Uint8                pmbus_status_temperature;
EXTERN Uint8                pmbus_status_input;
EXTERN Uint8                pmbus_status_cml;
EXTERN Uint8                pmbus_status_fans_1_2;
EXTERN Uint8                pmbus_status_fans_3_4;
EXTERN Uint16               pmbus_status_word;
EXTERN Uint16               second_pmbus_status_word;
EXTERN Uint8                setup_id_length;

typedef struct
{
 Uint16 vout_cal_offset;
 Uint16 iout_cal_gain;
 Uint16 iout_cal_offset;
 Uint16 temperature_cal_offset;
 Uint16 temperature_cal_gain;
 Uint16 vout_cal_monitor;
}PMBUS_DCDC_CAL;//must be even number of int16

//first rail
#define DEFAULT_PMBUS_DCDC_1_CAL {0, 0, 0, 0, 0, 0}

//second rail default value
#define DEFAULT_PMBUS_DCDC_2_CAL  {0, 0, 0, 0, 0, 0}        

typedef struct
{
 Uint16 vin_scale;
 Uint16 vin_offset;
}PMBUS_DCDC_CAL_NONPAGED; //must be even number of int16

typedef struct 
{
 Uint16 vout_cmd;
 Uint16 vout_ov_fault_limit;
 Uint16 vout_ov_warn_limit;
 Uint16 vout_uv_fault_limit;
 Uint16 vout_uv_warn_limit;
 Uint16 iout_oc_fault_limit;
 Uint16 iout_oc_warn_limit;
 Uint16 temp_ot_fault_limit;
 Uint16 temp_ot_warn_limit;
 Uint16 vin_ov_fault_limit;
 Uint16 vin_ov_warn_limit;
 Uint16 vin_uv_fault_limit;
 Uint16 vin_uv_warn_limit;
 Uint16 iin_oc_fault_limit;
 Uint16 iin_oc_warn_limit;
 Uint16 pgood_on_limit;
 Uint16 pgood_off_limit; 
 Uint16 vin_on_limit;
 Uint16 vin_off_limit;
 Uint32 ton_rise;
 Uint32 vout_transition_rate;
 Uint16 dead_time_1;
 Uint16 dead_time_2;
 Uint16 dead_time_3;
 Uint16 dead_time_4;
 Uint16 sample_rate;
 Uint16 min_period;
 Uint16 max_period;
 Uint16 tsrmax;
 Uint32 ll_turn_on_thresh;
 Uint32 ll_turn_off_thresh;
 Uint8  ll_en;
 Uint16 cpcc_pmax;
 Uint16 cpcc_imax;
 Uint16	cpcc_ton;
 Uint8	cpcc_enable;
 Uint8  cpcc_time_out_en;
 Uint8	ishare_enable;
 Uint8  iout_oc_fault_response;
 Uint8  rsvd;
}PMBUS_DCDC_CONFIG; //must be even number of int16

#define VOUT_0 				(6144)
#define VOUT_OV_FAULT_LIMIT (7168)
#define VOUT_OV_WARN_LIMIT 	(6656)
#define VOUT_UV_FAULT_LIMIT (1536)
#define VOUT_UV_WARN_LIMIT 	(5632)
#define IOUT_OC_FAULT_LIMIT (40)
#define IOUT_OC_WARN_LIMIT 	(30)
#define TEMP_OT_FAULT_LIMIT (110)
#define TEMP_OT_WARN_LIMIT 	(90)
#define VIN_OV_FAULT_LIMIT 	(430)
#define VIN_OV_WARN_LIMIT 	(400)
#define VIN_UV_FAULT_LIMIT 	(365)
#define VIN_UV_WARN_LIMIT 	(370)
#define IIN_OC_FAULT_LIMIT	(5)
#define IIN_OC_WARN_LIMIT	(4)
#define PGOOD_ON_LIMIT		(5888)
#define PGOOD_OFF_LIMIT		(5632)
#define VIN_ON_LIMIT		(350)
#define VIN_OFF_LIMIT		(340)
#define TON_RISE 			(30)
#define VOUT_0_TRAN 		(0x9B33)
#define DEADTIME1			((int)(DT1 / TDPWM + 0.5))
#define DEADTIME2			((int)(DT2 / TDPWM + 0.5))
#define DEADTIME3			((int)(DT1SR / TDPWM + 0.5))
#define DEADTIME4			((int)(DT2SR / TDPWM + 0.5))
#define SAMPLERATE 			((int)(TSAMP / TDPWM / 16 + 0.5))
#define MINPER				((int)(TH / TDPWM / 16 + 0.5))
#define MAXPER				((int)(TL / TDPWM / 16 + 0.5))
#define TSRMAX	 			((int)((TSR - DT2SR) / TDPWM / 16 + 0.5))
#define TURN_ON_THRESHOLD	(23000)
#define TURN_OFF_THRESHOLD	(23000)
#define LL_ENABLE			(0)
#define CPCC_PMAX			(380)
#define CPCC_IMAX			(37)
#define CPCC_TON			(8817) //Time in milli-seconds, linear 11 is 10s
#define CPCC_ENABLE			(1)
#define CPCC_TIME_OUT_EN	(1)
#define ISHARE_ENABLE		(0)
#define IOUT_OC_FAULT_RESPONSE	(0)


#define  HALF_PERIOD    ((MINPER + 1) >> 1)

  // ---------------------- DPWM0
#define  PWM0_EV1       ((DEADTIME1 + 8) >> 4)
#define  PWM0_EV2       (MINPER * 8 - (DEADTIME2 >> 1))
#define  PWM0_EV3       (MINPER * 8 + (DEADTIME2 >> 1))
#define  PWM0_EV4       (MINPER * 16 - DEADTIME1)
#define  PWM0_EV5       ((DEADTIME3 + 8) >> 1) 
#define  PWM0_EV6       (MINPER * 8 - (DEADTIME4 >> 2))
#define  PWM0_CYC_ADJ   (-DEADTIME2)
  // ---------------------- DPWM1
#define  PWM1_EV1       ((DEADTIME3 + 8) >> 4)
#define  PWM1_EV2       (MINPER * 8 - (DEADTIME4 >> 1))
#define  PWM1_EV3       (MINPER * 8 + (DEADTIME4 >> 1))
#define  PWM1_EV4       (MINPER * 16 - DEADTIME3)
#define  PWM1_EV5       ((DEADTIME3 + 8) >> 1) 
#define  PWM1_EV6       (MINPER * 8 - (DEADTIME4 >> 2))
#define  PWM1_CYC_ADJ   (-DEADTIME4)
  // ---------------------- DPWM2
#define  PWM2_EV1       ((DEADTIME1 + 8) >> 4)
#define  PWM2_EV2       (MINPER * 8 - (DEADTIME2 >> 1))
#define  PWM2_EV3       (MINPER * 8 + (DEADTIME2 >> 1))
#define  PWM2_EV4       (MINPER * 16 - DEADTIME1)
#define  PWM2_EV5       ((DEADTIME3 + 8) >> 1)
#define  PWM2_EV6       (MINPER * 8 - (DEADTIME4 >> 2))
#define  PWM2_CYC_ADJ   (-DEADTIME2)
  // ---------------------- DPWM3 
#define  PWM3_EV1       ((DEADTIME1 + 8) >> 4)
#define  PWM3_EV2       (MINPER * 8 - (DEADTIME2 >> 1))
#define  PWM3_EV3       (MINPER * 8 + (DEADTIME2 >> 1))
#define  PWM3_EV4       (MINPER * 16 - DEADTIME1)
#define  PWM3_CYC_ADJ   (-DEADTIME2)
 
  // ----------------------Intra-Mux
#define  PASS_THROUGH   (0)
#define  EDGEGEN        (1)
#define  PWMC           (2)
#define  CROSSOVER      (3)
#define  BELOW_A        (4)
#define  BELOW_B        (5)
#define  BELOW_C        (6)
#define  BELOW2_C       (7)
#define  BELOW3_C       (8) 
  // ----------------------Edge-Gen
#define  CURRENT_POS_A  (0)
#define  CURRENT_NEG_A  (1)
#define  CURRENT_POS_B  (2)
#define  CURRENT_NEG_B  (3)
#define  NEXT_POS_A     (4)
#define  NEXT_NEG_A     (5)
#define  NEXT_POS_B     (6)
#define  NEXT_NEG_B     (7)
  // ----------------------PWM modes
#define  NORMAL         (0)
#define  RESONANT       (1)
#define  MULTI          (2)


//first rail default value          
#define DEFAULT_PMBUS_DCDC_1_CONFIG  {\
VOUT_0, \
VOUT_OV_FAULT_LIMIT, \
VOUT_OV_WARN_LIMIT, \
VOUT_UV_FAULT_LIMIT,\
VOUT_UV_WARN_LIMIT, \
IOUT_OC_FAULT_LIMIT,\
IOUT_OC_WARN_LIMIT,\
TEMP_OT_FAULT_LIMIT,\
TEMP_OT_WARN_LIMIT,\
VIN_OV_FAULT_LIMIT,\
VIN_OV_WARN_LIMIT, \
VIN_UV_FAULT_LIMIT,\
VIN_UV_WARN_LIMIT, \
IIN_OC_FAULT_LIMIT,\
IIN_OC_WARN_LIMIT,\
PGOOD_ON_LIMIT,\
PGOOD_OFF_LIMIT, \
VIN_ON_LIMIT,\
VIN_OFF_LIMIT,\
TON_RISE, \
VOUT_0_TRAN,\
DEADTIME1,\
DEADTIME2,\
DEADTIME3,\
DEADTIME4,\
SAMPLERATE,\
MINPER,\
MAXPER,\
TSRMAX,\
TURN_ON_THRESHOLD,\
TURN_OFF_THRESHOLD,\
LL_ENABLE,\
CPCC_PMAX,\
CPCC_IMAX,\
CPCC_TON,\
CPCC_ENABLE,\
CPCC_TIME_OUT_EN,\
ISHARE_ENABLE,\
IOUT_OC_FAULT_RESPONSE,\
0}

//second rail default value
#define DEFAULT_PMBUS_DCDC_2_CONFIG  {22222, POUT_MAX_ADC, IOUT_MAX_ADC, 15360, 14848, 13312, 33, 30, 300, 0, 0, 5, 20, 1, 0}        

typedef struct
{
 Uint16 ot_limit_DCDC_1;
 Uint16 ot_limit_DCDC_2;
 Uint16 ot_limit_DCDC_3;
 Uint16 ot_limit_DCDC_4;
 int16  deadband_config[8];
 Uint16 vin_ov_fault_limit;
 Uint16 vin_uv_fault_limit;
 Uint16 vin_uv_warn_limit;
 char  mfr_date[14];
 char  rom_password[ROM_PASSWORD_BYTES];
}PMBUS_DCDC_CONFIG_NONPAGED; //must be even number int16


//EXTERN PMBUS_DCDC_READING            pmbus_dcdc_reading[DCDC_PAGE_NUMS];
EXTERN PMBUS_DCDC_CONFIG             pmbus_dcdc_config[DCDC_PAGE_NUMS];
EXTERN PMBUS_DCDC_CONFIG             pmbus_dcdc_config_translated[DCDC_PAGE_NUMS];
EXTERN PMBUS_DCDC_CAL                pmbus_dcdc_cal[DCDC_PAGE_NUMS]; 

EXTERN PMBUS_DCDC_CONFIG_NONPAGED    pmbus_dcdc_config_nonpaged;
EXTERN PMBUS_DCDC_CONFIG_NONPAGED    pmbus_dcdc_config_nonpaged_translated;
EXTERN PMBUS_DCDC_CAL_NONPAGED       pmbus_dcdc_cal_nonpaged;
EXTERN Uint16                        dcdc_temperature_1, dcdc_temperature_2, dcdc_temperature_3, dcdc_temperature_4;

struct FILTER_MISC_REG_BITS{
    Uint32 rsvd0:28;                // Reserved
	Uint32 AUTO_GEAR_SHIFT:1;		// AUTO_GEAR_SHIFT - configuration bit to control the automatic gear shifting function
	Uint32 AFE_GAIN:2;				// AFE_GAIN - AFE gain
    Uint32 NL_MODE:1;          		// NL_MODE - stored in Filter Control Register
  };

union FILTER_MISC_REG{         // bits   description
    struct FILTER_MISC_REG_BITS bit;
    Uint32                 all;
  };

struct FILTER_MISC_GAIN_BITS{
    Uint32 rsvd0:12;    //Reserved          
	Uint32 KCOMP:14;	//KCOMP
    Uint32 CLA_SCALE:3;	//CLA_SCALE
    Uint32 YN_SCALE:3;  //YN_SCALE       		
  };

union FILTER_MISC_GAIN{         // bits   description
    struct FILTER_MISC_GAIN_BITS bit;
    Uint32                 all;
  };

  //=============================================================================
  // Filter Module Register File
  //=============================================================================
typedef struct  {
    union COEFCONFIG_REG        COEFCONFIG;         // Coefficient Configuration Register
    union FILTERKPCOEF0_REG     FILTERKPCOEF0;      // Filter KP Coefficient 0 Register
    union FILTERKPCOEF1_REG     FILTERKPCOEF1;      // Filter KP Coefficient 1 Register
    union FILTERKICOEF0_REG     FILTERKICOEF0;      // Filter KI Coefficient 0 Register
    union FILTERKICOEF1_REG     FILTERKICOEF1;      // Filter KI Coefficient 1 Register
    union FILTERKDCOEF0_REG     FILTERKDCOEF0;      // Filter KD Coefficient 0 Register
    union FILTERKDCOEF1_REG     FILTERKDCOEF1;      // Filter KD Coefficient 1 Register
    union FILTERKDALPHA_REG     FILTERKDALPHA;      // Filter KD Alpha Register
    union FILTERNL0_REG         FILTERNL0;          // Filter Non-Linear Limit 0 Register
    union FILTERNL1_REG         FILTERNL1;          // Filter Non-Linear Limit 1 Register
    union FILTERNL2_REG         FILTERNL2;          // Filter Non-Linear Limit 2 Register
    union FILTERKICLPHI_REG     FILTERKICLPHI;      // Filter KI Feedback Clamp High Register
    union FILTERKICLPLO_REG     FILTERKICLPLO;      // Filter KI Feedback Clamp Low Register
    union FILTERYNCLPHI_REG     FILTERYNCLPHI;      // Filter YN Clamp High Register
    union FILTERYNCLPLO_REG     FILTERYNCLPLO;      // Filter YN Clamp Low Register
	union FILTEROCLPHI_REG		FILTEROCLPHI;       // Filter Output Clamp High Register
	union FILTEROCLPLO_REG		FILTEROCLPLO;       // Filter Output Clamp Low Register
	union FILTER_MISC_REG		FILTER_MISC;        // Miscellaneous bits
	union FILTER_MISC_GAIN		FILTER_MISC_GAIN;   // 
  }FILTER_PMBUS_REGS;

#define DEFAULT_PMBUS_DCDC_CONFIG_NONPAGED {81, 82, 83, 84, {1 ,400 ,400 ,400 ,400 , 0, 0, 0}, 55, 32, 38,{'1','1','/','1','2','/','2','0','0','9',0,0,0,0},{'A','B','C','D'}}
                                         
#define DEFAULT_PMBUS_DCDC_CAL_NONPAGED    {0, 0}

//FILTER 0 pmbus controlled coefficients  
#define INIT_FILTER_0_BIN0_ALPHA (0)           // 3      Bin0 Alpha Configuration
#define INIT_FILTER_0_BIN0_CONFIG (0)          // 2:0    Bin0 Configuration
#define INIT_FILTER_0_BIN1_ALPHA (1)           // 7      Bin1 Alpha Configuration
#define INIT_FILTER_0_BIN1_CONFIG (1)          // 6:4    Bin1 Configuration
#define INIT_FILTER_0_BIN2_ALPHA (2)           // 11     Bin2 Alpha Configuration
#define INIT_FILTER_0_BIN2_CONFIG (2)          // 10:8   Bin2 Configuration
#define INIT_FILTER_0_BIN3_ALPHA (2)           // 15     Bin3 Alpha Configuration
#define INIT_FILTER_0_BIN3_CONFIG (2)          // 14:12  Bin3 Configuration
#define INIT_FILTER_0_BIN4_ALPHA (2)           // 19     Bin4 Alpha Configuration
#define INIT_FILTER_0_BIN4_CONFIG (2)          // 18:16  Bin4 Configuration
#define INIT_FILTER_0_BIN5_ALPHA (2)           // 23     Bin5 Alpha Configuration
#define INIT_FILTER_0_BIN5_CONFIG (2)          // 22:20  Bin5 Configuration
#define INIT_FILTER_0_BIN6_ALPHA (2)           // 27     Bin6 Alpha Configuration
#define INIT_FILTER_0_BIN6_CONFIG (2)          // 26:24  Bin6 Configuration

#define INIT_FILTER_0_KP_COEF_0 (12500) //(2486)//21571)//29033)//             // 15:0   KP Coefficient 0
#define INIT_FILTER_0_KP_COEF_1 (15500)             // 31:16  KP Coefficient 1
#define INIT_FILTER_0_KP_COEF_2 (24500)             // 15:0   KP Coefficient 2

#define INIT_FILTER_0_KI_COEF_0 (3000) //(1374)//47)//             // 15:0   KI Coefficient 0
#define INIT_FILTER_0_KI_COEF_1 (4000)             // 31:16  KI Coefficient 1
#define INIT_FILTER_0_KI_COEF_2 (5000)             // 15:0   KI Coefficient 2
#define INIT_FILTER_0_KI_COEF_3 (5000)             // 31:16  KI Coefficient 3

#define INIT_FILTER_0_KD_COEF_0 (7500) //(24969)//31457)//-25532)//             // 15:0   KD Coefficient 0
#define INIT_FILTER_0_KD_COEF_1 (15500)             // 31:16  KD Coefficient 1
#define INIT_FILTER_0_KD_COEF_2 (15500)             // 15:0   KD Coefficient 2

#define INIT_FILTER_0_KD_ALPHA_0 (150)//31)//218)//             // 24:16  KD Alpha 1 Value
#define INIT_FILTER_0_KD_ALPHA_1 (150)             // 8:0    KD Alpha 0 Value

#define INIT_FILTER_0_LIMIT0 (15)              // 8:0    Non-Linear Limit 0 Value
#define INIT_FILTER_0_LIMIT1 (25)              // 24:16  Non-Linear Limit 1 Value
#define INIT_FILTER_0_LIMIT2 (31)              // 8:0    Non-Linear Limit 2 Value
#define INIT_FILTER_0_LIMIT3 (31)              // 24:16  Non-Linear Limit 3 Value
#define INIT_FILTER_0_LIMIT4 (31)              // 8:0    Non-Linear Limit 4 Value
#define INIT_FILTER_0_LIMIT5 (31)              // 24:16  Non-Linear Limit 5 Value


#define INIT_FILTER_0_KI_CLAMP_HIGH (0x7FFFFF)       // 23:0   KI Clamp High
#define INIT_FILTER_0_KI_CLAMP_LOW (0)       // 23:0   KI Clamp Low

#define INIT_FILTER_0_YN_CLAMP_HIGH (0x7FFFFF)//Set to 2^23-1;)       // 23:0   Output Clamp High
#define INIT_FILTER_0_YN_CLAMP_LOW ((int)(DT / TL * ((1<<23) - 1) + 0.5))       // 23:0   Output Clamp Low

#define INIT_FILTER_0_OUTPUT_CLAMP_HIGH (0x3FFFF)
#define INIT_FILTER_0_OUTPUT_CLAMP_LOW  (1600) //(0)

#define INIT_FILTER_0_AUTO_GEAR_SHIFT (0)
#define INIT_FILTER_0_AFE_GAIN (3)
#define INIT_FILTER_0_NL_MODE (1)                // written to FILTERCTRL

#define INIT_FILTER_0_KCOMP     (0)
#define INIT_FILTER_0_CLA_SCALE (0)
#define INIT_FILTER_0_YN_SCALE  (0)

//FILTER 0 pmbus controlled coefficients  
#define INIT_FILTER_0_START_UP_BIN0_ALPHA (0)           // 3      Bin0 Alpha Configuration
#define INIT_FILTER_0_START_UP_BIN0_CONFIG (0)          // 2:0    Bin0 Configuration
#define INIT_FILTER_0_START_UP_BIN1_ALPHA (0)           // 7      Bin1 Alpha Configuration
#define INIT_FILTER_0_START_UP_BIN1_CONFIG (0)          // 6:4    Bin1 Configuration
#define INIT_FILTER_0_START_UP_BIN2_ALPHA (0)           // 11     Bin2 Alpha Configuration
#define INIT_FILTER_0_START_UP_BIN2_CONFIG (0)          // 10:8   Bin2 Configuration
#define INIT_FILTER_0_START_UP_BIN3_ALPHA (0)           // 15     Bin3 Alpha Configuration
#define INIT_FILTER_0_START_UP_BIN3_CONFIG (0)          // 14:12  Bin3 Configuration
#define INIT_FILTER_0_START_UP_BIN4_ALPHA (0)           // 19     Bin4 Alpha Configuration
#define INIT_FILTER_0_START_UP_BIN4_CONFIG (0)          // 18:16  Bin4 Configuration
#define INIT_FILTER_0_START_UP_BIN5_ALPHA (0)           // 23     Bin5 Alpha Configuration
#define INIT_FILTER_0_START_UP_BIN5_CONFIG (0)          // 22:20  Bin5 Configuration
#define INIT_FILTER_0_START_UP_BIN6_ALPHA (0)           // 27     Bin6 Alpha Configuration
#define INIT_FILTER_0_START_UP_BIN6_CONFIG (0)          // 26:24  Bin6 Configuration

#define INIT_FILTER_0_START_UP_KP_COEF_0 (12500) //(2486)          // 15:0   KP Coefficient 0
#define INIT_FILTER_0_START_UP_KP_COEF_1 (0)             // 31:16  KP Coefficient 1
#define INIT_FILTER_0_START_UP_KP_COEF_2 (0)             // 15:0   KP Coefficient 2

#define INIT_FILTER_0_START_UP_KI_COEF_0 (2000) //(1374)           // 15:0   KI Coefficient 0
#define INIT_FILTER_0_START_UP_KI_COEF_1 (0)             // 31:16  KI Coefficient 1
#define INIT_FILTER_0_START_UP_KI_COEF_2 (0)             // 15:0   KI Coefficient 2
#define INIT_FILTER_0_START_UP_KI_COEF_3 (0)             // 31:16  KI Coefficient 3

#define INIT_FILTER_0_START_UP_KD_COEF_0 (7500) //(24969)         // 15:0   KD Coefficient 0
#define INIT_FILTER_0_START_UP_KD_COEF_1 (0)             // 31:16  KD Coefficient 1
#define INIT_FILTER_0_START_UP_KD_COEF_2 (0)             // 15:0   KD Coefficient 2

#define INIT_FILTER_0_START_UP_KD_ALPHA_0 (150)          // 24:16  KD Alpha 1 Value
#define INIT_FILTER_0_START_UP_KD_ALPHA_1 (0)            // 8:0    KD Alpha 0 Value

#define INIT_FILTER_0_START_UP_LIMIT0 (0)              // 8:0    Non-Linear Limit 0 Value
#define INIT_FILTER_0_START_UP_LIMIT1 (0)              // 24:16  Non-Linear Limit 1 Value
#define INIT_FILTER_0_START_UP_LIMIT2 (0)              // 8:0    Non-Linear Limit 2 Value
#define INIT_FILTER_0_START_UP_LIMIT3 (0)              // 24:16  Non-Linear Limit 3 Value
#define INIT_FILTER_0_START_UP_LIMIT4 (0)              // 8:0    Non-Linear Limit 4 Value
#define INIT_FILTER_0_START_UP_LIMIT5 (0)              // 24:16  Non-Linear Limit 5 Value


#define INIT_FILTER_0_START_UP_KI_CLAMP_HIGH (0x7FFFFF)       // 23:0   KI Clamp High
#define INIT_FILTER_0_START_UP_KI_CLAMP_LOW (0)       // 23:0   KI Clamp Low

#define INIT_FILTER_0_START_UP_YN_CLAMP_HIGH (0x7FFFFF)//Set to 2^23-1;)       // 23:0   Output Clamp High
#define INIT_FILTER_0_START_UP_YN_CLAMP_LOW ((int)(DT / TL * ((1<<23) - 1) + 0.5))       // 23:0   Output Clamp Low

#define INIT_FILTER_0_START_UP_OUTPUT_CLAMP_HIGH (0x3FFFF)
#define INIT_FILTER_0_START_UP_OUTPUT_CLAMP_LOW  (1600)//(0)

#define INIT_FILTER_0_START_UP_AUTO_GEAR_SHIFT (0)
#define INIT_FILTER_0_START_UP_AFE_GAIN (3)
#define INIT_FILTER_0_START_UP_NL_MODE (1)                // written to FILTERCTRL

#define INIT_FILTER_0_START_UP_KCOMP     (0)
#define INIT_FILTER_0_START_UP_CLA_SCALE (0)
#define INIT_FILTER_0_START_UP_YN_SCALE  (0)

//FILTER 0 pmbus controlled coefficients  
#define INIT_FILTER_0_CP_BIN0_ALPHA (0)           // 3      Bin0 Alpha Configuration
#define INIT_FILTER_0_CP_BIN0_CONFIG (0)          // 2:0    Bin0 Configuration
#define INIT_FILTER_0_CP_BIN1_ALPHA (0)           // 7      Bin1 Alpha Configuration
#define INIT_FILTER_0_CP_BIN1_CONFIG (0)          // 6:4    Bin1 Configuration
#define INIT_FILTER_0_CP_BIN2_ALPHA (0)           // 11     Bin2 Alpha Configuration
#define INIT_FILTER_0_CP_BIN2_CONFIG (0)          // 10:8   Bin2 Configuration
#define INIT_FILTER_0_CP_BIN3_ALPHA (0)           // 15     Bin3 Alpha Configuration
#define INIT_FILTER_0_CP_BIN3_CONFIG (0)          // 14:12  Bin3 Configuration
#define INIT_FILTER_0_CP_BIN4_ALPHA (0)           // 19     Bin4 Alpha Configuration
#define INIT_FILTER_0_CP_BIN4_CONFIG (0)          // 18:16  Bin4 Configuration
#define INIT_FILTER_0_CP_BIN5_ALPHA (0)           // 23     Bin5 Alpha Configuration
#define INIT_FILTER_0_CP_BIN5_CONFIG (0)          // 22:20  Bin5 Configuration
#define INIT_FILTER_0_CP_BIN6_ALPHA (0)           // 27     Bin6 Alpha Configuration
#define INIT_FILTER_0_CP_BIN6_CONFIG (0)          // 26:24  Bin6 Configuration

#define INIT_FILTER_0_CP_KP_COEF_0 (1000)          // 15:0   KP Coefficient 0
#define INIT_FILTER_0_CP_KP_COEF_1 (0)             // 31:16  KP Coefficient 1
#define INIT_FILTER_0_CP_KP_COEF_2 (0)             // 15:0   KP Coefficient 2

#define INIT_FILTER_0_CP_KI_COEF_0 (10)           // 15:0   KI Coefficient 0
#define INIT_FILTER_0_CP_KI_COEF_1 (0)             // 31:16  KI Coefficient 1
#define INIT_FILTER_0_CP_KI_COEF_2 (0)             // 15:0   KI Coefficient 2
#define INIT_FILTER_0_CP_KI_COEF_3 (0)             // 31:16  KI Coefficient 3

#define INIT_FILTER_0_CP_KD_COEF_0 (0)         // 15:0   KD Coefficient 0
#define INIT_FILTER_0_CP_KD_COEF_1 (0)             // 31:16  KD Coefficient 1
#define INIT_FILTER_0_CP_KD_COEF_2 (0)             // 15:0   KD Coefficient 2

#define INIT_FILTER_0_CP_KD_ALPHA_0 (0)          // 24:16  KD Alpha 1 Value
#define INIT_FILTER_0_CP_KD_ALPHA_1 (0)            // 8:0    KD Alpha 0 Value

#define INIT_FILTER_0_CP_LIMIT0 (0)              // 8:0    Non-Linear Limit 0 Value
#define INIT_FILTER_0_CP_LIMIT1 (0)              // 24:16  Non-Linear Limit 1 Value
#define INIT_FILTER_0_CP_LIMIT2 (0)              // 8:0    Non-Linear Limit 2 Value
#define INIT_FILTER_0_CP_LIMIT3 (0)              // 24:16  Non-Linear Limit 3 Value
#define INIT_FILTER_0_CP_LIMIT4 (0)              // 8:0    Non-Linear Limit 4 Value
#define INIT_FILTER_0_CP_LIMIT5 (0)              // 24:16  Non-Linear Limit 5 Value


#define INIT_FILTER_0_CP_KI_CLAMP_HIGH (0x7FFFFF)       // 23:0   KI Clamp High
#define INIT_FILTER_0_CP_KI_CLAMP_LOW (0)       // 23:0   KI Clamp Low

#define INIT_FILTER_0_CP_YN_CLAMP_HIGH (0x7FFFFF)//Set to 2^23-1;)       // 23:0   Output Clamp High
#define INIT_FILTER_0_CP_YN_CLAMP_LOW ((int)(DT / TL * ((1<<23) - 1) + 0.5))       // 23:0   Output Clamp Low

#define INIT_FILTER_0_CP_OUTPUT_CLAMP_HIGH (0x3FFFF)
#define INIT_FILTER_0_CP_OUTPUT_CLAMP_LOW  (1600)//(0)

#define INIT_FILTER_0_CP_AUTO_GEAR_SHIFT (0)
#define INIT_FILTER_0_CP_AFE_GAIN (3)
#define INIT_FILTER_0_CP_NL_MODE (1)                // written to FILTERCTRL

#define INIT_FILTER_0_CP_KCOMP     (0)
#define INIT_FILTER_0_CP_CLA_SCALE (0)
#define INIT_FILTER_0_CP_YN_SCALE  (0)

//FILTER 0 pmbus controlled coefficients  
#define INIT_FILTER_1_BIN0_ALPHA (0)           // 3      Bin0 Alpha Configuration
#define INIT_FILTER_1_BIN0_CONFIG (0)          // 2:0    Bin0 Configuration
#define INIT_FILTER_1_BIN1_ALPHA (0)           // 7      Bin1 Alpha Configuration
#define INIT_FILTER_1_BIN1_CONFIG (0)          // 6:4    Bin1 Configuration
#define INIT_FILTER_1_BIN2_ALPHA (0)           // 11     Bin2 Alpha Configuration
#define INIT_FILTER_1_BIN2_CONFIG (0)          // 10:8   Bin2 Configuration
#define INIT_FILTER_1_BIN3_ALPHA (0)           // 15     Bin3 Alpha Configuration
#define INIT_FILTER_1_BIN3_CONFIG (0)          // 14:12  Bin3 Configuration
#define INIT_FILTER_1_BIN4_ALPHA (0)           // 19     Bin4 Alpha Configuration
#define INIT_FILTER_1_BIN4_CONFIG (0)          // 18:16  Bin4 Configuration
#define INIT_FILTER_1_BIN5_ALPHA (0)           // 23     Bin5 Alpha Configuration
#define INIT_FILTER_1_BIN5_CONFIG (0)          // 22:20  Bin5 Configuration
#define INIT_FILTER_1_BIN6_ALPHA (0)           // 27     Bin6 Alpha Configuration
#define INIT_FILTER_1_BIN6_CONFIG (0)          // 26:24  Bin6 Configuration

#define INIT_FILTER_1_KP_COEF_0 (100)             // 15:0   KP Coefficient 0
#define INIT_FILTER_1_KP_COEF_1 (0)             // 31:16  KP Coefficient 1
#define INIT_FILTER_1_KP_COEF_2 (0)             // 15:0   KP Coefficient 2

#define INIT_FILTER_1_KI_COEF_0 (10)             // 15:0   KI Coefficient 0
#define INIT_FILTER_1_KI_COEF_1 (0)             // 31:16  KI Coefficient 1
#define INIT_FILTER_1_KI_COEF_2 (0)             // 15:0   KI Coefficient 2
#define INIT_FILTER_1_KI_COEF_3 (0)             // 31:16  KI Coefficient 3

#define INIT_FILTER_1_KD_COEF_0 (0)             // 15:0   KD Coefficient 0
#define INIT_FILTER_1_KD_COEF_1 (0)             // 31:16  KD Coefficient 1
#define INIT_FILTER_1_KD_COEF_2 (0)             // 15:0   KD Coefficient 2

#define INIT_FILTER_1_KD_ALPHA_0 (0)             // 24:16  KD Alpha 1 Value
#define INIT_FILTER_1_KD_ALPHA_1 (0)             // 8:0    KD Alpha 0 Value

#define INIT_FILTER_1_LIMIT0 (0)              // 8:0    Non-Linear Limit 0 Value
#define INIT_FILTER_1_LIMIT1 (0)              // 24:16  Non-Linear Limit 1 Value
#define INIT_FILTER_1_LIMIT2 (0)              // 8:0    Non-Linear Limit 2 Value
#define INIT_FILTER_1_LIMIT3 (0)              // 24:16  Non-Linear Limit 3 Value
#define INIT_FILTER_1_LIMIT4 (0)              // 8:0    Non-Linear Limit 4 Value
#define INIT_FILTER_1_LIMIT5 (0)              // 24:16  Non-Linear Limit 5 Value


#define INIT_FILTER_1_KI_CLAMP_HIGH (0x7FFFFF)       // 23:0   KI Clamp High
#define INIT_FILTER_1_KI_CLAMP_LOW (0)       // 23:0   KI Clamp Low

#define INIT_FILTER_1_YN_CLAMP_HIGH (0x7FFFFF)//Set to 2^23-1;)       // 23:0   Output Clamp High
#define INIT_FILTER_1_YN_CLAMP_LOW ((int)(DT / TL * ((1<<23) - 1) + 0.5))       // 23:0   Output Clamp Low

#define INIT_FILTER_1_OUTPUT_CLAMP_HIGH (0x3FFFF)
#define INIT_FILTER_1_OUTPUT_CLAMP_LOW  (1600)//(0)

#define INIT_FILTER_1_AUTO_GEAR_SHIFT (0)
#define INIT_FILTER_1_AFE_GAIN (3) 
#define INIT_FILTER_1_NL_MODE (1)                // written to FILTERCTRL

#define INIT_FILTER_1_KCOMP     (0)
#define INIT_FILTER_1_CLA_SCALE (0)
#define INIT_FILTER_1_YN_SCALE  (0)

#define DEFAULT_FILTER_0_PMBUS_REGS \
{\
	{\
		0,\
	    INIT_FILTER_0_BIN6_ALPHA,\
	    INIT_FILTER_0_BIN6_CONFIG,\
	    INIT_FILTER_0_BIN5_ALPHA,\
	    INIT_FILTER_0_BIN5_CONFIG,\
	    INIT_FILTER_0_BIN4_ALPHA,\
	    INIT_FILTER_0_BIN4_CONFIG,\
	    INIT_FILTER_0_BIN3_ALPHA,\
	    INIT_FILTER_0_BIN3_CONFIG,\
	    INIT_FILTER_0_BIN2_ALPHA,\
	    INIT_FILTER_0_BIN2_CONFIG,\
	    INIT_FILTER_0_BIN1_ALPHA,\
	    INIT_FILTER_0_BIN1_CONFIG,\
	    INIT_FILTER_0_BIN0_ALPHA,\
	    INIT_FILTER_0_BIN0_CONFIG\
	},\
	{\
		INIT_FILTER_0_KP_COEF_1,\
	    INIT_FILTER_0_KP_COEF_0\
	},\
	{\
		0,\
	    INIT_FILTER_0_KP_COEF_2\
	},\
	{\
		INIT_FILTER_0_KI_COEF_1,\
	    INIT_FILTER_0_KI_COEF_0\
	},\
	{\
		INIT_FILTER_0_KI_COEF_3,\
	    INIT_FILTER_0_KI_COEF_2\
	},\
	{\
		INIT_FILTER_0_KD_COEF_1,\
	    INIT_FILTER_0_KD_COEF_0\
	},\
	{\
		0,\
	    INIT_FILTER_0_KD_COEF_2\
	},\
	{\
		0,\
		INIT_FILTER_0_KD_ALPHA_1,\
		0,\
	    INIT_FILTER_0_KD_ALPHA_0\
	},\
	{\
		0,\
		INIT_FILTER_0_LIMIT1,\
		0,\
	    INIT_FILTER_0_LIMIT0\
	},\
	{\
		0,\
		INIT_FILTER_0_LIMIT3,\
		0,\
	    INIT_FILTER_0_LIMIT2\
	},\
	{\
		0,\
		INIT_FILTER_0_LIMIT5,\
		0,\
	    INIT_FILTER_0_LIMIT4\
	},\
	{\
		0,\
	    INIT_FILTER_0_KI_CLAMP_HIGH\
	},\
	{\
		0,\
	    INIT_FILTER_0_KI_CLAMP_LOW\
	},\
	{\
		0,\
	    INIT_FILTER_0_YN_CLAMP_HIGH\
	},\
	{\
		0,\
	    INIT_FILTER_0_YN_CLAMP_LOW\
	},\
	{\
		0,\
	    INIT_FILTER_0_OUTPUT_CLAMP_HIGH\
	},\
	{\
		0,\
	    INIT_FILTER_0_OUTPUT_CLAMP_LOW\
	},\
	{\
		0,\
		INIT_FILTER_0_AUTO_GEAR_SHIFT,\
		INIT_FILTER_0_AFE_GAIN,\
	    INIT_FILTER_0_NL_MODE\
	},\
	{\
		0,\
		INIT_FILTER_0_KCOMP,\
		INIT_FILTER_0_CLA_SCALE,\
		INIT_FILTER_0_YN_SCALE\
	},\
}

#define DEFAULT_FILTER_0_START_UP_PMBUS_REGS \
{\
	{\
		0,\
	    INIT_FILTER_0_START_UP_BIN6_ALPHA,\
	    INIT_FILTER_0_START_UP_BIN6_CONFIG,\
	    INIT_FILTER_0_START_UP_BIN5_ALPHA,\
	    INIT_FILTER_0_START_UP_BIN5_CONFIG,\
	    INIT_FILTER_0_START_UP_BIN4_ALPHA,\
	    INIT_FILTER_0_START_UP_BIN4_CONFIG,\
	    INIT_FILTER_0_START_UP_BIN3_ALPHA,\
	    INIT_FILTER_0_START_UP_BIN3_CONFIG,\
	    INIT_FILTER_0_START_UP_BIN2_ALPHA,\
	    INIT_FILTER_0_START_UP_BIN2_CONFIG,\
	    INIT_FILTER_0_START_UP_BIN1_ALPHA,\
	    INIT_FILTER_0_START_UP_BIN1_CONFIG,\
	    INIT_FILTER_0_START_UP_BIN0_ALPHA,\
	    INIT_FILTER_0_START_UP_BIN0_CONFIG\
	},\
	{\
		INIT_FILTER_0_START_UP_KP_COEF_1,\
	    INIT_FILTER_0_START_UP_KP_COEF_0\
	},\
	{\
		0,\
	    INIT_FILTER_0_START_UP_KP_COEF_2\
	},\
	{\
		INIT_FILTER_0_START_UP_KI_COEF_1,\
	    INIT_FILTER_0_START_UP_KI_COEF_0\
	},\
	{\
		INIT_FILTER_0_START_UP_KI_COEF_3,\
	    INIT_FILTER_0_START_UP_KI_COEF_2\
	},\
	{\
		INIT_FILTER_0_START_UP_KD_COEF_1,\
	    INIT_FILTER_0_START_UP_KD_COEF_0\
	},\
	{\
		0,\
	    INIT_FILTER_0_START_UP_KD_COEF_2\
	},\
	{\
		0,\
		INIT_FILTER_0_START_UP_KD_ALPHA_1,\
		0,\
	    INIT_FILTER_0_START_UP_KD_ALPHA_0\
	},\
	{\
		0,\
		INIT_FILTER_0_START_UP_LIMIT1,\
		0,\
	    INIT_FILTER_0_START_UP_LIMIT0\
	},\
	{\
		0,\
		INIT_FILTER_0_START_UP_LIMIT3,\
		0,\
	    INIT_FILTER_0_START_UP_LIMIT2\
	},\
	{\
		0,\
		INIT_FILTER_0_START_UP_LIMIT5,\
		0,\
	    INIT_FILTER_0_START_UP_LIMIT4\
	},\
	{\
		0,\
	    INIT_FILTER_0_START_UP_KI_CLAMP_HIGH\
	},\
	{\
		0,\
	    INIT_FILTER_0_START_UP_KI_CLAMP_LOW\
	},\
	{\
		0,\
	    INIT_FILTER_0_START_UP_YN_CLAMP_HIGH\
	},\
	{\
		0,\
	    INIT_FILTER_0_START_UP_YN_CLAMP_LOW\
	},\
	{\
		0,\
	    INIT_FILTER_0_START_UP_OUTPUT_CLAMP_HIGH\
	},\
	{\
		0,\
	    INIT_FILTER_0_START_UP_OUTPUT_CLAMP_LOW\
	},\
	{\
		0,\
		INIT_FILTER_0_START_UP_AUTO_GEAR_SHIFT,\
		INIT_FILTER_0_START_UP_AFE_GAIN,\
	    INIT_FILTER_0_START_UP_NL_MODE\
	},\
	{\
		0,\
		INIT_FILTER_0_START_UP_KCOMP,\
		INIT_FILTER_0_START_UP_CLA_SCALE,\
		INIT_FILTER_0_START_UP_YN_SCALE\
	},\
}

#define DEFAULT_FILTER_0_CP_PMBUS_REGS \
{\
	{\
		0,\
	    INIT_FILTER_0_CP_BIN6_ALPHA,\
	    INIT_FILTER_0_CP_BIN6_CONFIG,\
	    INIT_FILTER_0_CP_BIN5_ALPHA,\
	    INIT_FILTER_0_CP_BIN5_CONFIG,\
	    INIT_FILTER_0_CP_BIN4_ALPHA,\
	    INIT_FILTER_0_CP_BIN4_CONFIG,\
	    INIT_FILTER_0_CP_BIN3_ALPHA,\
	    INIT_FILTER_0_CP_BIN3_CONFIG,\
	    INIT_FILTER_0_CP_BIN2_ALPHA,\
	    INIT_FILTER_0_CP_BIN2_CONFIG,\
	    INIT_FILTER_0_CP_BIN1_ALPHA,\
	    INIT_FILTER_0_CP_BIN1_CONFIG,\
	    INIT_FILTER_0_CP_BIN0_ALPHA,\
	    INIT_FILTER_0_CP_BIN0_CONFIG\
	},\
	{\
		INIT_FILTER_0_CP_KP_COEF_1,\
	    INIT_FILTER_0_CP_KP_COEF_0\
	},\
	{\
		0,\
	    INIT_FILTER_0_CP_KP_COEF_2\
	},\
	{\
		INIT_FILTER_0_CP_KI_COEF_1,\
	    INIT_FILTER_0_CP_KI_COEF_0\
	},\
	{\
		INIT_FILTER_0_CP_KI_COEF_3,\
	    INIT_FILTER_0_CP_KI_COEF_2\
	},\
	{\
		INIT_FILTER_0_CP_KD_COEF_1,\
	    INIT_FILTER_0_CP_KD_COEF_0\
	},\
	{\
		0,\
	    INIT_FILTER_0_CP_KD_COEF_2\
	},\
	{\
		0,\
		INIT_FILTER_0_CP_KD_ALPHA_1,\
		0,\
	    INIT_FILTER_0_CP_KD_ALPHA_0\
	},\
	{\
		0,\
		INIT_FILTER_0_CP_LIMIT1,\
		0,\
	    INIT_FILTER_0_CP_LIMIT0\
	},\
	{\
		0,\
		INIT_FILTER_0_CP_LIMIT3,\
		0,\
	    INIT_FILTER_0_CP_LIMIT2\
	},\
	{\
		0,\
		INIT_FILTER_0_CP_LIMIT5,\
		0,\
	    INIT_FILTER_0_CP_LIMIT4\
	},\
	{\
		0,\
	    INIT_FILTER_0_CP_KI_CLAMP_HIGH\
	},\
	{\
		0,\
	    INIT_FILTER_0_CP_KI_CLAMP_LOW\
	},\
	{\
		0,\
	    INIT_FILTER_0_CP_YN_CLAMP_HIGH\
	},\
	{\
		0,\
	    INIT_FILTER_0_CP_YN_CLAMP_LOW\
	},\
	{\
		0,\
	    INIT_FILTER_0_CP_OUTPUT_CLAMP_HIGH\
	},\
	{\
		0,\
	    INIT_FILTER_0_CP_OUTPUT_CLAMP_LOW\
	},\
	{\
		0,\
		INIT_FILTER_0_CP_AUTO_GEAR_SHIFT,\
		INIT_FILTER_0_CP_AFE_GAIN,\
	    INIT_FILTER_0_CP_NL_MODE\
	},\
	{\
		0,\
		INIT_FILTER_0_CP_KCOMP,\
		INIT_FILTER_0_CP_CLA_SCALE,\
		INIT_FILTER_0_CP_YN_SCALE\
	},\
}

#define DEFAULT_FILTER_1_PMBUS_REGS \
{\
	{\
		0,\
	    INIT_FILTER_1_BIN6_ALPHA,\
	    INIT_FILTER_1_BIN6_CONFIG,\
	    INIT_FILTER_1_BIN5_ALPHA,\
	    INIT_FILTER_1_BIN5_CONFIG,\
	    INIT_FILTER_1_BIN4_ALPHA,\
	    INIT_FILTER_1_BIN4_CONFIG,\
	    INIT_FILTER_1_BIN3_ALPHA,\
	    INIT_FILTER_1_BIN3_CONFIG,\
	    INIT_FILTER_1_BIN2_ALPHA,\
	    INIT_FILTER_1_BIN2_CONFIG,\
	    INIT_FILTER_1_BIN1_ALPHA,\
	    INIT_FILTER_1_BIN1_CONFIG,\
	    INIT_FILTER_1_BIN0_ALPHA,\
	    INIT_FILTER_1_BIN0_CONFIG\
	},\
	{\
		INIT_FILTER_1_KP_COEF_1,\
	    INIT_FILTER_1_KP_COEF_0\
	},\
	{\
		0,\
	    INIT_FILTER_1_KP_COEF_2\
	},\
	{\
		INIT_FILTER_1_KI_COEF_1,\
	    INIT_FILTER_1_KI_COEF_0\
	},\
	{\
		INIT_FILTER_1_KI_COEF_3,\
	    INIT_FILTER_1_KI_COEF_2\
	},\
	{\
		INIT_FILTER_1_KD_COEF_1,\
	    INIT_FILTER_1_KD_COEF_0\
	},\
	{\
		0,\
	    INIT_FILTER_1_KD_COEF_2\
	},\
	{\
		0,\
		INIT_FILTER_1_KD_ALPHA_1,\
		0,\
	    INIT_FILTER_1_KD_ALPHA_0\
	},\
	{\
		0,\
		INIT_FILTER_1_LIMIT1,\
		0,\
	    INIT_FILTER_1_LIMIT0\
	},\
	{\
		0,\
		INIT_FILTER_1_LIMIT3,\
		0,\
	    INIT_FILTER_1_LIMIT2\
	},\
	{\
		0,\
		INIT_FILTER_1_LIMIT5,\
		0,\
	    INIT_FILTER_1_LIMIT4\
	},\
	{\
		0,\
	    INIT_FILTER_1_KI_CLAMP_HIGH\
	},\
	{\
		0,\
	    INIT_FILTER_1_KI_CLAMP_LOW\
	},\
	{\
		0,\
	    INIT_FILTER_1_YN_CLAMP_HIGH\
	},\
	{\
		0,\
	    INIT_FILTER_1_YN_CLAMP_LOW\
	},\
	{\
		0,\
	    INIT_FILTER_1_OUTPUT_CLAMP_HIGH\
	},\
	{\
		0,\
	    INIT_FILTER_1_OUTPUT_CLAMP_LOW\
	},\
	{\
		0,\
		INIT_FILTER_1_AUTO_GEAR_SHIFT,\
		INIT_FILTER_1_AFE_GAIN,\
	    INIT_FILTER_1_NL_MODE\
	},\
	{\
		0,\
		INIT_FILTER_1_KCOMP,\
		INIT_FILTER_1_CLA_SCALE,\
		INIT_FILTER_1_YN_SCALE\
	},\
}

EXTERN FILTER_PMBUS_REGS filter0_pmbus_regs;
EXTERN FILTER_PMBUS_REGS filter0_start_up_pmbus_regs;
EXTERN FILTER_PMBUS_REGS filter0_cp_pmbus_regs;
EXTERN FILTER_PMBUS_REGS filter1_pmbus_regs;
EXTERN FILTER_PMBUS_REGS *filter_destination;



void copy_coefficients_to_filter(volatile struct FILTER_REGS *dest, const FILTER_PMBUS_REGS *source);
void copy_coefficients_to_ram(volatile FILTER_PMBUS_REGS *dest, volatile struct FILTER_REGS *source);
void configure_new_compensation(volatile FILTER_PMBUS_REGS *source);


