#ifndef _UtilitiesErp_H_
#define _UtilitiesErp_H_

#include "UtilitiesErrorcode.h"
#include "UtilitiesKeyboard.h"
#include "UtilitiesTimer.h"
#include "SystemConfig.h"
#include "UtilitiesBor.h"
#include "LCB.h"

typedef enum
{
	ERP_NO_EVENT = 0x0,
	
	NUM_0,
	NUM_1 ,
	NUM_2 ,
	NUM_3 ,
	NUM_4 ,
	NUM_5 ,
	NUM_6 ,
	NUM_7,
	NUM_8,
	NUM_9,	
	ENTER_DEMO,
	REFRESH_DEMO,
    ERP_QUIT_DEMO,
    Enter_ERP,
    ERP_PROCESSED,
	ERP_EVENT_TOTAL
}ERP_EVENT;

void ERP_Quick_In(void);
void ERP_Initial_Data(void);
void Erp_Regist_Functions(void);
void ERP_Set_time(unsigned int ERP_TIME);
ERP_EVENT ERP_Process(ReturnEventHandler flag);
VR_KEY UTL_ERP_Set_VK(VR_KEY vk);

#endif