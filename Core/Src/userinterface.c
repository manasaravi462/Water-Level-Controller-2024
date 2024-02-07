/*********************************** Includes***************************/
#include <stdint.h>
#include <userinterface.h>
#include <voltagesensor.h>
#include "stm32f401xe.h"
#include "main.h"
#include "lcd.h"

/*****extern declarations******/
extern SFlags SystemFlags;
extern SParameter VoltageParameters;

/********************************Private functions*****************************************/
struct SLevel WaterLevel = { };

void ReadModeSwitch() {
	SystemFlags.modeFlag = HAL_GPIO_ReadPin(manual_GPIO_Port, manual_Pin);
}

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

void UIStatus(Lcd_HandleTypeDef lcd, uint8_t row, uint8_t column, char *message,
		GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, _Bool pinState) {
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, pinState);
	LcdCursor(&lcd, row, column);
	LcdString(&lcd, message);
}

void UIMessage(Lcd_HandleTypeDef lcd, uint8_t row, uint8_t column,
		char *message, uint8_t value) {
	LcdCursor(&lcd, row, column);
	LcdString(&lcd, message);
	LcdInt(&lcd, value);
}
void TurnOnMotor(Lcd_HandleTypeDef lcd) {
	if (VoltageParameters.protectionFlag) {
		LcdClear(&lcd);
		VoltageParameters.protectionFlag = 0;
	}
	UIStatus(lcd, 1, 0, "MOTOR ON ", redLed_GPIO_Port, redLed_Pin,
			GPIO_PIN_RESET);
	HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(greenLed_GPIO_Port, greenLed_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(relay_GPIO_Port, relay_Pin, GPIO_PIN_SET);
}
void AutoProtection(Lcd_HandleTypeDef lcd) {
	VoltageParameters.protectionFlag = 1;
	UIStatus(lcd, 1, 0, "MOTOR OFF", greenLed_GPIO_Port, greenLed_Pin,
			GPIO_PIN_RESET);
	HAL_GPIO_WritePin(relay_GPIO_Port, relay_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET);

	if (SystemFlags.underVoltageFlag) {
		UIStatus(lcd, 1, 10, "V_LOW ", redLed_GPIO_Port, redLed_Pin, GPIO_PIN_SET);
	}
	if (SystemFlags.overVoltageFlag) {
		UIStatus(lcd,  1, 10, "V_HIGH",redLed_GPIO_Port, redLed_Pin, GPIO_PIN_SET);
	}
	HAL_Delay(5000);

}

