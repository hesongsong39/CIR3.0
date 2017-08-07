/************************************************************
  Copyright (C), 2016, 712.Co.,Ltd.
  FileName: crc.c
  Author: zhaohualei      
  Description: 
  Version:       V1.0.0.1  
  Function List:   
  
  History:        
  <author>  <time>   <version >   <desc>
      
***********************************************************/
#include "cmdfunction.h"

//车次号转换（从4+3Tax箱格式转换为字符串）
Bool GetTnByTaxFormat(CHAR *szTn, UNS_8 *bTaxFormatTn)
{
    int i = 0, j = 0, iTnNum = 0;
    
    CHAR szTnNum[16];
    
    memset(szTnNum, 0, sizeof(szTnNum));
    
    if(szTn == NULL || bTaxFormatTn == NULL)
    {
        return FALSE;
    }
    
   for(i = 0, j = 0; i < 4 && j < 4; i++)
    {
        if(47 < *(bTaxFormatTn+i) && *(bTaxFormatTn+i) < 58)
        {
            szTn[j++] = *(bTaxFormatTn+i);
        }
        else if(64 < *(bTaxFormatTn+i) && *(bTaxFormatTn+i) < 91)
        {
            szTn[j++] = *(bTaxFormatTn+i);                
        }
        else if(96 < *(bTaxFormatTn+i) && *(bTaxFormatTn+i) < 123)
        {
            szTn[j++] = *(bTaxFormatTn+i);                
        }
        else
        {				
            continue ;
        }
    }

    iTnNum = (*(bTaxFormatTn+6))*0x10000 + (*(bTaxFormatTn+5))*0x100 + (*(bTaxFormatTn+4));
    
    if(iTnNum != 0xffffff)
    {
        sprintf((CHAR *)szTnNum, "%d", iTnNum);
    }
    else
    {
        memset(szTnNum, 0, sizeof(szTnNum));
    }
    
    for(i = 0; i < strlen(szTnNum) && j < 7; i++)
    {
        if(47 < *(szTnNum+i) && *(szTnNum+i) < 58)
        {
            szTn[j++] = *(szTnNum+i);
        }
        else if(64 < *(szTnNum+i) && *(szTnNum+i) < 91)
        {
            szTn[j++] = *(szTnNum+i);                
        }
        else if(96 < *(szTnNum+i) && *(szTnNum+i) < 123)
        {
            szTn[j++] = *(szTnNum+i);                
        }
        else
        {				
            continue ;
        }
    }
    
    return TRUE;
}

//车次号转换（从字符串转换为4+3Tax箱格式）
Bool GetTaxFormatByTn(UNS_8 *bTaxFormatTn, CHAR *szTn)
{
    int i = 0, j = 0, iTnCharCnt = 0;
    
    UNS_32 iTnNum = 0;
    
    if(szTn == NULL || bTaxFormatTn == NULL)
    {
        return FALSE;
    }
    
    if(!IsInvalidTn(szTn, strlen(szTn), &iTnCharCnt)) //合法车次号
    {
        for(i = 0, j = 0; i < 4-iTnCharCnt; i++)
        {
            *(bTaxFormatTn+j) = ' ';
            j++;
        }
        
         for(i = 0; i < iTnCharCnt; i++)
        {
            *(bTaxFormatTn+j) = *(szTn+i);
            
            j++;
        }
        
        iTnNum = atoi((CHAR *)(szTn+iTnCharCnt));
        
        if(iTnNum == 0)
        {
            *(bTaxFormatTn+4) = 0xff;
            *(bTaxFormatTn+5) = 0xff;
            *(bTaxFormatTn+6) = 0xff;            
        }
        else
        {
            UNS_16 wH, wL;
           
            wH = HIWORD(iTnNum);
            wL = LOWORD(iTnNum);
            
            *(bTaxFormatTn+4) = LOBYTE(wL);
            *(bTaxFormatTn+5) = HIBYTE(wL);
            *(bTaxFormatTn+6) = LOBYTE(wH);  
        }
        
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//车次号合法性判断
Bool IsInvalidTn(CHAR *szTn, int iTnLen, int *iChartCnt)
{
   int i = 0;
   
   int iLXCharCnt = 0, iLXNumCnt = 0, iBlankCnt = 0;

   Bool bLXNum = FALSE;
    
   if(szTn == NULL || iTnLen <= 0)
   {
	   return TRUE;
   }
   
   for(i = iTnLen-1; i >= 0; i--)
   {
	   if(szTn[i] == ' ')
	   {
		   iBlankCnt++;
	   }
   }

   iTnLen -= iBlankCnt;

   for(i = iTnLen-1; i >= 0; i--)
   {
	   if(szTn[i] >= '0' && szTn[i] <= '9')
	   {
		   if(i == (iTnLen-1))
		   {
			    bLXNum = TRUE;
		   }
		   
		   if(bLXNum)
		   {
				iLXNumCnt++;
		   }
	   }
	   else if(szTn[i] >= 'A' && szTn[i] <= 'Z')
	   {
		   if(i == (iTnLen-1))
		   {
				return TRUE;
		   }
		   else
		   {
			    bLXNum = FALSE;

				iLXCharCnt = i+1;

				break ;
		   }
	   }
	   else if(szTn[i] == ' ')
	   {

	   }
	   else
	   {
			return TRUE;
	   }
   }

   //特殊
   if(iLXCharCnt == 0 && iLXNumCnt >= 3 && szTn[0] == '0' && szTn[1] == '0' && szTn[2] == '0')
   {
	   return TRUE;
   }
   //特殊

   if(iLXCharCnt == 0 && iLXNumCnt >= 3 && szTn[0] == '0' && szTn[1] == '0' && szTn[2] != '0')
   {
	   iLXCharCnt = 2;

	   iLXNumCnt -= 2;
   }

   if(iLXCharCnt == 0 && iLXNumCnt >= 2 && szTn[0] == '0' && szTn[1] != '0')
   {
	   iLXCharCnt = 1;

	   iLXNumCnt -= 1;
   }
   
   if(iLXCharCnt > 2 || iLXNumCnt > 5)
   {
	   return TRUE;
   }

   *iChartCnt = iLXCharCnt;
   
   return FALSE;
}
