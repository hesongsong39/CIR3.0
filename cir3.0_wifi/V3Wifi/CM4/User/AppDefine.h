/*************************************************
  Copyright (C), 2016, 712.Co.,Ltd.
  File name:     AppDefine.h
  Author:        Version:   v1.0.01     Date: 16.03.16
  Description:   ȫ�ֱ�ʶ����
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


//******************ȫ�ֽṹ��******************//

#pragma push
#pragma pack(1)                               /* 1�ֽڶ��� */

//Ӧ�ò�����ṹ��
typedef struct
{
		uint8_t  SelfPort;//����˿ں�
		uint8_t  SelfNum;//����Ԫ����
		uint8_t  MasterCanBus;//CAN���������ߺ�
		uint8_t  ControlPort;//���ÿ��Ƶ�Ԫ�˿ں�
		uint8_t  MMIPort;//����MMI�˿ں�
		uint8_t  WorkMode;//����ģʽ������ģʽ�����ģʽ������ģʽ
}_AppPara;


#pragma pop

extern _AppPara         g_ParaInfo;
extern _McuBusIInfo g_stMcuBusInfo;
void Comprehensive_Info_Handle(uint8_t *pdata);



#endif




