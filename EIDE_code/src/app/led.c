#include "led.h"

void LED_Init(GPIO_TypeDef * GPIOx, u16 GPIO_Pin_x){
    GPIO_RCC_ENABLE(GPIOx);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOx,&GPIO_InitStructure);
}
