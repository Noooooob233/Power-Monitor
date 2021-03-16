#ifndef __USART_H
#define __USART_H
#include "stm32f1xx_hal.h"

void USART1_Init(uint32_t bound);
void USART1_DMA_Enable(uint8_t *buffer, uint16_t sendlen);

void USART2_Init(uint32_t bound);

void USART3_Init(uint32_t bound);

#endif
