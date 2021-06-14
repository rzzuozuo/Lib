/*
 * Can.h
 *
 *  Created on: Apr 14, 2021
 *      Author: Administrator
 */

#ifndef APPLICATION_HAL_CPP_CAN_H_
#define APPLICATION_HAL_CPP_CAN_H_

#include "Hal.hpp"
#include <Rtos/Rtos.hpp>

#ifdef HAL_CAN_MODULE_ENABLED

#define LIB_DRIVER_CAN_ENABLED

class Can: public Hal {
	friend void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
	friend void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan);



	//static Can* getClass(CAN_HandleTypeDef* handle);
public:
	Can(CAN_HandleTypeDef &hcan);
	virtual ~Can();

	virtual void error(){}

	virtual StatusTypeDef filter(int fifo);
	StatusTypeDef filter(uint16_t* id,int size, int fifo);

	StatusTypeDef start();
	StatusTypeDef activateNotification(int fifo);

	uint32_t getTxMailboxesFreeLevel();

	StatusTypeDef addTxMessage(uint16_t id, uint8_t* data, int size);
	StatusTypeDef getFifo0Message();
	StatusTypeDef getFifo1Message();

	CAN_HandleTypeDef* getHandle(){
		return &handle;
	}

	virtual void fifo0Callback();
	virtual void fifo1Callback();
	osMessageQueueId_t getQueueId() const {
		return queue_id;
	}

	void putMessage(
			CAN_RxHeaderTypeDef* pHeader,
			uint8_t* data);

	struct Header{
		uint32_t id:32;
		uint8_t dlc:4;
		bool rtr:1;
		bool ide:1;
	};

	struct Msg:Header{
		uint8_t data[8];
	};

protected:
	CAN_HandleTypeDef &handle;
	uint32_t txMailbox;
	CAN_TxHeaderTypeDef txHeader;
	CAN_RxHeaderTypeDef fifo0Header;
	CAN_RxHeaderTypeDef fifo1Header;
	uint8_t fifo0Data[8];
	uint8_t fifo1Data[8];
	osMessageQueueId_t queue_id = NULL;
};
#endif // HAL_CAN_MODULE_ENABLED
#endif /* APPLICATION_HAL_CPP_CAN_H_ */
