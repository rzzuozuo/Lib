/*
 * UartIo.cpp
 *
 *  Created on: May 13, 2021
 *      Author: Administrator
 */

#include "UartIo.hpp"

#ifdef LIB_DRIVER_UART_ENABLED

UartIo::UartIo(UART_HandleTypeDef &huart,int txSize,int rxSize):Uart(huart),txSize(txSize),rxSize(rxSize){

}

UartIo::~UartIo() {

}

void UartIo::transmit(uint8_t *data, int size) {
	if(txMsgQueue!= NULL){
		TxMsg txMsg = {NULL,0};
		txMsg.data = new uint8_t[size];
		assert_param(txMsg.data != NULL);
		if(txMsg.data != NULL){
			txMsg.size = size;
			for(int i = 0; i < size; ++i){
				txMsg.data[i] = data[i];
			}
			osStatus_t status;
			status = osMessageQueuePut(txMsgQueue, &txMsg, 0U, 0U);
			if(status != osOK){
				Rtos::error();
			}
		}
	}
}

void UartIo::run() {
	osStatus_t txStatus,txMutexStatus,rxStatus;
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

		txStatus = osMessageQueueGet(txMsgQueue, &txMsg, NULL, 1);   // wait for message
	    if (txStatus == osOK) {
	    	txMutexStatus = osSemaphoreAcquire(txSemaphore, osWaitForever);
	    	if(txMutexStatus == osOK)
	    		Uart::transmit(txMsg.data,txMsg.size);
	    }

	    rxStatus = osMessageQueueGet(rxMsgQueue, &readyBuff, NULL, 1);   // wait for message
	    if (rxStatus == osOK) {
	    	decodeMsg(readyBuff);
	    }
	}
}

void UartIo::txCpltCallback() {
	if(txMsg.data != nullptr){
		delete [] txMsg.data;
		if(osSemaphoreRelease(txSemaphore) !=  osOK){
			Thread::error();
		}
	}
}

void UartIo::rxCpltCallback() {
	if(rxMsgQueue!= NULL)
		if(osMessageQueuePut(rxMsgQueue, &rxbuff, 0U, 0U)!= osOK){
			Thread::error();
		}
	receive(&rxbuff, 1);
}

#endif // LIB_DRIVER_UART_ENABLED
