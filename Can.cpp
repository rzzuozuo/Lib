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
	// TODO Auto-generated destructor stub
}

Can::StatusTypeDef Can::init(uint32_t Prescaler,uint32_t TimeSeg1, uint32_t TimeSeg2, uint32_t SyncJumpWidth){
	StatusTypeDef state;

	hcan.Instance = CAN1;
	hcan.Init.Prescaler = 8;
	hcan.Init.Mode = CAN_MODE_NORMAL;
	hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan.Init.TimeSeg1 = CAN_BS1_2TQ;
	hcan.Init.TimeSeg2 = CAN_BS2_1TQ;
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
void Can::error(){

}