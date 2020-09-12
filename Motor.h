/*
 * Motor.h
 *
 *  Created on: Oct 30, 2019
 *      Author: Sean
 */

#ifndef MOTOR_H_
#define MOTOR_H_


int ki, kp, kd;

signed long int P, I, D, U;

void PIDcontrol(void);
void actuator(int);

#endif /* MOTOR_H_ */
