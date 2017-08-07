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

int Year_To_Day(int year_begin, int year_end);  //两个年份之间相差的天数;
int Month_To_Day(date now);  //判断该日期是该年的第几天
int Day_To_Second(date now); //把时间转化为一天之内的第几秒
int Leap_Year(int year);  //判断是否是闰年,是闰年返回1，不是闰年返回0;
datespan Calculate(date a, date b); //比较两个日期的差值;a表示的日期如果在b之后，返回一个datespan类型的结构体成员全部为0
date Date_String_To_Format(char *string_date); //把字符串改为日期类型
int Week(char *string_date); //判断输入的日期为星期几（字符串类型输入）
Bool Judge(date d1); //判断时间是否是合理的,如果合理为TRUE，不合理为FALSE;
Bool Judge2(date date_begin, date date_end);//判断两个日期参数先后顺序，第一个参数日期在前为TRUE，否则为FALSE；
date Now_to_Later(date now, datespan gap); //从现在日期计算多少天之后的日期;
date Now_To_Fronter(date now, datespan gap); //从现在日期计算多少天之前的日期
datespan JudgeSpan(int day, int hour, int minute, int second);

#endif


