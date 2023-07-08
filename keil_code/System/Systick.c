#include "Systick.h"

static u8 fac_us = 0;

//一般而言，我们不会对以SYSCLK为源的HCLK分频处理，所以这里我直接就将SYSCLK和HCLK画上等号了
void Systick_Init(u8 SYSCLK){	//传入参数为系统频率，默认是72(MHz),当然是可以改变的
	//你想不分频也可以，但是72MHz实在太快了，到时候计时器计满了都计不够一秒（悲）
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//配置时钟源(HCLK八分频)，实际上systick也只能由HCLK提供源
	fac_us = SYSCLK / 8;									//定时1微妙所需次数
	//实际上systick计数完成之后也会产生一个中断信号，想要的话也可以配置对应的中断函数
}

void delay_us(u32 nus){		
	if(nus != 0){
		u32 temp;	    	 
		SysTick->LOAD=nus*fac_us; 					//时间加载	  		 
		SysTick->VAL=0x000;        					//清空计数器
		SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数，别看这个SysTick_CTRL_ENABLE_Msk那么复杂，其实就是对CTRL的第一位置1来使能systick
		do{
			temp=SysTick->CTRL;
		}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达,CTRL的第16位是SYSTICK是否计到过0，读取后自动清零
		SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
		SysTick->VAL =0X00; 						//清空计数器
	}		 
}

void delay_ms(u32 nms){	 
	while(nms--){
		delay_us(1000);
	}
} 

void delay_s(u32 ns){
	while(ns--){
		delay_ms(1000);
	}
}
