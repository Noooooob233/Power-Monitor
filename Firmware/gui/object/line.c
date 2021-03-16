#include "line.h"
#include <rtthread.h>

void gui_line_draw_cb(gui_obj_t *line, gui_area_t *area);

void gui_line_init(gui_obj_t *line, gui_line_attribute_t *attribute)
{
    line->per = NULL;
    line->next = NULL;

    line->hide = 0;

    line->area.x1 = 0;
    line->area.x2 = 0;
    line->area.y1 = 0;
    line->area.y2 = 0;

    line->draw_cb = gui_line_draw_cb;

    line->attribute = (void *)attribute;

    attribute->lines = NULL;
    attribute->size = 0;
}

void gui_line_set_localtion(gui_obj_t *line, int16_t x, int16_t y)
{
    int16_t width = line->area.x2 - line->area.x1;
    int16_t height = line->area.y2 - line->area.y1;

    line->area.x1 = x;
    line->area.x2 = x + width;

    line->area.y1 = y;
    line->area.y2 = y + height;
}

void gui_line_set_size(gui_obj_t *line, int16_t width, int16_t height)
{
    line->area.x2 = line->area.x1 + width - 1;
    line->area.y2 = line->area.y1 + height - 1;
}

void gui_line_add_lines(gui_obj_t *line, gui_line_t *lines, uint16_t size)
{
    gui_line_attribute_t *attribute = line->attribute;

    attribute->lines = lines;
    attribute->size = size;
}

void gui_line_draw_cb(gui_obj_t *line, gui_area_t *area)
{
    gui_line_attribute_t *attribute = line->attribute;

    for (uint16_t i = 0; i < attribute->size; i++)
    {
        gui_draw_line(area,
                      attribute->lines[i].x0,
                      attribute->lines[i].y0,
                      attribute->lines[i].x1,
                      attribute->lines[i].y1,
                      1);
    }
}