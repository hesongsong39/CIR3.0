/**********************************************************************
* $Id$      main              2015-12-16
*//**
* @file     main.c
* @brief    main函数入口
* @version  1.0.0
* @date     2015/12/16 星期三 15:20:15
* @author   ZhangYanMing
*
* Copyright(C) 2015, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/

/************************** Includes **********************************/
#include <ucos_ii.h>
#include <cpu.h>
#include "Task.h"
#include "TaskStartPrc.h"


/*********************************************************************//**
 * @brief			The standard entry point for C code.  It is assumed that
 *            your code will call main() once you have performed all
 *            necessary initialization.
 *
 * @param[in]	None
 * @return 		None
 **********************************************************************/
int  main (void)
{
    CPU_IntDis();                                               /* Initialize the interrupts                            */
 
    OSInit();                                                   /* Initialize "uC/OS-II, The Real-Time Kernel"          */
    OSTaskCreateExt((void (*)(void *)) TaskStartPrc,            /* Create the start task                                */
                    (void           *) 0,
                    (OS_STK         *)&AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE - 1],
                    (INT8U           ) APP_CFG_TASK_START_PRIO,
                    (INT16U          ) APP_CFG_TASK_START_PRIO,
                    (OS_STK         *)&AppTaskStartStk[0],
                    (INT32U          ) APP_CFG_TASK_START_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

    OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II)   */

    return (1);
}






