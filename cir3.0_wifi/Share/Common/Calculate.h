#ifndef CALCULATE_H
#define CALCULATE_H

#include "lpc_types.h"

typedef struct
{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
}date;

typedef struct{
	int span_day;
	int span_hour;
	int span_minute;
	int span_second;
}datespan;

int Year_To_Day(int year_begin, int year_end);  //�������֮����������;
int Month_To_Day(date now);  //�жϸ������Ǹ���ĵڼ���
int Day_To_Second(date now); //��ʱ��ת��Ϊһ��֮�ڵĵڼ���
int Leap_Year(int year);  //�ж��Ƿ�������,�����귵��1���������귵��0;
datespan Calculate(date a, date b); //�Ƚ��������ڵĲ�ֵ;a��ʾ�����������b֮�󣬷���һ��datespan���͵Ľṹ���Աȫ��Ϊ0
date Date_String_To_Format(char *string_date); //���ַ�����Ϊ��������
int Week(char *string_date); //�ж����������Ϊ���ڼ����ַ����������룩
Bool Judge(date d1); //�ж�ʱ���Ƿ��Ǻ����,�������ΪTRUE��������ΪFALSE;
Bool Judge2(date date_begin, date date_end);//�ж��������ڲ����Ⱥ�˳�򣬵�һ������������ǰΪTRUE������ΪFALSE��
date Now_to_Later(date now, datespan gap); //���������ڼ��������֮�������;
date Now_To_Fronter(date now, datespan gap); //���������ڼ��������֮ǰ������
datespan JudgeSpan(int day, int hour, int minute, int second);

#endif


