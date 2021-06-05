/*
 * Rs422.cpp
 *
 *  Created on: Apr 29, 2021
 *      Author: Administrator
 */

#include "Rs422.hpp"

#ifdef LIB_DRIVER_UART_ENABLED

Rs422::Rs422(UART_HandleTypeDef &huart):Uart(huart) {

}

Rs422::~Rs422() {
}

void Rs422::enableTransmit() {
	de->setPin();
}

void Rs422::enableReceive() {
	re->resetPin();
}

void Rs422::disableTransmit() {
	de->resetPin();
}

void Rs422::disableReceive() {
	re->setPin();
}

Rs422::StatusTypeDef Rs422::transmit(uint8_t* data, int size){
	enableTransmit();
	return Uart::transmit(data, size);
}

Rs422::StatusTypeDef Rs422::receive(uint8_t *data, int size) {
	enableReceive();
	return Uart::receive(data, size);
}

void Rs422::txCpltCallback() {
	disableTransmit();
	rs422TxCpltCallback();
}

void Rs422::rxCpltCallback() {
	disableReceive();
	rs422RxCpltCallback();
}

#endif // LIB_DRIVER_UART_ENABLED
