#ifndef PWM_H
#define PWM_H

#include "pwm_fading_rules.c"

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
    int16_t next_step_summand;
    uint16_t fading_rules_position;
    struct  pwm_time_measuring_t next_step_time;
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
void pwm_update_fading_controll(char);
void pwm_fading_engine(void);
uint8_t pwm_is_time_for_next_fading_step(char);

#endif
