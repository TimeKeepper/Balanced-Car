#include "hsr04.h"

//static variables which save the parametre when HSR04_Init() is called
static GPIO_TypeDef * LOC_Trig_PORT = 0;
static u16 LOC_Trig_Pin = 0;
static GPIO_TypeDef * LOC_Echo_PORT = 0;
static u16 LOC_Echo_Pin = 0;
static TIM_TypeDef * LOC_TIMx = 0;

//static variables which save the distance value
static uint32_t LOC_Distance = 0;
static uint32_t LOC_Distance_count = 0;
static uint32_t LOC_Final_Distance = 0;

void HSR04_Init(GPIO_TypeDef * Trig_PORT, u16 Trig_Pin, GPIO_TypeDef * Echo_PORT, u16 Echo_Pin, TIM_TypeDef * TIMx){
    LOC_Trig_PORT = Trig_PORT;
    LOC_Trig_Pin = Trig_Pin;
    LOC_Echo_PORT = Echo_PORT;
    LOC_Echo_Pin = Echo_Pin;
    LOC_TIMx = TIMx;

    GP_Timer_IT_Init(LOC_TIMx);

    GPIO_RCC_ENABLE(Trig_PORT);GPIO_RCC_ENABLE(Echo_PORT);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStruct.GPIO_Pin = Trig_Pin;
    GPIO_Init(Trig_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Pin = Echo_Pin;
    GPIO_Init(Echo_PORT, &GPIO_InitStruct);

    GPIO_EXTILineConfig(GET_PORT_SOURCE(Echo_PORT), GET_GPIO_PinSource(Echo_Pin));
    EXTI_InitTypeDef EXTI_InitStruct;
    EXTI_InitStruct.EXTI_Line = GET_EXTI_LINE(Echo_Pin);
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);

    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = GET_EXTI_IRQn(Echo_Pin);
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}

void HSR04_Start(void){
    GPIO_SetBits(LOC_Trig_PORT, LOC_Trig_Pin);
    delay_us(30);
    GPIO_ResetBits(LOC_Trig_PORT, LOC_Trig_Pin);
}

uint32_t HSR04_Get_Distance_cm(void){
    return LOC_Final_Distance;
}

void HSR_When_EXTI_Interrupt(void){
    uint32_t HSR_EXTI_LINE = GET_EXTI_LINE(LOC_Echo_Pin);
    if(EXTI_GetITStatus(HSR_EXTI_LINE) != RESET){
        EXTI_ClearITPendingBit(HSR_EXTI_LINE);
        if(GPIO_ReadInputDataBit(LOC_Echo_PORT, LOC_Echo_Pin) == 1){
            LOC_Distance = TIM_GetCounter(LOC_TIMx);LOC_Distance_count = 0;
        }
        else{
            LOC_Distance = (LOC_Distance_count * 10000)+(TIM_GetCounter(LOC_TIMx))- LOC_Distance;
            LOC_Final_Distance = LOC_Distance * 0.017;
        }
    }
}

void HSR_When_TIM_Interrupt(void){
    if(TIM_GetITStatus(LOC_TIMx, TIM_IT_Update) != RESET){
        TIM_ClearITPendingBit(LOC_TIMx, TIM_IT_Update);
        LOC_Distance_count++;
    }
}
