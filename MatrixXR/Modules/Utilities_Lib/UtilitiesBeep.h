#ifndef _UtilitiesBeep_H_
#define _UtilitiesBeep_H_

#include "HAL_Lib.h"

void Beep_Initial_IO(void);
UCHAR Beep_Get_OK(void);
void Beep_Initial_Data(void);
void Beep_Set_Enable(bool isEnabled);
void Beep_Set_Beeps(UCHAR times, UCHAR onPeriod, UCHAR offPeriod);


#endif