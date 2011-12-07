#ifndef UART_C
#define UART_C

#include "uart.h"

void uart_init(void)
{
    /* baud rate */
    UBRRH = (UART_BAUDRATE >> 8);
    UBRRL = UART_BAUDRATE;
    /* asynchronous mode, 8 bit character size, 1 stop bit, even parity */
    UCSRC = (1<<URSEL) | (1<<UPM1) | (1<<UCSZ1) | (1<<UCSZ0);
    /* enable uart receiver, transmitter and receive interrupt */
    UCSRB |= (1<<RXEN) | (1<<TXEN) | (1<<RXCIE);
}

void uart_putc(unsigned char c)
{
    /* wait until data can be sent */
    while ( !(UCSRA & (1<<UDRE)) )  {}
 
    /* sent */
    UDR = c;
}

void uart_puts (char *s)
{
    /* calling putc for every character in s */
    while (*s)  {
        uart_putc(*s);
        s++;
    }
}

void uart_handler(void)
{
    if( uart_received_string[0] == 'p' )    {

        if( uart_received_string[1] == 'r' || uart_received_string[1] == 'b'  ||
            uart_received_string[1] == 'g' )    {

            uint8_t value = 0;
            if( uart_received_string[2] >= '0' && uart_received_string[2] <= '3' )  {
                value += (uart_received_string[2]-48)*10;  }
            if( uart_received_string[3] >= '0' && uart_received_string[3] <= '9' )  {
                value += (uart_received_string[3]-48);   }

            pwm_set_brightness(value, uart_received_string[1]);
            uart_puts("ok\n");
        }
    }

} 

/* uart receive complete interrupt handler */
ISR(USART_RXC_vect)
{
    uint8_t recv_char;

    /* parity check */
    if( UCSRA & PE )   {
        recv_char = UDR;
        return;
    }

    /* get received char */
    recv_char = UDR;

    /* if the last string was not read yet do not read a new character */
    if( !uart_string_received )   {

        /* save new character if it is not newline or carriage return and if there is space left */
        if( recv_char != '\n' && recv_char != '\r' &&
            uart_rx_character_count < MAX_RX_STRING_LENGTH )   {

            uart_received_string[uart_rx_character_count] = recv_char;
            uart_rx_character_count++;
        }
        /* string complete received */
        else    {
            uart_received_string[uart_rx_character_count] = '\0';
            uart_rx_character_count = 0;
            uart_string_received = TRUE;
        }
    }
}

#endif
