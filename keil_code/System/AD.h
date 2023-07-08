#ifndef _AD_H_
#define _AD_H_

#include "system.h"
#include "OLED.h"

#define ADCx 					ADC1
#define RCC_APB2Periph_ADCx 	RCC_APB2Periph_ADC1
#define ADC_SampleTime			ADC_SampleTime_239Cycles5
#define RCC_PCLK2_Divx 			RCC_PCLK2_Div6
#define RCC_AHBPeriph_DMAx 		RCC_AHBPeriph_DMA1
#define DMAx_Channelx			DMA1_Channel1
#define DMA_fliter				20				//该值越大越准确稳定，越小越灵敏节省空间

extern uint16_t TEM_Load[DMA_fliter];

void ADC_FORTEMP_Init(void);
u16 Get_ADC_Ret(u8 time);
int GET_TEMP(void);
void Reload_tem(void);

#endif
