/*
 * main.c
 *
 *  Created on: May 7, 2023
 *      Author: Omar Saad
 */

#include "GPT.h"
#include "Gpio.h"
#include "Rcc.h"
#include "Std_Types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* Push Buttons Configurations*/
#define HANDLE_LOCK_BUTTON 2
#define DOOR_LOCK_BUTTON   3

/* LEDs Configurations*/
#define VEHICLE_LOCK_LED 5
#define HAZARD_LIGHT_LED  6
#define AMBIENT_LIGHT_LED 7

/* Active High Led States*/
#define BUTTON_PRESSED            LOW
#define BUTTON_RELEASED           HIGH

 /* Active LOW Push Buttons States*/
#define LED_ON  HIGH
#define LED_OFF  LOW

/*******************************************************************************
 *                                Main                                         *
 *******************************************************************************/


int main() {
	/* Initialize RCC Driver */
	Rcc_Init();
	/* Enable Clock for GPIO PORT A*/
	Rcc_Enable(RCC_GPIOA);
	/* Enable Clock for GPIO PORT B*/
	Rcc_Enable(RCC_GPIOB);


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


	while (1)
	{

	}
}

