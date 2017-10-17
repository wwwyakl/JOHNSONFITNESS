#include "Pro_CustomHr.h"
#ifdef __PROGRAM_CUSTOMHR__

__no_init static PROFILE Profile;
__no_init static const UCHAR *Setting_Array;
UCHAR by_HrSegment = 0;

__no_init static UINT16 aw_CustomHr_SegmentTime[MAX_CUSTOM_HR_SEGMENTS];
__no_init static UCHAR aby_CustomHr_TargetHR[MAX_CUSTOM_HR_SEGMENTS];

__no_init static UINT16 w_CustomHr_TotalTime,w_CustomHr_SumTime;
__no_init static UINT16 w_Last_Point;
__no_init static UINT16 w_Segment_Time;
static UCHAR by_Segment = 0;




void CustomHr_Data_Initial(void)
{
    CUSTOM_CONFIG ProCustomHr_Info = SystemConfig_Get_CustomInfo(User_Get_UserSlot());
    
    w_CustomHr_TotalTime = 0;
    by_Segment = 0;
    for(char i = 0;i < MAX_CUSTOM_HR_SEGMENTS;i++)
    {
        aw_CustomHr_SegmentTime[i] = ProCustomHr_Info.Custom_HR.customHRTime[i];
        aby_CustomHr_TargetHR[i] = ProCustomHr_Info.Custom_HR.customTargetHeartRate[i];
        w_CustomHr_SumTime += aw_CustomHr_SegmentTime[i];
    }
    w_CustomHr_SumTime += 4*60*2;//WarmUp + CoolDown
    w_CustomHr_TotalTime = ProCustomHr_Info.Custom_HR.customHRTotalTime;
    Setting_Array = TM_CustomHr_SetUp;
}
//void ProCustom_TotalTime(void)
//{
//    for(char i = 0;i < MAX_CUSTOM_SEGMENTS;i++)
//    {
//        w_TotalTime += aw_Custom_SegmentTime[i];
//    }
//}

UINT16 ProCustomHr_Get_Time(void)
{
    return aw_CustomHr_SegmentTime[by_Segment];
}
UCHAR ProCustomHr_Get_HRTarget(void)
{
    return aby_CustomHr_TargetHR[by_Segment];
}

PROGRAM_PENDING_EVTS CustomHr_SetUp(ProgramKeyEvts *keyevt)
{
    PROGRAM_PENDING_EVTS pendingEvt;
    pendingEvt.evt = PROGRAM_NONE_EVENT;
    pendingEvt.WorkOutEvts.AllEvts = 0;
    
    if(keyevt->All_KeyEvents == 0) return pendingEvt;
    
    if(keyevt->EventBits.Key_Enter)
    {
        keyevt->EventBits.Key_Enter = false;
        keyevt->EventBits.Key_StateChange = true;
        
        if(Setting_Index == 2)
        {
            if(by_Segment >= 3)
            {
                by_Segment = 0;
                if(w_CustomHr_TotalTime < w_CustomHr_SumTime)
                {
                    Setting_Index = 0;
                    keyevt->EventBits.Key_StateChange = false;
                    pendingEvt.evt = TOTALTIME_LESS_EVENT;
                    return pendingEvt;
                }
                else
                {
                    Data_Set_Time(w_CustomHr_TotalTime);
                }
            }
            else
            {
                Setting_Index = 0;
                by_Segment++;
            }
        }
        Setting_Index++;
    }
    else if(keyevt->EventBits.Key_Reset)
    {
        by_Segment = 0;
        Setting_Index = 0;
        ProCustom_Save_Info();
    }
    
    SettingState = (SETTING_ENUM)Setting_Array[Setting_Index];
    
    switch(SettingState)
    {
//        case SET_PROGRAM:
//            if(keyevt->EventBits.Key_StateChange)
//            {
//                keyevt->EventBits.Key_StateChange = false;
//                pendingEvt.evt = PROGRAM_CHANGE_EVENT;
//                g_StateMachine = PS_SELECTPRG;
//            }
//            break;
        case SET_CUSTOMHR_SEGMENT_TIME:
            if(keyevt->EventBits.Key_StateChange)
            {
                keyevt->EventBits.Key_StateChange = false;
                pendingEvt.evt = TIME_CHANGE_EVENT;
                return pendingEvt;
            }
            w_CustomHr_SumTime -= aw_CustomHr_SegmentTime[by_Segment];
            if(keyevt->EventBits.Key_Up)
            {
                if(aw_CustomHr_SegmentTime[by_Segment] < 10*60)
                {
                    aw_CustomHr_SegmentTime[by_Segment] += 30;
                }
                else
                {
                    aw_CustomHr_SegmentTime[by_Segment] = 10*60;
                }
            }
            else if(keyevt->EventBits.Key_Dn)
            {
                if(aw_CustomHr_SegmentTime[by_Segment] > 30)
                {
                    aw_CustomHr_SegmentTime[by_Segment] -= 30;
                }
                else
                {
                    aw_CustomHr_SegmentTime[by_Segment] = 30;
                }
            }
            w_CustomHr_SumTime += aw_CustomHr_SegmentTime[by_Segment];
            pendingEvt.evt = TIME_CHANGE_EVENT;
            break;
        case SET_CUSTOMHR_SEGMENT_HEART:
            if(keyevt->EventBits.Key_StateChange)
            {
                keyevt->EventBits.Key_StateChange = false;
                pendingEvt.evt = HEART_CHANGE_EVENT;
                return pendingEvt;
            }
            if(keyevt->EventBits.Key_Up)
            {
                if(aby_CustomHr_TargetHR[by_Segment] < 190)
                    aby_CustomHr_TargetHR[by_Segment] += 5;
                else
                    aby_CustomHr_TargetHR[by_Segment] = 190;
            }
            else if(keyevt->EventBits.Key_Dn)
            {
                if(aby_CustomHr_TargetHR[by_Segment] > 50)
                    aby_CustomHr_TargetHR[by_Segment] -= 5; 
                else
                    aby_CustomHr_TargetHR[by_Segment] = 50;
            }
            pendingEvt.evt = HEART_CHANGE_EVENT;
            break;
        case SET_TOTAL_TIME:
            if(keyevt->EventBits.Key_StateChange)
            {
                keyevt->EventBits.Key_StateChange = false;
                pendingEvt.evt = TOTAL_TIME_CHANGE_EVENT;
                return pendingEvt;
            }
            if(keyevt->EventBits.Key_Up)
            {
                if(w_CustomHr_TotalTime < 99*60)
                {
                    w_CustomHr_TotalTime += 60;
                }
                else
                {
                    w_CustomHr_TotalTime = 99*60;
                }
            }
            else if(keyevt->EventBits.Key_Dn)
            {
                if(w_CustomHr_TotalTime > 15*60)
                {
                    w_CustomHr_TotalTime -= 60;
                }
                else
                {
                    w_CustomHr_TotalTime = 60;
                }
            }
            Data_Set_Time(w_CustomHr_TotalTime);
            pendingEvt.evt = TOTAL_TIME_CHANGE_EVENT;
            break;
        case SET_PRESS_START:
            if(keyevt->EventBits.Key_StateChange)
            {
                keyevt->EventBits.Key_StateChange = false;
                pendingEvt.evt = SETTING_COMPLETE_EVENT;
            }
            break;
    }
    
    return pendingEvt;
}

PROFILE *CustomHr_GetShow(PROFILE_TAG tag_index,UCHAR* Dotmatrix_pos)
{
    UINT8 by_SEG;
    UINT8 i,k;
    by_SEG = MAX_CUSTOM_SEGMENTS- 1 - by_Segment;
    if(by_SEG < ProgramParameter->DotMatrixLength)
    {
        *Dotmatrix_pos = ProgramParameter->DotMatrixLength - 1 - by_SEG;
    }
    else
    {
        *Dotmatrix_pos = 0;
    }
    for( i = *Dotmatrix_pos,k = 0; i < ProgramParameter->DotMatrixLength; i++,k++)
    {
        
//        DotmatrixData.SpeedProfile[i] = aby_Custom_Speed[by_Segment+k];
//        DotmatrixData.InclineProfile[i] = aby_Custom_Incline[by_Segment+k];
//        DotmatrixData.ResistanceProfile[i] = aby_Custom_Resistance[by_Segment+k];
    }
    if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8) == TREADMILL)
  	{
        switch(tag_index)
  		{
            case SPEED_PROFILE:
			Profile.ProfileData=DotmatrixData.SpeedProfile;
			break;
            case INCLINE_PROFILE:
			Profile.ProfileData=DotmatrixData.InclineProfile;
			break;
            case DEFAULT_PROFILE:
            
			break;
            default:
            break;
  		}
  	}
    else
  	{
        switch(tag_index)
  		{
            case RESISTANCE_PROFILE:
			Profile.ProfileData=DotmatrixData.ResistanceProfile;
			break;
            case INCLINE_PROFILE:
			Profile.ProfileData=DotmatrixData.InclineProfile;
			break;
            case DEFAULT_PROFILE:
			break;
            default:
            break;
  		}  	
    }
	Profile.Length = ProgramParameter->DotMatrixLength;
    
    return &Profile;
}

PROGRAM_PENDING_EVTS CustomHr_Run(ProgramKeyEvts *keyevt)
{
    UINT16 w_Auto;
    UINT16 w_Diff;
    UINT16 w_Target;
    UINT16 w_Time;
    UINT16 w_Distance;
    static UINT16 WorkOut_Old_Time = 0xFFFF;
    
    PROGRAM_PENDING_EVTS pendingEvt;
    pendingEvt.evt = PROGRAM_NONE_EVENT;
    pendingEvt.WorkOutEvts.AllEvts = 0;
    if(g_StateMachine < PS_WARMUP || g_StateMachine > PS_COOLDOWN)return pendingEvt;
    
    w_Target = w_CustomHr_TotalTime;
    w_Time = Data_Get_Time(ELAPSED_TIME);
    w_Auto = w_Target - w_Time;
    
    if(WorkOut_Old_Time != w_Time)
    {
        WorkOut_Old_Time = w_Time;
        
        if(w_Auto==0)
        {
            if(g_StateMachine != PS_COMPLETED)
            {
                g_StateMachine = PS_COMPLETED;
                pendingEvt.evt = WORKOUT_DATA_REFRESH_EVENT;
                pendingEvt.WorkOutEvts.EvtBit.Workout_Complete_Evt = true;
            }
        }
        else
        {
            if(w_Time)
            {
                w_Diff = w_Time - w_Last_Point;
                if(w_Diff >= w_Segment_Time)
                {
                    w_Last_Point = w_Time;
                    by_Segment++;
                    w_Segment_Time =  aw_CustomHr_SegmentTime[by_Segment];
                    Data_Set_StageTime(w_Segment_Time);
                }
            }
            else
            {
                by_Segment = 0;
                w_Last_Point = 0;
                w_Segment_Time = aw_CustomHr_SegmentTime[by_Segment];
                Data_Set_StageTime(w_Segment_Time);
            }
        }
    }
    
    if(keyevt->EventBits.Key_SpeedUp)
    {
        keyevt->EventBits.Key_SpeedUp=false;
		Data_Get_Speed(Program_Get_Speed(DATA_AUTO,0)+1,1);
       // Data_Set_Speed(Program_Get_Speed(DATA_AUTO,0)+1,SystemConfig_Get_Display_Unit(User_Get_UserSlot( ))==METRIC?METRIC:STANDARD,1);
    }
    else if(keyevt->EventBits.Key_SpeedDn)
    {
        keyevt->EventBits.Key_SpeedDn=false;
		Data_Get_Speed(Program_Get_Speed(DATA_AUTO,0)-1,1);
       // Data_Set_Speed(Program_Get_Speed(DATA_AUTO,0)-1,SystemConfig_Get_Display_Unit(User_Get_UserSlot( ))==METRIC?METRIC:STANDARD,1);
    } 
    if(keyevt->EventBits.Key_InclineUp)
    {
        keyevt->EventBits.Key_InclineUp=false;
        Data_Set_Incline(Program_Get_Incline(0)+5,1);
    }
    else if(keyevt->EventBits.Key_InclineDn)
    {
        keyevt->EventBits.Key_InclineDn=false;
        if(Program_Get_Incline(0))
            Data_Set_Incline(Program_Get_Incline(0)-5,1);
    }      
    if(keyevt->EventBits.Key_ResistanceUp)
    {
        keyevt->EventBits.Key_ResistanceUp=false;
        Data_Set_Resistance(Program_Get_Resistance(0)+1,1);
    }
    else if(keyevt->EventBits.Key_ResistanceDn)
    {
        keyevt->EventBits.Key_ResistanceDn=false;
        Data_Set_Resistance(Program_Get_Resistance(0)-1,1);
    }
    
    return pendingEvt;
}

void ProCustomHr_Save_Info(void)
{
    CUSTOM_CONFIG ProCustomInfo = SystemConfig_Get_CustomInfo(User_Get_UserSlot());
    
    for(char i = 0;i < MAX_CUSTOM_HR_SEGMENTS;i++)
    {
        ProCustomInfo.Custom_HR.customHRTime[i] = aw_CustomHr_SegmentTime[i];
        ProCustomInfo.Custom_HR.customTargetHeartRate[i] = aby_CustomHr_TargetHR[i];
        ProCustomInfo.Custom_HR.customHRTotalTime = w_CustomHr_TotalTime;
    }
    if(SystemConfig_Set_CustomInfo(ProCustomInfo,User_Get_UserSlot()))
    {
        SystemConfigProcess(SVK_SAVE_CUSTOMPRG2FLASH, NULL,0);
    }
}

void initCustomHr(void)
{
	struProgram CustomHr;
    
	CustomHr.Programid = PS_CUSTOM_HR;
	CustomHr.Initial = CustomHr_Data_Initial;
	CustomHr.Setup = CustomHr_SetUp;
	CustomHr.GetProfile = CustomHr_GetShow;
	CustomHr.Workout = CustomHr_Run;
	Program_Register_Function(CustomHr);
}

#endif