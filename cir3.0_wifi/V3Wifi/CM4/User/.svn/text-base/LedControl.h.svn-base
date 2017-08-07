/**********************************************************************
* $Id$      LedControl              2016-07-13
*//**
* @file     LedControl.h
* @brief    面板显示灯控制相关程序头文件
* @version  1.0.0
* @date     2016/07/13 星期三 10:28:44
* @author   ZhangYanMing
*
* Copyright(C) 2016, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
#ifndef _LEDCONTROL_H_
#define _LEDCONTROL_H_
/**************************** Includes ********************************/
#include "lpc_types.h"



/************************ Config Macro Definition *********************/
//面板显示灯编号
#define LED_RUN				0
#define LED_CAN0			1
#define LED_VOICE			2   //面板语音灯
#define LED_CAN1			3
#define LED_DATA			4   //面板数据灯
#define LED_COM				5		//面板COM灯


//面板显示灯亮时间，单位毫秒
#define LEDRUNTIMEOUT   500
#define LEDCAN0TIMEOUT  50
#define LEDCAN1TIMEOUT  50

#pragma push
#pragma pack(1)                               /* 1字节对齐 */

typedef struct
{
		Bool  LedFlag;
		uint32_t  lastupdate;
}LightSparkCon;

#pragma pop


/************************** Function Interface ************************/
void BoardLightData_Init(void);
void BoardLightOn(uint8_t num);
void BoardLightCtr_Handle(void);

#endif




