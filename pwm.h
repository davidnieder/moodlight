#ifndef PWM_H
#define PWM_H

#include "pwm_timeslots.c"
#include "pwm_fading_rules.c"

#ifndef F_CPU
    #error pwm.h: F_CPU not defined
#endif

#define TICKS_PER_MILLISECOND   (F_CPU / 1000)
#define TICKS_TO_NEXT_PWM_SYCLE (F_CPU / (256*100))

/* holds the actuall brightness of each channel */
struct pwm_brightness_t   {
    uint8_t redchannel;
    uint8_t bluechannel;
    uint8_t greenchannel;
};

/* time measuring */
struct pwm_time_measuring_t {
    uint16_t milliseconds;
    uint8_t  seconds;
    uint8_t  minutes;
};

/* when was the last fading step performed */
struct pwm_last_fading_step_t  {
    struct pwm_time_measuring_t
        redchannel,
        bluechannel,
        greenchannel;
};

/* holds all information needed for the next step */
struct pwm_fading_steps_t   {
    int16_t  next_step_summand;
    uint16_t fading_rules_position;
    uint8_t  fading_activated;
    struct   pwm_time_measuring_t next_step_time;
    const prog_int8_t (*fading_rules_array)[4];
};

/* encapsulates pwm_fading_steps_t for three channels */
struct pwm_fading_controll_t    {
    struct pwm_fading_steps_t redchannel,
        bluechannel, greenchannel;
};

volatile uint8_t pwm_count;
uint8_t pwm_fading_is_on;

const char red_channel_id   = 'r';
const char blue_channel_id  = 'b';
const char green_channel_id = 'g';

/* prototypes */
void pwm_init(void);
void pwm_set_brightness(uint8_t, char);
void pwm_update_channels(void);
void pwm_update_fading_controll(struct pwm_fading_steps_t*);
void pwm_fading_engine(void);
void pwm_trigger_fading(uint8_t);
uint8_t pwm_is_time_for_next_fading_step(struct pwm_fading_steps_t*,
                                         struct pwm_time_measuring_t*);

#endif
