/*
 * lcm1602c.c
 *
 *  Created on: 1 nov. 2021
 *      Author: dgfiloso
 */
#include <lcm1602.h>

#define I2C_ADDR		(0x3F << 1)
#define INIT_DELAY_MS	500
#define LCD_DELAY_MS 	5

#define PIN_RS			(1 << 0)
#define PIN_EN    		(1 << 2)
#define BACKLIGHT 		(1 << 3)

// Instruction Set
#define CMD_SCREEN_CLEAR	0b00000001
#define CMD_CURSOR_RETURN	0b00000010
#define CMD_INPUT_SET_INC	0b00000110
#define CMD_INPUT_SET_DEC	0b00000100
#define	CMD_DISPLAY_SWITCH	0b00001000
#define CMD_SHIFT			0b00010000
#define CMD_FUNCTION_SET	0b00100000
#define CMD_CGRAM_AD_SET	0b01000000
#define CMD_DDRAM_AD_SET	0b10000000

// DISPLAY_SWITCH Flags
#define DISPLAY_ON_FLAG		0b00000100
#define CURSOR_ON_FLAG		0b00000010
#define BLINK_ON_FLAG		0b00000001

// SHIFT Flags
#define DISPLAY_SHIFT		0b00001000
#define CURSOR_SHIFT		0b00000000
#define RIGHT_SHIFT			0b00000100
#define LEFT_SHIFT			0b00000000

// FUNCTION_SET Flags
#define DL_8D				0b00010000
#define DL_4D				0b00000000
#define ROWS_2				0b00001000
#define ROWS_1				0b00000000
#define FONT_5x10			0b00000100
#define FONT_5x7			0b00000000

/**
 * Private functions declaration
 */
static HAL_StatusTypeDef LCM1602_SendData(I2C_HandleTypeDef *hi2c, uint8_t byte, uint8_t rs);

/**
 * Public functions definition
 */
HAL_StatusTypeDef LCM1602_Init(I2C_HandleTypeDef *hi2c)
{
	HAL_StatusTypeDef res;

	HAL_Delay(INIT_DELAY_MS);

	//	Set 4-bit mode, 2 lines and 5x7 format
	if ( (res = LCM1602_SendCommand(hi2c, CMD_FUNCTION_SET | DL_4D | ROWS_2 | FONT_5x10)) != HAL_OK)
		return res;
	//	Display and cursor return
	if ( (res = LCM1602_SendCommand(hi2c, CMD_CURSOR_RETURN)) != HAL_OK)
		return res;
	//	Display on, right shift, underline off and blink off
	if ( (res = LCM1602_SendCommand(hi2c, CMD_DISPLAY_SWITCH | DISPLAY_ON_FLAG)) != HAL_OK)
		return res;
	//	Clear display
	if ( (res = LCM1602_SendCommand(hi2c, CMD_SCREEN_CLEAR)) != HAL_OK)
		return res;

	return res;
}

HAL_StatusTypeDef LCM1602_SendCommand(I2C_HandleTypeDef *hi2c, uint8_t command)
{
	return LCM1602_SendData(hi2c, command, 0);
}

HAL_StatusTypeDef LCM1602_DisplayString(I2C_HandleTypeDef *hi2c, uint8_t row, uint8_t col, char* msg, int len)
{
	int i;
	HAL_StatusTypeDef res;
	uint8_t row_offset[] = {0x00, 0x40};

	if (row < 0 || row > 1)
		return HAL_ERROR;

	if (col < 0 || col > 15)
		return HAL_ERROR;

	LCM1602_SendCommand(hi2c, CMD_DDRAM_AD_SET | (col + row_offset[row]));

	for (i = 0; i < len; i++)
	{
		if ( (res = LCM1602_SendData(hi2c, msg[i], PIN_RS)) != HAL_OK)
			return res;
	}

	return res;
}

HAL_StatusTypeDef LCM1602_ClearDisplay(I2C_HandleTypeDef *hi2c)
{
	return LCM1602_SendCommand(hi2c, CMD_SCREEN_CLEAR);
}

/**
 * Private functions definition
 */
static HAL_StatusTypeDef LCM1602_SendData(I2C_HandleTypeDef *hi2c, uint8_t byte, uint8_t rs)
{
	HAL_StatusTypeDef res;
	for(;;) {
		res = HAL_I2C_IsDeviceReady(hi2c, I2C_ADDR, 1, HAL_MAX_DELAY);
		if(res == HAL_OK)
			break;
	}

	uint8_t up = byte & 0xF0;
	uint8_t lo = (byte << 4) & 0xF0;

	uint8_t data_arr[4];
	data_arr[0] = up|rs|BACKLIGHT|PIN_EN;
	data_arr[1] = up|rs|BACKLIGHT;
	data_arr[2] = lo|rs|BACKLIGHT|PIN_EN;
	data_arr[3] = lo|rs|BACKLIGHT;

	res = HAL_I2C_Master_Transmit(hi2c, I2C_ADDR, data_arr, sizeof(data_arr), HAL_MAX_DELAY);
	HAL_Delay(LCD_DELAY_MS);
	return res;
}
