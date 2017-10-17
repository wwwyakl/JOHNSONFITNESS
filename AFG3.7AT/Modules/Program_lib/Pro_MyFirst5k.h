#include "Program_Inner.h"
#ifdef __PROGRAM_MYFIRST_5K__

#ifndef __Pro_MyFirst5k__
#define WARMUP 0
#define JOG 1
#define WALK 2
#define COOLDOWN 3

UINT8 ProMyFirst5k_Get_GoodLuckFlag(void);
void ProMyFirst5k_Set_DefaultData(void);
void initMyFirst5k(void);
#endif
#else
UINT8 ProMyFirst5k_Get_GoodLuckFlag(void){
}
void ProMyFirst5k_Set_DefaultData(void){
}
void initMyFirst5k(void){
}

#endif
