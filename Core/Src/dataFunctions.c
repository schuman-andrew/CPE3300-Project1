/*
 * dataFunctions.c
 *
 *  Created on: Feb 1, 2025
 *      Author: schumana
 */
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

//static volatile EXTI_TypeDef* const exti = (EXTI_TypeDef*) EXTI_BASE;

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

static int rxPin;
extern int timerInt;


/* CHANNEL MONITOR
 * idle - 1 (while busy)
 * busy - any signal edge (while in idle or collision)
 * collision - logic 0 for 1.1bit period (while busy)
 *
 * need to indicate current state
 *
 * PB8 | D15 - IDLE
 * PB9 | D14 - BUSY
 * PB6 | D10 - COLLISION
 */
enum state busState = IDLE;


/* resetBuffer
 * -resets realterm input buffer
 *
 */
void resetBuffer(char * buffer){
	for(int x=0; x<256; x++){
		buffer[x] = 0;
	}
}

void wait(){
	int Nmax = 1000;
	int N = rand() % (Nmax+1);

	int t = (N/Nmax); //in ms
	delay_ms(t);
}

/*
 *
 *
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

	if(getState() == COLLISION){

		bool dataSent = false;

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
}


void setState(enum state state){
	 busState = state;
}


enum state getState(){
	return busState;
}


void rxRead(){

	  rxPin = (int)HAL_GPIO_ReadPin(RX_PIN_GPIO_Port, RX_PIN_Pin);
//	  printf("%d",rxPin);
	  resetTimer();
	  timerInt = 0;
	  cnt++;
	//	  if(cnt == 50){
	//		  disableTimer();
	//		  exti->IMR = 0;
	//	  }
	//	  printf(" %d ", getTime());

		  // reset timer
	//	  resetTimer();
		  // save data
	  dataRead[cnt] = (uint8_t) rxPin;
	  dataRead[cnt+1] = 2;
	//	  printf("%d", (int) dataRead[cnt]);

}


void processData(void)
{

	int index = 0;
	uint16_t preamble = 0;
	uint16_t length = 0;
	uint32_t data = 0;

//	printf(" %d ", (int) dataRead[cnt]);
//	printf( "%d ", (int) dataRead[index]);
	//check preamble
	while((dataRead[index] != 2) && (index < 9))
	{
		if(dataRead[index] == 1)
		{
			preamble |= 0b1;
		}
		preamble = preamble<<1;
		index++;
	}
	preamble = preamble>>1;
	//printf("%d", preamble);

	if(preamble == 0x55)
	{
		while((dataRead[index] != 2) && (index > 8) && (index < 17))
		{
			if(dataRead[index] == 1)
			{
				length |= 0b1;
			}
			length = length<<1;
			index++;
		}
		length = length>>1;

		//printLength = length;

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

	// check if length correct
	printf("\n");

	cnt = 0;
}

void monitorPin(enum state s){
	HAL_GPIO_WritePin(GPIOB, IDLE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, BUSY_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, COLLISION_Pin, GPIO_PIN_RESET);

	if(s == IDLE){
		HAL_GPIO_WritePin(GPIOB, IDLE_Pin, GPIO_PIN_SET);
	} else if(s == BUSY){
		HAL_GPIO_WritePin(GPIOB, BUSY_Pin, GPIO_PIN_SET);
	} else if(s == COLLISION){
		HAL_GPIO_WritePin(GPIOB, COLLISION_Pin, GPIO_PIN_SET);
	}
}
