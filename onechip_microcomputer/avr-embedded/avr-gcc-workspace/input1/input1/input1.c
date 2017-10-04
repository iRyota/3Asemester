/*
 * input1.c
 *
 * Created: 2015/09/29 20:24:03
 *  Author: mech-user
 */ 


#include <avr/io.h>

int main(void)
{
	DDRB = _BV(PORTB2) | _BV(PORTB3) | _BV(PORTB4) | _BV(PORTB5);
	DDRD = _BV(PIND2) | _BV(PIND3) | _BV(PIND4) | _BV(PIND5);
	PORTB = _BV(PORTB2) | _BV(PORTB3) | _BV(PORTB4) | _BV(PORTB5);
	
    while(1)
    {
		if(bit_is_set(PIND,PIND2)){
			PORTB |= _BV(PORTB2);	
		} else {
			PORTB &= ~_BV(PORTB2);	
		}
		if(bit_is_set(PIND,PIND3)){
			PORTB |= _BV(PORTB3);
			} else {
			PORTB &= ~_BV(PORTB3);
		}
		if(bit_is_set(PIND,PIND4)){
			PORTB |= _BV(PORTB4);
			} else {
			PORTB &= ~_BV(PORTB4);
		}
		if(bit_is_set(PIND,PIND5)){
			PORTB |= _BV(PORTB5);
			} else {
			PORTB &= ~_BV(PORTB5);
		} 
    }
	return 0;
}