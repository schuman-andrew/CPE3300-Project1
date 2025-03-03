 /******************************************************************************
  	* @file : dataFunctions.c
  	* @date : Feb 1, 2025
	* @author : Andrew Schuman & Matt Andersen
	* @course : CPE3300
	* @section : 111
	* @assignment : Project 1 Network Interface Node
	* @brief : functions for all data-related operations for the node
	*  		- includes sending/receiving data and a state machine monitor
  ******************************************************************************/

#ifndef INC_DATAFUNCTIONS_H_
#define INC_DATAFUNCTIONS_H_

#include "gpio.h"
#include "stdbool.h"

enum state{
	IDLE,
	BUSY,
	COLLISION
};

/*
 * @brief resets value of an array
 * @param buffer character array to be reset
 */
void resetBuffer(char * buffer);

/*
 * @brief transmits bytes of preamble, length of data, and data
 *  - data transmitted on PA10 - D3
 *  - if COLLISION, function will wait and try sending again
 *  - pin set to idle at exit
 *  @param data array of characters to be sent
 *  @param length data array size
 */
void sendData(char * data, int length);

/*
 * @brief changes the state of node
 * @param state the new state of node
 */
void setState(enum state state);

/*
 * @brief retrieves current state of node
 * @retval current state of node
 */
enum state getState();

/*
 * @brief stores current value of receiver pin
 *  - resets half-bit timer and timer interrupt counter
 */
void rxRead(void);

/*
 * @brief checks and prints received data
 *  - checks preamble and length
 *  - prints data
 *  - sets state to idle
 */
void processData(void);

/*
 * @brief updates monitor pins
 *  - resets all pins
 *  - sets pin of current state
 *  - PB8 | D15 - IDLE
 * 	- PB9 | D14 - BUSY
 *  - PB6 | D10 - COLLISION
 * @param s current state of node
 */
void monitorPin(enum state s);

#endif /* INC_DATAFUNCTIONS_H_ */
