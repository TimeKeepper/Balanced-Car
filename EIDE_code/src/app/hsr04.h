/*
using HSR04 to measure distance
notice:
using example:
int main(){
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);           set NVIC priority group
    SysTick_Init(72);                                         init systick
    HSR04_Init(GPIOB, GPIO_Pin_12, GPIOB, GPIO_Pin_13, TIM2); Init before use
    while(1){
        HSR04_Start();                                        start a measure
        delay_ms(10);                                         wait for measure finish
        OLED_ShowNum(1, 1, HSR04_Get_Distance_cm(), 15);      you can get the distance by call this function
        LED1 = !LED1;
    }
    return 0;
}

void TIM2_IRQHandler(void){ 
    HSR_When_TIM_Interrupt();                                 call this function in TIMx_IRQHandler
}

void EXTI15_10_IRQHandler(void){
    HSR_When_EXTI_Interrupt();                                call this function in EXTI15_10_IRQHandler
}

1.after the power on, the HSR04 need times to initialize, so you need to wait for a while before you start to Init and use it.
2.make sure the GPIO_Pin you choose is correct, or it will not work.
3.make sure the TIMx you choose is only used by HSR04, or you should change the code in TIMx_IRQHandler.same as EXTI
4.Ensure that the GPIO pins are not connected in parallel to other devices
5.Trust the hardware environment, suspect the software and yourself.
6.ensure you're using STM32F103C8T6, althought it can use in GD32f103c8t6, but just some of them.if you find the code can not use in GD32, maybe it's the reason.
7.if you have any question, please contact me by email
8.the HSR04's measure is not very correct, what I get is compeletly original data, You may need to do some processing reduce the misdeclaration rate.
9.Suspect the hardware environment, trust the software and yourself.
10.the notice above is all my experience, it really waste tons of my time:(
*/


#ifndef _HSR04_H_
#define _HSR04_H_

#include "system.h"
#include "GP_Tim.h"

void HSR04_Init(GPIO_TypeDef * Trig_PORT, u16 Trig_Pin, GPIO_TypeDef * Echo_PORT, u16 Echo_Pin, TIM_TypeDef * TIMx);
void HSR04_Start(void);
uint32_t HSR04_Get_Distance_cm(void);
void HSR_When_TIM_Interrupt(void);
void HSR_When_EXTI_Interrupt(void);

#endif // _HSR04_H_
