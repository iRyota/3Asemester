.include "m328Pdef.inc"

.def tmp1_in_main = r17
.def tmp2_in_main = r18

.def loop_var1 = r19
.def loop_var2 = r20
.def loop_var3 = r21

.cseg
  rjmp main

my_delay:
  ldi loop_var1, 20
mainloop:
  ldi loop_var2, 0xff
innerloop1:
  ldi loop_var3, 0xff
innerloop2:
  dec loop_var3
  brne innerloop2
  dec loop_var2
  brne innerloop1
  dec loop_var1
  brne mainloop
  ret

setup:
  ldi r18, 60
  out DDRB, r18
  ret

loop:
  ldi tmp1_in_main, 0
  out PORTB, tmp1_in_main
  ldi tmp2_in_main, 1
loop_for_delay:
  rcall my_delay
  dec tmp2_in_main
  brne loop_for_delay

  ldi tmp1_in_main, 60
  out PORTB, tmp1_in_main
  rcall my_delay
  ret

main:
  rcall setup
while_1:
  rcall loop
  rjmp while_1
