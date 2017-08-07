/************************************************************
  Copyright (C), 2009, 712.Co.,Ltd.
  FileName: crc.c
  Author: zhaoli      
  Description: 校验函数,BCH  编解码处理
  Version:       V1.0.0.1  
  Function List:   
  1. BCH  编解码
  History:        
  <author>  <time>   <version >   <desc>
   zhaoli      08.12.25      1.0.0.1    
***********************************************************/
#include "Bch.h"

 
/*********************** BCH编码 **************************
参    数:　uint8_t *srcPtr	  源数据地址
	               uint8_t *dstPtr   编码数据存放地址
	               uint8_t len	  源数据长度
返 回 值:　无
日    期:  2008.3.13
注    释:  	
*************************************************************/
void Bch_code(uint8_t *srcPtr,uint8_t *dstPtr,uint8_t len)
{
	uint8_t grpCnt;
	uint8_t byteCnt;
	uint8_t bitCnt;
	uint8_t dt;
	uint8_t bchDt0;
	uint8_t bchDt1;
	if (len % 2) *(srcPtr + (len++)) = 0x00;			//数据长度补足为偶数
	for (grpCnt = 0; grpCnt < (len >> 1) ; grpCnt++)
	{
		bchDt0 = 0;
		bchDt1 = 0;
		for (byteCnt = 0 ; byteCnt < 2 ; byteCnt++)
		{
			dt = *(srcPtr + byteCnt);
			for (bitCnt = 0 ; bitCnt < 8 ; bitCnt++)
			{
				if ((dt ^ bchDt0) & 0x80)
				{
					bchDt0 ^= 0x37;
					bchDt1 |= 0x20;	
				}
				else
				{
					bchDt1 &= 0xDF;	
				}
				bchDt0 <<= 1;
				if  (bchDt1 & 0x80) bchDt0 |= 0x01;
				bchDt1 <<= 1;
				dt <<= 1;
			}
		}
		bchDt1 &= 0xC0;
		*dstPtr = *srcPtr;
		*(dstPtr + 1) = *(srcPtr + 1);
		*(dstPtr + 2) = bchDt0;
		*(dstPtr + 3) = bchDt1;
		srcPtr += 2;
		dstPtr += 4;
	}				
}

/******************* BCH检错(1组) *************************
参    数:　uint8_t *Ptr	数据地址
返 回 值:　uint16_t		检错余式
日    期:  2008.3.13
注    释:  	
*************************************************************/
uint16_t Bch_chkErr(uint8_t *ptr)
{
	uint8_t errH = 0;				//2
	uint8_t errL = 0;				//1
	uint16_t  retErr;
	uint8_t chkBuf[4];
	uint8_t i;
	for (i = 0; i < 4; i++)
	{
		*(chkBuf + i) = *(ptr + i);
	}
	for (i =0 ; i < 32; i++)
	{
		errL <<= 1;				//循环左移
		if (errH & 0x80) errL |= 0x01;
		errH <<= 1;
		if (*chkBuf & 0x80) errH |= 0x01;
		*chkBuf <<= 1;
		if (*(chkBuf + 1) & 0x80) *chkBuf |= 0x01;
		*(chkBuf + 1) <<= 1;
		if (*(chkBuf + 2) & 0x80) *(chkBuf + 1) |= 0x01;
		*(chkBuf + 2) <<= 1;
		if (*(chkBuf + 3) & 0x80) *(chkBuf + 2) |= 0x01;
		*(chkBuf + 3) <<= 1;
		if (errL & 0x04) 
		{
			errL ^= 0x05;
			errH ^= 0xB9;
		}
	}
	retErr = errH;
	retErr <<= 8;
	retErr |= errL;
	return(retErr); 
}

/******************* BCH纠错(1组) *************************
参    数:　uint8_t *Ptr	数据地址
	   		 uint16_t err	前次检错余式
返 回 值:  uint8_t	纠错结果:0--失败;1--成功				
日    期:  2008.3.13
注    释:  	
*************************************************************/
uint8_t Bch_crtErr(uint8_t *ptr,uint16_t err)
{
	uint8_t qDt[4];
	uint8_t i;
	uint8_t j;
	for (i = 1 ; i < 32 ; i++)
	{
		*qDt = i;
		*(qDt + 1) = 0x00;
		*(qDt + 2) = 0x00;
		*(qDt + 3) = 0x00;
		for (j = 0 ; j < 22 ;j++)
		{
			if (Bch_chkErr(qDt) == err)
			{
				for (i = 0 ; i < 4; i++) 				
				{	
					*(ptr + i) ^= *(qDt + i);		//保存纠错结果
				}
				return(1);					        //返回纠错成功标志 
			}
			else
			{
				*(qDt + 3) >>= 1;				    //循环右移
				if (*(qDt + 2) & 0x01) *(qDt + 3) |= 0x80;
				*(qDt + 2) >>= 1;
				if (*(qDt + 1) & 0x01) *(qDt + 2) |= 0x80;
				*(qDt + 1) >>= 1;
				if (*qDt & 0x01) *(qDt + 1) |= 0x80;
				*qDt >>= 1;
			}
		}
	}
	return(0);								//纠错失败
}

/*********************** BCH解码 **************************
参    数:　uint8_t *srcPtr	       源数据地址
	               uint8_t *dstPtr	       解码数据存放地址
	               uint8_t len		源数据长度
返 回 值:  uint16_t			解码后数据长度
日    期:      2008.3.13
注    释:  	
*************************************************************/
uint16_t Bch_decode(uint8_t *srcPtr,uint8_t *dstPtr,uint16_t len)
{
	uint8_t decBuf[4];					//缓存区
	uint8_t grpCnt;						//组计数
	uint8_t i;
	uint16_t err;						    //检错结果
	uint16_t byteCnt = 0;					//解码字节数
	for (grpCnt = 0 ; grpCnt < (len >> 2) ; grpCnt++)		//数据读入缓存区
	{
		for (i = 0; i < 4; i++)
		{
			*(decBuf + i) =*(srcPtr + i);
		}
		*(decBuf + 3) &= 0xC0; 			//屏蔽无用位
		err = Bch_chkErr(decBuf);		//检错
		if (!err) 						//无错误
		{
			*dstPtr = *decBuf;
			*(dstPtr + 1) = *(decBuf + 1);
			byteCnt += 2;
			srcPtr += 4;
			dstPtr += 2;
			continue;
		} 
		else							//纠错					
		{
			if(Bch_crtErr(decBuf,err))	//纠错成功
			{
				*dstPtr = *decBuf;
				*(dstPtr + 1) = *(decBuf + 1);	
				byteCnt += 2;
				srcPtr += 4;
				dstPtr += 2;
			}
			else						//纠错不成功
			{
				return(0);			
			}
		}
	}
	return(byteCnt);				   //返回解码字节数
}

