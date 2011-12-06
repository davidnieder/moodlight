#ifndef PWM_FADING_RULES_C
#define PWM_FADING_RULES_C

#define FADING_RULES_ARRAY_LENGTH 8

const int8_t redchannel_fading_rules[FADING_RULES_ARRAY_LENGTH][4] PROGMEM = {

                                            /* step min sec msec */
                                               { 1, 0,  1,  0 },
                                               { 1, 0,  1,  0 },
                                               { 1, 0,  1,  0 },
                                               { 1, 0,  1,  0 },
                                               { 1, 0,  1,  0 },
                                               { 1, 0,  1,  0 },
                                               { 1, 0,  1,  0 },
                                               { 1, 0,  1,  0 },

                                            };

const int8_t bluechannel_fading_rules[FADING_RULES_ARRAY_LENGTH][4] PROGMEM = {
                                               { 0, 0,  0,  0 },
                                               { 0, 0,  0,  0 },
                                               { 0, 0,  0,  0 },
                                               { 0, 0,  0,  0 },
                                               { 0, 0,  0,  0 },
                                               { 0, 0,  0,  0 },
                                               { 0, 0,  0,  0 },
                                               { 0, 0,  0,  0 }
                                            };

const int8_t greenchannel_fading_rules[FADING_RULES_ARRAY_LENGTH][4] PROGMEM = {
                                               { 0, 0,  0,  0 },
                                               { 0, 0,  0,  0 },
                                               { 0, 0,  0,  0 },
                                               { 0, 0,  0,  0 },
                                               { 0, 0,  0,  0 },
                                               { 0, 0,  0,  0 },
                                               { 0, 0,  0,  0 },
                                               { 0, 0,  0,  0 }
                                            };

#endif
