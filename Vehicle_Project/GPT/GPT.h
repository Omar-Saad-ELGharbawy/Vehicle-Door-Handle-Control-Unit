/* *****************************************************************************
 * Module: GPT
 *
 * File Name: GPT.h
 *
 * Description: Header file for the STM32 GPT driver
 *
 * Author: Omar Saad
 *
 *******************************************************************************/

#ifndef GPT_H_
#define GPT_H_

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


/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define NO_OVERFLOW 0
#define OVERFLOW    1
#define INITIAL_STATE 0xFF

#define TIMER_NOT_STARTED 20

#define PSC_VALUE 15999

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Function : GPT_Init
 * Input : void
 * Output : void
 * Description :
 * A function to initialize the GPT registers with the needed initial values
 * to support the needed timing actions.
 */
void GPT_Init(void);

/*
 * Function : GPT_StartTimer
 * Input : unsigned long int OverFlowTicks
 * Output : void
 * Description :
 *  A function to request the GPT to start and send its number of
 *  tickets before timer overflow and stop.
 */
void GPT_StartTimer(unsigned long int OverFlowTicks);

/*
 * Function : GPT_EndTimer
 * Input : void
 * Output : void
 * Description :
 *  A function to End the GPT timer and clear the counter register to be able to start new timer
 */
void GPT_EndTimer(void);

/*
 * Function : GPT_CheckTimeIsElapsed
 * Input : void
 * Output : unsigned char
 * Description :
 * A function to return (1) if an overflow occurred after the last call of GPT_StartTimer
 * and (0) if no overflow occurred or GPT_StartTimer is not called from the last read.
 */
unsigned char GPT_CheckTimeIsElapsed(void);

/*
 * Function : GPT_GetElapsedTime
 * Input : void
 * Output : unsigned long int
 * Description :
 *  A function to return number of elapsed ticks from the last call of the GPT_StartTimer,
 *  0 if it is not called and 0xffffffff if an overflow occurred.
 */
unsigned long int GPT_GetElapsedTime(void);

/*
 * Function : GPT_GetRemainingTime
 * Input : void
 * Output : unsigned long int
 * Description :
 *   A function to return number of remaining ticks till the overflow ticks passed to
 *   GPT_StartTimer, 0xffffffff if GPT_startTime is not called, 0 if an overflow occurred
 */
unsigned long int GPT_GetRemainingTime(void);



/*
 * Function : GPT_StopTimer
 * Input : void
 * Output : void
 * Description :
 *  A function to Stop the GPT timer without clearing the counter register to be able to cintinue counting be ContinueTimer
 */
void GPT_StopTimer(void);

/*
 * Function : GPT_ContinueTimer
 * Input : void
 * Output : void
 * Description :
 *  A function to Continue the GPT timer after stopping it.
 */
void GPT_ContinueTimer(void);


#endif /* GPT_H_ */
