/*************************************************
  Copyright (C), 2016, 712.Co.,Ltd.
  File name:     CmdDefine.h
  Author:        Frank 
	Version:       v1.0 
	Date:          16.03.16
  Description:   全局标识定义
  Others:         
  Function List:  
  

  History:      
  1. 初版                   16.03.16    v1.0
  2. 调整业务类型和命令      16.12.06    v1.1
	
*************************************************/

#ifndef __CMDFUNCTION_H
#define __CMDFUNCTION_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "lpc_types.h"

typedef  uint8_t    BYTE;
typedef  uint16_t   WORD;
typedef  uint32_t   DWORD;

#define MAKEWORD(a, b)		((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
#define MAKEDWORD(a, b)		((DWORD)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))

#define LOWORD(l)			((WORD)(l))
#define HIWORD(l)			((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define LOBYTE(w)			((BYTE)(w))
#define HIBYTE(w)			((BYTE)(((WORD)(w) >> 8) & 0xFF))

//车次号转换（从4+3Tax箱格式转换为字符串）
Bool GetTnByTaxFormat(CHAR *szTn, UNS_8 *bTaxFormatTn);
//车次号转换（从字符串转换为）4+3Tax箱格式
Bool GetTaxFormatByTn(UNS_8 *bTaxFormatTn, CHAR *szTn);
//车次号合法性判断
Bool IsInvalidTn(CHAR *szTn, int iTnLen, int *iChartCnt);

#endif

