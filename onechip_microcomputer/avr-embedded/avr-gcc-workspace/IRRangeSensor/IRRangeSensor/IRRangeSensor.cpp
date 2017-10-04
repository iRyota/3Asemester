
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>



//Serial用の便利クラスを借用
#include "HardwareSerial.h"

void setup() {
	ADCSRA = _BV(ADEN) | _BV(ADPS2);//AD変換を許容、プリスケーラの設定
	ADMUX = _BV(REFS0)| _BV(MUX0);//基準電圧の設定、入力ピンの設定

	Serial.begin(19200);
}

void loop(){
	ADCSRA |= _BV(ADSC);
	while (ADCSRA & _BV(ADSC)) {}
	
	int voltage = ADC;
	
	double dist = 0.;
	if(voltage < 487){
		if(voltage > 84){
			dist = (6787. / (voltage - 3.)) - 4.;
			} else{
			dist = 81.;
		}
	}

	if(dist < 80 && dist > 10){
		Serial.print(dist);
		Serial.println(" [cm]");
		} else{
		Serial.println("out of range");
	}

	_delay_ms(50);
}

void init(){
}

int main()
{
	init();
	setup();

	for(;;){
		loop();
	}
	
	return 0;
}