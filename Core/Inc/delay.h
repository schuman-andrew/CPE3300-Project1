/**
  ******************************************************************************
  * @file        delay.h
  * @author      Matt Andersen
  ******************************************************************************
*/

#ifndef DELAY_H_
#define DELAY_H_

#define CYCLE_PER_MS 7940

#define CYCLE_PER_US 5.22
#define SYSTICK_BASE 0xE000E010
#define CTRL 0
#define LOAD 1
#define VAL 2
#define ENABLE 1<<0
#define CLOCKSRC 1<<2
#define COUNTFLAG 1<<16

// FUNCTION PROTOTYPES
void delay_ms(int);
void delay_us(int);

#endif /* DELAY_H_ */
