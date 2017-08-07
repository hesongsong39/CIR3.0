/**********************************************************************
* $Id$      GPIO              2016-03-21
*//**
* @file     GPIO.h
* @brief    目标板通用I/O初始化头文件
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


/************************** Function Interface ************************/
void Uart_Pin_Init(uint8_t UartNum);
void CAN_Pin_Init(uint8_t CanNum);
void HWVer_GPIO_Init(void);
void Location_GPIO_Init(void);
void SSP0_Reset_Pin_Init(void);
void SSP0_Pin_Init(void);
void I2S_Pin_Init(void);
void Codec_Reset_Pin_Init(void);
void GSMR_GPIO_Init(void);
void Net_Pin_Init(void);


