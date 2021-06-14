/*
 * CanIo.h
 *
 *  Created on: Apr 27, 2021
 *      Author: Administrator
 */

#ifndef CANIO_H_
#define CANIO_H_

#include <Io/CanRxQueue.hpp>
#include <Rtos/Thread.hpp>
#include <Drivers/Can.hpp>

#ifdef LIB_DRIVER_CAN_ENABLED

class CanTxQueue: public Thread {
public:
	CanTxQueue(Can &can);
	virtual ~CanTxQueue();

	void sendMsg(Can::Msg msg);

	osMessageQueueId_t getQueueId() const {
		return queue_id;
	}

protected:
	Can::StatusTypeDef setFilter(uint16_t* id,int size);
private:
	Can &can;
	void run() override;
	osMessageQueueId_t queue_id = NULL;
};

#endif // LIB_DRIVER_CAN_ENABLED
#endif /* CANIO_H_ */
