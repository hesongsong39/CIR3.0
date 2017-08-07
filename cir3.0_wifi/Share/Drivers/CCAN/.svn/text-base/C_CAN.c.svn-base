/**********************************************************************
* $Id$      C_CAN              2016-01-07
*//**
* @file     C_CAN.c
* @brief    C_CAN总线驱动
* @version  1.0.1
* @date     2016/01/07 星期四 20:58:36
* @author   ZhangYanMing
*
* Copyright(C) 2016, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
/**************************** Includes **********************************/
#include "lpc_types.h"
#include <ucos_ii.h>
#include <string.h>
#include "C_CAN.h"
#include "ccan_18xx_43xx.h"
#include "DriverConfig.h"
#include "LedControl.h"

/*****************Private variables ***********************************/
static uint32_t tx_mo [2] = { 0 };
static uint32_t rx_mo [2] = { 0 };
CAN_Queue CANRxqueue;


#ifdef _CCAN0
static CANInterface g_CANI0;  	
static CCAN_MSG_OBJECT CAN0TxBuf[CAN0_BUFFER_SIZE] = {0};
static CCAN_MSG_OBJECT CAN0RxBuf [32] ;
static uint8_t CAN0RxLen = 0;
#endif

#ifdef _CCAN1
static CANInterface g_CANI1;
static CCAN_MSG_OBJECT CAN1TxBuf[CAN1_BUFFER_SIZE] = {0};
static CCAN_MSG_OBJECT CAN1RxBuf [32] ;
static uint8_t CAN1RxLen = 0;
#endif

/*****************Public variables ************************************/


/*****************Extern variables ************************************/

/*********************************************************************//**
 * @brief		向CAN发送缓冲区添加数据
 * @param[in]	pCANI，CAN相应端口数据结构指针
 * @param[in]	pMsgObj，CAN数据对象指针
 * @param[in]	plen，数据长度
 * @param[in]	totalnum，数据总长度
 * @return 		TRUE，添加成功
              		FALSE，添加失败
 **********************************************************************/
static Bool EnQueue(CANInterface *pCANI,CCAN_MSG_OBJECT *pMsgObj,uint16_t plen,uint16_t totalnum)
{
		Bool retval = TRUE;
		uint16_t i = 0;
		#if OS_CRITICAL_METHOD == 3                         
			OS_CPU_SR  cpu_sr;
		#endif
		
		OS_ENTER_CRITICAL();
		// 状态检查
		if(pCANI->pWrite >= pCANI->pRead)
		{
			if((pCANI->pWrite - pCANI->pRead + totalnum) > (pCANI->SendBufSize - 1) )
			{
				OS_EXIT_CRITICAL();
				return FALSE; 
			}
					
		}
		else
		{
			if((pCANI->SendBufSize - pCANI->pRead + pCANI->pWrite + totalnum) > (pCANI->SendBufSize - 1))
			{
				OS_EXIT_CRITICAL();
				return FALSE; 
			}
		}
			
		for(i=0;i<plen;i++)
		{
			pCANI->SendBuf[pCANI->pWrite].id = pMsgObj->id;
			pCANI->SendBuf[pCANI->pWrite].dlc = pMsgObj->dlc;
			memcpy(pCANI->SendBuf[pCANI->pWrite].data, pMsgObj->data,8);
			pCANI->pWrite++;
			pMsgObj++;
			if(pCANI->pWrite >= pCANI->SendBufSize)
			{
				pCANI->pWrite = 0;
			}
		}
		OS_EXIT_CRITICAL();
		
		return retval;
}

/*********************************************************************//**
 * @brief		从CAN发送缓冲区取出数据
 * @param[in]	pCANI，CAN相应端口数据结构指针
 * @param[in]	pMsgObj，CAN数据对象指针
 * @return 	     
 **********************************************************************/

static INLINE void DeQueue(CANInterface *pCANI,CCAN_MSG_OBJ_T *pMsgObj)
{
	pMsgObj->id = pCANI->SendBuf[pCANI->pRead].id;
	pMsgObj->dlc = (uint32_t)pCANI->SendBuf[pCANI->pRead].dlc;
	memcpy(pMsgObj->data,pCANI->SendBuf[pCANI->pRead].data,8);
	pCANI->pRead++;
	if(pCANI->pRead >= pCANI->SendBufSize)
	{
		pCANI->pRead = 0;
	}
}


/*********************************************************************//**
 * @brief			获取CCAN控制器状态
 * @param[in]	CanNum,CCAN号
 * @return 		None
 **********************************************************************/
uint8_t CANStatusGet(uint8_t CanNum)
{
		CANInterface *pCANI = NULL;
		switch(CanNum)
		{
			case 0:
				{
					#ifdef _CCAN0
						pCANI =  &g_CANI0;
					#endif
				}
				break;
			case 1:
				{
					#ifdef _CCAN1
						pCANI =  &g_CANI1;
					#endif
				}
				break;
			default:
				break;
		}
		return pCANI->CANStatus;
}


#ifdef _CCAN0

/*********************************************************************//**
 * @brief		C_CAN0 interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void CAN0_IRQHandler(void)
{
	CCAN_MSG_OBJ_T msg_buf;
	uint32_t can_int, can_stat;
	uint8_t i = 0;
	uint32_t msg_interrupt;
	CANInterface *pCANI = &g_CANI0;
	
	OS_CPU_SR  cpu_sr;
	
	OS_ENTER_CRITICAL();                                        /* Tell uC/OS-II that we are starting an ISR            */
	OSIntNesting++;
	OS_EXIT_CRITICAL();
	CAN0RxLen = 0;
	while ( (can_int = Chip_CCAN_GetIntID(LPC_CCAN0)) != 0 )
	{
		if (can_int & CCAN_INT_STATUS)
		{
			can_stat = Chip_CCAN_GetStatus(LPC_CCAN0);
			// TODO with error or TXOK, RXOK
			if (can_stat & CCAN_STAT_EPASS)
			{
				pCANI->CANStatus = DEVICE_C_CAN_PASSIVE;
				OSIntExit(); 
				return;
			}
			if (can_stat & CCAN_STAT_EWARN)
			{
				OSIntExit(); 
				return;
			}
			if (can_stat & CCAN_STAT_BOFF)
			{ 
				pCANI->CANStatus = DEVICE_C_CAN_BOFF;
				OSIntExit();
				return;
			}
			pCANI->CANStatus = DEVICE_C_CAN_ACTIVE;
			Chip_CCAN_ClearStatus(LPC_CCAN0, CCAN_STAT_TXOK);
			Chip_CCAN_ClearStatus(LPC_CCAN0, CCAN_STAT_RXOK);
		}
		else if ((1 <= CCAN_INT_MSG_NUM(can_int)) && (CCAN_INT_MSG_NUM(can_int) <= 0x20))
		{	
			msg_interrupt = Chip_CCAN_GetInterruptMsg(LPC_CCAN0);
			for(i= can_int;i<=32;i++)
			{
					if (((msg_interrupt >> (i - 1)) & 1UL))
					{
							// Process msg num canint
							if (tx_mo[0] & (1 << (i-1)))
							{
									//发送完成读取相应消息对象作用是消除INDPND位，防止一直在中断服务程序中无法正常跳出
									Chip_CCAN_GetMsgObject(LPC_CCAN0, CCAN_MSG_IF1, i ,&msg_buf);
									if(pCANI->pRead == pCANI->pWrite)
									{
											freeMsgObject(LPC_CCAN0, CCAN_MSG_IF1, i);
											tx_mo[0] &= ~(1 << (i-1));        /* Clear obj as not used by tx*/
											continue;
									}
									DeQueue(pCANI,&msg_buf);
									Chip_CCAN_SetMsgObject(LPC_CCAN0, CCAN_MSG_IF1, CCAN_TX_DIR, FALSE,TRUE,i, &msg_buf,0);		
									BoardLightOn(LED_CAN0);
							}
							else if(rx_mo[0] & (1 << (i-1)))
							{
									Chip_CCAN_GetMsgObject(LPC_CCAN0, CCAN_MSG_IF1, i ,&msg_buf);
									CAN0RxBuf[CAN0RxLen].id = msg_buf.id;
									CAN0RxBuf[CAN0RxLen].dlc = msg_buf.dlc;
									memcpy(CAN0RxBuf[CAN0RxLen].data,msg_buf.data,8);
									CAN0RxLen++;
									//清除相应编号消象NEWDATA位
									Chip_CCAN_ClearNewDataFlag(LPC_CCAN0,CCAN_MSG_IF1, i);
									BoardLightOn(LED_CAN0);
							}
					}
			}
		}
	}
	if(CAN0RxLen > 0)
	{
		for(i=0;i<CAN0RxLen;i++)
		{
			CANRxqueue.pmsg[CANRxqueue.pwrite].DeviceIndex = 0;
			CANRxqueue.pmsg[CANRxqueue.pwrite].Canmsg = CAN0RxBuf[i];
			CANRxqueue.pwrite++;
			if(CANRxqueue.pwrite >= CCANQUEUESIZE)
			{
				CANRxqueue.pwrite = 0;
			}
		}
	}
	OSIntExit();                                                /* Tell uC/OS-II that we are leaving the ISR       */
}

#endif



#ifdef _CCAN1

/*********************************************************************//**
 * @brief		C_CAN1 interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void CAN1_IRQHandler(void)
{
	CCAN_MSG_OBJ_T msg_buf;
	uint32_t can_int, can_stat;
	uint8_t i = 0;

	uint32_t msg_interrupt;
	CANInterface *pCANI = &g_CANI1;
	OS_CPU_SR  cpu_sr;
	
	OS_ENTER_CRITICAL();                                        /* Tell uC/OS-II that we are starting an ISR            */
	OSIntNesting++;
	OS_EXIT_CRITICAL();
	CAN1RxLen = 0;
	while ( (can_int = Chip_CCAN_GetIntID(LPC_CCAN1)) != 0 )
	{
		if (can_int & CCAN_INT_STATUS)
		{
			can_stat = Chip_CCAN_GetStatus(LPC_CCAN1);
			// TODO with error or TXOK, RXOK
			if (can_stat & CCAN_STAT_EPASS)
			{
				pCANI->CANStatus = DEVICE_C_CAN_PASSIVE;
				OSIntExit(); 
				return;
			}
			if (can_stat & CCAN_STAT_EWARN)
			{
				OSIntExit(); 
				return;
			}
			if (can_stat & CCAN_STAT_BOFF)
			{
				pCANI->CANStatus = DEVICE_C_CAN_BOFF;
				OSIntExit();
				return;
			}
			pCANI->CANStatus = DEVICE_C_CAN_ACTIVE;
			Chip_CCAN_ClearStatus(LPC_CCAN1, CCAN_STAT_TXOK);
			Chip_CCAN_ClearStatus(LPC_CCAN1, CCAN_STAT_RXOK);
		}
		else if ((1 <= CCAN_INT_MSG_NUM(can_int)) && (CCAN_INT_MSG_NUM(can_int) <= 0x20))
		{
			msg_interrupt = Chip_CCAN_GetInterruptMsg(LPC_CCAN1);
			for(i= can_int;i<=32;i++)
			{
					if (((msg_interrupt >> (i - 1)) & 1UL))
					{
														// Process msg num canint
							if (tx_mo[1] & (1 << (i-1)))
							{
									//发送完成读取相应消息对象作用是消除INDPND位，防止一直在中断服务程序中无法正常跳出
									Chip_CCAN_GetMsgObject(LPC_CCAN1, CCAN_MSG_IF1, i ,&msg_buf);
									if(pCANI->pRead == pCANI->pWrite)
									{
											freeMsgObject(LPC_CCAN1, CCAN_MSG_IF1, i);
											tx_mo[1] &= ~(1 << (i-1));        /* Clear obj as not used by tx*/
											continue;
									}
									DeQueue(pCANI,&msg_buf);
									Chip_CCAN_SetMsgObject(LPC_CCAN1, CCAN_MSG_IF1, CCAN_TX_DIR, FALSE,TRUE,i, &msg_buf,0);	
									BoardLightOn(LED_CAN1);
							}
							else if(rx_mo[1] & (1 << (i-1)))
							{
									Chip_CCAN_GetMsgObject(LPC_CCAN1, CCAN_MSG_IF1, i ,&msg_buf);
									//添加到发送接收队列
									CAN1RxBuf[CAN1RxLen].id = msg_buf.id;
									CAN1RxBuf[CAN1RxLen].dlc = msg_buf.dlc;
									memcpy(CAN1RxBuf[CAN1RxLen].data,msg_buf.data,8);
									CAN1RxLen++;
									//清除相应编号消象NEWDATA位
									Chip_CCAN_ClearNewDataFlag(LPC_CCAN1,CCAN_MSG_IF1, i);
									BoardLightOn(LED_CAN1);
							}
					}
			}
		}
	}
	if(CAN1RxLen > 0)
	{
		for(i=0;i<CAN1RxLen;i++)
		{
			CANRxqueue.pmsg[CANRxqueue.pwrite].DeviceIndex = 1;
			CANRxqueue.pmsg[CANRxqueue.pwrite].Canmsg = CAN1RxBuf[i];
			CANRxqueue.pwrite++;
			if(CANRxqueue.pwrite >= CCANQUEUESIZE)
			{
				CANRxqueue.pwrite = 0;
			}
		}
	}
	OSIntExit();                                                /* Tell uC/OS-II that we are leaving the ISR       */
}

#endif



/*********************************************************************//**
 * @brief		获得 CAN 相应端口数据结构指针
 * @param[in]	CanNum	，CAN口号
 * @return 		pCANI，CAN 相应端口数据结构指针
 **********************************************************************/
CANInterface *GetpCANIbyNum(uint8_t CanNum)
{
	CANInterface *pCANI = NULL;
	switch(CanNum)
	{
		case 0:
			{
				#ifdef _CCAN0
					pCANI =  &g_CANI0;
				#endif
			}
			break;
		case 1:
			{
				#ifdef _CCAN1
					pCANI =  &g_CANI1;
				#endif
			}
			break;
		default:
			break;
	}

	return pCANI;
}

/*********************************************************************//**
 * @brief		  Register a message ID for receiving
 * @param[in]	
 * @return 		
 **********************************************************************/
Bool CANMessageAdd(uint8_t CanNum,uint32_t id,uint32_t MaskBit,Bool EOBstate,Bool Extend)
{
    CCAN_MSG_OBJ_T temp;
	uint8_t msgNum = 0;
	OS_CPU_SR  cpu_sr;

	CANInterface *pCANI = NULL;
		
		
	//检测参数
	if (CanNum > 1)
	{
		return FALSE;	
	}

	//获得相应CAN 口数据结构指针
	pCANI = GetpCANIbyNum(CanNum);
	if(pCANI == NULL)
	{
		return FALSE;	
	}
	
	OS_ENTER_CRITICAL(); 
    msgNum = getFreeMsgObject(pCANI->pCCAN);
    if (!msgNum) 
	{
		OS_EXIT_CRITICAL();
		return FALSE;
    }
		if(pCANI->pCCAN == LPC_CCAN1)
		{
				rx_mo[1] |= (1 << (msgNum - 1));
		}
		else
		{
				rx_mo[0] |= (1 << (msgNum - 1));
		}
	if(Extend == TRUE)
	{
		temp.id = id | (1<<30);
	}
	else
	{
		temp.id = id ;
	}
    
    Chip_CCAN_SetMsgObject(pCANI->pCCAN, CCAN_MSG_IF1, CCAN_RX_DIR, FALSE, EOBstate ,msgNum, &temp,MaskBit);
	OS_EXIT_CRITICAL();
		
	return TRUE;
}

/*********************************************************************//**
 * @brief		  Clear the can message
 * @param[in]	
 * @return 		
 **********************************************************************/
void CANMessageClear(uint8_t CanNum)
{
	uint8_t i = 0;
    OS_CPU_SR  cpu_sr;

	OS_ENTER_CRITICAL();
	switch(CanNum)
	{
		case 0:
			{
				tx_mo [0] = 0;                 
				rx_mo [0] = 0;
				for (i = 1; i <= CCAN_MSG_MAX_NUM; i++)
				{
					freeMsgObject(LPC_CCAN0, CCAN_MSG_IF1, i);
				}
			}
			break;
		case 1:
			{
				tx_mo [1] = 0;                
				rx_mo [1] = 0;
				for (i = 1; i <= CCAN_MSG_MAX_NUM; i++)
				{
					freeMsgObject(LPC_CCAN1, CCAN_MSG_IF1, i);
				}
			}
			break;
		default:
			break;
	}
	OS_EXIT_CRITICAL();
}

/*********************************************************************//**
 * @brief		CAN口发送缓冲大小设置
 * @param[in]	CanNum ,CAN口端号
 * @param[in]     Buf ，CAN口发送数据缓冲
 * @param[in]     BufSize ，CAN口发送数据缓冲大小
 * @return 		TRUE/FALSE
 **********************************************************************/
Bool CANSendBufSet(uint8_t CanNum,CCAN_MSG_OBJECT *Buf,uint16_t BufSize)
{
	#if OS_CRITICAL_METHOD == 3                         
        OS_CPU_SR  cpu_sr;
	#endif
	CANInterface *pCANI = NULL; 

	//获得相应CAN 口端号数据结构指针
	pCANI = GetpCANIbyNum(CanNum);
	if(pCANI == NULL)
	{
		return FALSE;
	}
	
	OS_ENTER_CRITICAL();
	pCANI->SendBuf = Buf;
	pCANI->SendBufSize = BufSize;
	OS_EXIT_CRITICAL();

	return TRUE;
}




/*********************************************************************//**
 * @brief	  CAN初始化函数
 * @param[in]	CanNum	CAN口号
 * @param[in] bitRate 波特率
 * @param[in] callback CAN口收数据处理函数指针
 * @param[in] fpininit CAN口引脚初始化函数指针
 * @return 		TRUE/FALSE
 **********************************************************************/
Bool CANInit(uint8_t CanNum,uint32_t bitRate,pFnfCanpininit fpininit)
{
		uint8_t i = 0;
		uint32_t pClk = 0;
		CANInterface *pCANI = NULL; 
	
		//检测参数
		if (CanNum > 1)
		{
			return FALSE;
		}
		if(fpininit == NULL)
		{
			return FALSE;
		}
		//获得相应CAN口端号数据结构指针
		pCANI = GetpCANIbyNum(CanNum);
		if(pCANI == NULL)
		{
			return FALSE;
		}
		//初始化串口引脚，与硬件引脚设计有关，根据硬件原理图配置
		fpininit(CanNum);
		//初始化CAN口相关数据结构
		pCANI->Num = CanNum;
		pCANI->Baudrate = bitRate;
		
		pCANI->pRead = 0;
		pCANI->pWrite = 0;
		//CAN控制器初始化状态为ERROR ACTIVE
		pCANI->CANStatus = DEVICE_C_CAN_ACTIVE;
		switch(CanNum)
		{
				case 0:
					{
							tx_mo [0] = 0;                 /* Clear transmit message objects mask */
							rx_mo [0] = 0;                 /* Clear receive  message objects mask */
							pCANI->pCCAN = LPC_CCAN0;
							
							CGU_EntityConnect(CGU_CLKSRC_PLL1, CGU_BASE_APB3);
							pClk = CGU_GetPCLKFrequency(CGU_PERIPHERAL_APB3_BUS);
		
							/* Enable C_CAN0 interrupt                                           */
							NVIC_SetPriority (C_CAN0_IRQn, 1);  /* Set C_CAN0 irq priority       */
							NVIC_EnableIRQ  (C_CAN0_IRQn);      /* Enable C_CAN0 interrupt       */

							#ifdef _CCAN0
							CANSendBufSet(CanNum,CAN0TxBuf,CAN0_BUFFER_SIZE);
							#endif
					}
					break;
				case 1:
					{
							tx_mo [1] = 0;                 /* Clear transmit message objects mask */
							rx_mo [1] = 0;                 /* Clear receive  message objects mask */
							pCANI->pCCAN = LPC_CCAN1;
							CGU_EntityConnect(CGU_CLKSRC_PLL1, CGU_BASE_APB1);
							pClk = CGU_GetPCLKFrequency(CGU_PERIPHERAL_APB1_BUS);
		
							/* Enable C_CAN1 interrupt                                           */
							NVIC_SetPriority (C_CAN1_IRQn, 1);  /* Set C_CAN1 irq priority       */
							NVIC_EnableIRQ  (C_CAN1_IRQn);      /* Enable C_CAN1 interrupt       */

							#ifdef _CCAN1
							CANSendBufSet(CanNum,CAN1TxBuf,CAN1_BUFFER_SIZE);
							#endif
					}
					break;
				default:
					break;
		}
		//位速率设置
		Chip_CCAN_SetBitRate(pCANI->pCCAN, pClk, bitRate);
		//中断使能
		Chip_CCAN_EnableInt(pCANI->pCCAN, (CCAN_CTRL_IE | CCAN_CTRL_SIE | CCAN_CTRL_EIE));
		
		//消息对象初始化
		for (i = 1; i <= CCAN_MSG_MAX_NUM; i++)
		{
			freeMsgObject(pCANI->pCCAN, CCAN_MSG_IF1, i);
		}
		//清除接收、发送成功标识
		Chip_CCAN_ClearStatus(pCANI->pCCAN, (CCAN_STAT_RXOK | CCAN_STAT_TXOK));
		
		return TRUE;
}



/*********************************************************************//**
 * @brief		Send data through CAN peripheral
 * @param[in]	CanNum	CAN口号
 * @param[in]     pMsgObj， 发送数据缓冲
 * @param[in]     plen 发送数据长度
 * @param[in]     totalnum 发送数据长度
 * @return 		
 **********************************************************************/
CCAN_MSG_STATE CANSend(uint8_t CanNum, CCAN_MSG_OBJECT *pMsgObj,uint16_t plen,uint16_t totalnum)
{
		uint8_t msgNum ;
		CANInterface *pCANI = NULL;
		CCAN_MSG_OBJ_T msg_buf;
		OS_CPU_SR  cpu_sr;
		
		//检测参数
		if (CanNum > 1)
		{
			return CCAN_NONE;
		}

		//获得相应CAN 口数据结构指针
		pCANI = GetpCANIbyNum(CanNum);
		if(pCANI == NULL)
		{
			return CCAN_NONE;
		}
	
		OS_ENTER_CRITICAL();                                       
		if(pCANI->pCCAN == LPC_CCAN1)
		{
				//判断相应CAN端口是否正常
			  if(pCANI->CANStatus == DEVICE_C_CAN_BOFF)
				{
						OS_EXIT_CRITICAL();
						return CCAN_BUSOFF;
				}
				//判断是否已经有消息对象发送数据
				if(tx_mo[1] == 0)
				{
						msgNum = getFreeMsgObject(pCANI->pCCAN);
						if (!msgNum) 
						{
								OS_EXIT_CRITICAL();
								return CCAN_MSG_NONE;
						}
						tx_mo[1] |= (1 << (msgNum - 1));
						
						if(EnQueue(pCANI,pMsgObj,plen,totalnum) == FALSE)
						{
							freeMsgObject(pCANI->pCCAN, CCAN_MSG_IF1, msgNum);
							tx_mo[1] &= ~(1 << (msgNum-1)); 
							OS_EXIT_CRITICAL();
							return CCAN_BUFFER_FULL;
						}
						DeQueue(pCANI,&msg_buf);
						Chip_CCAN_SetMsgObject(pCANI->pCCAN, CCAN_MSG_IF1, CCAN_TX_DIR, FALSE,TRUE,msgNum,&msg_buf,0);	
						
						OS_EXIT_CRITICAL();
						return CCAN_MSG_OK;
				}
		}
		else if(pCANI->pCCAN == LPC_CCAN0)
		{
				//判断相应CAN端口是否正常
				if(pCANI->CANStatus == DEVICE_C_CAN_BOFF)
				{
						OS_EXIT_CRITICAL();
						return CCAN_BUSOFF;
				}
				//判断是否已经有消息对象发送数据
				if(tx_mo[0] == 0)
				{
						msgNum = getFreeMsgObject(pCANI->pCCAN);
						if (!msgNum) 
						{
								OS_EXIT_CRITICAL();
								return CCAN_MSG_NONE;
						}
						tx_mo[0] |= (1 << (msgNum - 1));
						if(EnQueue(pCANI,pMsgObj,plen,totalnum) == FALSE)
						{
							freeMsgObject(pCANI->pCCAN, CCAN_MSG_IF1, msgNum);
							tx_mo[0] &= ~(1 << (msgNum-1));
							OS_EXIT_CRITICAL();
							return CCAN_BUFFER_FULL;
						}
						DeQueue(pCANI,&msg_buf);
						Chip_CCAN_SetMsgObject(pCANI->pCCAN, CCAN_MSG_IF1, CCAN_TX_DIR, FALSE,TRUE,msgNum,&msg_buf,0);	
						OS_EXIT_CRITICAL();
						return CCAN_MSG_OK;
				}
		}
		OS_EXIT_CRITICAL();

		if(EnQueue(pCANI,pMsgObj,plen,totalnum) == TRUE)
		{
			return CCAN_MSG_OK;
		}
		else
		{
			return CCAN_BUFFER_FULL;
		}
		
}



/*********************************************************************//**
 * @brief       CCAN报文滤波相关设置
 * @param[in]   None
 * @return      None
 **********************************************************************/
Bool CCAN_RecMessageSet(uint8_t CanNum,uint8_t desaddr,Bool EOBstate)
{
		Bool ret = FALSE;
		uint32_t  id = 0;
		
	  id = ((uint32_t)desaddr) << DESTINATION;
		ret = CANMessageAdd(CanNum,id,CCAN_RX_MASK_ID,EOBstate,TRUE);
		
		return ret;
}

