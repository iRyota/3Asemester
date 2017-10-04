//example 6 of timer0 and timer2

//2010.09.02 Masamichi Shimosaka
//2010.09.30 Masamichi Shimosaka
//2010.10.01 Masamichi Shimosaka

#include <avr/io.h>
#include <avr/interrupt.h>

void setup_io()
{
	DDRB = _BV(DDB3);

}

void initialize_timer()
{
	//タイマーの設定
	TCCR2A = _BV(COM2A1) | _BV(COM2A0) |_BV(WGM21) | _BV(WGM20);
	... = ...;//OC2Aの出力状態を変える際の条件となるレジスタの設定
	TCCR2B = _BV(CS21);
	TCNT2 = 0;

	//タイマーの設定
	TCCR0A = 0;
	TCCR0B = _BV(CS01) | _BV(CS00);
	TCNT0 = 0;
	TIMSK0 = _BV(...);
}

volatile unsigned char state;

ISR(TIMER0_OVF_vect)
{
	OCR2A = ...;

}

int main(int argc, char* argv[])
{
	
	setup_io();
	initialize_timer();

	state = 0;

	sei();

	while (1);

	return 0;
}
