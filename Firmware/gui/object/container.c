#include "container.h"

void gui_container_draw_cb(gui_obj_t *con, gui_area_t *area);

void gui_container_init(gui_obj_t *con, gui_container_attribute_t *attribute)
{
    con->per = NULL;
    con->next = NULL;

    con->hide = 0;

    con->area.x1 = 0;
    con->area.x2 = 0;
    con->area.y1 = 0;
    con->area.y2 = 0;

    con->draw_cb = gui_container_draw_cb;

    con->attribute = (void *)attribute;

    attribute->child = NULL;
}

void gui_container_set_localtion(gui_obj_t *con, int16_t x, int16_t y)
{
    int16_t width = con->area.x2 - con->area.x1;
    int16_t height = con->area.y2 - con->area.y1;

    int16_t x_offset = x - con->area.x1;
    int16_t y_offset = y - con->area.y1;

    con->area.x1 = x;
    con->area.x2 = x + width;

    con->area.y1 = y;
    con->area.y2 = y + height;

    gui_container_attribute_t *att = con->attribute;
    gui_obj_t *p = att->child;

    while (p != NULL)
    {
        gui_obj_set_x(p, p->area.x1 + x_offset);
        gui_obj_set_y(p, p->area.y1 + y_offset);

        p = p->next;
    }
}

void gui_container_set_x(gui_obj_t *con, int16_t x)
{
    int16_t width = con->area.x2 - con->area.x1;

    int16_t x_offset = x - con->area.x1;

    con->area.x1 = x;
    con->area.x2 = x + width;

    gui_container_attribute_t *att = con->attribute;
    gui_obj_t *p = att->child;

    while (p != NULL)
    {
        gui_obj_set_x(p, p->area.x1 + x_offset);

        p = p->next;
    }
}

void gui_container_set_y(gui_obj_t *con, int16_t y)
{
    int16_t height = con->area.y2 - con->area.y1;

    int16_t y_offset = y - con->area.y1;

    con->area.y1 = y;
    con->area.y2 = y + height;

    gui_container_attribute_t *att = con->attribute;
    gui_obj_t *p = att->child;

    while (p != NULL)
    {
        gui_obj_set_y(p, p->area.y1 + y_offset);

        p = p->next;
    }
}

void gui_container_set_size(gui_obj_t *con, int16_t width, int16_t height)
{
    con->area.x2 = con->area.x1 + width - 1;
    con->area.y2 = con->area.y1 + height - 1;
}

// void gui_container_set_obj_x(gui_obj_t *con, int16_t x)
// {
//     gui_container_attribute_t *att = con->attribute;
//     gui_obj_t *p = att->child;
//     // gui_area_t

//     while (p != NULL)
//     {
//         gui_obj_set_x(p, x);

//         p = p->next;
//     }
// }

// void gui_container_set_obj_y()
// {
//     gui_container_attribute_t *att = con->attribute;
//     gui_obj_t *p = att->child;
//     // gui_area_t

//     while (p != NULL)
//     {
//         if (p->draw_cb && p->hide == 0)
//         {
//             p->draw_cb(p, &p->area);
//         }

//         p = p->next;
//     }
// }

// void gui_container_set_obj_localtion()
// {
//     gui_container_attribute_t *att = con->attribute;
//     gui_obj_t *p = att->child;
//     // gui_area_t

//     while (p != NULL)
//     {
//         if (p->draw_cb && p->hide == 0)
//         {
//             p->draw_cb(p, &p->area);
//         }

//         p = p->next;
//     }
// }

void gui_container_set_edge_style(gui_obj_t *con, uint8_t style)
{
    gui_container_attribute_t *att = con->attribute;
    att->edge_style = style;
}

void gui_container_add_obj(gui_obj_t *con, gui_obj_t *obj)
{
    gui_container_attribute_t *att = con->attribute;
    gui_obj_t *p = att->child;

    if (p == NULL)
    {
        att->child = obj;
        return;
    }

    while (p->next != NULL)
    {
        p = p->next;
    }

    p->next = obj;
}

void gui_container_draw_cb(gui_obj_t *con, gui_area_t *area)
{
    gui_container_attribute_t *att = con->attribute;
    gui_obj_t *p = att->child;
    gui_area_t area_common;

    if (gui_area_find_common(&area_common, &con->area, area))
    {
        while (p != NULL)
        {
            if (p->draw_cb && p->hide == 0)
            {
                p->draw_cb(p, &area_common);
            }

            p = p->next;
        }

        // 绘制边框
        switch (att->edge_style)
        {
        case 0:
            break;

        case 1:
            gui_draw_line(&area_common, con->area.x1, con->area.y1, con->area.x1 + 4, con->area.y1, 1);
            gui_draw_line(&area_common, con->area.x1, con->area.y1, con->area.x1, con->area.y1 + 4, 1);

            gui_draw_line(&area_common, con->area.x2 - 4, con->area.y1, con->area.x2, con->area.y1, 1);
            gui_draw_line(&area_common, con->area.x2, con->area.y1, con->area.x2, con->area.y1 + 4, 1);

            gui_draw_line(&area_common, con->area.x1, con->area.y2, con->area.x1 + 4, con->area.y2, 1);
            gui_draw_line(&area_common, con->area.x1, con->area.y2 - 4, con->area.x1, con->area.y2, 1);

            gui_draw_line(&area_common, con->area.x2 - 4, con->area.y2, con->area.x2, con->area.y2, 1);
            gui_draw_line(&area_common, con->area.x2, con->area.y2 - 4, con->area.x2, con->area.y2, 1);
            break;

        default:
            break;
        }
    }
}