/*
 *  ======== uartlogging.c ========
 */
#define __MSP432P401R__

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Log.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/Power.h>

/* Driver configuration */
#include "Board.h"
#include "UARTUtils.h"

#include "Main_Threads.h"
#include "Commands.h"
#include "GPIO_library.h"
#include "ADC_library.h"
#include "PWM_library.h"
#include "UART_library.h"
#include "Motor.h"

#define NUMSTATES 2

struct state {
    void (*FuncPt)(void);
    struct state *next; //????
    int id;
};

struct FSM {
    struct state st[NUMSTATES];


} fsm;


void SetupFSM(void)
{
    int i;
    for(i = 0; i < NUMSTATES; i++)
    {
        fsm.st[i].id = i;
        fsm.st[i].FuncPt = NULL;
    }

}

/*
// ISR for Timer0, calls the ADC distance-sensor functions, period of 30 ms
void Timer0_Handler(void)
{
    //readFront();
    //readRight();
}

void Timer1_Handler(void)
{

}
*/
/*
 *  ======== main ========
 */
int main(void)
{
    /* Call board init functions */
    Board_initGeneral();
    GPIO_init();
    UART_init();

    /* Call Peripheral Setup functions */
    SetupGPIO();
    SetupUART();
    TableInit();
    SetupPWM();
    SetupADC();

    SetupFSM();
    semInit();

    Power_disablePolicy(); //Disable Power management.

    /* SysMin will only print to the console when you call flush or exit */
    System_flush();
    /* Start BIOS */
    BIOS_start();

    return (0);
}
