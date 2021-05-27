/*
 * Thread.hpp
 *
 *  Created on: Apr 19, 2021
 *      Author: Administrator
 */

#ifndef APPLICATION_USER_APP_THREAD_HPP_
#define APPLICATION_USER_APP_THREAD_HPP_

#include <Rtos.hpp>

void taskFunction(void* argument);

class Thread: public Rtos {
	friend void taskFunction(void* argument);
public:
	Thread();
	virtual ~Thread();

	void setName(const char* name){
		attributes.name = name;
	}

	void setPriority(osPriority_t priority){
		attributes.priority = priority;
	}

	void setStaticSize(uint32_t size){
		attributes.stack_size = size;
	}

	void start(){
		id = osThreadNew(taskFunction, this, &attributes);
		if(id == nullptr)
			error();
	}

protected:
	osThreadId_t id = NULL;
	virtual void error(){

	}
private:
	osThreadAttr_t attributes = {nullptr, osThreadDetached, nullptr, 0, nullptr, 0, osPriorityNormal, 0, 0};
	virtual void run(){
		for(;;){
			delayMs(1);
		}
	}
};




#endif /* APPLICATION_USER_APP_THREAD_HPP_ */
