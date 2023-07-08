#include "AD.h"

uint16_t TEM_Load[DMA_fliter];

void ADC_FORTEMP_Init(void){	//此函数为实现ADC实时检测芯片温度的功能而写
	OLED_ShowString(1,1,"CoreTem:00`C");
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADCx, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Divx);//配置时钟为12MHz
	ADC_RegularChannelConfig(ADC1,ADC_Channel_16,1,ADC_SampleTime);
	
	ADC_InitTypeDef ADC_InitSturcture;
	ADC_InitSturcture.ADC_ContinuousConvMode = ENABLE;//连续采样模式
	ADC_InitSturcture.ADC_DataAlign = ADC_DataAlign_Right;//右对齐模式
	ADC_InitSturcture.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//不需要外部触发
	ADC_InitSturcture.ADC_Mode = ADC_Mode_Independent;//工作在独立模式，不干涉ADC1的运行
	ADC_InitSturcture.ADC_NbrOfChannel = 1;//转换数量为1
	ADC_InitSturcture.ADC_ScanConvMode = DISABLE;//非扫描模式
	ADC_Init(ADCx,&ADC_InitSturcture);//ADC初始化完成
	ADC_TempSensorVrefintCmd(ENABLE);//开启内部温度传感器和参考电压
	
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMAx, ENABLE);
	
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//以外设为源
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//使用硬件触发
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//循环模式，自动传输
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low;//优先级设置
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//地址自增
	DMA_InitStructure.DMA_BufferSize = DMA_fliter;	//重复次数
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//不自增
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)TEM_Load;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//传输16位，高4位置0
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADCx->DR);
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_Init(DMAx_Channelx,&DMA_InitStructure);
	
	DMA_Cmd(DMAx_Channelx,ENABLE);//开启DMA
	
	ADC_DMACmd(ADCx,ENABLE);	//开启ADCDMA
	
	ADC_Cmd(ADCx,ENABLE);			//开启ADC
	
	ADC_ResetCalibration(ADCx);//复位校准
	while(ADC_GetResetCalibrationStatus(ADCx)==SET);//等待复位校准完成
	ADC_StartCalibration(ADCx);//开始校准
	while(ADC_GetCalibrationStatus(ADCx)==SET);//等待校准完成
	
	ADC_SoftwareStartConvCmd(ADCx,ENABLE);//ADC开始转换
}


u16 Get_ADC_Ret(u8 time)
{
	u32 temp = 0;
	u8 t;
	
	for(t = time;t;t--)
	{
		temp += ADC_GetConversionValue(ADC1);
		delay_ms(5);
	}
	return temp/time;
}

void Reload_tem(void){
	double V;uint32_t TEM_Value=0;u8 i = 0;
	for(i=0;i<DMA_fliter;i++){
		TEM_Value+=TEM_Load[i];
	}
	TEM_Value/=DMA_fliter;
	V = (double)TEM_Value*(3.3/4096);
	V = (1.43 - V)/0.0043 + 25;
	OLED_ShowNum(1,9,(uint16_t)V,2);
//	OLED_ShowNum(1,12,(uint16_t)(V * 100)%100,2);
}
