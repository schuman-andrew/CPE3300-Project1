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

#include "dataFunctions.h"
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include <stdio.h>
#include "delay.h"
#include "stdbool.h"
#include "timer.h"
#include "stm32f411xe.h"
#include <stdlib.h>

int printLength;

//tx
uint8_t preamble = 0x55;
uint8_t dataLength = 0;
int byteLenght = 8;

//rx
uint8_t preambleIn;
bool preamRx;
uint8_t dataLengthIn;
bool lengthRx;
uint8_t dataIn[255];
uint8_t dataRead[255];
static int cnt = 0;
int fails = 0;

static int rxPin;
extern int timerInt;


/* CHANNEL MONITOR
 * idle - 1 (while busy)
 * busy - any signal edge (while in idle or collision)
 * collision - logic 0 for 1.1bit period (while busy)
 *
 * need to indicate current state
 */
enum state busState = IDLE;


/*
 * @brief resets value of an array
 * @param buffer character array to be reset
 */
void resetBuffer(char * buffer){
	for(int x=0; x<256; x++){
		buffer[x] = 0;
	}
}

/*
 * @brief generates a random wait time between 0-1000ms
 */
void wait(){
	int Nmax = 1000;
	int N = rand() % (Nmax+1);

	int t = (N/Nmax); //in ms
	delay_ms(t);
}

/*
 * @brief transmits bytes of preamble, length of data, and data
 *  - data transmitted on PA10 - D3
 *  - if COLLISION, function will wait and try sending again
 *  - pin set to idle at exit
 *  @param data array of characters to be sent
 *  @param length data array size
 */
void sendData(char * data, int length){

	dataLength = (uint8_t) length;

	//send preamble
	for(int x = 0; x < byteLenght; x++){
		if(getState() != COLLISION){
			if(((preamble<<x) & 0x80) == 0x80){
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
			} else if(((preamble<<x) & 0x80) == 0x00){
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
			}
			delay_us(500);
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_10);
			delay_us(500);
		} else {// if collision then exit all loops
			x = 100;
		}
	}

	//send length
	for(int x = 0; x < byteLenght; x++){
		if(getState() != COLLISION){
			if(((dataLength<<x) & 0x80) == 0x80){
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
			} else if(((dataLength<<x) & 0x80) == 0x00){
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
			}
			delay_us(500);
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_10);
			delay_us(500);
		} else {
			x = 100;
		}
	}

	//send data
	for(int x = 0; x < dataLength; x++){
		for(int y = 0; y < byteLenght; y++){
			if(getState() != COLLISION){
				if(((data[x]<<y) & 0x80) == 0x80){
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
				} else if(((data[x]<<y) & 0x80) == 0x00){
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
				}
				delay_us(500);
				HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_10);
				delay_us(500);
			}
			else {
				x = 300;
				y = 100;
			}
		}
	}

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);

	if((getState() == COLLISION) && (fails < 5)){
		printf("%d/n", fails);
		bool dataSent = false;
		fails++;

		while(getState() == COLLISION){
			//ends when set back to idle
		}
		while(!dataSent){

			wait();

			if(getState() == IDLE){
				sendData(data, length);
				dataSent = true;
			}
		}
	}
	else if (fails == 5)
	{
		printf("Transmission Timeout\n");
		fails++;
	}
}


/*
 * @brief changes the state of node
 * @param state the new state of node
 */
void setState(enum state state){
	 busState = state;
}


/*
 * @brief retrieves current state of node
 * @retval current state of node
 */
enum state getState(){
	return busState;
}


/*
 * @brief stores current value of receiver pin
 *  - resets half-bit timer and timer interrupt counter
 */
void rxRead(){

	  rxPin = (int)HAL_GPIO_ReadPin(RX_PIN_GPIO_Port, RX_PIN_Pin);

	  resetTimer();

	  dataRead[cnt] = (uint8_t) rxPin;
	  cnt++;
	  dataRead[cnt] = 2;
}

/*
 * @brief checks and prints received data
 *  - checks preamble and length
 *  - prints data
 *  - sets state to idle
 */
void processData(void)
{

	int index = 0;
	uint16_t preamble = 0;
	uint16_t length = 0;
	uint32_t data = 0;

	//check preamble
	while((dataRead[index] != 2) && (index < 8))
	{
		if(dataRead[index] == 1)
		{
			preamble |= 0b1;
		}
		preamble = preamble<<1;
		index++;
	}
	preamble = preamble>>1;

	if(preamble == 0x55)
	{
		while((dataRead[index] != 2) && (index > 7) && (index < 16))
		{
			if(dataRead[index] == 1)
			{
				length |= 0b1;
			}
			length = length<<1;
			index++;
		}
		length = length>>1;

		while((dataRead[index] != 2) && (index != 255))
		{
			for(int i=0; (i<8) && (dataRead[index] != 2); i++)
			{
				if(dataRead[index] == 1)
				{
					data |= 0b1;
				}
				data = data<<1;
				index++;
			}
			data = data>>1;
			printf("%c", (char)data);
			data = 0;
		}
	}

	setState(IDLE);
	monitorPin(IDLE);

	printf("\n");

	cnt = 0;
}

/*
 * @brief updates monitor pins
 *  - resets all pins
 *  - sets pin of current state
 *  - PB8 | D15 - IDLE
 * 	- PB9 | D14 - BUSY
 *  - PB6 | D10 - COLLISION
 * @param s current state of node
 */
void monitorPin(enum state s){

	if(s == IDLE){
		HAL_GPIO_WritePin(GPIOB, IDLE_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, BUSY_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, COLLISION_Pin, GPIO_PIN_RESET);
	} else if(s == BUSY){
		HAL_GPIO_WritePin(GPIOB, BUSY_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, IDLE_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, COLLISION_Pin, GPIO_PIN_RESET);
	} else if(s == COLLISION){
		HAL_GPIO_WritePin(GPIOB, COLLISION_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, IDLE_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, BUSY_Pin, GPIO_PIN_RESET);
	}
}
