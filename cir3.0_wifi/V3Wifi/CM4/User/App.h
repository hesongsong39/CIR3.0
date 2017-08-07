/**********************************************************************
* $Id$      App              2015-12-18
*//**
* @file     App.c
* @brief    Ӧ�ò����ҵ��
* @version  1.0.0
* @date     2015/12/18 ������ 11:20:15
* @author   ZhangYanMing
*
* Copyright(C) 2015, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
#ifndef __APP__
#define __APP__
/************************** Includes **********************************/
#include "lpc_types.h"




#pragma push
#pragma pack(1)                               /* 1�ֽڶ��� */



#pragma pop



/************************** Function Interface ************************/
void App_Init(void);	
void AppPara_Init(void);
void CanMessageInit(void);
uint8_t Unit_AddrRead(void);
uint8_t HW_VerRead(void);


#endif 


