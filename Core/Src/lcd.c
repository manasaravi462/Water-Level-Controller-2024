/*
 * lcd.c
 *
 *    Created on: 15/11/2023
 *      Author: Manasa R
 *//**
  ******************************************************************************
  * @file         lcd.c
  * @brief        This file provides code for the LCD Initialization and function definition
  ******************************************************************************/

#include "lcd.h"
extern Lcd_HandleTypeDef lcd;

/*Private Variables-----------------------------------*/
const uint8_t C__ROW_16[] = { 0x00, 0x40, 0x10, 0x50 };
static uint8_t S__clearState = 0;
/*Private Variables end-----------------------------------*/


/* Static declarations ----------------------------------------*/

static void LcdWriteData(Lcd_HandleTypeDef *lcd, uint8_t data);
static void LcdWriteCommand(Lcd_HandleTypeDef *lcd, uint8_t command);
static void LcdWrite(Lcd_HandleTypeDef *lcd, uint8_t data, uint8_t len);

/* Function definitions ---------------------------------------*/

/**
 * Create new Lcd_HandleTypeDef and initialize the Lcd
 */
Lcd_HandleTypeDef LcdCreate(
Lcd_PortType lcd_port[4], Lcd_PinType lcd_pin[4],
Lcd_PortType rs_port, Lcd_PinType rs_pin,
Lcd_PortType en_port, Lcd_PinType en_pin, LcdModeTypeDef mode) {

	Lcd_HandleTypeDef lcd = {

	.lcd_mode = mode, .lcd_en_pin = en_pin, .lcd_en_port = en_port,
			.lcd_rs_pin = rs_pin, .lcd_rs_port = rs_port };

	for (int i = 0; i < 4; i++) {
		lcd.lcd_data_port[i] = lcd_port[i];
		lcd.lcd_data_pin[i] = lcd_pin[i];
	}

	LcdInit(&lcd);

	return lcd;
}

/**
 * Initialize 16x2-lcd without cursor
 */
void LcdInit(Lcd_HandleTypeDef *lcd) {
	if (lcd->lcd_mode == LCD_4_BIT_MODE) {
		LcdWriteCommand(lcd, 0x33);
		LcdWriteCommand(lcd, 0x32);
		LcdWriteCommand(lcd, FUNCTION_SET | OPT_N);			// 4-bit mode
	} else
		LcdWriteCommand(lcd, FUNCTION_SET | OPT_DL | OPT_N);

	LcdWriteCommand(lcd, CLEAR_DISPLAY);						// Clear screen
	LcdWriteCommand(lcd, DISPLAY_ON_OFF_CONTROL | OPT_D);// Lcd-on, cursor-off, no-blink
	LcdWriteCommand(lcd, ENTRY_MODE_SET | OPT_INC);		// Increment cursor
}

/******* Lcd port and pin define****/

Lcd_HandleTypeDef LcdInitialize(void) {
	Lcd_PortType ports[] = { D4_GPIO_Port, D5_GPIO_Port, D6_GPIO_Port,
	D7_GPIO_Port };
	// Lcd_PinType pins[] = {D4_Pin, D5_Pin, D6_Pin, D7_Pin};
	Lcd_PinType pins[] = { D4_Pin, D5_Pin, D6_Pin, D7_Pin };
	// Lcd_create(ports, pins, RS_GPIO_Port, RS_Pin, EN_GPIO_Port, EN_Pin, LCD_4_BIT_MODE);
	return LcdCreate(ports, pins, RSDisplay_GPIO_Port, RSDisplay_Pin,
	enableDisplay_GPIO_Port, enableDisplay_Pin, LCD_4_BIT_MODE);
}

/**
 * Write a number on the current position
 */
void LcdInt(Lcd_HandleTypeDef *lcd, int number) {
	char buffer[11] = "";
	sprintf(buffer, "%d", number);

	LcdString(lcd, buffer);
}

/**
 * Write a string on the current position
 */
void LcdString(Lcd_HandleTypeDef *lcd, char *string) {
	for (uint8_t i = 0; i < strlen(string); i++) {
		LcdWriteData(lcd, string[i]);
	}
}

/**
 * Set the cursor position
 */
void LcdCursor(Lcd_HandleTypeDef *lcd, uint8_t row, uint8_t col) {

	LcdWriteCommand(lcd, SET_DDRAM_ADDR + C__ROW_16[row] + col);
}

/**
 * Clear the screen
 */
void LcdClear(Lcd_HandleTypeDef *lcd) {
	LcdWriteCommand(lcd, CLEAR_DISPLAY);
}

/************************************** Static function definition **************************************/

/**
 * Write a byte to the command register
 */
void LcdWriteCommand(Lcd_HandleTypeDef *lcd, uint8_t command) {
	HAL_GPIO_WritePin(lcd->lcd_rs_port, lcd->lcd_rs_pin, LCD_COMMAND_REG);// Write to command register

	if (lcd->lcd_mode == LCD_4_BIT_MODE) {
		LcdWrite(lcd, (command >> 4), LCD_NIB);
		LcdWrite(lcd, command & 0x0F, LCD_NIB);
	} else {
		LcdWrite(lcd, command, LCD_BYTE);
	}

}

/**
 * Write a byte to the data register
 */
void LcdWriteData(Lcd_HandleTypeDef *lcd, uint8_t data) {
	HAL_GPIO_WritePin(lcd->lcd_rs_port, lcd->lcd_rs_pin, LCD_DATA_REG);	// Write to data register

	if (lcd->lcd_mode == LCD_4_BIT_MODE) {
		LcdWrite(lcd, data >> 4, LCD_NIB);
		LcdWrite(lcd, data & 0x0F, LCD_NIB);
	} else {
		LcdWrite(lcd, data, LCD_BYTE);
	}

}

/**
 * Set len bits on the bus and toggle the enable line
 */
void LcdWrite(Lcd_HandleTypeDef *lcd, uint8_t data, uint8_t len) {
	for (uint8_t i = 0; i < len; i++) {
		HAL_GPIO_WritePin(lcd->lcd_data_port[i], lcd->lcd_data_pin[i],
				(data >> i) & 0x01);
	}

	HAL_GPIO_WritePin(lcd->lcd_en_port, lcd->lcd_en_pin, 1);
	DELAY(1);
	HAL_GPIO_WritePin(lcd->lcd_en_port, lcd->lcd_en_pin, 0); // Data receive on falling edge
}

/****************          functions        ************************/

/* to clear the single bit in lcd*/

void LcdClearBit(Lcd_HandleTypeDef *lcd, uint8_t row, uint8_t col) {

	LcdWriteCommand(lcd, SET_DDRAM_ADDR + C__ROW_16[row] + col);
	LcdString(lcd, "      ");
}

/*Display the level in percentage*/

void DisplayLevel(Lcd_HandleTypeDef *lcd, uint8_t level) {
	LcdCursor(lcd, 0, 6);
	LcdInt(lcd, level);
	if (level >= 100) {
		LcdCursor(lcd, 0, 9);
		LcdString(lcd, "%");
	}
	if (level <= 99) {
		LcdCursor(lcd, 0, 8);
		LcdString(lcd, "% ");
	}
}

/*clear the display when it switches between modes*/

void DisplayClearInAuto(Lcd_HandleTypeDef *lcd) {
	if (S__clearState == 1) {
		LcdClear(lcd);
		S__clearState = 0;
	}
}
void DisplayClearInManual(Lcd_HandleTypeDef *lcd) {
	if (S__clearState == 0) {
		LcdClear(lcd);
		S__clearState = 1;
	}
}

