#include "hwyk.h"



u32 gGUA_InfraredReceiver_Data = 0;
static void GUA_Infrared_Receiver_IO_Init(void);
static u16 GUA_Infrared_Receiver_GetHighLevelTime(void);
static u16 GUA_Infrared_Receiver_GetLowLevelTime(void);

u8 IR_RECEIVE[5];
int down16,up16;
u8 runtime_account = 0;

static void GUA_Infrared_Receiver_IO_Init(void)
{   
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(GUA_INFRARED_RECEIVER_RCC | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GUA_INFRARED_RECEIVER_PIN;        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    GPIO_Init(GUA_INFRARED_RECEIVER_PORT, &GPIO_InitStructure);  

    GPIO_EXTILineConfig(GUA_INFRARED_RECEIVER_PORTSOURCE, GUA_INFRARED_RECEIVER_PINSOURCE); 
}

//******************************************************************************            
//name:             GUA_Infrared_Receiver_Exti_Init                           
//******************************************************************************
static void GUA_Infrared_Receiver_Exti_Init(void)
{   

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;	//用于滤波，电平判断频率由时钟提供，此处配置的分频系数即判断次数，越高则滤波效果越好
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65535;		//此处应设置一个0~65535的整数，也就是ARR的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1440-1;			//预分频系数
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 1-1;//重复计数次数，高级计数器特有，此处配置0就好
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
	TIM_Cmd(TIM1,ENABLE);
	
   NVIC_InitTypeDef NVIC_InitStructure;
     EXTI_InitTypeDef EXTI_InitStructure;



     EXTI_InitStructure.EXTI_Line = GUA_INFRARED_RECEIVER_EXTI_LINE;
     EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
     EXTI_InitStructure.EXTI_Trigger =  EXTI_Trigger_Rising_Falling ;  
     EXTI_InitStructure.EXTI_LineCmd = ENABLE;
     EXTI_Init(&EXTI_InitStructure);

     NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);

}


//******************************************************************************        
//name:             GUA_Infrared_Receiver_Init         
//******************************************************************************  
void GUA_Infrared_Receiver_Init(void)
{
GUA_Infrared_Receiver_IO_Init();
GUA_Infrared_Receiver_Exti_Init();
}

//******************************************************************************        
//name:             GUA_Infrared_Receiver_GetHighLevelTime        
//****************************************************************************** 
static u16 GUA_Infrared_Receiver_GetHighLevelTime(void)
{
    u16 nGUA_Num = 0;


    while(GUA_INFRARED_RECEIVER)
    {

        if(nGUA_Num >= 250) 
        {
            return nGUA_Num;
        }

        nGUA_Num++;
        delay_us(20);//该延时针对外部晶振8000的STM32C8T6核心板，对于不同类型不同外部晶振的开发板需要根据波形图调整延时,保证20us延时       
    }

    return nGUA_Num;
}

//******************************************************************************        
//name:             GUA_Infrared_Receiver_GetLowLevelTime          
//****************************************************************************** 
static u16 GUA_Infrared_Receiver_GetLowLevelTime(void)
{
    u16 nGUA_Num = 0;


    while(!GUA_INFRARED_RECEIVER)
    {
        if(nGUA_Num >= 500) 
        {
            return nGUA_Num;
        }

        nGUA_Num++;

        delay_us(20);//该延时针对外部晶振8000的STM32C8T6核心板，对于不同类型不同外部晶振的开发板需要根据波形图调整延时，保证20us延时                   
    }

    return nGUA_Num;
}

//******************************************************************************            
//name:             GUA_Infrared_Receiver_Process                  
//******************************************************************************
u8 GUA_Infrared_Receiver_Process(void)
{
    u16 nGUA_Time_Num = 0;
    u8 nGUA_Data = 0;
    u8 nGUA_Byte_Num = 0;
    u8 nGUA_Bit_Num = 0;    

    nGUA_Time_Num = GUA_Infrared_Receiver_GetLowLevelTime();
    if((nGUA_Time_Num >= 500) || (nGUA_Time_Num <= 400))
    {
        return GUA_INFRARED_RECEIVER_ERROR;
    }   

    nGUA_Time_Num = GUA_Infrared_Receiver_GetHighLevelTime();
    if((nGUA_Time_Num >= 250) || (nGUA_Time_Num <= 200))
    {
        return GUA_INFRARED_RECEIVER_ERROR;
    }   

    for(nGUA_Byte_Num = 0; nGUA_Byte_Num < 4; nGUA_Byte_Num++)
    {

        for(nGUA_Bit_Num = 0; nGUA_Bit_Num < 8; nGUA_Bit_Num++)
        {

            nGUA_Time_Num = GUA_Infrared_Receiver_GetLowLevelTime();
            if((nGUA_Time_Num >= 60) || (nGUA_Time_Num <= 20))
            {
                return GUA_INFRARED_RECEIVER_ERROR;
            }   

            nGUA_Time_Num = GUA_Infrared_Receiver_GetHighLevelTime();
            if((nGUA_Time_Num >=60) && (nGUA_Time_Num < 100))
            {
                nGUA_Data = 1;
            }           
            else if((nGUA_Time_Num >=10) && (nGUA_Time_Num < 50))
            {
                nGUA_Data = 0;
            }
            else
            {
                return GUA_INFRARED_RECEIVER_ERROR;
            }

            gGUA_InfraredReceiver_Data <<= 1;   
            gGUA_InfraredReceiver_Data |= nGUA_Data;                        
        }
    }

    return GUA_INFRARED_RECEIVER_OK;    
}

u8 exti_sign = 0;
u8 hwyk_exti_sign = 0;
u8 hwyk_exti_tragger = 0;
u8 detcet_level_sign = 0;
void EXTI15_10_IRQHandler(void)
{	
	u32 nGUA_Num;
  if(EXTI_GetITStatus(GUA_INFRARED_RECEIVER_EXTI_LINE) != RESET)
  {
      EXTI_ClearITPendingBit(GUA_INFRARED_RECEIVER_EXTI_LINE);
	  EXTI->IMR &= ~(GUA_INFRARED_RECEIVER_EXTI_LINE);
	  if(GUA_INFRARED_RECEIVER == detcet_level_sign) {TIM_SetCounter(TIM1,0);}
	  else {nGUA_Num = TIM_GetCounter(TIM1);hwyk_exti_tragger = 1;TIM_SetCounter(TIM1,0);}
	  if(hwyk_exti_tragger){
		  hwyk_exti_tragger = 0;
		  
		  if(hwyk_exti_sign == 0){if((nGUA_Num <= 500) && (nGUA_Num >= 400)) {hwyk_exti_sign = 1;detcet_level_sign = 1;}else {hwyk_exti_sign = 0;detcet_level_sign = 0;}}
		  else if(hwyk_exti_sign == 1){if((nGUA_Num <= 250) && (nGUA_Num >= 200)) {hwyk_exti_sign = 2;detcet_level_sign = 0;}
										else if((nGUA_Num <= 135) && (nGUA_Num >= 95)) {hwyk_exti_sign = 0;detcet_level_sign = 0;runtime_account = 13;}	
										else {hwyk_exti_sign = 0;detcet_level_sign = 0;}}
		  else if(hwyk_exti_sign == 2){if((nGUA_Num <= 60) && (nGUA_Num >= 20)) {hwyk_exti_sign = 3;detcet_level_sign = 1;}else {hwyk_exti_sign = 0;detcet_level_sign = 0;}}
		  else if(hwyk_exti_sign == 3){if((nGUA_Num >=60) && (nGUA_Num < 100)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 1;hwyk_exti_sign = 4;detcet_level_sign = 0;}
									   else if((nGUA_Num >=10) && (nGUA_Num < 50)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 0;hwyk_exti_sign = 4;detcet_level_sign = 0;}
									   else hwyk_exti_sign = 0;detcet_level_sign = 0;}
		  else if(hwyk_exti_sign == 4){if((nGUA_Num <= 60) && (nGUA_Num >= 20)) {hwyk_exti_sign = 5;detcet_level_sign = 1;}else {hwyk_exti_sign = 0;detcet_level_sign = 0;}}
		  else if(hwyk_exti_sign == 5){if((nGUA_Num >=60) && (nGUA_Num < 100)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 1;hwyk_exti_sign = 6;detcet_level_sign = 0;}
									   else if((nGUA_Num >=10) && (nGUA_Num < 50)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 0;hwyk_exti_sign = 6;detcet_level_sign = 0;}
									   else hwyk_exti_sign = 0;detcet_level_sign = 0;}
		  else if(hwyk_exti_sign == 6){if((nGUA_Num <= 60) && (nGUA_Num >= 20)) {hwyk_exti_sign = 7;detcet_level_sign = 1;}else {hwyk_exti_sign = 0;detcet_level_sign = 0;}}
		  else if(hwyk_exti_sign == 7){if((nGUA_Num >=60) && (nGUA_Num < 100)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 1;hwyk_exti_sign = 8;detcet_level_sign = 0;}
									   else if((nGUA_Num >=10) && (nGUA_Num < 50)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 0;hwyk_exti_sign = 8;detcet_level_sign = 0;}
									   else hwyk_exti_sign = 0;detcet_level_sign = 0;}
		  else if(hwyk_exti_sign == 8){if((nGUA_Num <= 60) && (nGUA_Num >= 20)) {hwyk_exti_sign = 9;detcet_level_sign = 1;}else {hwyk_exti_sign = 0;detcet_level_sign = 0;}}
		  else if(hwyk_exti_sign == 9){if((nGUA_Num >=60) && (nGUA_Num < 100)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 1;hwyk_exti_sign = 10;detcet_level_sign = 0;}
									   else if((nGUA_Num >=10) && (nGUA_Num < 50)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 0;hwyk_exti_sign = 10;detcet_level_sign = 0;}
									   else hwyk_exti_sign = 0;detcet_level_sign = 0;}
		  else if(hwyk_exti_sign == 10){if((nGUA_Num <= 60) && (nGUA_Num >= 20)) {hwyk_exti_sign = 11;detcet_level_sign = 1;}else {hwyk_exti_sign = 0;detcet_level_sign = 0;}}
		  else if(hwyk_exti_sign == 11){if((nGUA_Num >=60) && (nGUA_Num < 100)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 1;hwyk_exti_sign = 12;detcet_level_sign = 0;}
									   else if((nGUA_Num >=10) && (nGUA_Num < 50)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 0;hwyk_exti_sign = 12;detcet_level_sign = 0;}
									   else hwyk_exti_sign = 0;detcet_level_sign = 0;}
		  else if(hwyk_exti_sign == 12){if((nGUA_Num <= 60) && (nGUA_Num >= 20)) {hwyk_exti_sign = 13;detcet_level_sign = 1;}else {hwyk_exti_sign = 0;detcet_level_sign = 0;}}
		  else if(hwyk_exti_sign == 13){if((nGUA_Num >=60) && (nGUA_Num < 100)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 1;hwyk_exti_sign = 14;detcet_level_sign = 0;}
									   else if((nGUA_Num >=10) && (nGUA_Num < 50)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 0;hwyk_exti_sign = 14;detcet_level_sign = 0;}
									   else hwyk_exti_sign = 0;detcet_level_sign = 0;}
		  else if(hwyk_exti_sign == 14){if((nGUA_Num <= 60) && (nGUA_Num >= 20)) {hwyk_exti_sign = 15;detcet_level_sign = 1;}else {hwyk_exti_sign = 0;detcet_level_sign = 0;}}
		  else if(hwyk_exti_sign == 15){if((nGUA_Num >=60) && (nGUA_Num < 100)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 1;hwyk_exti_sign = 16;detcet_level_sign = 0;}
									   else if((nGUA_Num >=10) && (nGUA_Num < 50)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 0;hwyk_exti_sign = 16;detcet_level_sign = 0;}
									   else hwyk_exti_sign = 0;detcet_level_sign = 0;}
		  else if(hwyk_exti_sign == 16){if((nGUA_Num <= 60) && (nGUA_Num >= 20)) {hwyk_exti_sign = 17;detcet_level_sign = 1;}else {hwyk_exti_sign = 0;detcet_level_sign = 0;}}
		  else if(hwyk_exti_sign == 17){if((nGUA_Num >=60) && (nGUA_Num < 100)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 1;hwyk_exti_sign = 18;detcet_level_sign = 0;}
									   else if((nGUA_Num >=10) && (nGUA_Num < 50)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 0;hwyk_exti_sign = 18;detcet_level_sign = 0;}
									   else hwyk_exti_sign = 0;detcet_level_sign = 0;}
		  else if(hwyk_exti_sign == 18){if((nGUA_Num <= 60) && (nGUA_Num >= 20)) {hwyk_exti_sign = 19;detcet_level_sign = 1;}else {hwyk_exti_sign = 0;detcet_level_sign = 0;}}
		  else if(hwyk_exti_sign == 19){if((nGUA_Num >=60) && (nGUA_Num < 100)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 1;hwyk_exti_sign = 20;detcet_level_sign = 0;}
									   else if((nGUA_Num >=10) && (nGUA_Num < 50)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 0;hwyk_exti_sign = 20;detcet_level_sign = 0;}
									   else hwyk_exti_sign = 0;detcet_level_sign = 0;}
		  else if(hwyk_exti_sign == 20){if((nGUA_Num <= 60) && (nGUA_Num >= 20)) {hwyk_exti_sign = 21;detcet_level_sign = 1;}else {hwyk_exti_sign = 0;detcet_level_sign = 0;}}
		  else if(hwyk_exti_sign == 21){if((nGUA_Num >=60) && (nGUA_Num < 100)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 1;hwyk_exti_sign = 22;detcet_level_sign = 0;}
									   else if((nGUA_Num >=10) && (nGUA_Num < 50)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 0;hwyk_exti_sign = 22;detcet_level_sign = 0;}
									   else hwyk_exti_sign = 0;detcet_level_sign = 0;}
		  else if(hwyk_exti_sign == 22){if((nGUA_Num <= 60) && (nGUA_Num >= 20)) {hwyk_exti_sign = 23;detcet_level_sign = 1;}else {hwyk_exti_sign = 0;detcet_level_sign = 0;}}
		  else if(hwyk_exti_sign == 23){if((nGUA_Num >=60) && (nGUA_Num < 100)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 1;hwyk_exti_sign = 24;detcet_level_sign = 0;}
									   else if((nGUA_Num >=10) && (nGUA_Num < 50)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 0;hwyk_exti_sign = 24;detcet_level_sign = 0;}
									   else hwyk_exti_sign = 0;detcet_level_sign = 0;}
		  else if(hwyk_exti_sign == 24){if((nGUA_Num <= 60) && (nGUA_Num >= 20)) {hwyk_exti_sign = 25;detcet_level_sign = 1;}else {hwyk_exti_sign = 0;detcet_level_sign = 0;}}
		  else if(hwyk_exti_sign == 25){if((nGUA_Num >=60) && (nGUA_Num < 100)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 1;hwyk_exti_sign = 26;detcet_level_sign = 0;}
									   else if((nGUA_Num >=10) && (nGUA_Num < 50)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 0;hwyk_exti_sign = 26;detcet_level_sign = 0;}
									   else hwyk_exti_sign = 0;detcet_level_sign = 0;}
		  else if(hwyk_exti_sign == 26){if((nGUA_Num <= 60) && (nGUA_Num >= 20)) {hwyk_exti_sign = 27;detcet_level_sign = 1;}else {hwyk_exti_sign = 0;detcet_level_sign = 0;}}
		  else if(hwyk_exti_sign == 27){if((nGUA_Num >=60) && (nGUA_Num < 100)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 1;hwyk_exti_sign = 28;detcet_level_sign = 0;}
									   else if((nGUA_Num >=10) && (nGUA_Num < 50)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 0;hwyk_exti_sign = 28;detcet_level_sign = 0;}
									   else hwyk_exti_sign = 0;detcet_level_sign = 0;}
		  else if(hwyk_exti_sign == 28){if((nGUA_Num <= 60) && (nGUA_Num >= 20)) {hwyk_exti_sign = 29;detcet_level_sign = 1;}else {hwyk_exti_sign = 0;detcet_level_sign = 0;}}
		  else if(hwyk_exti_sign == 29){if((nGUA_Num >=60) && (nGUA_Num < 100)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 1;hwyk_exti_sign = 30;detcet_level_sign = 0;}
									   else if((nGUA_Num >=10) && (nGUA_Num < 50)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 0;hwyk_exti_sign = 30;detcet_level_sign = 0;}
									   else hwyk_exti_sign = 0;detcet_level_sign = 0;}
		  else if(hwyk_exti_sign == 30){if((nGUA_Num <= 60) && (nGUA_Num >= 20)) {hwyk_exti_sign = 31;detcet_level_sign = 1;}else {hwyk_exti_sign = 0;detcet_level_sign = 0;}}
		  else if(hwyk_exti_sign == 31){if((nGUA_Num >=60) && (nGUA_Num < 100)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 1;hwyk_exti_sign = 32;detcet_level_sign = 0;}
									   else if((nGUA_Num >=10) && (nGUA_Num < 50)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 0;hwyk_exti_sign = 32;detcet_level_sign = 0;}
									   else hwyk_exti_sign = 0;detcet_level_sign = 0;}
		  else if(hwyk_exti_sign == 32){if((nGUA_Num <= 60) && (nGUA_Num >= 20)) {hwyk_exti_sign = 33;detcet_level_sign = 1;}else {hwyk_exti_sign = 0;detcet_level_sign = 0;}}
		  else if(hwyk_exti_sign == 33){if((nGUA_Num >=60) && (nGUA_Num < 100)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 1;hwyk_exti_sign = 34;detcet_level_sign = 0;}
									   else if((nGUA_Num >=10) && (nGUA_Num < 50)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 0;hwyk_exti_sign = 34;detcet_level_sign = 0;}
									   else hwyk_exti_sign = 0;detcet_level_sign = 0;}
		  else if(hwyk_exti_sign == 34){if((nGUA_Num <= 60) && (nGUA_Num >= 20)) {hwyk_exti_sign = 35;detcet_level_sign = 1;}else {hwyk_exti_sign = 0;detcet_level_sign = 0;}}
		  else if(hwyk_exti_sign == 35){if((nGUA_Num >=60) && (nGUA_Num < 100)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 1;hwyk_exti_sign = 36;detcet_level_sign = 0;}
									   else if((nGUA_Num >=10) && (nGUA_Num < 50)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 0;hwyk_exti_sign = 36;detcet_level_sign = 0;}
									   else hwyk_exti_sign = 0;detcet_level_sign = 0;}
		  else if(hwyk_exti_sign == 36){if((nGUA_Num <= 60) && (nGUA_Num >= 20)) {hwyk_exti_sign = 37;detcet_level_sign = 1;}else {hwyk_exti_sign = 0;detcet_level_sign = 0;}}
		  else if(hwyk_exti_sign == 37){if((nGUA_Num >=60) && (nGUA_Num < 100)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 1;hwyk_exti_sign = 38;detcet_level_sign = 0;}
									   else if((nGUA_Num >=10) && (nGUA_Num < 50)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 0;hwyk_exti_sign = 38;detcet_level_sign = 0;}
									   else hwyk_exti_sign = 0;detcet_level_sign = 0;}
		  else if(hwyk_exti_sign == 38){if((nGUA_Num <= 60) && (nGUA_Num >= 20)) {hwyk_exti_sign = 39;detcet_level_sign = 1;}else {hwyk_exti_sign = 0;detcet_level_sign = 0;}}
		  else if(hwyk_exti_sign == 39){if((nGUA_Num >=60) && (nGUA_Num < 100)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 1;hwyk_exti_sign = 40;detcet_level_sign = 0;}
									   else if((nGUA_Num >=10) && (nGUA_Num < 50)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 0;hwyk_exti_sign = 40;detcet_level_sign = 0;}
									   else hwyk_exti_sign = 0;detcet_level_sign = 0;}
		  else if(hwyk_exti_sign == 40){if((nGUA_Num <= 60) && (nGUA_Num >= 20)) {hwyk_exti_sign = 41;detcet_level_sign = 1;}else {hwyk_exti_sign = 0;detcet_level_sign = 0;}}
		  else if(hwyk_exti_sign == 41){if((nGUA_Num >=60) && (nGUA_Num < 100)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 1;hwyk_exti_sign = 42;detcet_level_sign = 0;}
									   else if((nGUA_Num >=10) && (nGUA_Num < 50)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 0;hwyk_exti_sign = 42;detcet_level_sign = 0;}
									   else hwyk_exti_sign = 0;detcet_level_sign = 0;}
		  else if(hwyk_exti_sign == 42){if((nGUA_Num <= 60) && (nGUA_Num >= 20)) {hwyk_exti_sign = 43;detcet_level_sign = 1;}else {hwyk_exti_sign = 0;detcet_level_sign = 0;}}
		  else if(hwyk_exti_sign == 43){if((nGUA_Num >=60) && (nGUA_Num < 100)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 1;hwyk_exti_sign = 44;detcet_level_sign = 0;}
									   else if((nGUA_Num >=10) && (nGUA_Num < 50)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 0;hwyk_exti_sign = 44;detcet_level_sign = 0;}
									   else hwyk_exti_sign = 0;detcet_level_sign = 0;}
		  else if(hwyk_exti_sign == 44){if((nGUA_Num <= 60) && (nGUA_Num >= 20)) {hwyk_exti_sign = 45;detcet_level_sign = 1;}else {hwyk_exti_sign = 0;detcet_level_sign = 0;}}
		  else if(hwyk_exti_sign == 45){if((nGUA_Num >=60) && (nGUA_Num < 100)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 1;hwyk_exti_sign = 46;detcet_level_sign = 0;}
									   else if((nGUA_Num >=10) && (nGUA_Num < 50)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 0;hwyk_exti_sign = 46;detcet_level_sign = 0;}
									   else hwyk_exti_sign = 0;detcet_level_sign = 0;}
		  else if(hwyk_exti_sign == 46){if((nGUA_Num <= 60) && (nGUA_Num >= 20)) {hwyk_exti_sign = 47;detcet_level_sign = 1;}else {hwyk_exti_sign = 0;detcet_level_sign = 0;}}
		  else if(hwyk_exti_sign == 47){if((nGUA_Num >=60) && (nGUA_Num < 100)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 1;hwyk_exti_sign = 48;detcet_level_sign = 0;}
									   else if((nGUA_Num >=10) && (nGUA_Num < 50)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 0;hwyk_exti_sign = 48;detcet_level_sign = 0;}
									   else hwyk_exti_sign = 0;detcet_level_sign = 0;}
		  else if(hwyk_exti_sign == 48){if((nGUA_Num <= 60) && (nGUA_Num >= 20)) {hwyk_exti_sign = 49;detcet_level_sign = 1;}else {hwyk_exti_sign = 0;detcet_level_sign = 0;}}
		  else if(hwyk_exti_sign == 49){if((nGUA_Num >=60) && (nGUA_Num < 100)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 1;hwyk_exti_sign = 50;detcet_level_sign = 0;}
									   else if((nGUA_Num >=10) && (nGUA_Num < 50)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 0;hwyk_exti_sign = 50;detcet_level_sign = 0;}
									   else hwyk_exti_sign = 0;detcet_level_sign = 0;}
		  else if(hwyk_exti_sign == 50){if((nGUA_Num <= 60) && (nGUA_Num >= 20)) {hwyk_exti_sign = 51;detcet_level_sign = 1;}else {hwyk_exti_sign = 0;detcet_level_sign = 0;}}
		  else if(hwyk_exti_sign == 51){if((nGUA_Num >=60) && (nGUA_Num < 100)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 1;hwyk_exti_sign = 52;detcet_level_sign = 0;}
									   else if((nGUA_Num >=10) && (nGUA_Num < 50)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 0;hwyk_exti_sign = 52;detcet_level_sign = 0;}
									   else hwyk_exti_sign = 0;detcet_level_sign = 0;}
		  else if(hwyk_exti_sign == 52){if((nGUA_Num <= 60) && (nGUA_Num >= 20)) {hwyk_exti_sign = 53;detcet_level_sign = 1;}else {hwyk_exti_sign = 0;detcet_level_sign = 0;}}
		  else if(hwyk_exti_sign == 53){if((nGUA_Num >=60) && (nGUA_Num < 100)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 1;hwyk_exti_sign = 54;detcet_level_sign = 0;}
									   else if((nGUA_Num >=10) && (nGUA_Num < 50)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 0;hwyk_exti_sign = 54;detcet_level_sign = 0;}
									   else hwyk_exti_sign = 0;detcet_level_sign = 0;}
		  else if(hwyk_exti_sign == 54){if((nGUA_Num <= 60) && (nGUA_Num >= 20)) {hwyk_exti_sign = 55;detcet_level_sign = 1;}else {hwyk_exti_sign = 0;detcet_level_sign = 0;}}
		  else if(hwyk_exti_sign == 55){if((nGUA_Num >=60) && (nGUA_Num < 100)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 1;hwyk_exti_sign = 56;detcet_level_sign = 0;}
									   else if((nGUA_Num >=10) && (nGUA_Num < 50)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 0;hwyk_exti_sign = 56;detcet_level_sign = 0;}
									   else hwyk_exti_sign = 0;detcet_level_sign = 0;}
		  else if(hwyk_exti_sign == 56){if((nGUA_Num <= 60) && (nGUA_Num >= 20)) {hwyk_exti_sign = 57;detcet_level_sign = 1;}else {hwyk_exti_sign = 0;detcet_level_sign = 0;}}
		  else if(hwyk_exti_sign == 57){if((nGUA_Num >=60) && (nGUA_Num < 100)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 1;hwyk_exti_sign = 58;detcet_level_sign = 0;}
									   else if((nGUA_Num >=10) && (nGUA_Num < 50)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 0;hwyk_exti_sign = 58;detcet_level_sign = 0;}
									   else hwyk_exti_sign = 0;detcet_level_sign = 0;}
		  else if(hwyk_exti_sign == 58){if((nGUA_Num <= 60) && (nGUA_Num >= 20)) {hwyk_exti_sign = 59;detcet_level_sign = 1;}else {hwyk_exti_sign = 0;detcet_level_sign = 0;}}
		  else if(hwyk_exti_sign == 59){if((nGUA_Num >=60) && (nGUA_Num < 100)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 1;hwyk_exti_sign = 60;detcet_level_sign = 0;}
									   else if((nGUA_Num >=10) && (nGUA_Num < 50)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 0;hwyk_exti_sign = 60;detcet_level_sign = 0;}
									   else hwyk_exti_sign = 0;detcet_level_sign = 0;}
		  else if(hwyk_exti_sign == 60){if((nGUA_Num <= 60) && (nGUA_Num >= 20)) {hwyk_exti_sign = 61;detcet_level_sign = 1;}else {hwyk_exti_sign = 0;detcet_level_sign = 0;}}
		  else if(hwyk_exti_sign == 61){if((nGUA_Num >=60) && (nGUA_Num < 100)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 1;hwyk_exti_sign = 62;detcet_level_sign = 0;}
									   else if((nGUA_Num >=10) && (nGUA_Num < 50)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 0;hwyk_exti_sign = 62;detcet_level_sign = 0;}
									   else hwyk_exti_sign = 0;detcet_level_sign = 0;}
		  else if(hwyk_exti_sign == 62){if((nGUA_Num <= 60) && (nGUA_Num >= 20)) {hwyk_exti_sign = 63;detcet_level_sign = 1;}else {hwyk_exti_sign = 0;detcet_level_sign = 0;}}
		  else if(hwyk_exti_sign == 63){if((nGUA_Num >=60) && (nGUA_Num < 100)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 1;hwyk_exti_sign = 64;detcet_level_sign = 0;}
									   else if((nGUA_Num >=10) && (nGUA_Num < 50)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 0;hwyk_exti_sign = 64;detcet_level_sign = 0;}
									   else hwyk_exti_sign = 0;detcet_level_sign = 0;}
		  else if(hwyk_exti_sign == 64){if((nGUA_Num <= 60) && (nGUA_Num >= 20)) {hwyk_exti_sign = 65;detcet_level_sign = 1;}else {hwyk_exti_sign = 0;detcet_level_sign = 0;}}
		  else if(hwyk_exti_sign == 65){if((nGUA_Num >=60) && (nGUA_Num < 100)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 1;hwyk_exti_sign = 0;IR_RECEIVE[2] = gGUA_InfraredReceiver_Data>>8;runtime_account = 13;}
									   else if((nGUA_Num >=10) && (nGUA_Num < 50)){gGUA_InfraredReceiver_Data <<= 1;gGUA_InfraredReceiver_Data |= 0;hwyk_exti_sign = 0;IR_RECEIVE[2] = gGUA_InfraredReceiver_Data>>8;runtime_account = 13;}
									   else hwyk_exti_sign = 0; exti_sign = 1;}     
         }
	 }
         EXTI->IMR|=(GUA_INFRARED_RECEIVER_EXTI_LINE);
		if(EXTI_GetITStatus(HSR_EXTI_Line) != RESET)
    	{
			if(Echo) {Hsr_distant = TIM_GetCounter(TIM2);Hsr_pull_out_counter = 0;}
			else {Hsr_distant = TIM_GetCounter(TIM2) - Hsr_distant + 7200*Hsr_pull_out_counter;
					Hsr_distant*=17;Hsr_distant/=3600;if(dostamt_load_account--){distant_average_load += Hsr_distant;}else{current_distant = 0.9*distant_average_load/10 + 0.1*lastime_distant;lastime_distant = current_distant;dostamt_load_account = 10;distant_average_load = 0;}}
    	}
    	EXTI_ClearITPendingBit(HSR_EXTI_Line);
		
//        if(GUA_Infrared_Receiver_Process() == GUA_INFRARED_RECEIVER_OK)
//        {
//            down16=gGUA_InfraredReceiver_Data;
//          up16=gGUA_InfraredReceiver_Data>>16;
//          //从上到下依次为用户反码、用户码、地址反码、地址码
//          IR_RECEIVE[3]=down16;
//          IR_RECEIVE[2]=(down16>>8);
//          IR_RECEIVE[1]=up16;
//          IR_RECEIVE[0]=(up16>>8);
//          //IR_RECPRO();自己写接收处理函数x
//        }
//    else
//        {   
//            gGUA_InfraredReceiver_Data=0;
//        }  
}

