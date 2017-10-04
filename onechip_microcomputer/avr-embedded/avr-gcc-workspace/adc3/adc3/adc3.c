//example 3 of ADC
//2010.08.10 Masamichi Shimosaka

#include <avr/io.h>
#include <avr/interrupt.h>

void setup_io()
{
	DDRB = _BV(PB5) | _BV(PB4) | _BV(PB3) | _BV(PB2);
}

void output(int n)
{
	int res = (n + 102) / 204;

	if (res == 0) {
		PORTB = 0;
		} else {
		int i = 0;
		int outcode = 0;
		for (i = 0; i < res; i++) {
			outcode = outcode | (1 << (i - 1)) << 2;
		}
		PORTB = outcode;
	}
}

ISR(ADC_vect)
{
	output(ADC);
}

ISR(TIMER0_OVF_vect){
	TCNT0 = 99;
}

void initialize_timer()
{
	//�^�C�}�[�̐ݒ�
	TCCR0A = 0;
	TCCR0B = _BV(CS02) | _BV(CS00);  //1/1024������
	TCNT0 = ...;//�J�E���^�l�̏�����
	TIMSK0 = _BV(TOIE0);//�I�[�o�[�t���[���荞�݋���ݒ�
}

int main(int argc, char* argv[])
{
	setup_io();

	initialize_timer();

	ADCSRA = _BV(...) | _BV(ADPS2)|_BV(...)| _BV(...);
	//AD�ϊ������e�A�v���X�P�[���̐ݒ�AAD�ϊ����荞�݂̋��A�I�[�g�g���K���[�h�ݒ�
	ADCSRB = _BV(...);//�g���K�[�\�[�X�̓^�C�}�[0�I�[�o�[�t���[���荞��

	ADMUX = _BV(REFS0);//��d���̐ݒ�
	ADCSRA |= _BV(...);//AD�ϊ��̊J�n

	sei();
	while (1);

}
