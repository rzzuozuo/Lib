/*
 * Thread.hpp
 *
 *  Created on: Apr 19, 2021
 *      Author: Administrator
 */

#ifndef APPLICATION_USER_APP_THREADDUAL_HPP_
#define APPLICATION_USER_APP_THREADDUAL_HPP_

#include "Rtos.hpp"

void taskFunction1(void* argument);
void taskFunction2(void* argument);

class ThreadDual: public Rtos {
	friend void taskFunction1(void* argument);
	friend void taskFunction2(void* argument);
public:
	ThreadDual();
	virtual ~ThreadDual();

	void setName(int idx, const char* name){
		attributes[idx].name = name;
	}

	void setPriority(int idx, osPriority_t priority){
		attributes[idx].priority = priority;
	}

	void setStackSize(int idx, uint32_t size){
		attributes[idx].stack_size = size;
	}

	void start(int idx){
		if(idx == 0)
			id[0] = osThreadNew(taskFunction1, this, &attributes[0]);
		else
			id[1] = osThreadNew(taskFunction2, this, &attributes[1]);
		assert(id[idx] != NULL);
		if(id[idx] == nullptr)
			error();
	}

	void start();

protected:
	osThreadId_t id[2] = {0};

	virtual void run1(){
		for(;;){
			delayMs(1);
		}
	}

	virtual void run2(){
		for(;;){
			delayMs(1);
		}
	}
private:
	osThreadAttr_t attributes[2] = {
			{nullptr, osThreadDetached, nullptr, 0, nullptr, 0, osPriorityNormal, 0, 0},
			{nullptr, osThreadDetached, nullptr, 0, nullptr, 0, osPriorityNormal, 0, 0}
	};
};




#endif /* APPLICATION_USER_APP_THREAD_HPP_ */
