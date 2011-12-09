#ifndef PWM_C
#define PWM_C

#include "pwm.h"

struct pwm_brightness_t pwm_brightness;
volatile struct pwm_time_measuring_t pwm_actual_time;
struct pwm_last_fading_step_t pwm_last_fading_step;
struct pwm_fading_controll_t pwm_fading_controll;

const uint8_t pwm_timeslot_table[32] PROGMEM = {
                                 0,   1,   2,   3,   4,   5,
                                 6,   7,   8,   9,  10,  11,
                                12,  13,  14,  16,  19,  23,
                                27,  32,  38,  45,  54,  64,
                                76,  91, 108, 128, 152, 181,
                               215, 255
                            };

void pwm_init(void)
{
    /* initialize timer/counter 1 */
    TCCR1B |= (1<<CS10);                    /* no prescaling */
    OCR1A   = 313;                          /* compare value - software pwm */
    OCR1B   = 8000;                         /* compare value - time measuring */
    TIMSK  |= (1<<OCIE1A) | (1<<OCIE1B);    /* enable compare interupt a and b */

    pwm_count = 0;
    pwm_fading_is_on = TRUE;

    /* initialize all structures - puh */

    pwm_brightness.redchannel = 0;
    pwm_brightness.bluechannel = 0;
    pwm_brightness.greenchannel = 0;

    pwm_actual_time.milliseconds = 0;
    pwm_actual_time.seconds = 0;
    pwm_actual_time.minutes = 0;

    pwm_last_fading_step.redchannel.milliseconds = 0;
    pwm_last_fading_step.redchannel.seconds = 0;
    pwm_last_fading_step.redchannel.minutes = 0;

    pwm_last_fading_step.bluechannel.milliseconds = 0;
    pwm_last_fading_step.bluechannel.seconds = 0;
    pwm_last_fading_step.greenchannel.minutes = 0;

    pwm_last_fading_step.greenchannel.milliseconds = 0;
    pwm_last_fading_step.greenchannel.seconds = 0;
    pwm_last_fading_step.greenchannel.minutes = 0;

    pwm_fading_controll.redchannel.fading_rules_position = 0;
    pwm_fading_controll.redchannel.next_step_summand = pgm_read_byte(
        &redchannel_fading_rules[0][0]);
    pwm_fading_controll.redchannel.next_step_time.minutes = pgm_read_byte(
        &redchannel_fading_rules[0][1]);
    pwm_fading_controll.redchannel.next_step_time.milliseconds = pgm_read_byte(
        &redchannel_fading_rules[0][2]);
    pwm_fading_controll.redchannel.next_step_time.milliseconds = pgm_read_byte(
        &redchannel_fading_rules[0][3]) *5;
 
    pwm_fading_controll.bluechannel.fading_rules_position = 0;
    pwm_fading_controll.bluechannel.next_step_summand = pgm_read_byte(
        &bluechannel_fading_rules[0][0]);
    pwm_fading_controll.bluechannel.next_step_time.minutes = pgm_read_byte(
        &bluechannel_fading_rules[0][1]);
    pwm_fading_controll.bluechannel.next_step_time.seconds = pgm_read_byte(
        &bluechannel_fading_rules[0][2]);
    pwm_fading_controll.bluechannel.next_step_time.milliseconds = pgm_read_byte(
        &bluechannel_fading_rules[0][3]) *5;
 
    pwm_fading_controll.greenchannel.fading_rules_position = 0;
    pwm_fading_controll.greenchannel.next_step_summand = pgm_read_byte(
        &greenchannel_fading_rules[0][0]);
    pwm_fading_controll.greenchannel.next_step_time.minutes = pgm_read_byte(
        &greenchannel_fading_rules[0][1]);
    pwm_fading_controll.greenchannel.next_step_time.seconds = pgm_read_byte(
        &greenchannel_fading_rules[0][2]);
    pwm_fading_controll.greenchannel.next_step_time.milliseconds = pgm_read_byte(
        &greenchannel_fading_rules[0][3]) *5;
 
}

void pwm_set_brightness(uint8_t value, char channel)
{
    if( value >= 0 && value <= 31 ) {
        if( channel == red_channel_id )
            pwm_brightness.redchannel = value;
        else if( channel == blue_channel_id )
            pwm_brightness.bluechannel = value;
        else if( channel == green_channel_id )
            pwm_brightness.greenchannel = value;
    }
}

void pwm_fading_engine(void)
{
    if( pwm_fading_is_on )  {
        /* red channel */
        if( pwm_is_time_for_next_fading_step( red_channel_id ) )   {
            pwm_set_brightness((pwm_brightness.redchannel +
                                pwm_fading_controll.redchannel.next_step_summand),
                                red_channel_id );

            pwm_last_fading_step.redchannel = pwm_actual_time;
            pwm_fading_controll.redchannel.fading_rules_position++;
            pwm_update_fading_controll( red_channel_id );

            uart_puts("redch: ");
            uart_puti( pwm_brightness.redchannel );
            uart_putc('\n');

            uart_puts("pos: ");
            uart_puti( pwm_fading_controll.redchannel.fading_rules_position );
            uart_putc('\n');

            uart_puts("actsec: ");
            uart_puti( pwm_actual_time.seconds );
            uart_putc('\n');

            uart_puts("lasec: ");
            uart_puti( pwm_last_fading_step.redchannel.seconds );
            uart_putc('\n');
            
        }
        /* blue channel */
        if( pwm_is_time_for_next_fading_step( blue_channel_id ) )   {
            pwm_set_brightness((pwm_brightness.bluechannel +
                                pwm_fading_controll.bluechannel.next_step_summand),
                                blue_channel_id );

            pwm_last_fading_step.bluechannel = pwm_actual_time;
            pwm_fading_controll.bluechannel.fading_rules_position++;
            pwm_update_fading_controll( blue_channel_id );
        }

        /* green channel */
        if( pwm_is_time_for_next_fading_step( green_channel_id ) )   {
            pwm_set_brightness((pwm_brightness.greenchannel +
                                pwm_fading_controll.greenchannel.next_step_summand),
                                green_channel_id );

            pwm_last_fading_step.greenchannel = pwm_actual_time;
            pwm_fading_controll.greenchannel.fading_rules_position++;
            pwm_update_fading_controll( green_channel_id );
        }
    }
}

/* checks if it is time for next fading step - ugly implemented */
uint8_t pwm_is_time_for_next_fading_step(char channel) {
    if( channel == red_channel_id ) {
        if( pwm_fading_controll.redchannel.next_step_time.minutes == 0 &&
            pwm_fading_controll.redchannel.next_step_time.seconds == 0 &&
            pwm_fading_controll.redchannel.next_step_time.milliseconds +
            pwm_last_fading_step.redchannel.milliseconds <=
            pwm_actual_time.milliseconds )

            return TRUE;
        else if(
            pwm_fading_controll.redchannel.next_step_time.minutes == 0 &&
            pwm_fading_controll.redchannel.next_step_time.seconds +
            pwm_last_fading_step.redchannel.seconds + 1 <=
            pwm_actual_time.seconds )

            return TRUE;
            if(
            pwm_fading_controll.redchannel.next_step_time.minutes == 0 &&
            pwm_fading_controll.redchannel.next_step_time.seconds +
            pwm_last_fading_step.redchannel.seconds <=
            pwm_actual_time.seconds &&
            pwm_fading_controll.redchannel.next_step_time.milliseconds +
            pwm_last_fading_step.redchannel.milliseconds <=
            pwm_actual_time.milliseconds )

            return TRUE;
        else if(
            pwm_fading_controll.redchannel.next_step_time.minutes +
            pwm_last_fading_step.redchannel.minutes <=
            pwm_actual_time.minutes &&
            pwm_fading_controll.redchannel.next_step_time.seconds +
            pwm_last_fading_step.redchannel.seconds <=
            pwm_actual_time.seconds &&
            pwm_fading_controll.redchannel.next_step_time.milliseconds +
            pwm_last_fading_step.redchannel.milliseconds <=
            pwm_actual_time.milliseconds )

            return TRUE;
    }
    else if( channel == blue_channel_id ) {
        if( pwm_fading_controll.bluechannel.next_step_time.minutes == 0 &&
            pwm_fading_controll.bluechannel.next_step_time.seconds == 0 &&
            pwm_fading_controll.bluechannel.next_step_time.milliseconds +
            pwm_last_fading_step.bluechannel.milliseconds <=
            pwm_actual_time.milliseconds )

            return TRUE;
        else if(
            pwm_fading_controll.bluechannel.next_step_time.minutes == 0 &&
            pwm_fading_controll.bluechannel.next_step_time.seconds +
            pwm_last_fading_step.bluechannel.seconds <=
            pwm_actual_time.seconds &&
            pwm_fading_controll.bluechannel.next_step_time.milliseconds +
            pwm_last_fading_step.bluechannel.milliseconds <=
            pwm_actual_time.milliseconds )

            return TRUE;
        else if(
            pwm_fading_controll.bluechannel.next_step_time.minutes +
            pwm_last_fading_step.bluechannel.minutes <=
            pwm_actual_time.minutes &&
            pwm_fading_controll.bluechannel.next_step_time.seconds +
            pwm_last_fading_step.bluechannel.seconds <=
            pwm_actual_time.seconds &&
            pwm_fading_controll.bluechannel.next_step_time.milliseconds +
            pwm_last_fading_step.bluechannel.milliseconds <=
            pwm_actual_time.milliseconds )

            return TRUE;
    }
    else if( channel == green_channel_id ) {
        if( pwm_fading_controll.greenchannel.next_step_time.minutes == 0 &&
            pwm_fading_controll.greenchannel.next_step_time.seconds == 0 &&
            pwm_fading_controll.greenchannel.next_step_time.milliseconds +
            pwm_last_fading_step.greenchannel.milliseconds <=
            pwm_actual_time.milliseconds )

            return TRUE;
        else if(
            pwm_fading_controll.greenchannel.next_step_time.minutes == 0 &&
            pwm_fading_controll.greenchannel.next_step_time.seconds +
            pwm_last_fading_step.greenchannel.seconds <=
            pwm_actual_time.seconds &&
            pwm_fading_controll.greenchannel.next_step_time.milliseconds +
            pwm_last_fading_step.greenchannel.milliseconds <=
            pwm_actual_time.milliseconds )

            return TRUE;
        else if(
            pwm_fading_controll.greenchannel.next_step_time.minutes +
            pwm_last_fading_step.greenchannel.minutes <=
            pwm_actual_time.minutes &&
            pwm_fading_controll.greenchannel.next_step_time.seconds +
            pwm_last_fading_step.greenchannel.seconds <=
            pwm_actual_time.seconds &&
            pwm_fading_controll.greenchannel.next_step_time.milliseconds +
            pwm_last_fading_step.greenchannel.milliseconds <=
            pwm_actual_time.milliseconds )

            return TRUE;
    }
    return FALSE;
}

/* prepare pwm_fading_steps for next fading step */
void pwm_update_fading_controll(char channel)   {
    if( channel == red_channel_id ) {
        if( pwm_fading_controll.redchannel.fading_rules_position >=
            FADING_RULES_ARRAY_LENGTH ) {
            pwm_fading_controll.redchannel.fading_rules_position = 0;
            uart_puts("update\n");    }

        pwm_fading_controll.redchannel.next_step_summand =
            pgm_read_byte( &redchannel_fading_rules
            [pwm_fading_controll.redchannel.fading_rules_position][0] );

        pwm_fading_controll.redchannel.next_step_time.minutes =
            pgm_read_byte( &redchannel_fading_rules
            [pwm_fading_controll.redchannel.fading_rules_position][1] );

        pwm_fading_controll.redchannel.next_step_time.seconds =
            pgm_read_byte( &redchannel_fading_rules
            [pwm_fading_controll.redchannel.fading_rules_position][2] );

        pwm_fading_controll.redchannel.next_step_time.milliseconds =
            pgm_read_byte( &redchannel_fading_rules
            [pwm_fading_controll.redchannel.fading_rules_position][3] );
    }
    else if( channel == blue_channel_id )   {
        if( pwm_fading_controll.bluechannel.fading_rules_position >=
            FADING_RULES_ARRAY_LENGTH )
            pwm_fading_controll.bluechannel.fading_rules_position = 0;

        pwm_fading_controll.bluechannel.next_step_summand =
            pgm_read_byte( &bluechannel_fading_rules
            [pwm_fading_controll.bluechannel.fading_rules_position][0] );

        pwm_fading_controll.bluechannel.next_step_time.minutes =
            pgm_read_byte( &bluechannel_fading_rules
            [pwm_fading_controll.bluechannel.fading_rules_position][1] );

        pwm_fading_controll.bluechannel.next_step_time.seconds =
            pgm_read_byte( &bluechannel_fading_rules
            [pwm_fading_controll.bluechannel.fading_rules_position][2] );

        pwm_fading_controll.bluechannel.next_step_time.milliseconds =
            pgm_read_byte( &bluechannel_fading_rules
            [pwm_fading_controll.bluechannel.fading_rules_position][3] );
    }
    else if( channel == green_channel_id )  {
        if( pwm_fading_controll.greenchannel.fading_rules_position >=
            FADING_RULES_ARRAY_LENGTH )
            pwm_fading_controll.greenchannel.fading_rules_position = 0;

        pwm_fading_controll.greenchannel.next_step_summand =
            pgm_read_byte( &greenchannel_fading_rules
            [pwm_fading_controll.greenchannel.fading_rules_position][0] );

        pwm_fading_controll.greenchannel.next_step_time.minutes =
            pgm_read_byte( &greenchannel_fading_rules
            [pwm_fading_controll.greenchannel.fading_rules_position][1] );

        pwm_fading_controll.greenchannel.next_step_time.seconds =
            pgm_read_byte( &greenchannel_fading_rules
            [pwm_fading_controll.greenchannel.fading_rules_position][2] );

        pwm_fading_controll.greenchannel.next_step_time.milliseconds =
            pgm_read_byte( &greenchannel_fading_rules
            [pwm_fading_controll.greenchannel.fading_rules_position][3] );
    }
}

/* timer1 compare a interrupt - software pwm */
ISR(TIMER1_COMPA_vect)
{
    OCR1A += 313-1;

    uint8_t output = 0;
    pwm_count++;

    if( pwm_count < pgm_read_byte( &pwm_timeslot_table
                                    [pwm_brightness.redchannel] ))
        output |= (1<<REDLED);
    else
        output &= ~(1<<REDLED);

    if( pwm_count < pgm_read_byte( &pwm_timeslot_table
                                    [pwm_brightness.bluechannel] ))
        output |= (1<<BLUELED);
    else
        output &= ~(1<<BLUELED);

    if( pwm_count < pgm_read_byte( &pwm_timeslot_table
                                    [pwm_brightness.greenchannel] ))
        output |= (1<<GREENLED);
    else
        output &= ~(1<<GREENLED);

    LEDPORT = output;
}

/* timer1 compare b interrupt - time measuring */
ISR(TIMER1_COMPB_vect)
{
    OCR1B += 8000-1;
    pwm_actual_time.milliseconds++;

    if( pwm_actual_time.milliseconds >= 1000 )   {
        pwm_actual_time.milliseconds = 0;
        pwm_actual_time.seconds++;
    } 
    if( pwm_actual_time.seconds >= 60 )    {
        pwm_actual_time.seconds = 0;
        pwm_actual_time.minutes++;

        if( pwm_last_fading_step.redchannel.seconds >= 59 )
            pwm_last_fading_step.redchannel.seconds = 0;
        if( pwm_last_fading_step.bluechannel.seconds >= 59 )
            pwm_last_fading_step.bluechannel.seconds = 0;
        if( pwm_last_fading_step.greenchannel.seconds >= 59 )
            pwm_last_fading_step.greenchannel.seconds = 0;
 
    }
    if( pwm_actual_time.minutes == 255 ) {
        pwm_actual_time.minutes = 0;
        /* to implement: update other time structures */
    }
}

#endif
