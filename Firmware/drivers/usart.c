#include "usart.h"
#include "rtthread.h"
#include "ringbuffer.h"
#include "oled.h"

uint8_t usart1_rx_temp = 0;
uint8_t usart1_rx_buffer[16] = {0};
struct rt_ringbuffer usart1_rx_rb;
struct rt_semaphore sem_usart1_rx;
struct rt_semaphore sem_usart1_tx_cplt;

UART_HandleTypeDef huart1;
void USART1_Init(uint32_t bound)
{
    rt_ringbuffer_init(&usart1_rx_rb, usart1_rx_buffer, 16);
    rt_sem_init(&sem_usart1_rx, "sem_usart1_rx", 0, RT_IPC_FLAG_FIFO);
    rt_sem_init(&sem_usart1_tx_cplt, "sem_usart1_tx_cplt", 0, RT_IPC_FLAG_FIFO);

    huart1.Instance = USART1;
    huart1.Init.BaudRate = bound;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;

    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        while (1)
            ;
    }

    __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);

    // HAL_UART_Receive_IT(&huart1, &usart1_rx_temp, 1);
}

UART_HandleTypeDef huart2;
void USART2_Init(uint32_t bound)
{
    huart2.Instance = USART2;
    huart2.Init.BaudRate = bound;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;

    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        while (1)
            ;
    }

    __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);

    // HAL_UART_Receive_IT(&huart1, &usart1_rx_temp, 1);
}

UART_HandleTypeDef huart3;
void USART3_Init(uint32_t bound)
{
    huart3.Instance = USART3;
    huart3.Init.BaudRate = bound;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;

    if (HAL_UART_Init(&huart3) != HAL_OK)
    {
        while (1)
            ;
    }
}

//开启一次DMA传输
void USART1_DMA_Enable(uint8_t *buffer, uint16_t sendlen)
{
    if (HAL_UART_Transmit_DMA(&huart1, buffer, sendlen) != HAL_OK)
    {
        //Error_Handler();
    }
}

void USART1_IRQHandler()
{
    int ch = -1;

    /* enter interrupt */
    rt_interrupt_enter(); //在中断中一定要调用这对函数，进入中断

    if ((__HAL_UART_GET_FLAG(&(huart1), UART_FLAG_RXNE) != RESET) &&
        (__HAL_UART_GET_IT_SOURCE(&(huart1), UART_IT_RXNE) != RESET))
    {
        while (1)
        {
            ch = -1;
            if (__HAL_UART_GET_FLAG(&(huart1), UART_FLAG_RXNE) != RESET)
            {
                ch = huart1.Instance->DR & 0xff;
            }
            if (ch == -1)
            {
                break;
            }
            /* 读取到数据，将数据存入 ringbuffer */
            rt_ringbuffer_putchar(&usart1_rx_rb, ch);
        }
        rt_sem_release(&sem_usart1_rx);
    }

    /* leave interrupt */
    rt_interrupt_leave(); //在中断中一定要调用这对函数，离开中断
}

extern DMA_HandleTypeDef hdma_uart1tx;
void DMA1_Channel4_IRQHandler(void)
{
    rt_interrupt_enter();

    HAL_DMA_IRQHandler(&hdma_uart1tx);

    rt_interrupt_leave();
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        rt_sem_release(&sem_usart1_tx_cplt);
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        rt_ringbuffer_putchar(&usart1_rx_rb, usart1_rx_temp);

        HAL_UART_Receive_IT(&huart1, &usart1_rx_temp, 1);

        // rt_sem_release(&sem_usart1_rx);
    }
    else if (huart->Instance == USART3)
    {
    }
}

void rt_hw_console_output(const char *str)
{
    rt_size_t i = 0, size = 0;
    char a = '\r';

    size = rt_strlen(str);

    for (i = 0; i < size; i++)
    {
        if (*(str + i) == '\n')
        {
            HAL_UART_Transmit(&huart1, (uint8_t *)&a, 1, 1000);
        }
        HAL_UART_Transmit(&huart1, (uint8_t *)(str + i), 1, 1000);
    }
}

char rt_hw_console_getchar(void)
{
    char ch = 0;

    /* 从 ringbuffer 中拿出数据 */
    while (rt_ringbuffer_getchar(&usart1_rx_rb, (rt_uint8_t *)&ch) != 1)
    {
        rt_sem_take(&sem_usart1_rx, RT_WAITING_FOREVER);
    }
    return ch;
}
