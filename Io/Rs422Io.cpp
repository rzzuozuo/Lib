/*
 * Rs422Io.cpp
 *
 *  Created on: Apr 30, 2021
 *      Author: Administrator
 */

#include "Rs422Io.hpp"

#ifdef LIB_DRIVER_UART_ENABLED

Rs422Io::Rs422Io(UART_HandleTypeDef &huart, Pin* re, Pin* de,int txSize, int rxSize):Rs422(huart, re, de),txSize(txSize),rxSize(rxSize){

}

Rs422Io::~Rs422Io() {
	txMsg.data = nullptr;
}

void Rs422Io::transmit(void *data, int size) {
	if(txMsgQueue!= NULL){
		uint8_t* pdata = (uint8_t*)data;
		TxMsg txMsg = {NULL,0};
		txMsg.data = new uint8_t[size];
		assert_param(txMsg.data != NULL);
		if(txMsg.data != NULL){
			txMsg.size = size;
			for(int i = 0; i < size; ++i){
				txMsg.data[i] = pdata[i];
			}
			osStatus_t status;
			status = osMessageQueuePut(txMsgQueue, &txMsg, 0U, 0U);
			if(status != osOK){
				error();
			}
		}
	}
}

void Rs422Io::run() {
	osStatus_t txMqStatus,txSemaphoreStatus,rxStatus;
	uint8_t readyBuff;
	txMsgQueue= osMessageQueueNew(txSize, sizeof(TxMsg), NULL);
	rxMsgQueue= osMessageQueueNew(rxSize, 1, NULL);
	txSemaphore = osSemaphoreNew(1U, 1U, NULL);

	/* Check the parameters */
	assert_param(txMsgQueue != NULL);
	assert_param(rxMsgQueue != NULL);
	assert_param(txSemaphore != NULL);

	init();
	receive(&rxbuff, 1);
	for(;;){

		txSemaphoreStatus = osSemaphoreAcquire(txSemaphore, 1);
	    if (txSemaphoreStatus == osOK) {
	    	txMqStatus = osMessageQueueGet(txMsgQueue, &txMsg, NULL, 1);
	    	if(txMqStatus == osOK)
	    		Rs422::transmit(txMsg.data,txMsg.size);
	    }

	    rxStatus = osMessageQueueGet(rxMsgQueue, &readyBuff, NULL, 1);   // wait for message
	    if (rxStatus == osOK) {
	    	decodeMsg(readyBuff);
	    }
	}
}

void Rs422Io::rs422RxCpltCallback() {
	if(rxMsgQueue!= NULL)
		if(osMessageQueuePut(rxMsgQueue, &rxbuff, 0U, 0U)!= osOK){
			error();
		}
	receive(&rxbuff, 1);
}

void Rs422Io::rs422TxCpltCallback() {
	if(txMsg.data != nullptr){
		delete [] txMsg.data;
		if(osSemaphoreRelease(txSemaphore) !=  osOK){
			error();
		}
	}
}

void Rs422Io::error() {

}

#endif //LIB_DRIVER_UART_ENABLED
