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


//统计相关内容
int g_iRecvSuccessTotalCount = 0;
int g_iRecvErrorTotalCount = 0;
int g_iSendSuccessTotalCount = 0;
int g_iSendErrorTotalCount = 0;

unsigned int g_dwRecvTimeOut = 5000; //接收超时时间，单位ms
int g_bRecvTimeOutEnabled = 0; //是否使能接收超时

unsigned char g_FlowNo = 0; //CAN帧的流水号
unsigned char g_FlowNo2 = 0; //CAN帧的流水号2


int g_iRecvFrameBufferCount = FRAME_BUFFER_CNT_2048 + FRAME_BUFFER_CNT_1024 + FRAME_BUFFER_CNT_512 + FRAME_BUFFER_CNT_256 +
	FRAME_BUFFER_CNT_128 + FRAME_BUFFER_CNT_64 + FRAME_BUFFER_CNT_32;
FrameBuffer g_RecvFrameBuffer[FRAME_BUFFER_CNT_2048 + FRAME_BUFFER_CNT_1024 + FRAME_BUFFER_CNT_512 + FRAME_BUFFER_CNT_256 +
	FRAME_BUFFER_CNT_128 + FRAME_BUFFER_CNT_64 + FRAME_BUFFER_CNT_32];

CCAN_MSG_STATE (*g_pfSendFrame)(unsigned char CANIndex, CCAN_MSG_OBJECT *pMsgObj, unsigned short iMsgObjCount, unsigned short iTotalMsgObjCount) = 0;
int (*g_pfRecvDataPack)(unsigned char *pData, unsigned short wDataLen, CAN_Param *pCAN_Param) = 0;
unsigned int (*g_pfGetTickCount)(void) = 0;
int (*g_pfGetRecvFrameBufferPtr)(FrameBuffer *pBuffer) = 0;


//初始化CAN，将清空所有接收缓冲区，清除回调函数指针
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


//设置获取系统运行毫秒数回调函数
void CanGetTickCountCallbackFunc(unsigned int (*pf)(void))
{
	g_pfGetTickCount = pf;
}


//参数：dwRecvTimeOut 超时时间
void CanSetRecvTimeOut(unsigned int dwRecvTimeOut)
{
	g_dwRecvTimeOut = dwRecvTimeOut;
}


//是否启用接收超时
//参数：bEnabled 是否启用。0不启用，1启用
void CanSetRecvTimeOutEnabled(int bEnabled)
{
	g_bRecvTimeOutEnabled = bEnabled;
}


//===============================================================发送相关函数
//发送一个CIR3.0基础数据(pbCIR2.0)到CAN总线
//参数：pData 指向CIR3.0基础数据(pbCIR2.0)，wDataLen 为长度，pCAN_Param 为一些发送参数
//返回值: >0表示成功，<=0表示失败
int CanSendDataPackToCanBus(unsigned char *pData, unsigned short wDataLen, CAN_Param *pCAN_Param)
{
	int i = 0;
	CAN_ID ID; //CAN帧的ID
	int result = 0;
	CCAN_MSG_OBJECT SendBuffer;
	unsigned short wCanFrameCount = 0;
	unsigned short wCurSendCANFrame = 0;

	//“数据长度”表示从“特征”开始到“CRC”结束的字节长度。
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

	//计算需要发送的CAN帧数量
	if (wDataLen % 8 == 0)
	{
		wCanFrameCount = wDataLen / 8;
	} 
	else
	{
		wCanFrameCount = wDataLen / 8 + 1;
	}

	ID.Unused = 0x02; //表示发送扩展帧
	ID.ZongXianDiZhi = pCAN_Param->ZongXianDiZhi;
	ID.Priority = pCAN_Param->Priority; 
	ID.ProtocolVer = pCAN_Param->ProtocolVer;
	ID.DataType = pCAN_Param->DataType; 
	ID.SrcAddr = pCAN_Param->CANSrcAddr;
	ID.DestAddr = pCAN_Param->CANDestAddr;
	ID.ZhuanFaBiaoZhi = pCAN_Param->ZhuanFaBiaoZhi;

	//发送第1个CAN帧
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
	SendBuffer.dlc = 8; //长度固定为8
	memcpy(SendBuffer.data, pData, SendBuffer.dlc);
	result = (*g_pfSendFrame)(pCAN_Param->CANIndex, &SendBuffer, 1, wCanFrameCount);
	if (result != CCAN_MSG_OK) //判断是否发送失败
	{
		g_iSendErrorTotalCount++;
		return result;
	}

	wCurSendCANFrame++;

	//发送中间的CAN帧
	ID.SectionFlag = 0x02;
	SendBuffer.dlc = 8; //长度固定为8

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
		if (result != CCAN_MSG_OK) //判断是否发送失败
		{
			g_iSendErrorTotalCount++;
			return result;
		}
	
		wCurSendCANFrame++;
	}

	//发送最后1个CAN帧
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
	if (result != CCAN_MSG_OK) //判断是否发送失败
	{
		g_iSendErrorTotalCount++;
		return result;
	}

	g_iSendSuccessTotalCount++;
	return result;
}


//设置发送CAN帧回调函数   
//pMsgObj 指向所有的CAN帧，iMsgObjCount 为CAN帧的数量，iTotalMsgObjCount 为CAN帧的总数量
void CanSendFrameCallbackFunc(CCAN_MSG_STATE (*pf)(unsigned char CANIndex, CCAN_MSG_OBJECT *pMsgObj, 
	unsigned short iMsgObjCount, unsigned short iTotalMsgObjCount))
{
	g_pfSendFrame = pf;
}


//===============================================================接收相关函数
//清空所有接收缓冲区
void CanClearAllRecvBuffer(void)
{
	int i = 0;
	for (i = 0; i < g_iRecvFrameBufferCount; i++)
	{
		memset(g_RecvFrameBuffer[i].pBuffer, 0, g_RecvFrameBuffer[i].wBufferLen);
	}
}


//丢弃超过5秒没收全的长报文就丢弃了
void CanRemoveTimeOutDataPack(void)
{
	int i = 0;
	FrameBuffer_Header *pHeader = 0;

	//检查一下是否有接收超时的报文
	for (i = 0; i < g_iRecvFrameBufferCount; i++) //遍历所有报文的缓冲区
	{
		pHeader = (FrameBuffer_Header *)g_RecvFrameBuffer[i].pBuffer;
		if (pHeader->RecvState == 0x01) //接收中
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

	if ((pData[0] & 0xc0) == 0x40) //3.0帧格式
	{
		len = (((unsigned short)pData[1]) << 8) + pData[2] + sizeof(FrameBuffer_Header);
	}
	else if ((pData[0] & 0xc0) == 0x00) //2.0帧格式
	{
		len = (((unsigned short)pData[0]) << 8) + pData[1] + 2 + sizeof(FrameBuffer_Header);
	}
	else
	{
		return 0;
	}

	for (i = 0; i < g_iRecvFrameBufferCount; i++) //遍历报文缓冲区
	{
		pHeaderData = (FrameBuffer_HeaderData *)g_RecvFrameBuffer[i].pBuffer;
		if (len > g_RecvFrameBuffer[i].wBufferLen)
		{
			continue;
		}

		if (pHeaderData->Header.RecvState == 0x01) //接收中
		{
			if ((pHeaderData->Header.SrcAddr == (unsigned char)pID->SrcAddr) //源地址相同
				&& (pHeaderData->Header.CANIndex == CANIndex))
			{
				//丢弃以前接收的东西，从头开始接收
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
				pHeaderData->Header.RecvState = 0x01; //接收中
				return 1;
			}
		}
	}

	for (i = 0; i < g_iRecvFrameBufferCount; i++) //遍历报文的缓冲区
	{
		pHeaderData = (FrameBuffer_HeaderData *)g_RecvFrameBuffer[i].pBuffer;
		if (len > g_RecvFrameBuffer[i].wBufferLen)
		{
			continue;
		}

		if (pHeaderData->Header.RecvState == 0x00) //是空闲的缓冲区
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
			pHeaderData->Header.RecvState = 0x01; //接收中
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

	for (i = 0; i < g_iRecvFrameBufferCount; i++) //遍历报文的缓冲区
	{
		pHeaderData = (FrameBuffer_HeaderData *)g_RecvFrameBuffer[i].pBuffer;
		if ((pHeaderData->Header.RecvState == 0x01)
			&& (pHeaderData->Header.SrcAddr == (unsigned char)pID->SrcAddr)
			&& (pHeaderData->Header.CANIndex == CANIndex))
		{
			if (pHeaderData->Header.DataLen + wDataLen + sizeof(FrameBuffer_Header) > 
				g_RecvFrameBuffer[i].wBufferLen) //超长
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

	for (i = 0; i < g_iRecvFrameBufferCount; i++) //遍历报文的缓冲区
	{
		pHeaderData = (FrameBuffer_HeaderData *)g_RecvFrameBuffer[i].pBuffer;
		if ((pHeaderData->Header.RecvState == 0x01) 
			&& (pHeaderData->Header.SrcAddr == (unsigned char)pID->SrcAddr)
			&& (pHeaderData->Header.CANIndex == CANIndex))
		{
			if (pHeaderData->Header.DataLen + sizeof(FrameBuffer_Header) > 
				g_RecvFrameBuffer[i].wBufferLen) //超长
			{
				memset(g_RecvFrameBuffer[i].pBuffer, 0, g_RecvFrameBuffer->wBufferLen);
				return 0;
			}

			memcpy(&(pHeaderData->Data[pHeaderData->Header.DataLen]), pData, wDataLen);
			pHeaderData->Header.DataLen += wDataLen;
			pHeaderData->Header.RecvState = 0x02; //接收完成

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


//设置接收数据包回调函数
//参数：pData 指向CIR3.0基础数据(pbCIR2.0)，wDataLen 为长度，pCAN_Param 为一些发送参数
void CanRecvDataPackCallbackFunc(int (*pf)(unsigned char *pData, unsigned short wDataLen, 
	CAN_Param *pCAN_Param))
{
	g_pfRecvDataPack = pf;
}


//将接收到的CAN帧填入接收缓冲区
//pData 指向所有的CAN帧，iDataLen 为CAN帧的数量
//返回值: >0表示成功，<=0表示失败
int CanAddFrameToRecvBuffer(unsigned char CANIndex, CCAN_MSG_OBJECT *pData, int iDataLen)
{
	CAN_ID *pID = 0;
	int i = 0; 
	CAN_Param Param;
	memset(&Param, 0, sizeof(CAN_Param));

	if (g_bRecvTimeOutEnabled == 1)
	{
		CanRemoveTimeOutDataPack(); //删除接收超时的报文
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
			if (pID->SectionFlag == 0x00) //数据不分段便是精简帧
			{
				return -1;
			}
			else //非精简帧
			{
				if (pID->SectionFlag == 0x01) //是报文的第一帧
				{
					CanRecvFirstFrame(CANIndex, pData[i].id, pData[i].data, pData[i].dlc);
				}
				else if (pID->SectionFlag == 0x02) //是报文的中间帧
				{
					CanRecvMidFrame(CANIndex, pData[i].id, pData[i].data, pData[i].dlc);
				}
				else //是报文的结束帧
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


//设置接收数据帧缓冲区回调函数
//参数：pBuffer 指向存放接收缓冲区指针的数组，iBufferCount 为数组的个数
void CanGetRecvFrameBufferPtrCallbackFunc(int (*pf)(FrameBuffer *pBuffer))
{
	g_pfGetRecvFrameBufferPtr = pf;
}







