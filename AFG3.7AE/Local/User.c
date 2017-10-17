#include "User.h"
#include "em_assert.h"
#include "Global_Config.h"
#include "Events_Handler.h"
#include "page.h"
#include "show.h"
#include "USBManager.h"
#include "Bluetooth_lib.h"
extern const char * timezone_string[];
extern UINT8 EXTERNAL_EVENTS_BUFFER[EVT_ID_TOTAL];
const unsigned char D_USER_SLOTS = 6; //GUEST + 4 USERS
#define STATE_MACHINE_BUFFER_SIZE  10

#include "Program_Inner.h"


__no_init static union
{
    struct
    {
        UINT16 key_up :1;
        UINT16 key_dn :1;
        UINT16 key_update:1;
        UINT16 key_enter :1;
        UINT16 key_save: 1;
        UINT16 key_reset: 1;
        UINT16 key_num:1;
        UINT16 key_rf:1;
        UINT16 key_wifi:1;
        UINT16 key_back:1;
        UINT16 key_start_pause:1;
        UINT16 key_eng:1;
    }fields;
    UINT16 event;
}User_event;

__no_init static USER_SETTING_STATUS g_UserStateMachine;
__no_init static USER_SETTING_STATUS g_aUserStateMachineBuffer[STATE_MACHINE_BUFFER_SIZE];
__no_init static UINT8 g_StateBufferIndex;
__no_init static UINT8 g_UserSlot;
__no_init static UINT8 g_xid[14];
__no_init static UINT8 g_passcode[4];
//__no_init static UINT8 subState;
__no_init static UINT16 g_UserWeightSTDX100;
__no_init static UINT8 g_UserAge;
__no_init static UINT8 g_UserGender;
//__no_init static UINT8 g_InputIndex;
//__no_init static UINT8 g_UserSession[4];
__no_init static UINT16 g_KeyTemp;
__no_init static UINT8 g_QuickValueExceed;
static void UserUpdateInfo(void)
{
    if(g_UserSlot > D_USER_SLOTS) return;
    USERS_INFO userinfo = SystemConfig_Get_UserInfo(g_UserSlot);
    g_UserWeightSTDX100 = userinfo.weightStandardX100;
    g_UserAge = userinfo.age;
    g_UserGender =  userinfo.gender;
}

static void User_Get_QuickeyValue(UINT8 type)
{
	if(type == 0 )
	{
		if(g_KeyTemp!=0)
		{
			if(SystemConfig_Get_Display_Unit(User_Get_UserSlot()) == METRIC)
			{
				if((g_KeyTemp<=(UINT32)SystemConfig_Get_Int(MAX_WEIGHT_METRIC_UINT16))  && (g_KeyTemp>= (UINT32)SystemConfig_Get_Int(MIN_WEIGHT_METRIC_UINT16)))
				{
					g_QuickValueExceed = 0;
					g_UserWeightSTDX100 = (UINT16)((UINT32)(g_KeyTemp) * 47400 / 215);  //<= 60000
				}
				else
					g_QuickValueExceed = 1;
			}
			else
			{
				if(g_KeyTemp<=(UINT32)SystemConfig_Get_Int(MAX_WEIGHT_STANDARD_UINT16) &&g_KeyTemp>= (UINT32)SystemConfig_Get_Int(MIN_WEIGHT_STANDARD_UINT16))
				{
					g_QuickValueExceed = 0;
					g_UserWeightSTDX100 = (UINT32)(g_KeyTemp) * 100; //<= 60000
				}
				else
					g_QuickValueExceed = 1;
			}
		}
	}
	else if(type == 1)
	{
		if(g_KeyTemp!=0)
		{
			if((g_KeyTemp<=130)  && (g_KeyTemp>= 13))
			{
				g_QuickValueExceed = 0;
				g_UserAge = g_KeyTemp;
			}	
			else
				g_QuickValueExceed = 1;
		}
	}
}
			


//static void UserAdjustInputValue(void)
//{
//    g_KeyTemp %= 100;
//    if(g_KeyTemp == 0)
//        g_InputIndex = 0;
//    else if(g_KeyTemp < 10)
//        g_InputIndex = 1;
//    else
//        g_InputIndex = 2;
//}

static void UserSaveData(void)
{
    USERS_INFO userinfo = SystemConfig_Get_UserInfo(g_UserSlot);
    userinfo.weightStandardX100 = g_UserWeightSTDX100;
    userinfo.age = g_UserAge;
    userinfo.gender = g_UserGender;
    while(Beep_Get_OK() == false);
    if((SystemConfig_Set_UserInfo(userinfo,g_UserSlot)))
    {
        SystemConfigProcess(SVK_SAVE_USER2FLASH, &g_UserSlot,sizeof(g_UserSlot));
    }
    Beep_Initial_IO();
#ifdef __WIFI_FUNC__
    if(WIFIIsOnline() == 1)
    {
        WifiEditUser(g_UserSlot, g_UserWeightSTDX100, g_UserAge, g_UserGender);
    }
    //User_Initial_Data();
//#else
    //User_Initial_Data();
#endif
    
}

static UINT8 UserPushStateBuffer(USER_SETTING_STATUS states)
{
    EFM_ASSERT(g_StateBufferIndex < STATE_MACHINE_BUFFER_SIZE - 1);
    UINT8 result = 0;
    if(g_StateBufferIndex < STATE_MACHINE_BUFFER_SIZE - 1)
    {
        g_aUserStateMachineBuffer[g_StateBufferIndex] = states;  
        g_StateBufferIndex++;
        result = 1;
    }
    return result;
    
}


static  USER_EVENT UserPopStateBuffer(void)
{
    USER_EVENT evtTemp = USER_EVT_NONE;
    if(g_StateBufferIndex > 1)
    {
        g_StateBufferIndex --;
        g_UserStateMachine  =  g_aUserStateMachineBuffer[g_StateBufferIndex - 1];
        UserUpdateInfo();
        switch(g_UserStateMachine){
        case SELECT_USER:
        case EDIT_USER:
        case DELETE_USER:
            evtTemp = USER_CHANGE_EVT;
            break;
        case EDIT_USER_AGE:
            g_KeyTemp = 0;
//            g_InputIndex = 0;
            evtTemp= USER_AGE_CHANGE_EVT;
            break;
        case EDIT_USER_WEIGHT:
            g_KeyTemp = 0;
//            g_InputIndex = 0;
            evtTemp= USER_WEIGHT_CHANGE_EVT;
            break;
        case EDIT_USER_GENDER:
            g_KeyTemp = 0;
//            g_InputIndex = 0;
            evtTemp= USER_GENDER_CHANGE_EVT;
            break;
#ifdef __WIFI_FUNC__
        case INPUT_XID:
            memset(g_xid,'\0',14);
            g_InputIndex = 0;
            evtTemp = INPUT_XID_EVT;
            break;
        case INPUT_PASSCODE:
            g_InputIndex = 0;
            memset(g_passcode, '\0', 4);
            evtTemp= INPUT_PASSCODE_EVT;
            break;
#endif
        default:
            EFM_ASSERT(0);
            break;
        }
    }
    return evtTemp;
}

void User_Initial_Data(void)
{
    g_UserStateMachine = INITIAL_USER;
    memset(g_aUserStateMachineBuffer,0,10);
    g_StateBufferIndex = 0;
    if(Bluetooth_Get_ConnectStatus() == 0)
    {
        g_UserSlot = 0;
        g_UserAge =  30;  //DEFAULT_AGE;
        g_UserGender = 1; //DEFAULT_GENDER;
    }
    User_event.event = 0;
//    g_InputIndex = 0;
    g_KeyTemp = 0;
    memset(g_xid,'\0',14);
    memset(g_passcode,'\0',4);
#ifdef __WIFI_FUNC__    
    subState = 0;
	 if(!(WIFIIsOnline()))
	 {
		memset(g_UserSession,0,4);		
	 }
#endif
     UserUpdateInfo();
}

void User_Set_VirtualKey(UINT16 vk)
{ 
    
    UINT8 keyVlaue = (UINT8)(vk&0x00FF);
    if( keyVlaue == VK_INITIAL )
    {
       User_Initial_Data();
       Page_Reset_All_ModalWnds();
       nCurrentPage = PAGE_SELECTUSER;
       ShowPage(nCurrentPage,1);
//       User_event.fields.key_reset = 1;
    }   
    
	UINT16 TempKey = vk & 0xFF00;
    if(TempKey == FRAME_STAGE_AGINGTEST)
    {
        User_event.fields.key_reset = 0;
    }
    if(TempKey != FRAME_STAGE_USER) return;
    TempKey = (UINT8)(vk&0x00FF);

if(VR_MANUAL==TempKey||VR_INTERVALS==TempKey||VR_WEIGHT_LOSS==TempKey\
	||VR_DISTANCE==TempKey||VR_CALORIES==TempKey)
{
    Program_NextStatus(PS_SELECTPRG);
    Event_Handler_SetNewState(FRAME_STAGE_PRGSETUP);
	Program_Set_VirtualKey((UINT16)TempKey|(vk & 0xFF00));
return;
}

    switch(TempKey)
    {
    case VK_UPDATE: 
      Data_Process_Intilal_Data();
      g_UserStateMachine = SELECT_USER;
      User_event.fields.key_update = 1;
      break;
    case VK_STOP: 
      break;
    case VK_PAUSE_START:
      User_event.fields.key_save = 1;
      User_event.fields.key_start_pause =1;
      break;
    case VK_ENG:
      {
        User_event.fields.key_eng = 1;
        TempKey = VK_ENG;	
      }
      break;
    case VK_RESET:
      User_event.fields.key_reset = 1;
      break;
    case VK_SPEEDUP_LONG_PRESS:	
    case VK_INCLINEUP:
    case VK_SPEEDUP:	
    case VK_RESISTANCEUP:
    case VK_UP:
      if(Bluetooth_Get_ConnectStatus() == 0)
        User_event.fields.key_up = 1;
      break;
    case VK_INCLINEDOWN:
    case VK_SPEEDDOWN:		
    case VK_DOWN:
    case VK_RESISTANCEDOWN:
      if(Bluetooth_Get_ConnectStatus() == 0)
        User_event.fields.key_dn = 1;
      break;
    case VK_ENTER:
    case VK_ENTER_CHANGEDIS:
//      if(Bluetooth_Get_ConnectStatus() == 0)
        User_event.fields.key_enter = 1;
      break;
    case VK_BACK:
      User_event.fields.key_back = 1;
      break;
    case VK_RF_SYNC:
      User_event.fields.key_rf = 1;
      break;
    case VK_WIFI_AP:
      User_event.fields.key_wifi = 1;
      break;   
    case VK_NUM0:
    case VK_NUM1:
    case VK_NUM2:
    case VK_NUM3:
    case VK_NUM4:
    case VK_NUM5:
    case VK_NUM6:
    case VK_NUM7:
    case VK_NUM8:
    case VK_NUM9:
#ifdef __WIFI_FUNC__
      if(g_UserStateMachine ==  INPUT_XID)
      {
        if(g_InputIndex < 14)
        {
          g_xid[g_InputIndex++] = (TempKey - VK_NUM0 + 0x30);
          User_event.fields.key_num = 1;
        }
      }
      else if(g_UserStateMachine ==  INPUT_PASSCODE)
      {
        if(g_InputIndex < 4)
        {
          g_passcode[g_InputIndex++] = (TempKey - VK_NUM0 + 0x30);
          User_event.fields.key_num = 1;
        }
      }
      else if((g_UserStateMachine == EDIT_USER_WEIGHT )||(g_UserStateMachine == EDIT_USER_AGE) )
      {
        if(g_InputIndex == 0 && TempKey!= VK_NUM0)
        {
          g_KeyTemp = (TempKey - VK_NUM0); 
          g_InputIndex++;
        }
        else if(g_InputIndex == 1)
        {
          g_KeyTemp =  (g_KeyTemp * 10) + (TempKey - VK_NUM0);
          g_InputIndex++;
        }
        else if(g_InputIndex == 2)
        {
          g_InputIndex++;
          g_KeyTemp = (g_KeyTemp * 10) + (TempKey - VK_NUM0);  //<= 999
        }
        else if(g_InputIndex == 3)	
        {
          g_QuickValueExceed = 1;
        }
        if(!g_QuickValueExceed)
          User_event.fields.key_num = 1;       
      }
#endif
      /* else if(g_UserStateMachine == EDIT_USER_AGE)
      {
      if(g_InputIndex == 0)
      {
      g_KeyTemp = (TempKey - VK_NUM0); 
      g_InputIndex++;
    }
      else if(g_InputIndex == 1)
      {
      g_KeyTemp =  (g_KeyTemp * 10) + (TempKey - VK_NUM0);
      g_InputIndex++;
    }
      else if(g_InputIndex == 2)
      {
      g_KeyTemp = (g_KeyTemp * 10) + (TempKey - VK_NUM0);
      if(g_KeyTemp > 130)
      UserAdjustInputValue();
    }
      g_UserAge = g_KeyTemp;
      User_event.fields.key_num = 1;
    }*/
      
      break;
    default:
      break;
    }
}



char* User_Get_Name(void)
{
    if(g_UserSlot==0)
    {
        return((char*)NULL);
    }
    else
    {
        USERS_INFO usersinfo = SystemConfig_Get_UserInfo(g_UserSlot);
        if(strlen(usersinfo.name)>0)
        {
            return(usersinfo.name);
        }
        else
        {
            return (char*)NULL;
        }
    }
}

UINT8 User_Get_UserSlot(void)
{
    return g_UserSlot;
}
void User_Set_UserSlot(UINT8 UserID)
{
    g_UserSlot = UserID;
}
UINT16 User_Get_Weight(void)
{
    UINT16 ret = g_UserWeightSTDX100;
	if(g_KeyTemp !=0)
	{
		ret =  g_KeyTemp;
	}
	else
	{
	    if(SystemConfig_Get_Display_Unit(User_Get_UserSlot()) == STANDARD)
	    {
	        ret = ret/100;
	    }
	    else
	    {
	        //极限值范围2^32（4294967296）, 实际计算最大值为752500
	        UINT32 temp = (UINT32)ret * (UINT32)215;
	        temp = temp/474;
	        ret = (UINT16)(temp+50)/100;
	    }
	}
    return ret;
}
UINT16 User_Get_StandardWeight(void)
{
    return g_UserWeightSTDX100;
}
UINT16 User_Get_Age(void)
{
	UINT16 ret = g_UserAge ;
	if(g_KeyTemp!=0)
		ret = g_KeyTemp;
    return ret;
}

UINT8 User_Get_Gender(void)
{
    return g_UserGender;
}

USER_SETTING_STATUS User_Get_SettingFlow(void)
{
    return g_UserStateMachine;
}

UINT8 *User_Get_Xid(void)
{
    return g_xid;
}

UINT8 *User_Get_Passcode(void)
{
    return g_passcode;
}


USER_EVENT User_Process(void)
{
    USER_EVENT evt = USER_EVT_NONE; 
    
    switch(g_UserStateMachine)
    {
    case INITIAL_USER:
      {
        if(User_event.fields.key_up == 1)
        {
          g_UserSlot = 0;
          UserUpdateInfo();
#ifdef __WIFI_FUNC__
          subState = 0;
          g_UserStateMachine = SYNC_USER;
#else
          evt = USER_CHANGE_EVT;
          g_UserStateMachine = SELECT_USER;
          UserPushStateBuffer(g_UserStateMachine);
          
#endif
        }
        else if(User_event.fields.key_dn == 1)
        {
          g_UserSlot = D_USER_SLOTS-1;
          UserUpdateInfo();
#ifdef __WIFI_FUNC__
          subState = 0;
          g_UserStateMachine = SYNC_USER;
#else
          evt = USER_CHANGE_EVT;
          g_UserStateMachine = SELECT_USER;
          UserPushStateBuffer(g_UserStateMachine);
#endif           
        }
        else if(User_event.fields.key_enter == 1)
        {
          if(Bluetooth_Get_ConnectStatus() == 0)
          {
            g_UserSlot = 0;
            UserUpdateInfo();
#ifdef __WIFI_FUNC__
            subState = 0;
            g_UserStateMachine = SYNC_USER;
#else
            evt = USER_CHANGE_EVT;
            g_UserStateMachine = SELECT_USER;
            UserPushStateBuffer(g_UserStateMachine);
#endif
          }
          else
          {
            g_KeyTemp = 0;
            g_QuickValueExceed = 0;			
            UserSaveData();
            g_UserStateMachine = USER_SETUP_COMPLETE;
            UserPushStateBuffer(g_UserStateMachine);
            evt = USER_SETTING_COMPLETE;
          }
        }
      }
      break;
      
#ifdef __WIFI_FUNC__
    case SYNC_USER:
      {
        switch(subState)
        {
        case 0:
          if(WIFIIsOnline() == 1)
            subState++;
          else
          {
            evt = USER_CHANGE_EVT;
            g_UserStateMachine = SELECT_USER;
            UserPushStateBuffer(g_UserStateMachine);
          }
          break;
        case 1:
          UpdateUserInfo();
          evt = USER_SYNC_INPROGRESS_EVT;
          g_UserStateMachine = CHECK_CMD_RESULT;
          break;
        }
      }
      break;
#ifdef __WIFI_FUNC__
    case INPUT_XID:
      if(User_event.fields.key_enter == 1)
      {
        if(g_InputIndex > 4)
        {
          g_InputIndex = 0;
          memset(g_passcode, '\0', 4);
          g_UserStateMachine = INPUT_PASSCODE;
          evt = INPUT_PASSCODE_EVT;
        }
      }
      else if(User_event.fields.key_num == 1)
      {
        evt = XID_CHANGE_EVT;
      }
      else if(User_event.fields.key_back == 1)
      {
        evt = UserPopStateBuffer();
      }
      break;
    case INPUT_PASSCODE:
      if(User_event.fields.key_enter == 1)
      {
        if(g_InputIndex == 4)
        {
          UINT8 User_Name_Existed = 0 ;
          
          g_InputIndex = 0;
          
          for(UINT8 temp = 1 ;temp<MAX_SUPPORTED_USERS;temp++)	
          {
            USERS_INFO userinfo = SystemConfig_Get_UserInfo(temp);
            
            if(memcmp(userinfo.xid, g_xid, 14) == 0)
            {
              User_Name_Existed = 1;
              break;
            }
          }
          if(User_Name_Existed)
          {
            User_Initial_Data();
            evt = LOGIN_FAIL_EVT;
          }
          else
          {
            Login(g_UserSlot, g_xid, g_passcode);
            //Login(1,"201510281111","1111");
            g_UserStateMachine = CHECK_CMD_RESULT;
            evt = LOGIN_INPROCESS_EVT;
          }
        }
      }
      else if(User_event.fields.key_num == 1)
      {
        evt = PASSCODE_CHANGE_EVT;
      }
      else if(User_event.fields.key_back == 1)
      {
        evt = UserPopStateBuffer();
      }       
      break;
#endif
    case CHECK_CMD_RESULT:
      {
        WIFI_STATUS_TYPE status = wifiGetStatus();
        switch(status)
        {
        case WIFI_STATUS_SYNC_SUCCESS:
          g_UserStateMachine = SELECT_USER;
          UserPushStateBuffer(g_UserStateMachine);
          g_UserSlot = 1;
          UserUpdateInfo();
          evt = USER_CHANGE_EVT;
          break;
        case WIFI_STATUS_SYNC_FAIL:
          User_Initial_Data();
          evt = USER_SYNC_FAIL_EVT;
          break;
        case WIFI_STATUS_LOGIN_SUCCESS:
          g_UserStateMachine = WELCOME_USER;
          UserPushStateBuffer(g_UserStateMachine);
          g_UserSession[g_UserSlot-1] = 1 ;
          evt = LOGIN_SUCCESS_EVT;
          break;
        case WIFI_STATUS_LOGIN_FAIL:
          User_Initial_Data();
          evt = LOGIN_FAIL_EVT;
          break;
        case WIFI_STATUS_DELETE_SUCCESS:
          User_Initial_Data();
          evt = DELETE_SUCCESS_EVT;
          break;
        case WIFI_STATUS_DELETE_FAIL:
          User_Initial_Data();
          evt = DELETE_FALI_EVT;
          break;
        case WIFI_STATUS_TIMEOUT:
          User_Initial_Data();
          evt = USER_SYNC_TIMEOUT_EVT;
          break;			
        case WIFI_STATUS_OFFLINE:
          memset(g_UserSession,0,4);
          break;
        default:break;
        }
      }
      break;
    case WELCOME_USER:
      {
        if(WIFI_STATUS_LOGIN_SUCCESS != wifiGetStatus() || User_event.event != 0)
        {
          g_UserStateMachine = USER_SETUP_COMPLETE;
          evt = USER_SETTING_COMPLETE;
        }
      }
      break;
#endif
    case SELECT_USER:
      if(User_event.fields.key_update)
      {
        UserUpdateInfo();
        if(nCurrentPage != PAGE_ERP_COUNTDOWN)
          evt = USER_CHANGE_EVT;
      }
      else if(User_event.fields.key_up == 1)
      {
        if(g_UserSlot < D_USER_SLOTS-1)
        {
          g_UserSlot++;
          
          if(g_UserSlot < D_USER_SLOTS)   //user0 ~ 4
          {
            UserUpdateInfo();
            evt = USER_CHANGE_EVT;
          }
          else if(g_UserSlot == D_USER_SLOTS)  //edit user
          {
            evt = USER_EDIT_SELECTED_EVT;
          }
          else if(g_UserSlot == D_USER_SLOTS + 1) //delete user
          {
            evt = USER_DELETE_SELECTED_EVT;
          }
          else;
        }
        else
        {
          g_UserSlot = 0;
          UserUpdateInfo();
          evt = USER_CHANGE_EVT;
        }
      }
      else if(User_event.fields.key_dn == 1)
      {
        if(g_UserSlot > 0)
        {
          g_UserSlot--;
          if(g_UserSlot < D_USER_SLOTS)
          {
            UserUpdateInfo();
            evt = USER_CHANGE_EVT;
          }
          else if(g_UserSlot == D_USER_SLOTS)
          {
            evt = USER_EDIT_SELECTED_EVT;
          }
        }
        else
        {
          g_UserSlot = D_USER_SLOTS - 1;
          UserUpdateInfo();
          evt = USER_CHANGE_EVT;
          //                evt = USER_DELETE_SELECTED_EVT;
        }
      }
      else if(User_event.fields.key_enter == 1)
      {
        if(g_UserSlot == D_USER_SLOTS + 1)  //delete user
        {
          g_UserSlot =  1;                //user 1
          UserUpdateInfo();
          evt = USER_CHANGE_EVT;
          g_UserStateMachine = DELETE_USER;
          UserPushStateBuffer(g_UserStateMachine);
        }
        else if(g_UserSlot == D_USER_SLOTS) //edit user
        {
          g_UserSlot = 1;
          UserUpdateInfo();
          evt = USER_CHANGE_EVT;
          g_UserStateMachine = EDIT_USER;
          UserPushStateBuffer(g_UserStateMachine);
        }
        //            else if(g_UserSlot == 0 ) //guest
        //            {
        //                evt = USER_SETTING_COMPLETE;
        //                g_UserStateMachine = USER_SETUP_COMPLETE;        
        //                UserPushStateBuffer(g_UserStateMachine);
        //            }
        else if(g_UserSlot < D_USER_SLOTS)
        {
#ifdef __WIFI_FUNC__
          USERS_INFO userinfo = SystemConfig_Get_UserInfo(g_UserSlot);
          if(WIFIIsOnline() == 1 && g_UserSession[g_UserSlot-1] !=1)
          {
            memset(g_xid,'0',14);
            //if there is no preset user, start to log in.
            if(memcmp(userinfo.xid, g_xid, 14) == 0)
            {
              memset(g_xid,'\0',14);
              g_InputIndex = 0;
              evt = INPUT_XID_EVT;
              g_UserStateMachine = INPUT_XID;
              UserPushStateBuffer(g_UserStateMachine);
            }
            else //otherwise, directly login.
            {
              memset(g_xid,'\0',14);
              memcpy(g_xid, userinfo.xid, 14);
              memcpy(g_passcode, userinfo.passcode, 4);
              Login(g_UserSlot, g_xid, g_passcode);
              g_UserStateMachine = CHECK_CMD_RESULT;
              evt = LOGIN_INPROCESS_EVT;
            }
          }
          else
          {
            evt = USER_SETTING_COMPLETE;
            g_UserStateMachine = USER_SETUP_COMPLETE;                    
            UserPushStateBuffer(g_UserStateMachine);
          }
#else
          if(Bluetooth_Get_ConnectStatus() == 0)
          {
          evt = USER_WEIGHT_CHANGE_EVT;
          g_UserStateMachine = EDIT_USER_WEIGHT;
          UserPushStateBuffer(g_UserStateMachine);
          }
          else
          {
            evt = USER_SETTING_COMPLETE;
            g_UserStateMachine = USER_SETUP_COMPLETE;                    
            UserPushStateBuffer(g_UserStateMachine);
          }
#endif
        }
      }
      break;
    case EDIT_USER:
      if(User_event.fields.key_up == 1)
      {
        evt = USER_CHANGE_EVT;
        if(g_UserSlot < D_USER_SLOTS - 1)
          g_UserSlot++;
        else
          g_UserSlot = 1;
        UserUpdateInfo();
      }
      else if(User_event.fields.key_dn == 1)
      {
        evt = USER_CHANGE_EVT;
        if(g_UserSlot > 1)
          g_UserSlot--;
        else
          g_UserSlot = D_USER_SLOTS - 1;
        UserUpdateInfo();
      }
      else if(User_event.fields.key_enter == 1)
      {
        g_KeyTemp = 0;
        //            g_InputIndex = 0;
        evt = USER_WEIGHT_CHANGE_EVT;
        g_UserStateMachine = EDIT_USER_WEIGHT;
        UserPushStateBuffer(g_UserStateMachine);
      }
      else if(User_event.fields.key_back == 1)
      {
        evt = UserPopStateBuffer();
      }
      break;
    case DELETE_USER:
      if(User_event.fields.key_up == 1)
      {
        evt = USER_CHANGE_EVT;
        if(g_UserSlot < D_USER_SLOTS - 1)
          g_UserSlot++;
        else
          g_UserSlot = 1;
      }
      else if(User_event.fields.key_dn == 1)
      {
        evt = USER_CHANGE_EVT;
        if(g_UserSlot > 1)
          g_UserSlot--;
        else
          g_UserSlot = D_USER_SLOTS - 1;
      }
      else if(User_event.fields.key_enter == 1)
      {
#ifdef __WIFI_FUNC__
        if(WIFIIsOnline() == 1)
        {
          WIFIDeleteUser(g_UserSlot);
          evt = USER_SYNC_INPROGRESS_EVT;
          g_UserStateMachine = CHECK_CMD_RESULT;
        }
        else
        {
          //                SystemConfig_Reset_User_Default(g_UserSlot);
          //                g_UserStateMachine = SELECT_USER;
          User_Initial_Data();
          g_UserStateMachine = SELECT_USER;
          UserPushStateBuffer(g_UserStateMachine);
          evt = USER_CHANGE_EVT;
        }
#else
        //            SystemConfig_Reset_User_Default(g_UserSlot);
        User_Initial_Data();
        g_UserStateMachine = SELECT_USER;
        UserPushStateBuffer(g_UserStateMachine);	
        evt = USER_CHANGE_EVT;
#endif
      }
      else if(User_event.fields.key_back == 1)
      {
        evt = UserPopStateBuffer();
      }
      break;
    case EDIT_USER_WEIGHT:
      if(User_event.fields.key_up == 1)
      {
        g_KeyTemp = 0;
        //            g_InputIndex = 0;
        g_QuickValueExceed  =0;
        if(SystemConfig_Get_Display_Unit(User_Get_UserSlot()) == STANDARD)
        {
          if(g_UserWeightSTDX100 < SystemConfig_Get_Int(MAX_WEIGHT_STANDARD_UINT16) * 100)
            g_UserWeightSTDX100 += 100;
        }
        else
        {
          //                UINT32 maxweight = (UINT32)SystemConfig_Get_Int(MAX_WEIGHT_METRIC_UINT16);
          //                if(g_UserWeightSTDX100 < maxweight*474/215*100 )
          if(g_UserWeightSTDX100 < SystemConfig_Get_Int(MAX_WEIGHT_STANDARD_UINT16) * 100)
            g_UserWeightSTDX100 +=  220;
        }
        evt = USER_WEIGHT_CHANGE_EVT;
      }
      else if(User_event.fields.key_dn == 1)
      {
        g_KeyTemp = 0;
        //            g_InputIndex = 0;
        g_QuickValueExceed  =0;			
        if(SystemConfig_Get_Display_Unit(User_Get_UserSlot()) == STANDARD)
        {
          if(g_UserWeightSTDX100 > SystemConfig_Get_Int(MIN_WEIGHT_STANDARD_UINT16) * 100)
            g_UserWeightSTDX100 -= 100;
          else
            g_UserWeightSTDX100 = SystemConfig_Get_Int(MIN_WEIGHT_STANDARD_UINT16)*100;
        }
        else
        {
          //                UINT32 minweight = (UINT32)SystemConfig_Get_Int(MIN_WEIGHT_METRIC_UINT16);
          //                if(g_UserWeightSTDX100 > minweight *474 /215*100 )
          if(g_UserWeightSTDX100 > SystemConfig_Get_Int(MIN_WEIGHT_STANDARD_UINT16) * 100)
            g_UserWeightSTDX100 -=  220;
          else
            g_UserWeightSTDX100 = SystemConfig_Get_Int(MIN_WEIGHT_STANDARD_UINT16)*100;
        }
        
        evt = USER_WEIGHT_CHANGE_EVT;
        
      }
      else if(User_event.fields.key_num == 1)
      {
        evt = USER_WEIGHT_CHANGE_EVT;
      }
      else if(User_event.fields.key_enter == 1)
      {
        //        	User_Get_QuickeyValue(0);
        
        //        	if(g_QuickValueExceed == 0)
        //        	{
        g_KeyTemp = 0;
        //	            g_InputIndex = 0;
        g_QuickValueExceed = 0;			
        UserSaveData();
        g_UserStateMachine = USER_SETUP_COMPLETE;
        UserPushStateBuffer(g_UserStateMachine);
        evt = USER_SETTING_COMPLETE;
        //			}
        //			else
        //			{
        //				g_KeyTemp = 0;
        //	            g_InputIndex = 0;
        //				g_QuickValueExceed = 0;
        //				evt = USER_WEIGHT_CHANGE_EVT;
        //			}
      }
      else if(User_event.fields.key_save == 1)
      {
        User_Get_QuickeyValue(0);
        UserSaveData();
      }
      else if(User_event.fields.key_back == 1)
      {
        if(g_KeyTemp != 0)
        {
          g_KeyTemp = 0;
          //	            g_InputIndex = 0;
          g_QuickValueExceed = 0;
          evt = USER_WEIGHT_CHANGE_EVT;
        }
        else
        {
          evt = UserPopStateBuffer();
          
        }
      }
      break;
    case EDIT_USER_AGE:
      if(User_event.fields.key_up == 1)
      {
        g_KeyTemp = 0;
        g_QuickValueExceed = 0;						
        //            g_InputIndex = 0;
        if(g_UserAge < 130)
          g_UserAge++;
        evt = USER_AGE_CHANGE_EVT;
        
      }
      else if(User_event.fields.key_dn == 1)
      {
        g_KeyTemp = 0;
        g_QuickValueExceed = 0;						
        //            g_InputIndex = 0;
        if(g_UserAge > 13)
          g_UserAge--;
        evt = USER_AGE_CHANGE_EVT;
        
      }
      else if(User_event.fields.key_num == 1)
      {
        evt = USER_AGE_CHANGE_EVT;
      }
      else if(User_event.fields.key_enter == 1)
      {
        User_Get_QuickeyValue(1);
        
        if(g_QuickValueExceed==0)
        {
          USERS_INFO userinfo = SystemConfig_Get_UserInfo(g_UserSlot);
          userinfo.weightStandardX100 = g_UserWeightSTDX100;
          userinfo.age = g_UserAge;
          userinfo.gender = g_UserGender;
          if((SystemConfig_Set_UserInfo(userinfo,g_UserSlot)))
          {
            SystemConfigProcess(SVK_SAVE_USER2FLASH, &g_UserSlot,sizeof(g_UserSlot));
          }
#ifdef __WIFI_FUNC__
          if(WIFIIsOnline() == 1)
          {
            WifiEditUser(g_UserSlot, g_UserWeightSTDX100, g_UserAge, g_UserGender);
            evt = USER_SYNC_INPROGRESS_EVT;
            g_UserStateMachine = CHECK_CMD_RESULT;
          }
          else
          {
            evt = USER_SETTING_COMPLETE;
            g_UserStateMachine = USER_SETUP_COMPLETE;
            UserPushStateBuffer(g_UserStateMachine);
          }
#else
          g_UserStateMachine = USER_SETUP_COMPLETE;
          UserPushStateBuffer(g_UserStateMachine);
          evt = USER_SETTING_COMPLETE;
#endif
        }
        else
        {
          g_KeyTemp = 0;
          //	            g_InputIndex = 0;
          g_QuickValueExceed = 0;
          evt = USER_AGE_CHANGE_EVT;
        }
        
        //            g_KeyTemp = 0;
        //            g_InputIndex = 0;
        //            g_UserStateMachine = EDIT_USER_GENDER;
        //            UserPushStateBuffer(g_UserStateMachine);
        //            evt = USER_GENDER_CHANGE_EVT;
      }
      else if(User_event.fields.key_save == 1)
      {
        User_Get_QuickeyValue(1);
        UserSaveData();
      }
      else if(User_event.fields.key_back == 1)
      {
        if(g_KeyTemp != 0)
        {
          g_KeyTemp = 0;
          //	            g_InputIndex = 0;
          g_QuickValueExceed = 0;
          evt = USER_AGE_CHANGE_EVT;
        }
        else
        {
          evt = UserPopStateBuffer();
          
        }			
      }
      break;
    case EDIT_USER_GENDER:
      if(User_event.fields.key_up == 1 || User_event.fields.key_dn == 1)
      {
        if(g_UserGender == 1) g_UserGender = 0;
        else g_UserGender = 1;
        evt = USER_GENDER_CHANGE_EVT;
      }
      else if(User_event.fields.key_save == 1 )
      {
        UserSaveData();
      }
      else if(User_event.fields.key_enter == 1)
      {
        USERS_INFO userinfo = SystemConfig_Get_UserInfo(g_UserSlot);
        userinfo.weightStandardX100 = g_UserWeightSTDX100;
        userinfo.age = g_UserAge;
        userinfo.gender = g_UserGender;
        if((SystemConfig_Set_UserInfo(userinfo,g_UserSlot)))
        {
          SystemConfigProcess(SVK_SAVE_USER2FLASH, &g_UserSlot,sizeof(g_UserSlot));
        }
#ifdef __WIFI_FUNC__
        if(WIFIIsOnline() == 1)
        {
          WifiEditUser(g_UserSlot, g_UserWeightSTDX100, g_UserAge, g_UserGender);
          evt = USER_SYNC_INPROGRESS_EVT;
          g_UserStateMachine = CHECK_CMD_RESULT;
        }
        else
        {
          evt = USER_SETTING_COMPLETE;
          g_UserStateMachine = USER_SETUP_COMPLETE;
          UserPushStateBuffer(g_UserStateMachine);
        }
#else
        g_UserStateMachine = USER_SETUP_COMPLETE;
        UserPushStateBuffer(g_UserStateMachine);
        evt = USER_SETTING_COMPLETE;
#endif
      }
      else if(User_event.fields.key_back == 1)
      {
        evt = UserPopStateBuffer();
      }
      break;
    case USER_SETUP_COMPLETE:
      if(User_event.fields.key_back == 1)
      {
        evt = UserPopStateBuffer();
      }
      break;
      
    default:
      EFM_ASSERT(0);
      break;
    }
	if( 1 == User_event.fields.key_reset)
	{
		User_Initial_Data();
		evt = USER_RESET_EVT;
	}
    if( 1==User_event.fields.key_start_pause  )
    {
        if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
        {
            if(Data_Get_RPM(MOTOR_RPM)==0)
                evt = USER_SETUP_ABORTED_EVT;
        }
        else   
           evt = USER_SETUP_ABORTED_EVT;
    }
    if(User_event.fields.key_wifi == 1 || User_event.fields.key_rf == 1)
	evt = USER_SETUP_ABORTED_EVT;
   
    User_event.event = 0;
    return evt;
}
