#ifndef _OLED_H_
#define _OLED_H_

#include "system.h"
#include "IOIIC.h"

void OLED_Init(GPIO_TypeDef * SCL_PORT, u16 SCL_Pin, GPIO_TypeDef * SDA_PORT, u16 SDA_Pin);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char * String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Num, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Num, uint8_t Length);



#endif // _OLED_H_
