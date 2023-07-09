#include "system.h"
#include "GP_Tim.h"
#include "led.h"
#include "OLED.h"
#include "hsr04.h"

int main()
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    SysTick_Init(72);
    LED_Init(GPIOC, GPIO_Pin_13);
    OLED_Init(GPIOB,GPIO_Pin_8,GPIOB,GPIO_Pin_9);
    HSR04_Init(GPIOB, GPIO_Pin_12, GPIOB, GPIO_Pin_13, TIM2);
    while(1){
        HSR04_Start();
        delay_ms(109);
        OLED_ShowNum(1, 1, HSR04_Get_Distance_cm(), 15);
    }
    return 0;
}

void TIM2_IRQHandler(void){ 
    When_TIM_Interrupt();
}

void EXTI15_10_IRQHandler(void){
    When_EXTI_Interrupt();
}
