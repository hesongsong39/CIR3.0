/************************************************************
  Copyright (C), 2016, 712.Co.,Ltd.
  FileName: AppDefine.c
  Author: zhaohualei      
  Description: 
  Version:       V1.0.01  
  Function List:   
  
  History:        
  <author>  <time>   <version >   <desc>
      
***********************************************************/
#include <string.h>
#include <ucos_ii.h>
#include "AppDefine.h"

//******************全局变量*******************//
_AppPara  g_ParaInfo;
_McuBusIInfo g_stMcuBusInfo;


/*********************************************************************//**
 * @brief			综合通告信息处理
 * @param[in]	
 * @return 		None
 **********************************************************************/
void Comprehensive_Info_Handle(uint8_t *pdata)
{
		//涉及到公用业务参数，需要进行共享资源保护									
        OSSchedLock();             /* 关调度保护 */		
		memcpy(&g_stMcuBusInfo,pdata,sizeof(g_stMcuBusInfo));
		switch(g_stMcuBusInfo.iPrimaryCan)
		{
			case 0xE1:
			{
				g_ParaInfo.MasterCanBus = INTERFACE_CAN1;
			}
			break;
			case 0XE2:
			{
				g_ParaInfo.MasterCanBus = INTERFACE_CAN2;
			}
			break;
			default:break;
		}
		OSSchedUnlock();           /* 解除关调度保护 */
}

uint8_t PrimaryCAN_Get(void)
{
	  uint8_t  PrimaryCAN = 0;
		
		OSSchedLock();             /* 关调度保护 */
		PrimaryCAN = g_ParaInfo.MasterCanBus;
		OSSchedUnlock();           /* 解除关调度保护 */
		
		return  PrimaryCAN;
}
