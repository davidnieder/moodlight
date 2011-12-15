#ifndef RC5_h
#define RC5_H

#ifndef F_CPU
    #error rc5.h: F_CPU not defined
#endif

#ifndef IRPIN
    #error rc5.h: IRPIN not defined
#endif

#ifndef IRLED
    #error rc5.h: IRLED not defined
#endif


#define RC5TIME     1.778e-3        /* 1.778msec */
#define PULSE_MIN   (uint8_t)(F_CPU / 512 * RC5TIME * 0.4 + 0.5)
#define PULSE_1_2   (uint8_t)(F_CPU / 512 * RC5TIME * 0.8 + 0.5)
#define PULSE_MAX   (uint8_t)(F_CPU / 512 * RC5TIME * 1.2 + 0.5)


uint8_t  rc5_bit;       /* bit value */
uint8_t  rc5_time;      /* measure bit time */
uint16_t rc5_tmp;       /* temporary value */
int16_t  rc5_data;      /* store result */

/* prototypes */
void rc5_init(void);
void rc5_handler(void);

#endif
