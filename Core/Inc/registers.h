/*
 * registers.h
 *
 *  Created on: Jan 15, 2023
 *      Author: andersenm
 */

#ifndef REGISTERS_H_
#define REGISTERS_H_

#include <stdint.h>

//// ADC REGISTER STRUCTURE
//typedef struct {
//	uint32_t SR;
//	uint32_t CR1;
//	uint32_t CR2;
//	uint32_t SMPR1;
//	uint32_t SMPR2;
//	uint32_t JOFR1;
//	uint32_t JOFR2;
//	uint32_t JOFR3;
//	uint32_t JOFR4;
//	uint32_t HTR;
//	uint32_t LTR;
//	uint32_t SQR1;
//	uint32_t SQR2;
//	uint32_t SQR3;
//	uint32_t JSQR;
//	uint32_t JDR1;
//	uint32_t JDR2;
//	uint32_t JDR3;
//	uint32_t JDR4;
//	uint32_t DR;
//	uint32_t CCR;
//}ADC;
//
//// RCC REGISTER STRUCTURE
//typedef struct {
//	uint32_t CR;
//	uint32_t PLLCFGR;
//	uint32_t CFGR;
//	uint32_t CIR;
//	uint32_t AHB1RSTR;
//	uint32_t AHB2RSTR;
//	uint32_t reserved1;
//	uint32_t reserved2;
//	uint32_t APB1RSTR;
//	uint32_t APB2RSTR;
//	uint32_t reserved3;
//	uint32_t reserved4;
//	uint32_t AHB1ENR;
//	uint32_t AHB2ENR;
//	uint32_t reserved5;
//	uint32_t reserved6;
//	uint32_t APB1ENR;
//	uint32_t APB2ENR;
//}RCC;

//// GENERIC TIMER REGISTER STRUCTURE
//typedef struct {
//	uint32_t CR1;
//	uint32_t CR2;
//	uint32_t SMCR;
//	uint32_t DIER;
//	uint32_t SR;
//	uint32_t EGR;
//	uint32_t CCMR1;
//	uint32_t CCMR2;
//	uint32_t CCER;
//	uint32_t CNT;
//	uint32_t PSC;
//	uint32_t ARR;
//	uint32_t reserved1;
//	uint32_t CCR1;
//	uint32_t CCR2;
//	uint32_t CCR3;
//	uint32_t CCR4;
//	uint32_t reserved2;
//	uint32_t DCR;
//	uint32_t DMAR;
//}TIMx;
//
//// GENERIC GPIO PORT REGISTER STRUCTURE
//typedef struct {
//	uint32_t MODER;
//	uint32_t OTYPER;
//	uint32_t OSPEEDR;
//	uint32_t PUPDR;
//	uint32_t IDR;
//	uint32_t ODR;
//	uint32_t BSRR;
//	uint32_t LCKR;
//	uint32_t AFRL;
//	uint32_t AFRH;
//}GPIOx;

// NVIC REGISTER STRUCT
typedef struct {
	uint32_t ISER0;
	uint32_t ISER1;
	uint32_t ISER2;
	uint32_t ISER3;
	uint32_t ISER4;
	uint32_t ISER5;
	uint32_t ISER6;
	uint32_t ISER7;
	// ADD more later
}NVIC_REG;
//
//// EXTI REGISTER STRUCT
//typedef struct {
//	uint32_t IMR;
//	uint32_t EMR;
//	uint32_t RTSR;
//	uint32_t FTSR;
//	uint32_t SWIER;
//	uint32_t PR;
//}EXTI;

#endif /* REGISTERS_H_ */
