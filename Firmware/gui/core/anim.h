#ifndef __GUI_ANIM_H
#define __GUI_ANIM_H
#include "stm32f1xx_hal.h"
#include "core.h"

struct _gui_anim_t;

typedef void (*gui_anim_control_cb)(gui_obj_t *, int16_t);     // 对对象的控制函数
typedef int16_t (*gui_anim_path_cb)(struct _gui_anim_t *anim); // 控制路径
typedef void (*gui_anim_over_cb)(struct _gui_anim_t *anim);    // 动画结束回调

typedef struct _gui_anim_t
{
    struct _gui_anim_t *next;

    gui_obj_t *obj; // 动画控制的对象

    gui_anim_control_cb control_cb; // 对对象的控制函数
    gui_anim_path_cb path_cb;       // 控制路径
    gui_anim_over_cb over_cb;       // 动画结束回调

    uint8_t playing : 1;        // 指示当前动画正在动作
    uint8_t enable : 1;         // 动画使能
    uint8_t loop_mode : 1;      // 循环模式
    uint8_t paly_back_mode : 1; // 回放模式
    uint8_t paly_back_now : 1;  // 指示当前是否在回放模式的回放过程

    uint16_t start_tick;        // 动画开始的延迟
    uint16_t paly_back_tick;    // 回放延迟
    int16_t paly_back_now_tick; // 回放延迟定时器

    uint16_t tick;    // 动画需要执行的时间
    int16_t now_tick; // 动画已经执行的时间

    int16_t start; // 起始值
    int16_t end;   // 结束值
} gui_anim_t;

inline void gui_anim_set_control_obj(gui_anim_t *anim, gui_obj_t *obj, gui_anim_control_cb control_cb)
{
    anim->obj = obj;
    anim->control_cb = control_cb;
}

inline void gui_anim_set_path_cb(gui_anim_t *anim, gui_anim_path_cb path_cb)
{
    anim->path_cb = path_cb;
}

inline void gui_anim_set_over_cb(gui_anim_t *anim, gui_anim_over_cb over_cb)
{
    anim->over_cb = over_cb;
}

inline void gui_anim_set_start(gui_anim_t *anim, int16_t val)
{
    anim->start = val;
}

inline void gui_anim_set_end(gui_anim_t *anim, int16_t val)
{
    anim->end = val;
}

inline void gui_anim_set_time(gui_anim_t *anim, uint16_t val)
{
    anim->tick = val;
}

inline void gui_anim_set_loop_mode(gui_anim_t *anim, uint8_t en)
{
    anim->loop_mode = en;
}

inline void gui_anim_set_paly_back_mode(gui_anim_t *anim, int16_t delay, uint8_t en)
{
    anim->paly_back_mode = en;
    anim->paly_back_tick = delay;
}

void gui_anim_init(gui_anim_t *anim);
void gui_anim_add(gui_anim_t *anim);
void gui_anim_start(gui_anim_t *anim, int16_t delay);
void gui_anim_stop(gui_anim_t *anim);

int16_t gui_anim_path_linear(gui_anim_t *anim);
int16_t gui_anim_path_overshoot(gui_anim_t *a);
int16_t gui_anim_path_ease_in(gui_anim_t *a);
int16_t gui_anim_path_ease_out(gui_anim_t *a);
int16_t gui_anim_path_ease_in_out(gui_anim_t *a);
int16_t gui_anim_path_bounce(gui_anim_t *a);

void gui_anim_handler();

#endif