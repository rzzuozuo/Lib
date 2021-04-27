/*
 * Thread.cpp
 *
 *  Created on: Apr 19, 2021
 *      Author: Administrator
 */

#include <Thread.hpp>

Thread::Thread() {

}

Thread::~Thread() {

}

void taskFunction(void* argument){
	Thread* thread =(Thread*)argument;
	thread->run();
}
