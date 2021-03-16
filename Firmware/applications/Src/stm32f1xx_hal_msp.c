#include "main.h"
#include "stm32f1xx_hal.h"
#include "spi.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

void HAL_MspInit(void)
{
    __HAL_RCC_AFIO_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    __HAL_AFIO_REMAP_SWJ_NOJTAG();
}

DMA_HandleTypeDef hdma_uart1tx = {0};
void HAL_UART_MspInit(UART_HandleTypeDef *huart) //会被HAL_UART_Init()所调用
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (huart->Instance == USART1)
    {
        __HAL_RCC_GPIOA_CLK_ENABLE(); //GPIOA时钟开启

        __HAL_RCC_USART1_CLK_ENABLE(); //串口1外设时钟开启

        __HAL_RCC_DMA1_CLK_ENABLE(); //DMA1外设时钟开启

        GPIO_InitStruct.Pin = GPIO_PIN_9; //TX
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_10; //RX
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        //设置DMA
        hdma_uart1tx.Instance = DMA1_Channel4;
        hdma_uart1tx.Init.Direction = DMA_MEMORY_TO_PERIPH;          //内存至外设
        hdma_uart1tx.Init.PeriphInc = DMA_PINC_DISABLE;              //外设地址不变
        hdma_uart1tx.Init.MemInc = DMA_MINC_ENABLE;                  //内存地址自增
        hdma_uart1tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; //外设数据宽度8位
        hdma_uart1tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;    //内存数据宽度8位
        hdma_uart1tx.Init.Mode = DMA_NORMAL;                         //普通模式
        hdma_uart1tx.Init.Priority = DMA_PRIORITY_LOW;               //优先级为低

        HAL_DMA_Init(&hdma_uart1tx); //DMA初始化

        __HAL_LINKDMA(huart, hdmatx, hdma_uart1tx); //连接串口与DMA通道

        HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 4); //设置DMA1通道4 的中断优先级
        HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);

        HAL_NVIC_SetPriority(USART1_IRQn, 2, 3); //设置串口1的中断优先级
        HAL_NVIC_EnableIRQ(USART1_IRQn);
    }
    else if (huart->Instance == USART2)
    {
        __HAL_RCC_GPIOA_CLK_ENABLE(); //GPIOA时钟开启

        __HAL_RCC_USART2_CLK_ENABLE(); //串口1外设时钟开启

        // __HAL_RCC_DMA1_CLK_ENABLE(); //DMA1外设时钟开启

        GPIO_InitStruct.Pin = GPIO_PIN_2; // TX
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_3; // RX
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_8; // RE
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);

        // //设置DMA
        // hdma_uart1tx.Instance = DMA1_Channel4;
        // hdma_uart1tx.Init.Direction = DMA_MEMORY_TO_PERIPH;          //内存至外设
        // hdma_uart1tx.Init.PeriphInc = DMA_PINC_DISABLE;              //外设地址不变
        // hdma_uart1tx.Init.MemInc = DMA_MINC_ENABLE;                  //内存地址自增
        // hdma_uart1tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; //外设数据宽度8位
        // hdma_uart1tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;    //内存数据宽度8位
        // hdma_uart1tx.Init.Mode = DMA_NORMAL;                         //普通模式
        // hdma_uart1tx.Init.Priority = DMA_PRIORITY_LOW;               //优先级为低

        // HAL_DMA_Init(&hdma_uart1tx); //DMA初始化

        // __HAL_LINKDMA(huart, hdmatx, hdma_uart1tx); //连接串口与DMA通道

        // HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 4); //设置DMA1通道4 的中断优先级
        // HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);

        HAL_NVIC_SetPriority(USART2_IRQn, 2, 3); //设置串口1的中断优先级
        HAL_NVIC_EnableIRQ(USART2_IRQn);
    }
    else if (huart->Instance == USART3)
    {
        __HAL_RCC_GPIOB_CLK_ENABLE(); //GPIOB时钟开启

        __HAL_RCC_USART3_CLK_ENABLE(); //串口3外设时钟开启

        //__HAL_RCC_DMA1_CLK_ENABLE(); //DMA1外设时钟开启

        GPIO_InitStruct.Pin = GPIO_PIN_10; //TX
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_11; //RX
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        // //设置DMA
        // hdma_uart1tx.Instance = DMA1_Channel4;
        // hdma_uart1tx.Init.Direction = DMA_MEMORY_TO_PERIPH;          //内存至外设
        // hdma_uart1tx.Init.PeriphInc = DMA_PINC_DISABLE;              //外设地址不变
        // hdma_uart1tx.Init.MemInc = DMA_MINC_ENABLE;                  //内存地址自增
        // hdma_uart1tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; //外设数据宽度8位
        // hdma_uart1tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;    //内存数据宽度8位
        // hdma_uart1tx.Init.Mode = DMA_NORMAL;                         //普通模式
        // hdma_uart1tx.Init.Priority = DMA_PRIORITY_LOW;               //优先级为低

        // HAL_DMA_Init(&hdma_uart1tx); //DMA初始化

        // __HAL_LINKDMA(huart, hdmatx, hdma_uart1tx); //连接串口与DMA通道

        // HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 4); //设置DMA1通道4 的中断优先级
        // HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);

        HAL_NVIC_SetPriority(USART3_IRQn, 2, 4); //设置串口3的中断优先级
        HAL_NVIC_EnableIRQ(USART3_IRQn);
    }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        __HAL_RCC_USART1_FORCE_RESET();
        __HAL_RCC_USART1_RELEASE_RESET();

        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_10);
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9);

        HAL_DMA_DeInit(&hdma_uart1tx);

        HAL_NVIC_DisableIRQ(DMA1_Channel4_IRQn);
        HAL_NVIC_DisableIRQ(USART1_IRQn);
    }
    else
    {
    }
}

DMA_HandleTypeDef hdma_spi1tx = {0};
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (hspi->Instance == SPI1)
    {
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_SPI1_CLK_ENABLE(); //SPI1时钟开启
        __HAL_RCC_DMA1_CLK_ENABLE();

        // __HAL_AFIO_REMAP_SPI1_ENABLE(); //SPI1重映射

        GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_7; //SPI1_PIN_SCK | SPI1_PIN_MOSI;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9; // DC  CS
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Pull = GPIO_PULLUP;

        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8 | GPIO_PIN_9, GPIO_PIN_SET); //默认高

        GPIO_InitStruct.Pin = GPIO_PIN_15; //RST
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Pull = GPIO_PULLUP;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET); //初始高

        //DMA
        hdma_spi1tx.Instance = DMA1_Channel3;
        hdma_spi1tx.Init.Direction = DMA_MEMORY_TO_PERIPH;          //内存到外设
        hdma_spi1tx.Init.PeriphInc = DMA_PINC_DISABLE;              //外设地址不变
        hdma_spi1tx.Init.MemInc = DMA_MINC_ENABLE;                  //内存地址自增
        hdma_spi1tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; //外设数据宽度8位
        hdma_spi1tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;    //内存数据宽度8位
        hdma_spi1tx.Init.Mode = DMA_NORMAL;                         //普通模式
        hdma_spi1tx.Init.Priority = DMA_PRIORITY_HIGH;              //优先级为高

        HAL_DMA_Init(&hdma_spi1tx);

        __HAL_LINKDMA(hspi, hdmatx, hdma_spi1tx);

        HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 2, 3);
        HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
    }
    else if (hspi->Instance == SPI2)
    {
    }
}

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (hi2c->Instance == I2C1)
    {
        /* Peripheral clock enable */
        __HAL_RCC_I2C1_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();

        /**I2C1 GPIO Configuration
        PB6     ------> I2C1_SCL
        PB7     ------> I2C1_SDA
        */
        GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim_base)
{
    if (htim_base->Instance == TIM1)
    {
        __HAL_RCC_TIM1_CLK_ENABLE();
        __HAL_AFIO_REMAP_TIM1_DISABLE();
    }
}

void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (hcan->Instance == CAN1)
    {
        /* Peripheral clock enable */
        __HAL_RCC_CAN1_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        // CAN GPIO Configuration
        // PA11     ------> CAN_RX
        // PA12     ------> CAN_TX

        GPIO_InitStruct.Pin = GPIO_PIN_11;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_12;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        // 中断初始化
        // HAL_NVIC_SetPriority(USB_HP_CAN1_TX_IRQn, 0, 0);
        // HAL_NVIC_EnableIRQ(USB_HP_CAN1_TX_IRQn);
        HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
        HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
        // HAL_NVIC_SetPriority(CAN1_SCE_IRQn, 0, 0);
        // HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);
    }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef *hcan)
{
    if (hcan->Instance == CAN1)
    {
        __HAL_RCC_CAN1_CLK_DISABLE();

        // CAN GPIO Configuration
        // PA11     ------> CAN_RX
        // PA12     ------> CAN_TX
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11 | GPIO_PIN_12);
    }
}