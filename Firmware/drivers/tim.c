#include "tim.h"
#include "rtthread.h"

TIM_HandleTypeDef htim1 = {0};
void TIM1_Timer_Init(uint16_t arr, uint16_t psc)
{
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    htim1.Instance = TIM1;
    htim1.Init.Prescaler = psc;
    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim1.Init.Period = arr;
    htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
    {
    }
    else
    {
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
    {
    }
}

void TIM1_Timer_Start_IT(void)
{
    if (HAL_TIM_Base_Start_IT(&htim1) != HAL_OK)
    {
    }
    else
    {
    }
}

void TIM1_PWM_Init(uint16_t arr, uint16_t psc)
{
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};
    TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

    htim1.Instance = TIM1;
    htim1.Init.Prescaler = psc;
    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim1.Init.Period = arr;
    htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim1.Init.RepetitionCounter = 0;
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    HAL_TIM_PWM_Init(&htim1);

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig);

    HAL_TIM_PWM_Init(&htim1);

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
    {
    }

    sConfigOC.OCMode = TIM_OCMODE_PWM2;
    sConfigOC.Pulse = 2000;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;

    HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3);

    sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
    sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
    sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
    sBreakDeadTimeConfig.DeadTime = 0;
    sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
    sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
    sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
    HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig);

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    //HAL_TIM_MspPostInit(&htim3); //初始化pwm输出引脚}
}

void TIM1_PWM_Start(void)
{
    //HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
}

void TIM1_SetCompare(uint32_t Channel, uint16_t Compare)
{
    __HAL_TIM_SET_COMPARE(&htim1, Channel, Compare);
}

TIM_HandleTypeDef htim2 = {0};
void TIM2_Timer_Init(uint16_t arr, uint16_t psc)
{
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    htim2.Instance = TIM2;
    htim2.Init.Prescaler = psc;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = arr;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
    {
        //printf("TIM2_Base_Init_Fail\r\n");
    }
    else
    {
        //printf("TIM2_Base_Init_OK\r\n");
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

    if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
    {
        //printf("sMasterConfig_Init_Fail\r\n");
    }
    else
    {
        //printf("sMasterConfig_Init_OK\r\n");
    }
}

void TIM2_Timer_Start_IT(void)
{
    if (HAL_TIM_Base_Start_IT(&htim2) != HAL_OK)
    {
        //printf("TIM2_Base_IT_Init_Fail\r\n");
    }
    else
    {
        //printf("TIM2_Base_IT_Init_OK\r\n");
    }
}

TIM_HandleTypeDef htim3 = {0};
void TIM3_PWM_Init(uint16_t arr, uint16_t psc)
{
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};

    htim3.Instance = TIM3;
    htim3.Init.Prescaler = psc;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = arr;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
    {
        //printf("TIM3_Pwm_Init_Fail\r\n");
    }
    else
    {
        //printf("TIM3_Pwm_Init_OK\r\n");
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
    {
    }

    sConfigOC.OCMode = TIM_OCMODE_PWM2;
    sConfigOC.Pulse = 1500;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

    if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
    {
        //printf("TIM3_PwmC1_Init_Fail\r\n");
    }
    else
    {
        //printf("TIM3_PwmC1_Init_OK\r\n");
    }

    if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
    {
        //printf("TIM3_PwmC2_Init_Fail\r\n");
    }
    else
    {
        //printf("TIM3_PwmC2_Init_OK\r\n");
    }

    if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
    {
        //printf("TIM3_PwmC3_Init_Fail\r\n");
    }
    else
    {
        //printf("TIM3_PwmC3_Init_OK\r\n");
    }

    //HAL_TIM_MspPostInit(&htim3); //初始化pwm输出引脚
}

void TIM3_PWM_Start(void)
{
    if (HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1) != HAL_OK)
    {
        //printf("TIM3_PwmC1_Start_Fail\r\n");
    }
    else
    {
        //printf("TIM3_PwmC1_Start_OK\r\n");
    }

    if (HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2) != HAL_OK)
    {
        //printf("TIM3_PwmC2_Start_Fail\r\n");
    }
    else
    {
        //printf("TIM3_PwmC2_Start_OK\r\n");
    }

    if (HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3) != HAL_OK)
    {
        //printf("TIM3_PwmC3_Start_Fail\r\n");
    }
    else
    {
        //printf("TIM3_PwmC3_Start_OK\r\n");
    }
}

void TIM3_SetCompare(uint32_t Channel, uint16_t Compare)
{
    __HAL_TIM_SET_COMPARE(&htim3, Channel, Compare);
}

TIM_HandleTypeDef htim4 = {0};
void TIM4_Encoder_Init()
{
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_Encoder_InitTypeDef sConfig = {0};

    htim4.Instance = TIM4;
    htim4.Init.Prescaler = 0;
    htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim4.Init.Period = 0xFFFF;
    htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
    sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
    sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
    sConfig.IC1Filter = 6;
    sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
    sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
    sConfig.IC2Filter = 6;

    if (HAL_TIM_Encoder_Init(&htim4, &sConfig) != HAL_OK)
    {
        //printf("TIM4_Encoder_Init_Fail\r\n");
    }
    else
    {
        //printf("TIM4_Encoder_Init_OK\r\n");
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
    {
    }
}

void TIM4_Encoder_Start()
{
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_1);
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_2);
}

void TIM4_Encoder_Setcounter(uint16_t counter)
{
    __HAL_TIM_SET_COUNTER(&htim4, counter);
}

uint16_t TIM4_Encoder_Getcounter()
{
    return __HAL_TIM_GET_COUNTER(&htim4);
}
