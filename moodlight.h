#ifndef MOODLIGHT_H
#define MOODLOGHT_H

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#ifndef BOOLEANS
#define FALSE 0
#define TRUE  1
#endif

#define GREENLED  PB1
#define REDLED    PB2
#define BLUELED   PB3

#define LEDPORT   PORTB

#define IRLED     PD6

#define IRPIN     PIND

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

//#include "pwm.c"
#include "uart.c"
#include "rc5.c"

#endif
