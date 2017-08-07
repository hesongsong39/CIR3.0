/**********************************************************************
* $Id$      Led              2015-12-16
*//**
* @file     Led.h
* @brief    灯控制程序头文件
* @version  1.0.0
* @date     2015/12/16 星期三 15:20:15
* @author   ZhangYanMing
*
* Copyright(C) 2015, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
#ifndef __LED_H
#define __LED_H
/************************** Includes **********************************/
#include "lpc_types.h"

/****************************Macro Definition *************************/

/************************** Function Interface ************************/
void     LED_Init(void);
void     LED_On(uint32_t num);
void     LED_Off(uint32_t num);



#endif 



