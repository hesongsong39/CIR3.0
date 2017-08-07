/**********************************************************************
* $Id$      Uart              2015-12-23
*//**
* @file     Uart.c
* @brief    串口的相关操作
* @version  1.0.0
* @date      2015/12/23
* @author   ZhangYanMing
*
* Copyright(C) 2015, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
/**************************** Includes **********************************/
#include <ucos_ii.h>
#include <string.h>
#include "lpc43xx_uart.h"
#include "Uart.h"
#include "DriverConfig.h"
/*****************Private variables ***********************************/

#ifdef _UART0
static UartInterface g_UI0;  	
static uint8_t Uart0TxBuf[UART0_BUFFER_SIZE] = {0};
static uint8_t Uart0RxBuf[UART0_BUFFER_SIZE] = {0};
#endif

#ifdef _UART1
static UartInterface g_UI1;
static uint8_t Uart1TxBuf[UART1_BUFFER_SIZE] = {0};
static uint8_t Uart1RxBuf[UART1_BUFFER_SIZE] = {0};
#endif

#ifdef _UART2
static UartInterface g_UI2;
static uint8_t Uart2TxBuf[UART2_BUFFER_SIZE] = {0};
static uint8_t Uart2RxBuf[UART2_BUFFER_SIZE] = {0};
#endif

#ifdef _UART3
static UartInterface g_UI3;
static uint8_t Uart3TxBuf[UART3_BUFFER_SIZE] = {0};
static uint8_t Uart3RxBuf[UART3_BUFFER_SIZE] = {0};
#endif



/*****************Public variables ************************************/


/*****************Extern variables ************************************/

#ifdef _UART0
/*********************************************************************//**
 * @brief		UARTn interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void UART0_IRQHandler (void)
{
    OS_CPU_SR  cpu_sr;
    uint32_t intsrc, tmp;
		uint8_t fifo_cnt = 0;
		UartInterface *pUI = &g_UI0; 

    OS_ENTER_CRITICAL();																				/* Tell uC/OS-II that we are starting an ISR						*/
    OSIntNesting++;
    OS_EXIT_CRITICAL();
    /* Determine the interrupt source */
    intsrc = LPC_USART0->IIR;
    tmp = intsrc & UART_IIR_INTID_MASK;

    // Receive Data Available or Character time-out
    if ((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI))
    {	    
    	while(LPC_USART0->LSR & UART_LSR_RDR)
    	{
    		//从FIFO 读出 
				pUI->RecBuf[pUI->rpWrite++] = LPC_USART0->RBR;
				if(pUI->rpWrite >= pUI->RecBufSize)
				{
					pUI->rpWrite = 0;
				}
    	}
    }
    // Transmit Holding Empty
    if (tmp == UART_IIR_INTID_THRE)
    {
        /* Wait until THR empty */
        while (UART_CheckBusy(LPC_USART0) == SET);
			
				fifo_cnt = UART_TX_FIFO_SIZE;
				while(fifo_cnt)
				{
						if(pUI->pRead == pUI->pWrite)
            {
								//发送完毕关闭中断
								UART_IntConfig(LPC_USART0, UART_INTCFG_THRE, DISABLE);	
                break;
            }
						fifo_cnt--;
						UART_SendByte(LPC_USART0,pUI->SendBuf[pUI->pRead++]);	
						if(pUI->pRead >= pUI->SendBufSize)
						{
							pUI->pRead = 0;
						}								
				}
    }

    OSIntExit(); 																							 /* Tell uC/OS-II that we are leaving the ISR 					 */

}

#endif


#ifdef _UART1

/*********************************************************************//**
 * @brief		UARTn interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void UART1_IRQHandler(void)
{
    OS_CPU_SR  cpu_sr;
    uint32_t intsrc, tmp;
		LPC_USARTn_Type *UART1;
		uint8_t fifo_cnt = 0;
		UartInterface *pUI = &g_UI1;

    OS_ENTER_CRITICAL();																				/* Tell uC/OS-II that we are starting an ISR						*/
    OSIntNesting++;
    OS_EXIT_CRITICAL();

    /* Determine the interrupt source */
    intsrc = LPC_UART1->IIR;
    tmp = intsrc & UART_IIR_INTID_MASK;

    // Receive Data Available or Character time-out
    if ((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI))
    {
    	UART1 = (LPC_USARTn_Type *)LPC_UART1;
			while(UART1->LSR & UART_LSR_RDR)
    	{
    		//从FIFO 读出 
				pUI->RecBuf[pUI->rpWrite++] = UART1->RBR;
				if(pUI->rpWrite >= pUI->RecBufSize)
				{
					pUI->rpWrite = 0;
				}
    	}
    }
    // Transmit Holding Empty
    if (tmp == UART_IIR_INTID_THRE)
    {
        /* Wait until THR empty */
        while (UART_CheckBusy((LPC_USARTn_Type *)LPC_UART1) == SET);
				
				fifo_cnt = UART_TX_FIFO_SIZE;
				while(fifo_cnt)
				{
						if(pUI->pRead == pUI->pWrite)
            {
								//发送完毕关闭中断
								UART_IntConfig((LPC_USARTn_Type *)LPC_UART1, UART_INTCFG_THRE, DISABLE);	
                break;
            }
						fifo_cnt--;
						UART_SendByte((LPC_USARTn_Type *)LPC_UART1, pUI->SendBuf[pUI->pRead++]);
						if(pUI->pRead >= pUI->SendBufSize)
						{
							pUI->pRead = 0;
						}								
				}
    }

    OSIntExit(); 																							 /* Tell uC/OS-II that we are leaving the ISR 					 */

}

#endif



#ifdef _UART2

/*********************************************************************//**
 * @brief		UARTn interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void UART2_IRQHandler(void)
{
    OS_CPU_SR  cpu_sr;
    uint32_t intsrc, tmp;
		uint8_t fifo_cnt = 0;
		UartInterface *pUI = &g_UI2;

    OS_ENTER_CRITICAL();																				/* Tell uC/OS-II that we are starting an ISR						*/
    OSIntNesting++;
    OS_EXIT_CRITICAL();
    /* Determine the interrupt source */
    intsrc = LPC_USART2->IIR;
    tmp = intsrc & UART_IIR_INTID_MASK;

    // Receive Data Available or Character time-out
    if ((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI))
    {
			while(LPC_USART2->LSR & UART_LSR_RDR)
    	{
    		//从FIFO 读出 
				pUI->RecBuf[pUI->rpWrite++] = LPC_USART2->RBR;
				if(pUI->rpWrite >= pUI->RecBufSize)
				{
					pUI->rpWrite = 0;
				}
    	}
    }
    // Transmit Holding Empty
    if (tmp == UART_IIR_INTID_THRE)
    {
        /* Wait until THR empty */
        while (UART_CheckBusy(LPC_USART2) == SET);
				
				fifo_cnt = UART_TX_FIFO_SIZE;
				while(fifo_cnt)
				{
						if(pUI->pRead == pUI->pWrite)
            {
								//发送完毕关闭中断
								UART_IntConfig(LPC_USART2, UART_INTCFG_THRE, DISABLE);	
                break;
            }
						fifo_cnt--;
						UART_SendByte(LPC_USART2, pUI->SendBuf[pUI->pRead++]);
						if(pUI->pRead >= pUI->SendBufSize)
						{
							pUI->pRead = 0;
						}								
				}
    }

    OSIntExit(); 																							 /* Tell uC/OS-II that we are leaving the ISR 					 */

}

#endif


#ifdef _UART3

/*********************************************************************//**
 * @brief		UARTn interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void UART3_IRQHandler(void)
{
    OS_CPU_SR  cpu_sr;
    uint32_t intsrc, tmp;
		uint8_t fifo_cnt = 0;
		UartInterface *pUI = &g_UI3;
	
    OS_ENTER_CRITICAL();																				/* Tell uC/OS-II that we are starting an ISR						*/
    OSIntNesting++;
    OS_EXIT_CRITICAL();

    /* Determine the interrupt source */
    intsrc = LPC_USART3->IIR;
    tmp = intsrc & UART_IIR_INTID_MASK;

    // Receive Data Available or Character time-out
    if ((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI))
    {
			while(LPC_USART3->LSR & UART_LSR_RDR)
    	{
    		//从FIFO 读出 	
				pUI->RecBuf[pUI->rpWrite++] = LPC_USART3->RBR;
				if(pUI->rpWrite >= pUI->RecBufSize)
				{
					pUI->rpWrite = 0;
				}
    	}
    }
    // Transmit Holding Empty
    if (tmp == UART_IIR_INTID_THRE)
    {
        /* Wait until THR empty */
        while (UART_CheckBusy(LPC_USART3) == SET);
			
				fifo_cnt = UART_TX_FIFO_SIZE;
				while(fifo_cnt)
				{
						if(pUI->pRead == pUI->pWrite)
            {
								//发送完毕关闭中断
								UART_IntConfig(LPC_USART3, UART_INTCFG_THRE, DISABLE);
                break;
            }
						fifo_cnt--;
						UART_SendByte(LPC_USART3, pUI->SendBuf[pUI->pRead++]);
						if(pUI->pRead >= pUI->SendBufSize)
						{
							pUI->pRead = 0;
						}								
				}
    }

    OSIntExit(); 																							 /* Tell uC/OS-II that we are leaving the ISR 					 */
}

#endif

UartInterface *GetpUIbyNum(uint8_t uartnum)
{
	UartInterface *pUI = NULL;
	switch(uartnum)
	{
		case 0:
			{
				#ifdef _UART0
					pUI =  &g_UI0;
				#endif
			}
			break;
		case 1:
			{
				#ifdef _UART1
					pUI =  &g_UI1;
				#endif
			}
			break;
		case 2:
			{
				#ifdef _UART2
					pUI =  &g_UI2;
				#endif
			}
			break;
		case 3:
			{
				#ifdef _UART3
					pUI =  &g_UI3;
				#endif
			}
			break;
		default:
			break;
	}

	return pUI;
}

/*********************************************************************//**
 * @brief 		UART configuration
 * @param[in]	uartnum 物理串口号0/1/2/3
 * @param[in]     Baudrate 串口波特率
 * @return 		TRUE/FALSE
 **********************************************************************/
static Bool UartConfig(uint8_t uartnum,uint32_t Baudrate)
{
	// UART Configuration structure variable
	UART_CFG_Type UARTConfigStruct;
	// UART FIFO configuration Struct variable
	UART_FIFO_CFG_Type UARTFIFOConfigStruct;
	LPC_USARTn_Type* UARTx;

	switch (uartnum)
	{
		case 0:
			{
				UARTx = LPC_USART0;
			}
			break;
		case 1:
			{
				UARTx = (LPC_USARTn_Type *)LPC_UART1;
			}
			break;
		case 2:
			{
				UARTx = LPC_USART2;
			}
			break;
		case 3:
			{
				UARTx = LPC_USART3;
			}
			break;
		default:
			return FALSE;															//Error-return
	}

	/* Initialize UART Configuration parameter structure to default state:
	 * 8 data bit
	 * 1 Stop bit
	 * None parity
	 */
	UARTConfigStruct.Databits = UART_DATABIT_8;
	UARTConfigStruct.Parity = UART_PARITY_NONE;
	UARTConfigStruct.Stopbits = UART_STOPBIT_1;
	UARTConfigStruct.Clock_Speed = 0;
	UARTConfigStruct.Baud_rate = Baudrate;

	// Initialize UART peripheral with given to corresponding parameter
	UART_Init(UARTx, &UARTConfigStruct);

	/* Initialize FIFOConfigStruct to default state:
	 *				- FIFO_DMAMode = DISABLE
	 *				- FIFO_Level = UART_FIFO_TRGLEV2
	 *				- FIFO_ResetRxBuf = ENABLE
	 *				- FIFO_ResetTxBuf = ENABLE
	 *				- FIFO_State = ENABLE
	 */
	UARTFIFOConfigStruct.FIFO_DMAMode = DISABLE;
	UARTFIFOConfigStruct.FIFO_Level = UART_FIFO_TRGLEV2;			/* Receive 8byte trigger ISR */
	UARTFIFOConfigStruct.FIFO_ResetRxBuf = ENABLE;
	UARTFIFOConfigStruct.FIFO_ResetTxBuf = ENABLE;

	// Initialize FIFO for UART peripheral
	UART_FIFOConfig(UARTx, &UARTFIFOConfigStruct);

	// Enable UART Transmit
	UART_TxCmd(UARTx, ENABLE);

	/* Enable UART Rx interrupt */
	UART_IntConfig(UARTx, UART_INTCFG_RBR, ENABLE);

	switch (uartnum)
	{
		case 0:
			{
				NVIC_SetPriority(USART0_IRQn, ((0x01<<3)|0x02));	/* preemption = 1, sub-priority = 1 */
				NVIC_EnableIRQ(USART0_IRQn);
			}
			break;
		case 1:
			{
				NVIC_SetPriority(UART1_IRQn, ((0x01<<3)|0x02)); /* preemption = 1, sub-priority = 1 */
				NVIC_EnableIRQ(UART1_IRQn);
			}
			break;
		case 2:
			{
				NVIC_SetPriority(USART2_IRQn, ((0x01<<3)|0x02));	/* preemption = 1, sub-priority = 1 */
				NVIC_EnableIRQ(USART2_IRQn);
			}
			break;
		case 3:
			{
				NVIC_SetPriority(USART3_IRQn, ((0x01<<3)|0x02));	/* preemption = 1, sub-priority = 1 */
				NVIC_EnableIRQ(USART3_IRQn);
			}
			break;
		default:
			break;
	}

	return TRUE;
}


/*********************************************************************//**
 * @brief		Send data through UART peripheral
 * @param[in]	uartnum	串口号
 * @param[in]     pdata 发送数据缓冲
 * @param[in]     plen 发送数据长度
 * @return 		TRUE/FALSE
 **********************************************************************/
Bool UartSend(uint8_t uartnum, uint8_t *pdata, uint16_t plen)
{
  	Bool ret = TRUE;
	uint16_t i = 0;
	#if OS_CRITICAL_METHOD == 3                         
        OS_CPU_SR  cpu_sr;
	#endif

	UartInterface *pUI = NULL; 
	
  	//检测参数
	if (uartnum > 3)
	{
		return FALSE;
	}

	//获得相应串口端号数据结构指针
	pUI = GetpUIbyNum(uartnum);
	if(pUI == NULL)
	{
		return FALSE;
	}
	
	OS_ENTER_CRITICAL();
	// 状态检查
	if(pUI->pWrite >= pUI->pRead)
	{
			if((pUI->pWrite - pUI->pRead + plen) > pUI->SendBufSize )
			{
					OS_EXIT_CRITICAL();
					return FALSE; 
			}
				
	}
	else
	{
			if((pUI->SendBufSize - pUI->pRead + pUI->pWrite + plen) > pUI->SendBufSize)
			{
					OS_EXIT_CRITICAL();
					return FALSE; 
			}
	}
	
	for(i=0;i<plen;i++)
	{
		pUI->SendBuf[pUI->pWrite++] = *(pdata + i);
		if(pUI->pWrite >= pUI->SendBufSize)
		{
			pUI->pWrite = 0;
		}
	}

	OS_EXIT_CRITICAL();
	
	switch (uartnum)
	{
		case 0:
			{
					// Disable THRE interrupt
					UART_IntConfig(LPC_USART0, UART_INTCFG_THRE, DISABLE);
					/* Wait until THR empty */
					while (UART_CheckBusy(LPC_USART0) == SET);
					//放入THR触发中断
					UART_SendByte(LPC_USART0, pUI->SendBuf[pUI->pRead++]);			
					if(pUI->pRead >= pUI->SendBufSize)
					{
						pUI->pRead = 0;
					}
					UART_IntConfig(LPC_USART0, UART_INTCFG_THRE, ENABLE);
			}
			break;
		case 1:
			{
					// Disable THRE interrupt
					UART_IntConfig((LPC_USARTn_Type *)LPC_UART1, UART_INTCFG_THRE, DISABLE);
					/* Wait until THR empty */
					while (UART_CheckBusy((LPC_USARTn_Type *)LPC_UART1) == SET);
					//放入THR触发中断
					UART_SendByte((LPC_USARTn_Type *)LPC_UART1, pUI->SendBuf[pUI->pRead++]);			
					if(pUI->pRead >= pUI->SendBufSize)
					{
						pUI->pRead = 0;
					}
					UART_IntConfig((LPC_USARTn_Type *)LPC_UART1, UART_INTCFG_THRE, ENABLE);
			}
			break;
		case 2:
			{
					// Disable THRE interrupt
					UART_IntConfig(LPC_USART2, UART_INTCFG_THRE, DISABLE);
					/* Wait until THR empty */
					while (UART_CheckBusy(LPC_USART2) == SET);
					//放入THR触发中断
					UART_SendByte(LPC_USART2, pUI->SendBuf[pUI->pRead++]);		
					if(pUI->pRead >= pUI->SendBufSize)
					{
						pUI->pRead = 0;
					}				
					UART_IntConfig(LPC_USART2, UART_INTCFG_THRE, ENABLE);
			}
			break;
		case 3:
			{
					// Disable THRE interrupt
					UART_IntConfig(LPC_USART3, UART_INTCFG_THRE, DISABLE);
					/* Wait until THR empty */
					while (UART_CheckBusy(LPC_USART3) == SET);
					//放入THR触发中断
					UART_SendByte(LPC_USART3, pUI->SendBuf[pUI->pRead++]);	
					if(pUI->pRead >= pUI->SendBufSize)
					{
						pUI->pRead = 0;
					}				
					UART_IntConfig(LPC_USART3, UART_INTCFG_THRE, ENABLE);
			}
			break;
		default:
				ret = FALSE;
				break;
	}
	return ret;
}


/*********************************************************************//**
 * @brief		串口初始化函数
 * @param[in]	uartnum	串口号
 * @param[in]     Baudrate 波特率
 * @param[in]     fpininit  串口引脚初始化函数指针
 * @return 		TRUE/FALSE
 **********************************************************************/
Bool UartInit(uint8_t UartNum,uint32_t Baudrate,pFnfpininit fpininit)
{
	UartInterface *pUI = NULL; 
	//检测参数
	if (UartNum > 3)
	{
		return FALSE;
	}
	if(fpininit == NULL)
	{
		return FALSE;
	}

	//获得相应串口端号数据结构指针
	pUI = GetpUIbyNum(UartNum);
	if(pUI == NULL)
	{
		return FALSE;
	}
	//初始化串口引脚，与硬件引脚设计有关，根据硬件原理图配置
	fpininit(UartNum);
	//串口进行配置
	UartConfig(UartNum,Baudrate);
	//初始化串口相关数据结构
	pUI->Num = UartNum;
	pUI->Baudrate = Baudrate;
	
	switch(UartNum)
	{
		case 0:
			{
				#ifdef _UART0
				UartSendBufSet(UartNum,Uart0TxBuf,sizeof(Uart0TxBuf));
				UartRecBufSet(UartNum,Uart0RxBuf,sizeof(Uart0RxBuf));
				#endif
			}
			break;
		case 1:
			{
				#ifdef _UART1
				UartSendBufSet(UartNum,Uart1TxBuf,sizeof(Uart1TxBuf));
				UartRecBufSet(UartNum,Uart1RxBuf,sizeof(Uart1RxBuf));
				#endif
			}
			break;
		case 2:
			{
				#ifdef _UART2
				UartSendBufSet(UartNum,Uart2TxBuf,sizeof(Uart2TxBuf));
				UartRecBufSet(UartNum,Uart2RxBuf,sizeof(Uart2RxBuf));
				#endif
			}
			break;
		case 3:
			{
				#ifdef _UART3
				UartSendBufSet(UartNum,Uart3TxBuf,sizeof(Uart3TxBuf));
				UartRecBufSet(UartNum,Uart3RxBuf,sizeof(Uart3RxBuf));
				#endif
			}
			break;
		default:
			break;
	}

	return TRUE;
}


/*********************************************************************//**
 * @brief		串口发送数据缓冲大小
 * @param[in]	UartNum 串口号
 * @param[in]     Buf 串口发送数据缓冲指针
 * @param[in]     BufSize 串口发送数据缓冲大小
 * @return 		TRUE/FALSE
 **********************************************************************/
Bool UartSendBufSet(uint8_t UartNum,uint8_t *Buf,uint16_t BufSize)
{
	#if OS_CRITICAL_METHOD == 3                         
        OS_CPU_SR  cpu_sr;
	#endif
	UartInterface *pUI = NULL; 

	//获得相应串口端号数据结构指针
	pUI = GetpUIbyNum(UartNum);
	if(pUI == NULL)
	{
		return FALSE;
	}
	
	OS_ENTER_CRITICAL();
	pUI->SendBuf = Buf;
	pUI->SendBufSize = BufSize;
	pUI->pRead = 0;
	pUI->pWrite = 0;
	memset(pUI->SendBuf,0,BufSize);
	OS_EXIT_CRITICAL();

	return TRUE;
}

/*********************************************************************//**
 * @brief		串口接收数据缓冲大小
 * @param[in]	UartNum 串口号
 * @param[in]     Buf 串口接收数据缓冲指针
 * @param[in]     BufSize 串口接收数据缓冲大小
 * @return 		TRUE/FALSE
 **********************************************************************/
Bool UartRecBufSet(uint8_t UartNum,uint8_t *Buf,uint16_t BufSize)
{
	#if OS_CRITICAL_METHOD == 3                         
        OS_CPU_SR  cpu_sr;
	#endif
	UartInterface *pUI = NULL; 

	//获得相应串口端号数据结构指针
	pUI = GetpUIbyNum(UartNum);
	if(pUI == NULL)
	{
		return FALSE;
	}
	
	OS_ENTER_CRITICAL();
	pUI->RecBuf = Buf;
	pUI->RecBufSize = BufSize;
	pUI->rpRead = 0;
	pUI->rpWrite = 0;
	memset(pUI->RecBuf,0,BufSize);
	OS_EXIT_CRITICAL();

	return TRUE;
}



