//example 3 of ADC
//2010.08.10 Masamichi Shimosaka

#include <avr/io.h>
#include <avr/interrupt.h>

void setup_io()
{
	DDRB = _BV(PB5) | _BV(PB4) | _BV(PB3) | _BV(PB2);
}

void output(int n)
{
	int res = (n + 102) / 204;

	if (res == 0) {
		PORTB = 0;
		} else {
		int i = 0;
		int outcode = 0;
		for (i = 0; i < res; i++) {
			outcode = outcode | (1 << (i - 1)) << 2;
		}
		PORTB = outcode;
	}
}

ISR(ADC_vect)
{
	output(ADC);
}

ISR(TIMER0_OVF_vect){
	TCNT0 = 99;
}

void initialize_timer()
{
	//タイマーの設定
	TCCR0A = 0;
	TCCR0B = _BV(CS02) | _BV(CS00);  //1/1024分周比
	TCNT0 = ...;//カウンタ値の初期化
	TIMSK0 = _BV(TOIE0);//オーバーフロー割り込み許可を設定
}

int main(int argc, char* argv[])
{
	setup_io();

	initialize_timer();

	ADCSRA = _BV(...) | _BV(ADPS2)|_BV(...)| _BV(...);
	//AD変換を許容、プリスケーラの設定、AD変換割り込みの許可、オートトリガモード設定
	ADCSRB = _BV(...);//トリガーソースはタイマー0オーバーフロー割り込み

	ADMUX = _BV(REFS0);//基準電圧の設定
	ADCSRA |= _BV(...);//AD変換の開始

	sei();
	while (1);

}
