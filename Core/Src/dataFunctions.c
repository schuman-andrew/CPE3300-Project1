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

uint8_t preamble = 0x55;
uint8_t dataLength = 0;
int byteLenght = 8;

uint8_t zero = 0xf0;
uint8_t one = 0x0f;

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
		if(((preamble>>x) & 0x01) == 0b1){
			//printf("1");
			HAL_UART_Transmit(&huart2, (uint8_t *)&one, 1, HAL_MAX_DELAY);
		} else if(((preamble>>x) & 0b1) == 0b0){
			//printf("0");
			HAL_UART_Transmit(&huart2, (uint8_t *)&zero, 1, HAL_MAX_DELAY);
		}
	}

	//send length
	for(int x = 0; x < byteLenght; x++){
		if((dataLength>>x) & 0x01 == 0b1){
			HAL_UART_Transmit(&huart2, (uint8_t *)&one, 1, HAL_MAX_DELAY);
		} else if((dataLength>>x) & 0x01 == 0b1){
			HAL_UART_Transmit(&huart2, (uint8_t *)&zero, 1, HAL_MAX_DELAY);
		}
	}

	//send data
	for(int x = 0; x < dataLength; x++){
		for(int y = 0; y < byteLenght; y++){
			if((data[x]>>y) & 0x01 == 0b1){
				HAL_UART_Transmit(&huart2, (uint8_t *)&one, 1, HAL_MAX_DELAY);
			} else if((data[x]>>y) & 0x01 == 0b1){
				HAL_UART_Transmit(&huart2, (uint8_t *)&zero, 1, HAL_MAX_DELAY);
			}
		}
	}
}
