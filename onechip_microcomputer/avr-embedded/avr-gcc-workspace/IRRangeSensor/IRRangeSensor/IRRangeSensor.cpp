
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>



//Serial�p�֗̕��N���X���ؗp
#include "HardwareSerial.h"

void setup() {
	ADCSRA = _BV(ADEN) | _BV(ADPS2);//AD�ϊ������e�A�v���X�P�[���̐ݒ�
	ADMUX = _BV(REFS0)| _BV(MUX0);//��d���̐ݒ�A���̓s���̐ݒ�

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