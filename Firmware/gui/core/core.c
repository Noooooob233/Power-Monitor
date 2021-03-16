#include <core.h>
#include "label.h"
#include "anim.h"
#include "stdlib.h"

gui_screen_t screen; // 屏幕

//uint8_t OLED_GRAM[128][8]; //128列
//   highbit  // ...//   highbit  //
//   . [0][0] // ...//   .[127][0]//
//   .        // ...//   .        //
//   lowbit   // ...//   lowbit   //
//                .               //
//                .               //   行8x8
//                .               //
//   highbit  // ...//   highbit  //
//   . [0][7] // ...//   .        //
//   .        // ...//   .        //
//   lowbit   // ...//   lowbit   //
//              列x128            //

uint8_t vdb[8][128];
//   highbit  // ...//   highbit  //
//   . [0][0] // ...//   .[0][127]//
//   .        // ...//   .        //
//   lowbit   // ...//   lowbit   //
//                .               //
//                .               //   8x8页
//                .               //
//   highbit  // ...//   highbit  //
//   . [7][0] // ...//   .        //
//   .        // ...//   .        //
//   lowbit   // ...//   lowbit   //
//              列x128            //

#if SHOW_FPS
gui_obj_t fps_monitoer;
gui_label_attribute_t fps_monitoer_att;
extern gui_font_t font1206;
#endif

uint8_t oled_protect_mode = 0;
uint16_t oled_off_timeout = 0;

void gui_draw_object(gui_obj_t *obj, gui_area_t *area);
void gui_screen_draw();

static rt_uint8_t gui_thread_stack[512];
static struct rt_thread gui_thread;
void gui_entry(void *parameter);

void gui_init()
{
    screen.area.x1 = 0;
    screen.area.x2 = 127;
    screen.area.y1 = 0;
    screen.area.y2 = 63;

    screen.active_layer_index = 0;
    screen.sys_layer = NULL;

    rt_thread_init(&gui_thread, //gui线程初始化
                   "gui",
                   gui_entry,
                   RT_NULL,
                   &gui_thread_stack[0],
                   sizeof(gui_thread_stack),
                   3,
                   20);
    rt_thread_startup(&gui_thread);
}

void gui_entry(void *parameter)
{
#if SHOW_FPS
    uint8_t fps = 0;
    rt_tick_t last_tick = 0;
    rt_tick_t used_tick = 0;
    char tmp[5];

    gui_label_init(&fps_monitoer, &fps_monitoer_att);
    gui_label_set_localtion(&fps_monitoer, 0, 0);
    gui_label_set_size(&fps_monitoer, 20, 10);
    gui_label_set_font(&fps_monitoer, &font1206);

    gui_screen_sys_layer_add_obj(&fps_monitoer);
#endif

    while (1)
    {
        gui_anim_handler();

        if (oled_protect_mode)
        {
            oled_off_timeout++;
        }

        if (oled_off_timeout >= 6050)
        {
            oled_off_timeout = 0;
        }
        else if (oled_off_timeout >= 6000) //oled关闭 一段时间  减少烧屏
        {
            gui_clear_all_pixel();
        }
        else
        {
#if SHOW_FPS
            used_tick = rt_tick_get() - last_tick;
            if (used_tick > 200) // 经过了1s
            {
                last_tick = rt_tick_get();
                rt_sprintf(tmp, "%d", fps * 1000 / used_tick);
                gui_label_set_text_by_string(&fps_monitoer, tmp);
                fps = 0;
            }
            fps++;
#endif

            gui_screen_draw(); // 绘制ui
        }

        rt_thread_mdelay(10);
    }
}

void gui_set_oled_protect_mode(uint8_t en)
{
    oled_protect_mode = en;
    oled_off_timeout = 0;
}

uint8_t *gui_get_vdb()
{
    return &vdb[0][0];
}

void gui_screen_layer_add_obj(gui_obj_t *obj, uint8_t layer_index)
{
    gui_obj_t *p = screen.layers[layer_index];

    if (p == NULL)
    {
        screen.layers[layer_index] = obj;
        return;
    }

    while (p->next != NULL)
    {
        p = p->next;
    }

    p->next = obj;
}

void gui_screen_sys_layer_add_obj(gui_obj_t *obj)
{
    gui_obj_t *p = screen.sys_layer;

    if (p == NULL)
    {
        screen.sys_layer = obj;
        return;
    }

    while (p->next != NULL)
    {
        p = p->next;
    }

    p->next = obj;
}

void gui_screen_set_active_layer(uint8_t layer_index)
{
    if (layer_index >= 0 && layer_index < LAYER_NUM_MAX)
    {
        screen.active_layer_index = layer_index;
    }
}

extern struct rt_mutex mutex_oled_lock;
void gui_screen_draw()
{
    rt_mutex_take(&mutex_oled_lock, RT_WAITING_FOREVER);
    gui_clear_all_pixel();

    gui_draw_object(screen.layers[screen.active_layer_index], &screen.area);

    gui_draw_object(screen.sys_layer, &screen.area);

    rt_mutex_release(&mutex_oled_lock);
}

void gui_draw_object(gui_obj_t *obj, gui_area_t *area)
{
    gui_obj_t *p = obj;
    gui_area_t area_common;

    while (p != NULL)
    {
        if (p->draw_cb && p->hide == 0)
        {
            if (gui_area_find_common(&area_common, area, &p->area))
            {
                p->draw_cb(p, &area_common);
            }
        }

        p = p->next;
    }
}

void gui_obj_set_localtion(gui_obj_t *obj, int16_t x, int16_t y)
{
    int16_t width = obj->area.x2 - obj->area.x1;
    int16_t height = obj->area.y2 - obj->area.y1;

    obj->area.x1 = x;
    obj->area.x2 = x + width;

    obj->area.y1 = y;
    obj->area.y2 = y + height;
}

void gui_obj_set_size(gui_obj_t *obj, int16_t width, int16_t height)
{
    obj->area.x2 = obj->area.x1 + width - 1;
    obj->area.y2 = obj->area.y1 + height - 1;
}

void gui_obj_set_x(gui_obj_t *obj, int16_t x)
{
    int16_t width = obj->area.x2 - obj->area.x1;

    obj->area.x1 = x;
    obj->area.x2 = x + width;
}

void gui_obj_set_y(gui_obj_t *obj, int16_t y)
{
    int16_t height = obj->area.y2 - obj->area.y1;

    obj->area.y1 = y;
    obj->area.y2 = y + height;
}

// 高位在前   列行式
// 确保使用时area在屏幕内  否则会溢出
void gui_draw_char(gui_area_t *area, int16_t x, int16_t y, gui_font_t *font, char c)
{
    if (x + font->width < area->x1 || area->x2 < x || y + font->height < area->y1 || area->y2 < y)
    {
        return;
    }

    const uint8_t *bitmap = gui_get_font_bitmap(font, c);

    if (bitmap == NULL)
    {
        return;
    }

    uint8_t i_start = (area->x1 < x) ? 0 : area->x1 - x;
    uint8_t i_end = (area->x2 < (x + font->width)) ? area->x2 - x + 1 : font->width;
    uint8_t j_start = (area->y1 < y) ? 0 : area->y1 - y;
    uint8_t j_end = (area->y2 < (y + font->height)) ? area->y2 - y + 1 : font->height;

    // uint16_t bit_mask_init = 0x80 >> (j_start % 8);
    uint16_t bit_mask_init = 0x80 >> (j_start & 0x07);
    uint16_t bit_mask = bit_mask_init;

    // uint16_t offset = j_start / 8;
    uint16_t offset = j_start >> 3; // 计算上部分不绘制的像素数量

    uint16_t bitmap_offset = font->col_size * i_start + offset; // 计算bitmap的初始偏移量
    bitmap += bitmap_offset;                                    // 移动至字符在区域内的初始字节位置

    // uint16_t left_px = (font->col_size << 3) - j_end;
    // offset = (left_px & 0x07) > 0 ? offset + (left_px >> 3): offset + left_px >> 3; // 计算每列绘制完毕后需增加的偏移量
    // // offset = (left_px % 0x08) > 0 ? offset + 1 + left_px / 0x08 : offset + left_px / 0x08; // 计算每列绘制完毕后需增加的偏移量
    // offset += j_start / 0x08;

    uint16_t px_offset = (font->col_size << 3) - j_end; // 计算下部分不绘制的像素数量
    offset += (px_offset >> 3) + 1;                     // 计算绘制完成一列后  bitmap需要的偏移量

    for (uint8_t i = i_start; i < i_end; i++)
    {
        bit_mask = bit_mask_init;

        for (uint8_t j = j_start; j < j_end; j++)
        {
            if (bit_mask & (*bitmap))
            {
                vdb[(y + j) / 8][x + i] |= (1 << ((y + j) % 8)); //置1
            }
            else
            {
                vdb[(y + j) / 8][x + i] &= ~(1 << ((y + j) % 8)); //置0
            }

            bit_mask >>= 1;
            if (bit_mask == 0 && j < j_end - 1) // 到达最底时bitmap不加
            {
                bit_mask = 0x80;
                bitmap++;
            }

            // // if (((j + 1) % 8) == 0) // 越过了一列的8个字符
            // if (((j + 1) & 0x07) == 0) // 越过了一列的8个字符
            // {
            //     bit_mask = 0x80;
            //     bitmap++;
            // }
            // else
            // {
            //     bit_mask >>= 1;
            // }
        }

        bitmap += offset;
    }
}

// 高位在前   行列式
// 确保使用时area在屏幕内
void gui_draw_bitmap(gui_area_t *area, int16_t x, int16_t y, uint8_t *bitmap, uint16_t width, uint16_t height)
{
    if (x + width < area->x1 || area->x2 < x || y + height < area->y1 || area->y2 < y)
    {
        return;
    }

    if (bitmap == NULL)
    {
        return;
    }

    uint16_t i_start = (area->x1 < x) ? 0 : area->x1 - x;
    uint16_t i_end = (area->x2 < (x + width)) ? area->x2 - x + 1 : width;
    uint16_t j_start = (area->y1 < y) ? 0 : area->y1 - y;
    uint16_t j_end = (area->y2 < (y + height)) ? area->y2 - y + 1 : height;

    uint16_t bit_mask_init = 0x80 >> (j_start & 0x07);
    uint16_t bit_mask = bit_mask_init;

    uint16_t offset = j_start >> 3; // 计算上部分不绘制的像素数量

    uint16_t bitmap_offset = i_start + offset * width; // 计算bitmap的初始偏移量
    bitmap += bitmap_offset;

    for (uint8_t j = j_start; j < j_end; j++)
    {
        for (uint8_t i = i_start; i < i_end; i++)
        {
            // if (bit_mask & (*bitmap)) // 高位在前
            if ((0x80 / bit_mask) & (*bitmap)) // 低位在前
            {
                vdb[(y + j) / 8][x + i] |= (1 << ((y + j) % 8)); //置1
            }
            else
            {
                vdb[(y + j) / 8][x + i] &= ~(1 << ((y + j) % 8)); //置0
            }

            if (i < i_end - 1) // 到达最右时bitmap不移动
            {
                bitmap++;
            }
        }

        bit_mask >>= 1;
        if (bit_mask == 0)
        {
            bit_mask = 0x80;
            bitmap += (width - i_end + i_start + 1);
        }
        else
        {
            bitmap -= (i_end - i_start - 1); // 返回行头
        }
    }
}

void gui_draw_bitmap1(gui_area_t *area, int16_t x, int16_t y, uint8_t *bitmap, uint16_t width, uint16_t height)
{
    if (x + width < area->x1 || area->x2 < x || y + height < area->y1 || area->y2 < y)
    {
        return;
    }

    if (bitmap == NULL)
    {
        return;
    }

    uint16_t i_start = (area->x1 < x) ? 0 : area->x1 - x;
    uint16_t i_end = (area->x2 < (x + width)) ? area->x2 - x + 1 : width;
    uint16_t j_start = (area->y1 < y) ? 0 : area->y1 - y;
    uint16_t j_end = (area->y2 < (y + height)) ? area->y2 - y + 1 : height;

    // uint16_t bit_mask_init = 0x80 >> (j_start % 8);
    uint16_t bit_mask_init = 0x80 >> (j_start & 0x07);
    uint16_t bit_mask = bit_mask_init;

    // uint16_t offset = j_start / 8;
    uint16_t offset = j_start >> 3; // 计算上部分不绘制的像素数量

    uint16_t bitmap_offset = (((height - 1) >> 3) + 1) * i_start + offset; // 计算bitmap的初始偏移量
    bitmap += bitmap_offset;                                               // 移动至字符在区域内的初始字节位置

    // uint16_t left_px = (font->col_size << 3) - j_end;
    // offset = (left_px & 0x07) > 0 ? offset + (left_px >> 3): offset + left_px >> 3; // 计算每列绘制完毕后需增加的偏移量
    // // offset = (left_px % 0x08) > 0 ? offset + 1 + left_px / 0x08 : offset + left_px / 0x08; // 计算每列绘制完毕后需增加的偏移量
    // offset += j_start / 0x08;

    uint16_t px_offset = ((((height - 1) >> 3) + 1) << 3) - j_end; // 计算下部分不绘制的像素数量
    offset += (px_offset >> 3) + 1;                                // 计算绘制完成一列后  bitmap需要的偏移量

    for (uint8_t i = i_start; i < i_end; i++)
    {
        bit_mask = bit_mask_init;

        for (uint8_t j = j_start; j < j_end; j++)
        {
            if (bit_mask & (*bitmap))
            {
                gui_draw_pixel(x + i, y + j, 1);
            }
            else
            {
                gui_draw_pixel(x + i, y + j, 0);
            }

            bit_mask >>= 1;
            if (bit_mask == 0 && j < j_end - 1) // 到达最底时bitmap不加
            {
                bit_mask = 0x80;
                bitmap++;
            }
        }

        bitmap += offset;
    }
}

void gui_draw_string(gui_area_t *area, int16_t x, int16_t y, gui_font_t *font, char *str, uint16_t size)
{
    uint16_t offset = 0;

    for (uint16_t i = 0; i < size; i++)
    {
        gui_draw_char(area, x + offset, y, font, *(str + i));
        offset += font->width;
    }
}

void gui_draw_pixel(int16_t x, int16_t y, uint8_t color)
{
    // 若在屏幕外
    if (x < 0 || x > 127 || y < 0 || y > 63)
    {
        return;
    }

    switch (color)
    {
    case 0:
        vdb[y / 8][x] &= ~(1 << (y % 8)); //置0
        break;

    case 1:
        vdb[y / 8][x] |= (1 << (y % 8)); //置1
        break;

    case 2:
        vdb[y / 8][x] ^= (1 << (y % 8)); //取反
        break;

    default:
        break;
    }
}

void gui_draw_pixel_in_area(gui_area_t *area, int16_t x, int16_t y, uint8_t color)
{
    // 若在屏幕外
    if (x < 0 || x > 127 || y < 0 || y > 63)
    {
        return;
    }

    // 若在区域外
    if (x < area->x1 || x > area->x2 || y < area->y1 || y > area->y2)
    {
        return;
    }

    switch (color)
    {
    case 0:
        vdb[y / 8][x] &= ~(1 << (y % 8)); //置0
        break;

    case 1:
        vdb[y / 8][x] |= (1 << (y % 8)); //置1
        break;

    case 2:
        vdb[y / 8][x] ^= (1 << (y % 8)); //取反
        break;

    default:
        break;
    }
}

void draw_hline(gui_area_t *area, int16_t x0, int16_t x1, int16_t y, uint8_t color);
void draw_vline(gui_area_t *area, int16_t y0, int16_t y1, int16_t x, uint8_t color);
void draw_line(gui_area_t *area, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color);

inline void swap(int16_t *a, int16_t *b)
{
    int16_t temp = *a;
    *a = *b;
    *b = temp;
}

void gui_draw_line(gui_area_t *area, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color)
{
    if (x0 == x1)
    {
        //垂直线
        draw_vline(area, y0, y1, x0, color);
    }
    else if (y0 == y1)
    {
        //水平线
        draw_hline(area, x0, x1, y0, color);
    }
    else
    {
        draw_line(area, x0, y0, x1, y1, color);
    }
}

void draw_hline(gui_area_t *area, int16_t x0, int16_t x1, int16_t y, uint8_t color)
{
    int8_t step = 1;
    int16_t len = x1 - x0 + 1;

    if (y < area->y1 || y > area->y2)
    {
        return;
    }

    if (x0 > x1)
    {
        len = x0 - x1 + 1;
        step = -1;
    }

    while (len--)
    {
        if (x0 >= area->x1 && x0 <= area->x2)
        {
            if (color)
            {
                vdb[y / 8][x0] |= (1 << (y % 8)); //置1
            }
            else
            {
                vdb[y / 8][x0] &= ~(1 << (y % 8)); //置0
            }
        }
        x0 += step;
    }
}

void draw_vline(gui_area_t *area, int16_t y0, int16_t y1, int16_t x, uint8_t color)
{
    int8_t step = 1;
    int16_t len = y1 - y0 + 1;

    if (x < area->x1 || x > area->x2)
    {
        return;
    }

    if (y0 > y1)
    {
        len = y0 - y1 + 1;
        step = -1;
    }

    while (len--)
    {
        if (y0 >= area->y1 && y0 <= area->y2)
        {
            if (color)
            {
                vdb[y0 / 8][x] |= (1 << (y0 % 8)); //置1
            }
            else
            {
                vdb[y0 / 8][x] &= ~(1 << (y0 % 8)); //置0
            }
        }
        y0 += step;
    }
}

void draw_line(gui_area_t *area, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color)
{
    int8_t steep = abs(y1 - y0) > abs(x1 - x0);

    if (steep)
    {
        swap(&x0, &y0);
        swap(&x1, &y1);
    }

    if (x0 > x1)
    {
        swap(&x0, &x1);
        swap(&y0, &y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int16_t err = dx >> 1;
    int8_t ystep;

    if (y0 < y1)
    {
        ystep = 1;
    }
    else
    {
        ystep = -1;
    }

    for (; x0 <= x1; x0++)
    {

        if (steep)
        {
            if (y0 >= area->x1 && y0 <= area->x2 && x0 >= area->y1 && x0 <= area->y2)
            {
                if (color)
                {
                    vdb[x0 / 8][y0] |= (1 << (x0 % 8)); //置1
                }
                else
                {
                    vdb[x0 / 8][y0] &= ~(1 << (x0 % 8)); //置0
                }
            }
        }
        else
        {
            if (x0 >= area->x1 && x0 <= area->x2 && y0 >= area->y1 && y0 <= area->y2)
            {
                if (color)
                {
                    vdb[y0 / 8][x0] |= (1 << (y0 % 8)); //置1
                }
                else
                {
                    vdb[y0 / 8][x0] &= ~(1 << (y0 % 8)); //置0
                }
            }
        }

        err -= dy;

        if (err < 0)
        {
            y0 += ystep;
            err += dx;
        }
    }
}

//清屏
void gui_clear_all_pixel(void)
{
    rt_memset(*vdb, 0x00, 1024);
}

void gui_fill_all_pixel(void)
{
    rt_memset(*vdb, 0xFF, 1024);
}

// void OLED_DrawCircle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color)
// {
//     int16_t f = 1 - r;
//     int16_t ddF_x = 1;
//     int16_t ddF_y = -(r << 1);
//     int16_t x = 0;
//     int16_t y = r;

//     OLED_DrawPixel(x0, y0 + r, color);
//     OLED_DrawPixel(x0, y0 - r, color);
//     OLED_DrawPixel(x0 + r, y0, color);
//     OLED_DrawPixel(x0 - r, y0, color);

//     while (x < y)
//     {
//         if (f >= 0)
//         {
//             y--;
//             ddF_y += 2;
//             f += ddF_y;
//         }
//         x++;
//         ddF_x += 2;
//         f += ddF_x;

//         OLED_DrawPixel(x0 + x, y0 + y, color);
//         OLED_DrawPixel(x0 - x, y0 + y, color);
//         OLED_DrawPixel(x0 + x, y0 - y, color);
//         OLED_DrawPixel(x0 - x, y0 - y, color);
//         OLED_DrawPixel(x0 + y, y0 + x, color);
//         OLED_DrawPixel(x0 - y, y0 + x, color);
//         OLED_DrawPixel(x0 + y, y0 - x, color);
//         OLED_DrawPixel(x0 - y, y0 - x, color);
//     }
// }

// void Adafruit_GFX::drawCircleHelper( int16_t x0, int16_t y0,
//         int16_t r, uint8_t cornername, uint16_t color) {
//     int16_t f     = 1 - r;
//     int16_t ddF_x = 1;
//     int16_t ddF_y = -2 * r;
//     int16_t x     = 0;
//     int16_t y     = r;

//     while (x<y) {
//         if (f >= 0) {
//             y--;
//             ddF_y += 2;
//             f     += ddF_y;
//         }
//         x++;
//         ddF_x += 2;
//         f     += ddF_x;
//         if (cornername & 0x4) {
//             writePixel(x0 + x, y0 + y, color);
//             writePixel(x0 + y, y0 + x, color);
//         }
//         if (cornername & 0x2) {
//             writePixel(x0 + x, y0 - y, color);
//             writePixel(x0 + y, y0 - x, color);
//         }
//         if (cornername & 0x8) {
//             writePixel(x0 - y, y0 + x, color);
//             writePixel(x0 - x, y0 + y, color);
//         }
//         if (cornername & 0x1) {
//             writePixel(x0 - y, y0 - x, color);
//             writePixel(x0 - x, y0 - y, color);
//         }
//     }
// }

// void Adafruit_GFX::fillCircle(int16_t x0, int16_t y0, int16_t r,
//         uint16_t color) {
//     startWrite();
//     writeFastVLine(x0, y0-r, 2*r+1, color);
//     fillCircleHelper(x0, y0, r, 3, 0, color);
//     endWrite();
// }
