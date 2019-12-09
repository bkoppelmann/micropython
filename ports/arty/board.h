#ifndef MICROPY_INCLUDED_UPDUINO_BOARD_H
#define MICROPY_INCLUDED_UPDUINO_BOARD_H

#include "hifive1_spi.h"

void led_init(void);
void led_state(int led, int state);
void led_toggle(int led);

#endif // MICROPY_INCLUDED_UPDUINO_BOARD_H
