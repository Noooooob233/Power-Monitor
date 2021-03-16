#include "stm32f1xx_hal.h"
#include <rtthread.h>
#include "oled.h"
#include "led.h"
#include "flash.h"
#include "iic.h"
#include "draw.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"

static void MX_GPIO_Init(void);

struct rt_semaphore sem_usb_rx;

int main(void)
{
	eeprom_init();

	oled_init();

	led_init();

	I2C1_Init();

	oled_show_init();

	MX_GPIO_Init();
	rt_thread_mdelay(500);
	MX_USB_DEVICE_Init();

	return 0;
}

static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitStruct.Pin = GPIO_PIN_12; //TX
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

	/* USER CODE END Error_Handler_Debug */
}
