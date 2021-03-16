#ifndef __GUI_CORE_H
#define __GUI_CORE_H
#include "stm32f1xx_hal.h"
#include "rtthread.h"
#include "area.h"
#include "font.h"

#define SHOW_FPS 0
#define LAYER_NUM_MAX 5 // 界面数量

struct _gui_obj_t;

typedef void (*gui_obj_draw_cb_t)(struct _gui_obj_t *obj, gui_area_t *area); // 控件绘制回调

typedef struct _gui_obj_t
{
    struct _gui_obj_t *per;
    struct _gui_obj_t *next;

    uint8_t hide;
    gui_area_t area;

    gui_obj_draw_cb_t draw_cb;

    void *attribute;
} gui_obj_t;

typedef struct _gui_screen_t
{
    gui_area_t area;

    gui_obj_t *layers[LAYER_NUM_MAX];
    uint8_t active_layer_index;

    gui_obj_t *sys_layer;
} gui_screen_t;

void gui_init();

void gui_screen_layer_add_obj(gui_obj_t *obj, uint8_t active_layer_index);
void gui_screen_sys_layer_add_obj(gui_obj_t *obj);
void gui_screen_set_active_layer(uint8_t layer_index);

void gui_set_oled_protect_mode(uint8_t en);

uint8_t *gui_get_vdb();

void gui_obj_set_localtion(gui_obj_t *obj, int16_t x, int16_t y);
void gui_obj_set_size(gui_obj_t *obj, int16_t width, int16_t height);
void gui_obj_set_x(gui_obj_t *obj, int16_t x);
void gui_obj_set_y(gui_obj_t *obj, int16_t y);

inline void gui_obj_set_hide(gui_obj_t *obj, uint8_t en)
{
    obj->hide = en;
}

void gui_clear_all_pixel(void);
void gui_fill_all_pixel(void);
void gui_draw_pixel(int16_t x, int16_t y, uint8_t color);
void gui_draw_pixel_in_area(gui_area_t *area, int16_t x, int16_t y, uint8_t color);
void gui_draw_char(gui_area_t *area, int16_t x, int16_t y, gui_font_t *font, char c);
void gui_draw_string(gui_area_t *area, int16_t x, int16_t y, gui_font_t *font, char *str, uint16_t size);
void gui_draw_bitmap(gui_area_t *area, int16_t x, int16_t y, uint8_t *bitmap, uint16_t width, uint16_t height);

void gui_draw_line(gui_area_t *area, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color);

#endif
