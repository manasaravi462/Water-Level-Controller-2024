//*****************************************************************************
//File Name : level.h
//Creation Date : 05-Feb-2023
//Author : Shivakumara UL
//**
/**
  ******************************************************************************
  * @file    level.h
  * @brief   This file contains functions prototype and structure to measure the level.
  ******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LEVEL_H
#define LEVEL_H
/* Define to prevent recursive inclusion end -------------------------------------*/

/* Includes-----------------------------------------------*/
#include "lcd.h"
#include <stdint.h>
/* Includes end-----------------------------------------------*/


struct SLevel {
	_Bool levelOne;
	_Bool levelTwo;
	_Bool levelThree;
	_Bool sumpFlag;
} ;

/* Function defines ----------------------------------------------------------*/
void ReadLevel(void);
void TurnOnMotor(Lcd_HandleTypeDef lcd);
void AutoProtection(Lcd_HandleTypeDef lcd);
void SumpIsEmpty(Lcd_HandleTypeDef lcd);
void CheckLevel(Lcd_HandleTypeDef lcd);


#endif /*LEVEL_H*/
