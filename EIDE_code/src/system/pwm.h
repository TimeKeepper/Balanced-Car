#ifndef _PWM_H_
#define _PWM_H_

#include "system.h"

#define PWM_TIMx_PRESCALER              72      //you can change two value below if you like:)
#define PWM_TIMx_PERIOD                 1000
#define PWM_DEFAULT_LEVEL               0       //if you want to keep the PWM's default high level, set this value to PWM_TIMx_PERIOD
#define PWM_Mode_DEFAULT                TIM_OCMode_Inactive //the default mode after PWM be initialized

void PWM_Timer_Init(TIM_TypeDef* PWM_TIMx, GPIO_TypeDef* PWM_PORT, u16 PWM_GPIO_Pin_x,u8 PWM_Channel_x);
void PWM_Set_frequency(u32 PWM_Prescaler,TIM_TypeDef* PWM_TIMx);
void PWM_Set_Compare(u8 PWM_Compare,TIM_TypeDef* PWM_TIMx,u8 PWM_Channel_x);
void PWM_ON(TIM_TypeDef* PWM_TIMx,u8 PWM_Channel);
void PWM_STOP(TIM_TypeDef* PWM_TIMx,u8 PWM_Channel);
void PWM_KEEP_OFF(TIM_TypeDef* PWM_TIMx,u8 PWM_Channel);
void PWM_KEEP_ON(TIM_TypeDef* PWM_TIMx,u8 PWM_Channel);

#endif
