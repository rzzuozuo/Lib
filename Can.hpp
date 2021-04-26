/*
 * Can.h
 *
 *  Created on: Apr 14, 2021
 *      Author: Administrator
 */

#ifndef APPLICATION_HAL_CPP_CAN_H_
#define APPLICATION_HAL_CPP_CAN_H_

#include <Hal.hpp>
#include <functional>
extern "C"{
void MX_CAN_Init(void);
}

class Can: public Hal {
	struct Baudrate_t{
		uint32_t Prescale;
		uint32_t TimeSeg1;
		uint32_t TimeSeg2;
		uint32_t SyncJumpWidth;
	};
public:
	Can(CAN_HandleTypeDef &hcan);
	virtual ~Can();

	static void initByCubeMx(){
		MX_CAN_Init();
	}

	virtual void error() ;
	StatusTypeDef init(uint32_t Prescaler,uint32_t TimeSeg1, uint32_t TimeSeg2, uint32_t SyncJumpWidth);
	StatusTypeDef init(Baudrate_t baudrate){
		return init(baudrate.Prescale, baudrate.TimeSeg1, baudrate.TimeSeg2, baudrate.SyncJumpWidth);
	}
	virtual StatusTypeDef filter();
	StatusTypeDef start();
	StatusTypeDef startReceive();
	void Fifo0MsgPendingCallback(CAN_HandleTypeDef *_hcan);
	void setRegisterRxCallback(void (* pFifo0MsgPendingCallback)(CAN_HandleTypeDef *_hcan)){
		this->pFifo0MsgPendingCallback = pFifo0MsgPendingCallback;
	}
protected:
	CAN_HandleTypeDef &hcan;
	void (* pFifo0MsgPendingCallback)(CAN_HandleTypeDef *_hcan) = nullptr;

};
typedef void (* Fifo0MsgPendingCallback_t)(CAN_HandleTypeDef*);
#endif /* APPLICATION_HAL_CPP_CAN_H_ */
