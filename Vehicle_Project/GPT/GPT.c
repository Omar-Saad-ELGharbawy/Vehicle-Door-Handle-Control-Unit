/* *****************************************************************************
 * Module: GPT
 *
 * File Name: GPT.c
 *
 * Description: Source file for the STM32 GPT driver
 *
 * Author: Omar Saad
 *
 *******************************************************************************/

#include "GPT.h"
#include "GPT_Private.h"
#include "Rcc.h"
#include "Macros.h"


/*******************************************************************************
 *                      Global Variables   	                                   *
 *******************************************************************************/
uint8 g_overflow_flag;


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Function : GPT_Init
 * Input : void
 * Output : void
 * Description :
 * A function to initialize the GPT registers with the needed initial values
 * to support the needed timing actions.
 */
void GPT_Init(void){

	/*initialize Timer 2 register as Up-Counter to count 1 msec at one clock*/

	/* Enable Clock for TIMER 2*/
	Rcc_Enable(RCC_TIM2);

	/* Control Register 1 (CR1) Describtion:
        Set Counter Enable(CEN) bit at GPT Start Timer
        Update Disable (UDIS) kept at reset state (0)
        URS Bit is set : Generate update flag from overflow only
		OPM Bit is set : Counter stops counting at the next update event (clearing the bit CEN)
        DIR bit is 0 : (Up Conuter)
        CMS is 00 : Edge alligned mode
        ARPE is 0 : Auto reload preload disabled
        CKD is 00 : Clock division is 1
	 */

	/* set Update request source (URS) to generate update flag from overflow only*/
	SET_BIT(TIM2->CR1,2);
	/*set pre_scaler value to 1599 (16,000,000/15,999+1) */
	TIM2->PSC = PSC_VALUE;
	/* enable update interrupt */
	SET_BIT(TIM2->DIER,0);
	g_overflow_flag = INITIAL_STATE;
}

/*
 * Function : GPT_StartTimer
 * Input : unsigned long int OverFlowTicks
 * Output : void
 * Description :
 *  A function to request the GPT to start and send its number of
 *  tickets before timer overflow and stop.
 */
void GPT_StartTimer(unsigned long int OverFlowTicks){
	/*set overflow number to Auto Reload Register*/
	TIM2->ARR = OverFlowTicks;
	/*Enable counter by setting Counter Enable bit Control Register 1 */
	SET_BIT(TIM2->CR1,0);
	g_overflow_flag = NO_OVERFLOW;
}

/*
 * Function : GPT_EndTimer
 * Input : void
 * Output : void
 * Description :
 *  A function to End the GPT timer and clear the counter register to be able to start new timer
 */
void GPT_EndTimer(void){
	/*stop the timer */
	CLEAR_BIT(TIM2->CR1,0);
	/* Clear counter register */
	TIM2->CNT = 0;
	g_overflow_flag = OVERFLOW;
}

/*
 * Function : GPT_CheckTimeIsElapsed
 * Input : void
 * Output : unsigned char
 * Description :
 * A function to return (1) if an overflow occurred after the last call of GPT_StartTimer
 * and (0) if no overflow occurred or GPT_StartTimer is not called from the last read.
 */
unsigned char GPT_CheckTimeIsElapsed(void){
	/* check if overflow occurred by Reading the UIF bit */
	if(TIM2->CNT == (TIM2->ARR - 1))
	{
		/*End the timer */
		GPT_EndTimer();
		return OVERFLOW;
	}else if(READ_BIT(TIM2->CR1,0) == 0){
		return TIMER_NOT_STARTED;
	}else{
		return NO_OVERFLOW;
	}
}

/*
 * Function : GPT_GetElapsedTime
 * Input : void
 * Output : unsigned long int
 * Description :
 *  A function to return number of elapsed ticks from the last call of the GPT_StartTimer,
 *  0 if it is not called and 0xffffffff if an overflow occurred.
 */
unsigned long int GPT_GetElapsedTime(void){

	switch (GPT_CheckTimeIsElapsed())
	{
	case NO_OVERFLOW:
		// return elapsed time
		return TIM2->CNT;
		break;
	case OVERFLOW:
		return 0xffffffff;
		break;
	case TIMER_NOT_STARTED:
		return 0;
		break;
	default:
		return 0;
		break;
	}
}

/*
 * Function : GPT_GetRemainingTime
 * Input : void
 * Output : unsigned long int
 * Description :
 *   A function to return number of remaining ticks till the overflow ticks passed to
 *   GPT_StartTimer, 0xffffffff if GPT_startTime is not called, 0 if an overflow occurred
 */
unsigned long int GPT_GetRemainingTime(void){
	/* check if timer not started*/
	if(READ_BIT(TIM2->CR1,0) == 0){
		return 0xffffffff;
	}
	else if(GPT_CheckTimeIsElapsed() == NO_OVERFLOW ){
		unsigned long int remainig_ticks = TIM2->ARR - TIM2->CNT;
		return remainig_ticks;
	}else{
		/* Overflow */
		return 0;
	}
}



/*
 * Function : GPT_StopTimer
 * Input : void
 * Output : void
 * Description :
 *  A function to Stop the GPT timer without clearing the counter register to be able to cintinue counting be ContinueTimer
 */
void GPT_StopTimer(void){
	/*stop the timer */
	CLEAR_BIT(TIM2->CR1,0);
}

/*
 * Function : GPT_ContinueTimer
 * Input : void
 * Output : void
 * Description :
 *  A function to Continue the GPT timer after stopping it.
 */
void GPT_ContinueTimer(void){
	/*Enable counter by setting Counter Enable bit Control Register 1 */
	SET_BIT(TIM2->CR1,0);
}
