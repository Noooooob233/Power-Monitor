#ifndef __GUI_AREA_H
#define __GUI_AREA_H
#include "stm32f1xx_hal.h"

typedef struct _gui_area_t
{
    int16_t x1;
    int16_t y1;
    int16_t x2;
    int16_t y2;
} gui_area_t;

uint8_t gui_area_find_common(gui_area_t *res, gui_area_t *area1, gui_area_t *area2);
void gui_area_join(gui_area_t *res, gui_area_t *area1, gui_area_t *area2);

#endif