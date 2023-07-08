#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "system.h"
#include "hsr04.h"

//以下是PWM输出各个默认值
#define MOTOR_Counter_Default	30000
#define MOTOR_PWM_DEFAULT_LEAVEL 0 //定义PWM输出的默认电平，若定义为高电平则修改为arr,定义为低电平则修改为0
#define MOTOR_PWM_ARR_Default 7200-1	 //默认ARR的值，1000是因为合适
#define MOTOR_PWM_PSC_Default 11-1	 	//默认状态下是9KHz,为了提高信号分辨率，这个数值最好不要设置太大
#define MOTOR_PWM_Mode_Default TIM_OCMode_Inactive //初始化后默认状态为无效电平

//是否复用
#define MOTOR_PWM_GPIO_REMAP 	0
//PWM通道宏定义
#define MOTOR_PWM_TIM		TIM2
#define MOTOR_PWM_TIM_RCC	RCC_APB1Periph_TIM2
#define MOTOR_PWMA_PORT 	GPIOA
#define MOTOR_PWMA_Pin 		GPIO_Pin_1
#define MOTOR_PWMB_PORT 	GPIOA
#define MOTOR_PWMB_Pin 		GPIO_Pin_0
//IN1，2通道宏定义
#define AIN_PORT 	GPIOB
#define AIN1_Pin 	GPIO_Pin_1
#define AIN2_Pin 	GPIO_Pin_0
#define AIN1 		PBout(1)
#define AIN2 		PBout(0)
#define BIN_PORT 	GPIOB
#define BIN1_Pin 	GPIO_Pin_10
#define BIN2_Pin 	GPIO_Pin_11
#define BIN1 		PBout(10)
#define BIN2 		PBout(11)
//输出比较通道宏定义
#define MOTOR_PWM_TIM_OCx1Init	TIM_OC1Init
#define MOTOR_PWM_TIM_OCx2Init	TIM_OC2Init
//其他宏定义
#define MOTOR_TIM_SetComparex1	TIM_SetCompare1
#define MOTOR_TIM_SetComparex2	TIM_SetCompare2

//下面是编码器方面的宏定义
#define MOTOR_Counter1_GPIO_PORT	GPIOA
#define MOTOR_Counter1_TIM			TIM3
#define MOTOR_Counter1_TIM_RCC 		RCC_APB1Periph_TIM3
#define MOTOR_Counter1_GPIO_RCC		RCC_APB2Periph_GPIOA
#define	MOTOR_Counter1_GPIO_Pin1	GPIO_Pin_6
#define	MOTOR_Counter1_GPIO_Pin2	GPIO_Pin_7

#define MOTOR_Counter2_GPIO_PORT	GPIOB
#define MOTOR_Counter2_TIM			TIM4
#define MOTOR_Counter2_TIM_RCC 		RCC_APB1Periph_TIM4
#define MOTOR_Counter2_GPIO_RCC		RCC_APB2Periph_GPIOB
#define	MOTOR_Counter2_GPIO_Pin1	GPIO_Pin_6
#define	MOTOR_Counter2_GPIO_Pin2	GPIO_Pin_7

extern u32 sign;//全局阻塞标志位

void Motor_Init(void);
void Motor_SetSpeed1(int Speed);
void Motor_SetSpeed2(int Speed);
void Encoder_Init(void);
int Encoder1_Get(void);
int Encoder2_Get(void);

#endif
