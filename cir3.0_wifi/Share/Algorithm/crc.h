/*************************************************
  Copyright (C), 2009, 712.Co.,Ltd.
  File name:     app.h
  Author:  zhaoli     Version:   v1.0.0.1     Date: 09.08.07
  Description:  crc 校验,bch  编解码处理
  Others:         
  Function List:  
  1. crc  校验
  2. bch  编解码
  History:      
  1. zhaoli   08.12.25    v1.0.0.1  
*************************************************/
#ifndef __CRC_H
#define __CRC_H

#ifdef __cplusplus
extern "C" {
#endif

unsigned short CalCRC16(const unsigned char *ptr, unsigned short wlen);
int CheckCalCRC16(const unsigned char *ptr,unsigned short wlen);
unsigned char CalCRC8(unsigned char *ptr,unsigned char len);
int CheckCalCRC8(unsigned char *ptr,unsigned char len);

#ifdef __cplusplus
}
#endif

#endif

