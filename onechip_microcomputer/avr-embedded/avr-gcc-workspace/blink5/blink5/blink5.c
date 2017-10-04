//blink LED Arduino Duemilanove
//2010.08.03 Masamichi Shimosaka

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

// 4 step�̎��s��__count��J��Ԃ��B
// 16MHz�Ȃ̂ŁA��__count = 4000���1ms���s�����B
void delay1msWithAsm(uint16_t __count)
{
	__asm__ volatile (
	"1: sbiw %0,1" "\n\t"
	"brne 1b"
	: "=w" (__count)
	: "0" (__count)
	);
}

void myDelay(int ms){
	int i = 0;
	for(i = 0; i < ms; i++){
		delay1msWithAsm(4000);
	}
}

int main(int argc, char* argv[])
{
	unsigned char on, off;
	DDRB = ...;
	on = ...;
	off = ...;

	while(1) {
		PORTB = on;
		myDelay(500);
		PORTB = off;
		myDelay(100);
	}

	return 0;
}