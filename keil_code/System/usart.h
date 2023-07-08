#ifndef _USART_H
#define _USART_H

extern int uart_message;

#include "system.h"
#include "stdio.h"

//串口通信对应GPIO口宏定义，TX即transmit（发送），RX即receive（接收）
#define USART1_TX_PORT GPIOA
#define USART1_RX_PORT GPIOA
#define USART2_TX_PORT GPIOA
#define USART2_RX_PORT GPIOA
#define USART3_TX_PORT GPIOB
#define USART3_RX_PORT GPIOB

#define USART1_TX_Pin GPIO_Pin_9
#define USART1_RX_Pin GPIO_Pin_10
#define USART2_TX_Pin GPIO_Pin_2
#define USART2_RX_Pin GPIO_Pin_3
#define USART3_TX_Pin GPIO_Pin_10
#define USART3_RX_Pin GPIO_Pin_11

void USART1_Init(u32 pound);
u8 USART_RC(void);
u16 USART_GET_LEN(void);
void USART_Clear_RX_STA(void);

#endif
