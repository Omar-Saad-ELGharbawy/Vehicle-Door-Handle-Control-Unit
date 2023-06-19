/*
 * main.c
 *
 *  Created on: May 7, 2023
 *      Author: Omar Saad
 */

// #include "GPT.h"
#include "Gpio.h"
#include "Rcc.h"
#include "Std_Types.h"
#include "GPT.h"
#include "NVIC.h"

/*disable optimizations for for loops*/
#pragma GCC optimize("-O0")

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* Push Buttons Configurations*/
#define HANDLE_LOCK_BUTTON 2
#define DOOR_LOCK_BUTTON 3

/* LEDs Configurations*/
#define VEHICLE_LOCK_LED 5
#define HAZARD_LIGHT_LED 6
#define AMBIENT_LIGHT_LED 7

/* Active High Led States*/
#define BUTTON_PRESSED LOW
#define BUTTON_RELEASED HIGH

/* Active LOW Push Buttons States*/
#define LED_ON HIGH
#define LED_OFF LOW

/* USE CASES*/
#define DEFAULT_STATE 0
#define DOOR_UNLOCK 1
#define DOOR_IS_OPEN 2
#define ANTI_THEFT_LOCK 3
#define CLOSING_THE_DOOR 4
#define LOCKING_THE_DOOR 5

/* DOOR status*/
#define DOOR_LOCKED LOW
#define DOOR_UNLOCKED HIGH
#define DOOR_CLOSED LOW
#define DOOR_OPENED HIGH

/* Times status */
#define STARTED 1
#define ENDED 0

/*******************************************************************************
 *                            Global Variables                                 *
 *******************************************************************************/
uint8 use_case = DEFAULT_STATE;

uint8 handle_lock = DOOR_LOCKED;
uint8 door_lock = DOOR_CLOSED;

// uint8 g_overflow_flag;
extern unsigned long int g_overflow_flag;

/*******************************************************************************
 *                                Main                                         *
 *******************************************************************************/
int main()
{

	/* ***********************Initializations*********************** */
	/* Initialize RCC Driver */
	Rcc_Init();
	/* Enable Clock for GPIO PORT A*/
	Rcc_Enable(RCC_GPIOA);
	/* Enable Clock for GPIO PORT B*/
	Rcc_Enable(RCC_GPIOB);
	/* Enable Clock for System configuration controller */
	Rcc_Enable(RCC_SYSCFG);
	/* Enable Clock for TIMER 2*/
	Rcc_Enable(RCC_TIM2);
	/* Initialize GPT TIMER*/
	GPT_Init();

	/* ***********************Configurations*********************** */

	/*initialize interrupts for line 2 and line 3 from PORT A as Falling Edge (Input Push Buttons) */
	Exti_Init(PORT_A,HANDLE_LOCK_BUTTON,FALLING_EDGE);
	Exti_Init(PORT_A,DOOR_LOCK_BUTTON,FALLING_EDGE);

	/* Enable interrupts */
	Exti_Enable(HANDLE_LOCK_BUTTON);
	Exti_Enable(DOOR_LOCK_BUTTON);

	/* Configure pins for Output LEDS */
	Gpio_ConfigPin(GPIO_B, VEHICLE_LOCK_LED, GPIO_OUTPUT, GPIO_PUSH_PULL, GPIO_PULL_UP);
	Gpio_WritePin(GPIO_B, VEHICLE_LOCK_LED, LOW);
	Gpio_ConfigPin(GPIO_B, HAZARD_LIGHT_LED, GPIO_OUTPUT, GPIO_PUSH_PULL, GPIO_PULL_UP);
	Gpio_WritePin(GPIO_B, HAZARD_LIGHT_LED, LOW);
	Gpio_ConfigPin(GPIO_B, AMBIENT_LIGHT_LED, GPIO_OUTPUT, GPIO_PUSH_PULL, GPIO_PULL_UP);
	Gpio_WritePin(GPIO_B, AMBIENT_LIGHT_LED, LOW);


	uint8 door_unlock_timer = FALSE;
	uint8 anti_theft_timer = FALSE;
	uint8 closing_door_timer = FALSE;
	uint8 locking_door_timer = FALSE;
	while (1)
	{
		/*******************************************************************************
		 *                            Vehicle   Cases                                  *
		 *******************************************************************************/

		switch (use_case)
		{
		/* System is Powered ON
		 * No Buttons is pressed */
		case DEFAULT_STATE:
			if (handle_lock == DOOR_UNLOCKED)
			{
				use_case = DOOR_UNLOCK;
			}
			break;
			/* **************Vehicle Door Unlocked but still Closed *************/
		case DOOR_UNLOCK:
			/* Vehicle Lock LED is ON*/
			Gpio_WritePin(GPIO_B, VEHICLE_LOCK_LED, HIGH);

			/*HAZARD LED is Blinking for one time ( 0.5 sec high and 0.5 sec low ) */
			/* Ambient Light Led is on for 2 seconds */
			/* Start counting for 10 seconds If no buttons is pressed go to ANTI_THEFT_LOCK state */
			/* Check that timer did not started before to start the timer */
			if ( (GPT_CheckTimeIsElapsed() == TIMER_NOT_STARTED) && (door_unlock_timer != STARTED) )
			{
				/* Start timer for 10 seconds */
				GPT_StartTimer(10000);
				door_unlock_timer = STARTED;
			}

			/* Timer Conditions */
			/* If no overflow occurred blink leds */
			if (g_overflow_flag == NO_OVERFLOW)
			{
				if (GPT_CheckTimeIsElapsed() == OVERFLOW)
				{
					/* if overflow occurs (10 seconds ended) close the LEDs and go to ANTI_THEFT_LOCK State */
					use_case = ANTI_THEFT_LOCK;
					GPT_EndTimer();
					door_unlock_timer = ENDED;
					// g_overflow_flag = TRUE;
				}
				else if( GPT_GetElapsedTime() < 500 )
				{
					Gpio_WritePin(GPIO_B, HAZARD_LIGHT_LED, HIGH);
					Gpio_WritePin(GPIO_B, AMBIENT_LIGHT_LED, HIGH);
				}
				else if ((GPT_GetElapsedTime() >= 500) && (GPT_GetElapsedTime() < 1000))
				{
					Gpio_WritePin(GPIO_B, HAZARD_LIGHT_LED, LOW);
				}
				else if (GPT_GetElapsedTime() >= 2000)
				{
					/* if 2 seconds ended close the leds */
					Gpio_WritePin(GPIO_B, AMBIENT_LIGHT_LED, LOW);
				}

			}
			/* if overflow occurs (10 seconds ended) close the leds and go to ANTI_THEFT_LOCK State */
			else if (g_overflow_flag == OVERFLOW)
			{
				use_case = ANTI_THEFT_LOCK;
				GPT_EndTimer();
				door_unlock_timer = ENDED;
			}

			/* check buttons*/
			if (door_lock == DOOR_OPENED)
			{
				GPT_EndTimer();
				door_unlock_timer = ENDED;
				use_case = DOOR_IS_OPEN;
			}
			if (handle_lock == DOOR_LOCKED)
			{
				GPT_EndTimer();
				door_unlock_timer = ENDED;
				use_case = LOCKING_THE_DOOR;
			}
			break;
			/* **************** Vehicle Door Unlocked and Door is Open *******************/
		case DOOR_IS_OPEN:
			/* Ambient LED is ON*/
			Gpio_WritePin(GPIO_B, AMBIENT_LIGHT_LED, HIGH);
			Gpio_WritePin(GPIO_B, VEHICLE_LOCK_LED, HIGH);
			Gpio_WritePin(GPIO_B, HAZARD_LIGHT_LED, LOW);

			if (door_lock == DOOR_CLOSED)
			{
				use_case = CLOSING_THE_DOOR;
			}
			break;
		case ANTI_THEFT_LOCK:
			/* Vehicle Lock LED is OFF*/
			Gpio_WritePin(GPIO_B, VEHICLE_LOCK_LED, LOW);
			/* Ambient Led is OFF */
			Gpio_WritePin(GPIO_B, AMBIENT_LIGHT_LED, LOW);
			/*HAZARD LED is Blinking for 2 times ( 0.5 sec high and 0.5 sec low ) for each blink */
			/* Check that timer did not started before to start the timer */
			if ( (GPT_CheckTimeIsElapsed() == TIMER_NOT_STARTED) && (anti_theft_timer != STARTED) )
			{
				/* Start timer for 2 seconds */
				GPT_StartTimer(2000);
				anti_theft_timer = STARTED;
			}

			/* Timer Conditions */
			/* If no overflow occurred blink LEDS */
			if (g_overflow_flag == NO_OVERFLOW)
			{

				if (GPT_CheckTimeIsElapsed() == OVERFLOW)
				{
					/* if overflow occurs (2 seconds ended) close the Leds and go to DEFAULT_STATE */
					GPT_EndTimer();
					anti_theft_timer = ENDED;
					handle_lock = DOOR_LOCKED;
					use_case = DEFAULT_STATE;
				}
				else if( GPT_GetElapsedTime() < 500 )
				{
					Gpio_WritePin(GPIO_B, HAZARD_LIGHT_LED, HIGH);
				}
				else if ((GPT_GetElapsedTime() > 500) && (GPT_GetElapsedTime() < 1000))
				{
					Gpio_WritePin(GPIO_B, HAZARD_LIGHT_LED, LOW);
				}
				else if ((GPT_GetElapsedTime() >= 1000) && (GPT_GetElapsedTime() < 1500))
				{
					Gpio_WritePin(GPIO_B, HAZARD_LIGHT_LED, HIGH);
				}
				else if ((GPT_GetElapsedTime() > 1500))
				{
					Gpio_WritePin(GPIO_B, HAZARD_LIGHT_LED, LOW);
				}
			}
			/* if overflow occurs (2 seconds ended) close the Leds and go to DEFAULT_STATE */
			else if (g_overflow_flag == OVERFLOW)
			{
				GPT_EndTimer();
				anti_theft_timer = ENDED;
				handle_lock = DOOR_LOCKED;
				use_case = DEFAULT_STATE;
			}
			break;
			/* ***************** Vehicle Door Unlocked and Door is Closed *****************/
		case CLOSING_THE_DOOR:
			/* VEHICLE LED IS OFF */
			Gpio_WritePin(GPIO_B, VEHICLE_LOCK_LED, LOW);
			/* HAZARD LED IS OFF */
			Gpio_WritePin(GPIO_B, HAZARD_LIGHT_LED, LOW);
			/* Ambient Led is ON for 1 second then OFF */
			/* Check that timer did not started before to start the timer */
			if ( ( (GPT_CheckTimeIsElapsed() == TIMER_NOT_STARTED) && (closing_door_timer != STARTED) ) )
			{
				/* Start timer for 10 seconds */
				GPT_StartTimer(10000);
				closing_door_timer = STARTED;
			}
			/* Timer Conditions */
			if (g_overflow_flag == NO_OVERFLOW)
			{
				if( GPT_GetElapsedTime() < 1000 )
				{
					Gpio_WritePin(GPIO_B, AMBIENT_LIGHT_LED, HIGH);
				}
				else if ( (GPT_GetElapsedTime() >= 1000) )
				{
					Gpio_WritePin(GPIO_B, AMBIENT_LIGHT_LED, LOW);
				}
				else if (GPT_CheckTimeIsElapsed() == OVERFLOW)
				{
					/* if overflow occurs (10 seconds ended) go to ANTI_THEFT_LOCK state  */
					GPT_EndTimer();
					closing_door_timer = ENDED;
					use_case = ANTI_THEFT_LOCK;
				}
			}
			else if (g_overflow_flag == OVERFLOW)
			{
				/* if overflow occurs (10 seconds ended) go to ANTI_THEFT_LOCK state  */
				GPT_EndTimer();
				closing_door_timer = ENDED;
				use_case = ANTI_THEFT_LOCK;
			}
			/* check buttons*/
			if (door_lock == DOOR_OPENED)
			{
				GPT_EndTimer();
				closing_door_timer = ENDED;
				use_case = DOOR_IS_OPEN;
			}
			if (handle_lock == DOOR_LOCKED)
			{
				GPT_EndTimer();
				closing_door_timer = ENDED;
				use_case = LOCKING_THE_DOOR;
			}
			break;
		/* ********************Vehicle Door Locked and Door is Closed******************* */
		case LOCKING_THE_DOOR:
			/* VEHICLE LED IS OFF */
			Gpio_WritePin(GPIO_B, VEHICLE_LOCK_LED, LOW);
			/* Ambient Led is OFF */
			Gpio_WritePin(GPIO_B, AMBIENT_LIGHT_LED, LOW);
			/*HAZARD LED is Blinking for 2 times */
			/* Check that timer did not started before to start the timer */
			if ( ( (GPT_CheckTimeIsElapsed() == TIMER_NOT_STARTED) && (locking_door_timer != STARTED) ) )
			{
				/* Start timer for 2 seconds */
				GPT_StartTimer(2000);
				door_unlock_timer = ENDED;
				locking_door_timer = STARTED;
			}

			/* Timer Conditions */
			/* If no overflow occurred blink LEDs */
			if (g_overflow_flag == NO_OVERFLOW)
			{
				if (GPT_CheckTimeIsElapsed() == OVERFLOW)
				{
					/* if overflow occurs (2 seconds ended) close the LEDs and go to DEFAULT_STATE */
					Gpio_WritePin(GPIO_B, HAZARD_LIGHT_LED, LOW);
					GPT_EndTimer();
					locking_door_timer = ENDED;
					handle_lock = DOOR_LOCKED;
					use_case = DEFAULT_STATE;
				}
				else if( GPT_GetElapsedTime() < 500 )
				{
					Gpio_WritePin(GPIO_B, HAZARD_LIGHT_LED, HIGH);
				}
				else if ((GPT_GetElapsedTime() > 500) && (GPT_GetElapsedTime() < 1000))
				{
					Gpio_WritePin(GPIO_B, HAZARD_LIGHT_LED, LOW);
				}
				else if ((GPT_GetElapsedTime() >= 1000) && (GPT_GetElapsedTime() < 1500))
				{
					Gpio_WritePin(GPIO_B, HAZARD_LIGHT_LED, HIGH);
				}
				else if ((GPT_GetElapsedTime() > 1500))
				{
					Gpio_WritePin(GPIO_B, HAZARD_LIGHT_LED, LOW);
				}
			}
			/* if overflow occurs (2 seconds ended) close the LEDS and go to DEFAULT_STATE */
			else if (g_overflow_flag == OVERFLOW)
			{
				Gpio_WritePin(GPIO_B, HAZARD_LIGHT_LED, LOW);
				GPT_EndTimer();
				locking_door_timer = ENDED;
				handle_lock = DOOR_LOCKED;
				use_case = DEFAULT_STATE;
			}
			if (handle_lock == DOOR_UNLOCKED)
			{
				GPT_EndTimer();
				locking_door_timer = ENDED;
				use_case = DOOR_UNLOCK;
			}
			break;
		default:
			use_case = DEFAULT_STATE;
			break;
		}
	}
}

void EXTI2_IRQHandler(void) {
	/* Handle Lock Button Interrupt */
	handle_lock = !handle_lock;

	//clear pending flag of LINE_2
	Exti_ClearPendingFlag(LINE_2);
}

void EXTI3_IRQHandler(void) {
	/* Door Lock Button Interrupt*/
	if (handle_lock == DOOR_UNLOCKED){
		door_lock = !door_lock;
	}
	else if( (handle_lock == DOOR_LOCKED) && (door_lock == DOOR_OPENED ) ){
		door_lock = !door_lock;
	}

	//clear pending flag of LINE_3
	Exti_ClearPendingFlag(LINE_3);
}
