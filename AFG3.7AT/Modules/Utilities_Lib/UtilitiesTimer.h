#ifndef _UtilitiesTimer_H_
#define _UtilitiesTimer_H_

#include "HAL_Lib.h"

#define T_LOOP 0x80

void Timer_Initial_Data(void);
unsigned char Timer_Clear(void);
void Timer_Regist_Functions(void);
void Timer_Set_Flag_100ms_Int(void);
unsigned char Timer_Counter(UCHAR channel, UINT16 w_Period);


#endif
