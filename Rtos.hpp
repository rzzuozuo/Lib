/*
 * Rtos.hpp
 *
 *  Created on: 2021年4月19日
 *      Author: Administrator
 */

#ifndef APPLICATION_RTOS_HPP_
#define APPLICATION_RTOS_HPP_
#include "cmsis_os.h"
class Rtos {
public:
	Rtos();
	virtual ~Rtos();
	static osStatus_t initialize(){
		  return osKernelInitialize();

	}

	static osStatus_t kernelStart(){
		return osKernelStart();
	}

	static osStatus_t delayMs (uint32_t ticks){
		return osDelay (ticks);
	}

	static uint32_t getTick(){
		return osKernelGetTickCount();
	}
};

#endif /* APPLICATION_RTOS_HPP_ */
