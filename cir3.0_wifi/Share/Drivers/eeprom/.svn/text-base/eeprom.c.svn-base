/**********************************************************************
* $Id$      eeprom              2014-01-04
*//**
* @file     eeprom.c
* @brief    EEPROM的相关操作
* @version  1.0.0.0
* @date     2014/1/4 星期六 14:45:14
* @author   HaoZhilei
*
* Copyright(C) 2014, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
* 说明：1. 该驱动来源于LPCOpen2.04
*       2. KEIL的LPC43xx.h中无EEPROM寄存器的申明，在本文件的头文件中有定义
*       3. 该驱动支持自动编程擦除模式，但并无中断服务程序，而为查询状态位
***********************************************************************/
#include <LPC43xx.H>
#include <string.h>
#include <ucos_ii.h>
#include "lpc43xx_cgu.h"
#include "eeprom.h"



/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
#define AUTOPROG_ON     0					/* Auto programming on/off */



/*********************************************************************//**
 * @brief      Set EEPROM clock divider
 * @param[in]  pEEPROM Pointer to EEPROM peripheral block structure
 * @return     None
 **********************************************************************/
void EEPROM_SetClkDiv(LPC_EEPROM_T *pEEPROM)
{
	uint32_t clk;

	/* Setup EEPROM timing to 375KHz based on PCLK rate */
	clk = CGU_GetPCLKFrequency(CGU_PERIPHERAL_EEPROM);

	/* Set EEPROM clock divide value*/
	pEEPROM->CLKDIV = clk / EEPROM_CLOCK_DIV - 1;
}

/*********************************************************************//**
 * @brief      Set EEPROM wait states
 * @param[in]  pEEPROM Pointer to EEPROM peripheral block structure
 * @return     None
 **********************************************************************/
void EEPROM_SetState(LPC_EEPROM_T *pEEPROM)
{
	EEPROM_SetReadWaitState(pEEPROM, EEPROM_READ_WAIT_STATE_VAL);
	EEPROM_SetWaitState(pEEPROM, EEPROM_WAIT_STATE_VAL);
}

/*********************************************************************//**
 * @brief      Wait for interrupt occurs
 * @param[in]  pEEPROM Pointer to EEPROM peripheral block structure
 * @param[in]  mask 		Expected interrupt
 * @return     None
 **********************************************************************/
void EEPROM_WaitForIntStatus(LPC_EEPROM_T *pEEPROM, uint32_t mask)
{
	uint32_t status;
	while (1)
	{
		status = EEPROM_GetIntStatus(pEEPROM);
		if ((status & mask) == mask)
		{
			break;
		}
	}
	EEPROM_ClearIntStatus(pEEPROM, mask);
}

/*********************************************************************//**
 * @brief      Erase/Program an EEPROM page
 *             Write data from page register to non-volatile memory
 * @param[in]  pEEPROM Pointer to EEPROM peripheral block structure
 * @return     None
 **********************************************************************/
void EEPROM_EraseProgramPage(LPC_EEPROM_T *pEEPROM)
{
	EEPROM_ClearIntStatus(pEEPROM, EEPROM_CMD_ERASE_PRG_PAGE);
	EEPROM_SetCmd(pEEPROM, EEPROM_CMD_ERASE_PRG_PAGE);
	EEPROM_WaitForIntStatus(pEEPROM, EEPROM_INT_ENDOFPROG);
}

/*********************************************************************//**
 * @brief      Initializes the EEPROM peripheral
 * @param[in]  pEEPROM Pointer to EEPROM peripheral block structure
 * @return     None
 **********************************************************************/
void EEPROM_Init(LPC_EEPROM_T *pEEPROM)
{
	/* Disable EEPROM power down mode */
	EEPROM_DisablePowerDown(pEEPROM);
	EEPROM_SetClkDiv(pEEPROM);
// 	EEPROM_SetState(pEEPROM);

#if AUTOPROG_ON
	/* Set Auto Programming mode */
	EEPROM_SetAutoProg(LPC_EEPROM, EEPROM_AUTOPROG_AFT_1WORDWRITTEN);
#else
	/* Set Auto Programming mode */
	EEPROM_SetAutoProg(LPC_EEPROM, EEPROM_AUTOPROG_OFF);
#endif /*AUTOPROG_ON*/
}

/*********************************************************************//**
 * @brief      Read data from EEPROM
 *             size must be multiple of 4 bytes
 *
 * @param[in]  pageOffset   page offset
 * @param[in]  pageAddr     page address
 * @param[in]  ptr          Pointer to the data to be read
 * @param[in]  size         length of the data to be read
 * @return     None
 **********************************************************************/
void EEPROM_Read(uint32_t pageOffset, uint32_t pageAddr, uint32_t* ptr, uint32_t size)
{
	uint8_t *pEepromMem = (uint8_t*)EEPROM_ADDRESS(pageAddr, pageOffset);
	memcpy((uint8_t*)ptr, pEepromMem, size);
}

/*********************************************************************//**
 * @brief      Erase a page in EEPROM
 * @param[in]  pageAddr  page address to be erased
 * @return     None
 **********************************************************************/
void EEPROM_Erase(uint32_t pageAddr)
{
	uint32_t i = 0;
	uint32_t *pEepromMem = (uint32_t*)EEPROM_ADDRESS(pageAddr, 0);
	for(i = 0; i < EEPROM_PAGE_SIZE / 4; i++)
	{
		pEepromMem[i] = 0;
#if AUTOPROG_ON
		EEPROM_WaitForIntStatus(LPC_EEPROM, EEPROM_INT_ENDOFPROG);
#endif
	}
#if (AUTOPROG_ON == 0)
	EEPROM_EraseProgramPage(LPC_EEPROM);
#endif
}

/*********************************************************************//**
 * @brief      Write data to a page in EEPROM
 *             size must be multiple of 4 bytes
 *
 * @param[in]  pageOffset  page offset
 * @param[in]  pageAddr    page address
 * @param[in]  ptr         Pointer to the data to be written
 * @param[in]  size        length of the data to be written
 * @return     None
 **********************************************************************/
void EEPROM_Write(uint32_t pageOffset, uint32_t pageAddr, uint32_t* ptr, uint32_t size)
{
	uint32_t i = 0;
	uint32_t *pEepromMem = (uint32_t*)EEPROM_ADDRESS(pageAddr, pageOffset);

	OSSchedLock();
	
	if(size > EEPROM_PAGE_SIZE - pageOffset)
		size = EEPROM_PAGE_SIZE - pageOffset;

	for(i = 0; i < size / 4; i++)
	{
		pEepromMem[i] = ptr[i];
#if AUTOPROG_ON
		EEPROM_WaitForIntStatus(LPC_EEPROM, EEPROM_INT_ENDOFPROG);
#endif
	}

#if (AUTOPROG_ON == 0)
	EEPROM_EraseProgramPage(LPC_EEPROM);
#endif
	
	OSSchedUnlock();
}

/*********************************************************************//**
 * @brief      [Extend]Write data EEPROM
 *             size must be multiple of 4 bytes
 *
 * @param[in]  pageOffset  page offset
 * @param[in]  pageAddr    page address
 * @param[in]  ptr         Pointer to the data to be written
 * @param[in]  size        length of the data to be written
 * @return     None
 **********************************************************************/
void EEPROM_Write_Ex(uint32_t pageOffset, uint32_t pageAddr, uint32_t* ptr, uint32_t size)
{
	uint8_t *addr = 0;
	uint32_t len = 0;
	uint32_t i = 0;
	
	// 若是有偏移，则写第一页
	if(pageOffset != 0)
	{
		if((size + pageOffset) <= EEPROM_PAGE_SIZE)
		{
				EEPROM_Write(pageOffset, pageAddr, ptr, size);
				return ;
		}
		EEPROM_Write(pageOffset, pageAddr, ptr, EEPROM_PAGE_SIZE - pageOffset);
		len = EEPROM_PAGE_SIZE - pageOffset;
		pageAddr++;  /* 移至下一页 */
	}
	
	
	// 整页连续写入
	for(i=0; i<(size-len)/EEPROM_PAGE_SIZE; i++)
	{
		addr = (uint8_t *)ptr+len+EEPROM_PAGE_SIZE*i;  /* 字节偏移 */
		EEPROM_Write(0, pageAddr+i, (uint32_t *)addr, EEPROM_PAGE_SIZE);
	}
	
	// 若非整页，则写最后一页
	if((size-len) % EEPROM_PAGE_SIZE) 
	{
		addr = (uint8_t *)ptr+len+EEPROM_PAGE_SIZE*i;  /* 字节偏移 */
		EEPROM_Write(0, pageAddr+i, (uint32_t *)addr, (size-len) % EEPROM_PAGE_SIZE);
	}
}

/**************************** End of file *****************************/
