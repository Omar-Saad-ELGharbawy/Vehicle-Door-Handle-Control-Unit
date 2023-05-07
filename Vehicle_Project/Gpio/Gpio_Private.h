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

/******************* GPIO  General OFFSET ********************/
#define GPIOx_MODER    (0x00)
#define GPIOx_OTYPER   (0x04)
#define GPIOx_OSPEEDR  (0x08)
#define GPIOx_PUPDR    (0x0C)
#define GPIOx_IDR      (0x10)
#define GPIOx_ODR      (0x14)
#define GPIOx_BSRR     (0x18)
#define GPIOx_LCKR     (0x1C)
#define GPIOx_AFRL     (0x20)
#define GPIOx_AFRH    (0x24)


#endif /* GPIO_PRIVATE_H */
