#include "anim.h"

gui_anim_t *anim_head;

void gui_anim_init(gui_anim_t *anim)
{
    anim->next = NULL;
    anim->obj = NULL;

    anim->enable = 0;

    anim->playing = 0;

    anim->control_cb = NULL;
    anim->path_cb = gui_anim_path_linear;
    anim->over_cb = NULL;

    anim->loop_mode = 0;
    anim->paly_back_mode = 0;
    anim->paly_back_tick = 0;
    anim->paly_back_now = 0;
    anim->paly_back_now_tick = 0;

    anim->start = 0;
    anim->end = 100;

    anim->tick = 500;
    anim->now_tick = 0;
}

void gui_anim_add(gui_anim_t *anim)
{
    gui_anim_t *p = anim_head;

    if (p == NULL)
    {
        anim_head = anim;
        return;
    }

    while (p->next != NULL)
    {
        p = p->next;
    }

    p->next = anim;
}

void gui_anim_start(gui_anim_t *anim, int16_t delay)
{
    if (!anim->enable)
    {
        anim->enable = 1;
        anim->playing = 1;
        anim->now_tick = -delay;
        anim->start_tick = delay;
        anim->paly_back_now_tick = 0;
        anim->paly_back_now = 0;
    }
}

void gui_anim_stop(gui_anim_t *anim)
{
    anim->enable = 0;
    anim->playing = 0;
    anim->now_tick = 0;
    anim->paly_back_now_tick = 0;
    anim->paly_back_now = 0;
}

int32_t lv_bezier3(uint32_t t, int32_t u0, int32_t u1, int32_t u2, int32_t u3)
{
    uint32_t t_rem = 1024 - t;
    uint32_t t_rem2 = (t_rem * t_rem) >> 10;
    uint32_t t_rem3 = (t_rem2 * t_rem) >> 10;
    uint32_t t2 = (t * t) >> 10;
    uint32_t t3 = (t2 * t) >> 10;

    uint32_t v1 = ((uint32_t)t_rem3 * u0) >> 10;
    uint32_t v2 = ((uint32_t)3 * t_rem2 * t * u1) >> 20;
    uint32_t v3 = ((uint32_t)3 * t_rem * t2 * u2) >> 20;
    uint32_t v4 = ((uint32_t)t3 * u3) >> 10;

    return v1 + v2 + v3 + v4;
}

int16_t gui_anim_path_linear(gui_anim_t *anim)
{
    uint32_t step;
    if (anim->tick <= anim->now_tick)
    {
        step = 1024;
    }
    else
    {
        step = ((int32_t)anim->now_tick << 10) / anim->tick;
    }

    int32_t new_value;
    new_value = (int32_t)step * (anim->end - anim->start);
    new_value = new_value >> 10;
    new_value += anim->start;

    return (int16_t)new_value;
}

int16_t gui_anim_path_overshoot(gui_anim_t *a)
{
    /*Calculate the current step*/
    uint32_t t;
    if (a->tick == a->now_tick)
        t = 1024;
    else
        t = (uint32_t)((uint32_t)a->now_tick * 1024) / a->tick;

    int32_t step = lv_bezier3(t, 0, 600, 1300, 1024);

    int32_t new_value;
    new_value = (int32_t)step * (a->end - a->start);
    new_value = new_value >> 10;
    new_value += a->start;

    return (int16_t)new_value;
}

int16_t gui_anim_path_ease_in(gui_anim_t *a)
{
    /*Calculate the current step*/
    uint32_t t;
    if (a->tick == a->now_tick)
        t = 1024;
    else
        t = (uint32_t)((uint32_t)a->now_tick * 1024) / a->tick;

    int32_t step = lv_bezier3(t, 0, 1, 1, 1024);

    int32_t new_value;
    new_value = (int32_t)step * (a->end - a->start);
    new_value = new_value >> 10;
    new_value += a->start;

    return (int16_t)new_value;
}

int16_t gui_anim_path_ease_out(gui_anim_t *a)
{
    /*Calculate the current step*/
    uint32_t t;
    if (a->tick == a->now_tick)
        t = 1024;
    else
        t = (uint32_t)((uint32_t)a->now_tick * 1024) / a->tick;

    int32_t step = lv_bezier3(t, 0, 1023, 1023, 1024);

    int32_t new_value;
    new_value = (int32_t)step * (a->end - a->start);
    new_value = new_value >> 10;
    new_value += a->start;

    return (int16_t)new_value;
}

int16_t gui_anim_path_ease_in_out(gui_anim_t *a)
{
    /*Calculate the current step*/
    uint32_t t;
    if (a->tick == a->now_tick)
        t = 1024;
    else
        t = (uint32_t)((uint32_t)a->now_tick * 1024) / a->tick;

    int32_t step = lv_bezier3(t, 0, 100, 924, 1024);

    int32_t new_value;
    new_value = (int32_t)step * (a->end - a->start);
    new_value = new_value >> 10;
    new_value += a->start;

    return (int16_t)new_value;
}

int16_t gui_anim_path_bounce(gui_anim_t *a)
{
    /*Calculate the current step*/
    uint32_t t;
    if (a->tick == a->now_tick)
        t = 1024;
    else
        t = (uint32_t)((uint32_t)a->now_tick * 1024) / a->tick;

    int32_t diff = (a->end - a->start);

    /*3 bounces has 5 parts: 3 down and 2 up. One part is t / 5 long*/

    if (t < 408)
    {
        /*Go down*/
        t = (t * 2500) >> 10; /*[0..1024] range*/
    }
    else if (t >= 408 && t < 614)
    {
        /*First bounce back*/
        t -= 408;
        t = t * 5; /*to [0..1024] range*/
        t = 1024 - t;
        diff = diff / 6;
    }
    else if (t >= 614 && t < 819)
    {
        /*Fall back*/
        t -= 614;
        t = t * 5; /*to [0..1024] range*/
        diff = diff / 6;
    }
    else if (t >= 819 && t < 921)
    {
        /*Second bounce back*/
        t -= 819;
        t = t * 10; /*to [0..1024] range*/
        t = 1024 - t;
        diff = diff / 16;
    }
    else if (t >= 921 && t <= 1024)
    {
        /*Fall back*/
        t -= 921;
        t = t * 10; /*to [0..1024] range*/
        diff = diff / 16;
    }

    if (t > 1024)
        t = 1024;

    int32_t step = lv_bezier3(t, 1024, 1024, 800, 0);

    int32_t new_value;
    new_value = (int32_t)step * diff;
    new_value = new_value >> 10;
    new_value = a->end - new_value;

    return (int16_t)new_value;
}

void gui_anim_handler()
{
    gui_anim_t *anim = anim_head;
    int16_t path_value = 0;

    while (anim != NULL)
    {
        if (anim->enable)
        {
            anim->now_tick += 10;

            if (anim->now_tick > 0)
            {
                if (anim->now_tick >= anim->tick)
                {
                    anim->now_tick = anim->tick;
                }

                if (anim->path_cb != NULL)
                {
                    path_value = anim->path_cb(anim); // 计算路径点
                }

                if (anim->control_cb != NULL)
                {
                    anim->control_cb(anim->obj, path_value); // 设置路径点
                }

                if (anim->now_tick >= anim->tick) // 到达设定的时间
                {
                    if (anim->paly_back_mode && !anim->paly_back_now) // 若为回放模式且回放模式未到回放阶段
                    {
                        anim->paly_back_now_tick += 10;                       // 延时定时器增加
                        if (anim->paly_back_now_tick >= anim->paly_back_tick) // 超过延时时间
                        {
                            int16_t temp = anim->end; // 将起始点和终点调换
                            anim->end = anim->start;
                            anim->start = temp;

                            anim->paly_back_now_tick = 0;
                            anim->now_tick = 0;
                            anim->paly_back_now = 1; // 进入回放模式的回放阶段
                        }
                    }
                    else // 若不在回放模式或回放阶段动画完成
                    {
                        anim->paly_back_now_tick = 0;
                        anim->now_tick = -anim->start_tick;
                        anim->paly_back_now = 0;

                        if (anim->paly_back_mode) // 若在回放模式 需要将起始点和终点调换
                        {
                            int16_t temp = anim->end;
                            anim->end = anim->start;
                            anim->start = temp;
                        }

                        if (!anim->loop_mode) // 若不在循环模式  结束动画
                        {
                            anim->enable = 0;
                            anim->playing = 0;
                        }

                        if (anim->over_cb) // 动画结束回调
                        {
                            anim->over_cb(anim);
                        }
                    }
                }
            }
        }

        anim = anim->next;
    }
}
