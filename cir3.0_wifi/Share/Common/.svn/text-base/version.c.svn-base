/**********************************************************************
* $Id$      Version              2016-03-10
*//**
* @file     Version.c
* @brief    程序软件版本号管理
* @version  1.0.0
* @date     2016/03/10 星期四 08:20:15
* @author   ZhangYanMing
*
* Copyright(C) 2016, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
/************************** Includes **********************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Version.h"

/*****************Private variables ***********************************/

/*****************Public variables ************************************/


/*****************Extern variables ************************************/

/*********************************************************************//**
 * @brief 			软件版版本号
 * @param[in]		pVersion 存放软件版本号的内存指针
 * @param[in]   plen 存放软件版本号的内存缓冲长度
 * @param[in]		pSrc 存放软件版本号规则内存指针
 * @param[in]   slen 软件版本号源长度
 * @return			软件版本号占内存字节数，若plen不足则返回0
 ************************************************************************/
uint8_t GetSoftwareVersion(char *pVersion,uint8_t plen,char *pSrc,uint8_t slen)
{
	uint8_t i = 0,day = 0;
	char szDate[16],sztime[12];
	const char Month[12][8] = {"Jan", "Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
	
	memset(szDate, 0, sizeof(szDate));
	memset(sztime,0,sizeof(sztime));
	strcpy(szDate, __DATE__);

	day = atoi(szDate+4);

	for(i = 0; i < 12; i++)
	{
		if(strstr(szDate, Month[i]))
		{
			sprintf(sztime,"%s/%02d/%02d",szDate+9,i+1,day);
			if((strlen(sztime) + slen + 1) >= plen)
			{
					return 0;
			}
			memcpy(pVersion,pSrc,slen);
			pVersion += slen;
			*pVersion++ = '-';
			memcpy(pVersion,sztime,strlen(sztime));
			break;
		}
	}
	return (strlen(sztime) + slen + 1);
}

