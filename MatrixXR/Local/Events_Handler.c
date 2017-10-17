#include "Events_Handler.h"



//void Demo_Set_VirtualKey(UINT16 vk);

//------------------------------Private Marcos----------------------------------------
#define EVT_SPRINTF_BUFFER_SIZE  65
#define DEMO_REFRESH_TIME 5
//------------------------------Private variables--------------------------------------

#define SUPPORTED_MACHINE_TYPE_TOTAL	14

__no_init struct
{
    unsigned Syncing:1;
    unsigned Refresh_DemoDis:1;
	unsigned PageChanged:1;
	unsigned DisChangeForHR:1;
    
	unsigned DisAllOn:1;
    unsigned DisAllOff:1;
    unsigned isSaftyKeyOff:1;
    UINT8 Demo_RefreshConter;
}Event_Flag;
typedef  void (*showfunc)(void *p);
__no_init showfunc BackShowFunc;
__no_init STRINGID BackString_For_Syncing;
__no_init Win_Items g_BackMiddleWnd;//0,8
__no_init VR_PAGE  g_BackPage;
__no_init Matrix_Type g_martix_resume_type;
__no_init Matrix_Type g_martix_modal_type;
__no_init static FRAME_STAGE Frame_Stage;
__no_init unsigned char NameSprintBuffer[EVT_SPRINTF_BUFFER_SIZE];
__no_init UINT8 EXTERNAL_EVENTS_BUFFER[EVT_ID_TOTAL];
extern PAGES const pages[PAGE_TOTAL];
__no_init     	MAIN_STAGE Main_Stage;
//------------------------------Private function protypes--------------------------------
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
    g_wndMiddleBottomTextBox._isHidden = ishidden;
}

/*@brief: this function is used to disable all flashing elements */
static void Event_DisableFlashingAllWnds(void)
{
    for(int i=0;i<(pages[nCurrentPage].nWindowCount-2);i++)
    {
        if(pages[nCurrentPage].pItem[i]->pWnd->_isHidden == 0xAA)
        {
            pages[nCurrentPage].pItem[i]->pWnd->_isHidden = 0 ;
        }
    }
    
    g_wndMiddleNumerical._blinkCount = 0;
    g_wndMiddleNumerical._blinkDelayCount= 0;
    g_wndRightTopNumerical._blinkCount = 0;
    g_wndRightTopNumerical._blinkDelayCount = 0;
    g_wndLeftBottomNumerical._blinkCount = 0;
    g_wndLeftBottomNumerical._blinkDelayCount = 0;
    g_wndRightBottomNumerical._blinkCount = 0;
    g_wndRightBottomNumerical._blinkDelayCount = 0;
}


/*@brief: this function is used to convert phykey into virtual key */
VR_KEY Event_VK_Converter(char phyKey)
{
    VR_KEY keyret = VK_NULL;
		
    if(phyKey  == KB_NONE || phyKey ==KB_NA || phyKey ==0xAA)
        return(VK_NULL);
    
    switch(phyKey)
    {
        case KB_START:keyret = VK_PAUSE_START;   break;
        case KB_SPEED_LONGPRESS:
		{
			if(Main_Stage == STAGE_PRG_RUN && (SystemConfig_Get_Char(MACHINE_TYPE_UINT8) == TREADMILL))
        			keyret = VK_SPEEDUP_LONG_PRESS;
			else
				keyret = VK_SPEEDUP;	
        	}
        break;
        case KB_RELEASED:
        keyret = VK_SPEEDUP_RELEASE;
        break;
        case KB_RESISTANCE_LONGPRESS:
		{
			if(Main_Stage == STAGE_PRG_RUN && (SystemConfig_Get_Char(MACHINE_TYPE_UINT8) == CLIMB_MILL))
        			keyret = VK_RESISTANCEUP_LONGPRESSED;
			else
				keyret = VK_RESISTANCEUP;	
        	}
        break;
        case KB_STOP:  		keyret = VK_PAUSE; break;
        //case KB_Hold_9:		keyret = VK_CLEAR_BOOT; break;
       // case KB_Hold_0:		keyret = VK_ENG; break;
				
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
        
        case KB_ENTER:
        case KB_CHANGE_DISPLAY:keyret = VK_ENTER_CHANGEDIS;break;
        case KB_SPEED_FAST:
        if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8) == TREADMILL)
            keyret = VK_SPEEDUP;	
        else
            keyret = VK_RESISTANCEUP;	
        break;
        case KB_SPEED_SLOW:
        if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8) == TREADMILL)
            keyret = VK_SPEEDDOWN;	
        else
            keyret = VK_RESISTANCEDOWN;	     
        break;
        case KB_INCLINE_UP:
        if(SystemConfig_Get_Char(DRIVE_TYPE_UINT8) == ECB_JIS || SystemConfig_Get_Char(DRIVE_TYPE_UINT8) == BIKE_0C_JIS)
            keyret = VK_NULL;	
        else
            keyret = VK_INCLINEUP;	
        break;
        case KB_INCLINE_DOWN:
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
        keyret = VK_UP;	
        break;
        case KB_ARROW_DOWN:
        keyret = VK_DOWN;
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
        case KB_SAVE:
        keyret = VK_SAVE;	
		break;	
        case KB_LUBEBELTRESET:
        keyret = VK_LUBEBELT_RESET;	
		break;	
        default: break;
    }
    return(keyret);
}

ReturnEventHandler Event_UTL_BlueBelt_Process(LUBEBELT_EVENT evt,ReturnEventHandler flag)
{
	ReturnEventHandler ret = flag;
	
	switch (evt)
	{
		case LUBEBELT_EVENT_NOTICE:
		{
			Display_Set_All(0);
			Event_HideAllWnds(1);
            if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==CLIMB_MILL)
			    Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_CLIMB_MILL_LUBE_BELT_ID,0,0,0,0,ShowCommonTextBox);
			else
                Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_LUBE_BELT_ID,0,0,0,0,ShowCommonTextBox);
            ret = RET_UNPROCESS_UNDISPLAY;			
		}
		break;	
		case LUBEBELT_EVENT_PROCESSED:
		{
			ret = RET_UNPROCESS_UNDISPLAY;	
		}
		break;
		case LUBEBELT_EVENT_CLEARD:
		default:
		break;	
	}
	return ret;
}

//Event_UTL_BurnTest_Process:2016.3.05 Peter done
ReturnEventHandler Event_UTL_BurnTest_Process(BURNTEST_EVENT evt,ReturnEventHandler flag)
{
	extern    UINT8 BLINK_DISPLAY_CHANNEL ;
	extern     UINT8 STATIC_DISPLAY_CHANNEL;
	extern     UINT8 MODAL_WINDOW_CHANNEL;
	extern     UINT8 REFRESH_WINDOW_CHANNEL;
	ReturnEventHandler ret = flag;
	switch (evt)
	{
		case E_PROCESSED:
			{
				ret = RET_UNPROCESS_UNDISPLAY;
			}
			break;
		case E_AGINGOFF:
			{
				Timer_Set_Timer_Start(BLINK_DISPLAY_CHANNEL);
				Timer_Set_Timer_Start(STATIC_DISPLAY_CHANNEL);
				Timer_Set_Timer_Start(MODAL_WINDOW_CHANNEL);
				Timer_Set_Timer_Start(REFRESH_WINDOW_CHANNEL);	
                	HAL_BOR_Enable_ExtPower(Power_Channel_ERP,0);
				ret = RET_REFRESH_DISPLAY;			
			}
			break;	
		case E_AGINGON:
			{
				Timer_Set_Timer_Paused(BLINK_DISPLAY_CHANNEL);
				Timer_Set_Timer_Paused(STATIC_DISPLAY_CHANNEL);
				Timer_Set_Timer_Paused(MODAL_WINDOW_CHANNEL);
				Timer_Set_Timer_Paused(REFRESH_WINDOW_CHANNEL);	
	            Display_Set_All(true);
                HAL_BOR_Enable_ExtPower(Power_Channel_ERP,1);
				ret = RET_UNPROCESS_UNDISPLAY;			
			}
			break;	
		default:
			break;	
	}
	return ret;
}


ReturnEventHandler Event_UTL_ERP_Process(ERP_EVENT evt,ReturnEventHandler flag)
{
	extern    UINT8 BLINK_DISPLAY_CHANNEL ;
	extern     UINT8 STATIC_DISPLAY_CHANNEL;
	extern     UINT8 MODAL_WINDOW_CHANNEL;
	extern     UINT8 REFRESH_WINDOW_CHANNEL;

	ReturnEventHandler ret = flag;
    if( evt == ERP_NO_EVENT ) return ret;
    
    ret = RET_UNPROCESS_UNDISPLAY;
	switch(evt)
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
            Display_Set_All(0);
            g_wndBottomTextBox._z = NULL;				
//            STRINGID Tempid = (STRINGID)(evt)-(STRINGID)(NUM_0) + STR_0_TITLE_ID;
            STRINGID Tempid = (STRINGID)(evt) + STR_0_TITLE_ID;
            Event_Set_Wnd_Para(&g_wndBottomTextBox,Tempid,0,0,0,0,ShowCommonTextBox);		
        }
		break;
        case Enter_ERP:
        {
            Display_Set_All(0);
        }
        break;
		case REFRESH_DEMO:
		{
			Timer_Set_Timer_Paused(BLINK_DISPLAY_CHANNEL);
			Timer_Set_Timer_Paused(STATIC_DISPLAY_CHANNEL);
			Timer_Set_Timer_Paused(MODAL_WINDOW_CHANNEL);
			Timer_Set_Timer_Paused(REFRESH_WINDOW_CHANNEL);
			
			Display_DEMO_Process();
		}
		break;
        case ERP_QUIT_DEMO:
        {
			Timer_Set_Timer_Start(BLINK_DISPLAY_CHANNEL);
			Timer_Set_Timer_Start(STATIC_DISPLAY_CHANNEL);
			Timer_Set_Timer_Start(MODAL_WINDOW_CHANNEL);
			Timer_Set_Timer_Start(REFRESH_WINDOW_CHANNEL);
			Display_Set_All(0);		
			ret = RET_REFRESH_DISPLAY;
        }
        break;
		case ERP_PROCESSED:
        break;
        default: break;
    }
	
	return ret;
}


ReturnEventHandler Event_UTL_Error_Code_Process(ERR_EVENT evt,ReturnEventHandler flag)
{
	ReturnEventHandler ret = flag;
    if( evt == S_NO_EVENT ) return ret;
    
    ret = RET_UNPROCESS_UNDISPLAY;
	switch(evt)
	{
		case ERR_PROCESSED:
		{
				 ret = RET_UNPROCESS_UNDISPLAY;
		}
		break;
        case ERR_EVT_NO_SPM:
        {

        }
        break;
		case ERR_EVT_IRSENEOR://显示错误信息IRSENEOR
		{
//			Display_Set_All(0);
//			nCurrentPage = PAGE_SAFETYKEY_OFF;
//			ShowPage(nCurrentPage,1);
//			Beep_Set_Beeps(6,5,5);	
//			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_IR_SENSOR_ERROR_ID,0,0,0,0,ShowCommonTextBox);	
//			ret = RET_UNPROCESS_UNDISPLAY;
		}
		break;
        case ERR_EVT_CONTROL_ZONE://显示错误信息CONTROL ZONE
		{
//			Display_Set_All(0);
//			nCurrentPage = PAGE_SAFETYKEY_OFF;
//			ShowPage(nCurrentPage,1);
//			Beep_Set_Beeps(6,5,5);	
//			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_IR_CONTROLZONE_ERROR_ID,0,0,0,0,ShowCommonTextBox);	
//			ret = RET_UNPROCESS_UNDISPLAY;
		}
		break;
        case ERR_EVT_UCB_EMERGENCY://显示错误信息UCB EMERGENCY
		{
			Display_Set_All(0);
            Main_Stage = STAGE_USER ;
			nCurrentPage = PAGE_SAFETYKEY_OFF;
			ShowPage(nCurrentPage,1);
			Beep_Set_Beeps(6,5,5);	
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_IR_UCBEMERG_ERROR_ID,0,0,0,0,ShowCommonTextBox);	
			ret = RET_UNPROCESS_UNDISPLAY;
		}
		break;
        case ERR_EVT_HANDRAIL_EMERGENCY://显示错误信息HANDRAIL EMERGENCY
		{
			Display_Set_All(0);
            Main_Stage = STAGE_USER ;
			nCurrentPage = PAGE_SAFETYKEY_OFF;
			ShowPage(nCurrentPage,1);
			Beep_Set_Beeps(6,5,5);	
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_IR_HANDRAILEMERG_ERROR_ID,0,0,0,0,ShowCommonTextBox);	
			ret = RET_UNPROCESS_UNDISPLAY;
		}
		break;
		case Safekey_off_to_on:
		{
			extern void Main_Initial_Data(INIT_TYPE type);
			Main_Initial_Data(TYPE_RST_INIT );
			Beep_Set_Beeps(6,10,10);	
            ret = RET_UNPROCESS_UNDISPLAY;
		}
		break;
		case Safekey_on_to_off:
		{
		   Display_Set_All(0);
		   Main_Stage = STAGE_USER ;
		   nCurrentPage = PAGE_SAFETYKEY_OFF;
           ShowPage(nCurrentPage,1);
		   Beep_Set_Beeps(6,5,5);	
           ret = RET_UNPROCESS_UNDISPLAY;
		}
		break;
        case S_ERROR_EVENT:
		{
            Display_Set_All(0);
            Main_Stage = STAGE_USER ;
            nCurrentPage = PAGE_ERRORCODE_INFO;
            ShowPage(nCurrentPage,1);
            Beep_Set_Beeps(6,5,5);	
//            if(SystemConfig_Get_Char(SHOW_ERROR_FLAG_UCHAR)==1&&UTL_Get_ErrorLevel()==0)
//            {
//                Event_Set_Wnd_Para(&g_wndBottomTextBox,0,0,3,0,0,ShowErrorCode);	
//            }
//            else if(UTL_Get_ErrorLevel()==1)
//            {
//                Event_Set_Wnd_Para(&g_wndBottomTextBox,0,0,0,0,0,ShowErrorCode);	
//            }
            ret = RET_UNPROCESS_UNDISPLAY;
		}
		break;
        case S_ERROR_RELEASED:
        {
            extern void Main_Initial_Data(INIT_TYPE type);
			Main_Initial_Data(TYPE_RST_INIT );
			Beep_Set_Beeps(6,10,10);	
            ret = RET_UNPROCESS_UNDISPLAY;
//            ret = RET_REFRESH_DISPLAY ;
        }
        break;
		default:
        break;
	}
	return ret;
}

ReturnEventHandler Event_UTL_HR_Blink_Process(UINT8 NAData,ReturnEventHandler flag)
{
#define HR_EVENT_OFF 0	
#define	HR_EVENT_ON  1   
	ReturnEventHandler ret = flag;
    UCHAR hrstatus = Program_Get_Status()>=PS_PAUSED && (Program_Get_Status()<=PS_COOLDOWN)&&\
		(Event_Flag.DisChangeForHR ||nCurrentPage == PAGE_WORK_TM2 || nCurrentPage == PAGE_WORK_BK2 ||nCurrentPage == PAGE_WORK_AS2 ||nCurrentPage == PAGE_WORK_BK2_EUR30 ||nCurrentPage ==PAGE_WORK_AS2_A30 ||nCurrentPage ==PAGE_WORK_CLIMBMILL2);
	UCHAR evt = (Midware_Heart_Get_Blink())&& hrstatus;
	//if(!( (Program_Get_Status()>=PS_PAUSED) && (Program_Get_Status()<=PS_COOLDOWN)&&(Event_Flag.DisChangeForHR == 1||nCurrentPage == PAGE_WORK_TM2 || nCurrentPage == PAGE_WORK_BK2 ||nCurrentPage == PAGE_WORK_AS2)))
	if(evt == HR_EVENT_OFF)
	{
		Display_HEART_ICON(0,"");
	}
	else
	{
		Display_HEART_ICON(1,"");
	}
    return ret;
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
		case CLIMB_MILL:
        {
//            nCurrentPage =  PAGE_WORK_CLIMBMILL1;
            nCurrentPage =  PAGE_WORK_CLIMBMILL2;
        }
        break;
		case ASCENT:		
		{
			if(( Program_Get_ProgramID()==PS_CONSTANT_WATTS)||(Program_Get_ProgramID()==PS_THR_ZONE ))
			{
				if(  SystemConfig_Get_Char(DRIVE_TYPE_UINT8)== EP_0B_JIS)
					nCurrentPage = PAGE_WORK_AS2_A30;
				else
					nCurrentPage = PAGE_WORK_AS2;
			}
			else
			{
				nCurrentPage = PAGE_WORK_AS1;
			}
		}
		break; 
		case BIKE:
		case ELLIPTICAL:	
		{
			if(( Program_Get_ProgramID()==PS_CONSTANT_WATTS) ||(Program_Get_ProgramID()==PS_THR_ZONE ))
			{
				if(  SystemConfig_Get_Char(DRIVE_TYPE_UINT8)== DIGITAL_ECB)
					nCurrentPage = PAGE_WORK_BK2_EUR30;
				else
					nCurrentPage = PAGE_WORK_BK2;
			}
			else
			{
				nCurrentPage = PAGE_WORK_BK1;
			}
		}
		break; 
		default:break;
	}
	martix_type = (Matrix_Type)(Program_Get_DefaultDotMatrix());
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
	martix_type = (Matrix_Type)(Program_Get_DefaultDotMatrix());	
    g_martix_modal_type = Show_None;
    
#ifdef FUNCTION_AUTO_MODEL     
    if(SystemConfig_Get_Char(IS_MACHINE_CONFIGED_UCHAR)!=0xAA)
        Frame_Stage = FRAME_STAGE_MODEL ;
    else
#endif 
        Frame_Stage = FRAME_STAGE_USER; 
    
    memset(&Event_Flag,0,sizeof(Event_Flag));
    memset(NameSprintBuffer, '\0', EVT_SPRINTF_BUFFER_SIZE);
	memset(&g_BackMiddleWnd,0,sizeof(Win_Items));
    BackString_For_Syncing = STR_NULL_ID;
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



VR_KEY Event_Set_Virtual_key(VR_KEY Temp_VK)
{
	VR_KEY retvk  = Temp_VK;
	if(retvk==VK_NULL) return retvk;

	switch(Main_Stage)
	{
		case STAGE_USER:
		{
			retvk = UTL_BurnTest_Set_VK(retvk);
			retvk = UTL_ERP_Set_VK(retvk);
			retvk = UTL_ERR_Set_VK(retvk);
			retvk = USBManager_Set_VirtualKey(retvk);
#ifdef  __WIFI_FUNC__
	        retvk = Wifi_Set_VirtualKey(retvk);
#endif
#ifdef RF_MODULE_EXIST_   
     		retvk = RF_Set_VirtualKey(retvk);
#endif  
			retvk = UTL_LubeBelt_Set_VK(retvk);
			retvk = User_Set_VirtualKey(retvk);
		}
		//break;
		case STAGE_PRG_SETUP :
		{
			retvk = UTL_BurnTest_Set_VK(retvk);
			retvk = UTL_ERP_Set_VK(retvk);
			retvk = UTL_ERR_Set_VK(retvk);
			retvk = USBManager_Set_VirtualKey(retvk);
#ifdef  __WIFI_FUNC__
	        retvk = Wifi_Set_VirtualKey(retvk);
#endif
#ifdef RF_MODULE_EXIST_   
     		retvk = RF_Set_VirtualKey(retvk);
#endif  
			retvk = Program_Set_VirtualKey(retvk);	
		}
		//break;
		case STAGE_PRG_RUN :
		{
			retvk = Program_Set_VirtualKey(retvk);	
		}
		//break;
		case STAGE_ENG_MODE :
		{
			retvk = Engmode_Set_VirtualKey(retvk);
			retvk = UTL_ERP_Set_VK(retvk);
		}
		break;
		default:
		break;
	}
    return retvk;
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
    VR_KEY Temp_VK = Event_VK_Converter(nKey);
    Temp_VK = Event_Set_Virtual_key(Temp_VK);
	
    if(Temp_VK==VK_CLEAR_BOOT && (Main_Stage == STAGE_USER))
    {
        Display_Set_All(0);
        UINT8 boot =0xFF;
        SystemConfigProcess(SVK_TOGGLE_FIRSTBOOT,&boot,sizeof(boot));
        SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
        BOR_Set_Passcode(0);
        g_wndBottomTextBox._string_id = STR_REBOOTING_ID;
        g_wndBottomTextBox.pfShow       = ShowCommonTextBox;//ShowInitialTitle;
        g_wndBottomTextBox.pfShow(&g_wndBottomTextBox);
        Beep_Set_Beeps(6,10,10);
        while(Beep_Get_OK()==false)
        {
#ifdef WDT_ENABLE
            HAL_Clear_WatchDog();
#endif 
            Display_Updata_Data(true);
        }
        HAL_Reset_System(); 
    }

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
			STRINGID tempid = STR_ENGLISH_ID+Model_Get_Language_Index();
			Event_Set_Wnd_Para(&g_wndBottomTextBox,tempid,0,0,0,0,ShowCommonTextBox);
			Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_LANGUAGE_ID,0,0,0,0,ShowCommonTextBox);
		}
		break;
        case MODEL_EVENT_FLASH_HAVE_NO_LANGUAGE:
        {
            Event_Set_Wnd_Para(&g_wndBottomTextBox,0,0,0,0,0,ShowUpGradeLanguagePackage);	
            Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_LANGUAGE_ID,0,0,0,0,ShowCommonTextBox);
        }
        break;
		case MODEL_EVENT_MODEL_CHANGED:
		{
			STRINGID tempid = Model_Get_Set_Slot()+STR_TF30_ID;
			
			Event_Set_Wnd_Para(&g_wndBottomTextBox,tempid,0,0,0,0,ShowCommonTextBox);
			Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_MACHINE_ID,0,0,0,0,ShowCommonTextBox);		
		}
		break;
		case MODEL_EVENT_UNIT_CHANGE:
		{
			STRINGID tempid = STR_METRIC_ID;
			 if(Model_Get_Set_Slot()==STANDARD)
            {
                tempid = STR_STANDARD_ID;
            }

			Event_Set_Wnd_Para(&g_wndBottomTextBox,tempid,0,0,0,0,ShowCommonTextBox);
			Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_UNIT_ID,0,0,0,0,ShowCommonTextBox);		
		}
		break;
		case MODEL_EVENT_COMPLETE:
		{
            extern void Main_Init_Model_Data(void);
			Main_Init_Model_Data();
	        nCurrentPage = PAGE_SELECTUSER;
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
ReturnEventHandler User_Events_Handler(USER_EVENT evt,ReturnEventHandler flag)
{
Event_Set_Wnd_Para(&g_wndRFIcon,0,0,0,0,0,ShowRFICON);//by ckm
	ReturnEventHandler ret = flag;
	if(evt == USER_EVT_NONE ) return flag;
	
	if(evt != USER_EVT_NONE && evt < USER_EVENT_TOTAL)
    {
		Display_Set_All(0);
		Event_HideAllWnds(1);
    }
	ret = RET_UNPROCESS_UNDISPLAY;	 	
    switch(evt)
    {
        case USER_CHANGE_EVT:
        {
	        if(User_Get_Name() == NULL)
	        {
	        	STRINGID tempid = STR_GUEST_ID + User_Get_UserSlot();
				Event_Set_Wnd_Para(&g_wndBottomTextBox,tempid,0,0,0,0,ShowCommonTextBox);		
	        }
	        else
	        {
	        	Event_Set_Wnd_Para(&g_wndBottomTextBox,0,0,0,0,0,ShowUsrName);		
	        }
        }
        break;
        case USER_EDIT_SELECTED_EVT:
        {
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_EDIT_A_USER_ID,0,0,0,0,ShowCommonTextBox);				
        }
        break;
        case USER_DELETE_SELECTED_EVT:
        {
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_DELETE_A_USER_ID,0,0,0,0,ShowCommonTextBox);						
        }
        break;
        case USER_WEIGHT_CHANGE_EVT:
        {
			Event_Set_Wnd_Para(&g_wndRightTopNumerical,STR_DELETE_A_USER_ID,0xAA,0,0,2,ShowWeightData);						

			STRINGID tempid;
	        if(Get_User_Unit()==STANDARD)
	        {
	             tempid = STR_LBS_ID;
	        }
	        else
	        {
	            tempid = STR_KG_ID;
	        }
			Event_Set_Wnd_Para(&g_wndRightTopTextBox,tempid,0,0,0,0,ShowWeightUnit);						
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SET_WEIGHT_ID,0,0,0,0,ShowCommonTextBox);						
        }
        break;
        case USER_AGE_CHANGE_EVT:
        {
			Event_Set_Wnd_Para(&g_wndRightTopNumerical,0,0xAA,0,0,2,ShowAgeData);						
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SET_AGE_ID,0,0,0,0,ShowCommonTextBox);						
        }
        break;
        case USER_GENDER_CHANGE_EVT:
        {
        	Event_Set_Wnd_Para(&g_wndRightTopNumerical,0,0,0,0,0,ShowSetGenderData);						
        	Event_Set_Wnd_Para(&g_wndRightTopTextBox,0,0,0,0,0,ShowMaleTitle);						
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SET_GENDER_ID,0,0,0,0,ShowCommonTextBox);						
        }
        break;
        case USER_SYNC_INPROGRESS_EVT:
        {
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_UPDATING_USER_INFO_ID,0,0,0,0,ShowCommonTextBox);						
        }
        break; 
        case USER_SYNC_TIMEOUT_EVT:
        {
            Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SELECT_USER_ID,0,0,0,0,ShowCommonTextBox);
            Page_Push_OverlappedWindow(&g_wndBottomTextBox, &g_wndBottomTextBox);

			Event_Set_Wnd_Para(&g_wndRFIcon,0,0,0,0,0,ShowRFICON);
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_TIME_OUT_ID,0,3,0,0,ShowCommonTextBox);
        }
        break;
        case USER_SYNC_FAIL_EVT:
        {
            Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SELECT_USER_ID,0,0,0,0,ShowCommonTextBox);
            Page_Push_OverlappedWindow(&g_wndBottomTextBox, &g_wndBottomTextBox);

			Event_Set_Wnd_Para(&g_wndRFIcon,0,0,0,0,0,ShowRFICON);
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
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_ENTER_PASSCODE_ID,0,0,0,0,ShowPasscode );
        }
        break;
        case INPUT_PASSCODE_EVT:
        {
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_ENTER_PASSCODE_ID,0,0,0,0,ShowCommonTextBox);
        }
        break;
        case LOGIN_INPROCESS_EVT:
        {
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_PLEASE_WAIT_ID,0,0,0,0,ShowCommonTextBox);
        }
        break;
        case LOGIN_SUCCESS_EVT:
        {
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_WELCOME_ID,0,0,0,0,ShowCommonTextBox);
        }
        break;
        case USER_SETTING_COMPLETE:
        {
			Main_Stage = STAGE_PRG_SETUP ;
            Program_Set_VirtualKey(VK_INITIAL);
			nCurrentPage = PAGE_PROGRAM;
            ShowPage(nCurrentPage,1);
        }
        break;
        case LOGIN_FAIL_EVT:
        {
            Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SELECT_USER_ID,0,0,0,0,ShowCommonTextBox);
            Page_Push_OverlappedWindow(&g_wndBottomTextBox, &g_wndBottomTextBox);

			Event_Set_Wnd_Para(&g_wndRFIcon,0,0,0,0,0,ShowRFICON);
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_INVALID_USER_PIN_ID,0,3,0,0,ShowCommonTextBox);
        }
        break;

        case USER_RESET_EVT:
        {
			extern void Main_Initial_Data(INIT_TYPE type);
			Main_Initial_Data(TYPE_RST_INIT );	
        }
        break;
    	case DELETE_SUCCESS_EVT:
		{
			//push overlapped windows 1st 
            Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SELECT_USER_ID,0,0,0,0,ShowCommonTextBox);
            Page_Push_OverlappedWindow(&g_wndBottomTextBox, &g_wndBottomTextBox);

			//then display the Modal window
			Event_Set_Wnd_Para(&g_wndRFIcon,0,0,0,0,0,ShowRFICON);
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SUCCESS_ID,0,3,0,0,ShowCommonTextBox);
		}
		break;
		case DELETE_FALI_EVT:
		{
			//push overlapped windows 1st 
            Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SELECT_USER_ID,0,0,0,0,ShowCommonTextBox);
            Page_Push_OverlappedWindow(&g_wndBottomTextBox, &g_wndBottomTextBox);

			//then display the Modal window
			Event_Set_Wnd_Para(&g_wndRFIcon,0,0,0,0,0,ShowRFICON);
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_WIFI_FAIL_ID,0,3,0,0,ShowCommonTextBox);
		}
		break;	
		case USER_INIT_EVT:
		{
			nCurrentPage = PAGE_SELECTUSER ;
			ShowPage(nCurrentPage,1);
			//Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SELECT_USER_ID,0,3,0,0,ShowCommonTextBox);
		}
		break;
		case USER_EVENT_START_WORKOUT:
		{
			Main_Stage = STAGE_PRG_RUN ;
			nCurrentPage = PAGE_PROGRAM;
		}
        break;
		case USER_EVENT_ENTER_ENGMODE:
		{
			Main_Stage = STAGE_ENG_MODE;
			nCurrentPage = PAGE_ENG0_0_DisplayTest;
		}
		break;
        default:
		{
			ret = RET_UNPROCESS_UNDISPLAY;	 	
        }
        break;
    }    
    g_wndWiFiIcon.pfShow     = ShowWifiIcon;
    g_wndWiFiIcon.pfShow(&g_wndWiFiIcon);
    
    g_wndRFIcon.pfShow       = ShowRFICON;
    g_wndRFIcon.pfShow(&g_wndRFIcon); 

	return ret;
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
ReturnEventHandler Program_Events_Handler(PROGRAM_PENDING_EVTS program_evts,ReturnEventHandler flag)
{
	ReturnEventHandler ret = flag;
    if(program_evts.evt == PROGRAM_NONE_EVENT) return flag;
    
    ret = RET_UNPROCESS_UNDISPLAY;
		
    if(program_evts.evt != PROGRAM_NONE_EVENT && program_evts.evt != PROGRAM_EVENT_PROCESSED &&\
		 program_evts.evt!= WORKOUT_DATA_REFRESH_EVENT && program_evts.evt!= PROGRAM_PAUSE_EVENT)
    {
    	Display_Set_All(0);
        Event_HideAllWnds(1);
    }
    switch(program_evts.evt)
    {
    	case PROGRAM_EVENT_ENTER_ENGMODE:
		{
			Main_Stage = STAGE_ENG_MODE ;
            nCurrentPage = PAGE_ENG0_0_DisplayTest;
		}
		break;
        case BACK_END_EVENT:
        {
            nCurrentPage = PAGE_SELECTUSER;
			Main_Stage = STAGE_USER;
			ret = RET_REFRESH_DISPLAY;		
        }
		break;
        case PROGRAM_INITIAL_EVENT:
        {
		    Program_Select_ID(PS_MANUAL);
        }
        case PROGRAM_CHANGE_EVENT:
        {
            STRINGID stringid = STR_MANUAL_ID;
            if( Program_Get_ProgramID()==PS_INTERVALS )
                stringid = STR_INTERVALS_ID;
            else if( Program_Get_ProgramID()==PS_FATBURN )
                stringid = STR_FATBURN_ID;
            else if( Program_Get_ProgramID()==PS_THR_ZONE )
                stringid = STR_THR_ID;
            else if( Program_Get_ProgramID()==PS_ROLLING_HILLS )
                stringid = STR_ROLLINGHILLS_ID;
            else if( Program_Get_ProgramID()==PS_SPRINT8 )
                stringid = STR_SPRINT8_ID;
            else if( Program_Get_ProgramID()==PS_CONSTANT_WATTS )
                stringid = STR_CONSTANT_WATTS_ID;
            else if(Program_Get_ProgramID()== PS_LANDMARK)
                stringid = STR_LANDMARK_ID;

			martix_type = (Matrix_Type)(Program_Get_DefaultDotMatrix());
						
            Event_Set_Wnd_Para(&g_wndBottomTextBox,stringid,0,0,0,0,ShowCommonTextBox);
			Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,0,0,0,0,0,ShowMatrixData);        
        }
		break;
        case TIME_CHANGE_EVENT:
        {
            g_wndMiddleNumerical._z = 2;
			Event_Set_Wnd_Para(&g_wndMiddleNumerical,0,0XAA,0,0,2,ShowTimeData);
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SET_TIME_ID,0,0,0,0,ShowCommonTextBox);
			Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,0,0,0,0,0,ShowMatrixData);
        }
		break;
        case TARGET_CHANGE_EVENT:
        {
            STRINGID tempid = STR_SET_TARGET_ID + GetTargetID();
            g_wndMiddleNumerical._z = 2;
			Event_Set_Wnd_Para(&g_wndBottomTextBox,tempid,0,0,0,0,ShowCommonTextBox);
            Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,0,0,0,0,0,ShowMatrixData);
        }
        break;
        case HEART_CHANGE_EVENT:
        {
			g_wndRightBottomNumerical._z = 0;		
			Event_Set_Wnd_Para(&g_wndRightBottomNumerical,0,0XAA,0,0,2,ShowTargetHeartData);
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SET_TARGET_HEARTRATE_ID,0,0,0,0,ShowCommonTextBox);
			Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,0,0,0,0,0,ShowMatrixData);
        }
		break;
        case WATTS_CHANGE_EVENT:
        {
			g_wndRightBottomNumerical._z = 0;			
			Event_Set_Wnd_Para(&g_wndRightBottomNumerical,0,0XAA,0,0,2,ShowWattsData);
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SET_TARGET_WATTS_ID,0,0,0,0,ShowCommonTextBox);
			Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,0,0,0,0,0,ShowMatrixData);
           
        }
		break;
        case CALORIES_CHANGE_EVENT:
        {
            
        }
		break;		
        case DISTANCE_CHANGE_EVENT:
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
           	Event_Set_Wnd_Para(&g_wndRightBottomNumerical,0,0XAA,0,0,2,ShowLevelData);
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SET_LEVEL_ID,0,0,0,0,ShowCommonTextBox);
			Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,0,0,0,0,0,ShowMatrixData);
        }
		break;
        case CLIMBMILL_LEVEL_CHANGE_EVENT:
        {
            g_wndRightBottomNumerical._z = 0;		
           	Event_Set_Wnd_Para(&g_wndLeftBottomNumerical,0,0XAA,0,0,2,ShowClimbMillLevelData);
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SET_LEVEL_ID,0,0,0,0,ShowCommonTextBox);
			Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,0,0,0,0,0,ShowMatrixData);
        }
        break;
        case SPEED_CHANGE_EVENT:
        { 
            martix_type = Show_Speed;
			g_wndRightBottomNumerical._z = 1;
            Event_Set_Wnd_Para(&g_wndRightBottomNumerical,0,0XAA,0,0,2,ShowSpeedData);
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SET_SPEED_ID,0,0,0,0,ShowCommonTextBox);
			Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,0,0,0,0,0,ShowMatrixData);
        } 
		break;
        case INCLINE_CHANGE_EVENT:
        { 
            martix_type = Show_Incline;
			g_wndLeftBottomNumerical._z = 1;
            Event_Set_Wnd_Para(&g_wndLeftBottomNumerical,0,0XAA,0,0,2,ShowInclineData);
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SET_INCLINE_ID,0,0,0,0,ShowCommonTextBox);
			Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,0,0,0,0,0,ShowMatrixData);
        }
		break;
        case RESISTANCE_CHANGE_EVENT:
        {  
            martix_type = Show_Resistance;
			g_wndRightBottomNumerical._z = 0;	
            Event_Set_Wnd_Para(&g_wndRightBottomNumerical,0,0XAA,0,0,2,ShowResistanceData);
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SET_RESISTANCE_ID,0,0,0,0,ShowCommonTextBox);
			Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,0,0,0,0,0,ShowMatrixData);
        }
		break; 
        case WORKOUT_DATA_REFRESH_EVENT:
        { 
			if(program_evts.WorkOutEvts.EvtBit.Workout_Refresh_Data == 1)	
			{
				ShowPage(nCurrentPage,0);
			}
            if(program_evts.WorkOutEvts.EvtBit.Workout_Complete_Evt==1)
            {         
                Event_Flag.DisChangeForHR = 0 ;
				Event_Get_Page_matrix();
                Beep_Set_Beeps(6,10,10);
				ShowPage(nCurrentPage,1);
				Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_WORKOUT_COMPLETE_ID,0,0,0,0,ShowCommonTextBox);
		    }
//            if(program_evts.WorkOutEvts.EvtBit.Workout_Init_LCB==1)
//            {
//                extern void Main_Initial_Data(INIT_TYPE type);
//                Main_Initial_Data(TYPE_RST_INIT );
//                Beep_Set_Beeps(6,10,10);	
//                ret = RET_UNPROCESS_UNDISPLAY;
//            }
            if(program_evts.WorkOutEvts.EvtBit.Workout_ChangeDisplay==1)
            {
            	UINT8 WorkoutFinished = 0,Bottom_Middle_Dis = 0;
				if(g_wndBottomTextBox._string_id ==STR_WORKOUT_COMPLETE_ID )
				{
					WorkoutFinished = 1;
				}
				if(g_wndMiddleBottomTextBox._string_id !=0 && g_wndMiddleBottomTextBox._isHidden!=1)
				{
					Bottom_Middle_Dis = 1;
					memcpy(&g_BackMiddleWnd,&g_wndMiddleBottomTextBox,sizeof(Win_Items));
				}
                switch(nCurrentPage)
                {
                    case PAGE_WORK_TM1:
					{
	                        if(martix_type == Show_Speed)
								martix_type = Show_Incline;
							else
								martix_type = Show_Speed;
	                        nCurrentPage = PAGE_WORK_TM2;       
	            	}
					break;
	                 case PAGE_WORK_TM2:
					{
	                         if(martix_type == Show_Speed)
								martix_type = Show_Incline;
							else
								martix_type = Show_Speed;
	                        nCurrentPage = PAGE_WORK_TM1;       
					}
						break;					
	                case PAGE_WORK_AS1:
					{
	                        if(martix_type == Show_Resistance)
								martix_type = Show_Incline;
							else
								martix_type = Show_Resistance;
						if(  SystemConfig_Get_Char(DRIVE_TYPE_UINT8)== EP_0B_JIS)
							nCurrentPage = PAGE_WORK_AS2_A30;
						else
							nCurrentPage = PAGE_WORK_AS2;
					}
					break;	
                    case PAGE_WORK_AS2_A30:
                    case PAGE_WORK_AS2:
                    { 
						 if(martix_type == Show_Resistance)
							martix_type = Show_Incline;
						else
							martix_type = Show_Resistance;
                        nCurrentPage = PAGE_WORK_AS1;       
                    }
					break;					
                    case PAGE_WORK_BK1:
                    {
                        martix_type = Show_Resistance;
                        if(  SystemConfig_Get_Char(DRIVE_TYPE_UINT8)== DIGITAL_ECB)
                            nCurrentPage = PAGE_WORK_BK2_EUR30;
                        else
                            nCurrentPage = PAGE_WORK_BK2;
                    }
					break;	
				    case PAGE_WORK_BK2_EUR30:					
                    case PAGE_WORK_BK2:
                    {
						martix_type =  Show_Resistance;
                        nCurrentPage = PAGE_WORK_BK1;      
                    }
					break;	
                    case PAGE_WORK_CLIMBMILL1:
                    {
                        if(Program_Get_ProgramID() == PS_LANDMARK)
                        {
                            martix_type = Show_Run_Hight;
                        }
                        else
                        {
                            martix_type =  Show_Resistance;
                        }
                        nCurrentPage = PAGE_WORK_CLIMBMILL2;     
                    }
                    break;
                    case PAGE_WORK_CLIMBMILL2:
                    {
                        if(Program_Get_ProgramID() == PS_LANDMARK)
                        {
                            martix_type = Show_Run_Hight;
                        }
                        else
                        {
                            martix_type =  Show_Resistance;
                        }
                        nCurrentPage = PAGE_WORK_CLIMBMILL1;     
                    }
                    break;
                    default:break;
                }
				Event_Flag.PageChanged = 1;
				Event_Flag.DisChangeForHR = 0 ;
				
				//Page_Reset_All_ModalWnds();
				g_BackPage = nCurrentPage;
                g_martix_modal_type = Show_None;
				g_martix_resume_type = martix_type;
				ShowPage(nCurrentPage,1);
				Page_Reset_All_ModalWnds();
				if(WorkoutFinished==1)
				{
					g_wndMiddleBottomTextBox._isHidden =1 ;
					g_wndLeftBottomTextBox._isHidden =1 ;
					g_wndRightBottomTextBox._isHidden =1 ;	
					Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_WORKOUT_COMPLETE_ID,0,0,0,0,ShowCommonTextBox);
				}
				else
				{
					Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,0,0xAA,0,0,0,ShowMatrixData);
				}
				if(Bottom_Middle_Dis ==1 )
				{
					memcpy(&g_wndMiddleBottomTextBox,&g_BackMiddleWnd,sizeof(Win_Items));
					g_wndMiddleBottomTextBox.pfShow(&g_wndMiddleBottomTextBox);
					memset(&g_BackMiddleWnd,0,sizeof(Win_Items));
				}
            }
			 if(program_evts.WorkOutEvts.EvtBit.Workout_Speed_Evt==1 || program_evts.WorkOutEvts.EvtBit.Workout_QuickKeySpd_Evt==1)
            {
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
				Event_Set_Wnd_Para(&g_wndRightBottomNumerical,0,0,0,0,0,ShowSpeedData);
				Page_Push_OverlappedWindow(&g_wndRightBottomNumerical, &g_wndRightBottomNumerical);
				Event_Set_Wnd_Para(&g_wndRightBottomNumerical,0,8,0,Recover_typeflag,2,ShowSpeedData);
				
				Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,0,0xAA,0,0,0,ShowMatrixData);
            }
            if(program_evts.WorkOutEvts.EvtBit.Workout_Climbmill_ResChange==1)
            {
                if( nCurrentPage == PAGE_WORK_CLIMBMILL1 )
                {
					g_wndLeftBottomNumerical._z = 0;
					Event_Set_Wnd_Para(&g_wndLeftBottomTextBox,STR_LEVEL_ID,0,0,0,0,ShowLevelTitle);					
					Event_Set_Wnd_Para(&g_wndLeftBottomNumerical,0,0,0,0,0,ShowClimbMillLevelData);		
                }
                else
                {
                    g_wndLeftBottomNumerical._z = 0;
					Event_Set_Wnd_Para(&g_wndLeftBottomTextBox,STR_SPM_ID,0,0,0,0,ShowSPMTitle);					
					Event_Set_Wnd_Para(&g_wndLeftBottomNumerical,0,0,0,0,0,ShowSPMData);		
                }
                Page_Push_OverlappedWindow(&g_wndLeftBottomNumerical, &g_wndLeftBottomNumerical);
				Page_Push_OverlappedWindow(&g_wndLeftBottomTextBox, &g_wndLeftBottomTextBox);
				
				Event_Set_Wnd_Para(&g_wndLeftBottomTextBox,STR_LEVEL_ID,8,6,0,2,ShowLevelTitle);					
				Event_Set_Wnd_Para(&g_wndLeftBottomNumerical,0,8,6,0,2,ShowClimbMillLevelData);	
                
				Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,0,0xAA,0,0,0,ShowMatrixData);
			}
             
            if(program_evts.WorkOutEvts.EvtBit.Workout_Incline_Evt==1)
            {
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
            	if(( g_wndLeftBottomTextBox._activetime ==0 ) && ( nCurrentPage == PAGE_WORK_AS2||nCurrentPage == PAGE_WORK_AS2_A30))	
				{
					Event_Set_Wnd_Para(&g_wndLeftBottomTextBox,STR_METS_ID,0,0,0,0,ShowMetsTitle);					
					Event_Set_Wnd_Para(&g_wndLeftBottomNumerical,0,0,0,0,0,ShowMetsData);		
						
					Page_Push_OverlappedWindow(&g_wndLeftBottomNumerical, &g_wndLeftBottomNumerical);
					Page_Push_OverlappedWindow(&g_wndLeftBottomTextBox, &g_wndLeftBottomTextBox);

					Event_Set_Wnd_Para(&g_wndLeftBottomTextBox,STR_INCLINE_ID,8,6,0,2,ShowInclineTitle);					
					Event_Set_Wnd_Para(&g_wndLeftBottomNumerical,0,8,6,0,2,ShowInclineData);	
				}
                else
                {
                		Event_Set_Wnd_Para(&g_wndLeftBottomTextBox,STR_INCLINE_ID,0,0,0,0,ShowInclineTitle);					
					Event_Set_Wnd_Para(&g_wndLeftBottomNumerical,0,0,0,0,0,ShowInclineData);	

					Page_Push_OverlappedWindow(&g_wndLeftBottomNumerical, &g_wndLeftBottomNumerical);
					Page_Push_OverlappedWindow(&g_wndLeftBottomTextBox, &g_wndLeftBottomTextBox);
					
					Event_Set_Wnd_Para(&g_wndLeftBottomTextBox,STR_INCLINE_ID,8,6,Recover_typeflag,2,ShowInclineTitle);					
					Event_Set_Wnd_Para(&g_wndLeftBottomNumerical,0,8,6,Recover_typeflag,2,ShowInclineData);	
                }
				Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,0,0xAA,0,0,0,ShowMatrixData);
			}
            if((program_evts.WorkOutEvts.EvtBit.Workout_Resistance_Evt==1) || (program_evts.WorkOutEvts.EvtBit.Workout_QuickKeyRes_Evt==1))
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
            	if((nCurrentPage == PAGE_WORK_AS2)||(nCurrentPage == PAGE_WORK_BK2)||(nCurrentPage == PAGE_WORK_BK2_EUR30)||(nCurrentPage == PAGE_WORK_AS2_A30))
            	{
	            	if( nCurrentPage == PAGE_WORK_AS2 ||nCurrentPage == PAGE_WORK_AS2_A30)
					{
						g_wndRightBottomNumerical._z = 0;
						Event_Set_Wnd_Para(&g_wndRightBottomTextBox,STR_RPM_ID,0,0,0,0,ShowRpmTitle);					
						Event_Set_Wnd_Para(&g_wndRightBottomNumerical,0,0,0,0,0,ShowRpmData);					
					}
					if(nCurrentPage == PAGE_WORK_BK2||nCurrentPage == PAGE_WORK_BK2_EUR30) 
					{
						g_wndRightBottomNumerical._z = 1;
						Event_Set_Wnd_Para(&g_wndRightBottomTextBox,STR_SPEED_ID,0,0,0,0,ShowSpeedTitle);					
						Event_Set_Wnd_Para(&g_wndRightBottomNumerical,0,0,0,0,0,ShowSpeedData);					
					}
					Page_Push_OverlappedWindow(&g_wndRightBottomNumerical, &g_wndRightBottomNumerical);
					Page_Push_OverlappedWindow(&g_wndRightBottomTextBox, &g_wndRightBottomTextBox);

					g_wndRightBottomNumerical._z = 0;
					Event_Set_Wnd_Para(&g_wndRightBottomTextBox,STR_RESISTANCE_ID,0,6,0,0,ShowResistanceTitle);					
					Event_Set_Wnd_Para(&g_wndRightBottomNumerical,0,8,6,Recover_typeflag,2,ShowResistanceData);					
				}
				else
				{
					g_wndRightBottomNumerical._z = 0;
					Event_Set_Wnd_Para(&g_wndRightBottomTextBox,STR_RESISTANCE_ID,0,0,0,0,ShowResistanceTitle);					
					Event_Set_Wnd_Para(&g_wndRightBottomNumerical,0,0,0,0,0,ShowResistanceData);		
					
					Page_Push_OverlappedWindow(&g_wndRightBottomNumerical, &g_wndRightBottomNumerical);
					Page_Push_OverlappedWindow(&g_wndRightBottomTextBox, &g_wndRightBottomTextBox);
					
					Event_Set_Wnd_Para(&g_wndRightBottomTextBox,STR_RESISTANCE_ID,0,8,6,2,ShowResistanceTitle);					
					Event_Set_Wnd_Para(&g_wndRightBottomNumerical,0,8,6,Recover_typeflag,2,ShowResistanceData);		
				}
				Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,0,0xAA,0,0,0,ShowMatrixData);
            }	
			if(program_evts.WorkOutEvts.EvtBit.Workout_Warmup_Evt==1)
			{
				memset(&g_BackMiddleWnd,0,sizeof(Win_Items));
				//Page_Reset_Modal_Window(&g_wndMiddleBottomTextBox);
				//Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_NULL_ID,0,0,1,0,ShowCommonTextBox);					
				//Page_Push_OverlappedWindow(&g_wndMiddleBottomTextBox, &g_wndMiddleBottomTextBox);
				//Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_WARMUP_ID,0,0xAA,0,0,ShowCommonTextBox);
				Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_WARMUP_ID,0,0,0,0,ShowCommonTextBox);
                Beep_Set_Beeps(1,3,1);
			}
            if(program_evts.WorkOutEvts.EvtBit.Workout_Run_Evt == 1)
            {
				memset(&g_BackMiddleWnd,0,sizeof(Win_Items));
				//Page_Reset_Modal_Window(&g_wndMiddleBottomTextBox);
				//Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_NULL_ID,0,0,0,0,ShowCommonTextBox);
				Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_NULL_ID,0,0,0,0,ShowCommonTextBox);
				Beep_Set_Beeps(1,3,1);
            }
			if(program_evts.WorkOutEvts.EvtBit.Workout_Cooldown_Evt==1)
			{
				memset(&g_BackMiddleWnd,0,sizeof(Win_Items));
				//Page_Reset_Modal_Window(&g_wndMiddleBottomTextBox);
				//Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_NULL_ID,0,0,1,0,ShowCommonTextBox);					
				//Page_Push_OverlappedWindow(&g_wndMiddleBottomTextBox, &g_wndMiddleBottomTextBox);
				//Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_COOLDOWN_ID,0,0xAA,0,0,ShowCommonTextBox);
				Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_COOLDOWN_ID,0,0,0,0,ShowCommonTextBox);
                Beep_Set_Beeps(1,3,1);
			}
            if(program_evts.WorkOutEvts.EvtBit.Workout_Sprint8_RampUp==1)
			{
				memset(&g_BackMiddleWnd,0,sizeof(Win_Items));
				//Page_Reset_Modal_Window(&g_wndMiddleBottomTextBox);
				//Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_NULL_ID,0,0,1,0,ShowCommonTextBox);					
				//Page_Push_OverlappedWindow(&g_wndMiddleBottomTextBox, &g_wndMiddleBottomTextBox);
				//Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_READY_ID,0,0xAA,0,0,ShowCommonTextBox);
				Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_RAMPUP_ID,0,0,0,0,ShowCommonTextBox);
                Beep_Set_Beeps(1,3,1);
			}
			if(program_evts.WorkOutEvts.EvtBit.Workout_Sprint8_Ready==1)
			{
				memset(&g_BackMiddleWnd,0,sizeof(Win_Items));
				//Page_Reset_Modal_Window(&g_wndMiddleBottomTextBox);
				//Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_NULL_ID,0,0,1,0,ShowCommonTextBox);					
				//Page_Push_OverlappedWindow(&g_wndMiddleBottomTextBox, &g_wndMiddleBottomTextBox);
				//Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_READY_ID,0,0xAA,0,0,ShowCommonTextBox);
				Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_READY_ID,0,0,0,0,ShowCommonTextBox);
                Beep_Set_Beeps(1,3,1);
			}
			if(program_evts.WorkOutEvts.EvtBit.Workout_Sprint8_Set==1)
			{
				memset(&g_BackMiddleWnd,0,sizeof(Win_Items));
				//Page_Reset_Modal_Window(&g_wndMiddleBottomTextBox);
				//Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_NULL_ID,0,0,1,0,ShowCommonTextBox);					
				//Page_Push_OverlappedWindow(&g_wndMiddleBottomTextBox, &g_wndMiddleBottomTextBox);
				//Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_SET_ID,0,0xAA,0,0,ShowCommonTextBox);
				Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_SET_ID,0,0,0,0,ShowCommonTextBox);
                Beep_Set_Beeps(1,3,1);
			}
			if(program_evts.WorkOutEvts.EvtBit.Workout_Sprint8_Go==1)
			{
				memset(&g_BackMiddleWnd,0,sizeof(Win_Items));
				//Page_Reset_Modal_Window(&g_wndMiddleBottomTextBox);
				//Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_NULL_ID,0,0,1,0,ShowCommonTextBox);					
				//Page_Push_OverlappedWindow(&g_wndMiddleBottomTextBox, &g_wndMiddleBottomTextBox);
				//Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_GO_ID,0x00,0xAA,0,0,ShowCommonTextBox);
				Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_GO_ID,0x00,0,0,0,ShowCommonTextBox);
                Beep_Set_Beeps(1,3,1);
			}
			if(program_evts.WorkOutEvts.EvtBit.Workout_Sprint8_SprintStage==1)
			{
				memset(&g_BackMiddleWnd,0,sizeof(Win_Items));
				//Page_Reset_Modal_Window(&g_wndMiddleBottomTextBox);
				//Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_NULL_ID,0,0,1,0,ShowCommonTextBox);					
				//Page_Push_OverlappedWindow(&g_wndMiddleBottomTextBox, &g_wndMiddleBottomTextBox);
				//Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_SPRINT1_ID+(Sprint8_Get_SprintStage()-1),0,0xAA,0,0,ShowCommonTextBox);
				Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_SPRINT1_ID+(Sprint8_Get_SprintStage()-1),0,0,0,0,ShowCommonTextBox);
                Beep_Set_Beeps(1,3,1);
			}
			if(program_evts.WorkOutEvts.EvtBit.Workout_Sprint8_ActiveRecoveryStage==1)
			{
				memset(&g_BackMiddleWnd,0,sizeof(Win_Items));
				//Page_Reset_Modal_Window(&g_wndMiddleBottomTextBox);
				//Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_NULL_ID,0,0,1,0,ShowCommonTextBox);					
				//Page_Push_OverlappedWindow(&g_wndMiddleBottomTextBox, &g_wndMiddleBottomTextBox);
				//Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_RECOVERY1_ID+(Sprint8_Get_SprintStage()-1),0,0xAA,0,0,ShowCommonTextBox);
				Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_RECOVERY1_ID+(Sprint8_Get_SprintStage()-1),0,0,0,0,ShowCommonTextBox);
                Beep_Set_Beeps(1,3,1);
			}
			
			if(program_evts.WorkOutEvts.EvtBit.Workout_HR_ChangeToZero==1)
			{
                if(Event_Flag.DisChangeForHR==1 && (nCurrentPage == PAGE_WORK_AS1)||(nCurrentPage == PAGE_WORK_BK1)||(nCurrentPage == PAGE_WORK_TM1))
                {
                	Event_Flag.DisChangeForHR=0 ;
                    Page_Reset_Modal_Window(&g_wndLeftTopTextBox);
                    Page_Reset_Modal_Window(&g_wndLeftTopNumerical);
                    Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_CALORIES_ID,0,0,0,0,ShowCaloriesTitle);					
					Event_Set_Wnd_Para(&g_wndLeftTopNumerical,0,0,0,0,0,ShowCaloriesData);	
                }	
			}
			if(program_evts.WorkOutEvts.EvtBit.Workout_HR_IsNotZero==1)
			{
				if( (nCurrentPage == PAGE_WORK_AS1)||(nCurrentPage == PAGE_WORK_BK1)||(nCurrentPage == PAGE_WORK_TM1))	
				{
					Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_CALORIES_ID,0,0,0,0,ShowCaloriesTitle);					
					Event_Set_Wnd_Para(&g_wndLeftTopNumerical,0,0,0,0,0,ShowCaloriesData);		

					Page_Push_OverlappedWindow(&g_wndLeftTopNumerical, &g_wndLeftTopNumerical);
					Page_Push_OverlappedWindow(&g_wndLeftTopTextBox, &g_wndLeftTopTextBox);

					Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_HR_ID,0,0xAA,0,0,ShowHeartTitle);					
					Event_Set_Wnd_Para(&g_wndLeftTopNumerical,0,0,0xAA,0,0,ShowHeartData);	

					Event_Flag.DisChangeForHR = 1 ;
				}
			}
        }  
		break;
        case PROGRAM_RESET_EVENT:
        {
            Beep_Set_Beeps(4,6,6);
			extern void Main_Initial_Data(INIT_TYPE type);
			Main_Initial_Data(TYPE_RST_INIT );				
#if 0						
			Event_Flag.PageChanged = 0;	
			Event_Flag.DisChangeForHR = 0 ;
            g_martix_resume_type = Show_None;
			g_martix_modal_type = Show_None;
			Page_Reset_All_ModalWnds();
			memset(&g_BackMiddleWnd,0,sizeof(Win_Items));
            Frame_Stage = FRAME_STAGE_USER;
            User_Set_VirtualKey(VK_RESET|Frame_Stage);
#endif				
        } 
        break;
        case PROGRAM_RUN_EVENT:
        {
			if(Event_Flag.PageChanged==1)
			{
				nCurrentPage = g_BackPage;
				martix_type = g_martix_resume_type;
			}
			else
			{
				Event_Get_Page_matrix();
			}
			ShowPage(nCurrentPage,1);
			Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,0,0xAA,0,0,0,ShowMatrixData);
			if(g_BackMiddleWnd._string_id !=STR_NULL_ID && g_BackMiddleWnd._string_id !=0 )
			{
				memcpy(&g_wndMiddleBottomTextBox,&g_BackMiddleWnd,sizeof(Win_Items));
				g_wndMiddleBottomTextBox.pfShow(&g_wndMiddleBottomTextBox);
				memset(&g_BackMiddleWnd,0,sizeof(Win_Items));
			}
            Frame_Stage = FRAME_STAGE_PRGRUN;
			Main_Stage = STAGE_PRG_RUN;
        }
		break;		
        case PROGRAM_PAUSE_EVENT:
        {	
			memcpy(&g_BackMiddleWnd,&g_wndMiddleBottomTextBox,sizeof(Win_Items));
			Page_Reset_All_ModalWnds();
			Event_DisableFlashingAllWnds();
			g_wndMiddleBottomTextBox._isHidden =1 ;
			g_wndLeftBottomTextBox._isHidden =1 ;
			g_wndRightBottomTextBox._isHidden =1 ;
			Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_NULL_ID,0,0,1,0,ShowCommonTextBox);
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_WORKOUT_PAUSED_ID,0,0,0,0,ShowCommonTextBox);
			Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,0,0,0,0,0,ShowMatrixData);
        }
		 break;
        case SETTING_COMPLETE_EVENT:
        {
			memset(&g_BackMiddleWnd,0,sizeof(Win_Items));
			Event_Set_Wnd_Para(&g_wndMiddleBottomTextBox,STR_NULL_ID,0,0,1,0,ShowCommonTextBox);
			Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,0,0,0,0,0,ShowMatrixData);
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_PRESS_START_ID,0,0,0,0,ShowCommonTextBox);
        }
		break;
        case PROGRAM_ENGINEER_EVENT:
        {
            
        }
		break;		
        
        case THREE_EVENT:
        {
			Event_Flag.DisChangeForHR = 0;		
            Beep_Set_Beeps(6,10,10);
        }
        case ONE_EVENT:
        case TWO_EVENT:
        {
		UINT8 Temp_id =  STR_3_TITLE_ID-(program_evts.evt-THREE_EVENT);
		Frame_Stage = FRAME_STAGE_PRGRUN;
		Event_DisableFlashingAllWnds();

		Event_Set_Wnd_Para(&g_wndMiddleMatrixBox,0,0,0,0,0,ShowMatrixData);
		Event_Set_Wnd_Para(&g_wndBottomTextBox,Temp_id,0,0,0,0,ShowCommonTextBox);			
        }
	break;
	case MOTOR_SPEED_OVER_EVENT:
	{
		Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SPEED_RANGE_ERROR_ID,0,0,0,0,ShowCommonTextBox);
	}
	break;
//    case MOTOR_WORKOUTINIT_LCB_EVENT:
//    {
//        extern void Main_Initial_Data(INIT_TYPE type);        
//        Main_Initial_Data(TYPE_RST_INIT);
//        Beep_Set_Beeps(6,10,10);
//        LCB_Send_Cmd(DS_INC_LOCATION,Data_Get_Incline(MIN_INCLINEADC));
//        ret = RET_UNPROCESS_UNDISPLAY;
//    }
//    break;
	case PROGRAM_EVENT_PROCESSED:
       default:   break;
    }
    g_wndWiFiIcon.pfShow= ShowWifiIcon;
    g_wndWiFiIcon.pfShow(&g_wndWiFiIcon);
    
    g_wndRFIcon.pfShow= ShowRFICON;
    g_wndRFIcon.pfShow(&g_wndRFIcon);

	return ret;	
}

/***************************************************************************//**
* @brief
*   Events_ENG_Handler 
*
* @note
*   
* @param[in]:ENG_EVENT eng_evts
* @param[out]:none
******************************************************************************/
ReturnEventHandler Events_ENG_Handler(ENG_EVENT eng_evts,ReturnEventHandler flag)
{
	extern     UINT8 BLINK_DISPLAY_CHANNEL ;
	extern     UINT8 STATIC_DISPLAY_CHANNEL;
	extern     UINT8 MODAL_WINDOW_CHANNEL;
	extern     UINT8 REFRESH_WINDOW_CHANNEL;

	ReturnEventHandler ret = flag;
	if(eng_evts == ENG_EVENT_NONE ) return ret;
	if(eng_evts == ENG_EVENT_PROCESSED )
    {
        ret = RET_UNPROCESS_UNDISPLAY ;
        return ret;
    }
    EFM_ASSERT(eng_evts < ENG_EVENT_TOTAL);
	Display_Set_All(0);
	ret = RET_UNPROCESS_UNDISPLAY;	 	
    switch(eng_evts)
    {
        case DISPLAY_ENG_EVENT:
        {
			Timer_Set_Timer_Paused(BLINK_DISPLAY_CHANNEL);
			Timer_Set_Timer_Paused(STATIC_DISPLAY_CHANNEL);
			Timer_Set_Timer_Paused(MODAL_WINDOW_CHANNEL);		
			Timer_Set_Timer_Paused(REFRESH_WINDOW_CHANNEL);			
			//Event_Flag.DisAllOn =0;
            //Event_Flag.DisAllOff =0;
            HAL_BOR_Enable_ExtPower(Power_Channel_ERP,0);
            //Frame_Stage=FRAME_STAGE_ENG;
            //nCurrentPage = PAGE_ENG0_0_DisplayTest;
            
			Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_ENG0_ID,0,0,0,0,ShowCommonTextBox);
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_DISPLAY_TEST_ID,0,0,0,0,ShowCommonTextBox);				
        }
		break;
        case ALL_DOTSON_ENG_EVENT:
        {
			//Timer_Set_Timer_Paused(BLINK_DISPLAY_CHANNEL);
			//Timer_Set_Timer_Paused(STATIC_DISPLAY_CHANNEL);
			//Timer_Set_Timer_Paused(MODAL_WINDOW_CHANNEL);		
			//Timer_Set_Timer_Paused(REFRESH_WINDOW_CHANNEL);		
			Display_Set_All(1);
			//Event_Flag.DisAllOn =1;
           // Event_Flag.DisAllOff =0;
            HAL_BOR_Enable_ExtPower(Power_Channel_ERP,1);
        }
		break;
        case ALL_DOTSOFF_ENG_EVENT:
        {
			//Timer_Set_Timer_Start(BLINK_DISPLAY_CHANNEL);
			//Timer_Set_Timer_Start(STATIC_DISPLAY_CHANNEL);
			//Timer_Set_Timer_Start(MODAL_WINDOW_CHANNEL);		
			//Timer_Set_Timer_Start(REFRESH_WINDOW_CHANNEL);			
			//Event_Flag.DisAllOn =0;
            //Event_Flag.DisAllOff =1;
						
            Display_Set_All(0);
	        HAL_BOR_Enable_ExtPower(Power_Channel_ERP,0);            
        }
        break;
		case ENG0_DIS_NUMBER:
		{
			//Event_Flag.DisAllOn =0;
            //Event_Flag.DisAllOff =1;
			//Timer_Set_Timer_Paused(BLINK_DISPLAY_CHANNEL);
			//Timer_Set_Timer_Paused(STATIC_DISPLAY_CHANNEL);
			//Timer_Set_Timer_Paused(MODAL_WINDOW_CHANNEL);		
			//Timer_Set_Timer_Paused(REFRESH_WINDOW_CHANNEL);							
            Display_Set_All(0);
			HAL_BOR_Enable_ExtPower(Power_Channel_ERP,0);
			Event_Set_Wnd_Para(&g_wndLeftBottomNumerical,NULL,0,0,0,0,ShowENG0Number);
			Event_Set_Wnd_Para(&g_wndRightBottomNumerical,NULL,0,0,0,0,ShowENG0Number);
			Event_Set_Wnd_Para(&g_wndRightTopNumerical,NULL,0,0,0,0,ShowENG0Number);
			Event_Set_Wnd_Para(&g_wndLeftTopNumerical,NULL,0,0,0,0,ShowENG0Number);
			Event_Set_Wnd_Para(&g_wndMiddleNumerical,NULL,0,0,0,0,ShowENG0Number);
		}
		break;
		case ENG0_DIS_CHAR:
		{
	
		}
		break;
		case ENG0_DIS_ROWSCAN:
		{

		}
		break;
		case ENG0_DIS_KEY_VALUE:
		{
			Event_Flag.DisAllOn =0;
            Event_Flag.DisAllOff =0;
			HAL_BOR_Enable_ExtPower(Power_Channel_ERP,0);
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_KEY_TEST_ID,0,0,0,0,ShowCommonTextBox);
			Event_Set_Wnd_Para(&g_wndRightBottomNumerical,NULL,0,0,0,0,ShowENGKeyValue);
		}
		break;
        //eng1
        case HARDWARE_ENG_EVENT:
        {
			Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_ENG1_ID,0,0,0,0,ShowCommonTextBox);
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_HARDWARE_TEST_ID,0,0,0,0,ShowCommonTextBox);				
        }
        break;
		case BASICFUNCTIONS_ENG_EVENT:
		{
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_NULL_ID,0,0,1,0,ShowCommonTextBox);				
			//MCB_TYPE _type = (MCB_TYPE)LCB_Get_MCBType();
			//g_wndMiddleNumerical-->HR        
	        g_wndMiddleNumerical._z = 0;
	       	Event_Set_Wnd_Para(&g_wndMiddleNumerical,0,0,0,0,0,ShowHeartData);
			if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)		
			//if(_type ==DC_LCB_30_325HP_JIS||_type ==MCB_DELTA || _type ==MCB_TOPTEK|| _type ==MCB_DELTA_COMMOCIAL)
			{
				 //LeftTopTextBox-->speed
		       	Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_SPEED_ID,0,0,0,0,ShowCommonTextBox);
		        //g_wndLeftTopNumerical-->RPM
		        g_wndLeftTopNumerical._z = 0;
		       	Event_Set_Wnd_Para(&g_wndLeftTopNumerical,0,0,0,0,0,ShowENGSetRPMData);

				//RightTopTextBox-->PWM
		       	Event_Set_Wnd_Para(&g_wndRightTopTextBox,STR_RPM_ID,0,0,0,0,ShowCommonTextBox);
		        //g_wndRightTopNumerical-->RPM
		        g_wndRightTopNumerical._z = 0;
				Event_Set_Wnd_Para(&g_wndRightTopNumerical,0,0,0,0,0,ShowENGLCBRPMData);			
			}
	       	else
	       	{
	       		//LeftTopTextBox-->ECB count Setted
		       	Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_ECB_ID,0,0,0,0,ShowCommonTextBox);
		        //g_wndLeftTopNumerical-->RPM
		        g_wndLeftTopNumerical._z = 0;
		       	Event_Set_Wnd_Para(&g_wndLeftTopNumerical,0,0,0,0,0,ShowENGSettedECBCount);

				//g_wndLeftTopNumerical-->ECB count From LCB
		        //g_wndRightTopNumerical._z = 0;
				//Event_Set_Wnd_Para(&g_wndRightTopNumerical,0,0,0,0,0,ShowENGLCBECBCount);	
			}
			if(LCB_Get_MCBType()!=DIGITAL_ECB )	
			{
		        //LeftBottomTextBox-->INCLINE
		       	Event_Set_Wnd_Para(&g_wndLeftBottomTextBox,STR_INCLINE_ID,0,0,0,0,ShowCommonTextBox);
				//g_wndLeftBottomNumerical-->incline ADC
		        g_wndLeftBottomNumerical._z = 0;
		       	Event_Set_Wnd_Para(&g_wndLeftBottomNumerical,0,0,0,0,0,ShowENGInclineADCData);

		        //RightBottomTextBox-->ADC
		       //	Event_Set_Wnd_Para(&g_wndRightBottomTextBox,STR_ADC_ID,0,0,0,0,ShowCommonTextBox);
		        //g_wndLeftBottomNumerical-->incline
		       // g_wndRightBottomNumerical._z = 0;
		       	//Event_Set_Wnd_Para(&g_wndRightBottomNumerical,0,0,0,0,0,ShowENGLCBECBCount);
			}
		}
        break;
        
        case USB_ENG_EVENT:
		{
	       	Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_USB_TEST_ID,0,0,0,0,ShowCommonTextBox);
        }
        break;
        case AUTO_ADC_ENG_EVENT:
        {
            Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_AUTO_ADC_ID,0,0,0,0,ShowCommonTextBox);
        }
        break;
        case AUTO_INCLINE_ENG_EVENT:
        {
            Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_INCLINE_ID,0,0,0,0,ShowCommonTextBox);
            g_wndLeftTopNumerical._z = 0;
		    Event_Set_Wnd_Para(&g_wndLeftTopNumerical,0,0,0,0,0,ShowCurrentInclineADC);
            Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_AUTO_ADC_ID,0,0,0,0,ShowCommonTextBox);
        }
        break;
        case AUTO_INCLINE_PASS:
            Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_AUTO_INCLINE_PASS,0,0,0,0,ShowCommonTextBox);
        break;
        case AUTO_INCLINE_FAIL:
            Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_AUTO_INCLINE_FAIL,0,0,0,0,ShowCommonTextBox);
        break;
        case USB_TESTING_ENG_EVENT:
		{
	       	Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_USB_TESTING_ID,0,0,0,0,ShowCommonTextBox);
        }
        break;			
        case WIFI_ENG_EVENT:
        {
            //Wifi_Get_EnggineTestResult();
	       	Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_WIFI_TEST_ID,0,0,0,0,ShowCommonTextBox);
        } 
		break;
        case WIFI_TESTING_ENG_EVENT:
        {
            //Wifi_Get_EnggineTestResult();
	       	Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_PLEASE_WAIT_ID,0,0,0,0,ShowCommonTextBox);
			ret = RET_PROCESS_DISPLAY ;			
        } 
		break;
        //eng2
        case NOTAVAILABLE_ENG_EVENT:
        {
	       	Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_ENG2_ID,0,0,0,0,ShowCommonTextBox);
	       	Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_NOT_AVAILABLE_ID,0,0,0,0,ShowCommonTextBox);
        }
		break;
        //eng3
        case SWITCH_ENG_EVENT:
        {
			STRINGID Tempid ;		
            if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
                Tempid = STR_ENG3_ID;
            else
                Tempid = STR_ENG2_ID;

			Event_Set_Wnd_Para(&g_wndLeftTopTextBox,Tempid,0,0,0,0,ShowCommonTextBox);
	       	Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SWITCH_FUNCATION_ID,0,0,0,0,ShowCommonTextBox);
        }
		break;		
        //eng4
        case INFORMATION_ENG_EVENT:
        {
			STRINGID Tempid ;			
            if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
                Tempid = STR_ENG4_ID;
            else
                Tempid = STR_ENG3_ID;
						
			Event_Set_Wnd_Para(&g_wndLeftTopTextBox,Tempid,0,0,0,0,ShowCommonTextBox);
	       	Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_INFORMATION_ID,0,0,0,0,ShowCommonTextBox);
        }
		break;		
        case ACCINF_ENG_EVENT:
        {
            //show acc
           	Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_ACCUMULATED_INFO_ID,0,0,0,0,ShowCommonTextBox);
            //show time
	       	Event_Set_Wnd_Para(&g_wndRightTopTextBox,STR_TIME_ID,0,0,0,0,ShowCommonTextBox);
            if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==CLIMB_MILL)
            {
                //show stairs
	       	    Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_STAIRS_ID,0,0,0,0,ShowCommonTextBox);
                //show stairs data
	       	    Event_Set_Wnd_Para(&g_wndLeftTopNumerical,0,0,0,0,0,ShowACCStairs);
            }
            else
            {
                //show distance
	       	    Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_DISTANCE_ID,0,0,0,0,ShowCommonTextBox);
                //show distance data
	       	    Event_Set_Wnd_Para(&g_wndLeftTopNumerical,0,0,0,0,0,ShowACCDis);
            }
            
            //show distance data
	       	Event_Set_Wnd_Para(&g_wndRightTopNumerical,0,0,0,0,0,ShowACCTime);
        }
		break;		
        case RSCU_ENG_EVENT:
        { 
            STRINGID Tempid;	
            if( SystemConfig_Get_Char(RSCU_FLAG_UCHAR) == 0xA5 )
            {
               Tempid = STR_RSCU_ON_ID;
            }
            else
            {
                Tempid = STR_RSCU_OFF_ID;
            }
           	Event_Set_Wnd_Para(&g_wndBottomTextBox,Tempid,0,0,0,0,ShowPXTextBox);
        }
        break;
        case UCBVERSION_ENG_EVENT:
        {
            //dis UCB
	       	Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_UCB_VERSION_ID,0,0,0,0,ShowCommonTextBox);
	       	Event_Set_Wnd_Para(&g_wndLeftTopTextBox,0,0,0,0,0,ShowUCBVision);
        }
		break;
        case MCBVERSION_ENG_EVENT:
        {
            //dis MCB
           	Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_MCB_VERSION_ID,0,0,0,0,ShowCommonTextBox);
	       	Event_Set_Wnd_Para(&g_wndRightTopTextBox,0,0,0,0,0,ShowMCBVision);
	       	Event_Set_Wnd_Para(&g_wndLeftTopTextBox,0,0,0,0,0,ShowMCBType);
        }
		break;		
        //eng8
        case TEST_ENG_EVETN:
        {
			Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_ENG8_ID,0,0,0,0,ShowCommonTextBox);
	       	Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_TEST_ID,0,0,0,0,ShowCommonTextBox);
        }
		break;		
        case BURN_ENG_EVENT:
		{
        	//P3
			Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_P3_ID,0,0,0,0,ShowPXTextBox);
	       	Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_ACCUMULATE_AGING_HOURS_ID,0,0,0,0,ShowCommonTextBox);
	       	Event_Set_Wnd_Para(&g_wndMiddleNumerical,0,0,0,0,0,ShowACCAgingHours);
       }
        break;
        case UNIT_ENG_EVENT:
        {  
			STRINGID Tempid2,Tempid3 ;				
			//P1
			Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_P1_ID,0,0,0,0,ShowPXTextBox);
            
            //standard or metric
            if(SystemConfig_Get_Char(UNIT_FLAG_UCHAR)==STANDARD)
            {
                Tempid2 = STR_STANDARD_ID;
                Tempid3 = 0;
            }
            else
            {
                Tempid2 = STR_METRIC_ID;
              	Tempid3 = 1;
            }
		g_wndRightTopNumerical._z = 0 ;
		Event_Set_Wnd_Para(&g_wndBottomTextBox, Tempid2,0,0,0,0,ShowCommonTextBox);
		Event_Set_Wnd_Para(&g_wndRightTopNumerical, Tempid3,0,0,0,0,ShowCommonData);
		g_wndRightTopNumerical._string_id = NULL;
        }
		break;		
        case LANGUAGE_ENG_EVENT:
        {         
		//P2
		STRINGID Tempid = STR_ENGLISH_ID+SystemConfig_Get_Char(LANGUAGE_ID_UINT8);
		Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_P2_ID,0,0,0,0,ShowPXTextBox);
		Event_Set_Wnd_Para(&g_wndBottomTextBox,Tempid,0,0,0,0,ShowCommonTextBox);
        }
		break;		
        case MACHINE_ENG_EVENT:
        {
			STRINGID Tempid = STR_TREADMILL_ID+SystemConfig_Get_Char(MACHINE_TYPE_UINT8);		
           	Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_P3_ID,0,0,0,0,ShowPXTextBox);
			Event_Set_Wnd_Para(&g_wndBottomTextBox,Tempid,0,0,0,0,ShowCommonTextBox);
        }
		break;		
        case MODEL_ENG_EVENT:
        {
            //P4
           	Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_P4_ID,0,0,0,0,ShowPXTextBox);
            
            UINT8 temptypeID = SystemConfig_Get_Char(MACHINE_MODLE_UINT8);
            //UINT8 tempMachineID = SystemConfig_Get_Char(MACHINE_TYPE_UINT8);
			switch(SystemConfig_Get_Char(MACHINE_TYPE_UINT8))	
			{
                case CLIMB_MILL:
				{
					if(temptypeID==1)
	                    temptypeID =STR_C50_ID;  
				}
				break;
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
	                    temptypeID =STR_E30_ID;  
	                else if(temptypeID==2)
	                    temptypeID =STR_E50_ID; 
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
                    else if(temptypeID==5) 
                        temptypeID = STR_T75_ID;
                    else if(temptypeID==6) 
                        temptypeID = STR_T30_ID;
				}
				break;
				default:break;
			}
           	Event_Set_Wnd_Para(&g_wndBottomTextBox,temptypeID,0,0,0,0,ShowCommonTextBox);
        }
		break;		
        case ERP_ENG_EVENT:
        {
			STRINGID Tempid1;	
            //P5
           	Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_P5_ID,0,0,0,0,ShowPXTextBox);
            
            if( SystemConfig_Get_Char(ENERGY_SAVER_FLAG_UCHAR) == 1 )
            {
               	Tempid1 = STR_ENERGY_SAVE_ON_ID;
            }
            else
            {
                Tempid1 = STR_ENERGY_SAVE_OFF_ID; 
            }
           	Event_Set_Wnd_Para(&g_wndBottomTextBox,Tempid1,0,0,0,0,ShowCommonTextBox);
        }
		break;		
        case BOOT_ENG_EVENT:
        {
            //P6
           	Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_P6_ID,0,0,0,0,ShowPXTextBox);
            
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
//		case OVERSPEEDFLAG_ENG_EVENT:
//		{
//            //P7
//           	Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_P7_ID,0,0,0,0,ShowPXTextBox);
//            
//            STRINGID Tempid;	
//            if( SystemConfig_Get_Char(OVERSPEED_SWITCH_UINT8) == 1 )
//            {
//               Tempid = STR_OVERSPEED_ON_ID;
//            }
//            else
//            {
//                Tempid = STR_OVERSPEED_OFF_ID;
//            }
//           	Event_Set_Wnd_Para(&g_wndBottomTextBox,Tempid,0,0,0,0,ShowCommonTextBox);
//            
//        }
//		break;
		case ERR_ENG_EVENT:
        {
            //P7
            STRINGID Tempid1;
            Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_P7_ID,0,0,0,0,ShowPXTextBox);
            if( SystemConfig_Get_Char(SHOW_ERROR_FLAG_UCHAR) == 1 )
            {
               	Tempid1 = STR_ERRORCODE_ON_ID;
            }
            else
            {
                Tempid1 = STR_ERRORCODE_OFF_ID; 
            }
            Event_Set_Wnd_Para(&g_wndBottomTextBox,Tempid1,0,0,0,0,ShowCommonTextBox);
        }
        break;
        case CONTROL_ENG_EVENT:
        {
            //P8
            STRINGID Tempid1;
            Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_P8_ID,0,0,0,0,ShowPXTextBox);
            if( SystemConfig_Get_Char(CONTROL_ZONE_FLAG_UCHAR) == 1 )
            {
               	Tempid1 = STR_CONTROL_ZONE_ON_ID;
            }
            else
            {
                Tempid1 = STR_CONTROL_ZONE_OFF_ID; 
            }
            Event_Set_Wnd_Para(&g_wndBottomTextBox,Tempid1,0,0,0,0,ShowCommonTextBox);
        }
        break;
        case SECOND_CONTROL_ENG_EVENT:
        {
            //P8
            STRINGID Tempid1;
            if( SystemConfig_Get_Char(SECOND_CONTROL_ZONE_FLAG_UCHAR) == 1 )
            {
               	Tempid1 = STR_SECOND_CONTROL_ZONE_ON_ID;
            }
            else
            {
                Tempid1 = STR_SECOND_CONTROL_ZONE_OFF_ID; 
            }
            Event_Set_Wnd_Para(&g_wndBottomTextBox,Tempid1,0,0,0,0,ShowCommonTextBox);
        }
        break;
        case SHOW_CONTROL_ENG_EVENT:
        {
            Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_1ST_CONTROL_ZONE_ID,0,0,0,0,ShowCommonTextBox);//左上角点阵显示示1ST CONTROL ZONE
            Event_Set_Wnd_Para(&g_wndLeftTopNumerical,NULL,0,0,0,0,ShowEng1StControlZoneData);      //左上角LED管显示1ST CONTROL ZONE DATA
            Event_Set_Wnd_Para(&g_wndRightTopTextBox,STR_2ND_CONTROL_ZONE_ID,0,0,0,0,ShowCommonTextBox);//右上角点阵显示示2ND CONTROL ZONE
            Event_Set_Wnd_Para(&g_wndRightTopNumerical,NULL,0,0,0,0,ShowEng2NdControlZoneData);      //右上角LED管显示2ND CONTROL ZONE DATA
//            Event_Set_Wnd_Para(&g_wndLeftBottomTextBox,STR_REAL_CONTROL_ZONE_ID,0,0,0,0,ShowCommonTextBox);//左下角显示点阵REAL DATA
//            Event_Set_Wnd_Para(&g_wndLeftBottomNumerical,NULL,0,0,0,0,ShowEngRealControlZoneData);//左下角LED管显示REAL CONTROL ZONE DATA
//            Event_Set_Wnd_Para(&g_wndRightBottomTextBox,STR_TRAGET_CONTROL_ZONE_ID,0,0,0,0,ShowCommonTextBox);//右下角LED管显示SET TARGET CONTROL ZONE DATA
//            Event_Set_Wnd_Para(&g_wndRightBottomNumerical,NULL,0,0,0,0,ShowEngSetControlZoneData);//右下角LED管显示SET TARGET CONTROL ZONE DATA
            Event_Set_Wnd_Para(&g_wndLeftBottomTextBox,STR_TRAGET_CONTROL_ZONE_ID,0,0,0,0,ShowCommonTextBox);//右下角LED管显示SET TARGET CONTROL ZONE DATA
            Event_Set_Wnd_Para(&g_wndLeftBottomNumerical,NULL,0,0,0,0,ShowEngSetControlZoneData);//右下角LED管显示SET TARGET CONTROL ZONE DATA
        }
        break;
        case IR_SENSOR_ENG_EVENT:
        {
            //P9
            STRINGID Tempid1;
            Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_P9_ID,0,0,0,0,ShowPXTextBox);
            if( SystemConfig_Get_Char(IR_SENSOR_FLAG_UCHAR) == 1 )
            {
               	Tempid1 = STR_IR_SENSOR_ON_ID;
            }
            else
            {
                Tempid1 = STR_IR_SENSOR_OFF_ID; 
            }
            Event_Set_Wnd_Para(&g_wndBottomTextBox,Tempid1,0,0,0,0,ShowCommonTextBox);
            Event_Set_Wnd_Para(&g_wndRightBottomNumerical,0,0XAA,0,0,2,ShowEngIrSensorData);
        }
        break;
        case MAX_LEVEL_ENG_EVENT:
        {
            //P10
            Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_P10_ID,0,0,0,0,ShowPXTextBox);
            Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_MAX_LEVEL_ID,0,0,0,0,ShowCommonTextBox);
            Event_Set_Wnd_Para(&g_wndRightBottomNumerical,0,0XAA,0,0,2,ShowEngMaxLevelData);
        }
        break;
        case QUICK_PRO_ENG_EVENT:
        {
		//P1
		Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_P1_ID,0,0,0,0,ShowPXTextBox);
		STRINGID Tempid;	
		if(Engmode_Get_PROG_Mult_Times()==1)
		{
			Tempid = STR_NORMAL_ID;
		}
		else
		{
			Tempid = STR_QUICK_ID;
		}
		g_wndRightTopNumerical._z= 0 ;
		Event_Set_Wnd_Para(&g_wndBottomTextBox,Tempid,0,0,0,0,ShowCommonTextBox);
		Event_Set_Wnd_Para(&g_wndRightTopNumerical,0,0,0,0,0,ShowQuickMul);
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
           	Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SET_DAPI_ID,0,0,0,0,ShowCommonTextBox);
        }
		break;		
        case SETDAPI_ENG_EVENT:
        {
            //set dapi 
            STRINGID Tempid1,Tempid2;		
           	Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_SET_DAPI_ID,0,0,0,0,ShowCommonTextBox);
            
            MACHINE_INFO machineinfo = SystemConfig_Get_MachineInfo();
            if(strncmp(machineinfo.marioBox, JOHNSON_WIFI_SERVERIP_PRODUCTION, 256) == 0 )
            {
               	Tempid1 = STR_PRODUCTION_ID;
               	Tempid2 = 0;
            } 
            else if(strncmp(machineinfo.marioBox, JOHNSON_WIFI_SERVERIP_QA, 256) == 0 )
            {
                Tempid1 = STR_QA_ID;
                Tempid2 = 1;
            } 
            else if(strncmp(machineinfo.marioBox, JOHNSON_WIFI_SERVERIP_STAGING, 256) == 0 )
            {
            	Tempid1 = STR_STAGING_ID;
                Tempid2 = 2;
                //g_wndBottomTextBox._string_id = STR_STAGING_ID;
                //g_wndLeftTopNumerical._string_id = 2;
            }
			else
			{
				Tempid1 = STR_DEV_ID;
                Tempid2 = 3;
                //g_wndBottomTextBox._string_id = STR_DEV_ID;
                //g_wndLeftTopNumerical._string_id = 2;
			}
           	Event_Set_Wnd_Para(&g_wndBottomTextBox,Tempid1,0,0,0,0,ShowCommonTextBox);
			g_wndMiddleNumerical._z = 0 ;
           	Event_Set_Wnd_Para(&g_wndMiddleNumerical,Tempid2,0,0,0,0,ShowCommonData);
        }    
        break;
        case CLEAR_ENG_EVENT:
        {
            //P2
            Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_P2_ID,0,0,0,0,ShowPXTextBox);
           	Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_NO_CLEAR_ID,0,0,0,0,ShowCommonTextBox);
        }
		break;		
        case CLEARED_ENG_EVENT:
        {
            //P2
            Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_P2_ID,0,0,0,0,ShowPXTextBox);
           	Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_CLEAR_ID,0,0,0,0,ShowCommonTextBox);
			RF_Set_VirtualKey(VK_INITIAL);
		}
		break;		
        case QUICK_ERP_ENG_EVENT:
        {
            //P4
            Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_P4_ID,0,0,0,0,ShowPXTextBox);
           	Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_PRESS_START_ENTER_ERP_ID,0,0,0,0,ShowCommonTextBox);
        }
		break;		
        case ERP_WAIT_TIME:
        {
			Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_P5_ID,0,0,0,0,ShowPXTextBox);
           	Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SET_ERP_TIME_ID,0,0,0,0,ShowCommonTextBox);		
           	Event_Set_Wnd_Para(&g_wndRightTopNumerical,0,0,0,0,0,ShowERPWaitTime);		
        }
		break;		
        case QUIT_ENG_EVENT:
        {
			extern void Main_Initial_Data(INIT_TYPE type);
            Main_Initial_Data(TYPE_RST_INIT );
            if(!DigitalGetRFstatus())Main_Initial_Data(TYPE_ALL_INIT );//PG2 DISCONNECT
			Timer_Set_Timer_Start(BLINK_DISPLAY_CHANNEL);
			Timer_Set_Timer_Start(STATIC_DISPLAY_CHANNEL);
			Timer_Set_Timer_Start(MODAL_WINDOW_CHANNEL);		
			Timer_Set_Timer_Start(REFRESH_WINDOW_CHANNEL);	
			Beep_Set_Beeps(4,6,6);
#if 0					
            Main_Init_Model_Data();
            //Event_Handler_Init_Data();
            Frame_Stage = FRAME_STAGE_USER;
            nCurrentPage = PAGE_SELECTUSER;
            Event_Set_Virtual_key(VK_INITIAL|Frame_Stage);
#endif						
        }
        break;
		case SET_RPM:
		{
			Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_P7_ID,0,0,0,0,ShowPXTextBox);
           	Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SET_RPM_ID,0,0,0,0,ShowCommonTextBox);		
           	Event_Set_Wnd_Para(&g_wndRightTopNumerical,0,0,0,0,0,ShowRpmData);		

		}
		break;
		case OVERSPEEDVALUE_EVENT:
        {
			Event_Set_Wnd_Para(&g_wndRightTopTextBox,STR_MILE_ID,0,0,0,0,ShowCommonTextBox);
			Event_Set_Wnd_Para(&g_wndLeftTopTextBox,STR_P6_ID,0,0,0,0,ShowPXTextBox);
           	Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SET_OVER_SPEED_ID,0,0,0,0,ShowCommonTextBox);		
           	Event_Set_Wnd_Para(&g_wndRightTopNumerical,0,0,0,0,0,ShowOverSpeedValue);		
        }
		break;	
        default: break;
    }
	return ret ;
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
ReturnEventHandler Events_USB_Handler(USB_EVENTS evt,ReturnEventHandler flag)
{ 
	ReturnEventHandler ret = flag;
    if(evt == EVT_USB_NONE) return flag;
    EFM_ASSERT(evt < EVT_USB_TOTAL);
    
    ret = RET_UNPROCESS_UNDISPLAY;
    
    switch(evt)
    {
        case EVT_FILE_DETECTED:
        {
            Display_Set_All(0);
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SOFTWARE_UPDATE_PRESS_ENTER_ID,0,0,0,0,ShowCommonTextBox);
        }
        break;
        case EVT_FILE_LANGUAGE_UPDATE:
        {
            Display_Set_All(0);
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_LANGUAGE_UPDATE_PRESS_ENTER_ID,0,0,0,0,ShowCommonTextBox);
        }
        break;
        case EVT_FILE_LANGUAGE_UPDATEING:
        {
            Display_Set_All(0);
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_LANGUAGE_UPDATEING_ID,0,0,0,0,ShowCommonTextBox);
        }
        break;
        case EVT_FILE_UPDATE_CANCELED:
        case EVT_USB_REMOVED:
        {
            ret = RET_REFRESH_DISPLAY;
        }
        break;
        case EVT_FILE_UPDATE_CONFIRMED:
        {
            Display_Set_All(0);
            
            BOR_Set_Passcode(0);
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_REBOOTING_ID,0,0,0,0,ShowCommonTextBox);		
            BOR_Set_Passcode(0);
            Beep_Set_Beeps(6,10,10);
            while(Beep_Get_OK()==false)
            {
#ifdef WDT_ENABLE
                HAL_Clear_WatchDog();
#endif                 
                Display_Updata_Data(true);
            }
            HAL_Reset_System(); 
        }
        break;
        default: break;
    }
	return ret;
}


/***************************************************************************//**
* @brief
*   void Events_RF_Handler(RF_EVENT rf_evts) 
*
* @note
*  RF_EVENT???ù￡o
*   
* @param[in]:RF_EVENT rf_evts
* @param[out]:none
******************************************************************************/
#ifdef RF_MODULE_EXIST_   
ReturnEventHandler Events_RF_Handler(RF_EVENT evt,ReturnEventHandler flag)
{
	ReturnEventHandler ret = flag;
	
    if(evt == RF_EVENT_NONE) return flag;
    
    ret = RET_UNPROCESS_UNDISPLAY;
    
    switch(evt)
    {
        case RF_ONLINE:
        {
            Event_Flag.Syncing = false;
			Event_Set_Wnd_Para(&g_wndRFIcon,0,0,0,0,0,ShowRFICON);
			ret = flag;
            break;
        }
        case RF_OFFLINE:
        {
            Event_Flag.Syncing = false;
			Event_Set_Wnd_Para(&g_wndRFIcon,0,0,0,0,0,ShowRFICON);			
			ret = flag;
            break;
        }
        case RF_SYNC_ING:
        {
            BackString_For_Syncing = g_wndBottomTextBox._string_id;
            BackShowFunc = g_wndBottomTextBox.pfShow;
			Event_Set_Wnd_Para(&g_wndRFIcon,0,0xAA,0,0,0,ShowRFICON);	
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_RF_SYNC_ID,0,0,0,0,ShowCommonTextBox);

            Event_Flag.Syncing = true;
        }
        break;
        case RF_SYNC_FINISH:
		{
			Event_Flag.Syncing = false;
			if(BackShowFunc == ShowUsrName)
            {
				Event_Set_Wnd_Para(&g_wndBottomTextBox,0,0,0,0,0,ShowUsrName);
            }
			else
			{
				if(g_wndBottomTextBox._string_id!=STR_RF_SYNC_ID)
	            {
	                BackString_For_Syncing = g_wndBottomTextBox._string_id;
	            }				
				Event_Set_Wnd_Para(&g_wndBottomTextBox,BackString_For_Syncing,0,0,0,0,ShowCommonTextBox);
			}
			Page_Push_OverlappedWindow(&g_wndBottomTextBox, &g_wndBottomTextBox);
			
			Event_Set_Wnd_Para(&g_wndRFIcon,0,0,0,0,0,ShowRFICON);	
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_PASSPORT_READY_ID,0,3,0,0,ShowCommonTextBox);
            
            Beep_Set_Beeps(2,10,10);
		}
		break;
        case RF_SYNC_TIMEOUT://3
       	{
			Event_Flag.Syncing = false;

			if(BackShowFunc == ShowUsrName)
            {
				Event_Set_Wnd_Para(&g_wndBottomTextBox,0,0,0,0,0,ShowUsrName);
            }
			else
			{
				if(g_wndBottomTextBox._string_id!=STR_RF_SYNC_ID)
	            {
	                BackString_For_Syncing = g_wndBottomTextBox._string_id;
	            }				
				Event_Set_Wnd_Para(&g_wndBottomTextBox,BackString_For_Syncing,0,0,0,0,ShowCommonTextBox);
			}
            Page_Push_OverlappedWindow(&g_wndBottomTextBox, &g_wndBottomTextBox);

			Event_Set_Wnd_Para(&g_wndRFIcon,0,0,0,0,0,ShowRFICON);
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_TIME_OUT_ID,0,3,0,0,ShowCommonTextBox);
            
            Beep_Set_Beeps(2,10,10);
		}
		break;
        case RF_EVENT_NONE:
        default: break;
    }
	return ret;
}
#endif
#ifdef  __WIFI_FUNC__
ReturnEventHandler WIFI_Events_Handler(WIFI_EVENT_HANDLE wifi_evts,ReturnEventHandler flag)
{
	ReturnEventHandler ret = flag;
	
    if(wifi_evts == WIFI_EVENT_IDLE) return flag;

	ret = RET_PROCESS_DISPLAY;
			
    switch(wifi_evts)
    {
    	case WIFI_EVENT_SUCCESS:
		{

		}
		break;
		case WIFI_EVENT_TIMEOUT:
		{
			Display_Set_All(0);
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_TIME_OUT_ID,0,3,0,0,ShowCommonTextBox);
            Beep_Set_Beeps(2,10,10);
		}
		break;
		case WIFI_EVENT_UPDATE_FAIL:
        {
			nCurrentPage = PAGE_SELECTUSER;
            Frame_Stage = FRAME_STAGE_USER;
            Event_Set_Virtual_key(VK_RESET);
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
            Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_NO_IP_ADDRESS_ID,0,0,0,0,ShowCommonTextBox);
		}
		break;
		case WIFI_EVENT_NO_MAC:
		{
			Display_Set_All(0);
            Event_HideAllWnds(1);
            Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_NO_MAC_ADDRESS_ID,0,0,0,0,ShowCommonTextBox);
		}
		break;
		case WIFI_EVENT_WIFI_IS_OK:
		{
			Display_Set_All(0);
            Event_HideAllWnds(1);
            Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_WIFI_IS_OK_ID,0,0,0,0,ShowCommonTextBox);
		}
		break;

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
			Beep_Set_Beeps(6,10,10);
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
    		ret = RET_UNPROCESS_UNDISPLAY;
            Frame_Stage = FRMAE_STATE_WIFI;
            Display_Set_All(0);
            Event_HideAllWnds(1);
            Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SET_TIMEZONE_ID,0,0,0,0,ShowCommonTextBox);
			g_wndRightBottomNumerical._z = 0;
            Event_Set_Wnd_Para(&g_wndRightBottomNumerical,0,0xAA,0,0,2,ShowTimezoneData);
        }
        break;
        case WIFI_EVENT_TIMEZONE_COMPLETE:
        {
            //nCurrentPage = PAGE_SELECTUSER;
            //Frame_Stage = FRAME_STAGE_USER;
            //Event_Set_Virtual_key(VK_RESET);
			extern void Main_Initial_Data(INIT_TYPE type);
			Main_Initial_Data(TYPE_RST_INIT );			
        }
        break;
        case  WIFI_EVENT_OFFLINE:
		break;
									
		case WIFI_EVENT_UPDATE_CANCELED:
		{
			User_Set_VirtualKey(VK_INITIAL);
		}
		break;
		case WIFI_EVENT_AVALIABLE_UPDATE:
		{
    		ret = RET_UNPROCESS_UNDISPLAY;
			if(Frame_Stage ==FRAME_STAGE_ENG ) return ret;
			
			if(Frame_Stage ==FRMAE_STATE_DEMOMODE )
			{
				Event_Flag.DisAllOff =0;
	            Event_Flag.isSaftyKeyOff = 0 ;
			}
			Frame_Stage = FRMAE_STATE_WIFI;
			nCurrentPage = PAGE_UPDATE_INFO;
			ERP_Initial_Data();			
            Display_Set_All(0);
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_SOFTWARE_UPDATE_PRESS_ENTER_ID,0,0,0,0,ShowCommonTextBox);					
		}
		break;
		case WIFI_EVENT_UPDATE_PROGRESS:
		{
    		ret = RET_UNPROCESS_UNDISPLAY;
            //Display_Set_All(0);
			ERP_Initial_Data();			
			Event_Set_Wnd_Para(&g_wndBottomTextBox,NULL,0,0,0,0,ShowWiFiUpdateProcess);					
		}
		break;		
		case WIFI_EVENT_UPDATE_COMPLETED:
		{
			Display_Set_All(0);
            
            BOR_Set_Passcode(0);
			Event_Set_Wnd_Para(&g_wndBottomTextBox,STR_REBOOTING_ID,0,0,0,0,ShowCommonTextBox);		

            Beep_Set_Beeps(6,10,10);
            while(Beep_Get_OK()==false)
            {
				#ifdef WDT_ENABLE
                HAL_Clear_WatchDog();
				#endif                 
                Display_Updata_Data(true);
            }
            HAL_Reset_System(); 
		}
		break;
        
        default: break;
    }
	return ret;
}
#endif


bool Event_Get_RF_Syncing(void)
{
    return(Event_Flag.Syncing);
}

bool Event_Get_DisFlag_AllOn(void)
{
	return(Event_Flag.DisAllOn);
}
bool Event_Get_DisFlag_AllOff(void)
{
	return(Event_Flag.DisAllOff);
}
