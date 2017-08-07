/************************************************************
  Copyright (C), 2016, 712.Co.,Ltd.
  FileName: comm.c
  Author: LiuZheng      
  Description: 通用的数据帧组包\解包函数
  Version:       V1.0.0.2  
  Function List:   
  1. 数据帧组包\  解包
  History:        
  <author>  <time>   <version >   <desc>
   zouhuayong    08/12/12     1.0.0.0     build this moudle  
   zhaoli		    09.08.27      1.0.0.1     添加具体业务处理
   liuzheng			16/04/14	1.0.0.2		modify
***********************************************************/
#include "ptcir.h"
#include <string.h>



/*********************************************************************//**
 * @brief		去掉重复10
 * @param[out]	DleFrameAr    ：去掉10后数据
 * @param[in]	UnDleFrameAr  ：未去掉10数据 
 * @param[in]	dwLen         ：未去掉10数据长度 
 * @return 		去掉10后数据长度
 **********************************************************************/
uint16_t UnPackDLE(uint8_t *DleFrameAr, uint8_t *UnDleFrameAr, uint32_t dwLen)
{
	uint8_t *pSrc = UnDleFrameAr;
	uint8_t *pDst = DleFrameAr;
	uint16_t i = 0, iLen = 0;
	
	pDst[iLen++] = pSrc[i++]; 	// 头
	pDst[iLen++] = pSrc[i++];
	
	for ( ; i < dwLen - 2; i++)
	{
		if (DLE == pSrc[i] && DLE == pSrc[i+1] )
		{
			i++;        		// 遇到两个10,则去掉一个10
		}
		
		pDst[iLen++] = pSrc[i];
	}
	
	pDst[iLen++] = pSrc[i++];	// 尾
	pDst[iLen++] = pSrc[i++];

	return iLen;
}

/*********************************************************************//**
 * @brief		pbCIR2.0、pbCIR3.0、psCIR3.0去掉头尾1002-1003，并去掉pbCIR2.0中的DLE
 * @param[out]	DleFrameAr    ：去掉头尾的数据
 * @param[in]	pBytes  	  ：源数据
 * @param[in]	dwLen         ：源数据长度 
 * @return 		去掉头尾数据长度
 **********************************************************************/
uint16_t RemoveHeadAndTail(uint8_t *DelFrameAr, uint8_t *pBytes, uint16_t iLen)
{
	uint16_t UDArLen = 0;
	
	if(iLen > 1024 || iLen < 12 || DLE != pBytes[0] || STX != pBytes[1] || DLE != pBytes[iLen - 2] || ETX != pBytes[iLen - 1])
	{
		return  0;
	}
	
	if(pBytes[2] & 0xC0)	// ptCIR3.0
	{
		memcpy(DelFrameAr, pBytes + 2, iLen - 4);
		
		return iLen - 4;
	}
	else	// ptCIR2.0
	{
		uint8_t stream[1024] = {0};
		
		UDArLen = UnPackDLE(stream, pBytes, iLen);
		
		memcpy(DelFrameAr, stream + 2, UDArLen - 4);
		
		return UDArLen - 4;
	}
	
}
/*********************************************************************//**
 * @brief		遇10补10
 * @param[out]	DleFrameAr    ：补10后数据
 * @param[in]	UnDleFrameAr  ：未补10数据
 * @param[in]	dwLen         ：未补10数据长度 
 * @return 		去掉10后数据长度
 **********************************************************************/
uint16_t PackDLE(uint8_t *DleFrameAr, uint8_t *UnDleFrameAr,uint32_t dwLen)
{
	uint8_t *pSrc = UnDleFrameAr;
	uint8_t *pDst = DleFrameAr;
	uint16_t i = 0, iLen = 0;
	
	pDst[iLen++]=pSrc[i++]; 		//头
	pDst[iLen++]=pSrc[i++];
	
	for ( ; i < dwLen - 2; i++)
	{
		if (DLE == pSrc[i])
		{
			pDst[iLen++] = pSrc[i]; 	//遇到 X010 则补一个0X10
		}
		
		pDst[iLen++] = pSrc[i];
	}
	
	pDst[iLen++] = pSrc[i++];		//尾
	pDst[iLen++] = pSrc[i++];

	return iLen;
}

/*********************************************************************//**
 * @brief		组成pbCIR2.0基础帧格式
 * @param[out]	FrameAr       ：组成基础帧数据格式
 * @param[in]	FrameLen      ：FrameAr长度
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
 * @return 		组成基础帧数据格式后长度
 **********************************************************************/
uint16_t BuildptCIR20Frame(uint8_t *FrameAr, uint16_t FrameLen, uint8_t op, uint8_t cmd, uint8_t sport, uint8_t saddrlen, uint8_t *saddr, uint8_t dport, uint8_t daddrlen, uint8_t *daddr, uint8_t *data, uint16_t datalen)
{
	uint16_t infolen = 8 + saddrlen + daddrlen + datalen;
	uint8_t *pStr = FrameAr;
	uint16_t crc;
	
	if(FrameLen < infolen + 2)
	{
		return 0;
	}
	
	// 信息长度高位在前
	*pStr++ = HIBYTE(infolen);
	*pStr++ = LOBYTE(infolen);

	// 源地址信息
	*pStr++ = sport;
    
	if(saddrlen)
	{
		if(saddr)
		{
			*pStr++ = saddrlen;
			memcpy(pStr, saddr, saddrlen);
			pStr += saddrlen;
		}
	}
	else
	{
		*pStr++ = 0;
	}

	 // 目标地址信息
	*pStr++ = dport;
	
	if(daddrlen)
	{
		if(daddr)
		{
			*pStr++ = daddrlen;
			memcpy(pStr, daddr, daddrlen);
			pStr += daddrlen;
		}
	}
	else
	{
		*pStr++ = 0;
	}

	*pStr++ = op;
	*pStr++ = cmd;

	if(data && datalen > 0)
	{
		memcpy(pStr, data, datalen);
		pStr += datalen;
	}

	crc = CalCRC16(FrameAr, infolen);

	*pStr++ = HIBYTE(crc);
	*pStr++ = LOBYTE(crc);
		
	return infolen + 2;
}

/*********************************************************************//**
 * @brief		组成10 02 ptCIR2.0 10 03数据格式
 * @param[out]	FrameAr       ：输出数据
 * @param[in]	FrameLen      ：FrameAr长度
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
 * @return 		输出数据长度, 包含补得10
 **********************************************************************/
/*** 此函数内部包含已被1k的临时变量会使每个调用其的任务堆栈增大 ***/
uint16_t BuildptCIR20FrameStream(uint8_t *FrameAr, uint16_t FrameLen, uint8_t op, uint8_t cmd, uint8_t sport, uint8_t saddrlen, uint8_t *saddr, uint8_t dport, uint8_t daddrlen, uint8_t *daddr, uint8_t *data, uint16_t datalen)
{
	uint8_t stream[1024] = {0}; 
	uint16_t iLen = 0;

	if(FrameLen > 1024)
	{
		return 0;
	}
	
	stream[0] = DLE;
	stream[1] = STX;
	
	iLen = BuildptCIR20Frame(stream + 2, FrameLen, op, cmd, sport, saddrlen, saddr, dport, daddrlen, daddr, data, datalen);
	
	stream[iLen + 2] = DLE;
	stream[iLen + 3] = ETX;
	
	return PackDLE(FrameAr, stream, iLen + 4);
}
   
/*********************************************************************//**
 * @brief		组成10 02 ptCIR2.0 10 03基础数据格式
 * @param[out]	FrameAr       ：输出数据
 * @param[in]	FrameLen      ：FrameAr长度
 * @param[in]	ptCIR20       ：ptCIR20基础帧
 * @param[in]	datalen       ：ptCIR20基础帧长度
 * @return 		输出数据长度, 包含补得10
 **********************************************************************/
/*** 此函数内部包含已被1k的临时变量会使每个调用其的任务堆栈增大 ***/
uint16_t  BuildptCIR20FrameStreamByptCIR20Frame(uint8_t *FrameAr, uint16_t FrameLen, uint8_t *ptCIR20, uint16_t datalen)
{
	uint8_t stream[1024] = {0}; 

	if(FrameLen > 1024)
	{
		return 0;
	}
	
	stream[0] = DLE;
	stream[1] = STX;
	
	memcpy(stream + 2, ptCIR20, datalen);
	
	stream[datalen + 2] = DLE;
	stream[datalen + 3] = ETX;
	
	return PackDLE(FrameAr, stream, datalen + 4);
}


/*********************************************************************//**
 * @brief		组成ptCIR3.0帧格式
 * @param[out]	FrameAr       ：组成基础帧数据格式
 * @param[in]	FrameLen      ：FrameAr长度
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
 * @param[in]	bAck       	  ：是否需应答
 * @param[in]	bResend       ：是否重发
 * @param[in]	iSN           ：流水号
 * @return 		组成基础帧数据格式后长度
 **********************************************************************/
uint16_t BuildptCIR30Frame(uint8_t *FrameAr, uint16_t FrameLen, uint8_t op, uint8_t cmd, uint8_t sport, uint8_t saddrlen, uint8_t *saddr, uint8_t dport, uint8_t daddrlen, uint8_t *daddr, uint8_t *data, uint16_t datalen, Bool bAck, Bool bResend, uint8_t iSN)
{
	uint16_t iLen = 11 + saddrlen + daddrlen + datalen;
	uint16_t infolen = 11 + saddrlen + daddrlen + datalen;
	uint8_t *pStr = FrameAr;
	uint16_t crc = 0;
	
	if(FrameLen < iLen)
	{
		return 0;
	}
	
	// 帧头
	*pStr++ = 0x40 | ((bResend == TRUE) ? 0x20 : 0x00) | ((bAck == TRUE) ? 0x10 : 0x00) | 0x00 | (iSN & 0x07);
	
	// 信息长度高位在前
	*pStr++ = HIBYTE(infolen);
	*pStr++ = LOBYTE(infolen);

	// 源地址信息
	*pStr++ = sport;
    
	if(saddrlen > 0)
	{
		if(saddr)
		{
			*pStr++ = saddrlen;
			memcpy(pStr, saddr, saddrlen);
			pStr += saddrlen;
		}
	}
	else
	{
		*pStr++ = 0;
	}

	 // 目标地址信息
	*pStr++ = dport;
	
	if(daddrlen > 0)
	{
		if(daddr)
		{
			*pStr++ = daddrlen;
			memcpy(pStr, daddr, daddrlen);
			pStr += daddrlen;
		}
	}
	else
	{
		*pStr++ = 0;
	}

	*pStr++ = op;
	*pStr++ = cmd;

	if(data && datalen > 0)
	{
		memcpy(pStr, data, datalen);
		pStr += datalen;
	}

	crc = CalCRC16(FrameAr, infolen - 2);

	*pStr++ = HIBYTE(crc);
	*pStr++ = LOBYTE(crc);
	
	return iLen;
}

/*********************************************************************//**
 * @brief		组成10 02 ptCIR3.0 10 03数据格式
 * @param[out]	FrameAr       ：输出数据
 * @param[in]	FrameLen      ：FrameAr长度
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
 * @param[in]	bAck       	  ：是否需应答
 * @param[in]	bResend       ：是否重发
 * @param[in]	iSN           ：流水号
 * @return 		输出数据长度
 **********************************************************************/
uint16_t  BuildptCIR30FrameStream(uint8_t *FrameAr, uint16_t FrameLen, uint8_t op, uint8_t cmd, uint8_t sport, uint8_t saddrlen, uint8_t *saddr, uint8_t dport, uint8_t daddrlen, uint8_t *daddr, uint8_t *data, uint16_t datalen, Bool bAck, Bool bResend, uint8_t iSN)
{
	uint16_t iLen = 0;

	if(FrameLen < datalen + 11 + 4 + saddrlen + daddrlen)
	{
		return 0;
	}
	
	FrameAr[0] = DLE;
	FrameAr[1] = STX;
	
	iLen = BuildptCIR30Frame(FrameAr + 2, FrameLen, op, cmd, sport, saddrlen, saddr, dport, daddrlen, daddr, data, datalen, bAck, bResend, iSN);
	
	if(iLen == 0)
	{
		return 0;
	}
	
	FrameAr[iLen + 2] = DLE;
	FrameAr[iLen + 3] = ETX;
	
	return iLen + 4;
}

/*********************************************************************//**
 * @brief		组成10 02 pbCIR3.0 10 03基础数据格式
 * @param[out]	FrameAr       ：输出数据
 * @param[in]	FrameLen      ：FrameAr长度
 * @param[in]	ptCIR30       ：ptCIR30帧格式
 * @param[in]	datalen       ：ptCIR30帧格式长度
 * @return 		输出数据长度
 **********************************************************************/
/*** 此函数内部包含已被1k的临时变量会使每个调用其的任务堆栈增大 ***/
uint16_t  BuildptCIR30FrameStreamByptCIR30Frame(uint8_t *FrameAr, uint16_t FrameLen, uint8_t *ptCIR30, uint16_t datalen)
{
	if(FrameLen < datalen + 4)
	{
		return 0;
	}
	
	FrameAr[0] = DLE;
	FrameAr[1] = STX;
	
	memcpy(FrameAr + 2, ptCIR30, datalen);
	
	FrameAr[datalen + 2] = DLE;
	FrameAr[datalen + 3] = ETX;
	
	return datalen + 4;
}

/*********************************************************************//**
 * @brief		pbCIR2.0、pbCIR3.0、psCIR3.0基础帧格式解析函数
 * @param[out]	pTF           ：基础帧格式数据结构体
 * @param[in]	pBytes        ：基础帧格式数据
 * @param[in]	dwLen         ：基础帧格式数据长度
 * @return 		TRUE/FALSE
 **********************************************************************/
/*** 此函数内部包含已被1k的临时变量会使每个调用其的任务堆栈增大 ***/
Bool ParseptCIRFrame(Tran_ptCIR_Frame *pTF, uint8_t *pBytes, uint16_t iLen)
{
	uint8_t *pSrc;
	Tran_ptCIR_Frame *pDst = pTF;
	
	if(iLen > 2048)
	{
		return FALSE;
	}
	
	if(pBytes == NULL)
	{
		return FALSE;
	}
	
	if(!CheckCalCRC16(pBytes, iLen))
	{
		return FALSE;
	}
	
	if((pBytes[0] & 0xC0) == 0x40)		// ptCIR3.0
	{
		if(iLen != MAKEWORD(pBytes[2], pBytes[1]))
		{
			return FALSE;
		}
				
		pDst->FrameHead.iForm = (pBytes[0] & 0xC0) >> 6;
		pDst->FrameHead.iResend = (pBytes[0] & 0x20) >> 5;
		pDst->FrameHead.iAck = (pBytes[0] & 0x10) >> 4;		
		pDst->FrameHead.iRes = (pBytes[0] & 0x08) >> 3;		
		pDst->FrameHead.iSN = pBytes[0] & 0x07;
		
		pDst->iInfoLen = iLen;
		
		pSrc = pBytes + 3;
	
		pDst->iSport = *pSrc++;
		pDst->iSaddrlen = *pSrc++;
		
		if(pDst->iSaddrlen > 0)
		{
			if (pDst->iSaddrlen > 8)
			{
				return FALSE;
			}
			
			memcpy(pDst->szSaddr, pSrc, pDst->iSaddrlen);       
			pSrc += pDst->iSaddrlen;		
		}
		
		pDst->iDport = *pSrc++;
		pDst->iDaddrlen = *pSrc++;

		if(pDst->iDaddrlen > 0)
		{
			if(pDst->iDaddrlen > 8)
			{
				return FALSE;
			}
			
			memcpy(pDst->szDaddr, pSrc, pDst->iDaddrlen);
			pSrc += pDst->iDaddrlen;		
		}
		
		pDst->iOp = *pSrc++;
		pDst->iCmd = *pSrc++;
		
		pDst->iDataLen = iLen - 11 - pDst->iSaddrlen - pDst->iDaddrlen;
		
		if(pDst->iDataLen > 0)
		{
			memcpy(pDst->pData, pSrc, pDst->iDataLen);	
		}
	}
	else if((pBytes[0] & 0xC0) == 0x00)	// ptCIR2.0
	{		
		pDst->FrameHead.iForm = 0;
		pDst->FrameHead.iResend = 0;
		pDst->FrameHead.iAck = 0;		
		pDst->FrameHead.iRes = 0;		
		pDst->FrameHead.iSN = 0;
		
		pDst->iInfoLen = iLen - 2;
		
		if(MAKEWORD(pBytes[1], pBytes[0]) != pDst->iInfoLen)
		{
			return FALSE;
		}
		
		pSrc = pBytes + 2;
	
		pDst->iSport = *pSrc++;
		pDst->iSaddrlen = *pSrc++;
		
		if(pDst->iSaddrlen > 0)
		{
			if (pDst->iSaddrlen > 8)
			{
				return FALSE;
			}
			
			memcpy(pDst->szSaddr, pSrc, pDst->iSaddrlen);       
			pSrc += pDst->iSaddrlen;		
		}
		
		pDst->iDport = *pSrc++;
		pDst->iDaddrlen = *pSrc++;

		if(pDst->iDaddrlen > 0)
		{
			if(pDst->iDaddrlen > 8)
			{
				return FALSE;
			}
			
			memcpy(pDst->szDaddr, pSrc, pDst->iDaddrlen);
			pSrc += pDst->iDaddrlen;		
		}
		
		pDst->iOp = *pSrc++;
		pDst->iCmd = *pSrc++;
		
		pDst->iDataLen = iLen - 10 - pDst->iSaddrlen - pDst->iDaddrlen;
		
		if(pDst->iDataLen > 0)
		{
			memcpy(pDst->pData, pSrc, pDst->iDataLen);	
		}
	}
	else
	{
		return FALSE;
	}
	
	return TRUE;
}





