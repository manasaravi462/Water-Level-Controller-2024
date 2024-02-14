//**************************************************************************** Header File *
//File Name : userinterface.h
//Creation Date : 05-Dec-2023
//Modification Date : 07-02-2024
//Author : Punith J
/**
  ******************************************************************************
  * @file    userinterface.h
  * @brief   This file contains functions prototype.
  ******************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H
/* Define to prevent recursive inclusion end -------------------------------------*/

/*Private includes ---------------------------------------------------*/
#include "lcd.h"

/* Private function defines ----------------------------------------------------------*/
void ReadModeSwitch(void);
void ManualMode(Lcd_HandleTypeDef lcd);
void Protection(Lcd_HandleTypeDef lcd);
void UIStatus(Lcd_HandleTypeDef lcd, uint8_t row, uint8_t column, char *message,GPIO_TypeDef *GPIOx,
		uint16_t GPIO_Pin, _Bool pinState);
/* Private function defines end ----------------------------------------------------------*/

#endif  // USER_INTERFACE_H
