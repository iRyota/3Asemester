//example 1 of ADC
//2010.08.10 Masamichi Shimosaka

#include <avr/io.h>

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


int main(int argc, char* argv[])
{
	setup_io();

	ADCSRA = _BV(ADEN) | _BV(ADPS2);//AD変換を許容、プリスケーラの設定

	ADMUX = _BV(REFS0);//基準電圧の設定

	while (1) {
		ADCSRA |= _BV(....);//AD変換の開始

		loop_until_bit_is_set(ADCSRA,....);

		output(ADC);//AD変換値の読み込みと値に応じたLEDの表示
		ADCSRA |= _BV(..);//割り込み要求フラグをセットする．
	}
}
