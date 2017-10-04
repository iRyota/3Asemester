//example 1 of timer1

//2010.09.02 Masamichi Shimosaka

#include <avr/io.h>

void setup_io()
{
	DDRB = _BV(DDB5) | _BV(DDB4) | _BV(DDB3) | _BV(DDB2);
}

void initialize_timer()
{
	//�^�C�}�[�̐ݒ�
	TCCR1A = 0;//�m�[�}��������s�킹��
	TCCR1B = _BV(CS12) | _BV(CS10);  //1024������
	TCNT1 = 0;//�J�E���^�l�̏�����
	TIMSK1 = 0; //���荞�݂͗p���Ȃ�
}

void blink_leds(unsigned char status)
{
	PORTB = (status >> 4) << 2;
}


int main(int argc, char* argv[])
{
	
	setup_io();
	initialize_timer();


	while (1){
		blink_leds(TCNT1 / 256);
	}

	return 0;
}
