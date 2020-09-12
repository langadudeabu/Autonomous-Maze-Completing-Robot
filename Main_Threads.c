/*
 * Main_Threads.c
 *
 *  Created on: Oct 29, 2019
 *      Author: Sean
 */
/* XDCtools Header files */
#define __MSP432P401R__

#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Log.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Swi.h>
#include <xdc/cfg/global.h> // needed to get the global from the .cfg file
//#include <ti/sysbios/knl/Semaphore.h>
#include <ti/devices/msp432p4xx/driverlib/gpio.h>

#include <Semaphore.h>

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "Commands.h"
#include "UART_library.h"
#include "GPIO_library.h"
#include "ADC_library.h"
#include "PWM_Library.h"

#include "Main_Threads.h"

void heartBeat(UArg arg0, UArg arg1) {
    arg0 = 1000;
    while (1) {
        pinTOGGLE(1,0);
        Task_sleep(arg0);
    }
}
void bluetooth(UArg arg0, UArg arg1)
{

    int numLetters = 0;
    char inChars[2];
    bool isValid = true;

    while (1)
    {
        numLetters = 0;
        isValid = true;
        while(numLetters < 2 && isValid)
        {
            UART_GetChar(&inChars[numLetters]); //polls on terminal input
            UART_PutChar(inChars[numLetters]); //ECHO to UART terminal
            if(isalpha(inChars[numLetters]))
            {
                numLetters++;
                continue;
            }
            else
            { //Char not valid
                isValid = false;
                break;
            }
        }
        if(!isValid)
        {
            UART_PutString("\nError: Please Enter 2 Alphabetical Chars <Char><Char>.\n\t");
            continue; //Is this a good approach?
        }
        else //if it is VALID
        {
            CommandHandler(inChars);
        }
        Task_sleep(1);
    }
}

void ReadRight(UArg arg0, UArg arg1)
{
    //ADC_convert(adcR, &Right);

    /*
    uint32_t value[1];

    ADCIntClear(ADC_RIGHT, sRight);
    ADCProcessorTrigger(ADC_RIGHT, sRight);

    //while(!ADCIntStatus(ADC_FRONT, 1, false)){;}
    SysCtlDelay(200);

    ADCSequenceDataGet(ADC_RIGHT, sRight, value);
    char message[50] = "";

    UARTPutString("Right Distance Sensor = ");
    sprintf(message,"%u\n\r", value[0]);
    UARTPutString((unsigned char *)message);

    Task_sleep(1);
    */
    Task_sleep(1);
}

void ReadFront(UArg arg0, UArg arg1)
{
    //ADC_convert(adcF, &Front);
    Task_sleep(1);
    /*
    uint32_t value[8];

    ADCIntClear(ADC_FRONT, sFront);
    ADCProcessorTrigger(ADC_FRONT, sFront);
    //UARTPutString("\tRight Distance Sensor = ");
    //while(!ADCIntStatus(ADC_FRONT, 1, false)){;}
    SysCtlDelay(200);

    //UARTPutString("\tRight Distance Sensor = ");
    ADCSequenceDataGet(ADC_FRONT, sFront, value);
    char message[50] = "";
    //int i;
    UARTPutString("Front Distance Sensor = ");
    sprintf(message,"%u\n\r", value[0]);
    UARTPutString((unsigned char *)message);
    //sprintf(message, "%u \n\r", value[0]);
    //UARTPutString((unsigned char *)message);

    Task_sleep(1);
    */
}


void PID_Handler(void)
{
    Swi_post(pidSWI);
}

void PID(UArg arg0, UArg arg1)
{
    setRGB(1,0,1);
    char message[75];
    uint32_t duty = 0;
    readRight();
    readFront();
    if (Front > 13000)
    {
        Spin();
        while(Front > 3000){
            readFront();
            UART_PutString("\nEntering SPIN...\n\r");
            sprintf(message, "FRONT = %d\n\r", Front);
            UART_PutString(message);
        }
        HighSpeed();
    }

    sprintf(message, "\nRight = %u\n\r", Right);
    UART_PutString(message);

    kp = 9;
    ki = 2;
    kd = 1;

    signed static int last_error = 0;
    signed int error = RIGHT - Right;

    sprintf(message, "Error = %d\n\r", error);
    UART_PutString(message);

    if(print)
    {
        Q = (Q + 1) % 2; // Toggle Q (Either 0 or 1)

        if(Q == 1)
        {
            if(queueAddress[queueNum] == 20)
            {
                queueNum = (queueNum + 1) % 2; // Toggle: Either 0 or 1
                Semaphore_post(sema4);
                pinTOGGLE(2,2); // Toggle Blue LED
            }
            queue[queueNum][queueAddress[queueNum]] = error;
            queueAddress[queueNum]++;
        }
    }

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
    last_error = error;

    /*ACTUATOR*/
    if (U > 0)
    {
        pinON(5,1); // MODE == 1
        pinON(3,7); //PHASE_A = 1 (Left Wheel - Forward)
        pinON(3,5); //PHASE_B = 1 (Right Wheel - Forward)

        duty = MAXSPEED - U;
        ChangeDuty(1,duty);
        duty = MAXSPEED;
        ChangeDuty(0, duty);
    }
    else if (U < 0)
    {
        pinON(5,1); // MODE == 1
        pinON(3,7); //PHASE_A = 1 (Left Wheel - Forward)
        pinON(3,5); //PHASE_B = 1 (Right Wheel - Forward)

        duty = MAXSPEED - abs(U);
        ChangeDuty(0, duty);
        duty = MAXSPEED;
        ChangeDuty(1, duty);
    }
    else
    {
        PWM_setDuty(pwmL, MOTORPERIOD);
        PWM_setDuty(pwmR, MOTORPERIOD);
    }
}

void *printQueues(void *arg0) {
    queueNum = 0;
    queueAddress[0] = 0;
    queueAddress[1] = 0;
    uint8_t temp;
    int i;
    char a[10];

    while (1) {
        setRGB(0,1,0);
        setRGB(0,0,1);
        Semaphore_pend(sema4, BIOS_WAIT_FOREVER);
        temp = (queueNum + 1) % 2;
        for (i = 0; i < queueAddress[temp]; i++) {
            sprintf(a, "%d\n\r", queue[temp][i]);
            UART_PutString(a);
        }
        queueAddress[temp] = 0;
        Task_sleep( 25 );
        setRGB(0,1,0);
        setRGB(0,0,1);
    }
}

void SemInit(void) {
    int retc = 0;
    retc = sem_init(&sema4, 0, 0);
    if (retc == -1) {
        while (1);
    }

}

void globalInit(void)
{
    P = I = D = 0;
    print = 0;
    dutyL = dutyR = MAXSPEED;
    kp = 800;
    ki = 100;
    kd = 50000;
    queueNum = 0;
    queueAddress[0] = 0;
    queueAddress[1] = 0;
    Q = 0;

}

void *lightSensor(void *arg0) {
    // 1.Configure GPIO pin as output
    // 2.Set output to HIGH
    // 3.Allow at least 10[µs] for the sensor output voltage to rise
    // 4.Reconfigure GPIO pin as input
    // 5.Measure time elapsed until input is LOW
    // 6.Use the measured time to determine whether on black or white surface
    int white = 1; // if the last time this was ran the answer was black this will be a 0
    int countPin6 = 0;
    int counter = 0;
    int threshold = 700;
    int i;
    while(1){
        GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN1);    // configure port 3 pin 6 as an output pin
        GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN1);//set the port 3 pin 6 to GPIO_INPUT_PIN_HIGH

        //start a timer
        for(i = 0; i < 1000; i++) {}
        GPIO_setAsInputPin(GPIO_PORT_P6, GPIO_PIN1);     // reconfigure this to an input pin
        countPin6 = 0;
        while(GPIO_getInputPinValue(GPIO_PORT_P6, GPIO_PIN1) != 0) {
            countPin6 +=  GPIO_getInputPinValue(GPIO_PORT_P6, GPIO_PIN1);
        }                      // read from port 3 pin 6 and 7
        //sprintf(a, "countPin6 %d \n\r", countPin6); // then output command unknown
        if(countPin6 > threshold && white) {
            //putString("Black");
            pinTOGGLE(2, 2);
            white = 0;
            counter++;
        } else if(countPin6 < threshold && white == 0) {
            if(counter > 3) {
                stop();
                queueNum = (queueNum + 1) % 2;
                Semaphore_post(sema4);
                Task_sleep(25);
                printTime();
            } else {
                print = (print + 1) % 2;
            }
            counter = 0;
            white = 1;
        } else if(countPin6 > threshold) counter++;

        // pass the time to a function that determines if the thing we just saw was black or white
        Task_sleep(25);
    }
}

