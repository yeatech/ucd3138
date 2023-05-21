//###########################################################################
//
// FILE:    uart.c
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
#include "cyclone_device.h"     // UCD30xx Headers Include File
#include "software_interrupts.h"
#include "system_defines.h"
#include "variables.h"

void init_uart0(void)
{
	volatile Uint32 rx_byte0; //volatile to make warning go away about set but not read

	Uart0Regs.UARTCTRL3.bit.SW_RESET = 0; //software reset while initializing UART

	Uart0Regs.UARTCTRL0.bit.DATA_SIZE = 7; //8 bits
	Uart0Regs.UARTCTRL0.bit.STOP = 1; //2 stop bits
	Uart0Regs.UARTCTRL0.bit.SYNC_MODE = 1; //asynchronous mode

	Uart0Regs.UARTHBAUD.all = 0;
	Uart0Regs.UARTMBAUD.all = 0;
	Uart0Regs.UARTLBAUD.all = 50;  //for 38400 //47 for control board, 44 for open loop

	Uart0Regs.UARTRXST.bit.RX_ENA = 1 ;//enable RX

	Uart0Regs.UARTTXST.bit.TX_ENA = 1;//enable TX

	Uart0Regs.UARTINTST.all = 0xff;  //these two statements are supposed to clear the status bits
	Uart0Regs.UARTINTST.all = 0;

	rx_byte0 = Uart0Regs.UARTRXBUF.all; //clear RXRDY flag

	Uart0Regs.UARTIOCTRLTX.bit.IO_FUNC = 1; //enable transmit pin
	Uart0Regs.UARTIOCTRLRX.bit.IO_FUNC = 1; //enable receive pin

	Uart0Regs.UARTCTRL3.bit.CLOCK = 1; //internal clock select;
	Uart0Regs.UARTCTRL3.bit.SW_RESET = 1; //software reset released UART init done?

	Uart0Regs.UARTIOCTRLSCLK.bit.IO_FUNC = 0; //disable external clock for UART.

	Uart0Regs.UARTTXBUF.all = '\n'; //put out a byte to get things started.
}

void init_uart1(void)
{
	volatile Uint32 rx_byte1; //volatile to make warning go away about set but not read

	Uart1Regs.UARTCTRL3.bit.SW_RESET = 0; //software reset while initializing UART

	Uart1Regs.UARTCTRL0.bit.DATA_SIZE = 7; //8 bits
	Uart1Regs.UARTCTRL0.bit.STOP = 1; //2 stop bits
	Uart1Regs.UARTCTRL0.bit.SYNC_MODE = 1; //asynchronous mode

	Uart1Regs.UARTHBAUD.all = 0;
	Uart1Regs.UARTMBAUD.all = 0;
	Uart1Regs.UARTLBAUD.all = 50;  //for 38400 //47 for control board, 44 for open loop

	Uart1Regs.UARTRXST.bit.RX_ENA = 1 ;//enable RX

	Uart1Regs.UARTTXST.bit.TX_ENA = 1;//enable TX

	Uart1Regs.UARTINTST.all = 0xff;  //these two statements are supposed to clear the status bits
	Uart1Regs.UARTINTST.all = 0;

	rx_byte1 = Uart1Regs.UARTRXBUF.all; //clear RXRDY flag

	Uart1Regs.UARTIOCTRLTX.bit.IO_FUNC = 1; //enable transmit pin
	Uart1Regs.UARTIOCTRLRX.bit.IO_FUNC = 1; //enable receive pin

	Uart1Regs.UARTCTRL3.bit.CLOCK = 1; //internal clock select;
	Uart1Regs.UARTCTRL3.bit.SW_RESET = 1; //software reset released UART init done?

	Uart1Regs.UARTIOCTRLSCLK.bit.IO_FUNC = 0; //disable external clock for UART.

	Uart1Regs.UARTTXBUF.all = '\n'; //put out a byte to get things started.
}

char translate_nybble_out(char byte,char * pchar2 )
{
	char char1;
	char1 = (byte & 0xf0) >> 4;
	*pchar2 = (byte & 0x0f);
	if (char1 < 10)
	{
		char1 +='0';
	}else if ((char1 >= 0xa) && (char1 <= 0xf))
	{
		char1 = char1 + 'a' - 10;
	}
	if (*pchar2 < 10)
	{
		*pchar2 +='0';
	}else if ((*pchar2 >= 0xa) && (*pchar2 <= 0xf))
	{
		*pchar2 = *pchar2 + 'a' - 10;
	}
	return char1;
}

void translate_raw_to_text(void)
{
	Uint16 i;
	Uint8 char2;
	for(i = 0; i < UART_TX_SIZE ;i++)
	{ 
		uart_text_tx_buf[2 * i] =  translate_nybble_out(uart_tx_buf[i], &char2);
		uart_text_tx_buf[(2 * i)+1] = char2;
	}
}

void uart_transmit_data(void)
{
	static Uint8 previous_pfc_command;

	if(uart_tx_data_rdy == 0) //if previous packet is already sent out, assembly the new data packet 
	{
		Uint8 i;

		pfc_command = 4 * pfc_phase_2_enable + 2 * pfc_zvs_enable + pfc_os_enable;

		if(pfc_command ^ previous_pfc_command)
		{	
			switch (pfc_command)
			{
			case 0:
				{
					uart_tx_buf[0] = 0x40; // Disable Phase B
					uart_tx_buf[1] = 0xBE; // DC bus volts: 390 - 200 = 190 = 0xBE
					uart_tx_buf[2] = 0x2D; // Switching frequency: 100 - 55 = 45 = 0x2D
					uart_tx_buf[3] = 0x00; // Disable ZVS
					uart_tx_buf[4] = 0x01; // Disable over sampling
					uart_tx_buf[5] = 0x00;
					uart_tx_buf[6] = 0x00;
					uart_tx_buf[7] = 0x00;
					uart_tx_buf[8] = 0x00;
					break;
				}
			case 1:
				{
					uart_tx_buf[0] = 0x40; // Disable Phase B
					uart_tx_buf[1] = 0xBE; // DC bus volts: 390 - 200 = 190 = 0xBE
					uart_tx_buf[2] = 0x2D; // Switching frequency: 100 - 55 = 45 = 0x2D
					uart_tx_buf[3] = 0x00; // Disable ZVS
					uart_tx_buf[4] = 0x00; // Enable over sampling
					uart_tx_buf[5] = 0x00;
					uart_tx_buf[6] = 0x00;
					uart_tx_buf[7] = 0x00;
					uart_tx_buf[8] = 0x00;
					break;
				}
			case 2:
				{
					uart_tx_buf[0] = 0x40; // Disable Phase B
					uart_tx_buf[1] = 0xBE; // DC bus volts: 390 - 200 = 190 = 0xBE
					uart_tx_buf[2] = 0x2D; // Switching frequency: 100 - 55 = 45 = 0x2D
					uart_tx_buf[3] = 0x01; // Enable ZVS
					uart_tx_buf[4] = 0x01; // Disable over sampling
					uart_tx_buf[5] = 0x00;
					uart_tx_buf[6] = 0x00;
					uart_tx_buf[7] = 0x00;
					uart_tx_buf[8] = 0x00;
					break;
				}
			case 3:
				{
					uart_tx_buf[0] = 0x40; // Disable Phase B
					uart_tx_buf[1] = 0xBE; // DC bus volts: 390 - 200 = 190 = 0xBE
					uart_tx_buf[2] = 0x2D; // Switching frequency: 100 - 55 = 45 = 0x2D
					uart_tx_buf[3] = 0x01; // Enable ZVS
					uart_tx_buf[4] = 0x00; // Enable over sampling
					uart_tx_buf[5] = 0x00;
					uart_tx_buf[6] = 0x00;
					uart_tx_buf[7] = 0x00;
					uart_tx_buf[8] = 0x00;
					break;
				}
			case 4:
				{
					uart_tx_buf[0] = 0x00; // Enable Phase B
					uart_tx_buf[1] = 0xBE; // DC bus volts: 390 - 200 = 190 = 0xBE
					uart_tx_buf[2] = 0x2D; // Switching frequency: 100 - 55 = 45 = 0x2D
					uart_tx_buf[3] = 0x00; // Disable ZVS
					uart_tx_buf[4] = 0x01; // Disable over sampling
					uart_tx_buf[5] = 0x00;
					uart_tx_buf[6] = 0x00;
					uart_tx_buf[7] = 0x00;
					uart_tx_buf[8] = 0x00;
					break;
				}
			case 5:
				{
					uart_tx_buf[0] = 0x00; // Enable Phase B
					uart_tx_buf[1] = 0xBE; // DC bus volts: 390 - 200 = 190 = 0xBE
					uart_tx_buf[2] = 0x2D; // Switching frequency: 100 - 55 = 45 = 0x2D
					uart_tx_buf[3] = 0x00; // Disable ZVS
					uart_tx_buf[4] = 0x00; // Enable over sampling
					uart_tx_buf[5] = 0x00;
					uart_tx_buf[6] = 0x00;
					uart_tx_buf[7] = 0x00;
					uart_tx_buf[8] = 0x00;
					break;
				}
			case 6:
				{
					uart_tx_buf[0] = 0x00; // Enable Phase B
					uart_tx_buf[1] = 0xBE; // DC bus volts: 390 - 200 = 190 = 0xBE
					uart_tx_buf[2] = 0x2D; // Switching frequency: 100 - 55 = 45 = 0x2D
					uart_tx_buf[3] = 0x01; // Enable ZVS
					uart_tx_buf[4] = 0x01; // Disable over sampling
					uart_tx_buf[5] = 0x00;
					uart_tx_buf[6] = 0x00;
					uart_tx_buf[7] = 0x00;
					uart_tx_buf[8] = 0x00;
					break;
				}
			case 7:
				{
					uart_tx_buf[0] = 0x00; // Enable Phase B
					uart_tx_buf[1] = 0xBE; // DC bus volts: 390 - 200 = 190 = 0xBE
					uart_tx_buf[2] = 0x2D; // Switching frequency: 100 - 55 = 45 = 0x2D
					uart_tx_buf[3] = 0x01; // Enable ZVS
					uart_tx_buf[4] = 0x00; // Enable over sampling
					uart_tx_buf[5] = 0x00;
					uart_tx_buf[6] = 0x00;
					uart_tx_buf[7] = 0x00;
					uart_tx_buf[8] = 0x00;
					break;
				}
			}		
			uart_tx_buf[UART_TX_SIZE-1] = 0; //uart_tx_buf[9] = 0xF4; // Checksum	
			for(i=0; i<(UART_TX_SIZE-1); i++) //calculate checksum 
			{
				uart_tx_buf[UART_TX_SIZE-1] += uart_tx_buf[i]; //put checksum into the last buffer
			}
			translate_raw_to_text(); // Translate raw content of "uart_tx_buf" into text in "uart_text_tx_buf"
			uart_text_tx_buf[2 * UART_TX_SIZE] = '\n';
			uart_text_tx_buf[(2 * UART_TX_SIZE) + 1] = '\r';

			uart_tx_data_rdy = 1; //set flag, new data packet is ready for transmitting
			previous_pfc_command = pfc_command;
		}
	}
	else //send the new data packet
	{
		if(uart_tx_timeout >= 1000) //if 100ms has passed since last transmit
		{
			if(Uart0Regs.UARTTXST.bit.TX_RDY == 1) 
			{
				//Uart1Regs.UARTTXBUF.all = uart_tx_buf[uart_tx_buf_ptr]; //send out a byte and clear TX_RDY flag
				Uart0Regs.UARTTXBUF.all = uart_text_tx_buf[uart_tx_buf_ptr]; //send out a byte and clear TX_RDY flag
				uart_tx_buf_ptr ++; //point to next byte
				//if(uart_tx_buf_ptr == UART_TX_SIZE) //all bytes are sent out
				if(uart_tx_buf_ptr == (UART_TX_SIZE*2) + 2) //all bytes are sent out
				{
					uart_tx_buf_ptr = 0;  //reset the pointer
					uart_tx_data_rdy = 0; //clear flag, so background can start to prepare the next data packet 
					uart_tx_timeout = 0;    //reset the timeout
				}
			}
		}
	}
}

//process received data from UART
void uart_process_rx_data(void)
{
	if(uart_rx_data_rdy == 1) //if received a new data packet
	{
		Uint8 checksum, i;

		checksum = 0;
		for(i=0; i<(UART_RX_SIZE-1); i++)  //calculate checksum
		{
			checksum += uart_rx_buf[i];
		}

		if(checksum == uart_rx_buf[UART_RX_SIZE-1]) //process received data if checksum is correct
		{
			pfc_out_struct.pfc_status0 = uart_rx_buf[0];
			pfc_out_struct.pfc_status1 = uart_rx_buf[1];
			// 45V offset to extend the range (255 in UART buffer means 300V)
			pfc_out_struct.pfc_vac = uart_rx_buf[2] + 45;
			pfc_out_struct.pfc_iac = uart_rx_buf[3] + (uart_rx_buf[2] << 8);
			if(pfc_out_struct.pfc_status1 & 0x02)// Status 1, bit 1: 0=Input Power/ 1=Output Power
			{
				pfc_out_struct.pfc_pout = ((((unsigned short)uart_rx_buf[4])) << 8) + ((unsigned short)uart_rx_buf[5] & 0x00ff);
			}
			else
			{
				pfc_out_struct.pfc_pin = ((((unsigned short)uart_rx_buf[4])) << 8) + ((unsigned short)uart_rx_buf[5] & 0x00ff);
			}
			// 200V offset to extend the range (255 in UART buffer means 455V)
			pfc_out_struct.pfc_vdc_bus = uart_rx_buf[6] + 200;
			pfc_out_struct.pfc_sw_frequency = uart_rx_buf[7];
			// 0-255 range -- 0 -> -50 degrees C , 255 -> 205 degrees C
			pfc_out_struct.pfc_temperature = uart_rx_buf[8] - 50;
		}

		uart_rx_data_rdy = 0; //clear flag, now it's ready to receive next data packet
	}
}

char translate_nybble_in(char nyb)
{
	if((nyb >= '0') && (nyb <= '9'))
	{
		return nyb - '0';
	}
	else if((nyb >= 'a') && (nyb <= 'f'))
	{
		return nyb - 'a' + 10;
	}
	else if((nyb >= 'A') && (nyb <= 'F'))
	{
		return nyb - 'A' + 10;
	}
	else
	{
		return 0;
	}
}
 
void translate_text_to_raw(void)
{
	//translates text serial in buffer to raw one.
	int i; //loop counter, index into input array
	int j = 0;

	for(i = 0; i < (UART_RX_SIZE *2);i++)
	{
		uart_rx_buf[j] = translate_nybble_in(uart_text_rx_buf[i]) << 4; //high nybble
		i++;
		uart_rx_buf[j] = translate_nybble_in(uart_text_rx_buf[i]) + uart_rx_buf[j]; //add in low nybble
		j++;
	}
}

//for frimary and secdonary communication
void uart_receive_data(void)
{
	//count for how much time has passed since the last packet been sent out
	if(uart_tx_timeout <= 10000) //1000ms 
	{
		uart_tx_timeout++;
	}

	//receive data, one byte a time
	if(uart_rx_data_rdy == 0) 
	{
		if(Uart0Regs.UARTRXST.bit.RX_RDY == 1)
		{
			//unsigned char i;
			//uart_rx_buf[uart_rx_buf_ptr] =  Uart1Regs.UARTRXBUF.byte.RXDAT; //read a byte and clear RX_RDY flag
			uart_text_rx_buf[uart_rx_buf_ptr] =  Uart0Regs.UARTRXBUF.bit.RXDAT; //read a byte and clear RX_RDY flag

			uart_rx_buf_ptr ++; //point to next byte
			uart_rx_timeout = 0; 

			//if(uart_rx_buf_ptr == UART_RX_SIZE) //received all bytes
			if(uart_rx_buf_ptr == ((UART_RX_SIZE * 2) + 2)) //received all bytes
			{
				//unsigned char i;
				uart_rx_buf_ptr = 0; //reset the pointer
				uart_rx_data_rdy = 1; //set flag to let background process received data
				translate_text_to_raw();
				/*					 
				    char_out('\n');
				    char_out('\r');
				    for(i =0;i < UART_RX_SIZE; i++)
				     {
				      byte_out_space(uart_rx_buf[i]);
				     }
				     char_out('\n');
				     char_out('\r');
				*/
			}
		}
		else
		{
			uart_rx_timeout ++;

			if(uart_rx_timeout > (10000 - 200))//if we didn't receive data for more than 80ms, the data packet is invalid, start over
			{
				uart_rx_buf_ptr = 0;
			}
		}
	}
}


void char_out(char data)
{
	while(Uart0Regs.UARTTXST.bit.TX_RDY == 0)
	{
		//do nothing while waiting for data to transmit.
	}
	Uart0Regs.UARTTXBUF.all = data; //put out a byte
}

