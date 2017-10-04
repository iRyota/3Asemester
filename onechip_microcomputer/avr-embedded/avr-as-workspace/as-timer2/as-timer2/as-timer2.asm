.include "m328Pdef.inc"

.def tmp1 = r17
.def state = r18

.cseg
  rjmp main

.org 0x0020
  rjmp timer0_ovf_func

timer0_ovf_func:
  inc state

blink_led:
  mov tmp1, state
  lsr tmp1
  lsr tmp1
  lsr tmp1
  lsr tmp1
  lsl tmp1
  lsl tmp1
  out PORTB, tmp1
  reti



setup:
  ldi tmp1, 0
  out TCCR0A, tmp1
  sbr tmp1, ((1 << CS02) + (1 << CS00))
  out TCCR0B, tmp1
  ldi tmp1, 0
  out TCNT0, tmp1
  sbr tmp1, (1 << TOIE0)
  sts TIMSK0, tmp1
  sbr tmp1, ((1 << DDB5) + (1 << DDB4) + (1 << DDB3) + (1 << DDB2))
  out DDRB, tmp1
  ldi state, 0
  ret

loop:
  nop
end_of_loop:
  ret

main:
  rcall setup
  sei
while_1:
  rcall loop
  rjmp while_1
