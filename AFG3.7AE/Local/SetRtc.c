#include "SetRtc.h"
#include "UtilitiesRtc.h"

typedef enum
{
	RTC_SETTING_IDEL = 0,
	SET_RTC_MONTH,
	SET_RTC_DATE,
	SET_RTC_YEAR,
	SET_RTC_HOUR,
	SET_RTC_MINNUTE,
	SET_RTC_AM_PM,
	RTC_SETTING_ALL
}RTC_Setting_enum;

__no_init static union
{
  struct
  {
    UINT32 Key_Up:1;
    UINT32 Key_Dn:1;
    UINT32 Key_Enter:1;
    UINT32 Key_Back:1;
    UINT32 Key_SpeedUp:1;
    UINT32 Key_SpeedDn:1;
    UINT32 Key_InclineUp:1;
    UINT32 Key_InclineDn:1;
    UINT32 Key_ResistanceUp:1;
    UINT32 Key_ResistanceDn:1;
	UINT32 Key_StateChange:1;
    UINT32 Key_Reset:1;
    UINT32 Key_QuickKey:1;
	UINT32 FirstIn:1;
  }Bits;
  UINT32 All_KeyEvents;
}RTC_Evts;
#define AUTO_QUIT_TIME		30000 //unit ms
RTC_Setting_enum RTCSettingState;
DateTime_T  Rtc_Data;
bool TimeisPM;
UINT16	RTCTimerCnt;


static void RTC_1ms_ISR(void)
{
	if(RTCSettingState>RTC_SETTING_IDEL)
	{
		if(RTCTimerCnt++>=AUTO_QUIT_TIME)
			RTCTimerCnt = AUTO_QUIT_TIME;
	}
}

static UCHAR RTC_Is_LeapYear(UINT16 year)
{
    return(((year %4 ==0 && year%100 !=0) || !(year %400)));
}

static UCHAR RTC_Get_NumberOfDay(char by_Month,char by_Year)
{
    const static UCHAR aby_NDM[12]={31,28,31,30,31,30,31,31,30,31,30,31};
    UCHAR by_NumberOfDay;
    if(RTC_Is_LeapYear(2000+by_Year) && by_Month==2 )
    {
        by_NumberOfDay = 29;
    }
    else
    {
        by_NumberOfDay = aby_NDM[by_Month-1];
    }
    return(by_NumberOfDay);
}

void RTCSetModeInitialData(void)
{
	RTCSettingState = RTC_SETTING_IDEL ;
	if(SystemConfig_Get_Char(RTC_USED_FLAG_UCHAR) == 0)
	{
		Rtc_Data.month=	1;
		Rtc_Data.date=	1;
		Rtc_Data.year=	TIME_CURRENT_YEAR;
		Rtc_Data.hours=	1;
		Rtc_Data.minutes=	0;
		Rtc_Data.seconds= 0;
		RTC_Write_Time(&Rtc_Data);
		DateTime_T readback ;
		RTC_Read_Time(&readback);

		if(Rtc_Data.year == readback.year )
		{
			if(SystemConfig_Get_Char(RTC_USED_FLAG_UCHAR) == 0)
			{
				UINT8 isUsed = 1;
				SystemConfigProcess(SVK_TOGGLE_RTC,&isUsed,sizeof(UINT8));
				SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
			}
		}
	}

	RTC_Read_Time(&Rtc_Data);
	RTCTimerCnt = 0 ;
	RTC_Evts.All_KeyEvents =  0;
	if(Rtc_Data.year > 50 || Rtc_Data.year < TIME_CURRENT_YEAR )
		Rtc_Data.year = TIME_CURRENT_YEAR;
	if(Rtc_Data.month > 12 || Rtc_Data.month == 0)
		Rtc_Data.month  = 1;
	UINT8 Date = RTC_Get_NumberOfDay(Rtc_Data.month,Rtc_Data.year);
	if(Rtc_Data.date >= Date)
	{
		Rtc_Data.date = Date;
	}
	if(Rtc_Data.date == 0)
		Rtc_Data.date = 1;
	if(Rtc_Data.hours > 12 || Rtc_Data.hours == 0)
		Rtc_Data.hours = 1;
	if(Rtc_Data.minutes > 59) 
		Rtc_Data.minutes = 1;
	Rtc_Data.seconds = 0;
	
	TimeisPM  = Rtc_Data.hours/12;
	if(TimeisPM)
	{
		if((Rtc_Data.hours = (Rtc_Data.hours + 12)%24) == 0)
		{
			Rtc_Data.hours = 12;
		}
	}
	else
	{
		if(Rtc_Data.hours == 12)
		{
			Rtc_Data.hours = 0;
		}
	}

	}

void RTCSetModeRegistFunction(void)
{
    Hal_Regist_ISR(MS_ISR,RTC_ISR_IDX,RTC_1ms_ISR);
}



void RTC_Set_VirtualKey(UINT16 vk)
{ 
    UINT16 TempKey = vk&0xFF00;
    if((TempKey == FRAME_STAGE_USER)||(TempKey == FRAME_STAGE_PRGSETUP))
    {
        UINT8 keyValue = (UINT8)(vk&0x00FF);
        if(keyValue == VK_RTC)
        {
        		RTCSetModeInitialData();
            	RTC_Evts.Bits.FirstIn= true;
        }
    }
    
    if(TempKey != FRAME_STAGE_RTC)  return;
    
    TempKey = (UINT8)(vk&0x00FF);
    if(TempKey == VK_NULL) return ;
	RTCTimerCnt = 0 ;
    switch(TempKey)
    {
        case VK_RESET:
	    RTC_Evts.Bits.Key_Reset = 1;
        break;
        case VK_SPEEDUP_LONG_PRESS:	
        case VK_INCLINEUP:
        case VK_SPEEDUP:	
        case VK_RESISTANCEUP:
        case VK_UP:
        RTC_Evts.Bits.Key_Up = 1;
        break;
        case VK_INCLINEDOWN:
        case VK_SPEEDDOWN:		
        case VK_DOWN:
        case VK_RESISTANCEDOWN:
        RTC_Evts.Bits.Key_Dn = 1;
        break;
        case VK_ENTER_CHANGEDIS:
        case VK_ENTER:
        RTC_Evts.Bits.Key_Enter = 1;
        break;
        break;
    }
}

RTCSET_EVT RTCSetMode_Process(void)
{
	RTCSET_EVT  ret = NO_SETTING_EVENT;
	if(RTCSettingState>RTC_SETTING_IDEL)
	{
		if(RTC_Evts.Bits.Key_Reset )
		{
			RTC_Write_Time(&Rtc_Data) ;
			RTCSetModeInitialData();
			RTCSettingState = RTC_SETTING_IDEL;
			ret = RTC_QUIT_SETTING_MODE;
		}
	}
	if(RTC_Evts.Bits.FirstIn)
	{
		RTCSettingState =	SET_RTC_MONTH;	
        ret = RTC_MONTH_SET_EVENT;
	}
	if( RTCTimerCnt >= AUTO_QUIT_TIME)
	{
		RTCTimerCnt = 0 ;
		RTCSetModeInitialData();
		RTCSettingState = RTC_SETTING_IDEL;
		ret = RTC_QUIT_SETTING_MODE;
	}
   	switch(RTCSettingState)
   	{
		case RTC_SETTING_IDEL:
		{
			if(RTC_Evts.Bits.FirstIn)
			{
				RTCSettingState =	SET_RTC_MONTH;		
			}
		}
		break;
		case SET_RTC_MONTH:
		{
			if( RTC_Evts.Bits.Key_Up )
			{
				if(Rtc_Data.month<12)	
					++Rtc_Data.month;
				else 
					Rtc_Data.month = 1;
				ret=RTC_MONTH_SET_EVENT;
			}
			if( RTC_Evts.Bits.Key_Dn )
			{
				if(Rtc_Data.month>1)	
					--Rtc_Data.month;
				else 
					Rtc_Data.month = 12;
				ret=RTC_MONTH_SET_EVENT;
			}
			if( RTC_Evts.Bits.Key_Enter )
			{
				RTCSettingState = SET_RTC_DATE;
				ret=RTC_DATE_SET_EVENT;
			}
		}
		break;
		case SET_RTC_DATE:
		{
			if( RTC_Evts.Bits.Key_Up )
			{
				if(Rtc_Data.date< RTC_Get_NumberOfDay(Rtc_Data.month,Rtc_Data.year))
					++Rtc_Data.date;
				else 
					Rtc_Data.date = 1;
				ret=RTC_DATE_SET_EVENT;
			}
			if( RTC_Evts.Bits.Key_Dn )
			{
				if(Rtc_Data.date>1)
					--Rtc_Data.date;
				else 
					Rtc_Data.date = RTC_Get_NumberOfDay(Rtc_Data.month,Rtc_Data.year);
				ret = RTC_DATE_SET_EVENT;
			}
			if( RTC_Evts.Bits.Key_Enter )
			{
				RTCSettingState = SET_RTC_YEAR;
				ret=RTC_YEAR_SET_EVENT;
			}
		}
		break;
		case SET_RTC_YEAR:
		{
			if( RTC_Evts.Bits.Key_Up )
			{
				if(Rtc_Data.year<50) 
					++Rtc_Data.year;
				else 
					Rtc_Data.year = TIME_CURRENT_YEAR;
				
				ret = RTC_YEAR_SET_EVENT;
			}
			if( RTC_Evts.Bits.Key_Dn )
			{
				if(Rtc_Data.year>TIME_CURRENT_YEAR)
					--Rtc_Data.year;
				else 
					Rtc_Data.year = 50;
				
				ret =RTC_YEAR_SET_EVENT;
			}
			if( RTC_Evts.Bits.Key_Enter )
			{
				RTCSettingState = SET_RTC_HOUR;
				ret=RTC_HOUR_SET_EVENT;
			}
		}
		break;
		case SET_RTC_HOUR:
		{
			if( RTC_Evts.Bits.Key_Up )
			{
				if(Rtc_Data.hours<12)
					++Rtc_Data.hours;
				else 
					Rtc_Data.hours = 1;
				ret =RTC_HOUR_SET_EVENT;
			}
			if( RTC_Evts.Bits.Key_Dn )
			{
				if(Rtc_Data.hours>1)
					--Rtc_Data.hours;
				else 
					Rtc_Data.hours = 12;
				ret = RTC_HOUR_SET_EVENT;
			}
			if( RTC_Evts.Bits.Key_Enter )
			{
				RTCSettingState = SET_RTC_MINNUTE;
				ret=RTC_MINUTE_SET_EVENT;
			}
		}
		break;
		case SET_RTC_MINNUTE:
		{
			if( RTC_Evts.Bits.Key_Up )
			{
				if(Rtc_Data.minutes<59)
					++Rtc_Data.minutes;
				else 
					Rtc_Data.minutes = 0;
				
				ret = RTC_MINUTE_SET_EVENT;
			}
			if( RTC_Evts.Bits.Key_Dn )
			{
				if(Rtc_Data.minutes>0)
					--Rtc_Data.minutes;
				else
					Rtc_Data.minutes = 59;
				
				ret = RTC_MINUTE_SET_EVENT;
			}
			if( RTC_Evts.Bits.Key_Enter )
			{
				RTCSettingState = SET_RTC_AM_PM;
				ret=RTC_AM_PM_SET_EVENT;
			}
		}
		break;		
		case SET_RTC_AM_PM:
		{
			if( RTC_Evts.Bits.Key_Up )
			{
				if(TimeisPM)
					TimeisPM = 0;
				else 
					TimeisPM = 1;
				ret = RTC_AM_PM_SET_EVENT;
			}
			if( RTC_Evts.Bits.Key_Dn )
			{
				if(TimeisPM)
					TimeisPM = 0;
				else 
					TimeisPM = 1;
				ret =RTC_AM_PM_SET_EVENT;
			}
			if( RTC_Evts.Bits.Key_Enter )
			{
				RTCSettingState = SET_RTC_MONTH;
				ret=RTC_MONTH_SET_EVENT;
			}
		}
		break;		
		default:break;
	}
	RTC_Evts.All_KeyEvents =  0;
     return ret;
}

DateTime_T* RTCGetSettedTimeData(void)
{
	return(&Rtc_Data);
}

bool RTCGetPMAMInfo(void)
{
	return(TimeisPM);
}


