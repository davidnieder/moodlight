#ifndef MOODLIGHT_C
#define MOODLIGHT_C

#include "moodlight.h"

int main(void)
{
    cli();

    /* define LED pins as output pins */
    DDRB |= (1<<REDLED) | (1<<BLUELED) | (1<<GREENLED);
    /* turn LEDs off */
    PORTB &= ~( (1<<REDLED) | (1<<BLUELED) | (1<<GREENLED) );

    /* initiate modules */
    uart_init();
    rc5_init();
    pwm_init();

    /* send hello message */
    uart_puts("init\n");

    sei();

    while (TRUE)    {

        /* check for new uart data */
        if( uart_string_received )  {
            uart_protocol_handler();
            uart_string_received = FALSE;
        }

        /* check for new infrared data */
        if( rc5_data )  {
//            rc5_handler();
            uart_puts("rc5 recv\n");
            _delay_ms(100);
            rc5_data = FALSE;
        }

        pwm_fading_engine();
    }
}

#endif
