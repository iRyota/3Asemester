//2010.08.17 Masamichi SHIMOSAKA

#define F_CPU 16000000UL
#include <avr/io.h>

#include <avr/interrupt.h>

#include <util/delay.h>



void init()
{

	sei();

}




void setup()
{

	TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS11);
	TCCR1A = _BV(COM1A1) | _BV(WGM11);
	ICR1 = ....;


	ADCSRA = _BV(ADEN) | _BV(ADPS2);
	ADMUX = _BV(REFS0);


	DDRB = _BV(DDB5) | _BV(DDB4) | _BV(DDB3) | _BV(DDB2) | _BV(DDB1);



	DDRD = 0;

}



void loop()
{

	
	int OCR1Amin = 1500;

	int OCR1Amax = 4250;


	int value = 0;


	_delay_ms(100);


	ADCSRA |= _BV(ADSC);


	while (ADCSRA & _BV(ADSC)) { }

	
	value = ADC;

	
	OCR1A = (int) ((value / 1024.) * (....a.... - ....b....) + ....b....);

}


int main()
{

	init();

	setup();


	for(;;)loop();

	
	return 0;

}
