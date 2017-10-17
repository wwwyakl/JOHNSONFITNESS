#include "Events_Handler.h"
#include "Programs.h"
#include "SystemConfig.h"
#include "page.h"
#include "Show.h"
#include "Main.h"
#include "UtilitiesKeyboard.h"
#include "stdio.h"
#include "Auto_Model.h"
#include "Pro_Custom.h"
#include "Bluetooth_lib.h"


void Demo_Set_VirtualKey(UINT16 vk);

//------------------------------Private Marcos----------------------------------------
#define EVT_SPRINTF_BUFFER_SIZE  65
#define DEMO_REFRESH_TIME 2
//------------------------------Private variables--------------------------------------

#define SUPPORTED_MACHINE_TYPE_TOTAL	14

__no_init struct
{
    unsigned Connected:1;
    unsigned Syncing:1;
    unsigned Refresh_DemoDis:1;
	unsigned PageChanged:1;
	unsigned DisChangeForHR:1;
    UINT8 Demo_RefreshConter;
}Event_Flag;


UINT8 Demo_Event_Quit;

__no_init Win_Items g_BackMiddleWnd;//0,8
__no_init VR_PAGE  g_BackPage;
__no_init Matrix_Type g_martix_resume_type;
__no_init Matrix_Type g_martix_modal_type;
__no_init  FRAME_STAGE Frame_Stage;
__no_init static FRAME_STAGE Frame_OldStage;
__no_init unsigned char NameSprintBuffer[EVT_SPRINTF_BUFFER_SIZE];
__no_init UINT8 EXTERNAL_EVENTS_BUFFER[EVT_ID_TOTAL];
__no_init static UCHAR Timer_500msHigh_Level;
extern PAGES const pages[PAGE_TOTAL];

//------------------------------Private function protypes--------------------------------
static char Event_VK_Converter(char key);
static void Event_HideAllWnds(UINT8 ishidden);
static void Event_DisableFlashingAllWnds(void);

static void Event_Set_Wnd_Para(Win_Items* p,STRINGID stringid,UINT8 blinkcount,UINT8 activetime,UINT8 ishidden,UINT8 blinkDelayCount, void (*pf)(void*) )
{
    EFM_ASSERT(stringid<=STRINGIDEND && p!=NULL );
    
    p->_string_id = stringid;
    p->_blinkCount = blinkcount;
    p->_activetime = activetime;
    p->_isHidden = ishidden;
    p->_blinkDelayCount = blinkDelayCount;
    p->pfShow = pf;

    if(pf != NULL && p->_isHidden != 1)
			p->pfShow(p);
}
/*@brief: this function is used to hide all active windows */
static void Event_HideAllWnds(UINT8 ishidden)
{
    g_wndLeftTopTextBox._isHidden = ishidden;
    g_wndLeftTopNumerical._isHidden = ishidden;
    g_wndRightTopTextBox._isHidden = ishidden;
    g_wndRightTopNumerical._isHidden = ishidden;
    g_wndLeftBottomTextBox._isHidden = ishidden;
    g_wndLeftBottomNumerical._isHidden = ishidden;
    g_wndRightBottomTextBox._isHidden = ishidden;
    g_wndRightBottomNumerical._isHidden = ishidden;
    g_wndBottomTextBox._isHidden = ishidden;
    g_wndMiddleMatrixBox._isHidden = ishidden;
    g_wndMiddleNumerical._isHidden = ishidden;
}

/*@brief: this function is used to disable all flashing elements */
static void Event_DisableFlashingAllWnds(void)
{
    g_wndLeftTopNumerical._blinkCount = 0;
    g_wndLeftTopNumerical._blinkDelayCount= 0;
    g_wndRightTopNumerical._blinkCount = 0;
    g_wndRightTopNumerical._blinkDelayCount = 0;
    g_wndLeftBottomNumerical._blinkCount = 0;
    g_wndLeftBottomNumerical._blinkDelayCount = 0;
    g_wndRightBottomNumerical._blinkCount = 0;
    g_wndRightBottomNumerical._blinkDelayCount = 0;
}


/*@brief: this function is used to convert phykey into virtual key */
static char Event_VK_Converter(char phyKey)
{
    char keyret = VK_NULL;
		
    if(Frame_Stage == FRAME_STAGE_ERP_COUNT) phyKey = KB_NONE;
		
    if(phyKey  == KB_NONE || phyKey ==KB_NA || phyKey ==0xAA)
        return(VK_NULL);
    
    switch(phyKey)
    {
        case KB_RTC:keyret = VK_RTC;break;
        case KB_START:keyret = VK_START;   break;
        case KB_SPEED_LONGPRESS:
        case KB_QUICKSPEED_LONGPRESS:
        keyret = VK_SPEEDUP_LONG_PRESS;
        break;
        case KB_RELEASED:
        keyret = VK_SPEEDUP_RElEASE;
        break;
        case KB_STOP:  		keyret = VK_PAUSE; break;
        case KB_Hold_9:		keyret = VK_CLEAR_BOOT; break;
        case KB_RESET: 		keyret = VK_RESET; break;
        case KB_ENG: 		    keyret = VK_ENG; break;
#ifdef RF_MODULE_EXIST_
        case KB_RF_SYNC: 	    keyret = VK_RF_SYNC; break;
#endif
#ifdef __WIFI_FUNC__
        case KB_WIFI_AP: 	    keyret = VK_WIFI_AP; break;
#endif
        case KB_BACK:         keyret = VK_BACK; break;
        case KB_NUMBER0:      keyret = VK_NUM0; break;
        case KB_NUMBER1:      keyret = VK_NUM1; break;
        case KB_NUMBER2:      keyret = VK_NUM2; break;
        case KB_NUMBER3:      keyret = VK_NUM3; break;
        case KB_NUMBER4:      keyret = VK_NUM4; break;
        case KB_NUMBER5:      keyret = VK_NUM5; break;
        case KB_NUMBER6:      keyret = VK_NUM6; break;
        case KB_NUMBER7:      keyret = VK_NUM7; break;
        case KB_NUMBER8:      keyret = VK_NUM8; break;
        case KB_NUMBER9:      keyret = VK_NUM9; break;
        
        case KB_INCLINE_0:    keyret = VK_QK_INCLINE_0; break;
        case KB_INCLINE_1:    keyret = VK_QK_INCLINE_10; break;
        case KB_INCLINE_2:    keyret = VK_QK_INCLINE_20; break;
        case KB_INCLINE_3:    keyret = VK_QK_INCLINE_30; break;
        case KB_INCLINE_4:    keyret = VK_QK_INCLINE_40; break;
        case KB_INCLINE_5:    keyret = VK_QK_INCLINE_50; break;
        case KB_INCLINE_6:    keyret = VK_QK_INCLINE_60; break;
        case KB_INCLINE_7:    keyret = VK_QK_INCLINE_70; break;
        case KB_INCLINE_8:    keyret = VK_QK_INCLINE_80; break;
        case KB_INCLINE_9:    keyret = VK_QK_INCLINE_90; break;
        case KB_INCLINE_10:   keyret = VK_QK_INCLINE_100; break;
        case KB_INCLINE_12:   keyret = VK_QK_INCLINE_120; break;
        //case KB_INCLINE_15:   keyret = VK_QK_INCLINE_150; break;
        
        case KB_SPEED_1:      keyret = VK_QK_SPEED1; break;
        case KB_SPEED_2:      keyret = VK_QK_SPEED2; break;
        case KB_SPEED_3:      keyret = VK_QK_SPEED3; break;
        case KB_SPEED_4:      keyret = VK_QK_SPEED4; break;
        case KB_SPEED_5:      keyret = VK_QK_SPEED5; break;
        case KB_SPEED_6:      keyret = VK_QK_SPEED6; break;
        case KB_SPEED_7:      keyret = VK_QK_SPEED7; break;
        case KB_SPEED_8:      keyret = VK_QK_SPEED8; break;
        case KB_SPEED_9:      keyret = VK_QK_SPEED9; break;
        case KB_SPEED_10:     keyret = VK_QK_SPEED10; break;
        case KB_SPEED_11:     keyret = VK_QK_SPEED11; break;
        case KB_SPEED_12:     keyret = VK_QK_SPEED12; break;
        
        case KB_LEVEL_1:      keyret = VK_QK_RESISTACE_1; break;
        case KB_LEVEL_2:      keyret = VK_QK_RESISTACE_2; break;
        case KB_LEVEL_3:      keyret = VK_QK_RESISTACE_3; break;
        case KB_LEVEL_4:      keyret = VK_QK_RESISTACE_4; break;
        case KB_LEVEL_5:      keyret = VK_QK_RESISTACE_5; break;
        case KB_LEVEL_6:      keyret = VK_QK_RESISTACE_6; break;
        case KB_LEVEL_7:      keyret = VK_QK_RESISTACE_7; break;
        case KB_LEVEL_8:      keyret = VK_QK_RESISTACE_8; break;
        case KB_LEVEL_9:      keyret = VK_QK_RESISTACE_9; break;
        case KB_LEVEL_10:     keyret = VK_QK_RESISTACE_10; break;
        
        case KB_MANUAL:       keyret = VR_MANUAL; break;
        case KB_DISTANCE:     keyret = VR_DISTANCE; break;
        case KB_WEIGHT_LOSS:     keyret = VR_WEIGHT_LOSS; break;
        case KB_CALORIES:     keyret = VR_CALORIES; break;
        case KB_INTERVALS:   keyret = VR_INTERVALS; break;
        case KB_HRT_RATE:     keyret = VR_HRT_RATE; break;
        case KB_MY_FIRST5K:   keyret = VR_MY_FISRT5K; break;
        case KB_CUSTOM:       keyret = VR_CUSTOM; break;
        case KB_CUSTOM_HRT:   keyret = VR_CUSTOM_HRT; break;
        case KB_SPRINTS:      keyret = VR_SPRINTS; break;
        case KB_SELECT_PROGRAM: keyret = VR_SELECT_PROGRAM; break;
        
        case KB_ENTER:
        case KB_CHANGE_DISPLAY:keyret = VK_ENTER_CHANGEDIS;break;
        case KB_SPEED_FAST:
        case KB_QUICK_SPD_FAST:
        if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8) == TREADMILL)
            keyret = VK_SPEEDUP;	
        else
            keyret = VK_RESISTANCEUP;	
        break;
        case KB_SPEED_SLOW:
        case KB_QUICK_SPD_SLOW:
        if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8) == TREADMILL)
            keyret = VK_SPEEDDOWN;
        else
            keyret = VK_RESISTANCEDOWN;	     
        break;
//        case KB_LEVEL_UP:
//            keyret = VK_RESISTANCEUP;	
//        break;
//        case KB_LEVEL_DOWN:
//            keyret = VK_RESISTANCEDOWN;	     
//        break;
        case KB_INCLINE_UP:
        case KB_QUICK_INC_UP:
        if(SystemConfig_Get_Char(DRIVE_TYPE_UINT8) == ECB_JIS || SystemConfig_Get_Char(DRIVE_TYPE_UINT8) == BIKE_0C_JIS)
            keyret = VK_NULL;	
        else
            keyret = VK_INCLINEUP;	
        break;
        case KB_INCLINE_DOWN:
        case KB_QUICK_INC_DOWN:
        if(SystemConfig_Get_Char(DRIVE_TYPE_UINT8) == ECB_JIS || SystemConfig_Get_Char(DRIVE_TYPE_UINT8) == BIKE_0C_JIS)
            keyret = VK_NULL;	
        else
            keyret = VK_INCLINEDOWN;
        break;
        case KB_RESISTANCE_UP:
        keyret = VK_RESISTANCEUP;
        break;
        case KB_RESISTANCE_DOWN:
        keyret = VK_RESISTANCEDOWN;
        break;
        case KB_ARROW_UP:
        case KB_SWITCH_UP:
        keyret = VK_UP;	
        break;
        case KB_ARROW_DOWN:
        case KB_SWITCH_DOWN:
        keyret = VK_DOWN;
        break;
        case KB_FAN:
        keyret = VK_FAN;        
        break;			
        case KB_RF:
		keyret = VK_RF;
        break;			
        case KB_WIFI:
		keyret = VK_WIFI;
		break;	
		case KB_SCAN:
		keyret = VK_SCAN;	
		break;	
        case KB_BLUETOOTH:
        keyret = VK_BLUETOOTH;
        break;
        case KB_BT_PAIR:
		   keyret = VK_BT_CHANGMODE ;
        break;
        default: break;
    }
    return(keyret);
}

void Event_Get_Page_matrix(void)
{
	switch(SystemConfig_Get_Char(MACHINE_TYPE_UINT8))
	{
		case TREADMILL:	
		{
			nCurrentPage = PAGE_WORK_TM1;
		}
		break;
		case ASCENT:		
		{
			nCurrentPage = PAGE_WORK_AS1;
//			martix_type = Show_Incline;
		}
		break; 
		case BIKE:
		case ELLIPTICAL:	
		{
			nCurrentPage = PAGE_WORK_BK1;
//			martix_type = Show_Resistance;
		}
		break; 
		default:break;
	}
    martix_type = (Matrix_Type)Program_Get_DefaultDotMatrix();
}
/***************************************************************************//**
* @brief
*   Event_Handler_Init_Data 
*
* @note
*   This function is called by Main_Initial_Data
* @param[in]: none
* @param[out]:none
******************************************************************************/
void Event_Handler_Init_Data(void)
{
    if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8) == TREADMILL)
        martix_type = Show_Speed;
    else
        martix_type = Show_Resistance; 
    g_martix_modal_type = Show_None;
    
#ifdef FUNCTION_AUTO_MODEL     
    if(SystemConfig_Get_Char(IS_MACHINE_CONFIGED_UCHAR)!=0xAA)
        Frame_Stage = FRAME_STAGE_MODEL ;
    else
#endif 
        Frame_Stage = FRAME_STAGE_USER;
    Frame_OldStage = FRAME_STAGE_USER;
    memset(&Event_Flag,0,sizeof(Event_Flag));
    memset(NameSprintBuffer, '\0', EVT_SPRINTF_BUFFER_SIZE);
	memset(&g_BackMiddleWnd,0,sizeof(Win_Items));
    Timer_500msHigh_Level =0;
}

/***************************************************************************//**
* @brief
*   Event_Handler_SetNewState(FRAME_STAGE newState)
*
* @note
*   This function is called by Main_Initial_Data
* @param[in]: none
* @param[out]:none
******************************************************************************/
void Event_Handler_SetNewState(FRAME_STAGE newState)
{
    Frame_Stage = newState;
}
FRAME_STAGE Event_Handler_GetNewState(void)
{
    return Frame_Stage;
}


void Event_Set_Virtual_key(VR_KEY Temp_VK)
{
    User_Set_VirtualKey((UINT16)Temp_VK|Frame_Stage);
    USBManager_Set_VirtualKey((UINT16)Temp_VK|Frame_Stage);
    Engmode_Set_VirtualKey((UINT16)Temp_VK|Frame_Stage);
    Program_Set_VirtualKey((UINT16)Temp_VK|Frame_Stage);
    Model_Set_VirtualKey((UINT16)Temp_VK|Frame_Stage);
    Demo_Set_VirtualKey((UINT16)Temp_VK|Frame_Stage);
    RF_Set_VirtualKey((UINT16)Temp_VK|Frame_Stage);
    UTL_Set_Virtual_Key((UINT16)Temp_VK|Frame_Stage);
#ifdef __WIFI_FUNC__
    Wifi_Set_VirtualKey((UINT16)Temp_VK|Frame_Stage);
#endif
    Bluetooth_Set_VirtualKey((UINT16)Temp_VK|Frame_Stage);
	RTC_Set_VirtualKey((UINT16)Temp_VK|Frame_Stage);
    UTL_Fan_Set_Virtualkey((UINT16)Temp_VK|Frame_Stage);
}
/***************************************************************************//**
* @brief
*   Key_Events_Handler 
*
* @note
*   This function is used to route all key events to  certain running thread & assitant thread(s)...
* @param[in]: nkey
* @param[out]:none
******************************************************************************/
void Key_Events_Handler(char nKey)
{
    UINT8 Temp_VK = Event_VK_Converter(nKey);
	
	if( Temp_VK != VK_NULL ) 
    {
        ERP_Clear_time();
    }
    if(Temp_VK==VK_CLEAR_BOOT)
    {
        Display_Set_All(0);
        UINT8 boot =0xFF;
        SystemConfigProcess(SVK_TOGGLE_FIRSTBOOT,&boot,sizeof(boot));
        SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
        BOR_Set_Passcode(0);
        g_wndBottomTextBox._string_id = STR_RETOOTING_ID;
        g_wndBottomTextBox.pfShow = ShowCommonTextBox;//ShowInitialTitle;
        g_wndBottomTextBox.pfShow(&g_wndBottomTextBox);
        Beep_Set_Beeps(6,10,10);
        while(Beep_Get_OK()==false)
        {
            Display_Updata_Data(true);
        }
        HAL_Reset_System(); 
    }
    
    extern UINT16 appProId;    //zsf
    if((Temp_VK == VK_PAUSE) && (appProId == 30))
    {
        Temp_VK = VK_RESET;
    }
//	if(Temp_VK==VK_RF)
//	{
//		wifiSetFirmwareUpdate(DEPLOYMENT_QA,SOFTWARE_TYPE_NON_MCU);
//		//wifiTestRSCU(SOFTWARE_TYPE_NON_MCU,"2487428144c10679ba9e4d0853893652");
//		//wifiTestRSCU(SOFTWARE_TYPE_NON_MCU,"a8db6c0ae647a52ef14e61418c6d4715");
//		//wifiTestRSCU(SOFTWARE_TYPE_NON_MCU,"2487428144c10679ba9e4d0853893652");
//	}
    Event_Set_Virtual_key((VR_KEY)Temp_VK);
}

/***************************************************************************//**
* @brief
*   Auto_Model_Event_Handler 
*
* @note
*   This function is used to handle auto model selection messages...
* @param[in]:MODEL_EVENT model_event
* @param[out]:none
******************************************************************************/
void  Auto_Model_Event_Handler(MODEL_EVENT model_event)
{
	if(model_event == MODEL_EVENT_NONE) return;
	
	if(model_event != MODEL_EVENT_NONE && model_event < ALL_MODEL_EVENT)
	{
		Display_Set_All(0);
		Event_HideAllWnds(1);
	}
	switch(model_event)
	{
		case MODEL_FIRST_DISPLAY:
		case MODEL_EVENT_LANGUAGE_CHANGE:
		{
			STRINGID tempid = (STRINGID)(STR_ENGLISH_ID+Model_Get_Language_Index());
			Event_Set_Wnd_Para(&g_wndBottomTextBox,tempid,0,0,0,0,ShowCommonTextBox);
			Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_SELECT_LANGUAGE_ID,0,0,0,0,ShowCommonTextBox);
		}
		break;
		case MODEL_EVENT_MODEL_CHANGED:
		{
			STRINGID tempid = (STRINGID)(Model_Get_Set_Slot()+STR_TF30_ID);
			
			Event_Set_Wnd_Para(&g_wndBottomTextBox,tempid,0,0,0,0,ShowCommonTextBox);
			Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_SELECT_MODEL_ID,0,0,0,0,ShowCommonTextBox);		
		}
		break;
		case MODEL_EVENT_UNIT_CHANGE:
		{
			STRINGID tempid = (STRINGID)(STR_METRIC_ID+Model_Get_Set_Slot());
			Event_Set_Wnd_Para(&g_wndBottomTextBox,tempid,0,0,0,0,ShowCommonTextBox);
			Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_SELECT_UNIT_ID,0,0,0,0,ShowCommonTextBox);		
		}
		break;
		case MODEL_EVENT_COMPLETE:
		{
			Main_Init_Model_Data();
	        Frame_Stage = FRAME_STAGE_USER;
	        nCurrentPage = PAGE_SELECTUSER;
	        Event_Set_Virtual_key(VK_INITIAL);
		}
		break;
		case MODEL_EVENT_UNSUPPORTED_MCB_FOUND:
		default:break;
	}
}
/***************************************************************************//**
* @brief
*   User_Events_Handler 
*
* @note
*   
* @param[in]:USER_EVENT User_Event
* @param[out]:none
******************************************************************************/
void User_Events_Handler(USER_EVENT User_Event)
{
  if(Frame_Stage == FRAME_STAGE_SAFFKEYOFF) return;
  if(User_Event != USER_EVT_NONE && User_Event < USER_EVENT_TOTAL)
  {
    //		Display_Set_All(0);
    //        LED_Set_All(0);
    Event_HideAllWnds(1);
  }
  switch(User_Event)
  {
  case USER_CHANGE_EVT:
    {
      if(User_Get_Name() == NULL)
      {
        STRINGID tempid = STR_GUEST_ID;
        Event_Set_Wnd_Para(&g_wndBottomTextBox,tempid,0,0,0,0,ShowCommonTextBox);		
      }
      else
      {
        Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_NULL_ID,0,0,0,0,ShowUsrName);		
        //g_wndBottomTextBox._isHidden = 0 ;
        //memcpy(NameSprintBuffer, User_Get_Name(), EVT_SPRINTF_BUFFER_SIZE);
        // DisplayString(0,3,0,NameSprintBuffer);  
      }
    }
    break;
  case USER_EDIT_SELECTED_EVT:
    {
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_EDIT_ID,0,0,0,0,ShowCommonTextBox);				
    }
    break;
  case USER_DELETE_SELECTED_EVT:
    {
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_DELETE_ID,0,0,0,0,ShowCommonTextBox);						
    }
    break;
  case USER_WEIGHT_CHANGE_EVT:
    {
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SELECTWEIGHT_ID,0xAA,0,0,2,ShowWeightData);						
      
      //			STRINGID tempid;
      //	        if(SystemConfig_Get_Char(UNIT_FLAG_UCHAR)==STANDARD)
      //	        {
      //	             tempid = STR_LBS_ID;
      //	        }
      //	        else
      //	        {
      //	            tempid = STR_KGS_ID;
      //	        }
      //			Event_Set_Wnd_Para(&g_wndRightTopTextBox,tempid,0,0,0,0,ShowWeightUnit);						
      //			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SELECTWEIGHT_ID,0,0,0,0,ShowCommonTextBox);						
    }
    break;
  case USER_AGE_CHANGE_EVT:
    {
      Event_Set_Wnd_Para(&g_wndRightTopNumerical,STR_NULL_ID,0xAA,0,0,2,ShowAgeData);						
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SELECTAGE_ID,0,0,0,0,ShowCommonTextBox);						
    }
    break;
  case USER_GENDER_CHANGE_EVT:
    {
      Event_Set_Wnd_Para(&g_wndRightTopNumerical,STR_NULL_ID,0,0,0,0,ShowSetGenderData);						
      Event_Set_Wnd_Para(&g_wndRightTopTextBox,STR_NULL_ID,0,0,0,0,ShowMaleTitle);						
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SELECTGENDER_ID,0,0,0,0,ShowCommonTextBox);						
    }
    break;
  case USER_SYNC_INPROGRESS_EVT:
    {
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_UPDATE_USER_ID,0,0,0,0,ShowCommonTextBox);						
    }
    break; 
  case USER_SYNC_TIMEOUT_EVT:
    {
      //push overlapped windows 1st 
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SELECTUSER_ID,0,0,0,0,ShowCommonTextBox);
      Page_Push_OverlappedWindow(&g_wndBottomTextBox, &g_wndBottomTextBox);
      
      //then display the Modal window
      //			Event_Set_Wnd_Para(&g_wndRFIcon,0,0,0,0,0,ShowRFICON);
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_TIMEOUT_ID,0,3,0,0,ShowCommonTextBox);
    }
    break;
  case USER_SYNC_FAIL_EVT:
    {
      //push overlapped windows 1st 
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SELECTUSER_ID,0,0,0,0,ShowCommonTextBox);
      Page_Push_OverlappedWindow(&g_wndBottomTextBox, &g_wndBottomTextBox);
      
      //then display the Modal window
      //			Event_Set_Wnd_Para(&g_wndRFIcon,0,0,0,0,0,ShowRFICON);
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_WIFI_FAIL_ID,0,3,0,0,ShowCommonTextBox);
    }
    break;
  case INPUT_XID_EVT:
    {
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_ENTER_XID_ID,0,0,0,0,ShowCommonTextBox);
    }
    break;
  case XID_CHANGE_EVT:
    {
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_ENTER_XID_ID,0,0,0,0,ShowXID);
    }
    break;
  case PASSCODE_CHANGE_EVT:
    {
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_PASSCOD_ID,0,0,0,0,ShowPasscode );
    }
    break;
  case INPUT_PASSCODE_EVT:
    {
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_PASSCOD_ID,0,0,0,0,ShowCommonTextBox);
    }
    break;
  case LOGIN_INPROCESS_EVT:
    {
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_LOGIN_ID,0,0,0,0,ShowCommonTextBox);
    }
    break;
  case LOGIN_SUCCESS_EVT:
    {
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_WELCOME_ID,0,0,0,0,ShowCommonTextBox);
      Beep_Set_Beeps(6,10,10);
    }
    break;
  case USER_SETTING_COMPLETE:
    {
      Frame_Stage = FRAME_STAGE_PRGSETUP;
      nCurrentPage = PAGE_PROGRAM;
      Program_Set_VirtualKey((UINT16)VK_INITIAL|Frame_Stage);
    }
  case USER_SETUP_ABORTED_EVT:
    break;
  case LOGIN_FAIL_EVT:
    {
      //push overlapped windows 1st 
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SELECTUSER_ID,0,0,0,0,ShowCommonTextBox);
      Page_Push_OverlappedWindow(&g_wndBottomTextBox, &g_wndBottomTextBox);
      
      //then display the Modal window
      //			Event_Set_Wnd_Para(&g_wndRFIcon,0,0,0,0,0,ShowRFICON);
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_INVALIDPW_ID,0,3,0,0,ShowCommonTextBox);
    }
    break;
    
  case USER_RESET_EVT:
    {
      Beep_Set_Beeps(4,6,6);
      Page_Reset_All_ModalWnds();		
      nCurrentPage = PAGE_SELECTUSER;
      ShowPage(nCurrentPage,1);
    }
    break;
  
  case DELETE_SUCCESS_EVT:
  default:
    break;
  }    
  
  //    g_wndWiFiIcon.pfShow     = ShowWifiIcon;
  //    g_wndWiFiIcon.pfShow(&g_wndWiFiIcon);
  
  //    g_wndRFIcon.pfShow       = ShowRFICON;
  //    g_wndRFIcon.pfShow(&g_wndRFIcon); 
}
void Bluetooth_Events_Handler(BLUETOOTH_EVENT Bluetooth_Event)
{
	if(Frame_Stage == FRAME_STAGE_SAFFKEYOFF) return;
	switch(Bluetooth_Event)
	{
		case BLUETOOTH_CHANGE_USER:
		{

		}
		break;
		case BLUETOOTH_CHANGE_PROGRAM:
		{

		}
		break;
		case BLUETOOTH_MODE_MASTER:
		{
			Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_SELECTUSER_ID,0,0,0,0,ShowCommonTextBox);
			Page_Push_OverlappedWindow(&g_wndLEDTextBox, &g_wndLEDTextBox);
			Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_HEART_RATE_MONITOR_ONLY_ID,0,11,0,0,ShowCommonTextBox);
		}
		break;
		case BLUETOOTH_MODE_SLAVE:
		{
			Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_SELECTUSER_ID,0,0,0,0,ShowCommonTextBox);
			Page_Push_OverlappedWindow(&g_wndLEDTextBox, &g_wndLEDTextBox);
			Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_TABLET_ENABLED_ID,0,3,0,0,ShowCommonTextBox);
		}
		break;
                case BLUETOOTH_UPDATE_START:
                {
                    Event_Set_Wnd_Para(&g_wndBottomTextBox,NULL,0,0,0,0,ShowBTUpdatePercent);
		}
                break;
                case BLUETOOTH_UPDATE_END:
                {
                    Event_Set_Wnd_Para(&g_wndBottomTextBox,NULL,0,0,0,0,ShowBTUpdatePercent);
                    Display_Set_All(0);
            
                    BOR_Set_Passcode(0);
                    Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_RETOOTING_ID,0,0,0,0,ShowCommonTextBox);	

                    Beep_Set_Beeps(6,10,10);
                    while(Beep_Get_OK()==false)
                    {
                        Display_Updata_Data(true);
//                LED_Updata(true);
                    }
                    HAL_Reset_System(); 
		}
                break;
        case BLUETOOTH_UPDATE_FAIL:
		{
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_USB_BTUPDATEED_FAIL_ID,0,0,0,0,ShowBottomTextBox);
		}
		break;
		default:
		break;
	}
}

/***************************************************************************//**
* @brief
*   Program_Events_Handler 
*
* @note
*   
* @param[in]:PROGRAM_PENDING_EVTS program_evts
* @param[out]:none
******************************************************************************/
void Program_Events_Handler(PROGRAM_PENDING_EVTS program_evts)
{
  static bool b_TIME_OR_DISTANCE_FLAG;
  STRINGID stringid = STR_MANUAL_ID;
  if(Frame_Stage == FRAME_STAGE_SAFFKEYOFF) return;
  //if(Frame_Stage != FRAME_STAGE_PRGSETUP && Frame_Stage != FRAME_STAGE_PRGRUN) return;
  //  if(Frame_Stage == FRAME_STAGE_PRGSETUP)
  if(program_evts.evt != PROGRAM_NONE_EVENT && program_evts.evt < PROGRAM_TOTAL_EVENT &&\
    program_evts.evt!= WORKOUT_DATA_REFRESH_EVENT && program_evts.evt!= PROGRAM_PAUSE_EVENT)
  {
    //    	Display_Set_All(0);
    Event_HideAllWnds(1);
  }
  switch(program_evts.evt)
  {
  case BACK_END_EVENT:
    {
      nCurrentPage = PAGE_SELECTUSER;
      Frame_Stage = FRAME_STAGE_USER;
      User_Set_VirtualKey((UINT16)VK_BACK|Frame_Stage);
    }
    break;
  case PROGRAM_INITIAL_EVENT:
    {
      ShowPage(nCurrentPage,1);
      Program_Select_ID(PS_MANUAL);
    }
  case PROGRAM_CHANGE_EVENT:
    {
      ShowPage(nCurrentPage,1);
      Show_ProgramLED(Program_Get_ProgramID());
      LED_Light(LED5_TIME, LED_OFF);    //ZSF
      if( Program_Get_ProgramID()==PS_WEIGHT_LOSS)
        stringid = STR_WEIGHT_LOSS_ID;
      else if( Program_Get_ProgramID()==PS_INTERVALS )
        stringid = STR_INTERVALS_ID;
      else if( Program_Get_ProgramID()==PS_DISTANCE_GOAL )
        stringid = STR_DISTANCE_ID;
      else if( Program_Get_ProgramID()==PS_FATBURN )
        stringid = STR_FATBURN_ID;
      else if( Program_Get_ProgramID()==PS_THR_ZONE )
        stringid = STR_THRZONE_ID;
      else if( Program_Get_ProgramID()==PS_HILL_CLIMB )
        stringid = STR_HILLCLIMB_ID;
      else if( Program_Get_ProgramID()==PS_SPRINT8 )
        stringid = STR_SPRINT8_ID;
      else if( Program_Get_ProgramID()==PS_CALORIES_GOAL )
        stringid = STR_CALORIES_ID;
      else if( Program_Get_ProgramID()==PS_MYFIRST5K )
        stringid = STR_MYFIRST5K_ID;
      else if( Program_Get_ProgramID()==PS_CUSTOM_DISTANCE || Program_Get_ProgramID()==PS_CUSTOM_TIME || Program_Get_ProgramID()==PS_CUSTOM)
      {
        b_TIME_OR_DISTANCE_FLAG = true;
        stringid = STR_CUSTOM_ID;
      }
      else if( Program_Get_ProgramID()==PS_CUSTOM_HR )
        stringid = STR_CUSTOMHR_ID;
      
      //			if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8) == TREADMILL)
      //                martix_type = Show_Speed;
      //            else
      //                martix_type = Show_Resistance; 
      martix_type = (Matrix_Type)Program_Get_DefaultDotMatrix(); 	
      //            if(stringid == STR_CUSTOM_TIME_ID || stringid == STR_CUSTOM_DISTANCE_ID)
      //                Event_Set_Wnd_Para(&g_wndLEDTextBox,stringid,0xAA,0,0,2,ShowCommonTextBox);
      //            else
      
      Event_Set_Wnd_Para(&g_wndBottomTextBox,stringid,0,0,0,0,ShowCommonTextBox);
      Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,STR_NULL_ID,0,0,0,0,ShowMatrixData);        
    }
    break;
  case SUBPROGRAM_CHANGE_EVENT:
    {
      if( Program_Get_ProgramID()==PS_CUSTOM_TIME )
        stringid = STR_CUSTOM_TIME_ID;
      else if( Program_Get_ProgramID()==PS_CUSTOM_DISTANCE )
        stringid = STR_CUSTOM_DISTANCE_ID;
      
      if(b_TIME_OR_DISTANCE_FLAG)
      {
        b_TIME_OR_DISTANCE_FLAG = false;
        Page_Reset_Modal_Window(&g_wndLEDTextBox);
        Event_Set_Wnd_Para(&g_wndLEDTextBox,stringid,0xAA,0,0,2,ShowCommonTextBox);
        Page_Push_OverlappedWindow(&g_wndLEDTextBox,&g_wndLEDTextBox);
        Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_TIMEORDISTANCE_BASED_ID,0,12,0,0,ShowCommonTextBox);
      }
      else
      {
        Event_Set_Wnd_Para(&g_wndLEDTextBox,stringid,0xAA,0,0,2,ShowCommonTextBox);
      }
      Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,STR_NULL_ID,0,0,0,0,ShowMatrixData);
    }
    break;
  case CUSTOM_SEGMENT_CHANGE_EVENT:
    {
      Page_Reset_Modal_Window(&g_wndLEDTextBox);
      if( Program_Get_ProgramID()==PS_CUSTOM_TIME )
        Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_SELECTTIME_ID,0XAA,0,0,2,ShowSetTime);
      else if( Program_Get_ProgramID()==PS_CUSTOM_DISTANCE )
        Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_CUSTOM_DISTANCE_ID,0XAA,0,0,2,ShowSetDistance);
      Page_Push_OverlappedWindow(&g_wndLEDTextBox,&g_wndLEDTextBox);
      
      Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_CUSTOM_SEGMENT_ID,0,5,0,0,ShowCustomSegment);
      Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,STR_NULL_ID,0,0,0,0,ShowMatrixData);
    }
    break;
  case TIME_CHANGE_EVENT:
    {
#ifdef TM
      Show_LEDLight(LED5_TIME,LED_ON);
#else
      Show_LEDLight(LED10_TIME_EP,LED_ON);
#endif
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SELECTTIME_ID,0,0,0,0,ShowCommonTextBox);
      Event_Set_Wnd_Para(&g_wndLED1TextBox,STR_NULL_ID,0XAA,0,0,2,ShowCountTimeData);
      Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,STR_NULL_ID,0,0,0,0,ShowMatrixData);
    }
    break;
  case WALKSPEED_CHANGE_EVENT:
    {
      Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_WALKSPEED_ID,0XAA,0,0,2,ShowSetSpeed);
      Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,STR_NULL_ID,0,0,0,0,ShowMatrixData);
    }
    break;
  case JOGSPEED_CHANGE_EVENT:
    {
      Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_JOGSPEED_ID,0XAA,0,0,2,ShowSetSpeed);
      Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,STR_NULL_ID,0,0,0,0,ShowMatrixData);
    }
    break;
  case MYFIRST5K_1WEEKMISSEDREPEATLASTWEEK:
    {
      Page_Reset_Modal_Window(&g_wndLEDTextBox);
      Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_NULL_ID,0,0,0,0,showWeekWorkout);
      Page_Push_OverlappedWindow(&g_wndLEDTextBox,&g_wndLEDTextBox);
      Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_MYFIRST5K_1WEEKMISSED_ID,0,12,0,0,ShowCommonTextBox);
      Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,STR_NULL_ID,0,0,0,0,ShowMatrixData);
    }
    break;
  case MYFIRST5K_2WEEKMISSEDSTARTOVER:
    {
      Page_Reset_Modal_Window(&g_wndLEDTextBox);
      Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_NULL_ID,0,0,0,0,showWeekWorkout);
      Page_Push_OverlappedWindow(&g_wndLEDTextBox,&g_wndLEDTextBox);
      Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_MYFIRST5K_2WEEKMISSED_ID,0,10,0,0,ShowCommonTextBox);
      Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,STR_NULL_ID,0,0,0,0,ShowMatrixData);
    }
    break;
    
    //        case TOTAL_TIME_CHANGE_EVENT:
    //        {
    ////            g_wndMiddleNumerical._z = 2;
    //			Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_CUSTOM_TOTALTIME_ID,0XAA,0,0,2,ShowSetTotalTime);
    ////			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SELECTTIME_ID,0,0,0,0,ShowCommonTextBox);
    //			Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,0,0,0,0,0,ShowMatrixData);
    //        }
    break;
  case DISTANCE_CHANGE_EVENT:
    {
      if( Program_Get_ProgramID() == PS_CUSTOM_DISTANCE )
      {
        Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_CUSTOM_DISTANCE_ID,0XAA,0,0,2,ShowSetDistance);
        Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,STR_NULL_ID,0,0,0,0,ShowMatrixData);
      }
      else if( Program_Get_ProgramID() == PS_DISTANCE_GOAL )
      {
        //                Event_Set_Wnd_Para(&g_wndLEDTextBox,(STRINGID)(STR_SELECTDISTANCE_ID+Program_Get_DistanceLable()),0XAA,0,0,2,ShowSelectDistance);
        //                Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,STR_NULL_ID,0,0,0,0,ShowMatrixData);
        Event_Set_Wnd_Para(&g_wndBottomTextBox,(STRINGID)(STR_SELECTDISTANCE_ID+Program_Get_DistanceID()-1),0XAA,0,0,2,ShowSelectDistance);
        Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,STR_NULL_ID,0,0,0,0,ShowMatrixData);                
      }
    }
    break;
    //        case DISTANCE_MAX_EVENT:
    //        {
    //            Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_DISTANCE_MAX_ID,0,0,0,0,ShowCommonTextBox);
    //            Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,0,0,0,0,0,ShowMatrixData);
    //        }
    //        break;
  case HEART_CHANGE_EVENT:
    {
      Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_HEART_RATE_ID,0XAA,0,0,2,ShowTargetHeartData);
      Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,STR_NULL_ID,0,0,0,0,ShowMatrixData);
    }
    break;
  case WATTS_CHANGE_EVENT:
    {
      Event_Set_Wnd_Para(&g_wndLeftTopNumerical,STR_NULL_ID,0XAA,0,0,2,ShowWattsData);
      Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_SELECTTARGETWATTS_ID,0,0,0,0,ShowCommonTextBox);
      Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,STR_NULL_ID,0,0,0,0,ShowMatrixData);           
    }
    break;
  case CALORIES_CHANGE_EVENT:
    {
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SELECTCALORIES_ID,0XAA,0,0,2,ShowSetCalories);
      Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,STR_NULL_ID,0,0,0,0,ShowMatrixData);
    }
    break;		
  case SET_GOAL_PRE_EVENT:
    {
      
    }
    break;
  case STEPS_CHANGE_EVENT:
    {
      
    }
    break;
  case CLIMB_CHANGE_EVENT:
    {
      
    }
    break;
  case LEVEL_CHANGE_EVENT:
    {
      g_wndRightBottomNumerical._z = 0;		
      g_wndLED1TextBox._blinkCount = 0;
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SELECTLEVEL_ID,0,0,0,0,ShowCommonTextBox);
      Event_Set_Wnd_Para(&g_wndLED2TextBox,STR_NULL_ID,0XAA,0,0,2,ShowSetLevel);
      Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,STR_NULL_ID,0,0,0,0,ShowMatrixData);
    }
    break;
  case WEEK_WORKOUT_CHANGE_EVENT:
    {
      Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_NULL_ID,0,0,0,0,showWeekWorkout);
      Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,STR_NULL_ID,0,0,0,0,ShowMatrixData);
    }
    break;
  case SPEED_CHANGE_EVENT:
    { 
      martix_type = Show_Speed;
      g_wndLED1TextBox._blinkCount = 0;
      Show_LEDLight(LED7_SPEED,LED_ON);
      Show_LEDLight(LED8_INCLINE,LED_OFF);
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SELECTSPEED_ID,0,0,0,0,ShowCommonTextBox);
      if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
        Event_Set_Wnd_Para(&g_wndLED2TextBox,STR_NULL_ID,0XAA,0,0,2,ShowSpeedData);
      else
        Event_Set_Wnd_Para(&g_wndLED2TextBox,STR_NULL_ID,0XAA,0,0,2,ShowRpmData);
      Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,STR_NULL_ID,0,0,0,0,ShowMatrixData);
    } 
    break;
  case INCLINE_CHANGE_EVENT:
    { 
      martix_type = Show_Incline;
      Show_LEDLight(LED7_SPEED,LED_OFF);
      Show_LEDLight(LED8_INCLINE,LED_ON);
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SELECTINCLINE_ID,0,0,0,0,ShowCommonTextBox);
      Event_Set_Wnd_Para(&g_wndLED2TextBox,STR_NULL_ID,0XAA,0,0,2,ShowInclineData);
      Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,STR_NULL_ID,0,0,0,0,ShowMatrixData);
    }
    break;
  case RESISTANCE_CHANGE_EVENT:
    {  
      martix_type = Show_Resistance;
      g_wndLED1TextBox._blinkCount = 0;  //zsf
      g_wndRightBottomNumerical._z = 0;
      Show_LEDLight(LED1_RPM_EP,LED_OFF);
      Show_LEDLight(LED8_RESISTANCE_EP,LED_ON);
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SELECTRESISTANCE_ID,0/*0XAA*/,0,0,0/*2*/,ShowSetResistance);
      Event_Set_Wnd_Para(&g_wndLED2TextBox,STR_NULL_ID,0XAA,0,0,2,ShowResistanceData);
      Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,STR_NULL_ID,0,0,0,0,ShowMatrixData);
    }
    break; 
  case WORKOUT_DATA_REFRESH_EVENT:
    { 
      //extern UINT8 appStart;    //zsf
      if(program_evts.WorkOutEvts.Workout_Complete_Evt==1)
      { 
        //if (appStart == 0)    //zsf
            Bluetooth_Set_VirtualKey((UINT16)VK_WORKOUT_COMPLETE);
        Event_Get_Page_matrix();
        Beep_Set_Beeps(6,10,10);
        ShowPage(nCurrentPage,1);
//WWW        Event_Set_Wnd_Para(&g_wndTimeIcon,STR_NULL_ID,0,0,1,0,ShowElapsedTimeData);
        Event_Set_Wnd_Para(&g_wndTimeIcon,STR_NULL_ID,0,0,1,0,ShowTimeData);//WWW
        if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
          Event_Set_Wnd_Para(&g_wndDistanceIcon,STR_NULL_ID,0,0,1,0,ShowSpeedData);
        else
		{
          Event_Set_Wnd_Para(&g_wndLED2TextBox,STR_NULL_ID,0XAA,0,0,2,ShowRpmData);
		  Event_Set_Wnd_Para(&g_wndDistanceIcon,STR_NULL_ID,0,0,1,0,ShowLcdDistanceData);
		}
        Event_Set_Wnd_Para(&g_wndCaloriesIcon,STR_NULL_ID,0,0,1,0,ShowDistanceData);
        //                Event_Set_Wnd_Para(&g_wndLED4TextBox,STR_NULL_ID,0,0,1,0,ShowCaloriesData);

        if(Program_Get_ProgramID() == PS_MYFIRST5K)
        {
          if(program_evts.WorkOutEvts.Workout_MyFirst5k_NextWorkoutIn2Days)
            Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_NEXT_IN_2_DAYS_ID,0,0,0,0,ShowCommonTextBox);
          else if(program_evts.WorkOutEvts.Workout_MyFirst5k_NextWorkoutIn3Days)
            Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_NEXT_IN_3_DAYS_ID,0,0,0,0,ShowCommonTextBox);
          else if(program_evts.WorkOutEvts.Workout_MyFirst5k_CongratulationsGoodLuckWithYour5k)
            Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_CONGRATULATIONS_ID,0,0,0,0,ShowCommonTextBox);
        }
        else    
          Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_WORKOUTFINISH_ID,0,0,0,0,ShowCommonTextBox);
      }
      if(program_evts.WorkOutEvts.Workout_ChangeDisplay==1)
      {    
        switch(nCurrentPage)
        {
        case PAGE_WORK_TM1:
          {
            Show_LEDLight(LED5_TIME,LED_OFF);
            Show_LEDLight(LED7_SPEED,LED_OFF);
            Show_LEDLight(LED9_DISTANCE,LED_OFF);
            Show_LEDLight(LED6_CALORIES,LED_ON);
            Show_LEDLight(LED8_INCLINE,LED_ON);
            Show_LEDLight(LED10_HRT,LED_ON);
            martix_type = Show_Incline;
            nCurrentPage = PAGE_WORK_TM2;       
          }
          break;
        case PAGE_WORK_TM2:
          {
            Show_LEDLight(LED5_TIME,LED_ON);
            Show_LEDLight(LED7_SPEED,LED_ON);
            Show_LEDLight(LED9_DISTANCE,LED_ON);
            Show_LEDLight(LED6_CALORIES,LED_OFF);
            Show_LEDLight(LED8_INCLINE,LED_OFF);
            Show_LEDLight(LED10_HRT,LED_OFF);
            martix_type = Show_Speed;
            nCurrentPage = PAGE_WORK_TM1;       
          }
          break;
        case PAGE_WORK_AS1:
          {
            martix_type = Show_Resistance;
            nCurrentPage = PAGE_WORK_AS2;       
          }
          break;					
        case PAGE_WORK_AS2:
          { 
            martix_type =  Show_Incline;
            nCurrentPage = PAGE_WORK_AS1;       
          }
          break;					
        case PAGE_WORK_BK1:
          {
            Show_LEDLight(LED10_TIME_EP,LED_OFF);
            Show_LEDLight(LED1_RPM_EP,LED_OFF);
            Show_LEDLight(LED2_DISTANCE_EP,LED_OFF);
            Show_LEDLight(LED6_CALORIES_EP,LED_ON);
            Show_LEDLight(LED8_RESISTANCE_EP,LED_ON);
            Show_LEDLight(LED3_HRT_EP,LED_ON);
            martix_type = Show_Resistance;
            nCurrentPage = PAGE_WORK_BK2;      
          }
          break;					
        case PAGE_WORK_BK2:
          {
            Show_LEDLight(LED10_TIME_EP,LED_ON);
            Show_LEDLight(LED1_RPM_EP,LED_ON);
            Show_LEDLight(LED2_DISTANCE_EP,LED_ON);
            Show_LEDLight(LED6_CALORIES_EP,LED_OFF);
            Show_LEDLight(LED8_RESISTANCE_EP,LED_OFF);
            Show_LEDLight(LED3_HRT_EP,LED_OFF);
            martix_type =  Show_Resistance;
            nCurrentPage = PAGE_WORK_BK1;      
          }
          break;
        default:break;
        }
        Event_Flag.PageChanged = 1;
        Event_Flag.DisChangeForHR = 1 ;
        if(g_BackMiddleWnd._string_id !=0 )
        {
          memcpy(&g_wndMiddleBottomTextBox,&g_BackMiddleWnd,sizeof(Win_Items));
          g_wndMiddleBottomTextBox.pfShow(&g_wndMiddleBottomTextBox);
        }
        //Page_Reset_All_ModalWnds();
        //				g_BackPage = nCurrentPage;
        g_martix_modal_type = Show_None;
        g_martix_resume_type = martix_type;

        Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_NULL_ID,0,0,0,0,ShowCommonTextBox);  //zsf
        ShowPage(nCurrentPage,1);
        
//        Event_Set_Wnd_Para(&g_wndTimeIcon,STR_NULL_ID,0,0,1,0,ShowElapsedTimeData);
//        Event_Set_Wnd_Para(&g_wndDistanceIcon,STR_NULL_ID,0,0,1,0,ShowSpeedData);
//        Event_Set_Wnd_Para(&g_wndCaloriesIcon,STR_NULL_ID,0,0,1,0,ShowDistanceData);
//        Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_WORKOUTFINISH_ID,0,0,0,0,ShowCommonTextBox);
        //Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_NULL_ID,0,0,1,0,ShowCommonTextBox);
        
        Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,STR_NULL_ID,0xAA,0,0,0,ShowMatrixData);
        if(g_BackMiddleWnd._string_id !=0 )
        {
          memcpy(&g_wndMiddleBottomTextBox,&g_BackMiddleWnd,sizeof(Win_Items));
          g_wndMiddleBottomTextBox.pfShow(&g_wndMiddleBottomTextBox);
        }
      }
      if(program_evts.WorkOutEvts.Workout_Speed_Evt==1 || program_evts.WorkOutEvts.Workout_QuickKeySpd_Evt==1)
      {
        if(Program_Get_ProgramID() == PS_DISTANCE_GOAL)
        {
          Data_Set_ElapsedOldTime(Data_Get_Time(ELAPSED_TIME));
          Data_Set_remainingOldDistancex1000(Data_Get_Distance(REMAINING_DISTANCE));
        }
        else if(Program_Get_ProgramID() == PS_CALORIES_GOAL)
        {
          Data_Set_ElapsedOldTime(Data_Get_Time(ELAPSED_TIME));
          Data_Set_remainingOld_Calories(Data_Get_Calories(REMAINING_CALORIES));
        }
        else if(Program_Get_ProgramID() == PS_CUSTOM_DISTANCE)
        {
          Data_Set_ElapsedOldTime(Data_Get_Time(ELAPSED_TIME));
          ProCustom_Set_SegRemainingDistance();
        }
        UINT8 Recover_typeflag = 0;
        if(martix_type != Show_Speed )
        {
          g_martix_modal_type =martix_type; 
          martix_type = Show_Speed;
        }
        if(g_martix_modal_type != Show_None)
        {
          Recover_typeflag = 0xAA;
        }
        
        UINT8 modalflag = 0;
        if( g_wndLeftBottomTextBox._activetime ==0 )
        {
          if(nCurrentPage == PAGE_WORK_TM2 || nCurrentPage == PAGE_WORK_AS2)
          {
            Event_Set_Wnd_Para(&g_wndLED2TextBox,STR_NULL_ID,0,0,0,0,ShowInclineData);		
            Page_Push_OverlappedWindow(&g_wndLED2TextBox, &g_wndLED2TextBox);
            modalflag = 1 ;
          }
        }
        else
        {
          if(g_wndLED2TextBox.pfShow == ShowInclineData)
          {
            if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
              Event_Set_Wnd_Para(&g_wndDistanceIcon,STR_NULL_ID,0,0,1,0,ShowSpeedData);
            else
              Event_Set_Wnd_Para(&g_wndLED2TextBox,STR_NULL_ID,0XAA,0,0,2,ShowRpmData);	
            Page_Push_OverlappedWindow(&g_wndLED2TextBox, &g_wndLED2TextBox); 
            modalflag = 1 ;
          }
        }
        Event_Set_Wnd_Para(&g_wndLED2TextBox,STR_NULL_ID,8,(modalflag==1?6:0),Recover_typeflag,2,ShowSpeedData);	
        g_wndLEDTextBox._isHidden = 1; 
#ifdef TM
        Show_LEDLight(LED7_SPEED,LED_ON);
        Show_LEDLight(LED8_INCLINE,LED_OFF);
#else
        Show_LEDLight(LED1_RPM_EP,LED_ON);
        Show_LEDLight(LED8_RESISTANCE_EP,LED_OFF);
#endif
        //                Beep_Set_Beeps(1,3,1);
      }
      else if((program_evts.WorkOutEvts.Workout_Resistance_Evt==1) || (program_evts.WorkOutEvts.Workout_QuickKeyRes_Evt==1))
      {
        UINT8 Recover_typeflag = 0;
        if(martix_type != Show_Resistance)
        {
          g_martix_modal_type =martix_type; 
          martix_type = Show_Resistance;
        }
        if(g_martix_modal_type != Show_None)
        {
          Recover_typeflag = 0xAA;
        }	
        UINT8 modalflag = 0;
        if( g_wndLeftBottomTextBox._activetime ==0 )
        {
#ifdef TM
          if((nCurrentPage == PAGE_WORK_AS2)||(nCurrentPage == PAGE_WORK_BK2))
          {
            Event_Set_Wnd_Para(&g_wndLED2TextBox,STR_NULL_ID,0,0,0,0,ShowInclineData);		
            Page_Push_OverlappedWindow(&g_wndLED2TextBox, &g_wndLED2TextBox);
            modalflag = 1 ;
          }
        }
        else
        {
          if(g_wndLED2TextBox.pfShow == ShowInclineData)
          {
            Event_Set_Wnd_Para(&g_wndLED2TextBox,STR_NULL_ID,0,0,0,0,ShowResistanceData);		
            Page_Push_OverlappedWindow(&g_wndLED2TextBox, &g_wndLED2TextBox); 
            modalflag = 1 ;
          }
        }
#else
          if((nCurrentPage == PAGE_WORK_AS1)||(nCurrentPage == PAGE_WORK_BK1))
          {
            Event_Set_Wnd_Para(&g_wndLED2TextBox,STR_NULL_ID,0,0,0,0,ShowRpmData);		
            Page_Push_OverlappedWindow(&g_wndLED2TextBox, &g_wndLED2TextBox);
            modalflag = 1 ;
          }
        }
        else
        {
          if(g_wndLED2TextBox.pfShow == ShowRpmData)
          {
            Event_Set_Wnd_Para(&g_wndLED2TextBox,STR_NULL_ID,0,0,0,0,ShowResistanceData);		
            Page_Push_OverlappedWindow(&g_wndLED2TextBox, &g_wndLED2TextBox); 
            modalflag = 1 ;
          }
        }
#endif
        Event_Set_Wnd_Para(&g_wndLED2TextBox,STR_NULL_ID,8,(modalflag==1?6:0),Recover_typeflag,2,ShowResistanceData);	
        g_wndLEDTextBox._isHidden = 1; 
      }	
      else if(program_evts.WorkOutEvts.Workout_Incline_Evt==1)
      {
        if(Program_Get_ProgramID() == PS_CALORIES_GOAL)
        {
          Data_Set_ElapsedOldTime(Data_Get_Time(ELAPSED_TIME));
          Data_Set_remainingOld_Calories(Data_Get_Calories(REMAINING_CALORIES));
        }
        else if(Program_Get_ProgramID() == PS_CUSTOM_DISTANCE)
        {
          Data_Set_ElapsedOldTime(Data_Get_Time(ELAPSED_TIME));
          ProCustom_Set_SegRemainingDistance();
        }
        UINT8 Recover_typeflag = 0;
        if(martix_type != Show_Incline)
        {
          g_martix_modal_type =martix_type; 
          martix_type = Show_Incline;
        }
        if(g_martix_modal_type != Show_None)
        {
          Recover_typeflag = 0xAA;
        }	
        
        UINT8 modalflag = 0;
        if( g_wndLeftBottomTextBox._activetime ==0 )
        {
          if(nCurrentPage == PAGE_WORK_TM1 || nCurrentPage == PAGE_WORK_AS1)
          {
            Event_Set_Wnd_Para(&g_wndLED2TextBox,STR_NULL_ID,0,0,0,0,ShowSpeedData);		
            Page_Push_OverlappedWindow(&g_wndLED2TextBox, &g_wndLED2TextBox);
            modalflag = 1 ;
          }
          else if(nCurrentPage == PAGE_WORK_BK1)
          {
            Event_Set_Wnd_Para(&g_wndLED2TextBox,STR_NULL_ID,0,0,0,0,ShowResistanceData);		
            Page_Push_OverlappedWindow(&g_wndLED2TextBox, &g_wndLED2TextBox);
            modalflag = 1 ;
          }
        }
        else
        {
          if(g_wndLED2TextBox.pfShow == ShowSpeedData)
          {
            Event_Set_Wnd_Para(&g_wndLED2TextBox,STR_NULL_ID,0,0,0,0,ShowInclineData);		
            Page_Push_OverlappedWindow(&g_wndLED2TextBox, &g_wndLED2TextBox); 
            modalflag = 1;
          }
          else if(g_wndLED2TextBox.pfShow == ShowResistanceData)
          {
            Event_Set_Wnd_Para(&g_wndLED2TextBox,STR_NULL_ID,0,0,0,0,ShowInclineData);		
            Page_Push_OverlappedWindow(&g_wndLED2TextBox, &g_wndLED2TextBox); 
            modalflag = 1;
          }
          
        }
        g_wndLEDTextBox._isHidden = 1; 
#ifdef TM
        Show_LEDLight(LED7_SPEED,LED_OFF);
        Show_LEDLight(LED8_INCLINE,LED_ON);
#else
        Show_LEDLight(LED1_RPM_EP,LED_OFF);
        Show_LEDLight(LED8_RESISTANCE_EP,LED_ON);
#endif
        Event_Set_Wnd_Para(&g_wndLED2TextBox,STR_NULL_ID,8,(modalflag==1?6:0),Recover_typeflag,2,ShowInclineData);	
        Event_Set_Wnd_Para(&g_wndInclineIcon,STR_NULL_ID,0,0,0,2,ShowIncline_LCDData);
      }
      //            Beep_Set_Beeps(1,3,1);
      
      
      if(program_evts.WorkOutEvts.Workout_Warmup_Evt==1)
      {
        Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_WARMUP_ID,0,5,0,0,ShowCommonTextBox);
        g_wndLED2TextBox._blinkCount = 2;
      }
      else if(program_evts.WorkOutEvts.Workout_Run_Evt == 1)
      {
        Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_RUN_ID,0,5,0,0,ShowCommonTextBox);
        g_wndLED2TextBox._blinkCount = 2;
      }
      else if(program_evts.WorkOutEvts.Workout_Cooldown_Evt==1)
      {
        Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_COOLDOWN_ID,0,5,0,0,ShowCommonTextBox);
        g_wndLED2TextBox._blinkCount = 2;
      }
      //			if(program_evts.WorkOutEvts.Workout_Sprint8_Ready==1)
      //			{
      //				Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_NULL_ID,0,0,1,0,ShowCommonTextBox);					
      //				Page_Push_OverlappedWindow(&g_wndMiddleBottomTextBox, &g_wndMiddleBottomTextBox);
      //				Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_READY_ID,0,0xAA,0,0,ShowCommonTextBox);
      //			}
      //			if(program_evts.WorkOutEvts.Workout_Sprint8_Go==1)
      //			{
      //				Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_NULL_ID,0,0,1,0,ShowCommonTextBox);					
      //				Page_Push_OverlappedWindow(&g_wndMiddleBottomTextBox, &g_wndMiddleBottomTextBox);
      //				Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_GO_ID,0xAA,0xAA,0,0,ShowCommonTextBox);
      //			}
      //			if(program_evts.WorkOutEvts.Workout_Sprint8_SprintStage==1)
      //			{
      //				Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_NULL_ID,0,0,1,0,ShowCommonTextBox);					
      //				Page_Push_OverlappedWindow(&g_wndMiddleBottomTextBox, &g_wndMiddleBottomTextBox);
      //				Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_SPRINT1_ID+(Sprint8_Get_SprintStage()-1),0,0xAA,0,0,ShowCommonTextBox);
      //			}
      //			if(program_evts.WorkOutEvts.Workout_Sprint8_ActiveRecoveryStage==1)
      //			{
      //				Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_NULL_ID,0,0,1,0,ShowCommonTextBox);					
      //				Page_Push_OverlappedWindow(&g_wndMiddleBottomTextBox, &g_wndMiddleBottomTextBox);
      //				Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_SPRINT1_ID+(Sprint8_Get_SprintStage()-1),0,1,0,0,ShowCommonTextBox);
      //			}
      
      if(program_evts.WorkOutEvts.Workout_HR_ChangeToZero==1)
      {
        if(Event_Flag.DisChangeForHR==1 && (nCurrentPage == PAGE_WORK_AS1)||(nCurrentPage == PAGE_WORK_BK1)||(nCurrentPage == PAGE_WORK_TM1))
        {
          Event_Flag.DisChangeForHR=0 ;
          //                    Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_CALORIES_TITLE_ID,0,0,0,0,ShowCaloriesTitle);					
          Event_Set_Wnd_Para(&g_wndLED3TextBox,STR_NULL_ID,0,0,0,0,ShowDistanceData);	
        }	
      }
      if(program_evts.WorkOutEvts.Workout_HR_IsNotZero==1)
      {
        if( (nCurrentPage == PAGE_WORK_AS1)||(nCurrentPage == PAGE_WORK_BK1)||(nCurrentPage == PAGE_WORK_TM1))	
        {
          //					Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_CALORIES_TITLE_ID,0,0,0,0,ShowCaloriesTitle);					
          Event_Set_Wnd_Para(&g_wndLED3TextBox,STR_NULL_ID,0,0,1,0,ShowDistanceData);		
          
          //					Page_Push_OverlappedWindow(&g_wndLeftTopNumerical, &g_wndLeftTopNumerical);
          //					Page_Push_OverlappedWindow(&g_wndLeftTopTextBox, &g_wndLeftTopTextBox);
          
          //					Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_HR_TITLE_ID,0,0xAA,0,0,ShowHeartTitle);					
          Event_Set_Wnd_Para(&g_wndLED3TextBox,STR_NULL_ID,0,0xAA,0,0,ShowHeartData);	
          
          Event_Flag.DisChangeForHR = 1;
        }
      }
    }  
    break;
  case PROGRAM_RESET_EVENT:
    {
      Event_Set_Wnd_Para(&g_wndTimeIcon,STR_NULL_ID,0,0,1,0,ShowTimeData);//zsf
      Event_Set_Wnd_Para(&g_wndDistanceIcon,STR_NULL_ID,0,0,1,0,ShowSpeedData);
      Event_Set_Wnd_Para(&g_wndCaloriesIcon,STR_NULL_ID,0,0,1,0,ShowDistanceData);
      
      Beep_Set_Beeps(4,6,6);
      Show_ClearLedLight();
      Event_Flag.PageChanged = 0;	
      Event_Flag.DisChangeForHR = 0 ;
      Page_Reset_All_ModalWnds();
      memset(&g_BackMiddleWnd,0,sizeof(Win_Items));
      Frame_Stage = FRAME_STAGE_USER;
      User_Set_VirtualKey((UINT16)VK_RESET|Frame_Stage);
      Bluetooth_Set_VirtualKey(VK_RESET);
      
      //extern UINT8 appStart;    //zsf
      //appStart = 0;
    } 
    break;
  case PROGRAM_RUN_EVENT:
    {
      if(Event_Flag.PageChanged==1)
      {
        //				nCurrentPage = g_BackPage;
        martix_type = g_martix_resume_type;
      }
      else
      {
        Event_Get_Page_matrix();
      }
      ShowPage(nCurrentPage,1);
      
      //            if(Program_Get_ProgramID() == PS_THR_ZONE ||Program_Get_ProgramID() == PS_MYFIRST5K)
      //            {
      //                Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_NULL_ID,0,0,1,0,ShowCommonTextBox);					
      //				Page_Push_OverlappedWindow(&g_wndMiddleBottomTextBox, &g_wndMiddleBottomTextBox);
      //				Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_NULL_ID,0,5,0,0,ShowCommonTextBox);
      //            }
      
      Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,STR_NULL_ID,0xAA,0,0,0,ShowMatrixData);
      //			if(g_BackMiddleWnd._string_id !=0 )
      //			{
      //				memcpy(&g_wndMiddleBottomTextBox,&g_BackMiddleWnd,sizeof(Win_Items));
      //				g_wndMiddleBottomTextBox.pfShow(&g_wndMiddleBottomTextBox);
      //			}
      Frame_Stage = FRAME_STAGE_PRGRUN;
    }
    break;		
  case PROGRAM_PAUSE_EVENT:
    {	
      //			memcpy(&g_BackMiddleWnd,&g_wndLEDTextBox,sizeof(Win_Items));
//WWW      Event_Set_Wnd_Para(&g_wndTimeIcon,STR_NULL_ID,0,0,1,0,ShowElapsedTimeData);
      Event_Set_Wnd_Para(&g_wndTimeIcon,STR_NULL_ID,0,0,1,0,ShowTimeData);//WWW
#ifdef TM
      Event_Set_Wnd_Para(&g_wndDistanceIcon,STR_NULL_ID,0,0,1,0,ShowSpeedData);
      Event_Set_Wnd_Para(&g_wndCaloriesIcon,STR_NULL_ID,0,0,1,0,ShowDistanceData);
      //			Event_Set_Wnd_Para(&g_wndLED4TextBox,STR_NULL_ID,0,0,1,0,ShowCaloriesData);
#else
      Event_Set_Wnd_Para(&g_wndDistanceIcon,STR_NULL_ID,0,0,1,0,ShowLcdDistanceData);
      Event_Set_Wnd_Para(&g_wndCaloriesIcon,STR_NULL_ID,0,0,1,0,ShowLcdCaloriesData); 
#endif   
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_WORKOUTPAUSE_ID,0,0,0,0,ShowCommonTextBox);
      Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,STR_NULL_ID,0,0,0,0,ShowMatrixData);
    }
    break;
  case SETTING_COMPLETE_EVENT:
    {
      g_wndLED2TextBox._blinkCount = 0;
      //			Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_NULL_ID,0,0,1,0,ShowCommonTextBox);
      Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,STR_NULL_ID,0,0,0,0,ShowMatrixData);
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_PRESSSTART_TITLE_ID,0,0,0,0,ShowCommonTextBox);
    }
    break;
  case PROGRAM_ENGINEER_EVENT:
    {
      
    }
    break;		
    
  case THREE_EVENT:
    {      
      Beep_Set_Beeps(6,10,10);
      
      Event_Set_Wnd_Para(&g_wndTimeIcon,STR_NULL_ID,0,0,1,0,ShowTimeData);//zsf
      Event_Set_Wnd_Para(&g_wndDistanceIcon,STR_NULL_ID,0,0,1,0,ShowSpeedData);
      Event_Set_Wnd_Para(&g_wndCaloriesIcon,STR_NULL_ID,0,0,1,0,ShowDistanceData);     
    }
  case ONE_EVENT:
  case TWO_EVENT:
    {
      UINT8 Temp_id =  STR_3_TITLE_ID-(program_evts.evt-THREE_EVENT);
      Frame_Stage = FRAME_STAGE_PRGRUN;
      Event_DisableFlashingAllWnds();
#ifdef TM
      if(nCurrentPage != PAGE_WORK_TM2)
      {
        Show_LEDLight(LED5_TIME,LED_ON);
        Show_LEDLight(LED7_SPEED,LED_ON);
        Show_LEDLight(LED9_DISTANCE,LED_ON);
        Show_LEDLight(LED6_CALORIES,LED_OFF);
        Show_LEDLight(LED8_INCLINE,LED_OFF);
        Show_LEDLight(LED10_HRT,LED_OFF);
      }
#else
      if( PAGE_PROGRAM == nCurrentPage || PAGE_WORK_BK1 == nCurrentPage )
      {
        ;
      }
     else if(nCurrentPage != PAGE_WORK_BK2)
      {
        Show_LEDLight(LED6_CALORIES_EP,LED_OFF);
        Show_LEDLight(LED8_RESISTANCE_EP,LED_OFF);
        Show_LEDLight(LED3_HRT_EP,LED_OFF);
        Show_LEDLight(LED10_TIME_EP,LED_ON);
        Show_LEDLight(LED1_RPM_EP,LED_ON);
        Show_LEDLight(LED2_DISTANCE_EP,LED_ON);
        
      }
#endif
      Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,STR_NULL_ID,0,0,0,0,ShowMatrixData);
      Event_Set_Wnd_Para(&g_wndBottomTextBox,(STRINGID)Temp_id,0,0,0,0,ShowCommonTextBox);			
    }
    break;
  default:   break;
  }
  //    g_wndWiFiIcon.pfShow= ShowWifiIcon;
  //    g_wndWiFiIcon.pfShow(&g_wndWiFiIcon);
  
  //    g_wndRFIcon.pfShow= ShowRFICON;
  //    g_wndRFIcon.pfShow(&g_wndRFIcon);
}


void RTC_Event_Handler(RTCSET_EVT RTCevts)
{
	if( !(Frame_Stage == FRAME_STAGE_USER || Frame_Stage == FRAME_STAGE_PRGSETUP|| Frame_Stage == FRAME_STAGE_RTC)) 
        return;

	if(RTCevts==NO_SETTING_EVENT) return;

	Display_Set_All(0);
	switch( RTCevts )
	{
		case RTC_MONTH_SET_EVENT:
		{
			Event_Set_Wnd_Para(&g_wndLEDTextBox,NULL,0xAA,0,0,1,ShowRTCMonth);	
			Frame_Stage = FRAME_STAGE_RTC;	
		}
		break;
		case RTC_DATE_SET_EVENT:
		{
			Event_Set_Wnd_Para(&g_wndLEDTextBox,NULL,0xAA,0,0,1,ShowRTCDate);	
		}
		break;
		case RTC_YEAR_SET_EVENT:
		{
			Event_Set_Wnd_Para(&g_wndLEDTextBox,NULL,0xAA,0,0,1,ShowRTCYear);	
		}
		break;
		case RTC_HOUR_SET_EVENT:
		{
			Event_Set_Wnd_Para(&g_wndLEDTextBox,NULL,0xAA,0,0,1,ShowRTCHour);	
		}
		break;
		case RTC_MINUTE_SET_EVENT:
		{
			Event_Set_Wnd_Para(&g_wndLEDTextBox,NULL,0xAA,0,0,1,ShowRTCMinute);	
		}
		break;
		case RTC_AM_PM_SET_EVENT:
		{
			Event_Set_Wnd_Para(&g_wndLEDTextBox,NULL,0xAA,0,0,1,ShowRTCAMPM);	
		}
		break;
		case RTC_QUIT_SETTING_MODE:
		{
			Frame_Stage = FRAME_STAGE_USER;
	        nCurrentPage = PAGE_SELECTUSER;
	        Event_Set_Virtual_key(VK_INITIAL);
		}
		break;
		default:break;
	}
}

/***************************************************************************//**
* @brief
*   ENG_Events_Handler 
*
* @note
*   
* @param[in]:ENG_EVENT eng_evts
* @param[out]:none
******************************************************************************/
void ENG_Events_Handler(ENG_EVENT eng_evts)
{
  if(Frame_Stage == FRAME_STAGE_SAFFKEYOFF) return;
  
  if(eng_evts==NO_ENG_EVENT) return;
  EFM_ASSERT(eng_evts < ENG_EVENT_TOTAL);
  Display_Set_All(0);
  Event_Set_Wnd_Para(&g_wndLapsCircleIcon,0,0,0,0,0,ShowLapsCircle);
  switch(eng_evts)
  {
  case DISPLAY_ENG_EVENT:
    {
      LED_Set_All(false);
      Frame_Stage=FRAME_STAGE_ENG;
      Bluetooth_Set_VirtualKey(VK_ENG);  //zsf
      nCurrentPage = PAGE_ENG0_0_DisplayTest;    //zsf
      //			Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_ENG0_ID,0,0,0,0,ShowCommonTextBox);
      //			Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_DISPLAYTEST_ID,0,0,0,0,ShowCommonTextBox);		
//      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_ENG0_ID,0,0,0,0,ShowBottomTextBox);
      
      g_wndLED1TextBox._x = 2;
      g_wndLED1TextBox._y = 0;
      g_wndLED1TextBox._z = 0;
      Event_Set_Wnd_Para(&g_wndLED1TextBox,STR_ENG0_ID,0,0,0,0,ShowBottomTextBox);   //zsf
      
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_DISPLAYTEST_ID,0,0,0,0,ShowBottomTextBox);
    }
    break;
  case ALL_DOTSON_ENG_EVENT:
    {
      Display_Set_All(true);
      LED_Set_All(true);
      HAL_BOR_Enable_ExtPower(Power_Channel_ERP,1);
    }
    break;
  case ALL_DOTSOFF_ENG_EVENT:
    {
      Display_Set_All(false);
      LED_Set_All(false);
      HAL_BOR_Enable_ExtPower(Power_Channel_ERP,0);            
    }
    break;
  case ENG0_DIS_NUMBER:
    {
      HAL_BOR_Enable_ExtPower(Power_Channel_ERP,0);
      //			Event_Set_Wnd_Para(&g_wndLeftBottomNumerical,NULL,0,0,0,0,ShowENG0Number);
      //			Event_Set_Wnd_Para(&g_wndRightBottomNumerical,NULL,0,0,0,0,ShowENG0Number);
      //			Event_Set_Wnd_Para(&g_wndRightTopNumerical,NULL,0,0,0,0,ShowENG0Number);
      //			Event_Set_Wnd_Para(&g_wndLeftTopNumerical,NULL,0,0,0,0,ShowENG0Number);
      //			Event_Set_Wnd_Para(&g_wndLEDTextBox,NULL,0,0,0,0,ShowENG0LedNumber);
      //          Event_Set_Wnd_Para(&g_wndLED1TextBox,NULL,0,0,0,0,ShowENG0LedNumber);
      Event_Set_Wnd_Para(&g_wndLED1TextBox,NULL,0,0,0,0,ShowENG0LedNumber);
      Event_Set_Wnd_Para(&g_wndLED2TextBox,NULL,0,0,0,0,ShowENG0LedNumber);
      Event_Set_Wnd_Para(&g_wndLED3TextBox,NULL,0,0,0,0,ShowENG0LedNumber);
    }
    break;
  case ENG0_DIS_KEY_VALUE:
    {
      //			Event_Flag.DisAllOn =0;
      //            Event_Flag.DisAllOff =0;
      LED_Set_All(false);
      HAL_BOR_Enable_ExtPower(Power_Channel_ERP,0);
      //			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_KEY_TEST_ID,0,0,0,0,ShowCommonTextBox);
      //			Event_Set_Wnd_Para(&g_wndLEDTextBox,NULL,0,0,0,0,ShowENGKeyValue);
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_KEY_TEST_ID,0,0,0,0,ShowBottomTextBox);
      Event_Set_Wnd_Para(&g_wndLED2TextBox,NULL,0,0,0,0,ShowENGKeyValue);
    }
    break;
    //eng1
  case HARDWARE_ENG_EVENT:
    {
      LED_Set_All(false);
      //			Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_ENG1_ID,0,0,0,0,ShowCommonTextBox);
      //			Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_HARDWARE_ID,0,0,0,0,ShowCommonTextBox);		
      //LCD DISPLAY
      Event_Set_Wnd_Para(&g_wndLED1TextBox,STR_ENG1_ID,0,0,0,0,ShowBottomTextBox);
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_HARDWARE_ID,0,0,0,0,ShowBottomTextBox);	
    }
    break;
  case BASICFUNCTIONS_ENG_EVENT:
    {
      //			MCB_TYPE _type = (MCB_TYPE)LCB_Get_MCBType();
      //g_wndLED4TextBox-->HR   
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_HARDWARE1_ID,0,0,0,0,ShowBottomTextBox);  //zsf
      //Event_Set_Wnd_Para(&g_wndRightBottomNumerical,STR_NULL_ID,0,0,0,0,ShowHeartData);
      
      //      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_ADC_ID,0,0,0,0,ShowBottomTextBox);
      //      g_wndLeftBottomNumerical._z = 0;
      //            Event_Set_Wnd_Para(&g_wndLeftBottomNumerical,STR_NULL_ID,0,0,0,0,ShowENGInclineADCData);
      //      Event_Set_Wnd_Para(&g_wndLED2TextBox,STR_NULL_ID,0,0,0,0,ShowENGInclineADCData);
//          Event_Set_Wnd_Para(&g_wndLED1TextBox,STR_NULL_ID,0,0,0,0,ShowENGInclineADCData);
      
      if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
      {
        //         Event_Set_Wnd_Para(&g_wndLED2TextBox,STR_NULL_ID,0,0,0,0,ShowENGSpeedData);
        
        //SET RPM
        //        Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_SPEED_TITLE_ID,0,0,0,0,ShowCommonTextBox);
        //        g_wndLeftTopNumerical._z = 0;
        //        Event_Set_Wnd_Para(&g_wndLeftTopNumerical,STR_NULL_ID,0,0,0,0,ShowENGSetRPMData);
        //actual RPM
        //        Event_Set_Wnd_Para(&g_wndRightTopTextBox,STR_RPM_ID,0,0,0,0,ShowCommonTextBox);
        //        g_wndRightTopNumerical._z = 0;
        Event_Set_Wnd_Para(&g_wndLED1TextBox,STR_NULL_ID,0,0,0,0,ShowENGInclineADCData);  //  WWW
        g_wndLED3TextBox._z = 0;
        //        Event_Set_Wnd_Para(&g_wndRightTopNumerical,STR_NULL_ID,0,0,0,0,ShowENGLCBRPMData);
        Event_Set_Wnd_Para(&g_wndLED3TextBox,STR_NULL_ID,0,0,0,0,ShowENGSpeedData);
      }
      else
      {
        /*  
        Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_ECB_ID,0,0,0,0,ShowCommonTextBox);
        g_wndLeftTopNumerical._z = 0;
        Event_Set_Wnd_Para(&g_wndLeftTopNumerical,STR_NULL_ID,0,0,0,0,ShowENGSettedECBCount);
        g_wndRightTopNumerical._z = 0;
        Event_Set_Wnd_Para(&g_wndRightTopNumerical,STR_NULL_ID,0,0,0,0,ShowENGLCBECBCount);	
        */
        //Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_RPM_RESISTANCE_ID,0,0,0,0,ShowCommonTextBox);
        g_wndLED1TextBox._x = 2;
        g_wndLED1TextBox._y = 0;
        g_wndLED1TextBox._z = 0;  
        Event_Set_Wnd_Para(&g_wndLED1TextBox,STR_NULL_ID,0,0,0,0,ShowRpmData); 
        g_wndLED2TextBox._z = 0;
        Event_Set_Wnd_Para(&g_wndLED2TextBox,STR_NULL_ID,0,0,0,0,ShowResistanceData);
      }
      //			if(_type ==DC_LCB_30_325HP_JIS||_type ==MCB_DELTA || _type ==MCB_TOPTEK|| _type ==MCB_DELTA_COMMOCIAL)
      //			{
      //				 //LeftTopTextBox-->speed
      //		       	Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_SPEED_TITLE_ID,0,0,0,0,ShowCommonTextBox);
      //		        //g_wndLeftTopNumerical-->RPM
      //		        g_wndLeftTopNumerical._z = 0;
      //		       	Event_Set_Wnd_Para(&g_wndLeftTopNumerical,STR_NULL_ID,0,0,0,0,ShowENGSetRPMData);
      //
      //				//RightTopTextBox-->PWM
      //		       	Event_Set_Wnd_Para(&g_wndRightTopTextBox,STR_RPM_ID,0,0,0,0,ShowCommonTextBox);
      //		        //g_wndRightTopNumerical-->RPM
      //		        g_wndRightTopNumerical._z = 0;
      //				Event_Set_Wnd_Para(&g_wndRightTopNumerical,STR_NULL_ID,0,0,0,0,ShowENGLCBRPMData);	
      //		       	Event_Set_Wnd_Para(&g_wndLED2TextBox,STR_NULL_ID,0,0,0,0,ShowSpeedData);
      //			}
      //	       	else
      //	       	{
      //	       		//LeftTopTextBox-->ECB count Setted
      //		       	Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_ECB_ID,0,0,0,0,ShowCommonTextBox);
      //		        //g_wndLeftTopNumerical-->RPM
      //		        g_wndLeftTopNumerical._z = 0;
      //		       	Event_Set_Wnd_Para(&g_wndLeftTopNumerical,STR_NULL_ID,0,0,0,0,ShowENGSettedECBCount);
      //
      //				//g_wndLeftTopNumerical-->ECB count From LCB
      //		        g_wndRightTopNumerical._z = 0;
      //				Event_Set_Wnd_Para(&g_wndRightTopNumerical,STR_NULL_ID,0,0,0,0,ShowENGLCBECBCount);	
      //			}
      //			if(_type!=DIGITAL_ECB )	
      //			{
      //		        //LeftBottomTextBox-->INCLINE
      //		       	Event_Set_Wnd_Para(&g_wndLeftBottomTextBox,STR_INCLINE_TITLE_ID,0,0,0,0,ShowCommonTextBox);
      //				//g_wndLeftBottomNumerical-->incline ADC
      //		        g_wndLeftBottomNumerical._z = 0;
      //		       	Event_Set_Wnd_Para(&g_wndLeftBottomNumerical,STR_NULL_ID,0,0,0,0,ShowENGInclineADCData);
      //
      //		        //RightBottomTextBox-->ADC
      //		       	Event_Set_Wnd_Para(&g_wndRightBottomTextBox,STR_ADC_ID,0,0,0,0,ShowCommonTextBox);
      //		        //g_wndLeftBottomNumerical-->incline
      //		        g_wndRightBottomNumerical._z = 0;
      //		       	Event_Set_Wnd_Para(&g_wndRightBottomNumerical,STR_NULL_ID,0,0,0,0,ShowENGLCBECBCount);
      //		       	Event_Set_Wnd_Para(&g_wndInclineIcon,STR_NULL_ID,0,0,0,0,ShowIncline_LCDData);
      //			}
    }
    break;
    
  case USB_ENG_EVENT:
    {
      Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_USBTEST_ID,0,0,0,0,ShowCommonTextBox);
    }
    break;
  case USB_TESTING_ENG_EVENT:
    {
      Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_USBTESTING_ID,0,0,0,0,ShowCommonTextBox);
    }
    break;	
#ifdef __WIFI_FUNC__	
  case WIFI_ENG_EVENT:
    {
      Wifi_Get_EnggineTestResult();
      Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_WIFITEST_ID,0,0,0,0,ShowCommonTextBox);
    } 
    break;
  case WIFI_TESTING_ENG_EVENT:
    {
      Wifi_Get_EnggineTestResult();
      Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_WIFI_WAITTING_ID,0,0,0,0,ShowCommonTextBox);
    } 
    break;
#endif
  case ENG_SHOW_MAC_EVENT:
    {
      Event_Set_Wnd_Para(&g_wndBottomTextBox,0,0,0,0,0,ShowMacAddress);
    }
    break;
  case ENG_SHOW_BT_VERSION_EVENT:
    {
      Event_Set_Wnd_Para(&g_wndBottomTextBox,0,0,0,0,0,ShowBTVersion);
    }
    break;
    //eng2
  case NOTAVAILABLE_ENG_EVENT:
    {
      Event_Set_Wnd_Para(&g_wndLED1TextBox,STR_ENG2_ID,0,0,0,0,ShowCommonTextBox);
      Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_NOT_AVAILABLE_ID,0,0,0,0,ShowCommonTextBox);
      //LCD DISPLAY
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_ENG2_ID,0,0,0,0,ShowBottomTextBox);
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_NOT_AVAILABLE_ID,0,0,0,0,ShowBottomTextBox);
    }
    break;
    //eng3
  case SWITCH_ENG_EVENT:
    {
      LED_Set_All(false);
      STRINGID Tempid;
      if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
        Tempid = STR_ENG3_ID;
      else
        Tempid = STR_ENG2_ID;
      
      Event_Set_Wnd_Para(&g_wndLED1TextBox,Tempid,0,0,0,0,ShowCommonTextBox);
      Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_SWITCHFUNCATION_ID,0,0,0,0,ShowCommonTextBox);
      //LCD DISPLAY
      //      Event_Set_Wnd_Para(&g_wndBottomTextBox,Tempid,0,0,0,0,ShowBottomTextBox);
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SWITCHFUNCATION_ID,0,0,0,0,ShowBottomTextBox);
    }
    break;		
    //eng4
  case INFORMATION_ENG_EVENT:
    {
      LED_Set_All(false);
      STRINGID Tempid;
      if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
        Tempid = STR_ENG4_ID;
      else
        Tempid = STR_ENG3_ID;
      //            LED_Light(DP_LED1,0);
      Event_Set_Wnd_Para(&g_wndLED1TextBox,Tempid,0,0,0,0,ShowCommonTextBox);
      Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_INFORMATION_ID,0,0,0,0,ShowCommonTextBox);
      //LCD DISPLAY
      Event_Set_Wnd_Para(&g_wndBottomTextBox,Tempid,0,0,0,0,ShowBottomTextBox);
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_INFORMATION_ID,0,0,0,0,ShowBottomTextBox);
    }
    break;		
  case ACCINF_ENG_EVENT:
    {
      //show acc
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_TITLE_ACCUMULATEDINF_ID,0,0,0,0,ShowCommonTextBox);
      
      //show distance
      //	       	Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_DISTANCE_ID,0,0,0,0,ShowCommonTextBox);
      
      //show time
      //	       	Event_Set_Wnd_Para(&g_wndRightTopTextBox,STR_TIME_ID,0,0,0,0,ShowCommonTextBox);
      //            LED_Light(DP_LED1,1);
      //show distance data
      Event_Set_Wnd_Para(&g_wndLED3TextBox,STR_NULL_ID,0,0,0,0,ShowACCDis);
      
      //show time data
      Event_Set_Wnd_Para(&g_wndLED1TextBox,STR_NULL_ID,0,0,0,0,ShowACCTime);
    }
    break;		
  case UCBVERSION_ENG_EVENT:
    {
      //dis UCB
      //            LED_Light(DP_LED1,0);
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_TITLE_UCBVERSION_ID,0,0,0,0,ShowUCBVision);
      //	       	Event_Set_Wnd_Para(&g_wndLeftTopTextBox,0,0,0,0,0,ShowUCBVision);
    }
    break;
  case MCBVERSION_ENG_EVENT:
    {
      //dis MCB
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_TITLE_MCBVERSION_ID,0,0,0,0,ShowMCBVision);
      //	       	Event_Set_Wnd_Para(&g_wndLeftTopTextBox,0,0,0,0,0,ShowMCBVision);
    }
    break;		
    //eng8
  case TEST_ENG_EVETN:
    {
      LED_Set_All(false);
      Event_Set_Wnd_Para(&g_wndLED1TextBox,STR_ENG8_ID,0,0,0,0,ShowCommonTextBox);
      Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_TEST_ID,0,0,0,0,ShowCommonTextBox);
      //LCD DISPLAY
      Event_Set_Wnd_Para(&g_wndLED1TextBox,STR_ENG8_ID,0,0,0,0,ShowBottomTextBox);
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_TEST_ID,0,0,0,0,ShowBottomTextBox);
    }
    break;		
  case BURN_ENG_EVENT:
    {
      //P3
      //			Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_P3_ID,0,0,0,0,ShowCommonTextBox);
      Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_TITLE_RESERVED_ID,0,0,0,0,ShowCommonTextBox);
    }
    break;
  case UNIT_ENG_EVENT:
    {  
      STRINGID Tempid2;				
//WWW     if(Engmode_Get_Uint() == METRIC)
          if(SystemConfig_Get_Char(UNIT_FLAG_UCHAR) == METRIC) //WWW 
      {
        Tempid2 = STR_METRIC_ID;
      }
      else
      {
        Tempid2 = STR_STANDARD_ID;
      }
      Event_Set_Wnd_Para(&g_wndBottomTextBox, Tempid2,0,0,0,0,ShowCommonTextBox);
    }
    break;		
  case LANGUAGE_ENG_EVENT:
    {         
      //P2
      STRINGID Tempid = (STRINGID)(STR_ENGLISH_ID+SystemConfig_Get_Char(LANGUAGE_ID_UINT8));
      Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_P2_ID,0,0,0,0,ShowCommonTextBox);
      Event_Set_Wnd_Para(&g_wndBottomTextBox,Tempid,0,0,0,0,ShowCommonTextBox);
    }
    break;		
  case MACHINE_ENG_EVENT:
    {
      STRINGID Tempid = (STRINGID)(STR_TREADMILL_ID+SystemConfig_Get_Char(MACHINE_TYPE_UINT8));		
      Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_P3_ID,0,0,0,0,ShowCommonTextBox);
      Event_Set_Wnd_Para(&g_wndBottomTextBox,Tempid,0,0,0,0,ShowCommonTextBox);
    }
    break;		
  case MODEL_ENG_EVENT:
    {
      //P4
      Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_P4_ID,0,0,0,0,ShowCommonTextBox);
      
      UINT8 temptypeID = SystemConfig_Get_Char(MACHINE_MODLE_UINT8);
      //UINT8 tempMachineID = SystemConfig_Get_Char(MACHINE_TYPE_UINT8);
      switch(SystemConfig_Get_Char(MACHINE_TYPE_UINT8))	
      {
      case ASCENT:
        {
          if(temptypeID==1)
            temptypeID =STR_A30_ID;  
          else if(temptypeID==2)
            temptypeID =STR_A50_ID; 
        }
        break;
      case BIKE:
        {
          if(temptypeID==1)
            temptypeID =STR_U30_ID;  
          else if(temptypeID==2)
            temptypeID =STR_U50_ID; 
          else if(temptypeID==3)
            temptypeID =STR_R30_ID;   
          else if(temptypeID==4)
            temptypeID =STR_R50_ID;    
        }
        break;
      case ELLIPTICAL:
        {
          if(temptypeID==1)
            temptypeID =STR_SE30_ID;  
          else if(temptypeID==2)
            temptypeID =STR_SE50_ID; 
        }
        break;
      case TREADMILL:
        {
          if(temptypeID==1)
            temptypeID =STR_TF30_ID;  
          else if(temptypeID==2)
            temptypeID =STR_TF50_ID;  
          else if(temptypeID==3)
            temptypeID =STR_T50_ID;   
          else if(temptypeID==4)
            temptypeID =STR_T70_ID;
        }
        break;
      default:break;
      }
      Event_Set_Wnd_Para(&g_wndBottomTextBox,(STRINGID)temptypeID,0,0,0,0,ShowCommonTextBox);
    }
    break;		
  case ERP_ENG_EVENT:
    {
      STRINGID Tempid1;	
      //P5
      //           	Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_P5_ID,0,0,0,0,ShowCommonTextBox);
      
      if( SystemConfig_Get_Char(ENERGY_SAVER_FLAG_UCHAR) == 1 )
      {
        Tempid1 = STR_ENERGYSAVEON_ID;
      }
      else
      {
        Tempid1 = STR_ENERGYSAVEOFF_ID; 
      }
      Event_Set_Wnd_Para(&g_wndBottomTextBox,Tempid1,0,0,0,0,ShowCommonTextBox);
    }
    break;
  case BOOT_ENG_EVENT:
    {
      //P6
      //           	Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_P6_ID,0,0,0,0,ShowCommonTextBox);
      
      STRINGID Tempid;	
      if( SystemConfig_Get_Char(FIRST_BOOT_FLAG_UCHAR) == 0xA5 )
      {
        Tempid = STR_BOOT_OFF_ID;
      }
      else
      {
        Tempid = STR_BOOT_ON_ID;
      }
      Event_Set_Wnd_Para(&g_wndBottomTextBox,Tempid,0,0,0,0,ShowCommonTextBox);
    }
    break;		
  case QUICK_PRO_ENG_EVENT:
    {
      //P1
      //           	Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_P1_ID,0,0,0,0,ShowCommonTextBox);
      if(Engmode_Get_PROG_Mult_Times()==1)
      {
        Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_NORMAL_ID,0,0,0,0,ShowCommonTextBox);
      }
      else
      {
        Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_QUICK_ID,0,0,0,0,ShowQuickMul);
      }
    }
    break;		
  case DAPI_ENG_EVENT:
    {
      STRINGID Tempid;			
      if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
        Tempid = STR_ENG5_ID;
      else
        Tempid = STR_ENG4_ID;
      
      Event_Set_Wnd_Para(&g_wndLeftTopTextBox,Tempid,0,0,0,0,ShowCommonTextBox);
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_DAPI_ID,0,0,0,0,ShowCommonTextBox);
      //LCD DISPLAY
      Event_Set_Wnd_Para(&g_wndBottomTextBox,Tempid,0,0,0,0,ShowBottomTextBox);
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_DAPI_ID,0,0,0,0,ShowBottomTextBox);
    }
    break;		
  case SETDAPI_ENG_EVENT:
    {
      //set dapi 
      STRINGID Tempid1,Tempid2;		
      Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_DAPI_ID,0,0,0,0,ShowCommonTextBox);
      
      MACHINE_INFO machineinfo = SystemConfig_Get_MachineInfo();
      if(strncmp(machineinfo.marioBox, JOHNSON_WIFI_SERVERIP_PRODUCTION, 256) == 0 )
      {
        Tempid1 = STR_PRODUCTION_ID;
        Tempid2 = (STRINGID)0;
      } 
      else if(strncmp(machineinfo.marioBox, JOHNSON_WIFI_SERVERIP_QA, 256) == 0 )
      {
        Tempid1 = STR_QA_ID;
        Tempid2 = (STRINGID)1;
      } 
      else
      {
        Tempid1 = STR_STAGING_ID;
        Tempid2 = (STRINGID)2;
        g_wndBottomTextBox._string_id = STR_STAGING_ID;
        g_wndLeftTopNumerical._string_id = (STRINGID)2;
      }
      Event_Set_Wnd_Para(&g_wndBottomTextBox,Tempid1,0,0,0,0,ShowCommonTextBox);
      Event_Set_Wnd_Para(&g_wndLeftTopNumerical,Tempid2,0,0,0,0,ShowCommonData);
    }    
    break;
  case CLEAR_ENG_EVENT:
    {
      //P2
      //            Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_P2_ID,0,0,0,0,ShowCommonTextBox);
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_NOCLEAR_ID,0,0,0,0,ShowCommonTextBox);
    }
    break;		
  case CLEARED_ENG_EVENT:
    {
      //P2
      //            Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_P2_ID,0,0,0,0,ShowCommonTextBox);
	                Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_CLEAR_ID,0,0,0,0,ShowCommonTextBox);
    }
    break;		
  case QUICK_ERP_ENG_EVENT:
    {
      //P4
      //            Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_P4_ID,0,0,0,0,ShowCommonTextBox);
      Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_PRESSSTARTBUTTON_ID,0,0,0,0,ShowCommonTextBox);
    }
    break;		
  case ERP_WAIT_TIME:
    {
      //			Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_P5_ID,0,0,0,0,ShowCommonTextBox);
      Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_SET_ERP_TIME_ID,0,0,0,0,ShowERPWaitTime);		
      //           	Event_Set_Wnd_Para(&g_wndRightTopNumerical,STR_NULL_ID,0,0,0,0,ShowERPWaitTime);		
    }
    break;		
  case QUIT_ENG_EVENT:
    {
      Beep_Set_Beeps(4,6,6);
      Main_Init_Model_Data();
      Event_Handler_Init_Data();
      Frame_Stage = FRAME_STAGE_USER;
      nCurrentPage = PAGE_SELECTUSER;
      Event_Set_Virtual_key(VK_INITIAL);
    }
    break;
  case SET_RPM:
    {
      //			Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_P6_ID,0,0,0,0,ShowCommonTextBox);
      Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_SET_RPM_ID,0,0,0,0,ShowSettedRPM);		
      //           	Event_Set_Wnd_Para(&g_wndRightTopNumerical,STR_NULL_ID,0,0,0,0,ShowSettedRPM);		
      
    }
  default: break;
  }
}


void UTL_Events_Handler(UTL_EVENT UTL_Event)
{
    switch(UTL_Event.err_event)
    {
        case S_NO_EVENT:
        {
            break;
        }
        case Safekey_off_to_on:
        {
            if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)!=TREADMILL) break;
            Frame_Stage = FRAME_STAGE_USER;
            nCurrentPage = PAGE_SELECTUSER;
            User_Set_VirtualKey((UINT16)VK_RESET|FRAME_STAGE_USER);  
            Program_Set_VirtualKey(VK_SAFEKEYON);
            Beep_Set_Beeps(4,6,6);
            ERP_Clear_time();
            break;
        }
        case Safekey_on_to_off:
        {
            Show_ClearLedLight();
            if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)!=TREADMILL) break;
            Program_Set_VirtualKey((UINT16)VK_RESET|FRAME_STAGE_PRGSETUP);
            Engmode_Set_VirtualKey((UINT16)VK_RESET|FRAME_STAGE_ENG);
            //LCB_Reset_Buff();
            LCB_Initial();
            Frame_Stage = FRAME_STAGE_SAFFKEYOFF;
			nCurrentPage = 	PAGE_SAFETYKE_OFF;		
            Display_Set_All(0);
            Event_HideAllWnds(1);
            Beep_Set_Beeps(6,5,5);
            Event_Flag.PageChanged = 0;
		    ShowPage(nCurrentPage,1);
            ERP_Clear_time();
            
            //zsf
            g_wndLED1TextBox._x = 2;
            g_wndLED1TextBox._y = 0;
            g_wndLED1TextBox._z = 0;
            Event_Set_Wnd_Para(&g_wndLED1TextBox,NULL,0,0,0,0,ShowSafekeyOffLedNumber);
            
            g_wndLED2TextBox._x = 2;
            g_wndLED2TextBox._y = 1;
            g_wndLED2TextBox._z = 0;
            Event_Set_Wnd_Para(&g_wndLED2TextBox,NULL,0,0,0,0,ShowSafekeyOffLedNumber);
            
            g_wndLED3TextBox._x = 2;
            g_wndLED3TextBox._y = 2;
            g_wndLED3TextBox._z = 0;
            Event_Set_Wnd_Para(&g_wndLED3TextBox,NULL,0,0,0,0,ShowSafekeyOffLedNumber);
      
      
           // g_BackPage = PAGE_INITAL;
           //	Event_Set_Wnd_Para(&g_wndLEDTextBox,STR_SAFEKEY_OFF_ID,0,0,0,0,ShowCommonTextBox);		
        }
		 break;
        default: break;
    }
    
    switch(UTL_Event.erp_event)
    {
        case NUM_0:
        case NUM_1:
        case NUM_2:
        case NUM_3:
        case NUM_4:
        case NUM_5:
        case NUM_6:
        case NUM_7:
        case NUM_8:
        case NUM_9:
        {
            Display_Set_All(false);
            LED_Set_All(false);
//            if(UTL_Event.erp_event!=NUM_0)
//            {
                nCurrentPage = PAGE_ERP_COUNTDOWN;
                Frame_Stage = FRAME_STAGE_ERP_COUNT;
				g_wndBottomTextBox._z = NULL;				
				STRINGID Tempid = (STRINGID)(UTL_Event.erp_event-NUM_0 + STR_0_TITLE_ID);
           		Event_Set_Wnd_Para(&g_wndBottomTextBox,Tempid,0,0,0,0,ShowCommonTextBox);		
//            }
        }
		break;
        case ERP_NO_EVENT:
        {
//            Display_Set_All(false);
            break;
        }
        case Enter_ERP:
        {
            Display_Set_All(0);
        }
        break;
        case ENTER_DEMO:
		{
			if(Frame_Stage!=FRAME_STAGE_ENG&&Frame_Stage!=FRAME_STAGE_MODEL&&Frame_Stage!=FRAME_STAGE_RTC)
			{
                if(Frame_Stage != FRMAE_STATE_DEMOMODE && Frame_Stage != FRAME_STAGE_ERP_COUNT)
                    Frame_OldStage = Frame_Stage;
                if(nCurrentPage != PAGE_ERP_COUNTDOWN)
                g_BackPage = nCurrentPage;
                
        		Frame_Stage = FRMAE_STATE_DEMOMODE;
                nCurrentPage = PAGE_ERP_COUNTDOWN;
                Show_ClearLedLight();
                Bluetooth_Set_VirtualKey(VK_DEMO);  //zsf
			}
       	}
        break;
        default: break;
    }
    
    switch(UTL_Event.burn_event)
    {
        case  E_AGINGOFF:
        {
            Frame_Stage = FRAME_STAGE_USER;
            nCurrentPage = PAGE_SELECTUSER;
            User_Set_VirtualKey((UINT16)VK_RESET|Frame_Stage);
            LED_Set_All(false);
            Display_Set_All(false);
            break;
        }
        case  E_AGINGON:
        {
            Frame_Stage = FRAME_STAGE_AGINGTEST;
            nCurrentPage = PAGE_AGINTEST;
            User_Set_VirtualKey((UINT16)VK_RESET|Frame_Stage);
            LED_Set_All(true);
            Display_Set_All(true);
            break;
        }
        default: break;
    }
    if(UTL_Event.heart_event)
    {
        if(g_wndLED3TextBox.pfShow == ShowHeartData)
            Show_LEDLight(LED_HRT_ICON,LED_ON);
        Display_HEART_ICON(true);
    }
    else if(Frame_Stage != FRAME_STAGE_ENG  && Frame_Stage != FRAME_STAGE_AGINGTEST)
    {
        Show_LEDLight(LED_HRT_ICON,LED_OFF);
        Display_HEART_ICON(false);
    }
}



/***************************************************************************//**
* @brief
*   void USB_Events_Handler(USB_EVENTS evt)
*
* @note
*   
* @param[in]:USB_EVENTS evt
* @param[out]:none
******************************************************************************/
void USB_Events_Handler(USB_EVENTS evt)
{ 
	if(Frame_Stage == FRAME_STAGE_SAFFKEYOFF) return;
	
    if(evt == EVT_USB_NONE) return;
    EFM_ASSERT(evt < EVT_USB_TOTAL);
    
    switch(evt)
    {
        case EVT_FILE_DETECTED:
        case EVT_BTUPDATEFILE_DETECTED:
        {
            Frame_Stage = FRAME_STATE_USBMAN;
            Display_Set_All(0); 
            if( EVT_FILE_DETECTED==evt)
                Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_USB_UPDATE_NOTIES_ID,0,0,0,0,ShowCommonTextBox);
            else
                Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_USB_BTUPDATE_NOTIES_ID,0,0,0,0,ShowCommonTextBox);
        }
        break;
        case EVT_FILE_UPDATE_CANCELED:
        case EVT_USB_REMOVED:
        {
            Frame_Stage = FRAME_STAGE_USER;
            User_Set_VirtualKey(VK_INITIAL);
        }
        break;
        case EVT_FILE_UPDATE_CONFIRMED:
        {
            Display_Set_All(0);
            
            BOR_Set_Passcode(0);
            Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_RETOOTING_ID,0,0,0,0,ShowCommonTextBox);		

            BOR_Set_Passcode(0);
            Beep_Set_Beeps(6,10,10);
            while(Beep_Get_OK()==false)
            {
                Display_Updata_Data(true);
//                LED_Updata(true);
            }
            HAL_Reset_System(); 
        }
        break;
        case EVT_FILE_BT_UPDATE_CONFIRMED:
            Bluetooth_Set_VirtualKey(VK_BT_UPDATE);   //zsf
        break;
        default: break;
    }
}


/***************************************************************************//**
* @brief
*   void RF_Events_Handler(RF_EVENT rf_evts) 
*
* @note
*  RF_EVENT???ù￡o
*   
* @param[in]:RF_EVENT rf_evts
* @param[out]:none
******************************************************************************/
#ifdef RF_MODULE_EXIST_   
void RF_Events_Handler(RF_EVENT rf_evts)
{
    UINT32 TempData = 0 ;
    
    switch(rf_evts)
    {
        case RF_ONLINE:
        {
            Event_Flag.Syncing = false;
            Event_Flag.Connected = true;
//			Event_Set_Wnd_Para(&g_wndRFIcon,0,0,0,0,0,ShowRFICON);
            break;
        }
        case RF_OFFLINE:
        {
            Event_Flag.Syncing = false;
            Event_Flag.Connected = false;
//			Event_Set_Wnd_Para(&g_wndRFIcon,0,0,0,0,0,ShowRFICON);			
            break;
        }
        case RF_SYNC_ING:
        {
//			Event_Set_Wnd_Para(&g_wndRFIcon,0,0xAA,0,0,0,ShowRFICON);	
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_RF_SYNC_ID,0,0,0,0,ShowCommonTextBox);

            Event_Flag.Syncing = true;
            Event_Flag.Connected = false;
        }
        break;
        case RF_SYNC_FINISH:
		{
			Event_Flag.Syncing = false;
			Event_Flag.Connected = true;
			
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SELECTUSER_ID,0,0,0,0,ShowCommonTextBox);
			Page_Push_OverlappedWindow(&g_wndBottomTextBox, &g_wndBottomTextBox);
			
//			Event_Set_Wnd_Para(&g_wndRFIcon,0,0,0,0,0,ShowRFICON);	
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_PASSPORTREADY_ID,0,3,0,0,ShowCommonTextBox);
		}
		break;
        case RF_SYNC_TIMEOUT://3
       	{
			Event_Flag.Syncing = false;
			Event_Flag.Connected = false;
				
            //push overlapped windows 1st 
            Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SELECTUSER_ID,0,0,0,0,ShowCommonTextBox);
            Page_Push_OverlappedWindow(&g_wndBottomTextBox, &g_wndBottomTextBox);

			//then display the Modal window
//			Event_Set_Wnd_Para(&g_wndRFIcon,0,0,0,0,0,ShowRFICON);
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_TIMEOUT_ID,0,3,0,0,ShowCommonTextBox);
		}
		break;
        case RF_TERRAIN_CHANGED:
        {
            TempData = RF_AP_Get_STB_Terrain_Data(MODE_TERRAIN_INCLINE)*5;;
            Program_Set_Parameter(VK_SET_INCLINE,&TempData);    
            break;
        }
        case RF_RESISTANCE_CHANGED:
        {
            TempData = RF_AP_Get_STB_Terrain_Data(MODE_TERRAIN_RESISTANCE);
            Program_Set_Parameter(VK_SET_RESISTANCE,&TempData);    
            break;
        }
        case RF_TERRAIN_RES_CHANGED:
        {
            TempData = RF_AP_Get_STB_Terrain_Data(MODE_TERRAIN_INCLINE);
            Program_Set_Parameter(VK_SET_INCLINE,&TempData);  
            TempData = RF_AP_Get_STB_Terrain_Data(MODE_TERRAIN_RESISTANCE);
            Program_Set_Parameter(VK_SET_RESISTANCE,&TempData);    
            break;
        }
        case RF_USER_CHANGED:
        {
            TempData = RF_AP_Get_STB_Terrain_Data(D_STARTUP_DATA_USERNUMBER);
            Program_Set_Parameter(VK_SET_USER,&TempData);    
            break;
        }
        case RF_SYNC_DATA_WHEN_STARTUP:
        {
            TempData=RF_AP_Get_STB_StartUp_Data(D_STARTUP_DATA_WORKOUTTIME);
            Program_Set_Parameter(VK_SET_WORKOUT_TIME,&TempData);    

            TempData = RF_AP_Get_STB_StartUp_Data(D_STARTUP_DATA_INCLINE)*5;
            Program_Set_Parameter(VK_SET_INCLINE,&TempData);
            
            TempData=RF_AP_Get_STB_StartUp_Data(D_STARTUP_DATA_RESISTANCE);
            Program_Set_Parameter(VK_SET_RESISTANCE,&TempData);    
            break;
        }
        case RF_NO_EVENT:
        default: break;
    }
}
#endif
#ifdef  __WIFI_FUNC__
void WIFI_Events_Handler(WIFI_EVENT_HANDLE wifi_evts)
{
    if(Frame_Stage == FRAME_STAGE_SAFFKEYOFF) return;
    switch(wifi_evts)
    {
    	case WIFI_EVENT_SUCCESS:
		{

		}
		break;
		case WIFI_EVENT_TIMEOUT:
		{

		}
		break;
		case WIFI_EVENT_FAIL:
		{

		}
		break;
        case  WIFI_EVENT_IDLE:
        {
            break;
        }
		case WIFI_EVENT_NO_IP:
		{
			Display_Set_All(0);
            Event_HideAllWnds(1);
            Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_NOIP_ID,0,0,0,0,ShowCommonTextBox);
		}
		break;
		case WIFI_EVENT_NO_MAC:
		{
			Display_Set_All(0);
            Event_HideAllWnds(1);
            Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_NOMAC_ID,0,0,0,0,ShowCommonTextBox);
		}
		break;
		case WIFI_EVENT_WIFI_IS_OK:
		{
			Display_Set_All(0);
            Event_HideAllWnds(1);
            Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_WIFIOK_ID,0,0,0,0,ShowCommonTextBox);
		}
		break;
		
        case WIFI_EVENT_RSCU_CHECKING:
        {
            snprintf(NameSprintBuffer,sizeof(NameSprintBuffer),"%s ","RSCU CHECKING");
            DisplayString(0,3,0,NameSprintBuffer);  
            Beep_Set_Beeps(2,10,10);while(Beep_Get_OK()==false){Display_Updata_Data(true);LCB_Process();}
            break;
        }
        case WIFI_EVENT_DOWNLOADING_SOFTWARE:
        {
            snprintf(NameSprintBuffer,sizeof(NameSprintBuffer),"%s ","DOWNLOADING SOFTWARE");
            DisplayString(0,3,0,NameSprintBuffer); 
            Beep_Set_Beeps(2,10,10);while(Beep_Get_OK()==false){Display_Updata_Data(true);LCB_Process();}
            break;
        }
        case WIFI_EVENT_DOWNLOAD_COMPLETED:
        {
            //              snprintf(NameSprintBuffer,sizeof(NameSprintBuffer),"%s ","DOWNLOAD COMPLETED");
            snprintf(NameSprintBuffer,sizeof(NameSprintBuffer),"%s ","pls reset ucb");
            DisplayString(0,3,0,NameSprintBuffer); 
            Beep_Set_Beeps(2,10,10);while(Beep_Get_OK()==false){Display_Updata_Data(true);LCB_Process();}
            //              HAL_Reset_System();
            break;
        }       
        case  WIFI_SIGNAL_CHANGED:
        {
            g_wndWiFiIcon.pfShow       = ShowWifiIcon;
            g_wndWiFiIcon.pfShow(&g_wndWiFiIcon);
            break;
        }

        case  WIFI_EVENT_ACTIVATED:
        {
            Display_Set_All(0);
            Event_HideAllWnds(1);
            Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_ACTIVATED_ID,0,0,0,0,ShowCommonTextBox);
			Beep_Set_Beeps(2,10,10);
        }
        break;
        case  WIFI_EVENT_AP_MODE:  
        {
            g_wndWiFiIcon._blinkCount = 0xAA ;
            g_wndWiFiIcon.pfShow       = ShowWifiIcon;
            g_wndWiFiIcon.pfShow(&g_wndWiFiIcon);
        }
        break;
        
        case WIFI_EVENT_INPUT_TIMEZONE:
        {
            Frame_Stage = FRMAE_STATE_WIFI;
            
            Display_Set_All(0);
            Event_HideAllWnds(1);
            Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_TIMEZONE_ID,0,0,0,0,ShowCommonTextBox);

            Event_Set_Wnd_Para(&g_wndRightBottomNumerical,0,0xAA,0,0,2,ShowTimezoneData);
        }
        break;
        case WIFI_EVENT_TIMEZONE_COMPLETE:
        {
            nCurrentPage = PAGE_SELECTUSER;
            Frame_Stage = FRAME_STAGE_USER;
            Event_Set_Virtual_key(VK_RESET);
        }
        break;
        case  WIFI_EVENT_OFFLINE:
        default: break;
    }
}
#endif

void Event_Modal_Timer_Handler(void)
{
    for(UINT16 i = 0; i < pages[nCurrentPage].nWindowCount; i++)
    {
        if( pages[nCurrentPage].pItem[i]->pWnd != NULL&&\
            pages[nCurrentPage].pItem[i]->pWnd->_isHidden!=1)
        {
            if(pages[nCurrentPage].pItem[i]->pWnd->_activetime >1 &&\
                pages[nCurrentPage].pItem[i]->pWnd->_activetime!=0xAA) 
            {
                pages[nCurrentPage].pItem[i]->pWnd->_activetime--;
            }
            else if(pages[nCurrentPage].pItem[i]->pWnd->_activetime==1)
            {
				pages[nCurrentPage].pItem[i]->pWnd->_activetime = 0 ;
				pages[nCurrentPage].pItem[i]->pWnd->_isHidden = 1;
				Win_Items *pPopedWnds;
				UINT8 cnt ;
				pPopedWnds = Page_Pop_OverlappedWindow(pages[nCurrentPage].pItem[i]->pWnd, &cnt);
                if(pPopedWnds)
                {
                    memcpy(pages[nCurrentPage].pItem[i]->pWnd,pPopedWnds,sizeof(Win_Items));//2016.02.24修改，把pop出的数据copy到page的位置
                    for(UINT8 i = 0;i < cnt; i++)
                    {
                        pPopedWnds[i]._isHidden = 0;
                        if(pPopedWnds[i].pfShow)
                            pPopedWnds[i].pfShow(&pPopedWnds[i]);
                    }
                }
				Page_Reset_Modal_Window(pages[nCurrentPage].pItem[i]->pWnd);
			}
		}
	}
}
void Event_Static_Timer_Handler(void)
{
	for(UINT16 i = 0; i < pages[nCurrentPage].nWindowCount; i++)
	{
        if( pages[nCurrentPage].pItem[i]->pWnd != NULL&&\
            pages[nCurrentPage].pItem[i]->pWnd->_isHidden!=1)
        {
            if(pages[nCurrentPage].pItem[i]->pWnd->_blinkDelayCount >1 &&\
                pages[nCurrentPage].pItem[i]->pWnd->_blinkDelayCount!=0xAA) 
            {
                pages[nCurrentPage].pItem[i]->pWnd->_blinkDelayCount--;
            }
            else if(pages[nCurrentPage].pItem[i]->pWnd->_blinkDelayCount==1)
                pages[nCurrentPage].pItem[i]->pWnd->_blinkDelayCount = 0 ;
        }
    }  
}



void Events_Blink_Timer_Handler(void)
{
    Timer_500msHigh_Level = !Timer_500msHigh_Level;
    
    for(UINT16 i = 0; i < pages[nCurrentPage].nWindowCount; i++)
    {
        if( pages[nCurrentPage].pItem[i]->pWnd != NULL&&\
            pages[nCurrentPage].pItem[i]->pWnd->_isHidden!=1)
        {
            if(pages[nCurrentPage].pItem[i]->pWnd->_blinkDelayCount==0 ||\
                pages[nCurrentPage].pItem[i]->pWnd->_blinkDelayCount ==0xAA) 
                {
                    //pages[nCurrentPage].pItem[i]->pWnd->_blinkCount-=2;
                if(pages[nCurrentPage].pItem[i]->pWnd->_blinkCount>2)
                {
                    if(pages[nCurrentPage].pItem[i]->pWnd->_blinkDelayCount==0 && Timer_500msHigh_Level)
                    {
                        pages[nCurrentPage].pItem[i]->pWnd->_blinkDelayCount = 0xAA;
                    }
                    if(pages[nCurrentPage].pItem[i]->pWnd->_blinkDelayCount == 0xAA)
                        pages[nCurrentPage].pItem[i]->pWnd->_blinkCount --;
                    
                    if(pages[nCurrentPage].pItem[i]->pWnd->_blinkCount ==0xA8 )//?a0xAAê±ò??±éá??
                        pages[nCurrentPage].pItem[i]->pWnd->_blinkCount = 0xAA;
                    
                    pages[nCurrentPage].pItem[i]->pWnd->pfShow(pages[nCurrentPage].pItem[i]->pWnd);
                }
                else if(pages[nCurrentPage].pItem[i]->pWnd->_blinkCount==2)
                {
                    pages[nCurrentPage].pItem[i]->pWnd->_blinkCount = 0 ;
					if(pages[nCurrentPage].pItem[i]->pWnd->_isHidden == 0xAA )
	            	{
	            		pages[nCurrentPage].pItem[i]->pWnd->_isHidden = 0 ;
						martix_type = g_martix_modal_type;
					}					
                }
            }
        }
    }
}

void Event_Refresh_Timer_Handler(void)
{
    if(Event_Flag.Demo_RefreshConter++>= DEMO_REFRESH_TIME)
    {
        Event_Flag.Demo_RefreshConter = 0;
        if(Frame_Stage==FRMAE_STATE_DEMOMODE)
        {
            Display_DEMO_Process();
        }
    }
	if(Frame_Stage == FRAME_STAGE_SAFFKEYOFF) return;
	
    if((Frame_Stage == FRAME_STAGE_USER)||(Frame_Stage == FRAME_STAGE_PRGSETUP)||(Frame_Stage == FRAME_STAGE_PRGRUN)||(Frame_Stage == FRAME_STAGE_AGINGTEST))
    {
        ShowPage(nCurrentPage,0);
    }
}

extern char Str_OldBuff[50];

void Demo_Events_Handler(void)
{
  //    if(Frame_Stage!=FRMAE_STATE_DEMOMODE) return;
  if(Demo_Event_Quit == 1 )
  {
    Demo_Event_Quit = 0;
    
    //		Program_Set_VirtualKey((UINT16)VK_RESET|FRAME_STAGE_PRGSETUP);
    //        User_Set_VirtualKey((UINT16)VK_RESET|FRAME_STAGE_USER);
    
    Frame_Stage = Frame_OldStage;
    nCurrentPage = g_BackPage;
    //        LED_Clear_string();
    if( nCurrentPage == PAGE_SAFETYKE_OFF)		
    {
      Display_Set_All(0);
      Event_HideAllWnds(1);
      ShowPage(nCurrentPage,1);
    }
  }
}

void Demo_Set_VirtualKey(UINT16 vk)
{
    UINT16 TempKey = vk & 0xFF00;
    UINT8 key_value = (UINT8)(vk&0x00FF);
    if(TempKey == FRMAE_STATE_DEMOMODE && key_value!=VK_NULL)
    {
        Demo_Event_Quit = 1;
    }
}

bool Event_Get_RF_Connected(void)
{
    return(Event_Flag.Connected);
}


bool Event_Get_RF_Syncing(void)
{
    return(Event_Flag.Syncing);
}

void Event_Clr_ChangeDisplayFlag(void)
{
    Event_Flag.PageChanged = 0;
}
