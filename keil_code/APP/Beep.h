#ifndef _BEEP_H_
#define _BEEP_H_

#include "system.h"

#define BEEP_PWM_DEFAULT_LEAVEL 0 //定义PWM输出的默认电平，若定义为高电平则修改为arr,定义为低电平则修改为0
#define BEEP_PWM_ARR_Default 72
#define BEEP_PWM_PSC_Default 100	 //默认状态下是10KHz
#define BEEP_PWM_TIMx 					TIM2
#define BEEP_PWM_TIMx_IRQn				TIM2_IRQn
#define BEEP_PWM_TIMx_IRQHandler		TIM2_IRQHandler
#define BEEP_PWM_RCC_APB1Periph_TIMx 	RCC_APB1Periph_TIM2
#define BEEP_PWM_TIM_OCxInit			TIM_OC4Init
#define BEEP_PWM_TIM_OCChannel			TIM_Channel_4
#define BEEP_TIM_SetComparex			TIM_SetCompare4
#define BEEP_PWM_PORT_REMAP				GPIO_FullRemap_TIM2		//如果不需要重映射设置为0
#define BEEP_PWM_PORT					GPIOB
#define BEEP_PWM_GPIO_Pin_x				GPIO_Pin_11
#define BEEP_PWM_RCC_APB2Periph_GPIOx	RCC_APB2Periph_GPIOB

void BEEP_PWM_Init(void);
void Set_BEEP_frequency(u32 BEEP_frequency);
void Set_BEEP_volume(u8 BEEP_volume);
void BEEP_ON(void);
void BEEP_STOP(void);
void BEEP_KEEP_OFF(void);
void BEEP_KEEP_ON(void);
void BEEP_MUSIC(int* spectrum,float* rhythm,u8 size);
//BEEP_MUSIC(music1,music1_delay,sizeof(music1) / sizeof(music1[0])); 使用模板

//一下内容是关于频率与音调的宏定义，不需要修改
/********C调*********/
#define L1 262
#define L2 294
#define L3 330
#define L4 349   //低
#define L5 392
#define L6 440
#define L7 494

#define M1 523
#define M2 587
#define M3 659
#define M4 698    //中
#define M5 784
#define M6 880
#define M7 988

#define H1 1046
#define H2 1109
#define H3 1318
#define H4 1397   //高
#define H5 1568
#define H6 1760
#define H7 1976

#define none 0    //空
  
//*****蜜雪冰城*****//
static int music1[] = {
    L5, M1, M2,
    M3, M5, M5, M6, M5, M3, M2, M1, M1, M2,
    M3, M3, M2, M1, M2, none, M2, M1,
    M3, M5, M5, M6, M5, M3, M2, M1, M1, M2,
    M3, M3, M2, M2, M1, none,
    M4, M4, M5, M6, M6,
    M5, M5, M3, M1, M2, none, M1, M2,
    M3, M5, M5, M6, M5, M3, M2, M1, M1, M2,
    M3, M3, M2, M2, M1, M1
};

//节拍

static float music1_delay[] = {
    0.5, 0.25, 0.25,
    0.5, 0.5, 0.75, 0.25, 0.5, 0.25, 0.25, 0.5, 0.25, 0.25,
    0.5, 0.5, 0.5, 0.5, 1, 0.5, 0.25, 0.25,
    0.5, 0.5, 0.75, 0.25, 0.5, 0.25, 0.25, 0.5, 0.25, 0.25,
    0.5, 0.5, 0.5, 0.5, 1, 1,
    1, 0.5, 0.5, 1, 1,
    0.5, 0.5, 0.5, 0.5, 1, 0.5, 0.25, 0.25,
    0.5, 0.5, 0.5, 0.5, 0.5, 0.25, 0.25, 0.5, 0.25, 0.25,
    0.5, 0.5, 0.5, 0.25, 0.25, 1
};

//*****小蜜蜂*****//
//频谱
static int music2[] = {
    M5, M3, M3, none,
    M4, M2, M2, none,
    M1, M2, M3, M4,
    M5, M5, M5, none,
    M5, M3, M3, M3,
    M4, M2, M2, M2,
    M1, M3, M5, M5,
    M3, M3, M1, none,
    M2, M2, M2, M2,
    M2, M3, M4, none,
    M3, M3, M3, M3,
    M3, M4, M5, none,
    M5, M3, M3, M3,
    M4, M2, M2, M2,
    M1, M3, M5, M5,
    M3, M3, M1, none
};
/*由于是四四拍，所以节拍简单*/
static float music2_delay[] = {
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1
};
  
#endif
