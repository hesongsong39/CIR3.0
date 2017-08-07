/**********************************************************************
* $Id$      DataTransfer              2016-06-03
*//**
* @file     DataTransfer.c
* @brief    ���ݷ�����ؽӿ�ͷ�ļ�
* @version  1.0.0
* @date     2016/06/03 ������ 11:20:15
* @author   ZhangYanMing
*
* Copyright(C) 2016, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
#ifndef __DATATRANSFER__
#define __DATATRANSFER__
/************************** Includes **********************************/
#include "lpc_types.h"
#include <ucos_ii.h>
#include "ptcir.h"
#include "ptcan.h"
#include "cmddefine.h"


/**************************** Macro Definition ************************/

//Ӧ�������ֽ�
#define  CMDACK     0x41
//�����ط���ʱ�궨��
#define  WAETX_TIMEOUT 300
//�����ط������궨��
#define  WAETX_COUNT	 (uint8_t)(3)
#define  NO_WAETX	 	   (uint8_t)(1)

//CANЭ��汾��
#define  CCANPROTOCOL    0x00

//��������
#define  PERIPH_UART       0<<4
#define  PERIPH_CAN        1<<4
#define  PERIPH_ETHERNET   2<<4

#pragma push
#pragma pack(1)                               /* 1�ֽڶ��� */


typedef struct tag_TRAN_ITEM
{
  uint8_t  chFrom;   	
	uint16_t Infolen;				
	uint8_t Info[1];
}TranItem;


typedef struct tag_TRAN_CCAN
{   
	CAN_Param  CanParam;
	uint16_t Infolen;				
	uint8_t Info[1];
}CCANItem;


#pragma pop

/************************** Function Interface ************************/
uint8_t FlowNumGet(void);
Bool MQ_AddTail(OS_EVENT * pMsg,uint8_t chForm,uint8_t *data,uint16_t datalen);
Bool Common_SendData(uint8_t nDevNum,uint8_t *sdData,uint16_t sdLen);
Bool UARTDatapbCIR20SendPost(uint8_t *FrameAr,uint16_t Framelen,uint8_t chto, uint8_t op, uint8_t cmd, uint8_t sport,uint8_t saddrlen,uint8_t *saddr,uint8_t dport,uint8_t daddrlen,uint8_t *daddr,uint8_t *data, uint16_t datalen, Bool bAck);
Bool UARTDatapbCIR20SimpleSendPost(uint8_t *FrameAr,uint16_t Framelen,uint8_t chto, uint8_t op, uint8_t cmd, uint8_t sport,uint8_t dport,uint8_t *data, uint16_t datalen, Bool bAck);
Bool UARTDatapbCIR30SendPost(uint8_t *FrameAr,uint16_t Framelen,uint8_t chto, uint8_t op, uint8_t cmd, uint8_t sport,uint8_t saddrlen,uint8_t *saddr,uint8_t dport,uint8_t daddrlen,uint8_t *daddr,uint8_t *data, uint16_t datalen, Bool bAck,Bool bResend);
Bool UARTDatapbCIR30SimpleSendPost(uint8_t *FrameAr,uint16_t Framelen,uint8_t chto, uint8_t op, uint8_t cmd, uint8_t sport,uint8_t dport,uint8_t *data, uint16_t datalen, Bool bAck);
Bool UARTDatapbCIR30FullSendPost(uint8_t *FrameAr,uint16_t Framelen,uint8_t chto, uint8_t op, uint8_t cmd, uint8_t sport,uint8_t saddrlen,uint8_t *saddr,uint8_t dport,uint8_t daddrlen,uint8_t *daddr,uint8_t *data, uint16_t datalen, Bool bAck);
Bool CCANDataSendPost(uint8_t *FrameAr,uint16_t Framelen,uint8_t chto,uint8_t op,uint8_t cmd, uint8_t sport,uint8_t saddrlen,uint8_t *saddr,uint8_t dport,uint8_t daddrlen,uint8_t *daddr,uint8_t *data, uint16_t datalen, Bool bAck,Bool bResend);
Bool CCANDataSimpleSendPost(uint8_t *FrameAr,uint16_t Framelen,uint8_t chto,uint8_t op,uint8_t cmd, uint8_t sport,uint8_t dport,uint8_t *data, uint16_t datalen, Bool bAck);
Bool CCANDataFullSendPost(uint8_t *FrameAr,uint16_t Framelen,uint8_t chto,uint8_t op,uint8_t cmd, uint8_t sport,uint8_t saddrlen,uint8_t *saddr,uint8_t dport,uint8_t daddrlen,uint8_t *daddr,uint8_t *data, uint16_t datalen, Bool bAck);
Bool EthernetDataSendPost(uint8_t *FrameAr,uint16_t Framelen,uint16_t port,uint8_t op,uint8_t cmd, uint8_t sport,uint8_t saddrlen,uint8_t *saddr,uint8_t dport,uint8_t daddrlen,uint8_t *daddr,uint8_t *data, uint16_t datalen, Bool bAck,Bool bResend);
Bool EthernetDatapbCIR30FullSendPost(uint8_t *FrameAr,uint16_t Framelen,uint16_t port, uint8_t op, uint8_t cmd, uint8_t sport,uint8_t saddrlen,uint8_t *saddr,uint8_t dport,uint8_t daddrlen,uint8_t *daddr,uint8_t *data, uint16_t datalen, Bool bAck);


#endif 



