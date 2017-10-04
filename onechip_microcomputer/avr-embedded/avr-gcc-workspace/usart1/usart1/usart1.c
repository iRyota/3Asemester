
#include <avr/io.h>

void setup();
void sendData(unsigned char value);
unsigned char readData();

int main(int argc, char* argv[])
{
	setup();
	while (1) {
		sendData(readData());
	}

	return 0;
}

void setup() {
	UCSR0B = _BV(TXEN0) | _BV(RXEN0);//Rx Tx機能を有効化
	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);

	UBRR0 = ...;//USART Baud Rate Register
	//baud rate = f_osc / 16 / (UBRR0 + 1)
	//baud rate = 19200
}

unsigned char readData(){
	while(!(UCSR0A & _BV(...))){}
	return UDR0;
}

void sendData(unsigned char value)
{
	char premessage[] = "I receive ";
	int prelength = 10;
	char postmessage[] = " from Arduino";
	int postlength = 13;
	int i;

	for(i = 0; i < prelength; i++){
		while (!(UCSR0A & _BV(...))){}
		UDR0 = premessage[i];
	}

	while (!(UCSR0A & _BV(...))){}
	UDR0 = value;

	for(i = 0; i < postlength; i++){
		while (!(UCSR0A & _BV(...))){}
		UDR0 = postmessage[i];
	}
