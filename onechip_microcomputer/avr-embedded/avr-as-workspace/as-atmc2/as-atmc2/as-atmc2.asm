.include "m328Pdef.inc"

.def tmp1 = r18
.def tmp2 = r19
.def state = r20

.cseg
  rjmp main
  ret

setup:
  ldi tmp1, 60
  out DDRB, tmp1
  ldi tmp1, 0
  out DDRD, tmp1
  out PORTD, tmp1
  out PORTB, tmp1
  ldi state, 0
  ret

loop:
  in tmp1, PIND
  sbrs tmp1, 5
  rjmp update_old_val
  rjmp check_old_val
update_old_val:
  mov tmp2, tmp1
  rjmp change_portb

check_old_val:
  sbrc tmp2, 5
  rjmp update_old_val2
  rjmp state_change

update_old_val2:
  mov tmp2, tmp1
  rjmp change_portb

state_change:
  sbrs state, 0
  rjmp set_state
  clr state
  rjmp update_old_val3

set_state:
  ser state


update_old_val3:
  mov tmp2, tmp1

change_portb:
  sbrs state, 0
  rjmp turn_off
  rjmp turn_on

turn_off:
  ldi tmp1, 0
  out PORTB, tmp1
  rjmp end_of_loop

turn_on:
  ldi tmp1, 60
  out PORTB, tmp1

end_of_loop:
  ret

main:
  rcall setup
while_1:
  rcall loop
  rjmp while_1
