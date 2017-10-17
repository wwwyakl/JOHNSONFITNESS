#ifndef _SETRTC_H_
#define _SETRTC_H_

#include "SystemConfig.h"
#include "UtilitiesRtc.h"

typedef enum
{
	NO_SETTING_EVENT = 0,
	RTC_MONTH_SET_EVENT,
	RTC_DATE_SET_EVENT,
	RTC_YEAR_SET_EVENT,
	RTC_HOUR_SET_EVENT,
	RTC_MINUTE_SET_EVENT,
	RTC_AM_PM_SET_EVENT,
	RTC_QUIT_SETTING_MODE,
	RTC_SETTIME_ALLEVENT
}RTCSET_EVT;


void RTCSetModeInitialData(void);
void RTCSetModeRegistFunction(void);
void RTC_Set_VirtualKey(UINT16 vk);
RTCSET_EVT RTCSetMode_Process(void);
DateTime_T* RTCGetSettedTimeData(void);
bool RTCGetPMAMInfo(void);

#endif






