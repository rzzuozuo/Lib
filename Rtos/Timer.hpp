/*
 * Timer.hpp
 *
 *  Created on: May 27, 2021
 *      Author: Administrator
 */

#ifndef TIMER_HPP_
#define TIMER_HPP_

#include "Rtos.hpp"

void timer_callback(void* argument);

class Timer: public Rtos {
public:
	friend void timer_callback(void* argument);
	Timer();
	virtual ~Timer();


	void _newOnce()
	{
		if(id != NULL)
			osTimerDelete(id);
		id = osTimerNew(timer_callback, osTimerOnce, this, NULL);
		assert(id != NULL);
	}

	void _newPeriodic()
	{
		if(id != NULL)
			osTimerDelete(id);
		id = osTimerNew(timer_callback, osTimerPeriodic, this, NULL);
		assert(id != NULL);
	}

	osStatus_t start(uint32_t tick){
		assert(id != NULL);
		startCallBack();
		return osTimerStart(id, tick);
	}

	osStatus_t stop(){
		assert(id != NULL);

		return osTimerStop(id);
	}

	bool isRunning(){
		assert(id != NULL);

		return (osTimerIsRunning(id) == 1);
	}

	osTimerId_t getId(){
		return id;
	}

	virtual void timeoutCallback(){}
	virtual void startCallBack(){}
	virtual void error(){}
protected:
	osTimerId_t id = NULL;
};

#endif /* TIMER_HPP_ */
