#ifndef _SYSTICK_H_
#define _SYSTICK_H_

#include "stm32f10x.h"

void SysTick_Init(u8 SYSCLK);
void delay_us(uint32_t count);
void delay_ms(uint32_t count);
void delay_s(uint32_t count);

#endif // _SYSTICK_H_
