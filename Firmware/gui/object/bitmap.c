#include "bitmap.h"

void gui_bitmap_draw_cb(gui_obj_t *bitmap, gui_area_t *area);

void gui_bitmap_init(gui_obj_t *bitmap, gui_bitmap_attribute_t *attribute)
{
    bitmap->per = NULL;
    bitmap->next = NULL;

    bitmap->hide = 0;

    bitmap->area.x1 = 0;
    bitmap->area.x2 = 0;
    bitmap->area.y1 = 0;
    bitmap->area.y2 = 0;

    bitmap->draw_cb = gui_bitmap_draw_cb;

    bitmap->attribute = (void *)attribute;

    attribute->bitmap = NULL;
}

void gui_bitmap_set_localtion(gui_obj_t *bitmap, int16_t x, int16_t y)
{
    int16_t width = bitmap->area.x2 - bitmap->area.x1;
    int16_t height = bitmap->area.y2 - bitmap->area.y1;

    bitmap->area.x1 = x;
    bitmap->area.x2 = x + width;

    bitmap->area.y1 = y;
    bitmap->area.y2 = y + height;
}

void gui_bitmap_set_size(gui_obj_t *bitmap, int16_t width, int16_t height)
{
    bitmap->area.x2 = bitmap->area.x1 + width - 1;
    bitmap->area.y2 = bitmap->area.y1 + height - 1;
}

void gui_bitmap_set_x(gui_obj_t *bitmap, int16_t x)
{
    int16_t width = bitmap->area.x2 - bitmap->area.x1;

    bitmap->area.x1 = x;
    bitmap->area.x2 = x + width;
}

void gui_bitmap_set_y(gui_obj_t *bitmap, int16_t y)
{
    int16_t height = bitmap->area.y2 - bitmap->area.y1;

    bitmap->area.y1 = y;
    bitmap->area.y2 = y + height;
}

void gui_bitmap_set_bitmap(gui_obj_t *bitmap, uint8_t *bmp, uint16_t width, uint16_t height)
{
    ((gui_bitmap_attribute_t *)bitmap->attribute)->bitmap = bmp;
    ((gui_bitmap_attribute_t *)bitmap->attribute)->width = width;
    ((gui_bitmap_attribute_t *)bitmap->attribute)->height = height;
}

void gui_bitmap_draw_cb(gui_obj_t *bitmap, gui_area_t *area)
{
    gui_bitmap_attribute_t *attribute = bitmap->attribute;
    gui_area_t area_common;

    if (gui_area_find_common(&area_common, area, &bitmap->area))
    {
        gui_draw_bitmap(&area_common,
                        bitmap->area.x1 + attribute->x_offset,
                        bitmap->area.y1 + attribute->y_offset,
                        attribute->bitmap,
                        attribute->width,
                        attribute->height);
    }
}