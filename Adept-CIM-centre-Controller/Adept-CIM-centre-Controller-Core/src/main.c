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
#include "cim_centre_io.h"
#include "console.h"

int main (void)
{
	sysclk_init();
	board_init();

	configure_console();
	
	//Initialize io pins on the bus
	io_init();

	while(1){
		usart_write_line(CONF_UART, "TEST\r\n");
		//usart_putchar(CONF_UART, 'b');
		//uint32_t dw_status = usart_get_status(CONF_UART);
		//if (dw_status & US_CSR_RXRDY) {
			//uint32_t received_byte;
			//usart_read(CONF_UART, &received_byte);
			//usart_write_line(CONF_UART, received_byte);
		//}
	}
}
