/**********************************************************************
* $Id$      UartRxpro              2015-12-23
*//**
* @file     UartRxpro.h
* @brief    串口接收数据处理头文件
* @version  1.0.0
* @date     2015/12/23 星期三 14:28:44
* @author   ZhangYanMing
*
* Copyright(C) 2015, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
#ifndef _UARTRXPRO_H_
#define _UARTRXPRO_H_
/**************************** Includes **********************************/
#include "lpc_types.h"

/************************ Config Macro Definition *********************/
#define  UART_BUFFER_SIZE  1024
#define  UARTTIMEOUT       50
/***************************UartInterface structure********************/

#pragma push
#pragma pack(1)                               /* 1字节对齐 */

typedef struct
{
	uint8_t  Num;																	//端口号
  uint8_t  FrameState; 			
	uint16_t RecvCount;														// 接收到数据的数量 
  uint8_t  RecvBuf[UART_BUFFER_SIZE];		// 接收缓冲区
}UartInterface_Type;

#pragma pop


/************************** Function Interface ************************/
void  Uart_QueueProc(void);
void  Board_Uart_Init(void);
void  callback_DataReceive(void *ptmr, void *parg);
void UART0_QueueProc(void);
void UART1_QueueProc(void);
void UART3_QueueProc(void);


#endif






