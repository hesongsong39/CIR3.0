/**********************************************************************
* $Id$      Led              2015-12-16
*//**
* @file     Led.c
* @brief    灯控制程序
* @version  1.0.0
* @date     2015/12/16 星期三 15:20:15
* @author   ZhangYanMing
*
* Copyright(C) 2015, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
/************************** Includes **********************************/
#include <LPC43xx.h>                    
#include "LED.h"


const uint32_t led_mask[] = { 1UL << 0,1UL << 1,1UL << 2,1UL << 3,1UL << 4,1UL << 5};

/* Clock Control Unit register bits */
#define CCU_CLK_CFG_RUN   (1 << 0)
#define CCU_CLK_CFG_AUTO  (1 << 1)
#define CCU_CLK_STAT_RUN  (1 << 0)

/*********************************************************************//**
 * @brief 			灯相关引脚初始化
 * @param[in] 	无
 * @return			None
 ************************************************************************/
void LED_Init (void)
{

    /* Enable clock and init GPIO outputs */
		LPC_CCU1->CLK_M4_GPIO_CFG  = CCU_CLK_CFG_AUTO | CCU_CLK_CFG_RUN;
		while (!(LPC_CCU1->CLK_M4_GPIO_STAT & CCU_CLK_STAT_RUN));

		LPC_SCU->SFSP4_0   =  0;              /* GPIO2[0]                          */
		LPC_SCU->SFSP4_1   =  0;              /* GPIO2[1]                          */
		LPC_SCU->SFSP4_2   =  0;              /* GPIO2[2]                          */
		LPC_SCU->SFSP4_3   =  0;              /* GPIO2[3]                          */
		LPC_SCU->SFSP4_4   =  0;              /* GPIO2[4]                          */
		LPC_SCU->SFSP4_5   =  0;              /* GPIO2[5]                          */

		LPC_GPIO_PORT->DIR[2] |= (led_mask[0] | led_mask[1] | led_mask[2] | led_mask[3] | led_mask[4] | led_mask[5]);
		
		LPC_GPIO_PORT->SET[2] |= (led_mask[0] | led_mask[1] | led_mask[2] | led_mask[3] | led_mask[4] | led_mask[5]);
		
}



/*********************************************************************//**
 * @brief 			开灯
 * @param[in] 	num，灯编号
 * @return			None
 ************************************************************************/
void LED_On (uint32_t num)
{
	LPC_GPIO_PORT->CLR[2] = led_mask[num];
}

/*********************************************************************//**
 * @brief 			关灯
 * @param[in] 	num，灯编号
 * @return			None
 ************************************************************************/
void LED_Off (uint32_t num)
{
	LPC_GPIO_PORT->SET[2] = led_mask[num];
}


