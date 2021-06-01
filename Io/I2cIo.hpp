/*
 * I2c.hpp
 *
 *  Created on: May 31, 2021
 *      Author: Administrator
 */

#ifndef IO_I2CIO_HPP_
#define IO_I2CIO_HPP_

#include <Drivers/I2cPin.hpp>
#include <Rtos/Rtos.hpp>

class I2cIO: public I2cPin ,public Rtos{
public:
	I2cIO(GPIO_TypeDef  *scl_gpiox, uint16_t scl_pin, GPIO_TypeDef  *sda_gpiox,uint16_t sda_pin);
	virtual ~I2cIO();

private:
	virtual State waitStretchRelease(){
		uint32_t time = 0;
		while(scl->readPin() != Pin::SET)
		{
			Rtos::delayMs(1);
			time++;
//			if(time > 25)
//				return TimeOut;
		}
		return Ok;
	}
};

#endif /* IO_I2CIO_HPP_ */
