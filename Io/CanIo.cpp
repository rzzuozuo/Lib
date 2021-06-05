/*
 * CanIo.cpp
 *
 *  Created on: Apr 27, 2021
 *      Author: Administrator
 */

#include "CanIo.hpp"

#ifdef LIB_DRIVER_CAN_ENABLED

#define MAX_CAN_NUM	2
static CanIo* handles[MAX_CAN_NUM] = {0};

CanIo::CanIo(CAN_HandleTypeDef &hcan,int txSize,int rxSize):Can(hcan){

	for(int i = 0; i < MAX_CAN_NUM ;i++){
		if(handles[i] == NULL){
			handles[i] = this;
			break;
		}
	}

	this->txSize = txSize;
	this->rxSize = rxSize;
	setThread();
}

CanIo::~CanIo() {
}

void CanIo::init(){
	setCanDevice();
	Can::init(baudrate);
}

void CanIo::run(){

	if(txMsgQueue == NULL)
		txMsgQueue= osMessageQueueNew(txSize, sizeof(CanMsg_t), NULL);
	if(rxMsgQueue == NULL)
		rxMsgQueue= osMessageQueueNew(rxSize, sizeof(CanMsg_t), NULL);

	init();

	filter();
	Can::start();
	startReceive();

	osStatus_t txStatus,rxStatus;
	CAN_TxHeaderTypeDef   TxHeader = {0};
	uint32_t TxMailbox;
	CanMsg_t txMsg;
	CanMsg_t rxMsg;
	for(;;){

		while(HAL_CAN_GetTxMailboxesFreeLevel(&hcan)==0)	//wait for a empty txMailBox
		{
			osDelay(1);
		}

		txStatus = osMessageQueueGet(txMsgQueue, &txMsg, NULL, 0);   // wait for message
	    if (txStatus == osOK) {
	    	TxHeader.StdId = txMsg.id;
	    	TxHeader.DLC = txMsg.length;

	    	/* Start the Transmission process */
	        if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, txMsg.data, &TxMailbox) != HAL_OK)
	        {
	          /* Transmission request Error */
	          Error_Handler();
	        }
	    }

	    rxStatus = osMessageQueueGet(rxMsgQueue, &rxMsg, NULL, 1);   // wait for message
	    if (rxStatus == osOK) {
	    	msgDecode(rxMsg);
	    }
	}
}

void CanIo::setCanDevice(){
	setBaudrate();
}

static const char threadName[] = "CanIo";
void CanIo::setThread(){
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
		//size : ID列表大小
		//configSize : 需要配置的大小，4的倍数
		int filterBank = 0;
		int configSize = 0;

		if((size % 4) != 0)
			configSize = (size / 4 +1)*4;// 对齐 宽度为4
		else
			configSize = size;


		for(int i = 0; i < configSize; ++i){

			 if((i%4) == 0)
				 sFilterConfig.FilterIdHigh = i < size ? id[i] << 5 : id[size - 1] << 5;
			 else if((i%4) == 1)
				 sFilterConfig.FilterIdLow = i < size ? id[i] << 5 : id[size - 1] << 5;
			 else if((i%4) == 2)
				 sFilterConfig.FilterMaskIdHigh = i < size ? id[i] << 5 : id[size - 1] << 5;
			 else if((i%4) == 3)
			 {
				 sFilterConfig.FilterMaskIdLow = i < size ? id[i] << 5 : id[size - 1] << 5;
				 sFilterConfig.FilterBank = filterBank;
				 state = (StatusTypeDef)HAL_CAN_ConfigFilter(&hcan, &sFilterConfig);
				 if (state != OK)
				 {
					 Can::error();
					 return state;
				 }
				 filterBank ++;
			 }
		}
		return state;
	}

}

void CanIo::sendMsg(CanMsg_t msg){
	osMessageQueuePut(txMsgQueue, &msg, 0U, 0U);
}

void CanIo::fifo0callBack(CanMsg_t &msg){
	if(rxMsgQueue!= NULL)
		osMessageQueuePut(rxMsgQueue, &msg, 0U, 0U);
}

/**
  * @brief  Rx FIFO 0 message pending callback.
  * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval None
  */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef Header;
	CanIo::CanMsg_t msg;
	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &Header, msg.data);

	if((Header.IDE == CAN_ID_STD)&&(Header.RTR == CAN_RTR_DATA)){
		msg.id = Header.StdId;
		msg.length = Header.DLC;
	}

	for(int i = 0; (handles[i] != NULL)&& (i < MAX_CAN_NUM); ++i){
		if(handles[i]->hcan.Instance == hcan->Instance){
			handles[i]->fifo0callBack(msg);
		}
	}
}

#endif //LIB_DRIVER_CAN_ENABLED
