#include "systick.h"
#include "misc.h"

static uint8_t TimingDelay;
//static means that this variable is only visible to the current file.

void SysTick_Init(u8 SYSCLK)
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    TimingDelay = SYSCLK/8;
}

void delay_us(uint32_t count){
    if (count == 0)return;                  
    u32 temp;                                   //temp is a variable used to store the value of SysTick->CTRL
    SysTick->LOAD = count*TimingDelay;          //Set reload register
    SysTick->VAL=0x00;                          //Set the counter value to 0
    SysTick->CTRL|=0x01;                        //Start counting down
    do temp=SysTick->CTRL;                      
    while((temp&0x01)&&!(temp&(1<<16)));        //Wait for the counter to count down to 0
    SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;    //Close counter
    SysTick->VAL =0X00;                         //Set the counter value to 0
}

void delay_ms(uint32_t count){
    while(count--)delay_us(1000);
}

void delay_s(uint32_t count){
    while(count--)delay_ms(1000);
}
