/*
 * GPIO_library.h
 *
 *  Created on: Oct 29, 2019
 *      Author: Sean
 */

#ifndef GPIO_LIBRARY_H_
#define GPIO_LIBRARY_H_

void SetupGPIO(void);

void pinON(int, int);
void pinOFF(int, int);
void pinTOGGLE(int, int);

void setRGB(int, int, int);


#endif /* GPIO_LIBRARY_H_ */
