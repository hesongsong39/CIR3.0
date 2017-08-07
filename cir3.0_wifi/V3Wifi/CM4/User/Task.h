/**********************************************************************
* $Id$      Task              2015-12-16
*//**
* @file     Task.h
* @brief    创建系统任务、信号量、邮箱、队列头文件
* @version  1.0.0
* @date     2015/12/16 星期三 15:20:15
* @author   ZhangYanMing
*
* Copyright(C) 2015, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
#ifndef _TASK_H_
#define _TASK_H_

/************************** Includes **********************************/
#include <ucos_ii.h>

/************************** TASK PRIORITIES ***************************/
#define  APP_CFG_TASK_START_PRIO        				2u
#define  APP_CFG_TASK_MSGDISPATCH_PRIO  				9u
#define  APP_CFG_TASK_MSGTX_PRIO       					13u
#define  APP_CFG_TASK_MSGRX_PRIO       					7u
#define  APP_CFG_TASK_NET_PRIO							4u

/************************** TASK STACK SIZES **************************/
#define APP_CFG_TASK_START_STK_SIZE      					1024u
#define APP_CFG_TASK_MSGDISPATCH_STK_SIZE     				5120u
#define APP_CFG_TASK_MSGTX_STK_SIZE      					512u
#define APP_CFG_TASK_MSGRX_STK_SIZE      					512u
#define APP_CFG_TASK_NET_STK_SIZE							512u

/************************** TASK STACK SIZES **************************/
#define g_MsgDispatchTbl_MAX     16
#define g_MsgTxTbl_MAX        	 16
#define g_MsgUart0TxTbl_MAX      8
#define g_MsgUart1TxTbl_MAX      8
#define g_MsgUart2TxTbl_MAX      8
#define g_MsgUart3TxTbl_MAX      8
#define g_MsgCan0TxTbl_MAX       8
#define g_MsgCan1TxTbl_MAX       8
#define g_QNetTxMsgTbl_MAX       8  


/*****************Extern variables ************************************/
extern  OS_STK  AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];
extern  OS_STK  AppTaskUartRxStk[APP_CFG_TASK_MSGDISPATCH_STK_SIZE];
extern  OS_STK  AppTaskUartTxStk[APP_CFG_TASK_MSGTX_STK_SIZE];
extern  OS_STK  AppTaskMsgRxStk[APP_CFG_TASK_MSGRX_STK_SIZE];

extern OS_EVENT  *g_MsgDispatch;
extern OS_EVENT  *g_MsgTx;
extern OS_EVENT  *g_MsgUart0Tx;
extern OS_EVENT  *g_MsgUart1Tx;
extern OS_EVENT  *g_MsgUart2Tx;
extern OS_EVENT  *g_MsgUart3Tx;
extern OS_EVENT  *g_MsgCan0Tx;
extern OS_EVENT  *g_MsgCan1Tx;
extern OS_EVENT  *g_NetTXMsgQ;

/************************** Function Interface ************************/
void create_os_task(void);
void create_os_mutex(void);
void create_os_queue(void);
void create_os_mem(void);
void create_os_eventflag(void);
void create_os_tmr(void);
void create_os_sem(void);



#endif



