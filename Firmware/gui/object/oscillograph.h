#ifndef __GUI_OCSILLOGRAPH_H
#define __GUI_OCSILLOGRAPH_H
#include "stm32f1xx_hal.h"
#include "core.h"

typedef struct _gui_osc_attribute_t
{
    int16_t ymin;
    int16_t ymax;

    uint8_t div; // 刻度

    int16_t *points;          // 每个点的数值
    uint16_t points_num;      // 点的数量
    uint16_t point_start_num; // 指示当前第一个点的位置（最旧的点）
} gui_osc_attribute_t;

void gui_osc_init(gui_obj_t *osc, gui_osc_attribute_t *attribute);
void gui_osc_set_localtion(gui_obj_t *osc, int16_t x, int16_t y);
void gui_osc_set_size(gui_obj_t *osc, int16_t width, int16_t height);
void gui_osc_add_points(gui_obj_t *osc, int16_t *points, uint16_t size);
void gui_osc_points_init(gui_obj_t *osc);
void gui_osc_set_div(gui_obj_t *osc, uint8_t div);
void gui_osc_add_data(gui_obj_t *osc, uint8_t data);


#endif