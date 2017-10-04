/*
 * blink1.c
 *
 * Created: 2017/10/03 13:36:22
 *  Author: mech-user
 */ 


#include <avr/io.h>

int main(int argc, char* argv[])
{
	DDRB = 0b00100000;
	PORTB = 0b00100000;
    
	while(1)
    {
		PORTB = 0b00100000;
    }
}