#ifndef _HSR04_H_
#define _HSR04_H_

#include "system.h"
#include "GP_Tim.h"

void HSR04_Init(GPIO_TypeDef * Trig_PORT, u16 Trig_Pin, GPIO_TypeDef * Echo_PORT, u16 Echo_Pin, TIM_TypeDef * TIMx);
void HSR04_Start(void);
uint32_t HSR04_Get_Distance_cm(void);
void When_TIM_Interrupt(void);
void When_EXTI_Interrupt(void);

#endif // _HSR04_H_
