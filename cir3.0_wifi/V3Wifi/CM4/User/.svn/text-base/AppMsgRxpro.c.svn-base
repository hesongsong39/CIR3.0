/**********************************************************************
* $Id$      AppMsgRxpro              2015-12-23
*//**
* @file     AppMsgRxpro.c
* @brief    接收数据处理
* @version  1.0.0
* @date     2015/12/23 星期三 14:28:44
* @author   ZhangYanMing
*
* Copyright(C) 2015, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
#include "lpc_types.h"
#include <ucos_ii.h>
#include "Task.h"
#include "AppMsgRxpro.h"
#include "CanRxpro.h"
#include "UartRxpro.h"
#include "DataTransfer.h"

/*********************************************************************//**
 * @brief			AppTaskUartRxProc
 * @param[in]	p_arg   
 * @return 		None
 **********************************************************************/
void  AppTaskMsgRxProc(void *p_arg)
{	
	Board_Can_Init();
	Board_Uart_Init();
	while(1)
	{
			CCAN_QueueProc();
			Uart_QueueProc();
			OSTimeDlyHMSM(0,0,0,10);
	}
}







