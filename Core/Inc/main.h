/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define manualModeRelayON_Pin GPIO_PIN_1
#define manualModeRelayON_GPIO_Port GPIOA
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define floatSwitch_Pin GPIO_PIN_4
#define floatSwitch_GPIO_Port GPIOA
#define debug_Pin GPIO_PIN_5
#define debug_GPIO_Port GPIOA
#define level1_Pin GPIO_PIN_6
#define level1_GPIO_Port GPIOA
#define level2_Pin GPIO_PIN_7
#define level2_GPIO_Port GPIOA
#define voltageRead_Pin GPIO_PIN_0
#define voltageRead_GPIO_Port GPIOB
#define manual_Pin GPIO_PIN_1
#define manual_GPIO_Port GPIOB
#define relay_Pin GPIO_PIN_12
#define relay_GPIO_Port GPIOB
#define buzzer_Pin GPIO_PIN_13
#define buzzer_GPIO_Port GPIOB
#define level3_Pin GPIO_PIN_8
#define level3_GPIO_Port GPIOA
#define greenLed_Pin GPIO_PIN_10
#define greenLed_GPIO_Port GPIOA
#define redLed_Pin GPIO_PIN_11
#define redLed_GPIO_Port GPIOA
#define blueLed_Pin GPIO_PIN_12
#define blueLed_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define D4_Pin GPIO_PIN_4
#define D4_GPIO_Port GPIOB
#define enableDisplay_Pin GPIO_PIN_5
#define enableDisplay_GPIO_Port GPIOB
#define RSDisplay_Pin GPIO_PIN_6
#define RSDisplay_GPIO_Port GPIOB
#define D7_Pin GPIO_PIN_7
#define D7_GPIO_Port GPIOB
#define D6_Pin GPIO_PIN_8
#define D6_GPIO_Port GPIOB
#define D5_Pin GPIO_PIN_9
#define D5_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
typedef struct {
	_Bool modeFlag;
	_Bool overVoltageFlag;
	_Bool underVoltageFlag;
	_Bool motorStatusFlag;
} SFlags;

typedef struct {
	float voltage;
	uint16_t samplesTaken;
	float sum;
	_Bool ready;
	_Bool protectionFlag;
} SParameter;

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif 
/* __MAIN_H */
