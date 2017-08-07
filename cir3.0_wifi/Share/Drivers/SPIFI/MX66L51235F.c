/**********************************************************************
* $Id$      MX66L51235F             2014-07-08
*//**
* @file     MX66L51235F.c
* @brief    MX25L12835F Driver file
* @version  1.1.0
* @date     2014/7/8 星期二 11:35:28
* @author   HaoZhilei
*
* Copyright(C) 2014, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
/*
 *-NOTICE: 	该驱动文件中并没有相应的读操作函数，因为在使用SPIFI接口时，
 *					LPC4357已经完成了地址映射
 */

/* Includes --------------------------------------------------------- */
#include <LPC43xx.H>
#include <stdio.h>
#include "spifilib_api.h"
#include "MX66L51235F.h"



/* PRIVATE VARIABLES **************************************************/
static SPIFI_HANDLE_T *pSpifi;
/* Local memory, 32-bit aligned that will be used for driver context (handle) */
static uint32_t lmem[21];

 
static SPIFI_HANDLE_T *initializeSpifi(void)
{
    uint32_t memSize;
    SPIFI_HANDLE_T *pReturnVal = NULL;

    /* Initialize LPCSPIFILIB library, reset the interface */
    spifiInit(LPC_SPIFI_BASE, true);

    /* register support for the family(s) we may want to work with
         (only 1 is required) */
    spifiRegisterFamily(SPIFI_REG_FAMILY_Macronix_2Byte_Status);

    /* Get required memory for detected device, this may vary per device family */
    memSize = spifiGetHandleMemSize(LPC_SPIFI_BASE);
    if (memSize == 0)
    {
        return pReturnVal;
    }

    /* Initialize and detect a device and get device context */
    /* NOTE: Since we don't have malloc enabled we are just supplying
         a chunk of memory that we know is large enough. It would be
         better to use malloc if it is available. */
    pReturnVal = spifiInitDevice(&lmem, sizeof(lmem), LPC_SPIFI_BASE, SPIFLASH_BASE_ADDRESS);
   
    return pReturnVal;
}
 
/*********************************************************************//**
 * @brief 		SPIFI Initialization
 * @datasheet 
			Pin-SI/SIO0
			- Serial Data Input or Serial Data Input/Output for 2 x I/O read mode and 4 x I/O read mode
			Pin-SO/SIO1
			- Serial Data Output or Serial Data Input/Output for 2 x I/O read mode and 4 x I/O read mode
			Pin-WP#/SIO2
			- Hardware write protection or serial data Input/Output for 4 x I/O read mode
			Pin-RESET#/SIO3
			- Hardware Reset pin or Serial input & Output for 4 x I/O read mode
 * @param[in]	None
 * @return 		0
 **********************************************************************/
int SPIFI_Init(void)
{
    SPIFI_ERR_T errCode;
    
	/* Setup SPIFI FLASH pin muxing (QUAD) */
    LPC_SCU->SFSP3_3 = 0xF3; /* high drive for SCLK */
    /* IO pins */
    LPC_SCU->SFSP3_4 = 0xD3;
    LPC_SCU->SFSP3_5 = 0xD3;
    LPC_SCU->SFSP3_6 = 0xD3;
    LPC_SCU->SFSP3_7 = 0xD3;
    LPC_SCU->SFSP3_8 = 0x13; /* CS doesn't need feedback */
	
	/* 注意MX66L51235F支持90M、60M，不支持45M */
    /* set SPIFI clock 180/3 = 60MHz */
    LPC_CGU->IDIVB_CTRL = (0x09<<24) | (1<<11) | (2<<2);
    LPC_CGU->BASE_SPIFI_CLK = (0x0D<<24) | (1<<11); /* Clock source: IDIVB*/
	    
    /* Initialize the spifi library. This registers the device family and detects the part */
    pSpifi = initializeSpifi();
		if(pSpifi == NULL)
		{
				return -1;
		}
    errCode = spifiDevUnlockDevice(pSpifi);
    if (errCode != SPIFI_ERR_NONE)
    {
        return -1;
    }
    /* Enable quad.  If not supported it will be ignored */
    spifiDevSetOpts(pSpifi, SPIFI_OPT_USE_QUAD, true);
    /* Enter memMode */
    spifiDevSetMemMode(pSpifi, true);

		return 0;
}

/*********************************************************************//**
 * @brief 		SPIFI write
							It is not needed for re-programing because spifi_program
							automatically erases as necessary
 * @datasheet
			Programming :
			- 256byte page buffer
			- Quad Input/Output page program(4PP) to enhance program performance
 * @param[in]	dst  point to destination address
 * @param[in]	src  point to source address
 * @param[in]	len  length of programming
 * @return 		Error codes
 **********************************************************************/
int SPIFI_Write(uint32_t dst, uint32_t *src, uint16_t len)
{
	SPIFI_ERR_T errCode = SPIFI_ERR_NONE;
	uint32_t pageAddress = 0,pageBytes = 0,deviceBytes = 0,sector = 0;
	
	// 参数检查
	if((src == NULL)||(len == 0))
	{
		return -1;
	}
	
	// 整页写入 
	pageBytes = spifiDevGetInfo(pSpifi, SPIFI_INFO_PAGESIZE);
	if(len != pageBytes)
	{
		return -1;
	}
	
	if((dst - pSpifi->pInfoData->baseAddr)%pageBytes != 0)
	{
		return -1;	
	}
	
	deviceBytes = spifiDevGetInfo(pSpifi, SPIFI_INFO_DEVSIZE);
	pageAddress = spifiGetAddrFromBlock(pSpifi, 0);	
	if((dst < pageAddress) || ((dst + len) > (pageAddress+deviceBytes)))
	{
		return -1;
	}
	
	// 整块扇区开始则擦除 
	if(!(dst % SPIFI_FLASH_SECTORSIZE))
	{
		sector = spifiGetSubBlockFromAddr(pSpifi, dst);
		if(SPIFI_EraseSectors(sector, 1) == -1)
		{
				return -1;
		}
	}
		
	spifiDevSetMemMode(pSpifi, false);
	errCode = spifiDevPageProgram(pSpifi, dst, src, len);
	/* Enter memMode */
  spifiDevSetMemMode(pSpifi, true);
	
	if (errCode != SPIFI_ERR_NONE)
	{
	    return -1;
	}
    
	return 0;
}
	
/*********************************************************************//**
 * @brief 		SPIFI Entire erase
 * @param[in]	None
 * @return 		Error codes
 **********************************************************************/
int SPIFI_EntireErase(void)
{
		SPIFI_ERR_T errCode = SPIFI_ERR_NONE;
	
		spifiDevSetMemMode(pSpifi, false);
    /* Next erase everything */
    errCode = spifiErase(pSpifi, 0, spifiDevGetInfo(pSpifi, SPIFI_INFO_ERASE_BLOCKS));
    spifiDevSetMemMode(pSpifi, true);
	
		if (errCode != SPIFI_ERR_NONE)
    {
       return -1;
    }
    
    return 0;
}

/*********************************************************************//**
 * @brief 		SPIFI Erase Blocks
 * @param[in]	None
 * @return 		Error codes
 **********************************************************************/
int SPIFI_Erase(uint32_t pFirstBlock, uint32_t pNumBlocks)
{
		SPIFI_ERR_T errCode = SPIFI_ERR_NONE;
	
		spifiDevSetMemMode(pSpifi, false);
    /* Next erase everything */
    errCode = spifiErase(pSpifi, pFirstBlock, pNumBlocks);
    spifiDevSetMemMode(pSpifi, true);
	
		if (errCode != SPIFI_ERR_NONE)
    {
       return -1;
    }
    
    return 0;
}


/*********************************************************************//**
 * @brief 		SPIFI Erase Sectors
 * @param[in]	None
 * @return 		Error codes
 **********************************************************************/
int SPIFI_EraseSectors(uint32_t pFirstSector, uint32_t pNumSectors)
{
		SPIFI_ERR_T errCode = SPIFI_ERR_NONE;
	
		spifiDevSetMemMode(pSpifi, false);
    /* Next erase Sector */
    errCode = spifiEraseSector(pSpifi, pFirstSector, pNumSectors);
    spifiDevSetMemMode(pSpifi, true);
		if (errCode != SPIFI_ERR_NONE)
    {
        return -1;
    }
    
    return 0;
}


/*********************************************************************//**
 * @brief 		SPIFI write
 * @param[in]	dst  point to destination address
 * @param[in]	src  point to source address
 * @param[in]	len  length of programming
 * @return 		Error codes
 **********************************************************************/
int SPIFI_WriteEx(uint32_t dst, uint32_t *src, uint32_t len)
{
		SPIFI_ERR_T errCode = SPIFI_ERR_NONE;
		uint32_t sendBytes = 0,baseAddr = 0,pageBytes = 0,deviceBytes = 0,sector = 0;
	
		// 参数检查
		if((src == NULL)||(len == 0))
		{
			return -1;
		}
		
		deviceBytes = spifiDevGetInfo(pSpifi, SPIFI_INFO_DEVSIZE);
		baseAddr = spifiGetAddrFromBlock(pSpifi, 0);	
		if((dst < baseAddr) || ((dst + len) > (baseAddr + deviceBytes)))
		{
			return -1;
		}
	
		pageBytes = pSpifi->pInfoData->pageSize;
		sendBytes = pageBytes - (dst - pSpifi->pInfoData->baseAddr)%pageBytes;
		if(sendBytes >= len)
		{
				// 整块扇区开始则擦除 
				if(!(dst % SPIFI_FLASH_SECTORSIZE))
				{
					sector = spifiGetSubBlockFromAddr(pSpifi, dst);
					if(SPIFI_EraseSectors(sector, 1) == -1)
					{
							return -1;
					}
				}
				spifiDevSetMemMode(pSpifi, false);
				//一页内剩余空间大于写入字节个数
				errCode = spifiProgram(pSpifi, dst, (const uint32_t *)src, len);
				spifiDevSetMemMode(pSpifi, true);
		}
		else
		{
				while ((len > 0) && (errCode == SPIFI_ERR_NONE))
				{
						// 整块扇区开始则擦除 
						if(!(dst % SPIFI_FLASH_SECTORSIZE))
						{
							sector = spifiGetSubBlockFromAddr(pSpifi, dst);
							if(SPIFI_EraseSectors(sector, 1) == -1)
							{
									return -1;
							}
						}
						
						spifiDevSetMemMode(pSpifi, false);
						errCode = spifiProgram(pSpifi, dst, (const uint32_t *)src, sendBytes);
						dst += sendBytes;
						src += (sendBytes / sizeof(uint32_t));
						len -= sendBytes;
						
						sendBytes = len;
						if(sendBytes > pageBytes)
						{
								sendBytes = pageBytes;
						}
						spifiDevSetMemMode(pSpifi, true);
				}
		}
	
		if (errCode != SPIFI_ERR_NONE)
    {
        return -1;
    }
    
		return 0;
}


/**************************** End of file *****************************/
	
	
	
	
	
