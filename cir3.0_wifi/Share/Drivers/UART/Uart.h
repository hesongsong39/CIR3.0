/**********************************************************************
* $Id$      Uart             2015-12-23
*//**
* @file     Uart.h
* @brief    ���ڵ���ز�����ͷ�ļ�
* @version  1.0.0
* @date      2015/12/23
* @author   ZhangYanMing
*
* Copyright(C) 2015, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
#ifndef __UART__
#define __UART__

/**************************Uart Config Macro Definition ************************/

//�������ݻ����С
#define UART0_BUFFER_SIZE	1024
#define UART1_BUFFER_SIZE	1024
#define UART2_BUFFER_SIZE	1024
#define UART3_BUFFER_SIZE	1024

typedef  void (*pFnfpininit)(uint8_t UartNum);

/*
 * @brief	UartInterface structure
 */
typedef struct Tag_UartInterface
{
	uint8_t  Num;								//�˿ں�
	uint32_t Baudrate;  	   		//���ڵĲ�����
	//���ݷ������
	uint8_t   *SendBuf;					//���ڵķ��ͻ���
	uint16_t  pRead;						//���ڷ��ͻ����ָ��
	uint16_t  pWrite;           //���ڷ��ͻ���дָ��
	uint16_t  SendBufSize;			//���ڷ��ͻ����С
	//���ݽ������
	uint8_t  *RecBuf;					  //���ڵĽ��ջ���
	uint16_t  rpRead;						//���ڽ��ջ����ָ��
	uint16_t  rpWrite;           //���ڽ��ջ���дָ��
	uint16_t  RecBufSize;				//���ڽ��ջ��峤��
}UartInterface;


/************************** Function Interface ************************/
Bool UartInit(uint8_t UartNum,uint32_t Baudrate,pFnfpininit pininit);
Bool UartSend(uint8_t uartnum, uint8_t *pdata, uint16_t plen);
Bool UartSendBufSet(uint8_t UartNum,uint8_t *Buf,uint16_t BufSize);
Bool UartRecBufSet(uint8_t UartNum,uint8_t *Buf,uint16_t BufSize);
UartInterface *GetpUIbyNum(uint8_t uartnum);


#endif 
