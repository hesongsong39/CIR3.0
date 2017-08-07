/**********************************************************************
* $Id$      SSP             2015-07-20
*//**
* @file     SSP.c
* @brief    SSP操作片外SPIFLASH
* @version  1.0.0
* @date     2015/07/20 星期一 20:12:25	
* @author   ZhangYanMing
*
* Copyright(C) 2015, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
#include "lpc43xx.h"
#include "lpc43xx_gpio.h"
#include "lpc43xx_ssp.h"
#include "lpc43xx_scu.h"
#include "SPIFLASH.h"

static SPI_INFODATA_T g_spiinfo;

static void SPI_Para_Init(SPI_INFODATA_T *pHandle)
{
		pHandle->baseAddr = 0;
		pHandle->numBlocks = 1024;
		pHandle->blockSize = 0x10000;
		pHandle->numSubBlocks = 16384;
		pHandle->subBlockSize = 0x1000;
		pHandle->pageSize = 0x100;
		pHandle->maxReadSize = 32768;
}

/*******************************************************************************
* Function Name  : SPI_FLASH_Init
* Description    : SSP-SPI总线初始化
* Input          : none
* Output         : none
* Return         : None
*******************************************************************************/
Bool SPI_FLASH_Init(LPC_SSPn_Type* SSPx,pFnfSsppininit fpininit,pFnfSspReset fSspReset)
{
	SSP_CFG_Type SSP_ConfigStruct;
	
	if((fpininit == NULL)||(fSspReset == NULL))
	{
			return FALSE;
	}
	
	//初始化串口引脚，与硬件引脚设计有关，根据硬件原理图配置
	fpininit();
	//对SPIFLASH进行复位操作
	fSspReset();
	
	SPI_FLASH_CS_HIGH();
	/* Initializing Slave SSP device section ------------------------------------------- */
	// initialize SSP configuration structure to default
	SSP_ConfigStructInit(&SSP_ConfigStruct);
	SSP_ConfigStruct.ClockRate = 24000000;
	// Initialize SSP peripheral with parameter given in structure above
	SSP_Init(SSPx, &SSP_ConfigStruct);
	// Enable SSP peripheral
	SSP_Cmd(SSPx, ENABLE);
	
	//SPIFLASH 参数初始化
	SPI_Para_Init(&g_spiinfo);
	
	return TRUE;
}




/*******************************************************************************
* Function Name  : LPC43xx_SPI_SendRecvByte
* Description    : Send one byte then recv one byte of response
* @param[in]	SSPx	SSP peripheral selected, should be:
* 				 	- LPC_SSP0	:SSP0 peripheral
* 					- LPC_SSP1	:SSP1 peripheral
* @param[in]	byte_s	Data
* Output         : None
* Return         : None
*******************************************************************************/
static uint8_t LPC43xx_SPI_SendRecvByte (LPC_SSPn_Type* SSPx,uint8_t byte_s)
{
	/* wait for current SSP activity complete */
	while (SSP_GetStatus(SSPx, SSP_STAT_BUSY) ==  SET);
	
	SSP_SendData(SSPx, (uint16_t) byte_s);
	
	while (SSP_GetStatus(SSPx, SSP_STAT_RXFIFO_NOTEMPTY) == RESET);
	return (SSP_ReceiveData(SSPx));
}


/*****************************************************************************
* Function Name  : Flash_ReadWriteByte
* Description    : 通过硬件SPI发送一个字节到SPIFLASH
* @param[in]	SSPx	SSP peripheral selected, should be:
* 				 	- LPC_SSP0	:SSP0 peripheral
* 					- LPC_SSP1	:SSP1 peripheral
* @param[in]	byte_s	Data,发送的数据
* Return      : SPIFLASH 返回的数据
*******************************************************************************/
static uint8_t Flash_ReadWriteByte(LPC_SSPn_Type* SSPx,uint8_t data)		
{
		return LPC43xx_SPI_SendRecvByte(SSPx,data );                             
}


/*******************************************************************************
* Function Name  : SPI_prvSetWREN
* Description    : Software write Enable
* @param[in]	SSPx	SSP peripheral selected, should be:
* 				 	- LPC_SSP0	:SSP0 peripheral
* 					- LPC_SSP1	:SSP1 peripheral
* Return         : none
*******************************************************************************/
static void SPI_prvSetWREN(LPC_SSPn_Type* SSPx)
{
		SPI_FLASH_CS_LOW();				
		Flash_ReadWriteByte(SSPx,SPI_CMD_WREN);						    /* 发送写使能命令 */
		SPI_FLASH_CS_HIGH();			
}


/*******************************************************************************
* Function Name  : SPI_prvGetStatusRegister
* Description    : 获取SPIFLASH状态
* @param[in]	SSPx	SSP peripheral selected, should be:
* 				 	- LPC_SSP0	:SSP0 peripheral
* 					- LPC_SSP1	:SSP1 peripheral
* Return         : SPIFLASH状态寄存器值
*******************************************************************************/
static uint8_t SPI_prvGetStatusRegister(LPC_SSPn_Type* SSPx)
{
	uint8_t statRegister;
	
	SPI_FLASH_CS_LOW();				
	Flash_ReadWriteByte(SSPx,SPI_CMD_RDSR);						    /* 发送读状态命令 */		
	statRegister = Flash_ReadWriteByte(SSPx,0x00);
	SPI_FLASH_CS_HIGH();

	return statRegister;
}


/*******************************************************************************
* Function Name  : SPI_prvWaitUnBusy
* Description    : Wait for device to complete operation (go unbusy)
* @param[in]	SSPx	SSP peripheral selected, should be:
* 				 	- LPC_SSP0	:SSP0 peripheral
* 					- LPC_SSP1	:SSP1 peripheral
* Return         : none
*******************************************************************************/
static void SPI_prvWaitUnBusy(LPC_SSPn_Type* SSPx)
{
	/* Device wait for device to go unbusy  I.e wip and wel are both 0 */
	while ((SPI_prvGetStatusRegister(SSPx) & (SPI_FLASH_STATUS_WIP | SPI_FLASH_STATUS_WEL)) != 0) {}
}


/*******************************************************************************
* Function Name  : SPI_prvCheckWriteState
* Description    : Checks to see if the device is writable and not busy
* @param[in]	SSPx	SSP peripheral selected, should be:
* 				 	- LPC_SSP0	:SSP0 peripheral
* 					- LPC_SSP1	:SSP1 peripheral
* Return         : none
*******************************************************************************/
static SPI_ERR_T SPI_prvCheckWriteState(LPC_SSPn_Type* SSPx)
{
	uint16_t stat;
	/* Get status */
	stat = SPI_prvGetStatusRegister(SSPx);

	/* Exit if blocks are locked or WIP in progress */
	if ((stat & SPI_FLASH_STATUS_BPMASK) != 0) 
	{
			return SPI_ERR_LOCKED;
	}
	else if ((stat & SPI_FLASH_STATUS_WIP) != 0) 
	{
			return SPI_ERR_BUSY;
	}

	return SPI_ERR_NONE;
}


/*******************************************************************************
* Function Name  : SPI_deviceGetStatus
* Description    : Converts a device status to an OR'ed API status
* @param[in]	SSPx	SSP peripheral selected, should be:
* 				 	- LPC_SSP0	:SSP0 peripheral
* 					- LPC_SSP1	:SSP1 peripheral
* Return         : device status
*******************************************************************************/
static uint32_t SPI_deviceGetStatus(LPC_SSPn_Type* SSPx)
{
	uint16_t devStat;
	uint32_t status = 0;

	/* Read device status word */
	devStat = SPI_prvGetStatusRegister(SSPx);

	if ((devStat & SPI_FLASH_STATUS_BPMASK) != 0) 
	{
			if ((devStat & SPI_FLASH_STATUS_BPMASK) == SPI_FLASH_STATUS_BPMASK) 
			{
				status |= SPI_STAT_FULLLOCK;
			}
			else 
			{
				status |= SPI_STAT_PARTLOCK;
			}
	}
	if ((devStat & SPI_FLASH_STATUS_WIP) != 0) 
	{
			status |= SPI_STAT_BUSY;
	}

	return status;
}


/*******************************************************************************
* Function Name  : SPI_FLASH_RdID
* Description    : SPIFLASH的读ID函数
* @param[in]	SSPx	SSP peripheral selected, should be:
* 				 	- LPC_SSP0	:SSP0 peripheral
* 					- LPC_SSP1	:SSP1 peripheral
* Return         : none
*******************************************************************************/
uint32_t SSP_FLASH_RdID(LPC_SSPn_Type* SSPx)
{
		uint32_t temp = 0;
		SPI_FLASH_CS_LOW();	
		while (SSP_GetStatus(SSPx, SSP_STAT_BUSY) ==  SET);
				
		Flash_ReadWriteByte(SSPx,SPI_CMD_RDID);		 		         /* 发送读ID命令(9Fh)	*/  
    temp = (temp | Flash_ReadWriteByte(SSPx,0x00)) << 8;  /* 接收数据 */
		temp = (temp | Flash_ReadWriteByte(SSPx,0x00)) << 8;	
    temp = (temp | Flash_ReadWriteByte(SSPx,0x00)); 	     

    SPI_FLASH_CS_HIGH();

		return  temp;
}

/*******************************************************************************
* Function Name  : SPI_deviceeEraseSubBlock
* Description    : Erase a block by sub-block number
* @param[in]	SSPx	SSP peripheral selected, should be:
* 				 	- LPC_SSP0	:SSP0 peripheral
* 					- LPC_SSP1	:SSP1 peripheral
* @param[in]	pHandle	device handle
* @param[in]	subBlockNum块号
* Return         : none
*******************************************************************************/
static SPI_ERR_T SPI_deviceeEraseSubBlock(LPC_SSPn_Type* SSPx,SPI_INFODATA_T *pHandle,uint32_t subBlockNum)
{
		uint32_t addr;
		uint16_t stat;
	
		SPI_ERR_T status = SPI_ERR_RANGE;
		if (subBlockNum < pHandle->numSubBlocks)
		{
				status = SPI_prvCheckWriteState(SSPx);
				if (status == SPI_ERR_NONE)
				{
						addr = subBlockNum * pHandle->subBlockSize;
						SPI_prvSetWREN(SSPx);
						SPI_FLASH_CS_LOW();			
						/* 发送4K sector擦除命令 */					
						Flash_ReadWriteByte(SSPx,SPI_CMD_P4E); 		
						/* 发送4个字节的地址信息 */		
						Flash_ReadWriteByte(SSPx,(((addr) & 0xFFFFFFFF) >> 24));  					
						Flash_ReadWriteByte(SSPx,(((addr) & 0xFFFFFF) >> 16));  
						Flash_ReadWriteByte(SSPx,(((addr) & 0xFFFF) >> 8));
						Flash_ReadWriteByte(SSPx,(addr) & 0xFF);
						SPI_FLASH_CS_HIGH();		
						/* 等待设备完成擦除操作 */
						SPI_prvWaitUnBusy(SSPx);
						/* 读状态来校验错误标示位 */
						stat = SPI_deviceGetStatus(SSPx);
						if ((stat & SPI_STAT_ERASEERR) != 0) 
						{
								status = SPI_ERR_ERASEERR;
						}	
				}
		}
		
		return status;
}


/*******************************************************************************
* Function Name  : SPI_deviceEraseBlock
* Description    : Erase a block 
* Input          : SPI_INFODATA_T *pHandle device handle,BlockNum块号
* Return         : none
*******************************************************************************/
static SPI_ERR_T SPI_deviceEraseBlock(LPC_SSPn_Type* SSPx,SPI_INFODATA_T *pHandle,uint32_t BlockNum)
{
		uint32_t addr;
		uint16_t stat;
	
		SPI_ERR_T status = SPI_ERR_RANGE;
		if (BlockNum < pHandle->numBlocks)
		{
				status = SPI_prvCheckWriteState(SSPx);
				if (status == SPI_ERR_NONE)
				{
						addr = BlockNum * pHandle->blockSize;
						SPI_prvSetWREN(SSPx);
						SPI_FLASH_CS_LOW();			
						/* 发送64K block擦除命令 */					
						Flash_ReadWriteByte(SSPx,SPI_CMD_SECT_ERASE); 		
						/* 发送4个字节的地址信息 */		
						Flash_ReadWriteByte(SSPx,(((addr) & 0xFFFFFFFF) >> 24));  					
						Flash_ReadWriteByte(SSPx,(((addr) & 0xFFFFFF) >> 16));  
						Flash_ReadWriteByte(SSPx,(((addr) & 0xFFFF) >> 8));
						Flash_ReadWriteByte(SSPx,(addr) & 0xFF);
						SPI_FLASH_CS_HIGH();		
						/* 等待设备完成擦除操作 */
						SPI_prvWaitUnBusy(SSPx);
						/* 读状态来校验错误标示位 */
						stat = SPI_deviceGetStatus(SSPx);
						if ((stat & SPI_STAT_ERASEERR) != 0) 
						{
								status = SPI_ERR_ERASEERR;
						}	
				}
		}
		
		return status;
}



/*******************************************************************************
* Function Name  : SPI_deviceProgram
* Description    : Program a region
* Input          : SPI_INFODATA_T *pHandle device handle,dst 目的地址,Sendbuf 源数据buf,nlen 写入数据长度
* Return         : none
*******************************************************************************/
static SPI_ERR_T SPI_deviceProgram(LPC_SSPn_Type* SSPx,SPI_INFODATA_T *pHandle,uint32_t dst, uint32_t* Sendbuf,uint32_t nlen)
{
		uint16_t stat;
		uint32_t i = 0;
		uint8_t *writeBuff8 = (uint8_t *) Sendbuf;
		SPI_ERR_T status = SPI_ERR_PAGESIZE;
		if (nlen <= pHandle->pageSize)
		{
				status = SPI_prvCheckWriteState(SSPx);
				if (status == SPI_ERR_NONE)
				{
						SPI_prvSetWREN(SSPx);
						SPI_FLASH_CS_LOW();			
						/* 发送写入命令 */					
						Flash_ReadWriteByte(SSPx,SPI_CMD_PAGE_PROGRAM); 		
						/* 发送4个字节的地址信息 */		
						Flash_ReadWriteByte(SSPx,(((dst) & 0xFFFFFFFF) >> 24));  					
						Flash_ReadWriteByte(SSPx,(((dst) & 0xFFFFFF) >> 16));  
						Flash_ReadWriteByte(SSPx,(((dst) & 0xFFFF) >> 8));
						Flash_ReadWriteByte(SSPx,((dst) & 0xFF));
						/* 写入数据 */
						while (nlen) 
						{
								Flash_ReadWriteByte(SSPx,writeBuff8[i]);	
								++writeBuff8;
								--nlen;
						}
						SPI_FLASH_CS_HIGH();		
						/* 等待设备完成写入操作 */
						SPI_prvWaitUnBusy(SSPx);
						/* 读状态来校验错误标示位 */
						stat = SPI_deviceGetStatus(SSPx);
						if ((stat & SPI_STAT_PROGERR) != 0) 
						{
								status = SPI_ERR_PROGERR;
						}	
				}
		}
		return status;
}

/*******************************************************************************
* Function Name  : SPI_deviceRead
* Description    : read a region
* Input          : SPI_INFODATA_T *pHandle device handle,dst 源地址,readbuf 读数据buf,nlen 读出数据长度
* Return         : none
*******************************************************************************/
static SPI_ERR_T SPI_deviceRead(LPC_SSPn_Type* SSPx,SPI_INFODATA_T *pHandle,uint32_t dst, uint32_t* readbuf,uint32_t nlen)
{
		uint16_t devStat;
		uint32_t i = 0;
		uint8_t *readBuff8 = (uint8_t *) readbuf;
		SPI_ERR_T status = SPI_ERR_RANGE;
		if (nlen < pHandle->maxReadSize)
		{
				devStat = SPI_deviceGetStatus(SSPx);
				if(devStat == 0)
				{
						SPI_FLASH_CS_LOW();			
						/* 发送读数据命令 */					
						Flash_ReadWriteByte(SSPx,SPI_CMD_FAST_READ); 		
						/* 发送4个字节的地址信息 */		
						Flash_ReadWriteByte(SSPx,(((dst) & 0xFFFFFFFF) >> 24));  					
						Flash_ReadWriteByte(SSPx,(((dst) & 0xFFFFFF) >> 16));  
						Flash_ReadWriteByte(SSPx,(((dst) & 0xFFFF) >> 8));
						Flash_ReadWriteByte(SSPx,((dst) & 0xFF));
						/* dummy */
						Flash_ReadWriteByte(SSPx,0x00);
						/* 读入数据 */
						while (nlen) 
						{
								readBuff8[i] = Flash_ReadWriteByte(SSPx,0x00);	
								++readBuff8;
								--nlen;
						}
						
						SPI_FLASH_CS_HIGH();		
						status = SPI_ERR_NONE;
				}
		}
		
		return status;
}


/*******************************************************************************
* Function Name  : SPI_Erase
* Description    : Erase multiple blocks
* Input          : SPI_INFODATA_T *pHandle device handle,firstBlock 起始块号,numBlocks 块数
* Return         : none
*******************************************************************************/
static SPI_ERR_T SPI_EraseBlock(LPC_SSPn_Type* SSPx,SPI_INFODATA_T *pHandle, uint32_t firstBlock, uint32_t numBlocks)
{
	SPI_ERR_T err = SPI_ERR_NONE;
	if ((firstBlock + numBlocks) > pHandle->numBlocks) 
	{
		return SPI_ERR_RANGE;
	}

	/* Only perform erase if numBlocks is != 0 */
	for (; (numBlocks); ++firstBlock, --numBlocks) 
	{
		err = SPI_deviceEraseBlock(SSPx,pHandle, firstBlock);
		if (err != SPI_ERR_NONE) 
		{
			break;
		}
	}

	return err;
}


/*******************************************************************************
* Function Name  : SPI_Erase
* Description    : Erase multiple SubBlocks-Sector
* Input          : SPI_INFODATA_T *pHandle device handle,firstSectors 起始4K sub块号,numSectors 4K块数
* Return         : none
*******************************************************************************/
static SPI_ERR_T SPI_EraseSector(LPC_SSPn_Type* SSPx,SPI_INFODATA_T *pHandle, uint32_t firstSectors, uint32_t numSectors)
{
	SPI_ERR_T err = SPI_ERR_NONE;

	if ((firstSectors + numSectors) > pHandle->numSubBlocks) 
	{
		return SPI_ERR_RANGE;
	}

	/* Only perform erase if numBlocks is != 0 */
	for (; (numSectors); ++firstSectors, --numSectors) 
	{
		err = SPI_deviceeEraseSubBlock(SSPx,pHandle, firstSectors);
		if (err != SPI_ERR_NONE) {
			break;
		}
	}

	return err;
}


/*******************************************************************************
* Function Name  : SPI_Program
* Description    : Program the device with the passed buffer
* Input          : SPI_INFODATA_T *pHandle device handle,addr 目的地址,writeBuff 源数据缓冲,bytes 字节数目
* Return         : none
*******************************************************************************/
static SPI_ERR_T SPI_Program(LPC_SSPn_Type* SSPx,SPI_INFODATA_T *pHandle, uint32_t addr, uint32_t *writeBuff, uint32_t bytes)
{
	uint32_t sendBytes;
	SPI_ERR_T err = SPI_ERR_NONE;

	/* Program using up to page size */
	while ((bytes > 0) && (err == SPI_ERR_NONE)) 
	{
		sendBytes = bytes;
		if (sendBytes > pHandle->pageSize) 
		{
			sendBytes = pHandle->pageSize;
		}

		err = SPI_deviceProgram(SSPx,pHandle, addr, writeBuff, sendBytes);
		addr += sendBytes;
		writeBuff += (sendBytes / sizeof(uint32_t));
		bytes -= sendBytes;
	}

	return err;
}


/*******************************************************************************
* Function Name  : SPI_Program
* Description    : Read the device into the passed buffer
* Input          : SPI_INFODATA_T *pHandle device handle,addr 目的地址,readBuff 源数据缓冲,bytes 字节数目
* Return         : none
*******************************************************************************/
static SPI_ERR_T SPI_Read(LPC_SSPn_Type* SSPx,SPI_INFODATA_T *pHandle, uint32_t addr, uint32_t *readBuff, uint32_t bytes)
{
	uint32_t readBytes;
	SPI_ERR_T err = SPI_ERR_NONE;

	/* Read using up to the maximum read size */
	while ((bytes > 0) && (err == SPI_ERR_NONE)) 
	{
		readBytes = bytes;
		if (readBytes > pHandle->maxReadSize) {
			readBytes = pHandle->maxReadSize;
		}
		err = SPI_deviceRead(SSPx,pHandle, addr, readBuff, readBytes);
		addr += readBytes;
		readBuff += (readBytes / sizeof(uint32_t));
		bytes -= readBytes;
	}

	return err;
}


/*******************************************************************************
* Function Name  : SPI_EntireErase
* Description    : Erase the entire spiflash
* Input          : none
* Return         : TRUE: success
									 FALSE:fail
*******************************************************************************/
Bool SSP_EntireErase(LPC_SSPn_Type* SSPx,SPI_INFODATA_T *pHandle)
{
		Bool ret = TRUE;
		SPI_ERR_T errCode;
	
    errCode = SPI_EraseBlock(SSPx,pHandle,0,pHandle->numBlocks);
    if (errCode != SPI_ERR_NONE)
    {
        ret = FALSE;
    }
    
    return ret;
}

/*******************************************************************************
* Function Name  : SPI_Erase
* Description    : SPI Erase Blocks
* Input          : pFirstBlock 首块,pNumBlocks 块数
* Return         : TRUE: success
									 FALSE:fail
*******************************************************************************/
Bool SSP_Erase(LPC_SSPn_Type* SSPx,SPI_INFODATA_T *pHandle,uint32_t pFirstBlock, uint32_t pNumBlocks)
{
		Bool ret = TRUE;
		SPI_ERR_T errCode;
    /* Next erase everything */
    errCode = SPI_EraseBlock(SSPx,pHandle,pFirstBlock,pNumBlocks);
    if (errCode != SPI_ERR_NONE)
    {
        ret = FALSE;
    }
    
    return ret;
}


/*******************************************************************************
* Function Name  : SPI_EraseSectors
* Description    : SPI Erase Sectors
* Input          : pFirstBlock 首块,pNumBlocks 块数
* Return         : TRUE: success
									 FALSE:fail
*******************************************************************************/
Bool SSP_EraseSectors(LPC_SSPn_Type* SSPx,SPI_INFODATA_T *pHandle,uint32_t pFirstSector, uint32_t pNumSectors)
{
		Bool ret = TRUE;
		SPI_ERR_T errCode;
	
    /* Next erase Sector */
    errCode = SPI_EraseSector(SSPx,pHandle,pFirstSector,pNumSectors);
    if (errCode != SPI_ERR_NONE)
    {
        ret = FALSE;
    }
    
    return ret;
}

/*******************************************************************************
* Function Name  : SSP_Write
* Description    : SPI Write
* Input          : dst 目的地址,src 源地址,len 长度
* Return         : TRUE: success
									 FALSE:fail
*******************************************************************************/
Bool SSP_Write(LPC_SSPn_Type* SSPx,uint32_t dst, uint32_t *src, uint32_t len)
{
		SPI_ERR_T errCode = SPI_ERR_NONE;
		uint32_t baseAddr = 0,deviceBytes = 0,sector = 0,sendBytes = 0,pageBytes = 0;
		SPI_INFODATA_T *pHandle = &g_spiinfo;
	
		//检查入口参数
		if((src == NULL)||(len == 0))
		{
			return FALSE;
		}
		
		deviceBytes = pHandle->numBlocks * pHandle->blockSize ;
		baseAddr = pHandle->baseAddr ;	
		
		if((dst < baseAddr) || ((dst + len) > (baseAddr + deviceBytes)))
		{
			return FALSE;
		}
		
		pageBytes = pHandle->pageSize;
		sendBytes = pageBytes - (dst - pHandle->baseAddr)%pageBytes;
		if(sendBytes >= len)
		{
				// 整块扇区开始则擦除 
				if(!(dst % pHandle->subBlockSize))
				{
					sector = (dst - pHandle->baseAddr) / (pHandle->subBlockSize);
					if(SSP_EraseSectors(SSPx,pHandle,sector, 1) == FALSE)
					{
							return FALSE;
					}
				}
				//一页内剩余空间大于写入字节个数
				errCode = SPI_Program(SSPx,pHandle,dst,(uint32_t *)src,len);
		}
		else
		{
				while ((len > 0) && (errCode == SPI_ERR_NONE))
				{
						// 整块扇区开始则擦除 
						if(!(dst % pHandle->subBlockSize))
						{
							sector = (dst - pHandle->baseAddr) / (pHandle->subBlockSize);
							if(SSP_EraseSectors(SSPx,pHandle,sector, 1) == FALSE)
							{
									return FALSE;
							}
						}
						errCode = SPI_Program(SSPx,pHandle,dst,(uint32_t *)src,sendBytes);
						dst += sendBytes;
						src += (sendBytes / sizeof(uint32_t));
						len -= sendBytes;
						
						sendBytes = len;
						if(sendBytes > pageBytes)
						{
								sendBytes = pageBytes;
						}
				}
		}
		
		if (errCode != SPI_ERR_NONE)
    {
       return FALSE;
    }
		
		return TRUE;
}


/*******************************************************************************
* Function Name  : SSP_Read
* Description    : SPI read
* Input          : dst 目的地址,src 源地址,len 长度
* Return         : TRUE: success
									 FALSE:fail
*******************************************************************************/
Bool SSP_Read(LPC_SSPn_Type* SSPx,uint32_t dst, uint32_t *src, uint32_t len)
{
		SPI_ERR_T errCode = SPI_ERR_NONE;
		uint32_t deviceBytes = 0,baseAddr = 0;
		SPI_INFODATA_T *pHandle = NULL;
	
		//检查入口参数
		if((src == NULL)||(len == 0))
		{
			return FALSE;
		}
		
		pHandle = &g_spiinfo;
		
		deviceBytes = pHandle->numBlocks * pHandle->blockSize ;
		baseAddr = pHandle->baseAddr ;	
		
		if((dst < baseAddr) || ((dst + len) > (baseAddr + deviceBytes)))
		{
			return FALSE;
		}
		
		errCode = SPI_Read(SSPx,pHandle,dst,(uint32_t *)src,len);
		if (errCode != SPI_ERR_NONE)
    {
        return FALSE;
    }
		
		return TRUE;
}

