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
 *                                Definitions                                  *
 *******************************************************************************/


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



#endif /* GPT_H_ */
