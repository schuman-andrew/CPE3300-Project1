/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 	* @file : main.c
  	* @date : Feb 1, 2025
	* @author : Andrew Schuman & Matt Andersen
	* @course : CPE3300
	* @section : 111
	* @assignment : Project 1 Network Interface Node
	* @brief : initializes all peripherals and sends data
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include <stdio.h>
#include "string.h"
#include "dataFunctions.h"
#include "stdbool.h"
#include "timer.h"
#include "stm32f411xe.h"
#include "delay.h"
#include "console.h"
#include "uart_driver.h"

/* Variables ------------------------------------------------------------------*/
bool timerFlag = true;

/* Private user code ---------------------------------------------------------*/
//PUTCHAR_PROTOTYPE {
//	HAL_UART_Transmit(&huart2, (uint8_t*) &ch, 1, HAL_MAX_DELAY);
//	return ch;
//}
//
//GETCHAR_PROTOTYPE {
//	uint8_t ch = 0;
//	__HAL_UART_CLEAR_OREFLAG(&huart2);
//	HAL_UART_Receive(&huart2, (uint8_t*) &ch, 1, HAL_MAX_DELAY);
//	return ch;
//}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
//	setvbuf(stdin, NULL, _IONBF, 0);

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

  /* Configure the system clock */
//	SystemClock_Config();

  /* Initialize all configured peripherals */
	MX_GPIO_Init();
//	MX_USART2_UART_Init();

	initTimer();

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);

	init_usart2(57600,F_CPU);
	char rxData[255] = {0};
	int index = 0;
	uint8_t data;
	while (1) {

		data = usart2_getch();
//		printf("%d \n", data[index-1]);
		if(((data >= 32) && (data <= 126)) || data == 13 || data == 10)
		{
//			printf("works");
			if(data == 13 || data == 10)
			{
//				printf("works");
				sendData(rxData, index);
				data = 0;
				index = 0;
			}
			else
			{
				rxData[index] = data;
				index++;
//				rxData[index] = '0';
			}
		}


//		uint8_t data;
//
//		for(int i = 0; i < 255; i++){
//			printf("%c", dataReceived[i]);
//			if(dataReceived[i+1] == '0'){
//				x = 300;
//			}
//		}
//
//
//		HAL_StatusTypeDef rx;
//
//		// set tx and rx to GPIO in/out
//		// rx interrupt for clock edge - starts timer, calls function
//		// if statement for rx, if statement for tx
//
//		// idle at 1
//
//		if(HAL_UART_GetState(&huart2) == HAL_UART_STATE_BUSY_RX){
//		 while(rx == HAL_BUSY){
//		HAL_UART_Receive(&huart2, &data, 1, HAL_MAX_DELAY);
//
//		if (data == 13 || data == 10) {
//			for (int y = 0; y < x; y++) {
//				printf("%c", rxString[y]);
//			}
//			sendData(rxString, x);
//			resetBuffer(rxString);
//			//printf("\n");
//			x = 0;
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
//		} else {
//			rxString[x] = data;
//			x++;
//		}
//		//printf("%c - %d\n", rxString[x], x);
//
//		if (x > 255) {
//			x = 0;
//		}*/
	}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief starts timer on edge and reads rx pin
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

	enableTimer();

	if (timerFlag)
	{
		timerFlag = false;

		setState(BUSY);
		monitorPin(BUSY);
		rxRead();
	}

}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
}
