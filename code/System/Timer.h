#ifndef _TIMER_H_
#define _TIMER_H_

#include "system.h"
#include "led.h"
//以下是PWM输出各个默认值
#define PWM_DEFAULT_LEAVEL 0 //定义PWM输出的默认电平，若定义为高电平则修改为arr,定义为低电平则修改为0
#define PWM_ARR_Default 100	 //默认ARR的值，100分频是因为顺眼
#define PWM_PSC_Default 72	 //默认状态下是10KHz
#define PWM_Mode_Default TIM_OCMode_Inactive //初始化后默认状态为无效电平

//以下是通用定时器中断模式初始化宏定义
#define GP_IT_TIMx 					TIM1
#define GP_IT_TIMx_IRQn				TIM1_IRQn
#define GP_IT_TIMx_IRQHandler		TIM3_IRQHandler
#define GP_IT_RCC_APB1Periph_TIMx 	RCC_APB2Periph_TIM1

//以下是PWM输出的外设宏定义
#define PWM_TIMx 					TIM2
#define PWM_TIMx_IRQn				TIM2_IRQn
#define PWM_TIMx_IRQHandler			TIM2_IRQHandler
#define PWM_RCC_APB1Periph_TIMx 	RCC_APB1Periph_TIM2
#define PWM_TIM_OCxInit				TIM_OC1Init
#define PWM_TIM_OCChannel			TIM_Channel_1
#define TIM_SetComparex				TIM_SetCompare1
#define PWM_PORT_REMAP				0		//如果不需要重映射设置为0
#define PWM_PORT					GPIOA
#define PWM_GPIO_Pin_x				GPIO_Pin_0
#define PWM_RCC_APB2Periph_GPIOx	RCC_APB2Periph_GPIOA


void GP_Timer_IT_Init(void);
void PWM_Timer_Init(void);
void PWM_Set_frequency(u32 PWM_Prescaler);
void PWM_Set_Compare(u8 PWM_Compare);
void PWM_ON(void);
void PWM_STOP(void);
void PWM_KEEP_OFF(void);
void PWM_KEEP_ON(void);

#endif
