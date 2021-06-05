/*
 * I2cHw.h
 *
 *  Created on: May 10, 2021
 *      Author: Administrator
 */

#ifndef I2CHW_H_
#define I2CHW_H_

#include "Hal.hpp"

#ifdef HAL_I2C_MODULE_ENABLED
#define LIB_DRIVER_I2C_ENABLED

class I2cHw: public Hal {
	friend void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c);
	friend void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c);
	friend void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c);
	friend void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c);
public:
	I2cHw(I2C_HandleTypeDef& hi2c);
	virtual ~I2cHw();

	void init();
	void init(uint8_t slaverAddr);

	void transmit(uint16_t DevAddress, uint8_t *pData, uint16_t Size);
	void transmit(uint8_t *pData, uint16_t Size);

	void receive(uint16_t DevAddress, uint8_t *pData, uint16_t Size);
	void receive(uint8_t *pData, uint16_t Size);

private:
	virtual void transmitCpl() = 0;
	virtual void receiveCpl() = 0;
	I2C_HandleTypeDef& handle;
};

#endif /* HAL_I2C_MODULE_ENABLED */
#endif /* I2CHW_H_ */
