/**********************************************************************
* $Id$      AppAudiopro              2016-05-30
*//**
* @file     AppAudiopro.c
* @brief    ��Ƶ��ش�������
* @version  1.0.0
* @date     2016/05/30 ����һ 14:28:44
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
 * @brief			��Ƶ���ݶ��еĴ�����
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
				//���ݽ��ն���������
				Audio_Q_RemoveTail(&g_AQRX,AudioInBuf,AUDIO_BUFFER_SIZE);
				//G723�㷨����ѹ��
				Coder(AudioInBuf, AudioEnOutBuf);
				//��ѹ����ɵ������������
				memset(audiotxbuf,0,sizeof(audiotxbuf));
				CCANDataSimpleSendPost(audiotxbuf,sizeof(audiotxbuf),g_ParaInfo.MasterCanBus,OP_CMUNICN712_FFSKTT_STORAGE,CMD_CMUNICN712_VOICEFRAME,g_ParaInfo.SelfPort,g_ParaInfo.MMIPort,(uint8_t *)AudioEnOutBuf,sizeof(AudioEnOutBuf),FALSE);
		}
		else
		{
				//�ж�I2S�����ж�״̬����ֹ�ڴ�����£����յ����ݲ���һ֡240�����ִ��G.723ѹ���㷨
				if(Audio_StatusQuery(RX) == FALSE)
				{
						//�����յ��Ĳ���һ��������(<240)
						temp_offset = Audio_GetRXLength();
						if(temp_offset > 0)
						{
								memset(AudioInBuf,0,sizeof(AudioInBuf));
								Audio_Q_GetValue(&g_AQRX,AudioInBuf,temp_offset);
								Audio_GetRXSet();
								Coder(AudioInBuf, AudioEnOutBuf);
								//��ѹ����ɵ������������
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
 * @brief			��Ƶ���ݶ��еĴ�����
 * @param[in]	audio,�������ݻ���ָ��
 * @param[in]	audiolen,�������ݳ���
 * @return 		FALSE,���ݷ��Ͷ���û�пռ�
							TRUE,�ɹ�������������ӵ����Ͷ���
 **********************************************************************/
Bool Audio_Encode(char *audio,uint8_t audiolen)
{
		int16_t AudioOutBuf[AUDIO_BUFFER_SIZE] = {0};

		if(audiolen != 24)
		{
				return FALSE;
		}
		memset(AudioOutBuf,0,sizeof(AudioOutBuf));
		//G.723��ѹ����������
		Decod(AudioOutBuf,audio,0);
		//����ѹ����������������ӵ����Ͷ���
	  Audio_Q_AddTail(&g_AQTX, AudioOutBuf, AUDIO_BUFFER_SIZE);
		
		return TRUE;
}




