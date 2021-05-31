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
	I2cPin(GPIO_TypeDef  *scl_gpiox, uint16_t scl_pin, GPIO_TypeDef  *sda_gpiox,uint16_t sda_pin);

	void init();

	uint8_t sendBit(uint8_t bit);
	uint8_t readBit();

	virtual ~I2cPin();

protected:
	Pin* scl;
	Pin* sda;
};

#endif /* DRIVERS_I2C_PIN_HPP_ */
