/**********************************************************************
* $Id$      eMMC              2016-03-16
*//**
* @file     eMMC.h
* @brief    SD/MMC接口操作外部eMMC头文件
* @version  1.0.0
* @date     2016/03/17 星期四 08:20:15
* @author   ZhangYanMing
*
* Copyright(C) 2016, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
#ifndef _EMMC_H_
#define _EMMC_H_
/************************** Includes **********************************/
#include "lpc_types.h"
#include "lpc43xx_scu.h"

typedef  void (*pFnpininit)(void);

/************************** Macro definition  *************************/
#define  EMMCFASTINOUTPIN (PUP_DISABLE | MD_EZI | MD_EHS | MD_ZI)
#define  EMMC_WIDTH_4BIT   4
#define  EMMC_WIDTH_8BIT   8


/************************** Function Interface ************************/
Bool EmmcInit(uint8_t width,pFnpininit fpininit);
Bool Emmc_Write(uint32_t StartSector,uint8_t SectorNum,uint8_t *buf,uint32_t wlen);
Bool Emmc_Read(uint32_t StartSector,uint8_t SectorNum,uint8_t *buf,uint32_t rlen);


#endif



