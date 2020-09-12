/*
 * ADC_library.h
 *
 *  Created on: Oct 29, 2019
 *      Author: Sean
 */
#include <ti/drivers/ADC.h>
#include <ti/drivers/adc/ADCMSP432.h>

#include <stdint.h>

#ifndef ADC_LIBRARY_H_
#define ADC_LIBRARY_H_

ADC_Handle adcF; // FRONT SENSOR
ADC_Handle adcR; // RIGHT SENSOR

uint16_t Right, Front;

void SetupADC(void);
void readRight(void);
void readFront(void);

#endif /* ADC_LIBRARY_H_ */
