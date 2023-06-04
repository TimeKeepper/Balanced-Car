#include "hsr04.h"

float Hsr_distant = 0;
u16 Hsr_pull_out_counter = 0;
float current_distant = 60;
float lastime_distant = 60;
u8 dostamt_load_account = 10;
float distant_average_load = 0;
void HSR04_Init()
{
    GPIO_InitTypeDef  GPIO_InitSture;
	EXTI_InitTypeDef  EXTI_InitSture;
	NVIC_InitTypeDef  NVIC_InitSture;
	
    RCC_APB2PeriphClockCmd(GET_GPIO_RCC(Trig_Port)|GET_GPIO_RCC(Echo_Port),ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	if(JTAG_Config){
	GPIO_PinRemapConfig(JTAG_Config,ENABLE);
	}
	
    /**************************GPIO结构体的初始化**********************************/
	GPIO_InitSture.GPIO_Mode=GPIO_Mode_Out_PP;   
	GPIO_InitSture.GPIO_Pin=Trig_Pin;             
	GPIO_InitSture.GPIO_Speed=GPIO_Speed_10MHz;  
	GPIO_Init(Trig_Port,&GPIO_InitSture);
	
	GPIO_InitSture.GPIO_Mode=GPIO_Mode_IN_FLOATING;    
	GPIO_InitSture.GPIO_Pin=Echo_Pin;               
	GPIO_Init(Echo_Port,&GPIO_InitSture);

    /**************************EXTI外部中断结构体的初始化***************************/
	GPIO_EXTILineConfig(Echo_GPIO_PortSource,Echo_GPIO_PinSource);
	EXTI_InitSture.EXTI_Line=HSR_EXTI_Line;
	EXTI_InitSture.EXTI_LineCmd=ENABLE;
	EXTI_InitSture.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitSture.EXTI_Trigger=EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_InitSture);

    /**************************NVIC中断结构体的初始化*******************************/
	NVIC_InitSture.NVIC_IRQChannel=HSR_EXTI_IRQn;
	NVIC_InitSture.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitSture.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitSture.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&NVIC_InitSture);
}

/**
 * 函数名：HSR04_Init()
 * 功能：驱动Trig发送超声波
 */
void HSR04_Trig_Open()
{
    Trig = 1;
    delay_us(20);
    Trig = 0;
}


//void HSR_EXTI_IRQHandler()
//{
//    if(EXTI_GetITStatus(HSR_EXTI_Line) != RESET)
//    {
//		u32 current_distance;
//        SysTick->LOAD=0XFFFFFF;
//		SysTick->VAL=0x000;
//        SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
//        while(Echo);
//        
//        SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
//        current_distance=(0XFFFFFF - (SysTick->VAL))*34/27000;
//		SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
//		SysTick->VAL =0X00;
//		Hsr_distant = current_distance;
//    }
//    EXTI_ClearITPendingBit(HSR_EXTI_Line);
//}
