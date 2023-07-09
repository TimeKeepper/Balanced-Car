#ifndef __OLED_H
#define __OLED_H

#include "system.h"

void OLED_Init(GPIO_TypeDef * OLED_SCL_PORT, uint16_t OLED_SCL_Pin, GPIO_TypeDef * OLED_SDA_PORT, uint16_t OLED_SDA_Pin);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);

#endif
