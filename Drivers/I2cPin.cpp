/*
 * Si2c.cpp
 *
 *  Created on: May 28, 2021
 *      Author: Administrator
 */

#include <Drivers/I2cPin.hpp>

I2cPin::I2cPin(GPIO_TypeDef  *scl_gpiox, uint16_t scl_pin, GPIO_TypeDef  *sda_gpiox,uint16_t sda_pin) {
	scl = new Pin(scl_gpiox,scl_pin);
	sda = new Pin(sda_gpiox,sda_pin);
}

void I2cPin::init() {
	scl->setPin();
	scl->setModeOutputOD();

	sda->setPin();
	sda->setModeOutputOD();
}

uint8_t I2cPin::sendBit(uint8_t bit) {
	bit == 1 ? scl->setPin():scl->resetPin();
	return sda->readPin() == Pin::SET ? 0x01:0x00;
}

uint8_t I2cPin::readBit() {
	return sda->readPin() == Pin::SET ? 0x01:0x00;
}

I2cPin::~I2cPin() {
	// TODO Auto-generated destructor stub
}

