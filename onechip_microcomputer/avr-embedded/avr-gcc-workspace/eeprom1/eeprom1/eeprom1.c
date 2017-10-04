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
	
	//書き込みが行われている状態かを確認し
	//読み書き可能な状況まで待機。
	while(EECR & _BV(...)){
	}

	EEAR = 0x00;//EEPROMのアドレスを設定。0から0x3FF
	EECR |= _BV(...);//読み込みを開始


	//EEPROMのEEARアドレス番地の中身を取得。
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
