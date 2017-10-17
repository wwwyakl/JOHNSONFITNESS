#include "Program_Inner.h"


#ifdef __PROGRAM_CUSTOMHR__

#ifndef __PRO_CUSTOMHR
#define __PRO_CUSTOMHR

void initCustomHr(void);
void ProCustomHr_Set_TotalTime(void);
UINT16 ProCustomHr_Get_Time(void);
UCHAR ProCustomHr_Get_HRTarget(void);
void ProCustomHr_Save_Info(void);

#endif
#else
void initCustomHr(void){
}

#endif