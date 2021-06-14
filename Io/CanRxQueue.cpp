/*
 * Can.cpp
 *
 *  Created on: 2021年6月14日
 *      Author: Administrator
 */

#include <Io/CanRxQueue.hpp>

CanRxQueue::CanRxQueue(Can &can):can(can) {
	setName("CanRxQueue");
}

void CanRxQueue::run(){
	osStatus_t status;
	Can::Msg msg;
	can.activateNotification(0);
	for(;;){
		status = osMessageQueueGet(can.getQueueId(), &msg, NULL, osWaitForever);   // wait for message
	    if (status == osOK) {
	    	msgDecode(&msg);
	    }
	}
}

CanRxQueue::~CanRxQueue() {
}
