//control led blinking
//2010.08.06 Masamichi Shimosaka
//2010.09.01 Masamichi Shimosaka
//2010.09.30 Masamichi Shimosaka

#include <avr/io.h>

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
