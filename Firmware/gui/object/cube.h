#ifndef __GUI_CUBE_H
#define __GUI_CUBE_H
#include "stm32f1xx_hal.h"
#include "core.h"

typedef struct _gui_cube_attribute_t
{

} gui_cube_attribute_t;

void gui_cube_init(gui_obj_t *spec, gui_cube_attribute_t *attribute);
void gui_cube_set_localtion(gui_obj_t *spec, int16_t x, int16_t y);
void gui_cube_set_size(gui_obj_t *spec, int16_t width, int16_t height);
void gui_cube_data_update(gui_obj_t *spec, uint8_t *data);

#endif