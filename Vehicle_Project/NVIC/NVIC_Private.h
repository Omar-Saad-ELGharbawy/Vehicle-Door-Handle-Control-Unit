/* *****************************************************************************
 * Module: NVIC
 *
 * File Name: NVIC_Private.h
 *
 * Description: Header Private file for the STM32 NVIC driver
 *
 * Author: Omar Saad
 *
 *******************************************************************************/

#ifndef NVIC_PRIVATE_H_
#define NVIC_PRIVATE_H_

#include "Std_Types.h"


/*******************************************************************************
 *                                Memory Mapping                               *
 *******************************************************************************/

/*******************  BASE ADDRESSES ********************/
#define EXTI_BASE_ADDR 0x40013C00
#define SYSCFG_BASE_ADDR 0x40013800
#define NVIC_BASE_ADDR 0xE000E100

/********************** Structure Memory Mapping ************************/

/* External Interrupt controller Registers */
typedef struct {
	uint32 IMR; 	//Interrupt mask register
	uint32 EMR;		//Event mask register
	uint32 RTSR;	//Rising trigger selection register
	uint32 FTSR;	//Falling trigger selection register
	uint32 SWIER;	//Software interrupt event register
	uint32 PR;		//Pending register
} ExtiType;

/* System configuration controller Registers */
typedef struct {
	uint32 MEMRMP;		//Memory map register
	uint32 PMC;			//Peripheral mode configuration register
	uint16 EXTICR1;		//External interrupt configuration register 1 (EXTI 0-3)
	uint16 RESERVED1;	//Reserved
	uint16 EXTICR2;		//External interrupt configuration register 2 (EXTI 4-7)
	uint16 RESERVED2;	//Reserved
	uint16 EXTICR3;		//External interrupt configuration register 3 (EXTI 8-11)
	uint16 RESERVED3;	//Reserved
	uint16 EXTICR4;		//External interrupt configuration register 4 (EXTI 12-15)
	uint16 RESERVED4;	//Reserved
    uint32 GAP[3];
	uint32 CMPCR;		//Compensation cell control register
} SyscfgType;

/* NVIC registers map */
typedef struct {
	/* Interrupt set-enable registers (ISERx 0:7) */
	uint32 ISER[8];
	uint32 GAP_ISER_ICER[24];
	/* Interrupt clear-enable registers (ICERx 0:7) */
	uint32 ICER[8];
	uint32 GAP_ICER_ISPR[24];
	/* Interrupt set-pending registers (ISPRx 0:7) */
	uint32 ISPR[8];
	uint32 GAP_ISPR_ICPR[24];
	/* Interrupt clear-pending registers (ICPRx 0:7) */
	uint32 ICPR[8];
	uint32 GAP_ICPR_IABR[24];
	/* Interrupt active bit registers (IABRx 0:7) */
	uint32 IABR[8];
	uint32 GAP_IABR_IPR[57];
	/* Interrupt priority registers (IPRx 0:59) */
	uint32 IPR[60];
} NvicType;

/* NVIC_STIR (Software trigger interrupt) register is located in a separate block*/
#define NVIC_STIR (*(uint32 *)0xE000EF00)

/* Pointers to base address with structures data type */
#define EXTI ((ExtiType *)EXTI_BASE_ADDR)
#define SYSCFG ((SyscfgType *)SYSCFG_BASE_ADDR)
#define NVIC ((NvicType *)NVIC_BASE_ADDR)


#endif /* NVIC_PRIVATE_H_ */
