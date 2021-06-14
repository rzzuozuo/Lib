/*
 * CanTxQueue.cpp
 *
 *  Created on: Apr 27, 2021
 *      Author: Administrator
 */

#include <Io/CanTxQueue.hpp>

#ifdef LIB_DRIVER_CAN_ENABLED


CanTxQueue::CanTxQueue(Can &can):can(can){
	setName("CanTxQueue");
	setStackSize(512);
	queue_id= osMessageQueueNew(16, sizeof(Can::Msg), NULL);
}

CanTxQueue::~CanTxQueue() {
}


void CanTxQueue::run(){
	osStatus_t status;
	Can::Msg msg;
	for(;;){

		while(can.getTxMailboxesFreeLevel()==0)	//wait for a empty txMailBox
		{
			osDelay(1);
		}

		status = osMessageQueueGet(queue_id, &msg, NULL, osWaitForever);   // wait for message
	    if (status == osOK) {
	    	/* Start the Transmission process */
	        if (can.addTxMessage(msg.id, msg.data, msg.dlc) != Can::OK)
	        {
	          /* Transmission request Error */
	          Error_Handler();
	        }
	    }
	}
}


void CanTxQueue::sendMsg(Can::Msg msg){
	osMessageQueuePut(queue_id, &msg, 0U, 0U);
}


#endif //LIB_DRIVER_CAN_ENABLED
