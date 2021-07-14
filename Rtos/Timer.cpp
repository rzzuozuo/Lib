/*
 * Timer.cpp
 *
 *  Created on: May 27, 2021
 *      Author: Administrator
 */

#include "Timer.hpp"

Timer::Timer(bool isPeriod) {
	if(isPeriod)
		_newPeriodic();
	else
		_newOnce();
}

Timer::~Timer() {
	osTimerDelete(id);
}


void timer_callback(void* argument){
	assert(argument != NULL);

	((Timer*)argument)->timeoutCallback();
}
