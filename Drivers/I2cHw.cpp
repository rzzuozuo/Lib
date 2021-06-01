/*
 * I2cHw.cpp
 *
 *  Created on: May 10, 2021
 *      Author: Administrator
 */

#include "I2cHw.h"
#define MAX_I2C_NUM		4
static I2cHw* handles[MAX_I2C_NUM] = {0};

I2cHw::I2cHw(I2C_HandleTypeDef& hi2c):handle(hi2c) {
	for(int i = 0; i < MAX_I2C_NUM ;i++){
		if(handles[i] == NULL){
			handles[i] = this;
			break;
		}
	}
}

I2cHw::~I2cHw() {
}

void I2cHw::init() {

}

void I2cHw::init(uint8_t slaverAddr) {
}

void I2cHw::transmit(uint16_t DevAddress, uint8_t *pData, uint16_t Size) {
	HAL_I2C_Master_Transmit_IT(&handle, DevAddress << 1, pData, Size);
}

void I2cHw::transmit(uint8_t *pData, uint16_t Size) {
	HAL_I2C_Slave_Transmit_IT(&handle, pData, Size);
}

void I2cHw::receive(uint16_t DevAddress, uint8_t *pData, uint16_t Size) {
	HAL_I2C_Master_Receive_IT(&handle, DevAddress << 1, pData, Size);
}

void I2cHw::receive(uint8_t *pData, uint16_t Size) {
	HAL_I2C_Slave_Receive_IT(&handle, pData, Size);
}


void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *handle){
	for(int i = 0; (handles[i] != NULL)&& (i < MAX_I2C_NUM); ++i){
		if(handles[i]->handle.Instance == handle->Instance){
			handles[i]->transmitCpl();
			break;
		}
	}
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *handle){
	for(int i = 0; (handles[i] != NULL)&& (i < MAX_I2C_NUM); ++i){
		if(handles[i]->handle.Instance == handle->Instance){
			handles[i]->transmitCpl();
			break;
		}
	}
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *handle){
	for(int i = 0; (handles[i] != NULL)&& (i < MAX_I2C_NUM); ++i){
		if(handles[i]->handle.Instance == handle->Instance){
			handles[i]->receiveCpl();
			break;
		}
	}
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *handle){
	for(int i = 0; (handles[i] != NULL)&& (i < MAX_I2C_NUM); ++i){
		if(handles[i]->handle.Instance == handle->Instance){
			handles[i]->receiveCpl();
			break;
		}
	}
}
