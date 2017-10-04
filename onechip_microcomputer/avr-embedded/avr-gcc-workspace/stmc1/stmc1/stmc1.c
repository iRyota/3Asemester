//control led blinking

//2010.08.06 Masamichi Shimosaka
//2010.09.01 Masamichi Shimosaka
//2010.09.30 Masamichi Shimosaka

#include <avr/io.h>

int main(int argc, char* argv[])
{
	unsigned char on = _BV(PORTB5);
	unsigned char off = 0;
	int state = 0;

	DDRB = _BV(DDB5);
	DDRD = 0;
	PORTD = 0;

	while (1) {
		if(bit_is_set(PIND, PIND5)){
			state = 1 - state;
		}

		if (state) {
			PORTB = on;
			} else {
			PORTB = off;
		}
	}
	return 0;
}