#include "system.h"

uint32_t GET_GPIO_RCC(GPIO_TypeDef* GPIOx)
{
	if		(GPIOx == GPIOA)
	{
		return RCC_APB2Periph_GPIOA;
	}
	else if	(GPIOx == GPIOB)
	{
		return RCC_APB2Periph_GPIOB;
	}
	else if	(GPIOx == GPIOC)
	{
		return RCC_APB2Periph_GPIOC;
	}
	else if	(GPIOx == GPIOD)
	{
		return RCC_APB2Periph_GPIOD;
	}
	else if	(GPIOx == GPIOE)
	{
		return RCC_APB2Periph_GPIOE;
	}
	else
	{
		return 0;
	}
}
