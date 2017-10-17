#include"Pro_goals.h"

#ifdef __PROGRAM_GOAL__

__no_init static PROFILE Profile;
__no_init static const UCHAR *Setting_Array;
__no_init static UINT8 SetTargetID;

void Landmrak_Data_Initial(void)
{
    SetTargetID = 0;
    Data_Set_Hight(56);
    DefaultDotmatrix=HIGHT_TYPE;
}
PROGRAM_PENDING_EVTS Landmark_SetUp(ProgramKeyEvts *keyevt)
{
    PROGRAM_PENDING_EVTS pendingEvt;
    pendingEvt.evt=PROGRAM_NONE_EVENT;
    pendingEvt.WorkOutEvts.AllEvts=0;
    
    if(keyevt->EventBits.Key_Enter)
    {
        keyevt->EventBits.Key_Enter=false;
        if(Setting_Array[Setting_Index] != SET_PRESS_START )
        {
            keyevt->EventBits.Key_StateChange=true;
            Setting_Index++;
            if(SET_TIME==Setting_Array[Setting_Index])
            {
                Data_Set_Time(30*60);
            }
        }
    }
    else if(keyevt->EventBits.Key_Back)
    {
        keyevt->EventBits.Key_Back=false;
        if(Setting_Array[Setting_Index] != NO_SETTING )
        {
            keyevt->EventBits.Key_StateChange=true;
            if(SET_TIME==Setting_Array[Setting_Index])
            {
                Data_Set_Time(30*60);
            }
            else if(SET_LEVEL==Setting_Array[Setting_Index])
            {
                Data_Set_Level(Data_Get_Level(MIN_LEVEL));
            }
            Setting_Index--;
        }
    }
    SettingState=(SETTING_ENUM)Setting_Array[Setting_Index];
    switch(SettingState)
    {
        case SET_PROGRAM:
        if(keyevt->EventBits.Key_StateChange)
        {
            keyevt->EventBits.Key_StateChange=false;
            pendingEvt.evt=PROGRAM_CHANGE_EVENT;
            g_StateMachine=PS_SELECTPRG;
            Landmrak_Data_Initial();
        }      
        break;
        case SET_TARGET:
        {
            if(keyevt->EventBits.Key_StateChange)
            {
                keyevt->EventBits.Key_StateChange=false;
                pendingEvt.evt=TARGET_CHANGE_EVENT;
            }  
            if(keyevt->EventBits.Key_Up)
            {
                if(SetTargetID<10)
                {
                    SetTargetID++;   
                }
                else
                {
                    SetTargetID = 0;
                }
                pendingEvt.evt=TARGET_CHANGE_EVENT;
            }
            else if(keyevt->EventBits.Key_Dn)
            {
                if(SetTargetID>0)
                {
                    SetTargetID--;   
                }
                else
                {
                    SetTargetID = 10;
                }
                pendingEvt.evt=TARGET_CHANGE_EVENT;
            }
            switch(SetTargetID)
            {
                case 0:
                {
                    Data_Set_Hight(56);
                }
                break;
                case 1:
                {
                    Data_Set_Hight(93);
                }
                break;
                case 2:
                {
                    Data_Set_Hight(509);
                }
                break;
                case 3:
                {
                    Data_Set_Hight(381);
                }
                break;
                case 4:
                {
                    Data_Set_Hight(541);
                }
                break;
                case 5:
                {
                    Data_Set_Hight(828);
                }
                break;
                case 6:
                {
                    Data_Set_Hight(48);
                }
                break;
                case 7:
                {
                    Data_Set_Hight(139);
                }
                break;
                case 8:
                {
                    Data_Set_Hight(73);
                }
                break;
                case 9:
                {
                    Data_Set_Hight(324);
                }
                break;
                case 10:
                {
                    Data_Set_Hight(710);
                }
                break;
                default:
                break;
                break;
            }
        }
        break;
//        case SET_TIME:
//        if(keyevt->EventBits.Key_StateChange)
//        {
//            keyevt->EventBits.Key_StateChange=false;
//            pendingEvt.evt=TIME_CHANGE_EVENT;
//        }
//        if(keyevt->EventBits.Key_Up)
//        {
//            if(Data_Get_Time(TARGET_TIME)==99*60)
//            {
//                Data_Set_Time(99*60);
//            }
//            else
//            {
//                Data_Set_Time(Data_Get_Time(TARGET_TIME)+60);
//            }
//            pendingEvt.evt=TIME_CHANGE_EVENT;
//        }
//        else if(keyevt->EventBits.Key_Dn)
//        {
//            if(Data_Get_Time(TARGET_TIME)==5*60)
//            {
//                Data_Set_Time(5*60);
//            }
//            else
//            {
//                Data_Set_Time(Data_Get_Time(TARGET_TIME)-60);
//            }    
//            pendingEvt.evt=TIME_CHANGE_EVENT;
//        }
//        break;
        case SET_RESISTANCE:
        if(keyevt->EventBits.Key_StateChange)
        {
            keyevt->EventBits.Key_StateChange=false;
//            pendingEvt.evt=LEVEL_CHANGE_EVENT;
            pendingEvt.evt=CLIMBMILL_LEVEL_CHANGE_EVENT;
        }
        if(keyevt->EventBits.Key_Up)
        {
            keyevt->EventBits.Key_Up=false;
//            Data_Set_Level(Program_Get_WorkoutLevel(0)+1);
//            pendingEvt.evt=LEVEL_CHANGE_EVENT;
            Data_Set_Resistance(Program_Get_Resistance(0)+1,1);
            if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==CLIMB_MILL)
            {
                pendingEvt.evt=CLIMBMILL_LEVEL_CHANGE_EVENT;
            }
            else
            {
                pendingEvt.evt=RESISTANCE_CHANGE_EVENT;
            }
        }
        else if(keyevt->EventBits.Key_Dn)
        {
//            Data_Set_Level(Program_Get_WorkoutLevel(0)-1);
//            pendingEvt.evt=LEVEL_CHANGE_EVENT;
            Data_Set_Resistance(Program_Get_Resistance(0)-1,1);
          if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==CLIMB_MILL)
          {
             pendingEvt.evt=CLIMBMILL_LEVEL_CHANGE_EVENT;
          }
          else
          {
            pendingEvt.evt=RESISTANCE_CHANGE_EVENT;
          }
        }
        break;
        case SET_PRESS_START:
        if(keyevt->EventBits.Key_StateChange)
        {
            keyevt->EventBits.Key_StateChange=false;
            pendingEvt.evt=SETTING_COMPLETE_EVENT;
        }   
    }
    return pendingEvt; 
}

PROFILE *ProgramLandmark_GetShow(PROFILE_TAG tag_index,UCHAR* Dotmatrix_pos)
{
    if((g_StateMachine == PS_SELECTPRG)||(g_StateMachine == PS_SETUP)||(g_StateMachine == PS_WAIT_3S))
    {
        DotmatrixData.HightProfile[0] = 0;
        DotmatrixData.HightProfile[1] = 0;
        for(UINT8 i = 2;i<11;i++)
        {
            DotmatrixData.HightProfile[i] = i-1;
        }
        for(UINT8 i = 20;i>10;i--)
        {
            DotmatrixData.HightProfile[i] = 20-i;
        }
        DotmatrixData.HightProfile[21] = 10;
        DotmatrixData.HightProfile[22] = 11;
        DotmatrixData.HightProfile[23] = 12;
        DotmatrixData.HightProfile[24] = 13;
        DotmatrixData.HightProfile[25] = 12;
        DotmatrixData.HightProfile[26] = 11;
        DotmatrixData.HightProfile[27] = 10;
        DotmatrixData.HightProfile[28] = 0;
        
        DotmatrixData.HightProfile[29] = 14;
        DotmatrixData.HightProfile[30] = 15;
        DotmatrixData.HightProfile[31] = 14;
        DotmatrixData.HightProfile[32] = 0;
        DotmatrixData.HightProfile[33] = 0;
        DotmatrixData.HightProfile[34] = 0;
        if(g_StateMachine == PS_WAIT_3S)
        {
            DefaultDotmatrix=RUN_HIGHT_TYPE;
        }
    }
    else
    {
        for(UINT8 i = 1; i < 5; i++)
        {
            DotmatrixData.HightProfile[i]=Program_Get_Height_Percent()/10;
        }
        for(UINT8 i = 10; i < 16; i++)
        {
            if(Program_Get_Height_Percent() == 100)
            {
                DotmatrixData.HightProfile[i] = 9;
            }
            else
            {
                DotmatrixData.HightProfile[i]=Program_Get_Height_Percent()/10;
                if(DotmatrixData.HightProfile[i] == 0)
                {
                    DotmatrixData.HightProfile[i] = 10;
                }
            }
        }
        for(UINT8 i = 17; i < 23; i++)
        {
            if(Program_Get_Height_Percent() == 100)
            {
                DotmatrixData.HightProfile[i] = 9;
            }
            else
            {
                DotmatrixData.HightProfile[i]=Program_Get_Height_Percent()%10;
            }
        }    
    }
    Profile.ProfileData = DotmatrixData.HightProfile;
    Profile.Length=35;
	return &Profile;
}



PROGRAM_PENDING_EVTS Goal_Run(ProgramKeyEvts *keyevt)
{  
    PROGRAM_PENDING_EVTS pendingEvt;
    pendingEvt.evt=PROGRAM_NONE_EVENT;
    pendingEvt.WorkOutEvts.AllEvts=0;
    if(g_StateMachine<PS_WARMUP||g_StateMachine>PS_COOLDOWN)return pendingEvt;
    
    if(Data_Get_Stairs( REMAINING_HEIGHT) == 0)
    {
        if(g_StateMachine!=PS_COMPLETED)
        {
            g_StateMachine=PS_COMPLETED;
            pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
            pendingEvt.WorkOutEvts.EvtBit.Workout_Complete_Evt=true;
        }
    }
    else
    {
        g_StateMachine=PS_RUN;
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
UINT8 GetTargetID(void)
{
    return SetTargetID;   
}
void initLandmark(void)
{
    struProgram Landmark;
    Landmark.Initial = Landmrak_Data_Initial;
    Landmark.Setup = Landmark_SetUp;
    Landmark.GetProfile=ProgramLandmark_GetShow;
    Landmark.Workout = Goal_Run;
    Landmark.Programid = PS_LANDMARK;
    Program_Add_Array(Landmark);  
    Setting_Array=C50_Landmark_SetUp;
}

#endif




