/*************************************************
  Copyright (C), 2009, 712.Co.,Ltd.
  File name:     app.h
  Author:  zhaoli     Version:   v1.0.0.1     Date: 09.08.07
  Description:  crc 校验,bch  编解码处理
  Others:         
  Function List:  
  1. bch  编解码
  History:      
  1. zhaoli   08.12.25    v1.0.0.1  
*************************************************/
#ifndef __BCH_H_
#define __BCH_H_

#include "lpc_types.h"

void Bch_code(uint8_t * srcPtr, uint8_t * dstPtr, uint8_t len);
uint16_t Bch_chkErr(uint8_t * ptr);
uint8_t Bch_crtErr(uint8_t * ptr, uint16_t err);
uint16_t Bch_decode(uint8_t * srcPtr, uint8_t * dstPtr, uint16_t len);

#endif

