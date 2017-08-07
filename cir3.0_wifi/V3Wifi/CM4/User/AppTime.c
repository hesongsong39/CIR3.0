/**********************************************************************
* $Id$      AppTime              2016-05-31
*//**
* @file     AppTime.c
* @brief    ��ʱ����ز���
* @version  1.0.0
* @date     2016/05/31 ���ڶ� 14:28:44
* @author   ZhangYanMing
*
* Copyright(C) 2016, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
/**************************** Includes **********************************/
#include "lpc_types.h"
#include <ucos_ii.h>
#include "AppTime.h"

DS_TIME g_timer[TIMECNT];

/*********************************************************************//**
 * @brief      ��ʱ��ѯ
 * @param[in]  num      ��ʱ����
 * @param[in]  to  		  ��ʱʱ��
 * @return     None
 **********************************************************************/
void TimerRef(uint8_t num,uint32_t to)
{
	g_timer[num].lastupdate = OSTimeGet();	
	g_timer[num].timeout = to;
}

/*********************************************************************//**
 * @brief      ��ʱ����ʼ��
 * @param[in]  num      ��ʱ����
 * @return     None
 **********************************************************************/
void TimerInit(uint8_t num)
{
	g_timer[num].lastupdate = 0;		
	g_timer[num].timeout = 0;
}

