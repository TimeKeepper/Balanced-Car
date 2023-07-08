#include "GP_Tim.h"


void GP_Timer_IT_Init(TIM_TypeDef* GP_IT_TIMx){
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_RCC_ENABLE(GP_IT_TIMx);

    TIM_InternalClockConfig(GP_IT_TIMx);
    TIM_TimeBaseStructure.TIM_Period = GP_IT_TIMx_PERIOD;
    TIM_TimeBaseStructure.TIM_Prescaler = GP_IT_TIMx_PRESCALER;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
    TIM_TimeBaseInit(GP_IT_TIMx, &TIM_TimeBaseStructure);
    TIM_ClearFlag(GP_IT_TIMx, TIM_FLAG_Update);
    TIM_ITConfig(GP_IT_TIMx, TIM_IT_Update, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = GET_TIM_IRQn(GP_IT_TIMx);
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//I don't want to add two new variables for interrupt priority,so I just use 0x01 here.
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;       //you can add or change this fuction to fit your own habit as you like
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    TIM_Cmd(GP_IT_TIMx, ENABLE);
}
