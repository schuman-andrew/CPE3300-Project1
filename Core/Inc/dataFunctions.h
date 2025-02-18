/*
 * dataFunctions.h
 *
 *  Created on: Feb 1, 2025
 *      Author: schumana
 */

#ifndef INC_DATAFUNCTIONS_H_
#define INC_DATAFUNCTIONS_H_

#include "gpio.h"
#include "stdbool.h"


void resetBuffer(char * buffer);

void sendData(char * data, int length);

void rxRead(void);

void processData(void);

void printReceived();

bool receiveStatus();

#endif /* INC_DATAFUNCTIONS_H_ */
