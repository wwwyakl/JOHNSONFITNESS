#ifndef __ECB_H
#define __ECB_H

#include "HAL_Lib.h" 

#define  RES_VR_TOP	 3365 // 3412 //2120//2500 //4096 12bit  2786  
#define  RES_VR_Bottom	 610  //210//100   

extern  char   C_MAX_RESISTANCE;

void ECB_Initial_Data(void);
void ECB_Dec_Resistance(void);
void ECB_Add_Resistance(void);
void ECB_Set_Resistance(UCHAR by_Data);
void ECB_Dec_Base(void);
void ECB_Add_Base(void);
UCHAR ECB_Get_Base(void);
void ECB_Set_Base(UCHAR by_Data);
void ECB_Set_Zero(void);
void ECB_VR2_ADC_IntHandle(UINT ulADC);
void ECB_Data_Updata(void);
void ECB_Set_Pause(void);
void ECB_Set_Start(void);
void ECB_Cul_ADC(void);
#endif

