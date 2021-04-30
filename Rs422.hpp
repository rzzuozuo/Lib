/*
 * Rs422.hpp
 *
 *  Created on: Apr 29, 2021
 *      Author: Administrator
 */

#ifndef RS422_HPP_
#define RS422_HPP_

#include <Pin.hpp>
#include <Uart.hpp>

class Rs422: public Uart {
public:
	Rs422(UART_HandleTypeDef &huart);
	virtual ~Rs422();

	StatusTypeDef transmit(uint8_t* data, int size);
	StatusTypeDef receive(uint8_t* data, int size);
private:
	void enableTransmit();
	void enableReceive();
	void disableTransmit();
	void disableReceive();

	Pin* re;
	Pin* de;

	void rxCpltCallback() override;
	void txCpltCallback() override;

	virtual void rs422TxCpltCallback(){

	}

	virtual void rs422RxCpltCallback(){

	}
};

#endif /* RS422_HPP_ */
