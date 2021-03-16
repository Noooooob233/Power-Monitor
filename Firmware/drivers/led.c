#include "led.h"
#include "tim.h"
#include <rtthread.h>

static rt_uint8_t led_thread_stack[154];
static struct rt_thread led_thread;
void led_entry(void *parameter);

void led_init()
{
    rt_err_t ret = RT_ERROR;

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE(); //GPIOB时钟开启

    GPIO_InitStruct.Pin = GPIO_PIN_15; //LED
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    ret = rt_thread_init(&led_thread,
                         "led",
                         led_entry,
                         RT_NULL,
                         &led_thread_stack[0],
                         sizeof(led_thread_stack),
                         5,
                         20);
    if (ret == RT_EOK)
        rt_thread_startup(&led_thread);
}

static uint16_t delay = 2000;
void led_control(uint16_t val)
{
    val = (val < 200) ? 200 : val;
    val = (val < 2000) ? 2000 : val;

    delay = val;
}

void led_entry(void *parameter)
{
    while (1)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
        rt_thread_mdelay(50);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
        rt_thread_mdelay(delay);
    }
}