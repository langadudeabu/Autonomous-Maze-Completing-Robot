/*
 * GPIO_library.c
 *
 *  Created on: Oct 29, 2019
 *      Author: Sean
 */
#define __MSP432P401R__

#include <ti/drivers/GPIO.h>
#include <ti/devices/msp432p4xx/driverlib/gpio.h>
#include <stdbool.h>

#include "GPIO_library.h"
#include "UART_library.h"

void SetupGPIO(void)
{
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0); // RED LED pin (P1.0)
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0); // onboard RGB LED red pin (P2.0)
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1); // onboard RGB LED green pin (P2.1)
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2); // onboard RGB LED blue pin (P2.2)
    setRGB(0,0,0);

    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN7); // PHASE bit for left Wheel (Motor A) (P3.7)
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN5); // PHASE bit for right Wheel (Motor B) (P3.5)
    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN1);// MODE bit of the motor-driver
    pinON(3,7); //Set Phase bit of left Motor (A) to Forward (1)
    pinON(3,5); // Set Phase bit of right Motor (B) to Forward(1)
    pinON(5,1); // Set the driver's MODE bit to enter PHASE/ENABLE mode
}

void setRGB(int redVal, int greenVal, int blueVal)
{
    bool isValid = true;

    // Make sure the values are either: 0 or 1
    if(redVal < 0 || redVal > 1)
        isValid = false;
    if(greenVal < 0 || greenVal > 1)
        isValid = false;
    if(blueVal < 0 || blueVal > 1)
        isValid = false;

    if(!isValid)
    {
        UART_PutString("\nIncorrect Values! Enter 0 or 1 for each input.\n\r");
        return;
    }

    (redVal == 1) ? pinON(2,0) : pinOFF(2,0);
    (greenVal == 1) ? pinON(2,1) : pinOFF(2,1);
    (blueVal == 1) ? pinON(2,2) : pinOFF(2,2);
}

void pinON(int port, int pin)
{
    GPIO_setOutputHighOnPin(port, 1 << pin);
}

void pinOFF(int port, int pin)
{
    GPIO_setOutputLowOnPin(port, 1 << pin);
}

void pinTOGGLE(int port, int pin){
    GPIO_toggleOutputOnPin(port, 1 << pin);
}
