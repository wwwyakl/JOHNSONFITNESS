#ifndef _UtilitiesBurntest_H_
#define _UtilitiesBurntest_H_

#include "HAL_Lib.h"
#include "UtilitiesErp.h"



#define AGINGOFF  1
#define AGINGON  0

typedef enum
{
	E_NO_EVENT = 0,
	E_PROCESSED,
	E_AGINGOFF,
	E_AGINGON,
	E_TOTAL
}BURNTEST_EVENT;

void BurnTest_Initial_IO(void);
void BurnTest_Initial_Data(void);
void BurnTest_Clear_MEM(void);
void BurnTest_Save_MEM_FIFO(bool by_Dat);
VR_KEY UTL_BurnTest_Set_VK(VR_KEY vk);
BURNTEST_EVENT BurnTest_Process(ReturnEventHandler flag);
#endif
