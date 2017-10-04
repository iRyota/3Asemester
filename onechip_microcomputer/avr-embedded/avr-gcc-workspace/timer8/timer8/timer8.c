//example 2 of timer1

//2010.09.02 Masamichi Shimosaka

#include <avr/io.h>
#include <avr/interrupt.h>

void setup_io()
{
	DDRB = _BV(DDB5) | _BV(DDB4) | _BV(DDB3) | _BV(DDB2);
}

void initialize_timer()
{
	//�^�C�}�[�̐ݒ�
	TCCR1A = 0;
	TCCR1B = _BV(...); //8������
	TCNT1 = ...;//�J�E���^�l�̏�����
	TIMSK1 = _BV(...);//�I�[�o�[�t���[���荞�݋���ݒ�
}

void blink_leds(unsigned char status)
{
	PORTB = (status >> 4) << 2;
}

volatile unsigned char state;

//Timre0�̃I�[�o�[�t���[�ɂ�蔭�����銄�荞�݂ōs���֐�
ISR(TIMER1_OVF_vect)
{
	state++;
	blink_leds(state);

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
