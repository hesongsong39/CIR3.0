/**********************************************************************
* $Id$      AppNetpro              2016-12-15
*//**
* @file     AppNetpro.c
* @brief    以太网处理任务头文件
* @version  1.0.0
* @date     2016/12/15
* @author   ZhangYanMing
*
* Copyright(C) 2016, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
#ifndef __APPNETPRO_H__
#define __APPNETPRO_H__
/**************************** Includes ********************************/
#include "ucos_ii.h"
#include "lpc_types.h"

/**************************** Macro Definition ************************/
#define UDPSENDPORT     30000
#define UDPMONITOR      10000

#define FLAG_ENET_OK 		1u<<1		/* 以太网EMAC收到数据帧 */
#define FLAG_SEND_OK		1u<<2		/* 要发送以太网数据 */


#pragma push
#pragma pack(1)                               /* 1字节对齐 */

typedef struct
{
	uint8_t type;		
	uint8_t socket;	
	uint16_t localport;	
	uint8_t remip[4];
	uint16_t remport;
}Ethernet_Param_Type;

typedef struct
{
	Ethernet_Param_Type  EthernetParam;
	uint16_t Infolen;			//信息长度
	uint8_t  Info[1];			//信息指针
}Ethernet_Info_Type;

#pragma pop


extern OS_FLAG_GRP *NETFlag;

/************************** Function Interface ************************/
void  AppTaskNetProc (void *p_arg);

#endif 




