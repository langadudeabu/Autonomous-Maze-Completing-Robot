/*
 * Commands.h
 *
 *  Created on: Oct 29, 2019
 *      Author: Sean
 */
#include <stdint.h>

#define THRESH 6000

#ifndef COMMANDS_H_
#define COMMANDS_H_

void (*Table[26][26])(void);

void TableInit(void);
void ListCommands(void); //ll

void ToggleR(void); //rr
void ToggleG(void);//gg
void ToggleB(void);//bb
void TurnOffLED(void);//of
void ToggleWh(void);//wh
void ToggleAq(void);//bg
void TogglePu(void);//br
void ToggleBr(void);//rg

void DistanceSensor(void); //ds

void LowSpeed(void); //ls
void MediumSpeed(void); //ms
void HighSpeed(void); //hs

void Start(void); //st
void Stop(void); //sp

void Reverse(void); //rv
void ReverseMedium(void); // rm
void ReverseLow(void); //rl

void Turn180(void); //zz
void LightSensor(void); // sn
void Spin(void); // tu
void SwivelLeft(void); //sl

void CommandHandler(char *);

#endif /* COMMANDS_H_ */
