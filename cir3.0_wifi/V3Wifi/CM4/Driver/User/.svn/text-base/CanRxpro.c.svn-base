/**********************************************************************
* $Id$      CanRxpro              2016-01-07
*//**
* @file     CanRxpro.c
* @brief    串口接收数据处理
* @version  1.0.0
* @date     2016/01/07 星期四 14:28:44
* @author   ZhangYanMing
*
* Copyright(C) 2016, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
/**************************** Includes **********************************/
#include "lpc_types.h"
#include <ucos_ii.h>
#include <string.h>
#include "Task.h"
#include "C_CAN.h"
#include "CanRxpro.h"
#include "ptcan.h"
#include "DataTransfer.h"

/*****************Private variables ***********************************/
static  uint8_t  g_CanRxbuf[1024];

/*****************Public variables ************************************/


/*****************Extern variables ************************************/




/*********************************************************************//**
 * @brief       CAN接口数据队列处理函数
 * @param[in]   None
 * @return      None
 **********************************************************************/
void CCAN_QueueProc(void)
{
		CAN_Frame msg_buf;
		while(CANRxqueue.pwrite != CANRxqueue.pread)
		{
				memcpy(&msg_buf,&CANRxqueue.pmsg[CANRxqueue.pread],sizeof(CAN_Frame));
				CANRxqueue.pread++;
				if(CANRxqueue.pread >= CCANQUEUESIZE)
				{
					CANRxqueue.pread = 0;
				}
				CanAddFrameToRecvBuffer(msg_buf.DeviceIndex,&msg_buf.Canmsg,1);				
		}
        
}


/*********************************************************************//**
 * @brief       CAN协议处理转换单元回调，将CAN帧发送至业务处理任务
 * @param[in]   
 * @return      None
 **********************************************************************/
static int CanRecvDataPackCallbackFuncFn(unsigned char *pData, unsigned short wDataLen, 
	CAN_Param *pCAN_Param)
{
		CCANItem *pTI = (CCANItem *)g_CanRxbuf;
		uint16_t   rxlen = 0;
	
		switch(pCAN_Param->CANIndex)
		{
				case DEVICE_C_CAN0:
					{
							pCAN_Param->CANIndex = INTERFACE_CAN1;
					}
					break;
				case DEVICE_C_CAN1:
					{
							pCAN_Param->CANIndex = INTERFACE_CAN2;
					}
					break;
				default:
					break;
		}
		//先将缓冲清零
		memset(g_CanRxbuf,0,sizeof(g_CanRxbuf)); 
		memcpy((uint8_t *)(&pTI->CanParam),(uint8_t *)pCAN_Param,sizeof(CAN_Param));
		pTI->Infolen = wDataLen;
		memcpy(pTI->Info,pData,wDataLen);
		rxlen = sizeof(CAN_Param) + 2 + pTI->Infolen;
		
		if(MQ_AddTail(g_MsgDispatch,pCAN_Param->CANIndex,g_CanRxbuf,rxlen) == TRUE)
		{
				return 1;
		}
		return 0;
}


unsigned char g_FrameBuffer_32[FRAME_BUFFER_CNT_32][32];
unsigned char g_FrameBuffer_64[FRAME_BUFFER_CNT_64][64];
unsigned char g_FrameBuffer_128[FRAME_BUFFER_CNT_128][128];
unsigned char g_FrameBuffer_256[FRAME_BUFFER_CNT_256][256];
unsigned char g_FrameBuffer_512[FRAME_BUFFER_CNT_512][512];
unsigned char g_FrameBuffer_1024[FRAME_BUFFER_CNT_1024][1024];

#ifdef USE_BIG_BUFFER
unsigned char g_FrameBuffer_2048[FRAME_BUFFER_CNT_2048][2048];
#endif

/*********************************************************************//**
 * @brief       CAN协议处理转换单元回调，获取接收缓冲区的指针
 * @param[in]   
 * @return      None
 **********************************************************************/
int CanGetRecvFrameBufferPtrCallbackFuncFn(FrameBuffer *pBuffer)
{
    int i = 0, j = 0;
	for (i = 0; i < FRAME_BUFFER_CNT_32; i++)
	{
		pBuffer[j].pBuffer = g_FrameBuffer_32[i];
		pBuffer[j++].wBufferLen = 32;
	}

	for (i = 0; i < FRAME_BUFFER_CNT_64; i++)
	{
		pBuffer[j].pBuffer = g_FrameBuffer_64[i];
		pBuffer[j++].wBufferLen = 64;
	}

	for (i = 0; i < FRAME_BUFFER_CNT_128; i++)
	{
		pBuffer[j].pBuffer = g_FrameBuffer_128[i];
		pBuffer[j++].wBufferLen = 128;
	}

	for (i = 0; i < FRAME_BUFFER_CNT_256; i++)
	{
		pBuffer[j].pBuffer = g_FrameBuffer_256[i];
		pBuffer[j++].wBufferLen = 256;
	}

	for (i = 0; i < FRAME_BUFFER_CNT_512; i++)
	{
		pBuffer[j].pBuffer = g_FrameBuffer_512[i];
		pBuffer[j++].wBufferLen = 512;
	}

	for (i = 0; i < FRAME_BUFFER_CNT_1024; i++)
	{
		pBuffer[j].pBuffer = g_FrameBuffer_1024[i];
		pBuffer[j++].wBufferLen = 1024;
	}
	
#ifdef USE_BIG_BUFFER
	for (i = 0; i < FRAME_BUFFER_CNT_2048; i++)
	{
		pBuffer[j].pBuffer = g_FrameBuffer_2048[i];
		pBuffer[j++].wBufferLen = 2048;
	}
#endif
	
	return 1;
}



/********************************************************************//**
 * @brief 		CAN板级层相关初始化
 * @param[in]	None
 * @return 		None
 *********************************************************************/
void Board_Can_Init(void)
{
    memset(&CANRxqueue,0,sizeof(CANRxqueue));

    //协议转换功能初始化
    CanGetTickCountCallbackFunc(OSTimeGet);
    CanSetRecvTimeOutEnabled(1);
    CanSetRecvTimeOut(5000);
		CanSendFrameCallbackFunc(CANSend);
		CanRecvDataPackCallbackFunc(CanRecvDataPackCallbackFuncFn);
		CanGetRecvFrameBufferPtrCallbackFunc(CanGetRecvFrameBufferPtrCallbackFuncFn);
		if (CanInit() <= 0)
		{
			return;
		}
}

