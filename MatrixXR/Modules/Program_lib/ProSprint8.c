#include"ProSprint8.h"
#ifdef __PROGRAM_SPRINT8__

#define WARM_UP	0
#define COOL_DOWN	1
#define REPEAT	2


__no_init static PROFILE Profile;

__no_init static const unsigned char *Setting_Array;
__no_init static TableStruct Sprint8_table;

__no_init static UINT16 w_CooldownTime, w_WarmupTime;
__no_init static UCHAR by_LoopStart, by_CoolStart, by_WarmCool;
__no_init static UINT16 TotalTime;

__no_init static UCHAR by_Segment;
__no_init static UINT16 w_Segment_Time;
__no_init static UINT16 w_Last_Point;
//__no_init static  struct
//{
//  INT16 SpeedOffset;
//  INT16 IncOffset;
//  INT16 ResOffset;
//}Sprint,Recovery;
UINT16 Sprin8_Get_SpeedDotMatrixData(UCHAR by_Dat)
{
  UINT16 Out=0;
  UINT16 SpeedMax,SpeedMin;
  UINT16 Sprint8Base=Sprint8_table.Speed_table[Sprint8_table.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Dat];
  INT16 Sprint8Offset=0;  
  if(by_Dat<=by_Segment)
  {
    Sprint8Offset=Data_Get_SpeedOffset();
  }
  if(by_Segment>0 && by_Dat<Sprint8_table.Max_Segment-1)
  {
    if(by_Segment%2)
    {
      if(by_Dat==by_Segment+1)
      {
        Sprint8Offset=Data_Get_SpeedOffset();
      }
    }
  }
  if(Get_User_Unit()==STANDARD)
  {
    SpeedMax=SystemConfig_Get_Int(MAX_MPHX10_UINT16);
    SpeedMin=SystemConfig_Get_Int(MIN_MPHX10_UINT16);
  }
  else
  {
    SpeedMax=SystemConfig_Get_Int(MAX_KPHX10_UINT16);
    SpeedMin=SystemConfig_Get_Int(MIN_KPHX10_UINT16);
  }
  if(Sprint8Base+Sprint8Offset>SystemConfig_Get_Int(MAX_MPHX10_UINT16))
  {
    Out=SystemConfig_Get_Int(MAX_MPHX10_UINT16);
  }
  else if(Sprint8Base+Sprint8Offset<SystemConfig_Get_Int(MIN_MPHX10_UINT16))
  {
    Out=SystemConfig_Get_Int(MIN_MPHX10_UINT16);
  }
  else 
  {
    Out=Sprint8Base+Sprint8Offset;
  }
  return Out;
}
UINT16 Sprin8_Get_InclineDotMatrixData(UCHAR by_Dat)
{
  UINT16 Out=0;
  UINT16 Sprint8Base=Sprint8_table.Incline_table[Sprint8_table.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Dat];
  INT16 Sprint8Offset=0;  
  if(by_Dat<=by_Segment)
  {
    Sprint8Offset=Data_Get_InclineOffset();
  }
  if(by_Segment>0 && by_Dat<Sprint8_table.Max_Segment-1)
  {
    if(by_Segment%2)
    {
      if(by_Dat==by_Segment+1)
      {
        Sprint8Offset=Data_Get_InclineOffset();
      }
    }
  }

  if(Sprint8Base+Sprint8Offset>Data_Get_Incline(MAX_INCLINEX10))
  {
    Out=Data_Get_Incline(MAX_INCLINEX10);
  }
  else if(Sprint8Base+Sprint8Offset<Data_Get_Incline(MIN_INCLINEX10))
  {
    Out=Data_Get_Incline(MIN_INCLINEX10);
  }
  else 
  {
    Out=Sprint8Base+Sprint8Offset;
  }
  return Out;
}
UINT16 Sprin8_Get_ResistanceDotMatrixData(UCHAR by_Dat)
{
  UINT16 Out=0;
  UINT16 Sprint8Base=Sprint8_table.Resistance_table[Sprint8_table.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Dat];
  INT16 Sprint8Offset=0;  
  if(by_Dat<=by_Segment)
  {
    Sprint8Offset=Data_Get_ResistanceOffset();
  }
  if(by_Segment>0 && by_Dat<Sprint8_table.Max_Segment-1)
  {
    if(by_Segment%2)
    {
      if(by_Dat==by_Segment+1)
      {
        Sprint8Offset=Data_Get_ResistanceOffset();
      }
    }
  }

  if(Sprint8Base+Sprint8Offset>Data_Get_Resistance(MAX_RESISTANCE))
  {
    Out=Data_Get_Resistance(MAX_RESISTANCE);
  }
  else if(Sprint8Base+Sprint8Offset<Data_Get_Resistance(MIN_RESISTANCE))
  {
    Out=Data_Get_Resistance(MIN_RESISTANCE);
  }
  else 
  {
    Out=Sprint8Base+Sprint8Offset;
  }
  return Out;
}
void ProgramSprint8_LookUpParameter(void)
{
  char loopState = 0, returnVal = 0;
  TotalTime=0;
  w_CooldownTime = 0;
  w_WarmupTime = 0;
  by_LoopStart = 0;
  by_CoolStart = 0;
  for(char i = 0; i<Sprint8_table.Max_Segment; i++)
  {
    returnVal=Sprint8_table.StatusTime[Sprint8_table.Max_Segment*(2*(Data_Get_Level(TARGET_LEVEL)-1))+i];
    if(loopState==0 && returnVal==2)
    {
      w_WarmupTime = TotalTime;
      by_LoopStart = i;
      loopState = 1;
    }
    if(loopState==1 && returnVal==3)
    {
      w_CooldownTime += Sprint8_table.StatusTime[Sprint8_table.Max_Segment*(2*Data_Get_Level(TARGET_LEVEL)-1)+i];
      if(by_CoolStart == 0) 	by_CoolStart = i;
    }
    TotalTime  += Sprint8_table.StatusTime[Sprint8_table.Max_Segment*(2*Data_Get_Level(TARGET_LEVEL)-1)+i];
  }
  Data_Set_WarmUP_Time(w_WarmupTime);
  Data_Set_CoolDown_Time(w_CooldownTime);
  Data_Set_Time(TotalTime);
  if(Sprint8_table.Speed_table!=NULL)
  {
    Data_Set_Speed( Sprint8_table.Speed_table[Sprint8_table.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment],STANDARD,0);
  }
  if(Sprint8_table.Incline_table!=NULL)
  {
    Data_Set_Incline( Sprint8_table.Incline_table[Sprint8_table.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment],0);
  }
  if(Sprint8_table.Resistance_table!=NULL)
  {
    Data_Set_Resistance( Sprint8_table.Resistance_table[Sprint8_table.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment],0);
  }
}

void ProgramSprint8_Data_Initial(void)
{
  char i;
  Data_Set_Time(1800);
  by_WarmCool = WARM_UP;
//  memset(&Sprint,0,sizeof(Sprint));
//  memset(&Recovery,0,sizeof(Recovery));
  by_Segment=0;
  Sprint8Stage=0;
  Data_Set_Level(1);
  Setting_Array=ProSprint8_SetUp;
  for(i=0;i<ProgramParameter->PtableNum;i++)
  {
    if(ProgramParameter->ProgramidArray[i]==PS_SPRINT8)
    {
      Sprint8_table = *(ProgramParameter->TablePara[i]);
      break;
    }
  }
  Data_Set_Level_MaxVal(Sprint8_table.Max_Level);
  ProgramSprint8_LookUpParameter();
  if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
  {
    if(Sprint8_table.DefalultProfle_Type==SPEED_TYPE)
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
    if(Sprint8_table.DefalultProfle_Type==RESISTANCE_TYPE)
    {
      DefaultDotmatrix=RESISTANCE_TYPE;
    }
    else
    {
      DefaultDotmatrix=INCLINE_TYPE;
    }
  }
}

PROGRAM_PENDING_EVTS ProgramSprint8_SetUp(ProgramKeyEvts *keyevt)
{
  
  UINT16 w_Diff;
  UINT16 w_Target;
  UINT16 w_Time;
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
      ProgramSprint8_Data_Initial();
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
  ProgramSprint8_LookUpParameter();
  w_Target=Data_Get_Time(TARGET_TIME);
  w_Time=Data_Get_Time(ELAPSED_TIME);
  if(w_Time)
  {
    w_Diff = w_Time - w_Last_Point;
    if(w_Diff >= w_Segment_Time)
    {
      w_Last_Point = w_Time;
      by_Segment++;
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
      w_Segment_Time =  Sprint8_table.StatusTime[Sprint8_table.Max_Segment*(2*Data_Get_Level(TARGET_LEVEL)-1)+by_Segment];
      Data_Set_StageTime(w_Segment_Time);
      if(Sprint8_table.Speed_table!=NULL)
      {
	Data_Set_Speed( Sprint8_table.Speed_table[Sprint8_table.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment],STANDARD,0);
      }
      if(Sprint8_table.Incline_table!=NULL)
      {
	Data_Set_Incline( Sprint8_table.Incline_table[Sprint8_table.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment],0);
      }
      if(Sprint8_table.Resistance_table!=NULL)
      {
	Data_Set_Resistance( Sprint8_table.Resistance_table[Sprint8_table.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment],0);
      }
    }
  }
  else
  {
    by_Segment = 0;
    w_Last_Point = 0;
    w_Segment_Time = Sprint8_table.StatusTime[Sprint8_table.Max_Segment*(2*Data_Get_Level(TARGET_LEVEL)-1)+by_Segment];
    Data_Set_StageTime(w_Segment_Time);
  }
  
  {
    if((by_WarmCool != COOL_DOWN) & ((w_Target - w_Last_Point - w_Segment_Time) < w_CooldownTime))
    {
      w_Segment_Time = w_Target - w_Last_Point;
      w_Segment_Time -= w_CooldownTime;
    }
    if(w_Last_Point >= w_WarmupTime && by_WarmCool != COOL_DOWN)
    {
      by_WarmCool = REPEAT;
      pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
    }
  } 
  return pendingEvt; 
}
PROFILE *ProgramSprint8_GetShow(PROFILE_TAG tag_index,UCHAR* Dotmatrix_pos)
{
  
  UINT16 by_SEG;
  by_SEG = Sprint8_table.Max_Segment- 1 - by_Segment;
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
    if(Sprint8_table.Speed_table!=NULL)
    {
      DotmatrixData.SpeedProfile[i]=Sprin8_Get_SpeedDotMatrixData( i+by_Segment-*Dotmatrix_pos);
    }
    else
    {
      DotmatrixData.SpeedProfile[i]=Program_Get_Speed(DATA_AUTO,0);
    }
    
    if(Sprint8_table.Incline_table!=NULL)
    {
      DotmatrixData.InclineProfile[i]=Sprin8_Get_InclineDotMatrixData( i+by_Segment-*Dotmatrix_pos);
    }
    else
    {
      DotmatrixData.InclineProfile[i]=Data_Get_Incline(TARGET_INCLINEX10);
    }
    if(Sprint8_table.Resistance_table!=NULL)
    {
      DotmatrixData.ResistanceProfile[i]=Sprin8_Get_ResistanceDotMatrixData( i+by_Segment-*Dotmatrix_pos);
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
      if(Sprint8_table.DefalultProfle_Type==SPEED_TYPE)
        Profile.ProfileData=DotmatrixData.SpeedProfile;
      else
        Profile.ProfileData=DotmatrixData.InclineProfile;
      
      break;
    default:
      if(Sprint8_table.DefalultProfle_Type==SPEED_TYPE)
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
      if(Sprint8_table.DefalultProfle_Type==RESISTANCE_TYPE)
        Profile.ProfileData=DotmatrixData.ResistanceProfile;
      else
        Profile.ProfileData=DotmatrixData.InclineProfile;
      break;
    default:
      if(Sprint8_table.DefalultProfle_Type==RESISTANCE_TYPE)
        Profile.ProfileData=DotmatrixData.ResistanceProfile;
      else
        Profile.ProfileData=DotmatrixData.InclineProfile;
      break;
    }  
  }
  Profile.Length=ProgramParameter->DotMatrixLength;
  return &Profile;
}



PROGRAM_PENDING_EVTS ProgramSprint8_Run(ProgramKeyEvts *keyevt)
{  
    UINT16 w_Auto;
    UINT16 w_Diff;
    UINT16 w_Target;
    UINT16 w_Time;
    UINT16 w_SpeedAheadTime;
    UINT16 w_IncliAheadTime;
    UINT16 w_ResisAheadTime;
    UINT8 w_AheadTime;
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
            if(w_Time==0)
            {
                by_WarmCool = WARM_UP;
                pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
                pendingEvt.WorkOutEvts.EvtBit.Workout_Warmup_Evt=true;
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
                
                
//                if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
//                {
//                    if(by_Segment%2==0)
//                    {
//                        w_SpeedAheadTime = Sprint8_table.Incline_table[Sprint8_table.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment+1]/5;
//                        w_IncliAheadTime = Data_Get_Incline(TARGET_INCLINEX10)*10/71;
//                    }
//                    else
//                    {
//                        w_SpeedAheadTime = 0;
//                        w_IncliAheadTime = 0;
//                    }
//                    if(w_SpeedAheadTime>=w_IncliAheadTime)
//                    {
//                        w_AheadTime = w_SpeedAheadTime;
//                        if((w_Diff == w_Segment_Time-w_SpeedAheadTime)&&(w_Diff != w_Segment_Time))
//                        {
//                            by_Segment++;
//                            if(Sprint8_table.Speed_table!=NULL)
//                            {
//                                {
//                                    if(by_Segment%2)
//                                    {
//                                        Data_Set_SpeedOffset(0);
//                                    }
//                                }
//                                Data_Set_Speed( Sprint8_table.Speed_table[Sprint8_table.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment],STANDARD,0);
//                            }
//                        }
//                        if(w_Diff == w_Segment_Time-w_IncliAheadTime)
//                        {
//                            if(Sprint8_table.Incline_table!=NULL)
//                            {
//                                {
//                                    if(by_Segment%2)
//                                    {
//                                        Data_Set_InclineOffset(0);
//                                    }
//                                }
//                                Data_Set_Incline( Sprint8_table.Incline_table[Sprint8_table.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment],0);
//                            }
//                        }
//                    }
//                    else if(w_SpeedAheadTime<w_IncliAheadTime)
//                    {
//                        w_AheadTime = w_IncliAheadTime;
//                        if(w_Diff == w_Segment_Time-w_SpeedAheadTime)
//                        {
//                            
//                            if(Sprint8_table.Speed_table!=NULL)
//                            {
//                                {
//                                    if(by_Segment%2)
//                                    {
//                                        Data_Set_SpeedOffset(0);
//                                    }
//                                }
//                                Data_Set_Speed( Sprint8_table.Speed_table[Sprint8_table.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment],STANDARD,0);
//                            }
//                        }
//                        if(w_Diff == w_Segment_Time-w_IncliAheadTime)
//                        {
//                            by_Segment++;
//                            if(Sprint8_table.Incline_table!=NULL)
//                            {
//                                {
//                                    if(by_Segment%2)
//                                    {
//                                        Data_Set_InclineOffset(0);
//                                    }
//                                }
//                                Data_Set_Incline( Sprint8_table.Incline_table[Sprint8_table.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment],0);
//                            }
//                        }
//                    }
//                }
//                else if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==ASCENT)
//                {
//                    w_IncliAheadTime = Data_Get_Incline(TARGET_INCLINEX10)*10/37;
//                    w_ResisAheadTime = Data_Get_Resistance(TARGET_RESISTANCE)/5;
//                }
//                else
//                {
//                    w_ResisAheadTime = Data_Get_Resistance(TARGET_RESISTANCE)/5;
//                }
                if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
                {
                    w_SpeedAheadTime = Sprint8_table.Speed_table[Sprint8_table.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment+1]/5;
                    w_IncliAheadTime = Sprint8_table.Incline_table[Sprint8_table.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment+1]*10/71;
                    if(w_SpeedAheadTime > w_IncliAheadTime)
                    {
                        w_AheadTime = w_SpeedAheadTime;
                    }
                    else if(w_SpeedAheadTime < w_IncliAheadTime)
                    {
                        w_AheadTime = w_IncliAheadTime;
                    }
                }
                else if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==ASCENT)
                {
                    w_IncliAheadTime = Sprint8_table.Incline_table[Sprint8_table.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment+1]*10/37;
                    w_ResisAheadTime = Sprint8_table.Resistance_table[Sprint8_table.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment+1]/5;
                    if(w_IncliAheadTime > w_ResisAheadTime)
                    {
                        w_AheadTime = w_IncliAheadTime;
                    }
                    else if(w_IncliAheadTime < w_ResisAheadTime)
                    {
                        w_AheadTime = w_ResisAheadTime;
                    }
                }
                else
                {
                    w_ResisAheadTime = Data_Get_Resistance(TARGET_RESISTANCE)/5;
                    w_AheadTime = w_ResisAheadTime;
                }
                if(by_Segment%2==0)
                {
                    if(w_Diff == w_Segment_Time-w_SpeedAheadTime)
                    {
                        if(Sprint8_table.Speed_table!=NULL)
                        {
                            if(by_Segment%2)
                            {
                                Data_Set_SpeedOffset(0);
                            }
                            Data_Set_Speed( Sprint8_table.Speed_table[Sprint8_table.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment+1],STANDARD,0);
                        }
                    }
                    if(w_Diff == w_Segment_Time-w_IncliAheadTime)
                    {
                        if(Sprint8_table.Incline_table!=NULL)
                        {
                            if(by_Segment%2)
                            {
                                Data_Set_InclineOffset(0);
                            }
                            Data_Set_Incline( Sprint8_table.Incline_table[Sprint8_table.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment+1],0);
                        }
                    }
                    if(w_Diff == w_Segment_Time-w_ResisAheadTime)
                    {
                        if(Sprint8_table.Resistance_table!=NULL)
                        {
                            {
                                if(by_Segment%2)
                                {
                                    Data_Set_ResistanceOffset(0);
                                }
                            }
                            Data_Set_Resistance( Sprint8_table.Resistance_table[Sprint8_table.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment+1],0);
                        }
                    }
                }
                else
                {
                    if(w_Diff >= w_Segment_Time)
                    {
                        if(Sprint8_table.Speed_table!=NULL)
                        {
                            if(by_Segment%2)
                            {
                                Data_Set_SpeedOffset(0);
                            }
                            Data_Set_Speed( Sprint8_table.Speed_table[Sprint8_table.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment+1],STANDARD,0);
                        }
                        if(Sprint8_table.Incline_table!=NULL)
                        {
                            if(by_Segment%2)
                            {
                                Data_Set_InclineOffset(0);
                            }
                            Data_Set_Incline( Sprint8_table.Incline_table[Sprint8_table.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment+1],0);
                        }
                        if(Sprint8_table.Resistance_table!=NULL)
                        {
                            if(by_Segment%2)
                            {
                                Data_Set_ResistanceOffset(0);
                            }
                            Data_Set_Resistance( Sprint8_table.Resistance_table[Sprint8_table.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment+1],0);
                        }
                    }
                }
                if(w_Diff >= w_Segment_Time)
                {
                    w_Last_Point = w_Time;
                    by_Segment++;
                    Beep_Set_Beeps(1,3,1);                      
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
                                pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
                                pendingEvt.WorkOutEvts.EvtBit.Workout_Cooldown_Evt=true;
                                by_Segment = by_CoolStart;
                            }
                        }
                    }	
                    w_Segment_Time =  Sprint8_table.StatusTime[Sprint8_table.Max_Segment*(2*Data_Get_Level(TARGET_LEVEL)-1)+by_Segment];
                    Data_Set_StageTime(w_Segment_Time);
                    
                    //          if(Sprint8_table.Speed_table!=NULL)
                    //          {
                    //            if(by_Segment>0 && by_Segment<Sprint8_table.Max_Segment)
                    //            {
                    //              if(by_Segment%2==0)
                    //              {
                    //                Data_Set_SpeedOffset(Recovery.SpeedOffset);
                    //              }
                    //              else
                    //              {
                    //                Data_Set_SpeedOffset(Sprint.SpeedOffset);
                    //              }  
                    //            }
                    //            Data_Set_Speed( Sprint8_table.Speed_table[Sprint8_table.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment],STANDARD,0);
                    //          }
                    //          if(Sprint8_table.Incline_table!=NULL)
                    //          {
                    //            if(by_Segment>0 && by_Segment<Sprint8_table.Max_Segment)
                    //            {
                    //              if(by_Segment%2==0)
                    //              {
                    //                Data_Set_InclineOffset(Recovery.IncOffset);
                    //              }
                    //              else
                    //              {
                    //                Data_Set_InclineOffset(Sprint.IncOffset);
                    //              }
                    //            }
                    //            Data_Set_Incline( Sprint8_table.Incline_table[Sprint8_table.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment],0);
                    //          }
                    //          if(Sprint8_table.Resistance_table!=NULL)
                    //          {
                    //            if(by_Segment>0 && by_Segment<Sprint8_table.Max_Segment)
                    //            {
                    //              if(by_Segment%2==0)
                    //              {
                    //                Data_Set_ResistanceOffset(Recovery.ResOffset);
                    //              }
                    //              else
                    //              {
                    //                Data_Set_ResistanceOffset(Sprint.ResOffset);
                    //              }
                    //            }
                    //            Data_Set_Resistance( Sprint8_table.Resistance_table[Sprint8_table.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment],0);
                    //          }
////                    if(Sprint8_table.Speed_table!=NULL)
////                    {
////                        //            if(by_Segment>0 && by_Segment<Sprint8_table.Max_Segment)
////                        {
////                            if(by_Segment%2)
////                            {
////                                Data_Set_SpeedOffset(0);
////                            }
////                        }
////                        Data_Set_Speed( Sprint8_table.Speed_table[Sprint8_table.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment],STANDARD,0);
////                    }
////                    if(Sprint8_table.Incline_table!=NULL)
////                    {
////                        //            if(by_Segment>0 && by_Segment<Sprint8_table.Max_Segment)
////                        {
////                            if(by_Segment%2)
////                            {
////                                Data_Set_InclineOffset(0);
////                            }
////                        }
////                        Data_Set_Incline( Sprint8_table.Incline_table[Sprint8_table.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment],0);
////                    }
////                    if(Sprint8_table.Resistance_table!=NULL)
////                    {
////                        //            if(by_Segment>0 && by_Segment<Sprint8_table.Max_Segment)
////                        {
////                            if(by_Segment%2)
////                            {
////                                Data_Set_ResistanceOffset(0);
////                            }
////                        }
////                        Data_Set_Resistance( Sprint8_table.Resistance_table[Sprint8_table.Max_Segment*(Data_Get_Level(TARGET_LEVEL)-1)+by_Segment],0);
////                    }
                }
            }
            else
            {
                by_Segment = 0;
                w_Last_Point = 0;
                w_Segment_Time = Sprint8_table.StatusTime[Sprint8_table.Max_Segment*(2*Data_Get_Level(TARGET_LEVEL)-1)+by_Segment];
                Data_Set_StageTime(w_Segment_Time);
            }
            if((by_WarmCool != COOL_DOWN) & ((w_Target - w_Last_Point - w_Segment_Time) < w_CooldownTime))
            {
                w_Segment_Time = w_Target - w_Last_Point;
                w_Segment_Time -= w_CooldownTime;
            }
            if(w_Last_Point >= w_WarmupTime && by_WarmCool != COOL_DOWN)
            {
                by_WarmCool = REPEAT;
                pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
            }
        }
        
        Stage_Remaining_Time=Data_Get_Time(STAGE_TIME)-Data_Get_Time(TIME_IN_STAGE);
        if(by_Segment%2==0)//activerecovery
        {
            if(w_Auto>Data_Get_Time(COOLDOWN_TIME))
            {
                if((Stage_Remaining_Time==w_AheadTime)&&(w_AheadTime > 4))
                {
                    pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
                    pendingEvt.WorkOutEvts.EvtBit.Workout_Sprint8_RampUp=true;
                }
                else if(Stage_Remaining_Time==3)
                {
                    pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
                    pendingEvt.WorkOutEvts.EvtBit.Workout_Sprint8_Ready=true;  
                }
                else if(Stage_Remaining_Time==2)
                {
                    pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
                    pendingEvt.WorkOutEvts.EvtBit.Workout_Sprint8_Set=true;  
                }
                else if(Stage_Remaining_Time==1)
                {
                    pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
                    pendingEvt.WorkOutEvts.EvtBit.Workout_Sprint8_Go=true;
                }
                else if(Data_Get_Time(TIME_IN_STAGE)==0 && w_Time>Data_Get_Time(WARMUP_TIME))
                {
                    pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
                    pendingEvt.WorkOutEvts.EvtBit.Workout_Sprint8_ActiveRecoveryStage=true;        
                }
                
            }
        }
        else
        {
            //      if(Data_Get_Time(TIME_IN_STAGE)==0)
            //      {
            //        pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
            //        pendingEvt.WorkOutEvts.EvtBit.Workout_Sprint8_Go=true;
            //      }
            //      else 
            if(Data_Get_Time(TIME_IN_STAGE)==0)
            {
                Sprint8Stage++;
                pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
                pendingEvt.WorkOutEvts.EvtBit.Workout_Sprint8_SprintStage=true;
            }
        }
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

void initSprint8(void)
{
  struProgram Sprint8;
  Sprint8.Initial = ProgramSprint8_Data_Initial;
  Sprint8.Setup = ProgramSprint8_SetUp;
  Sprint8.GetProfile=ProgramSprint8_GetShow;
  Sprint8.Workout = ProgramSprint8_Run;
  Sprint8.Programid = PS_SPRINT8;
  Program_Add_Array(Sprint8);  
}

#endif

