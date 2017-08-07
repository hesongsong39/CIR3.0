/**********************************************************************
* $Id$      DebugOut              2016-03-11
*//**
* @file     DebugOut.c
* @brief    调试信息输出
* @version  1.0.0
* @date     2016/03/11 星期五 08:20:15
* @author   ZhangYanMing
*
* Copyright(C) 2016, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
/************************** Includes **********************************/
#include <string.h>                  
#include <stdio.h>                    
#include <stdarg.h>
#include "lpc_types.h"
#include <LPC43xx.H>
#include "DebugOut.h"
#include "CmdDefine.h"
#include "DataTransfer.h"


/*****************Private variables ***********************************/

/*****************Public variables ************************************/

/*****************Extern variables ************************************/




/*********************************************************************//**
 * @brief		print function that supports format as same as printf()
 * 				function of <stdio.h> library
 * @param[in]	format formated string to be print
 * @return		None
 **********************************************************************/
void Debug_Output_CAN(uint8_t chto,uint8_t sport,const  char *format, ...)
{
		char DebugBuf[OUTPUT_MAXSIZE] = {0};
		uint8_t  txbuf[OUTPUT_MAXSIZE] = {0};
		
    va_list vArgs;
    va_start(vArgs, format);
    memset(txbuf, 0, sizeof(txbuf));
		memset(DebugBuf, 0, sizeof(DebugBuf));
    vsnprintf(DebugBuf, OUTPUT_MAXSIZE, (char const *)format, vArgs);
    va_end(vArgs);
		//发送出去_
		CCANDataSimpleSendPost(txbuf,sizeof(txbuf),chto,OP_MANAGE712, CMD_MANAGE712_DEBUG_OUT, sport, PORT_WEIHU,(uint8_t *)DebugBuf, strlen(DebugBuf), FALSE);
}


/*********************************************************************//**
 * @brief		print function that supports format as same as printf()
 * 				function of <stdio.h> library
 * @param[in]	format formated string to be print
 * @return		None
 **********************************************************************/
void Debug_Output_Ethernet(uint8_t sport,const  char *format, ...)
{
		uint8_t  canaddr = 0;
		uint8_t	 saddr[4] = {NETADDR_IP1,NETADDR_IP2,NETADDR_IP3,0};
		uint8_t  daddr[4] = {NETADDR_IP1,NETADDR_IP2,NETADDR_IP3,0};
		char DebugBuf[OUTPUT_MAXSIZE] = {0};
		uint8_t  txbuf[OUTPUT_MAXSIZE] = {0};
		uint16_t port = 0;
		
    va_list vArgs;
    va_start(vArgs, format);
    memset(txbuf, 0, sizeof(txbuf));
		memset(DebugBuf, 0, sizeof(DebugBuf));
    vsnprintf(DebugBuf, OUTPUT_MAXSIZE, (char const *)format, vArgs);
    va_end(vArgs);
		
		GetCanNetAddrByPort(sport,&canaddr,&saddr[3]);
		GetCanNetAddrByPort(PORT_WEIHU,&canaddr,&daddr[3]);
		GetNetPortByPort(sport,&port);
		//发送出去
		EthernetDatapbCIR30FullSendPost(txbuf,sizeof(txbuf),port,OP_MANAGE712,CMD_MANAGE712_DEBUG_OUT,sport,4,saddr,PORT_WEIHU,4,daddr,(uint8_t *)DebugBuf, strlen(DebugBuf), FALSE);
}






