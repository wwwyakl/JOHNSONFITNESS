#ifndef _UtilitiesLubeBelt_H_
#define _UtilitiesLubeBelt_H_

#include "HAL_Lib.h"


typedef enum
{
	LUBEBELT_EVENT_NONE = 0,
	LUBEBELT_EVENT_PROCESSED,
	LUBEBELT_EVENT_NOTICE,
	LUBEBELT_EVENT_CLEARD,
	LUBEBELT_EVENT_ALL
}LUBEBELT_EVENT;

void LubeBelt_Initial_Data(void);
LUBEBELT_EVENT LubeBelt_Process(ReturnEventHandler flag);
VR_KEY UTL_LubeBelt_Set_VK(VR_KEY vk);



#endif
