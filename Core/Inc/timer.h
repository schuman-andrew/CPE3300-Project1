/*
 * timer.h
 *
 *  Created on: Feb 11, 2025
 *      Author: andersenm
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "dataFunctions.h"

#define F_CPU 16000000UL
#define EXTI12 1<<12
#define GPIOA_EN 1<<0
#define TIMER5 0x40000C00
#define NVICBASE 0xE000E100
#define GPIOCEN 1<<2
#define TIM5EN 1<<3
#define NVIC_EXT12 1<<8
#define INPUT 0b00
#define PA12 24
#define ALTFUNC 0b10
#define PA0 0
#define PA1 2
#define PULLUP 0x1
#define PA0_TIM5 0x2<<0
#define PA1_TIM5 0x2<<4
#define CEN 1<<0
#define SMS 0
#define RISING_EDGE ~(0b11<<1)
#define ENCODER_MODE1 0b001
#define IC1_TI1 0x1
#define CC1E 1<<0

void initTimer(void);
int getTime(void);
void resetTimer(void);
void disableTimer(void);
#endif /* INC_TIMER_H_ */
