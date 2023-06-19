/* *****************************************************************************
 * Module: GPT
 *
 * File Name: GPT_Private.c
 *
 * Description: Header Private file for the STM32 GPT driver
 *
 * Author: Omar Saad
 *
 *******************************************************************************/

#ifndef GPT_PRIVATE_H_
#define GPT_PRIVATE_H_

#include "Std_Types.h"


/*******************************************************************************
 *                                Memory Mapping                               *
 *******************************************************************************/

/*******************  BASE ADDRESSES ********************/
#define TIM2_BASE_ADDR 0x40000000
#define TIM3_BASE_ADDR 0x40000400
#define TIM4_BASE_ADDR 0x40000800
#define TIM5_BASE_ADDR 0x40000C00


/********************** Structure Memory Mapping ************************/

/* General-purpose timers (TIM2 to TIM5) Registers*/
typedef struct {
    uint32 CR1;  //TIMx control register 1
    uint32 CR2;  //TIMx control register 2
    uint32 SMCR; //TIMx slave mode control register
    uint32 DIER; //TIMx DMA/interrupt enable register
    uint32 SR;   //TIMx status register
    uint32 EGR;  //TIMx event generation register
    uint32 CCMR1;//TIMx capture/compare mode register 1
    uint32 CCMR2;//TIMx capture/compare mode register 2
    uint32 CCER; //TIMx capture/compare enable register
    uint32 CNT;  //TIMx counter
    uint32 PSC;  //TIMx prescaler
    uint32 ARR;  //TIMx auto-reload register
    uint32 Reserved1;
    uint32 CCR1; //TIMx capture/compare register 1
    uint32 CCR2; //TIMx capture/compare register 2
    uint32 CCR3; //TIMx capture/compare register 3
    uint32 CCR4; //TIMx capture/compare register 4
    uint32 Reserved2;
    uint32 DCR;  //TIMx DMA control register
    uint32 DMAR; //TIMx DMA address for full transfer
    uint32 TIM2_OR;   //TIM2 option register
    uint32 TIM5_OR;   //TIM5 option register
}TimxType;

/* Pointers to base address with structures data type */
#define TIM2 ((TimxType *)TIM2_BASE_ADDR)
#define TIM3 ((TimxType *)TIM3_BASE_ADDR)
#define TIM4 ((TimxType *)TIM4_BASE_ADDR)
#define TIM5 ((TimxType *)TIM5_BASE_ADDR)



#endif /* GPT_PRIVATE_H_ */
