#include <avr/io.h>
#include <avr/interrupt.h>

//Serial用の便利クラスを借用
#include "HardwareSerial.h"

volatile unsigned long t0_ovf_counter = 0;
int ir_in = 0;
unsigned char c;
unsigned char last;
unsigned long us;


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

// セットアップ
void setup() {
	Serial.begin(19200);        // シリアル通信速度の設定
	DDRB = 0;// 入出力ピンの設定
	us = elapsedMicrosec();
}

static int idx = -1;

void loop() {
	unsigned char val;
	
	unsigned int nextline = 0;
	while ( (val = (PINB & _BV(ir_in))) == last) {  // パルスが切り替わるまで待機
		unsigned long us3 = elapsedMicrosec();

		if(us3 - us > 2800){
			if(idx == 8){//文字を受信している場合、PCへ受信文字を送信
				Serial.print(c);
				idx = -1;
				c = 0;
			}
		}
		if(us3 - us > 10000L && !nextline){//しばらくメッセージが来なければ一行改行
			Serial.println();
			nextline = 1;
		}
	}

	unsigned long us2 = elapsedMicrosec();//現在の時間を取得。

	//信号の立ち上がりの際、Lレベルの経過時間からデコード
	if(val){
		int duration = us2-us;
		if(idx  > -1){
			if(duration < 800){
				//Lの時間が600usであれば0
				idx++;
				}else if(duration < 1600){//1200us
				//Lの時間が1200usであれば1
				//受信文字cを調整する必要が生じる。
				//1番目に受信したデータが1ならcの7ビット目をセット
				//2番目に受信したデータが1ならcの6ビット目をセット
				//3番目以降も同様．．．
				//というようにidxの値に応じてシフト演算子を利用し、
				//特定のビットをセットし、cの値を書き換える
				c |= .....;
				idx++;
				} else{
				//スタートビットとみなす。
				idx = 0;
				c = 0;
			}
			} else{
			idx = 0;
			c = 0;
		}
	}
	
	last = val;
	us = us2;
}


int main()
{
	init();
	setup();
	for(;;){
		loop();
	}
}