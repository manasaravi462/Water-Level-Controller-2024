//*************************************************************************** Legal Notice *
// Copyright (C) Skanray Technologies Pvt. Ltd. - All Rights Reserved.
//
// Unauthorized copying of this file, via any medium is strictly prohibited.
//
// If you are not the intended recipient of this file, please destroy all copies and notify
// Skanray at office@skanray.com.
// Proprietary and confidential.
//**************************************************************************** Source File *
//File Name : voltage_sensor.c
//Creation Date : 05-Dec-2023
//Modification Date : 07-02-2024
//Author : Abineshwar Kannan
//******************************************************************************************

/* Includes ------------------------------------------------------------------*/

#include <math.h>
#include <voltagesensor.h>
#include "main.h"
#include "stm32f4xx_hal.h"

/* Includes end ----------------------------------------------------------*/

/* extern declarations ---------------------------------------------------------*/
extern ADC_HandleTypeDef hadc1;
extern SFlags SystemFlags;
extern SParameter VoltageParameters;
/* extern declarations end ---------------------------------------------------------*/

/* Private variables end ---------------------------------------------------------*/

/* Variables initialize end ---------------------------------------------------------*/

/* Function handlers ------------------------------------------------------------------*/

/* ***** MAIN FUNCTION *****
 * Every 2ms the voltage sensor gets sampled through the ADC pin.
 * The analog value per sample is squared and accumulated for every 100 samples before being averaged.
 * The averaged value is then getting square-rooted.
 * And multiplied with scaling factor of calibration to map the actual voltage to the sensor output voltage.
 * The final vRMS value is obtained and value gets refreshed whenever the sample count reaches 500.
 * set the ready flag to know the read is completed
 * @param  None
 * @retval None                   */

void ReadVoltage() {
	if (VoltageParameters.samplesTaken >= 100) {
		VoltageParameters.voltage = sqrt(
				VoltageParameters.sum
						/ VoltageParameters.samplesTaken) * SCALING_FACTOR;
		VoltageParameters.ready = 1;
		VoltageParameters.samplesTaken = 0;
		VoltageParameters.sum = 0;
	}

}

/* @brief  to check the voltage is linear voltage.
 * @param  None
 * @retval None                           */
void CheckOverVoltage(void) {
	for (int a = 0; a < 10; a++) {
		if (VoltageParameters.ready) {
			VoltageParameters.ready = 0;
			Check(VoltageParameters.voltage);
		}
	}
}
/* @brief  check the over/under  voltage.
 * @param  voltage. measurd voltage stored in the structure memeber
 * @retval None                           */
void Check(float voltage) {

	static int S__highCount = 0, S__lowCount = 0;
	// voltage is more then 250 highCount is increment.
	if (VoltageParameters.voltage > 250) {
		S__highCount++;
	}
	//voltage is below then 200 lowCount is increment.
	if (VoltageParameters.voltage < 200) {
		S__lowCount++;
	}
	// voltage is normal  highCount& lowCount is reset.
	if (VoltageParameters.voltage < 250 && VoltageParameters.voltage > 200) {
		S__highCount = 0;
		S__lowCount = 0;
	}

	// five sample voltage are more or less compare to normal voltage set the Over/Under voltage flags.
	if (S__highCount >= 5) {
		SystemFlags.overVoltageFlag = 1;
	} else {
		SystemFlags.overVoltageFlag = 0;
	}
	if (S__lowCount >= 5) {
		SystemFlags.underVoltageFlag = 1;
	} else {
		SystemFlags.underVoltageFlag = 0;
	}
}

/* Function handlers end ------------------------------------------------------------------*/
