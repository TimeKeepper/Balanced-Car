#ifndef _LED_H_
#define _LED_H_

#include "system.h"	

//因为我使用了递减for循环来简化代码，所以请移植时务必将目标LED宏定义为以全部LED为最高的连续整数
#define T_LEDS1				((u16)(1<<0))
#define T_LEDS2				((u16)(1<<1))
#define T_LED1				((u16)(1<<2))
#define T_LED2				((u16)(1<<3))
#define T_LED3				((u16)(1<<4))
#define T_LED4				((u16)(1<<5))
#define T_LED5				((u16)(1<<6))
#define T_LED6				((u16)(1<<7))
#define T_LED7				((u16)(1<<8))
#define T_LED8				((u16)(1<<9))
#define T_ALLLED			((u16)(0xffff))

#define LEDS1_PORT     		GPIOC						//LED端口
#define LEDS1_PIN     		GPIO_Pin_13					//LED引脚
#define LEDS1	PCout(13)

#define LEDS2_PORT			GPIOE
#define LEDS2_PIN 			GPIO_Pin_5
#define LEDS2	PEout(5)

#define LED1_PORT			GPIOE
#define LED1_PIN 			GPIO_Pin_0
#define LED1	PEout(0)

#define LED2_PORT			GPIOE
#define LED2_PIN 			GPIO_Pin_2
#define LED2	PEout(2)

#define LED3_PORT			GPIOE
#define LED3_PIN 			GPIO_Pin_4
#define LED3	PEout(4)

#define LED4_PORT			GPIOE
#define LED4_PIN 			GPIO_Pin_6
#define LED4	PEout(6)

#define LED5_PORT			GPIOB
#define LED5_PIN 			GPIO_Pin_9
#define LED5	PBout(9)

#define LED6_PORT			GPIOE
#define LED6_PIN 			GPIO_Pin_1
#define LED6	PEout(1)

#define LED7_PORT			GPIOE
#define LED7_PIN 			GPIO_Pin_3
#define LED7	PEout(3)

#define LED8_PORT			GPIOC
#define LED8_PIN 			GPIO_Pin_13
#define LED8	PCout(13)

#define LED1_ON				((u8)1<<0)
#define LED2_ON				((u8)1<<1)
#define LED3_ON				((u8)1<<2)
#define LED4_ON				((u8)1<<3)
#define LED5_ON				((u8)1<<4)
#define LED6_ON				((u8)1<<5)
#define LED7_ON				((u8)1<<6)
#define LED8_ON				((u8)1<<7)

//我不知道存不存在那种另一端接VSS的吊诡LED，如果有的话注意初始化时默认设置电平改为低电平


void LED_Init(u16 T_LEDx);
void LEDMarquee_Init(u8 uwi);
void LEDMarquee_Config(u8 LEDx_ON);

#endif

