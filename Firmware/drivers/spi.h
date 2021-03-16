#ifndef __SPI_H
#define __SPI_H

#include "stm32f1xx_hal.h"

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

void SPI1_Init(void);
void SPI2_Init(void);
uint8_t SPI2_ReadWriteByte(uint8_t TxData);

inline void SPI1_DMA_Enable(uint8_t *pData, uint16_t Size)
{
    HAL_SPI_Transmit_DMA(&hspi1, pData, Size);
}

inline void SPI1_WriteByte(uint8_t TxData)
{
    HAL_SPI_Transmit(&hspi1, &TxData, 1, 1000);
}


#endif
