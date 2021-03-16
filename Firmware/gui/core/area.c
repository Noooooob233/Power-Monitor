#include "area.h"

uint8_t gui_area_find_common(gui_area_t *res, gui_area_t *area1, gui_area_t *area2)
{
    res->x1 = area1->x1 < area2->x1 ? area2->x1 : area1->x1;
    res->x2 = area1->x2 > area2->x2 ? area2->x2 : area1->x2;
    res->y1 = area1->y1 < area2->y1 ? area2->y1 : area1->y1;
    res->y2 = area1->y2 > area2->y2 ? area2->y2 : area1->y2;

    if ((res->x1 > res->x2) || (res->y1 > res->y2))
    {
        return 0;
    }

    return 1;
}

void gui_area_join(gui_area_t *res, gui_area_t *area1, gui_area_t *area2)
{
    res->x1 = area1->x1 > area2->x1 ? area2->x1 : area1->x1;
    res->x2 = area1->x2 < area2->x2 ? area2->x2 : area1->x2;
    res->y1 = area1->y1 > area2->y1 ? area2->y1 : area1->y1;
    res->y2 = area1->y2 < area2->y2 ? area2->y2 : area1->y2;
}
