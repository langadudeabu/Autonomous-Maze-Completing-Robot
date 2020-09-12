/*
 * Main_Threads.h
 *
 *  Created on: Oct 29, 2019
 *      Author: Sean
 */
#include <stdint.h>

#ifndef MAIN_THREADS_H_
#define MAIN_THREADS_H_

#define ERR 300
#define DIV 10
#define RIGHT 8000

#define FIFOSIZE          10

int ki, kp, kd;
signed long int P, I, D, U;

//ping-pong queues
uint8_t queueNum;
uint8_t queueAddress[2];
int queue[2][20];

// Run Time variables
int print;
uint32_t runTime;

int Q; // either 0 or 1

// FIFO int queue struct
typedef struct queue {
    int size;
    int getI;
    int putI;
    int queue[FIFOSIZE];
} Queue;

void heartBeat(UArg arg0, UArg arg1);
void ReadFront(UArg, UArg);
void ReadRight(UArg, UArg);
void bluetooth(UArg, UArg);

void semInit(void);

#endif /* MAIN_THREADS_H_ */
