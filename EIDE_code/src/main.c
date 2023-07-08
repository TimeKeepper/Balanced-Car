#include "system.h"
#include "GP_Tim.h"
#include "led.h"
#include "OLED.h"

static int TIMIT_Count = 0;

int main()
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    SysTick_Init(72);
    LED_Init(GPIOC, GPIO_Pin_13);
    OLED_Init(GPIOB, GPIO_Pin_8, GPIOB, GPIO_Pin_9);
    GP_Timer_IT_Init(TIM3);
    while(1){
        if(TIMIT_Count >= 100){
            TIMIT_Count = 0;
            LED1 = !LED1;
        }
    }
    return 0;
}

void TIM3_IRQHandler(void){
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        TIMIT_Count++;
    }
}
