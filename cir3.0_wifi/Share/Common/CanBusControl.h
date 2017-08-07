/**********************************************************************
* $Id$      CanBusControl              2016-09-02
*//**
* @file     CanBusControl.h
* @brief    CAN主备总线控制头文件
* @version  1.0.0
* @date     2016/09/02 星期五 10:28:44
* @author   ZhangYanMing
*
* Copyright(C) 2016, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
#ifndef _CANBUSCONTROL_H_
#define _CANBUSCONTROL_H_
/**************************** Includes ********************************/
#include "lpc_types.h"



/************************ Config Macro Definition *********************/
//CIR3.0单元模块个数
#define  UNITNUMBER   17
//CAN总线状态判断周期,单位为秒
#define  CANJUDGEPERIOD    30


//单元模块编号
typedef enum 
{
		DEVICE_MMI1 = 0, 
		DEVICE_MMI2, 
		DEVICE_450M, 
		DEVICE_GPS, 
		DEVICE_RECORD,
		DEVICE_800MLBJ, 
		DEVICE_MCU1,  
    DEVICE_MCU2, 
		DEVICE_CONVERSE1, 
		DEVICE_CONVERSE2, 
		DEVICE_IFUNIT, 
		DEVICE_BATTERY, 
		DEVICE_WIFI,
		DEVICE_400MLBJ,
		DEVICE_SWITCH, 
		DEVICE_LTE1, 
		DEVICE_LTE2, 
}_DEVICE_INDEX;

#pragma push
#pragma pack(1)                               /* 1字节对齐 */

//点名结构体
typedef struct 
{
		uint8_t  port;	//端口号
		uint8_t  index; //单元设备索引号
		Bool Status_Can0;	//CAN0总线状态
		Bool Status_Can1;	//CAN1总线状态
		uint8_t  ReplyCnt_Can0; //Can0总线点名计数器
		uint8_t  ReplyCnt_Can1; //Can1总线点名计数器
} _BusCallControl;


#pragma pop


/************************** Function Interface ************************/
void CANBusControHandle(uint8_t cbCanChannel,uint8_t cbsport);
void CANbusControlPeriodHandle(uint8_t SelfPort);
void CANNormalCntGet(uint8_t *pCan0Normal,uint8_t *pCan1Normal);
Bool UnitConfigInfo_Get(uint8_t UnitPort);

#endif



