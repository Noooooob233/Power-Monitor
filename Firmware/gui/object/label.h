#ifndef __GUI_LABEL_H
#define __GUI_LABEL_H
#include "stm32f1xx_hal.h"
#include "core.h"
#include "font.h"

#define LABEL_TEXT_MAX_SIZE 20

typedef struct _gui_label_attribute_t
{
    char text[LABEL_TEXT_MAX_SIZE];
    uint8_t size;

    gui_font_t *font;

    int16_t text_x_offset;
    int16_t text_y_offset;
} gui_label_attribute_t;

void gui_label_init(gui_obj_t *label, gui_label_attribute_t *attribute);
void gui_label_set_localtion(gui_obj_t *label, int16_t x, int16_t y);
void gui_label_set_size(gui_obj_t *label, int16_t width, int16_t height);
void gui_label_set_x(gui_obj_t *label, int16_t x);
void gui_label_set_y(gui_obj_t *label, int16_t y);
void gui_label_set_font(gui_obj_t *label, gui_font_t *font);
void gui_label_set_text_offset(gui_obj_t *label, int16_t x_offset, int16_t y_offset);

inline void gui_label_set_hide(gui_obj_t *label, uint8_t en)
{
    gui_obj_set_hide(label, en);
}

inline int16_t gui_label_get_text_x_offset(gui_obj_t *label)
{
    return ((gui_label_attribute_t *)label->attribute)->text_x_offset;
}

inline int16_t gui_label_get_text_y_offset(gui_obj_t *label)
{
    return ((gui_label_attribute_t *)label->attribute)->text_y_offset;
}

void gui_label_set_text_by_arr(gui_obj_t *label, char *text, uint8_t size);
void gui_label_set_text_by_string(gui_obj_t *label, char *str);

#endif