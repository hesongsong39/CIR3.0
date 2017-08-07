/**********************************************************************
* $Id$      LedControl              2016-07-13
*//**
* @file     LedControl.c
* @brief    面板显示灯控制相关程序
* @version  1.0.0
* @date     2016/07/13 星期三 10:28:44
* @author   ZhangYanMing
*
* Copyright(C) 2016, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
/**************************** Includes ********************************/
#include "lpc_types.h"
#include <ucos_ii.h>
#include <string.h>
#include "LedControl.h"
#include "LED.h"
#include "AppDefine.h"
#include "cmddefine.h"

static LightSparkCon  LedCan0Control;
static LightSparkCon  LedCan1Control;


/*********************************************************************//**
 * @brief			面板显示灯控制程序数据初始化
 * @param[in]	None
 * @return 		None
 **********************************************************************/
 void BoardLightData_Init(void)
 {
		memset(&LedCan0Control,0,sizeof(LedCan0Control));
		memset(&LedCan1Control,0,sizeof(LedCan1Control));
 }
 
 
/*********************************************************************//**
 * @brief			面板运行等控制程序
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void OS_at_Run(void)
{
	static uint8_t flag;
	
	flag++;
	
	if(flag & 0x01)
	{
		LED_On(LED_RUN);
	}
	else
	{
		LED_Off(LED_RUN);
	}
}
 
 
 /*********************************************************************//**
 * @brief			打开面板显示灯
 * @param[in]	num，灯编号
 * @return 		None
 **********************************************************************/
 void BoardLightOn(uint8_t num)
 {
		OS_CPU_SR  cpu_sr;
	 
		OS_ENTER_CRITICAL();
		switch(num)
		{
			case LED_CAN0:
				{
						LedCan0Control.LedFlag = TRUE;
				}
				break;
			case LED_CAN1:
				{
						LedCan1Control.LedFlag = TRUE;
				}
				break;
			default:
				break;
		}
		OS_EXIT_CRITICAL();

 }


/*********************************************************************//**
 * @brief			面板显示灯控制程序
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void BoardLightCtr_Handle(void)
{
			OS_CPU_SR  cpu_sr;
		static uint32_t  LightRunCnt = 0;
	
		//运行灯控制程序
		if((OSTimeGet() - LightRunCnt) >= LEDRUNTIMEOUT)
		{
				LightRunCnt = OSTimeGet();
				OS_at_Run();
		}
		
		//LED_CAN0控制程序
		OS_ENTER_CRITICAL();            
		if(LedCan0Control.LedFlag == TRUE)
		{
				if(LedCan0Control.lastupdate == 0)
				{
						LED_On(LED_CAN0);
						LedCan0Control.lastupdate = OSTimeGet();
				}
				else
				{
						if((LedCan0Control.lastupdate + LEDCAN0TIMEOUT) < OSTimeGet())
						{
								LED_Off(LED_CAN0);
								LedCan0Control.LedFlag = FALSE;
								LedCan0Control.lastupdate = 0;
						}
				}
		}
		else
		{
				if(PrimaryCAN_Get() == INTERFACE_CAN1)
				{
						LED_On(LED_CAN0);
						LED_Off(LED_CAN1);
				}
		}
		OS_EXIT_CRITICAL();          
		
		//LED_CAN0控制程序
		OS_ENTER_CRITICAL();             
		if(LedCan1Control.LedFlag == TRUE)
		{
				if(LedCan1Control.lastupdate == 0)
				{
						LED_On(LED_CAN1);
						LedCan1Control.lastupdate = OSTimeGet();
				}
				else
				{
						if((LedCan1Control.lastupdate + LEDCAN1TIMEOUT) < OSTimeGet())
						{
								LED_Off(LED_CAN1);
								LedCan1Control.LedFlag = FALSE;
								LedCan1Control.lastupdate = 0;
						}
				}
		}
		else
		{
				if(PrimaryCAN_Get() == INTERFACE_CAN2)
				{
						LED_On(LED_CAN1);
						LED_Off(LED_CAN0);
				}
		}
		OS_EXIT_CRITICAL();
}



