/*
 * Motor.c
 *
 *  Created on: Oct 30, 2019
 *      Author: Sean
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "Commands.h"
#include "PWM_library.h"
#include "ADC_library.h"
#include "UART_library.h"
#include "GPIO_Library.h"

#include "Motor.h"


#define ERR 300
#define DIV 10
#define RIGHT 8000
void PIDcontrol(void)
{

    int desiredR = RIGHT;
    //readRight();

    char message[75];
    sprintf(message, "\nRight = %u\n\r", Right);
    UART_PutString(message);

    kp = 9;
    ki = 2;
    kd = 1;

    signed static int last_error = 0;
    signed int error = desiredR - Right;

    sprintf(message, "Error = %d\n\r", error);
    UART_PutString(message);


    if (error > ERR || error < -ERR)
    {
        P = (kp * error) / DIV;
        sprintf(message, "P = %d\t", P);
        UART_PutString(message);
        I = (ki * (error + last_error)) / DIV;
        sprintf(message, "I = %d\t", I);
        UART_PutString(message);
        D = (kd * (error - last_error)) / DIV;
        sprintf(message, "D = %d\t", D);
        UART_PutString(message);
        U = P + I + D;
        sprintf(message, "U = %d\n\r", U);
        UART_PutString(message);
    }
    else
    {
        U = 0;
    }
    if (U > 5000){
        U = 5000;
    }
    if (U < -5000){
        U = -5000;
    }
    actuator(U);
    last_error = error;
}


// Turn RIGHT if (PID_Val > 0)
// Turn LEFT if (PID_Val < 0)
void actuator(int PID_Val)
{
/*
    if(PID_Val > 0)
    {
        dutyL = MOTORPERIOD;
        dutyR = MOTORPERIOD - PID_Val;
    }
    else if(PID_Val < 0)
    {
        dutyR = MOTORPERIOD;
        dutyL = MOTORPERIOD + PID_Val;
    }
    else
    {
        dutyR = MOTORPERIOD;
        dutyL = MOTORPERIOD;
    }
*/

    //double mult = abs(PID_Val)/MOTORPERIOD;
    //Too far from the wall, needs to correct and go right
    //Going right requires the left wheel spinning faster than the right wheel
    if (PID_Val > 0){
        //PWM_setDuty(pwmR, MOTORPERIOD-PID_Val);
  //      PWM_setDuty(pwmR, 0);
    //    PWM_setDuty(pwmL, 0);
        pinON(5,1); // MODE == 1
        pinON(3,7); //PHASE_A = 1 (Left Wheel - Forward)
        pinON(3,5); //PHASE_B = 1 (Right Wheel - Forward)
        UART_PutString("\nChanging Speed to HIGH...\n\r");
        uint32_t duty = MAXSPEED - PID_Val; // 85% duty cycle
        ChangeDuty(1, duty); // 2 = Change the duty for both wheels
        duty = MAXSPEED; // 85% duty cycle
        ChangeDuty(0, duty); // 2 = Change the duty for both wheels

        setRGB(0,1,0); //GREEN for RIGHT
        /*dutyL -= PID_Val;
        dutyR += PID_Val;
        if (dutyR > MOTORPERIOD){
            dutyR = MOTORPERIOD;
        }
        //UART_PutString("Changing the right wheel's spin");
        PWM_setDuty(pwmR, dutyR);
        PWM_setDuty(pwmL, dutyL);*/
    }

    //Too close to the wall, needs to correct and go left.
    //Going left needs to right wheel to spin faster than the left wheel
    else if (PID_Val < 0){
        pinON(5,1); // MODE == 1
        pinON(3,7); //PHASE_A = 1 (Left Wheel - Forward)
        pinON(3,5); //PHASE_B = 1 (Right Wheel - Forward)
        UART_PutString("\nChanging Speed to HIGH...\n\r");
        uint32_t duty = MAXSPEED - abs(PID_Val); // 85% duty cycle
        //uint32_t duty = 8;
        ChangeDuty(0, duty); // 2 = Change the duty for both wheels
        duty = MAXSPEED; // 85% duty cycle
        ChangeDuty(1, duty); // 2 = Change the duty for both wheels

        //PWM_setDuty(pwmL, MOTORPERIOD+PID_Val);
        //PWM_setDuty(pwmL, 0);
        //PWM_setDuty(pwmR, 0);
        setRGB(1,0,0); //RED for LEFT
      /*  dutyL -= PID_Val;
        dutyR += PID_Val;
        if (dutyL > MOTORPERIOD){
            dutyL = MOTORPERIOD;
        }
        //UART_PutString("Changing the left wheel's spin");
        PWM_setDuty(pwmR, dutyR);
        PWM_setDuty(pwmL, dutyL);*/
    }
    else{
        PWM_setDuty(pwmL, MOTORPERIOD);
        PWM_setDuty(pwmR, MOTORPERIOD);
        setRGB(0,0,1);
    }


/*
    if (PID_Val > 0) // Turn RIGHT?
    {
        //UART_PutString("\nAdjusting Right...\n\r");
        if (dutyL < MOTORPERIOD)
        {

            dutyR += PID_Val;
            dutyL = MOTORPERIOD;
        }
        else
            dutyL -= PID_Val;
    }
    else if(PID_Val < 0)  // Turn LEFT?
    {
        UART_PutString("\nAdjusting Left...\n\r");
        if (dutyR < MOTORPERIOD)
        {
            dutyL -= PID_Val;
            dutyR = MOTORPERIOD;
        }
        else
            dutyR += PID_Val;
    }
    else
    {
        dutyL = MOTORPERIOD;
        dutyR = MOTORPERIOD;
    }
*/


    //if (dutyL < (2*MOTORPERIOD)/10) dutyL = (2*MOTORPERIOD)/10;
    //if (dutyR < (2*MOTORPERIOD)/10) dutyR = (2*MOTORPERIOD)/10;
    if (dutyL > MAXSPEED) dutyL = MAXSPEED;
    if (dutyR > MAXSPEED) dutyR = MAXSPEED;
    ChangeDutyLR();

}
