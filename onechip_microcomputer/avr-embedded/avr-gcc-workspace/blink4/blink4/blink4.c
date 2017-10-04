//blink LED Arduino Duemilanove
//2010.08.03 Masamichi Shimosaka
//2017.10.02 Ryuma Niiyama

#define F_CPU 10000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(int argc, char* argv[])
{
	unsigned char on, off;
	DDRB = 0b00100000;
	on = 0b00100000;
	off = 0b00000000;

	while(1) {
		PORTB = on;
		_delay_ms(2000.0);
		PORTB = off;
		_delay_ms(1000.0);
	}

	return 0;
}
