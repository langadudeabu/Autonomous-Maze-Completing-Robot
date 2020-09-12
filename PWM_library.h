/*
 * PWM_library.h
 *
 *  Created on: Oct 30, 2019
 *      Author: Sean
 */
#include <stdint.h>
#include <ti/drivers/PWM.h>

#ifndef PWM_LIBRARY_H_
#define PWM_LIBRARY_H_

#define MOTORPERIOD 10000
#define MAXSPEED 9500

PWM_Handle pwmL;
PWM_Handle pwmR;
uint32_t dutyL, dutyR;


void SetupPWM(void);
void ChangeDuty(int, uint32_t);
void ChangeDutyLR(void);
void AdjustLeft(uint32_t);
void AdjustRight(uint32_t);


#endif /* PWM_LIBRARY_H_ */
