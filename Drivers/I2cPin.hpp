/*
 * I2c.hpp
 *
 *  Created on: May 28, 2021
 *      Author: Administrator
 */

#ifndef DRIVERS_I2C_PIN_HPP_
#define DRIVERS_I2C_PIN_HPP_

#include <Drivers/Pin.hpp>

class I2cPin {
public:

	enum State{
		Ok,
		Error,
	};

	I2cPin(GPIO_TypeDef  *scl_gpiox, uint16_t scl_pin, GPIO_TypeDef  *sda_gpiox,uint16_t sda_pin);

	void init();

	uint8_t sendBit(uint8_t bit);
	uint8_t readBit();

	State start(){
		if((scl->readPin() != Pin::SET) || (sda->readPin() != Pin::SET)){
			return Error;
		}

		sda->resetPin();
		scl->resetPin();

		return Ok;
	}

	State stop(){
		scl->setPin();
		sda->setPin();
		return Ok;
	}

	virtual ~I2cPin();

protected:
	Pin* scl;
	Pin* sda;
};

#endif /* DRIVERS_I2C_PIN_HPP_ */
