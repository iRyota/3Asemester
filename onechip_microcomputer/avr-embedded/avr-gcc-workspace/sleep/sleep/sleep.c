#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

volatile unsigned char count;

ISR(TIMER1_OVF_vect)
{
	count++;

	PORTB = 0xff;

	if(count > 15){
		count = 0;
	}

	if(!count){
		PORTB = 15 << 2;
		} else{
		PORTB = 0x00;
	}
}

int main(void)
{
	count = 0;

	DDRB = _BV(DDB5) | _BV(DDB4) | _BV(DDB3) | _BV(DDB2);

	TCCR1A = 0;

	TCCR1B = _BV(CS11) | _BV(CS10);

	TIMSK1 = _BV(TOIE1); //Timer1 Overflow Interrupt

	set_sleep_mode(SLEEP_MODE_IDLE);
	
	sei();
	while(1){
		sleep_mode();
	}
	return 0;
}
