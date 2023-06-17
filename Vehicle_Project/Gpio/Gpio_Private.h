/* *****************************************************************************
 * Module: GPIO
 *
 * File Name: Gpio_Private.c
 *
 * Description: Header Private file for the STM32 GPIO driver
 *
 * Created on: Apr 16, 2023
 * Author: Omar Saad
 *
 *******************************************************************************/

#ifndef GPIO_PRIVATE_H
#define GPIO_PRIVATE_H

/******************* GPIO BASE ADDRESSES ********************/
#define GPIOA_BASE_ADDR 0x40020000
#define GPIOB_BASE_ADDR 0x40020400
#define GPIOC_BASE_ADDR 0x40020800
#define GPIOD_BASE_ADDR 0x40020C00
#define GPIOE_BASE_ADDR 0x40021000
#define GPIOH_BASE_ADDR 0x40021C00

/******************* GPIO OFFSET Structure ********************/
typedef struct {
	uint32 GPIO_MODER;    //mode register
	uint32 GPIO_OTYPER;   //output type register
	uint32 GPIO_OSPEEDR;  //output speed register
	uint32 GPIO_PUPDR;    //pull-up/pull-down register
	uint32 GPIO_IDR;	  //input data register
	uint32 GPIO_ODR;	  //output data register
	uint32 GPIO_BSRR;	  //bit set/reset register
	uint32 GPIO_LCKR;	  //configuration lock register
	uint32 GPIO_AFRL;	  //alternate function low register
	uint32 GPIO_AFRH;	  //alternate function high register
} GpioType;

#endif /* GPIO_PRIVATE_H */
