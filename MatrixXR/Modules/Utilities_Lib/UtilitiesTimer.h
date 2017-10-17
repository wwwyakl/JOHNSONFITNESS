#ifndef _UtilitiesTimer_H_
#define _UtilitiesTimer_H_

#include "HAL_Lib.h"

#define T_LOOP 0x80


void Timer_Initial_Data(void);
UINT8 Timer_Malloc_Channels(bool isAutoClear);
void Timer_Release_Timer(UINT8 chl);
void Timer_Set_Timer_Start(UINT8 chl);
void Timer_Set_Timer_Paused(UINT8 chl);
void Timer_Set_Timer(UINT8 chl,UINT16 value);
UINT16 Timer_Get_Timer(UINT8 chl);
unsigned char Timer_Counter(UCHAR channel, UINT16 x100ms);
void Timer_Regist_Functions(void);
#endif
