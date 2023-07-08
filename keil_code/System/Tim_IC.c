#include "Tim_IC.h"
#include "OLED.h"

//输入捕获状态,第十六位表示捕获完成，第十五位表示捕获开始，后十四位表示溢出次数
u16 TIMx_CHx_CAPTURE_STA;
//输入捕获值,表示计数器计数
u16 TIMx_CHx_CAPTURE_VAL;

void TIMx_CHx_Input_Init(void){
	RCC_APB2PeriphClockCmd(GET_GPIO_RCC(TC_IC_PORT),ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//下拉输入模式
	GPIO_InitStructure.GPIO_Pin = TC_IC_Pin;
	GPIO_Init(TC_IC_PORT,&GPIO_InitStructure);
	
	RCC_APB1PeriphClockCmd(TC_RCC_APB1Periph_TIMx,ENABLE);
	TIM_TimeBaseInitTypeDef TimeBaseInitStructure;
	TIM_TimeBaseStructInit(&TimeBaseInitStructure);
	TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//滤波设置
	TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TimeBaseInitStructure.TIM_Period = TC_Input_Period;//设置为上限，计时方便
	TimeBaseInitStructure.TIM_Prescaler = 72-1;
	TIM_TimeBaseInit(TC_TIMx,&TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TC_TIM_Channel_x;
	TIM_ICInitStructure.TIM_ICFilter = 0x00;//滤波器设置,就是过滤掉外部引脚的波动信号
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//上升沿
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//就是控制几个有效信号后将CNT数据转移至CCR
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//直接映射到通道3
	TIM_ICInit(TC_TIMx,&TIM_ICInitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TC_TIMx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TC_TIMx,TC_TIM_IT_CCx | TIM_IT_Update,ENABLE);
	TIM_Cmd(TC_TIMx,ENABLE);
}

u16 GET_ICState(void){
	return (TIMx_CHx_CAPTURE_STA & 0x8000);
}

u32 GET_ICTime(void){
	u32 u;
	u = TIMx_CHx_CAPTURE_STA & 0x3fff;
	u *= TC_Input_Period;
	u += TIMx_CHx_CAPTURE_VAL;
	return u;
}

void IC_GO_AGAIN(void){
	TIMx_CHx_CAPTURE_STA = 0;
}

void TC_TIMx_IRQHandler(void)
{
	if((TIMx_CHx_CAPTURE_STA&0x8000)==0)		//还未成功捕获
	{
		if(TIM_GetITStatus(TC_TIMx,TIM_IT_Update)) //发生更新中断
		{
			if(TIMx_CHx_CAPTURE_STA&0X4000)		//捕获已经开始
			{
				if((TIMx_CHx_CAPTURE_STA&0x3fff)==0x3fff) //溢出次数溢出
				{
					TIMx_CHx_CAPTURE_STA|=0x8000; //标志为捕获成功
					TIMx_CHx_CAPTURE_VAL=TC_Input_Period;//由于已经溢出，由发生更新中断，故计数器设置为上限
				}
				else
				{
					TIMx_CHx_CAPTURE_STA++;		//溢出次数自增
				}
			}
		}
		if(TIM_GetITStatus(TC_TIMx,TC_TIM_IT_CCx)) 	//发生捕获中断
		{
			if(TIMx_CHx_CAPTURE_STA&0X4000)		//已经开始捕获
			{
				TIMx_CHx_CAPTURE_STA|=0x8000; 	//捕获成功
				TIMx_CHx_CAPTURE_VAL=TC_TIM_GetCapturex(TC_TIMx);//获取计时器计数
				OLED_ShowNum(2,1,GET_ICTime(),8);
				OLED_ShowString(2,9,"us");
				IC_GO_AGAIN();
				TC_TIM_OCxPolarityConfig(TC_TIMx,TIM_ICPolarity_Rising); //设置上升沿捕获,等待下一次捕获
			}
			else
			{
				TIMx_CHx_CAPTURE_STA=0;
				TIMx_CHx_CAPTURE_VAL=0;
				TIMx_CHx_CAPTURE_STA|=0x4000; 	//捕获开始
				TIM_Cmd(TC_TIMx,DISABLE);			//暂时关闭定时器
				TIM_SetCounter(TC_TIMx,0); 		//定时器初值为0
				TC_TIM_OCxPolarityConfig(TC_TIMx,TIM_ICPolarity_Falling); //设置下降沿捕获
				TIM_Cmd(TC_TIMx,ENABLE);			//重新开启定时器
			}
		}
	}
	TIM_ClearITPendingBit(TC_TIMx,TC_TIM_IT_CCx|TIM_IT_Update);//清除中断标志位
}
