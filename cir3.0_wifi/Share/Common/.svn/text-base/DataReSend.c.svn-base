/**********************************************************************
* $Id$      DataReSend              2016-05-25
*//**
* @file     DataReSend.c
* @brief    数据重发处理
* @version  1.0.0
* @date     2016/05/25 星期三 14:28:44
* @author   ZhangYanMing
*
* Copyright(C) 2016, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
#include "lpc_types.h"
#include <string.h>
#include <ucos_ii.h>
#include "Task.h"
#include "DataReSend.h"
#include "MemManage.h"
#include "DataTransfer.h"

/*****************Public variables ************************************/


/*****************Private variables ***********************************/
static  Wait_Ack_Event g_WAE[WAE_QUEUE_SIZE]; 	    // 重发队列

/*****************Extern variables ************************************/



/********************************************************************//**
 * @brief 		初始化重发用的结构
 * @param[in]	*pWAE，指向数据应答结构体的指针
 * @return 		None
 *********************************************************************/
void WAE_Init(Wait_Ack_Event *pWAE)
{
	if(pWAE == NULL)
	{
			return ;
	}
	memset(pWAE, 0, sizeof(*pWAE));
}

/********************************************************************//**
 * @brief 		设置重发消息及参数
 * @param[in]	*pWAE，指向数据应答结构体的指针
 * @param[in]  nDevNum     端口号
 * @param[in]  sdData      报文
 * @param[in]  sdLen       报文长度
 * @param[in]  dwCnt       数据发送次数
 * @param[in]  cbop        应答的业务
 * @param[in]  data        应答的命令
 * @param[in]  ackdata     应答的数据
 * @param[in]  cbLen       应答的数据长度
 * @param[in]  timeout     重发时间间隔
 * @return 		FALSE,未成功将发送数据添加到发送消息队列
							TRUE,成功将发送数据添加到发送消息队列
 *********************************************************************/
static Bool WAE_SetEvent(Wait_Ack_Event *pWAE,uint8_t nDevNum, uint16_t port,uint8_t cbOp, void *data,  uint32_t cbLen,ResendImport *pImport,
                  void *sdData, uint32_t sdLen, uint32_t dwCnt,uint32_t timeout)
{
	uint8_t *pBlock = NULL;
	
	if((pWAE == NULL)||(data == NULL)||(pImport == NULL))
	{
			return FALSE;
	}
	pBlock = MemApply(sdLen);
	if(!pBlock)
	{
			return FALSE;
	}
	
	pWAE->portnum = nDevNum;
	pWAE->remport = port;
	pWAE->m_cbOp = cbOp;
	pWAE->m_cbLen = cbLen;

	if(pWAE->m_cbLen > 0)
	{
		memcpy(pWAE->m_data, data, pWAE->m_cbLen);
	}
	
	memcpy(&pWAE->m_FrameHead,pImport,sizeof(pWAE->m_FrameHead));

	pWAE->m_cbSendLen = sdLen;
	pWAE->m_SendData = pBlock;
	memcpy(pBlock, sdData, pWAE->m_cbSendLen);
	pWAE->m_dwCnt = dwCnt;
	pWAE->m_dwLastTick = OSTimeGet();
	pWAE->m_dwStartTick = OSTimeGet();
	pWAE->m_timeout = timeout;
	
	return TRUE;
}

/********************************************************************//**
 * @brief 		重发完毕后释放相应队列空间
 * @param[in]	*pWAE，指向数据应答结构体的指针
 * @return 		None
 *********************************************************************/
static void WAE_FreeEvent(Wait_Ack_Event *pWAE)
{
	if(pWAE == NULL)
	{
			return ;
	}
	//释放缓存重发数据的内存
	MemFree(pWAE->m_SendData);
	memset(pWAE, 0, sizeof(*pWAE));
}


/********************************************************************//**
 * @brief 		判断接收的消息是否与等待的消息匹配
 * @param[in]  cbop        应答的业务
 * @param[in]  cbCmd       应答的命令
 * @param[in]  data        应答的数据
 * @param[in]  cbLen       应答的数据长度
 * @return 		FALSE: 接收到的消息与等待消息不匹配
							TRUE:  接受到的消息与等待消息匹配
 *********************************************************************/
Bool WAE_CheckPair(uint8_t cbOp, uint8_t cbCmd, uint8_t *data, uint8_t cbLen)
{
	uint8_t i = 0;
	Bool  ret = FALSE;
	
	OSSchedLock();             /* 关调度保护 */
	for (i = 0; i < sizeof(g_WAE)/sizeof(g_WAE[0]) ; i++)
	{
		//	为了防止业务、命令相同，加上判断端口号
		if((cbOp == g_WAE[i].m_cbOp) && (cbLen == g_WAE[i].m_cbLen) && (!memcmp(data, g_WAE[i].m_data, cbLen)))
		{
			WAE_FreeEvent(&g_WAE[i]);
			ret = TRUE;
			break;
		}
	}
	OSSchedUnlock();           /* 解除关调度保护 */

	return ret;
}

/********************************************************************//**
 * @brief 		重发消息
 * @param[in]	*pWAE，指向数据应答结构体的指针
 * @return 		None
 *********************************************************************/
static Bool WAE_DataRevise(Wait_Ack_Event *pWAE,uint8_t *pdata,uint8_t datalen)
{
		if(pWAE == NULL)
		{
				return FALSE;
		}
		
		pWAE->m_cbLen = datalen;

		if(pWAE->m_cbLen > 0)
		{
			memcpy(pWAE->m_data, pdata, pWAE->m_cbLen);
		}
		
		return TRUE;
}


/********************************************************************//**
 * @brief 		重发消息
 * @param[in]	*pWAE，指向数据应答结构体的指针
 * @return 		None
 *********************************************************************/
static Bool WAE_DataReSend(Wait_Ack_Event *pWAE,uint8_t *txbuf,uint16_t plen)
{
		Bool  ret = FALSE;
		uint8_t want[2] = {0};
		if(pWAE == NULL)
		{
				return ret;
		}
		memset(txbuf,0,plen);
		switch(pWAE->m_FrameHead.iCIRType & 0x0F)
		{
			case PACK_CIR30:
				{
						//CIR3.0协议重发，每次需修改业务帧流水号
						switch(pWAE->m_FrameHead.iCIRType & 0xF0)
						{
								case PERIPH_CAN:
									{
											//CAN
											ret = CCANDataSendPost(txbuf,plen,pWAE->portnum,pWAE->m_FrameHead.iOp,pWAE->m_FrameHead.iCmd,pWAE->m_FrameHead.iSport,pWAE->m_FrameHead.iSaddrlen,
															pWAE->m_FrameHead.szSaddr,pWAE->m_FrameHead.iDport,pWAE->m_FrameHead.iDaddrlen,pWAE->m_FrameHead.szDaddr,pWAE->m_SendData,pWAE->m_cbSendLen,TRUE,TRUE);
									}
									break;
								case PERIPH_UART:
									{
											//UART
											ret = UARTDatapbCIR30SendPost(txbuf,plen,pWAE->portnum,pWAE->m_FrameHead.iOp,pWAE->m_FrameHead.iCmd,pWAE->m_FrameHead.iSport,pWAE->m_FrameHead.iSaddrlen,
															pWAE->m_FrameHead.szSaddr,pWAE->m_FrameHead.iDport,pWAE->m_FrameHead.iDaddrlen,pWAE->m_FrameHead.szDaddr,pWAE->m_SendData,pWAE->m_cbSendLen,TRUE,TRUE);
									}
									break;
								case PERIPH_ETHERNET:
									{
											//Ethernet
											ret = EthernetDataSendPost(txbuf,plen,pWAE->remport,pWAE->m_FrameHead.iOp,pWAE->m_FrameHead.iCmd,pWAE->m_FrameHead.iSport,pWAE->m_FrameHead.iSaddrlen,
														pWAE->m_FrameHead.szSaddr,pWAE->m_FrameHead.iDport,pWAE->m_FrameHead.iDaddrlen,pWAE->m_FrameHead.szDaddr,pWAE->m_SendData,pWAE->m_cbSendLen,TRUE,TRUE);
									}
									break;
								default:
									break;
						}
						//每次发送需要更改重发结构体中的匹配数据
						want[0] = pWAE->m_FrameHead.iCmd;
						want[1] = FlowNumGet();
						WAE_DataRevise(pWAE,want,sizeof(want));
				}
				break;
			case PACK_CIR20:
				{
						//CIR2.0协议重发
						ret = UARTDatapbCIR20SendPost(txbuf,plen,pWAE->portnum,pWAE->m_FrameHead.iOp,pWAE->m_FrameHead.iCmd,pWAE->m_FrameHead.iSport,pWAE->m_FrameHead.iSaddrlen,
										pWAE->m_FrameHead.szSaddr,pWAE->m_FrameHead.iDport,pWAE->m_FrameHead.iDaddrlen,pWAE->m_FrameHead.szDaddr,pWAE->m_SendData,pWAE->m_cbSendLen,TRUE);
				}
				break;
			default:
				break;
		}
		
		return ret;
}

/********************************************************************//**
 * @brief 		重发消息
 * @param[in]	*pWAE，指向数据应答结构体的指针
 * @return 		None
 *********************************************************************/
static void WAE_ReSend(Wait_Ack_Event *pWAE,uint8_t *txbuf,uint16_t plen)
{
	if (pWAE->m_dwCnt > 0)
	{
		WAE_DataReSend(pWAE,txbuf,plen);
		pWAE->m_dwCnt--;
		pWAE->m_dwLastTick = OSTimeGet();

		if (0 == pWAE->m_dwCnt)
		{
			WAE_FreeEvent(pWAE);
		}
	}
}



/********************************************************************//**
 * @brief 		如存在等待消息并且满足条件则重发消息
 * @param[in]	None
 * @return 		None
 *********************************************************************/
void DataReSend_Proc (uint8_t *txbuf,uint16_t plen)
{
	uint32_t dwTick = OSTimeGet();
	uint32_t tempTick = OSTimeGet();
	uint8_t i = 0 , j = 0 , ResendNum = 0;
	
	OSSchedLock();             /* 关调度保护 */

	//每次在重发队列中寻找进入队列最早的报文数据发送
	for (i = 0; i < sizeof(g_WAE)/sizeof(g_WAE[0]) ; i++)
	{
		ResendNum = sizeof(g_WAE)/sizeof(g_WAE[0]);
		for (j = 0; j < sizeof(g_WAE)/sizeof(g_WAE[0]) ; j++)
		{
				if (g_WAE[j].m_dwCnt > 0)
				{
					if (g_WAE[j].m_dwLastTick + g_WAE[j].m_timeout < dwTick)
					{
						if(g_WAE[j].m_dwStartTick < tempTick)
						{
								tempTick = g_WAE[j].m_dwStartTick;
								ResendNum = j;
						}
					}
				}
		}
		if(ResendNum < sizeof(g_WAE)/sizeof(g_WAE[0]))
		{
				WAE_ReSend(&g_WAE[ResendNum],txbuf, plen);
		}
	}
	
	OSSchedUnlock();           /* 解除关调度保护 */
}


/********************************************************************//**
 * @brief 		初始化重发用的结构
 * @param[in]	fFnDataSend，数据发送函数指针
 * @return 		None
 *********************************************************************/
Bool DataReSend_Init(void)
{
	uint8_t i = 0;
	OSSchedLock();            /* 关调度保护 */
	for (i = 0; i < sizeof(g_WAE)/sizeof(g_WAE[0]) ; i++)
	{
		WAE_Init(&g_WAE[i]);
	}
	OSSchedUnlock();          /* 解除关调度保护 */
	return TRUE;
}

/********************************************************************//**
 * @brief 		设置相关重发数据结构
 * @param[in]  nDevNum     端口号
 * @param[in]  sdData      报文
 * @param[in]  sdLen       报文长度
 * @param[in]  nSendCnt    数据发送次数
 * @param[in]  cbop        应答的业务
 * @param[in]  cbcmd       应答的命令
 * @param[in]  pdata       应答的数据
 * @param[in]  acklen      应答的数据长度
 * @param[in]  timeout     重发时间间隔
 * @return 		FALSE,未成功将发送数据添加到发送消息队列
							TRUE,成功将发送数据添加到发送消息队列
 *********************************************************************/
Bool DataReSend_SetEvent(uint8_t pType,uint8_t nDevNum,uint16_t port,uint8_t nReSendCnt,uint8_t Flownum,uint8_t op,uint8_t cmd, uint8_t sport,uint8_t saddrlen,uint8_t *saddr,uint8_t dport,uint8_t daddrlen,uint8_t *daddr,uint8_t *pdata,uint16_t datalen,uint32_t timeout)
{
	uint8_t i = 0;
	uint8_t want[2] = {0};
	ResendImport t_FrameHead;
	Bool  ret = FALSE;
	
	memset(&t_FrameHead,0,sizeof(t_FrameHead));
	
	switch(pType & 0x0F)
	{
		case PACK_CIR30:
			{
					want[0] = cmd;
					want[1] = Flownum;
			}
			break;
		case PACK_CIR20:
			{
					want[0] = cmd;
					want[1] = datalen >0 ? pdata[0] : 0xFF  ;
			}
			break;
		default:
			break;
	}
	
	t_FrameHead.iCIRType = pType;
	t_FrameHead.iOp = op;
	t_FrameHead.iCmd = cmd;
	t_FrameHead.iSport = sport;
	t_FrameHead.iSaddrlen = saddrlen;
	memcpy(t_FrameHead.szSaddr,saddr,saddrlen);
	
	t_FrameHead.iDport = dport;
	t_FrameHead.iDaddrlen = daddrlen;
	memcpy(t_FrameHead.szDaddr,daddr,daddrlen);
	
	OSSchedLock();             /* 关调度保护 */
	
	for (i = 0; i < sizeof(g_WAE)/sizeof(g_WAE[0]) ; i++)
	{
		if (g_WAE[i].m_dwCnt == 0)
		{
			if(WAE_SetEvent(&g_WAE[i], nDevNum ,port,op, want, 2, &t_FrameHead,pdata, datalen, nReSendCnt,timeout) == TRUE)
			{
					ret = TRUE;
					break;
			}
		}
	}
	
	if(i == sizeof(g_WAE)/sizeof(g_WAE[0]))
	{
			WAE_FreeEvent(&g_WAE[0]);
			//队列都满的话就冲掉第一条
			if(WAE_SetEvent(&g_WAE[0], nDevNum ,port,op, want, 2,&t_FrameHead ,pdata, datalen, nReSendCnt,timeout) == TRUE)
			{
					ret = TRUE;
			}
	}
	
	OSSchedUnlock();           /* 解除关调度保护 */

	return ret;
}



