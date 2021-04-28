/*
 * CanIo.cpp
 *
 *  Created on: Apr 27, 2021
 *      Author: Administrator
 */

#include <CanIo.hpp>

CanIo::CanIo(CAN_HandleTypeDef &hcan,int txSize,int rxSize):Can(hcan){
	this->txSize = txSize;
	this->rxSize = rxSize;
	setThreada();
}

CanIo::~CanIo() {
}

void CanIo::run(){

	if(txMsgQueue == NULL)
		txMsgQueue= osMessageQueueNew(txSize, sizeof(CanMsg_t), NULL);
	if(rxMsgQueue == NULL)
		rxMsgQueue= osMessageQueueNew(rxSize, sizeof(CanMsg_t), NULL);



	setCanDevice();
	init(baudrate);


	filter();
	Can::start();
	startReceive();

	osStatus_t status;
	CAN_TxHeaderTypeDef   TxHeader = {0};
	uint32_t TxMailbox;
	CanMsg_t txMsg;
	CanMsg_t rxMsg;
	for(;;){

		while(HAL_CAN_GetTxMailboxesFreeLevel(&hcan)==0)	//wait for a empty txMailBox
		{
			osDelay(1);
		}

		status = osMessageQueueGet(txMsgQueue, &txMsg, NULL, 0);   // wait for message
	    if (status == osOK) {
	    	TxHeader.StdId = txMsg.id;
	    	TxHeader.DLC = txMsg.length;

	    	/* Start the Transmission process */
	        if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, txMsg.data, &TxMailbox) != HAL_OK)
	        {
	          /* Transmission request Error */
	          Error_Handler();
	        }
	    }

		status = osMessageQueueGet(rxMsgQueue, &rxMsg, NULL, 1);   // wait for message
	    if (status == osOK) {
	    	msgDecode(rxMsg);
	    }
	}
}

void CanIo::setCanDevice(){
	setBaudrate();
}

static const char threadName[] = "CanIo";
void CanIo::setThreada(){
	setName(threadName);
	setStaticSize(512);
}

Can::StatusTypeDef CanIo::filter(){
	return setFilter(nullptr,0);
}

Can::StatusTypeDef CanIo::setFilter(uint16_t* id,int size){
	StatusTypeDef state;
	CAN_FilterTypeDef sFilterConfig;

	sFilterConfig.SlaveStartFilterBank = 14;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig.FilterActivation = ENABLE;

	if(id == nullptr)
	{
		sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
		sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
		sFilterConfig.FilterIdHigh = 0x0000;
		sFilterConfig.FilterIdLow = 0x0000;
		sFilterConfig.FilterMaskIdHigh = 0x0000;
		sFilterConfig.FilterMaskIdLow = 0x0000;
		sFilterConfig.FilterBank = 0;
		state = (StatusTypeDef)HAL_CAN_ConfigFilter(&hcan, &sFilterConfig);
		if (state != OK)
		{
			Can::error();
		}
		return state;
	}else{
		for(int i = 0; i < size; ++i){
			if(size != 0 && (size % 4) == 0){
				state = (StatusTypeDef)HAL_CAN_ConfigFilter(&hcan, &sFilterConfig);
				if (state != OK)
				{
					Can::error();
					return state;
				}
			}
			if(i % 4 == 0){
				sFilterConfig.FilterIdHigh =		id[i] << 5;
				sFilterConfig.FilterIdLow = 		id[i] << 5;
				sFilterConfig.FilterMaskIdHigh =	id[i] << 5;
				sFilterConfig.FilterMaskIdLow = 	id[i] << 5;
			}
			if(i % 4 == 1){
				sFilterConfig.FilterIdLow = 		id[i] << 5;
				sFilterConfig.FilterMaskIdHigh = 	id[i] << 5;
				sFilterConfig.FilterMaskIdLow = 	id[i] << 5;
			}
			if(i % 4 == 2){
				sFilterConfig.FilterMaskIdHigh = 	id[i] << 5;
				sFilterConfig.FilterMaskIdLow = 	id[i] << 5;
			}
			if(i % 4 == 3){
				sFilterConfig.FilterMaskIdLow = 	id[i] << 5;
			}

			sFilterConfig.FilterBank = size / 4;

		}
		state = (StatusTypeDef)HAL_CAN_ConfigFilter(&hcan, &sFilterConfig);
		if (state != OK)
		{
			Can::error();
			return state;
		}
		return state;
	}

}

void CanIo::sendMsg(CanMsg_t msg){
	osMessageQueuePut(txMsgQueue, &msg, 0U, 0U);
}

void CanIo::fifo0callBack(CanIo &canIo){

	CAN_RxHeaderTypeDef Header;
	CanIo::CanMsg_t msg;
	HAL_CAN_GetRxMessage(&canIo.hcan, CAN_RX_FIFO0, &Header, msg.data);
	if((Header.IDE == CAN_ID_STD)&&(Header.RTR == CAN_RTR_DATA)){
		msg.id = Header.StdId;
		msg.length = Header.DLC;
		if(canIo.rxMsgQueue!= NULL)
			osMessageQueuePut(canIo.rxMsgQueue, &msg, 0U, 0U);
	}
}
