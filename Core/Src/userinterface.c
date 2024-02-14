//**************************************************************************** Header File *
//File Name : userinterface.c
//Creation Date : 05-Dec-2023
//Modification Date : 07-02-2024
//Author : Punith J
//

/* Includes------------------------------------------------------*/
#include <stdint.h>
#include "userinterface.h"
#include "voltagesensor.h"
#include "stm32f401xe.h"
#include "main.h"
#include "level.h"
/* Includes end------------------------------------------------------*/

/*Extern declarations--------------------------------------------------*/
extern SFlags SystemFlags;
extern SParameter VoltageParameters;
extern struct SLevel WaterLevel;
/*Extern declarations end--------------------------------------------------*/


/*Functions*/

/* @brief  switch between Manual Mode and Auto Mode.
 * @param  None
 * @retval None                           */

void ReadModeSwitch() {

	SystemFlags.modeFlag = HAL_GPIO_ReadPin(manual_GPIO_Port, manual_Pin);
}

/* @brief  Function for manual mode.It checks voltage flag.
 * User should turn on/off the motor manually. Relay is set when user switch on the Motor.
 * Throughout manual mode blue led will be on. Red led turns on when voltage flag is set.
 * @param  Lcd_HandleTypeDef where it defines lcd port and pins.
 * @retval None                           */

void ManualMode(Lcd_HandleTypeDef lcd) {
	DisplayClearInManual(&lcd);
	UIStatus(lcd, 0, 3, "MANUAL MODE", blueLed_GPIO_Port,
	blueLed_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(greenLed_GPIO_Port, greenLed_Pin, GPIO_PIN_RESET);
	Protection(lcd); // Check the voltage Flags

	// To turn ON / OFF motor in manual mode
	if (HAL_GPIO_ReadPin(manualModeRelayON_GPIO_Port,
	manualModeRelayON_Pin)) {
		HAL_GPIO_WritePin(relay_GPIO_Port, relay_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(relay_GPIO_Port, relay_Pin, GPIO_PIN_RESET);
	}
}

/* @brief  checks voltage.
 * when voltage is below 200 then it displays v_low and it is above 250, it displays v_high
 * @param  Lcd_HandleTypeDef where it defines lcd port and pins.
 * @retval None                           */

void Protection(Lcd_HandleTypeDef lcd) {
	if (SystemFlags.overVoltageFlag || SystemFlags.underVoltageFlag) {
		HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(relay_GPIO_Port, relay_Pin, GPIO_PIN_RESET);
		if (SystemFlags.overVoltageFlag) {
			UIStatus(lcd, 1, 0, "Warning!! V_high", redLed_GPIO_Port,
			redLed_Pin, GPIO_PIN_SET);
		} else if (SystemFlags.underVoltageFlag) {
			UIStatus(lcd, 1, 0, "Warning!! V_Low", redLed_GPIO_Port, redLed_Pin,
					GPIO_PIN_SET);
		}
		ReadVoltage();
		if (SystemFlags.overVoltageFlag || SystemFlags.underVoltageFlag) {
			HAL_Delay(10000);
		}
		LcdClear(&lcd);

	} else {
		HAL_GPIO_WritePin(relay_GPIO_Port, relay_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(redLed_GPIO_Port, redLed_Pin, GPIO_PIN_RESET);
	}
}

/* @brief  UIstatus function is used for writing message and switch on/off led.
 * @param  Lcd_HandleTypeDef where it defines lcd port and pins.
 * Row and column where message should be displayed. Message to be displayed.
 * Led to turn on / off.
 * @retval None                           */

void UIStatus(Lcd_HandleTypeDef lcd, uint8_t row, uint8_t column, char *message,
		GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, _Bool pinState) {
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, pinState);
	LcdCursor(&lcd, row, column);
	LcdString(&lcd, message);
}
