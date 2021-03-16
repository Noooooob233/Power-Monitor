#include "font.h"

const uint8_t *gui_get_font_bitmap(gui_font_t *font, char c)
{
    if (font->font)
    {
        return font->font + font->offset * (c - ' ');
    }
    else
    {
        return NULL;
    }
}