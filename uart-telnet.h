#ifndef uart_telnet_h
#define uart_telnet_h

#include <avr/io.h>

#include <stdint.h>

inline uint8_t uart_transmit_ready(void)
{
	if(UCSR0B & _BV(UDRIE0))
		return(0);

	return(1);
}

inline uint8_t uart_receive_ready(void)
{
	if(UCSR0B & _BV(RXCIE0))
		return(0);

	return(1);
}

void uart_init(void);
uint8_t uart_transmit(const uint8_t *buffer);
uint8_t uart_receive(uint16_t size, uint8_t *buffer);

#endif