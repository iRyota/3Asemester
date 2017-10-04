#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(int argc, char* argv[])
{
	
	unsigned char numberOfBoots;

	int i = 0;

	for(i = 0; i < 20; i++){
		_delay_ms(100.);
	}
	
	//�������݂��s���Ă����Ԃ����m�F��
	//�ǂݏ����\�ȏ󋵂܂őҋ@�B
	while(EECR & _BV(...)){
	}

	EEAR = 0x00;//EEPROM�̃A�h���X��ݒ�B0����0x3FF
	EECR |= _BV(...);//�ǂݍ��݂��J�n


	//EEPROM��EEAR�A�h���X�Ԓn�̒��g���擾�B
	numberOfBoots = EEDR;

	numberOfBoots++;

	if(numberOfBoots > 15){
		numberOfBoots = 0;
	}

	while(EECR & _BV(...)){
	}

	EEAR = 0x00;
	EEDR = numberOfBoots;
	EECR |= _BV(EEMPE);

	EECR |= _BV(...);


	DDRB = _BV(DDB5) | _BV(DDB4) | _BV(DDB3) | _BV(DDB2);

	PORTB = numberOfBoots  << 2;

	while(1){
	}

	return 0;
}
