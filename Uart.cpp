/*
 * Uart.cpp
 *
 *  Created on: Apr 29, 2021
 *      Author: Administrator
 */

#include <Uart.hpp>

#define MAX_UART_NUM 	8
static Uart* handles[MAX_UART_NUM] = {0};

Uart::Uart(UART_HandleTypeDef &huart):huart(huart) {
	for(int i = 0; i < MAX_UART_NUM ;i++){
		if(handles[i] == NULL){
			handles[i] = this;
			break;
		}
	}
}

Uart::~Uart() {
	HAL_UART_DeInit(&huart);
}

Uart::StatusTypeDef Uart::init(USART_TypeDef * uart,uint32_t baudrate, WordLength wordLength , StopBits stopBits, Parity parity){
	StatusTypeDef status;

	huart.Instance = uart;
	huart.Init.BaudRate = baudrate;
	huart.Init.WordLength = wordLength;
	huart.Init.StopBits = stopBits;
	huart.Init.Parity = parity;
	huart.Init.Mode = UART_MODE_TX_RX;
	huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart.Init.OverSampling = UART_OVERSAMPLING_16;

	status = (StatusTypeDef)HAL_UART_Init(&huart);
	if ( status!= OK)
	{
		error();
	}
	return status;
}


Uart::StatusTypeDef Uart::receive(uint8_t* data,int size){
	__HAL_UART_CLEAR_OREFLAG(&huart);
	return (StatusTypeDef)HAL_UART_Receive_IT(&huart, data , size);
}

Uart::StatusTypeDef  Uart::transmit(uint8_t* data, int size){
	return (StatusTypeDef)HAL_UART_Transmit_IT(&huart, data , size);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	for(int i = 0; (handles[i] != NULL)&& (i < MAX_UART_NUM); ++i){
		if(handles[i]->huart.Instance == huart->Instance){
			handles[i]->rxCpltCallback();
			break;
		}
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	for(int i = 0; (handles[i] != NULL)&& (i < MAX_UART_NUM); ++i){
		if(handles[i]->huart.Instance == huart->Instance){
			handles[i]->txCpltCallback();
			break;
		}
	}
}

