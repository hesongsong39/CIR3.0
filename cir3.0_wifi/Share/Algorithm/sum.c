/**********************************************************************
* $Id$      Sum              2016-06-07
*//**
* @file     sum.c
* @brief    Ӧ�ò����ҵ��
* @version  1.0.0
* @date     2016/06/07 
* @author   ZhaoLi
*
* Copyright(C) 2016, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
/************************** Includes **********************************/
#include <stdio.h>   
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "sum.h" 


/*****************Private variables ***********************************/


/*****************Public variables ************************************/


/*****************Extern variables ************************************/



/***********************************************************************
��������:����У���
�������:
����ʱ��:
�޸�ʱ��:
***********************************************************************/
uint16_t Get_CheckSum (uint8_t *pData, uint16_t len)
{
	uint16_t sum = 0;
	uint16_t i = 0;

	for (i = 0; i < len; i++)
	{
		sum += *(pData + i);
	}

	return sum;
}

/***********************************************************************
��������:��֤У���
�������:
����ʱ��:
�޸�ʱ��:
***********************************************************************/
Bool Validate_CheckSum(uint8_t *pData, uint16_t len)
{
	uint16_t chksum = 0;
	uint16_t sum = 0;
	uint16_t i = 0;

	for (i = 0; i < (len - 2); i++)
	{
		sum += *(pData + i);    //��֤У���
	}

	memcpy(&chksum, pData + len - sizeof chksum, sizeof chksum);

	if(sum == 0)
	{
			return FALSE;
	}
	
	return (sum == chksum) ? TRUE : FALSE;
}

