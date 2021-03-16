#include "draw.h"
#include "rtthread.h"
#include "oled.h"
#include "led.h"
#include "label.h"
#include "oscillograph.h"
#include "anim.h"
#include "ina226.h"

extern gui_font_t font1206;
extern gui_font_t font1608;

gui_obj_t vbus_label;
gui_label_attribute_t vbus_label_att;

gui_obj_t current_label;
gui_label_attribute_t current_label_att;

gui_obj_t power_label;
gui_label_attribute_t power_label_att;

gui_obj_t power_osc;
gui_osc_attribute_t power_osc_att;
int16_t power_osc_points[33];
gui_anim_t power_osc_anim;

static rt_uint8_t oled_draw_thread_stack[512];
struct rt_thread oled_draw_thread;
void oled_draw_entry(void *parameter);
void oled_show_init()
{
    gui_init();

    gui_set_oled_protect_mode(0);

    gui_label_init(&vbus_label, &vbus_label_att);
    gui_label_set_localtion(&vbus_label, 0, 10);
    gui_label_set_size(&vbus_label, 128, 16);
    gui_label_set_font(&vbus_label, &font1608);
    gui_screen_layer_add_obj(&vbus_label, 0);

    gui_label_init(&current_label, &current_label_att);
    gui_label_set_localtion(&current_label, 0, 26);
    gui_label_set_size(&current_label, 128, 16);
    gui_label_set_font(&current_label, &font1608);
    gui_screen_layer_add_obj(&current_label, 0);

    gui_label_init(&power_label, &power_label_att);
    gui_label_set_localtion(&power_label, 0, 42);
    gui_label_set_size(&power_label, 128, 16);
    gui_label_set_font(&power_label, &font1608);
    gui_screen_layer_add_obj(&power_label, 0);

    // gui_osc_init(&power_osc, &power_osc_att);
    // gui_osc_set_localtion(&power_osc, 0, 14);
    // gui_osc_set_size(&power_osc, 65, 37);
    // gui_osc_add_points(&power_osc, power_osc_points, 33);
    // gui_osc_points_init(&power_osc);
    // gui_osc_set_div(&power_osc, 2);
    // gui_screen_layer_add_obj(&power_osc, 0);

    rt_thread_init(&oled_draw_thread,
                   "draw",
                   oled_draw_entry,
                   RT_NULL,
                   &oled_draw_thread_stack[0],
                   sizeof(oled_draw_thread_stack),
                   2,
                   20);
    rt_thread_startup(&oled_draw_thread);
}

void oled_draw_entry(void *parameter)
{
    char tmp[30];

    double f = 0;
    uint8_t sign = 0;
    int16_t _int = 0;
    uint32_t _dec = 0;

    float volt = 0;
    float current = 0;
    float power = 0;

    ina226_init(0x80);

    while (1)
    {
        volt = ina226_read_vbus(0x80);
        current = ina226_read_current(0x80);
        power = ina226_read_power(0x80);

        led_control((50000 - power) * 18 / 500 + 200);

        if (volt < 1000)
        {
            f = volt + 0.005;
            _int = f;
            _dec = 100 * (f - _int);

            rt_sprintf(tmp, "V:%d.%02dmV", _int, _dec);
        }
        else
        {
            f = (volt / 1000.0) + 0.00005;
            _int = f;
            _dec = 10000 * (f - _int);

            rt_sprintf(tmp, "V:%d.%04dV", _int, _dec);
        }

        gui_label_set_text_by_string(&vbus_label, tmp);

        if (current < 0)
        {
            current = -current;
            sign = 1;
        }
        else
        {
            sign = 0;
        }

        if (current < 1000)
        {
            f = current + 0.005; // 四舍五入

            _int = f;
            _dec = 100 * (f - _int);

            if (sign)
            {
                rt_sprintf(tmp, "I:-%d.%02dmA", _int, _dec);
            }
            else
            {
                rt_sprintf(tmp, "I:%d.%02dmA", _int, _dec);
            }
        }
        else
        {
            f = (current / 1000.0) + 0.00005; // 四舍五入

            _int = f;
            _dec = 10000 * (f - _int);

            if (sign)
            {
                rt_sprintf(tmp, "I:-%d.%04dA", _int, _dec);
            }
            else
            {
                rt_sprintf(tmp, "I:%d.%04dA", _int, _dec);
            }
        }

        gui_label_set_text_by_string(&current_label, tmp);

        if (power < 1000)
        {
            f = power + 0.005; // 四舍五入

            _int = f;
            _dec = 100 * (f - _int);

            rt_sprintf(tmp, "P:%d.%02dmW", _int, _dec);
        }
        else
        {
            f = (power / 1000.0) + 0.00005; // 四舍五入

            _int = f;
            _dec = 10000 * (f - _int);

            rt_sprintf(tmp, "P:%d.%04dW", _int, _dec);
        }

        gui_label_set_text_by_string(&power_label, tmp);

        // gui_osc_add_data(&power_osc, data * 100 / 10000);

        rt_thread_mdelay(100);
    }
}
