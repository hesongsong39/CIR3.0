/************************************************************
  Copyright (C), 2016, 712.Co.,Ltd.
  FileName: ptcan.c
  Author:       
  Description: 
  Version:V1.0 
  Function List:   
  1.
  History:        
  <author>    <time>       <version >   <desc>
   frank      16.03.16       1.0    
***********************************************************/

#include <string.h>
#include "ptcan.h"


//ͳ���������
int g_iRecvSuccessTotalCount = 0;
int g_iRecvErrorTotalCount = 0;
int g_iSendSuccessTotalCount = 0;
int g_iSendErrorTotalCount = 0;

unsigned int g_dwRecvTimeOut = 5000; //���ճ�ʱʱ�䣬��λms
int g_bRecvTimeOutEnabled = 0; //�Ƿ�ʹ�ܽ��ճ�ʱ

unsigned char g_FlowNo = 0; //CAN֡����ˮ��
unsigned char g_FlowNo2 = 0; //CAN֡����ˮ��2


int g_iRecvFrameBufferCount = FRAME_BUFFER_CNT_2048 + FRAME_BUFFER_CNT_1024 + FRAME_BUFFER_CNT_512 + FRAME_BUFFER_CNT_256 +
	FRAME_BUFFER_CNT_128 + FRAME_BUFFER_CNT_64 + FRAME_BUFFER_CNT_32;
FrameBuffer g_RecvFrameBuffer[FRAME_BUFFER_CNT_2048 + FRAME_BUFFER_CNT_1024 + FRAME_BUFFER_CNT_512 + FRAME_BUFFER_CNT_256 +
	FRAME_BUFFER_CNT_128 + FRAME_BUFFER_CNT_64 + FRAME_BUFFER_CNT_32];

CCAN_MSG_STATE (*g_pfSendFrame)(unsigned char CANIndex, CCAN_MSG_OBJECT *pMsgObj, unsigned short iMsgObjCount, unsigned short iTotalMsgObjCount) = 0;
int (*g_pfRecvDataPack)(unsigned char *pData, unsigned short wDataLen, CAN_Param *pCAN_Param) = 0;
unsigned int (*g_pfGetTickCount)(void) = 0;
int (*g_pfGetRecvFrameBufferPtr)(FrameBuffer *pBuffer) = 0;


//��ʼ��CAN����������н��ջ�����������ص�����ָ��
int CanInit(void)
{
	if (g_pfSendFrame == 0)
	{
		return 0;
	}

	if (g_pfRecvDataPack == 0)
	{
		return -1;
	}

	if (g_pfGetTickCount == 0)
	{
		return -2;
	}

	if (g_pfGetRecvFrameBufferPtr == 0)
	{
		return -3;
	}

	if (g_iRecvFrameBufferCount == 0)
	{
		return -4;
	}

	if ((*g_pfGetRecvFrameBufferPtr)(g_RecvFrameBuffer) <= 0)
	{
		return -5;
	}

	CanClearAllRecvBuffer();

	return 1;
}


//���û�ȡϵͳ���к������ص�����
void CanGetTickCountCallbackFunc(unsigned int (*pf)(void))
{
	g_pfGetTickCount = pf;
}


//������dwRecvTimeOut ��ʱʱ��
void CanSetRecvTimeOut(unsigned int dwRecvTimeOut)
{
	g_dwRecvTimeOut = dwRecvTimeOut;
}


//�Ƿ����ý��ճ�ʱ
//������bEnabled �Ƿ����á�0�����ã�1����
void CanSetRecvTimeOutEnabled(int bEnabled)
{
	g_bRecvTimeOutEnabled = bEnabled;
}


//===============================================================������غ���
//����һ��CIR3.0��������(pbCIR2.0)��CAN����
//������pData ָ��CIR3.0��������(pbCIR2.0)��wDataLen Ϊ���ȣ�pCAN_Param ΪһЩ���Ͳ���
//����ֵ: >0��ʾ�ɹ���<=0��ʾʧ��
int CanSendDataPackToCanBus(unsigned char *pData, unsigned short wDataLen, CAN_Param *pCAN_Param)
{
	int i = 0;
	CAN_ID ID; //CAN֡��ID
	int result = 0;
	CCAN_MSG_OBJECT SendBuffer;
	unsigned short wCanFrameCount = 0;
	unsigned short wCurSendCANFrame = 0;

	//�����ݳ��ȡ���ʾ�ӡ���������ʼ����CRC���������ֽڳ��ȡ�
#ifdef USE_BIG_BUFFER
	if ((pCAN_Param->CANIndex > 1) || (pData == 0) || (wDataLen < 9) || (wDataLen > 2024))
	{
		g_iSendErrorTotalCount++;
		return 0;
	}
#else
	if ((pCAN_Param->CANIndex > 1) || (pData == 0) || (wDataLen < 9) || (wDataLen > 724))
	{
		g_iSendErrorTotalCount++;
		return 0;
	}
#endif

	//������Ҫ���͵�CAN֡����
	if (wDataLen % 8 == 0)
	{
		wCanFrameCount = wDataLen / 8;
	} 
	else
	{
		wCanFrameCount = wDataLen / 8 + 1;
	}

	ID.Unused = 0x02; //��ʾ������չ֡
	ID.ZongXianDiZhi = pCAN_Param->ZongXianDiZhi;
	ID.Priority = pCAN_Param->Priority; 
	ID.ProtocolVer = pCAN_Param->ProtocolVer;
	ID.DataType = pCAN_Param->DataType; 
	ID.SrcAddr = pCAN_Param->CANSrcAddr;
	ID.DestAddr = pCAN_Param->CANDestAddr;
	ID.ZhuanFaBiaoZhi = pCAN_Param->ZhuanFaBiaoZhi;

	//���͵�1��CAN֡
	ID.SectionFlag = 0x01;
	if (pCAN_Param->CANIndex == 0)
	{
		ID.FlowNo = g_FlowNo++;
	} 
	else
	{
		ID.FlowNo = g_FlowNo2++;
	}

	SendBuffer.id = *(unsigned int *)&ID;
	SendBuffer.dlc = 8; //���ȹ̶�Ϊ8
	memcpy(SendBuffer.data, pData, SendBuffer.dlc);
	result = (*g_pfSendFrame)(pCAN_Param->CANIndex, &SendBuffer, 1, wCanFrameCount);
	if (result != CCAN_MSG_OK) //�ж��Ƿ���ʧ��
	{
		g_iSendErrorTotalCount++;
		return result;
	}

	wCurSendCANFrame++;

	//�����м��CAN֡
	ID.SectionFlag = 0x02;
	SendBuffer.dlc = 8; //���ȹ̶�Ϊ8

	for (i = 1; i < wCanFrameCount - 1; i++)
	{
		if (pCAN_Param->CANIndex == 0)
		{
			ID.FlowNo = g_FlowNo++;
		} 
		else
		{
			ID.FlowNo = g_FlowNo2++;
		}

		SendBuffer.id = *(unsigned int *)&ID;
		memcpy(SendBuffer.data, &pData[8 * wCurSendCANFrame], SendBuffer.dlc);
		result = (*g_pfSendFrame)(pCAN_Param->CANIndex, &SendBuffer, 1, wCanFrameCount - i);
		if (result != CCAN_MSG_OK) //�ж��Ƿ���ʧ��
		{
			g_iSendErrorTotalCount++;
			return result;
		}
	
		wCurSendCANFrame++;
	}

	//�������1��CAN֡
	ID.SectionFlag = 0x03;
	if (pCAN_Param->CANIndex == 0)
	{
		ID.FlowNo = g_FlowNo++;
	} 
	else
	{
		ID.FlowNo = g_FlowNo2++;
	}

	SendBuffer.id = *(unsigned int *)&ID;
	SendBuffer.dlc = wDataLen - (wCanFrameCount - 1) * 8;
	memcpy(SendBuffer.data, &pData[8 * wCurSendCANFrame], SendBuffer.dlc);
	result = (*g_pfSendFrame)(pCAN_Param->CANIndex, &SendBuffer, 1, 1);
	if (result != CCAN_MSG_OK) //�ж��Ƿ���ʧ��
	{
		g_iSendErrorTotalCount++;
		return result;
	}

	g_iSendSuccessTotalCount++;
	return result;
}


//���÷���CAN֡�ص�����   
//pMsgObj ָ�����е�CAN֡��iMsgObjCount ΪCAN֡��������iTotalMsgObjCount ΪCAN֡��������
void CanSendFrameCallbackFunc(CCAN_MSG_STATE (*pf)(unsigned char CANIndex, CCAN_MSG_OBJECT *pMsgObj, 
	unsigned short iMsgObjCount, unsigned short iTotalMsgObjCount))
{
	g_pfSendFrame = pf;
}


//===============================================================������غ���
//������н��ջ�����
void CanClearAllRecvBuffer(void)
{
	int i = 0;
	for (i = 0; i < g_iRecvFrameBufferCount; i++)
	{
		memset(g_RecvFrameBuffer[i].pBuffer, 0, g_RecvFrameBuffer[i].wBufferLen);
	}
}


//��������5��û��ȫ�ĳ����ľͶ�����
void CanRemoveTimeOutDataPack(void)
{
	int i = 0;
	FrameBuffer_Header *pHeader = 0;

	//���һ���Ƿ��н��ճ�ʱ�ı���
	for (i = 0; i < g_iRecvFrameBufferCount; i++) //�������б��ĵĻ�����
	{
		pHeader = (FrameBuffer_Header *)g_RecvFrameBuffer[i].pBuffer;
		if (pHeader->RecvState == 0x01) //������
		{
			if ((*g_pfGetTickCount)() - pHeader->RecvTime > g_dwRecvTimeOut)
			{
				memset(g_RecvFrameBuffer[i].pBuffer, 0, g_RecvFrameBuffer->wBufferLen);
			}
		}
	}
}


int CanRecvFirstFrame(unsigned char CANIndex, unsigned int ID, unsigned char *pData, unsigned short wDataLen)
{
	int i = 0;
	CAN_ID *pID = (CAN_ID *)&ID;
	unsigned short len = 0;
	FrameBuffer_HeaderData *pHeaderData = 0;

	if ((pData[0] & 0xc0) == 0x40) //3.0֡��ʽ
	{
		len = (((unsigned short)pData[1]) << 8) + pData[2] + sizeof(FrameBuffer_Header);
	}
	else if ((pData[0] & 0xc0) == 0x00) //2.0֡��ʽ
	{
		len = (((unsigned short)pData[0]) << 8) + pData[1] + 2 + sizeof(FrameBuffer_Header);
	}
	else
	{
		return 0;
	}

	for (i = 0; i < g_iRecvFrameBufferCount; i++) //�������Ļ�����
	{
		pHeaderData = (FrameBuffer_HeaderData *)g_RecvFrameBuffer[i].pBuffer;
		if (len > g_RecvFrameBuffer[i].wBufferLen)
		{
			continue;
		}

		if (pHeaderData->Header.RecvState == 0x01) //������
		{
			if ((pHeaderData->Header.SrcAddr == (unsigned char)pID->SrcAddr) //Դ��ַ��ͬ
				&& (pHeaderData->Header.CANIndex == CANIndex))
			{
				//������ǰ���յĶ�������ͷ��ʼ����
				memset(g_RecvFrameBuffer[i].pBuffer, 0, g_RecvFrameBuffer->wBufferLen);
				pHeaderData->Header.RecvTime = (*g_pfGetTickCount)();
				pHeaderData->Header.CANIndex = CANIndex;
				pHeaderData->Header.ZongXianDiZhi = (unsigned char)pID->ZongXianDiZhi;
				pHeaderData->Header.Priority = (unsigned char)pID->Priority;
				pHeaderData->Header.ProtocolVer = (unsigned char)pID->ProtocolVer;
				pHeaderData->Header.DataType = (unsigned char)pID->DataType;
				pHeaderData->Header.SrcAddr = (unsigned char)pID->SrcAddr;
				pHeaderData->Header.DestAddr = (unsigned char)pID->DestAddr;
				pHeaderData->Header.ZhuanFaBiaoZhi = (unsigned char)pID->ZhuanFaBiaoZhi;
				memcpy(&(pHeaderData->Data[pHeaderData->Header.DataLen]), pData, wDataLen);
				pHeaderData->Header.DataLen += wDataLen;
				pHeaderData->Header.RecvState = 0x01; //������
				return 1;
			}
		}
	}

	for (i = 0; i < g_iRecvFrameBufferCount; i++) //�������ĵĻ�����
	{
		pHeaderData = (FrameBuffer_HeaderData *)g_RecvFrameBuffer[i].pBuffer;
		if (len > g_RecvFrameBuffer[i].wBufferLen)
		{
			continue;
		}

		if (pHeaderData->Header.RecvState == 0x00) //�ǿ��еĻ�����
		{
			memset(g_RecvFrameBuffer[i].pBuffer, 0, g_RecvFrameBuffer->wBufferLen);
			pHeaderData->Header.RecvTime = (*g_pfGetTickCount)();
			pHeaderData->Header.CANIndex = CANIndex;
			pHeaderData->Header.ZongXianDiZhi = (unsigned char)pID->ZongXianDiZhi;
			pHeaderData->Header.Priority = (unsigned char)pID->Priority;
			pHeaderData->Header.ProtocolVer = (unsigned char)pID->ProtocolVer;
			pHeaderData->Header.DataType = (unsigned char)pID->DataType;
			pHeaderData->Header.SrcAddr = (unsigned char)pID->SrcAddr;
			pHeaderData->Header.DestAddr = (unsigned char)pID->DestAddr;
			pHeaderData->Header.ZhuanFaBiaoZhi = (unsigned char)pID->ZhuanFaBiaoZhi;
			memcpy(&(pHeaderData->Data[pHeaderData->Header.DataLen]), pData, wDataLen);
			pHeaderData->Header.DataLen += wDataLen;
			pHeaderData->Header.RecvState = 0x01; //������
			return 1;
		}
	}

	return -1;
}


int CanRecvMidFrame(unsigned char CANIndex, unsigned int ID, unsigned char *pData, unsigned short wDataLen)
{
	int i = 0;
	CAN_ID *pID = (CAN_ID *)&ID;
	FrameBuffer_HeaderData *pHeaderData = 0;

	for (i = 0; i < g_iRecvFrameBufferCount; i++) //�������ĵĻ�����
	{
		pHeaderData = (FrameBuffer_HeaderData *)g_RecvFrameBuffer[i].pBuffer;
		if ((pHeaderData->Header.RecvState == 0x01)
			&& (pHeaderData->Header.SrcAddr == (unsigned char)pID->SrcAddr)
			&& (pHeaderData->Header.CANIndex == CANIndex))
		{
			if (pHeaderData->Header.DataLen + wDataLen + sizeof(FrameBuffer_Header) > 
				g_RecvFrameBuffer[i].wBufferLen) //����
			{
				memset(g_RecvFrameBuffer[i].pBuffer, 0, g_RecvFrameBuffer->wBufferLen);
				return 0;
			}

			memcpy(&(pHeaderData->Data[pHeaderData->Header.DataLen]), pData, wDataLen);
			pHeaderData->Header.DataLen += wDataLen;
			return 1;
		}
	}

	return -1;
}


int CanRecvLastFrame(unsigned char CANIndex, unsigned int ID, unsigned char *pData, unsigned short wDataLen)
{
	int i = 0;
	CAN_ID *pID = (CAN_ID *)&ID;
	int result = 0;
	FrameBuffer_HeaderData *pHeaderData = 0;
	CAN_Param Param;
	memset(&Param, 0, sizeof(CAN_Param));

	for (i = 0; i < g_iRecvFrameBufferCount; i++) //�������ĵĻ�����
	{
		pHeaderData = (FrameBuffer_HeaderData *)g_RecvFrameBuffer[i].pBuffer;
		if ((pHeaderData->Header.RecvState == 0x01) 
			&& (pHeaderData->Header.SrcAddr == (unsigned char)pID->SrcAddr)
			&& (pHeaderData->Header.CANIndex == CANIndex))
		{
			if (pHeaderData->Header.DataLen + sizeof(FrameBuffer_Header) > 
				g_RecvFrameBuffer[i].wBufferLen) //����
			{
				memset(g_RecvFrameBuffer[i].pBuffer, 0, g_RecvFrameBuffer->wBufferLen);
				return 0;
			}

			memcpy(&(pHeaderData->Data[pHeaderData->Header.DataLen]), pData, wDataLen);
			pHeaderData->Header.DataLen += wDataLen;
			pHeaderData->Header.RecvState = 0x02; //�������

			Param.CANIndex = CANIndex;
			Param.ZongXianDiZhi = (unsigned char)pID->ZongXianDiZhi;
			Param.CANSrcAddr = (unsigned char)pID->SrcAddr;
			Param.CANDestAddr = (unsigned char)pID->DestAddr;
			Param.DataType = (unsigned char)pID->DataType;
			Param.Priority = (unsigned char)pID->Priority;
			Param.ProtocolVer = (unsigned char)pID->ProtocolVer;
			Param.ZhuanFaBiaoZhi = (unsigned char)pID->ZhuanFaBiaoZhi;
			result = (*g_pfRecvDataPack)(pHeaderData->Data, pHeaderData->Header.DataLen, &Param);
			memset(g_RecvFrameBuffer[i].pBuffer, 0, g_RecvFrameBuffer->wBufferLen);
			if (result <= 0)
			{
				return -2;
			}

			return 1;
		}
	}

	return -3;
}


//���ý������ݰ��ص�����
//������pData ָ��CIR3.0��������(pbCIR2.0)��wDataLen Ϊ���ȣ�pCAN_Param ΪһЩ���Ͳ���
void CanRecvDataPackCallbackFunc(int (*pf)(unsigned char *pData, unsigned short wDataLen, 
	CAN_Param *pCAN_Param))
{
	g_pfRecvDataPack = pf;
}


//�����յ���CAN֡������ջ�����
//pData ָ�����е�CAN֡��iDataLen ΪCAN֡������
//����ֵ: >0��ʾ�ɹ���<=0��ʾʧ��
int CanAddFrameToRecvBuffer(unsigned char CANIndex, CCAN_MSG_OBJECT *pData, int iDataLen)
{
	CAN_ID *pID = 0;
	int i = 0; 
	CAN_Param Param;
	memset(&Param, 0, sizeof(CAN_Param));

	if (g_bRecvTimeOutEnabled == 1)
	{
		CanRemoveTimeOutDataPack(); //ɾ�����ճ�ʱ�ı���
	}

	if ((CANIndex > 1) || (pData == 0) || (iDataLen <= 0))
	{
		return 0;
	}

	for (i = 0; i < iDataLen; i++)
	{
		pID = (CAN_ID *)&pData[i].id;
		if ((pData[i].dlc > 0) && (pID->ProtocolVer == 0x00))
		{
			if (pID->SectionFlag == 0x00) //���ݲ��ֶα��Ǿ���֡
			{
				return -1;
			}
			else //�Ǿ���֡
			{
				if (pID->SectionFlag == 0x01) //�Ǳ��ĵĵ�һ֡
				{
					CanRecvFirstFrame(CANIndex, pData[i].id, pData[i].data, pData[i].dlc);
				}
				else if (pID->SectionFlag == 0x02) //�Ǳ��ĵ��м�֡
				{
					CanRecvMidFrame(CANIndex, pData[i].id, pData[i].data, pData[i].dlc);
				}
				else //�Ǳ��ĵĽ���֡
				{
					if (CanRecvLastFrame(CANIndex, pData[i].id, pData[i].data, pData[i].dlc) == 1)
					{
						g_iRecvSuccessTotalCount++;
					}
					else
					{
						g_iRecvErrorTotalCount++;
					}
				}
			}
		}
	}

	return 1;
}


//���ý�������֡�������ص�����
//������pBuffer ָ���Ž��ջ�����ָ������飬iBufferCount Ϊ����ĸ���
void CanGetRecvFrameBufferPtrCallbackFunc(int (*pf)(FrameBuffer *pBuffer))
{
	g_pfGetRecvFrameBufferPtr = pf;
}







