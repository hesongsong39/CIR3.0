/**********************************************************************
* $Id$      Target              2015-12-18
*//**
* @file     Target.c
* @brief    目标板配置相关操作
* @version  1.0.0
* @date     2015/12/18 星期五 11:20:15
* @author   ZhangYanMing
*
* Copyright(C) 2015, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
/************************** Includes **********************************/
#include "lpc_types.h"
#include "lpc43xx_i2c.h"
#include "Target.h"
#include "Uart.h"
#include "LED.h"
#include "GPIO.h"
#include "C_CAN.h"
#include "CanRxpro.h"
#include "UartRxpro.h"
#include "SPIFLASH.h"
#include "MX66L51235F.h"
#include "lpc43xx_i2c.h"
#include "audio.h"

extern SPI_INFODATA_T g_spiinfo;

#define  BUFSIZE    8192
#define  LOOPCNT    1

static uint8_t TestVal[BUFSIZE] = {0};
Bool SPI_Check(void)
{
		Bool ret = TRUE;
		uint32_t  i = 0,j = 0;
		uint32_t  ChipID = 0;
		for(i=0;i<BUFSIZE;i++)
		{
				TestVal[i] = (uint8_t)i ;
		}
		SSP_FLASH_RdID(LPC_SSP0);
		for(j=0;j<LOOPCNT;j++)
		{
				SSP_Write(LPC_SSP0,LOOPCNT * BUFSIZE,(uint32_t *)TestVal,BUFSIZE);
		}

		for(j=0;j<LOOPCNT;j++)
		{
				memset(TestVal,0x00,BUFSIZE);
				SSP_Read(LPC_SSP0,LOOPCNT * BUFSIZE,(uint32_t *)TestVal,BUFSIZE);
				for(i=0;i<BUFSIZE;i++)
				{
						if(TestVal[i] != (uint8_t)i)
						{
								ret = FALSE;
								break;
						}
				}
		}
		
		return ret;
}

//void EraseFlash()
//{
// SSP_EraseSectors(LPC_SSP0,&g_spiinfo,0x2000000/4096, 100);
//}
/*********************************************************************//**
 * @brief 			目标板硬件初始化
 * @param[in] 	无
 * @return			None
 ************************************************************************/
void Target_INIT(void)
{
		//显示灯程序初始化
		LED_Init();
		//I2C初始化
		I2C_Init(LPC_I2C0, 100000);
		I2C_Cmd(LPC_I2C0, ENABLE);/* 目标板初始化 */
		//I/O初始化
		HWVer_GPIO_Init();
		Location_GPIO_Init();
//		GSMR_GPIO_Init();
		Net_Pin_Init();
		//串口初始化
		UartInit(0,115200,Uart_Pin_Init);
		UartInit(1,115200,Uart_Pin_Init);
//		UartInit(3,115200,Uart_Pin_Init);
		//CAN初始化
		CANInit(0,500000,CAN_Pin_Init);
		CANInit(1,500000,CAN_Pin_Init);
		//SPIFI初始化SPIFLASH
		SPIFI_Init();
		//SSP初始化SPIFLASH	
		SPI_FLASH_Init(LPC_SSP0,SSP0_Pin_Init,SSP0_Reset_Pin_Init);
		//Audio相关初始化
//		Audio_INIT(I2S_Pin_Init,Codec_Reset_Pin_Init);
		SPI_Check();
//		EraseFlash();
}






