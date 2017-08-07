/**********************************************************************
* $Id$      DataTransfer              2016-06-03
*//**
* @file     DataTransfer.c
* @brief    ���ݷ�����ؽӿ�
* @version  1.0.0
* @date     2016/06/03 ������ 11:20:15
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
static uint8_t FlowNum = 0;//ҵ��֡��ˮ��
/*****************Public variables ************************************/


/*****************Extern variables ************************************/

/********************************************************************//**
 * @brief 		ҵ�����ˮ�ż�1
 * @param[in]	
 * @return 		��ˮ��
 *********************************************************************/
uint8_t FlowNumAdd(void)
{
		uint8_t temp = 0;
		OSSchedLock();            /* �ص��ȱ��� */
		FlowNum++;
		FlowNum &= 0x07;
		temp = FlowNum;
		OSSchedUnlock();          /* ����ص��ȱ��� */
		return temp;
}

/********************************************************************//**
 * @brief 		��ȡҵ�����ˮ��
 * @param[in]	
 * @return 		��ˮ��
 *********************************************************************/
uint8_t FlowNumGet(void)
{
		uint8_t temp = 0;
		OSSchedLock();            /* �ص��ȱ��� */
		temp = FlowNum;
		OSSchedUnlock();          /* ����ص��ȱ��� */
		return temp;
}


/********************************************************************//**
 * @brief 		����Ϣ��ӵ���������
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
		//�����ڴ����п�������
		return TRUE;
	}
	//�����ڴ���û�п�������
	return FALSE;
}


/*********************************************************************//**
 * @brief      ͨ�÷��ͱ���
 * @param[in]  nDevNum     ����
 * @param[in]  sdData      ����
 * @param[in]  sdLen       ���ĳ���
 * @return 		 FALSE,δ�ɹ�������������ӵ�������Ϣ����
							 TRUE,�ɹ�������������ӵ�������Ϣ����
 **********************************************************************/
Bool Common_SendData(uint8_t nDevNum,uint8_t *sdData,uint16_t sdLen)
{
	//����Ϣ��ӵ����Ͷ���
	if(MQ_AddTail(g_MsgTx,nDevNum,sdData,sdLen) == FALSE)
	{
			return FALSE;
	}
	
	return TRUE;
}

static Bool DataReSend_Set(uint8_t pType,uint8_t chto,uint16_t port,uint8_t op,uint8_t cmd, uint8_t sport,uint8_t saddrlen,uint8_t *saddr,uint8_t dport,uint8_t daddrlen,uint8_t *daddr,uint8_t *pdata, uint16_t datalen)
{
		uint8_t   SendCnt = 0,Flownum = 0;

		//��ȡ��ˮҵ��֡��ˮ��
		Flownum = FlowNumGet();
		//�ط��ж�
		SendCnt = WAETX_COUNT;
		
		SendCnt--; 
		if(DataReSend_SetEvent(pType,chto,port,SendCnt,Flownum,op,cmd,sport,saddrlen,saddr,dport,daddrlen,daddr,pdata,datalen,WAETX_TIMEOUT) == FALSE)
		{
				return FALSE;
		}
		
		return TRUE;
}


/*********************************************************************//**
 * @brief		���ݷ��ͽӿ�
 * @param[out]	FrameAr       ����ɻ���֡���ݸ�ʽ
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
 * @return 		TRUE/FALSE
 **********************************************************************/

Bool UARTDatapbCIR30SendPost(uint8_t *FrameAr,uint16_t Framelen,uint8_t chto, uint8_t op, uint8_t cmd, uint8_t sport,uint8_t saddrlen,uint8_t *saddr,uint8_t dport,uint8_t daddrlen,uint8_t *daddr,uint8_t *data, uint16_t datalen, Bool bAck,Bool bResend)
{
		uint8_t  UartFlowNum = 0;
		uint16_t  arlen = 0;
		//ҵ����ˮ�ż�1
		UartFlowNum = FlowNumAdd();
		
		//������������֡���ͳ�ȥ
		arlen = BuildptCIR30Frame(FrameAr,Framelen,op,cmd,sport,saddrlen,saddr,dport,daddrlen,daddr,data,datalen,bAck,FALSE,UartFlowNum);
		//����Ϣ��ӵ����Ͷ���
		if(Common_SendData(chto,FrameAr,arlen) == FALSE)
		{
				return FALSE;
		}
		return TRUE;
}

/*********************************************************************//**
 * @brief		���ݷ��ͽӿ�
 * @param[out]	FrameAr       ����ɻ���֡���ݸ�ʽ
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
 * @return 		TRUE/FALSE
 **********************************************************************/

Bool UARTDatapbCIR20SendPost(uint8_t *FrameAr,uint16_t Framelen,uint8_t chto, uint8_t op, uint8_t cmd, uint8_t sport,uint8_t saddrlen,uint8_t *saddr,uint8_t dport,uint8_t daddrlen,uint8_t *daddr,uint8_t *data, uint16_t datalen, Bool bAck)
{
		uint16_t  arlen = 0;
		
		//������������֡���ͳ�ȥ
		arlen = BuildptCIR20FrameStream(FrameAr,Framelen,op,cmd,sport,saddrlen,saddr,dport,daddrlen,daddr,data,datalen);
		//����Ϣ��ӵ����Ͷ���
		if(Common_SendData(chto,FrameAr,arlen) == FALSE)
		{
				return FALSE;
		}
		
		return TRUE;
}

/*********************************************************************//**
 * @brief		���ݷ��ͽӿ�
 * @param[out]	FrameAr       ����ɻ���֡���ݸ�ʽ
 * @param[in]	op            ��ҵ������
 * @param[in]	cmd           ������
 * @param[in]	sport         ��Դ�˿�
 * @param[in]	dport         ��Ŀ�Ķ˿�
 * @param[in]	data          ��Ҫ���֡��ʽ����Ч����
 * @param[in]	datalen       ����Ч���ݳ���
 * @return 		TRUE/FALSE
 **********************************************************************/
Bool UARTDatapbCIR20SimpleSendPost(uint8_t *FrameAr,uint16_t Framelen,uint8_t chto, uint8_t op, uint8_t cmd, uint8_t sport,uint8_t dport,uint8_t *data, uint16_t datalen, Bool bAck)
{	
		Bool retval = FALSE;
		
		OSSchedLock();            /* �ص��ȱ��� */
		if(UARTDatapbCIR20SendPost(FrameAr,Framelen,chto,op,cmd,sport,0,NULL,dport,0,NULL,data,datalen,bAck) == TRUE)
		{
				retval = TRUE;
				if(bAck == TRUE)
				{
						retval = DataReSend_Set(PACK_CIR20 | PERIPH_UART,chto,0,op,cmd,sport,0,NULL,dport,0,NULL,data,datalen);
				}
		}
		OSSchedUnlock();          /* ����ص��ȱ��� */
		
		return retval;
}

/*********************************************************************//**
 * @brief		���ݷ��ͽӿ�
 * @param[out]	FrameAr       ����ɻ���֡���ݸ�ʽ
 * @param[in]	op            ��ҵ������
 * @param[in]	cmd           ������
 * @param[in]	sport         ��Դ�˿�
 * @param[in]	dport         ��Ŀ�Ķ˿�
 * @param[in]	data          ��Ҫ���֡��ʽ����Ч����
 * @param[in]	datalen       ����Ч���ݳ���
 * @return 		TRUE/FALSE
 **********************************************************************/
Bool UARTDatapbCIR30SimpleSendPost(uint8_t *FrameAr,uint16_t Framelen,uint8_t chto, uint8_t op, uint8_t cmd, uint8_t sport,uint8_t dport,uint8_t *data, uint16_t datalen, Bool bAck)
{	
		Bool retval = FALSE;
		
		OSSchedLock();            /* �ص��ȱ��� */
		if(UARTDatapbCIR30SendPost(FrameAr,Framelen,chto,op,cmd,sport,0,NULL,dport,0,NULL,data,datalen,bAck,FALSE) == TRUE)
		{
				retval = TRUE;
				if(bAck == TRUE)
				{
						retval = DataReSend_Set(PACK_CIR30 | PERIPH_UART,chto,0,op,cmd,sport,0,NULL,dport,0,NULL,data,datalen);
				}
		}
		OSSchedUnlock();          /* ����ص��ȱ��� */
		
		return retval;
		
}


/*********************************************************************//**
 * @brief		���ݷ��ͽӿ�
 * @param[out]	FrameAr       ����ɻ���֡���ݸ�ʽ
 * @param[in]	op            ��ҵ������
 * @param[in]	cmd           ������
 * @param[in]	sport         ��Դ�˿�
 * @param[in]	dport         ��Ŀ�Ķ˿�
 * @param[in]	data          ��Ҫ���֡��ʽ����Ч����
 * @param[in]	datalen       ����Ч���ݳ���
 * @return 		TRUE/FALSE
 **********************************************************************/
Bool UARTDatapbCIR30FullSendPost(uint8_t *FrameAr,uint16_t Framelen,uint8_t chto, uint8_t op, uint8_t cmd, uint8_t sport,uint8_t saddrlen,uint8_t *saddr,uint8_t dport,uint8_t daddrlen,uint8_t *daddr,uint8_t *data, uint16_t datalen, Bool bAck)
{	
		Bool retval = FALSE;
		
		OSSchedLock();            /* �ص��ȱ��� */
		if(UARTDatapbCIR30SendPost(FrameAr,Framelen,chto,op,cmd,sport,saddrlen,saddr,dport,daddrlen,daddr,data,datalen,bAck,FALSE) == TRUE)
		{
				retval = TRUE;
				if(bAck == TRUE)
				{
						retval = DataReSend_Set(PACK_CIR30 | PERIPH_UART,chto,0,op,cmd,sport,saddrlen,saddr,dport,daddrlen,daddr,data,datalen);
				}
		}
		OSSchedUnlock();          /* ����ص��ȱ��� */
		
		return retval;
}

/*********************************************************************//**
 * @brief		CAN���ݷ��ͽӿ�
 * @param[out]	FrameAr       ����ɻ���֡���ݸ�ʽ
 * @param[in]	op            ��ҵ������
 * @param[in]	cmd           ������
 * @param[in]	sport         ��Դ�˿�
 * @param[in]	dport         ��Ŀ�Ķ˿�
 * @param[in]	data          ��Ҫ���֡��ʽ����Ч����
 * @param[in]	datalen       ����Ч���ݳ���
 * @param[in]	bAck          ���Ƿ���ҪӦ��
 * @param[in]	bResend       ���Ƿ���Ҫ�ط�
 * @return 		��ɻ���֡���ݸ�ʽ�󳤶�
 **********************************************************************/

Bool CCANDataSendPost(uint8_t *FrameAr,uint16_t Framelen,uint8_t chto,uint8_t op,uint8_t cmd, uint8_t sport,uint8_t saddrlen,uint8_t *saddr,uint8_t dport,uint8_t daddrlen,uint8_t *daddr,uint8_t *data, uint16_t datalen, Bool bAck,Bool bResend)
{
		_AddrPort  s_AddrItem;
		_AddrPort  d_AddrItem;
		uint8_t   Priority = 0, CanFlowNum = 0,DataType = 0,*pdata = NULL;
		CCANItem *pTI = (CCANItem *)FrameAr; 
		uint16_t   arlen = 0;
	
		//�жϴ����������ݻ����С������ֱ�ӷ���
		if(Framelen < 11 + saddrlen + daddrlen + datalen)
		{
				return FALSE;
		}
		pdata = data;
		
		//ͨ��Դ�˿ڡ�Ŀ�Ķ˿��Զ���ȡ������̫��IP��ַ��CAN���ߵ�Դ��ַ��Ŀ�ĵ�ַ
		s_AddrItem.port = sport;
		if(GetCanNetAddrByPort(s_AddrItem.port,&s_AddrItem.canaddr,&s_AddrItem.netaddr) == FALSE)
		{
				
		}
		
		d_AddrItem.port = dport;
		if(GetCanNetAddrByPort(d_AddrItem.port,&d_AddrItem.canaddr,&d_AddrItem.netaddr) == FALSE)
		{
				
		}
		//��ȡCAN�����е����ȼ�
		if(GetCanPriDataTypeByOpCmd(sport,dport,op,cmd,&Priority,&DataType) == FALSE)
		{
				
		}
		//CCAN�����ṹ�帳ֵ
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
		//ҵ����ˮ�ż�1
		CanFlowNum = FlowNumAdd();
		//������������֡���ͳ�ȥ
		pTI->Infolen = BuildptCIR30Frame(pTI->Info,Framelen - sizeof(CCANItem),op,cmd,sport,saddrlen,saddr,dport,daddrlen,daddr,pdata,datalen,bAck,bResend,CanFlowNum);
		
		//���㷢�͵����ݳ���
		arlen = sizeof(CAN_Param) + 2 + pTI->Infolen;
		//����Ϣ��ӵ����Ͷ���
		if(Common_SendData(chto,FrameAr,arlen) == FALSE)
		{
				return FALSE;
		}
		
		return TRUE;
}

/*********************************************************************//**
 * @brief		CAN���ݷ��ͽӿ�
 * @param[out]	FrameAr       ����ɻ���֡���ݸ�ʽ
 * @param[in]	op            ��ҵ������
 * @param[in]	cmd           ������
 * @param[in]	sport         ��Դ�˿�
 * @param[in]	dport         ��Ŀ�Ķ˿�
 * @param[in]	data          ��Ҫ���֡��ʽ����Ч����
 * @param[in]	datalen       ����Ч���ݳ���
 * @param[in]	bAck          ���Ƿ���ҪӦ��
 * @param[in]	bResend       ���Ƿ���Ҫ�ط�
 * @return 		��ɻ���֡���ݸ�ʽ�󳤶�
 **********************************************************************/

Bool CCANDataSimpleSendPost(uint8_t *FrameAr,uint16_t Framelen,uint8_t chto,uint8_t op,uint8_t cmd, uint8_t sport,uint8_t dport,uint8_t *data, uint16_t datalen, Bool bAck)
{
		Bool retval = FALSE;
		
		OSSchedLock();            /* �ص��ȱ��� */
		if(CCANDataSendPost(FrameAr,Framelen,chto,op,cmd,sport,0,NULL,dport,0,NULL,data,datalen,bAck,FALSE) == TRUE)
		{
				retval = TRUE;
				if(bAck == TRUE)
				{
						retval = DataReSend_Set(PACK_CIR30 | PERIPH_CAN,chto,0,op,cmd,sport,0,NULL,dport,0,NULL,data,datalen);
				}
		}
		OSSchedUnlock();          /* ����ص��ȱ��� */
		
		return retval;
}


/*********************************************************************//**
 * @brief		CAN���ݷ��ͽӿ�
 * @param[out]	FrameAr       ����ɻ���֡���ݸ�ʽ
 * @param[in]	op            ��ҵ������
 * @param[in]	cmd           ������
 * @param[in]	sport         ��Դ�˿�
 * @param[in]	dport         ��Ŀ�Ķ˿�
 * @param[in]	data          ��Ҫ���֡��ʽ����Ч����
 * @param[in]	datalen       ����Ч���ݳ���
 * @param[in]	bAck          ���Ƿ���ҪӦ��
 * @param[in]	bResend       ���Ƿ���Ҫ�ط�
 * @return 		��ɻ���֡���ݸ�ʽ�󳤶�
 **********************************************************************/

Bool CCANDataFullSendPost(uint8_t *FrameAr,uint16_t Framelen,uint8_t chto,uint8_t op,uint8_t cmd, uint8_t sport,uint8_t saddrlen,uint8_t *saddr,uint8_t dport,uint8_t daddrlen,uint8_t *daddr,uint8_t *data, uint16_t datalen, Bool bAck)
{
		Bool retval = FALSE;
		
		OSSchedLock();            /* �ص��ȱ��� */
		if(CCANDataSendPost(FrameAr,Framelen,chto,op,cmd,sport,saddrlen,saddr,dport,daddrlen,daddr,data,datalen,bAck,FALSE) == TRUE)
		{
				retval = TRUE;
				if(bAck == TRUE)
				{
						retval = DataReSend_Set(PACK_CIR30 | PERIPH_CAN,chto,0,op,cmd,sport,saddrlen,saddr,dport,daddrlen,daddr,data,datalen);
				}
		}
		OSSchedUnlock();          /* ����ص��ȱ��� */
		
		return retval;
}




/*********************************************************************//**
 * @brief		��̫�����ݷ��ͽӿ�
 * @param[out]	FrameAr       ����ɻ���֡���ݸ�ʽ
 * @param[in]	op            ��ҵ������
 * @param[in]	cmd           ������
 * @param[in]	sport         ��Դ�˿�
 * @param[in]	dport         ��Ŀ�Ķ˿�
 * @param[in]	data          ��Ҫ���֡��ʽ����Ч����
 * @param[in]	datalen       ����Ч���ݳ���
 * @param[in]	bAck          ���Ƿ���ҪӦ��
 * @param[in]	bResend       ���Ƿ���Ҫ�ط�
 * @return 		��ɻ���֡���ݸ�ʽ�󳤶�
 **********************************************************************/
Bool EthernetDataSendPost(uint8_t *FrameAr,uint16_t Framelen,uint16_t port,uint8_t op,uint8_t cmd, uint8_t sport,uint8_t saddrlen,uint8_t *saddr,uint8_t dport,uint8_t daddrlen,uint8_t *daddr,uint8_t *data, uint16_t datalen, Bool bAck,Bool bResend)
{
		uint8_t  EthernetFlowNum = 0;
		uint16_t  arlen = 0;
		//ҵ����ˮ�ż�1
		EthernetFlowNum = FlowNumAdd();
		
		//������������֡���ͳ�ȥ
		arlen = BuildptCIR30Frame(FrameAr,Framelen,op,cmd,sport,saddrlen,saddr,dport,daddrlen,daddr,data,datalen,bAck,FALSE,EthernetFlowNum);
		
		if(UDP_DataSend(daddr,port,FrameAr,arlen) == FALSE)
		{
				return FALSE;
		}
	
		return TRUE;
}

/*********************************************************************//**
 * @brief		���ݷ��ͽӿ�
 * @param[out]	FrameAr       ����ɻ���֡���ݸ�ʽ
 * @param[in]	op            ��ҵ������
 * @param[in]	cmd           ������
 * @param[in]	sport         ��Դ�˿�
 * @param[in]	dport         ��Ŀ�Ķ˿�
 * @param[in]	data          ��Ҫ���֡��ʽ����Ч����
 * @param[in]	datalen       ����Ч���ݳ���
 * @return 		TRUE/FALSE
 **********************************************************************/
Bool EthernetDatapbCIR30FullSendPost(uint8_t *FrameAr,uint16_t Framelen,uint16_t port, uint8_t op, uint8_t cmd, uint8_t sport,uint8_t saddrlen,uint8_t *saddr,uint8_t dport,uint8_t daddrlen,uint8_t *daddr,uint8_t *data, uint16_t datalen, Bool bAck)
{	
		Bool retval = FALSE;
		
		OSSchedLock();            /* �ص��ȱ��� */
		if(EthernetDataSendPost(FrameAr,Framelen,port,op,cmd,sport,saddrlen,saddr,dport,daddrlen,daddr,data,datalen,bAck,FALSE) == TRUE)
		{
				retval = TRUE;
				if(bAck == TRUE)
				{
						retval = DataReSend_Set(PACK_CIR30 | PERIPH_ETHERNET,INTERFACE_LAN,port,op,cmd,sport,saddrlen,saddr,dport,daddrlen,daddr,data,datalen);
				}
		}
		OSSchedUnlock();          /* ����ص��ȱ��� */
		
		return retval;
}
