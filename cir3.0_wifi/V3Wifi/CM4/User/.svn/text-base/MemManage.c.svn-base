/**********************************************************************
* $Id$      Task              2016-04-27
*//**
* @file     MemManage.c
* @brief    系统内存管理单元
* @version  1.0.0
* @date     2016/04/27 星期三 13:20:15
* @author   ZhangYanMing
*
* Copyright(C) 2016, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
/************************** Includes **********************************/
#include "lpc_types.h"
#include <ucos_ii.h>
#include "MemManage.h"





/*****************Private variables ***********************************/
static OS_MEM   *g_MemLen32;  
__align(4) static INT8U    g_TblMemLen32[MEMMANAGE_MAXLEN32_NUM][32];

static OS_MEM   *g_MemLen64;  
__align(4) static INT8U    g_TblMemLen64[MEMMANAGE_MAXLEN64_NUM][64];

static OS_MEM   *g_MemLen128;  
__align(4) static INT8U    g_TblMemLen128[MEMMANAGE_MAXLEN128_NUM][128];

static OS_MEM   *g_MemLen256;  
__align(4) static INT8U    g_TblMemLen256[MEMMANAGE_MAXLEN256_NUM][256];

static OS_MEM   *g_MemLen512;  
__align(4) static INT8U    g_TblMemLen512[MEMMANAGE_MAXLEN512_NUM][512];

static OS_MEM   *g_MemLen1024;  
__align(4) static INT8U    g_TblMemLen1024[MEMMANAGE_MAXLEN1024_NUM][1024];

/*****************Public variables ************************************/


/*****************Extern variables ************************************/








/*********************************************************************//**
 * @brief 			创建系统中的内存池
 * @param[in] 	无
 * @return			None
 ************************************************************************/
void create_os_mempool(void)
{
	INT8U  err;
	
	g_MemLen32 = OSMemCreate(g_TblMemLen32,MEMMANAGE_MAXLEN32_NUM,32, &err); 
	g_MemLen64 = OSMemCreate(g_TblMemLen64,MEMMANAGE_MAXLEN64_NUM,64, &err); 
	g_MemLen128 = OSMemCreate(g_TblMemLen128,MEMMANAGE_MAXLEN128_NUM,128, &err); 
	g_MemLen256 = OSMemCreate(g_TblMemLen256,MEMMANAGE_MAXLEN256_NUM,256, &err); 
	g_MemLen512 = OSMemCreate(g_TblMemLen512,MEMMANAGE_MAXLEN512_NUM,512, &err); 
	g_MemLen1024 = OSMemCreate(g_TblMemLen1024,MEMMANAGE_MAXLEN1024_NUM,1024, &err); 
}




/********************************************************************//**
 * @brief 		从内存池获得内存块
 * @param[in]	MemSiz,内存块数据长度
 * @return 		内存块地址
 *********************************************************************/
void  *MemApply(uint16_t MemSize)
{
		uint8_t err;
		char *pBlock = NULL;
		//根据数据长度分配内存块
		if(MemSize > 1024)
		{
				pBlock = NULL;
		}
		else if(MemSize > 512)
		{
				pBlock = OSMemGet(g_MemLen1024, &err);
		}
		else if(MemSize > 256)
		{
				pBlock = OSMemGet(g_MemLen512, &err);
				if(pBlock == 0)
				{
						pBlock = OSMemGet(g_MemLen1024, &err);
				}
		}
		else if(MemSize > 128)
		{
				pBlock = OSMemGet(g_MemLen256, &err);
				if(pBlock == 0)
				{
						pBlock = OSMemGet(g_MemLen512, &err);
						if(pBlock == 0)
						{
								pBlock = OSMemGet(g_MemLen1024, &err);
						}
				}
		}
		else if(MemSize > 64)
		{
				pBlock = OSMemGet(g_MemLen128, &err);
				if(pBlock == 0)
				{
						pBlock = OSMemGet(g_MemLen256, &err);
						if(pBlock == 0)
						{
								pBlock = OSMemGet(g_MemLen512, &err);
								if(pBlock == 0)
								{
										pBlock = OSMemGet(g_MemLen1024, &err);
								}
						}
				}
		}
		else if(MemSize > 32)
		{
				pBlock = OSMemGet(g_MemLen64, &err);
				if(pBlock == 0)
				{
						pBlock = OSMemGet(g_MemLen128, &err);
						if(pBlock == 0)
						{
								pBlock = OSMemGet(g_MemLen256, &err);
								if(pBlock == 0)
								{
										pBlock = OSMemGet(g_MemLen512, &err);
										if(pBlock == 0)
										{
												pBlock = OSMemGet(g_MemLen1024, &err);
										}
								}
						}
				}
		}
		else
		{
				pBlock = OSMemGet(g_MemLen32, &err);
				if(pBlock == 0)
				{
						pBlock = OSMemGet(g_MemLen64, &err);
						if(pBlock == 0)
						{
								pBlock = OSMemGet(g_MemLen128, &err);
								if(pBlock == 0)
								{
										pBlock = OSMemGet(g_MemLen256, &err);
										if(pBlock == 0)
										{
												pBlock = OSMemGet(g_MemLen512, &err);
												if(pBlock == 0)
												{
														pBlock = OSMemGet(g_MemLen1024, &err);
												}
										}
								}
						}
				}
		}
		
		return pBlock;
}




/********************************************************************//**
 * @brief 		从内存池获得内存块
 * @param[in]	
 * @return 		
 *********************************************************************/
void  MemFree(void *pMem)
{
		uint8_t i = 0;
		//进行各个内存块判断
		for(i=0;i<MEMMANAGE_MAXLEN32_NUM;i++)
		{
				if(pMem == g_TblMemLen32[i])
				{
						OSMemPut(g_MemLen32, pMem);
						return;
				}
		}
		
		for(i=0;i<MEMMANAGE_MAXLEN64_NUM;i++)
		{
				if(pMem == g_TblMemLen64[i])
				{
						OSMemPut(g_MemLen64, pMem);
						return;
				}
		}
		
		for(i=0;i<MEMMANAGE_MAXLEN128_NUM;i++)
		{
				if(pMem == g_TblMemLen128[i])
				{
						OSMemPut(g_MemLen128, pMem);
						return;
				}
		}
		
		for(i=0;i<MEMMANAGE_MAXLEN256_NUM;i++)
		{
				if(pMem == g_TblMemLen256[i])
				{
						OSMemPut(g_MemLen256, pMem);
						return;
				}
		}
		
		for(i=0;i<MEMMANAGE_MAXLEN512_NUM;i++)
		{
				if(pMem == g_TblMemLen512[i])
				{
						OSMemPut(g_MemLen512, pMem);
						return;
				}
		}
		
		for(i=0;i<MEMMANAGE_MAXLEN1024_NUM;i++)
		{
				if(pMem == g_TblMemLen1024[i])
				{
						OSMemPut(g_MemLen1024, pMem);
						return;
				}
		}
}




