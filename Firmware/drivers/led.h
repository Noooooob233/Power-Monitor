#ifndef __LED_H
#define __LED_H

#include "stm32f1xx_hal.h"

void led_init();

void led_control(uint16_t val);

#endif
