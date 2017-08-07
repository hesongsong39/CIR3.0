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

#include "converse.h"
#include "string.h"
#include "stdio.h"

INT_32 AsciiToBCD(const CHAR *pszAscii, INT_32 iLen, CHAR *pszBCD)
{
	UNS_8 chtemp;
	
	int i;
	
	for(i = 0; i < iLen; i++)
	{
		chtemp = (UNS_8)pszAscii[i];
		
		if(chtemp < '0' && chtemp>'9')
		{
			chtemp=0xFF; //非法码
		}

		if( (i % 2) == 0 )
		{
			pszBCD[i/2]  = ( chtemp & 0x0F ) << 4;
			pszBCD[i/2] |= 0x0F; //将低4位置1
		}
		else
		{
			pszBCD[i/2] &= 0xF0; //将低4位置0
			pszBCD[i/2] |= ( chtemp & 0x0F );
		}
	}
	
	iLen = (iLen + 1) / 2;
	
	return iLen;
}

Bool AsciiToHexChar(const CHAR *pszAscii, INT_32 iLen, CHAR *pszCharHex)
{	
    int i;
    
    char szTemp[12];
    
	int iAsciiLen = iLen;
    
    if(pszAscii == NULL || pszCharHex == NULL || iLen == 0)
    {
       return FALSE;
    } 
    
	for (i = 0; i < iAsciiLen; i++)
	{
		sprintf(szTemp, "%0.2x", pszAscii[i]);
        
		strcat(pszCharHex, szTemp);
	}

    return TRUE;
}

Bool HexCharToAscii(const CHAR *pszCharHex, CHAR *pszAscii)
{
    int i, iAsciiLen;
    
	UNS_8 ch1, ch2;
    
    char szTemp[12];
    
    if(pszAscii == NULL || pszCharHex == NULL || strlen(pszCharHex) == 0)
    {
       return FALSE;
    } 

	if(0 != strlen(pszCharHex) % 2)
    {
		return FALSE;
    }
    
	iAsciiLen = strlen(pszCharHex) / 2;

	for(i = 0; i < iAsciiLen; i++)
	{
		ch1 = *pszCharHex;
	    ch2 = *(pszCharHex + 1);
        
        ch1-= '0';
        
        if(ch1 > 9)
		{ 
			ch1 -= 7;
            
            if(ch1 > 16)
            {
				ch1 -= 32;
            }
		} 
        
		ch2 -= '0';
        
        if(ch2 > 9)
		{ 
			ch2 -= 7;
            
            if(ch2 > 16)
            {
				ch2 -= 32;
            }
		} 
        
		pszAscii[i] = ch2 + ch1 * 16;
        
		pszCharHex = pszCharHex + 2;
	}
    
	pszAscii[i] = 0;
    
	return TRUE;
}

INT_32 BCDToBin(UNS_8 bBcd)
{
	UNS_8 bH, bL;
    
	bH = bBcd >> 4;
	bL = bBcd & 0x0F;
	
	if(bH == 0x0F)
    {
		bH = 0;
    }
    
	if(bL == 0x0F)
    {
		bL = 0;
	}
    
	return bH*10 + bL;
}

//255以下
void BinToBCD(INT_32 iBin, UNS_8 *pszBCD)
{
	UNS_8 szTemp[2];

	memset(szTemp, 0, sizeof(szTemp));

	sprintf((CHAR *)szTemp, "%02d", iBin);

	AsciiToBCD((CHAR *)szTemp, 2, (CHAR *)pszBCD);
}


/* *************************************************************************************
** Function: HexWordtoBCD
** Inputs:   hex char
** Returns:  BCD hex byte
** Description: change hex char to BCD  byte
** ************************************************************************************/
uint8_t HextoBCD(uint8_t iHex)
{
	uint8_t bBCD = 0;
    
	bBCD  = iHex % 10;
	bBCD |= ((iHex % 100) / 10) << 4;
	bBCD &= 0xff;
    
	return bBCD;
}


/***********************************************************************
功能：BCD  转 HEX码

输入：int Dec                       待转换的十进制数据
                int length                    BCD码数据长度

输出：unsigned char *Bcd            转换后的BCD码
返回：0   success
***********************************************************************/
int BCDtoHEX(unsigned char *pszBCD, unsigned char *pszCharHex, int iLen)
{
	int i;

	for(i = 0; i < iLen; i++)
	{
		*(pszCharHex + i) = ((*(pszBCD + i) >> 4) & 0x0f) * 10 + ((*(pszBCD + i) & 0x0f));
	}

	return 0;
}

/***********************************************************************
功能：BCD  转 Acsii码

输入：uint8_t *Bcd_buff    待转换的BCD数据
      uint8_t len          ASICC数据长度

输出：char * ascii_buff    转换后的Acsii码
***********************************************************************/
void BCDToAcsii(UNS_8 *pszBCD, CHAR * pszAscii, UNS_8 iLen)
{	
    UNS_8 i;
    
	CHAR  ch;
	
	for(i = 0; i < iLen; i++)
	{
		if ( i & 1)
        {
			ch = *(pszBCD++) & 0x0F;
        }
		else
        {
			ch = ( (*pszBCD) >> 4) & 0x0F;
        }
		
		pszAscii[i] = ch + '0';
	}
}


/***************************????*********************
功能：BCD  转 Acsii码

输入：uint8_t *Bcd_buff    待转换的BCD数据
      uint8_t len           BCD数据长度

输出：char * ascii_buff    转换后的Acsii码
*************************************************************/
char* BinToAcsii(uint8_t iBin, char *pszAcsii)
{
   char *pszTmp = pszAcsii;
    
	uint8_t i = 0;
    
	uint8_t len;

	if (!pszTmp)
	{
		return 0;
	}
	
	do
	{
		int mode = iBin%10;
        
		iBin/=10;
        
		*pszTmp++ = mode + 0x30;		
	}
	while(iBin);
	
	*pszTmp = 0;
	
	len = pszTmp - pszAcsii;
	
	for (i=0; i<len/2; i++)
	{
		char ch = pszAcsii[i];
        
		pszAcsii[i] = pszAcsii[len-i-1];
        
		pszAcsii[len-i-1]=ch;
	}
	
	return pszAcsii;
	
}

