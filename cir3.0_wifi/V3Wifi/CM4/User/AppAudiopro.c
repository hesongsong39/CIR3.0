/**********************************************************************
* $Id$      AppAudiopro              2016-05-30
*//**
* @file     AppAudiopro.c
* @brief    音频相关处理任务
* @version  1.0.0
* @date     2016/05/30 星期一 14:28:44
* @author   ZhangYanMing
*
* Copyright(C) 2015, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
#include <string.h>
#include <ucos_ii.h>
#include "Task.h"
#include "AppAudiopro.h"
#include "audio.h"
#include "G723.h"
#include "DataTransfer.h"
#include "AppMsgDispatchpro.h"
#include "App.h"
#include "AppDefine.h"
/*****************Private variables ***********************************/

/*****************Public variables ************************************/


/*****************Extern variables ************************************/

/*********************************************************************//**
 * @brief			AppTaskAudioProc
 * @param[in]	p_arg   
 * @return 		None
 **********************************************************************/
void  AppTaskAudioProc(void *p_arg)
{
		G723_Init();
		while(1)
		{
				Audio_Proc();
				OSTimeDlyHMSM(0,0,0,10);
		}
}



/*********************************************************************//**
 * @brief			音频数据队列的处理函数
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void Audio_Proc(void)
{
		uint8_t  audiotxbuf[48] = {0};
		uint32_t temp_offset = 0;
		int16_t AudioInBuf[AUDIO_BUFFER_SIZE] = {0};
		int16_t AudioOutBuf[AUDIO_BUFFER_SIZE] = {0};
		char AudioEnOutBuf[24] = {0};
		
		memset(AudioInBuf,0,sizeof(AudioInBuf));
		memset(AudioOutBuf,0,sizeof(AudioOutBuf));
		memset(AudioEnOutBuf,0,sizeof(AudioEnOutBuf));
		
		if(Audio_Q_GetSize(&g_AQRX))
		{
				//数据接收队列有数据
				Audio_Q_RemoveTail(&g_AQRX,AudioInBuf,AUDIO_BUFFER_SIZE);
				//G723算法数据压缩
				Coder(AudioInBuf, AudioEnOutBuf);
				//对压缩完成的数据组包发送
				memset(audiotxbuf,0,sizeof(audiotxbuf));
				CCANDataSimpleSendPost(audiotxbuf,sizeof(audiotxbuf),g_ParaInfo.MasterCanBus,OP_CMUNICN712_FFSKTT_STORAGE,CMD_CMUNICN712_VOICEFRAME,g_ParaInfo.SelfPort,g_ParaInfo.MMIPort,(uint8_t *)AudioEnOutBuf,sizeof(AudioEnOutBuf),FALSE);
		}
		else
		{
				//判断I2S接收中断状态，防止在打开情况下，接收到数据不满一帧240个半字错进G.723压缩算法
				if(Audio_StatusQuery(RX) == FALSE)
				{
						//处理收到的不满一包的数据(<240)
						temp_offset = Audio_GetRXLength();
						if(temp_offset > 0)
						{
								memset(AudioInBuf,0,sizeof(AudioInBuf));
								Audio_Q_GetValue(&g_AQRX,AudioInBuf,temp_offset);
								Audio_GetRXSet();
								Coder(AudioInBuf, AudioEnOutBuf);
								//对压缩完成的数据组包发送
								memset(audiotxbuf,0,sizeof(audiotxbuf));
								CCANDataSimpleSendPost(audiotxbuf,sizeof(audiotxbuf),g_ParaInfo.MasterCanBus,OP_CMUNICN712_FFSKTT_STORAGE,CMD_CMUNICN712_VOICEFRAME,g_ParaInfo.SelfPort,g_ParaInfo.MMIPort,(uint8_t *)AudioEnOutBuf,sizeof(AudioEnOutBuf),FALSE);
						}
				}
		}
		
		if(Audio_Q_GetSize(&g_AQTX))
		{		
			 Audio_Setup(TX);		
		}
}



/*********************************************************************//**
 * @brief			音频数据队列的处理函数
 * @param[in]	audio,语音数据缓冲指针
 * @param[in]	audiolen,语音数据长度
 * @return 		FALSE,数据发送队列没有空间
							TRUE,成功将语音数据添加到发送队列
 **********************************************************************/
Bool Audio_Encode(char *audio,uint8_t audiolen)
{
		int16_t AudioOutBuf[AUDIO_BUFFER_SIZE] = {0};

		if(audiolen != 24)
		{
				return FALSE;
		}
		memset(AudioOutBuf,0,sizeof(AudioOutBuf));
		//G.723解压缩语音数据
		Decod(AudioOutBuf,audio,0);
		//将解压缩过的语音数据添加到发送队列
	  Audio_Q_AddTail(&g_AQTX, AudioOutBuf, AUDIO_BUFFER_SIZE);
		
		return TRUE;
}




