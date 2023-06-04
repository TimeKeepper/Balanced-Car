#include "Timer.h"

void GP_Timer_IT_Init(void){
	RCC_APB2PeriphClockCmd(GP_IT_RCC_APB1Periph_TIMx,ENABLE);
	
	TIM_InternalClockConfig(GP_IT_TIMx);//选择内部时钟源，如果不写这一步的话默认选择内部时钟源
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;	//用于滤波，电平判断频率由时钟提供，此处配置的分频系数即判断次数，越高则滤波效果越好
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 72-1;		//此处应设置一个0~65535的整数，也就是ARR的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 20-1;			//预分频系数
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 11-1;//重复计数次数，高级计数器特有，此处配置0就好
	TIM_TimeBaseInit(GP_IT_TIMx,&TIM_TimeBaseInitStructure);
}

void PWM_Timer_Init(void){
	RCC_APB1PeriphClockCmd(PWM_RCC_APB1Periph_TIMx,ENABLE);
	
	RCC_APB2PeriphClockCmd(PWM_RCC_APB2Periph_GPIOx,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = PWM_GPIO_Pin_x;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PWM_PORT,&GPIO_InitStructure);
	
	if(PWM_PORT_REMAP){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
		GPIO_PinRemapConfig(PWM_PORT_REMAP,ENABLE);
	}
	
	TIM_InternalClockConfig(PWM_TIMx);//配置为内部时钟源
	TIM_TimeBaseInitTypeDef TimeBaseInitStructure;
	TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//滤波设置
	TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TimeBaseInitStructure.TIM_Period = PWM_ARR_Default-1;//arr值，控制PWM输出的分辨率
	TimeBaseInitStructure.TIM_Prescaler = PWM_PSC_Default-1;//预分频系数
	TimeBaseInitStructure.TIM_RepetitionCounter = 0;//高级定时器设置，不用管
	TIM_TimeBaseInit(PWM_TIMx,&TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);//将所有成员赋一个初始值
	TIM_OCInitStructure.TIM_OCMode = PWM_Mode_Default;//可在宏定义中修改初始化后默认状态
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//输出极性
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//输出使能
	TIM_OCInitStructure.TIM_Pulse = PWM_DEFAULT_LEAVEL;//捕获/比较值,默认设置为0，也就是不修改该值直接开启PWM则为低电平
	PWM_TIM_OCxInit(PWM_TIMx,&TIM_OCInitStructure);
	
	TIM_Cmd(PWM_TIMx,ENABLE);
}

void PWM_Set_frequency(u32 PWM_frequency){//15~720000
	u16 i = 72000000/PWM_ARR_Default/PWM_frequency;
	TIM_PrescalerConfig(PWM_TIMx,i,TIM_PSCReloadMode_Immediate);
}

void PWM_Set_Compare(u8 PWM_Compare){		//0~100
	u16 i = PWM_ARR_Default*PWM_Compare/100;
	TIM_SetComparex(PWM_TIMx,i);
}

void PWM_ON(void){
	TIM_SelectOCxM(PWM_TIMx,PWM_TIM_OCChannel,TIM_OCMode_PWM1);
	TIM_CCxCmd(PWM_TIMx,PWM_TIM_OCChannel,TIM_CCx_Enable);
}

void PWM_STOP(void){
	TIM_SelectOCxM(PWM_TIMx,PWM_TIM_OCChannel,TIM_OCMode_Timing);//改为冻结状态，PWM输出停止
	TIM_CCxCmd(PWM_TIMx,PWM_TIM_OCChannel,TIM_CCx_Enable);
}

void PWM_KEEP_OFF(void){
	TIM_SelectOCxM(PWM_TIMx,PWM_TIM_OCChannel,TIM_OCMode_Inactive);
	TIM_CCxCmd(PWM_TIMx,PWM_TIM_OCChannel,TIM_CCx_Enable);
}

void PWM_KEEP_ON(void){
	TIM_SelectOCxM(PWM_TIMx,PWM_TIM_OCChannel,TIM_OCMode_Active);
	TIM_CCxCmd(PWM_TIMx,PWM_TIM_OCChannel,TIM_CCx_Enable);
}

void GP_IT_TIMx_IRQHandler(void){
	if(TIM_GetFlagStatus(GP_IT_TIMx,TIM_FLAG_Update) == SET){//判断标志位
		TIM_ClearFlag(GP_IT_TIMx,TIM_FLAG_Update);//根据手册，这里需要软件手动清零
	}
}
