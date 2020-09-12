/*
 * Commands.c
 *
 *  Created on: Oct 29, 2019
 *      Author: Sean
 */
#define __MSP432P401R__

#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

#include <ti/drivers/GPIO.h>
#include <ti/devices/msp432p4xx/driverlib/gpio.h>
#include <ti/sysbios/knl/Task.h>

#include "Commands.h"
#include "UART_library.h"
#include "GPIO_library.h"
#include "PWM_library.h"
#include "ADC_library.h"
#include "Motor.h"

void TableInit(void)
{
    int i, j;
    //Initialize Table
    for(i = 0; i < 26; i++)
        for(j = 0; j < 26; j++)
            Table[i][j] = 0;
    Table['r'-'a']['r'-'a'] = &ToggleR;//rr
    Table['g'-'a']['g'-'a'] = &ToggleG;//gg
    Table['b'-'a']['b'-'a'] = &ToggleB;//bb
    Table['o'-'a']['f'-'a'] = &TurnOffLED;//of
    Table['w'-'a']['h'-'a'] = &ToggleWh;//wh
    Table['b'-'a']['g'-'a'] = &ToggleAq;//bg
    Table['b'-'a']['r'-'a'] = &TogglePu;//br
    Table['r'-'a']['g'-'a'] = &ToggleBr; //rg
    Table['l'-'a']['c'-'a'] = &ListCommands; //lc
    Table['d'-'a']['s'-'a'] = &DistanceSensor; //ds
    Table['l'-'a']['s'-'a'] = &LowSpeed; //ls
    Table['m'-'a']['s'-'a'] = &MediumSpeed; //ms
    Table['h'-'a']['s'-'a'] = &HighSpeed; //hs
    Table['s'-'a']['t'-'a'] = &Start; // st
    Table['s'-'a']['p'-'a'] = &Stop; // sp
    Table['r'-'a']['v'-'a'] = &Reverse; // rv
    Table['r'-'a']['m'-'a'] = &ReverseMedium; // rm
    Table['r'-'a']['l'-'a'] = &ReverseLow; // rl
    Table['z'-'a']['z'-'a'] = &Turn180; // zz
    Table['s'-'a']['n'-'a'] = &LightSensor; // sn
    Table['t'-'a']['u'-'a'] = &Spin; //tu
}

void CommandHandler(char *cmd)
{
    cmd[0] = tolower(cmd[0]);
    cmd[1] = tolower(cmd[1]);
    int i1 = cmd[0] - 'a'; //ASCI value of 'a' is 97.... make 'a' == 0, 'b' == 1, etc...
    int i2 = cmd[1] - 'a'; // i1 and i2 are integer values between 0 and 25

    if(Table[i1][i2])
        Table[i1][i2]();
    else
        UART_PutString("Sorry! That is not a valid command.\n\r");
}

void DistanceSensor(void)
{
    int i = 0, count = 0;
    char test[10];
    while(count++ < 10) {
        uint16_t front = 0, right = 0;
        ADC_convert(adcF, &front); // FRONT SENSOR
        ADC_convert(adcR, &right); // RIGHT SENSOR
        sprintf(test, "\n%d\t%d\n\r", front, right);
        UART_PutString(test);
        for(i = 0; i < 100000; i++){}
    }
}

void ListCommands(void)
{

}

void ToggleR(void)//"rr"
{
    UART_PutString("\nYour Command Has Been Received\n\r");
    UART_PutString("Turning on Red LED...\n\r");
    setRGB(1,0,0);
}

void ToggleG(void)//"gg"
{
    UART_PutString("\nYour Command Has Been Received\n\r");
    UART_PutString("Turning on Green LED...\n\r");
    setRGB(0,1,0);
}

void ToggleB(void)//"bb"
{
    UART_PutString("\nYour Command Has Been Received\n\r");
    UART_PutString("Turning on Blue LED...\n\r");
    setRGB(0,0,1);
}

void TurnOffLED(void)
{
    UART_PutString("\nYour Command Has Been Received\n\r");
    UART_PutString("Turning off LED...\n\r");
    setRGB(0,0,0);
}

void ToggleWh(void) //"wh"
{
    UART_PutString("\nYour Command Has Been Received\n\r");
    UART_PutString("Turning on Red + Blue + Green LEDs...\n\r");
    setRGB(1,1,1);
}

void ToggleAq(void) //"bg"
{
    UART_PutString("\nYour Command Has Been Received\n\r");
    UART_PutString("Turning on Blue + Green LEDs...\n\r");
    setRGB(0,1,1);
}

void TogglePu(void) //"br"
{
    UART_PutString("\nYour Command Has Been Received\n\r");
    UART_PutString("Turning on Blue + Red LEDs...\n\r");
    setRGB(1,0,1);
}

void ToggleBr(void) //"rg"
{
    UART_PutString("\nYour Command Has Been Received\n\r");
    UART_PutString("Turning on Red + Green LEDs...\n\r");
    setRGB(1,1,0);
}

void LowSpeed(void) // ls forward
{
    pinON(5,1); // MODE == 1
    pinON(3,7); //PHASE_A = 1 (Left Wheel - Forward)
    pinON(3,5); //PHASE_B = 1 (Right Wheel - Forward)

    UART_PutString("\nChanging Speed to LOW...\n\r");

    uint32_t duty = (2*MOTORPERIOD)/10; // 20% duty cycle
    //uint32_t duty = 2;
    ChangeDuty(2, duty); // 2 = Change the duty for both wheels
}

void MediumSpeed(void) // ms forward
{
    pinON(5,1); // MODE == 1
    pinON(3,7); //PHASE_A = 1 (Left Wheel - Forward)
    pinON(3,5); //PHASE_B = 1 (Right Wheel - Forward)

    UART_PutString("\nChanging Speed to MEDIUM...\n\r");

    uint32_t duty = (6*MOTORPERIOD)/10; //60% duty cycle
    //uint32_t duty = 6;
    ChangeDuty(2, duty); // 2 = Change the duty for both wheels
}

void HighSpeed(void) // hs forward
{
    pinON(5,1); // MODE == 1
    pinON(3,7); //PHASE_A = 1 (Left Wheel - Forward)
    pinON(3,5); //PHASE_B = 1 (Right Wheel - Forward)

    UART_PutString("\nChanging Speed to HIGH...\n\r");

    uint32_t duty = (95*MOTORPERIOD)/100; // 85% duty cycle
    //uint32_t duty = 8;
    ChangeDuty(2, duty); // 2 = Change the duty for both wheels
}

void Start(void)
{
    int i;
    char message[75];
    HighSpeed();
    while(1)
    {
        readFront();
        readRight();
       /* if(Front > 9000)
        {
            UART_PutString("\nEntering SPIN...\n\r");
            Spin();
            do{
                readFront();
                sprintf(message, "FRONT = %d\n\r", Front);
                UART_PutString(message);
            } while(Front > 3500);
        }
*/

        if (Front > 13000){
            Spin();
            while(Front > 3500){
                readFront();
                UART_PutString("\nEntering SPIN...\n\r");
                sprintf(message, "FRONT = %d\n\r", Front);
                UART_PutString(message);
            }
            HighSpeed();
        }
        for(i = 0; i < 10000; i++){

        }
        PIDcontrol();

        //Task_sleep(50);
    }
}

void Stop(void) // sp
{
    pinON(5,1); // MODE == 1

    UART_PutString("\nStopping Motor...\n\r");

    uint32_t duty = 0; // uint32_t duty = 0 microseconds (us) ... (out of an 8 microsecond period)
    ChangeDuty(2, duty); // 2 = Change the duty for both wheels
}

void Reverse(void) //rv in high speed
{
    pinON(5,1); // MODE == 1
    pinOFF(3,7); //PHASE_A = 0 (Left Wheel - Reverse)
    pinOFF(3,5); //PHASE_B = 0 (Right Wheel - Reverse)

    UART_PutString("\nChanging Speed to HIGH in reverse...\n\r");

    uint32_t duty = 7; // uint32_t duty = 7 microseconds (us) ... (out of an 8 microsecond period)
    ChangeDuty(2, duty); // 2 = Change the duty for both wheels
}

void ReverseMedium(void) //rm
{
    pinON(5,1); // MODE == 1
    pinOFF(3,7); //PHASE_A = 0 (Left Wheel - Reverse)
    pinOFF(3,5); //PHASE_B = 0 (Right Wheel - Reverse)

    UART_PutString("\nChanging Speed to MEDIUM in reverse...\n\r");

    uint32_t duty = 5; // uint32_t duty = 5 microseconds (us) ... (out of an 8 microsecond period)
    ChangeDuty(2, duty); // 2 = Change the duty for both wheels
}

void ReverseLow(void) //rl
{
    pinON(5,1); // MODE == 1
    pinOFF(3,7); //PHASE_A = 0 (Left Wheel - Reverse)
    pinOFF(3,5); //PHASE_B = 0 (Right Wheel - Reverse)

    UART_PutString("\nChanging Speed to LOW in reverse...\n\r");

    uint32_t duty = 2; // uint32_t duty = 2 microseconds (us) ... (out of an 8 microsecond period)
    ChangeDuty(2, duty); // 2 = Change the duty for both wheels
}

void Spin(void) // tu
{
    pinON(5,1); // MODE == 1
    pinON(3,7); //PHASE_A = 1 (Left Wheel - Forward)
    pinOFF(3,5); //PHASE_B = 0 (Right Wheel - Reverse)

    UART_PutString("\nTurning 180? degrees...\n\r");

    //uint32_t duty = 8; // uint32_t duty = 7 microseconds (us) ... (out of an 8 microsecond period)
    uint32_t duty = MOTORPERIOD; // 100% duty cycle
    ChangeDuty(2, duty); // 2 = Change the duty for both wheels
}

void Turn180(void) // zz
{
    while(1)
    {
        HighSpeed();
        do{
            readFront();
        } while(Front < 9500);

        Spin();

        do
        {
            readFront();
        } while(Front > 3500);
    }

}

void SwivelLeft(void)
{
    pinON(5,1); // MODE == 1
    pinON(3,7); //PHASE_A = 1 (Left Wheel - Forward)
    pinON(3,5); //PHASE_B = 1 (Right Wheel - Forward)

}

void LightSensor(void) // sn
{
    while(1)
    {
        char message[50];
        uint32_t decay = 0;
        GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN1);    // configure port 3 pin 6 as an output pin
        GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN1);//set the port 3 pin 6 to GPIO_INPUT_PIN_HIGH

        int i;
        for(i = 0; i < 1000; i++) {}
        GPIO_setAsInputPin(GPIO_PORT_P6, GPIO_PIN1);     // reconfigure this to an input pin


        while(GPIO_getInputPinValue(GPIO_PORT_P6, GPIO_PIN1) != 0)
        {
            decay +=  GPIO_getInputPinValue(GPIO_PORT_P6, GPIO_PIN1);
        }

        if(decay > THRESH)
            setRGB(0,1,0);
        else
            setRGB(1,0,0);

        sprintf(message, "\nDecay is %u \n\r", decay); // then output command unknown
        UART_PutString(message);
    }

}



