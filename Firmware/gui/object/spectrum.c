#include "spectrum.h"

void gui_spectrum_draw_cb(gui_obj_t *spec, gui_area_t *area);

void gui_spectrum_init(gui_obj_t *spec, gui_spectrum_attribute_t *attribute)
{
    spec->per = NULL;
    spec->next = NULL;

    spec->hide = 0;

    spec->area.x1 = 0;
    spec->area.x2 = 0;
    spec->area.y1 = 0;
    spec->area.y2 = 0;

    spec->draw_cb = gui_spectrum_draw_cb;

    spec->attribute = (void *)attribute;

    attribute->fall_time = 4;
    attribute->max_stay_time = 7;
    attribute->width = 2;
    attribute->interval = 1;
    attribute->style = 1;
}

void gui_spectrum_set_localtion(gui_obj_t *spec, int16_t x, int16_t y)
{
    int16_t width = spec->area.x2 - spec->area.x1;
    int16_t height = spec->area.y2 - spec->area.y1;

    spec->area.x1 = x;
    spec->area.x2 = x + width;

    spec->area.y1 = y;
    spec->area.y2 = y + height;
}

void gui_spectrum_set_size(gui_obj_t *spec, int16_t width, int16_t height)
{
    spec->area.x2 = spec->area.x1 + width - 1;
    spec->area.y2 = spec->area.y1 + height - 1;
}

void gui_spectrum_data_update(gui_obj_t *spec, uint8_t *data)
{
    gui_spectrum_attribute_t *att = spec->attribute;

    for (uint8_t i = 0; i < 128; i++)
    {
        att->spectrum_data[i] = data[i] >> 2;
    }
}

void gui_spectrum_data_reset(gui_obj_t *spec)
{
    gui_spectrum_attribute_t *att = spec->attribute;

    rt_memset(att->spectrum_data, 0x00, 128);
}

void gui_spectrum_draw_cb(gui_obj_t *spec, gui_area_t *area)
{
    gui_spectrum_attribute_t *att = spec->attribute;

    uint8_t i = 0;
    uint8_t j = 0;

    static uint8_t counter = 0;
    counter++;

    for (i = 0; i < 128; i++) // 计算下落点和幅值高度
    {
        if (att->spectrum_data[i] > att->spectrum_data_draw[i])
        {
            att->spectrum_data_draw[i]++;
        }
        else if (att->spectrum_data[i] < att->spectrum_data_draw[i])
        {
            att->spectrum_data_draw[i]--;
        }

        if (att->spectrum_data_draw[i] >= att->spectrum_data_max[i]) //比上一次数据大
        {
            att->spectrum_data_max[i] = att->spectrum_data_draw[i];
            att->spectrum_data_max_stay_timer[i] = att->max_stay_time;
        }
        else
        {
            if (counter % att->fall_time == 0)
            {
                att->spectrum_data_max_stay_timer[i]--;
                if (att->spectrum_data_max_stay_timer[i] < 0)
                {
                    att->spectrum_data_max_stay_timer[i] = 0;
                    att->spectrum_data_max[i]--;
                }
            }
        }
    }

    for (i = 0, j = 0; i <= 128; i += (att->width + att->interval)) // 画频谱
    {
        for (uint8_t k = 0; k < att->width; k++)
        {
            gui_draw_line(area, i + k, 63 - att->spectrum_data_draw[j], i + k, 63, 1);
        }
        j++;
    }

    if (att->style) // 点阵效果
    {
        for (int8_t k = (63 - att->width); k >= 0; k -= (att->width + 1))
        {
            gui_draw_line(area, 0, k, 127, k, 0);
        }
    }

    for (i = 0, j = 0; i <= 128; i += (att->width + att->interval)) // 下落点
    {
        for (uint8_t k = 0; k < att->width; k++)
        {
            gui_draw_pixel_in_area(area, i + k, 63 - att->spectrum_data_max[j], 1);
        }
        j++;
    }
}