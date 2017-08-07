/**********************************************************************
* $Id$      EthernetUdp              2016-12-15
*//**
* @file     EthernetUdp.c
* @brief    ��̫��UDP���ݴ���
* @version  1.0.0
* @date     2016/12/15
* @author   ZhangYanMing
*
* Copyright(C) 2016, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
/**************************** Includes ********************************/
#include <string.h>	
#include "lpc_types.h"
#include "RTL.h"
#include "EthernetUdp.h"									


/*****************Private variables ***********************************/
static UdpSendInfo_Type    g_UdpSendInfo;;
static UdpRecInfo_Type    g_UdpRecInfo[UDPLOCALNUM];

/*****************Public variables ************************************/


/*****************Extern variables ************************************/

/*********************************************************************//**
 * @brief			UDP send data
 * @param[in]	soc			�׽���
 * @param[in]	remip		Զ�˻�IP��ַ
 * @param[in]	port		Զ�˻��˿ں�
 * @param[in]	buf			�����͵�����
 * @param[in]	len			�����͵����ݳ���
 * @return 		TRUE/FALSE
 **********************************************************************/
Bool UDP_SendData(uint8_t socket,uint8_t *remip,uint16_t remport,uint8_t *buf, uint16_t len)
{
	uint8_t *sendbuf = NULL;
	
	if((buf == NULL)||(len == 0)||(remip == NULL))
	{
		return FALSE;
	}
	sendbuf = udp_get_buf (len);							
	if(sendbuf == NULL)
	{
		return FALSE;
	}
	memcpy(sendbuf, buf, len);
	if(udp_send (socket, remip, remport, sendbuf, len) == __TRUE)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


/*********************************************************************//**
 * @brief			UDP Socket Callback function
 * @param[in]	soc			�׽���
 * @param[in]	rip			���յ���IP
 * @param[in]	rport		���յ���IP��ַ
 * @param[in]	buf			UDP����
 * @param[in]	len			UDP���ݳ���
 * @return 		None
 **********************************************************************/
static uint16_t udpsend_callback(uint8_t soc, uint8_t *rip, uint16_t rport, uint8_t *buf, uint16_t plen) 
{
  return (0);
}

/*********************************************************************//**
 * @brief			UDP Socket Callback function
 * @param[in]	socket	�׽���
 * @param[in]	rip			���յ���IP
 * @param[in]	rport		���յ���IP��ַ
 * @param[in]	buf			UDP����
 * @param[in]	len			UDP���ݳ���
 * @return 		None
 **********************************************************************/
static uint16_t udprev_callback(uint8_t socket, uint8_t *rip, uint16_t rport, uint8_t *buf, uint16_t plen) 
{
	uint8_t i = 0;
	for(i=0;i<sizeof(g_UdpRecInfo)/sizeof(g_UdpRecInfo[0]);i++)
	{
			if(g_UdpRecInfo[i].socket == socket)
			{
					break;
			}
	}
	if(i == sizeof(g_UdpRecInfo)/sizeof(g_UdpRecInfo[0]))
	{
			return (0);	
	}
	//û����Ч���մ�����
	if(g_UdpRecInfo[i].pfuncRcv == NULL)	
	{
		return (0);	
	}
	
	(g_UdpRecInfo[i].pfuncRcv)(ETHERNET_UDP,rip,rport,g_UdpRecInfo[i].localport,buf,plen);
	
  return (0);
}

/*********************************************************************//**
 * @brief 			UDP�ͻ��˳�ʼ��
 * @param[in] 	port
 * @param[in]   pRcvDatafuc�����ջص���������
 * @return			TRUE/FALSE
 ************************************************************************/
Bool UDP_ReceivePort_Init(uint16_t port,pFnRevData pRcvDatafuc)														
{
	Bool ret = FALSE;
	uint8_t i = 0;
	
	if(pRcvDatafuc == NULL)
	{
		return ret;
	}
	
	for(i=0;i<sizeof(g_UdpRecInfo)/sizeof(g_UdpRecInfo[0]);i++)
	{
			if(g_UdpRecInfo[i].socket == 0)
			{
					break;
			}
	}
	if(i == sizeof(g_UdpRecInfo)/sizeof(g_UdpRecInfo[0]))
	{
			return ret;
	}
	g_UdpRecInfo[i].socket = udp_get_socket (0, UDP_OPT_SEND_CS | UDP_OPT_CHK_CS, udprev_callback);
	if (g_UdpRecInfo[i].socket != 0) 
	{
		if(udp_open(g_UdpRecInfo[i].socket, port) == __TRUE)
		{
			g_UdpRecInfo[i].pfuncRcv = pRcvDatafuc;
			g_UdpRecInfo[i].localport = port;
			ret = TRUE;
		}
	}
	
	return ret;
}



/*********************************************************************//**
 * @brief 			UDP���Ͷ˿ڳ�ʼ��
 * @param[in] 	port:Զ�˻��˿ں�
 * @param[in]   pSendDatafuc�����ͻص���������
 * @return			TRUE/FALSE
 ************************************************************************/
Bool UDP_SendPort_Init(uint16_t port,pFnSendData pfuncSend)
{
		Bool ret = FALSE;
		if(pfuncSend == NULL)
		{
			return ret;
		}
		g_UdpSendInfo.socket = udp_get_socket(0, UDP_OPT_SEND_CS | UDP_OPT_CHK_CS, udpsend_callback);
		if (g_UdpSendInfo.socket != 0) 
		{
			if(udp_open (g_UdpSendInfo.socket,port) == __TRUE)
			{
				g_UdpSendInfo.pfuncSend = pfuncSend;
				g_UdpSendInfo.localport = port;
				ret = TRUE;
			}
		}
		
		return ret;
}

/*********************************************************************//**
 * @brief 			UDP���ͺ���
 * @param[in] 	remip:Զ�˻�IP��ַ
 * @param[in] 	remport:Զ�˻��˿ں�
 * @param[in] 	buf:����ָ��
 * @param[in] 	len:���ݳ���
 * @return			TRUE/FALSE
 ************************************************************************/
Bool UDP_DataSend(uint8_t *remip,uint16_t remport,uint8_t *buf,uint16_t len)
{
	if((buf == NULL)||(len == 0)||(remip == NULL))
	{
		return FALSE;
	}
	//û�л����Чsocket
	if((g_UdpSendInfo.socket == 0)||(g_UdpSendInfo.pfuncSend == NULL))	
	{
		return FALSE;
	}	
	
	return ((g_UdpSendInfo.pfuncSend)(buf,len,ETHERNET_UDP,remip,remport,g_UdpSendInfo.socket));
}


/*********************************************************************//**
 * @brief 			UDPģ��������ݽṹ��ʼ��
 * @param[in] 	none
 * @return			none
 ************************************************************************/
void UDP_DataInit(void)
{
	uint8_t i = 0;
	
	//�������ݽṹ
	g_UdpSendInfo.socket = 0;
	g_UdpSendInfo.localport = 0;
	g_UdpSendInfo.pfuncSend = NULL;
	
	//�������ݽṹ
	for(i=0;i<sizeof(g_UdpRecInfo)/sizeof(g_UdpRecInfo[0]);i++)
	{
			g_UdpRecInfo[i].socket = 0;
			g_UdpRecInfo[i].localport = 0;
			g_UdpRecInfo[i].pfuncRcv = NULL;
	}
}

