//2010.08.11 Masamichi Shimosaka
//2010.09.06 Masamichi Shimosaka

#include <avr/io.h>
#include <avr/interrupt.h>

void setup();
void blink_leds(unsigned char status);
void sendData(unsigned char value);


ISR(USART_RX_vect)
{
	unsigned char xx = UDR0;
	sendData(xx);
}

void blink_leds(unsigned char status)
{
	PORTB = (status >> 4) << 2;
}


int main(int argc, char* argv[]) {
	setup();
	sei();


	while (1) {
		blink_leds(TCNT1 / 256);
	}

	return 0;
}

void setup() {
	DDRB = _BV(PB5) | _BV(PB4) | _BV(PB3) | _BV(PB2);

	UCSR0B = _BV(TXEN0) | _BV(RXEN0) | _BV(...); //RX TX��L�������邾���łȂ���M���荞�݂�������D

	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);

	UBRR0 = ...;//USART Baud Rate Register
	//baud rate = f_osc / (N + 1)
	//baud rate = ??

	//�^�C�}�[�̐ݒ�
	TCCR1A = 0;//�m�[�}��������s�킹��
	TCCR1B = _BV(CS02) | _BV(CS00);  //1024������
	TCNT1 = 0;//�J�E���^�l�̏�����
	TIMSK1 = 0; //���荞�݂͗p���Ȃ�
}

void sendData(unsigned char value)
{
	char premessage[] = "\"I receive ";
	int prelength = 11;
	char postmessage[] = ",\" message from Arduino";
	int postlength = 24;
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

	while (!(UCSR0A & _BV(...))) {}
	UDR0 = '\n';
}
