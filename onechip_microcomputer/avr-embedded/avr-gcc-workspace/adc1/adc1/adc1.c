//example 1 of ADC
//2010.08.10 Masamichi Shimosaka

#include <avr/io.h>

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


int main(int argc, char* argv[])
{
	setup_io();

	ADCSRA = _BV(ADEN) | _BV(ADPS2);//AD�ϊ������e�A�v���X�P�[���̐ݒ�

	ADMUX = _BV(REFS0);//��d���̐ݒ�

	while (1) {
		ADCSRA |= _BV(....);//AD�ϊ��̊J�n

		loop_until_bit_is_set(ADCSRA,....);

		output(ADC);//AD�ϊ��l�̓ǂݍ��݂ƒl�ɉ�����LED�̕\��
		ADCSRA |= _BV(..);//���荞�ݗv���t���O���Z�b�g����D
	}
}
