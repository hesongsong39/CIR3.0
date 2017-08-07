/**********************************************************************
* $Id$      AppMsgTxpro              2015-12-23
*//**
* @file     AppMsgTxpro.c
* @brief    串口接收数据处理
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
#include "AppMsgTxpro.h"
#include "Uart.h"
#include "App.h"
#include "MemManage.h"
#include "DataReSend.h"
#include "ptcan.h"
#include "DataTransfer.h"

/*****************Private variables ***********************************/
static MsgTx_Queue_Status MsgTx_Uart0;
static MsgTx_Queue_Status MsgTx_Uart1;
static MsgTx_Queue_Status MsgTx_Uart2;
static MsgTx_Queue_Status MsgTx_Uart3;
static MsgTx_Queue_Status MsgTx_CCAN0;
static MsgTx_Queue_Status MsgTx_CCAN1;

static uint8_t txbuf[1024] = {0};
/*****************Public variables ************************************/


/*****************Extern variables ************************************/


/*********************************************************************//**
 * @brief       向串口发送队列中添加内存指针
 * @param[in]   None
 * @return      None
 **********************************************************************/
static Bool MsgTx_QueueAdd(OS_EVENT * pMsg,void *pStr)
{
		if(OSQPost(pMsg, pStr) == OS_ERR_NONE)
		{
				return TRUE;
		}
		else
		{
				return FALSE;
		}
}


/********************************************************************//**
 * @brief 		发送数据队列初始化
 * @param[in]	None
 * @return 		None
 *********************************************************************/
static void Board_MsgTxQueue_Init(void)
{
		MsgTx_Uart0.IsBusy = FALSE;
		MsgTx_Uart0.pbuf = NULL;
	
		MsgTx_Uart1.IsBusy = FALSE;
		MsgTx_Uart1.pbuf = NULL;
	
		MsgTx_Uart2.IsBusy = FALSE;
		MsgTx_Uart2.pbuf = NULL;
		
		MsgTx_Uart3.IsBusy = FALSE;
		MsgTx_Uart3.pbuf = NULL;
	
		MsgTx_CCAN0.IsBusy = FALSE;
		MsgTx_CCAN0.pbuf = NULL;
	
		MsgTx_CCAN1.IsBusy = FALSE;
		MsgTx_CCAN1.pbuf = NULL;
}

/*********************************************************************//**
 * @brief       串口数据发送队列处理函数
 * @param[in]   None
 * @return      None
 **********************************************************************/
static void MsgTx_QueueProc(void)
{
		OS_Q_DATA data;
		TranItem *pTI = NULL;
		CCANItem *pCan = NULL;
		INT8U err;
		//串口0底层发送队列是否满标识
		if(MsgTx_Uart0.IsBusy == TRUE)
		{
				pTI = MsgTx_Uart0.pbuf;
				if(UartSend(0,pTI->Info,pTI->Infolen) == TRUE)
				{
						MsgTx_Uart0.IsBusy = FALSE;
						//内存释放
						MemFree(pTI);
				}
		}
		else
		{
				//查询串口0发送消息队列
				err = OSQQuery(g_MsgUart0Tx,&data);
				if(OS_ERR_NONE ==err && data.OSNMsgs > 0 )
				{
						pTI = (TranItem *)OSQPend(g_MsgUart0Tx,0,&err);
						if(UartSend(0,pTI->Info,pTI->Infolen) == FALSE)
						{
								MsgTx_Uart0.IsBusy = TRUE;
								MsgTx_Uart0.pbuf = pTI;
						}
						else
						{
								MemFree(pTI);
						}
				}
		}
		
		//串口1底层发送队列是否满标识
		if(MsgTx_Uart1.IsBusy == TRUE)
		{
				pTI = MsgTx_Uart1.pbuf;
				if(UartSend(1,pTI->Info,pTI->Infolen) == TRUE)
				{
						MsgTx_Uart1.IsBusy = FALSE;
						//内存释放
						MemFree(pTI);
				}
		}
		else
		{
				//查询串口1发送消息队列
				err = OSQQuery(g_MsgUart1Tx,&data);
				if(OS_ERR_NONE ==err && data.OSNMsgs > 0 )
				{
						pTI = (TranItem *)OSQPend(g_MsgUart1Tx,0,&err);
						if(UartSend(1,pTI->Info,pTI->Infolen) == FALSE)
						{
								MsgTx_Uart1.IsBusy = TRUE;
								MsgTx_Uart1.pbuf = pTI;
						}
						else
						{
								MemFree(pTI);
						}
				}
		}
		
		//串口2底层发送队列是否满标识
		if(MsgTx_Uart2.IsBusy == TRUE)
		{
				pTI = MsgTx_Uart2.pbuf;
				if(UartSend(2,pTI->Info,pTI->Infolen) == TRUE)
				{
						MsgTx_Uart2.IsBusy = FALSE;
						//内存释放
						MemFree(pTI);
				}
		}
		else
		{
				//查询串口2发送消息队列
				err = OSQQuery(g_MsgUart2Tx,&data);
				if(OS_ERR_NONE ==err && data.OSNMsgs > 0 )
				{
						pTI = (TranItem *)OSQPend(g_MsgUart2Tx,0,&err);
						if(UartSend(2,pTI->Info,pTI->Infolen) == FALSE)
						{
								MsgTx_Uart2.IsBusy = TRUE;
								MsgTx_Uart2.pbuf = pTI;
						}
						else
						{
								MemFree(pTI);
						}
				}
		}
		
		//串口3底层发送队列是否满标识
		if(MsgTx_Uart3.IsBusy == TRUE)
		{
				pTI = MsgTx_Uart3.pbuf;
				if(UartSend(3,pTI->Info,pTI->Infolen) == TRUE)
				{
						MsgTx_Uart3.IsBusy = FALSE;
						//内存释放
						MemFree(pTI);
				}
		}
		else
		{
				//查询串口3发送消息队列
				err = OSQQuery(g_MsgUart3Tx,&data);
				if(OS_ERR_NONE ==err && data.OSNMsgs > 0 )
				{
						pTI = (TranItem *)OSQPend(g_MsgUart3Tx,0,&err);
						if(UartSend(3,pTI->Info,pTI->Infolen) == FALSE)
						{
								MsgTx_Uart3.IsBusy = TRUE;
								MsgTx_Uart3.pbuf = pTI;
						}
						else
						{
								MemFree(pTI);
						}
				}
		}
		
		//CCAN0底层发送队列是否满标识
		if(MsgTx_CCAN0.IsBusy == TRUE)
		{
				pTI = (TranItem *)MsgTx_CCAN0.pbuf;
				pCan = (CCANItem *)(pTI->Info);
				if(CanSendDataPackToCanBus(pCan->Info,pCan->Infolen,&(pCan->CanParam)) == CCAN_MSG_OK)
				{
						MsgTx_CCAN0.IsBusy = FALSE;
						//内存释放
						MemFree(pTI);
				}
		}
		else
		{
				//查询CCAN0发送消息队列
				err = OSQQuery(g_MsgCan0Tx,&data);
				if(OS_ERR_NONE ==err && data.OSNMsgs > 0 )
				{
						pTI = (TranItem *)OSQPend(g_MsgCan0Tx,0,&err);
						pCan = (CCANItem *)(pTI->Info);
						if(CanSendDataPackToCanBus(pCan->Info,pCan->Infolen,&(pCan->CanParam)) != CCAN_MSG_OK)
						{
								MsgTx_CCAN0.IsBusy = TRUE;
								MsgTx_CCAN0.pbuf = pTI;
						}
						else
						{
								MemFree(pTI);
						}
				}
		}
		
		//CCAN1底层发送队列是否满标识
		if(MsgTx_CCAN1.IsBusy == TRUE)
		{
				pTI = (TranItem *)MsgTx_CCAN1.pbuf;
				pCan = (CCANItem *)(pTI->Info);
				if(CanSendDataPackToCanBus(pCan->Info,pCan->Infolen,&(pCan->CanParam)) == CCAN_MSG_OK)
				{
						MsgTx_CCAN1.IsBusy = FALSE;
						//内存释放
						MemFree(pTI);
				}
		}
		else
		{
				//查询CCAN1发送消息队列
				err = OSQQuery(g_MsgCan1Tx,&data);
				if(OS_ERR_NONE ==err && data.OSNMsgs > 0 )
				{
						pTI = (TranItem *)OSQPend(g_MsgCan1Tx,0,&err);
						pCan = (CCANItem *)(pTI->Info);
						if(CanSendDataPackToCanBus(pCan->Info,pCan->Infolen,&(pCan->CanParam)) != CCAN_MSG_OK)
						{
								MsgTx_CCAN1.IsBusy = TRUE;
								MsgTx_CCAN1.pbuf = pTI;
						}
						else
						{
								MemFree(pTI);
						}
				}
		}
		
}

/*********************************************************************//**
 * @brief			AppTaskMsgTxProc
 * @param[in]	p_arg  
 * @return 		None
 **********************************************************************/
void  AppTaskMsgTxProc(void *p_arg)
{	
	INT8U err;
	TranItem *pTI = NULL;
	CCANItem *pCan = NULL;
	//发送队列相关变量初始化
	Board_MsgTxQueue_Init();
	while(1)
	{
		pTI = (TranItem *)OSQPend(g_MsgTx,10,&err);
		if(pTI)
		{
			switch(pTI->chFrom)
			{
				case INTERFACE_COM0:
					{
							if(MsgTx_QueueAdd(g_MsgUart0Tx,pTI) == FALSE)
							{
									MemFree(pTI);
							}
					}
					break;
				case INTERFACE_COM1:
					{
							if(MsgTx_QueueAdd(g_MsgUart1Tx,pTI) == FALSE)
							{
									MemFree(pTI);
							}
					}
					break;
				case INTERFACE_COM2:
					{
							if(MsgTx_QueueAdd(g_MsgUart2Tx,pTI) == FALSE)
							{
									MemFree(pTI);
							}
					}
					break;
				case INTERFACE_COM3:
					{
							if(MsgTx_QueueAdd(g_MsgUart3Tx,pTI) == FALSE)
							{
									MemFree(pTI);	
							}
					}
					break;
				case INTERFACE_CAN1:
					{
						  pCan = (CCANItem *)(pTI->Info);
							pCan->CanParam.CANIndex = 0;
							if(MsgTx_QueueAdd(g_MsgCan0Tx,pTI) == FALSE)
							{
									MemFree(pTI);	
							}
					}
					break;
				case INTERFACE_CAN2:
					{
							pCan = (CCANItem *)(pTI->Info);
							pCan->CanParam.CANIndex = 1;
							if(MsgTx_QueueAdd(g_MsgCan1Tx,pTI) == FALSE)
							{
									MemFree(pTI);	
							}
					}
					break;
				default:
					break;	
			}
		}
		MsgTx_QueueProc();
		DataReSend_Proc(txbuf,sizeof(txbuf));
	}
}








