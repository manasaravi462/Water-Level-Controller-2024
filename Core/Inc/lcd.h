/*
 * lcd.h
 *
 *  Created on: 15/11/2023
 *  //Modification Date : 07-02-2024
 *
 *      Author: Manasa R
 */

/**
 ******************************************************************************
 * @file    lcd.h
 * @brief   This file contains functions prototype and macros
 belongs to the 16*2 LCD display .
 ******************************************************************************/

#ifndef LCD_H_
#define LCD_H_

/*Includes-----------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "string.h"
#include "stdio.h"
#include "main.h"
/*Includes end-----------------------------------------------*/


// For row start addresses
extern const uint8_t C__ROW_16[];

/* Command register ---------------------------------------------*/
#define CLEAR_DISPLAY 0x01

#define RETURN_HOME 0x02

#define ENTRY_MODE_SET 0x04
#define OPT_S	0x01					// Shift entire display to right
#define OPT_INC 0x02					// Cursor increment

#define DISPLAY_ON_OFF_CONTROL 0x08
#define OPT_D	0x04					// Turn on display
#define OPT_C	0x02					// Turn on cursor
#define OPT_B 	0x01					// Turn on cursor blink
#define LEFT_SHIFT 0x1C

#define CURSOR_DISPLAY_SHIFT 0x10		// Move and shift cursor
#define OPT_SC 0x08
#define OPT_RL 0x04

#define FUNCTION_SET 0x20
#define OPT_DL 0x10						// Set interface data length
#define OPT_N 0x08						// Set number of display lines
#define OPT_F 0x04						// Set alternate font
#define SETCGRAM_ADDR 0x040
#define SET_DDRAM_ADDR 0x80				// Set DDRAM address

/* Helper macros -------------------------------------------------------------------*/
#define DELAY(X) HAL_Delay(X)

/* LCD defines---------------------------------------------------------*/
#define LCD_NIB 4
#define LCD_BYTE 8
#define LCD_DATA_REG 1
#define LCD_COMMAND_REG 0

/* LCD typedefs -----------------------------------------------------*/
#define Lcd_PortType GPIO_TypeDef*
#define Lcd_PinType uint16_t

typedef enum {
	LCD_4_BIT_MODE, LCD_8_BIT_MODE
} LcdModeTypeDef;

typedef struct {
	Lcd_PortType lcd_data_port[4];
	Lcd_PinType lcd_data_pin[4];

	Lcd_PortType lcd_rs_port;
	Lcd_PinType lcd_rs_pin;

	Lcd_PortType lcd_en_port;
	Lcd_PinType lcd_en_pin;

	LcdModeTypeDef lcd_mode;

} Lcd_HandleTypeDef;

/* Public Functions ----------------------------------------------*/
void LcdInit(Lcd_HandleTypeDef *lcd);
void LcdInt(Lcd_HandleTypeDef *lcd, int number);
void LcdString(Lcd_HandleTypeDef *lcd, char *string);
void LcdCursor(Lcd_HandleTypeDef *lcd, uint8_t row, uint8_t col);
Lcd_HandleTypeDef LcdCreate(
Lcd_PortType lcd_port[4], Lcd_PinType lcd_pin[4],
Lcd_PortType lcd_rs_port, Lcd_PinType lcd_rs_pin,
Lcd_PortType lcd_en_port, Lcd_PinType lcd_en_pin, LcdModeTypeDef lcd_mode);
void LcdClear(Lcd_HandleTypeDef *lcd);

/*functions--------------------------------*/

Lcd_HandleTypeDef LcdInitialize();
void LcdClearBit(Lcd_HandleTypeDef *lcd, uint8_t row, uint8_t col);
void LcdCursor(Lcd_HandleTypeDef *lcd, uint8_t row, uint8_t col);
void DisplayClearInAuto(Lcd_HandleTypeDef *lcd);
void DisplayClearInManual(Lcd_HandleTypeDef *lcd);
void DisplayLevel(Lcd_HandleTypeDef *lcd, uint8_t levelInpercent);

#endif
