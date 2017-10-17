#ifndef _UtilitiesRtc_H_
#define _UtilitiesRtc_H_

#include "HAL_Lib.h"

#define RTCADDNULL   0
#define RTCSUCCESSFULL 1

typedef struct  //datetime_t
{
	UINT8	seconds;
	UINT8	minutes;
	UINT8	hours;
	UINT8	date;
	UINT8	month;
	UINT8	day;
	UINT8	year;
} DateTime_T;

INT8 RTC_Write_Time(DateTime_T* ptTime);
INT8 RTC_Read_Time(DateTime_T * ptTime);
UINT8 Get_Age( UINT16 year, UINT8 month, UINT8 day );

#endif






