/**********************************************************************
* $Id$      Task              2015-12-16
*//**
* @file     TaskStartPrc.c
* @brief    启动任务相关操作
* @version  1.0.0
* @date     2015/12/16 星期三 15:25:15
* @author   ZhangYanMing
*
* Copyright(C) 2015, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
/************************** Includes **********************************/
#include "lpc_types.h"
#include <ucos_ii.h>
#include "lpc43xx_cgu.h"
#include "TaskStartPrc.h"
#include "Task.h"
#include "Target.h"
#include "App.h"
#include "LedControl.h"
#include "CanBusControl.h"
#include "AppDefine.h"
#include "EthernetUdp.h"
#include "update.h"

/*****************Private variables ***********************************/


/*****************Public variables ************************************/


/*****************Extern variables ************************************/
extern uint32_t SystemCoreClock;    /* System Clock Frequency (Core Clock) */

/*********************************************************************//**
 * @brief			TaskStartPrc
 * @param[in]	p_arg   is the argument passed to 'TaskStartPrc()' by 'OSTaskCreate()'
 * @return 		None
 **********************************************************************/
void  TaskStartPrc(void *p_arg)
{
    create_os_queue(); /* 创建应用中的大多数消息队列 	*/
    create_os_mem();   /* 创建应用中的大多数内存分区 	*/
    create_os_eventflag(); /* 创建应用中的通用事件组*/
    create_os_tmr();	 /* 创建应用中的定时器*/
    create_os_task();  /* 创建应用中的任务*/
    create_os_sem();	 /* 创建应用中的信号量*/

    CGU_UpdateClock();
    OS_CPU_SysTickInit(SystemCoreClock/1000); /* 初始化系统定时器 */

    Target_INIT();     /* 目标板硬件初始化 */
    App_Init();				 /* 应用层初始化 */


#if (OS_TASK_STAT_EN > 0)
    OSStatInit();                                               /* Determine CPU capacity                               */
#endif
    while (1)
    {
#ifdef WIFITEST
        char str[]="a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\nThis is a test\n";
        char IP[]= {192,168,2,64};
        static int64_t cnt_test=0;
        if((g_UdpSendInfo.socket == 0)||(g_UdpSendInfo.pfuncSend == NULL))
        {
            OSTimeDlyHMSM(0,0,0,25);
        }
        else
        {
            if(cnt_test < 10000)
            {
                UDP_DataSend(IP,20001,str,strlen(str));
                cnt_test ++;
                OSTimeDlyHMSM(0,0,0,2);
            }
            else
            {
                OSTimeDlyHMSM(0,0,0,25);
                //面板显示灯控制
                BoardLightCtr_Handle();
                //CAN总线控制
                CANbusControlPeriodHandle(g_ParaInfo.SelfPort);
            }
        }
#else
        static uint16_t s30;
        OSTimeDlyHMSM(0,0,0,25);
        //面板显示灯控制
        BoardLightCtr_Handle();
        //CAN总线控制
        CANbusControlPeriodHandle(g_ParaInfo.SelfPort);
        s30++;
        if(s30 > 120)
        {
//			                Debug_Update("\nDebug test");
            s30 = 0;
            Debug_Output_CAN(g_ParaInfo.MasterCanBus,PORT_WIFI,"无线宽带单元 已运行:%d时%d分%d秒\n",OSTimeGet()/1000/60/60,OSTimeGet()/1000/60%60,OSTimeGet()/1000%60);
			//Debug_Output_Ethernet(PORT_WIFI,"\nIf can see this ,sign update success");
		}
#endif
    }
}








