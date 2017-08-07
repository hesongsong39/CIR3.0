/**********************************************************************
* $Id$      DataReSend              2016-05-25
*//**
* @file     DataReSend.h
* @brief    �����ط�����ͷ�ļ�
* @version  1.0.0
* @date     2016/05/25 ������ 14:28:44
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
//�ط����ݽṹ��С
#define  WAE_QUEUE_SIZE      12
#define  WAE_QUEUE_MASK      (WAE_QUEUE_SIZE - 1)


#pragma push
#pragma pack(1)     /* 1�ֽڶ��� */

typedef struct
{
		uint8_t 	iCIRType;						// CIRЭ������(0:ptCIR2.0  1:ptCIR3.0)
		uint8_t 	iOp; 					  	// ҵ������
		uint8_t 	iCmd;  						// ����
		uint8_t 	iSport;						// Դ�˿�
		uint8_t 	iSaddrlen;					// Դ��ַ����
		uint8_t 	szSaddr[4];					// Դ��ַ
		uint8_t 	iDport;						// Ŀ��˿�
		uint8_t 	iDaddrlen;					// Ŀ���ַ����
		uint8_t 	szDaddr[4];					// Ŀ���ַ
}ResendImport;
	
/*
 * @brief �����ط��ṹ�嶨��
 */
typedef struct 
{
	uint8_t  portnum;         //�ж�����
	uint16_t remport;         //��̫������Զ�˻��˿ں�
	uint8_t  m_dwCnt; 				//���� 
	uint32_t m_dwLastTick; 		//���һ�η������ݵ�ʱ��
	uint32_t m_dwStartTick; 	//���һ�η������ݵ�ʱ��
	uint32_t m_timeout; 		  //�����ط����ʱ��

	//����ƥ��Ĳ�������
	uint8_t  m_cbOp;		 
	uint8_t  m_cbLen;
	uint8_t  m_data[2];
	
	//�洢�ط��ؼ�����
	ResendImport m_FrameHead;
	
  uint8_t  m_cbSendLen; 			//��������ʱ�����͵�����
  uint8_t  *m_SendData;                	
}Wait_Ack_Event;

#pragma pop

/************************** Function Interface ************************/
void DataReSend_Proc (uint8_t *txbuf,uint16_t plen);
Bool DataReSend_Init(void);
Bool DataReSend_SetEvent(uint8_t pType,uint8_t nDevNum,uint16_t port,uint8_t nReSendCnt,uint8_t Flownum,uint8_t op,uint8_t cmd, uint8_t sport,uint8_t saddrlen,uint8_t *saddr,uint8_t dport,uint8_t daddrlen,uint8_t *daddr,uint8_t *pdata,uint16_t datalen,uint32_t timeout);
Bool WAE_CheckPair(uint8_t cbOp, uint8_t cbCmd, uint8_t *data, uint8_t cbLen);

#endif



