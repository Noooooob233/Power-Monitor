#ifndef __GUI_SPECTRUM_H
#define __GUI_SPECTRUM_H
#include "stm32f1xx_hal.h"
#include "core.h"

typedef struct _gui_spectrum_attribute_t
{
    uint8_t spectrum_data[128];
    uint8_t spectrum_data_max[128];
    int8_t spectrum_data_max_stay_timer[128];
    uint8_t spectrum_data_draw[128];

    uint8_t width;
    uint8_t interval;

    uint8_t max_stay_time;
    uint8_t fall_time;
    uint8_t style;
} gui_spectrum_attribute_t;

void gui_spectrum_init(gui_obj_t *spec, gui_spectrum_attribute_t *attribute);
void gui_spectrum_set_localtion(gui_obj_t *spec, int16_t x, int16_t y);
void gui_spectrum_set_size(gui_obj_t *spec, int16_t width, int16_t height);
void gui_spectrum_data_update(gui_obj_t *spec, uint8_t *data);
void gui_spectrum_data_reset(gui_obj_t *spec);

#endif