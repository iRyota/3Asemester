//example 3 of timer0

//2010.08.10 Masamichi Shimosaka
//2010.09.02 Masamichi Shimosaka
//2010.09.30 Masamichi Shimosaka

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

void setup_io()
{
	DDRB = _BV(DDB5) | _BV(DDB4) | _BV(DDB3) | _BV(DDB2);
}

void initialize_timer()
{
	//�^�C�}�[�̐ݒ�
	TCCR0A = 0;
	TCCR0B = _BV(CS02) | _BV(CS00);  //1024������
	TCNT0 = 0;//�J�E���^�l�̏�����
	TIMSK0 = _BV(...);//�I�[�o�[�t���[���荞�݋���ݒ�
}

void blink_leds(unsigned char status)
{
	PORTB = (status >> 4) << 2;
}

volatile unsigned char state;

//Timre0�̃I�[�o�[�t���[�ɂ�蔭�����銄�荞�݂ōs���֐�
ISR(TIMER0_OVF_vect)
{
	state++;
	blink_leds(state);
	TCNT0 =...;
}

int main(int argc, char* argv[])
{
	
	setup_io();
	initialize_timer();

	state = 0;

	sei();//�S���荞�݂̋����s��

	while (1);

	return 0;
}
