/*
 * PWM_library.c
 *
 *  Created on: Oct 30, 2019
 *      Author: Sean
 */
#define __MSP432P401R__

#include <ti/drivers/PWM.h>
#include "Board.h"
#include <stdio.h>
#include <stdlib.h>

#include "UART_library.h"
#include "GPIO_library.h"
#include "PWM_library.h"



void ChangeDuty(int m, uint32_t duty)
{
    //UART_PutString("\nChanging Duty Cycle...\n\r");
    if(duty > MOTORPERIOD)
        duty = MOTORPERIOD;
    if (m == 0){
        UART_PutString("\nm = 0\n\r");
    }
    else if (m == 1){
        UART_PutString("\nm = 1\n\r");
    }
    else if (m == 2){
        UART_PutString("\nm = 2\n\r");
    }/*
    else if (duty > 2500 && duty < 5000){
        UART_PutString("\nDuty between 2500 and 5000\n\r");
    }
    else if (duty > 5000){
        UART_PutString("\nDuty higher than 5000\n\r");
    }*/
    switch(m)
    {
    case 0: //LEFT WHEEL (m = 0)
        PWM_setDuty(pwmL, duty);
        dutyL = duty;
        break;
    case 1: //RIGHT WHEEL (m = 1)
        PWM_setDuty(pwmR, duty);
        dutyR = duty;
        break;
    case 2: //BOTH WHEELS (m = 2)
        PWM_setDuty(pwmL, duty);
        PWM_setDuty(pwmR, duty);
        dutyL = dutyR = duty;
        break;
    }
  //  char message[100];
    //sprintf(message, "dutyL = %d/tdutyR = %d\n\r", dutyL, dutyR);
   // UART_PutString(message);

   // UART_PutString("\nDuty cycle changed...\n\r");
}

void ChangeDutyLR(void)
{
    PWM_setDuty(pwmL, dutyL);
    PWM_setDuty(pwmR, dutyR);
}

// WHAT SHOULD THE MOTOR-PERIOD BE ??
// (MOTORPERIOD = 8) -> 8microsecond period -> 125kHz PWM-frequency

PWM_Handle pwm0;
void SetupPWM(void)
{
    PWM_Params params;

    pwmL = NULL;
    pwmR = NULL;
    pwm0 = NULL;

    PWM_init();

    PWM_Params_init(&params);
    params.dutyUnits = PWM_DUTY_US; // microseconds (us)
    params.dutyValue = 0; // duty in microseconds (us)
    params.periodUnits = PWM_PERIOD_US; // microseconds (us)
    params.periodValue = MOTORPERIOD; // period in microseconds

    do {
        pwmL = PWM_open(Board_PWM2, &params); // (P2.6) (A0.6) (PWM2)
    } while(pwmL == NULL);
    PWM_start(pwmL);

    do {
        pwmR = PWM_open(Board_PWM3, &params); // (P2.7) (A0.7) (PWM3)
    } while(pwmR == NULL);
    PWM_start(pwmR);
}

void AdjustLeft(uint32_t amount)
{
    char message[75];
    // Make the right wheel turn faster
    if((dutyR + amount) > MOTORPERIOD){ // dutyR = ((dutyR + amount) > 255) ? (255) : (dutyR + amount);
        dutyR = MOTORPERIOD;
        UART_PutString("Speeding up right wheel");

    }
    else{
        dutyR += amount;
        UART_PutString("Right wheel speeding up.");
    }

    // Make the left wheel turn slower
    if((int)(dutyL - amount) < 0){ // dutyL = ((dutyL - amount) < 0) ? (0) : (dutyL - amount);
        dutyL = 0;
        UART_PutString("Slowing down left wheel");
    }
    else{
        dutyL -= amount;
        UART_PutString("Left wheel slowing down");
    }
    sprintf(message, "DutyL = %d\t", dutyL);
    UART_PutString(message);
    ChangeDutyLR();
}

void AdjustRight(uint32_t amount)
{
    char message[75];
    // Make the left wheel turn faster
    if((dutyL + amount) > MOTORPERIOD) // dutyL = ((dutyL + amount) > 255) ? (255) : (dutyL + amount);
        dutyL = MOTORPERIOD;
    else
        dutyL += amount;

    // Make the right wheel turn slower
    if((int)(dutyR - amount) < 0) // dutyR = ((dutyR - amount) < 0) ? (0) : (dutyR - amount);
        dutyR = 0;
    else
        dutyR -= amount;
    sprintf(message, "Duty R = %d\t", dutyR);
    UART_PutString(message);
    ChangeDutyLR();

}
/*
if((dutyL+amount) > MOTORPERIOD)
{
    if(dutyL == MOTORPERIOD)//If the left is already going max speed...
    {
        dutyR = (dutyR-amount >= 0) ? (dutyR-amount) : (0); // ...then just slow down the right wheel
    }
    else // if the left is not quite max speed, but too close to add the PID_amount...
    {
        amount = (MOTORPERIOD-dutyL );
        dutyL = MOTORPERIOD; //... then set the left to max speed,
        amount =
        dutyR = (dutyR-amount >= 0) ? (dutyR-amount) : (0); // AND slow down the right wheel
    }
}
*/


