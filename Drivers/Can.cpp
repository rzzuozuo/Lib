/*
 * Can.cpp
 *
 *  Created on: Apr 14, 2021
 *      Author: Administrator
 */

#include <Drivers/Can.hpp>
#include <Drivers/templateHandle.hpp>
#ifdef HAL_CAN_MODULE_ENABLED

static Can** classes;
#define MAX_CAN_NUM 2

Can::Can(CAN_HandleTypeDef &hcan):handle(hcan){
	classes = handleRegister<Can,MAX_CAN_NUM>(this);
	queue_id = osMessageQueueNew(16, sizeof(Msg), NULL);
}

Can::~Can() {
	HAL_CAN_DeInit(&handle);
}

Can::StatusTypeDef Can::filter(int fifo){
	StatusTypeDef state;
	CAN_FilterTypeDef sFilterConfig;

	sFilterConfig.FilterBank = 0;
	sFilterConfig.SlaveStartFilterBank = 14;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh = 0x0000;
	sFilterConfig.FilterIdLow = 0x0000;
	sFilterConfig.FilterMaskIdHigh = 0x0000;
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterFIFOAssignment = fifo == 0 ?CAN_RX_FIFO0:CAN_RX_FIFO1;
	sFilterConfig.FilterActivation = ENABLE;
	state = (StatusTypeDef)HAL_CAN_ConfigFilter(&handle, &sFilterConfig);
	if (state != OK)
	{
		error();
	}
	return state;
}

Can::StatusTypeDef Can::filter(uint16_t* id,int size,int fifo){
	StatusTypeDef state;
	CAN_FilterTypeDef sFilterConfig;

	sFilterConfig.FilterFIFOAssignment = fifo == 0 ?CAN_RX_FIFO0:CAN_RX_FIFO1;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.SlaveStartFilterBank = 14;

	if(id == nullptr)
	{
		sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
		sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
		sFilterConfig.FilterIdHigh = 0x0000;
		sFilterConfig.FilterIdLow = 0x0000;
		sFilterConfig.FilterMaskIdHigh = 0x0000;
		sFilterConfig.FilterMaskIdLow = 0x0000;
		sFilterConfig.FilterBank = 0;
		state = (StatusTypeDef)HAL_CAN_ConfigFilter(&handle, &sFilterConfig);
		if (state != OK)
		{
			Can::error();
		}
		return state;
	}else{

		sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;
		sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;
		//size : ID列表大小
		//configSize : 需要配置的大小，4的倍数
		int filterBank = fifo == 0 ? 0:sFilterConfig.SlaveStartFilterBank;
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
				 state = (StatusTypeDef)HAL_CAN_ConfigFilter(&handle, &sFilterConfig);
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

Can::StatusTypeDef Can::start(){
	StatusTypeDef state;
	state = (StatusTypeDef)HAL_CAN_Start(&handle);
	if (state != OK)
	{
		error();
	}
	return state;
}


/**
 * @brief  Rx FIFO 0 message pending callback.
 * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @retval None
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	Can* can = getClass(classes, hcan,MAX_CAN_NUM);

	if(can != nullptr){
		HAL_CAN_GetRxMessage(can->getHandle(), CAN_RX_FIFO0, &can->fifo0Header, can->fifo0Data);
		can->fifo0Callback();
	}
}

/**
 * @brief  Rx FIFO 0 message pending callback.
 * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @retval None
 */
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	Can* can = getClass(classes, hcan,MAX_CAN_NUM);

	if(can != nullptr){
		HAL_CAN_GetRxMessage(can->getHandle(), CAN_RX_FIFO0, &can->fifo0Header, can->fifo0Data);
		can->fifo1Callback();
	}
}

Can::StatusTypeDef Can::activateNotification(int fifo) {
	StatusTypeDef state;

	/* Activate CAN RX notification */
	state = (StatusTypeDef)HAL_CAN_ActivateNotification(&handle, fifo == 0?CAN_IT_RX_FIFO0_MSG_PENDING:CAN_IT_RX_FIFO1_MSG_PENDING);

	if (state != OK)
	{
		error();
	}
	return state;
}

uint32_t Can::getTxMailboxesFreeLevel() {
	return HAL_CAN_GetTxMailboxesFreeLevel(&handle);
}

Can::StatusTypeDef Can::addTxMessage(uint16_t id, uint8_t* data, int size){
	txHeader.StdId = id;
	txHeader.DLC = size < 9 ? size : 8;
	txHeader.ExtId = 0;
	txHeader.IDE = CAN_ID_STD;
	txHeader.RTR = CAN_RTR_DATA;
	txHeader.TransmitGlobalTime = DISABLE;
		return (StatusTypeDef)HAL_CAN_AddTxMessage(&handle, &txHeader, data,&txMailbox);
}

void Can::fifo0Callback() {

	putMessage(&fifo0Header, fifo0Data);
}

void Can::fifo1Callback() {

	putMessage(&fifo0Header, fifo0Data);
}

void Can::putMessage(
		CAN_RxHeaderTypeDef* pHeader,
		uint8_t* data) {

	Can::Msg msg;

	msg.ide = (pHeader->IDE != 0);
	if(msg.ide)
		msg.id = pHeader->ExtId;
	else
		msg.id = pHeader->StdId;

	msg.rtr = (pHeader->RTR != 0);
	if(!msg.rtr){
		msg.dlc = pHeader->DLC;
		for(int i = 0; i < 8; ++i){
			msg.data[i] = i < msg.dlc ? data[i] : 0;
		}
	}

	osMessageQueuePut(queue_id, &msg, 0U, 0U);
}
#endif // HAL_CAN_MODULE_ENABLED

