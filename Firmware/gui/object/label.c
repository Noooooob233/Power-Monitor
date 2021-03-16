#include "label.h"
#include <rtthread.h>

void gui_label_draw_cb(gui_obj_t *label, gui_area_t *area);

void gui_label_init(gui_obj_t *label, gui_label_attribute_t *attribute)
{
    label->per = NULL;
    label->next = NULL;

    label->hide = 0;

    label->area.x1 = 0;
    label->area.x2 = 0;
    label->area.y1 = 0;
    label->area.y2 = 0;

    label->draw_cb = gui_label_draw_cb;

    label->attribute = (void *)attribute;

    attribute->font = NULL;
    attribute->size = 0;
    attribute->text_x_offset = 0;
    attribute->text_y_offset = 0;
}

void gui_label_set_localtion(gui_obj_t *label, int16_t x, int16_t y)
{
    int16_t width = label->area.x2 - label->area.x1;
    int16_t height = label->area.y2 - label->area.y1;

    label->area.x1 = x;
    label->area.x2 = x + width;

    label->area.y1 = y;
    label->area.y2 = y + height;
}

void gui_label_set_size(gui_obj_t *label, int16_t width, int16_t height)
{
    label->area.x2 = label->area.x1 + width - 1;
    label->area.y2 = label->area.y1 + height - 1;
}

void gui_label_set_x(gui_obj_t *label, int16_t x)
{
    int16_t width = label->area.x2 - label->area.x1;

    label->area.x1 = x;
    label->area.x2 = x + width;
}

void gui_label_set_y(gui_obj_t *label, int16_t y)
{
    int16_t height = label->area.y2 - label->area.y1;

    label->area.y1 = y;
    label->area.y2 = y + height;
}

void gui_label_set_font(gui_obj_t *label, gui_font_t *font)
{
    ((gui_label_attribute_t *)label->attribute)->font = font;
}

void gui_label_set_text_offset(gui_obj_t *label, int16_t x_offset, int16_t y_offset)
{
    ((gui_label_attribute_t *)label->attribute)->text_x_offset = x_offset;
    ((gui_label_attribute_t *)label->attribute)->text_y_offset = y_offset;
}

void gui_label_set_text_by_arr(gui_obj_t *label, char *text, uint8_t size)
{
    gui_label_attribute_t *attribute = label->attribute;

    size = size > LABEL_TEXT_MAX_SIZE ? LABEL_TEXT_MAX_SIZE : size;

    rt_memcpy(attribute->text, text, size);
    attribute->size = size; // 字数
}

void gui_label_set_text_by_string(gui_obj_t *label, char *str)
{
    gui_label_attribute_t *attribute = label->attribute;
    uint8_t size = rt_strlen(str);

    size = size > LABEL_TEXT_MAX_SIZE ? LABEL_TEXT_MAX_SIZE : size;

    rt_memcpy(attribute->text, str, size);
    attribute->size = size; // 字数
}

void gui_label_draw_cb(gui_obj_t *label, gui_area_t *area)
{
    gui_label_attribute_t *attribute = label->attribute;
    gui_area_t area_common;

    if (gui_area_find_common(&area_common, area, &label->area))
    {
        gui_draw_string(&area_common,
                        label->area.x1 + attribute->text_x_offset,
                        label->area.y1 + attribute->text_y_offset,
                        attribute->font,
                        attribute->text,
                        attribute->size);
    }
}
