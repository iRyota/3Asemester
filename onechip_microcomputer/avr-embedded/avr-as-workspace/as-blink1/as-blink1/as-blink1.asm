.include "m328Pdef.inc"

.cseg
  rjmp main

main:
setup:
  ldi r18, 60; _BV(PORTB5) | _BV(PORTB4) | _BV(PORTB3) | _BV(PORTB2)と同じ
  out DDRB, r18; ポートBの入出力設定を行う。
  ldi r18, 60; 
  out PORTB, r18; レジスタの内容をI/Oレジスタに書き込み
loop:
  rjmp loop