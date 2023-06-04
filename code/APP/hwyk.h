#ifndef _HWYK_H_
#define _HWYK_H_

#include "system.h"
#include "OLED.h"
#include "hsr04.h"

#define GUA_INFRARED_RECEIVER_PORT               GPIOC
#define GUA_INFRARED_RECEIVER_PIN                GPIO_Pin_14
#define GUA_INFRARED_RECEIVER_RCC                RCC_APB2Periph_GPIOC
#define GUA_INFRARED_RECEIVER					 PCin(14)

#define GUA_INFRARED_RECEIVER_EXTI_LINE          EXTI_Line14
#define GUA_INFRARED_RECEIVER_PORTSOURCE         GPIO_PortSourceGPIOC
#define GUA_INFRARED_RECEIVER_PINSOURCE          GPIO_PinSource14

#define GUA_INFRARED_RECEIVER_OK                0
#define GUA_INFRARED_RECEIVER_ERROR             1

extern u32 gGUA_InfraredReceiver_Data;
extern u8 IR_RECEIVE[5];
extern u8 exti_sign;
extern u8 hwyk_exti_sign;
extern u8 hwyk_exti_tragger;
extern u8 runtime_account;

extern void GUA_Infrared_Receiver_Init(void);
extern u8 GUA_Infrared_Receiver_Process(void);
void IR_RECPRO(void);


#endif
