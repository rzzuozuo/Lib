/*
 * Can.hpp
 *
 *  Created on: 2021年6月14日
 *      Author: Administrator
 */

#ifndef DRIVERS_CAN_HPP_
#define DRIVERS_CAN_HPP_

#include <Drivers/Can.hpp>
#include <Rtos/Thread.hpp>

class CanRxQueue:public Thread {
public:
	CanRxQueue(Can &can);
	virtual ~CanRxQueue();
protected:
	Can &can;
	void run();
	virtual void msgDecode(Can::Msg* msg){};
private:

};

#endif /* DRIVERS_CAN_HPP_ */
