#include "cube.h"
#include "cube.h"
#include <rtthread.h>
#include "math.h"

float _cube[8][3] = {{-15, -15, -15}, {-15, 15, -15}, {15, 15, -15}, {15, -15, -15}, {-15, -15, 15}, {-15, 15, 15}, {15, 15, 15}, {15, -15, 15}}; //立方体各点坐标
int lineid[] = {1, 2, 2, 3, 3, 4, 4, 1, 5, 6, 6, 7, 7, 8, 8, 5, 8, 4, 7, 3, 6, 2, 5, 1};                                                          //记录点之间连接顺序

void rotate(float *obj, float x, float y, float z);
float *matconv(float *a, float b[3][3]);

void gui_cube_draw_cb(gui_obj_t *cube, gui_area_t *area);

void gui_cube_init(gui_obj_t *cube, gui_cube_attribute_t *attribute)
{
    cube->per = NULL;
    cube->next = NULL;

    cube->hide = 0;

    cube->area.x1 = 0;
    cube->area.x2 = 0;
    cube->area.y1 = 0;
    cube->area.y2 = 0;

    cube->draw_cb = gui_cube_draw_cb;

    cube->attribute = (void *)attribute;
}

void gui_cube_set_localtion(gui_obj_t *cube, int16_t x, int16_t y)
{
    int16_t width = cube->area.x2 - cube->area.x1;
    int16_t height = cube->area.y2 - cube->area.y1;

    cube->area.x1 = x;
    cube->area.x2 = x + width;

    cube->area.y1 = y;
    cube->area.y2 = y + height;
}

void gui_cube_set_size(gui_obj_t *cube, int16_t width, int16_t height)
{
    cube->area.x2 = cube->area.x1 + width - 1;
    cube->area.y2 = cube->area.y1 + height - 1;
}

void gui_cube_set_x(gui_obj_t *cube, int16_t x)
{
    int16_t width = cube->area.x2 - cube->area.x1;

    cube->area.x1 = x;
    cube->area.x2 = x + width;
}

void gui_cube_set_y(gui_obj_t *cube, int16_t y)
{
    int16_t height = cube->area.y2 - cube->area.y1;

    cube->area.y1 = y;
    cube->area.y2 = y + height;
}

int seed = 0;

void gui_cube_draw_cb(gui_obj_t *cube, gui_area_t *area)
{
    gui_area_t area_common;

    static float angle[3] = {-0.1, 0.3, 0.2};
    static uint32_t times = 0;

    times++;
    if (times % 150 == 0)
    {
        for (uint8_t i = 0; i < 3; i++)
        {
            seed = rt_tick_get();
            angle[i] = ((seed * 22695477 + 1) % 10) / 100.0 - 0.3;
            if (angle[i] > 0)
            {
                angle[i] += 0.1;
            }
            else
            {
                angle[i] -= 0.1;
            }
        }
    }

    if (times % 5 == 0)
    {
        for (int i = 0; i < 8; i++)
        {
            rotate(_cube[i], angle[0], angle[1], angle[2]); //旋转每个点
        }
    }

    if (gui_area_find_common(&area_common, area, &cube->area))
    {
        for (int i = 0; i < 24; i += 2)
        {
            // 绘制立方体
            gui_draw_line(&area_common,
                          64 + _cube[lineid[i] - 1][0],
                          32 + _cube[lineid[i] - 1][1],
                          64 + _cube[lineid[i + 1] - 1][0],
                          32 + _cube[lineid[i + 1] - 1][1],
                          1);
        }
    }
}

float *matconv(float *a, float b[3][3])
{
    //计算矩阵乘法
    float res[3];

    for (int i = 0; i < 3; i++)
        res[i] = b[i][0] * a[0] + b[i][1] * a[1] + b[i][2] * a[2];

    for (int i = 0; i < 3; i++)
        a[i] = res[i];

    return a;
}

void rotate(float *obj, float x, float y, float z)
{
    // 旋转该向量
    x /= 3.141592653589793;
    y /= 3.141592653589793;
    z /= 3.141592653589793;

    float rz[3][3] = {{cos(z), -sin(z), 0}, {sin(z), cos(z), 0}, {0, 0, 1}};
    float ry[3][3] = {{1, 0, 0}, {0, cos(y), -sin(y)}, {0, sin(y), cos(y)}};
    float rx[3][3] = {{cos(x), 0, sin(x)}, {0, 1, 0}, {-sin(x), 0, cos(x)}};

    matconv(matconv(matconv(obj, rz), ry), rx);
}
