#include "spi.h"
#include "rtthread.h"

extern DMA_HandleTypeDef hdma_spi1tx;
void SPI1_Init()
{
    /* SPI1 Config -------------------------------------------------------------*/
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_1LINE;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
    hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 10;

    if (HAL_SPI_Init(&hspi1) != HAL_OK)
    {
        rt_kprintf("SPI1_Init_Fail\r\n");
        while (1)
            ;
    }
    else
    {
        rt_kprintf("SPI1_Init_OK\r\n");
    }
}

void SPI2_Init()
{
    /* SPI2 Config -------------------------------------------------------------*/
    hspi2.Instance = SPI2;
    hspi2.Init.Mode = SPI_MODE_MASTER;
    hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    hspi2.Init.Direction = SPI_DIRECTION_2LINES;
    hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
    hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;
    hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi2.Init.CRCPolynomial = 10;
    hspi2.Init.NSS = SPI_NSS_SOFT;

    if (HAL_SPI_Init(&hspi2) != HAL_OK)
    {
        rt_kprintf("SPI2_Init_Fail\r\n");
        while (1)
            ;
    }
    else
    {
        rt_kprintf("SPI2_Init_OK\r\n");
    }
}

uint8_t SPI2_ReadWriteByte(uint8_t TxData)
{
    uint8_t RxData = 0;

    HAL_SPI_TransmitReceive(&hspi2, &TxData, &RxData, 1, 1000);

    return RxData;
}

void DMA1_Channel3_IRQHandler(void)
{
    rt_interrupt_enter();
    HAL_DMA_IRQHandler(&hdma_spi1tx);
    rt_interrupt_leave();
}
