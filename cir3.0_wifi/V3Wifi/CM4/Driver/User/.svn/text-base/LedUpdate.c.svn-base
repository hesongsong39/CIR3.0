/*-----------------------------------------------------------------------------
 * Name:    LED.c
 * Purpose: Low level LED functions
 * Note(s):
 *-----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2004-2012 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <LPC43xx.h>                    /* LPC43xx Definitions                */
#include "LedUpdate.h"


#define NUM_LEDS     2                  /* Number of user LEDs          */


const uint32_t led_mask[] = { 1UL << 1,
                              1UL << 2
                            };

/* Clock Control Unit register bits */
#define CCU_CLK_CFG_RUN   (1 << 0)
#define CCU_CLK_CFG_AUTO  (1 << 1)
#define CCU_CLK_STAT_RUN  (1 << 0)

/*-----------------------------------------------------------------------------
 *      LED_Init:  Initialize LEDs
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_Init (void)
{

    /* Enable clock and init GPIO outputs */
    LPC_CCU1->CLK_M4_GPIO_CFG  = CCU_CLK_CFG_AUTO | CCU_CLK_CFG_RUN;
    while (!(LPC_CCU1->CLK_M4_GPIO_STAT & CCU_CLK_STAT_RUN));

    LPC_SCU->SFSP8_1   =  0;              /* GPIO4[1]                          */
    LPC_SCU->SFSP8_2   =  0;              /* GPIO4[2]                          */

    LPC_GPIO_PORT->DIR[1] |= (1<<6) + (1<<7);
    LPC_GPIO_PORT->DIR[2] |= (1<<0) + (1<<1) + (1<<3);
    LPC_GPIO_PORT->CLR[4] |= 1<<0;
    LPC_GPIO_PORT->CLR[2] |= (1<<0) + (1<<1) + (1<<3);
    LPC_GPIO_PORT->CLR[1] |= (1<<6) + (1<<7); 	//	保持复位交换机，使其灯常亮。
}


/*-----------------------------------------------------------------------------
 *      LED_UnInit:  Uninitialize LEDs; pins are set to reset state
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_UnInit(void)
{
    LPC_GPIO_PORT->DIR[4] &= ~(led_mask[0] | led_mask[1]);
}

/*-----------------------------------------------------------------------------
 *      LED_On: Turns on requested LED
 *
 * Parameters:  num - LED number
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_On (uint32_t num)
{

    LPC_GPIO_PORT->SET[2] |= 1<<0;
}

void LED_On_ALL (void)
{

    LPC_GPIO_PORT->SET[2] |= (1<<0) + (1<<1) + (1<<3);
}

/*-----------------------------------------------------------------------------
 *       LED_Off: Turns off requested LED
 *
 * Parameters:  num - LED number
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_Off (uint32_t num)
{

    LPC_GPIO_PORT->CLR[2] |=1<<0;
}

void LED_Off_ALL (void)
{

    LPC_GPIO_PORT->CLR[2] |=(1<<0) + (1<<1) + (1<<3);
}
/*-----------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
