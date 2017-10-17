#ifndef _UtilitiesBor_H_
#define _UtilitiesBor_H_

#include "HAL_Lib.h"
#include "HT16CXX.h"


void BOR_Initial_IO(void);
void BOR_Check_Process(void);
void BOR_Set_Passcode(UINT32 passcode);
UINT32 BOR_Get_Passcode(void);

#endif