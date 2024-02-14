//**************************************************************************** Header File *
//File Name : voltage_sensor.h
//Creation Date : 05-Dec-2023
//Modification Date : 07-02-2024
//Author : Abineshwar Kannan
/**
  ******************************************************************************
  * @file    voltagesensor.h
  * @brief   This file contains functions prototype and macros
  belongs to the read voltage .
  ******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __VOLTAGE_SENSOR_H
#define __VOLTAGE_SENSOR_H
/* Define to prevent recursive inclusion end -------------------------------------*/

/* Private function defines ----------------------------------------------------------*/
#define SCALING_FACTOR  1.5

void ReadVoltage();
void Check(float voltage);
void CheckOverVoltage(void);
/* Private function defines end ----------------------------------------------------------*/

#endif /* __VOLTAGE_SENSOR_H */
