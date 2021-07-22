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
UartIo::TxMsg txMsgdd;
uint32_t newNum,pushNum,popNum,deletNum;
void UartIo::transmit(void *data, int size) {
	if(txMsgQueue!= NULL){
		txMsgdd = {NULL,0};
		txMsgdd.data = new uint8_t[size];
		newNum++;
		assert_param(txMsg.data != NULL);
		if(txMsgdd.data != NULL){
			txMsgdd.size = size;
			for(int i = 0; i < size; ++i){
				txMsgdd.data[i] = ((uint8_t*)data)[i];
			}
			osStatus_t status;
			pushNum++;
			status = osMessageQueuePut(txMsgQueue, &txMsgdd, 0U, 0U);
			if(status != osOK){
				Rtos::error();
			}
		}
	}
}

void UartIo::run1() {
	osStatus_t txStatus,txSemaphoreStatus;

	txMsgQueue= osMessageQueueNew(txSize, sizeof(TxMsg), NULL);
	txSemaphore = osSemaphoreNew(1U, 1U, NULL);

	/* Check the parameters */
	assert_param(txMsgQueue != NULL);

	assert_param(txSemaphore != NULL);

	for(;;){
		txSemaphoreStatus = osSemaphoreAcquire(txSemaphore, osWaitForever);
		   // wait for message
	    if (txSemaphoreStatus == osOK) {

	    	txStatus = osMessageQueueGet(txMsgQueue, &txMsg, NULL, osWaitForever);
	    	if(txStatus == osOK)
	    		Uart::transmit(txMsg.data,txMsg.size);
	    }
	}
}

void UartIo::run2() {
	osStatus_t rxStatus;
	uint8_t readyBuff;

	rxMsgQueue= osMessageQueueNew(rxSize, 1, NULL);


	/* Check the parameters */

	assert_param(rxMsgQueue != NULL);



	receive(&rxbuff, 1);
	for(;;){
	    rxStatus = osMessageQueueGet(rxMsgQueue, &readyBuff, NULL, osWaitForever);   // wait for message
	    if (rxStatus == osOK) {
	    	decodeMsg(readyBuff);
	    }
	}
}

void UartIo::txCpltCallback() {
	if(txMsg.data != nullptr){
		delete [] txMsg.data;
		if(osSemaphoreRelease(txSemaphore) !=  osOK){
		}
	}
}

void UartIo::rxCpltCallback() {
	if(rxMsgQueue!= NULL)
		if(osMessageQueuePut(rxMsgQueue, &rxbuff, 0U, 0U)!= osOK){
		}
	receive(&rxbuff, 1);
}

void UartIo::errorCallback() {
	receive(&rxbuff, 1);
}

#endif // LIB_DRIVER_UART_ENABLED
