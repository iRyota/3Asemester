//example 1 of timer0

//2010.08.10 Masamichi Shimosaka
//2010.09.02 Masamichi Shimosaka
//2010.09.30 Masamichi Shimosaka

#include <avr/io.h>

void setup_io()
{
	DDRB = _BV(DDB5) | _BV(DDB4) | _BV(DDB3) | _BV(DDB2);
}

void initialize_timer()
{
	//タイマーの設定
	TCCR0A = 0;
	TCCR0B = _BV(CS02) | _BV(CS00);  //1024分周比
	TCNT0 = 0; //カウンタ値の初期化
}

void blink_leds(unsigned char status)
{
	PORTB = (status >> 4) << 2;
}

int main(int argc, char* argv[])
{
	unsigned char state = 0;

	setup_io();
	initialize_timer();

	while (1) {
		loop_until_bit_is_set(TIFR0, ...);
		blink_leds(++state);
		TIFR0 |= _BV(...);
	}

	return 0;
}
