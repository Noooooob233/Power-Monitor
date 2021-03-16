#include "canvas.h"

void gui_canvas_draw_cb(gui_obj_t *canvas, gui_area_t *area);

void gui_canvas_init(gui_obj_t *canvas, gui_canvas_attribute_t *attribute)
{
    canvas->per = NULL;
    canvas->next = NULL;

    canvas->hide = 0;

    canvas->area.x1 = 0;
    canvas->area.x2 = 0;
    canvas->area.y1 = 0;
    canvas->area.y2 = 0;

    canvas->draw_cb = gui_canvas_draw_cb;

    canvas->attribute = (void *)attribute;

    attribute->user_draw_cb = NULL;
}

void gui_canvas_set_localtion(gui_obj_t *canvas, int16_t x, int16_t y)
{
}

void gui_canvas_set_size(gui_obj_t *canvas, int16_t width, int16_t height)
{
}

void gui_canvas_set_x(gui_obj_t *canvas, int16_t x)
{
}

void gui_canvas_set_y(gui_obj_t *canvas, int16_t y)
{
}

void gui_canvas_draw_bitmap_fast(gui_obj_t *canvas, uint8_t *bitmap)
{
    gui_canvas_attribute_t *attribute = canvas->attribute;

    rt_memcpy(attribute->buffer, bitmap, 1024);
}

void gui_canvas_draw_cb(gui_obj_t *canvas, gui_area_t *area)
{
}