#include "Beep.h"

void BEEP_PWM_Init(void){
	RCC_APB1PeriphClockCmd(BEEP_PWM_RCC_APB1Periph_TIMx,ENABLE);
	
	RCC_APB2PeriphClockCmd(BEEP_PWM_RCC_APB2Periph_GPIOx,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = BEEP_PWM_GPIO_Pin_x;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BEEP_PWM_PORT,&GPIO_InitStructure);
	
	if(BEEP_PWM_PORT_REMAP){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
		GPIO_PinRemapConfig(BEEP_PWM_PORT_REMAP,ENABLE);
	}
	
	TIM_InternalClockConfig(BEEP_PWM_TIMx);//配置为内部时钟源
	TIM_TimeBaseInitTypeDef TimeBaseInitStructure;
	TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//滤波设置
	TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TimeBaseInitStructure.TIM_Period = BEEP_PWM_ARR_Default-1;//arr值，控制PWM输出的分辨率
	TimeBaseInitStructure.TIM_Prescaler = BEEP_PWM_PSC_Default-1;//预分频系数
	TimeBaseInitStructure.TIM_RepetitionCounter = 0;//高级定时器设置，不用管
	TIM_TimeBaseInit(BEEP_PWM_TIMx,&TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);//将所有成员赋一个初始值
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;//冻结模式，需要用函数控制输出
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//输出极性
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//输出使能
	TIM_OCInitStructure.TIM_Pulse = BEEP_PWM_DEFAULT_LEAVEL;//捕获/比较值,默认设置为0，也就是初始音量为0
	BEEP_PWM_TIM_OCxInit(BEEP_PWM_TIMx,&TIM_OCInitStructure);
	
	TIM_Cmd(BEEP_PWM_TIMx,ENABLE);
}

void Set_BEEP_frequency(u32 BEEP_frequency){//15~1000000
	u16 i = 72000000/BEEP_PWM_ARR_Default/BEEP_frequency;
	TIM_PrescalerConfig(BEEP_PWM_TIMx,i,TIM_PSCReloadMode_Immediate);
}

void Set_BEEP_volume(u8 BEEP_volume){		//0~100
	u16 i = BEEP_PWM_ARR_Default*BEEP_volume/100;
	BEEP_TIM_SetComparex(BEEP_PWM_TIMx,i);
}

void BEEP_ON(void){
	TIM_SelectOCxM(BEEP_PWM_TIMx,BEEP_PWM_TIM_OCChannel,TIM_OCMode_PWM1);
	TIM_CCxCmd(BEEP_PWM_TIMx,BEEP_PWM_TIM_OCChannel,TIM_CCx_Enable);
}

void BEEP_STOP(void){
	TIM_SelectOCxM(BEEP_PWM_TIMx,BEEP_PWM_TIM_OCChannel,TIM_OCMode_Timing);//改为冻结状态，PWM输出停止
	TIM_CCxCmd(BEEP_PWM_TIMx,BEEP_PWM_TIM_OCChannel,TIM_CCx_Enable);
}

void BEEP_KEEP_OFF(void){
	TIM_SelectOCxM(BEEP_PWM_TIMx,BEEP_PWM_TIM_OCChannel,TIM_OCMode_Inactive);
	TIM_CCxCmd(BEEP_PWM_TIMx,BEEP_PWM_TIM_OCChannel,TIM_CCx_Enable);
}

void BEEP_KEEP_ON(void){
	TIM_SelectOCxM(BEEP_PWM_TIMx,BEEP_PWM_TIM_OCChannel,TIM_OCMode_Active);
	TIM_CCxCmd(BEEP_PWM_TIMx,BEEP_PWM_TIM_OCChannel,TIM_CCx_Enable);
}

void BEEP_MUSIC(int* spectrum,float* rhythm,u8 size){//spectrum即频谱，rhythm即节拍
	u16 i;
	for (i = 0; i < size; i++){
		Set_BEEP_frequency(spectrum[i]);
		BEEP_ON();
		delay_ms(rhythm[i]*500);
		BEEP_STOP();
	}
}
