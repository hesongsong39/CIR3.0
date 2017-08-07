/**********************************************************************
* $Id$      SSP             2015-07-20
*//**
* @file     SSP.h
* @brief    SSP操作片外SPIFLASH头文件
* @version  1.0.0
* @date     2015/07/20 星期一 20:12:25	
* @author   ZhangYanMing
*
* Copyright(C) 2015, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
#ifndef __SPIFLASH__
#define __SPIFLASH__

#include "lpc_types.h"
#include "lpc43xx_ssp.h"
#include "DriverConfig.h"


typedef  void (*pFnfSsppininit)(void);
typedef  void (*pFnfSspReset)(void);

/* macro,define the chip select pin */
#if defined(_SSP0_CS_P1_0)
	#define  SPI_FLASH_CS_LOW()     GPIO_ClearValue(0,1<<4)
	#define  SPI_FLASH_CS_HIGH()    GPIO_SetValue(0,1<<4)
#elif defined(_SSP0_CS_PF_1)
	#define  SPI_FLASH_CS_LOW()     GPIO_ClearValue(7,1<<16)
	#define  SPI_FLASH_CS_HIGH()    GPIO_SetValue(7,1<<16)
#else
	#error #define _SSP0_CS_P1_0 or _SSP0_CS_PF_1 please
#endif


/** Possible error codes that can be returned from functions */
typedef enum {
	SPI_ERR_NONE = 0,							/**< No error */
	SPI_ERR_BUSY,									/**< Device is busy */
	SPI_ERR_GEN,									/**< General error */
	SPI_ERR_NOTSUPPORTED,					/**< Capability not supported */
	SPI_ERR_ALIGNERR,							/**< Attempted to do an operation on an unaligned section of the device */
	SPI_ERR_LOCKED,								/**< Device was locked and a program/erase operation was attempted */
	SPI_ERR_PROGERR,							/**< Error programming device (blocking mode only) */
	SPI_ERR_ERASEERR,							/**< Erase error (blocking mode only) */
	SPI_ERR_NOTBLANK,							/**< Program operation on block that is not blank */
	SPI_ERR_PAGESIZE,							/**< PageProgram write size exceeds page size */
	SPI_ERR_VAL,									/**< Program operation failed validation or readback compare */
	SPI_ERR_RANGE,								/**< Range error, bad block number, address out of range, etc. */
	SPI_ERR_MEMMODE,							/**< Library calls not allowed while in memory mode. */
	SPI_ERR_LASTINDEX
} SPI_ERR_T;


/** Common data applicable to all devices */
typedef struct 
{
	uint32_t        baseAddr;								/**< Physical base address for the device */
	uint32_t        numBlocks;							/**< Number of blocks on the device */
	uint32_t        blockSize;							/**< Size of blocks on the device */
	uint32_t        numSubBlocks;						/**< Number of sub-blocks on the device */
	uint32_t        subBlockSize;						/**< Size of sub-blocks on the device */
	uint32_t        pageSize;								/**< Size of a page, usually denotes maximum write size in bytes for a single write operation */
	uint32_t        maxReadSize;						/**< Maximum read size in bytes for a single read operation */
} SPI_INFODATA_T;

/* Command definitions. Only used commands are defined. */
#define  SPI_CMD_FAST_READ               0x0C		/**< Read Data bytes at Fast Speed */
#define  SPI_CMD_QOR                     0x6C		/**< Quad Output Read */
#define  SPI_CMD_RDID                    0x9F		/**< Read Identification */
#define  SPI_CMD_WREN                    0x06		/**< Write Enable */
#define  SPI_CMD_P4E                     0x21		/**< 4 KB Parameter Sector Erase */
#define  SPI_CMD_BULK_ERASE              0xC7		/**< Bulk Erase */
#define  SPI_CMD_SECT_ERASE              0xDC		/**< 64KB Sector Erase */
#define  SPI_CMD_PAGE_PROGRAM            0x12		/**< Page Programming */
#define  SPI_CMD_QUAD_PAGE_PROGRAM       0x3E		/**< Quad Page Programming */
#define  SPI_CMD_RDSR                    0x05		/**< Read Status Register */
#define  SPI_CMD_WRITE_STATUS_REG        0x01		/**< Write (Status & Configuration) Register */

/* memory organization */
#define  SPI_BASEADDRESS     0     
#define  SPI_SUBBLOCKNUM     16384    
#define  SPI_SUBBLOCKSIZE    0x1000
#define  SPI_PAGESIZE        0x100
#define  SPI_ADDRWSS_SIZE    4

/* Status register definitions */
#define SPI_FLASH_STATUS_SRWD                   (1 << 7)
#define SPI_FLASH_STATUS_QUAD_ENABLE            (1 << 6)
#define SPI_FLASH_STATUS_BPMASK                 (0xf << 2)
#define SPI_FLASH_STATUS_WEL                    (1 << 1)
#define SPI_FLASH_STATUS_WIP                    (1 << 0)

/* Possible device statuses returned from getInfo() */
#define SPI_STAT_BUSY     (1 << 0)			/**< Device is busy erasing or programming */
#define SPI_STAT_ISWP     (1 << 1)			/**< Device is write protected (software or hardware) */
#define SPI_STAT_FULLLOCK (1 << 2)			/**< Device is fully locked */
#define SPI_STAT_PARTLOCK (1 << 3)			/**< Device is partially locked (device specific) */
#define SPI_STAT_PROGERR  (1 << 4)			/**< Device status shows a program error (non-blocking mode only) */
#define SPI_STAT_ERASEERR (1 << 5)			/**< Device status shows a erase error (non-blocking mode only) */



Bool SPI_FLASH_Init(LPC_SSPn_Type* SSPx,pFnfSsppininit fpininit,pFnfSspReset fSspReset);
uint32_t SSP_FLASH_RdID(LPC_SSPn_Type* SSPx);
Bool SSP_EntireErase(LPC_SSPn_Type* SSPx,SPI_INFODATA_T *pHandle);
Bool SSP_Erase(LPC_SSPn_Type* SSPx,SPI_INFODATA_T *pHandle,uint32_t pFirstBlock, uint32_t pNumBlocks);
Bool SSP_EraseSectors(LPC_SSPn_Type* SSPx,SPI_INFODATA_T *pHandle,uint32_t pFirstSector, uint32_t pNumSectors);
Bool SSP_Write(LPC_SSPn_Type* SSPx,uint32_t dst, uint32_t *src, uint32_t len);
Bool SSP_Read(LPC_SSPn_Type* SSPx,uint32_t dst, uint32_t *src, uint32_t len);

#endif



