// Calculate.c : 和时间操作相关的函数
//

#include "stdlib.h"
#include "string.h"
#include "Calculate.h"

#define  TotalTime  24*60*60

int month[12]={31,28,31,30,31,30,31,31,30,31,30,31};


int Year_To_Day(int year_begin, int year_end)
{
	int year=year_begin;
	int sum=0;
	
	while (year!=year_end)
	{
		sum=sum+365+Leap_Year(year);
		year++;
	}
	
	return sum;
}


int Month_To_Day(date now)
{
	int i, day = 0;
	
    for (i = 0; i < now.month - 1; ++i)
	{
	   day = day + month[i];
	}
	
    return (now.month >= 3) ? day + now.day + Leap_Year(now.year) : day + now.day;
}


int Day_To_Second(date now)
{
	return (now.hour)*60*60+(now.minute)*60+(now.second);
}


int Leap_Year(int year)
{
	return ((year % 400 == 0) || ((year % 4 == 0)&& (year % 100 != 0))) ? 1 : 0;
}


date Date_String_To_Format(char *string_date)
{
	 char year[5], month[3], day[3], hour[3], minute[3], second[3];
     date d1;
     memset(year, 0x0, sizeof(year));
     memset(month, 0x0, sizeof(month));
     memset(day, 0x0, sizeof(day));
     memset(hour, 0x0, sizeof(hour));
     memset(minute, 0x0, sizeof(minute));
	 memset(second, 0x0, sizeof(second));
     memcpy(year, string_date, 4);
     memcpy(month, string_date+4, 2);
     memcpy(day, string_date+6, 2);
	 memcpy(hour, string_date+8, 2);
     memcpy(minute, string_date+10, 2);
     memcpy(second, string_date+12, 2);
     d1.year = atoi(year);
     d1.month = atoi(month);
     d1.day = atoi(day);
	 d1.hour = atoi(hour);
	 d1.minute = atoi(minute);
	 d1.second = atoi(second);
     return d1;
}


Bool Judge(date d1)
{
	return ((d1.year>0 &&d1.year<=9999) && (d1.month>0 && d1.month<=12) 
        && (d1.day>0 
            && (((d1.month==2) && (d1.day<=month[d1.month-1]+Leap_Year(d1.year)))
                ||((d1.month!=2) && (d1.day<=month[d1.month-1]))))) ? TRUE : FALSE;
}


datespan Calculate(date date_begin,date date_end)
{
	int span_days = 0;
	int second_span = 0;
	Bool compare = FALSE;
	datespan d1={0};
	
	compare=Judge2(date_begin,date_end);
	if(compare == FALSE)
	{
		return d1;
	}
	
	span_days = Year_To_Day(date_begin.year, date_end.year) - Month_To_Day(date_begin) + Month_To_Day(date_end);
	
	/*比较两个时间在同一天内的先后*/
	if(Day_To_Second(date_begin) <= Day_To_Second(date_end))
	{
		second_span = Day_To_Second(date_end)-Day_To_Second(date_begin);
	}
	else
	{
		second_span = TotalTime - Day_To_Second(date_begin) + Day_To_Second(date_end);
		span_days--;
	}
	
	//转化成相应的结构体类型
	d1.span_day = span_days;
	d1.span_hour = second_span / (60*60);
	second_span=second_span % (60*60);
	d1.span_minute = second_span / 60 ;
	d1.span_second = second_span % 60 ;
	return d1;
}


int Week(char *string_date)
{
	date d1;
    int century = 0;
    int year = 0;
    int weekday = 0;
    int month = 0;
	
    d1=Date_String_To_Format(string_date);
    if (Judge(d1))
	{
		if (d1.month < 3)
		{
			month = d1.month + 12;
			d1.year--;
		}
		else
			month = d1.month;
		
		century = d1.year/100;
        year = d1.year % 100;
		weekday = year + (year/4)+(century/4)-(2*century)+(26*(month + 1)/10) + d1.day - 1;
        weekday = (weekday < 0) ? weekday + 7: weekday;
	}
	
	return (weekday % 7 == 0) ? 7 : weekday % 7;
}


datespan JudgeSpan(int day,int hour,int minute,int second)
{
	datespan result={0};
	int index = 0;
	
	while(hour>=24 || minute >= 60 || second >= 60)
	{
		index=0;
		
		if(hour >= 24)
		{
			index = hour;
			hour = hour % 24 ;
			day = day + (index / 24);
		}
		
		if(minute >= 60)
		{
			index = minute;
			minute = minute % 60;
			hour = hour + (index / 60);
		}
		
		if(second >=60)
		{
			index = second;
			second = second % 60;
			minute = minute + (index / 60);
		}
	}
	
	result.span_day=day;
	result.span_hour=hour;
	result.span_minute=minute;
	result.span_second= second;
	return result;
}


date Now_to_Later(date now, datespan gap)
{
	date d1={0};
	int totalsecond = 0;
	int span_totalsecond = 0;
	int sum = 0, sum_bak=0;
    int temp = 0;
	
	gap=JudgeSpan(gap.span_day,gap.span_hour,gap.span_minute,gap.span_second);
	totalsecond = now.hour* 3600 + now.minute * 60 + now.second;
	span_totalsecond = gap.span_hour* 3600 + gap.span_minute * 60 + gap.span_second;
	
	if((TotalTime-totalsecond) <= span_totalsecond)
	{
		span_totalsecond = span_totalsecond - (TotalTime - totalsecond);
		totalsecond = span_totalsecond;
		gap.span_day++;
	}
	else
	{
		totalsecond += span_totalsecond;
	}
	
	now.hour = totalsecond / (60*60);
	totalsecond = totalsecond % (60*60);
	now.minute = totalsecond / 60 ;
	now.second = totalsecond % 60 ;
    
    d1 = now;
	sum = gap.span_day;
	
    if(Judge(d1)&&(sum>0) && (sum < 3649270))
    {
		while(sum>365)
		{
			if(d1.month>=3)
			{
				d1.year++;
				sum=sum-365-Leap_Year(d1.year);
			}
			else
			{
				sum=sum-365-Leap_Year(d1.year);
				d1.year++;
			}
		}
		
		while(sum > 0)
		{
			if (d1.month != 2)
			{
				temp = month[d1.month - 1] -d1.day + 1;
			}
			else
			{
				temp = month[d1.month - 1] +Leap_Year(d1.year)- d1.day + 1;
			}
			
			sum_bak = sum;
			sum = sum - temp;
			if (sum  >= 0)
			{
				d1.month++;
				d1.day = 1;
				
				if (d1.month > 12)
				{
					d1.month = 1;
					d1.year++;
				}
				
				sum_bak = sum;
			}
		}
		
		d1.day += sum_bak;
	} 
	
	return d1;
}


date Now_To_Fronter(date now, datespan gap)
{
	date d1;
    int sum = 0, sum_bak = 0, index = 0;
    int temp = 0;
	int totalsecond = 0;
	int span_totalsecond = 0;
	
	gap=JudgeSpan(gap.span_day,gap.span_hour,gap.span_minute,gap.span_second);
	totalsecond = now.hour * 3600 + now.minute * 60 + now.second;
	span_totalsecond = gap.span_hour * 3600 + gap.span_minute * 60 + gap.span_second;
	
	if(totalsecond < span_totalsecond)
	{
		span_totalsecond = span_totalsecond - totalsecond;
		totalsecond = TotalTime - span_totalsecond;
		gap.span_day++;
	}
	else
	{
		totalsecond -= span_totalsecond;
	}
	
	now.hour = totalsecond / (60*60);
	totalsecond = totalsecond % (60*60);
	now.minute = totalsecond / 60 ;
	now.second = totalsecond % 60 ;
	d1 = now;
	index=-gap.span_day;
	sum = index;
    
	if(Judge(d1)&&(sum < 0) && (sum > -3649270))
    {
		while(sum < -365)
		{
			if(d1.month>=3)
			{
				sum = sum + 365 + Leap_Year(d1.year);
                d1.year--;
			}
			else
			{
				d1.year--;
				sum = sum + 365 + Leap_Year(d1.year);
			}
		}
		
		sum_bak = sum;
		while(sum < 0)
		{
			temp = -d1.day;
			sum = sum - temp;
			
			if (sum  <= 0)
			{
				d1.month--;
				if (d1.month < 1)
				{
					d1.month = 12;
					d1.year--;
				}
				
				if (d1.month == 2)
					d1.day = month[d1.month - 1] + Leap_Year(d1.year);
				else
					d1.day = month[d1.month - 1];
				
				sum_bak = -sum;
			}
		}
		
		if (sum_bak < 0)
		{
			d1.day = d1.day + sum_bak;
		}
		else
		{
			if (d1.month == 2)
				d1.day = month[d1.month - 1] + Leap_Year(d1.year) - sum_bak;
			else
				d1.day = month[d1.month - 1] - sum_bak;
		}
	}
	
	return d1;
}


Bool Judge2(date date_begin,date date_end)
{
	if(date_begin.year < date_end.year)
	{
		return TRUE;
	}
	else if (date_begin.year > date_end.year)
	{
		return FALSE;
	}
	else
	{
		if(date_begin.month < date_end.month)
		{
			return TRUE;
		}
		else if(date_begin.month > date_end.month )
		{
			return FALSE;
		}
		else
		{
			if(date_begin.day < date_end.day)
			{
				return TRUE;
			}
			else if (date_begin.day > date_end.day)
			{
				return FALSE;
			}
			else
			{
				if(date_begin.hour < date_end.hour)
				{
					return TRUE;
				}
				else if(date_begin.hour > date_end.hour)
				{
					return FALSE;
				}
				else
				{
					if(date_begin.minute < date_end.minute)
					{
						return TRUE;
					}
					else if(date_begin.minute > date_end.minute)
					{
						return FALSE;
					}
					else
					{
						if(date_begin.second > date_end.second)
						{
							return FALSE;
						}
						else
						{
							return TRUE;
						}
					}
				}
			}
		}
	}
}


//int main(void)
//{
//	datespan A=Calculate(Date_String_To_Format("20151121142347"),Date_String_To_Format("20141023102312"));
//	return 0;
//}

