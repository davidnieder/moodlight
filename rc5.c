/*
 * Modified version from
 * Peter Daneger's rc5 decoder:
 *
 * http://www.mikrocontroller.net/topic/12216
 */

#ifndef RC5_C
#define RC5_C

#include "rc5.h"

void rc5_init(void)
{
    /* initialze timer/counter 0 */
    TCCR0 |= (1<<CS02);    /* prescaler 256 */
    TIMSK |= (1<<TOIE0);   /* enable timer/counter0 overflow interrupt */
}

void rc5_handler(void)
{

}

ISR (TIMER0_OVF_vect)
{
    TCNT0 = -2;					                            /* 2*256 = 512 cycle */

    if( ++rc5_time > PULSE_MAX )    {			            /* pulse time in range? */
        if( !(rc5_tmp & 0x4000) && (rc5_tmp & 0x2000) )	    /* 14 bits received? */
            rc5_data = rc5_tmp;
            rc5_tmp = 0;
    }

    if( (rc5_bit ^ IRPIN) & (1<<IRLED) )  {		            /* change detected? */
        rc5_bit = ~rc5_bit;				                    /* invert bits */

        if( rc5_time < PULSE_MIN )			                /* pulse to short? */
            rc5_tmp = 0;

        if( !rc5_tmp || rc5_time > PULSE_1_2 )  {		    /* start or long pulse time */

            if( !(rc5_tmp & 0x4000) )			            /* not to many bits */
                rc5_tmp <<= 1;				                /* shift */

            if( !(rc5_bit & 1<<IRLED) )		                /* inverted bit */
                rc5_tmp |= 1;				                /* insert new bit */
                rc5_time = 0;				                /* count next pulse time */
        }
    }
}

#endif
