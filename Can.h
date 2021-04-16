/*
 * Can.h
 *
 *  Created on: Apr 14, 2021
 *      Author: Administrator
 */

#ifndef APPLICATION_HAL_CPP_CAN_H_
#define APPLICATION_HAL_CPP_CAN_H_

#include <Hal.hpp>

extern "C"{
void MX_CAN_Init(void);
}

class Can: public Hal {
public:
	Can();
	Can(CAN_HandleTypeDef &hcan);
	virtual ~Can();

	static void initByCubeMx(){
		MX_CAN_Init();
	}
protected:
	CAN_HandleTypeDef &hcan;
};

#endif /* APPLICATION_HAL_CPP_CAN_H_ */
