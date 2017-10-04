#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//Serial用の便利クラスを借用
#include "HardwareSerial.h"

const int ir_out = 7;
volatile unsigned long t0_ovf_counter = 0;

//起動後の経過時間計測用
ISR(TIMER0_OVF_vect)
{
	t0_ovf_counter++;
}

//AVR起動後の時間を計測（マイクロ秒）
//プリスケーラ64で設定したTimer0のオーバーフローの回数から算出
unsigned long elapsedMicrosec(){
	unsigned char t = TCNT0;
	unsigned long m = t0_ovf_counter;
	unsigned long val = ((m << 8) + t) * 64 / (F_CPU / 1000000L);
	//256 * m + tを64倍しCPUのステップ数を得て、
	//それをマイクロ秒あたりのCPUのステップ数で割る。

	return val;
}

//主として割り込みとタイマ0の設定
void init(){
	sei();
	TCCR0A = 0;
	TCCR0B = _BV(CS01) | _BV(CS00);
	TIMSK0 = _BV(TOIE0);
	t0_ovf_counter = 0;
	TCNT0 = 0;
}

//38 KHzキャリア信号を生成
void generatePulse(int microsec, int pinNumber){
	unsigned long endval = elapsedMicrosec() + microsec;

	//pinNumberのピン出力のレベルを一定時間毎に切り替える．
	//microsecマイクロ秒だけそれを繰り返す．
	while(endval > elapsedMicrosec()){
		PORTD ....
		_delay_us(...);
		PORTD ....
		_delay_us(...);
	}
}

//キャリア信号を停止
void generateSilence(int microsec, int pinNumber){
	unsigned long endval = elapsedMicrosec() + microsec;

	PORTD &= ~_BV(pinNumber);

	while(endval > elapsedMicrosec()){
		_delay_us(4);
	}
}

//pulse position moduration信号の生成
//0 bitの場合と1ビットの場合で発振時間が変わる
void output(int val, int pinNumber){
	if(val){
		generateSilence(600, pinNumber);
		generatePulse(1200, pinNumber);
		} else{
		generateSilence(600, pinNumber);
		generatePulse(600, pinNumber);
	}
}

//１バイト分の情報をPORTDのpin番号を通じてPPMで通信
void sendMessage(unsigned char cmd, int pin){

	//スタートビットの送信
	generatePulse(2400, pin);
	int i;
	for(i = 7; i >= 0; i--){
		//cmd 各ビットの値を38KHzキャリー波に乗せPPMで発信
		output(...., pin);
	}

	//念のため送信後赤外LEDを消灯させる。
	generateSilence(100, pin);
}

//シリアル通信の設定とポートの設定
void setup(){
	DDRD = _BV(ir_out);//赤外LEDのポート番号
	Serial.begin(19200);//Baud Rateの設定
}

//繰り返し処理される関数
void loop(){
	//メッセージをPCから受信したらそのデータを即座にエンコードして発信
	if(Serial.available()){
		unsigned char c = Serial.read();
		sendMessage(c, ir_out);
		_delay_ms(3);
	}
}

//Arduinoソフトウェアと同じ処理構成
int main()
{
	init();
	setup();
	
	for(;;)loop();
	
	return 0;
}