#ifndef __GUI_FONT_H
#define __GUI_FONT_H
#include "stm32f1xx_hal.h"

typedef struct _gui_font_t
{
    const uint8_t *font; // 字符集

    uint8_t offset;   // 每个字符所占字节数
    uint8_t col_size; // 每个字符一列所占字节数

    uint8_t width;  // 字符宽度
    uint8_t height; // 字符高度
} gui_font_t;

const uint8_t *gui_get_font_bitmap(gui_font_t *font, char c);

#endif
