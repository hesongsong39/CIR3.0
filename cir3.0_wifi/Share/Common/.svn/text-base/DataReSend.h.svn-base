/**********************************************************************
* $Id$      DataReSend              2016-05-25
*//**
* @file     DataReSend.h
* @brief    数据重发处理头文件
* @version  1.0.0
* @date     2016/05/25 星期三 14:28:44
* @author   ZhangYanMing
*
* Copyright(C) 2016, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
#ifndef _DATARESEND_H_
#define _DATARESEND_H_
/**************************** Includes ********************************/
#include "lpc_types.h"

/*************************** Macro Definition *************************/
//重发数据结构大小
#define  WAE_QUEUE_SIZE      12
#define  WAE_QUEUE_MASK      (WAE_QUEUE_SIZE - 1)


#pragma push
#pragma pack(1)     /* 1字节对齐 */

typedef struct
{
		uint8_t 	iCIRType;						// CIR协议类型(0:ptCIR2.0  1:ptCIR3.0)
		uint8_t 	iOp; 					  	// 业务类型
		uint8_t 	iCmd;  						// 命令
		uint8_t 	iSport;						// 源端口
		uint8_t 	iSaddrlen;					// 源地址长度
		uint8_t 	szSaddr[4];					// 源地址
		uint8_t 	iDport;						// 目标端口
		uint8_t 	iDaddrlen;					// 目标地址长度
		uint8_t 	szDaddr[4];					// 目标地址
}ResendImport;
	
/*
 * @brief 数据重发结构体定义
 */
typedef struct 
{
	uint8_t  portnum;         //判定对象
	uint16_t remport;         //以太网发送远端机端口号
	uint8_t  m_dwCnt; 				//计数 
	uint32_t m_dwLastTick; 		//最后一次发送数据的时间
	uint32_t m_dwStartTick; 	//最后一次发送数据的时间
	uint32_t m_timeout; 		  //数据重发间隔时间

	//用于匹配的参数设置
	uint8_t  m_cbOp;		 
	uint8_t  m_cbLen;
	uint8_t  m_data[2];
	
	//存储重发关键数据
	ResendImport m_FrameHead;
	
  uint8_t  m_cbSendLen; 			//满足条件时待发送的数据
  uint8_t  *m_SendData;                	
}Wait_Ack_Event;

#pragma pop

/************************** Function Interface ************************/
void DataReSend_Proc (uint8_t *txbuf,uint16_t plen);
Bool DataReSend_Init(void);
Bool DataReSend_SetEvent(uint8_t pType,uint8_t nDevNum,uint16_t port,uint8_t nReSendCnt,uint8_t Flownum,uint8_t op,uint8_t cmd, uint8_t sport,uint8_t saddrlen,uint8_t *saddr,uint8_t dport,uint8_t daddrlen,uint8_t *daddr,uint8_t *pdata,uint16_t datalen,uint32_t timeout);
Bool WAE_CheckPair(uint8_t cbOp, uint8_t cbCmd, uint8_t *data, uint8_t cbLen);

#endif



