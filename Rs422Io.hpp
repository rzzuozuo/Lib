/*
 * Rs422Io.hpp
 *
 *  Created on: Apr 30, 2021
 *      Author: Administrator
 */

#ifndef RS422IO_HPP_
#define RS422IO_HPP_

#include <Rs422.hpp>
#include <Thread.hpp>

class Rs422Io: public Rs422, public Thread {
public:
	struct TxMsg{
		uint8_t* data;
		int size;
	};
	Rs422Io(UART_HandleTypeDef &huart,int txSize, int rxSize);
	virtual ~Rs422Io();

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

	virtual void error();
	virtual void rs422TxCpltCallback() override;
	virtual void rs422RxCpltCallback() override;
	virtual void run() override;

	virtual void decodeMsg(uint8_t data){

	}
};

#endif /* RS422IO_HPP_ */
