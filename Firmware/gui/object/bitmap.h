#ifndef __GUI_BITMAP_H
#define __GUI_BITMAP_H
#include "stm32f1xx_hal.h"
#include "core.h"

typedef struct _gui_bitmap_attribute_t
{
    uint8_t *bitmap;

    int16_t x_offset;
    int16_t y_offset;

    uint16_t width;
    uint16_t height;

} gui_bitmap_attribute_t;

void gui_bitmap_init(gui_obj_t *bitmap, gui_bitmap_attribute_t *attribute);
void gui_bitmap_set_localtion(gui_obj_t *bitmap, int16_t x, int16_t y);
void gui_bitmap_set_size(gui_obj_t *bitmap, int16_t width, int16_t height);
void gui_bitmap_set_x(gui_obj_t *bitmap, int16_t x);
void gui_bitmap_set_y(gui_obj_t *bitmap, int16_t y);
void gui_bitmap_set_bitmap(gui_obj_t *bitmap, uint8_t *bmp, uint16_t width, uint16_t height);

#endif
