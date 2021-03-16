#ifndef __GUI_CANVAS_H
#define __GUI_CANVAS_H
#include "stm32f1xx_hal.h"
#include "core.h"

typedef void (*gui_user_draw_cb_t)(gui_obj_t *, gui_area_t *);

typedef struct _gui_canvas_attribute_t
{
    gui_user_draw_cb_t user_draw_cb;

    uint8_t buffer[1024];
} gui_canvas_attribute_t;

void gui_canvas_init(gui_obj_t *canvas, gui_canvas_attribute_t *attribute);
void gui_canvas_set_localtion(gui_obj_t *canvas, int16_t x, int16_t y);
void gui_canvas_set_size(gui_obj_t *canvas, int16_t width, int16_t height);
void gui_canvas_set_x(gui_obj_t *canvas, int16_t x);
void gui_canvas_set_y(gui_obj_t *canvas, int16_t y);

void gui_canvas_draw_bitmap_fast(gui_obj_t *canvas, uint8_t *bitmap);

#endif
