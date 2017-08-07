/**********************************************************************
* $Id$      AppNetpro              2016-12-15
*//**
* @file     AppNetpro.c
* @brief    以太网处理任务
* @version  1.0.0
* @date     2016/12/15
* @author   ZhangYanMing
*
* Copyright(C) 2016, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
/**************************** Includes ********************************/
#include <string.h>
#include <ucos_ii.h>
#include "lpc43xx_timer.h"
#include "AppNetpro.h"
#include "Net_config.h"
#include "task.h"
#include "DataTransfer.h"
#include "EthernetUdp.h"
#include "MemManage.h"
#include "App.h"
#include "EthernetSet.h"
#include "AppDefine.h"


/*****************Private variables ***********************************/


/*****************Public variables ************************************/
OS_FLAG_GRP *NETFlag;

/*****************Extern variables ************************************/


/*********************************************************************//**
 * @brief       TIMER0 interrupt handler sub-routine
 * @param[in]   None
 * @return      None
 **********************************************************************/
void TIMER0_IRQHandler(void)
{
    OS_CPU_SR  cpu_sr;

    OS_ENTER_CRITICAL();              /* Tell uC/OS-II that we are starting an ISR          */
    OSIntEnter();
    OS_EXIT_CRITICAL();

    TIM_ClearIntPending(LPC_TIMER0, TIM_MR0_INT);  // 清除中断标记

    timer_tick();

    OSIntExit();                      /* Tell uC/OS-II that we are leaving the ISR          */
}

/*********************************************************************//**
 * @brief			定时器0初始化
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void Timer0_Init(void)
{
    TIM_TIMERCFG_Type TIM_ConfigStruct;
    TIM_MATCHCFG_Type TIM_MatchConfigStruct;

    // Initialize timer 0, prescale count time of 1
    TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_TICKVAL;
    TIM_ConfigStruct.PrescaleValue	= 1;

    // use channel 0, MR0
    TIM_MatchConfigStruct.MatchChannel = 0;
    // Enable interrupt when MR0 matches the value in TC register
    TIM_MatchConfigStruct.IntOnMatch   = TRUE;
    //Enable reset on MR0: TIMER will reset if MR0 matches it
    TIM_MatchConfigStruct.ResetOnMatch = TRUE;
    //Stop on MR0 if MR0 matches it
    TIM_MatchConfigStruct.StopOnMatch  = FALSE;
    //Toggle MR0.0 pin if MR0 matches it
    TIM_MatchConfigStruct.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;
    // Set Match value,100ms
    TIM_MatchConfigStruct.MatchValue = CGU_GetPCLKFrequency(CGU_PERIPHERAL_TIMER0) / 10;

    // Set configuration for Tim_config and Tim_MatchConfig
    TIM_Init(LPC_TIMER0, TIM_TIMER_MODE, &TIM_ConfigStruct);
    TIM_ConfigMatch(LPC_TIMER0, &TIM_MatchConfigStruct);

    /* preemption = 0, sub-priority = 0 */
    NVIC_SetPriority(TIMER0_IRQn, ((0x01 << 3) | 0x02));
    /* Enable interrupt for timer 0 */
    NVIC_EnableIRQ(TIMER0_IRQn);
    // To start timer 0
    TIM_Cmd(LPC_TIMER0, ENABLE);
}


/*********************************************************************//**
 * @brief		以太网数据发送接口
 * @param[out]	data        ：需要发送的数据
 * @param[in]	  datalen     ：发送的数据长度
 * @param[in]	  type        ：数据类型
 * @param[in]	  remip       ：远端机IP地址
 * @param[in]	  port        ：端口号
 * @param[in]	  socket      ：套接字号
 * @return 		TRUE/FALSE
 **********************************************************************/
static Bool EthernetDataSend(uint8_t *data, uint16_t datalen,uint8_t type,uint8_t *remip,uint16_t port,uint8_t socket)
{
    uint8_t err = 0;
    Ethernet_Info_Type *pTI = NULL;

    pTI = (Ethernet_Info_Type *)MemApply(sizeof(Ethernet_Param_Type) + 2 + datalen);
    if(pTI == NULL)
    {
        return FALSE;
    }

    //Ethernet参数结构体赋值
    pTI->EthernetParam.type = type;
    pTI->EthernetParam.socket = socket;
    pTI->EthernetParam.localport = 0;
    memcpy(pTI->EthernetParam.remip,remip,sizeof(pTI->EthernetParam.remip));
    pTI->EthernetParam.remport = port;
    pTI->Infolen = datalen;
    memcpy(pTI->Info,data,datalen);

    if(OSQPost(g_NetTXMsgQ, pTI) != OS_ERR_NONE)
    {
        MemFree(pTI);
        return FALSE;
    }

    OSFlagPost(NETFlag, FLAG_SEND_OK, OS_FLAG_SET, &err);

    return TRUE;
}

/*********************************************************************//**
 * @brief		以太网接收数据投递
 * @param[in]	  type        ：数据类型
 * @param[in]	  remip       ：远端机IP地址
 * @param[in]	  port        ：端口号
 * @param[out]	data        ：需要发送的数据
 * @param[in]	  datalen     ：发送的数据长度
 * @return 		TRUE/FALSE
 **********************************************************************/
static Bool EthernetRevDataPost(uint8_t type,uint8_t *remip,uint16_t port,uint16_t localport,uint8_t *data, uint16_t datalen)
{
    TranItem *pTItem = NULL;
    Ethernet_Info_Type *pTI = NULL;

    pTItem = (TranItem *)MemApply(sizeof(Ethernet_Param_Type) + 2 + datalen + 3);
    if(pTItem == NULL)
    {
        return FALSE;
    }

    pTItem->chFrom = INTERFACE_LAN;
    pTItem->Infolen = sizeof(Ethernet_Param_Type) + 2 + datalen;

    pTI = (Ethernet_Info_Type *)(pTItem->Info);
    //Ethernet参数结构体赋值
    pTI->EthernetParam.type = type;
    pTI->EthernetParam.socket = 0;
    pTI->EthernetParam.localport = localport;
    memcpy(pTI->EthernetParam.remip,remip,sizeof(pTI->EthernetParam.remip));
    pTI->EthernetParam.remport = port;
    pTI->Infolen = datalen;
    memcpy(pTI->Info,data,datalen);

    if(OSQPost(g_MsgDispatch, pTItem) != OS_ERR_NONE)
    {
        MemFree(pTItem);
        return FALSE;
    }
    return TRUE;
}



/*********************************************************************//**
 * @brief		以太网发送队列处理
 * @param[in]	None
 * @return 		None
 **********************************************************************/
static void NetSendProc(void)
{
    uint8_t err = 0;
    Ethernet_Info_Type *pTI = NULL;

    while(1)
    {
        pTI = (Ethernet_Info_Type *)OSQAccept(g_NetTXMsgQ, &err);
        if(pTI)
        {
            switch(pTI->EthernetParam.type)
            {
            case ETHERNET_UDP:
            {
                UDP_SendData(pTI->EthernetParam.socket,pTI->EthernetParam.remip,pTI->EthernetParam.remport,pTI->Info,pTI->Infolen);
            }
            break;
            default:
                break;
            }
            MemFree(pTI);
        }
        else
        {
            break;
        }
    }
}



/*********************************************************************//**
 * @brief			UDP Task
 * @param[in]	p_arg   is the argument passed to 'App_TaskUDP()' by 'OSTaskCreate()'
 * @return 		None
 **********************************************************************/
void  AppTaskNetProc (void *p_arg)
{
    uint8_t err = 0;
    uint32_t flag = 0;
    uint8_t error;
    uint8_t IP = 0;
    uint8_t mac6 ;
    char HostName[16] = {0};
    uint16_t WifiPort;
    uint8_t addr = Unit_AddrRead();
    memset(HostName,0,sizeof(HostName));

    //通过读取硬件地址线确定自身端口号、单元代号
    if(addr == HANRD_ADDR_WIFI)
    {
		uint8_t igmp_add[]={238,0,100,1};
        GetUnitJPByPort(PORT_WIFI,HostName);
        GetNetAddrByPort(PORT_WIFI,&IP);
        GetMacAddrByPort(PORT_WIFI,&mac6);
//        SetMacAddr(NETADDR_MAC1,NETADDR_MAC2,NETADDR_MAC3,NETADDR_MAC4,NETADDR_MAC5,mac6);
        SetHostName(HostName,strlen(HostName));
        SetIpAdr(NETADDR_IP1,NETADDR_IP2,NETADDR_IP3,IP);
    }

    NETFlag = OSFlagCreate(0, &err);
    //以太网UDP数据结构初始化
    UDP_DataInit();
    //初始化TcpNet系统
    init_TcpNet ();
    //定时器0初始化,定时中断调用timer_tick
    Timer0_Init();

    GetNetPortByPort(g_ParaInfo.SelfPort,&WifiPort);
    //初始化接收、发送端口
    UDP_ReceivePort_Init(WifiPort,EthernetRevDataPost);
    UDP_SendPort_Init(20001,EthernetDataSend);

    while(1)
    {
        //以太网收到数据或者有数据要发送
        flag = OSFlagPend(NETFlag,FLAG_ENET_OK|FLAG_SEND_OK,OS_FLAG_WAIT_SET_ANY|OS_FLAG_CONSUME,20,&err);

        //以太网收数、超时处理
        if((flag & FLAG_ENET_OK)||(err == OS_ERR_TIMEOUT))
        {
            while (main_TcpNet() == __TRUE);
        }

        //以太网发数队列处理
        if(flag & FLAG_SEND_OK)
        {
            NetSendProc();
        }
    }
}

/* --------------------------------- End Of File ------------------------------ */


