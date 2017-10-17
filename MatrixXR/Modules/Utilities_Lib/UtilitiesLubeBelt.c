#include "UtilitiesLubeBelt.h"
#include "User.h"
#include "Data_Process.h"
#include "LCB.h"

#include "SystemConfig.h"

__no_init static union
{
    struct
    {
        UINT16 keyresetdistance :1;
        UINT16 keyinitial :1;
        UINT16 keyvalid :1;
    }fields;
    UINT16 event;
}LubeBelt_Key_Event;


typedef enum
{
	LUBEBELT_STATE_START = 0,
	LUBEBELT_STATE_IDEL = LUBEBELT_STATE_START,
	LUBEBELT_STATE_NOTICING,
	LUBEBELT_STATE_ALL
}LUBEBELT_STATE;

__no_init LUBEBELT_STATE LubeBelt_State ;

VR_KEY UTL_LubeBelt_Set_VK(VR_KEY vk)
{
	VR_KEY ret = vk;
	if(vk== VK_PROCESSED ||vk == VK_NULL || vk== VK_CLEAR_BOOT ) return ret ;
	
	if(LubeBelt_State == LUBEBELT_STATE_NOTICING )
	{
		LubeBelt_Key_Event.fields.keyvalid = 1;
		ret = VK_PROCESSED;
	}
	
	if(vk == VK_LUBEBELT_RESET)
	{
		LubeBelt_Key_Event.fields.keyresetdistance = 1;
		ret = VK_PROCESSED;
	}
	else if( vk == VK_INITIAL)
	{
		LubeBelt_Key_Event.fields.keyinitial = 1;	
	}	
	return ret;
}

void LubeBelt_Initial_Data(void)
{
	LubeBelt_State = LUBEBELT_STATE_IDEL;
	LubeBelt_Key_Event.event = 0 ;
	Data_Refresh_BeltDistanceData();
}

LUBEBELT_EVENT LubeBelt_Process(ReturnEventHandler flag)
{
	LUBEBELT_EVENT ret = LUBEBELT_EVENT_NONE;

	if(flag<RET_PROCESS_DISPLAY) return ret;
	if( !(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL && (SystemConfig_Get_Char(MACHINE_MODLE_UINT8)>=1 && SystemConfig_Get_Char(MACHINE_MODLE_UINT8)<=3) || (SystemConfig_Get_Char(MACHINE_MODLE_UINT8)==5 || SystemConfig_Get_Char(MACHINE_MODLE_UINT8)==6)||(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==CLIMB_MILL)))
	{
		ret = LUBEBELT_EVENT_NONE;
		return ret ;
	}
	/*
	UCHAR _type = LCB_Get_MCBType() ;
	if(_type<DC_LCB_30_325HP_JIS || _type>DC_LCB_30_325HP_JIS )
 	{
		ret = LUBEBELT_EVENT_NONE;
		return ret ;
	}*/
	if(LubeBelt_State == LUBEBELT_STATE_NOTICING)
	{
		ret = LUBEBELT_EVENT_PROCESSED ;
	}
	if(LubeBelt_Key_Event.fields.keyvalid )
	{
		LubeBelt_State = LUBEBELT_STATE_IDEL;
	}
	if(LubeBelt_Key_Event.fields.keyresetdistance )
	{
		UINT32 Temp = 0;
		SystemConfigProcess(SVK_ADD_ACCUMULATED_LUBE_MILESX100,&Temp,sizeof(UINT32));
		SystemConfigProcess(SVK_ADD_ACCUMULATED_LUBE_TIME,&Temp,sizeof(UINT32));
        SystemConfigProcess(SVK_SAVE_ACCUMULATEDDATA2FLASH,NULL,0);
		Data_Refresh_BeltDistanceData();
		LubeBelt_State = LUBEBELT_STATE_IDEL;
		ret = LUBEBELT_EVENT_CLEARD;
	}
	if(LubeBelt_Key_Event.fields.keyinitial )
	{
		UINT32 TempDistance = SystemConfig_Get_Long(ACCUMULATED_LUBE_MILESX100_LONG)*10;
		UINT32 LastDistance = Data_Get_BeltDistance() ;
		UINT32 TempLubeTime = SystemConfig_Get_Long(ACCUMULATED_LUBE_TIME);
		UINT32 LastLubeTime = Data_Get_BeltDistance() ;
        if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==CLIMB_MILL)
        {
            if( (TempLubeTime >= C_LUBEBEL_TIME)&&(LastLubeTime < C_LUBEBEL_TIME))
            {
                TempLubeTime = (C_LUBEBEL_TIME-C_LUBEBELT_EXTRA_TIME);
                SystemConfigProcess(SVK_ADD_ACCUMULATED_LUBE_TIME,&TempLubeTime,sizeof(UINT32));
                SystemConfigProcess(SVK_SAVE_ACCUMULATEDDATA2FLASH,NULL,0);
                Data_Refresh_BeltDistanceData();
                LubeBelt_State = LUBEBELT_STATE_NOTICING;
                ret = LUBEBELT_EVENT_NOTICE;
            }
        }
        else
        {
            if( (TempDistance >= C_LUBEBEL_DISTANCE)&&(LastDistance < C_LUBEBEL_DISTANCE))
            {
                TempDistance = (C_LUBEBEL_DISTANCE-C_LUBEBELT_EXTRA_DISTANCE)/10;
                SystemConfigProcess(SVK_ADD_ACCUMULATED_LUBE_MILESX100,&TempDistance,sizeof(UINT32));
                SystemConfigProcess(SVK_SAVE_ACCUMULATEDDATA2FLASH,NULL,0);
                Data_Refresh_BeltDistanceData();
                LubeBelt_State = LUBEBELT_STATE_NOTICING;
                ret = LUBEBELT_EVENT_NOTICE;
            }
        }
	}
    /*if(LubeBelt_Key_Event.fields.keyinitial )
	{
		UINT32 TempLubeTime = SystemConfig_Get_Long(ACCUMULATED_LUBE_TIME);
		UINT32 LastLubeTime = Data_Get_BeltDistance() ;
		if( (TempLubeTime >= C_LUBEBEL_TIME)&&(LastLubeTime < C_LUBEBEL_TIME))
		{
			TempLubeTime = (C_LUBEBEL_TIME-C_LUBEBELT_EXTRA_TIME);
			SystemConfigProcess(SVK_ADD_ACCUMULATED_LUBE_TIME,&TempLubeTime,sizeof(UINT32));
			SystemConfigProcess(SVK_SAVE_ACCUMULATEDDATA2FLASH,NULL,0);
			Data_Refresh_BeltDistanceData();
			LubeBelt_State = LUBEBELT_STATE_NOTICING;
			ret = LUBEBELT_EVENT_NOTICE;
		}
	}*/
	LubeBelt_Key_Event.event = 0 ;
	return ret;
}

