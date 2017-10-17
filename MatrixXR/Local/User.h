#ifndef __USER_H__
#define __USER_H__

#include "SystemConfig.h"
#include "wifi_interface.h"
#include "global_config.h"

typedef enum{
	USER_STATE_START = 0 ,
	USER_STATE_IDEL = USER_STATE_START,
    INITIAL_USER ,
#ifdef __WIFI_FUNC__
    SYNC_USER,
    INPUT_XID,
    INPUT_PASSCODE,
    CHECK_CMD_RESULT,
    INPUT_TIMEZONE,
    WELCOME_USER,
#endif
    SELECT_USER,
    EDIT_USER,
    DELETE_USER,
    EDIT_USER_WEIGHT,
    EDIT_USER_AGE,
    EDIT_USER_GENDER,
    USER_SETUP_COMPLETE,
    USER_RESET,
//    VALID_UPDATE_FOUND
}USER_SETTING_STATUS;


typedef enum
{
    USER_EVT_NONE  = 0,
	USER_EVT_PROCESSED,
//    USER_USB_UPDATE_FILE_DETECTED, 
//    USER_UPDATE_CONFIRMED,
    USER_INIT_EVT,
    USER_CHANGE_EVT,
    USER_EDIT_SELECTED_EVT,
    USER_DELETE_SELECTED_EVT,
    USER_WEIGHT_CHANGE_EVT,
    USER_AGE_CHANGE_EVT,
    USER_GENDER_CHANGE_EVT,
    USER_SETTING_COMPLETE,
    USER_SYNC_INPROGRESS_EVT,
    USER_SYNC_TIMEOUT_EVT,
    USER_SYNC_SUCCESS_EVT,
    USER_SYNC_FAIL_EVT,
	INPUT_TIMEZONE_EVT,
	TIMEZONE_COMPLETE,
    INPUT_XID_EVT,
    INPUT_PASSCODE_EVT,
    XID_CHANGE_EVT,
    PASSCODE_CHANGE_EVT,
    LOGIN_INPROCESS_EVT,
    LOGIN_SUCCESS_EVT,
    LOGIN_FAIL_EVT,
    DELETE_SUCCESS_EVT,
    USER_RESET_EVT,
    USER_EVENT_START_WORKOUT,
    USER_EVENT_ENTER_ENGMODE,
    DELETE_FALI_EVT,
    USER_EVENT_TOTAL

    
}USER_EVENT;


void User_Initial_Data(void);
VR_KEY User_Set_VirtualKey(VR_KEY vk);
USER_EVENT User_Process(ReturnEventHandler flag);
char* User_Get_Name(void);
UINT8 User_Get_UserSlot(void);
UINT16 User_Get_Weight(void);
UINT16 User_Get_Age(void);
UINT8 User_Get_Gender(void);
USER_SETTING_STATUS User_Get_SettingFlow(void);
UINT8 *User_Get_Xid(void);
UINT8 *User_Get_Passcode(void);
UINT8 USer_Get_MaxUser(void);
void USER_Set_UserSlot(UINT8 _slot);
void Set_g_SetUser_InfoSync(UINT8 _flag);
UINT8 Get_g_SetUser_InfoSync(void);
UINT8 Get_User_Unit(void);
#endif
