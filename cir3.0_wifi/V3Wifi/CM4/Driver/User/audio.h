/**********************************************************************
* $Id$      Audio              2014-07-01
*//**
* @file     Audio.h
* @brief    音频相关函数的头文件
* @version  2.0.0
* @date     2014/7/1 星期二 13:56:02
* @author   HaoZhilei
*
* Copyright(C) 2013, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
#ifndef __AUDIO__
#define __AUDIO__

#include "lpc_types.h"

typedef  void (*pFnfI2Spininit)(void);
typedef  void (*pFnfCodecReset)(void);


/************************** PRIVATE DEFINITIONS *************************/
/** Max buffer length */
#define AUDIO_BUFFER_SIZE         240           /* 480 byte, 30ms音频(8k 16bit) */
#define AUDIO_QUEUE_SIZE          4
#define AUDIO_QUEUE_MASK          AUDIO_QUEUE_SIZE-1


#pragma push
#pragma pack(1)                               /* 1字节对齐 */

/**
 * @brief audio data structure definition
 */
typedef struct
{
	uint32_t rd;    /* 队列读指针 */
	uint32_t wr;    /* 队列写指针 */
	int16_t buf[AUDIO_QUEUE_SIZE][AUDIO_BUFFER_SIZE];
} AudioQueue_Type;


#pragma pop

/**
 * \def AQ_DATA_COUNT(q)
 * This macro will get the number of items pending in queue
 */
#define AQ_DATA_COUNT(q) ((uint8_t) ((q)->wr - (q)->rd))

/**
 * \def AQ_IS_FULL(q)
 * This macro will evaluate to 1 if queue is full, 0 if it is not
 */
#define AQ_IS_FULL(q)    (AQ_DATA_COUNT(q) >= AUDIO_QUEUE_SIZE)

/**
 * \def AQ_IS_EMPTY(q)
 * This macro will evaluate to 1 if queue is empty, 0 if it is not
 */
#define AQ_IS_EMPTY(q)   ((q)->wr == (q)->rd)

/**
 * @brief audio mode control type
 */
typedef enum{
	RX = 0x80,
	TX,
	ALL
} AUDIO_MODE;

/* PUBLIC VARIABLES **************************************************/
extern AudioQueue_Type	g_AQRX;
extern AudioQueue_Type	g_AQTX;



Bool Audio_INIT(pFnfI2Spininit fpininit,pFnfCodecReset fCodecReset);
void Audio_Setup(AUDIO_MODE md);
void Audio_Stop(AUDIO_MODE md);
Bool Audio_StatusQuery(AUDIO_MODE md);

//队列缓冲操作---------------------------------------------------------------------------//
void Audio_Q_Init(void);
Bool Audio_Q_AddTail(AudioQueue_Type *pQ, int16_t *pdata, uint16_t plen);
Bool Audio_Q_RemoveTail(AudioQueue_Type *pQ, int16_t *pdata, uint16_t plen);
Bool Audio_Q_GetValue(AudioQueue_Type *pQ, int16_t *pdata, uint16_t plen);
uint8_t Audio_Q_GetSize(AudioQueue_Type *pQ);
uint32_t Audio_GetRXLength(void);
uint32_t Audio_GetTXLength(void);
void Audio_GetRXSet(void);
void Audio_GetTXSet(void);

#endif 


