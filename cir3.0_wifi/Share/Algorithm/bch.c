/************************************************************
  Copyright (C), 2009, 712.Co.,Ltd.
  FileName: crc.c
  Author: zhaoli      
  Description: У�麯��,BCH  ����봦��
  Version:       V1.0.0.1  
  Function List:   
  1. BCH  �����
  History:        
  <author>  <time>   <version >   <desc>
   zhaoli      08.12.25      1.0.0.1    
***********************************************************/
#include "Bch.h"

 
/*********************** BCH���� **************************
��    ��:��uint8_t *srcPtr	  Դ���ݵ�ַ
	               uint8_t *dstPtr   �������ݴ�ŵ�ַ
	               uint8_t len	  Դ���ݳ���
�� �� ֵ:����
��    ��:  2008.3.13
ע    ��:  	
*************************************************************/
void Bch_code(uint8_t *srcPtr,uint8_t *dstPtr,uint8_t len)
{
	uint8_t grpCnt;
	uint8_t byteCnt;
	uint8_t bitCnt;
	uint8_t dt;
	uint8_t bchDt0;
	uint8_t bchDt1;
	if (len % 2) *(srcPtr + (len++)) = 0x00;			//���ݳ��Ȳ���Ϊż��
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

/******************* BCH���(1��) *************************
��    ��:��uint8_t *Ptr	���ݵ�ַ
�� �� ֵ:��uint16_t		�����ʽ
��    ��:  2008.3.13
ע    ��:  	
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
		errL <<= 1;				//ѭ������
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

/******************* BCH����(1��) *************************
��    ��:��uint8_t *Ptr	���ݵ�ַ
	   		 uint16_t err	ǰ�μ����ʽ
�� �� ֵ:  uint8_t	������:0--ʧ��;1--�ɹ�				
��    ��:  2008.3.13
ע    ��:  	
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
					*(ptr + i) ^= *(qDt + i);		//���������
				}
				return(1);					        //���ؾ���ɹ���־ 
			}
			else
			{
				*(qDt + 3) >>= 1;				    //ѭ������
				if (*(qDt + 2) & 0x01) *(qDt + 3) |= 0x80;
				*(qDt + 2) >>= 1;
				if (*(qDt + 1) & 0x01) *(qDt + 2) |= 0x80;
				*(qDt + 1) >>= 1;
				if (*qDt & 0x01) *(qDt + 1) |= 0x80;
				*qDt >>= 1;
			}
		}
	}
	return(0);								//����ʧ��
}

/*********************** BCH���� **************************
��    ��:��uint8_t *srcPtr	       Դ���ݵ�ַ
	               uint8_t *dstPtr	       �������ݴ�ŵ�ַ
	               uint8_t len		Դ���ݳ���
�� �� ֵ:  uint16_t			��������ݳ���
��    ��:      2008.3.13
ע    ��:  	
*************************************************************/
uint16_t Bch_decode(uint8_t *srcPtr,uint8_t *dstPtr,uint16_t len)
{
	uint8_t decBuf[4];					//������
	uint8_t grpCnt;						//�����
	uint8_t i;
	uint16_t err;						    //�����
	uint16_t byteCnt = 0;					//�����ֽ���
	for (grpCnt = 0 ; grpCnt < (len >> 2) ; grpCnt++)		//���ݶ��뻺����
	{
		for (i = 0; i < 4; i++)
		{
			*(decBuf + i) =*(srcPtr + i);
		}
		*(decBuf + 3) &= 0xC0; 			//��������λ
		err = Bch_chkErr(decBuf);		//���
		if (!err) 						//�޴���
		{
			*dstPtr = *decBuf;
			*(dstPtr + 1) = *(decBuf + 1);
			byteCnt += 2;
			srcPtr += 4;
			dstPtr += 2;
			continue;
		} 
		else							//����					
		{
			if(Bch_crtErr(decBuf,err))	//����ɹ�
			{
				*dstPtr = *decBuf;
				*(dstPtr + 1) = *(decBuf + 1);	
				byteCnt += 2;
				srcPtr += 4;
				dstPtr += 2;
			}
			else						//�����ɹ�
			{
				return(0);			
			}
		}
	}
	return(byteCnt);				   //���ؽ����ֽ���
}

