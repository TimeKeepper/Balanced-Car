#include "led.h"

void LED_Init(u16 T_LEDx)
{
	u8 i;
	GPIO_InitTypeDef GPIO_InitStructure;				//初始化端口状态变量
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//端口状态设置
	for(i = 0;i < 16;i++)
	{
		if(T_LEDx & (1<<i))
		{
			switch(i)
			{
				case 0:		GPIO_InitStructure.GPIO_Pin = LEDS1_PIN;
								//RCC即reset and clock control复位与时钟控制器
								RCC_APB2PeriphClockCmd(GET_GPIO_RCC(LEDS1_PORT),ENABLE);//端口时钟使能
								GPIO_Init(LEDS1_PORT,&GPIO_InitStructure);	 //GPIO端口初始化
								GPIO_SetBits(LEDS1_PORT,LEDS1_PIN);			 //端口默认设置高电平
								break;
				case 1: 	GPIO_InitStructure.GPIO_Pin = LEDS2_PIN;
								RCC_APB2PeriphClockCmd(GET_GPIO_RCC(LEDS2_PORT),ENABLE);//端口时钟使能
								GPIO_Init(LEDS2_PORT,&GPIO_InitStructure);	 //GPIO端口初始化
								GPIO_SetBits(LEDS2_PORT,LEDS2_PIN);			 //端口默认设置高电平
								break;
				case 2: 	GPIO_InitStructure.GPIO_Pin = LED1_PIN;
								RCC_APB2PeriphClockCmd(GET_GPIO_RCC(LED1_PORT),ENABLE);
								GPIO_Init(LED1_PORT,&GPIO_InitStructure);
								GPIO_SetBits(LED1_PORT,LED1_PIN);
								break;
				case 3: 	GPIO_InitStructure.GPIO_Pin = LED2_PIN;
								RCC_APB2PeriphClockCmd(GET_GPIO_RCC(LED2_PORT),ENABLE);
								GPIO_Init(LED2_PORT,&GPIO_InitStructure);
								GPIO_SetBits(LED2_PORT,LED2_PIN);
								break;
				case 4: 	GPIO_InitStructure.GPIO_Pin = LED3_PIN;
								RCC_APB2PeriphClockCmd(GET_GPIO_RCC(LED3_PORT),ENABLE);
								GPIO_Init(LED3_PORT,&GPIO_InitStructure);
								GPIO_SetBits(LED3_PORT,LED3_PIN);
								break;
				case 5: 	GPIO_InitStructure.GPIO_Pin = LED4_PIN;
								RCC_APB2PeriphClockCmd(GET_GPIO_RCC(LED4_PORT),ENABLE);
								GPIO_Init(LED4_PORT,&GPIO_InitStructure);
								GPIO_SetBits(LED4_PORT,LED4_PIN);
								break;
				case 6: 	GPIO_InitStructure.GPIO_Pin = LED5_PIN;
								RCC_APB2PeriphClockCmd(GET_GPIO_RCC(LED5_PORT),ENABLE);
								GPIO_Init(LED5_PORT,&GPIO_InitStructure);
								GPIO_SetBits(LED5_PORT,LED5_PIN);
								break;
				case 7: 	GPIO_InitStructure.GPIO_Pin = LED6_PIN;
								RCC_APB2PeriphClockCmd(GET_GPIO_RCC(LED6_PORT),ENABLE);
								GPIO_Init(LED6_PORT,&GPIO_InitStructure);
								GPIO_SetBits(LED6_PORT,LED6_PIN);
								break;
				case 8: 	GPIO_InitStructure.GPIO_Pin = LED7_PIN;
								RCC_APB2PeriphClockCmd(GET_GPIO_RCC(LED7_PORT),ENABLE);
								GPIO_Init(LED7_PORT,&GPIO_InitStructure);
								GPIO_SetBits(LED7_PORT,LED7_PIN);
								break;
				case 9: 	GPIO_InitStructure.GPIO_Pin = LED8_PIN;
								RCC_APB2PeriphClockCmd(GET_GPIO_RCC(LED8_PORT),ENABLE);
								GPIO_Init(LED8_PORT,&GPIO_InitStructure);
								GPIO_SetBits(LED8_PORT,LED8_PIN);
								break;
				default:		break;
			}
		}
	}
}

void LEDMarquee_Init(u8 uwi)
{
	u8 i;
	for(i = 0;i<8;i++)
	{
		if(uwi & (u8)(1<<i))
		{
			switch(i+1)
			{
				case 1:LED_Init(T_LED1);break;
				case 2:LED_Init(T_LED2);break;
				case 3:LED_Init(T_LED3);break;
				case 4:LED_Init(T_LED4);break;
				case 5:LED_Init(T_LED5);break;
				case 6:LED_Init(T_LED6);break;
				case 7:LED_Init(T_LED7);break;
				case 8:LED_Init(T_LED8);break;
				default:				break;
			}
		}
	}
}

void LEDMarquee_Config(u8 LEDx_ON)
{
	u8 i;
	for(i = 0;i<8;i++)
	{
		if(LEDx_ON & (u8)(1 << i))
		{
			switch(i+1)
			{
				case 1:LED1 = 0;break;
				case 2:LED2 = 0;break;
				case 3:LED3 = 0;break;
				case 4:LED4 = 0;break;
				case 5:LED5 = 0;break;
				case 6:LED6 = 0;break;
				case 7:LED7 = 0;break;
				case 8:LED8 = 0;break;
				default:		break;
			}
		}
		else
		{
			switch(i+1)
			{
				case 1:LED1 = 1;break;
				case 2:LED2 = 1;break;
				case 3:LED3 = 1;break;
				case 4:LED4 = 1;break;
				case 5:LED5 = 1;break;
				case 6:LED6 = 1;break;
				case 7:LED7 = 1;break;
				case 8:LED8 = 1;break;
				default:		break;
			}
		}
	}
}
