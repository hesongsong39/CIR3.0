/**********************************************************************
* $Id$      CanBusControl              2016-09-02
*//**
* @file     CanBusControl.c
* @brief    CAN主备总线控制
* @version  1.0.0
* @date     2016/09/02 星期五 10:28:44
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
#include "CanBusControl.h"
#include "C_CAN.h"
#include "CmdDefine.h"
#include "DataTransfer.h"

/*****************Private variables ***********************************/

static _BusCallControl  g_CanBusInfo[UNITNUMBER] = 
{  
		{PORT_MMI1,DEVICE_MMI1,1,1,0,0},
		{PORT_MMI2,DEVICE_MMI2,1,1,0,0},
		{PORT_450M,DEVICE_450M,1,1,0,0},
		{PORT_GPS, DEVICE_GPS,1,1,0,0},
		{PORT_RECORD,DEVICE_RECORD,1,1,0,0},
		{PORT_800M,DEVICE_800MLBJ,1,1,0,0},
		{PORT_MCU1,DEVICE_MCU1,1,1,0,0},
		{PORT_MCU2, DEVICE_MCU2,1,1,0,0},
		{PORT_CONVERSE1,DEVICE_CONVERSE1,1,1,0,0},
		{PORT_CONVERSE2,DEVICE_CONVERSE2,1,1,0,0},
		{PORT_IFUNIT,DEVICE_IFUNIT,1,1,0,0},
		{PORT_BATTERY, DEVICE_BATTERY,1,1,0,0},
		{PORT_WIFI,DEVICE_WIFI,1,1,0,0},
		{PORT_400M,DEVICE_400MLBJ,1,1,0,0},
		{PORT_SWITCH,DEVICE_SWITCH,1,1,0,0},
		{PORT_LTE1, DEVICE_LTE1,1,1,0,0},
		{PORT_LTE2, DEVICE_LTE2,1,1,0,0},
};
/*****************Public variables ************************************/


/*****************Extern variables ************************************/

/*********************************************************************//**
 * @brief			单元判断CAN总线通信状态信息
 * @param[in]	None
 * @return 		None
 **********************************************************************/
static void CANBusControlJudge(uint8_t SelfPort)
{
		uint8_t  i = 0;
		OSSchedLock();            /* 关调度保护 */
		for(i=0;i<(sizeof(g_CanBusInfo)/sizeof(g_CanBusInfo[0]));i++)
		{
				if(g_CanBusInfo[i].port == SelfPort)
				{
						//CAN0总线状态判断
						if(CANStatusGet(0) == DEVICE_C_CAN_ACTIVE)
						{
								g_CanBusInfo[i].Status_Can0 = TRUE;
						}
						else
						{
								g_CanBusInfo[i].Status_Can0 = FALSE;
						}
						//CAN1总线状态判断
						if(CANStatusGet(1) == DEVICE_C_CAN_ACTIVE)
						{
								g_CanBusInfo[i].Status_Can1 = TRUE;
						}
						else
						{
								g_CanBusInfo[i].Status_Can1 = FALSE;
						}
				}
				else
				{
						//CAN0总线状态判断
						if(g_CanBusInfo[i].ReplyCnt_Can0)
						{
								g_CanBusInfo[i].Status_Can0 = TRUE;
						}
						else
						{
								g_CanBusInfo[i].Status_Can0 = FALSE;
						}
						//CAN1总线状态判断
						if(g_CanBusInfo[i].ReplyCnt_Can1)
						{
								g_CanBusInfo[i].Status_Can1 = TRUE;
						}
						else
						{
								g_CanBusInfo[i].Status_Can1 = FALSE;
						}
						//计数器清零
						g_CanBusInfo[i].ReplyCnt_Can0 = 0;
						g_CanBusInfo[i].ReplyCnt_Can1 = 0;
				}
		}
		OSSchedUnlock();          /* 解除关调度保护 */
}

/*********************************************************************//**
 * @brief			CAN总线通信状态信息处理
 * @param[in]	cbCanChannel,can号
							cbsport,源端口号
 * @return 		None
 **********************************************************************/
void CANBusControHandle(uint8_t cbCanChannel,uint8_t cbsport)
{
		uint8_t  i = 0;
		OSSchedLock();            /* 关调度保护 */
		for(i=0;i<(sizeof(g_CanBusInfo)/sizeof(g_CanBusInfo[0]));i++)
		{
				if(cbsport == g_CanBusInfo[i].port)
				{
						switch(cbCanChannel)
						{
							case INTERFACE_CAN1:
								{
										g_CanBusInfo[i].Status_Can0 = TRUE;
										g_CanBusInfo[i].ReplyCnt_Can0++;
								}
								break;
							case INTERFACE_CAN2:
								{
										g_CanBusInfo[i].Status_Can1 = TRUE;
										g_CanBusInfo[i].ReplyCnt_Can1++;
								}
								break;
							default:
								break;
						}
						break;//找到自身端口号后结束查找
				}
		}
		OSSchedUnlock();          /* 解除关调度保护 */
}

/*********************************************************************//**
 * @brief			单元CAN总线控制处理
 * @param[in]	
 * @return 		None
 **********************************************************************/
void CANbusControlPeriodHandle(uint8_t SelfPort)
{
		static uint32_t CANJudgePeriod = 0;//CAN总线状态判断周期计数器
		//CAN总线状态判断处理
		if((OSTimeGet()- CANJudgePeriod) >= CANJUDGEPERIOD*1000 )
		{
				//更新查询时间计数器
				CANJudgePeriod = OSTimeGet();
				CANBusControlJudge(SelfPort);
		}
}



/*********************************************************************//**
 * @brief			获取CAN总线功能好的单元号
 * @param[in]	
 * @return 		None
 **********************************************************************/
void CANNormalCntGet(uint8_t *pCan0Normal,uint8_t *pCan1Normal)
{
		uint8_t  i = 0;
		if((pCan0Normal == NULL)||(pCan1Normal == NULL))
		{
				return ;
		}
		OSSchedLock();            /* 关调度保护 */
		for(i=0;i<(sizeof(g_CanBusInfo)/sizeof(g_CanBusInfo[0]));i++)
		{
				if(g_CanBusInfo[i].Status_Can0 == TRUE)
				{
						(*pCan0Normal)++;
				}
				if(g_CanBusInfo[i].Status_Can1 == TRUE)
				{
						(*pCan1Normal)++;
				}
		}
		OSSchedUnlock();          /* 解除关调度保护 */
}


/*********************************************************************//**
 * @brief			获取单元配置信息
 * @param[in]	UnitPort,端口号
 * @return 		None
 **********************************************************************/
Bool UnitConfigInfo_Get(uint8_t UnitPort)
{
		Bool ret = FALSE;
		uint8_t  i = 0;
		OSSchedLock();            /* 关调度保护 */
		for(i=0;i<(sizeof(g_CanBusInfo)/sizeof(g_CanBusInfo[0]));i++)
		{
				if(g_CanBusInfo[i].port == UnitPort)
				{
						if((g_CanBusInfo[i].Status_Can0 == TRUE)||(g_CanBusInfo[i].Status_Can1 == TRUE))
						{
								ret = TRUE;
						}
				}
		}
		OSSchedUnlock();          /* 解除关调度保护 */
		
		return ret;
}



