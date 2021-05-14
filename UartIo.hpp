/*
 * UartIo.hpp
 *
 *  Created on: May 13, 2021
 *      Author: Administrator
 */

#ifndef UARTIO_HPP_
#define UARTIO_HPP_

#include <Uart.hpp>
#include <Thread.hpp>



class UartIo:public Thread,public  Uart {
public:
	UartIo(UART_HandleTypeDef &huart,int txSize,int rxSize);
	virtual ~UartIo();

	Uart* uart;

	struct TxMsg{
		uint8_t* data;
		int size;
	};

	void transmit(uint8_t* data,int size);

private:
	int txSize;
	int rxSize;

	//buffer
	uint8_t rxbuff;
	TxMsg txMsg;
	osMessageQueueId_t txMsgQueue = NULL;
	osMessageQueueId_t rxMsgQueue = NULL;
	osSemaphoreId_t txSemaphore = NULL;

	virtual void run() override;

	virtual void decodeMsg(uint8_t data) = 0;

	virtual void txCpltCallback();
	virtual void rxCpltCallback();
};



#endif /* UARTIO_HPP_ */
