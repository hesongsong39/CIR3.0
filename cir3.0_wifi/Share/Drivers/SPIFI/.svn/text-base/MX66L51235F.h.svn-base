/**********************************************************************
* $Id$      MX66L51235F              2014-07-08
*//**
* @file     MX66L51235F.h
* @brief    MX66L51235F Driver header file
* @version  1.1.0
* @date     2014/7/8 ÐÇÆÚ¶þ 11:35:28
* @author   HaoZhilei
*
* Copyright(C) 2014, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
#ifndef __MX25L12835F__
#define __MX25L12835F__
/**************************** Includes ********************************/
#include "lpc_types.h"

#define SPIFLASH_BASE_ADDRESS     0x14000000

#define SPIFI_FLASH_PAGESIZE      (256)
#define SPIFI_FLASH_SECTORSIZE    (SPIFI_FLASH_PAGESIZE*16)
#define SPIFI_FLASH_BLOCKSIZE     (SPIFI_FLASH_SECTORSIZE*16)

#define SPIFI_FLASH_BLOCKNUM      (1024)
#define SPIFI_FLASH_SECTORNUM     (SPIFI_FLASH_BLOCKNUM*16)
#define SPIFI_FLASH_PAGENUM       (SPIFI_FLASH_SECTORNUM*16)

#define SPIFI_FLASH_SIZE          (SPIFI_FLASH_BLOCKNUM*SPIFI_FLASH_BLOCKSIZE)


/***********************************************************************
 * Macros define for SPIFI Flash IO configure
 **********************************************************************/
#define SPIFI_FLASH_RST(i)	i>0 ?  ( LPC_GPIO_PORT->SET[3] |= (1<<11) ):( LPC_GPIO_PORT->CLR[3] |= (1<<11) )
#define SPIFI_FLASH_CS(i)	i>0 ?  ( LPC_GPIO_PORT->SET[5] |= (1<<11) ):( LPC_GPIO_PORT->CLR[5] |= (1<<11) )



int SPIFI_Init(void);
int SPIFI_Write(uint32_t dst, uint32_t *src, uint16_t len);
int SPIFI_EntireErase(void);
int SPIFI_Erase(uint32_t pFirstBlock, uint32_t pNumBlocks);
int SPIFI_EraseSectors(uint32_t pFirstSector, uint32_t pNumSectors);
int SPIFI_WriteEx(uint32_t dst, uint32_t *src, uint32_t len);

#endif /* __LPC4357__MX25L12835F__ */




