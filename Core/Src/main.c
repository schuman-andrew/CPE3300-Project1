
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

/ Includes ------------------------------------------------------------------*/
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
#include "uart_driver.h"

/* Variables ------------------------------------------------------------------*/
bool timerFlag = true;

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

  /* Initialize all configured peripherals */
	MX_GPIO_Init();

	initTimer();

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);

	init_usart2(57600,F_CPU);

	char rxTest[2] = {'H', 'i'};
	sendData(rxTest, 2);

	char rxData[255] = {0};
	int index = 0;
	uint8_t data;

	while (1) {

		data = usart2_getch();
		if(((data >= 32) && (data <= 126)) || data == 13 || data == 10)
		{
			if(data == 13 || data == 10)
			{
				sendData(rxData, index);
				data = 0;
				index = 0;
			}
			else
			{
				rxData[index] = data;
				index++;
			}
		}
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
