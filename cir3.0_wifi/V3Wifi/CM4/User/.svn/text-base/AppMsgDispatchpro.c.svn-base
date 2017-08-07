/**********************************************************************
* $Id$      AppMsgDispatchpro              2015-12-23
*//**
* @file     AppMsgDispatchpro.c
* @brief    业务处理任务
* @version  1.0.0
* @date     2015/12/23 星期三 14:28:44
* @author   ZhangYanMing
*
* Copyright(C) 2015, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
/**************************** Includes **********************************/
#include "lpc_types.h"
#include <ucos_ii.h>
#include <string.h>
#include "Task.h"
#include "App.h"
#include "MemManage.h"
#include "AppMsgDispatchpro.h"
#include "ptcir.h"
#include "DataReSend.h"
#include "Version.h"
#include "DataTransfer.h"
#include "audio.h"
#include "AppAudiopro.h"
#include "cmddefine.h"
#include "AppDefine.h"
#include "CanBusControl.h"
#include "AppNetpro.h"
#include "EthernetUdp.h"
#include "AppWifi.h"
#include "AppDefine.h"
#include "update.h"
/*****************Private variables ***********************************/
static uint8_t  g_rxbuf[1024] = {0};
static Tran_ptCIR_Frame  g_TF;
/*****************Public variables ************************************/

void RouteTest(uint8_t chto, Tran_ptCIR_Frame *pTF);
/*****************Extern variables ************************************/

/*********************************************************************//**
 * @brief			软件版本号查询
 * @param[in]
 * @return 		None
 **********************************************************************/
void Version_Query_Handle(uint8_t chto,uint8_t dport)
{
    uint8_t txbuf[64] = {0};
    char tVerData[36] = {0};
    char  WIFIVersion[24] = "V3.0.02";

    memset(tVerData,0,sizeof(tVerData));
    //单元代号
    tVerData[0] = CIR_FACTORY_712;
	tVerData[1] = g_ParaInfo.SelfNum;
    GetSoftwareVersion(&tVerData[2],sizeof(tVerData) - 2,WIFIVersion,strlen(WIFIVersion));

    //广播发送出去
    CCANDataSimpleSendPost(txbuf,sizeof(txbuf),chto,OP_MANAGE,CMD_MANAGE_VER_SOFT,g_ParaInfo.SelfPort,dport,(uint8_t *)tVerData,strlen(&tVerData[2]) + 2,TRUE);
}


/*********************************************************************//**
 * @brief			OpSwitchMachine_Handle  开关机业务处理
 * @param[in]
 * @return 		None
 **********************************************************************/
Bool OpSwitchMachine_Handle(uint8_t chto, Tran_ptCIR_Frame *pTF)
{
//    Bool  ret = FALSE;
//    uint8_t  txbuf[64] = {0};

//    switch(pTF->iCmd)
//    {
//    case CMD_POWER_ON:
//    {

//    }
//    break;
//    case CMD_POWER_OFF:
//    {
//        uint8_t data = 0x01;

//        ret = CCANDataSimpleSendPost(txbuf,sizeof(txbuf),chto,pTF->iOp,CMD_POWER_OFF_ACK, g_ParaInfo.SelfPort, pTF->iSport,&data, 1, FALSE);
//    }
//    break;
//    default:
//        break;
//    }

//    return ret;
}

/*********************************************************************//**
 * @brief			GSMR数据单元处理
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void Uart_Data_Handle(TranItem *pTI)
{
}

/*********************************************************************//**
 * @brief			CAN数据总线主业务处理函数
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void CCANHost_Handle(Tran_ptCIR_Frame *pTF,TranItem *pTI)
{
    CCANItem *pCan = (CCANItem *)(pTI->Info);
    //报文解析
    if(ParseptCIRFrame(pTF, pCan->Info, pCan->Infolen) == FALSE)
    {
		__NOP();
        return;
    }

    //匹配通用应答
    if(pTF->iCmd == CMDACK)
    {
        WAE_CheckPair(pTF->iOp, pTF->iCmd, pTF->pData, pTF->iDataLen);
        return;
    }

    if(pTF->iOp == OP_UPDATE)
    {
       UpdateDataIn(pTF,pTI->chFrom);
    }


    //--------------------------------------广播/点播------------------------------------
    switch(pTF->iOp)
    {
    case OP_MANAGE:
    {
		//软件版本查询
        if(pTF->iCmd == CMD_MANAGE_VER_QUE_SOFT)
        {
            Version_Query_Handle(pTI->chFrom,pTF->iSport);
        }
    }
    break;
    case OP_MANAGE712:
    {
        if(pTF->iCmd == CMD_MANAGE712_TEST_ROUTE)
        {
            RouteTest(pTI->chFrom,pTF);
        }
    }
    break;
	case OP_DEV:
	{
        if(pTF->iCmd == CMD_DEV_QUE)
        {
			EquipmentSta(pTI->chFrom);
        }
	}
	break;
	default:break;
    }

    //----------------------------------------广播业务----------------------------------------
    if(pTF->iDport == 0XFF)
    {
        switch(pTF->iOp)
        {
        case CMD_BUSINESS_INFO:
        {
            //主机综合业务状态信息
            Comprehensive_Info_Handle(pTF->pData);
        }
        break;
        default:
            break;
        }
    }
    else
    {
        //自动应答
        if(pTF->FrameHead.iAck == 1)
        {
            uint8_t  txbuf[32] = {0};
            uint8_t ackData[2] = {0};
            ackData[0] = pTF->iCmd;
            ackData[1] = pTF->iDataLen >0 ? *(pTF->pData) : 0xFF;
            CCANDataSimpleSendPost(txbuf,sizeof(txbuf),pTI->chFrom,pTF->iOp, CMDACK, pTF->iDport, pTF->iSport, ackData, sizeof(ackData), FALSE);
        }
    }
}

/*********************************************************************//**
 * @brief			以太网数据处理函数
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void EthernetHost_Handle(Tran_ptCIR_Frame *pTF,TranItem *pTItem)
{
    Ethernet_Info_Type *pTI = NULL;
    pTI = (Ethernet_Info_Type *)(pTItem->Info);

    //报文解析
    if(ParseptCIRFrame(pTF,pTI->Info,pTI->Infolen) == FALSE)
    {
	
        return;
    }
    switch(pTF->iOp)
    {
    case OP_UPDATE:
    {
        UpdateDataIn(pTF,INTERFACE_LAN);
    }
    break;
    default:
        break;
    }
}

/*********************************************************************//**
 * @brief			AppTaskMsgDispatchProc
 * @param[in]	p_arg
 * @return 		None
 **********************************************************************/
void  AppTaskMsgDispatchProc(void *p_arg)
{
    INT8U err = 0;
    TranItem *pTI = NULL;
    uint16_t len;
    while(1)
    {
        pTI = (TranItem *)OSQPend(g_MsgDispatch,10,&err);
        if(pTI)
        {
            memset(g_rxbuf,0,sizeof(g_rxbuf));
            //将数据全部暂存下载
            len = pTI->Infolen + 3;
//            memcpy(g_rxbuf,pTI,pTI->Infolen + 3);
            memcpy(g_rxbuf,pTI,len);
            MemFree(pTI);
            pTI =(TranItem *)g_rxbuf;
//            WifiOut(pTI,len);
            switch(pTI->chFrom)
            {
                //控制盒I、II串口业务处理
            case INTERFACE_COM0:
            {

            }
            break;
            case INTERFACE_COM1:
            {
                Uart_Data_Handle(pTI);
            }
            break;
            case INTERFACE_COM3:
            {

            }
            break;
            case INTERFACE_CAN1:
            case INTERFACE_CAN2:
            {
                CCANHost_Handle(&g_TF,pTI);
            }
            break;

            case INTERFACE_LAN:
            {
                EthernetHost_Handle(&g_TF,pTI);
            }
            break;

            default:
                break;
            }
        }
    }
}
/*********************************************************************//**
 * @brief			
 * @param[in]	
 * @return 		
 **********************************************************************/
void RouteTest(uint8_t chto, Tran_ptCIR_Frame *pTF)
{
    uint8_t txbuf[64] = {0};
    CCANDataSimpleSendPost(txbuf,sizeof(txbuf),chto,OP_MANAGE712,CMD_MANAGE712_TEST_ROUTE_ACK,g_ParaInfo.SelfPort,pTF->iSport,(uint8_t *)pTF->pData,pTF->iDataLen,FALSE);
}




