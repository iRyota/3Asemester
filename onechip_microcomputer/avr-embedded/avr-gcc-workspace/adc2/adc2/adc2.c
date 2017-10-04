//example 2 of ADC
//2010.08.10 Masamichi Shimosaka

#include <avr/io.h>
#include <avr/interrupt.h>

void setup()
{
	DDRB = _BV(PB5) | _BV(PB4) | _BV(PB3) | _BV(PB2);
	ADCSRA = _BV(...) | _BV(ADPS2)|_BV(...)| _BV(...);
	//AD�ϊ������e�A�v���X�P�[���̐ݒ�A�I�[�g�g���K���[�h�ݒ�AAD�ϊ����荞�݂̋��A
	ADCSRB = ....;//�g���K�[�\�[�X�Ȃ��i�t���[�����j���O���[�h�j

	ADMUX = _BV(REFS0);//��d���̐ݒ�
	ADCSRA |= _BV(...);//AD�ϊ��̊J�n
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

int main(int argc, char* argv[])
{

	setup();

	sei();
	while (1);

}
