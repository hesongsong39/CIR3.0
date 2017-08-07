/*************************************************
  Copyright (C), 2016, 712.Co.,Ltd.
  File name:     AppDefine.h
  Author:        Version:   v1.0.01     Date: 16.03.16
  Description:   全局标识定义
  Others:         
  Function List:  
  1. 

  History:      
  1. xxx   16.03.16    v1.0
*************************************************/
#ifndef __APPDEFINE_H
#define __APPDEFINE_H

#include "lpc_types.h"
#include "CmdDefine.h"


//******************全局结构体******************//

#pragma push
#pragma pack(1)                               /* 1字节对齐 */

//应用层参数结构体
typedef struct
{
		uint8_t  SelfPort;//自身端口号
		uint8_t  SelfNum;//自身单元代号
		uint8_t  MasterCanBus;//CAN主、备总线号
		uint8_t  ControlPort;//主用控制单元端口号
		uint8_t  MMIPort;//主用MMI端口号
		uint8_t  WorkMode;//工作模式、调试模式、库检模式、升级模式
}_AppPara;


#pragma pop

extern _AppPara         g_ParaInfo;
extern _McuBusIInfo g_stMcuBusInfo;
void Comprehensive_Info_Handle(uint8_t *pdata);



#endif




