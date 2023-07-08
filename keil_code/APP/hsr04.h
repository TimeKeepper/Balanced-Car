#ifndef _HSR04_H_
#define _HSR04_H_

#include "system.h"
#include "OLED.h"

extern float Hsr_distant;
extern u16 Hsr_pull_out_counter;
extern float current_distant;
extern float lastime_distant;
extern u8 dostamt_load_account;
extern float distant_average_load;

#define  Trig_Port       GPIOB
#define  Trig_Pin        GPIO_Pin_12
#define  Trig			 PBout(12)

#define  Echo_Port       GPIOB
#define  Echo_Pin        GPIO_Pin_13
#define  Echo 			 PBin(13)
#define  Echo_GPIO_PortSource	GPIO_PortSourceGPIOB
#define  Echo_GPIO_PinSource	GPIO_PinSource13

#define  HSR_EXTI_Line	 EXTI_Line13
#define  HSR_EXTI_IRQn   EXTI15_10_IRQn
#define  HSR_EXTI_IRQHandler EXTI15_10_IRQHandler
//如果不需要关闭JTAG，将下面的宏定义定义为0
#define JTAG_Config		GPIO_Remap_SWJ_JTAGDisable  //GPIO_Remap_SWJ_JTAGDisable

extern int Distance;

void HSR04_Init(void);
void HSR04_Trig_Open(void);

#endif
