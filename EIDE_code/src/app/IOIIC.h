#ifndef _IOIIC_H_
#define _IOIIC_H_

#include "system.h"

void IOIIC_Init(GPIO_TypeDef * SCL_PORT, u16 SCL_Pin, GPIO_TypeDef * SDA_PORT, u16 SDA_Pin);
void IOIIC_Start(void);
void IOIIC_Stop(void);
void IOIIC_Ack(void);
void IOIIC_NAck(void);
void IOIIC_W_Byte(uint8_t Byte);

#endif // _IOIIC_H_
