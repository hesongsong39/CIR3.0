/************************************************************
  Copyright (C), 2009, 712.Co.,Ltd.
  FileName: converse.c
  Author:       
  Description: 主要是一些进制转换
  Version:       V1.0.01  
  Function List:   
 
  History:        
  <author>  <time>        <version >   <desc>
  frank      16.06.14      1.0.01       初稿 
***********************************************************/

#ifndef __CONVERSE_H
#define __VONVERSE_H

#include "lpc_types.h"

extern INT_32 AsciiToBCD(const CHAR *pszAscii, INT_32 iLen, CHAR *pszBCD);
extern void BCDToAcsii(UNS_8 *pszBCD, CHAR * pszAscii, UNS_8 iLen);

extern Bool AsciiToHexChar(const CHAR *pszAscii, INT_32 ilen, CHAR *pszCharHex);
extern Bool HexCharToAscii(const CHAR *pszCharHex, CHAR *pszAscii);

extern INT_32 BCDToBin(UNS_8 bBcd);
extern void BinToBCD(INT_32 iBin, UNS_8 *pszBCD);//255以下

extern INT_32 BCDtoHEX(unsigned char *pszBCD, unsigned char *pszCharHex, int iLen);

extern char* BinToAcsii(uint8_t iBin, char *pszAcsii);

extern uint8_t HextoBCD(uint8_t iHex);

#endif

