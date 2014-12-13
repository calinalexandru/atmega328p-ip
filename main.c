#include "spi.h"
#include "twi_master.h"
#include "uart.h"
#include "net.h"
#include "ethernet.h"
#include "arp.h"
#include "ipv4.h"
#include "bootp.h"
#include "stats.h"
#include "eeprom.h"
#include "enc.h"
#include "application.h"
#include "util.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>

#include <string.h>
#include <stdint.h>

enum
{
	max_frame_size = 384,
};

static ipv4_addr_t	my_ipv4_address;
static mac_addr_t	my_mac_address;

ISR(WDT_vect)
{
	wd_interrupts++;
}

int main(void)
{
	uint8_t 		rx_frame[max_frame_size];
	uint8_t			tx_frame[max_frame_size];

	uint16_t		rx_frame_length;
	uint16_t		tx_frame_length;

	cli();
    wdt_reset();
    MCUSR = 0;
    wdt_disable();

	PRR = 0xff;

#if 0
	b0		/cs				b0		O	square green				spi ss
	b1						oc1a	O	small red					DEBUG 4
	b2		(/ss)			oc1b	O	small green					DEBUG 3
	b3		mosi					O	square green				spi mosi
	b4		miso					I	square red					spi miso
	b5		sck						O	square red					spi sck
	b6						XTAL1
	b7						XTAL2

	c0						adc0	I								tmp36
	c1						c1		O								*
	c2						c2		O								*
	c3						c3		O								*
	c4				sda				O								i2c sda
	c5				scl				O								i2c scl
	c6						RESET

	d0						RXD
	d1						TXD
	d2						int0	I								enc int
	d3						d3		O	rectangular green			DEBUG 1
	d4		 				d4		O	small red					DEBUG 2
	d5						d5		O	large green					heartbeat
	d6						d6		O	large red					cmd
	d7						d7		O								ESP8266 /RESET
#endif

	MCUCR	|= _BV(PUD);		//	disable pullups
	DDRB	= _BV(0) | _BV(1) | _BV(2) | _BV(3) | _BV(5);
	DDRC	= _BV(1) | _BV(2) | _BV(3) | _BV(4) | _BV(5);
	DDRD	= _BV(3) | _BV(4) | _BV(5) | _BV(6) | _BV(7);

	eeprom_read_mac_address(&my_mac_address);

	my_ipv4_address.byte[0] = 0;
	my_ipv4_address.byte[1] = 0;
	my_ipv4_address.byte[2] = 0;
	my_ipv4_address.byte[3] = 0;

	PORTD = 0;
	sleep(200);
	PORTD = _BV(5);
	sleep(200);
	PORTD = _BV(6);
	sleep(200);
	PORTD = 0;

	wdt_enable(WDTO_2S);
	spi_init();
	twi_master_init();
	uart_init();
	uart_baud(75000);
	enc_init(max_frame_size, &my_mac_address);
	enc_set_led(PHLCON_LED_RCV, PHLCON_LED_XMIT);

	PORTD &= ~_BV(7); // reset ESP8266
	sleep(200);
	PORTD |= _BV(7);

	application_init();

	sei();

	for(;;)
	{
		pause_idle();			// gets woken by the 122 Hz timer1 interrupt or packet arrival or watchdog interrupt
		WDTCSR |= _BV(WDIE);	// enable wdt interrupt, reset
		application_periodic();	// run periodic tasks

		if(ipv4_address_match(&my_ipv4_address, &ipv4_addr_zero))
		{
			tx_frame_length = bootp_create_request(tx_frame, &my_mac_address);
			enc_send_frame(tx_frame, tx_frame_length);
		}

		if((rx_frame_length = enc_receive_frame(rx_frame, sizeof(rx_frame))) == 0)
			continue;

		if((tx_frame_length = ethernet_process_frame(rx_frame, rx_frame_length, tx_frame, sizeof(tx_frame), &my_mac_address, &my_ipv4_address)) == 0)
			continue;

		enc_send_frame(tx_frame, tx_frame_length);
	}
}
