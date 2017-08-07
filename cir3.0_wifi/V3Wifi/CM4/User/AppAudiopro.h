/**********************************************************************
* $Id$      AppAudiopro              2016-05-30
*//**
* @file     AppAudiopro.h
* @brief    ��Ƶ��ش�������ͷ�ļ�
* @version  1.0.0
* @date     2016/05/30 ����һ 14:28:44
* @author   ZhangYanMing
*
* Copyright(C) 2015, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
#ifndef _APPAUDIOPRO_H_
#define _APPAUDIOPRO_H_

#include "lpc_types.h"

/************************** Function Interface ************************/
void  AppTaskAudioProc(void *p_arg);
void  Audio_Proc(void);
Bool  Audio_Encode(char *audio,uint8_t audiolen);

#endif



