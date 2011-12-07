#ifndef RC5_C
#define RC5_C

#include "rc5.h"

void rc5_init(void)
{
    /* initialze timer/counter 0 */
    TCCR0 |= (1<<CS02);    /* prescaler 256 */
    TIMSK |= (1<<TOIE0);   /* enable timer/counter0 overflow interrupt */

    count = 0;
}

void rc5_handler(void)
{

}

ISR (TIMER0_OVF_vect)
{
  uint16_t tmp = rc5_tmp;				// for faster access
  TCNT0 = -2;					// 2 * 256 = 512 cycle

  if( ++rc5_time > PULSE_MAX ){			// count pulse time
    if( !(tmp & 0x4000) && tmp & 0x2000 )	// only if 14 bits received
      rc5_data = tmp;
    tmp = 0;
  }

  if( (rc5_bit ^ xRC5_IN) & 1<<xRC5 ){		// change detect
    rc5_bit = ~rc5_bit;				// 0x00 -> 0xFF -> 0x00

    if( rc5_time < PULSE_MIN )			// to short
      tmp = 0; 

    if( !tmp || rc5_time > PULSE_1_2 ){		// start or long pulse time
      if( !(tmp & 0x4000) )			// not to many bits
        tmp <<= 1;				// shift
      if( !(rc5_bit & 1<<xRC5) )		// inverted bit
        tmp |= 1;				// insert new bit
      rc5_time = 0;				// count next pulse time
    }
  }

  rc5_tmp = tmp;

}

#endif
