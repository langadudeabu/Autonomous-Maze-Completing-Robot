#define __MSP432P401R__

#include <ti/drivers/UART.h>
#include <ti/devices/msp432p4xx/driverlib/gpio.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <xdc/cfg/global.h> // needed to get the global from the .cfg file

/* Example/Board Header files */
#include "Board.h"

#include "UART_library.h"

void SetupUART(void)
{
    UART_Params uartParams;
    uart = NULL;

    /* Create a UART with data processing off. */
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = 115200;

    //RX -> (P3.2)
    //TX -> (P3.3)

    // sets global uart variable
    do {
        uart = UART_open(Board_UART1, &uartParams); //set to uart0 for usb or uart1 for bluetooth (helpful for setting the system up)
    } while(uart == NULL);
}

void UART_PutChar(char a) {
    UART_write(uart, &a, 1);
}

void UART_PutString(char *message)
{
    UART_write(uart, message, length(message));
}

void UART_GetChar(char *inChar)
{
    UART_read(uart, inChar, 1);
}

uint32_t length(char *m)
{
    uint32_t len = 0;
    while(m[len] != '\0')
    {
        len++;
    }
    return len;
}


