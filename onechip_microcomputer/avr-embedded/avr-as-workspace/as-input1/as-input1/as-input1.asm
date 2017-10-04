.include "m328Pdef.inc"

.def tmp1_in_main = r17
.def tmp2_in_main = r18

.cseg
  rjmp main

  ret

setup:
  ldi r18, 60
  out DDRB, r18
  ldi r18, 0
  out DDRD, r18
  out PORTD, r18
  ret

loop:
  in tmp1_in_main, PIND
  sbrs tmp1_in_main, 5
  rjmp light_off_5

  ;スイッチが押されているときの処理
  sbi PORTB, 5
  rjmp mode_4
light_off_5:
  cbi PORTB, 5
mode_4:
  sbrs tmp1_in_main, 4
  rjmp light_off_4

  ;スイッチが押されているときの処理
  sbi PORTB, 4
  rjmp mode_3
light_off_4:
  cbi PORTB, 4
mode_3:
  sbrs tmp1_in_main, 3
  rjmp light_off_3

  ;スイッチが押されているときの処理
  sbi PORTB, 3
  rjmp mode_2
light_off_3:
  cbi PORTB, 3
mode_2:
  sbrs tmp1_in_main, 2
  rjmp light_off_2

  ;スイッチが押されているときの処理
  sbi PORTB, 2
  rjmp mode_1
light_off_2:
  cbi PORTB, 2
mode_1:
  ret

main:
  rcall setup
while_1:
  rcall loop
  rjmp while_1
