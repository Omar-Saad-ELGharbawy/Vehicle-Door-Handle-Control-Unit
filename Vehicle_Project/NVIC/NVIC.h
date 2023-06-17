/* *****************************************************************************
 * Module: NVIC
 *
 * File Name: NVIC.h
 *
 * Description: Header file for the STM32 NVIC driver
 *
 * Author: Omar Saad
 *
 *******************************************************************************/

#ifndef NVIC_H_
#define NVIC_H_

#include "Std_Types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define BYTE_OFFSET  8

/* External Interrupts IRQ positions */
#define EXTI0_IRQ_POSITION 6
#define EXTI1_IRQ_POSITION 7
#define EXTI2_IRQ_POSITION 8
#define EXTI3_IRQ_POSITION 9
#define EXTI4_IRQ_POSITION 10
#define EXTI5_9_IRQ_POSITION 23
#define EXTI5_IRQ_POSITION 23
#define EXTI6_IRQ_POSITION 23
#define EXTI7_IRQ_POSITION 23
#define EXTI8_IRQ_POSITION 23
#define EXTI9_IRQ_POSITION 23
#define EXTI10_15_IRQ_POSITION 40
#define EXTI10_IRQ_POSITION 40
#define EXTI11_IRQ_POSITION 40
#define EXTI12_IRQ_POSITION 40
#define EXTI13_IRQ_POSITION 40
#define EXTI14_IRQ_POSITION 40
#define EXTI15_IRQ_POSITION 40

/* Ports */
#define PORT_A	0
#define PORT_B	1
#define PORT_C	2
#define PORT_D	3
#define PORT_E	4
#define PORT_H	7

/* LineNum (PinNum) */
#define LINE_0	0
#define LINE_1	1
#define LINE_2	2
#define LINE_3	3
#define LINE_4	4
#define LINE_5	5
#define LINE_6	6
#define LINE_7	7
#define LINE_8	8
#define LINE_9	9
#define LINE_10	10
#define LINE_11	11
#define LINE_12	12
#define LINE_13	13
#define LINE_14	14
#define LINE_15	15

/* Trigger_Type */
#define RISING_EDGE		0
#define FALLING_EDGE	1
#define RISING_FALLING_EDGE	 2

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
/* Function Description
 * Exti_Init
 * Description : Initialize the External Interrupt by
 * 1- Selecting the port of the required pin to enter to the EXTI Line (SYSCFG_EXTICR)
 * 2- Enable the required EXTI LINE for the selected pin (EXTI_IMR)
 * 3- Select the trigger selection (RTSR or FTSR) for the selected line
 * 4- Configure the GPIO Pins
 */
void Exti_Init(uint8 PortName, uint8 LineNum ,uint8 Trigger_Type );

/* Exti_Enable
 * Description :Enable External Interrupt by setting Interrupt set-enable register
 */
void Exti_Enable(uint8 LineNum);

/* Exti_Disable
 * Description :Disable External Interrupt by setting Interrupt clear-enable register
 */
void Exti_Disable(uint8 LineNum);

/* Exti_SetPriority
 * Description :Set the priority of the External Interrupt by setting the priority of the IRQn
 */
void Exti_SetPriority(uint8 IRQ_Position , uint8 Priority_Level);

/* Exti_ClearPendingFlag
 * Description :Clear the pending flag of the External Interrupt by setting the pending flag bit in the Interrupt clear-pending register
 */
void Exti_ClearPendingFlag(uint8 LineNum);

#endif /* NVIC_H_ */
