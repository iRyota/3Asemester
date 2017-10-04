.include "m328Pdef.inc"

.def tmp1 = r17
.def state = r18

.cseg
  rjmp main

.org 0x002A
  rjmp adc_conversion_complete_func


adc_conversion_complete_func:
  lds tmp1, ADCH
  lsl tmp1
  lsl tmp1
  out PORTB, tmp1
  reti

setup:
  sbr tmp1, ((1 << DDB5) + (1 << DDB4) + (1 << DDB3) + (1 << DDB2))
  out DDRB, tmp1
  sbr tmp1, ((1 << ADEN) + (1 << ADPS2) + (1 << ADATE) + (1 << ADIE))
  sts ADCSRA, tmp1
  ldi tmp1, 0
  sts ADCSRB, tmp1
  sbr tmp1, (1 << REFS0)
  sts ADMUX, tmp1
  lds tmp1, ADCSRA
  sbr tmp1, (1 << ADSC)
  sts ADCSRA, tmp1

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
