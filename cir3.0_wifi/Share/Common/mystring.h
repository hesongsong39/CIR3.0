/**********************************************************************
* $Id$      MyString              2016-06-24
*//**
* @file     mystring.h
* @brief    ��������汾�Ź���ͷ�ļ�
* @version  1.0.0
* @date     2016/06/24 
* @author   Zhaohualei
*
* Copyright(C) 2016, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
#ifndef _MY_STRING_H_
#define _MY_STRING_H_

/************************** Includes **********************************/
#include "lpc_types.h"

/************************** Function Interface ************************/
/*���ַ�������߽�ȡn���ַ�*/
CHAR * MyString_Left(CHAR *dst,CHAR *src,  INT_32 n);

/*���ַ���posλ�ý�ȡsize���ַ�*/
CHAR * MyString_Mid(CHAR *src, CHAR *dst, INT_32 pos, INT_32 size);

/*���ַ������ұ߽�ȡsize���ַ�*/
CHAR * MyString_Right(CHAR *dst, CHAR *src, INT_32 size);

//��szAll�в���szSub�ַ�����λ��
//INT_32 MyString_Find(CHAR *szAll, CHAR *szSub);
INT_32 MyString_Find(char all[], char sub[], INT_32 ipos);

//��posλ�ÿ�ʼ����,����������ȡ�ַ���
Bool MyString_ParseStringByComma(CHAR *dst, CHAR *src, INT_32 pos);
//��posλ�ÿ�ʼ����.����������ȡ�ַ���
Bool MyString_ParseStringByDot(CHAR *dst, CHAR *src, INT_32 pos);
//��posλ�ÿ�ʼ����������������ȡ�ַ���
Bool MyString_ParseStringByYinHao(CHAR *dst, CHAR *src, INT_32 pos);

//��posλ�ÿ�ʼ��������������������һ������
INT_32 MyString_ParseINTByComma(CHAR *dst,CHAR *src, INT_32 pos);
//��posλ�ÿ�ʼ����.��������������һ������
INT_32 MyString_ParseINTByDot(CHAR *dst,CHAR *src, INT_32 pos);
//��posλ�ÿ�ʼ��������������������һ������
INT_32 MyString_ParseINTByYinHao(CHAR *dst,CHAR *src, INT_32 pos);

//��src�ַ��������ȥ���ո�
void MyString_TrimLeft(CHAR *dst, CHAR *src);

//��src�ַ������ұ�ȥ���ո�
void MyString_TrimRight(CHAR *dst, CHAR *src);

//������ȥ���ո�
void MyString_TrimLeftRight(CHAR *dst, CHAR *src);

//��src�ַ��������ȥ���ո�
void MyString_TrimLeft2(CHAR *dst, CHAR *src, INT_32 _iLen);

//��src�ַ������ұ�ȥ���ո�
void MyString_TrimRight2(CHAR *dst, CHAR *src, INT_32 _iLen);

//������ȥ���ո�
void MyString_TrimLeftRight2(CHAR *dst, CHAR *src, INT_32 _iLen);

//��ȡ�ַ���
Bool MatchString(CHAR *szAll, CHAR *szSub);
void CutString(CHAR *szReturn, CHAR *szAll, CHAR *szSub);

//���������ַ�
void MyCharSwap(CHAR *a, CHAR *b);

#endif


