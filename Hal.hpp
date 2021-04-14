/*
 * Hal.h
 *
 *  Created on: 2021年4月12日
 *      Author: Administrator
 */

#ifndef APPLICATION_LIB_HAL_H_
#define APPLICATION_LIB_HAL_H_

#include "main.h"

extern "C"{
void SystemClock_Config(void);
}

class Hal {
public:
	typedef enum
	{
	  OK       = 0x00U,
	  ERROR    = 0x01U,
	  BUSY     = 0x02U,
	  TIMEOUT  = 0x03U
	} StatusTypeDef;

	Hal();
	virtual ~Hal();

	static StatusTypeDef init(){
		return (StatusTypeDef)HAL_Init();
	}

	static void systemClock_Config(){
		SystemClock_Config();
	}

	static void error_callBack(){
		Error_Handler();
	}

	static void delay(uint32_t ms){
		HAL_Delay(ms);
	}
};

#endif /* APPLICATION_LIB_HAL_H_ */
