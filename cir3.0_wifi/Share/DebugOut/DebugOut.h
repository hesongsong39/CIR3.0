/**********************************************************************
* $Id$      DebugOut              2016-03-11
*//**
* @file     DebugOut.h
* @brief    ������Ϣ���ͷ�ļ�
* @version  1.0.0
* @date     2016/03/11 ������ 08:20:15
* @author   ZhangYanMing
*
* Copyright(C) 2016, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
/************************** Includes **********************************/
#ifndef _DEBUGOUT_H_
#define _DEBUGOUT_H_

/************************** Includes **********************************/
#include "lpc_types.h"


/************************** Macro definition  *************************/
#define OUTPUT_MAXSIZE    128     /* �����������󻺳� */


/************************** Function Interface ************************/
void Debug_Output_CAN(uint8_t chto,uint8_t sport,const  char *format, ...);
void Debug_Output_Ethernet(uint8_t sport,const  char *format, ...);

#endif








