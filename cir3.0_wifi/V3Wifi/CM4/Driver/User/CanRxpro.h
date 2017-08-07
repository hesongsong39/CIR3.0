/**********************************************************************
* $Id$      CanRxpro             2016-01-07
*//**
* @file     CanRxpro.h
* @brief    串口接收数据处理头文件
* @version  1.0.0
* @date     2016/01/07 星期三 14:28:44
* @author   ZhangYanMing
*
* Copyright(C) 2016, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
#ifndef _CANRXPRO_H_
#define _CANRXPRO_H_
/**************************** Includes **********************************/
#include "lpc_types.h"

/******************************** Micro ********************************/

/************************** Function Interface ************************/
void Board_Can_Init(void);
void CCAN_QueueProc(void);



#endif






