#ifndef __GUI_CON_H
#define __GUI_CON_H
#include "stm32f1xx_hal.h"
#include "core.h"

typedef struct _gui_container_attribute_t
{
    gui_obj_t *child;

    int16_t child_x_offset;
    int16_t child_y_offset;

    uint8_t edge_style;
} gui_container_attribute_t;

void gui_container_init(gui_obj_t *con, gui_container_attribute_t *attribute);
void gui_container_set_localtion(gui_obj_t *con, int16_t x, int16_t y);
void gui_container_set_x(gui_obj_t *con, int16_t x);
void gui_container_set_y(gui_obj_t *con, int16_t y);
void gui_container_set_size(gui_obj_t *con, int16_t width, int16_t height);
void gui_container_set_edge_style(gui_obj_t *con, uint8_t style);
void gui_container_add_obj(gui_obj_t *con, gui_obj_t *obj);

#endif