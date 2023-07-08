#include "Motor.h"

u32 sign = 0;

void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(GET_GPIO_RCC(AIN_PORT), ENABLE);
	RCC_APB2PeriphClockCmd(GET_GPIO_RCC(BIN_PORT), ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = AIN1_Pin | AIN2_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(AIN_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = BIN1_Pin | BIN2_Pin;
	GPIO_Init(BIN_PORT, &GPIO_InitStructure);
	
	RCC_APB1PeriphClockCmd(MOTOR_PWM_TIM_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(GET_GPIO_RCC(MOTOR_PWMA_PORT), ENABLE);
	RCC_APB2PeriphClockCmd(GET_GPIO_RCC(MOTOR_PWMB_PORT), ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = MOTOR_PWMA_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MOTOR_PWMA_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = MOTOR_PWMB_Pin;
	GPIO_Init(MOTOR_PWMB_PORT, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(MOTOR_PWM_TIM);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = MOTOR_PWM_ARR_Default - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = MOTOR_PWM_PSC_Default - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(MOTOR_PWM_TIM, &TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(MOTOR_PWM_TIM,TIM_FLAG_Update);
	TIM_ITConfig(MOTOR_PWM_TIM,TIM_IT_Update,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR
	MOTOR_PWM_TIM_OCx1Init(MOTOR_PWM_TIM, &TIM_OCInitStructure);
	MOTOR_PWM_TIM_OCx2Init(MOTOR_PWM_TIM, &TIM_OCInitStructure);
	
	TIM_Cmd(MOTOR_PWM_TIM, ENABLE);
}

void TIM2_IRQHandler(void){
	Hsr_pull_out_counter++;
	sign++;
	TIM_ClearFlag(MOTOR_PWM_TIM,TIM_FLAG_Update);
}

void Motor_SetSpeed2(int Speed)
{
	if (Speed >= 0)
	{
		AIN1 = 0;
		AIN2 = 1;
		MOTOR_TIM_SetComparex2(MOTOR_PWM_TIM, Speed);
	}
	else
	{
		AIN1 = 1;
		AIN2 = 0;
		MOTOR_TIM_SetComparex2(MOTOR_PWM_TIM, -Speed);
	}
}

void Motor_SetSpeed1(int Speed)
{
	if (Speed >= 0)
	{
		BIN1 = 0;
		BIN2 = 1;
		MOTOR_TIM_SetComparex1(MOTOR_PWM_TIM, Speed);
	}
	else
	{
		BIN1 = 1;
		BIN2 = 0;
		MOTOR_TIM_SetComparex1(MOTOR_PWM_TIM, -Speed);
	}
}

void Encoder_Init(void)
{
	RCC_APB1PeriphClockCmd(MOTOR_Counter1_TIM_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(MOTOR_Counter1_GPIO_RCC, ENABLE);
	RCC_APB1PeriphClockCmd(MOTOR_Counter2_TIM_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(MOTOR_Counter2_GPIO_RCC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = MOTOR_Counter1_GPIO_Pin1 | MOTOR_Counter1_GPIO_Pin2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MOTOR_Counter1_GPIO_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = MOTOR_Counter2_GPIO_Pin1 | MOTOR_Counter2_GPIO_Pin2;
	GPIO_Init(MOTOR_Counter2_GPIO_PORT, &GPIO_InitStructure);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(MOTOR_Counter1_TIM, &TIM_TimeBaseInitStructure);
	TIM_TimeBaseInit(MOTOR_Counter2_TIM, &TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(MOTOR_Counter1_TIM, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(MOTOR_Counter1_TIM, &TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(MOTOR_Counter2_TIM, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(MOTOR_Counter2_TIM, &TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(MOTOR_Counter1_TIM, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_EncoderInterfaceConfig(MOTOR_Counter2_TIM, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	TIM_SetCounter(MOTOR_Counter1_TIM, MOTOR_Counter_Default);
	TIM_SetCounter(MOTOR_Counter2_TIM, MOTOR_Counter_Default);
	
	TIM_Cmd(MOTOR_Counter1_TIM, ENABLE);
	TIM_Cmd(MOTOR_Counter2_TIM, ENABLE);
}

int Encoder1_Get(void)
{
	int Temp;
	Temp = TIM_GetCounter(MOTOR_Counter1_TIM) - MOTOR_Counter_Default;
	TIM_SetCounter(MOTOR_Counter1_TIM, MOTOR_Counter_Default);
	return Temp;
}

int Encoder2_Get(void)
{
	int Temp;
	Temp = TIM_GetCounter(MOTOR_Counter2_TIM) - MOTOR_Counter_Default;
	TIM_SetCounter(MOTOR_Counter2_TIM, MOTOR_Counter_Default);
	return Temp;
}
