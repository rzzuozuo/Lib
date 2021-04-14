/*
 * Gpio.hpp
 *
 *  Created on: 2021年4月13日
 *      Author: Administrator
 */

#ifndef GPIO_HPP_
#define GPIO_HPP_

#include "Hal.hpp"
extern "C"{
void MX_GPIO_Init(void);
}
class Gpio:public Hal {
public:
	Gpio(GPIO_TypeDef  *GPIOx);
	virtual ~Gpio();

	static void mxInit(){
		MX_GPIO_Init();
	}


protected:
	GPIO_TypeDef *GPIOx;
};

#endif /* GPIO_HPP_ */
