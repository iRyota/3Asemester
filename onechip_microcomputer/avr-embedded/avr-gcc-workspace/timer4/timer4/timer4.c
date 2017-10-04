//example 4 of timer0

//2010.08.10 Masamichi Shimosaka
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
	TCCR0A = _BV(...);//ctc動作を行わせる
	... = ....;//TCNT0のTOP値をとあるレジスタに代入
	TCCR0B = _BV(CS02) | _BV(CS00);  //分周比1024
	TCNT0 = 0;//カウンタ値の初期化
	TIMSK0 = _BV(....);//比較一致割り込み許可を設定
}

void blink_leds(unsigned char status)
{
	PORTB = (status >> 4) << 2;
}

volatile unsigned char state;

//Timre0のOCR0Aとの比較一致割り込みで行う関数
ISR(TIMER0_COMPA_vect)
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
