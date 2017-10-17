#ifndef __RPM_BIKE_H
#define __RPM_BIKE_H
#include "HAL_Lib.h"


void RPM_Initial_Data(void);
void RPM_Calculate_Counter_Int(void);
void RPM_Reload_Int(void);
UCHAR RPM_Decoder(void);
UCHAR RPM_Get_RPM(void);

void RPM_Add(void);
void RPM_DEC(void);
UCHAR RPM_Get_AutoRPM(void);
void RPM_AUTO_Check(UCHAR by_Mode);
void RPM_AutoRPM_Clear(void);
unsigned char RPM_Get_AUTO_Check(void);
void RPM_Set_Auto(unsigned int set);

#endif
