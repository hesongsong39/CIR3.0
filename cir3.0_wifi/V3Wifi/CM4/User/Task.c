/**********************************************************************
* $Id$      Task              2015-12-16
*//**
* @file     Task.c
* @brief    创建系统任务、信号量、邮箱、队列
* @version  1.0.0
* @date     2015/12/16 星期三 15:20:15
* @author   ZhangYanMing
*
* Copyright(C) 2015, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
/************************** Includes **********************************/
#include "lpc_types.h"
#include "Task.h"
#include "MemManage.h"
#include "AppMsgTxpro.h"
#include "AppMsgDispatchpro.h"
#include "AppMsgRxpro.h"
#include "AppAudiopro.h"
#include "UartRxpro.h"
#include "AppNetpro.h"

/*****************Private variables ***********************************/


/*****************Public variables ************************************/
OS_STK  AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];
OS_STK  AppTaskMsgDispatchStk[APP_CFG_TASK_MSGDISPATCH_STK_SIZE];
OS_STK  AppTaskMsgTxStk[APP_CFG_TASK_MSGTX_STK_SIZE];
OS_STK  AppTaskMsgRxStk[APP_CFG_TASK_MSGRX_STK_SIZE];
OS_STK  AppTaskNetStk[APP_CFG_TASK_NET_STK_SIZE];


OS_EVENT *g_MsgDispatch;
void     *g_MsgDispatchTbl[g_MsgDispatchTbl_MAX];

OS_EVENT *g_MsgUart0Tx;
void     *g_MsgUart0TxTbl[g_MsgUart0TxTbl_MAX];

OS_EVENT *g_MsgUart1Tx;
void     *g_MsgUart1TxTbl[g_MsgUart1TxTbl_MAX];

OS_EVENT *g_MsgUart2Tx;
void     *g_MsgUart2TxTbl[g_MsgUart2TxTbl_MAX];

OS_EVENT *g_MsgUart3Tx;
void     *g_MsgUart3TxTbl[g_MsgUart3TxTbl_MAX];

OS_EVENT *g_MsgCan0Tx;
void     *g_MsgCan0TxTbl[g_MsgCan0TxTbl_MAX];

OS_EVENT *g_MsgCan1Tx;
void     *g_MsgCan1TxTbl[g_MsgCan1TxTbl_MAX];

OS_EVENT *g_MsgTx;
void     *g_MsgTxTbl[g_MsgTxTbl_MAX];

OS_EVENT *g_NetTXMsgQ;                                    
void     *g_NetTXMsgTbl[g_QNetTxMsgTbl_MAX];

/*****************Extern variables ************************************/


/*********************************************************************//**
 * @brief 			创建系统中的应用任务
 * @param[in] 	无
 * @return			None
 ************************************************************************/
void create_os_task(void)
{
		OSTaskCreateExt((void (*)(void *)) AppTaskMsgDispatchProc,           
									(void           *) 0,
									(OS_STK         *)&AppTaskMsgDispatchStk[APP_CFG_TASK_MSGDISPATCH_STK_SIZE - 1],
									(INT8U           ) APP_CFG_TASK_MSGDISPATCH_PRIO,
									(INT16U          ) APP_CFG_TASK_MSGDISPATCH_PRIO,
									(OS_STK         *)&AppTaskMsgDispatchStk[0],
									(INT32U          ) APP_CFG_TASK_MSGDISPATCH_STK_SIZE,
									(void           *) 0,
									(INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));	
		OSTaskCreateExt((void (*)(void *)) AppTaskMsgTxProc,                                         
									(void           *) 0,
									(OS_STK         *)&AppTaskMsgTxStk[APP_CFG_TASK_MSGTX_STK_SIZE - 1],
									(INT8U           ) APP_CFG_TASK_MSGTX_PRIO,
									(INT16U          ) APP_CFG_TASK_MSGTX_PRIO,
									(OS_STK         *)&AppTaskMsgTxStk[0],
									(INT32U          ) APP_CFG_TASK_MSGTX_STK_SIZE,
									(void           *) 0,
									(INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));	
	
		OSTaskCreateExt((void (*)(void *)) AppTaskMsgRxProc,           
									(void           *) 0,
									(OS_STK         *)&AppTaskMsgRxStk[APP_CFG_TASK_MSGRX_STK_SIZE - 1],
									(INT8U           ) APP_CFG_TASK_MSGRX_PRIO,
									(INT16U          ) APP_CFG_TASK_MSGRX_PRIO,
									(OS_STK         *)&AppTaskMsgRxStk[0],
									(INT32U          ) APP_CFG_TASK_MSGRX_STK_SIZE,
									(void           *) 0,
									(INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));	
		OSTaskCreateExt((void (*)(void *)) AppTaskNetProc,           
									(void           *) 0,
									(OS_STK         *)&AppTaskNetStk[APP_CFG_TASK_NET_STK_SIZE - 1],
									(INT8U           ) APP_CFG_TASK_NET_PRIO,
									(INT16U          ) APP_CFG_TASK_NET_PRIO,
									(OS_STK         *)&AppTaskNetStk[0],
									(INT32U          ) APP_CFG_TASK_NET_STK_SIZE,
									(void           *) 0,
									(INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));	
}


/*********************************************************************//**
 * @brief 			创建系统中的互斥信号量
 * @param[in] 	无
 * @return			None
 ************************************************************************/
void create_os_mutex(void)
{
			
}

/*********************************************************************//**
 * @brief 			创建系统中的消息队列
 * @param[in] 	无
 * @return			None
 ************************************************************************/
void create_os_queue(void)
{
	g_MsgDispatch = OSQCreate(&g_MsgDispatchTbl[0], g_MsgDispatchTbl_MAX);
	g_MsgTx = OSQCreate(&g_MsgTxTbl[0], g_MsgTxTbl_MAX);
	
	g_MsgUart0Tx = OSQCreate(&g_MsgUart0TxTbl[0], g_MsgUart0TxTbl_MAX);
	g_MsgUart1Tx = OSQCreate(&g_MsgUart1TxTbl[0], g_MsgUart1TxTbl_MAX);
	g_MsgUart2Tx = OSQCreate(&g_MsgUart2TxTbl[0], g_MsgUart2TxTbl_MAX);
	g_MsgUart3Tx = OSQCreate(&g_MsgUart3TxTbl[0], g_MsgUart3TxTbl_MAX);
	g_MsgCan0Tx = OSQCreate(&g_MsgCan0TxTbl[0], g_MsgCan0TxTbl_MAX);
	g_MsgCan1Tx = OSQCreate(&g_MsgCan1TxTbl[0], g_MsgCan1TxTbl_MAX);
	
	g_NetTXMsgQ = OSQCreate(&g_NetTXMsgTbl[0], g_QNetTxMsgTbl_MAX);	
	
}


/*********************************************************************//**
 * @brief 			创建系统中的内存分区
 * @param[in] 	无
 * @return			None
 ************************************************************************/
void create_os_mem(void)
{
	create_os_mempool();
}

/*********************************************************************//**
 * @brief 			创建系统中的事件组
 * @param[in] 	无
 * @return			None
 ************************************************************************/
void create_os_eventflag(void)
{
			
}

/*********************************************************************//**
 * @brief 			创建系统中的信号量
 * @param[in] 	无
 * @return			None
 ************************************************************************/
void create_os_sem(void)
{
	
}


/*********************************************************************//**
 * @brief 			创建系统中的定时器
 * @param[in] 	无
 * @return			None
 ************************************************************************/
void create_os_tmr(void)
{
	
}




