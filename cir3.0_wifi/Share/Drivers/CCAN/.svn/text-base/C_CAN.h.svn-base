/**********************************************************************
* $Id$      C_CAN              2016-01-07
*//**
* @file     C_CAN.h
* @brief    C_CAN总线驱动头文件
* @version  1.0.1
* @date     2016/01/07 星期四 20:58:36
* @author   ZhangYanMing
*
* Copyright(C) 2016, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
#ifndef __C_CAN_H__
#define __C_CAN_H__

/**************************** Includes **********************************/
#include "lpc_types.h"
#include "ccan_18xx_43xx.h"

/**************************CAN Config Macro Definition ************************/
//发送数据缓冲大小
#define CAN0_BUFFER_SIZE	256
#define CAN1_BUFFER_SIZE	256
//接收数据缓冲大小
#define CCANQUEUESIZE   256

#define DESTINATION     16
#define CCAN_RX_MASK_ID (0x000000FF << DESTINATION)

/*
 * @brief	CANSend 返回状态
 */
typedef enum 
{
		CCAN_MSG_NONE = 0,  
	  CCAN_MSG_OK, 
		CCAN_BUSOFF, 
		CCAN_BUFFER_FULL, 
		CCAN_NONE,
} CCAN_MSG_STATE;

/*
 * @brief	CAN State
 */
typedef enum 
{
		DEVICE_C_CAN_ACTIVE = 0,  
    DEVICE_C_CAN_PASSIVE, 
		DEVICE_C_CAN_BOFF
} CCAN_DEVICE_STATE;

typedef enum 
{
		DEVICE_C_CAN0 = 0,  
    DEVICE_C_CAN1, 
} CCAN_DEVICE_INDEX;

#pragma push
#pragma pack(1)                               /* 1字节对齐 */

typedef struct 
{
    uint32_t    id;  
    uint8_t     dlc; 
    uint8_t     data[8]; 
}CCAN_MSG_OBJECT;

typedef struct
{
		uint8_t         DeviceIndex;
		CCAN_MSG_OBJECT  Canmsg;
}CAN_Frame;

typedef struct
{
		uint16_t pwrite;
		uint16_t pread;
		CAN_Frame  pmsg[CCANQUEUESIZE];
}CAN_Queue;

#pragma pop

extern CAN_Queue CANRxqueue;

#define LPC_CCAN1     ((LPC_CCAN_T    *) LPC_C_CAN1_BASE)
#define LPC_CCAN0     ((LPC_CCAN_T    *) LPC_C_CAN0_BASE)

typedef  void (*pFnfCanpininit)(uint8_t CanNum);


/*
 * @brief	CANInterface structure
 */
typedef struct Tag_CANInterface
{
	uint8_t  Num;				//端口号
	uint32_t Baudrate;  	   	//波特率
	LPC_CCAN_T *pCCAN;
	//数据发送相关
	CCAN_MSG_OBJECT   *SendBuf;		  //发送缓冲
	uint16_t  pRead;				  //发送缓冲写指针
	uint16_t  pWrite;          		  //发送缓冲读指针
	uint16_t  SendBufSize;			  //发送缓冲大小

	//CAN总线状态，error active/error passive/bossoff
	CCAN_DEVICE_STATE CANStatus;
}CANInterface;


/************************** Function Interface ************************/
Bool CANInit(uint8_t CanNum,uint32_t bitRate,pFnfCanpininit pininit);
Bool CANMessageAdd(uint8_t CanNum,uint32_t id,uint32_t MaskBit,Bool EOBstate,Bool Extend);
void CANMessageClear(uint8_t CanNum);
CCAN_MSG_STATE CANSend(uint8_t CanNum, CCAN_MSG_OBJECT *pMsgObj,uint16_t plen,uint16_t totalnum);
Bool CANSendBufSet(uint8_t CanNum,CCAN_MSG_OBJECT *Buf,uint16_t BufSize);
CANInterface *GetpCANIbyNum(uint8_t CanNum);
uint8_t CANStatusGet(uint8_t CanNum);
Bool CCAN_RecMessageSet(uint8_t CanNum,uint8_t desaddr,Bool EOBstate);


#endif





