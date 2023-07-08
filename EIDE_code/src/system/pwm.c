#include "pwm.h"

void PWM_Timer_Init(TIM_TypeDef* PWM_TIMx, GPIO_TypeDef* PWM_PORT, u16 PWM_GPIO_Pin_x,u8 PWM_Channel_x){
    TIM_RCC_ENABLE(PWM_TIMx);
    GPIO_RCC_ENABLE(PWM_PORT);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = PWM_GPIO_Pin_x;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PWM_PORT,&GPIO_InitStructure);

    TIM_InternalClockConfig(PWM_TIMx);
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = PWM_TIMx_PERIOD-1;
    TIM_TimeBaseStructure.TIM_Prescaler = PWM_TIMx_PRESCALER-1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(PWM_TIMx, &TIM_TimeBaseStructure);

    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = PWM_Mode_DEFAULT;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = PWM_DEFAULT_LEVEL;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    switch(PWM_Channel_x){
        case 1:
            TIM_OC1Init(PWM_TIMx, &TIM_OCInitStructure);
            break;
        case 2:
            TIM_OC2Init(PWM_TIMx, &TIM_OCInitStructure);
            break;
        case 3:
            TIM_OC3Init(PWM_TIMx, &TIM_OCInitStructure);
            break;
        case 4:
            TIM_OC4Init(PWM_TIMx, &TIM_OCInitStructure);
            break;
        default:
            break;
    }

    TIM_Cmd(PWM_TIMx, ENABLE);
}

void PWM_Set_frequency(u32 PWM_Prescaler,TIM_TypeDef* PWM_TIMx){
    u16 i = (72000000/PWM_TIMx_PERIOD/PWM_Prescaler);
    TIM_PrescalerConfig(PWM_TIMx, i, TIM_PSCReloadMode_Immediate);
}

void PWM_Set_Compare(u8 PWM_Compare,TIM_TypeDef* PWM_TIMx,u8 PWM_Channel_x){
    u16 i = PWM_TIMx_PERIOD*PWM_Compare/100;
    switch(PWM_Channel_x){
        case 1:
            TIM_SetCompare1(PWM_TIMx, i);
            break;
        case 2:
            TIM_SetCompare2(PWM_TIMx, i);
            break;
        case 3:
            TIM_SetCompare3(PWM_TIMx, i);
            break;
        case 4:
            TIM_SetCompare4(PWM_TIMx, i);
            break;
        default:
            break;
    }
}

static uint16_t GET_PWM_Channel(u8 PWM_Channel){
    return (uint16_t)(0x0000+4*(PWM_Channel-1));
}

void PWM_ON(TIM_TypeDef* PWM_TIMx,u8 PWM_Channel){
    uint16_t TIM_Channel = GET_PWM_Channel(PWM_Channel);
    TIM_SelectOCxM(PWM_TIMx, TIM_Channel, TIM_OCMode_PWM1);
    TIM_CCxCmd(PWM_TIMx, TIM_Channel, TIM_CCx_Enable);
}

void PWM_STOP(TIM_TypeDef* PWM_TIMx,u8 PWM_Channel){
    uint16_t TIM_Channel = GET_PWM_Channel(PWM_Channel);
    TIM_SelectOCxM(PWM_TIMx, TIM_Channel, TIM_OCMode_Timing);
    TIM_CCxCmd(PWM_TIMx, TIM_Channel, TIM_CCx_Enable);
}

void PWM_KEEP_OFF(TIM_TypeDef* PWM_TIMx,u8 PWM_Channel){
    uint16_t TIM_Channel = GET_PWM_Channel(PWM_Channel);
    TIM_SelectOCxM(PWM_TIMx, TIM_Channel, TIM_OCMode_Inactive);
    TIM_CCxCmd(PWM_TIMx, TIM_Channel, TIM_CCx_Enable);
}

void PWM_KEEP_ON(TIM_TypeDef* PWM_TIMx,u8 PWM_Channel){
    uint16_t TIM_Channel = GET_PWM_Channel(PWM_Channel);
    TIM_SelectOCxM(PWM_TIMx, TIM_Channel, TIM_OCMode_Active);
    TIM_CCxCmd(PWM_TIMx, TIM_Channel, TIM_CCx_Enable);
}
