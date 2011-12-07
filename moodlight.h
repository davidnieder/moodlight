#ifndef MOODLIGHT_H
#define MOODLOGHT_H

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#ifndef BOOLEANS
#define FALSE 0
#define TRUE  1
#endif

#define GREENLED  PC0
#define REDLED    PC1
#define BLUELED   PC2

#define LEDPORT   PORTC

#define IRLED     PB5

#define IRPIN     PINB

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "uart.c"
#include "pwm.c"
#include "rc5.c"

#endif
