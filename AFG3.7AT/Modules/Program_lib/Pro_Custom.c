#include "Pro_Custom.h"
#include "remote_general.h"
#ifdef __PROGRAM_CUSTOM__

__no_init static PROFILE Profile;
UCHAR by_Segment = 0;
typedef struct
{
  UINT8  LastSegmentNumber;
  UINT8  TotalSegment;
  UINT8  SettingState;
}CustomData;
__no_init static UINT16 aw_Custom_SegmentTime[MAX_CUSTOM_SEGMENTS];
__no_init static UINT16 aw_Custom_SegmentDistance[MAX_CUSTOM_SEGMENTS];
__no_init static UINT16 aby_Custom_Speed[MAX_CUSTOM_SEGMENTS];
__no_init static UCHAR aby_Custom_Incline[MAX_CUSTOM_SEGMENTS];
__no_init static UCHAR aby_Custom_Resistance[MAX_CUSTOM_SEGMENTS];
__no_init struProgram Custom;

__no_init static UINT16 w_Custom_TotalTime,w_Custom_TotalDistance;
//__no_init static UINT16 w_Custom_MAXSegmentDistance;
//__no_init static UINT8 by_Custom_MINSegmentDistance;
//__no_init static UINT8 w_Custom_Speed;
__no_init static UINT16 w_Last_Point;
__no_init static UINT16 w_Segment_Time;
__no_init static UINT16 w_Segment_Distance;
__no_init CustomData g_CustomData;
__no_init bool b_WaitFlag;
__no_init static UINT16 w_Segment_RemainingDistance;
__no_init static bool b_ProCustom_SetFlag;

void ProCustom_Set_SegRemainingDistance(void)
{
    w_Segment_RemainingDistance = w_Segment_Distance+w_Last_Point-Program_Get_Distance(DATA_AUTO)*10;
}
UINT16 ProCustomDistance_Get_TotalTime(void)
{
    UINT16 w_CustomDistance_TotalTime = 0;
    UINT16 w_CustomDistance_SegmentxTime = 0;
    
    w_CustomDistance_SegmentxTime = (UINT32)w_Segment_RemainingDistance*36/(aby_Custom_Speed[by_Segment]+Data_Get_SpeedOffset());
    for(char i = by_Segment+1;i < g_CustomData.TotalSegment;i++)
    {
        w_CustomDistance_TotalTime += (UINT32)aw_Custom_SegmentDistance[i]*36/(aby_Custom_Speed[i]+Data_Get_SpeedOffset());
    }
    w_CustomDistance_TotalTime += w_CustomDistance_SegmentxTime + Data_Get_Time(ELAPSED_OLD_TIME);
    return w_CustomDistance_TotalTime;
}
void ProCustom_Set_APPData(SetCustomprogram *CustomAPPInfo)
{
    if( Program_Get_ProgramID() == PS_CUSTOM_TIME )
    {
        g_CustomData.LastSegmentNumber=CustomAPPInfo->TimeCustomSegnumber;
        g_CustomData.TotalSegment=CustomAPPInfo->TimeCustomSegnumber-1;
        for(char i=0;i < MAX_CUSTOM_SEGMENTS;i++)
        {
            aw_Custom_SegmentTime[i] = CustomAPPInfo->Time[i].Goal;
            if(SystemConfig_Get_Display_Unit(User_Get_UserSlot( ))==METRIC)//user km
            {
                aby_Custom_Speed[i] = ((UINT32)CustomAPPInfo->Time[i].speed*201168/125000+2)/100;
            }
            else
            {
                aby_Custom_Speed[i] = CustomAPPInfo->Time[i].speed/100;
            }
            aby_Custom_Incline[i] = CustomAPPInfo->Time[i].incline;
        }
    }
    else if( Program_Get_ProgramID() == PS_CUSTOM_DISTANCE )
    {
        g_CustomData.LastSegmentNumber=CustomAPPInfo->DistanceCustomSegnumber;
        g_CustomData.TotalSegment=CustomAPPInfo->DistanceCustomSegnumber-1;
        for(char i=0;i < MAX_CUSTOM_SEGMENTS;i++)
        {
            aw_Custom_SegmentDistance[i] = CustomAPPInfo->Distance[i].Goal;
            if(SystemConfig_Get_Display_Unit(User_Get_UserSlot( ))==METRIC)//user km
            {
                aby_Custom_Speed[i] = ((UINT32)CustomAPPInfo->Distance[i].speed*201168/125000+2)/100;
            }
            else
            {
                aby_Custom_Speed[i] = CustomAPPInfo->Distance[i].speed/100;
            }
            aby_Custom_Incline[i] = CustomAPPInfo->Distance[i].incline;
        }
    }
}

void Custom_Data_Initial(void)
{
    CUSTOM_CONFIG ProCustom_Info = SystemConfig_Get_CustomInfo(User_Get_UserSlot());
    
    w_Custom_TotalTime = 0;
    w_Custom_TotalDistance = 0;
    by_Segment = 0;
    g_CustomData.TotalSegment = 0;
    b_ProCustom_SetFlag = false;
        
    if( Program_Get_ProgramID() == PS_CUSTOM_TIME )
    {
        if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
            Custom.Setting_Array = TM_CustomTime_SetUp;
        else if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==ELLIPTICAL)
            Custom.Setting_Array = EP_CustomTime_SetUp;
        g_CustomData.LastSegmentNumber = ProCustom_Info.Custom_Time.customSegmentNumber;
        for(char i = 0;i < MAX_CUSTOM_SEGMENTS;i++)
        {
            aw_Custom_SegmentTime[i] = ProCustom_Info.Custom_Time.customWorkoutTime[i];
            if(aw_Custom_SegmentTime[i] > 10*60)
                aw_Custom_SegmentTime[i] = 60;
            if(SystemConfig_Get_Display_Unit(User_Get_UserSlot( ))==METRIC)//user km
            {
                aby_Custom_Speed[i] = ((UINT32)ProCustom_Info.Custom_Time.customSpeedX10[i]*201168/125000+2)/100;
            }
            else//user mile
            {
                aby_Custom_Speed[i] = ProCustom_Info.Custom_Time.customSpeedX10[i]/100;
            }
            aby_Custom_Incline[i] = ProCustom_Info.Custom_Time.customInclineX10[i];
            aby_Custom_Resistance[i] = ProCustom_Info.Custom_Time.customresistance[i];
        }
    }
    else if( Program_Get_ProgramID() == PS_CUSTOM_DISTANCE )
    {
        if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
            Custom.Setting_Array = TM_CustomDistance_SetUp;
        else if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==ELLIPTICAL)
            Custom.Setting_Array = EP_CustomDistance_SetUp;
        g_CustomData.LastSegmentNumber = ProCustom_Info.Custom_Distance.customSegmentNumber;

        for(char i = 0;i < MAX_CUSTOM_SEGMENTS;i++)
        {
            aw_Custom_SegmentDistance[i] = ProCustom_Info.Custom_Distance.customWorkoutDistance[i];
            if(SystemConfig_Get_Display_Unit(User_Get_UserSlot( ))==METRIC)
                aby_Custom_Speed[i] = ((UINT32)ProCustom_Info.Custom_Distance.customSpeedX10[i]*201168/125000+2)/100;
            else
                aby_Custom_Speed[i] = ProCustom_Info.Custom_Distance.customSpeedX10[i]/100;
            aby_Custom_Incline[i] = ProCustom_Info.Custom_Distance.customInclineX10[i];
            aby_Custom_Resistance[i] = ProCustom_Info.Custom_Distance.customresistance[i];
        }
    }
    Data_Set_SpeedOffset(0);
    Data_Set_InclineOffset(0);
    Data_Set_Speed(aby_Custom_Speed[0],SystemConfig_Get_Display_Unit(User_Get_UserSlot( ))==METRIC,0);
    Data_Set_Incline(aby_Custom_Incline[0],0);
}

void Custom_BeginWorkout(void)
{
    by_Segment = 0;
    if(g_CustomData.TotalSegment <= g_CustomData.LastSegmentNumber)
        g_CustomData.TotalSegment = g_CustomData.LastSegmentNumber;
    else
        g_CustomData.TotalSegment--;
    if( Program_Get_ProgramID() == PS_CUSTOM_TIME )
    {
        for(char i = 0; i < g_CustomData.TotalSegment;i++)
            w_Custom_TotalTime += aw_Custom_SegmentTime[i];
    }
    else if( Program_Get_ProgramID() == PS_CUSTOM_DISTANCE )
    {
        for(char i = 0;i < g_CustomData.TotalSegment;i++)
            w_Custom_TotalDistance += aw_Custom_SegmentDistance[i];
    }
    Data_Set_Time(w_Custom_TotalTime);
    ProCustom_Save_Info();
}

UINT16 ProCustom_SetUpData(UINT8 ParaIndex)
{
	UINT16 w_Dat=0;
    switch(ParaIndex)
	{
		case INDEX_TIME:
        w_Dat = Data_Get_Time(TARGET_TIME);
		break;
		case INDEX_SPEED:
        w_Dat = Data_Get_Speed(TARGET_SPEEDX10_AUTO);
		break;
        case INDEX_DISTANCE:
        w_Dat = aw_Custom_SegmentDistance[by_Segment];
        break;
        case INDEX_SEGMENT:
        w_Dat = by_Segment;
        break;
		default:break;
    }
    return w_Dat;
}
UINT16 ProCustom_RunData(UINT8 ParaIndex)
{
	UINT16 w_Dat=0;
    switch(ParaIndex)
	{
		case INDEX_TIME:
        if(Data_Get_Time(TARGET_TIME))
        {
            w_Dat=Data_Get_Time(REMAINING_TIME);
        }
        else
        {
            w_Dat=Data_Get_Time(ELAPSED_TIME);
        }
        break;
        case INDEX_SPEED:
        aby_Custom_Speed[by_Segment] = Data_Get_Speed(TARGET_SPEEDX10_AUTO);
        break;
        case INDEX_INCLINE:
        aby_Custom_Incline[by_Segment] = Data_Get_Incline(TARGET_INCLINEX10);
        break;
        case INDEX_RESISTANCE:
        aby_Custom_Resistance[by_Segment] = Data_Get_Resistance(TARGET_RESISTANCE);
        break;
    }
    return w_Dat;
}
PROGRAM_PENDING_EVTS Custom_SetUp(ProgramKeyEvts *keyevt)
{
    PROGRAM_PENDING_EVTS pendingEvt;
    pendingEvt.evt = PROGRAM_NONE_EVENT;
    
    if(keyevt->Bit_All == 0) return pendingEvt;
    
    if(keyevt->Bit_Flag.Key_Enter)
    {
        keyevt->Bit_Flag.Key_Enter = false;
        keyevt->Bit_Flag.Key_StateChange = true;
        
        if(Custom.SettingArray_Index == 5)
        {
            if(by_Segment >= 15)
            {
                if(Program_Get_ProgramID() == PS_CUSTOM_TIME)
                {
                    Data_Set_Time(w_Custom_TotalTime);
                }
                else
                {
                    Data_Set_Time(0);
                }
            }
            else
            {
                Custom.SettingArray_Index = 1;
            }
            by_Segment++;
            b_ProCustom_SetFlag = true;
        }
        g_CustomData.TotalSegment = by_Segment+1;
//        if(b_WaitFlag)
//        {
//            b_WaitFlag = false;
//            Custom.SettingArray_Index--;
//        }
        Custom.SettingArray_Index++;
        if(by_Segment < 16)
        {
            Data_Set_Time(aw_Custom_SegmentTime[by_Segment]);
			Data_Set_Speed_Base_User(aby_Custom_Speed[by_Segment],0);
         //   Data_Set_Speed(aby_Custom_Speed[by_Segment],SystemConfig_Get_Display_Unit(User_Get_UserSlot( )),0);
            Data_Set_Incline(aby_Custom_Incline[by_Segment],0);
            Data_Set_Resistance(aby_Custom_Resistance[by_Segment],0);
        }
    }
    else if(keyevt->Bit_Flag.Key_Up || keyevt->Bit_Flag.Key_Dn)
    {
        if(Custom.SettingArray_Index == 2)
            Custom.SettingArray_Index++;
    }
    else if(keyevt->Bit_Flag.Key_Reset)
    {
        by_Segment = 0;
//        b_WaitFlag = false;
        ProCustom_Save_Info();
    }
    g_CustomData.SettingState = Custom.Setting_Array[Custom.SettingArray_Index];
    switch(g_CustomData.SettingState)
    {
//        case SET_PROGRAM:
//            if(keyevt->Bit_Flag.Key_StateChange)
//            {
//                keyevt->Bit_Flag.Key_StateChange = false;
//                pendingEvt.evt = PROGRAM_CHANGE_EVENT;
//                g_StateMachine = PS_SELECTPRG;
//            }
//            break;
        case SET_WAIT_STAGE:
            if(keyevt->Bit_Flag.Key_StateChange)
            {
                keyevt->Bit_Flag.Key_StateChange = false;
//                Custom.SettingArray_Index++;
//                b_WaitFlag = true;
                pendingEvt.evt = CUSTOM_SEGMENT_CHANGE_EVENT;
            }
            break;
        case SET_CUSTOM_SEGMENT_TIME:
            if(keyevt->Bit_Flag.Key_StateChange)
            {
                keyevt->Bit_Flag.Key_StateChange = false;
//                b_WaitFlag = false;
                pendingEvt.evt = TIME_CHANGE_EVENT;
                return pendingEvt;
            }
//            b_WaitFlag = false;
            if(keyevt->Bit_Flag.Key_Up)
            {
                if(aw_Custom_SegmentTime[by_Segment] < 10*60)
                {
                    aw_Custom_SegmentTime[by_Segment] += 30;
                }
                else
                {
                    aw_Custom_SegmentTime[by_Segment] = 10*60;
                }
            }
            else if(keyevt->Bit_Flag.Key_Dn)
            {
                if(aw_Custom_SegmentTime[by_Segment] > 30)
                {
                    aw_Custom_SegmentTime[by_Segment] -= 30;
                }
                else
                {
                    aw_Custom_SegmentTime[by_Segment] = 30;
                }
            }
            Data_Set_Time(aw_Custom_SegmentTime[by_Segment]);
            pendingEvt.evt = TIME_CHANGE_EVENT;
            break;
        case SET_DISTANCE:
            if(keyevt->Bit_Flag.Key_StateChange)
            {
                keyevt->Bit_Flag.Key_StateChange = false;
//                b_WaitFlag = false;
                pendingEvt.evt = DISTANCE_CHANGE_EVENT;
                return pendingEvt;
            }
//            b_WaitFlag = false;
            if(keyevt->Bit_Flag.Key_Up)
            {
                if(aw_Custom_SegmentDistance[by_Segment] < D_CUSTOM_SEGMENT_MAXDISTANCE)
                {
                    aw_Custom_SegmentDistance[by_Segment] += 100;
                }
                else
                {
                    aw_Custom_SegmentDistance[by_Segment] = D_CUSTOM_SEGMENT_MAXDISTANCE;
                }
            }
            else if(keyevt->Bit_Flag.Key_Dn)
            {
                if(aw_Custom_SegmentDistance[by_Segment] > D_CUSTOM_SEGMENT_MINDISTANCE)
                {
                    aw_Custom_SegmentDistance[by_Segment] -= 100;
                }
                else
                {
                    aw_Custom_SegmentDistance[by_Segment] = D_CUSTOM_SEGMENT_MINDISTANCE;
                }
            }
            pendingEvt.evt = DISTANCE_CHANGE_EVENT;
            break;
        case SET_SPEED:
            if(keyevt->Bit_Flag.Key_StateChange)
            {
                keyevt->Bit_Flag.Key_StateChange = false;
                pendingEvt.evt = SPEED_CHANGE_EVENT;
                return pendingEvt;
            }
            if(keyevt->Bit_Flag.Key_Up)
            {
                if(aby_Custom_Speed[by_Segment] < Data_Get_Speed(MAX_SPEEDX10))
                    aby_Custom_Speed[by_Segment]++;
                else
                    aby_Custom_Speed[by_Segment] = Data_Get_Speed(MAX_SPEEDX10);
            }
            else if(keyevt->Bit_Flag.Key_Dn)
            {
                if(aby_Custom_Speed[by_Segment] > Data_Get_Speed(MIN_SPEEDX10))
                    aby_Custom_Speed[by_Segment]--;
                else
                    aby_Custom_Speed[by_Segment] = Data_Get_Speed(MIN_SPEEDX10);
            }
            else if(keyevt->Bit_Flag.Key_QuickKey==true)
            {
                aby_Custom_Speed[by_Segment] = Data_Get_Speed(TARGET_SPEEDX10_AUTO);
            }
			Data_Set_Speed_Base_User(aby_Custom_Speed[by_Segment],0);
            //Data_Set_Speed(aby_Custom_Speed[by_Segment],SystemConfig_Get_Display_Unit(User_Get_UserSlot( )),0);
            pendingEvt.evt = SPEED_CHANGE_EVENT;
            break;
        case SET_INCLINE:
            if(keyevt->Bit_Flag.Key_StateChange)
            {
                keyevt->Bit_Flag.Key_StateChange = false;
                pendingEvt.evt = INCLINE_CHANGE_EVENT;
                return pendingEvt;
            }
            if(keyevt->Bit_Flag.Key_Up)
            {
                if(aby_Custom_Incline[by_Segment] < Data_Get_Incline(MAX_INCLINEX10))
                    aby_Custom_Incline[by_Segment] += 5;
                else
                    aby_Custom_Incline[by_Segment] = Data_Get_Incline(MAX_INCLINEX10);
            }
            else if(keyevt->Bit_Flag.Key_Dn)
            {
                if(aby_Custom_Incline[by_Segment] > Data_Get_Incline(MIN_INCLINEX10))
                    aby_Custom_Incline[by_Segment] -= 5;
                else
                    aby_Custom_Incline[by_Segment] = Data_Get_Incline(MIN_INCLINEX10);
            }
            else if(keyevt->Bit_Flag.Key_QuickKey==true)
            {
                aby_Custom_Incline[by_Segment] = Data_Get_Incline(TARGET_INCLINEX10);
            }
            Data_Set_Incline(aby_Custom_Incline[by_Segment],0);
            pendingEvt.evt = INCLINE_CHANGE_EVENT;
            break;
        case SET_RESISTANCE:
            if(keyevt->Bit_Flag.Key_StateChange)
            {
                keyevt->Bit_Flag.Key_StateChange = false;
                pendingEvt.evt = RESISTANCE_CHANGE_EVENT;
                return pendingEvt;
            }
            if(keyevt->Bit_Flag.Key_Up)
            {
                if(aby_Custom_Resistance[by_Segment] < Data_Get_Resistance(MAX_RESISTANCE))
                    aby_Custom_Resistance[by_Segment]++; 
                else
                    aby_Custom_Resistance[by_Segment] = Data_Get_Resistance(MAX_RESISTANCE);
            }
            else if(keyevt->Bit_Flag.Key_Dn)
            {
                if(aby_Custom_Resistance[by_Segment] > Data_Get_Resistance(MIN_RESISTANCE))
                    aby_Custom_Resistance[by_Segment]--; 
                else
                    aby_Custom_Resistance[by_Segment] = Data_Get_Resistance(MIN_RESISTANCE);
            } 
            else if(keyevt->Bit_Flag.Key_QuickKey==true)
            {
                aby_Custom_Resistance[by_Segment] = Data_Get_Resistance(TARGET_RESISTANCE);
            }
            Data_Set_Resistance(aby_Custom_Resistance[by_Segment],0);
            pendingEvt.evt=RESISTANCE_CHANGE_EVENT;
            break;
        case SET_PRESS_START:
        
            if(keyevt->Bit_Flag.Key_StateChange)
            {
                keyevt->Bit_Flag.Key_StateChange = false;
                pendingEvt.evt = SETTING_COMPLETE_EVENT;
            }    
            break;
    }
    
    return pendingEvt;
}

PROFILE *Custom_GetShow(PROFILE_TAG tag_index,UCHAR* Dotmatrix_pos)
{
    UINT8 by_SEG,by_DisplaySegmentNumber;
    UINT8 i,k;
    
    if(g_CustomData.TotalSegment < g_CustomData.LastSegmentNumber)
        by_DisplaySegmentNumber = g_CustomData.LastSegmentNumber;
    else
        by_DisplaySegmentNumber = g_CustomData.TotalSegment;

    if(Program_Get_Status()<=PS_SETUP)
    {
        by_SEG = by_Segment;
        if(by_SEG < ProgramLibConfigPara->DotMatrixLength)
        {
            *Dotmatrix_pos = by_SEG;
            for( i = *Dotmatrix_pos,k = 0; i < ProgramLibConfigPara->DotMatrixLength; i++,k++)
            {
                if(by_Segment+k < by_DisplaySegmentNumber)
                {
                    Prg_ProfileData.SpeedProfile[i] = aby_Custom_Speed[by_Segment+k];
                    Prg_ProfileData.InclineProfile[i] = aby_Custom_Incline[by_Segment+k];
                    Prg_ProfileData.ResistanceProfile[i] = aby_Custom_Resistance[by_Segment+k];
                }
                else
                {
                    Prg_ProfileData.SpeedProfile[i] = 0xFF;
                    Prg_ProfileData.InclineProfile[i] = 0xFF;
                    Prg_ProfileData.ResistanceProfile[i] = 0xFF;
                }
            }
        }
        else
        {
            *Dotmatrix_pos = ProgramLibConfigPara->DotMatrixLength - 1;
            for(k = 0; k < ProgramLibConfigPara->DotMatrixLength; k++)
            {
                Prg_ProfileData.SpeedProfile[k] = aby_Custom_Speed[by_Segment-*Dotmatrix_pos+k];
                Prg_ProfileData.InclineProfile[k] = aby_Custom_Incline[by_Segment-*Dotmatrix_pos+k];
                Prg_ProfileData.ResistanceProfile[k] = aby_Custom_Resistance[by_Segment-*Dotmatrix_pos+k];
            }
        }
        
    }
    else
    {
        if(by_DisplaySegmentNumber <= ProgramLibConfigPara->DotMatrixLength)
        {
            *Dotmatrix_pos = by_Segment;
        }
        else
        {
            by_SEG = by_DisplaySegmentNumber - 1 - by_Segment;
            if(by_SEG < ProgramLibConfigPara->DotMatrixLength)
            {
                *Dotmatrix_pos = ProgramLibConfigPara->DotMatrixLength - 1-by_SEG;
            }
            else
            {
                *Dotmatrix_pos = 0;
            }
        }
        for( i = *Dotmatrix_pos,k = 0; i < ProgramLibConfigPara->DotMatrixLength; i++,k++)
        {
            if(by_Segment+k < by_DisplaySegmentNumber)
            {
                Prg_ProfileData.SpeedProfile[i] = aby_Custom_Speed[by_Segment+k];
                Prg_ProfileData.InclineProfile[i] = aby_Custom_Incline[by_Segment+k];
                Prg_ProfileData.ResistanceProfile[i] = aby_Custom_Resistance[by_Segment+k];
            }
            else
            {
                Prg_ProfileData.SpeedProfile[i] = 0xFF;
                Prg_ProfileData.InclineProfile[i] = 0xFF;
                Prg_ProfileData.ResistanceProfile[i] = 0xFF;
            }
        }
    }

    if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8) == TREADMILL)
  	{
        switch(tag_index)
  		{
            case SPEED_PROFILE:
			Profile.ProfileData=Prg_ProfileData.SpeedProfile;
			break;
            case INCLINE_PROFILE:
			Profile.ProfileData=Prg_ProfileData.InclineProfile;
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
			Profile.ProfileData=Prg_ProfileData.ResistanceProfile;
			break;
            case INCLINE_PROFILE:
			Profile.ProfileData=Prg_ProfileData.InclineProfile;
			break;
            case DEFAULT_PROFILE:
			break;
            default:
            break;
  		}  	
    }
	Profile.Length = ProgramLibConfigPara->DotMatrixLength;
    
    return &Profile;
}

PROGRAM_PENDING_EVTS Custom_Run(ProgramKeyEvts *keyevt)
{
    UINT16 w_Auto;
    UINT16 w_Diff;
    UINT16 w_Target;
    UINT16 w_Time;
    UINT16 w_Distance;
    static UINT16 WorkOut_Old_Time = 0xFFFF;
    static UINT16 WorkOut_Old_Distance = 0xFFFF;
    
    PROGRAM_PENDING_EVTS pendingEvt;
    pendingEvt.evt = PROGRAM_NONE_EVENT;
    memset(&pendingEvt.WorkOutEvts,0,sizeof(pendingEvt.WorkOutEvts));
    if(Custom.Prg_CurrentStatus() < PS_WARMUP || Custom.Prg_CurrentStatus() > PS_COOLDOWN)return pendingEvt;
    
    if(Program_Get_ProgramID() == PS_CUSTOM_TIME)
    {
        w_Target = w_Custom_TotalTime;
        w_Time = Data_Get_Time(ELAPSED_TIME);
        w_Auto = w_Target - w_Time;
        
        if(WorkOut_Old_Time != w_Time)
        {
            WorkOut_Old_Time = w_Time;
            if(w_Auto==0)
            {
                if(Custom.Prg_CurrentStatus() != PS_COMPLETED)
                {
                    Custom.Prg_NextStatus(PS_COMPLETED);
                    pendingEvt.evt = WORKOUT_DATA_REFRESH_EVENT;
                    pendingEvt.WorkOutEvts.Workout_Complete_Evt = true;
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
                        w_Segment_Time =  aw_Custom_SegmentTime[by_Segment];
                        Data_Set_StageTime(w_Segment_Time);
                        Data_Set_Incline( aby_Custom_Incline[by_Segment],0);

                        if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
                        {
                            if(aby_Custom_Speed[by_Segment+1] != aby_Custom_Speed[by_Segment] || aby_Custom_Incline[by_Segment+1] != aby_Custom_Incline[by_Segment])
                                Beep_Set_Beeps(1,3,1);
							Data_Set_Speed_Base_User(aby_Custom_Speed[by_Segment],0);
                            //Data_Set_Speed( aby_Custom_Speed[by_Segment],SystemConfig_Get_Display_Unit(User_Get_UserSlot( )),0);
                        }
                        else if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==ELLIPTICAL)
                        {
                            if(aby_Custom_Resistance[by_Segment+1] != aby_Custom_Resistance[by_Segment] || aby_Custom_Incline[by_Segment+1] != aby_Custom_Incline[by_Segment])
                                Beep_Set_Beeps(1,3,1);
                            Data_Set_Resistance( aby_Custom_Resistance[by_Segment],0);
                        }
                    }
                }
                else
                {
                    by_Segment = 0;
                    w_Last_Point = 0;
                    w_Segment_Time = aw_Custom_SegmentTime[by_Segment];
                    Data_Set_StageTime(w_Segment_Time);
					Data_Set_Speed_Base_User(aby_Custom_Speed[by_Segment],0);
                    //Data_Set_Speed(aby_Custom_Speed[by_Segment],SystemConfig_Get_Display_Unit(User_Get_UserSlot( )),0);
                    Data_Set_Incline(aby_Custom_Incline[by_Segment],0);
                    Data_Set_Resistance( aby_Custom_Resistance[by_Segment],0);
                }
            }
        }
    }
    else if(Program_Get_ProgramID() == PS_CUSTOM_DISTANCE)
    {
        w_Target = w_Custom_TotalDistance;
        w_Distance = Program_Get_Distance(DATA_AUTO)*10;
        w_Auto = w_Target - w_Distance;
        
        if(WorkOut_Old_Distance != w_Distance)
        {
            if(w_Distance > 0)
                WorkOut_Old_Distance = w_Distance;
            if(w_Auto==0)
            {
                if(Custom.Prg_CurrentStatus() != PS_COMPLETED)
                {
                    
                    Custom.Prg_NextStatus(PS_COMPLETED);
                    pendingEvt.evt = WORKOUT_DATA_REFRESH_EVENT;
                    pendingEvt.WorkOutEvts.Workout_Complete_Evt = true;
                }
            }
            else
            {
                if(w_Distance)
                {
                    w_Diff = w_Distance - w_Last_Point;
                    if(w_Diff >= w_Segment_Distance)
                    {
                        Data_Set_ElapsedOldTime(Data_Get_Time(ELAPSED_TIME));
                        w_Last_Point = w_Distance;
                        by_Segment++;
                        w_Segment_Distance =  aw_Custom_SegmentDistance[by_Segment];
                        if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
                        {
                            if(aby_Custom_Speed[by_Segment+1] != aby_Custom_Speed[by_Segment] || aby_Custom_Incline[by_Segment+1] != aby_Custom_Incline[by_Segment])
                            {  
                                Beep_Set_Beeps(1,3,1);
								Data_Set_Speed_Base_User(aby_Custom_Speed[by_Segment],0);
                                //Data_Set_Speed( aby_Custom_Speed[by_Segment],SystemConfig_Get_Display_Unit(User_Get_UserSlot( )),0);
                                Data_Set_Incline( aby_Custom_Incline[by_Segment],0);
                            }
                        }
                        else if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==ELLIPTICAL)
                        {
                            if(aby_Custom_Resistance[by_Segment+1] != aby_Custom_Resistance[by_Segment] || aby_Custom_Incline[by_Segment+1] != aby_Custom_Incline[by_Segment])
                            {    
                                Beep_Set_Beeps(1,3,1);
                                Data_Set_Resistance( aby_Custom_Resistance[by_Segment],0);
                                Data_Set_Incline( aby_Custom_Incline[by_Segment],0);
                            }
                        }
                    }
                }
                else if(Data_Get_Time(ELAPSED_TIME) == 0)
                {
                    by_Segment = 0;
                    w_Last_Point = 0;
                    w_Segment_Distance = aw_Custom_SegmentDistance[by_Segment];
//                    Data_Set_StageTime(w_Segment_Time);
                    ProCustom_Set_SegRemainingDistance();
                    Data_Set_Speed_Base_User(aby_Custom_Speed[by_Segment],0);
                    //Data_Set_Speed(aby_Custom_Speed[by_Segment],SystemConfig_Get_Display_Unit(User_Get_UserSlot( )),0);
                    Data_Set_Incline(aby_Custom_Incline[by_Segment],0);
                    Data_Set_Resistance( aby_Custom_Resistance[by_Segment],0);
                }
            }
        }
    }
    if(keyevt->Bit_Flag.Key_SPDQuickKey)
    {
        aby_Custom_Speed[by_Segment] = Data_Get_Speed(TARGET_SPEEDX10_AUTO);
        Data_Set_Speed_Base_User(aby_Custom_Speed[by_Segment],0);
    }
    else if(keyevt->Bit_Flag.Key_SpeedUp)
    {
        keyevt->Bit_Flag.Key_SpeedUp=false;
        if(aby_Custom_Speed[by_Segment] < Data_Get_Speed(MAX_SPEEDX10))
            aby_Custom_Speed[by_Segment]++;
        else
            aby_Custom_Speed[by_Segment] = Data_Get_Speed(MAX_SPEEDX10);
		Data_Set_Speed_Base_User(aby_Custom_Speed[by_Segment],0);
    }
    else if(keyevt->Bit_Flag.Key_SpeedDn)
    {
        keyevt->Bit_Flag.Key_SpeedDn=false;
        if(aby_Custom_Speed[by_Segment] > Data_Get_Speed(MIN_SPEEDX10))
            aby_Custom_Speed[by_Segment]--;
        else
            aby_Custom_Speed[by_Segment] = Data_Get_Speed(MIN_SPEEDX10);
		Data_Set_Speed_Base_User(aby_Custom_Speed[by_Segment],0);
    } 
    else if(keyevt->Bit_Flag.Key_INCQuickKey)
    {
        aby_Custom_Incline[by_Segment] = Data_Get_Incline(TARGET_INCLINEX10);
        Data_Set_Incline(aby_Custom_Incline[by_Segment],0);
    }
    else if(keyevt->Bit_Flag.Key_InclineUp)
    {
        keyevt->Bit_Flag.Key_InclineUp=false;
        if(aby_Custom_Incline[by_Segment] < Data_Get_Incline(MAX_INCLINEX10))
            aby_Custom_Incline[by_Segment] += 5;
        else
            aby_Custom_Incline[by_Segment] = Data_Get_Incline(MAX_INCLINEX10);
        Data_Set_Incline(aby_Custom_Incline[by_Segment],0);
    }
    else if(keyevt->Bit_Flag.Key_InclineDn)
    {
        keyevt->Bit_Flag.Key_InclineDn=false;
        if(aby_Custom_Incline[by_Segment] > Data_Get_Incline(MIN_INCLINEX10))
            aby_Custom_Incline[by_Segment] -= 5;
        else
            aby_Custom_Incline[by_Segment] = Data_Get_Incline(MIN_INCLINEX10);
        Data_Set_Incline(aby_Custom_Incline[by_Segment],0);
    }
    else if(keyevt->Bit_Flag.Key_RESQuickKey)
    {
        aby_Custom_Resistance[by_Segment] = Data_Get_Resistance(TARGET_RESISTANCE);
        Data_Set_Resistance(aby_Custom_Resistance[by_Segment],0);
    }
    else if(keyevt->Bit_Flag.Key_ResistanceUp)
    {
        keyevt->Bit_Flag.Key_ResistanceUp=false;
        if(aby_Custom_Resistance[by_Segment] < Data_Get_Resistance(MAX_RESISTANCE))
            aby_Custom_Resistance[by_Segment]++;
        else
            aby_Custom_Resistance[by_Segment] = Data_Get_Resistance(MAX_RESISTANCE);
        Data_Set_Resistance(aby_Custom_Resistance[by_Segment],0);
    }
    else if(keyevt->Bit_Flag.Key_ResistanceDn)
    {
        keyevt->Bit_Flag.Key_ResistanceDn=false;
        if(aby_Custom_Resistance[by_Segment] > Data_Get_Resistance(MIN_RESISTANCE))
            aby_Custom_Resistance[by_Segment]--;
        else
            aby_Custom_Resistance[by_Segment] = Data_Get_Resistance(MIN_RESISTANCE);
        Data_Set_Resistance(aby_Custom_Resistance[by_Segment],0);
    }
    
    return pendingEvt;
}

void ProCustom_Save_Info(void)
{
    if(Bluetooth_Get_ConnectStatus() || !b_ProCustom_SetFlag)return;
    b_ProCustom_SetFlag = false;
    CUSTOM_CONFIG ProCustomInfo = SystemConfig_Get_CustomInfo(User_Get_UserSlot());
    if(g_CustomData.TotalSegment < 2)return;
    if(Program_Get_ProgramID() == PS_CUSTOM_TIME)
    {
        ProCustomInfo.Custom_Time.customWorkoutTotalTime = w_Custom_TotalTime;
        ProCustomInfo.Custom_Time.customSegmentNumber = g_CustomData.TotalSegment;
        for(char i = 0;i < MAX_CUSTOM_SEGMENTS;i++)
        {
            ProCustomInfo.Custom_Time.customWorkoutTime[i] = aw_Custom_SegmentTime[i];
            if(SystemConfig_Get_Display_Unit(User_Get_UserSlot( )) == STANDARD)
                ProCustomInfo.Custom_Time.customSpeedX10[i] = aby_Custom_Speed[i]*100;
            else
                ProCustomInfo.Custom_Time.customSpeedX10[i] = (UINT32)aby_Custom_Speed[i]*12500000/201168+10;
            ProCustomInfo.Custom_Time.customInclineX10[i] = aby_Custom_Incline[i];
            ProCustomInfo.Custom_Time.customresistance[i] = aby_Custom_Resistance[i];
        }
        if(SystemConfig_Set_CustomInfo(ProCustomInfo,User_Get_UserSlot()))
        {
            SystemConfigProcess(SVK_SAVE_CUSTOMPRG2FLASH, NULL,0);
        }
    }
    else if(Program_Get_ProgramID() == PS_CUSTOM_DISTANCE)
    {
        ProCustomInfo.Custom_Distance.customSegmentNumber = g_CustomData.TotalSegment;

        for(char i = 0;i < MAX_CUSTOM_SEGMENTS;i++)
        {
            ProCustomInfo.Custom_Distance.customWorkoutDistance[i] = aw_Custom_SegmentDistance[i];
            if(SystemConfig_Get_Display_Unit(User_Get_UserSlot( )) == STANDARD)
                ProCustomInfo.Custom_Distance.customSpeedX10[i] = aby_Custom_Speed[i]*100;
            else
                ProCustomInfo.Custom_Distance.customSpeedX10[i] = (UINT32)aby_Custom_Speed[i]*12500000/201168+10;
            ProCustomInfo.Custom_Distance.customInclineX10[i] = aby_Custom_Incline[i];
            ProCustomInfo.Custom_Distance.customresistance[i] = aby_Custom_Incline[i];
        }
        if(SystemConfig_Set_CustomInfo(ProCustomInfo,User_Get_UserSlot()))
        {
            SystemConfigProcess(SVK_SAVE_CUSTOMPRG2FLASH, NULL,0);
        }
    }
    Beep_Initial_IO();
}

void initCustom(void)
{
	Custom.Programid = PS_CUSTOM_TIME;
    Custom.SettingArray_Index = 0;
    Custom.BeginWorkout = Custom_BeginWorkout;
	Custom.Prg_Initial = Custom_Data_Initial;
	Custom.Prg_Setup = Custom_SetUp;
	Custom.Prg_ProfileShow = Custom_GetShow;
	Custom.Prg_Workout = Custom_Run;
    Custom.Setting_Array = TM_CustomTime_SetUp;
    Custom.Prg_SetupData = ProCustom_SetUpData;
	Custom.Prg_RunData = ProCustom_RunData;
    
	Program_Register_Function(&Custom);
    Custom.Programid = PS_CUSTOM_DISTANCE;
	Program_Register_Function(&Custom);
    Custom.Programid = PS_CUSTOM;
	Program_Register_Function(&Custom);
}

#endif