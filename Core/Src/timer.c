 /******************************************************************************
  	* @file : timer.c
  	* @date : Feb 11, 2025
	* @author : Matt Andersen & Andrew Schuman
	* @course : CPE3300
	* @section : 111
	* @assignment : Project 1 Network Interface Node
	* @brief :
  ******************************************************************************/

#include "timer.h"
#include "stm32f411xe.h"
#include <stdbool.h>
#include "delay.h"
#include <stdio.h>
#include "registers.h"
#include "core_cm4.h"
#include "dataFunctions.h"

static volatile TIM_TypeDef* const tim5 = (TIM_TypeDef*) TIM5_BASE;
static volatile RCC_TypeDef* const rcc = (RCC_TypeDef*) RCC_BASE;
static volatile NVIC_REG* const nvic = (NVIC_REG*) NVICBASE;
//static volatile EXTI_TypeDef* const exti = (EXTI_TypeDef*) EXTI_BASE;

int monitor[3] = {2,2,2};
int maxMonitorIndex = 2;
int stateIndicator = 0;

int timerInt = 0;
//static int rxPin;
extern bool timerFlag;

/*
 *  @brief resets all values in monitor array
 */
void resetMonitor(){
	monitor[0] = 2;
	monitor[1] = 2;
	monitor[2] = 2;
}

/*
 *  @brief setup all registers for timer 5
 *   - timer set to run for (approximately) 500us
 */
void initTimer(void)
{
	rcc->APB1ENR |= TIM5EN;

	tim5->CCMR1 &= ~(0b11<<0);
	tim5->CCMR1 &= ~(0b111<<4);
	tim5->CCMR1 |= 0b001<<4;

	tim5->DIER |= 0x1;
	nvic->ISER1 |= (1<<18);

	tim5->ARR  = 0x14444;
	tim5->CCR1 = 0x14444;
	tim5->CCER |= CC1E;

//    tim5->CR1 |= CEN;
}

/*
 *  @brief returns current timer value
 *  @retval current timer count value
 */
int getTime(void)
{
	return (int) tim5->CNT;
}

/*
 *  @brief sets timer count to 0
 */
void resetTimer(void)
{
	tim5->CNT = 0;
}

/*
 *  @brief sets timer enable to
 */
void disableTimer(void)
{
	tim5->CR1 &= ~CEN;
}

/*
 * @brief
 *  - timerInt -> counts interrupts
 *  - stores value of rx pin
 *  - either detects idle, collision, or end of data transmission
 */
void TIM5_IRQHandler(void)
{
	tim5->SR &= ~(1<<0);
	timerFlag = true;
	timerInt++;

	stateIndicator = stateMonitor(HAL_GPIO_ReadPin(RX_PIN_GPIO_Port,
			RX_PIN_Pin));

	if(timerInt > 2)
	{
		if(stateIndicator == 1){
			setState(COLLISION);
			disableTimer();
			monitorPin(COLLISION);
			resetMonitor();
		}
		else if(stateIndicator == 2 && (getState() == COLLISION)){
			setState(IDLE);
			disableTimer();
			monitorPin(IDLE);
			resetMonitor();
		}
		else {
//		printf("Here");
			processData();
			disableTimer();
			resetMonitor();
		}
//	rxRead();
	}
}

/*
 * @brief stores and checks last 3 half-bit values
 *  - idle - 1 (while busy)
 *  - busy - any signal edge (while in idle or collision)
 *  - collision - logic 0 for 1.1bit period (while busy)
 * @param input last half-bit value on rx pin
 * @retval state of node
 */
int stateMonitor(int input){
	int collCheck = 0;
	int idleCheck = 0;

	for(int x = 0; x<maxMonitorIndex; x++){

		monitor[x] = monitor[x+1];

		if(monitor[x] == 0){
			collCheck++;
			idleCheck = 0;
		} else if (monitor[x] == 1){
			idleCheck++;
			collCheck = 0;
		}
	}
	monitor[maxMonitorIndex] = input;

	if(monitor[maxMonitorIndex] == 0){
		collCheck++;
		//idleCheck = 0;
	}
	else if (monitor[maxMonitorIndex] == 1){
		idleCheck++;
		//collCheck = 0;
	}

	if(collCheck == 3){
		return 1;
	}
	else if(idleCheck == 3){
		return 2;
	}
	return 0;
}
