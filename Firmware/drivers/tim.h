#ifndef __TIM_H
#define __TIM_H

#include "stm32f1xx_hal.h"

void TIM1_Timer_Init(uint16_t arr, uint16_t psc);
void TIM1_Timer_Start_IT(void);
void TIM1_PWM_Init(uint16_t arr, uint16_t psc);
void TIM1_PWM_Start(void);
void TIM1_SetCompare(uint32_t Channel, uint16_t Compare);

void TIM2_Timer_Init(uint16_t arr, uint16_t psc);
void TIM2_Timer_Start_IT(void);

void TIM3_PWM_Init(uint16_t arr, uint16_t psc);
void TIM3_PWM_Start(void);
void TIM3_SetCompare(uint32_t Channel, uint16_t Compare);

void TIM4_Encoder_Init(void);
void TIM4_Encoder_Start(void);
void TIM4_Encoder_Setcounter(uint16_t counter);
uint16_t TIM4_Encoder_Getcounter(void);

#endif
