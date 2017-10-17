#ifndef _UtilitiesKeyInputMethod_H_
#define _UtilitiesKeyInputMethod_H_

//#include <wifi_interface.h>
#include "SystemConfig.h"
#include "SystemConfig.h"
#include "HAL_Lib.h"


typedef enum
{
    Setting_In_Change_User=1,
    Setting_In_Set_Program,
    Setting_In_Set_Speed,    
    Setting_In_Set_Resistance,
    Setting_In_Set_Incline,
    Setting_In_Set_Time,
    Setting_In_Set_Level,
    Setting_In_Set_Watts,
    Setting_In_Set_Target_HR,
    Setting_In_Set_Weight,
    Setting_In_Set_Age,
    Setting_In_Set_XID,
    Setting_In_Set_Password,
    Setting_In_Set_TimeZone,
    Setting_In_Set_Gender,
    Setting_In_Work_TM,
    Setting_In_Work_EP,
    ALL_Setting_Pages
}QUICK_KEY_FUNCTION;

typedef enum
{
	KEYPAD_NO_EVENT = 1,
	KEYPAD_5S_EVENT,
	ALL_KEYPAD_EVENT
}KEYPAD_EVENT;

//extern unsigned char  Xid[XIDMAXLENGTH+1];
//extern unsigned char  PassCode[5];
extern unsigned char Xid_counter;
extern unsigned char PassCode_counter;


void Number_Set_Keypad(char by_Dat,QUICK_KEY_FUNCTION Current_FUNCTION);
int Number_Get_Keypad(void);
bool KEYPAD_Get_5S(void);
bool Number_Get_Setting_Flag(void);
void Number_Clear_Keypad();
void Number_Clear_Xid(void);
void Number_Clear_PassCode(void);
bool Keypad_XP_STATUS(void);
void Keypad_Delete_Number(QUICK_KEY_FUNCTION Current_FUNCTION);
#endif
