//example 2 of timer1

//2010.09.02 Masamichi Shimosaka

#include <avr/io.h>
#include <avr/interrupt.h>

void setup_io()
{
	DDRB = _BV(DDB5) | _BV(DDB4) | _BV(DDB3) | _BV(DDB2);
}

void initialize_timer()
{
	//タイマーの設定
	TCCR1A = 0;
	TCCR1B = _BV(...); //8分周比
	TCNT1 = ...;//カウンタ値の初期化
	TIMSK1 = _BV(...);//オーバーフロー割り込み許可を設定
}

void blink_leds(unsigned char status)
{
	PORTB = (status >> 4) << 2;
}

volatile unsigned char state;

//Timre0のオーバーフローにより発生する割り込みで行う関数
ISR(TIMER1_OVF_vect)
{
	state++;
	blink_leds(state);

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
