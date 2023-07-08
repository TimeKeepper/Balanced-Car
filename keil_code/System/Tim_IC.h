#ifndef _TIM_IC_
#define _TIM_IC_

#include "system.h"

//只能修改为通用定时器！！！
#define TC_TIMx 					 	TIM4
#define TC_TIMx_IRQn 					TIM4_IRQn
#define TC_TIMx_IRQHandler				TIM4_IRQHandler
#define TC_RCC_APB1Periph_TIMx 			RCC_APB1Periph_TIM4
#define TC_TIM_Channel_x   				TIM_Channel_1
#define TC_TIM_IT_CCx 					TIM_IT_CC1
#define TC_TIM_OCxPolarityConfig		TIM_OC1PolarityConfig
#define TC_TIM_GetCapturex				TIM_GetCapture1
#define TC_IC_PORT 						GPIOB
#define TC_IC_Pin  						GPIO_Pin_6
#define TC_Input_Period 				0xffff

extern u16 TIMx_CHx_CAPTURE_STA; //输入捕获的状态
extern u16 TIMx_CHx_CAPTURE_VAL;//输入捕获值
void TIMx_CHx_Input_Init(void);
u16 GET_ICState(void);			//获取捕获状态
u32 GET_ICTime(void);			//获取捕获时间
void IC_GO_AGAIN(void);			//再次捕获

#endif
