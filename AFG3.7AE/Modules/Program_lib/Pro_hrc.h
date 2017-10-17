#include "Program_Inner.h"
#include "Data_Process.h"


#ifdef __PROGRAM_HRC__
#ifndef __PRO_HRC
#define __PRO_HRC

typedef enum
{
  HC_NONE_INC=0,
  HC_LESS_6_19_INC,
  HC_LESS_20_INC,
  HC_MORE_6_10_INC,
  HC_MORE_11_19_INC,
  HC_MORE_20_24_INC,
  HC_MORE_25_INC,
  
  HC_NONE_RES,
  HC_LESS_6_19_RES,
  HC_LESS_20_RES,
  HC_MORE_6_10_RES,
  HC_MORE_11_19_RES,
  HC_MORE_20_24_RES,
  HC_MORE_25_RES,
  
  HC_NONE_SPD,
  HC_LESS_4_6_SPD,
  HC_LESS_7_11_SPD,
  HC_LESS_12_SPD,
  HC_MORE_4_6_SPD,
  HC_MORE_7_11_SPD,
  HC_MORE_12_15_SPD,
  HC_MORE_16_24_SPD,
  HC_MORE_25_SPD,
}HC_STATUS;
void initHrc(void);
void PHrc_Get_HRTarget(void);
void PHrc_Add_Heart(void);
void PHrc_Dec_Heart(void);
UCHAR PHrc_Get_Heart(UCHAR by_Index);
UINT16 PHrc_Get_Time(UCHAR by_Index);
void PHrc_Add_Time(void);
void PHrc_Dec_Time(void);
void ProHrc_Set_TargetTime(UINT16 w_Data);
void ProHrc_Set_TargetHR(UINT8 by_Dat);
#endif
#else
void initHrc(void){
}

#endif

