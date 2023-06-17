/* *****************************************************************************
 * Module: NVIC
 *
 * File Name: NVIC.c
 *
 * Description: Source file for the STM32 NVIC driver
 *
 * Author: Omar Saad
 *
 *******************************************************************************/

#include "NVIC.h"
#include "NVIC_Private.h"
#include "Macros.h"
#include "Gpio.h"


/* Static Function */
// convert LineNum to the corresponding EXT_IRQ_POSITION
uint8 Convert_Line_To_IRQ(uint8 LineNum){
	uint8 IRQ_Position;

	switch (LineNum)
	{
	case LINE_0:
	case LINE_1:
	case LINE_2:
	case LINE_3:
		IRQ_Position = LineNum + 6;
		break;
	case 4:
		IRQ_Position = EXTI4_IRQ_POSITION;
		break;
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		IRQ_Position = EXTI5_9_IRQ_POSITION;
		break;
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		IRQ_Position = EXTI10_15_IRQ_POSITION;
		break;
	default:
		IRQ_Position = 0;
		break;
	}
	return IRQ_Position;
}


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/* Function Description
 * Exti_Init
 * Description : Initialize the External Interrupt by
 * 1- Selecting the port of the required pin to enter to the EXTI Line (SYSCFG_EXTICR)
 * 2- Enable the required EXTI LINE for the selected pin (configure Mask Bits) (EXTI_IMR)
 * 3- Select the trigger selection (RTSR or FTSR) for the selected line
 * 4- Configure the GPIO Pins
 */

void Exti_Init(uint8 PortName, uint8 LineNum ,uint8 Trigger_Type )
{
	// LineNum is PinNum
	/* 1- Selecting the port of the required pin */
	switch( (uint8) LineNum / 4)
	{
		case 0:
			/* 0-3 */
			INSERT_4BITS_BLOCK(SYSCFG->EXTICR1, LineNum % 4 , PortName);
			break;
		case 1:
			/* 4-7 */
			INSERT_4BITS_BLOCK(SYSCFG->EXTICR2,  LineNum % 4, PortName);
			break;
		case 2:
			/* 8-11 */
			INSERT_4BITS_BLOCK(SYSCFG->EXTICR3,  LineNum % 4, PortName);
			break;
		case 3:
			/* 12-15 */
			INSERT_4BITS_BLOCK(SYSCFG->EXTICR4,  LineNum % 4, PortName);
			break;
		default:
			break;
	}
	/* 2- configure Mask Bits (enable exti_LineNum ) */
	SET_BIT(EXTI->IMR, LineNum);

	/* 3- Configure trigger selection for the selected line */
	switch (Trigger_Type)
	{
	case RISING_EDGE :
		SET_BIT(EXTI->RTSR, LineNum); // rising edge detection
		break;
	case FALLING_EDGE:
		SET_BIT(EXTI->FTSR, LineNum);  // falling edge detection
		break;
	case RISING_FALLING_EDGE :
		SET_BIT(EXTI->RTSR, LineNum); // rising edge detection
		SET_BIT(EXTI->FTSR, LineNum);  // falling edge detection
		break;
	default:
		SET_BIT(EXTI->FTSR, LineNum);  // falling edge detection
		break;
	}

	Gpio_ConfigPin(PortName, LineNum, GPIO_INPUT, GPIO_PUSH_PULL, GPIO_PULL_UP);
}

/* Exti_Enable
 * Description :Enable External Interrupt by setting Interrupt set-enable register
 */
void  Exti_Enable(uint8 LineNum)
{
	// convert LineNum to the corresponding EXT_IRQ_POSITION
	uint8 IRQ_Position= Convert_Line_To_IRQ(LineNum);

	/* Enable line on NVIC  */
	SET_BIT(NVIC->ISER[(uint8)IRQ_Position/32], IRQ_Position % 32);
}

/* Exti_Disable
 * Description :Disable External Interrupt by setting Interrupt clear-enable register
 */
void Exti_Disable(uint8 LineNum)
{
	// convert LineNum to the corresponding EXT_IRQ_POSITION
	uint8 IRQ_Position= Convert_Line_To_IRQ(LineNum);
	/* Disable line on NVIC  */
	SET_BIT(NVIC->ICER[(uint8)IRQ_Position/32], IRQ_Position % 32);
}

/* Exti_SetPriority
 * Description :Set the priority of the External Interrupt by setting the priority of the IRQn
 */
void Exti_SetPriority(uint8 IRQ_Position , uint8 Priority_Level){
	// calculate the IPR register number from the IRQ_Position (x)
	uint8 IPR_INDEX = (uint8) IRQ_Position / 4;
	// calculate the start BIT position from the IRQ_Position
	uint8 bit_shift = 4 * IPR_INDEX + (IRQ_Position % 4) * BYTE_OFFSET;

	/* clear first the 8 bits */
	NVIC->IPR[IPR_INDEX] &=  (0x00 << bit_shift) ;
	/* insert priority for the selected IRQ */
	NVIC->IPR[IPR_INDEX] |= (Priority_Level << bit_shift);
}

/* Exti_ClearPendingFlag
 * Description :Clear the pending flag of the External Interrupt by setting the pending flag bit in the Interrupt clear-pending register
 */
void Exti_ClearPendingFlag(uint8 LineNum){
	SET_BIT(EXTI->PR, LineNum);
}
