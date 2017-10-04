//control led blinking
//2010.08.06 Masamichi Shimosaka
//2010.09.01 Masamichi Shimosaka
//2010.09.30 Masamichi Shimosaka

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(int argc, char* argv[])
{
	unsigned char on = _BV(PORTB5);
	unsigned char off = 0;
	int x = 0;
	int x_old = 0;
	int state = 0;

	DDRB = _BV(DDB5);
	DDRD = _BV(DDD5);
	PORTD = 0;

	while (1) {
		x = bit_is_set(PIND, PIND5);

		if (x && !x_old) {
			state = 1 - state;
			_delay_ms(10);
		}

		x_old = x;

		if(state){
			PORTB = on;
			} else {
			PORTB = off;
		}
	}

	return 0;
}