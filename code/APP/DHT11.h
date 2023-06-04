#ifndef _DHT11_H_
#define _DHT11_H_

#include "system.h"
#include "Systick.h"

#define DHT11_PORT GPIOB
#define DHT11_Pin  GPIO_Pin_11
#define DHT11_RCC  RCC_APB2Periph_GPIOB

void DHT11_IO_OUT (void);
void DHT11_IO_IN (void);
void DHT11_RST (void);
void DHT11_WaitForReAct(u8 flag,u8* retry);
u8 DHT11_Init (void);
u8 DHT11_Read_Data(u8 *temp, u8 *humi);

#endif
