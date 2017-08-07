/**********************************************************************
* $Id$      App              2015-12-18
*//**
* @file     App.c
* @brief    应用层相关业务
* @version  1.0.0
* @date     2015/12/18 星期五 11:20:15
* @author   ZhangYanMing
*
* Copyright(C) 2015, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
/************************** Includes **********************************/
#include "lpc_types.h"
#include <ucos_ii.h>
#include <string.h>
#include "lpc43xx_gpio.h"
#include "App.h"
#include "DataReSend.h"
#include "cmddefine.h"
#include "CanRxpro.h"
#include "DataTransfer.h"
#include "LedControl.h"
#include "AppDefine.h"
#include "CanBusControl.h"
#include "cmddefine.h"

/*****************Private variables ***********************************/


/*****************Public variables ************************************/


/*****************Extern variables ************************************/



/*********************************************************************//**
 * @brief			应用层初始化
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void App_Init(void)
{

    //重发数据结构体初始化
    DataReSend_Init();
    //应用业务参数初始化
    AppPara_Init();
    //应用层CAN目的地址接收滤波消息对象初始化
    CanMessageInit();
    //控制灯相关数据初始化
    BoardLightData_Init();
    //CNA主备总线控制数据初始化
//    CANBusControlInit();
}

/*********************************************************************//**
 * @brief			硬件版本号读取
 * @param[in]	None
 * @return 		None
 **********************************************************************/
uint8_t HW_VerRead(void)
{
    uint8_t ver0=0;
    uint8_t ver1=0;
    uint8_t ver2=0;
    uint8_t ver3=0;

    uint8_t version=0;

    //读取电平值
    ver0 = ((GPIO_ReadValue(3)&(1<<5)) > 0)? 1 : 0;
    ver1 = ((GPIO_ReadValue(3)&(1<<6)) > 0)? 1 : 0;
    ver2 = ((GPIO_ReadValue(3)&(1<<7)) > 0)? 1 : 0;
    ver3 = ((GPIO_ReadValue(2)&(1<<8)) > 0)? 1 : 0;

    version = (ver0) | (ver1<<1) | (ver2<<2) | (ver3<<3);
    version = (~version) & 0x0F;
    version += 1;

    return version;
}


/*********************************************************************//**
 * @brief			读取硬件地址
 * @param[in]	None
 * @return 		None
 **********************************************************************/
uint8_t Unit_AddrRead(void)
{
    uint8_t addr=0,addr0=0,addr1=0,addr2=0,addr3=0;

    //读取电平值
    addr0 = ((GPIO_ReadValue(0)&(1<<5)) > 0)? 1 : 0;
    addr1 = ((GPIO_ReadValue(5)&(1<<15)) > 0)? 1 : 0;
    addr2 = ((GPIO_ReadValue(5)&(1<<16)) > 0)? 1 : 0;
    addr3 = ((GPIO_ReadValue(3)&(1<<4)) > 0)? 1 : 0;

    addr = (addr0) | (addr1 << 1) | (addr2 << 2)|(addr3 << 3);
    addr = (addr) & 0x0F;

    return addr;
}


/*********************************************************************//**
 * @brief			应用层通用参数初始化
 * @param[in]
 * @return 		None
 **********************************************************************/
void AppPara_Init(void)
{
    uint8_t addr = 0x00;

    addr = Unit_AddrRead();
    g_ParaInfo.MasterCanBus = INTERFACE_CAN1;
    //通过读取硬件地址线确定自身端口号、单元代号
    if(addr == HANRD_ADDR_WIFI)
    {
        g_ParaInfo.SelfPort = PORT_WIFI;
        g_ParaInfo.SelfNum = UNIT_CODE_WIFI_APP;
    }
    //通过自身端口号确定自身单元代号
    g_ParaInfo.ControlPort = PORT_MCU1;
    //主用MMI端口号，初始化默认为0x02
    g_ParaInfo.MMIPort = PORT_MMI1;
    //工作模式初始化为工作模式
    g_ParaInfo.WorkMode = MODE_WORK;
}



/*********************************************************************//**
 * @brief			应用层CAN消息对象目的地址滤波，
 * @param[in]
 * @return 		None
 **********************************************************************/
void CanMessageInit(void)
{
    uint8_t iCanAddr = 0,iNetAddr = 0;

    GetCanNetAddrByPort(PORT_BROADCAST,&iCanAddr,&iNetAddr);
    CCAN_RecMessageSet(0,iCanAddr,TRUE);
    CCAN_RecMessageSet(1,iCanAddr,TRUE);

//    GetCanNetAddrByPort(PORT_PUBLIC, &iCanAddr, &iNetAddr);
//    CCAN_RecMessageSet(0,iCanAddr,TRUE);
//    CCAN_RecMessageSet(1,iCanAddr,TRUE);

//    GetCanNetAddrByPort(PORT_MCU1, &iCanAddr, &iNetAddr);
//    CCAN_RecMessageSet(0,iCanAddr,TRUE);
//    CCAN_RecMessageSet(1,iCanAddr,TRUE);

//    GetCanNetAddrByPort(PORT_MCU2, &iCanAddr, &iNetAddr);
//    CCAN_RecMessageSet(0,iCanAddr,TRUE);
//    CCAN_RecMessageSet(1,iCanAddr,TRUE);

//    GetCanNetAddrByPort(PORT_MMI1, &iCanAddr, &iNetAddr);
//    CCAN_RecMessageSet(0,iCanAddr,TRUE);
//    CCAN_RecMessageSet(1,iCanAddr,TRUE);

    GetCanNetAddrByPort(g_ParaInfo.SelfPort,&iCanAddr,&iNetAddr);
    CCAN_RecMessageSet(0, iCanAddr,TRUE);
    CCAN_RecMessageSet(1, iCanAddr,TRUE);
}

/*********************************************************************//**
 * @brief		交换单元设备状态信息
 * @param[in]
 * @return 		None
 **********************************************************************/
Bool EquipmentSta(uint8_t chto)
{
    uint8_t txbuf[64] = {0};
	_SwitchDevInfo esta;
	
	esta.iUNCode= UNIT_CODE_WIFI_APP;
	esta.iCAN1State = CANStatusGet(0) == DEVICE_C_CAN_ACTIVE ? 0X54:0X46;
	esta.iCAN2State = CANStatusGet(1) == DEVICE_C_CAN_ACTIVE ? 0X54:0X46;
	esta.iNetState  = 0X54;
	esta.iBaseLen   = sizeof(esta) - sizeof(esta.iBaseLen);
    return CCANDataSimpleSendPost(txbuf,sizeof(txbuf),chto,OP_DEV,CMD_DEV_INFO,g_ParaInfo.SelfPort,PORT_BROADCAST,(uint8_t *)&esta,sizeof(esta),FALSE);
}