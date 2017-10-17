#ifndef _Events_Handler_h_
#define _Events_Handler_h_

#include "stdio.h"
#include "Engmode_Inner.h"
#include "Programs.h"
#include "RF_Lib.h"
#include "wifi_api.h"
#include "wifi_interface.h"
#include "User.h"
#include "Auto_Model.h"
#include "Global_Config.h"
#include "USBManager.h"
#include "SystemConfig.h"
#include "page.h"
#include "Show.h"

#include "Midware_Heart.h"
#include "UtilitiesErp.h"
#include "UtilitiesBurntest.h"
#include "UtilitiesErrorcode.h"
#include "UtilitiesLubeBelt.h"
#include "UtilitiesKeyboard.h"

#include "Auto_Model.h"
#include "HAL_Lib.h"


#define	DELAY_N_SECOND_TO_STRAT_PAGE			6


typedef enum
{
	TYPE_START =0,
	TYPE_ALL_INIT = TYPE_START,
	TYPE_RST_INIT,
	TYPE_ALL
}INIT_TYPE;


typedef enum 
{
	EVT_ID_RF = 0,
	EVT_ID_USB,
	EVT_ID_WIFI,
	EVT_ID_UTL,
	EVT_ID_TOTAL
}PHER_EVT_IDS;


VR_KEY Event_Set_Virtual_key(VR_KEY Temp_VK);
VR_KEY Event_VK_Converter(char key);
ReturnEventHandler Event_UTL_BurnTest_Process(BURNTEST_EVENT evt,ReturnEventHandler flag);
ReturnEventHandler Event_UTL_ERP_Process(ERP_EVENT evt,ReturnEventHandler flag);
ReturnEventHandler Event_UTL_Error_Code_Process(ERR_EVENT evt,ReturnEventHandler flag);
ReturnEventHandler Event_UTL_HR_Blink_Process(UINT8 NAData,ReturnEventHandler flag);
ReturnEventHandler Events_RF_Handler(RF_EVENT evt,ReturnEventHandler flag);
ReturnEventHandler Program_Events_Handler(PROGRAM_PENDING_EVTS program_evts,ReturnEventHandler flag);
ReturnEventHandler Events_ENG_Handler(ENG_EVENT eng_evts,ReturnEventHandler flag);
ReturnEventHandler Events_USB_Handler(USB_EVENTS evt,ReturnEventHandler flag);
ReturnEventHandler WIFI_Events_Handler(WIFI_EVENT_HANDLE wifi_evts,ReturnEventHandler flag);
ReturnEventHandler User_Events_Handler(USER_EVENT evt,ReturnEventHandler flag);
ReturnEventHandler Event_UTL_BlueBelt_Process(LUBEBELT_EVENT evt,ReturnEventHandler flag);



UINT8 Demomode_Process(void);
void Demo_Events_Handler(void);

void  Auto_Model_Event_Handler(MODEL_EVENT model_event);
void Event_Handler_Init_Data(void);
void Event_Handler_SetNewState(FRAME_STAGE newState);
void Key_Events_Handler(char nKey);
bool Event_Get_RF_Connected(void);
bool Event_Get_RF_Syncing(void);
void Event_Refresh_Timer_Handler(void);
bool Event_Get_DisFlag_AllOn(void);
bool Event_Get_DisFlag_AllOff(void);


#endif
