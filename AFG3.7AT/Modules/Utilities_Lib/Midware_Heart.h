#ifndef __MIDWARE_HEART_H__
#define __MIDWARE_HEART_H__

#include "JIStypes.h"
#include "HAL_Lib.h"

void Midware_Heart_Initial_Data(void);
void Midware_Heart_1ms_Int(void);
void Midware_Heart_Process(void);
UCHAR Midware_Heart_Get_Blink(void);
UCHAR Midware_Heart_Get_Status(void);
UCHAR Midware_Heart_Get_Heart(void);
void Midware_Heart_Regist_Functions(void);    //zsf
#endif