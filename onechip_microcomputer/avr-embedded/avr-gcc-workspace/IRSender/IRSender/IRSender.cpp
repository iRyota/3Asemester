#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//Serial�p�֗̕��N���X���ؗp
#include "HardwareSerial.h"

const int ir_out = 7;
volatile unsigned long t0_ovf_counter = 0;

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

//38 KHz�L�����A�M���𐶐�
void generatePulse(int microsec, int pinNumber){
	unsigned long endval = elapsedMicrosec() + microsec;

	//pinNumber�̃s���o�͂̃��x������莞�Ԗ��ɐ؂�ւ���D
	//microsec�}�C�N���b����������J��Ԃ��D
	while(endval > elapsedMicrosec()){
		PORTD ....
		_delay_us(...);
		PORTD ....
		_delay_us(...);
	}
}

//�L�����A�M�����~
void generateSilence(int microsec, int pinNumber){
	unsigned long endval = elapsedMicrosec() + microsec;

	PORTD &= ~_BV(pinNumber);

	while(endval > elapsedMicrosec()){
		_delay_us(4);
	}
}

//pulse position moduration�M���̐���
//0 bit�̏ꍇ��1�r�b�g�̏ꍇ�Ŕ��U���Ԃ��ς��
void output(int val, int pinNumber){
	if(val){
		generateSilence(600, pinNumber);
		generatePulse(1200, pinNumber);
		} else{
		generateSilence(600, pinNumber);
		generatePulse(600, pinNumber);
	}
}

//�P�o�C�g���̏���PORTD��pin�ԍ���ʂ���PPM�ŒʐM
void sendMessage(unsigned char cmd, int pin){

	//�X�^�[�g�r�b�g�̑��M
	generatePulse(2400, pin);
	int i;
	for(i = 7; i >= 0; i--){
		//cmd �e�r�b�g�̒l��38KHz�L�����[�g�ɏ悹PPM�Ŕ��M
		output(...., pin);
	}

	//�O�̂��ߑ��M��ԊOLED������������B
	generateSilence(100, pin);
}

//�V���A���ʐM�̐ݒ�ƃ|�[�g�̐ݒ�
void setup(){
	DDRD = _BV(ir_out);//�ԊOLED�̃|�[�g�ԍ�
	Serial.begin(19200);//Baud Rate�̐ݒ�
}

//�J��Ԃ����������֐�
void loop(){
	//���b�Z�[�W��PC�����M�����炻�̃f�[�^�𑦍��ɃG���R�[�h���Ĕ��M
	if(Serial.available()){
		unsigned char c = Serial.read();
		sendMessage(c, ir_out);
		_delay_ms(3);
	}
}

//Arduino�\�t�g�E�F�A�Ɠ��������\��
int main()
{
	init();
	setup();
	
	for(;;)loop();
	
	return 0;
}