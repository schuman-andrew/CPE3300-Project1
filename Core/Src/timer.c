/*
 * timer.c
 *
 *  Created on: Feb 11, 2025
 *      Author: andersenm
 */

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

int timerInt = 0;
//static int rxPin;
extern bool timerFlag;

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

int getTime(void)
{
	return (int) tim5->CNT;
}

void resetTimer(void)
{
	tim5->CNT = 0;
}

void disableTimer(void)
{
	tim5->CR1 &= ~CEN;
}

void TIM5_IRQHandler(void)
{
	tim5->SR &= ~(1<<0);
	timerFlag = true;
	timerInt++;
	if(timerInt > 3)
	{
//		printf("Here");
		processData();
		disableTimer();
	}
//	rxRead();
}
