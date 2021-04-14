/*
 * Pin.hpp
 *
 *  Created on: 2021年4月13日
 *      Author: Administrator
 */

#ifndef PIN_HPP_
#define PIN_HPP_

#include <Gpio.hpp>

class Pin: public Gpio {
public:
	typedef GPIO_InitTypeDef InitTypeDef;

	typedef enum
	{
	  RESET = 0u,
	  SET
	} PinState_t;

	typedef enum{
		LOW = 1u,
		MID,
		HIGH
	} Speed_t;

	typedef enum{
		NO_PULL = 0u,
		PULL_UP,
		PULL_DOWN
	} Pull;

	Pin(GPIO_TypeDef  *GPIOx, uint16_t GPIO_Pin);
	Pin(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init);
	virtual ~Pin();

	PinState_t readPin() const{
		return (PinState_t)HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
	}

	void setPin() const{
		HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
	}

	void resetPin() const{
		HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
	}

	void togglePin() const{
		HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
	}

	StatusTypeDef lockPin() const{
		return (StatusTypeDef)HAL_GPIO_LockPin(GPIOx, GPIO_Pin);
	}

	void setMode(const GPIO_InitTypeDef &GPIO_Init){
		this->GPIO_Init = GPIO_Init;
		HAL_GPIO_Init(GPIOx, &this->GPIO_Init);
	}

	void setModeOutput(bool isPushPull, Pull pull = NO_PULL, Speed_t speed = LOW){
		this->GPIO_Init.Mode = isPushPull?GPIO_MODE_OUTPUT_PP:GPIO_MODE_OUTPUT_OD;
		this->GPIO_Init.Pull = pull;
		this->GPIO_Init.Speed = speed;
		SetMode(this->GPIO_Init);
	}

	void setModeOutputPP(Pull pull = NO_PULL, Speed_t speed = LOW){
		SetModeOutput(true, pull, speed);
	}


	void setModeOutputOD(bool isPullUp = false, Speed_t speed = LOW){
		Pull pull = isPullUp?PULL_UP:NO_PULL;
		SetModeOutput(true, pull, speed);
	}

	void setModeInput(Pull pull = NO_PULL){
		this->GPIO_Init.Mode = GPIO_MODE_INPUT;
		this->GPIO_Init.Pull = pull;
		SetMode(this->GPIO_Init);
	}



protected:
	uint16_t GPIO_Pin;
	GPIO_InitTypeDef GPIO_Init;
};

#endif /* PIN_HPP_ */
