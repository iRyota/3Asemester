.include "m328Pdef.inc"

.cseg
  rjmp main

main:
setup:
  ldi r18, 60; _BV(PORTB5) | _BV(PORTB4) | _BV(PORTB3) | _BV(PORTB2)�Ɠ���
  out DDRB, r18; �|�[�gB�̓��o�͐ݒ���s���B
  ldi r18, 60; 
  out PORTB, r18; ���W�X�^�̓��e��I/O���W�X�^�ɏ�������
loop:
  rjmp loop