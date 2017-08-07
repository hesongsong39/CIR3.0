/**********************************************************************
* $Id$      AppMsgTxpro              2015-12-23
*//**
* @file     AppMsgTxpro.h
* @brief    ���ݷ�������
* @version  1.0.0
* @date     2015/12/23 ������ 14:28:44
* @author   ZhangYanMing
*
* Copyright(C) 2015, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
#ifndef _APPTXPRO_H_
#define _APPTXPRO_H_
/**************************** Includes **********************************/
#include "lpc_types.h"

#pragma push
#pragma pack(1)                               /* 1�ֽڶ��� */

typedef struct
{
		Bool  IsBusy;
		void  *pbuf;
}MsgTx_Queue_Status;

#pragma pop


/************************** Function Interface ************************/
void  AppTaskMsgTxProc(void *p_arg);



#endif


