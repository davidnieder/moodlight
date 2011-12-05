#ifndef RC5_h
#define RC5_H

#ifndef F_CPU
    #error rc5.h: F_CPU not defined
#endif

#define xRC5_IN     IRPIN
#define xRC5        IRLED     // IR input low active

#define RC5TIME     1.778e-3        // 1.778msec
#define PULSE_MIN   (unsigned char)(F_CPU / 512 * RC5TIME * 0.4 + 0.5)
#define PULSE_1_2   (unsigned char)(F_CPU / 512 * RC5TIME * 0.8 + 0.5)
#define PULSE_MAX   (unsigned char)(F_CPU / 512 * RC5TIME * 1.2 + 0.5)


uint8_t  rc5_bit;                // bit value
uint8_t  rc5_time;               // count bit time
uint16_t rc5_tmp;                // shift bits in
int16_t  rc5_data;               // store result

uint8_t  count;

/* prototypes */
void rc5_init(void);
void rc5_handler(void);

#endif
