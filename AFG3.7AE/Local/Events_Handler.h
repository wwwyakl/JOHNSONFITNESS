#ifndef _Events_Handler_h_
#define _Events_Handler_h_
//#include "Engmode.h"
#include "Engmode_Inner.h"
#include "Programs.h"
#include "UtilitiesErrorcode.h"
#include "UtilitiesErp.h"
#include "RF_Lib.h"
//#include "wifi_api.h"
#include "UtilitiesBurntest.h"
#include "UtilitiesProcess.h"
//#include "wifi_interface.h"
#include "User.h"
#include "Auto_Model.h"
#include "USBManager.h"
//#include "Bluetooth_lib.h"
#include "SetRtc.h"
#define	DELAY_N_SECOND_TO_STRAT_PAGE			6


typedef enum 
{
	EVT_ID_RF = 0,
	EVT_ID_USB,
	EVT_ID_WIFI,
	EVT_ID_UTL,
	EVT_ID_TOTAL
}PHER_EVT_IDS;

UINT8 Demomode_Process(void);
void Demo_Events_Handler(void);

void  Auto_Model_Event_Handler(MODEL_EVENT model_event);
void Event_Handler_Init_Data(void);
void Event_Handler_SetNewState(FRAME_STAGE newState);
FRAME_STAGE Event_Handler_GetNewState(void);
void User_Events_Handler(USER_EVENT User_Event);
void UTL_Events_Handler(UTL_EVENT UTL_Event);
void ERP_Events_Handler(ERP_EVENT erp_evts);
void RF_Events_Handler(RF_EVENT rf_evts);
#ifdef  __WIFI_FUNC__
void WIFI_Events_Handler(WIFI_EVENT_HANDLE wifi_evts);
#endif
void Key_Events_Handler(char nKey);
void Program_Events_Handler(PROGRAM_PENDING_EVTS program_evts);
void Events_Blink_Timer_Handler(void);
bool Event_Get_RF_Connected(void);
bool Event_Get_RF_Syncing(void);
void Event_Refresh_Timer_Handler(void);
void Event_Set_Virtual_key(VR_KEY Temp_VK);
void Event_Static_Timer_Handler(void);
void Event_Modal_Timer_Handler(void);
void USB_Events_Handler(USB_EVENTS evt);
void ENG_Events_Handler(ENG_EVENT eng_evts);
void RTC_Event_Handler(RTCSET_EVT eng_evts);
void Event_Clr_ChangeDisplayFlag(void);
//void Bluetooth_Events_Handler(BLUETOOTH_EVENT Bluetooth_Event);
#endif
