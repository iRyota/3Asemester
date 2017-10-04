//example 5 of timer0 (timer2)

//2010.09.02 Masamichi Shimosaka

#include <avr/io.h>

void setup_io()
{
	DDRB = _BV(DDB3);
}

void initialize_timer()
{
	//タイマーの設定
	TCCR2A = _BV(COM2A1) | _BV(COM2A0) |_BV(WGM21) | _BV(WGM20);
	... = ...;//OC2Aの出力状態を変える際の条件となるレジスタの設定
	TCCR2B = _BV(CS21); //8分周比
	TCNT2 = 0;//カウンタ値の初期化
}

int main(int argc, char* argv[])
{
	
	setup_io();
	initialize_timer();

	while (1);

	return 0;
}
