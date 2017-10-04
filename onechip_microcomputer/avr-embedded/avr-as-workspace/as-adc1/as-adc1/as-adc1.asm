.include "m328Pdef.inc"

.def tmp1 = r17
.def tmp2 = r18
.def state = r19

.cseg
  rjmp main


setup:
  sbr tmp1, ((1 << DDB5) + (1 << DDB4) + (1 << DDB3) + (1 << DDB2))
  out DDRB, tmp1
  sbr tmp1, ((1 << ADEN) + (1 << ADPS2))
  sts ADCSRA, tmp1
  ldi tmp1, 0
  sts ADCSRB, tmp1
  sbr tmp1, (1 << REFS0)
  sts ADMUX, tmp1
  ret

;
loop:
  lds tmp1, ADCSRA
  sbr tmp2, (1 << ADSC)
  or tmp1, tmp2
  sts ADCSRA, tmp1

loop_until_adc_is_not_done:
  lds tmp1, ADCSRA
  sbrc tmp1, ADIF
  rjmp loop_until_adc_is_not_done
  rjmp after_conversion

after_conversion:
  lds tmp1, ADCH
  lsl tmp1
  lsl tmp1
  out PORTB, tmp1
  lds tmp1, ADCSRA
  sbr tmp2, (1 << ADIF)
  or tmp1, tmp2
  sts ADCSRA, tmp1

end_of_loop:
  ret

main:
  rcall setup
while_1:
  rcall loop
  rjmp while_1
