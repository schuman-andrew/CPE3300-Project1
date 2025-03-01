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

enum state{
	IDLE,
	BUSY,
	COLLISION
};

void resetBuffer(char * buffer);

void sendData(char * data, int length);

void setState(enum state state);

enum state getState();

void rxRead(void);

bool collisionMonitor(int input);

void processData(void);

void monitorPin(enum state s);

#endif /* INC_DATAFUNCTIONS_H_ */
