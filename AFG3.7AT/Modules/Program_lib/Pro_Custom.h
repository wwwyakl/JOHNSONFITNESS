#include "Program_Inner.h"


#ifdef __PROGRAM_CUSTOM__

#ifndef __PRO_CUSTOM
#define __PRO_CUSTOM

void initCustom(void);
void ProCustom_Save_Info(void);
void ProCustom_Set_SegRemainingDistance(void);

#endif
#else
void initCustom(void){
}

#endif