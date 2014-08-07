#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <avr/io.h>
#include <avr/sleep.h>

#include "spi.h"
#include "twi_master.h"
#include "timer1.h"
#include "net.h"
#include "ethernet.h"
#include "arp.h"
#include "ipv4.h"
#include "watchdog.h"
#include "stats.h"
#include "eeprom.h"
#include "enc.h"

enum
{
	adc_warmup_init = 8,
	max_frame_size = 384,
	watchdog_prescaler = WATCHDOG_PRESCALER_256,
};

typedef struct
{
	uint16_t	multiplier;
	uint16_t	offset;
} temperature_calibration_t;

typedef struct
{
	temperature_calibration_t	temp_cal_data[8];
	mac_addr_t					my_mac_address;
	ipv4_addr_t					my_ipv4_address;
} eeprom_t;

static const eeprom_t *eeprom = (eeprom_t *)0;

static ipv4_addr_t my_ipv4_address;
static mac_addr_t my_mac_address;

static void sleep(uint16_t tm)
{
	while(tm-- > 0)
	{
		_delay_ms(1);
		watchdog_reset();
	}
}

ISR(WDT_vect, ISR_NOBLOCK)
{
	static uint8_t phase1 = 0;

	timer1_set_oc1a(_BV(phase1));
	timer1_set_oc1b(_BV(15 - phase1));

	if(++phase1 > 15)
		phase1 = 0;

	wd_interrupts++;
}

ISR (INT0_vect, ISR_NOBLOCK)
{
	eth_interrupts++;
}

int main(void)
{
	static	uint8_t 		rx_frame[max_frame_size];
	static	uint8_t			tx_frame[max_frame_size];

	static	uint16_t		rx_frame_length;
	static	uint16_t		tx_frame_length;
	static	uint16_t		tx_payload_length;

	//static struct
	//{
		//unsigned int bootp_done:1;
	//} flags = { 0 };

	//static uint16_t boot_timer = 0;

	cli();
	watchdog_stop();

	PRR = 0xff;

#if 0
	b0		/cs				b0		O	square green				spi ss
	b1						oc1a	O	small red					light
	b2		(/ss)			oc1b	O	small green					ir
	b3		mosi					O	square green				spi mosi
	b4		miso					I	square red					spi miso
	b5		sck						O	square red					spi sck
	b6						b6		O	small orange				led spi
	b7						b7		I								button light down

	c0						adc0	I								tmp36
	c1						c1		O								*
	c2						c2		O								*
	c3						c3		O								*
	c4				sda				O								i2c sda
	c5				scl				O								i2c scl
	c6						RESET	O

	d0						d0		O	small transparent green		led heartbeat
	d1						d1		O	small transparent red		led command
	d2						int0	I								enc int
	d3						d3		O	rectangular green			DEBUG 1
	d4		 				d4		O	small red					led i2c
	d5						d5		O	large green					DEBUG 2
	d6						d6		O	large red					DEBUG 3
	d7						d7		I								button light up
#endif

	MCUCR	|= _BV(PUD);		//	disable pullups
	DDRB	= _BV(0) | _BV(1) | _BV(2) | _BV(3) | _BV(5) | _BV(6);
	DDRC	= _BV(1) | _BV(2) | _BV(3) | _BV(4) | _BV(5) | _BV(6);
	DDRD	= _BV(0) | _BV(1) | _BV(3) | _BV(4) | _BV(5) | _BV(6);
	EICRA	= _BV(ISC01);		//	INT0 falling edge
	EIMSK	= _BV(INT0);		//	enable INT0

	set_sleep_mode(SLEEP_MODE_IDLE);

	my_mac_address.byte[0] = eeprom_read_uint8(&eeprom->my_mac_address.byte[0]);
	my_mac_address.byte[1] = eeprom_read_uint8(&eeprom->my_mac_address.byte[1]);
	my_mac_address.byte[2] = eeprom_read_uint8(&eeprom->my_mac_address.byte[2]);
	my_mac_address.byte[3] = eeprom_read_uint8(&eeprom->my_mac_address.byte[3]);
	my_mac_address.byte[4] = eeprom_read_uint8(&eeprom->my_mac_address.byte[4]);
	my_mac_address.byte[5] = eeprom_read_uint8(&eeprom->my_mac_address.byte[5]);

	my_ipv4_address.byte[0] = eeprom_read_uint8(&eeprom->my_ipv4_address.byte[0]);
	my_ipv4_address.byte[1] = eeprom_read_uint8(&eeprom->my_ipv4_address.byte[1]);
	my_ipv4_address.byte[2] = eeprom_read_uint8(&eeprom->my_ipv4_address.byte[2]);
	my_ipv4_address.byte[3] = eeprom_read_uint8(&eeprom->my_ipv4_address.byte[3]);

	PORTD = _BV(0);
	sleep(1000);
	PIND = _BV(0) | _BV(1);
	sleep(1000);
	PIND = _BV(0) | _BV(1);

	timer1_init(timer1_1);	// pwm timer 1 resolution: 16 bits, frequency = 122 Hz

	spi_init();
	twi_master_init();
	enc_init(max_frame_size, &my_mac_address);
	enc_set_led(PHLCON_LED_RCV, PHLCON_LED_XMIT);

	watchdog_start(watchdog_prescaler);
	sei();

	timer1_start();

	for(;;)
	{
		//if(!bootp_done && (bootp_timer == 0))
		//{
		
		if((rx_frame_length = ethernet_receive_frame(rx_frame, sizeof(rx_frame))) == 0)
			continue;

		if((tx_frame_length = ethernet_process_frame(rx_frame, rx_frame_length, tx_frame, sizeof(tx_frame), &my_mac_address, &my_ipv4_address)) == 0)
			continue;

		ethernet_send_frame(tx_frame, tx_frame_length);
	}
}
