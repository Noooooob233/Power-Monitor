#include "oled.h"
#include "rtthread.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "string.h"
#include "math.h"
#include "flash.h"
#include "led.h"

extern uint8_t vdb[8][128];

struct rt_semaphore sem_oled_dma_TxCplt;
struct rt_mutex mutex_oled_lock;

static rt_uint8_t oled_send_thread_stack[200];
static struct rt_thread oled_send_thread;

void oled_send_entry(void *parameter);

//OLED初始化
void oled_init(void)
{
    rt_sem_init(&sem_oled_dma_TxCplt, "dmatxcplt", 0, RT_IPC_FLAG_FIFO);
    rt_mutex_init(&mutex_oled_lock, "oled_lock", RT_IPC_FLAG_FIFO);

    rt_thread_init(&oled_send_thread, //OLED线程初始化
                   "oled_spi",
                   oled_send_entry,
                   RT_NULL,
                   &oled_send_thread_stack[0],
                   sizeof(oled_send_thread_stack),
                   4,
                   20);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET); //复位
    rt_thread_mdelay(1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET); //开启SSD1306片选

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET); //写入命令

    SPI1_WriteByte(0xAE); //关闭屏幕
    // HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);   //关闭SSD1306片选
    // HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET); //开启SSD1306片选

    SPI1_WriteByte(0x02); //设置列地址低位
    SPI1_WriteByte(0x10); //设置列地址高位

    SPI1_WriteByte(0x40); //设置显示起始线 40H-7FH  --set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)

    SPI1_WriteByte(0x81); //设置对比度
    SPI1_WriteByte(0x30); // 00H-FFH

    SPI1_WriteByte(0xA1); //设置段重映射--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    SPI1_WriteByte(0xC8); //--Set Common Output Scan Direction   0xc0上下反置 0xc8正常

    SPI1_WriteByte(0xA8); //--set multiplex ratio(1 to 64)
    SPI1_WriteByte(0x3F); //--1/64 duty 00H-3FH

    SPI1_WriteByte(0xD3); //设置显示偏移--set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    SPI1_WriteByte(0x00); //--not offset

    SPI1_WriteByte(0xD5); //--set display clock divide ratio/oscillator frequency
    SPI1_WriteByte(0x80); //--set divide ratio, Set Clock as 100 Frames/Sec 00H-FFH

    SPI1_WriteByte(0xD9); //--set pre-charge period
    SPI1_WriteByte(0x22); //--Set Pre-Charge as 15 Clocks & Discharge as 1 Clock F1

    SPI1_WriteByte(0xDA); //--set com pins hardware configuration
    SPI1_WriteByte(0x12); //02H-12H

    SPI1_WriteByte(0xDB); //--set vcomh 00H-FFH
    SPI1_WriteByte(0x35); //--Set VCOM Deselect Level

    SPI1_WriteByte(0xAD); //Set DC-DC OFF/ON
    SPI1_WriteByte(0x8B); //8AH-8BH

    SPI1_WriteByte(0x32); //设置电荷泵电压

    SPI1_WriteByte(0xA4); //--Disable Entire Display On (0xa4/0xa5)
    SPI1_WriteByte(0xA6); //--Disable Inverse Display On (0xa6/a7)

    // SPI1_WriteByte(0xAF); //开启屏幕

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET); //关闭SSD1306片选

    rt_thread_startup(&oled_send_thread); // 开启oled进程
}

void oled_set_light(uint8_t value)
{
    rt_mutex_take(&mutex_oled_lock, RT_WAITING_FOREVER);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET); //开启SSD1306片选
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET); //写入命令

    SPI1_WriteByte(0x81);
    SPI1_WriteByte(value);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET); //关闭SSD1306片选

    rt_mutex_release(&mutex_oled_lock);
}

void oled_set_power(uint8_t en)
{
    rt_mutex_take(&mutex_oled_lock, RT_WAITING_FOREVER);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET); //开启SSD1306片选
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET); //写入命令

    if (en)
    {
        SPI1_WriteByte(0XAF); //DISPLAY ON
    }
    else
    {
        SPI1_WriteByte(0XAE); //DISPLAY OFF
    }

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET); //关闭SSD1306片选

    rt_mutex_release(&mutex_oled_lock);
}

void oled_set_direction(uint8_t direction)
{
    rt_mutex_take(&mutex_oled_lock, RT_WAITING_FOREVER);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET); //开启SSD1306片选
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET); //写入命令

    if (direction)
    {
        SPI1_WriteByte(0xA0); //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
        SPI1_WriteByte(0xC0); //--Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    }
    else
    {
        SPI1_WriteByte(0xA1); //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
        SPI1_WriteByte(0xC8); //--Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    }

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET); //关闭SSD1306片选

    rt_mutex_release(&mutex_oled_lock);
}

// void oled_change_direction(void)
// {
//     config_t oled_config;
//     oled_config = get_config();

//     rt_mutex_take(&mutex_oled_lock, RT_WAITING_FOREVER);

//     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET); //开启SSD1306片选

//     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET); //写入命令

//     if (oled_config.direction)
//     {
//         SPI1_WriteByte(0xA1); //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
//         SPI1_WriteByte(0xC8); //--Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
//         oled_config.direction = 0;
//     }
//     else
//     {
//         SPI1_WriteByte(0xA0); //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
//         SPI1_WriteByte(0xC0); //--Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
//         oled_config.direction = 1;
//     }

//     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET); //关闭SSD1306片选

//     rt_mutex_release(&mutex_oled_lock);

//     save_config(oled_config);
// }
// FINSH_FUNCTION_EXPORT(oled_change_direction, change);
// MSH_CMD_EXPORT(oled_change_direction, change);

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI1)
    {
        rt_sem_release(&sem_oled_dma_TxCplt); //释放dma发送完毕信号量
    }
}

void oled_send_entry(void *parameter) // oled数据发送进程
{
    uint8_t i;
    uint8_t oled_cmd[] = {0xb0,  //页
                          0x02,  //起始列地址低位
                          0x10}; //起始列地址高位

    oled_set_power(1); // 开启屏幕

    while (1)
    {
        rt_mutex_take(&mutex_oled_lock, RT_WAITING_FOREVER);

        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET); //开启SSD1306片选

        for (i = 0; i < 8; i++)
        {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET); //写入命令

            oled_cmd[0] = 0xb0 + i;
            SPI1_DMA_Enable(oled_cmd, 3);
            rt_sem_take(&sem_oled_dma_TxCplt, RT_WAITING_FOREVER); //等待dma发送完毕

            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET); //写入数据

            SPI1_DMA_Enable(&vdb[i][0], 128);
            rt_sem_take(&sem_oled_dma_TxCplt, RT_WAITING_FOREVER); //等待dama发送完毕
        }

        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET); //关闭SSD1306片选}

        rt_mutex_release(&mutex_oled_lock);

        rt_thread_mdelay(10);
    }
}

DMA_HandleTypeDef hdma_oled_rec = {0};
void DMA1_channel7_Init(void)
{
    __HAL_RCC_DMA1_CLK_ENABLE();

    //DMA
    hdma_oled_rec.Instance = DMA1_Channel7;
    hdma_oled_rec.Init.Direction = DMA_MEMORY_TO_MEMORY;          //内存至内存
    hdma_oled_rec.Init.PeriphInc = DMA_PINC_ENABLE;               //外设地址自增
    hdma_oled_rec.Init.MemInc = DMA_MINC_ENABLE;                  //内存地址自增
    hdma_oled_rec.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; //8位
    hdma_oled_rec.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;    //高位在前
    hdma_oled_rec.Init.Mode = DMA_NORMAL;                         //
    hdma_oled_rec.Init.Priority = DMA_PRIORITY_HIGH;              //优先级高

    HAL_DMA_Init(&hdma_oled_rec);

    HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 2, 2);
    HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);
}

void DMA1_Channel7_IRQHandler(void)
{
    rt_interrupt_enter();
    HAL_DMA_IRQHandler(&hdma_oled_rec);
    rt_interrupt_leave();
}
