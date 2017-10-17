#include"Pro_goal.h"

#ifdef __PROGRAM_GOAL__
typedef struct{
  UINT16 SegmentTime;
  UINT8 WorkoutTotalSegment;
  UINT8 Stage;
  UINT8 SettingState;
}Manual_Data;
typedef struct{
  struProgram StruProgram;
  PROFILE Profile;
  Manual_Data ManualData;
}Goal;
__no_init static Goal g_Distance;
//__no_init static Goal g_Steps;
//__no_init static Goal g_Calories;
void Coal_Data_Initial(void)
{
  
  Data_Set_Speed_Base_User(Data_Get_Speed(MIN_SPEEDX10),0);
  //Data_Set_Speed(Data_Get_Speed(MIN_SPEEDX10),SystemConfig_Get_Display_Unit(User_Get_UserSlot( )),0); 
  Data_Set_Resistance(SystemConfig_Get_Char(MIN_RESISTANCE_UINT8),0);
  Data_Set_Incline(0,0);      
  Data_Set_WarmUP_Time(0);
  Data_Set_CoolDown_Time(0);
  if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
    DefaultDotmatrix=PROFILE_TYPE_SPEED;
  else
    DefaultDotmatrix=PROFILE_TYPE_RESISTANCE;
}
void Goal_distance_Data_Initial(void)
{
  UINT32 l_Dat = 0;
  Data_Set_Speed_Base_User(Data_Get_Speed(MIN_SPEEDX10),0);
  //Data_Set_Speed(Data_Get_Speed(MIN_SPEEDX10),SystemConfig_Get_Display_Unit(User_Get_UserSlot( )),0); 
//  Data_Set_Resistance(SystemConfig_Get_Char(MIN_RESISTANCE_UINT8),0);
  
  Data_Set_Incline(0,0);      
  Data_Set_WarmUP_Time(0);
  Data_Set_CoolDown_Time(0);
  Data_Set_Distance(TARGET_NONE);
  Data_Set_Step(0);
  Data_Set_Time(0);
  Data_Set_Calories(0);
  if(Program_Get_ProgramID()==PS_DISTANCE_GOAL)
  {
    Data_Set_Distance(TARGET_1MILE);
    l_Dat = (UINT32)Data_Get_Distance(TARGET_DISTANCE);
    l_Dat *= 100;               
    l_Dat /= ProgramLibConfigPara->DotMatrixLength;
    g_Distance.ManualData.SegmentTime = l_Dat;
  
  }
  else if(Program_Get_ProgramID()==PS_STEPS_GOAL)
  {
    Data_Set_Step(1000);
    l_Dat = (UINT32)Data_Get_Steps(TARGET_STEPS);
    l_Dat *= 100;               
    l_Dat /= ProgramLibConfigPara->DotMatrixLength;
    g_Distance.ManualData.SegmentTime = l_Dat;
  
  }
  else if(Program_Get_ProgramID()==PS_CALORIES_GOAL)
  {
    Data_Set_Calories(100);
    l_Dat = (UINT32)Data_Get_Calories(TARGET_CALORIES);
    l_Dat *= 100;               
    l_Dat /= ProgramLibConfigPara->DotMatrixLength;
    g_Distance.ManualData.SegmentTime = l_Dat;
  
  }
  if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
    DefaultDotmatrix=PROFILE_TYPE_SPEED;
  else
    DefaultDotmatrix=PROFILE_TYPE_RESISTANCE;
}
void Goal_BeginWorkout(void)
{
  
}
UINT16 Goal_SetUpData(UINT8 ParaIndex)
{
  UINT16 w_Dat=0;
  switch(ParaIndex)
  {
    
  case INDEX_TIME:
    w_Dat=Data_Get_Time(TARGET_TIME);
    break;
  case INDEX_SPEED:
    w_Dat=Data_Get_Speed(TARGET_SPEEDX10_AUTO);
    break;
    case INDEX_INCLINE:
    w_Dat=Data_Get_Incline(TARGET_INCLINEX10);
  default:break;
  }
  return w_Dat;
}
UINT16 Goal_RunData(UINT8 ParaIndex)
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
  case INDEX_WATT:
    w_Dat=Data_Get_Watts(CURRENT_WATT);
    break;
  }
  return w_Dat;
}
PROGRAM_PENDING_EVTS Goal_SetUp(ProgramKeyEvts *keyevt)
{
    UINT32 l_Dat = 0;
    extern TARGET_DIS_ENUM Target_distance;
    extern UINT16 Target_steps;
    extern UINT16 Target_calories;
    PROGRAM_PENDING_EVTS pendingEvt;
    pendingEvt.evt=PROGRAM_NONE_EVENT;
    memset(&pendingEvt.WorkOutEvts,0,sizeof(pendingEvt.WorkOutEvts));
    if(keyevt->Bit_All == 0) return pendingEvt;
    
    if(keyevt->Bit_Flag.Key_Enter)
    {
        keyevt->Bit_Flag.Key_Enter=false;
        if(g_Distance.StruProgram.Setting_Array[g_Distance.StruProgram.SettingArray_Index] != SET_PRESS_START )
        {
            keyevt->Bit_Flag.Key_StateChange=true;
            g_Distance.StruProgram.SettingArray_Index++;
            if(SET_TIME==g_Distance.StruProgram.Setting_Array[g_Distance.StruProgram.SettingArray_Index])
            {
                Data_Set_Time(30*60);
            }
        }
    }
    else if(keyevt->Bit_Flag.Key_Back)
    {
        keyevt->Bit_Flag.Key_Back=false;
        if(g_Distance.StruProgram.Setting_Array[g_Distance.StruProgram.SettingArray_Index] != NO_SETTING )
        {
            keyevt->Bit_Flag.Key_StateChange=true;
            if(SET_PROGRAM==g_Distance.StruProgram.Setting_Array[g_Distance.StruProgram.SettingArray_Index])
            {
                Data_Set_Time(0);
            }
            else if(SET_TIME==g_Distance.StruProgram.Setting_Array[g_Distance.StruProgram.SettingArray_Index])
            {
                Data_Set_Time(0);
            }
            else if(SET_SPEED==g_Distance.StruProgram.Setting_Array[g_Distance.StruProgram.SettingArray_Index])
            {
                
                UINT16 speed ;
                if(SystemConfig_Get_Display_Unit(User_Get_UserSlot( ))==METRIC)
                {
                    speed  = SystemConfig_Get_Int(MIN_KPHX10_UINT16);
                }
                else
                {
                    speed  = SystemConfig_Get_Int(MIN_MPHX10_UINT16);
                }
                Data_Set_Speed_Base_User(speed,0);
                
                // Data_Set_Speed(SystemConfig_Get_Display_Unit(User_Get_UserSlot( ))==METRIC?SystemConfig_Get_Int(MIN_KPHX10_UINT16):SystemConfig_Get_Int(MIN_MPHX10_UINT16),SystemConfig_Get_Display_Unit(User_Get_UserSlot( ))==METRIC?METRIC:STANDARD,0);
            }
            else if(SET_INCLINE==g_Distance.StruProgram.Setting_Array[g_Distance.StruProgram.SettingArray_Index])
            {
                Data_Set_Incline(0,0);
            }
            else if(SET_RESISTANCE==g_Distance.StruProgram.Setting_Array[g_Distance.StruProgram.SettingArray_Index])
            {
                Data_Set_Resistance(Data_Get_Resistance(MIN_RESISTANCE),0);
            }
            g_Distance.StruProgram.SettingArray_Index--;
        }
    }
    g_Distance.ManualData.SettingState=g_Distance.StruProgram.Setting_Array[g_Distance.StruProgram.SettingArray_Index];
    switch(g_Distance.ManualData.SettingState)
    {
        case SET_PROGRAM:
        if(keyevt->Bit_Flag.Key_StateChange)
        {
            keyevt->Bit_Flag.Key_StateChange=false;
            pendingEvt.evt=PROGRAM_CHANGE_EVENT;
            g_Distance.StruProgram.Prg_NextStatus(PS_SELECTPRG);
            Coal_Data_Initial();
        }      
        break;
        case SET_GOAL:
        {
            PROGRAM_ID _tempID = Program_Get_ProgramID();
//            #ifdef PRO_SETUP_DOUBLE_DISPLAY                
//                if(keyevt->Bit_Flag.Key_SettingStateChange)
//                {
//                    keyevt->Bit_Flag.Key_SettingStateChange = false ;
//                    isNormalSettingState = 1 ;
//                    if(_tempID==PS_DISTANCE_GOAL) pendingEvt.evt=DISTANCE_CHANGE_EVENT;
//                    else if(_tempID==PS_CALORIES_GOAL) pendingEvt.evt=CALORIES_CHANGE_EVENT;
//                    else if(_tempID==PS_STEPS_GOAL)  pendingEvt.evt=STEPS_CHANGE_EVENT;   
//                    
//                }
//            #endif
            
            if(keyevt->Bit_Flag.Key_StateChange)
            {
                keyevt->Bit_Flag.Key_StateChange=false;
                
//                #ifdef PRO_SETUP_DOUBLE_DISPLAY     
//                    pendingEvt.evt = SET_GOAL_PRE_EVENT ;
//                    isNormalSettingState = 0 ;
//                    Prg_isr_TimerCounterClear(TIMER_SETTINGSTATECHANGE);
//                #else
                    if(_tempID==PS_DISTANCE_GOAL) pendingEvt.evt=DISTANCE_CHANGE_EVENT;
                    else if(_tempID==PS_CALORIES_GOAL) pendingEvt.evt=CALORIES_CHANGE_EVENT;
                    else if(_tempID==PS_STEPS_GOAL)  pendingEvt.evt=STEPS_CHANGE_EVENT;   
//                #endif    
            }
//            #ifdef PRO_SETUP_DOUBLE_DISPLAY                
//            if( 0 == isNormalSettingState )
//            {
//                if( 0 != keyevt->Bit_All )
//                {
//                    isNormalSettingState = 1;
//                    if(_tempID==PS_DISTANCE_GOAL) pendingEvt.evt=DISTANCE_CHANGE_EVENT;
//                    else if(_tempID==PS_CALORIES_GOAL) pendingEvt.evt=CALORIES_CHANGE_EVENT;
//                    else if(_tempID==PS_STEPS_GOAL)  pendingEvt.evt=STEPS_CHANGE_EVENT;  
//                }
//            }
//            else
//            {
//#endif  
                if(keyevt->Bit_Flag.Key_Up)
                {
                    if(Program_Get_ProgramID()==PS_DISTANCE_GOAL)
                    {
                        if(Target_distance<(TARGET_ALL-1))
                        {
                            Target_distance++;
                        }
                        else
                        {
                            Target_distance=TARGET_1MILE;
                        }
                        Data_Set_Distance(Target_distance);
                        pendingEvt.evt=DISTANCE_CHANGE_EVENT;
                    }
                    else if(Program_Get_ProgramID()==PS_STEPS_GOAL)
                    {
                        if(Target_steps<10000)
                        {
                            Target_steps+=1000;
                        }
                        else
                        {
                            Target_steps=1000;
                        }
                        Data_Set_Step(Target_steps);
                        pendingEvt.evt=STEPS_CHANGE_EVENT;
                    }
                    else if(Program_Get_ProgramID()==PS_CALORIES_GOAL)
                    {
                        if(Target_calories<980)
                        {
                            Target_calories+=20;
                        }
                        else
                        {
                            Target_calories=20;
                        }
                        Data_Set_Calories(Target_calories);
                        pendingEvt.evt=CALORIES_CHANGE_EVENT;
                    }
                }
                else if(keyevt->Bit_Flag.Key_Dn)
                {
                    if(Program_Get_ProgramID()==PS_DISTANCE_GOAL)
                    {
                        if(Target_distance>TARGET_1MILE)
                        {
                            Target_distance--;
                        }
                        else
                        {
                            Target_distance=TARGET_ALL-1;
                        }
                        Data_Set_Distance(Target_distance);
                        pendingEvt.evt=DISTANCE_CHANGE_EVENT;
                    }
                    else if(Program_Get_ProgramID()==PS_STEPS_GOAL)
                    {
                        if(Target_steps>1000)
                        {
                            Target_steps-=1000;
                        }
                        else
                        {
                            Target_steps=10000;
                        }
                        Data_Set_Step(Target_steps);
                        pendingEvt.evt=STEPS_CHANGE_EVENT;
                    }
                    else if(Program_Get_ProgramID()==PS_CALORIES_GOAL)
                    {
                        if(Target_calories>20)
                        {
                            Target_calories-=20;
                        }
                        else
                        {
                            Target_calories=980;
                        }
                        Data_Set_Calories(Target_calories);
                        pendingEvt.evt=CALORIES_CHANGE_EVENT;
                    }
                }
//            }
        }
        break;
        case SET_TIME:
        {
//#ifdef PRO_SETUP_DOUBLE_DISPLAY                
//            if(keyevt->Bit_Flag.Key_SettingStateChange)
//            {
//                keyevt->Bit_Flag.Key_SettingStateChange = false ;
//                isNormalSettingState = 1 ;
//                pendingEvt.evt=SET_TIME_EVENT;
//            }
//#endif
            
            if(keyevt->Bit_Flag.Key_StateChange)
            {
                keyevt->Bit_Flag.Key_StateChange=false;
//#ifdef PRO_SETUP_DOUBLE_DISPLAY                
//                pendingEvt.evt=SET_TIME_PRE_EVENT;
//                isNormalSettingState = 0 ;
//                Prg_isr_TimerCounterClear(TIMER_SETTINGSTATECHANGE);
//#else               
                pendingEvt.evt=TIME_CHANGE_EVENT;
//#endif
            }          
//#ifdef PRO_SETUP_DOUBLE_DISPLAY                
//            if( 0 == isNormalSettingState )
//            {
//                if( 0 != keyevt->Bit_All )
//                {
//                    isNormalSettingState = 1;
//                    pendingEvt.evt=SET_TIME_EVENT;
//                }
//            }
//            else
//            {
//#endif  
                if(keyevt->Bit_Flag.Key_Up)
                {
                    if(Data_Get_Time(TARGET_TIME)==0)
                    {
                        Data_Set_Time(5*60);
                    }
                    else if(Data_Get_Time(TARGET_TIME)<99*60)
                    {
                        Data_Set_Time(Data_Get_Time(TARGET_TIME)+60);
                    }            
                    pendingEvt.evt=TIME_CHANGE_EVENT;
                }
                else if(keyevt->Bit_Flag.Key_Dn)
                {
                    if(Data_Get_Time(TARGET_TIME)>5*60)
                    {
                        Data_Set_Time(Data_Get_Time(TARGET_TIME)-60);
                    }
                    else if(Data_Get_Time(TARGET_TIME)==5*60)
                    {
                        Data_Set_Time(0);  
                    }            
                    pendingEvt.evt=TIME_CHANGE_EVENT;
                }
                
                
//#ifdef PRO_SETUP_DOUBLE_DISPLAY
//            }
//#endif
        }
        break;
        case SET_SPEED:
        {
//#ifdef PRO_SETUP_DOUBLE_DISPLAY                
//            if(keyevt->Bit_Flag.Key_SettingStateChange)
//            {
//                keyevt->Bit_Flag.Key_SettingStateChange = false ;
//                isNormalSettingState = 1 ;
//                pendingEvt.evt=SET_SPEED_EVENT;
//            }
//#endif
            if(keyevt->Bit_Flag.Key_StateChange)
            {
                keyevt->Bit_Flag.Key_StateChange=false;
//#ifdef PRO_SETUP_DOUBLE_DISPLAY                
//                pendingEvt.evt=SET_SPEED_PRE_EVENT;
//                isNormalSettingState = 0 ;
//                Prg_isr_TimerCounterClear(TIMER_SETTINGSTATECHANGE);
//#else               
                pendingEvt.evt=SPEED_CHANGE_EVENT;
//#endif
            }
//#ifdef PRO_SETUP_DOUBLE_DISPLAY                
//            if( 0 == isNormalSettingState )
//            {
//                if( 0 != keyevt->Bit_All )
//                {
//                    isNormalSettingState = 1;
//                    pendingEvt.evt=SET_SPEED_EVENT;
//                }
//            }
//            else
//            {
//#endif  
                if(keyevt->Bit_Flag.Key_Up)
                {
                    Data_Set_Speed_Base_User(Data_Get_Speed(TARGET_SPEEDX10_AUTO)+1,0);
                    pendingEvt.evt=SPEED_CHANGE_EVENT;
                }
                else if(keyevt->Bit_Flag.Key_Dn)
                {
                    Data_Set_Speed_Base_User(Data_Get_Speed(TARGET_SPEEDX10_AUTO)-1,0);
                    pendingEvt.evt=SPEED_CHANGE_EVENT;
                }
                else if(keyevt->Bit_Flag.Key_QuickKey==true)
                {
                    Data_Set_Speed_Base_User(Data_Get_Speed(TARGET_SPEEDX10_AUTO),0);
                    pendingEvt.evt=SPEED_CHANGE_EVENT;
                }
                
//#ifdef PRO_SETUP_DOUBLE_DISPLAY
//            }
//#endif
        }
        break;
        case SET_INCLINE:
        {
//#ifdef PRO_SETUP_DOUBLE_DISPLAY                
//            if(keyevt->Bit_Flag.Key_SettingStateChange)
//            {
//                keyevt->Bit_Flag.Key_SettingStateChange = false ;
//                isNormalSettingState = 1 ;
//                pendingEvt.evt=SET_INCLINE_EVENT;
//            }
//#endif
            
            if(keyevt->Bit_Flag.Key_StateChange)
            {
                keyevt->Bit_Flag.Key_StateChange=false;
//#ifdef PRO_SETUP_DOUBLE_DISPLAY                
//                pendingEvt.evt=SET_INCLINE_PRE_EVENT;
//                isNormalSettingState = 0 ;
//                Prg_isr_TimerCounterClear(TIMER_SETTINGSTATECHANGE);
//#else               
                pendingEvt.evt=INCLINE_CHANGE_EVENT;
//#endif
            }
//#ifdef PRO_SETUP_DOUBLE_DISPLAY                
//            if( 0 == isNormalSettingState )
//            {
//                if( 0 != keyevt->Bit_All )
//                {
//                    isNormalSettingState = 1;
//                    pendingEvt.evt=SET_INCLINE_EVENT;
//                }
//            }
//            else
//            {
//#endif 
                if(keyevt->Bit_Flag.Key_Up)
                {
                    if(Data_Get_Incline(TARGET_INCLINEX10)<Data_Get_Incline(MAX_INCLINEX10))
                        Data_Set_Incline(Program_Get_Incline(0)+5,0);        
                    pendingEvt.evt=INCLINE_CHANGE_EVENT;
                }
                else if(keyevt->Bit_Flag.Key_Dn)
                {
                    if(Program_Get_Incline(0))
                        Data_Set_Incline(Program_Get_Incline(0)-5,0);
                    pendingEvt.evt=INCLINE_CHANGE_EVENT;
                }
//#ifdef PRO_SETUP_DOUBLE_DISPLAY
//            }
//        }
//#endif
        }
        break;
        case SET_RESISTANCE:
        {
            if(keyevt->Bit_Flag.Key_StateChange)
            {
                keyevt->Bit_Flag.Key_StateChange=false;
            }
            if(keyevt->Bit_Flag.Key_Up)
            {
                Data_Set_Resistance(Program_Get_Resistance(0)+1,0);
            }
            else if(keyevt->Bit_Flag.Key_Dn)
            {
                Data_Set_Resistance(Program_Get_Resistance(0)-1,0);
            }
            else if(keyevt->Bit_Flag.Key_QuickKey==true)
            {
                Data_Set_Resistance(Data_Get_Resistance(TARGET_RESISTANCE),0);
            }
            pendingEvt.evt=RESISTANCE_CHANGE_EVENT;
        }
        break;
        case SET_PRESS_START:
        if(keyevt->Bit_Flag.Key_StateChange)
        {
            keyevt->Bit_Flag.Key_StateChange=false;
            pendingEvt.evt=SETTING_COMPLETE_EVENT;
        }    
        break;
    }
    //  if(Data_Get_Time(TARGET_TIME) == 0) 
    //  {
    //    g_Distance.ManualData.SegmentTime = 6000;  
    //  }
    //  else
    //  {
    //    l_Dat = (UINT32)Data_Get_Time(TARGET_TIME);
    //    l_Dat *= 100;               
    //    l_Dat /= ProgramLibConfigPara->DotMatrixLength;
    //    g_Distance.ManualData.SegmentTime = l_Dat;
    //  }
    
    if(Program_Get_ProgramID()==PS_DISTANCE_GOAL)
    {
        l_Dat = (UINT32)Data_Get_Distance(TARGET_DISTANCE);
        l_Dat *= 100;               
        l_Dat /= ProgramLibConfigPara->DotMatrixLength;
        g_Distance.ManualData.SegmentTime = l_Dat;
    }
    else if(Program_Get_ProgramID()==PS_CALORIES_GOAL)
    {
        l_Dat = (UINT32)Data_Get_Calories(TARGET_CALORIES);
        l_Dat *= 100;               
        l_Dat /= ProgramLibConfigPara->DotMatrixLength;
        g_Distance.ManualData.SegmentTime = l_Dat;
    }
    else if(Program_Get_ProgramID()==PS_STEPS_GOAL)
    {
        l_Dat = (UINT32)Data_Get_Steps(TARGET_STEPS);
        l_Dat *= 100;               
        l_Dat /= ProgramLibConfigPara->DotMatrixLength;
        g_Distance.ManualData.SegmentTime = l_Dat;
    }
    
    return pendingEvt; 
}

PROFILE *Goal_ProfileShow(PROFILE_TAG tag_index,UCHAR* Dotmatrix_pos)
{
  *Dotmatrix_pos=g_Distance.ManualData.Stage%ProgramLibConfigPara->DotMatrixLength;
  
  for(UINT8 i = *Dotmatrix_pos; i < ProgramLibConfigPara->DotMatrixLength; i++)
  {
    Prg_ProfileData.SpeedProfile[i]=Data_Get_Speed(TARGET_SPEEDX10_AUTO);
    Prg_ProfileData.InclineProfile[i]=Data_Get_Incline(TARGET_INCLINEX10);
    Prg_ProfileData.ResistanceProfile[i]=Data_Get_Resistance(TARGET_RESISTANCE);
  }
  if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
  {
    switch(tag_index)
    {
    case SPEED_PROFILE:
      g_Distance.Profile.ProfileData=Prg_ProfileData.SpeedProfile;
      break;
    case INCLINE_PROFILE:
      g_Distance.Profile.ProfileData=Prg_ProfileData.InclineProfile;
      break;
    case DEFAULT_PROFILE:
      g_Distance.Profile.ProfileData=Prg_ProfileData.SpeedProfile;
      break;
    default:
      g_Distance.Profile.ProfileData=Prg_ProfileData.SpeedProfile;
      break;
    }
  }
  else
  {
    switch(tag_index)
    {
    case RESISTANCE_PROFILE:
      g_Distance.Profile.ProfileData=Prg_ProfileData.ResistanceProfile;
      break;
    case INCLINE_PROFILE:
      g_Distance.Profile.ProfileData=Prg_ProfileData.InclineProfile;
      break;
    case DEFAULT_PROFILE:
      g_Distance.Profile.ProfileData=Prg_ProfileData.ResistanceProfile;
      break;
    default:
      g_Distance.Profile.ProfileData=Prg_ProfileData.ResistanceProfile;
      break;
    }  
  }
  g_Distance.Profile.Length=ProgramLibConfigPara->DotMatrixLength;
  return &g_Distance.Profile;
}



PROGRAM_PENDING_EVTS Goal_Run(ProgramKeyEvts *keyevt)
{  
  PROGRAM_PENDING_EVTS pendingEvt;
  static UINT16 LastValue=0xffff;
  UINT32 l_Dat = 0;
  pendingEvt.evt=PROGRAM_NONE_EVENT;
  memset(&pendingEvt.WorkOutEvts,0,sizeof(pendingEvt.WorkOutEvts));
  
  if(g_Distance.StruProgram.Prg_CurrentStatus()<PS_WARMUP
     ||g_Distance.StruProgram.Prg_CurrentStatus()>PS_COOLDOWN)
  {
    return pendingEvt;
  }
  {
    if(LastValue!=Data_Get_Time(ELAPSED_TIME))
    {
      LastValue=Data_Get_Time(ELAPSED_TIME);
//      if(Data_Get_Time(TARGET_TIME) == 0) //counting up
//      {
//        if(Data_Get_Time(ELAPSED_TIME) < MAX_WORKOUT_TIME -1) //data.c to handle this
//        {
//          l_Dat = (UINT32)Data_Get_Time(ELAPSED_TIME);
//          l_Dat *= 100;
//          l_Dat /=  g_Distance.ManualData.SegmentTime;
//          l_Dat %= ProgramLibConfigPara->DotMatrixLength;
//          g_Distance.ManualData.Stage = (UINT8)l_Dat; //This truncation is safe.
//          g_Distance.StruProgram.Prg_NextStatus(PS_RUN);
//        }
//      }
      if(Program_Get_ProgramID()==PS_DISTANCE_GOAL)
      {
        if(Data_Get_Distance(REMAINING_DISTANCE) == 0)
        {
          if(g_Distance.StruProgram.Prg_CurrentStatus()!=PS_COMPLETED)
          {
            g_Distance.StruProgram.Prg_NextStatus(PS_COMPLETED);
            pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
            pendingEvt.WorkOutEvts.Workout_Complete_Evt=true;
            
          }
        }
        else
        {
          if(Data_Get_Distance(ELAPSED_DISTANCE))
          {
            l_Dat = Data_Get_Distance(ELAPSED_DISTANCE);
            l_Dat *= 100;
            l_Dat /= g_Distance.ManualData.SegmentTime;
            g_Distance.ManualData.Stage = (UINT8)l_Dat; //safe truncation
          }
          else
            g_Distance.ManualData.Stage = 0;
          g_Distance.StruProgram.Prg_NextStatus(PS_RUN);
          
        }
      }
      else if(Program_Get_ProgramID()==PS_STEPS_GOAL)
      {
        if(Data_Get_Steps(REMAINING_STEPS) == 0)
        {
          if(g_Distance.StruProgram.Prg_CurrentStatus()!=PS_COMPLETED)
          {
            g_Distance.StruProgram.Prg_NextStatus(PS_COMPLETED);
            pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
            pendingEvt.WorkOutEvts.Workout_Complete_Evt=true;
            
          }
        }
        else
        {
          if(Data_Get_Steps(ELAPSED_STEPS))
          {
            l_Dat = Data_Get_Steps(ELAPSED_STEPS);
            l_Dat *= 100;
            l_Dat /= g_Distance.ManualData.SegmentTime;
            g_Distance.ManualData.Stage = (UINT8)l_Dat; //safe truncation
          }
          else
            g_Distance.ManualData.Stage = 0;
          g_Distance.StruProgram.Prg_NextStatus(PS_RUN);
          
        }
      }
      else if(Program_Get_ProgramID()==PS_CALORIES_GOAL)
      {
        if(Data_Get_Calories(REMAINING_CALORIES) == 0)
        {
          if(g_Distance.StruProgram.Prg_CurrentStatus()!=PS_COMPLETED)
          {
            g_Distance.StruProgram.Prg_NextStatus(PS_COMPLETED);
            pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
            pendingEvt.WorkOutEvts.Workout_Complete_Evt=true;
            
          }
        }
        else
        {
          if(Data_Get_Calories(ELAPSED_CALORIES))
          {
            l_Dat = Data_Get_Calories(ELAPSED_CALORIES);
            l_Dat *= 100;
            l_Dat /= g_Distance.ManualData.SegmentTime;
            g_Distance.ManualData.Stage = (UINT8)l_Dat; //safe truncation
          }
          else
            g_Distance.ManualData.Stage = 0;
          g_Distance.StruProgram.Prg_NextStatus(PS_RUN);
          
        }
      }
//      else   //counting down
//      {
//        if(Data_Get_Time(REMAINING_TIME) == 0)
//        {
//          if(g_Distance.StruProgram.Prg_CurrentStatus()!=PS_COMPLETED)
//          {
//            g_Distance.StruProgram.Prg_NextStatus(PS_COMPLETED);
//            pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
//            pendingEvt.WorkOutEvts.Workout_Complete_Evt=true;
//            
//          }
//        }
//        else
//        {
//          if(Data_Get_Time(ELAPSED_TIME))
//          {
//            l_Dat = Data_Get_Time(ELAPSED_TIME);
//            l_Dat *= 100;
//            l_Dat /= g_Distance.ManualData.SegmentTime;
//            g_Distance.ManualData.Stage = (UINT8)l_Dat; //safe truncation
//          }
//          else
//            g_Distance.ManualData.Stage = 0;
//          g_Distance.StruProgram.Prg_NextStatus(PS_RUN);
//          
//        }
//      }
    }
    if(keyevt->Bit_Flag.Key_SpeedUp)
    {
      keyevt->Bit_Flag.Key_SpeedUp=false;
      Data_Set_Speed_Base_User(Data_Get_Speed(TARGET_SPEEDX10_AUTO)+1,1);		//zsf		
      //Data_Set_Speed(Data_Get_Speed(TARGET_SPEEDX10_AUTO)+1,SystemConfig_Get_Display_Unit(User_Get_UserSlot( ))==METRIC?METRIC:STANDARD,1);
    }
    else if(keyevt->Bit_Flag.Key_SpeedDn)
    {
      keyevt->Bit_Flag.Key_SpeedDn=false;
      Data_Set_Speed_Base_User(Data_Get_Speed(TARGET_SPEEDX10_AUTO)-1,1);		//zsf		
      //Data_Set_Speed(Data_Get_Speed(TARGET_SPEEDX10_AUTO)-1,SystemConfig_Get_Display_Unit(User_Get_UserSlot( ))==METRIC?METRIC:STANDARD,1);
    } 
    if(keyevt->Bit_Flag.Key_InclineUp)
    {
      keyevt->Bit_Flag.Key_InclineUp=false;
      Data_Set_Incline(Program_Get_Incline(0)+5,1);    //zsf
    }
    else if(keyevt->Bit_Flag.Key_InclineDn)
    {
      keyevt->Bit_Flag.Key_InclineDn=false;
      if(Program_Get_Incline(0))
        Data_Set_Incline(Program_Get_Incline(0)-5,1);    //zsf
    }      
    if(keyevt->Bit_Flag.Key_ResistanceUp)
    {
      keyevt->Bit_Flag.Key_ResistanceUp=false;
      Data_Set_Resistance(Program_Get_Resistance(0)+1,0);
    }
    else if(keyevt->Bit_Flag.Key_ResistanceDn)
    {
      keyevt->Bit_Flag.Key_ResistanceDn=false;
      Data_Set_Resistance(Program_Get_Resistance(0)-1,0);
    }     
  }
  return pendingEvt;
}

void initGoal(void)
{
    g_Distance.StruProgram.Programid = PS_DISTANCE_GOAL;
    g_Distance.StruProgram.SettingArray_Index= 0;
    
    g_Distance.StruProgram.Prg_Initial = Goal_distance_Data_Initial;
    g_Distance.StruProgram.BeginWorkout=Goal_BeginWorkout;
    g_Distance.StruProgram.Prg_Setup = Goal_SetUp;
    g_Distance.StruProgram.Prg_ProfileShow=Goal_ProfileShow;
    g_Distance.StruProgram.Prg_Workout = Goal_Run;
    g_Distance.StruProgram.Prg_SetupData=Goal_SetUpData;
    g_Distance.StruProgram.Prg_RunData=Goal_RunData;
    Program_Register_Function(&g_Distance.StruProgram);
    if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL )
    {
        g_Distance.StruProgram.Setting_Array=TM_Goal_DistanceSetUp;
    }
    else
    {
        if(SystemConfig_Get_Char(DRIVE_TYPE_UINT8) == EP_0B_JIS 
           || SystemConfig_Get_Char(DRIVE_TYPE_UINT8) == INDUCTOR_BRAKE_INCLINE)
            
            g_Distance.StruProgram.Setting_Array=EP_Goal_SetUp;                
        else            
            g_Distance.StruProgram.Setting_Array=EP_Distance_SetUp;
    }
    memset(&g_Distance.ManualData,0,sizeof(g_Distance.ManualData));
    g_Distance.ManualData.SegmentTime = 6000;  
    g_Distance.StruProgram.Programid = PS_CALORIES_GOAL;
    Program_Register_Function(&g_Distance.StruProgram);
    g_Distance.StruProgram.Programid = PS_STEPS_GOAL;
    Program_Register_Function(&g_Distance.StruProgram);
    
    //  g_Calories.StruProgram.Programid = PS_CALORIES_GOAL;
    //  g_Calories.StruProgram.SettingArray_Index= 0;
    //  
    //  g_Calories.StruProgram.Prg_Initial = Goal_distance_Data_Initial;
    //  g_Calories.StruProgram.BeginWorkout=Goal_BeginWorkout;
    //  g_Calories.StruProgram.Prg_Setup = Goal_SetUp;
    //  g_Calories.StruProgram.Prg_ProfileShow=Goal_ProfileShow;
    //  g_Calories.StruProgram.Prg_Workout = Goal_Run;
    //  g_Calories.StruProgram.Prg_SetupData=Goal_SetUpData;
    //  g_Calories.StruProgram.Prg_RunData=Goal_RunData;
    //  Program_Register_Function(&g_Calories.StruProgram);
    //  g_Calories.StruProgram.Setting_Array=TM_ProTable_CaloriesSetUp;
    //  memset(&g_Calories.ManualData,0,sizeof(g_Calories.ManualData));
    //  g_Calories.ManualData.SegmentTime = 6000;  
    
    //  g_Steps.StruProgram.Programid = PS_STEPS_GOAL;
    //  g_Steps.StruProgram.SettingArray_Index= 0;
    //  
    //  g_Steps.StruProgram.Prg_Initial = Goal_distance_Data_Initial;
    //  g_Steps.StruProgram.BeginWorkout=Goal_BeginWorkout;
    //  g_Steps.StruProgram.Prg_Setup = Goal_SetUp;
    //  g_Steps.StruProgram.Prg_ProfileShow=Goal_ProfileShow;
    //  g_Steps.StruProgram.Prg_Workout = Goal_Run;
    //  g_Steps.StruProgram.Prg_SetupData=Goal_SetUpData;
    //  g_Steps.StruProgram.Prg_RunData=Goal_RunData;
    //  Program_Register_Function(&g_Steps.StruProgram);
    //  g_Steps.StruProgram.Setting_Array=TM_ProTable_StepsSetUp;
    //  memset(&g_Steps.ManualData,0,sizeof(g_Steps.ManualData));
    //  g_Steps.ManualData.SegmentTime = 6000;  
    
}





#endif
