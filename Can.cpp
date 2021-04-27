/*
 * Can.cpp
 *
 *  Created on: Apr 14, 2021
 *      Author: Administrator
 */

#include "Can.hpp"

Can::Can(CAN_HandleTypeDef &hcan):hcan(hcan){

}

Can::~Can() {
	HAL_CAN_DeInit(&hcan);
}

Can::StatusTypeDef Can::init(uint32_t Prescaler,uint32_t TimeSeg1, uint32_t TimeSeg2, uint32_t SyncJumpWidth){
	StatusTypeDef state;

	assert_param(Prescaler > 0);
	assert_param(TimeSeg1 > 0);
	assert_param(TimeSeg2 > 0);
	assert_param(SyncJumpWidth > 0);

	hcan.Instance = CAN1;
	hcan.Init.Prescaler = Prescaler;
	hcan.Init.Mode = CAN_MODE_NORMAL;
	hcan.Init.SyncJumpWidth = ((SyncJumpWidth-1) << CAN_BTR_SJW_Pos) & CAN_BTR_SJW_Msk;
	hcan.Init.TimeSeg1 = ((TimeSeg1-1) << CAN_BTR_TS1_Pos) & CAN_BTR_TS1_Msk;
	hcan.Init.TimeSeg2 = ((TimeSeg2-1) << CAN_BTR_TS2_Pos) & CAN_BTR_TS2_Msk;;
	hcan.Init.TimeTriggeredMode = DISABLE;
	hcan.Init.AutoBusOff = ENABLE;
	hcan.Init.AutoWakeUp = DISABLE;
	hcan.Init.AutoRetransmission = ENABLE;
	hcan.Init.ReceiveFifoLocked = DISABLE;
	hcan.Init.TransmitFifoPriority = DISABLE;

	state = (StatusTypeDef)HAL_CAN_Init(&hcan);
	if (state != OK)
	{
		error();
	}
	return state;
}



Can::StatusTypeDef Can::filter(){
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
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig.FilterActivation = ENABLE;
	state = (StatusTypeDef)HAL_CAN_ConfigFilter(&hcan, &sFilterConfig);
	if (state != OK)
	{
		error();
	}
	return state;
}

Can::StatusTypeDef Can::start(){
	StatusTypeDef state;

#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
	std::function<void(CAN_HandleTypeDef *)> callabck = std::bind(&Can::Fifo0MsgPendingCallback,this,std::placeholders::_1);

	//callabck(&this->hcan);

	state =  (StatusTypeDef)HAL_CAN_RegisterCallback(&hcan, HAL_CAN_RX_FIFO0_MSG_PENDING_CB_ID, (Fifo0MsgPendingCallback_t) &callabck);

	if (state != OK)
	{
		error();
		return state;
	}
#endif

	state = (StatusTypeDef)HAL_CAN_Start(&hcan);
	if (state != OK)
	{
		error();
	}
	return state;
}

Can::StatusTypeDef Can::startReceive(){
	StatusTypeDef state;

	/* Activate CAN RX notification */
	state = (StatusTypeDef)HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);

	if (state != OK)
	{
		error();
	}
	return state;
}

void Can::Fifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	if(hcan->Instance == this->hcan.Instance){
		StatusTypeDef state;
		CAN_RxHeaderTypeDef pHeader;
		uint8_t aData[8];
		state = (StatusTypeDef) HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &pHeader, aData);
		if (state != OK)
		{
			error();
		}
	}
}
void Can::error(){

}



