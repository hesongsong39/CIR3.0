/************************************************************
  Copyright (C), 2016, 712.Co.,Ltd.
  FileName: comm.c
  Author: LiuZheng      
  Description: ͨ�õ�����֡���\�������
  Version:       V1.0.0.2  
  Function List:   
  1. ����֡���\  ���
  History:        
  <author>  <time>   <version >   <desc>
   zouhuayong    08/12/12     1.0.0.0     build this moudle  
   zhaoli		    09.08.27      1.0.0.1     ��Ӿ���ҵ����
   liuzheng			16/04/14	1.0.0.2		modify
***********************************************************/
#include "ptcir.h"
#include <string.h>



/*********************************************************************//**
 * @brief		ȥ���ظ�10
 * @param[out]	DleFrameAr    ��ȥ��10������
 * @param[in]	UnDleFrameAr  ��δȥ��10���� 
 * @param[in]	dwLen         ��δȥ��10���ݳ��� 
 * @return 		ȥ��10�����ݳ���
 **********************************************************************/
uint16_t UnPackDLE(uint8_t *DleFrameAr, uint8_t *UnDleFrameAr, uint32_t dwLen)
{
	uint8_t *pSrc = UnDleFrameAr;
	uint8_t *pDst = DleFrameAr;
	uint16_t i = 0, iLen = 0;
	
	pDst[iLen++] = pSrc[i++]; 	// ͷ
	pDst[iLen++] = pSrc[i++];
	
	for ( ; i < dwLen - 2; i++)
	{
		if (DLE == pSrc[i] && DLE == pSrc[i+1] )
		{
			i++;        		// ��������10,��ȥ��һ��10
		}
		
		pDst[iLen++] = pSrc[i];
	}
	
	pDst[iLen++] = pSrc[i++];	// β
	pDst[iLen++] = pSrc[i++];

	return iLen;
}

/*********************************************************************//**
 * @brief		pbCIR2.0��pbCIR3.0��psCIR3.0ȥ��ͷβ1002-1003����ȥ��pbCIR2.0�е�DLE
 * @param[out]	DleFrameAr    ��ȥ��ͷβ������
 * @param[in]	pBytes  	  ��Դ����
 * @param[in]	dwLen         ��Դ���ݳ��� 
 * @return 		ȥ��ͷβ���ݳ���
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
 * @brief		��10��10
 * @param[out]	DleFrameAr    ����10������
 * @param[in]	UnDleFrameAr  ��δ��10����
 * @param[in]	dwLen         ��δ��10���ݳ��� 
 * @return 		ȥ��10�����ݳ���
 **********************************************************************/
uint16_t PackDLE(uint8_t *DleFrameAr, uint8_t *UnDleFrameAr,uint32_t dwLen)
{
	uint8_t *pSrc = UnDleFrameAr;
	uint8_t *pDst = DleFrameAr;
	uint16_t i = 0, iLen = 0;
	
	pDst[iLen++]=pSrc[i++]; 		//ͷ
	pDst[iLen++]=pSrc[i++];
	
	for ( ; i < dwLen - 2; i++)
	{
		if (DLE == pSrc[i])
		{
			pDst[iLen++] = pSrc[i]; 	//���� X010 ��һ��0X10
		}
		
		pDst[iLen++] = pSrc[i];
	}
	
	pDst[iLen++] = pSrc[i++];		//β
	pDst[iLen++] = pSrc[i++];

	return iLen;
}

/*********************************************************************//**
 * @brief		���pbCIR2.0����֡��ʽ
 * @param[out]	FrameAr       ����ɻ���֡���ݸ�ʽ
 * @param[in]	FrameLen      ��FrameAr����
 * @param[in]	op            ��ҵ������
 * @param[in]	cmd           ������
 * @param[in]	sport         ��Դ�˿�
 * @param[in]	saddrlen      ��Դ��ַ����
 * @param[in]	saddr         ��Դ��ַ
 * @param[in]	dport         ��Ŀ�Ķ˿�
 * @param[in]	daddrlen      ��Ŀ�ĵ�ַ����
 * @param[in]	daddr         ��Ŀ�ĵ�ַ
 * @param[in]	data          ��Ҫ���֡��ʽ����Ч����
 * @param[in]	datalen       ����Ч���ݳ���
 * @return 		��ɻ���֡���ݸ�ʽ�󳤶�
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
	
	// ��Ϣ���ȸ�λ��ǰ
	*pStr++ = HIBYTE(infolen);
	*pStr++ = LOBYTE(infolen);

	// Դ��ַ��Ϣ
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

	 // Ŀ���ַ��Ϣ
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
 * @brief		���10 02 ptCIR2.0 10 03���ݸ�ʽ
 * @param[out]	FrameAr       ���������
 * @param[in]	FrameLen      ��FrameAr����
 * @param[in]	op            ��ҵ������
 * @param[in]	cmd           ������
 * @param[in]	sport         ��Դ�˿�
 * @param[in]	saddrlen      ��Դ��ַ����
 * @param[in]	saddr         ��Դ��ַ
 * @param[in]	dport         ��Ŀ�Ķ˿�
 * @param[in]	daddrlen      ��Ŀ�ĵ�ַ����
 * @param[in]	daddr         ��Ŀ�ĵ�ַ
 * @param[in]	data          ��Ҫ���֡��ʽ����Ч����
 * @param[in]	datalen       ����Ч���ݳ���
 * @return 		������ݳ���, ��������10
 **********************************************************************/
/*** �˺����ڲ������ѱ�1k����ʱ������ʹÿ��������������ջ���� ***/
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
 * @brief		���10 02 ptCIR2.0 10 03�������ݸ�ʽ
 * @param[out]	FrameAr       ���������
 * @param[in]	FrameLen      ��FrameAr����
 * @param[in]	ptCIR20       ��ptCIR20����֡
 * @param[in]	datalen       ��ptCIR20����֡����
 * @return 		������ݳ���, ��������10
 **********************************************************************/
/*** �˺����ڲ������ѱ�1k����ʱ������ʹÿ��������������ջ���� ***/
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
 * @brief		���ptCIR3.0֡��ʽ
 * @param[out]	FrameAr       ����ɻ���֡���ݸ�ʽ
 * @param[in]	FrameLen      ��FrameAr����
 * @param[in]	op            ��ҵ������
 * @param[in]	cmd           ������
 * @param[in]	sport         ��Դ�˿�
 * @param[in]	saddrlen      ��Դ��ַ����
 * @param[in]	saddr         ��Դ��ַ
 * @param[in]	dport         ��Ŀ�Ķ˿�
 * @param[in]	daddrlen      ��Ŀ�ĵ�ַ����
 * @param[in]	daddr         ��Ŀ�ĵ�ַ
 * @param[in]	data          ��Ҫ���֡��ʽ����Ч����
 * @param[in]	datalen       ����Ч���ݳ���
 * @param[in]	bAck       	  ���Ƿ���Ӧ��
 * @param[in]	bResend       ���Ƿ��ط�
 * @param[in]	iSN           ����ˮ��
 * @return 		��ɻ���֡���ݸ�ʽ�󳤶�
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
	
	// ֡ͷ
	*pStr++ = 0x40 | ((bResend == TRUE) ? 0x20 : 0x00) | ((bAck == TRUE) ? 0x10 : 0x00) | 0x00 | (iSN & 0x07);
	
	// ��Ϣ���ȸ�λ��ǰ
	*pStr++ = HIBYTE(infolen);
	*pStr++ = LOBYTE(infolen);

	// Դ��ַ��Ϣ
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

	 // Ŀ���ַ��Ϣ
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
 * @brief		���10 02 ptCIR3.0 10 03���ݸ�ʽ
 * @param[out]	FrameAr       ���������
 * @param[in]	FrameLen      ��FrameAr����
 * @param[in]	op            ��ҵ������
 * @param[in]	cmd           ������
 * @param[in]	sport         ��Դ�˿�
 * @param[in]	saddrlen      ��Դ��ַ����
 * @param[in]	saddr         ��Դ��ַ
 * @param[in]	dport         ��Ŀ�Ķ˿�
 * @param[in]	daddrlen      ��Ŀ�ĵ�ַ����
 * @param[in]	daddr         ��Ŀ�ĵ�ַ
 * @param[in]	data          ��Ҫ���֡��ʽ����Ч����
 * @param[in]	datalen       ����Ч���ݳ���
 * @param[in]	bAck       	  ���Ƿ���Ӧ��
 * @param[in]	bResend       ���Ƿ��ط�
 * @param[in]	iSN           ����ˮ��
 * @return 		������ݳ���
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
 * @brief		���10 02 pbCIR3.0 10 03�������ݸ�ʽ
 * @param[out]	FrameAr       ���������
 * @param[in]	FrameLen      ��FrameAr����
 * @param[in]	ptCIR30       ��ptCIR30֡��ʽ
 * @param[in]	datalen       ��ptCIR30֡��ʽ����
 * @return 		������ݳ���
 **********************************************************************/
/*** �˺����ڲ������ѱ�1k����ʱ������ʹÿ��������������ջ���� ***/
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
 * @brief		pbCIR2.0��pbCIR3.0��psCIR3.0����֡��ʽ��������
 * @param[out]	pTF           ������֡��ʽ���ݽṹ��
 * @param[in]	pBytes        ������֡��ʽ����
 * @param[in]	dwLen         ������֡��ʽ���ݳ���
 * @return 		TRUE/FALSE
 **********************************************************************/
/*** �˺����ڲ������ѱ�1k����ʱ������ʹÿ��������������ջ���� ***/
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





