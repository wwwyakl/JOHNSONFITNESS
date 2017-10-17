#ifndef _UtilitiesProcess_H_
#define _UtilitiesProcess_H_

#include "UtilitiesKeyInputMethod.h"
#include "UtilitiesErrorcode.h"
#include "UtilitiesBurntest.h"
#include "Midware_Heart.h"
#include "UtilitiesErp.h"
#include "Programs.h"

typedef struct
{
	ERR_EVENT err_event;
	ERP_EVENT erp_event;
	BURNTEST_EVENT burn_event;
	bool heart_event;
	bool keypad_event;
}UTL_EVENT;


UTL_EVENT UTL_Process(void);
void UTL_Set_Virtual_Key(UINT16 vk);


#endif