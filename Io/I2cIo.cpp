/*
 * I2c.cpp
 *
 *  Created on: May 31, 2021
 *      Author: Administrator
 */

#include <Io/I2cIo.hpp>

I2cIO::I2cIO(GPIO_TypeDef  *scl_gpiox, uint16_t scl_pin, GPIO_TypeDef  *sda_gpiox,uint16_t sda_pin)
:I2cPin(scl_gpiox, scl_pin, sda_gpiox, sda_pin) {


}

I2cIO::~I2cIO() {

}

