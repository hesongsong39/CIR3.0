/**********************************************************************
* $Id$      AppTime              2016-05-31
*//**
* @file     AppTime.h
* @brief    定时器相关操作头文件
* @version  1.0.0
* @date     2016/05/31 星期二 14:28:44
* @author   ZhangYanMing
*
* Copyright(C) 2016, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
#ifndef _APPTIME_H_
#define _APPTIME_H_

/**************************** Includes ********************************/
#include "lpc_types.h"


/************************ Micron Definition ***************************/
#define   TIMECNT    2
#define   T_RXGD     0




#pragma push
#pragma pack(1)                               /* 1字节对齐 */

typedef struct
{     
	uint32_t lastupdate; 
	uint32_t timeout;
}DS_TIME;


#pragma pop

extern  DS_TIME g_timer[TIMECNT];

/************************** Function Interface ************************/
void TimerRef(uint8_t num,uint32_t to);
void TimerInit(uint8_t num);



#endif




