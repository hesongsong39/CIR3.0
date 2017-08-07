/*************************************************
  Copyright (C), 2016, 712.Co.,Ltd.
  File name:     ptcan.h
  Author:        Version:   v1.0     Date: 16.03.16
  Description:  版本管理
  Others:         
  Function List:  
  1. 

  History:      
  1. xxx   16.03.16    v1.0
*************************************************/
#ifndef __PTCAN20160316_H
#define __PTCAN20160316_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
#include "windows.h"

typedef struct 
{
	unsigned int    id;  
	unsigned int    dlc; 
	unsigned char   data[8]; 
}CCAN_MSG_OBJECT;

typedef enum 
{
    CCAN_MSG_NONE = 0,  
    CCAN_MSG_OK, 
    CCAN_BUSOFF, 
    CCAN_BUFFER_FULL, 
    CCAN_NONE,
} CCAN_MSG_STATE;

#else
#include "C_CAN.h"
#endif

//统计相关内容
extern int g_iRecvSuccessTotalCount;
extern int g_iRecvErrorTotalCount;
extern int g_iSendSuccessTotalCount;
extern int g_iSendErrorTotalCount;

//缓冲区个数定义区，至少为1
#define FRAME_BUFFER_CNT_32     6
#define FRAME_BUFFER_CNT_64     6
#define FRAME_BUFFER_CNT_128    2
#define FRAME_BUFFER_CNT_256    2
#define FRAME_BUFFER_CNT_512    2
#define FRAME_BUFFER_CNT_1024	2

//根据需要决定是否开启大缓冲区，要开启大缓冲区请在工程选项中设置

#ifdef USE_BIG_BUFFER
#define FRAME_BUFFER_CNT_2048	2
#else
#define FRAME_BUFFER_CNT_2048	0
#endif

#pragma pack(1)

typedef struct tag_CAN_Param
{
	unsigned char CANIndex;			//收发报文的CAN口，从0开始算
	unsigned char ProtocolVer;		//协议版本
	unsigned char Priority;			//优先级
	unsigned char DataType;			//数据类型
	unsigned char CANSrcAddr;		//CAN源地址
	unsigned char CANDestAddr;		//CAN目的地址
	unsigned char ZongXianDiZhi;	//总线地址
	unsigned char ZhuanFaBiaoZhi;	//转发标志
} CAN_Param;


//CAN的ID结构
typedef struct tag_CAN_ID
{
	//流水号. 递增二进制值
	unsigned int FlowNo: 3;

	//预留. 固定为0
	unsigned int Reserved: 1;

	//分段码标识. 
	//	00b：数据传输不分段；
	//	01b：数据传输的第一个分段；
	//	10b：数据传输的中间分段；
	//	11b：数据传输的最后分段。 
	unsigned int SectionFlag: 2;

	//数据类型. 0：语音, 1：数据
	unsigned int DataType: 1;

	//转发标志. 0：正常帧, 1：转发帧
	unsigned int ZhuanFaBiaoZhi: 1;

	//CAN源地址. 可以表示256个地址
	unsigned int SrcAddr: 8;

	//CAN目的地址. 可以表示256个地址
	unsigned int DestAddr: 8;

	//协议版本. 当前为0
	unsigned int ProtocolVer: 2;

	//优先级. 取值范围0~3，0最高，3最低
	unsigned int Priority: 2;

	//总线地址. 0:总线1，1:总线2
	unsigned int ZongXianDiZhi: 1;

	//预留. 目前用于区分扩展帧和标准帧
	unsigned int Unused: 3;
} CAN_ID;


typedef struct tag_Frame_Buffer
{
	unsigned char *pBuffer; //指向缓冲区的指针
	unsigned short wBufferLen; //缓冲区的大小
} FrameBuffer;


//用于接收报文的缓冲区头部定义
typedef struct tag_Frame_Buffer_Header
{
	//指示报文的接收状态. 0：空闲，1：接收中，2：接收完毕
	unsigned char RecvState;

	//指示接收第1帧的系统滴答数，该值用于判断接收是否超时，单位毫秒
	unsigned int RecvTime;

	//用于接收报文的CAN索引
	unsigned char CANIndex;			

	//优先级. 取值范围0~3，0最高，3最低
	unsigned char Priority;

	//协议版本. 取值范围0~3，目前为0
	unsigned char ProtocolVer;

	//数据类型. 0：语音, 1：数据
	unsigned char DataType;

	//转发标志. 0：正常帧, 1：转发帧
	unsigned char ZhuanFaBiaoZhi;

	//总线地址. 0:总线1，1:总线2
	unsigned char ZongXianDiZhi;

	//CAN源地址. 取值范围0~255
	unsigned char SrcAddr;

	//CAN目的地址. 取值范围0~255
	unsigned char DestAddr;	

	//数据的实际长度
	unsigned short DataLen;	
} FrameBuffer_Header;


typedef struct tag_Frame_BufferData
{
	FrameBuffer_Header Header; //头部
	unsigned char Data[1]; //数据部分
} FrameBuffer_HeaderData;


#pragma pack()


//初始化CAN，将清空所有接收缓冲区，清除回调函数指针
//返回值: >0表示成功，<=0表示失败
int CanInit(void);

//设置获取系统运行毫秒数回调函数
void CanGetTickCountCallbackFunc(unsigned int (*pf)(void));

//设置接收超时时间，单位ms。
//参数：dwRecvTimeOut 超时时间
void CanSetRecvTimeOut(unsigned int dwRecvTimeOut);

//是否启用接收超时
//参数：bEnabled 是否启用。0不启用，1启用
void CanSetRecvTimeOutEnabled(int bEnabled);

//===============================================================发送相关函数
//发送一个CIR3.0基础数据(pbCIR2.0)到CAN总线
//参数：pData 指向CIR3.0基础数据(pbCIR2.0)，wDataLen 为长度，pCAN_Param 为一些发送参数
//返回值: >0表示成功，<=0表示失败
int CanSendDataPackToCanBus(unsigned char *pData, unsigned short wDataLen, CAN_Param *pCAN_Param);


//设置发送CAN帧回调函数   
//pMsgObj 指向所有的CAN帧，iMsgObjCount 为CAN帧的数量，iTotalMsgObjCount 为CAN帧的总数量
void CanSendFrameCallbackFunc(CCAN_MSG_STATE (*pf)(unsigned char CANIndex, CCAN_MSG_OBJECT *pMsgObj, 
	unsigned short iMsgObjCount, unsigned short iTotalMsgObjCount));


//===============================================================接收相关函数
//清空所有接收缓冲区
void CanClearAllRecvBuffer(void);

//将接收到的CAN帧填入接收缓冲区
//pData 指向所有的CAN帧，iDataLen 为CAN帧的数量
//返回值: >0表示成功，<=0表示失败
int CanAddFrameToRecvBuffer(unsigned char CANIndex, CCAN_MSG_OBJECT *pData, int iDataLen);

//设置接收数据包回调函数
//参数：pData 指向CIR3.0基础数据(pbCIR2.0)，wDataLen 为长度，pCAN_Param 为一些发送参数
void CanRecvDataPackCallbackFunc(int (*pf)(unsigned char *pData, unsigned short wDataLen, 
	CAN_Param *pCAN_Param));

//设置接收数据帧缓冲区回调函数
//参数：pBuffer 指向存放接收缓冲区指针的数组，iBufferCount 为数组的个数
void CanGetRecvFrameBufferPtrCallbackFunc(int (*pf)(FrameBuffer *pBuffer));

#ifdef __cplusplus
}
#endif

#endif

