/**********************************************************************
* $Id$      eeprom              2014-01-04
*//**
* @file     eeprom.h
* @brief    EEPROM的相关操作的头文件
* @version  1.0.0.0
* @date     2014/1/4 星期六 14:45:14
* @author   HaoZhilei
*
* Copyright(C) 2014, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
#ifndef _RECORDUNIT_EEPROM_H_
#define _RECORDUNIT_EEPROM_H_


#include "lpc_types.h"




/** EEPROM start address */
#define EEPROM_START                    (0x20040000)
/** EEPROM byes per page */
#define EEPROM_PAGE_SIZE                (128)
/**The number of EEPROM pages. The last page is not writable.*/
#define EEPROM_PAGE_NUM                 (128)
/** Get the eeprom address */
#define EEPROM_ADDRESS(page, offset)     (EEPROM_START + (EEPROM_PAGE_SIZE * (page)) + offset)
#define EEPROM_CLOCK_DIV                 1500000
#define EEPROM_READ_WAIT_STATE_VAL       0x58
#define EEPROM_WAIT_STATE_VAL            0x232


/**
 * @brief EEPROM register block structure
 */
typedef struct {				/* EEPROM Structure */
	 uint32_t CMD;			/*!< EEPROM command register */
	uint32_t RESERVED0;
	 uint32_t RWSTATE;		/*!< EEPROM read wait state register */
	 uint32_t AUTOPROG;		/*!< EEPROM auto programming register */
	 uint32_t WSTATE;		/*!< EEPROM wait state register */
	 uint32_t CLKDIV;		/*!< EEPROM clock divider register */
	 uint32_t PWRDWN;		/*!< EEPROM power-down register */
	uint32_t RESERVED2[1007];
	  uint32_t INTENCLR;		/*!< EEPROM interrupt enable clear */
	  uint32_t INTENSET;		/*!< EEPROM interrupt enable set */
	  uint32_t INTSTAT;		/*!< EEPROM interrupt status */
	  uint32_t INTEN;		/*!< EEPROM interrupt enable */
	  uint32_t INTSTATCLR;	/*!< EEPROM interrupt status clear */
	  uint32_t INTSTATSET;	/*!< EEPROM interrupt status set */
} LPC_EEPROM_T;


/**
 * @brief Macro defines for Peripheral EEPROM addresses
 */
#define LPC_EEPROM_BASE           	0x4000E000
#define LPC_EEPROM                	((LPC_EEPROM_T           *) LPC_EEPROM_BASE)


/**
 * @brief Macro defines for EEPROM command register
 */
#define EEPROM_CMD_ERASE_PRG_PAGE       (6)		/*!< EEPROM erase/program command */

/**
 * @brief Macro defines for EEPROM Auto Programming register
 */
#define EEPROM_AUTOPROG_OFF     						 (0)		/*!<Auto programming off */
#define EEPROM_AUTOPROG_AFT_1WORDWRITTEN     (1)		/*!< Erase/program cycle is triggered after 1 word is written */
#define EEPROM_AUTOPROG_AFT_LASTWORDWRITTEN  (2)		/*!< Erase/program cycle is triggered after a write to AHB
														   address ending with ......1111100 (last word of a page) */

/**
 * @brief Macro defines for EEPROM power down register
 */
#define EEPROM_PWRDWN                   (1 << 0)

/**
 * @brief Macro defines for EEPROM interrupt related registers
 */
#define EEPROM_INT_ENDOFPROG            (1 << 2)

/**
 * @brief	Put EEPROM device in power down mode
 * @param	pEEPROM	: Pointer to EEPROM peripheral block structure
 * @return	Nothing
 */
 
STATIC INLINE void EEPROM_EnablePowerDown(LPC_EEPROM_T *pEEPROM)
{
	pEEPROM->PWRDWN = EEPROM_PWRDWN;
}

/**
 * @brief	Bring EEPROM device out of power down mode
 * @param	pEEPROM	: Pointer to EEPROM peripheral block structure
 * @return	Nothing
 */
STATIC INLINE void EEPROM_DisablePowerDown(LPC_EEPROM_T *pEEPROM)
{
	pEEPROM->PWRDWN = 0;
}

/**
 * @brief	De-initializes EEPROM
 * @param	pEEPROM	: Pointer to EEPROM peripheral block structure
 * @return	Nothing
 */
STATIC INLINE void EEPROM_DeInit(LPC_EEPROM_T *pEEPROM)
{
	/* Enable EEPROM power down mode */
	EEPROM_EnablePowerDown(pEEPROM);
}

/**
 * @brief	Set Auto program mode
 * @param	pEEPROM	: Pointer to EEPROM peripheral block structure
 * @param	mode	: Auto Program Mode (One of EEPROM_AUTOPROG_* value)
 * @return	Nothing
 */
STATIC INLINE void EEPROM_SetAutoProg(LPC_EEPROM_T *pEEPROM, uint32_t mode)
{
	pEEPROM->AUTOPROG = mode;
}

/**
 * @brief	Set EEPROM Read Wait State
 * @param	pEEPROM	: Pointer to EEPROM peripheral block structure
 * @param	ws      : Wait State value
 * @return	Nothing
 * @note    Bits 7:0 represents wait state for Read Phase 2 and
 *          Bits 15:8 represents wait state for Read Phase1
 */
STATIC INLINE void EEPROM_SetReadWaitState(LPC_EEPROM_T *pEEPROM, uint32_t ws)
{
	pEEPROM->RWSTATE = ws;
}

/**
 * @brief	Set EEPROM wait state
 * @param	pEEPROM	: Pointer to EEPROM peripheral block structure
 * @param	ws	    : Wait State value
 * @return	Nothing
 * @note    Bits 7:0 represents wait state for Phase 3,
 *          Bits 15:8 represents wait state for Phase2, and
 *          Bits 23:16 represents wait state for Phase1
 */
STATIC INLINE void EEPROM_SetWaitState(LPC_EEPROM_T *pEEPROM, uint32_t ws)
{
	pEEPROM->WSTATE = ws;
}

/**
 * @brief	Select an EEPROM command
 * @param	pEEPROM	: Pointer to EEPROM peripheral block structure
 * @param	cmd	    : EEPROM command
 * @return	Nothing
 * @note	The cmd is OR-ed bits value of  EEPROM_CMD_*
 */
STATIC INLINE void EEPROM_SetCmd(LPC_EEPROM_T *pEEPROM, uint32_t cmd)
{
	pEEPROM->CMD = cmd;
}

/**
 * @brief	Enable EEPROM interrupt
 * @param	pEEPROM	: Pointer to EEPROM peripheral block structure
 * @param	mask	: Interrupt mask (or-ed bits value of EEPROM_INT_*)
 * @return	Nothing
 */
STATIC INLINE void EEPROM_EnableInt(LPC_EEPROM_T *pEEPROM, uint32_t mask)
{
	pEEPROM->INTENSET =  mask;
}

/**
 * @brief	Disable EEPROM interrupt
 * @param	pEEPROM	: Pointer to EEPROM peripheral block structure
 * @param	mask	: Interrupt mask (or-ed bits value of EEPROM_INT_*)
 * @return	Nothing
 */
STATIC INLINE void EEPROM_DisableInt(LPC_EEPROM_T *pEEPROM, uint32_t mask)
{
	pEEPROM->INTENCLR =  mask;
}

/**
 * @brief	Get the value of the EEPROM interrupt enable register
 * @param	pEEPROM	: Pointer to EEPROM peripheral block structure
 * @return	OR-ed bits value of EEPROM_INT_*
 */
STATIC INLINE uint32_t EEPROM_GetIntEnable(LPC_EEPROM_T *pEEPROM)
{
	return pEEPROM->INTEN;
}

/**
 * @brief	Get EEPROM interrupt status
 * @param	pEEPROM	: Pointer to EEPROM peripheral block structure
 * @return	OR-ed bits value of EEPROM_INT_*
 */
STATIC INLINE uint32_t EEPROM_GetIntStatus(LPC_EEPROM_T *pEEPROM)
{
	return pEEPROM->INTSTAT;
}

/**
 * @brief	Set EEPROM interrupt status
 * @param	pEEPROM	: Pointer to EEPROM peripheral block structure
 * @param	mask	: Interrupt mask (or-ed bits value of EEPROM_INT_*)
 * @return	Nothing
 */
STATIC INLINE void EEPROM_SetIntStatus(LPC_EEPROM_T *pEEPROM, uint32_t mask)
{
	pEEPROM->INTSTATSET =  mask;
}

/**
 * @brief	Clear EEPROM interrupt status
 * @param	pEEPROM	: Pointer to EEPROM peripheral block structure
 * @param	mask	: Interrupt mask (or-ed bits value of EEPROM_INT_*)
 * @return	Nothing
 */
STATIC INLINE void EEPROM_ClearIntStatus(LPC_EEPROM_T *pEEPROM, uint32_t mask)
{
	pEEPROM->INTSTATCLR =  mask;
}


/*****************************************************************************
 * Public functions
 ****************************************************************************/
void EEPROM_Init(LPC_EEPROM_T *pEEPROM);
void EEPROM_Read(uint32_t pageOffset, uint32_t pageAddr, uint32_t* ptr, uint32_t size);
void EEPROM_Erase(uint32_t pageAddr);
void EEPROM_Write(uint32_t pageOffset, uint32_t pageAddr, uint32_t* ptr, uint32_t size);
void EEPROM_Write_Ex(uint32_t pageOffset, uint32_t pageAddr, uint32_t* ptr, uint32_t size);



#endif /* _RECORDUNIT_EEPROM_H_ */
