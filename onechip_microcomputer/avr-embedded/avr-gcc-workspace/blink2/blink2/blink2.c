/*
 * blink2.c
 *
 * Created: 2015/09/29 18:44:32
 *  Author: mech-user
 */ 


#include <avr/io.h>

void mywait(){
	volatile int i;
	volatile int j;
	for(i = 0; i < 2000; i++) {
		for(j = 0; j < 200; j++){}
	}
}

int main(void)
{
	DDRB = 0b11110000;
	PORTB = 0b11001111;
    while(1)
    {
		PORTB = 0b11101111;
		mywait();
		PORTB = 0b11001111;
		mywait();
        //TODO:: Please write your application code 
    }
}