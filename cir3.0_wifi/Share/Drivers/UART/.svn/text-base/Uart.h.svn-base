/**********************************************************************
* $Id$      Uart             2015-12-23
*//**
* @file     Uart.h
* @brief    串口的相关操作的头文件
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

//发送数据缓冲大小
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
	uint8_t  Num;								//端口号
	uint32_t Baudrate;  	   		//串口的波特率
	//数据发送相关
	uint8_t   *SendBuf;					//串口的发送缓冲
	uint16_t  pRead;						//串口发送缓冲读指针
	uint16_t  pWrite;           //串口发送缓冲写指针
	uint16_t  SendBufSize;			//串口发送缓冲大小
	//数据接收相关
	uint8_t  *RecBuf;					  //串口的接收缓冲
	uint16_t  rpRead;						//串口接收缓冲读指针
	uint16_t  rpWrite;           //串口接收缓冲写指针
	uint16_t  RecBufSize;				//串口接收缓冲长度
}UartInterface;


/************************** Function Interface ************************/
Bool UartInit(uint8_t UartNum,uint32_t Baudrate,pFnfpininit pininit);
Bool UartSend(uint8_t uartnum, uint8_t *pdata, uint16_t plen);
Bool UartSendBufSet(uint8_t UartNum,uint8_t *Buf,uint16_t BufSize);
Bool UartRecBufSet(uint8_t UartNum,uint8_t *Buf,uint16_t BufSize);
UartInterface *GetpUIbyNum(uint8_t uartnum);


#endif 
