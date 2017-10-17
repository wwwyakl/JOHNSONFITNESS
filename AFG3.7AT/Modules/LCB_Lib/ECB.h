#ifndef __ECB_H
#define __ECB_H

#include "HAL_Lib.h" 


void ECB_Set_Terrian_Resistance(signed int by_Dat);
void ECB_Initial_Data(void);
UCHAR ECB_Get_Resistance(void);
UCHAR ECB_Get_Res_Show(void);
void ECB_Dec_Resistance(void);
void ECB_Add_Resistance(void);
void ECB_Set_Resistance(UCHAR by_Data);
void ECB_Dec_Base(void);
void ECB_Add_Base(void);
UCHAR ECB_Get_Base(void);
void ECB_Set_Base(UCHAR by_Data);
signed char ECB_Get_Offset(void);
void ECB_Initial_Offset(void);

void ECB_Set_Zero(void);
UINT16 ECB_Get_VR2_ADC(void);
void ECB_VR2_ADC_IntHandle(UINT ulADC);
void ECB_Data_Updata(void);
char ECB_Get_Ecb_Icon(void);
void ECB_Set_Test(char by_Dir,int by_Target);
UINT16 ECB_Get_Target_ADC(void);
UCHAR ECB_Get_Max_Resistance(void);

void ECB_Set_Pause(void);
void ECB_Set_Start(void);
#endif

