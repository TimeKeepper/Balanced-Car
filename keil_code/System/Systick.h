#ifndef _SYSTIC_H
#define _SYSTIC_H

#include "system.h"

/*
本次没有使用systick的最后一个寄存器，CALIB寄存器，这个寄存器基本用不到，需要用到了去百度
*/
void Systick_Init(u8 SYSCLK);
void delay_us(u32 nus);
void delay_ms(u32 nms);
void delay_s(u32 ns);

#endif
