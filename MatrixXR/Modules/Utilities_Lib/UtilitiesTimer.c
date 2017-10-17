#include "UtilitiesTimer.h"

#define T_CHANNEL_MAX 32

#define FLAG_VALUE 0XFFFF

typedef struct
{
	UCHAR TimerFlag;
	UCHAR TimerStart;
	UINT16 aw_Timer_Data;
}TimerData;

__no_init static UINT16 w_Timer_Flag;
__no_init static TimerData aw_Timer_Data[T_CHANNEL_MAX];


static void Timer_Set_Flag_100ms_Int(void)
{
	static unsigned char i = 0;
	if(i >= 100)
	{
		i = 0;
		w_Timer_Flag = FLAG_VALUE;
	}
	else
	{
		i++;
	}
}

void Timer_Initial_Data(void)
{
	UCHAR by_Loop;
	w_Timer_Flag=0;
	for(by_Loop=0;by_Loop < T_CHANNEL_MAX;by_Loop++)
	{
		aw_Timer_Data[by_Loop].aw_Timer_Data=0;
		aw_Timer_Data[by_Loop].TimerFlag = 0;
		aw_Timer_Data[by_Loop].TimerStart = 0;
	}
}



UINT8 Timer_Malloc_Channels(bool isAutoClear)
{
	UCHAR by_Loop = 0;
	for( by_Loop = 0; by_Loop < T_CHANNEL_MAX; by_Loop++)
	{
		if( aw_Timer_Data[by_Loop].TimerFlag == 0 )
		{
			aw_Timer_Data[by_Loop].aw_Timer_Data = 0;
			aw_Timer_Data[by_Loop].TimerFlag = 1;
			aw_Timer_Data[by_Loop].TimerStart= 1;
			if( isAutoClear ) by_Loop |= 0x80;
			return by_Loop ;
		}
	}
	return by_Loop ;	
}
void Timer_Release_Timer(UINT8 chl)
{
	EFM_ASSERT((chl&0x7F)<T_CHANNEL_MAX);
	
	aw_Timer_Data[(chl&0x7F)].aw_Timer_Data = 0;
	aw_Timer_Data[(chl&0x7F)].TimerFlag = 0;
	aw_Timer_Data[(chl&0x7F)].TimerStart= 0;
}

void Timer_Set_Timer_Start(UINT8 chl)
{
	aw_Timer_Data[(chl&0x7F)].TimerStart= 1;
}

void Timer_Set_Timer_Paused(UINT8 chl)
{
	aw_Timer_Data[(chl&0x7F)].TimerStart= 0;
}

void Timer_Set_Timer(UINT8 chl,UINT16 value)
{
	aw_Timer_Data[(chl&0x7F)].aw_Timer_Data= value;
}

UINT16 Timer_Get_Timer(UINT8 chl)
{
	UINT16 ret = aw_Timer_Data[(chl&0x7F)].aw_Timer_Data ;
	return ret ;
}

unsigned char Timer_Counter(UCHAR channel, UINT16 x100ms)
{
	UCHAR b_Out = 0;
	UCHAR by_Channel;
	UINT16 w_Mask;
	
	by_Channel = channel & 0x7f;
	aw_Timer_Data[by_Channel].TimerFlag = 1;
	w_Mask = 0x01 << by_Channel;
	if((w_Timer_Flag & w_Mask)==0) return(0);
	if(aw_Timer_Data[by_Channel].TimerStart == 0 ) return(0);
	w_Timer_Flag &= ~w_Mask;
	
	if(aw_Timer_Data[by_Channel].aw_Timer_Data <= x100ms) ++aw_Timer_Data[by_Channel].aw_Timer_Data;
	if(aw_Timer_Data[by_Channel].aw_Timer_Data == x100ms) b_Out=1;
	if(aw_Timer_Data[by_Channel].aw_Timer_Data >= x100ms)
	{	/*******for auto clear "T_LOOP"**************/
		if(channel & 0x80) 
            aw_Timer_Data[by_Channel].aw_Timer_Data=0;
	}
	return(b_Out);
}

void Timer_Regist_Functions(void)
{
	HAL_SystemTicker_Alloc_ISR(Timer_Set_Flag_100ms_Int);
}
