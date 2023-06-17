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
extern g_overflow_flag;

/*******************************************************************************
 *                                Main                                         *
 *******************************************************************************/
int main()
{
	/* Initialize RCC Driver */
	Rcc_Init();
	/* Enable Clock for GPIO PORT A*/
	Rcc_Enable(RCC_GPIOA);
	/* Enable Clock for GPIO PORT B*/
	Rcc_Enable(RCC_GPIOB);
	/* Enable Clock for TIMER 2*/
	Rcc_Enable(RCC_TIM2);

	/* Configure pins for Input Push Buttons as Active Low */
	Gpio_ConfigPin(GPIO_A, HANDLE_LOCK_BUTTON, GPIO_INPUT, GPIO_PUSH_PULL, GPIO_PULL_UP);
	Gpio_ConfigPin(GPIO_A, DOOR_LOCK_BUTTON, GPIO_INPUT, GPIO_PUSH_PULL, GPIO_PULL_UP);

	/* Configure pins for Output LEDS */
	Gpio_ConfigPin(GPIO_B, VEHICLE_LOCK_LED, GPIO_OUTPUT, GPIO_PUSH_PULL, GPIO_PULL_UP);
	Gpio_WritePin(GPIO_B, VEHICLE_LOCK_LED, LOW);
	Gpio_ConfigPin(GPIO_B, HAZARD_LIGHT_LED, GPIO_OUTPUT, GPIO_PUSH_PULL, GPIO_PULL_UP);
	Gpio_WritePin(GPIO_B, HAZARD_LIGHT_LED, LOW);
	Gpio_ConfigPin(GPIO_B, AMBIENT_LIGHT_LED, GPIO_OUTPUT, GPIO_PUSH_PULL, GPIO_PULL_UP);
	Gpio_WritePin(GPIO_B, AMBIENT_LIGHT_LED, LOW);

	/* Initialize GPT TIMER*/
	GPT_Init();

	/* Start Timer with overflow at 2000 ticks(ms) */
	// GPT_StartTimer(2000);

	uint32 i = 0;
	/* buttons flags */
	uint8 handle_button_flag = TRUE;
	uint8 door_button_flag = TRUE;

	uint8 Timer_10sec_started = FALSE;

	while (1)
	{
		/* **************Reading Inputs *************/

		/* **************Read Handle LOCK Button *************/

		/* Check if key pressed for Falling Edge and apply Debouncing*/
		if (Gpio_ReadPin(GPIO_A, HANDLE_LOCK_BUTTON) == BUTTON_PRESSED)
		{
			/*wait 30 ms by for loop for debouncing*/
			for (i = 0; i < 7500; i++)
			{
			}
			/*second check due to switch de-bouncing*/
			if (Gpio_ReadPin(GPIO_A, HANDLE_LOCK_BUTTON) == BUTTON_PRESSED)
			{
				if (handle_button_flag)
				{
					handle_lock = !handle_lock;
					/*set the button flag value to 1 to not enter here again until the button is released.*/
					handle_button_flag = FALSE;
				}
			}
		}
		else
		{
			/* button is released reset the button flag to value 0 again.*/
			handle_button_flag = TRUE;
		}

		/* *********** Read DOOR LOCK Button **************/

		/* Check if key pressed for Falling Edge and apply Debouncing*/
		if (Gpio_ReadPin(GPIO_A, DOOR_LOCK_BUTTON) == BUTTON_PRESSED)
		{
			/*wait 30 ms by for loop for debouncing*/
			for (i = 0; i < 7500; i++)
			{
			}
			/*second check due to switch de-bouncing*/
			if (Gpio_ReadPin(GPIO_A, DOOR_LOCK_BUTTON) == BUTTON_PRESSED)
			{
				if (door_button_flag)
				{
					door_lock = !door_lock;
					/*set the button flag value to 1 to not enter here again until the button is released.*/
					door_button_flag = FALSE;
				}
			}
		}
		else
		{
			/* button is released reset the button flag to value 0 again.*/
			door_button_flag = TRUE;
		}

		//		if (handle_lock == DOOR_LOCKED )
		//		{
		//			/* Close Door Lock LED*/
		//			Gpio_WritePin(GPIO_B, VEHICLE_LOCK_LED, LOW);
		//		}
		//		if (handle_lock == DOOR_UNLOCKED )
		//		{
		//			/* Open Door Lock LED*/
		//			Gpio_WritePin(GPIO_B, VEHICLE_LOCK_LED, HIGH);
		//		}
		//
		//		if (door_lock == DOOR_CLOSED )
		//		{
		//			/* Close HAZARD Lock LED*/
		//			Gpio_WritePin(GPIO_B, HAZARD_LIGHT_LED, LOW);
		//		}
		//		if (door_lock == DOOR_OPENED )
		//		{
		//			/* Open HAZARD Lock LED*/
		//			Gpio_WritePin(GPIO_B, HAZARD_LIGHT_LED, HIGH);
		//		}
		//
		//		if ( (handle_lock == DOOR_UNLOCKED ) && (door_lock == DOOR_OPENED)  )
		//		{
		//			/* Open Ambient Lock LED*/
		//			Gpio_WritePin(GPIO_B, AMBIENT_LIGHT_LED, HIGH);
		//		}else{
		//			Gpio_WritePin(GPIO_B, AMBIENT_LIGHT_LED, LOW);
		//		}

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
			/* Vehicle Door Unlocked but still Closed */
		case DOOR_UNLOCK:
			/* Vehicle Lock LED is ON*/
			Gpio_WritePin(GPIO_B, VEHICLE_LOCK_LED, HIGH);

			/*HAZARD LED is Blinking for one time ( 0.5 sec high and 0.5 sec low ) */
			/* Ambient Light Led is on for 2 seconds */
			Gpio_WritePin(GPIO_B, HAZARD_LIGHT_LED, HIGH);
			Gpio_WritePin(GPIO_B, AMBIENT_LIGHT_LED, HIGH);
			/* Start counting for 10 seconds
			 * If no buttons is pressed
			 * go to ANTI_THEFT_LOCK state */
			if (GPT_CheckTimeIsElapsed() == TIMER_NOT_STARTED)
			{
				/* Start timer for 10 seconds */
				GPT_StartTimer(10000);
				Timer_10sec_started = TRUE;
			}

			/* Timer Conditions */
			/* If no overflow occured blink leds */
			if (g_overflow_flag == NO_OVERFLOW)
			{
				if ((GPT_GetElapsedTime() >= 499) && (GPT_GetElapsedTime() < 999))
				{
					Gpio_WritePin(GPIO_B, HAZARD_LIGHT_LED, LOW);
				}
				else if (GPT_GetElapsedTime() >= 1999)
				{
					/* if 2 seconds ended close the leds */
					Gpio_WritePin(GPIO_B, HAZARD_LIGHT_LED, LOW);
					Gpio_WritePin(GPIO_B, AMBIENT_LIGHT_LED, LOW);
				}
				else if (GPT_CheckTimeIsElapsed() == OVERFLOW)
				{
					/* if overflow occurs (10 seconds ended) close the leds and go to ANTI_THEFT_LOCK State */
					// Gpio_WritePin(GPIO_B, HAZARD_LIGHT_LED, LOW);
					// Gpio_WritePin(GPIO_B, AMBIENT_LIGHT_LED, LOW);
					use_case = ANTI_THEFT_LOCK;
					Timer_10sec_started = FALSE;
					// g_overflow_flag = TRUE;
				}
			}
			/* if overflow occurs (10 seconds ended) close the leds and go to ANTI_THEFT_LOCK State */
			else if (g_overflow_flag == OVERFLOW)
			{
				// Gpio_WritePin(GPIO_B, HAZARD_LIGHT_LED, LOW);
				// Gpio_WritePin(GPIO_B, AMBIENT_LIGHT_LED, LOW);
				use_case = ANTI_THEFT_LOCK;
				Timer_10sec_started = FALSE;
			}
			/* check buttons*/
			if (door_lock == DOOR_OPENED)
			{
				use_case = DOOR_IS_OPEN;
			}
			if (handle_lock == DOOR_LOCKED)
			{
				use_case = LOCKING_THE_DOOR;
			}
			break;
			/*Vehicle Door Unlocked and Door is Open */
		case DOOR_IS_OPEN:
			/* Ambient LED is ON*/
			Gpio_WritePin(GPIO_B, AMBIENT_LIGHT_LED, HIGH);

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
			/*HAZARD LED is Blinking for 2 time ( 0.5 sec high and 0.5 sec low ) for each bloink */
			// Gpio_WritePin(GPIO_B, HAZARD_LIGHT_LED, LOW);

			if (GPT_CheckTimeIsElapsed() == TIMER_NOT_STARTED)
			{
				/* Start timer for 2 seconds */
				GPT_StartTimer(2000);
			}

			/* Timer Conditions */
			/* If no overflow occured blink leds */
			if (g_overflow_flag == NO_OVERFLOW)
			{
				if ((GPT_GetElapsedTime() > 500) && (GPT_GetElapsedTime() < 1000))
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
				else if (GPT_CheckTimeIsElapsed() == OVERFLOW)
				{
					/* if overflow occurs (2 seconds ended) close the leds and go to DEFAULT_STATE */
					// Gpio_WritePin(GPIO_B, HAZARD_LIGHT_LED, LOW);
					handle_lock = DOOR_LOCKED;
					use_case = DEFAULT_STATE;
					// g_overflow_flag = TRUE;
				}
			}
			/* if overflow occurs (2 seconds ended) close the leds and go to DEFAULT_STATE */
			else if (g_overflow_flag == OVERFLOW)
			{
				// Gpio_WritePin(GPIO_B, HAZARD_LIGHT_LED, LOW);
				handle_lock = DOOR_LOCKED;
				use_case = DEFAULT_STATE;
			}
			break;
			/*Vehicle Door Unlocked and Door is Closed */
		case CLOSING_THE_DOOR:
			/* VEHICLE LED IS OFF */
			Gpio_WritePin(GPIO_B, VEHICLE_LOCK_LED, LOW);
			/* HAZARD LED IS OFF */
			Gpio_WritePin(GPIO_B, HAZARD_LIGHT_LED, LOW);

			/* Ambient Led is ON for 1 second then OFF */
			Gpio_WritePin(GPIO_B, AMBIENT_LIGHT_LED, HIGH);

			/* check if timer started in door unlock state but the 10 seconds didnot finish*/
			if ( (GPT_CheckTimeIsElapsed() == TIMER_NOT_STARTED) || (Timer_10sec_started == TRUE) )
			{
				/* Start timer for 1 second */
				GPT_StartTimer(1000);
			}
			/* Timer Conditions */
			if (g_overflow_flag == NO_OVERFLOW)
			{
				if (GPT_CheckTimeIsElapsed() == OVERFLOW)
				{
					/* if overflow occurs (1 seconds ended) close the AMBIENT_LIGHT_LED  */
					Gpio_WritePin(GPIO_B, AMBIENT_LIGHT_LED, LOW);
					// g_overflow_flag = TRUE;
				}
			}
			//
			else if (g_overflow_flag == OVERFLOW)
			{
				/* if overflow occurs (1 seconds ended) close the AMBIENT_LIGHT_LED  */
				Gpio_WritePin(GPIO_B, AMBIENT_LIGHT_LED, LOW);
			}

			/* Check buttons */
			if (handle_lock == DOOR_LOCKED)
			{
				use_case = LOCKING_THE_DOOR;
			}
			break;
			/*Vehicle Door Locked and Door is Closed */
		case LOCKING_THE_DOOR:
			/* VEHICLE LED IS OFF */
			Gpio_WritePin(GPIO_B, VEHICLE_LOCK_LED, LOW);

			Gpio_WritePin(GPIO_B, AMBIENT_LIGHT_LED, LOW);

			/*HAZARD LED is Blinking for 2 times */
			Gpio_WritePin(GPIO_B, HAZARD_LIGHT_LED, HIGH);

			/* check if timer started in door unlock state but the 10 seconds didnot finish*/
			if ( (GPT_CheckTimeIsElapsed() == TIMER_NOT_STARTED) || (Timer_10sec_started == TRUE) )
			{
				/* Start timer for 2 seconds */
				GPT_StartTimer(2000);
			}

			/* Timer Conditions */
			/* If no overflow occured blink leds */
			if (g_overflow_flag == NO_OVERFLOW)
			{
				if ((GPT_GetElapsedTime() > 500) && (GPT_GetElapsedTime() < 1000))
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
				else if (GPT_CheckTimeIsElapsed() == OVERFLOW)
				{
					/* if overflow occurs (2 seconds ended) close the leds and go to DEFAULT_STATE */
					// Gpio_WritePin(GPIO_B, HAZARD_LIGHT_LED, LOW);
					handle_lock = DOOR_LOCKED;
					use_case = DEFAULT_STATE;
					// g_overflow_flag = TRUE;
				}
			}
			/* if overflow occurs (2 seconds ended) close the leds and go to DEFAULT_STATE */
			else if (g_overflow_flag == OVERFLOW)
			{
				// Gpio_WritePin(GPIO_B, HAZARD_LIGHT_LED, LOW);
				handle_lock = DOOR_LOCKED;
				use_case = DEFAULT_STATE;
			}
			break;
		default:
			use_case = DEFAULT_STATE;
			break;
		}
	}
}
