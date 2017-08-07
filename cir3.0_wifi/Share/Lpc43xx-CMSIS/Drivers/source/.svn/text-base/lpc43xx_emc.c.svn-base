/**********************************************************************
* $Id: lpc43xx_emc.c 8765 2011-12-08 00:51:21Z nxp21346 $		lpc43xx_emc.c		2011-12-07
*//**
* @file		lpc43xx_emc.c
* @brief	Contains all functions support for Clock Generation and Control
* 			firmware library on lpc43xx
* @version	1.0
* @date		07. December. 2011
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2011, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors’
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/

#include "LPC43xx.h"
#include "lpc43xx_emc.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_gpio.h"

#define M32(x)	*((uint32_t *)x)
#define DELAYCYCLES(ns) (ns / ((1.0 / __EMCHZ) * 1E9))

void emc_WaitUS(volatile uint32_t us)
{
	us *= (SystemCoreClock / 1000000) / 3;
	while(us--);
}

void emc_WaitMS(uint32_t ms)
{
	emc_WaitUS(ms * 1000);
}

void MemoryPinInit(void)
{

  /* DATA LINES 0..31 > D0..D31 */
	/* P1_7 - EXTBUS_D0 — External memory data line 0 */
	scu_pinmux(0x1,  7,  EMC_IO, FUNC3);  /* P1_7: D0 (function 0) errata */
	scu_pinmux(0x1,  8,  EMC_IO, FUNC3);  /* P1_8: D1 (function 0) errata */
	scu_pinmux(0x1,  9,  EMC_IO, FUNC3);  /* P1_9: D2 (function 0) errata */
	scu_pinmux(0x1,  10, EMC_IO, FUNC3);  /* P1_10: D3 (function 0) errata */
	scu_pinmux(0x1,  11, EMC_IO, FUNC3);  /* P1_11: D4 (function 0) errata */
	scu_pinmux(0x1,  12, EMC_IO, FUNC3);  /* P1_12: D5 (function 0) errata */
	scu_pinmux(0x1,  13, EMC_IO, FUNC3);  /* P1_13: D6 (function 0) errata */
	scu_pinmux(0x1,  14, EMC_IO, FUNC3);  /* P1_14: D7 (function 0) errata */
// 	scu_pinmux(0x5,  4,  EMC_IO, FUNC2);  /* P5_4: D8 (function 0) errata */
// 	scu_pinmux(0x5,  5,  EMC_IO, FUNC2);  /* P5_5: D9 (function 0) errata */
// 	scu_pinmux(0x5,  6,  EMC_IO, FUNC2);  /* P5_6: D10 (function 0) errata */
// 	scu_pinmux(0x5,  7,  EMC_IO, FUNC2);  /* P5_7: D11 (function 0) errata */
// 	scu_pinmux(0x5,  0,  EMC_IO, FUNC2);  /* P5_0: D12 (function 0) errata */
// 	scu_pinmux(0x5,  1,  EMC_IO, FUNC2);  /* P5_1: D13 (function 0) errata */
// 	scu_pinmux(0x5,  2,  EMC_IO, FUNC2);  /* P5_2: D14 (function 0) errata */
// 	scu_pinmux(0x5,  3,  EMC_IO, FUNC2);  /* P5_3: D15 (function 0) errata */
#if defined(KEIL_BOARD)
	// activate D16-D31 (32-bit memories)
// 	scu_pinmux(0xD,  2,  EMC_IO, FUNC2);  /* PD_2: D16 (function 0) errata */
// 	scu_pinmux(0xD,  3,  EMC_IO, FUNC2);  /* PD_3: D17 (function 0) errata */
// 	scu_pinmux(0xD,  4,  EMC_IO, FUNC2);  /* PD_4: D18 (function 0) errata */
// 	scu_pinmux(0xD,  5,  EMC_IO, FUNC2);  /* PD_5: D19 (function 0) errata */
// 	scu_pinmux(0xD,  6,  EMC_IO, FUNC2);  /* PD_6: D20 (function 0) errata */
// 	scu_pinmux(0xD,  7,  EMC_IO, FUNC2);  /* PD_7: D21 (function 0) errata */
// 	scu_pinmux(0xD,  8,  EMC_IO, FUNC2);  /* PD_8: D22 (function 0) errata */
// 	scu_pinmux(0xD,  9,  EMC_IO, FUNC2);  /* PD_9: D23 (function 0) errata */
// 	scu_pinmux(0xE,  5,  EMC_IO, FUNC3);  /* PE_5: D24 (function 0) errata */
// 	scu_pinmux(0xE,  6,  EMC_IO, FUNC3);  /* PE_6: D25 (function 0) errata */
// 	scu_pinmux(0xE,  7,  EMC_IO, FUNC3);  /* PE_7: D26 (function 0) errata */
// 	scu_pinmux(0xE,  8,  EMC_IO, FUNC3);  /* PE_8: D27 (function 0) errata */
// 	scu_pinmux(0xE,  9,  EMC_IO, FUNC3);  /* PE_9: D28 (function 0) errata */
// 	scu_pinmux(0xE, 10,  EMC_IO, FUNC3);  /* PE_10: D29 (function 0) errata */
// 	scu_pinmux(0xE, 11,  EMC_IO, FUNC3);  /* PE_11: D30 (function 0) errata */
// 	scu_pinmux(0xE, 12,  EMC_IO, FUNC3);  /* PE_12: D31 (function 0) errata */
#endif
  
  /* ADDRESS LINES A0..A11 > A0..A11 */
	scu_pinmux(0x2,  9,  EMC_IO, FUNC3);	/* P2_9 - EXTBUS_A0 — External memory address line 0 */
	scu_pinmux(0x2, 10,  EMC_IO, FUNC3);	/* P2_10 - EXTBUS_A1 — External memory address line 1 */	
	scu_pinmux(0x2, 11,  EMC_IO, FUNC3);	/* P2_11 - EXTBUS_A2 — External memory address line 2 */	
	scu_pinmux(0x2, 12,  EMC_IO, FUNC3);	/* P2_12 - EXTBUS_A3 — External memory address line 3 */
	scu_pinmux(0x2, 13,  EMC_IO, FUNC3);	/* P2_13 - EXTBUS_A4 — External memory address line 4 */	
	scu_pinmux(0x1,  0,  EMC_IO, FUNC2);	/* P1_0 - EXTBUS_A5 — External memory address line 5 */
	scu_pinmux(0x1,  1,  EMC_IO, FUNC2);	/* P1_1 - EXTBUS_A6 — External memory address line 6 */	
// 	scu_pinmux(0x1,  2,  EMC_IO, FUNC2);	/* P1_2 - EXTBUS_A7 — External memory address line 7 */	
// 	scu_pinmux(0x2,  8,  EMC_IO, FUNC3);	/* P2_8 - EXTBUS_A8 — External memory address line 8 */
// 	scu_pinmux(0x2,  7,  EMC_IO, FUNC3);	/* P2_7 - EXTBUS_A9 — External memory address line 9 */	
// 	scu_pinmux(0x2,  6,  EMC_IO, FUNC2);	/* P2_6 - EXTBUS_A10 — External memory address line 10 */
// 	scu_pinmux(0x2,  2,  EMC_IO, FUNC2);	/* P2_2 - EXTBUS_A11 — External memory address line 11 */
// 	scu_pinmux(0x2,  1,  EMC_IO, FUNC2);	/* P2_1 - EXTBUS_A12 — External memory address line 12 */
// 	scu_pinmux(0x2,  0,  EMC_IO, FUNC2);	/* P2_0 - EXTBUS_A13 — External memory address line 13 */	
// 	scu_pinmux(0x6,  8,  EMC_IO, FUNC1);	/* P6_8 - EXTBUS_A14 — External memory address line 14 */
// 	scu_pinmux(0x6,  7,  EMC_IO, FUNC1);	/* P6_7 - EXTBUS_A15 — External memory address line 15 */	
// 	scu_pinmux(0xD, 16,  EMC_IO, FUNC2);	/* PD_16 - EXTBUS_A16 — External memory address line 16 */
// 	scu_pinmux(0xD, 15,  EMC_IO, FUNC2);	/* PD_15 - EXTBUS_A17 — External memory address line 17 */	
// 	scu_pinmux(0xE,  0,  EMC_IO, FUNC3);	/* PE_0 - EXTBUS_A18 — External memory address line 18 */
// 	scu_pinmux(0xE,  1,  EMC_IO, FUNC3);	/* PE_1 - EXTBUS_A19 — External memory address line 19 */
// 	scu_pinmux(0xE,  2,  EMC_IO, FUNC3);	/* PE_2 - EXTBUS_A20 — External memory address line 20 */
// 	scu_pinmux(0xE,  3,  EMC_IO, FUNC3);	/* PE_3 - EXTBUS_A21 — External memory address line 21 */
// 	scu_pinmux(0xE,  4,  EMC_IO, FUNC3);	/* PE_4 - EXTBUS_A22 — External memory address line 22 */	
// 	scu_pinmux(0xA,  4,  EMC_IO, FUNC3);	/* PA_4 - EXTBUS_A23 — External memory address line 23 */

  /* BYTE ENABLES */
// 	scu_pinmux(0x1,  4,  EMC_IO, FUNC3);	/* P1_4 - EXTBUS_BLS0 — LOW active Byte Lane select signal 0 */
// 	scu_pinmux(0x6,  6,  EMC_IO, FUNC1);	/* P6_6 - EXTBUS_BLS1 — LOW active Byte Lane select signal 1 */	
// 	scu_pinmux(0xD, 13,  EMC_IO, FUNC2);	/* PD_13 - EXTBUS_BLS2 — LOW active Byte Lane select signal 2 */
// 	scu_pinmux(0xD, 10,  EMC_IO, FUNC2);	/* PD_10 - EXTBUS_BLS3 — LOW active Byte Lane select signal 3 */		
    
// 	scu_pinmux(0x6,  9,  EMC_IO, FUNC3);  /* P6_9: EXTBUS_DYCS0  (function 0) > CS# errata */
// 	scu_pinmux(0x1,  6,  EMC_IO, FUNC3);  /* P1_6: WE (function 0) errata */
// 	scu_pinmux(0x6,  4,  EMC_IO, FUNC3);  /* P6_4: CAS  (function 0) > CAS# errata */
// 	scu_pinmux(0x6,  5,  EMC_IO, FUNC3);  /* P6_5: RAS  (function 0) > RAS# errata */

// 	LPC_SCU_CLK(0) = 0 + EMC_IO; 					/* SFSCLK0: EXTBUS_CLK0  (function 0, from datasheet) > CLK ds */
// 	LPC_SCU_CLK(1) = 0 + EMC_IO; 					/* SFSCLK1: EXTBUS_CLK1  (function 2, from datasheet) */
// 	LPC_SCU_CLK(2) = 0 + EMC_IO; 					/* SFSCLK2: EXTBUS_CLK2  (function 2, from datasheet) */
// 	LPC_SCU_CLK(3) = 0 + EMC_IO; 					/* SFSCLK3: EXTBUS_CLK3  (function 2, from datasheet) */

// 	scu_pinmux(0x6, 11,  EMC_IO, FUNC3);  /* P6_11: CKEOUT0  (function 0) > CKE errata */
// 	scu_pinmux(0x6, 12,  EMC_IO, FUNC3);  /* P6_12: DQMOUT0  (function 0) > DQM0 errata */
// 	scu_pinmux(0x6, 10,  EMC_IO, FUNC3);  /* P6_10: DQMOUT1  (function 0) > DQM1 errata */
// 	scu_pinmux(0xD,  0,  EMC_IO, FUNC2);  /* PD_0: DQMOUT2  (function 2, from datasheet) > DQM2 errata */
// 	scu_pinmux(0xE, 13,  EMC_IO, FUNC3);  /* PE_13: DQMOUT3  (function 3, from datasheet) > DQM3 errata */

	scu_pinmux(0x1,	 3,	 EMC_IO, FUNC3);		//P1_3: OE
	scu_pinmux(0x1,	 6,	 EMC_IO, FUNC3);		//P1_6: WE

}

void EMCFlashInit(void)
{
#if defined(HITEX_BOARD)
	// Hitex board SST39VF3201B Flash
	// Read Cycle Time 70 nS minimum
	// Chip Enable Access Time 70 ns maximum
	// Address Access Time 70 ns max
	// Toe 35 ns max
	// CE/OE high to inactive output 16 ns

	/* Set up EMC Controller */
	LPC_EMC->STATICWAITRD0 = DELAYCYCLES(70)+1;

	LPC_EMC->STATICWAITPAG0 = DELAYCYCLES(70)+1;
	LPC_EMC->CONTROL = 0x01;

	LPC_EMC->STATICCONFIG0 = (1UL<<7) | (1UL);
	LPC_EMC->STATICWAITOEN0 = DELAYCYCLES(35)+1;
#elif defined(KEIL_BOARD)
	// Hitex board S29GL064N90 Flash
	// Read Cycle Time 90 nS minimum
	// Chip Enable Access Time 90 ns maximum
	// Address Access Time 90 ns max
	// Toe 25 ns max
	// CE/OE high to inactive output 20 ns

	/* Set up EMC Controller */
	LPC_EMC->STATICWAITRD0 = DELAYCYCLES(90)+1;

	LPC_EMC->STATICWAITPAG0 = DELAYCYCLES(90)+1;
	LPC_EMC->CONTROL = 0x01;

	LPC_EMC->STATICCONFIG0 = (1UL<<7) | (2UL);
	LPC_EMC->STATICWAITOEN0 = DELAYCYCLES(25)+1;
#else
	#error Define KEIL_BOARD or HITEX_BOARD
#endif

    /*Enable Buffer for External Flash*/
    LPC_EMC->STATICCONFIG0 |= 1<<19;
}

/*********************************************************************//**
 * @brief         Configure the memory width
 *
 * @param[in]    index index number, should be from 0 to 3
 *
 * @param[in]     mem_width  memory width, should be:
 *
 *                - EMC_STATIC_CFG_MW_8BITS: 8-bits
 *
 *                - EMC_STATIC_CFG_MW_16BITS: 16-bits
 *
 *                - EMC_STATIC_CFG_MW_32BITS02: 32-bits
 *
 * @return         EMC_FUNC_OK/EMC_FUNC_INVALID_PARAM
 **********************************************************************/
EMC_FUNC_CODE EMC_StaMemConfigMW(uint32_t index , uint32_t mem_width)
{
    uint32_t mem_width_flg = 0;
    switch(mem_width)
    {
        case 8:
            mem_width_flg = EMC_STATIC_CFG_MW_8BITS;
            break;
        case 16:
            mem_width_flg = EMC_STATIC_CFG_MW_16BITS;
            break;
        case 32:
            mem_width_flg = EMC_STATIC_CFG_MW_32BITS;
            break;
        default:
            return EMC_FUNC_INVALID_PARAM;
    }
    switch ( index)
    {
        case 0:
            LPC_EMC->STATICCONFIG0 &= ~ EMC_STATIC_CFG_MEMWIDTH_BMASK;
            LPC_EMC->STATICCONFIG0 |= mem_width_flg;
            break;
        
        case 1:
            LPC_EMC->STATICCONFIG1 &= ~ EMC_STATIC_CFG_MEMWIDTH_BMASK;
            LPC_EMC->STATICCONFIG1 |= mem_width_flg;
            break;
        
        case 2:
            LPC_EMC->STATICCONFIG2 &= ~ EMC_STATIC_CFG_MEMWIDTH_BMASK;
            LPC_EMC->STATICCONFIG2 |= mem_width_flg;
            break;
        
        case 3:
            LPC_EMC->STATICCONFIG3 &= ~ EMC_STATIC_CFG_MEMWIDTH_BMASK;
            LPC_EMC->STATICCONFIG3 |= mem_width_flg;
            break;
    default:
            return EMC_FUNC_INVALID_PARAM;
    }
    return EMC_FUNC_OK;
}
/*********************************************************************//**
 * @brief         Configure the page mode
 *
 * @param[in]    index index number, should be from 0 to 3
 *
 * @param[in]     page_mode  page mode, should be:
 *
 *                - EMC_CFG_PM_DISABLE: disable
 *
 *                 - EMC_CFG_PM_ASYNC_ENABLE: asynchronous page mode enable
 *
 * @return         EMC_FUNC_OK/EMC_FUNC_INVALID_PARAM
 **********************************************************************/
EMC_FUNC_CODE EMC_StaMemConfigPM(uint32_t index , uint32_t page_mode)
{
    switch ( index)
    {
        case 0:
            LPC_EMC->STATICCONFIG0  &= ~EMC_STATIC_CFG_PAGEMODE_MASK;
            LPC_EMC->STATICCONFIG0 |= page_mode&EMC_STATIC_CFG_PAGEMODE_MASK;
            break;
        
        case 1:
            LPC_EMC->STATICCONFIG1  &= ~EMC_STATIC_CFG_PAGEMODE_MASK;
            LPC_EMC->STATICCONFIG1  |= page_mode&EMC_STATIC_CFG_PAGEMODE_MASK;
            break;
        
        case 2:
            LPC_EMC->STATICCONFIG2  &= ~EMC_STATIC_CFG_PAGEMODE_MASK;
            LPC_EMC->STATICCONFIG2  |= page_mode&EMC_STATIC_CFG_PAGEMODE_MASK;
            break;
        
        case 3:
            LPC_EMC->STATICCONFIG3  &= ~EMC_STATIC_CFG_PAGEMODE_MASK;
            LPC_EMC->STATICCONFIG3  |= page_mode&EMC_STATIC_CFG_PAGEMODE_MASK;
            break;
    default:
            return EMC_FUNC_INVALID_PARAM;
    }
    return EMC_FUNC_OK;
}


/*********************************************************************//**
 * @brief         Configure the chip select polarity
 *
 * @param[in]    index index number, should be from 0 to 3
 *
 * @param[in]     pagepol_val_mode  page mode, should be:
 *
 *                 - EMC_CFG_BYTELAND_PC_ACTIVE_LO: Active LOW ship select
 *
 *                 - EMC_CFG_BYTELAND_PC_ACTIVE_HI: Active HIGH chip select
 *
 * @return         EMC_FUNC_OK/EMC_FUNC_INVALID_PARAM
 **********************************************************************/
EMC_FUNC_CODE EMC_StaMemConfigPC(uint32_t index , uint32_t pol_val)
{
    switch ( index)
    {
        case 0:
            LPC_EMC->STATICCONFIG0 &= ~EMC_STATIC_CFG_CHIPPOLARITY_MASK;
            LPC_EMC->STATICCONFIG0  |=  pol_val&EMC_STATIC_CFG_CHIPPOLARITY_MASK;
            break;
            
        case 1:
            LPC_EMC->STATICCONFIG1 &= ~EMC_STATIC_CFG_CHIPPOLARITY_MASK;
            LPC_EMC->STATICCONFIG1  |=  pol_val&EMC_STATIC_CFG_CHIPPOLARITY_MASK;
            break;
            
        case 2:
            LPC_EMC->STATICCONFIG2 &= ~EMC_STATIC_CFG_CHIPPOLARITY_MASK;
            LPC_EMC->STATICCONFIG2 |=  pol_val&EMC_STATIC_CFG_CHIPPOLARITY_MASK;
            break;
            
        case 3:
            LPC_EMC->STATICCONFIG3 &= ~EMC_STATIC_CFG_CHIPPOLARITY_MASK;
            LPC_EMC->STATICCONFIG3  |=  pol_val&EMC_STATIC_CFG_CHIPPOLARITY_MASK;
            break;
    default:
            return EMC_FUNC_INVALID_PARAM;
    }
    return EMC_FUNC_OK;
}


/*********************************************************************//**
 * @brief         Configure the byte lane state
 *
 * @param[in]    index index number, should be from 0 to 3
 *
 * @param[in]     pb_val  Byte lane state, should be:
 *
 *                 - EMC_CFG_BYTELAND_READ_BITSHIGH: For reads all bits 
 *                in BLSn[3:0] are HIGH.
 *
 *                 - EMC_CFG_BYTELAND_READ_BITSLOW: For reads all bits 
 *                in BLSn[3:0] are LOW.
 *
 * @return         EMC_FUNC_OK/EMC_FUNC_INVALID_PARAM
 **********************************************************************/
EMC_FUNC_CODE EMC_StaMemConfigPB(uint32_t index , uint32_t pb_val)
{
    switch ( index)
    {
        case 0:
            LPC_EMC->STATICCONFIG0 &= ~EMC_STATIC_CFG_BYTELAND_MASK;
            LPC_EMC->STATICCONFIG0 |= pb_val&EMC_STATIC_CFG_BYTELAND_MASK;
            break;

        case 1:
            LPC_EMC->STATICCONFIG1 &= ~EMC_STATIC_CFG_BYTELAND_MASK;
            LPC_EMC->STATICCONFIG1 |= pb_val&EMC_STATIC_CFG_BYTELAND_MASK;
            break;

        case 2:
            LPC_EMC->STATICCONFIG2 &= ~EMC_STATIC_CFG_BYTELAND_MASK;
            LPC_EMC->STATICCONFIG2 |= pb_val&EMC_STATIC_CFG_BYTELAND_MASK;
            break;

        case 3:
            LPC_EMC->STATICCONFIG3 &= ~EMC_STATIC_CFG_BYTELAND_MASK;
            LPC_EMC->STATICCONFIG3 |= pb_val&EMC_STATIC_CFG_BYTELAND_MASK;
            break;
    default:
            return EMC_FUNC_INVALID_PARAM;
    }
    return EMC_FUNC_OK;
}

/*********************************************************************//**
 * @brief         Configure the extended wait value
 *
 * @param[in]    index index number, should be from 0 to 3
 *
 * @param[in]     ex_wait  Extended wait mode, should be:
 *
 *                - EMC_CFG_EW_DISABLED: Extended wait disabled.
 *
 *                - EMC_CFG_EW_ENABLED: Extended wait enabled.
 *
 * @return         EMC_FUNC_OK/EMC_FUNC_INVALID_PARAM
 **********************************************************************/
EMC_FUNC_CODE EMC_StaMemConfigEW(uint32_t index , uint32_t ex_wait)
{
    switch ( index)
    {
        case 0:
            LPC_EMC->STATICCONFIG0 &= ~EMC_STATIC_CFG_EXTWAIT_MASK;
            LPC_EMC->STATICCONFIG0 |=  ex_wait&EMC_STATIC_CFG_EXTWAIT_MASK;
            break;
        
        case 1:
            LPC_EMC->STATICCONFIG1 &= ~EMC_STATIC_CFG_EXTWAIT_MASK;
            LPC_EMC->STATICCONFIG1 |=  ex_wait&EMC_STATIC_CFG_EXTWAIT_MASK;
            break;
        
        case 2:
            LPC_EMC->STATICCONFIG2 &= ~EMC_STATIC_CFG_EXTWAIT_MASK;
            LPC_EMC->STATICCONFIG2  |=  ex_wait&EMC_STATIC_CFG_EXTWAIT_MASK;
            break;
        
        case 3:
            LPC_EMC->STATICCONFIG3 &= ~EMC_STATIC_CFG_EXTWAIT_MASK;
            LPC_EMC->STATICCONFIG3  |=  ex_wait&EMC_STATIC_CFG_EXTWAIT_MASK;
            break;
    default:
            return EMC_FUNC_INVALID_PARAM;
    }
    return EMC_FUNC_OK;
}

/*********************************************************************//**
 * @brief         Configure the buffer enable value
 *
 * @param[in]    index index number, should be from 0 to 3
 *
 * @param[in]     buf_val  Buffer mode, should be:
 *
 *                - EMC_CFG_BUF_DISABLED: Buffer disabled.
 *
 *                - EMC_CFG_BUF_ENABLED: Buffer enabled.
 *
 * @return         EMC_FUNC_OK/EMC_FUNC_INVALID_PARAM
 **********************************************************************/
EMC_FUNC_CODE EMC_StaMemConfigB(uint32_t index , uint32_t buf_val)
{    
    switch ( index)
    {
        case 0:
            LPC_EMC->STATICCONFIG0 &= ~EMC_STATIC_CFG_BUFENABLE_MASK;
            LPC_EMC->STATICCONFIG0 |=  buf_val&EMC_STATIC_CFG_BUFENABLE_MASK;
            break;
        
        case 1:
            LPC_EMC->STATICCONFIG1 &= ~EMC_STATIC_CFG_BUFENABLE_MASK;
            LPC_EMC->STATICCONFIG1  |=  buf_val&EMC_STATIC_CFG_BUFENABLE_MASK;
            break;
        
        case 2:
            LPC_EMC->STATICCONFIG2 &= ~EMC_STATIC_CFG_BUFENABLE_MASK;
            LPC_EMC->STATICCONFIG2  |=  buf_val&EMC_STATIC_CFG_BUFENABLE_MASK;
            break;
        
        case 3:
            LPC_EMC->STATICCONFIG3 &= ~EMC_STATIC_CFG_BUFENABLE_MASK;
            LPC_EMC->STATICCONFIG3  |=  buf_val&EMC_STATIC_CFG_BUFENABLE_MASK;
            break;
    default:
            return EMC_FUNC_INVALID_PARAM;
    }
    return EMC_FUNC_OK;
}

/*********************************************************************//**
 * @brief         Configure the write permission
 *
 * @param[in]    index index number, should be from 0 to 3
 *
 * @param[in]     per_val  Permission mode, should be:
 *
 *                - EMC_CFG_WRITEPROTECT_DISABLED: Write not protected.
 *
 *                - EMC_CFG_WRITEPROTECT_ENABLED: Write protected.
 *
 * @return         EMC_FUNC_OK/EMC_FUNC_INVALID_PARAM
 **********************************************************************/
EMC_FUNC_CODE EMC_StaMemConfigpP(uint32_t index , uint32_t per_val)
{
    switch ( index)
    {
        case 0:
            LPC_EMC->STATICCONFIG0 &= ~EMC_STATIC_CFG_WRIEPROTECT_MASK;
            LPC_EMC->STATICCONFIG0  |=  per_val&EMC_STATIC_CFG_WRIEPROTECT_MASK;
            break;
        
        case 1:
            LPC_EMC->STATICCONFIG1 &= ~EMC_STATIC_CFG_WRIEPROTECT_MASK;
            LPC_EMC->STATICCONFIG1  |=  per_val&EMC_STATIC_CFG_WRIEPROTECT_MASK;
            break;
        
        case 2:
            LPC_EMC->STATICCONFIG2 &= ~EMC_STATIC_CFG_WRIEPROTECT_MASK;
            LPC_EMC->STATICCONFIG2  |=  per_val&EMC_STATIC_CFG_WRIEPROTECT_MASK;
            break;
        
        case 3:
            LPC_EMC->STATICCONFIG3 &= ~EMC_STATIC_CFG_WRIEPROTECT_MASK;
            LPC_EMC->STATICCONFIG3  |=  per_val&EMC_STATIC_CFG_WRIEPROTECT_MASK;
            break;
    default:
            return EMC_FUNC_INVALID_PARAM;
    }
    return EMC_FUNC_OK;
}

/*********************************************************************//**
 * @brief         Set the value of LPC_EMC static memory registers
 *
 * @param[in]    index index number, should be from 0 to 3
 *
 * @param[in]     EMC_STA_MEM_PAR  Static register, should be:
 *
 *                - EMC_STA_MEM_WAITWEN: StaticWaitWen0 register
 *                - EMC_STA_MEM_WAITOEN: StaticWaitOen0 register
 *                - EMC_STA_MEM_WAITRD: StaticWaitRd0 register
 *                - EMC_STA_MEM_WAITPAGE: StaticWaitPage0 register
 *                - EMC_STA_MEM_WAITWR: StaticWaitWr0 register
 *                - EMC_STA_MEM_WAITTURN: StaticWaitTurn0 register
 *
 * @return         EMC_FUNC_OK/EMC_FUNC_INVALID_PARAM
 **********************************************************************/
EMC_FUNC_CODE EMC_SetStaMemoryParameter(uint32_t index ,EMC_STA_MEM_PAR par, uint32_t val)
{
    switch (index)
    {
        case 0:
            switch ( par)
            {
                case EMC_STA_MEM_WAITWEN:
                    LPC_EMC->STATICWAITWEN0 = EMC_StaticWaitWen_WAITWEN(val);
                    break; 
                
                case EMC_STA_MEM_WAITOEN:
                    LPC_EMC->STATICWAITOEN0 = EMC_StaticWaitOen_WAITOEN(val);
                    break;
                
                case EMC_STA_MEM_WAITRD:
                    LPC_EMC->STATICWAITRD0 = EMC_StaticWaitRd_WAITRD(val);
                    break;
                
                case EMC_STA_MEM_WAITPAGE:
                    LPC_EMC->STATICWAITPAG0 = EMC_StaticwaitPage_WAITPAGE(val);
                    break;
                
                case EMC_STA_MEM_WAITWR:
                    LPC_EMC->STATICWAITWR0 = EMC_StaticWaitwr_WAITWR(val);
                    break;
                
                case EMC_STA_MEM_WAITTURN:
                    LPC_EMC->STATICWAITTURN0 =EMC_StaticWaitTurn_WAITTURN(val);
                    break;
          default:
                    return EMC_FUNC_INVALID_PARAM;
            }
            break;
            
        case 1:
            switch ( par)
            {
                case EMC_STA_MEM_WAITWEN:
                    LPC_EMC->STATICWAITWEN1 = EMC_StaticWaitWen_WAITWEN(val);
                    break;
                
                case EMC_STA_MEM_WAITOEN:
                    LPC_EMC->STATICWAITOEN1 = EMC_StaticWaitOen_WAITOEN(val);
                    break;
                
                case EMC_STA_MEM_WAITRD:
                    LPC_EMC->STATICWAITRD1 = EMC_StaticWaitRd_WAITRD(val);
                    break;
                
                case EMC_STA_MEM_WAITPAGE:
                    LPC_EMC->STATICWAITPAG1 = EMC_StaticwaitPage_WAITPAGE(val);
                    break;
                
                case EMC_STA_MEM_WAITWR:
                    LPC_EMC->STATICWAITWR1 = EMC_StaticWaitwr_WAITWR(val);
                    break;
                
                case EMC_STA_MEM_WAITTURN:
                    LPC_EMC->STATICWAITTURN1 =EMC_StaticWaitTurn_WAITTURN(val);
                    break;
           default:
         return EMC_FUNC_INVALID_PARAM;

            }
            break;
            
        case 2:
            switch ( par)
            {
                case EMC_STA_MEM_WAITWEN:
                    LPC_EMC->STATICWAITWEN2 = EMC_StaticWaitWen_WAITWEN(val);
                    break;
                
                case EMC_STA_MEM_WAITOEN:
                    LPC_EMC->STATICWAITOEN2 = EMC_StaticWaitOen_WAITOEN(val);
                    break;
                
                case EMC_STA_MEM_WAITRD:
                    LPC_EMC->STATICWAITRD2 = EMC_StaticWaitRd_WAITRD(val);
                    break;
                
                case EMC_STA_MEM_WAITPAGE:
                    LPC_EMC->STATICWAITPAG2 = EMC_StaticwaitPage_WAITPAGE(val);
                    break;
                
                case EMC_STA_MEM_WAITWR:
                    LPC_EMC->STATICWAITWR2 = EMC_StaticWaitwr_WAITWR(val);
                    break;
                
                case EMC_STA_MEM_WAITTURN:
                    LPC_EMC->STATICWAITTURN2 =EMC_StaticWaitTurn_WAITTURN(val);
                    break;
           default:
         return EMC_FUNC_INVALID_PARAM;

            }
            break;
        
        case 3:
            switch ( par)
            {
                case EMC_STA_MEM_WAITWEN:
                    LPC_EMC->STATICWAITWEN3 = EMC_StaticWaitWen_WAITWEN(val);
                    break;
                
                case EMC_STA_MEM_WAITOEN:
                    LPC_EMC->STATICWAITOEN3 = EMC_StaticWaitOen_WAITOEN(val);
                    break;
                
                case EMC_STA_MEM_WAITRD:
                    LPC_EMC->STATICWAITRD3 = EMC_StaticWaitRd_WAITRD(val);
                    break;
                
                case EMC_STA_MEM_WAITPAGE:
                    LPC_EMC->STATICWAITPAG3 = EMC_StaticwaitPage_WAITPAGE(val);
                    break;
                
                case EMC_STA_MEM_WAITWR:
                    LPC_EMC->STATICWAITWR3 = EMC_StaticWaitwr_WAITWR(val);
                    break;
                
                case EMC_STA_MEM_WAITTURN:
                    LPC_EMC->STATICWAITTURN3 =EMC_StaticWaitTurn_WAITTURN(val);
                    break;
           default:
        return EMC_FUNC_INVALID_PARAM;
            }
            break;
    }
    return EMC_FUNC_OK;
}

/*********************************************************************//**
 * @brief         Initialize external static memory
 * @param[in]    pConfig    Configuration
 * @return         EMC_FUNC_OK/EMC_FUNC_INVALID_PARAM/EMC_FUNC_ERR
 **********************************************************************/
EMC_FUNC_CODE StaticMem_Init(EMC_STATIC_MEM_Config_Type* pConfig)
{
	EMC_FUNC_CODE ret = EMC_FUNC_OK;
 
	//MemoryPinInit();

	// Power On
// 	ret |= EMC_PwrOn();
	LPC_EMC->CONTROL = 0x00000001;   /* Enable the external memory controller */	


	// Configuration
	if(pConfig->AddressMirror)
	{
		LPC_EMC->CONTROL |= EMC_Control_M;
	}

 ret |= EMC_StaMemConfigMW(pConfig->CSn,pConfig->DataWidth);
	
	if(pConfig->PageMode)
		 ret |= EMC_StaMemConfigPM(pConfig->CSn,EMC_CFG_PM_ASYNC_ENABLE);
	else
		 ret |= EMC_StaMemConfigPM(pConfig->CSn,EMC_CFG_PM_DISABLE);

	if(pConfig->ByteLane)
		 ret |= EMC_StaMemConfigPB(pConfig->CSn, EMC_CFG_BYTELAND_READ_BITSLOW);
	else
		 ret |= EMC_StaMemConfigPB(pConfig->CSn, EMC_CFG_BYTELAND_READ_BITSHIGH);

	if(pConfig->ExtendedWait)
		 ret |= EMC_StaMemConfigEW(pConfig->CSn,EMC_CFG_EW_ENABLED);
	else
		 ret |= EMC_StaMemConfigEW(pConfig->CSn,EMC_CFG_EW_DISABLED);

	// Timing
	ret |= EMC_SetStaMemoryParameter(pConfig->CSn,EMC_STA_MEM_WAITWEN, pConfig->WaitWEn);
	ret |= EMC_SetStaMemoryParameter(pConfig->CSn,EMC_STA_MEM_WAITOEN, pConfig->WaitOEn);
	ret |= EMC_SetStaMemoryParameter(pConfig->CSn,EMC_STA_MEM_WAITRD, pConfig->WaitRd);
	ret |= EMC_SetStaMemoryParameter(pConfig->CSn,EMC_STA_MEM_WAITPAGE, pConfig->WaitPage);
	ret |= EMC_SetStaMemoryParameter(pConfig->CSn,EMC_STA_MEM_WAITWR, pConfig->WaitWr);
	ret |= EMC_SetStaMemoryParameter(pConfig->CSn,EMC_STA_MEM_WAITTURN, pConfig->WaitTurn);


	return ret;
}








/**
 * @}
 */

