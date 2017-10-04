#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#include <avr/eeprom.h>

int main(int argc, char* argv[])
{
	unsigned char numberOfBoots;
	int i = 0;

	for(i = 0; i < 20; i++){
		_delay_ms(100);
	}

	eeprom_busy_wait();/* �ǂݏ����\�ɂȂ�܂ő҂� */
	... = eeprom_read_byte(0x00); /* EEPROM��0�Ԓn�̒l��ǂݏo�� */

	... ++;

	if(numberOfBoots > 15){
		numberOfBoots = 0;
	}

	eeprom_busy_wait();/* �ǂݏ����\�ɂȂ�܂ő҂� */
	eeprom_write_byte(0x00, numberOfBoots);/* �l0xAA��EEPROM��0�Ԓn�ɏ������� */

	eeprom_busy_wait();/* �ǂݏ����\�ɂȂ�܂ő҂� */

	DDRB = _BV(DDB5) | _BV(DDB4) | _BV(DDB3) | _BV(DDB2);

	PORTB = ... << 2;

	while(1){
	}

	return 0;
}
