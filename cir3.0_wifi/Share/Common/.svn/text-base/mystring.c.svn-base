/**********************************************************************
* $Id$      mystring              2016-06-24
*//**
* @file     mystring.c
* @brief    自己写的字符串处理函数集合
* @version  1.0.0
* @date     2016/06/24
* @author   Zhaohualei
*
* Copyright(C) 2016, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
/************************** Includes **********************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "lpc_types.h"
#include "mystring.h"

/*****************Private variables ***********************************/

/*****************Public variables ************************************/

/*****************Extern variables ************************************/

/************************函数集合**************************************/

/*从字符串的左边截取n个字符*/
CHAR * MyString_Left(CHAR *dst,CHAR *src,  INT_32 n)
{
    CHAR *p = src;
    CHAR *q = dst;
	
    INT_32 len = strlen(src);
	
    if(n>len)
		{
			n = len;
		}
		
    /*p += (len-n);*//*从右边第n个字符开始*/
		
    while(n--) 
		{
			*(q++) = *(p++);
		}
		
    *(q++)='\0'; /*很有必要*/
		
    return dst;
}

/*从字符串的中间截取n个字符*/
CHAR * MyString_Mid(CHAR *src, CHAR *dst, INT_32 pos, INT_32 size) 
{
    CHAR *p = src;
	
    CHAR *q = dst;
	
    INT_32 len = strlen(src);
	
    if(size > len) 
		{
			size = len - pos;    /*从第m个到最后*/
		}
		
    if(pos < 0)
		{
			pos = 0;    /*从第一个开始*/
		}
		
    if(pos > len)
		{
			return NULL;
		}
		
    p += pos;
		
    while(size--)
		{
			*(q++) = *(p++);
		}
		
    *(q++)='\0'; /*很有必要*/
		
    return dst;
}

/*从字符串的右边截取n个字符*/
CHAR * MyString_Right(CHAR *dst, CHAR *src, INT_32 size)
{
    CHAR *p = src;
    CHAR *q = dst;
	
    INT_32 len = strlen(src);
	
    if(size > len)
		{
			size = len;
		}
		
    p += (len-size);   /*从右边第n个字符开始*/
	
    while(size--)	
		{
			*(q++) = *(p++);
		}
		
    *(q++)='\0'; /*很有必要*/
		
    return dst;
}

INT_32 MyString_Find(char all[], char sub[], INT_32 ipos)
{
    INT_32 i,j,temp;
	
	  INT_32 iLen = strlen(all) - strlen(sub);
	
    for(i = ipos; i <= iLen; i++)
    {
        temp = i;
			
        j = 0;
			
        while(j < strlen(sub) && all[temp] == sub[j])
        {
            temp++;
					
            j++;
        }
				
        if(j == strlen(sub))
				{
            return i;
				}
    }
		
    return -1;//返回-1则没找到指定的子串,否则找到
}

Bool MyString_ParseStringByComma(CHAR *dst, CHAR *src, INT_32 size)
{
	  INT_32 ilen = 0;
	  INT_32 ilen2= 0;
	
	  CHAR *p = src;
    CHAR *q = dst;
	
    ilen = strlen(src);
    
    if(size >= ilen)
		{
			return FALSE;
		}
		
		p+=size;
		
		ilen2 = ilen-size;
		
		while(ilen2--)
		{
			if(*(p) != ',')
			{
				*(q++) = *(p++);
			}
			else
			{
				break;
			}
		}
		
		*(q++)='\0'; /*有必要吗？很有必要*/
		
		return TRUE;
}

INT_32 MyString_ParseINTByComma(CHAR *dst,CHAR *src, INT_32 size)
{
	 if(MyString_ParseStringByComma(dst, src, size))
	 {
		 return atoi(dst);
	 }
	 else
	 {
		 return -1;
	 }
}

Bool MyString_ParseStringByDot(CHAR *dst, CHAR *src, INT_32 size)
{
	  INT_32 ilen = 0;
	  INT_32 ilen2= 0;
	
	  CHAR *p = src;
    CHAR *q = dst;
	
    ilen = strlen(src);
    
    if(size >= ilen)
		{
			return FALSE;
		}
		
		p+=size;
		
		ilen2 = ilen-size;
		
		while(ilen2--)
		{
			if(*(p) != '.')
			{
				*(q++) = *(p++);
			}
			else
			{
				break;
			}
		}
		
		*(q++)='\0'; /*有必要吗？很有必要*/
		
		return TRUE;
}

INT_32 MyString_ParseINTByDot(CHAR *dst,CHAR *src, INT_32 size)
{
	 if(MyString_ParseStringByDot(dst, src, size))
	 {
		 return atoi(dst);
	 }
	 else
	 {
		 return -1;
	 }
}

Bool MyString_ParseStringByYinHao(CHAR *dst, CHAR *src, INT_32 size)
{
	  INT_32 ilen = 0;
	  INT_32 ilen2= 0;
	
	  CHAR *p = src;
    CHAR *q = dst;
	
    ilen = strlen(src);
    
    if(size >= ilen)
		{
			return FALSE;
		}
		
		p+=size;
		
		ilen2 = ilen-size;
		
		while(ilen2--)
		{
			if(*(p) != '"')
			{
				*(q++) = *(p++);
			}
			else
			{
				break;
			}
		}
		
		*(q++)='\0'; /*有必要吗？很有必要*/
		
		return TRUE;
}

INT_32 MyString_ParseINTByYinHao(CHAR *dst,CHAR *src, INT_32 size)
{
	 if(MyString_ParseStringByYinHao(dst, src, size))
	 {
		 return atoi(dst);
	 }
	 else
	 {
		 return -1;
	 }
}

void MyString_TrimLeft(CHAR *dst, CHAR *src)
{
	INT_32 ilen = strlen(src);
	Bool bFirtFind = FALSE;
	
	CHAR *s = src;
	CHAR *d = dst;
	
	if(src == NULL || dst == NULL || ilen <= 0)
	{
		return ;
	}
	
	while(ilen--)
	{
		if(bFirtFind)
		{
			*(d++) = *(s++);			
	  }
		else
		{
			 if(*(s) == ' ')
			{
				s++;
			}
			else
			{
				*(d++) = *(s++);
				
				bFirtFind = TRUE;
			}
		}
	}
}

void MyString_TrimRight(CHAR *dst, CHAR *src)
{
	INT_32 ilen = strlen(src);
    
	Bool bFirtFind = FALSE;
	
	CHAR *s = src;
	CHAR *d = dst;
	
	if(src == NULL || dst == NULL || ilen <= 0)
	{
		return ;
	}
	
	ilen--;
	
	do
	{
		if(bFirtFind)
		{
			*(d+ilen) = *(s+ilen);			
	  }
		else
		{
			 if(*(s+ilen) == ' ')
			{
				;
			}
			else
			{
				*(d+ilen) = *(s+ilen);
				
				bFirtFind = TRUE;
			}
		}
		
	}while(ilen--);
}

void MyString_TrimLeftRight(CHAR *dst, CHAR *src)
{
	INT_32 ilen = strlen(src);
	
	CHAR *s = src;
	//CHAR *d = dst;
	
	INT_32 start = 0, end = ilen;
	
  Bool bChanged = TRUE;
	
	if(src == NULL || dst == NULL || ilen <= 0)
	{
		return ;
	}
	
  while (start < end && bChanged)
  {
			bChanged = FALSE;
			
			if (*(s+start) == ' ')
			{
				++start;
				
				bChanged = TRUE;
			}
			else
			{
					if (*(s+end-1) == ' ')
					{
						--end;
						
						bChanged = TRUE;
					}
			}
  }
	
	MyString_Mid(src, dst, start, end - start);
}

void MyString_TrimLeft2(CHAR *dst, CHAR *src, INT_32 _iLen)
{
	INT_32 ilen = _iLen;
    
	Bool bFirtFind = FALSE;
	
	CHAR *s = src;
	CHAR *d = dst;
	
	if(src == NULL || dst == NULL || ilen <= 0)
	{
		return ;
	}
	
	while(ilen--)
	{
		if(bFirtFind)
		{
			*(d++) = *(s++);			
	  }
		else
		{
			 if(*(s) == ' ')
			{
				s++;
			}
			else
			{
				*(d++) = *(s++);
				
				bFirtFind = TRUE;
			}
		}
	}
}

void MyString_TrimRight2(CHAR *dst, CHAR *src, INT_32 _iLen)
{
	INT_32 ilen = _iLen;
    
	Bool bFirtFind = FALSE;
	
	CHAR *s = src;
	CHAR *d = dst;
	
	if(src == NULL || dst == NULL || ilen <= 0)
	{
		return ;
	}
	
	ilen--;
	
	do
	{
		if(bFirtFind)
		{
			*(d+ilen) = *(s+ilen);			
	  }
		else
		{
			 if(*(s+ilen) == ' ')
			{
				;
			}
			else
			{
				*(d+ilen) = *(s+ilen);
				
				bFirtFind = TRUE;
			}
		}
		
	}while(ilen--);
}

void MyString_TrimLeftRight2(CHAR *dst, CHAR *src, INT_32 _iLen)
{
	INT_32 ilen = _iLen;
	
	CHAR *s = src;
	//CHAR *d = dst;
	
	INT_32 start = 0, end = ilen;
	
  Bool bChanged = TRUE;
	
	if(src == NULL || dst == NULL || ilen <= 0)
	{
		return ;
	}
	
  while (start < end && bChanged)
  {
			bChanged = FALSE;
			
			if (*(s+start) == ' ')
			{
				++start;
				
				bChanged = TRUE;
			}
			else
			{
					if (*(s+end-1) == ' ')
					{
						--end;
						
						bChanged = TRUE;
					}
			}
  }
	
	MyString_Mid(src, dst, start, end - start);
}

Bool MatchString(CHAR *szAll, CHAR *szSub)
{
	if(szAll == NULL || strlen(szAll) == 0)
	{
			return FALSE;
	}
	
	if(szSub == NULL || strlen(szSub) == 0)
	{
		return FALSE;
	}
	
	if (strstr(szAll, szSub))
	{
		return TRUE;
	}
	
	return FALSE;
}

void CutString(CHAR *szReturn, CHAR *szAll, CHAR *szSub)
{
	INT_32 iPos = -1;
	
	INT_32 iLen1 = strlen(szAll);
	INT_32 iLen2 = strlen(szSub);
	
	if(szAll == NULL || szSub == NULL || iLen1 <= 0 || iLen2 <= 0)
	{
		return ;
	}
	
	iPos = MyString_Find(szAll, szSub, 0);
	
	if(iPos == 0)
	{
		MyString_Right(szReturn, szAll, iLen1 - iLen2);
	}
}

void MyCharSwap(CHAR *a, CHAR *b)
{
	CHAR c;
	
	if(a == NULL || b == NULL)
	{
		return ;
	}
	
	c = *b;
	*b= *a;
	*a=c;
}
