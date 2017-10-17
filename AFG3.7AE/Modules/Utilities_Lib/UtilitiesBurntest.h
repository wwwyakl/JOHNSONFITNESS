#ifndef _UtilitiesBurntest_H_
#define _UtilitiesBurntest_H_

#include "HAL_Lib.h"

#define AGINGOFF  1
#define AGINGON  0

typedef enum
{
	E_NO_EVENT = 0,
	E_AGINGOFF,
	E_AGINGON,
	E_TOTAL
}BURNTEST_EVENT;

void BurnTest_Initial_IO(void);
void BurnTest_Initial_Data(void);
void BurnTest_Clear_MEM(void);
void BurnTest_Save_MEM_FIFO(bool by_Dat);
BURNTEST_EVENT BurnTest_Process(UINT16 vk);
#endif