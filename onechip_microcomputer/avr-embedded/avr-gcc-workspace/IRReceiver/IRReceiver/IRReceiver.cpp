#include <avr/io.h>
#include <avr/interrupt.h>

//Serial�p�֗̕��N���X���ؗp
#include "HardwareSerial.h"

volatile unsigned long t0_ovf_counter = 0;
int ir_in = 0;
unsigned char c;
unsigned char last;
unsigned long us;


//�N����̌o�ߎ��Ԍv���p
ISR(TIMER0_OVF_vect)
{
	t0_ovf_counter++;
}

//AVR�N����̎��Ԃ��v���i�}�C�N���b�j
//�v���X�P�[��64�Őݒ肵��Timer0�̃I�[�o�[�t���[�̉񐔂���Z�o
unsigned long elapsedMicrosec(){
	unsigned char t = TCNT0;
	unsigned long m = t0_ovf_counter;
	unsigned long val = ((m << 8) + t) * 64 / (F_CPU / 1000000L);
	//256 * m + t��64�{��CPU�̃X�e�b�v���𓾂āA
	//������}�C�N���b�������CPU�̃X�e�b�v���Ŋ���B

	return val;
}

//��Ƃ��Ċ��荞�݂ƃ^�C�}0�̐ݒ�
void init(){
	sei();
	TCCR0A = 0;
	TCCR0B = _BV(CS01) | _BV(CS00);
	TIMSK0 = _BV(TOIE0);
	t0_ovf_counter = 0;
	TCNT0 = 0;
}

// �Z�b�g�A�b�v
void setup() {
	Serial.begin(19200);        // �V���A���ʐM���x�̐ݒ�
	DDRB = 0;// ���o�̓s���̐ݒ�
	us = elapsedMicrosec();
}

static int idx = -1;

void loop() {
	unsigned char val;
	
	unsigned int nextline = 0;
	while ( (val = (PINB & _BV(ir_in))) == last) {  // �p���X���؂�ւ��܂őҋ@
		unsigned long us3 = elapsedMicrosec();

		if(us3 - us > 2800){
			if(idx == 8){//��������M���Ă���ꍇ�APC�֎�M�����𑗐M
				Serial.print(c);
				idx = -1;
				c = 0;
			}
		}
		if(us3 - us > 10000L && !nextline){//���΂炭���b�Z�[�W�����Ȃ���Έ�s���s
			Serial.println();
			nextline = 1;
		}
	}

	unsigned long us2 = elapsedMicrosec();//���݂̎��Ԃ��擾�B

	//�M���̗����オ��̍ہAL���x���̌o�ߎ��Ԃ���f�R�[�h
	if(val){
		int duration = us2-us;
		if(idx  > -1){
			if(duration < 800){
				//L�̎��Ԃ�600us�ł����0
				idx++;
				}else if(duration < 1600){//1200us
				//L�̎��Ԃ�1200us�ł����1
				//��M����c�𒲐�����K�v��������B
				//1�ԖڂɎ�M�����f�[�^��1�Ȃ�c��7�r�b�g�ڂ��Z�b�g
				//2�ԖڂɎ�M�����f�[�^��1�Ȃ�c��6�r�b�g�ڂ��Z�b�g
				//3�Ԗڈȍ~�����l�D�D�D
				//�Ƃ����悤��idx�̒l�ɉ����ăV�t�g���Z�q�𗘗p���A
				//����̃r�b�g���Z�b�g���Ac�̒l������������
				c |= .....;
				idx++;
				} else{
				//�X�^�[�g�r�b�g�Ƃ݂Ȃ��B
				idx = 0;
				c = 0;
			}
			} else{
			idx = 0;
			c = 0;
		}
	}
	
	last = val;
	us = us2;
}


int main()
{
	init();
	setup();
	for(;;){
		loop();
	}
}