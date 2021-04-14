/*
 * Gpio.cpp
 *
 *  Created on: 2021年4月13日
 *      Author: Administrator
 */

#include "Gpio.hpp"


Gpio::Gpio(GPIO_TypeDef  *GPIOx):GPIOx(GPIOx) {
	if(GPIOx == GPIOA)
		__HAL_RCC_GPIOA_CLK_ENABLE();
	if(GPIOx == GPIOB)
		__HAL_RCC_GPIOB_CLK_ENABLE();
	if(GPIOx == GPIOC)
		__HAL_RCC_GPIOC_CLK_ENABLE();
	if(GPIOx == GPIOD)
		__HAL_RCC_GPIOD_CLK_ENABLE();
	if(GPIOx == GPIOE)
		__HAL_RCC_GPIOE_CLK_ENABLE();
}

Gpio::~Gpio() {
	// TODO Auto-generated destructor stub

}

