/**********************************************************************
* $Id$      GPIO              2016-03-21
*//**
* @file     GPIO.c
* @brief    目标板通用I/O初始化
* @version  1.0.0
* @date     2016-03-21  星期一11:20:15
* @author   ZhangYanMing
*
* Copyright(C) 2016, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
/************************** Includes **********************************/
#include "lpc_types.h"
#include "GPIO.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_gpio.h"
#include "lpc43xx.h"


/*----------------------------------------------------------------------------
  Approximate delay function (must be used after SystemCoreClockUpdate() call)
 *----------------------------------------------------------------------------*/
#define CPU_NANOSEC(x) (((uint64_t)(x) * SystemCoreClock)/1000000000)

static void WaitUs (uint32_t us) {
  uint32_t cyc = us * CPU_NANOSEC(1000)/4;
  while(cyc--);
}

/********************************************************************//**
 * @brief 		串口引脚初始化
 * @param[in]	UartNum，串口物理端口号
 * @return 		None
 *********************************************************************/
void Uart_Pin_Init(uint8_t UartNum)
{
	switch(UartNum)
	{
		case 0:
			{
				scu_pinmux(0x2,0, UART_RX_TX, FUNC1); 		
				scu_pinmux(0x2,1, UART_RX_TX, FUNC1); 		
			}
			break;
		case 1:
			{
				scu_pinmux(0x5,6, UART_RX_TX, FUNC4); 		
				scu_pinmux(0x5,7, UART_RX_TX, FUNC4);
			}
			break;
		case 2:
			{
					
			}
			break;
		case 3:
			{
				scu_pinmux(0x2,3, UART_RX_TX, FUNC2);		
				scu_pinmux(0x2,4, UART_RX_TX, FUNC2);		
			}
			break;
		default:
			break;
	}
}



/********************************************************************//**
 * @brief 		CAN口引脚初始化
 * @param[in]	CanNum，CAN口物理端口号
 * @return 		None
 *********************************************************************/
void CAN_Pin_Init(uint8_t CanNum)
{
	switch(CanNum)
	{
		case 0:
			{
				scu_pinmux(0x3, 2, MD_PLN, FUNC2);				
				scu_pinmux(0x3, 1, MD_PLN | MD_EZI, FUNC2);		
			}
			break;
		case 1:
			{
				scu_pinmux(0x4,8,MD_PLN,FUNC6);		
				scu_pinmux(0x4,9,MD_PLN | MD_EZI ,FUNC6);
			}
			break;
		default:
			break;
	}
}


/********************************************************************//**
 * @brief 		网口引脚初始化
 * @param[in]	None
 * @return 		None
 *********************************************************************/
void Net_Pin_Init(void)
{
  /* Ethernet pins configuration */
  LPC_SCU->SFSP0_0  = (1 << 6) | (1 << 5) | 0x2; /* P0.0  = ENET_RXD1         */
  LPC_SCU->SFSP0_1  = (1 << 6) | (1 << 5) | 0x6; /* P0.1  = ENET_TX_EN        */

  LPC_SCU->SFSP1_15 = (1 << 6) | (1 << 5) | 0x3; /* P1.15 = ENET_RXD0         */
  LPC_SCU->SFSP1_16 = (1 << 6) | (1 << 5) | 0x7; /* P1.16 = ENET_RX_DV        */
  LPC_SCU->SFSP1_17 = (1 << 6) |            0x3; /* P1.17 = ENET_MDIO         */
  LPC_SCU->SFSP1_18 = (1 << 6) | (1 << 5) | 0x3; /* P1.18 = ENET_TXD0         */
  LPC_SCU->SFSP1_19 = (1 << 6) | (1 << 5) | 0x0; /* CLK0 = ENET_REF_CLK       */
  LPC_SCU->SFSP1_20 = (1 << 6) | (1 << 5) | 0x3; /* P1.20 = ENET_TXD1         */

  LPC_SCU->SFSP7_7  = (1 << 6) | (1 << 5) | 0x6; /* PC.1  = ENET_MDC          */

#ifdef _MII_
  LPC_CREG->CREG6 &= ~0x7;
  
  LPC_SCU->SFSP0_1 = (1 << 6) | (1 << 5) | 0x2;  /* P0.1  = ENET_COL          */
  
  LPC_SCU->SFSPC_0 = (1 << 6) | (1 << 5) | 0x0;  /* PC.0  = ENET_RX_CLK       */
  LPC_SCU->SFSPC_2 = (1 << 6) | (1 << 5) | 0x3;  /* PC.2  = ENET_TXD2         */
  LPC_SCU->SFSPC_3 = (1 << 6) | (1 << 5) | 0x3;  /* PC.3  = ENET_TXD3         */
  LPC_SCU->SFSPC_5 = (1 << 6) | (1 << 5) | 0x3;  /* PC.5  = ENET_TX_ER        */
  LPC_SCU->SFSPC_6 = (1 << 6) | (1 << 5) | 0x3;  /* PC.6  = ENET_RXD2         */
  LPC_SCU->SFSPC_7 = (1 << 6) | (1 << 5) | 0x3;  /* PC.7  = ENET_RXD3         */
  LPC_SCU->SFSPC_8 = (1 << 6) | (1 << 5) | 0x3;  /* PC.8  = ENET_RX_DV        */
#endif
}



/********************************************************************//**
 * @brief 		硬件版本号引脚初始化
 * @param[in]	None
 * @return 		None
 *********************************************************************/
void HWVer_GPIO_Init(void)
{
	//HW_VER_0
	scu_pinmux(0x6, 9, GPIO_PUP,FUNC0);		
	GPIO_SetDir(3, 1<<5, 0);
	//HW_VER_1
	scu_pinmux(0x6, 10, GPIO_PUP,FUNC0);		
	GPIO_SetDir(3, 1<<6, 0);
	//HW_VER_2
	scu_pinmux(0x6, 11, GPIO_PUP,FUNC0);		
	GPIO_SetDir(3, 1<<7, 0);
	//HW_VER_3
	scu_pinmux(0x6, 12, GPIO_PUP,FUNC0);		
	GPIO_SetDir(2, 1<<8, 0);
}

/********************************************************************//**
 * @brief 		位置判断引脚初始化
 * @param[in]	None
 * @return 		None
 *********************************************************************/
void Location_GPIO_Init(void)
{
	//ADD0
	scu_pinmux(0x6, 6, GPIO_PUP,FUNC0);		
	GPIO_SetDir(0, 1<<5, 0);
	//ADD1
	scu_pinmux(0x6, 7, GPIO_PUP,FUNC4);		
	GPIO_SetDir(5	, 1<<15, 0);
	//ADD2
	scu_pinmux(0x6, 8, GPIO_PUP,FUNC4);		
	GPIO_SetDir(5, 1<<16, 0);
	//ADD3
	scu_pinmux(0x6, 5, GPIO_PUP,FUNC0);		
	GPIO_SetDir(3, 1<<4, 0);
}



/********************************************************************//**
 * @brief 		SSP0复位引脚初始化
 * @param[in]	None
 * @return 		None
 *********************************************************************/
void SSP0_Reset_Pin_Init(void)
{
	uint32_t i = 0,j = 0;
	/* Reset pin */
	scu_pinmux(0x7,4,MD_PLN_FAST,FUNC0);	
	GPIO_SetDir(0x3,1<<12,1);
	GPIO_ClearValue(0x3,1<<12);
	for (i = 0; i < 10000; i++)
	{
		for (j = 0; j < 1000; j++)
			__NOP();
	}
	GPIO_SetValue(0x3,1<<12);
}


/********************************************************************//**
 * @brief 		SSP0口引脚初始化
 * @param[in]	None
 * @return 		None
 *********************************************************************/
void SSP0_Pin_Init(void)
{
	/* Configure SSP0 pins*/
	scu_pinmux(0x3,0,MD_PLN_FAST,FUNC4);	//func6=SSP1 SCK1
	scu_pinmux(0x1,0,MD_PLN_FAST,FUNC0);	//func4=GPIO0[4]
 	scu_pinmux(0x1,1,MD_PLN_FAST,FUNC5);	//func5=SSP1 MISO1
 	scu_pinmux(0x1,2,MD_PLN_FAST,FUNC5);	//func5=SSP1 MOSI1	
	
	GPIO_SetDir(0x0,1<<4,1);	
}



/********************************************************************//**
 * @brief 	  I2S引脚初始化
 * @param[in]	None
 * @return 		None
 *********************************************************************/
void I2S_Pin_Init(void)
{
	/* Pin configuration: from LPC43xx PinMux ver1 20120727.jar
	 * Assign:  - P6.2 as I2SRX_SDA
	 *					- PF.4 as I2STX_MCLK
	 *          - P6.0 as I2STX_CLK
	 *          - P6.1 as I2STX_WS
	 *          - P7.2 as I2STX_SDA
	 */
	scu_pinmux(6,2, MD_PLN_FAST, 3);
	scu_pinmux(6,0, MD_PLN_FAST, 4);
	scu_pinmux(0xF,4, MD_PLN_FAST, 6);
	scu_pinmux(6,1, MD_PLN_FAST, 3);
	scu_pinmux(7,2, MD_PLN_FAST, 2);
}


/********************************************************************//**
 * @brief 		codec复位引脚初始化
 * @param[in]	None
 * @return 		None
 *********************************************************************/
void Codec_Reset_Pin_Init(void)
{
	uint32_t i = 0,j = 0;
	/* Reset pin */
	scu_pinmux(0x5,3,MD_PLN_FAST,FUNC0);	
	GPIO_SetDir(0x2,1<<12,1);
	GPIO_ClearValue(0x2,1<<12);
	for (i = 0; i < 10000; i++)
	{
		for (j = 0; j < 1000; j++)
			__NOP();
	}
	GPIO_SetValue(0x2,1<<12);
}



/********************************************************************//**
 * @brief 		GSMR模块相关引脚初始化
 * @param[in]	None
 * @return 		None
 *********************************************************************/
void GSMR_GPIO_Init(void)
{
	//POW_EN1
	WaitUs(500000);
	scu_pinmux(0x2, 5, GPIO_PUP,FUNC4);		
	GPIO_SetDir(5, 1<<5, 1);
	GPIO_SetValue(0x5,1<<5);
	//POW_EN2
	WaitUs(500000);
	scu_pinmux(0x2, 6, GPIO_PUP,FUNC4);		
	GPIO_SetDir(5	, 1<<6, 1);
	GPIO_SetValue(0x5,1<<6);
}

