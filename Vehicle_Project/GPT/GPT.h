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

/* GPT Driver Documentation */
/* General Purpose Timer 2 Driver Using Up Counting Mode 
 * 1. Initialize the GPT module by calling GPT_Init() function.
 * 2. Start the GPT by calling GPT_StartTimer( ms ) function and passing to it the time in ms.
 * 3. Check if the time is elapsed by calling GPT_CheckTimeIsElapsed() function.
 * 4. Get elapsed time in ms using GPT_GetElapsedTime() function.
 * 5. Get remaining time in ms using GPT_GetRemainingTime() function.
 * 6. End the current timer by calling GPT_EndTimer() function.
 * 7. Stop the current timer by calling GPT_StopTimer() function.
 * 8. Continuew the current timer by calling GPT_ContinueTimer() function. 
 *  */


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
