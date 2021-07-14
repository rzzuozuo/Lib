/*
 * Thread.cpp
 *
 *  Created on: Apr 19, 2021
 *      Author: Administrator
 */

#include <Rtos/ThreadDual.hpp>

ThreadDual::ThreadDual() {

}

ThreadDual::~ThreadDual() {

}

void taskFunction1(void* argument){
	ThreadDual* thread =(ThreadDual*)argument;
	thread->run1();
}

void taskFunction2(void* argument){
	ThreadDual* thread =(ThreadDual*)argument;
	thread->run2();
}
