//example 4 of timer0

//2010.08.10 Masamichi Shimosaka
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
	TCCR0A = _BV(...);//ctc������s�킹��
	... = ....;//TCNT0��TOP�l���Ƃ��郌�W�X�^�ɑ��
	TCCR0B = _BV(CS02) | _BV(CS00);  //������1024
	TCNT0 = 0;//�J�E���^�l�̏�����
	TIMSK0 = _BV(....);//��r��v���荞�݋���ݒ�
}

void blink_leds(unsigned char status)
{
	PORTB = (status >> 4) << 2;
}

volatile unsigned char state;

//Timre0��OCR0A�Ƃ̔�r��v���荞�݂ōs���֐�
ISR(TIMER0_COMPA_vect)
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
