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
		TimeOut,
		StretchError,
		LogicError,
		AckError,
	};

	I2cPin(GPIO_TypeDef  *scl_gpiox, uint16_t scl_pin, GPIO_TypeDef  *sda_gpiox,uint16_t sda_pin);

	void init();



	State write(uint8_t addr, uint8_t* data, int size){
		State state = Ok;
		//step1: start
		state = start();
		if(state != Ok)
			return state;
		//step2: write address
		writeByte(addr << 1 | 0x00);
		state = waitAck();
		if(state != Ok){
			stop();
			return state;
		}


		//step3: write data
		for(int i=0; i< size; ++i){
			writeByte(data[i]);
			state = waitAck();
			if(i == (size-1))
				//step4: stop
				return stop();
			if(state != Ok){
				stop();
				return state;
			}
		}
		return state;
	}

	State read(uint8_t addr, uint8_t* data, int size){
		State state = Ok;
		//step1: start
		state = start();
		if(state != Ok)
			return state;
		//step2: write address
		writeByte(addr << 1 | 0x01);
		state = waitAck();
		if(state != Ok){
			stop();
			return state;
		}


		//step3: write data
		for(int i=0; i< size; ++i){
			readByte(data+i);
			if(i != (size-1)){
				sendAck();
			}
		}

		sendNack();
		//step4: stop
		return stop();
	}

	virtual ~I2cPin();

protected:
	Pin* scl;
	Pin* sda;

	virtual State waitStretchRelease(){
			while(scl->readPin() != Pin::SET){}
			return Ok;
		}

		State scl_raise(){
			State state = Ok;
			scl->setPin();
			waitStretchRelease();
			return state;
		}

		State scl_down(){
			scl->resetPin();
			return Ok;
		}

		State sda_raise(){
			State state = Ok;
			sda->setPin();
			return state;
		}

		State sda_down(){
			sda->resetPin();
			return Ok;
		}

		State start(){
			sda_raise();
			scl_raise();
			if((scl->readPin() != Pin::SET) || (sda->readPin() != Pin::SET)){
				return LogicError;
			}
			sda_down();
			return Ok;
		}

		State stop(){
			scl_down();
			sda->resetPin();
			scl_raise();
			sda->setPin();
			return Ok;
		}
		State writeByte(uint8_t data){
			for(int i = 0; i < 8; ++i){
				scl_down();
				(data&0x80) == 0x80 ? sda_raise():sda_down();
				data <<= 1;
				scl_raise();
			}
			return Ok;
		}

		State readByte(uint8_t* data){
			State state = Ok;
			uint8_t temp = 0,bit;
			for(int i = 0; i < 8; ++i){
				scl_down();
				sda_raise(); //release bus
				scl_raise();
				bit = sda->readPin() == Pin::SET ? 0x01:0x00;
				temp <<= 1;
				temp |= bit;
			}
			*data = temp;
			return state;
		}

		State waitAck(){
			scl_down();
			sda_raise(); //release bus
			scl_raise();
			if(sda->readPin() == Pin::RESET)
				return Ok;
			else
				return AckError;
		}

		State sendAck(){
			scl_down();
			sda_down();
			scl_raise();
			return Ok;
		}

		State sendNack(){
			scl_down();
			sda_raise(); //release bus
			scl_raise();
			return Ok;
		}
};

#endif /* DRIVERS_I2C_PIN_HPP_ */
