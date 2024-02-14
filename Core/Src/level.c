/******************************************************************************
//File Name : level.c
//Creation Date : 05-Feb-2023
//Author : Shivakumara UL
*/

/* Includes-------------------------------------------------------------*/
#include "level.h"
#include "userinterface.h"
/* Includes end----------------------------------------------------------*/

/*Extern declarations----------------------------------------------*/
struct SLevel WaterLevel = { 0 };
extern SFlags SystemFlags;
extern SParameter VoltageParameters;
/*Extern declarations end--------------------------------------------*/

/* Private Variables------------------------------------------------*/
static uint8_t S__thresholdLevel = 20;
static uint8_t S__maximumLevel = 100;
static uint8_t S__intermediateLevel = 60;
/* Private Variables end------------------------------------------------*/

/*Functions ------------------------------------------------------------*/

/* @brief  to read the level pins i,e thresholdlevel, intermediate level and maximum level.
 * @param  None
 * @retval None                           */

void ReadLevel(void) {
	WaterLevel.levelOne = HAL_GPIO_ReadPin(level1_GPIO_Port, level1_Pin);
	WaterLevel.levelTwo = HAL_GPIO_ReadPin(level2_GPIO_Port, level2_Pin);
	WaterLevel.levelThree = HAL_GPIO_ReadPin(level3_GPIO_Port, level3_Pin);
}

/* @brief  to check whether sump is empty or not.
 * @param  Lcd_HandleTypeDef where it defines lcd port and pins.
 * @retval None                           */

void SumpIsEmpty(Lcd_HandleTypeDef lcd)
{
	if (HAL_GPIO_ReadPin(floatSwitch_GPIO_Port, floatSwitch_Pin))
	{
		LcdClear(&lcd);
		UIStatus(lcd, 0, 3, "EMPTY SUMP", greenLed_GPIO_Port,
		greenLed_Pin, GPIO_PIN_RESET); // display empty sump
		HAL_GPIO_WritePin(relay_GPIO_Port, relay_Pin, GPIO_PIN_RESET); // Reset relay
		WaterLevel.sumpFlag = 1;
	}
	// To Clear display only once
	if (WaterLevel.sumpFlag)
	{
		LcdClear(&lcd);
		WaterLevel.sumpFlag = 0;
	}
}

/* @brief  to check water level along with high voltage or low voltage.
 * 			When voltage flag is set then motor is automatically turned off and buzzer is set.
 * @param  Lcd_HandleTypeDef where it defines lcd port and pins.
 * @retval None                           */

void CheckLevel(Lcd_HandleTypeDef lcd)
{
	UIStatus(lcd, 0, 0, "Level", blueLed_GPIO_Port,
	blueLed_Pin, GPIO_PIN_RESET); //display Level and reset blue led

	// to check water level - whether water is in thresholdlevel and to turn on motor
	if (WaterLevel.levelOne == 0 && WaterLevel.levelTwo == 0
			&& WaterLevel.levelThree == 0)
	{
		DisplayLevel(&lcd, S__thresholdLevel); // Display 20%

		if (SystemFlags.overVoltageFlag == 0
				&& SystemFlags.underVoltageFlag == 0)
		{
			TurnOnMotor(lcd); // Turn On motor when no fault occurs
		} else {
			// Turn OFF motor when Voltage Flag is set
			AutoProtection(lcd);
		}
	}
	//turn on the motor when device is in intermediateLevel(voltage flags is set)
	if (WaterLevel.levelOne == 1 && WaterLevel.levelTwo == 0
			&& WaterLevel.levelThree == 0)
	{
		DisplayLevel(&lcd, S__thresholdLevel); // Display 20%
		if (SystemFlags.overVoltageFlag == 1
				|| SystemFlags.underVoltageFlag == 1)
		{
			// Turn OFF motor when Voltage Flag is set
			AutoProtection(lcd);
		} else if (VoltageParameters.protectionFlag == 1
				&& SystemFlags.overVoltageFlag == 0
				&& SystemFlags.underVoltageFlag == 0)
		{
			TurnOnMotor(lcd);
		}
	}

	else if (WaterLevel.levelOne == 1 && WaterLevel.levelTwo == 1
			&& WaterLevel.levelThree == 0)
	{
		DisplayLevel(&lcd, S__intermediateLevel); // Display 60%
		if (SystemFlags.overVoltageFlag || SystemFlags.underVoltageFlag)
		{
			// Turn OFF motor when Voltage Flag is set
			AutoProtection(lcd);
		}
		if (VoltageParameters.protectionFlag == 1
				&& SystemFlags.overVoltageFlag == 0
				&& SystemFlags.underVoltageFlag == 0) {
			TurnOnMotor(lcd);
		}
	}

	// Turn Off Motor when tank is full
	else if (WaterLevel.levelOne & WaterLevel.levelTwo
			& WaterLevel.levelThree) {
		DisplayLevel(&lcd, S__maximumLevel); // Display 100%
		HAL_GPIO_WritePin(relay_GPIO_Port, relay_Pin, GPIO_PIN_RESET); // turn off relay
		UIStatus(lcd, 1, 0, "MOTOR OFF",
		greenLed_GPIO_Port, greenLed_Pin, GPIO_PIN_RESET);
	}
}

/* @brief  to turn on the motor automatically when it reaches below threshold level.
 * 			When voltage flag is set then motor is automatically turned off and buzzer is set.
 * @param  Lcd_HandleTypeDef where it defines lcd port and pins.
 * @retval None                           */

void TurnOnMotor(Lcd_HandleTypeDef lcd)
{
	if (VoltageParameters.protectionFlag) {
		LcdClear(&lcd);
		VoltageParameters.protectionFlag = 0;
	}
	UIStatus(lcd, 1, 0, "MOTOR ON ", redLed_GPIO_Port, redLed_Pin,
			GPIO_PIN_RESET); // display Motor on
	HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(greenLed_GPIO_Port, greenLed_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(relay_GPIO_Port, relay_Pin, GPIO_PIN_SET);
}


/* @brief  to provide proctection to motor.
 * 			When voltage is < 200 then undervoltage flag is set and display V_low.
 * 			When voltage is > 250 then overvoltage flag is set and display V_High.
 * @param  Lcd_HandleTypeDef where it defines lcd port and pins.
 * @retval None                           */

void AutoProtection(Lcd_HandleTypeDef lcd)
{
	// when voltage flag is set
	VoltageParameters.protectionFlag = 1;
	UIStatus(lcd, 1, 0, "MOTOR OFF", greenLed_GPIO_Port, greenLed_Pin,
			GPIO_PIN_RESET); // display Motor Off
	HAL_GPIO_WritePin(relay_GPIO_Port, relay_Pin, GPIO_PIN_RESET); // relay reset
	HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET); // buzzer set

	if (SystemFlags.underVoltageFlag) {
		UIStatus(lcd, 1, 10, "V_LOW ", redLed_GPIO_Port, redLed_Pin,
				GPIO_PIN_SET); // display voltage low
	}
	if (SystemFlags.overVoltageFlag) {
		UIStatus(lcd, 1, 10, "V_HIGH", redLed_GPIO_Port, redLed_Pin,
				GPIO_PIN_SET); // display voltage high
	}
	HAL_Delay(5000);

}


