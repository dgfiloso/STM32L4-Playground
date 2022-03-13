/*
 * lcm1602c.h
 *
 *  Created on: 1 nov. 2021
 *      Author: dgfiloso
 */
#ifndef INC_LCM1602_H_
#define INC_LCM1602_H_

#include "main.h"
#include "stm32l4xx_hal.h"

#define DEGREES_SYMBOL	0b11011111

HAL_StatusTypeDef LCM1602_Init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef LCM1602_SendCommand(I2C_HandleTypeDef *hi2c, uint8_t command);
HAL_StatusTypeDef LCM1602_DisplayString(I2C_HandleTypeDef *hi2c, uint8_t row, uint8_t col, char* msg, int len);
HAL_StatusTypeDef LCM1602_ClearDisplay(I2C_HandleTypeDef *hi2c);

#endif /* INC_LCM1602_H_ */
