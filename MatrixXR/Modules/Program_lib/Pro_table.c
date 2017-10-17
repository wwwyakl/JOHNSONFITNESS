#include"Pro_table.h"
#ifdef __PROGRAM_TABLE__

#define WARM_UP	0
#define COOL_DOWN	1
#define REPEAT	2


__no_init static PROFILE Profile;

__no_init static const unsigned char *Setting_Array;
//__no_init static unsigned char Setting_Index;
__no_init static TableStruct Ptable;

__no_init static UINT16 w_CooldownTime, w_WarmupTime;
__no_init static UCHAR by_LoopStart, by_CoolStart, by_WarmCool;
__no_init static UINT16 TotalTime;

__no_init static UCHAR by_Segment;
__no_init static UINT16 w_Segment_Time;
__no_init static UINT16 w_Last_Point;//last time point


void ProgramTable_LookUpParameter(void)
{
  char loopState = 0, returnVal = 0;
  TotalTime=0;
  w_CooldownTime = 0;
  w_WarmupTime = 0;
  by_LoopStart = 0;
  by_CoolStart = 0;
  
  if(Ptable.Table_Type == TIME_TABLE_FIX)
  {
    for(char i = 0; i<Ptable.Max_Segment; i++)
    {
      returnVal=Ptable.StatusTime[Ptable.Max_Segment*(2*(Data_Get_Level(TARGET_LEVEL)-1))+i];
      if(loopState==0 && returnVal==2)
      {
        w_WarmupTime = TotalTime;
        by_LoopStart = i;
        loopState = 1;
      }
      if(loopState==1 && returnVal==3)
      {
        w_CooldownTime += Ptable.StatusTime[Ptable.Max_Segment*(2*Data_Get_Level(TARGET_LEVEL)-1)+i];
        if(by_CoolStart == 0) 	by_CoolStart = i;
      }
      TotalTime  += Ptable.StatusTime[Ptable.Max_Segment*(2*Data_Get_Level(TARGET_LEVEL)-1)+i];
    }
    Data_Set_WarmUP_Time(w_WarmupTime);
    Data_Set_CoolDown_Time(w_CooldownTime);
    Data_Set_Time(TotalTime);
    if(Ptable.Speed_table!=NULL)
    {
      Data_Set_Speed( Ptable.Speed_table[Ptable.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment],STANDARD,0);
    }
    if(Ptable.Incline_table!=NULL)
    {
      Data_Set_Incline( Ptable.Incline_table[Ptable.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment],0);
    }
    if(Ptable.Resistance_table!=NULL)
    {
      Data_Set_Resistance( Ptable.Resistance_table[Ptable.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment],0);
    }
  }
  else
  {
    Data_Set_WarmUP_Time(0);
    Data_Set_CoolDown_Time(0);
    if(Ptable.Speed_table!=NULL)
    {
      Data_Set_Speed( Ptable.Speed_table[Ptable.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment],STANDARD,0);
    }
    if(Ptable.Incline_table!=NULL)
    {
      Data_Set_Incline( Ptable.Incline_table[Ptable.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment],0);
    }
    if(Ptable.Resistance_table!=NULL)
    {
      Data_Set_Resistance( Ptable.Resistance_table[Ptable.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment],0);
    }
  }
}

void ProgramTable_Data_Initial(void)
{
  UINT16 i;
  Data_Set_Time(1800);
  by_WarmCool = WARM_UP;
  //  memset(&DotmatrixData,0x00,sizeof(PROFILE_INEER));
  //  Setting_Index=1;
  by_Segment=0;
  Data_Set_Level(1);
  Setting_Array=ProTable_SetUp;
  Ptable = *(ProgramParameter->TablePara[0]);  
  for(i=0;i<ProgramParameter->PtableNum;i++)
  {
    if(ProgramParameter->ProgramidArray[i]==Program_Get_ProgramID())
    {
      Ptable = *(ProgramParameter->TablePara[i]);
      break;
    }
  }
//  if( ProgramParameter->ProgramidArray[i]==PS_SPRINT8)
//    Setting_Array=ProSprint8_SetUp;	
  Data_Set_Level_MaxVal(Ptable.Max_Level);
  ProgramTable_LookUpParameter();
  if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
  {
    if(Ptable.DefalultProfle_Type==SPEED_TYPE)
    {
      DefaultDotmatrix=SPEED_TYPE;
    }
    else
    {
      DefaultDotmatrix=INCLINE_TYPE;
    }
  }
  else
  {
    if(Ptable.DefalultProfle_Type==RESISTANCE_TYPE)
    {
      DefaultDotmatrix=RESISTANCE_TYPE;
    }
    else
    {
      DefaultDotmatrix=INCLINE_TYPE;
    }
  }
}

PROGRAM_PENDING_EVTS ProgramTable_SetUp(ProgramKeyEvts *keyevt)
{
  
  UINT16 w_Diff;
  UINT16 w_Target;
  UINT16 w_Time;
  PROGRAM_PENDING_EVTS pendingEvt;
  pendingEvt.evt=PROGRAM_NONE_EVENT;
  pendingEvt.WorkOutEvts.AllEvts=0;
  
  // if(g_StateMachine!=PS_SETUP) return pendingEvt; 
  if(keyevt->EventBits.Key_Enter)
  {
    keyevt->EventBits.Key_Enter=false;
    if(Setting_Array[Setting_Index] != SET_PRESS_START )
    {
      keyevt->EventBits.Key_StateChange=true;
      Setting_Index++;
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
      ProgramTable_Data_Initial();
    }      
    break;
  case SET_TIME:
    if(keyevt->EventBits.Key_StateChange)
    {
      keyevt->EventBits.Key_StateChange=false;
      pendingEvt.evt=TIME_CHANGE_EVENT;
    }
    if(keyevt->EventBits.Key_Up)
    {
      if(Data_Get_Time(TARGET_TIME)==99*60)
      {
        Data_Set_Time(99*60);
      }
      else
      {
        Data_Set_Time(Data_Get_Time(TARGET_TIME)+60);
      }
      pendingEvt.evt=TIME_CHANGE_EVENT;
    }
    else if(keyevt->EventBits.Key_Dn)
    {
      if(Data_Get_Time(TARGET_TIME)==5*60)
      {
        Data_Set_Time(5*60);
      }
      else
      {
        Data_Set_Time(Data_Get_Time(TARGET_TIME)-60);
      }    
      pendingEvt.evt=TIME_CHANGE_EVENT;
    }
    break;
  case SET_LEVEL:
    if(keyevt->EventBits.Key_StateChange)
    {
      keyevt->EventBits.Key_StateChange=false;
      pendingEvt.evt=LEVEL_CHANGE_EVENT;
    }
    if(keyevt->EventBits.Key_Up)
    {
      keyevt->EventBits.Key_Up=false;
      Data_Set_Level(Program_Get_WorkoutLevel(0)+1);
      pendingEvt.evt=LEVEL_CHANGE_EVENT;
    }
    else if(keyevt->EventBits.Key_Dn)
    {
      Data_Set_Level(Program_Get_WorkoutLevel(0)-1);
      pendingEvt.evt=LEVEL_CHANGE_EVENT;
    }
    break;
  case SET_PRESS_START:
    if(keyevt->EventBits.Key_StateChange)
    {
      keyevt->EventBits.Key_StateChange=false;
      pendingEvt.evt=SETTING_COMPLETE_EVENT;
    }   
    
  }
  ProgramTable_LookUpParameter();
  w_Target=Data_Get_Time(TARGET_TIME);
  w_Time=Data_Get_Time(ELAPSED_TIME);
  if(w_Time)
  {
    w_Diff = w_Time - w_Last_Point;
    if(w_Diff >= w_Segment_Time)
    {
      w_Last_Point = w_Time;
      by_Segment++;
      if(Ptable.Table_Type == TIME_TABLE_FIX)
      {
        
        if(w_Last_Point >= w_WarmupTime)
        {
          if(w_Target - w_Last_Point > w_CooldownTime)
          {
            if(by_Segment >= by_CoolStart)
            {
              by_Segment = by_LoopStart;
            }
          }
        }	
      }
      if(Ptable.Table_Type == TIME_TABLE_FIX)
        w_Segment_Time =  Ptable.StatusTime[Ptable.Max_Segment*(2*Data_Get_Level(TARGET_LEVEL)-1)+by_Segment];
      else if(Ptable.Table_Type == TIME_AVARAGE_TABLE_FIX)
        w_Segment_Time= w_Target/Ptable.Max_Segment;
      Data_Set_StageTime(w_Segment_Time);
      if(Ptable.Speed_table!=NULL)
      {
	Data_Set_Speed( Ptable.Speed_table[Ptable.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment],STANDARD,0);
      }
      if(Ptable.Incline_table!=NULL)
      {
	Data_Set_Incline( Ptable.Incline_table[Ptable.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment],0);
      }
      if(Ptable.Resistance_table!=NULL)
      {
	Data_Set_Resistance( Ptable.Resistance_table[Ptable.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment],0);
      }
    }
  }
  else
  {
    by_Segment = 0;
    w_Last_Point = 0;
    if(Ptable.Table_Type == TIME_TABLE_FIX)
      w_Segment_Time = Ptable.StatusTime[Ptable.Max_Segment*(2*Data_Get_Level(TARGET_LEVEL)-1)+by_Segment];
    else if(Ptable.Table_Type == TIME_AVARAGE_TABLE_FIX)
      w_Segment_Time= w_Target/Ptable.Max_Segment;
    Data_Set_StageTime(w_Segment_Time);
  }
  
  if(Ptable.Table_Type == TIME_TABLE_FIX)
  {
    if((by_WarmCool != COOL_DOWN) & ((w_Target - w_Last_Point - w_Segment_Time) < w_CooldownTime))
    {
      w_Segment_Time = w_Target - w_Last_Point;
      w_Segment_Time -= w_CooldownTime;
    }
    if(w_Last_Point >= w_WarmupTime && by_WarmCool != COOL_DOWN)
    {
      by_WarmCool = REPEAT;
      //      g_StateMachine=PS_RUN;
      pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
    }
  }
  
  
  
  
  return pendingEvt; 
}
PROFILE *ProgramTable_GetShow(PROFILE_TAG tag_index,UCHAR* Dotmatrix_pos)
{
  
  UINT16 by_SEG;
  by_SEG = Ptable.Max_Segment- 1 - by_Segment;
  if(by_SEG < ProgramParameter->DotMatrixLength)
  {
    *Dotmatrix_pos = ProgramParameter->DotMatrixLength - 1 - by_SEG;
  }
  else
  {
    *Dotmatrix_pos = 0;
  }
  for(UINT8 i = *Dotmatrix_pos; i < ProgramParameter->DotMatrixLength; i++)
  {
    if(Ptable.Speed_table!=NULL)
    {
      DotmatrixData.SpeedProfile[i]=Data_Get_DotmatrixSpeedData( Ptable.Speed_table[Ptable.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+i+by_Segment-*Dotmatrix_pos]);
    }
    else
    {
      DotmatrixData.SpeedProfile[i]=Program_Get_Speed(DATA_AUTO,0);
    }
    
    if(Ptable.Incline_table!=NULL)
    {
      DotmatrixData.InclineProfile[i]=Data_Get_DotmatrixInclineData( Ptable.Incline_table[Ptable.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+i+by_Segment-*Dotmatrix_pos]);
    }
    else
    {
      DotmatrixData.InclineProfile[i]=Data_Get_Incline(TARGET_INCLINEX10);
    }
    if(Ptable.Resistance_table!=NULL)
    {
      DotmatrixData.ResistanceProfile[i]=Data_Get_DotmatrixResistanceData( Ptable.Resistance_table[Ptable.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+i+by_Segment-*Dotmatrix_pos]);
    }
    else
    {
      DotmatrixData.ResistanceProfile[i]=Data_Get_Resistance(TARGET_RESISTANCE);
    }
  }
  if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
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
      if(Ptable.DefalultProfle_Type==SPEED_TYPE)
        Profile.ProfileData=DotmatrixData.SpeedProfile;
      else
        Profile.ProfileData=DotmatrixData.InclineProfile;
      
      break;
    default:
      if(Ptable.DefalultProfle_Type==SPEED_TYPE)
        Profile.ProfileData=DotmatrixData.SpeedProfile;
      else
        Profile.ProfileData=DotmatrixData.InclineProfile;
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
      if(Ptable.DefalultProfle_Type==RESISTANCE_TYPE)
        Profile.ProfileData=DotmatrixData.ResistanceProfile;
      else
        Profile.ProfileData=DotmatrixData.InclineProfile;
      break;
    default:
      if(Ptable.DefalultProfle_Type==RESISTANCE_TYPE)
        Profile.ProfileData=DotmatrixData.ResistanceProfile;
      else
        Profile.ProfileData=DotmatrixData.InclineProfile;
      break;
    }  
  }
  Profile.Length=ProgramParameter->DotMatrixLength;
  return &Profile;
}



PROGRAM_PENDING_EVTS ProgramTable_Run(ProgramKeyEvts *keyevt)
{  
  UINT16 w_Auto;
  UINT16 w_Diff;
  UINT16 w_Target;
  UINT16 w_Time;
  static UINT16 WorkOut_Old_Time=1;
  
  UINT16 Stage_Remaining_Time;
  PROGRAM_PENDING_EVTS pendingEvt;
  pendingEvt.evt=PROGRAM_NONE_EVENT;
  pendingEvt.WorkOutEvts.AllEvts=0;
  if(g_StateMachine<PS_WARMUP||g_StateMachine>PS_COOLDOWN)return pendingEvt;
  w_Target=Data_Get_Time(TARGET_TIME);
  w_Time=Data_Get_Time(ELAPSED_TIME);
  w_Auto=w_Target-w_Time;
  if(WorkOut_Old_Time!=w_Time)
  {
    WorkOut_Old_Time=w_Time;
//			pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
//	pendingEvt.WorkOutEvts.EvtBit.Workout_Refresh_Data=true;	

    if(w_Auto==0)
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
      if(Ptable.Table_Type == TIME_TABLE_FIX)
      {
        if(w_Time==0)
        {
          by_WarmCool = WARM_UP;
          //                  g_StateMachine=PS_WARMUP;
          pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
          pendingEvt.WorkOutEvts.EvtBit.Workout_Warmup_Evt=true;
        }
      }
      if(w_Time)
      {
        if(Data_Get_Time(ELAPSED_TIME)<Data_Get_Time(WARMUP_TIME))
        {
          g_StateMachine=PS_WARMUP;
        }
        else if(Data_Get_Time(REMAINING_TIME)<=Data_Get_Time(COOLDOWN_TIME))
        {
          g_StateMachine=PS_COOLDOWN;
        }
        else
        {
          g_StateMachine=PS_RUN;
        }
        w_Diff = w_Time - w_Last_Point;
        if(w_Diff >= w_Segment_Time)
        {
          w_Last_Point = w_Time;
          by_Segment++;
          Beep_Set_Beeps(1,3,1);
          if(Ptable.Table_Type == TIME_TABLE_FIX)
          {
            
            if(w_Last_Point >= w_WarmupTime)
            {
              if(w_Target - w_Last_Point > w_CooldownTime)
              {
                if(by_Segment >= by_CoolStart)
                {
                  by_Segment = by_LoopStart;
                }
              }
              else
              {
                if(by_WarmCool != COOL_DOWN)
                {
                  by_WarmCool = COOL_DOWN;
                  //                                  g_StateMachine=PS_COOLDOWN;
                  pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
                  pendingEvt.WorkOutEvts.EvtBit.Workout_Cooldown_Evt=true;
                  by_Segment = by_CoolStart;
                }
              }
            }	
          }
          if(Ptable.Table_Type == TIME_TABLE_FIX)
            w_Segment_Time =  Ptable.StatusTime[Ptable.Max_Segment*(2*Data_Get_Level(TARGET_LEVEL)-1)+by_Segment];
          else if(Ptable.Table_Type == TIME_AVARAGE_TABLE_FIX)
          {
            if(by_Segment!=Ptable.Max_Segment-1)
              w_Segment_Time= w_Target/Ptable.Max_Segment;
            else
              w_Segment_Time=w_Target%(w_Target/Ptable.Max_Segment)+w_Target/Ptable.Max_Segment;
          }
          Data_Set_StageTime(w_Segment_Time);
          if(Ptable.Speed_table!=NULL)
          {
            Data_Set_Speed( Ptable.Speed_table[Ptable.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment],STANDARD,0);
          }
          if(Ptable.Incline_table!=NULL)
          {
            Data_Set_Incline( Ptable.Incline_table[Ptable.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment],0);
          }
          if(Ptable.Resistance_table!=NULL)
          {
            Data_Set_Resistance( Ptable.Resistance_table[Ptable.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment],0);
          }
        }
      }
      else
      {
        by_Segment = 0;
        w_Last_Point = 0;
        if(Ptable.Table_Type == TIME_TABLE_FIX)
          w_Segment_Time = Ptable.StatusTime[Ptable.Max_Segment*(2*Data_Get_Level(TARGET_LEVEL)-1)+by_Segment];
        else if(Ptable.Table_Type == TIME_AVARAGE_TABLE_FIX)
          w_Segment_Time= w_Target/Ptable.Max_Segment;
        Data_Set_StageTime(w_Segment_Time);
      }
      
      if(Ptable.Table_Type == TIME_TABLE_FIX)
      {
        if((by_WarmCool != COOL_DOWN) & ((w_Target - w_Last_Point - w_Segment_Time) < w_CooldownTime))
        {
          w_Segment_Time = w_Target - w_Last_Point;
          w_Segment_Time -= w_CooldownTime;
        }
        if(w_Last_Point >= w_WarmupTime && by_WarmCool != COOL_DOWN)
        {
          by_WarmCool = REPEAT;
          //                  g_StateMachine=PS_RUN;
          pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
        }
      }
    }
//    if(Program_Get_ProgramID() == PS_SPRINT8)
//    {
//      Stage_Remaining_Time=Data_Get_Time(STAGE_TIME)-Data_Get_Time(TIME_IN_STAGE);
//      if(by_Segment%2==0)//activerecovery
//      {
//        if(w_Auto>=Data_Get_Time(COOLDOWN_TIME))
//        {
//          if(Stage_Remaining_Time==2)
//          {
//            pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
//            pendingEvt.WorkOutEvts.EvtBit.Workout_Sprint8_Ready=true;  
//          }
//        }
//      }
//      else
//      {
//        
//        if(Data_Get_Time(TIME_IN_STAGE)==0)
//        {
//          pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
//          pendingEvt.WorkOutEvts.EvtBit.Workout_Sprint8_Go=true;
//        }
//        else if(Data_Get_Time(TIME_IN_STAGE)==4)
//        {
//          Sprint8Stage++;
//          pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
//          pendingEvt.WorkOutEvts.EvtBit.Workout_Sprint8_SprintStage=true;
//        }
//        else if(Stage_Remaining_Time==1)
//        {
//          pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
//          pendingEvt.WorkOutEvts.EvtBit.Workout_Sprint8_ActiveRecoveryStage=true;        
//        }
//      }
//    }
  }
  if(keyevt->EventBits.Key_SpeedUp)
  {
    keyevt->EventBits.Key_SpeedUp=false;
    Data_Set_Speed(Program_Get_Speed(DATA_AUTO,0)+1,Get_User_Unit()==METRIC?METRIC:STANDARD,1);
  }
  else if(keyevt->EventBits.Key_SpeedDn)
  {
    keyevt->EventBits.Key_SpeedDn=false;
    Data_Set_Speed(Program_Get_Speed(DATA_AUTO,0)-1,Get_User_Unit()==METRIC?METRIC:STANDARD,1);
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

void initTable(void)
{
  struProgram table;
  table.Initial = ProgramTable_Data_Initial;
  table.Setup = ProgramTable_SetUp;
  table.GetProfile=ProgramTable_GetShow;
  table.Workout = ProgramTable_Run;
  table.Programid = PS_INTERVALS;
  Program_Add_Array(table);
  table.Programid = PS_WEIGHTLOSS;
  Program_Add_Array(table);
  table.Programid = PS_ROLLING_HILLS;
  Program_Add_Array(table);
  table.Programid = PS_FATBURN;
  Program_Add_Array(table);
}





#endif

