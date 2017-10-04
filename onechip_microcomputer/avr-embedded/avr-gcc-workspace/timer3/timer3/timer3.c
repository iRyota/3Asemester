//example 3 of timer0

//2010.08.10 Masamichi Shimosaka
//2010.09.02 Masamichi Shimosaka
//2010.09.30 Masamichi Shimosaka

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

void setup_io()
{
	DDRB = _BV(DDB5) | _BV(DDB4) | _BV(DDB3) | _BV(DDB2);
}

void initialize_timer()
{
	//タイマーの設定
	TCCR0A = 0;
	TCCR0B = _BV(CS02) | _BV(CS00);  //1024分周比
	TCNT0 = 0;//カウンタ値の初期化
	TIMSK0 = _BV(...);//オーバーフロー割り込み許可を設定
}

void blink_leds(unsigned char status)
{
	PORTB = (status >> 4) << 2;
}

volatile unsigned char state;

//Timre0のオーバーフローにより発生する割り込みで行う関数
ISR(TIMER0_OVF_vect)
{
	state++;
	blink_leds(state);
	TCNT0 =...;
}

int main(int argc, char* argv[])
{
	
	setup_io();
	initialize_timer();

	state = 0;

	sei();//全割り込みの許可を行う

	while (1);

	return 0;
}
