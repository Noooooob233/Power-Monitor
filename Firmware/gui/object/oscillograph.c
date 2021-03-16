#include "oscillograph.h"

void gui_osc_draw_cb(gui_obj_t *label, gui_area_t *area);

void gui_osc_init(gui_obj_t *osc, gui_osc_attribute_t *attribute)
{
    osc->next = NULL;
    osc->per = NULL;

    osc->hide = 0;

    osc->area.x1 = 0;
    osc->area.x2 = 0;
    osc->area.y1 = 0;
    osc->area.y2 = 0;

    osc->draw_cb = gui_osc_draw_cb;

    osc->attribute = attribute;

    attribute->ymin = 0;
    attribute->ymax = 100;
    attribute->div = 2;
    attribute->points = NULL;
    attribute->point_start_num = 0;
    attribute->points_num = 0;
}

void gui_osc_set_localtion(gui_obj_t *osc, int16_t x, int16_t y)
{
    int16_t width = osc->area.x2 - osc->area.x1;
    int16_t height = osc->area.y2 - osc->area.y1;

    osc->area.x1 = x;
    osc->area.x2 = x + width;

    osc->area.y1 = y;
    osc->area.y2 = y + height;
}

void gui_osc_set_size(gui_obj_t *osc, int16_t width, int16_t height)
{
    osc->area.x2 = osc->area.x1 + width - 1;
    osc->area.y2 = osc->area.y1 + height - 1;
}

void gui_osc_add_points(gui_obj_t *osc, int16_t *points, uint16_t size)
{
    gui_osc_attribute_t *att = osc->attribute;

    att->points = points;
    att->points_num = size;
}

void gui_osc_points_init(gui_obj_t *osc)
{
    gui_osc_attribute_t *att = osc->attribute;

    for (int16_t i = 0; i < att->points_num; i++)
    {
        att->points[i] = osc->area.y2;
    }
}

void gui_osc_set_div(gui_obj_t *osc, uint8_t div)
{
    gui_osc_attribute_t *att = osc->attribute;

    att->div = div;
}

void gui_osc_add_data(gui_obj_t *osc, uint8_t data)
{
    gui_osc_attribute_t *att = osc->attribute;
    int16_t point = 0;

    if (data >= att->ymax)
    {
        point = osc->area.y1;
    }
    else if (data <= att->ymin)
    {
        point = osc->area.y2;
    }
    else
    {
        point = osc->area.y1 + (100 - data) * (osc->area.y2 - osc->area.y1 + 1) / 100.0;
    }

    att->points[(att->point_start_num + att->points_num) % att->points_num] = point; //将数据填充入最后一个点

    att->point_start_num++;
    att->point_start_num %= att->points_num;
}

void gui_osc_draw_cb(gui_obj_t *osc, gui_area_t *area)
{
    gui_osc_attribute_t *att = osc->attribute;
    int16_t x = osc->area.x2 - att->div * (att->points_num - 1);
    gui_area_t area_common;

    if (gui_area_find_common(&area_common, area, &osc->area))
    {
        for (uint16_t i = att->point_start_num; i < att->points_num + att->point_start_num - 1; i++)
        {
            gui_draw_line(&area_common,
                          x, att->points[i % att->points_num],
                          x + att->div, att->points[(i + 1) % att->points_num],
                          1);
            x += att->div;
        }

        // 绘制边框
        gui_draw_line(&area_common, osc->area.x1, osc->area.y1, osc->area.x1 + 4, osc->area.y1, 1);
        gui_draw_line(&area_common, osc->area.x1, osc->area.y1, osc->area.x1, osc->area.y1 + 4, 1);

        gui_draw_line(&area_common, osc->area.x2 - 4, osc->area.y1, osc->area.x2, osc->area.y1, 1);
        gui_draw_line(&area_common, osc->area.x2, osc->area.y1, osc->area.x2, osc->area.y1 + 4, 1);

        gui_draw_line(&area_common, osc->area.x1, osc->area.y2, osc->area.x1 + 4, osc->area.y2, 1);
        gui_draw_line(&area_common, osc->area.x1, osc->area.y2 - 4, osc->area.x1, osc->area.y2, 1);

        gui_draw_line(&area_common, osc->area.x2 - 4, osc->area.y2, osc->area.x2, osc->area.y2, 1);
        gui_draw_line(&area_common, osc->area.x2, osc->area.y2 - 4, osc->area.x2, osc->area.y2, 1);
    }
}
