.include "m328Pdef.inc"

.cseg
  rjmp main

main:
  ldi r18, 60
  out DDRB, r18
  ldi r18, 0
  out PORTB, r18
start:
  ldi r19, 20
mainloop1:
  ldi r20, 0xff
innerloop11:
  ldi r21, 0xff
innerloop12:
  dec r21
  brne innerloop12
  dec r20
  brne innerloop11
  dec r19
  brne mainloop1
  ldi r18, 60
  out PORTB, r18
  ldi r19, 50
mainloop2:
  ldi r20, 0xff
innerloop21:
  ldi r21, 0xff
innerloop22:
  dec r21
  brne innerloop22
  dec r20
  brne innerloop21
  dec r19
  brne mainloop2
  ldi r18, 0
  out PORTB, r18
  rjmp start
