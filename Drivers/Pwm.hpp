/*
 * Pwm.hpp
 *
 *  Created on: Jun 5, 2021
 *      Author: Administrator
 */

#ifndef DRIVERS_PWM_HPP_
#define DRIVERS_PWM_HPP_

#include <Drivers/Hal.hpp>

#ifdef HAL_TIM_MODULE_ENABLED

class Pwm: public Hal {
public:
	Pwm(TIM_HandleTypeDef *htim);
	virtual ~Pwm();

	virtual void init();

	void start(int index);

	void setWidth(int index, int width);
protected:
	TIM_HandleTypeDef *htim;

private:
	uint32_t getChannel(int index);
};

#endif /* HAL_TIM_MODULE_ENABLED */
#endif /* DRIVERS_PWM_HPP_ */
