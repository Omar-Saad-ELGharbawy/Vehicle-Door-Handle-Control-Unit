/* *****************************************************************************
 * Module: GPIO
 *
 * File Name: Gpio.c
 *
 * Description: Source file for the STM32 GPIO driver
 *
 * Created on: Apr 16, 2023
 * Author: Omar Saad
 *
 *******************************************************************************/

#include "Gpio.h"
#include "Gpio_Private.h"
#include "Utils.h"
#include "Macros.h"

/*******************************************************************************
 *                      Macros & Glopal Variables                                  *
 *******************************************************************************/

#define GPIO_REG(REG_ID, PORT_ID)  ((uint32 *)((REG_ID) + (PORT_ID)))

uint32 gpioAddresses[6] = {GPIOA_BASE_ADDR,GPIOB_BASE_ADDR,GPIOC_BASE_ADDR,GPIOD_BASE_ADDR,GPIOE_BASE_ADDR,GPIOH_BASE_ADDR};

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
void Gpio_ConfigPin(uint8 PortName, uint8 PinNum, uint8 PinMode, uint8 DefaultState, uint8 InputMode) {

	/*
	 * Check if the input port Name is greater than NUM_OF_PINS_PER_PORT value.
	 * Or if the input pin number is greater than NUM_OF_PINS_PER_PORT value.
	 * In this case the input is not valid port/pin number
	 */
	if((PinNum > NUM_OF_PINS_PER_PORT) || (PortName >= NUM_OF_PORTS))
	{
		/* Do Nothing */
	}
	else
	{
		uint8 portId = PortName - GPIO_A;
		GpioType * gpioRegs = gpioAddresses[portId];
//		GpioType * gpioRegs = (uint32 *) gpioAddresses[portId];

		/* Insert PinMode in PinNum Block in Moder Register*/
		INSERT_2BITS_BLOCK( gpioRegs->GPIO_MODER , PinNum, PinMode);
		/* Insert DefaultState in PinNum Bit in OTYPER Register*/
		INSERT_BIT(  gpioRegs->GPIO_OTYPER  , PinNum, DefaultState);
		if (GPIO_INPUT == PinMode )
		{
			/* Insert InputMode in PinNum Block in PUPDR Register*/
			INSERT_2BITS_BLOCK( gpioRegs->GPIO_PUPDR , PinNum, InputMode);
		}
	}
}

uint8 Gpio_WritePin(uint8 PortName, uint8 PinNum, uint8 Data) {

	uint8 portId = PortName - GPIO_A;
	GpioType * gpioRegs = gpioAddresses[portId];

	/*check if the pin is output*/
	if ( GPIO_OUTPUT == (READ_2BITS_BLOCK( gpioRegs->GPIO_MODER ,PinNum)) )
	{
		/* Insert Data in PinNum Bit in ODR Register*/
		INSERT_BIT( gpioRegs->GPIO_ODR , PinNum, Data);
		return OK;
	}
	else
	{
		return NOK;
	}
}

uint8 Gpio_ReadPin(uint8 PortName, uint8 PinNum){

	uint8 portId = PortName - GPIO_A;
	GpioType * gpioRegs = gpioAddresses[portId];

	/*check if the pin is input*/
	if ( GPIO_INPUT == (READ_2BITS_BLOCK(gpioRegs->GPIO_MODER ,PinNum)) )
	{
		/* Read Data in PinNum Bit in IDR Register*/
		return READ_BIT( gpioRegs->GPIO_IDR , PinNum);
		// return OK;
	}
//	else
//	{
////		 return NOK;
//	}

}
