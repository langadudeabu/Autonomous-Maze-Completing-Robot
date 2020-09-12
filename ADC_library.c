/*
 * ADC_library.c
 *
 *  Created on: Oct 29, 2019
 *      Author: Sean
 */
#define __MSP432P401R__
#include <ti/drivers/ADC.h>
#include <ti/drivers/adc/ADCMSP432.h>

#include "ADC_library.h"
#include "UART_library.h"
#include "Board.h"

void SetupADC(void)
{
    ADC_init();
    ADC_Params params;

    adcF = NULL; // FRONT SENSOR
    adcR = NULL; // RIGHT SENSOR

    ADC_Params_init(&params);
    params.isProtected = false;
    do{
        adcF = ADC_open(Board_ADC9, &params); //FRONT SENSOR -> P4.4
    } while(adcF == NULL);


    ADC_Params_init(&params);
    params.isProtected = false;
    do {
        adcR = ADC_open(Board_ADC10, &params); //RIGHT SENSOR -> P4.3
    } while(adcR == NULL);

}

void readRight(void)
{
    ADC_convert(adcR, &Right);
}

void readFront(void)
{
    ADC_convert(adcF, &Front);
}



