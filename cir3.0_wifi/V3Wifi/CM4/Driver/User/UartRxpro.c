/**********************************************************************
* $Id$      UartRxpro              2015-12-23
*//**
* @file     UartRxpro.c
* @brief    串口接收数据处理
* @version  1.0.0
* @date     2015/12/23 星期三 14:28:44
* @author   ZhangYanMing
*
* Copyright(C) 2015, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
/**************************** Includes **********************************/
#include "lpc_types.h"
#include <ucos_ii.h>
#include <stdio.h> 
#include <string.h>
#include "Task.h"
#include "DataTransfer.h"
#include "Uart.h"
#include "UartRxpro.h"
#include "DriverConfig.h"
#include "AppTime.h"


/*****************Private variables ***********************************/

#ifdef _UART0
static UartInterface_Type g_Uart0Interface;
#endif

#ifdef _UART1
static UartInterface_Type g_Uart1Interface;
#endif

#ifdef _UART2
static UartInterface_Type g_Uart2Interface;
#endif

#ifdef _UART3
static UartInterface_Type g_Uart3Interface;
#endif


/*****************Public variables ************************************/


/*****************Extern variables ************************************/


/********************************************************************//**
 * @brief 		UART receive Frame function (Queue used)
 * @param[in]	None
 * @return 		None
 *********************************************************************/
void UART_Data_RevFmtProc(UartInterface_Type *pUI,uint8_t *RxData,uint8_t rxlen)
{
			if((g_timer[T_RXGD].lastupdate + g_timer[T_RXGD].timeout) > OSTimeGet())
			{
					*(pUI->RecvBuf + (pUI->RecvCount++)) = *RxData;
					if (pUI->RecvCount >= sizeof(pUI->RecvBuf))		
					{
							MQ_AddTail(g_MsgDispatch,pUI->Num,pUI->RecvBuf,pUI->RecvCount);
							pUI->RecvCount = 0;
					}
					TimerRef(T_RXGD, UARTTIMEOUT);
			}
			else								
			{
					pUI->RecvCount = 0;
					*(pUI->RecvBuf + (pUI->RecvCount++)) = *RxData;
					TimerRef(T_RXGD, UARTTIMEOUT);
			}
}





static UartInterface_Type *GetpUartbyNum(uint8_t uartnum)
{
	UartInterface_Type *pUI = NULL;
	switch(uartnum)
	{
		case 0:
			{
				#ifdef _UART0
					pUI =  &g_Uart0Interface;
				#endif
			}
			break;
		case 1:
			{
				#ifdef _UART1
					pUI =  &g_Uart1Interface;
				#endif
			}
			break;
		case 2:
			{
				#ifdef _UART2
					pUI =  &g_Uart2Interface;
				#endif
			}
			break;
		case 3:
			{
				#ifdef _UART3
					pUI =  &g_Uart3Interface;
				#endif
			}
			break;
		default:
			break;
	}

	return pUI;
}



/*********************************************************************//**
 * @brief       串口数据队列处理函数
 * @param[in]   None
 * @return      None
 **********************************************************************/
void Uart_QueueProc(void)
{
		
		#ifdef _UART0
			UART0_QueueProc();
		#endif
		
		#ifdef _UART1
			UART1_QueueProc();
		#endif
	
		#ifdef _UART2
			UART2_QueueProc();
		#endif
		
		#ifdef _UART3
			UART3_QueueProc();
		#endif
	
		
}


/********************************************************************//**
 * @brief 		UART1 receive Queue function 
 * @param[in]	None
 * @return 		None
 *********************************************************************/
void UART1_QueueProc(void)
{
		uint8_t rxdata = 0;
		UartInterface *pUI = NULL;
		UartInterface_Type *pUartType = NULL;
	
		pUI = GetpUIbyNum(1);
		pUartType = GetpUartbyNum(1);
		//查询队列中是否有数据
		while(pUI->rpRead != pUI->rpWrite)
		{
				rxdata = pUI->RecBuf[pUI->rpRead++];
				if(pUI->rpRead >= pUI->RecBufSize)
				{
					pUI->rpRead = 0;
				}
				UART_Data_RevFmtProc(pUartType,&rxdata,1);
		}
		//超时判断
		if (g_timer[T_RXGD].lastupdate != 0)		
		{
			if ((g_timer[T_RXGD].lastupdate + g_timer[T_RXGD].timeout) <= OSTimeGet())
			{
					MQ_AddTail(g_MsgDispatch,pUartType->Num,pUartType->RecvBuf,pUartType->RecvCount);
					pUartType->RecvCount = 0;
					TimerInit(T_RXGD);					
			}
		}
}

/********************************************************************//**
 * @brief 		UART0 receive Queue function 
 * @param[in]	None
 * @return 		None
 *********************************************************************/
void UART0_QueueProc(void)
{
		
}

/********************************************************************//**
 * @brief 		UART3 receive Queue function 
 * @param[in]	None
 * @return 		None
 *********************************************************************/
void UART3_QueueProc(void)
{
		
}


/********************************************************************//**
 * @brief 		串口板级层相关初始化
 * @param[in]	None
 * @return 		None
 *********************************************************************/
void Board_Uart_Init(void)
{
	#ifdef _UART0
		g_Uart0Interface.Num = INTERFACE_COM0;
		g_Uart0Interface.FrameState = 0;
		g_Uart0Interface.RecvCount = 0;
		memset(g_Uart0Interface.RecvBuf,0,sizeof(g_Uart0Interface.RecvBuf));
	#endif
	
	#ifdef _UART1
		g_Uart1Interface.Num = INTERFACE_COM1;
		g_Uart1Interface.FrameState = 0;
		g_Uart1Interface.RecvCount = 0;
		memset(g_Uart1Interface.RecvBuf,0,sizeof(g_Uart1Interface.RecvBuf));
	#endif
	
	#ifdef _UART2
		g_Uart2Interface.Num = INTERFACE_COM2;
		g_Uart2Interface.FrameState = 0;
		g_Uart2Interface.RecvCount = 0;
		memset(g_Uart2Interface.RecvBuf,0,sizeof(g_Uart2Interface.RecvBuf));
	#endif
	
	#ifdef _UART3
		g_Uart3Interface.Num = INTERFACE_COM3;
		g_Uart3Interface.FrameState = 0;
		g_Uart3Interface.RecvCount = 0;
		memset(g_Uart3Interface.RecvBuf,0,sizeof(g_Uart3Interface.RecvBuf));
	#endif
	
}
