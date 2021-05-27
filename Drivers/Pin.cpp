/*
 * Pin.cpp
 *
 *  Created on: 2021年4月13日
 *      Author: Administrator
 */

#include "Pin.hpp"

Pin::Pin(GPIO_TypeDef  *GPIOx, uint16_t GPIO_Pin):Gpio(GPIOx), GPIO_Pin(GPIO_Pin){

}


Pin::~Pin() {
	void  HAL_GPIO_DeInit(GPIO_TypeDef  *GPIOx, uint32_t GPIO_Pin);
}

void Pin::init(GPIO_InitTypeDef *GPIO_Init){
	HAL_GPIO_Init(GPIOx, GPIO_Init);
}
