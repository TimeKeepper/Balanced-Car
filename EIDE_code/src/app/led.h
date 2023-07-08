#ifndef _LED_H_
#define _LED_H_

#include "system.h"

#define LED1    PCout(13)
#define LED2    
#define LED3
#define LED4
//You can freely add any pins you want as new LEDs, and then you can control their on/off directly through this macro definition after initialization is complete

void LED_Init(GPIO_TypeDef * GPIOx, u16 GPIO_Pin_x);
//The variable GPIO_Pin_x can be a combination of any pins, so you can initialize multiple pins at once, provided they are in the same GPIO group
#endif
