#include "User.h"
#include "em_assert.h"
#include "Global_Config.h"
#include "Events_Handler.h"
#include "page.h"
#include "show.h"
#include "USBManager.h"
extern const char * timezone_string[];
extern UINT8 EXTERNAL_EVENTS_BUFFER[EVT_ID_TOTAL];
const unsigned char D_USER_SLOTS = 5; //GUEST + 4 USERS
#define STATE_MACHINE_BUFFER_SIZE  10


__no_init static union
{
    struct
    {
        UINT16 key_up :1;
        UINT16 key_dn :1;
        UINT16 key_enter :1;
        UINT16 key_save: 1;
        UINT16 key_reset: 1;
        UINT16 key_num:1;
        UINT16 key_rf:1;
        UINT16 key_wifi:1;
        UINT16 key_back:1;
        UINT16 key_start_pause:1;
        UINT16 key_eng:1;
        UINT16 key_initial:1;
    }fields;
    UINT16 event;
}User_event;
__no_init	static USER_EVENT gUserPerEvent;

__no_init static USER_SETTING_STATUS g_UserStateMachine;
__no_init static USER_SETTING_STATUS g_aUserStateMachineBuffer[STATE_MACHINE_BUFFER_SIZE];
__no_init static UINT8 g_StateBufferIndex;
__no_init static UINT8 g_UserSlot;
__no_init static UINT8 g_xid[14];
__no_init static UINT8 g_passcode[4];
__no_init static UINT8 subState;
__no_init static UINT16 g_UserWeightSTDX100;
__no_init static UINT8 g_UserAge;
__no_init static UINT8 g_UserGender;
__no_init static UINT8 g_InputIndex;
__no_init static UINT16 g_KeyTemp;
__no_init static UINT8 g_QuickValueExceed;
__no_init static UINT8 g_RF_Setted_User;
__no_init static char g_NumberPassword[LENGTH_OF_PASSWORD];
__no_init static UINT8 g_NumberPasswordIndex;
__no_init UINT16 ClearCnt  ;
__no_init UINT8 g_SetUser_InfoSync ;
void UserClearENGPassword(void)
{
	ClearCnt = 0 ;
	g_NumberPasswordIndex = 0 ;
	memset(g_NumberPassword,'\0',LENGTH_OF_PASSWORD);
}
void User1msISR(void)
{
	if(g_UserStateMachine == SELECT_USER ||g_UserStateMachine == INITIAL_USER )
	{
		if(++ClearCnt>=USER2ENG_SPACETIME)
		{
			ClearCnt = 0 ;
			UserClearENGPassword();
		}
	}
}
static void UserUpdateInfo(void)
{
    if(g_UserSlot > D_USER_SLOTS - 1) return;
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
			if(Get_User_Unit() == METRIC)
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
			

#if 0
static void UserAdjustInputValue(void)
{
    g_KeyTemp %= 100;
    if(g_KeyTemp == 0)
        g_InputIndex = 0;
    else if(g_KeyTemp < 10)
        g_InputIndex = 1;
    else
        g_InputIndex = 2;
}
#endif
static void UserSaveData(void)
{
    USERS_INFO userinfo = SystemConfig_Get_UserInfo(g_UserSlot);
    userinfo.weightStandardX100 = g_UserWeightSTDX100;
     if(userinfo.age > g_UserAge)
     {
       userinfo.year += userinfo.age - g_UserAge;
     }
     else
     {
       userinfo.year -= g_UserAge - userinfo.age;
     }
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
            g_InputIndex = 0;
            evtTemp= USER_AGE_CHANGE_EVT;
            break;
        case EDIT_USER_WEIGHT:
            g_KeyTemp = 0;
            g_InputIndex = 0;
            evtTemp= USER_WEIGHT_CHANGE_EVT;
            break;
        case EDIT_USER_GENDER:
            g_KeyTemp = 0;
            g_InputIndex = 0;
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
	gUserPerEvent = USER_EVT_NONE;
    g_UserStateMachine = USER_STATE_IDEL;
    memset(g_aUserStateMachineBuffer,0,10);
    g_StateBufferIndex = 0;
    g_UserSlot = 0;
    g_UserAge =  30;  //DEFAULT_AGE;
    g_UserGender = 1; //DEFAULT_GENDER;
    User_event.event = 0;
    subState = 0;
    g_InputIndex = 0;
    g_KeyTemp = 0;
    g_RF_Setted_User = 0;
    g_QuickValueExceed = 0 ;
    g_SetUser_InfoSync = 0;
    memset(g_xid,'\0',14);
    memset(g_passcode,'\0',4);
	UserClearENGPassword();
	HAL_SystemTicker_Alloc_ISR(User1msISR);
}

VR_KEY User_Set_VirtualKey(VR_KEY vk)
{ 
	VR_KEY TempKey = vk;

	if(TempKey==VK_PROCESSED ||TempKey == VK_CLEAR_BOOT )  return TempKey;
	if(g_UserStateMachine>USER_STATE_IDEL )
	{
		TempKey = VK_PROCESSED;
	}
//	if(g_UserStateMachine==USER_STATE_IDEL)
//	{
//			g_UserStateMachine = INITIAL_USER;
//	}
    switch(vk)
	{
	    case VK_STOP: 
	        break;
		case VK_ENG:
		{
			User_event.fields.key_eng = 1;
            TempKey = VK_ENG;	
		}
	    break;
	    case VK_PAUSE_START:
	        User_event.fields.key_save = 1;
	        User_event.fields.key_start_pause =1;
			TempKey = VK_PAUSE_START;		
	        break;
	    case VK_RESET:
	        User_event.fields.key_reset = 1;
	        break;
	    case VK_INCLINEUP:
	    case VK_SPEEDUP:	
	    case VK_RESISTANCEUP:
	    case VK_UP:
	        User_event.fields.key_up = 1;
	        break;
	    case VK_INCLINEDOWN:
	    case VK_SPEEDDOWN:		
	    case VK_DOWN:
	    case VK_RESISTANCEDOWN:
	        User_event.fields.key_dn = 1;
	        break;
	    case VK_ENTER:
	    case VK_ENTER_CHANGEDIS:
		{
	        User_event.fields.key_enter = 1;
			if(g_UserStateMachine == SELECT_USER ||g_UserStateMachine == INITIAL_USER)
			{
				if( strcmp(g_NumberPassword, ENG_PASSWORD) ==0)
				{
					UserClearENGPassword();
					User_event.fields.key_enter = 0;
					User_event.fields.key_eng = 1;
                    TempKey = VK_ENG;	
				}
				if( strcmp(g_NumberPassword, CLEARBOOT_PASSWORD) ==0)
				{
					UserClearENGPassword();
					User_event.fields.key_enter = 0;
                    TempKey = VK_CLEAR_BOOT;	
				}
			}
        }
	        break;
	    case VK_BACK:
	        User_event.fields.key_back = 1;
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
		{
			if(g_UserStateMachine == SELECT_USER||g_UserStateMachine == INITIAL_USER )
			{
				ClearCnt = 0 ;
				if( g_NumberPasswordIndex < (LENGTH_OF_PASSWORD-1) )
					g_NumberPassword[g_NumberPasswordIndex++] = (vk-VK_NUM0)+'0';
				else
					UserClearENGPassword();
			}
	        if(g_UserStateMachine ==  INPUT_XID)
	        {
	            if(g_InputIndex < 14)
	            {
	                g_xid[g_InputIndex++] = (vk - VK_NUM0 + 0x30);
	                User_event.fields.key_num = 1;
	            }
	        }
	        else if(g_UserStateMachine ==  INPUT_PASSCODE)
	        {
	            if(g_InputIndex < 4)
	            {
	                g_passcode[g_InputIndex++] = (vk - VK_NUM0 + 0x30);
	                User_event.fields.key_num = 1;
	            }
	        }
	        else if((g_UserStateMachine == EDIT_USER_WEIGHT )||(g_UserStateMachine == EDIT_USER_AGE) )
	        {
	            if(g_InputIndex == 0 && vk!= VK_NUM0)
	            {
	                g_KeyTemp = (vk - VK_NUM0); 
	                g_InputIndex++;
	            }
	            else if(g_InputIndex == 1)
	            {
	                g_KeyTemp =  (g_KeyTemp * 10) + (vk - VK_NUM0);
	                g_InputIndex++;
	            }
	            else if(g_InputIndex == 2)
	            {
	                g_InputIndex++;
	                g_KeyTemp = (g_KeyTemp * 10) + (vk - VK_NUM0);  //<= 999
	            }
				else if(g_InputIndex == 3)	
				{
					g_QuickValueExceed = 1;
				}
				if(!g_QuickValueExceed)
					User_event.fields.key_num = 1;       
			}
		}
        break;
		case VK_INITIAL:
		{
	     	User_event.fields.key_initial = 1;
		}
		break;
	    default:
	        break;
	}
	if(vk>VK_NUM9 ||vk< VK_NUM0)
	{
		UserClearENGPassword();
	}	
	
	return TempKey;
}



char* User_Get_Name(void)
{
    if(g_UserSlot==0)
    {
        return(NULL);
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
            return NULL;
        }
    }
}

UINT8 USer_Get_MaxUser(void)
{
    return(D_USER_SLOTS-1);
}
UINT8 User_Get_UserSlot(void)
{
    return g_UserSlot;
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
	    if(Get_User_Unit() == STANDARD)
	    {
	        ret = ret/100;
	    }
	    else
	    {
	        //极限值范围2^32（4294967296）, 实际计算最大值为752500
	        UINT32 temp = (UINT32)ret * (UINT32)215;
	        temp = temp/474 + 1;
	        ret = (UINT16)(temp / 100);
	    }
	}
    return ret;
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



USER_EVENT User_Process(ReturnEventHandler flag)
{
    USER_EVENT evt = USER_EVT_NONE; 
    if(flag < RET_PROCESS_DISPLAY)
		return evt;
	if(flag == RET_REFRESH_DISPLAY &&g_UserStateMachine !=USER_STATE_IDEL)
	{
		return gUserPerEvent;
	}
	if(g_UserStateMachine==USER_STATE_IDEL && User_event.event !=0 )
	{
			g_UserStateMachine = INITIAL_USER;
	}
    switch(g_UserStateMachine)
    {
    case INITIAL_USER:
        {
            if(1==g_RF_Setted_User)
            {
                g_RF_Setted_User = 0 ;
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
    case INPUT_XID:
        if(User_event.fields.key_enter == 1)
        {
            if(g_InputIndex >= 4)
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
                    g_SetUser_InfoSync = 0x55;
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
				break;
			default:break;
            }
        }
        break;
    case WELCOME_USER:
        {
            if(WIFI_STATUS_LOGIN_SUCCESS != wifiGetStatus() || User_event.event != 0)
            {
                g_UserStateMachine = SELECT_USER;
                evt = USER_SETTING_COMPLETE;
            }
        }
        break;
#endif
    case SELECT_USER:
        if(1==g_RF_Setted_User)
        {
            g_RF_Setted_User = 0 ;
            UserUpdateInfo();
            evt = USER_CHANGE_EVT;
        }
        if(User_event.fields.key_up == 1)
        {
            if(g_UserSlot < D_USER_SLOTS + 1)
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
                g_UserSlot = D_USER_SLOTS + 1;
                evt = USER_DELETE_SELECTED_EVT;
            }
        }
        else if(User_event.fields.key_enter == 1)
        {
            if(g_UserSlot == D_USER_SLOTS + 1)  //delete user
            {
            	if(WIFIIsOnline() == 1)
            	{
	                g_UserSlot =  1;                //user 1
	                UserUpdateInfo();
	                evt = USER_CHANGE_EVT;
	                g_UserStateMachine = DELETE_USER;
	                UserPushStateBuffer(g_UserStateMachine);
            	}
            }
            else if(g_UserSlot == D_USER_SLOTS) //edit user
            {
                g_UserSlot = 1;
                UserUpdateInfo();
                evt = USER_CHANGE_EVT;
                g_UserStateMachine = EDIT_USER;
                UserPushStateBuffer(g_UserStateMachine);
            }
            else if(g_UserSlot == 0 ) //guest
            {
                evt = USER_SETTING_COMPLETE;
                //UserPushStateBuffer(g_UserStateMachine);
				g_UserStateMachine = SELECT_USER;
            }
            else if(g_UserSlot < D_USER_SLOTS)
            {
#ifdef __WIFI_FUNC__
                USERS_INFO userinfo = SystemConfig_Get_UserInfo(g_UserSlot);
                if(User_logined()==0&&WIFIIsOnline() == 1)//lxl20160816
                    userinfo.isLoggedOn =0;
                if(WIFIIsOnline() == 1 && userinfo.isLoggedOn !=1)
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
                    //UserPushStateBuffer(g_UserStateMachine);
					g_UserStateMachine = SELECT_USER;					
                }
#else
                evt = USER_SETTING_COMPLETE;
                //UserPushStateBuffer(g_UserStateMachine);
				g_UserStateMachine = SELECT_USER;				
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
            g_InputIndex = 0;
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
                //SystemConfig_Reset_User_Default(g_UserSlot);
                //g_UserStateMachine = SELECT_USER;
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
            g_InputIndex = 0;
			g_QuickValueExceed  =0;
            if(Get_User_Unit() == STANDARD)
            {
                if(g_UserWeightSTDX100 < SystemConfig_Get_Int(MAX_WEIGHT_STANDARD_UINT16) * 100)
                    g_UserWeightSTDX100 += 100;
            }
            else
            {
                UINT32 maxweight = (UINT32)SystemConfig_Get_Int(MAX_WEIGHT_METRIC_UINT16)*100;
                if(g_UserWeightSTDX100 < maxweight*474/215 )
                    g_UserWeightSTDX100 +=  220;
            }
            evt = USER_WEIGHT_CHANGE_EVT;
        }
        else if(User_event.fields.key_dn == 1)
        {
            g_KeyTemp = 0;
            g_InputIndex = 0;
			g_QuickValueExceed  =0;			
            if(Get_User_Unit() == STANDARD)
            {
                if(g_UserWeightSTDX100 > SystemConfig_Get_Int(MIN_WEIGHT_STANDARD_UINT16) * 100)
                    g_UserWeightSTDX100 -= 100;;
            }
            else
            {
                UINT32 minweight = (UINT32)SystemConfig_Get_Int(MIN_WEIGHT_METRIC_UINT16)*100;
                if(g_UserWeightSTDX100 > minweight *474 /215 )
                    g_UserWeightSTDX100 -=  220;
            }
            
            evt = USER_WEIGHT_CHANGE_EVT;
            
        }
        else if(User_event.fields.key_num == 1)
        {
            evt = USER_WEIGHT_CHANGE_EVT;
        }
        else if(User_event.fields.key_enter == 1)
        {
        	User_Get_QuickeyValue(0);
			
        	if(g_QuickValueExceed == 0)
        	{
	            g_KeyTemp = 0;
	            g_InputIndex = 0;
				g_QuickValueExceed = 0;			
	            g_UserStateMachine = EDIT_USER_AGE;
	            UserPushStateBuffer(g_UserStateMachine);
	            evt = USER_AGE_CHANGE_EVT;
			}
			else
			{
				g_KeyTemp = 0;
	            g_InputIndex = 0;
				g_QuickValueExceed = 0;
				evt = USER_WEIGHT_CHANGE_EVT;
			}
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
	            g_InputIndex = 0;
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
            g_InputIndex = 0;
            if(g_UserAge < 120)
                g_UserAge++;
            evt = USER_AGE_CHANGE_EVT;
            
        }
        else if(User_event.fields.key_dn == 1)
        {
            g_KeyTemp = 0;
			g_QuickValueExceed = 0;						
            g_InputIndex = 0;
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
                    if(userinfo.age > g_UserAge)
                    {
                      userinfo.year += userinfo.age - g_UserAge;
                    }
                    else
                    {
                      userinfo.year -= g_UserAge - userinfo.age;
                    }
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
	               // UserPushStateBuffer(g_UserStateMachine);
					g_UserStateMachine = EDIT_USER_AGE;				
	            }
#else
	            //UserPushStateBuffer(g_UserStateMachine);
	            evt = USER_SETTING_COMPLETE;
				 g_UserStateMachine = EDIT_USER_AGE;			
#endif
			}
			else
			{
				g_KeyTemp = 0;
	            g_InputIndex = 0;
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
	            g_InputIndex = 0;
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
             if(userinfo.age > g_UserAge)
             {
               userinfo.year += userinfo.age - g_UserAge;
             }
             else
             {
               userinfo.year -= g_UserAge - userinfo.age;
             }
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
                //UserPushStateBuffer(g_UserStateMachine);
				g_UserStateMachine = EDIT_USER_GENDER;				
            }
#else
            //UserPushStateBuffer(g_UserStateMachine);
            g_UserStateMachine = EDIT_USER_GENDER;
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
        if( 1 == User_event.fields.key_initial )
        {                
        	evt = USER_INIT_EVT;
        }
	if( 1 == User_event.fields.key_reset)
	{
		evt = USER_RESET_EVT;
	}
    if( 1==User_event.fields.key_start_pause  )
    {
        if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
        {
            if(LCB_Get_Data(G_MOTOR_RPM)==0)
                evt = USER_EVENT_START_WORKOUT;
        }
        else   
           evt = USER_EVENT_START_WORKOUT;
    }
	if(User_event.fields.key_eng == 1)
	{
		evt = USER_EVENT_ENTER_ENGMODE;
	}
    
    User_event.event = 0;
	if(evt > USER_EVT_PROCESSED && evt < USER_SETTING_COMPLETE)
	{
		gUserPerEvent = evt;
	}
    return evt;
}
void USER_Set_UserSlot(UINT8 _slot)
{
    g_UserSlot = _slot;
    g_RF_Setted_User = 1;
}
void Set_g_SetUser_InfoSync(UINT8 _flag)
{
    g_SetUser_InfoSync = _flag;
}
UINT8 Get_g_SetUser_InfoSync(void)
{
    return g_SetUser_InfoSync;   
}

UINT8 Get_User_Unit(void)
{
    UINT8 ret = 0;
    if(g_UserSlot == 0)
    {
        ret = SystemConfig_Get_Char(UNIT_FLAG_UCHAR);
    }
    else
    {
        USERS_INFO userinfor =  SystemConfig_Get_UserInfo(g_UserSlot);
        //if(userinfor.xid[0])
        if(strcmp(userinfor.xid,"")==0)
        {
            ret = SystemConfig_Get_Char(UNIT_FLAG_UCHAR);
        }
        else
        {
            ret = userinfor.displayUnit;
        }
    }
    return ret;
}