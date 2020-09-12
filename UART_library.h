/*
 * UART_library.h
 *
 *  Created on: Oct 30, 2019
 *      Author: Sean
 */
#include <stdint.h>
#include <ti/drivers/UART.h>

#ifndef UART_LIBRARY_H_
#define UART_LIBRARY_H_

UART_Handle uart;

void SetupUART(void);
void UART_PutChar(char);
void UART_PutString(char *);
void UART_GetChar(char *);

uint32_t length(char *);

#endif /* UART_LIBRARY_H_ */
