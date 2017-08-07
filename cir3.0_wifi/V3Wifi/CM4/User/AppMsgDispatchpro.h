/**********************************************************************
* $Id$      AppMsgDispatchpro              2015-12-23
*//**
* @file     AppMsgDispatchpro.h
* @brief    业务处理任务头文件
* @version  1.0.0
* @date     2015/12/23 星期三 14:28:44
* @author   ZhangYanMing
*
* Copyright(C) 2015, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
#ifndef _APPMSGDISPRO_H_
#define _APPMSGDISPRO_H_

#include "lpc_types.h"
#include <ucos_ii.h>
#include "Uart.h"
#include "DataTransfer.h"
#include "ptcir.h"

/***************************  Macro Definition ************************/





/************************** Function Interface ************************/
void  AppTaskMsgDispatchProc(void *p_arg);



#endif






