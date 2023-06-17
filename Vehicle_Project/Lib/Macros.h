
/* *****************************************************************************
 * Module: Macros
 *
 * File Name: Macros.h
 *
 * Description: Macros for Bit and Block operations
 *
 * Author: Omar Saad
 *
 *******************************************************************************/

#ifndef MACROS_H_
#define MACROS_H_

/******************************* BIT OPERATIONS *******************************/

/* Insert value in certain Bit in any Register*/
#define INSERT_BIT(REG, BIT, VALUE)    ((REG) = ((REG) & (~(1 << (BIT) )) )| (( VALUE & 1 ) << (BIT) ))
/* Set certain Bit in any Register*/
#define SET_BIT(REG, BIT)              ((REG) |=  (1 << (BIT)))
/* Clear certain Bit in any Register*/
#define CLEAR_BIT(REG, BIT)            ((REG) &= ~(1 << (BIT)))
/* Toggle certain  Bit in any Register*/
#define TOGGLE_BIT(REG, BIT)           (REG ^= (1 << (BIT)))
/* Read certain bit in any register and return 0 or 1 */
#define READ_BIT(REG, BIT)             (( (REG) >> (BIT) ) & 1)
/* Check if a specific bit is set in any register and return true if yes */
#define BIT_IS_SET(REG,BIT)            READ_BIT(REG, BIT)
/* Check if a specific bit is cleared in any register and return true if yes */
#define BIT_IS_CLEAR(REG,BIT)          (!READ_BIT(REG, BIT)  )

/******************************* BLOCK OPERATIONS *******************************/

/******************************* 2d BLOCK  ***********************/

/* Insert value in certain 2_Bits_Block in any Register*/
#define INSERT_2BITS_BLOCK(REG, BLOCK, VALUE) ((REG) = ((REG) & (~(0x03 << ((BLOCK)*2) )) )| ((VALUE & 0x03) << ((BLOCK)*2)))
/* Set certain 2_Bits_Block in any Register*/
#define SET_2BITS_BLOCK (REG,BLOCK)     ((REG) |= (0x03 << ((BLOCK)*2) ))
/* Clear a certain 2_Bits_Block in any Register */
#define CLEAR_2BITS_BLOCK (REG,BLOCK)   ((REG) &= (~(0x03 << ((BLOCK)*2) )))
/* Toggle 2_Bits_Block  Bit in any Register*/
#define Toggle_2BITS_BLOCK (REG,BLOCK)  ((REG) ^= (~(0x03 << ((BLOCK)*2) )))
/* Read certain 2_Bits_Block in any register and return 0, 1, 2 or 3 */
#define READ_2BITS_BLOCK(REG, BLOCK)    (( (REG) >> ((BLOCK)*2) ) & 0x03)

/******************************* 4d BLOCK  ***********************/

/* Insert value in certain 4_Bits_Block in any Register*/
#define INSERT_4BITS_BLOCK(REG, BLOCK, VALUE) ((REG) = ((REG) & (~(0x0F << ((BLOCK)*4) )) )| ((VALUE & 0x0F) << ((BLOCK)*4)))
/* Set certain 4_Bits_Block in any Register*/
#define SET_4BITS_BLOCK (REG,BLOCK)     ((REG) |= (0x0F << ((BLOCK)*4) ))
/* Clear a certain 4_Bits_Block in any Register */
#define CLEAR_4BITS_BLOCK (REG,BLOCK)   ((REG) &= (~(0x0F << ((BLOCK)*4) )))
/* Toggle 4_Bits_Block  Bit in any Register*/
#define Toggle_4BITS_BLOCK (REG,BLOCK)  ((REG) ^= (~(0x0F << ((BLOCK)*4) )))
/* Read certain 4_Bits_Block in any register and return 0, 1, 2 or 3 */
#define READ_4BITS_BLOCK(REG, BLOCK)    (( (REG) >> ((BLOCK)*4) ) & 0x0F)


#endif /* MACROS_H_ */
