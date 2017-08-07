/**********************************************************************
* $Id$      Task              2016-04-27
*//**
* @file     MemManage.h
* @brief    系统内存管理单元头文件
* @version  1.0.0
* @date     2016/04/27 星期三 13:20:15
* @author   ZhangYanMing
*
* Copyright(C) 2016, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
#ifndef _MEMMANAGE_H_
#define _MEMMANAGE_H_
/************************** Includes **********************************/
#include "lpc_types.h"


/******************************* Macro ********************************/
#define  MEMMANAGE_MAXLEN32_NUM     	32   
#define  MEMMANAGE_MAXLEN64_NUM     	32   
#define  MEMMANAGE_MAXLEN128_NUM      8   
#define  MEMMANAGE_MAXLEN256_NUM      4  
#define  MEMMANAGE_MAXLEN512_NUM      4 
#define  MEMMANAGE_MAXLEN1024_NUM     8 


/************************** Function Interface ************************/
void  create_os_mempool(void);
void  *MemApply(uint16_t MemSize);
void  MemFree(void *pMem);

#endif




