/*
 * UartIo.hpp
 *
 *  Created on: May 13, 2021
 *      Author: Administrator
 */

#ifndef UARTIO_HPP_
#define UARTIO_HPP_

#include <Drivers/Uart.hpp>
#include <Rtos/ThreadDual.hpp>

#ifdef LIB_DRIVER_UART_ENABLED

class UartIo:public Uart, public ThreadDual{
public:
	UartIo(UART_HandleTypeDef &huart,int txSize,int rxSize);
	virtual ~UartIo();

	struct TxMsg{
		uint8_t* data;
		int size;
	};

	void transmit(void* data,int size);

private:
	int txSize;
	int rxSize;

	//buffer
	uint8_t rxbuff;
	TxMsg txMsg;
	osMessageQueueId_t txMsgQueue = NULL;
	osMessageQueueId_t rxMsgQueue = NULL;
	osSemaphoreId_t txSemaphore = NULL;

	virtual void run1() override;
	virtual void run2() override;

	virtual void decodeMsg(uint8_t data) = 0;

	virtual void txCpltCallback();
	virtual void rxCpltCallback();
};

#endif // LIB_DRIVER_UART_ENABLED

#endif /* UARTIO_HPP_ */
