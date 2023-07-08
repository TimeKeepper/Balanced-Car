#ifndef _GP_TIM_H_
#define _GP_TIM_H_

#include "system.h"

//General purpose timer's interaption macro definitions
#define GP_IT_TIMx_PRESCALER            72
#define GP_IT_TIMx_PERIOD               10000

//In my opinion this function should only receive TIM2~TIM7,but you can try TIM1 and TIM8 if you like,I don't know if it can run correctly anyway.
void GP_Timer_IT_Init(TIM_TypeDef* GP_IT_TIMx);

#endif
