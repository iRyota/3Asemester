//control multiple led blinking

//2010.08.10 Masamichi Shimosaka
//2010.09.01 Masamichi Shimosaka
//2010.09.30 Masamichi Shimosaka

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(int argc, char* argv[])
{
	int counter = 0;
	int x = 0;
	int x_old = 0;

	DDRB = _BV(DDB5) | _BV(DDB4) | _BV(DDB3) | _BV(DDB2);
	DDRD = _BV(PIND2) | _BV(PIND3) | _BV(PIND4) | _BV(PIND5);
	PORTB = 0b00000000;
	
	while (1) {
		x = bit_is_set(PIND, PIND5);
		
		if (x && !x_old){
			counter++;
			_delay_ms(20);
			if (counter>4){
				PORTB = 0b00000000;
				counter = 0;
			} else {
				PORTB += _BV(1 + counter);
			}
		}

		x_old = x;
		

	
	}
	return 0;
}