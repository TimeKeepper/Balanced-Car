#include "system.h"

/*A long time ago, I was trying to figure out how to modify 
this piece of code to make it look more concise and elegant. 
Today, I still have no clue.*/
uint32_t GET_GPIO_RCC(GPIO_TypeDef* GPIOx){
    switch((uint32_t)GPIOx){
        case (uint32_t)GPIOA:
            return RCC_APB2Periph_GPIOA;
        case (uint32_t)GPIOB:
            return RCC_APB2Periph_GPIOB;
        case (uint32_t)GPIOC:
            return RCC_APB2Periph_GPIOC;
        case (uint32_t)GPIOD:
            return RCC_APB2Periph_GPIOD;
        case (uint32_t)GPIOE:
            return RCC_APB2Periph_GPIOE;
        case (uint32_t)GPIOF:
            return RCC_APB2Periph_GPIOF;
        case (uint32_t)GPIOG:
            return RCC_APB2Periph_GPIOG;
        default:
            return 0;
    }
}

void GPIO_RCC_ENABLE(GPIO_TypeDef* GPIOx){
    RCC_APB2PeriphClockCmd(GET_GPIO_RCC(GPIOx), ENABLE);
}

uint32_t GET_TIM_RCC(TIM_TypeDef* TIMx){
    switch((uint32_t)TIMx){
        case (uint32_t)TIM1:
            return RCC_APB2Periph_TIM1;
        case (uint32_t)TIM2:
            return RCC_APB1Periph_TIM2;
        case (uint32_t)TIM3:
            return RCC_APB1Periph_TIM3;
        case (uint32_t)TIM4:
            return RCC_APB1Periph_TIM4;
        case (uint32_t)TIM5:
            return RCC_APB1Periph_TIM5;
        case (uint32_t)TIM6:
            return RCC_APB1Periph_TIM6;
        case (uint32_t)TIM7:
            return RCC_APB1Periph_TIM7;
        case (uint32_t)TIM8:
            return RCC_APB2Periph_TIM8;
        default:
            return 0;
    }
}

uint8_t GET_TIM_IRQn(TIM_TypeDef* TIMx){
    switch((uint32_t)TIMx){
        case (uint32_t)TIM1:
            return TIM1_UP_IRQn;
        case (uint32_t)TIM2:
            return TIM2_IRQn;
        case (uint32_t)TIM3:
            return TIM3_IRQn;
        case (uint32_t)TIM4:
            return TIM4_IRQn;
        #ifdef STM32F10X_HD
        case (uint32_t)TIM5:
            return TIM5_IRQn;
        case (uint32_t)TIM6:
            return TIM6_IRQn;
        case (uint32_t)TIM7:
            return TIM7_IRQn;
        case (uint32_t)TIM8:
            return TIM8_UP_IRQn;
        #endif
        default:
            return 0;
    }
}

void TIM_RCC_ENABLE(TIM_TypeDef* TIMx){
    if(TIMx == TIM1 || TIMx == TIM8){
        RCC_APB2PeriphClockCmd(GET_TIM_RCC(TIMx), ENABLE);
    }
    else{
        RCC_APB1PeriphClockCmd(GET_TIM_RCC(TIMx), ENABLE);
    }
}

uint32_t GET_EXTI_LINE(uint32_t GPIO_Pin){
    switch(GPIO_Pin){
        case GPIO_Pin_0:
            return EXTI_Line0;
        case GPIO_Pin_1:
            return EXTI_Line1;
        case GPIO_Pin_2:
            return EXTI_Line2;
        case GPIO_Pin_3:
            return EXTI_Line3;
        case GPIO_Pin_4:
            return EXTI_Line4;
        case GPIO_Pin_5:
            return EXTI_Line5;
        case GPIO_Pin_6:
            return EXTI_Line6;
        case GPIO_Pin_7:
            return EXTI_Line7;
        case GPIO_Pin_8:
            return EXTI_Line8;
        case GPIO_Pin_9:
            return EXTI_Line9;
        case GPIO_Pin_10:
            return EXTI_Line10;
        case GPIO_Pin_11:
            return EXTI_Line11;
        case GPIO_Pin_12:
            return EXTI_Line12;
        case GPIO_Pin_13:
            return EXTI_Line13;
        case GPIO_Pin_14:
            return EXTI_Line14;
        case GPIO_Pin_15:
            return EXTI_Line15;
        default:
            return 0;
    }
}

uint8_t GET_EXTI_IRQn(uint32_t GPIO_Pin){
    switch(GPIO_Pin){
        case GPIO_Pin_0:
            return EXTI0_IRQn;
        case GPIO_Pin_1:
            return EXTI1_IRQn;
        case GPIO_Pin_2:
            return EXTI2_IRQn;
        case GPIO_Pin_3:
            return EXTI3_IRQn;
        case GPIO_Pin_4:
            return EXTI4_IRQn;
        case GPIO_Pin_5:
        case GPIO_Pin_6:
        case GPIO_Pin_7:
        case GPIO_Pin_8:
        case GPIO_Pin_9:
            return EXTI9_5_IRQn;
        case GPIO_Pin_10:
        case GPIO_Pin_11:
        case GPIO_Pin_12:
        case GPIO_Pin_13:
        case GPIO_Pin_14:
        case GPIO_Pin_15:
            return EXTI15_10_IRQn;
        default:
            return 0;
    }
}

uint8_t GET_PORT_SOURCE(GPIO_TypeDef* GPIOx){
    switch((uint32_t)GPIOx){
        case (uint32_t)GPIOA:
            return GPIO_PortSourceGPIOA;
        case (uint32_t)GPIOB:
            return GPIO_PortSourceGPIOB;
        case (uint32_t)GPIOC:
            return GPIO_PortSourceGPIOC;
        case (uint32_t)GPIOD:
            return GPIO_PortSourceGPIOD;
        case (uint32_t)GPIOE:
            return GPIO_PortSourceGPIOE;
        case (uint32_t)GPIOF:
            return GPIO_PortSourceGPIOF;
        case (uint32_t)GPIOG:
            return GPIO_PortSourceGPIOG;
        default:
            return 0;
    }
}

uint8_t GET_GPIO_PinSource(uint16_t GPIO_Pin){
    switch(GPIO_Pin){
        case GPIO_Pin_0:
            return GPIO_PinSource0;
        case GPIO_Pin_1:
            return GPIO_PinSource1;
        case GPIO_Pin_2:
            return GPIO_PinSource2;
        case GPIO_Pin_3:
            return GPIO_PinSource3;
        case GPIO_Pin_4:
            return GPIO_PinSource4;
        case GPIO_Pin_5:
            return GPIO_PinSource5;
        case GPIO_Pin_6:
            return GPIO_PinSource6;
        case GPIO_Pin_7:
            return GPIO_PinSource7;
        case GPIO_Pin_8:
            return GPIO_PinSource8;
        case GPIO_Pin_9:
            return GPIO_PinSource9;
        case GPIO_Pin_10:
            return GPIO_PinSource10;
        case GPIO_Pin_11:
            return GPIO_PinSource11;
        case GPIO_Pin_12:
            return GPIO_PinSource12;
        case GPIO_Pin_13:
            return GPIO_PinSource13;
        case GPIO_Pin_14:
            return GPIO_PinSource14;
        case GPIO_Pin_15:
            return GPIO_PinSource15;
        default:
            return 0;
    }
}
