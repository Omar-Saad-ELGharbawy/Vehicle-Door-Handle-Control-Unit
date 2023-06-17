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
#include "Utils.h"
#include "Macros.h"


/*******************************************************************************
 *                      Global Variables   	                                   *
 *******************************************************************************/
unsigned long int g_OverFlowTicks;


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
uint8 g_overflow_flag;
/*
 * Function : GPT_Init
 * Input : void
 * Output : void
 * Description :
 * A function to initialize the GPT registers with the needed initial values
 * to support the needed timing actions.
 */
void GPT_Init(void){

	/*initialize Timer 2 register as upcounter with to count 1 msec at one clock*/

	/* Control Register 1 (CR1) Describtion:
        Set Counter Enable(CEN) bit at GPT Start Timer
        Update Disable (UDIS),URS kept at reset state (0) 
		OPM Bit is set : Counter stops counting at the next update event (clearing the bit CEN)
        DIR bit is 0 : (Up Conuter)
        CMS is 00 : Edge alligned mode
        ARPE is 0 : Auto reload preload disabled
        CKD is 00 : Clock division is 1
	 */

	/* UDIS bit is 0 to enable update event*/
	CLEAR_BIT(TIM2->CR1,1);
	/* set Update request source (URS) to generate update flag from overflow only*/
	SET_BIT(TIM2->CR1,2);
	/*Set One Pulse Mode Bit to stop counting at the next update event */
	//	 SET_BIT(TIM2->CR1,3);
	/* Set Auto reload preload enable(ARR is buffered) */
	//    SET_BIT(TIM2->CR1,7);

	/*set pre_scaler value to 1599 (16000/15999+1) */
	//	TIM2->PSC = 1599;
	//	TIM2->PSC = 0x3E7F;
	TIM2->PSC = PSC_VALUE;
	// enable update interrupt
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
	g_OverFlowTicks = OverFlowTicks;
	TIM2->ARR = OverFlowTicks;
	/*Enable counter by setting Counter Enable bit Control Register 1 */
	SET_BIT(TIM2->CR1,0);
//	g_overflow_flag = FALSE;
	g_overflow_flag = NO_OVERFLOW;
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

	/* check if overflow ocuured by Reading the UIF bit */
	//	unsigned char Overflow_flag = READ_BIT(TIM2->SR,0);
	if(TIM2->CNT == (2000 - 1))
	{
		//		if(READ_BIT(TIM2->SR,0) == OVERFLOW)
		/* Clear Update interrupt flag (UIF) if overflow occurs */
		// CLEAR_BIT(TIM2->SR,0);
		/*stop the timer */
		CLEAR_BIT(TIM2->CR1,0);
//		g_overflow_flag = TRUE;
		g_overflow_flag = OVERFLOW;
		TIM2->CNT = 0;
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
// unsigned long int GPT_GetElapsedTime(void){

// 	/* check if timer not started*/

// 	if(READ_BIT(TIM2->CR1,0) == TIMER_NOT_STARTED){
// 		return TIMER_NOT_STARTED;
// 	}
// 	else if(GPT_CheckTimeIsElapsed() == NO_OVERFLOW ){
// 		unsigned long int elapsed_ticks = TIM2->CNT;
// 		return elapsed_ticks;
// 	}else{
// 		return OVERFLOW;
// 	}
// }
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

	/* check if timer not started*/

	// if(READ_BIT(TIM2->CR1,0) == TIMER_NOT_STARTED){
	// 	return TIMER_NOT_STARTED;
	// }
	// else if(GPT_CheckTimeIsElapsed() == NO_OVERFLOW ){
	// 	unsigned long int elapsed_ticks = TIM2->CNT;
	// 	return elapsed_ticks;
	// }else{
	// 	return OVERFLOW;
	// }
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

	if(READ_BIT(TIM2->CR1,0) == TIMER_NOT_STARTED){
		return 0xffffffff;
	}
	else if(GPT_CheckTimeIsElapsed() == NO_OVERFLOW ){
		unsigned long int remainig_ticks = g_OverFlowTicks - TIM2->CNT;
		return remainig_ticks;
	}else{
		//		overflow
		return 0;
	}

}