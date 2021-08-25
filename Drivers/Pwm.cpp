/*
 * Pwm.cpp
 *
 *  Created on: Jun 5, 2021
 *      Author: Administrator
 */

#include <Drivers/Pwm.hpp>

Pwm::Pwm(TIM_HandleTypeDef *htim):htim(htim) {

}

Pwm::~Pwm() {

}

void Pwm::init() {
}

uint32_t Pwm::getChannel(int index){
	if(index == 1)
		return TIM_CHANNEL_1;
	else if(index == 2)
		return TIM_CHANNEL_2;
	else if(index == 3)
		return TIM_CHANNEL_3;
	else if(index == 4)
		return TIM_CHANNEL_4;
	else
		return TIM_CHANNEL_ALL;
}

void Pwm::start(int index) {
	HAL_TIM_PWM_Start(htim,getChannel(index));
}

void Pwm::setWidth(int index, int width) {
	__HAL_TIM_SET_COMPARE(htim,getChannel(index),width);
}
