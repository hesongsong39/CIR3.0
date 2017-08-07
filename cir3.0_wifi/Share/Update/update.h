#ifndef  _UPDATE_H__
#define  _UPDATE_H__

#include "updatecom.h"
#include "ptcir.h"
#include "lpc_types.h"
#include "DebugOut.h"

//#define WORKMODE_UPDATE 0X04

//#define UNIT_JP_NUM_MAX  64
//#define UPDATE_SEQUEN_MMI_SUB 0
//#define UPDATE_SEQUEN_MCU_SUB 1
//#define SHARENAME_UNIT_NUM 3

#define g_MsgUpdateRxTbl_MAX 			(BLOCKLENGTH)
#define g_MsgUpdateMemTbl_ITEM_MAX 		SENDBUFLEN

#define APP_TASKUPDATEPROC_STK_SIZE		 4096u
#define APP_CFG_TASK_UPDATEPROC_PRIO   (OS_LOWEST_PRIO-6)


//#define DEBUG_ETHERNET_UPDATE
#ifdef DEBUG_ETHERNET_UPDATE
	#define Debug_Update(format, ...) 	Debug_Output_Ethernet(GetOwnPort(),format, ##__VA_ARGS__)
#else
#ifdef DEBUG_CAN_UPDATE
	#define Debug_Update(format, ...)   Debug_Output_CAN(GetMasterCan(),GetOwnPort(),format, ##__VA_ARGS__)
#else
	#define Debug_Update(format, ...)	NULL
#endif
#endif

void WriteBootConfigArea(uint32_t len);
uint8_t CreatUpdateProc();
uint8_t UpdateDataIn(Tran_ptCIR_Frame *pTF,uint8_t ch);
void ResetToBootloader(void);
Bool ProgressOut(uint8_t sport,uint8_t enter,uint8_t type,char *str,float progress);
Bool WriteFlash_Update(uint32_t dst,const uint8_t *data,uint32_t len);
Bool ReadFlash_Update(uint32_t dst, uint32_t *src, uint32_t len);
Bool SendDataUpdate(uint8_t *FrameAr,uint16_t Framelen,uint8_t op,uint8_t cmd, uint8_t sport,uint8_t dport,uint8_t *data, uint16_t datalen,uint8_t interface);
#endif
