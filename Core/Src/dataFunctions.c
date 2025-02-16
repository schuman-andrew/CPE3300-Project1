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
#include "usart.h"

//static volatile EXTI_TypeDef* const exti = (EXTI_TypeDef*) EXTI_BASE;

//tx
uint8_t preamble = 0x55;
uint8_t dataLength = 0;
int byteLenght = 8;

uint8_t zero = 0xf0;
uint8_t one = 0x0f;

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
//static int edgeTrigger;
/* resetBuffer
 *
 */
void resetBuffer(char * buffer){
	for(int x=0; x<256; x++){
		buffer[x] = 0;
	}
}


/*
 *  sends each byte as 2 bits
 *  baud rate = 8000 -> each half byte sends in 500us to act like one bit
 *
 */
void sendData(char * data, int length){
	dataLength = (uint8_t) length;

	//HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState); w/ hal delay
	//other option

	//send preamble
	for(int x = 0; x < byteLenght; x++){
		if(((preamble<<x) & 0x80) == 0x80){
			//printf("1");
			//HAL_UART_Transmit(&huart2, (uint8_t *)&one, 1, HAL_MAX_DELAY);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
		} else if(((preamble<<x) & 0x80) == 0x00){
			//printf("0");
			//HAL_UART_Transmit(&huart2, (uint8_t *)&zero, 1, HAL_MAX_DELAY);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
		}
		delay_us(500);
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_10);
		delay_us(500);
	}

	//send length
	for(int x = 0; x < byteLenght; x++){
		if(((dataLength<<x) & 0x80) == 0x80){
			//HAL_UART_Transmit(&huart2, (uint8_t *)&one, 1, HAL_MAX_DELAY);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
		} else if(((dataLength<<x) & 0x80) == 0x00){
			//HAL_UART_Transmit(&huart2, (uint8_t *)&zero, 1, HAL_MAX_DELAY);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
		}
		delay_us(500);
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_10);
		delay_us(500);
	}

	//send data
	for(int x = 0; x < dataLength; x++){
		for(int y = 0; y < byteLenght; y++){
			if(((data[x]<<y) & 0x80) == 0x80){
				//HAL_UART_Transmit(&huart2, (uint8_t *)&one, 1, HAL_MAX_DELAY);
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
			} else if(((data[x]<<y) & 0x80) == 0x00){
				//HAL_UART_Transmit(&huart2, (uint8_t *)&zero, 1, HAL_MAX_DELAY);
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
			}
			delay_us(500);
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_10);
			delay_us(500);
		}
	}
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
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
	// check if length correct

	cnt = 0;
}
