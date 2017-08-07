/**********************************************************************
* @file     EthernetUdp.h
* @brief    以太网UDP数据处理头文件
* @version  1.0.0
* @date     2016/12/15 星期四  13:02:30		
* @author   ZhangYanMing
*
* Copyright(C) 2016, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
#ifndef __ETHERNETUDP_H__
#define __ETHERNETUDP_H__
/**************************** Includes ********************************/
#include "lpc_types.h"

/**************************** Macro Definition ************************/
#define ETHERNET_UDP				0
#define UDPLOCALNUM         8

typedef  Bool (*pFnRevData)(uint8_t type,uint8_t *rip, uint16_t rport,uint16_t localport,uint8_t *buf, uint16_t len);
typedef  Bool (*pFnSendData)(uint8_t *data,uint16_t datalen,uint8_t type,uint8_t *ip,uint16_t port,uint8_t soc);


#pragma push
#pragma pack(1)                               /* 1字节对齐 */

typedef struct
{
	uint8_t Socket;
	uint8_t ip[4];
	uint16_t port;
	uint8_t State;
}Ethernet_Type_Info;

typedef struct
{
	uint8_t  socket;
	uint16_t localport;
	
	pFnSendData pfuncSend;
}UdpSendInfo_Type;


typedef struct
{
	uint8_t  socket;
	uint16_t localport;
	
	pFnRevData pfuncRcv;
}UdpRecInfo_Type;

#pragma pop


/************************** Function Interface ************************/
Bool UDP_SendData(uint8_t socket,uint8_t *remip,uint16_t remport,uint8_t *buf, uint16_t len);
Bool UDP_ReceivePort_Init(uint16_t port,pFnRevData pRcvDatafuc);
Bool UDP_SendPort_Init(uint16_t port,pFnSendData pfuncSend);
Bool UDP_DataSend(uint8_t *remip,uint16_t remport,uint8_t *buf,uint16_t len);
void UDP_DataInit(void);

#endif


