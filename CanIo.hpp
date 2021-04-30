/*
 * CanIo.h
 *
 *  Created on: Apr 27, 2021
 *      Author: Administrator
 */

#ifndef CANIO_H_
#define CANIO_H_

#include <Can.hpp>
#include <Thread.hpp>

class CanIo: public Can, public Thread {
	friend void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
public:
	struct CanMsg_t{
		uint16_t id;
		uint16_t length;
		uint8_t data[8];
	};
	CanIo(CAN_HandleTypeDef &hcan,int txSize,int rxSize);
	virtual ~CanIo();

	virtual void setBaudrate() {
		baudrate = {9,2,1,1};
	}

	virtual void init() override;

	void sendMsg(CanMsg_t msg);
protected:
	Can::StatusTypeDef setFilter(uint16_t* id,int size);
private:
	Baudrate_t baudrate = {1,1,1,1};
	int txSize;
	int rxSize;
	virtual StatusTypeDef filter() override;
	void run() override;
	virtual void setCanDevice();
	virtual void setThread();
	virtual void msgDecode(CanMsg_t &msg){

	}
	osMessageQueueId_t txMsgQueue = NULL;
	osMessageQueueId_t rxMsgQueue = NULL;
	virtual void fifo0callBack(CanMsg_t &msg);
};

#endif /* CANIO_H_ */
