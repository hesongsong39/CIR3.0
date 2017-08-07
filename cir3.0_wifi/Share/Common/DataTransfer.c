/**********************************************************************
* $Id$      DataTransfer              2016-06-03
*//**
* @file     DataTransfer.c
* @brief    数据发送相关接口
* @version  1.0.0
* @date     2016/06/03 星期五 11:20:15
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
#include "Task.h"
#include "DataTransfer.h"
#include "DataReSend.h"
#include "ptcir.h"
#include "ptcan.h"
#include "cmddefine.h"
#include "MemManage.h"
#include "EthernetUdp.h"		


/*****************Private variables ***********************************/
static uint8_t FlowNum = 0;//业务帧流水号
/*****************Public variables ************************************/


/*****************Extern variables ************************************/

/********************************************************************//**
 * @brief 		业务层流水号加1
 * @param[in]	
 * @return 		流水号
 *********************************************************************/
uint8_t FlowNumAdd(void)
{
		uint8_t temp = 0;
		OSSchedLock();            /* 关调度保护 */
		FlowNum++;
		FlowNum &= 0x07;
		temp = FlowNum;
		OSSchedUnlock();          /* 解除关调度保护 */
		return temp;
}

/********************************************************************//**
 * @brief 		获取业务层流水号
 * @param[in]	
 * @return 		流水号
 *********************************************************************/
uint8_t FlowNumGet(void)
{
		uint8_t temp = 0;
		OSSchedLock();            /* 关调度保护 */
		temp = FlowNum;
		OSSchedUnlock();          /* 解除关调度保护 */
		return temp;
}


/********************************************************************//**
 * @brief 		把消息添加到队列任务
 * @param[in]	
 * @return 		None
 *********************************************************************/
Bool MQ_AddTail(OS_EVENT * pMsg,uint8_t chForm,uint8_t *data,uint16_t datalen)
{
	char *pBlock=NULL;
	if(pMsg == NULL)
	{
			return FALSE;
	}
	pBlock = MemApply(datalen + 3);
	if(pBlock)
	{
		TranItem *pTI2=(TranItem*)pBlock;
		pTI2->chFrom = chForm;
		pTI2->Infolen = datalen;
		memcpy(pTI2->Info,data,datalen);
		if(OSQPost(pMsg, pTI2) != OS_ERR_NONE)
		{
				MemFree(pBlock);
				return FALSE;
		}
		//发送内存区有可用区域
		return TRUE;
	}
	//发送内存区没有可用区域
	return FALSE;
}


/*********************************************************************//**
 * @brief      通用发送报文
 * @param[in]  nDevNum     串口
 * @param[in]  sdData      报文
 * @param[in]  sdLen       报文长度
 * @return 		 FALSE,未成功将发送数据添加到发送消息队列
							 TRUE,成功将发送数据添加到发送消息队列
 **********************************************************************/
Bool Common_SendData(uint8_t nDevNum,uint8_t *sdData,uint16_t sdLen)
{
	//将消息添加到发送队列
	if(MQ_AddTail(g_MsgTx,nDevNum,sdData,sdLen) == FALSE)
	{
			return FALSE;
	}
	
	return TRUE;
}

static Bool DataReSend_Set(uint8_t pType,uint8_t chto,uint16_t port,uint8_t op,uint8_t cmd, uint8_t sport,uint8_t saddrlen,uint8_t *saddr,uint8_t dport,uint8_t daddrlen,uint8_t *daddr,uint8_t *pdata, uint16_t datalen)
{
		uint8_t   SendCnt = 0,Flownum = 0;

		//获取流水业务帧流水号
		Flownum = FlowNumGet();
		//重发判断
		SendCnt = WAETX_COUNT;
		
		SendCnt--; 
		if(DataReSend_SetEvent(pType,chto,port,SendCnt,Flownum,op,cmd,sport,saddrlen,saddr,dport,daddrlen,daddr,pdata,datalen,WAETX_TIMEOUT) == FALSE)
		{
				return FALSE;
		}
		
		return TRUE;
}


/*********************************************************************//**
 * @brief		数据发送接口
 * @param[out]	FrameAr       ：组成基础帧数据格式
 * @param[in]	op            ：业务类型
 * @param[in]	cmd           ：命令
 * @param[in]	sport         ：源端口
 * @param[in]	saddrlen      ：源地址长度
 * @param[in]	saddr         ：源地址
 * @param[in]	dport         ：目的端口
 * @param[in]	daddrlen      ：目的地址长度
 * @param[in]	daddr         ：目的地址
 * @param[in]	data          ：要组成帧格式的有效数据
 * @param[in]	datalen       ：有效数据长度
 * @return 		TRUE/FALSE
 **********************************************************************/

Bool UARTDatapbCIR30SendPost(uint8_t *FrameAr,uint16_t Framelen,uint8_t chto, uint8_t op, uint8_t cmd, uint8_t sport,uint8_t saddrlen,uint8_t *saddr,uint8_t dport,uint8_t daddrlen,uint8_t *daddr,uint8_t *data, uint16_t datalen, Bool bAck,Bool bResend)
{
		uint8_t  UartFlowNum = 0;
		uint16_t  arlen = 0;
		//业务流水号加1
		UartFlowNum = FlowNumAdd();
		
		//将发送数据组帧后发送出去
		arlen = BuildptCIR30Frame(FrameAr,Framelen,op,cmd,sport,saddrlen,saddr,dport,daddrlen,daddr,data,datalen,bAck,FALSE,UartFlowNum);
		//将消息添加到发送队列
		if(Common_SendData(chto,FrameAr,arlen) == FALSE)
		{
				return FALSE;
		}
		return TRUE;
}

/*********************************************************************//**
 * @brief		数据发送接口
 * @param[out]	FrameAr       ：组成基础帧数据格式
 * @param[in]	op            ：业务类型
 * @param[in]	cmd           ：命令
 * @param[in]	sport         ：源端口
 * @param[in]	saddrlen      ：源地址长度
 * @param[in]	saddr         ：源地址
 * @param[in]	dport         ：目的端口
 * @param[in]	daddrlen      ：目的地址长度
 * @param[in]	daddr         ：目的地址
 * @param[in]	data          ：要组成帧格式的有效数据
 * @param[in]	datalen       ：有效数据长度
 * @return 		TRUE/FALSE
 **********************************************************************/

Bool UARTDatapbCIR20SendPost(uint8_t *FrameAr,uint16_t Framelen,uint8_t chto, uint8_t op, uint8_t cmd, uint8_t sport,uint8_t saddrlen,uint8_t *saddr,uint8_t dport,uint8_t daddrlen,uint8_t *daddr,uint8_t *data, uint16_t datalen, Bool bAck)
{
		uint16_t  arlen = 0;
		
		//将发送数据组帧后发送出去
		arlen = BuildptCIR20FrameStream(FrameAr,Framelen,op,cmd,sport,saddrlen,saddr,dport,daddrlen,daddr,data,datalen);
		//将消息添加到发送队列
		if(Common_SendData(chto,FrameAr,arlen) == FALSE)
		{
				return FALSE;
		}
		
		return TRUE;
}

/*********************************************************************//**
 * @brief		数据发送接口
 * @param[out]	FrameAr       ：组成基础帧数据格式
 * @param[in]	op            ：业务类型
 * @param[in]	cmd           ：命令
 * @param[in]	sport         ：源端口
 * @param[in]	dport         ：目的端口
 * @param[in]	data          ：要组成帧格式的有效数据
 * @param[in]	datalen       ：有效数据长度
 * @return 		TRUE/FALSE
 **********************************************************************/
Bool UARTDatapbCIR20SimpleSendPost(uint8_t *FrameAr,uint16_t Framelen,uint8_t chto, uint8_t op, uint8_t cmd, uint8_t sport,uint8_t dport,uint8_t *data, uint16_t datalen, Bool bAck)
{	
		Bool retval = FALSE;
		
		OSSchedLock();            /* 关调度保护 */
		if(UARTDatapbCIR20SendPost(FrameAr,Framelen,chto,op,cmd,sport,0,NULL,dport,0,NULL,data,datalen,bAck) == TRUE)
		{
				retval = TRUE;
				if(bAck == TRUE)
				{
						retval = DataReSend_Set(PACK_CIR20 | PERIPH_UART,chto,0,op,cmd,sport,0,NULL,dport,0,NULL,data,datalen);
				}
		}
		OSSchedUnlock();          /* 解除关调度保护 */
		
		return retval;
}

/*********************************************************************//**
 * @brief		数据发送接口
 * @param[out]	FrameAr       ：组成基础帧数据格式
 * @param[in]	op            ：业务类型
 * @param[in]	cmd           ：命令
 * @param[in]	sport         ：源端口
 * @param[in]	dport         ：目的端口
 * @param[in]	data          ：要组成帧格式的有效数据
 * @param[in]	datalen       ：有效数据长度
 * @return 		TRUE/FALSE
 **********************************************************************/
Bool UARTDatapbCIR30SimpleSendPost(uint8_t *FrameAr,uint16_t Framelen,uint8_t chto, uint8_t op, uint8_t cmd, uint8_t sport,uint8_t dport,uint8_t *data, uint16_t datalen, Bool bAck)
{	
		Bool retval = FALSE;
		
		OSSchedLock();            /* 关调度保护 */
		if(UARTDatapbCIR30SendPost(FrameAr,Framelen,chto,op,cmd,sport,0,NULL,dport,0,NULL,data,datalen,bAck,FALSE) == TRUE)
		{
				retval = TRUE;
				if(bAck == TRUE)
				{
						retval = DataReSend_Set(PACK_CIR30 | PERIPH_UART,chto,0,op,cmd,sport,0,NULL,dport,0,NULL,data,datalen);
				}
		}
		OSSchedUnlock();          /* 解除关调度保护 */
		
		return retval;
		
}


/*********************************************************************//**
 * @brief		数据发送接口
 * @param[out]	FrameAr       ：组成基础帧数据格式
 * @param[in]	op            ：业务类型
 * @param[in]	cmd           ：命令
 * @param[in]	sport         ：源端口
 * @param[in]	dport         ：目的端口
 * @param[in]	data          ：要组成帧格式的有效数据
 * @param[in]	datalen       ：有效数据长度
 * @return 		TRUE/FALSE
 **********************************************************************/
Bool UARTDatapbCIR30FullSendPost(uint8_t *FrameAr,uint16_t Framelen,uint8_t chto, uint8_t op, uint8_t cmd, uint8_t sport,uint8_t saddrlen,uint8_t *saddr,uint8_t dport,uint8_t daddrlen,uint8_t *daddr,uint8_t *data, uint16_t datalen, Bool bAck)
{	
		Bool retval = FALSE;
		
		OSSchedLock();            /* 关调度保护 */
		if(UARTDatapbCIR30SendPost(FrameAr,Framelen,chto,op,cmd,sport,saddrlen,saddr,dport,daddrlen,daddr,data,datalen,bAck,FALSE) == TRUE)
		{
				retval = TRUE;
				if(bAck == TRUE)
				{
						retval = DataReSend_Set(PACK_CIR30 | PERIPH_UART,chto,0,op,cmd,sport,saddrlen,saddr,dport,daddrlen,daddr,data,datalen);
				}
		}
		OSSchedUnlock();          /* 解除关调度保护 */
		
		return retval;
}

/*********************************************************************//**
 * @brief		CAN数据发送接口
 * @param[out]	FrameAr       ：组成基础帧数据格式
 * @param[in]	op            ：业务类型
 * @param[in]	cmd           ：命令
 * @param[in]	sport         ：源端口
 * @param[in]	dport         ：目的端口
 * @param[in]	data          ：要组成帧格式的有效数据
 * @param[in]	datalen       ：有效数据长度
 * @param[in]	bAck          ：是否需要应答
 * @param[in]	bResend       ：是否需要重发
 * @return 		组成基础帧数据格式后长度
 **********************************************************************/

Bool CCANDataSendPost(uint8_t *FrameAr,uint16_t Framelen,uint8_t chto,uint8_t op,uint8_t cmd, uint8_t sport,uint8_t saddrlen,uint8_t *saddr,uint8_t dport,uint8_t daddrlen,uint8_t *daddr,uint8_t *data, uint16_t datalen, Bool bAck,Bool bResend)
{
		_AddrPort  s_AddrItem;
		_AddrPort  d_AddrItem;
		uint8_t   Priority = 0, CanFlowNum = 0,DataType = 0,*pdata = NULL;
		CCANItem *pTI = (CCANItem *)FrameAr; 
		uint16_t   arlen = 0;
	
		//判断传进来的数据缓冲大小，不足直接返回
		if(Framelen < 11 + saddrlen + daddrlen + datalen)
		{
				return FALSE;
		}
		pdata = data;
		
		//通过源端口、目的端口自动获取自身以太网IP地址、CAN总线的源地址、目的地址
		s_AddrItem.port = sport;
		if(GetCanNetAddrByPort(s_AddrItem.port,&s_AddrItem.canaddr,&s_AddrItem.netaddr) == FALSE)
		{
				
		}
		
		d_AddrItem.port = dport;
		if(GetCanNetAddrByPort(d_AddrItem.port,&d_AddrItem.canaddr,&d_AddrItem.netaddr) == FALSE)
		{
				
		}
		//获取CAN参数中的优先级
		if(GetCanPriDataTypeByOpCmd(sport,dport,op,cmd,&Priority,&DataType) == FALSE)
		{
				
		}
		//CCAN参数结构体赋值
		pTI->CanParam.CANIndex = chto;
		pTI->CanParam.ProtocolVer = CCANPROTOCOL;
		pTI->CanParam.Priority = Priority;
		pTI->CanParam.DataType = DataType;
		pTI->CanParam.CANSrcAddr = s_AddrItem.canaddr;
		pTI->CanParam.CANDestAddr =  d_AddrItem.canaddr;
		
		if(INTERFACE_CAN2 == chto)
		{
				pTI->CanParam.ZongXianDiZhi = 0x0;
		}
		else
		{
				pTI->CanParam.ZongXianDiZhi = 0x1;
		}
		pTI->CanParam.ZhuanFaBiaoZhi = 0;
		//业务流水号加1
		CanFlowNum = FlowNumAdd();
		//将发送数据组帧后发送出去
		pTI->Infolen = BuildptCIR30Frame(pTI->Info,Framelen - sizeof(CCANItem),op,cmd,sport,saddrlen,saddr,dport,daddrlen,daddr,pdata,datalen,bAck,bResend,CanFlowNum);
		
		//计算发送的数据长度
		arlen = sizeof(CAN_Param) + 2 + pTI->Infolen;
		//将消息添加到发送队列
		if(Common_SendData(chto,FrameAr,arlen) == FALSE)
		{
				return FALSE;
		}
		
		return TRUE;
}

/*********************************************************************//**
 * @brief		CAN数据发送接口
 * @param[out]	FrameAr       ：组成基础帧数据格式
 * @param[in]	op            ：业务类型
 * @param[in]	cmd           ：命令
 * @param[in]	sport         ：源端口
 * @param[in]	dport         ：目的端口
 * @param[in]	data          ：要组成帧格式的有效数据
 * @param[in]	datalen       ：有效数据长度
 * @param[in]	bAck          ：是否需要应答
 * @param[in]	bResend       ：是否需要重发
 * @return 		组成基础帧数据格式后长度
 **********************************************************************/

Bool CCANDataSimpleSendPost(uint8_t *FrameAr,uint16_t Framelen,uint8_t chto,uint8_t op,uint8_t cmd, uint8_t sport,uint8_t dport,uint8_t *data, uint16_t datalen, Bool bAck)
{
		Bool retval = FALSE;
		
		OSSchedLock();            /* 关调度保护 */
		if(CCANDataSendPost(FrameAr,Framelen,chto,op,cmd,sport,0,NULL,dport,0,NULL,data,datalen,bAck,FALSE) == TRUE)
		{
				retval = TRUE;
				if(bAck == TRUE)
				{
						retval = DataReSend_Set(PACK_CIR30 | PERIPH_CAN,chto,0,op,cmd,sport,0,NULL,dport,0,NULL,data,datalen);
				}
		}
		OSSchedUnlock();          /* 解除关调度保护 */
		
		return retval;
}


/*********************************************************************//**
 * @brief		CAN数据发送接口
 * @param[out]	FrameAr       ：组成基础帧数据格式
 * @param[in]	op            ：业务类型
 * @param[in]	cmd           ：命令
 * @param[in]	sport         ：源端口
 * @param[in]	dport         ：目的端口
 * @param[in]	data          ：要组成帧格式的有效数据
 * @param[in]	datalen       ：有效数据长度
 * @param[in]	bAck          ：是否需要应答
 * @param[in]	bResend       ：是否需要重发
 * @return 		组成基础帧数据格式后长度
 **********************************************************************/

Bool CCANDataFullSendPost(uint8_t *FrameAr,uint16_t Framelen,uint8_t chto,uint8_t op,uint8_t cmd, uint8_t sport,uint8_t saddrlen,uint8_t *saddr,uint8_t dport,uint8_t daddrlen,uint8_t *daddr,uint8_t *data, uint16_t datalen, Bool bAck)
{
		Bool retval = FALSE;
		
		OSSchedLock();            /* 关调度保护 */
		if(CCANDataSendPost(FrameAr,Framelen,chto,op,cmd,sport,saddrlen,saddr,dport,daddrlen,daddr,data,datalen,bAck,FALSE) == TRUE)
		{
				retval = TRUE;
				if(bAck == TRUE)
				{
						retval = DataReSend_Set(PACK_CIR30 | PERIPH_CAN,chto,0,op,cmd,sport,saddrlen,saddr,dport,daddrlen,daddr,data,datalen);
				}
		}
		OSSchedUnlock();          /* 解除关调度保护 */
		
		return retval;
}




/*********************************************************************//**
 * @brief		以太网数据发送接口
 * @param[out]	FrameAr       ：组成基础帧数据格式
 * @param[in]	op            ：业务类型
 * @param[in]	cmd           ：命令
 * @param[in]	sport         ：源端口
 * @param[in]	dport         ：目的端口
 * @param[in]	data          ：要组成帧格式的有效数据
 * @param[in]	datalen       ：有效数据长度
 * @param[in]	bAck          ：是否需要应答
 * @param[in]	bResend       ：是否需要重发
 * @return 		组成基础帧数据格式后长度
 **********************************************************************/
Bool EthernetDataSendPost(uint8_t *FrameAr,uint16_t Framelen,uint16_t port,uint8_t op,uint8_t cmd, uint8_t sport,uint8_t saddrlen,uint8_t *saddr,uint8_t dport,uint8_t daddrlen,uint8_t *daddr,uint8_t *data, uint16_t datalen, Bool bAck,Bool bResend)
{
		uint8_t  EthernetFlowNum = 0;
		uint16_t  arlen = 0;
		//业务流水号加1
		EthernetFlowNum = FlowNumAdd();
		
		//将发送数据组帧后发送出去
		arlen = BuildptCIR30Frame(FrameAr,Framelen,op,cmd,sport,saddrlen,saddr,dport,daddrlen,daddr,data,datalen,bAck,FALSE,EthernetFlowNum);
		
		if(UDP_DataSend(daddr,port,FrameAr,arlen) == FALSE)
		{
				return FALSE;
		}
	
		return TRUE;
}

/*********************************************************************//**
 * @brief		数据发送接口
 * @param[out]	FrameAr       ：组成基础帧数据格式
 * @param[in]	op            ：业务类型
 * @param[in]	cmd           ：命令
 * @param[in]	sport         ：源端口
 * @param[in]	dport         ：目的端口
 * @param[in]	data          ：要组成帧格式的有效数据
 * @param[in]	datalen       ：有效数据长度
 * @return 		TRUE/FALSE
 **********************************************************************/
Bool EthernetDatapbCIR30FullSendPost(uint8_t *FrameAr,uint16_t Framelen,uint16_t port, uint8_t op, uint8_t cmd, uint8_t sport,uint8_t saddrlen,uint8_t *saddr,uint8_t dport,uint8_t daddrlen,uint8_t *daddr,uint8_t *data, uint16_t datalen, Bool bAck)
{	
		Bool retval = FALSE;
		
		OSSchedLock();            /* 关调度保护 */
		if(EthernetDataSendPost(FrameAr,Framelen,port,op,cmd,sport,saddrlen,saddr,dport,daddrlen,daddr,data,datalen,bAck,FALSE) == TRUE)
		{
				retval = TRUE;
				if(bAck == TRUE)
				{
						retval = DataReSend_Set(PACK_CIR30 | PERIPH_ETHERNET,INTERFACE_LAN,port,op,cmd,sport,saddrlen,saddr,dport,daddrlen,daddr,data,datalen);
				}
		}
		OSSchedUnlock();          /* 解除关调度保护 */
		
		return retval;
}
