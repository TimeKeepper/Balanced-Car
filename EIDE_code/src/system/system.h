#ifndef _SYSTEM_H_
#define _SYSTEM_H_

//including so much file is not neccessary for a embedded project,I do this just because clangd return many error without including.
#include "stm32f10x.h"
#include "misc.h"
#include "core_cm3.h"
#include "systick.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_exti.h"

//annoying clangd always marks some code segments as errors that are not problematic in embedded programming:(

#define mine_PI    3.14159265358979323846

//Get GPIO's RCC to avoid some repetitive code:)
uint32_t GET_GPIO_RCC(GPIO_TypeDef* GPIOx);
void GPIO_RCC_ENABLE(GPIO_TypeDef* GPIOx);
uint32_t GET_TIM_RCC(TIM_TypeDef* TIMx);
uint8_t GET_TIM_IRQn(TIM_TypeDef* TIMx);
void TIM_RCC_ENABLE(TIM_TypeDef* TIMx);
uint32_t GET_EXTI_LINE(uint32_t GPIO_Pin);
uint8_t GET_EXTI_IRQn(uint32_t GPIO_Pin);
uint8_t GET_PORT_SOURCE(GPIO_TypeDef* GPIOx);
uint8_t GET_GPIO_PinSource(uint16_t GPIO_Pin);

/*Bit-banding operation maps the configuration of GPIO output and input to some registers, 
which can be implemented through macro definitions. 
This is commonly used in situations where high-speed GPIO input and output are required. 
For more details, please refer to the official guide of ARM-cortexm3*/
//Bit-banding operation
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
//The reason why here we use volatile is that the value of the register may change at any time
//and volatile can ensure that compiler will not read the value of the register for optimization.
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

//IO address mapping
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 

//IO operation, only for one IO
//ensure that the parameter is a constant between 0 and 15
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //Output
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //Input

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //Output
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //Input

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //Output
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //Input

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //Output
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //Input

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //Output
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //Input

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //Output
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //Input

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //Output
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //Input

#endif // _SYSTEM_H_
