/************************************************************
  Copyright (C), 2016, 712.Co.,Ltd.
  FileName: comm.c
  Author: LiuZheng      
  Description: 通用的数据帧组包\  解包函数
  Version:       V1.0.0.2  
  Function List:   
  1. 数据帧组包\  解包
  History:        
  <author>  <time>   <version >   <desc>
   zouhuayong    08/12/12     1.0.0.0     build this moudle  
   zhaoli		    09.08.27      1.0.0.1     添加具体业务处理
   liuzheng			16/04/14	1.0.0.2		modify
***********************************************************/
#ifndef _DATAGRAM_H_
#define _DATAGRAM_H_

#include "lpc_types.h"
#include "crc.h"
#include "cmdfunction.h"

#define DLE 0x10
#define STX	0x02
#define ETX 0x03

#define PACK_CIR20          0x00
#define PACK_CIR30          0x01

#pragma pack(push,1)

//CIR 基础帧格式pbCIR20/pbCIR30/psCIR30
typedef struct tag_TRAN_PTCIR_FRAME
{
	struct
	{
		uint8_t     iForm:2;				// 帧格式 00:2.0帧格式 01:3.0帧格式 10 11保留
		uint8_t 	iResend:1;				// 是否重发 0:正常帧  1:重发帧	
		uint8_t		iAck:1;					// 是否要求应答 0:不要求应答  1:要求应答
		uint8_t     iRes:1;					// 预留固定 0		
		uint8_t		iSN:3;					// 流水号
	}FrameHead;
	
	uint16_t 	iInfoLen;					// 信息长度
	uint8_t 	iSport;						// 源端口
	uint8_t 	iSaddrlen;					// 源地址长度
	uint8_t 	szSaddr[8];					// 源地址
	uint8_t 	iDport;						// 目标端口
	uint8_t 	iDaddrlen;					// 目标地址长度
	uint8_t 	szDaddr[8];					// 目标地址
	uint8_t 	iOp; 					  	// 业务类型
	uint8_t 	iCmd;  						// 命令
	uint16_t 	iDataLen;					// 有效数据长度
	uint8_t 	pData[2048];					// 有效数据
}Tran_ptCIR_Frame;

#pragma pack(pop)



uint16_t BuildptCIR20Frame(uint8_t *FrameAr, uint16_t FrameLen, uint8_t op, uint8_t cmd, uint8_t sport, uint8_t saddrlen, uint8_t *saddr, uint8_t dport, uint8_t daddrlen, uint8_t *daddr, uint8_t *data, uint16_t datalen);

uint16_t  BuildptCIR20FrameStream(uint8_t *FrameAr, uint16_t FrameLen, uint8_t op, uint8_t cmd, uint8_t sport, uint8_t saddrlen, uint8_t *saddr, uint8_t dport, uint8_t daddrlen, uint8_t *daddr, uint8_t *data, uint16_t datalen);

uint16_t  BuildptCIR20FrameStreamByptCIR20Frame(uint8_t *FrameAr, uint16_t FrameLen, uint8_t *ptCIR20, uint16_t datalen);

uint16_t BuildptCIR30Frame(uint8_t *FrameAr, uint16_t FrameLen, uint8_t op, uint8_t cmd, uint8_t sport, uint8_t saddrlen, uint8_t *saddr, uint8_t dport, uint8_t daddrlen, uint8_t *daddr, uint8_t *data, uint16_t datalen, Bool bAck, Bool bResend, uint8_t iSN);

uint16_t  BuildptCIR30FrameStream(uint8_t *FrameAr, uint16_t FrameLen, uint8_t op, uint8_t cmd, uint8_t sport, uint8_t saddrlen, uint8_t *saddr, uint8_t dport, uint8_t daddrlen, uint8_t *daddr, uint8_t *data, uint16_t datalen, Bool bAck, Bool bResend, uint8_t iSN);

uint16_t  BuildptCIR30FrameStreamByptCIR30Frame(uint8_t *FrameAr, uint16_t FrameLen, uint8_t *ptCIR30, uint16_t datalen);

uint16_t PackDLE(uint8_t *DleFrameAr, uint8_t *UnDleFrameAr,uint32_t dwLen);

Bool ParseptCIRFrame(Tran_ptCIR_Frame *pTF, uint8_t *pBytes, uint16_t iLen);

uint16_t RemoveHeadAndTail(uint8_t *DelFrameAr, uint8_t *pBytes, uint16_t iLen);

uint16_t UnPackDLE(uint8_t *DleFrameAr, uint8_t *UnDleFrameAr, uint32_t dwLen);




#endif 



