/**
* \file
*
* \brief Empty user application template
*
*/

/**
* \mainpage User Application template doxygen documentation
*
* \par Empty user application template
*
* Bare minimum empty user application template
*
* \par Content
*
* -# Include the ASF header files (through asf.h)
* -# "Insert system clock initialization code here" comment
* -# Minimal main function that starts with a call to board_init()
* -# "Insert application code here" comment
*
*/

/*
* Include header files for all drivers that have been imported from
* Atmel Software Framework (ASF).
*/
/*
* Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
*/
#include <asf.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "cim_centre_io.h"
#include "console.h"

void updateEncoder(long int *pos, uint16_t encoderCounter);

//Current and previous positions of all axis
long int fb_pos = 0;
long int lr_pos = 0;
long int ud_pos = 0;

int main (void)
{
	sysclk_init();
	board_init();

	configure_console();
	
	//Initialize io pins on the bus
	io_init();

	while(1){
		//Read quadrature encoders
		updateEncoder(&fb_pos, readEncoder(encoderFB));
		updateEncoder(&lr_pos, readEncoder(encoderLR));
		updateEncoder(&ud_pos, readEncoder(encoderUD));
		
		//Hopefully this prints the correct values
		char consoleBuffer[100];
		sprintf(consoleBuffer, "FB: %ld\r\n", fb_pos);
		usart_write_line(CONF_UART, consoleBuffer);
		sprintf(consoleBuffer, "LR: %ld\r\n", lr_pos);
		usart_write_line(CONF_UART, consoleBuffer);
		sprintf(consoleBuffer, "UD: %ld\r\n", ud_pos);
		usart_write_line(CONF_UART, consoleBuffer);
		for (int i = 0; i <= 90000; i++)
		{
			__asm__ __volatile__ ("nop");
		}
		//usart_putchar(CONF_UART, 'b');
		//uint32_t dw_status = usart_get_status(CONF_UART);
		//if (dw_status & US_CSR_RXRDY) {
		//uint32_t received_byte;
		//usart_read(CONF_UART, &received_byte);
		//usart_write_line(CONF_UART, received_byte);
		//}
	}
}

void updateEncoder(long int *pos, uint16_t encoderCounter){
	//Check for rollover
	if(abs((*pos & 0xFFFF) - encoderCounter) > 30000){
		//Check if roll down
		long int temp = *pos >> 16;
		if(encoderCounter & 0x8000){
			temp--;
		}
		else{
			temp++;
		}
		*pos = temp << 16;
		*pos |= encoderCounter;
	}
	else{
		*pos &= 0xFFFF0000;
		*pos |= encoderCounter;
	}
}