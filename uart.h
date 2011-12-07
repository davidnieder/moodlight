#ifndef UART_H
#define UART_H

#ifndef PWM_H
#include "pwm.h"
#endif

#ifndef F_CPU
    #error uart.h: F_CPU not defined
#endif
#ifndef BOOLEANS
    #define FALSE 0
    #define TRUE  1
#endif

#ifndef BAUDRATE
#define BAUDRATE    9600UL
#endif

#define UART_BAUDRATE ( F_CPU/(BAUDRATE * 16L)-1 )

#define MAX_RX_STRING_LENGTH 10

/* variables */
volatile uint8_t uart_rx_character_count = 0;
volatile uint8_t uart_string_received = FALSE;
char uart_received_string[MAX_RX_STRING_LENGTH+1];

/* prototypes */
void uart_init(void);
void uart_putc(unsigned char);
void uart_puts(char *);
void uart_handler(void);

#endif
