/**
  ******************************************************************************
  * @file        delay.c
  * @author      Matt Andersen
  * @brief       This file contains delay timer methods.
  ******************************************************************************
*/
#include <stdio.h>
#include <stdint.h>
#include "delay.h"
#include <stdlib.h>

// Static register addresses
static volatile uint32_t * const systick_base = (uint32_t*) SYSTICK_BASE;

/*
 * @brief   This method uses the systick timer to wait
 * 			the specified amount of milliseconds.
 * @param   delay - The amount of milliseconds to wait
 */
void delay_ms(int delay)
{
	// Set the load value to delay * cycles in 1ms
	systick_base[LOAD] = (delay * CYCLE_PER_MS) - 1;
	// Set the timer value to 0
	systick_base[VAL] = 0;
	// Enable the timer to start counting
	systick_base[CTRL] |= EN;

	// Waits until the count flag goes high
	// which means the timer has reached 0
	while(!(systick_base[CTRL] & COUNTFLAG))
	{
		// busy loop
	}
	// Disable the timer
	systick_base[CTRL] &= !EN;
}

/*
 * @brief   This method uses the systick timer to wait
 * 			the specified amount of microseconds.
 * @param   delay - The amount of microseconds to wait
 */
void delay_us(int delay)
{
	// Set the load value to delay * cycles in 1us
	systick_base[LOAD] = (delay * CYCLE_PER_US) - 1;
	// Set the timer value to 0
	systick_base[VAL] = 0;
	// Enable the timer to start counting
	systick_base[CTRL] |= EN;

	// Waits until the count flag goes high
	// which means the timer has reached 0
	while(!(systick_base[CTRL] & COUNTFLAG))
	{
		// busy loop
	}
	// Disable the timer
	systick_base[CTRL] &= !EN;
}
