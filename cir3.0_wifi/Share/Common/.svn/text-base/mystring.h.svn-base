/**********************************************************************
* $Id$      MyString              2016-06-24
*//**
* @file     mystring.h
* @brief    程序软件版本号管理头文件
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
/*从字符串的左边截取n个字符*/
CHAR * MyString_Left(CHAR *dst,CHAR *src,  INT_32 n);

/*从字符串pos位置截取size个字符*/
CHAR * MyString_Mid(CHAR *src, CHAR *dst, INT_32 pos, INT_32 size);

/*从字符串的右边截取size个字符*/
CHAR * MyString_Right(CHAR *dst, CHAR *src, INT_32 size);

//从szAll中查找szSub字符串的位置
//INT_32 MyString_Find(CHAR *szAll, CHAR *szSub);
INT_32 MyString_Find(char all[], char sub[], INT_32 ipos);

//从pos位置开始到“,”结束，截取字符串
Bool MyString_ParseStringByComma(CHAR *dst, CHAR *src, INT_32 pos);
//从pos位置开始到“.”结束，截取字符串
Bool MyString_ParseStringByDot(CHAR *dst, CHAR *src, INT_32 pos);
//从pos位置开始到“；”结束，截取字符串
Bool MyString_ParseStringByYinHao(CHAR *dst, CHAR *src, INT_32 pos);

//从pos位置开始到“，”结束，分析出一个整数
INT_32 MyString_ParseINTByComma(CHAR *dst,CHAR *src, INT_32 pos);
//从pos位置开始到“.”结束，分析出一个整数
INT_32 MyString_ParseINTByDot(CHAR *dst,CHAR *src, INT_32 pos);
//从pos位置开始到“；”结束，分析出一个整数
INT_32 MyString_ParseINTByYinHao(CHAR *dst,CHAR *src, INT_32 pos);

//将src字符串从左边去除空格
void MyString_TrimLeft(CHAR *dst, CHAR *src);

//将src字符串从右边去除空格
void MyString_TrimRight(CHAR *dst, CHAR *src);

//从两端去除空格
void MyString_TrimLeftRight(CHAR *dst, CHAR *src);

//将src字符串从左边去除空格
void MyString_TrimLeft2(CHAR *dst, CHAR *src, INT_32 _iLen);

//将src字符串从右边去除空格
void MyString_TrimRight2(CHAR *dst, CHAR *src, INT_32 _iLen);

//从两端去除空格
void MyString_TrimLeftRight2(CHAR *dst, CHAR *src, INT_32 _iLen);

//截取字符串
Bool MatchString(CHAR *szAll, CHAR *szSub);
void CutString(CHAR *szReturn, CHAR *szAll, CHAR *szSub);

//交换两个字符
void MyCharSwap(CHAR *a, CHAR *b);

#endif


