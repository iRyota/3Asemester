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

	eeprom_busy_wait();/* 読み書き可能になるまで待つ */
	... = eeprom_read_byte(0x00); /* EEPROMの0番地の値を読み出す */

	... ++;

	if(numberOfBoots > 15){
		numberOfBoots = 0;
	}

	eeprom_busy_wait();/* 読み書き可能になるまで待つ */
	eeprom_write_byte(0x00, numberOfBoots);/* 値0xAAをEEPROMの0番地に書き込む */

	eeprom_busy_wait();/* 読み書き可能になるまで待つ */

	DDRB = _BV(DDB5) | _BV(DDB4) | _BV(DDB3) | _BV(DDB2);

	PORTB = ... << 2;

	while(1){
	}

	return 0;
}
