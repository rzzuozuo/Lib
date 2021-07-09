/*
 * Uart.h
 *
 *  Created on: Apr 29, 2021
 *      Author: Administrator
 */

#ifndef UART_HPP_
#define UART_HPP_

#include "Hal.hpp"

#ifdef HAL_UART_MODULE_ENABLED
#define LIB_DRIVER_UART_ENABLED

class Uart: public Hal {
	friend void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
	friend void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
public:
	enum WordLength:unsigned int{
		BYTES_8 = UART_WORDLENGTH_8B,
		BYTES_9 = UART_WORDLENGTH_9B,
	};

	enum StopBits:unsigned int{
		STOP_1 = UART_STOPBITS_1,
		STOP_2 = UART_STOPBITS_2,
	};

	enum Parity:unsigned int{
		NONE = UART_PARITY_NONE,
		EVEN = UART_PARITY_EVEN,
		ODD = UART_PARITY_ODD,
	};

	Uart(UART_HandleTypeDef &huart);
	virtual ~Uart();

	StatusTypeDef init(USART_TypeDef * uart, uint32_t baudrate = 115200, WordLength wordLength = BYTES_8, StopBits stopBits = STOP_1, Parity parity = NONE);
	virtual void init(){

	}
	StatusTypeDef transmit(uint8_t* data, int size);
	StatusTypeDef receive(uint8_t* data,int size);
private:
	virtual void txCpltCallback(){

	}
	virtual void rxCpltCallback(){

	}
	UART_HandleTypeDef &huart;
	virtual void error(){

	}
};

#endif // HAL_UART_MODULE_ENABLED

#endif /* UART_HPP_ */
