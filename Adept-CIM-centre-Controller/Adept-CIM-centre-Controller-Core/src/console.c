/*
 * console.c
 *
 * Created: 29/11/2020 8:17:39 PM
 *  Author: jbonwick
 */ 
#include "asf.h"
#include "console.h"

void configure_console(void)
{
	const sam_usart_opt_t usart_console_settings = {
		CONF_UART_BAUDRATE,
		CONF_UART_CHAR_LENGTH,
		CONF_UART_PARITY,
		CONF_UART_STOP_BITS,
		US_MR_CHMODE_NORMAL
	};
	sysclk_enable_peripheral_clock(CONF_UART_ID);
	usart_init_rs232(CONF_UART, &usart_console_settings, sysclk_get_peripheral_hz());
	usart_enable_tx(CONF_UART);
	usart_enable_rx(CONF_UART);
	
	//usart_enable_interrupt(CONF_UART, US_IER_RXRDY);
	
	//NVIC_EnableIRQ(USART0_IRQn);
}