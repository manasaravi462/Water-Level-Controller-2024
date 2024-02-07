#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "lcd.h"
/****************************Private variables******************/


/***********************Private Functions*****************************/
struct SLevel {
	_Bool levelOne;
	_Bool levelTwo;
	_Bool levelThree;
	_Bool sumpFlag;
} ;

void ReadModeSwitch(void);
void Protection(Lcd_HandleTypeDef lcd);
void UIStatus(Lcd_HandleTypeDef lcd, uint8_t row, uint8_t column, char *message,GPIO_TypeDef *GPIOx,
		uint16_t GPIO_Pin, _Bool pinState);
void UIMessage(Lcd_HandleTypeDef lcd, uint8_t row, uint8_t column,
		char *message, uint8_t value);
void TurnOnMotor(Lcd_HandleTypeDef lcd);
void AutoProtection(Lcd_HandleTypeDef lcd);

#endif  // USER_INTERFACE_H
