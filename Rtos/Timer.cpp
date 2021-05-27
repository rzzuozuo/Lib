/*
 * Timer.cpp
 *
 *  Created on: May 27, 2021
 *      Author: Administrator
 */

#include "Timer.hpp"

Timer::Timer() {
}

Timer::~Timer() {
	osTimerDelete(id);
}


void timer_callback(void* argument){
	assert(argument != NULL);

	((Timer*)argument)->callback();
}
