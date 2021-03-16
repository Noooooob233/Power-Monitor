#ifndef __GUI_LINE_H
#define __GUI_LINE_H
#include "stm32f1xx_hal.h"
#include "core.h"

typedef struct _gui_line_t
{
    int16_t x0;
    int16_t y0;
    int16_t x1;
    int16_t y1;
} gui_line_t;

typedef struct _gui_line_attribute_t
{
    gui_line_t *lines;

    uint16_t size;

} gui_line_attribute_t;

void gui_line_init(gui_obj_t *line, gui_line_attribute_t *attribute);
void gui_line_set_localtion(gui_obj_t *line, int16_t x, int16_t y);
void gui_line_set_size(gui_obj_t *line, int16_t width, int16_t height);
void gui_line_add_lines(gui_obj_t *line, gui_line_t *lines, uint16_t size);

#endif