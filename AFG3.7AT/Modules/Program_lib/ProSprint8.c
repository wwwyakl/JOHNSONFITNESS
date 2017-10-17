#include"ProSprint8.h"
#ifdef __PROGRAM_SPRINT8__

#define WARM_UP	0
#define COOL_DOWN	1
#define REPEAT	2
typedef struct{
  UINT16 WorkoutTotalTime;
  UINT16 StageTime;
  UINT16 SegmentTime;
  UINT16 WarmupTime;
  UINT16 CooldownTime;
  UINT16 LastPoint;//last time point
  UINT8 RepeatStart;
  UINT8 CoolDownStart;
  UINT8 WorkoutTotalSegment;
  UINT8 Stage;
  UINT8 State;
  UINT8 SprintStage;
}P_Sprint8;
__no_init static P_Sprint8 PSprint8;
__no_init static struProgram Sprint8;
__no_init static PROFILE Profile;
__no_init static Pro_RunPara Sprint8_table;

UINT16 Sprin8_Get_SpeedDotMatrixData(UCHAR by_Dat)
{
  UINT16 Out=0;
  UINT16 SpeedMax,SpeedMin;
  UINT16 Sprint8Base=Sprint8_table.SpeedTable[Sprint8_table.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+by_Dat];
  INT16 Sprint8Offset=0;  
  if(by_Dat<=PSprint8.Stage)
  {
    Sprint8Offset=Data_Get_SpeedOffset();
  }
  if(PSprint8.Stage>0 && by_Dat<Sprint8_table.Segamount-1)
  {
    if(PSprint8.Stage%2)
    {
      if(by_Dat==PSprint8.Stage+1)
      {
        Sprint8Offset=Data_Get_SpeedOffset();
      }
    }
  }
  if(SystemConfig_Get_Display_Unit(User_Get_UserSlot( ))==STANDARD)
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
  UINT16 Sprint8Base=Sprint8_table.InclineTable[Sprint8_table.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+by_Dat];
  INT16 Sprint8Offset=0;  
  if(by_Dat<=PSprint8.Stage)
  {
    Sprint8Offset=Data_Get_InclineOffset();
  }
  if(PSprint8.Stage>0 && by_Dat<Sprint8_table.Segamount-1)
  {
    if(PSprint8.Stage%2)
    {
      if(by_Dat==PSprint8.Stage+1)
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
  UINT16 Sprint8Base=Sprint8_table.ResistanceTable[Sprint8_table.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+by_Dat];
  INT16 Sprint8Offset=0;  
  if(by_Dat<=PSprint8.Stage)
  {
    Sprint8Offset=Data_Get_ResistanceOffset();
  }
  if(PSprint8.Stage>0 && by_Dat<Sprint8_table.Segamount-1)
  {
    if(PSprint8.Stage%2)
    {
      if(by_Dat==PSprint8.Stage+1)
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
  for(char i = 0; i<Sprint8_table.Segamount; i++)
  {
    returnVal=Sprint8_table.StatusStageGoal[i].Status;
    if(loopState==0 && returnVal==Repeat)
    {
      PSprint8.WarmupTime = PSprint8.WorkoutTotalTime;
      PSprint8.RepeatStart = i;
      loopState = 1;
    }
    if(loopState==1 && returnVal==CoolDown)
    {
      PSprint8.CooldownTime += Sprint8_table.StatusStageGoal[Sprint8_table.Segamount+i].StageGoal;
      if(PSprint8.CoolDownStart == 0) 	PSprint8.CoolDownStart = i;
    }
    PSprint8.WorkoutTotalTime  += Sprint8_table.StatusStageGoal[Sprint8_table.Segamount+i].StageGoal;
  }
  Data_Set_WarmUP_Time(PSprint8.WarmupTime);
  Data_Set_CoolDown_Time(PSprint8.CooldownTime);
  Data_Set_Time(PSprint8.WorkoutTotalTime);
  if(Sprint8_table.SpeedTable!=NULL)
  {
  Data_Set_Speed_Base_User(Sprint8_table.SpeedTable[Sprint8_table.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PSprint8.Stage],0);
    //Data_Set_Speed( Sprint8_table.SpeedTable[Sprint8_table.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PSprint8.Stage],STANDARD,0);
  }
  if(Sprint8_table.InclineTable!=NULL)
  {
    Data_Set_Incline( Sprint8_table.InclineTable[Sprint8_table.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PSprint8.Stage],0);
  }
  if(Sprint8_table.ResistanceTable!=NULL)
  {
    Data_Set_Resistance( Sprint8_table.ResistanceTable[Sprint8_table.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PSprint8.Stage],0);
  }
}

void ProgramSprint8_Data_Initial(void)
{
  char i;
  memset(&PSprint8,0,sizeof(PSprint8));
  PSprint8.State = WARM_UP;
  PSprint8.SprintStage=0;
  Data_Set_Level(1);
  for(i=0;i<ProgramLibConfigPara->ProgramTotalNum-1;i++)
  {
    if(ProgramLibConfigPara->Pro_Config[i].ProgramFirstID==PS_SPRINT8)
    {
      Sprint8_table = *(ProgramLibConfigPara->Pro_Config[i].RunParameter);
      break;
    }
  }
  Data_Set_Level_MaxVal(Sprint8_table.Levelamount);
  ProgramSprint8_LookUpParameter();
  if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
  {
    if(Sprint8_table.DefalultProfleType==PROFILE_TYPE_SPEED)
    {
      ProfileIndex=DefaultDotmatrix=PROFILE_TYPE_SPEED;
    }
    else
    {
      ProfileIndex=DefaultDotmatrix=PROFILE_TYPE_INCLINE;
    }
  }
  else
  {
    if(Sprint8_table.DefalultProfleType==PROFILE_TYPE_RESISTANCE)
    {
      ProfileIndex=DefaultDotmatrix=PROFILE_TYPE_RESISTANCE;
    }
    else
    {
      ProfileIndex=DefaultDotmatrix=PROFILE_TYPE_INCLINE;
    }
  }
}
void ProgramSprint8_Data_BeginWorkout(void)
{


}
UINT16 ProgramSprint8_SetUpData(UINT8 ParaIndex)
{
	UINT16 w_Dat=0;
	 return w_Dat;
}
UINT16 ProgramSprint8_RunData(UINT8 ParaIndex)
{
	UINT16 w_Dat=0;
     switch(ParaIndex)
	{
		case INDEX_TIME:
		        w_Dat=Data_Get_Time(STAGE_TIME)-Data_Get_Time(TIME_IN_STAGE);
			break;
		case INDEX_WATT:
				w_Dat=Data_Get_Watts(CURRENT_WATT);
			break;
     	}
	 return w_Dat;
}

PROGRAM_PENDING_EVTS ProgramSprint8_SetUp(ProgramKeyEvts *keyevt)
{
  
  UINT16 w_Diff;
  UINT16 Target_Data;
  UINT16 Elapsed_Data;
  PROGRAM_PENDING_EVTS pendingEvt;
  pendingEvt.evt=PROGRAM_NONE_EVENT;
  memset(&pendingEvt.WorkOutEvts,0,sizeof(pendingEvt.WorkOutEvts));
      if(keyevt->Bit_All == 0) return pendingEvt;

  if(keyevt->Bit_Flag.Key_Enter)
  {
    keyevt->Bit_Flag.Key_Enter=false;
    if(Sprint8.Setting_Array[Sprint8.SettingArray_Index] != SET_PRESS_START )
    {
      keyevt->Bit_Flag.Key_StateChange=true;
      Sprint8.SettingArray_Index++;
    }
  }
  else if(keyevt->Bit_Flag.Key_Back)
  {
    keyevt->Bit_Flag.Key_Back=false;
    if(Sprint8.Setting_Array[Sprint8.SettingArray_Index] != NO_SETTING )
    {
      keyevt->Bit_Flag.Key_StateChange=true;
      if(SET_TIME==Sprint8.Setting_Array[Sprint8.SettingArray_Index])
      {
        Data_Set_Time(30*60);
      }
      else if(SET_LEVEL==Sprint8.Setting_Array[Sprint8.SettingArray_Index])
      {
        Data_Set_Level(Data_Get_Level(MIN_LEVEL));
      }
      Sprint8.SettingArray_Index--;
    }
  }
  switch(Sprint8.Setting_Array[Sprint8.SettingArray_Index])
  {
  case SET_PROGRAM:
    if(keyevt->Bit_Flag.Key_StateChange)
    {
      keyevt->Bit_Flag.Key_StateChange=false;
      pendingEvt.evt=PROGRAM_CHANGE_EVENT;
	  Sprint8.Prg_NextStatus(PS_SELECTPRG);
      ProgramSprint8_Data_Initial();
    }      
    break;
  case SET_LEVEL:
    if(keyevt->Bit_Flag.Key_StateChange)
    {
      keyevt->Bit_Flag.Key_StateChange=false;
      pendingEvt.evt=LEVEL_CHANGE_EVENT;
    }
    if(keyevt->Bit_Flag.Key_Up)
    {
      keyevt->Bit_Flag.Key_Up=false;
      Data_Set_Level(Data_Get_Level(TARGET_LEVEL)+1);
      pendingEvt.evt=LEVEL_CHANGE_EVENT;
    }
    else if(keyevt->Bit_Flag.Key_Dn)
    {
      Data_Set_Level(Data_Get_Level(TARGET_LEVEL)-1);
      pendingEvt.evt=LEVEL_CHANGE_EVENT;
    }
    break;
  case SET_PRESS_START:
    if(keyevt->Bit_Flag.Key_StateChange)
    {
      keyevt->Bit_Flag.Key_StateChange=false;
      pendingEvt.evt=SETTING_COMPLETE_EVENT;
    }   
    
  }
  return pendingEvt; 
}
PROFILE *ProgramSprint8_ProfileShow(PROFILE_TAG tag_index,UCHAR* Dotmatrix_pos)
{
  
  UINT16 by_SEG;
  UINT8 i,k;
  by_SEG = Sprint8_table.Segamount- 1 - PSprint8.Stage;
  if(by_SEG < ProgramLibConfigPara->DotMatrixLength)
  {
    *Dotmatrix_pos = ProgramLibConfigPara->DotMatrixLength - 1 - by_SEG;
  }
  else
  {
    *Dotmatrix_pos = 0;
  }
  for( i = *Dotmatrix_pos,k=0; i < ProgramLibConfigPara->DotMatrixLength; i++,k++)
  {
    if(Sprint8_table.SpeedTable!=NULL)
    {
      Prg_ProfileData.SpeedProfile[i]=Sprin8_Get_SpeedDotMatrixData(k+PSprint8.Stage);
    }
    else
    {
      Prg_ProfileData.SpeedProfile[i]=Data_Get_Speed(TARGET_SPEEDX10_AUTO);
    }
    
    if(Sprint8_table.InclineTable!=NULL)
    {
      Prg_ProfileData.InclineProfile[i]=Sprin8_Get_InclineDotMatrixData( k+PSprint8.Stage);
    }
    else
    {
      Prg_ProfileData.InclineProfile[i]=Data_Get_Incline(TARGET_INCLINEX10);
    }
    if(Sprint8_table.ResistanceTable!=NULL)
    {
      Prg_ProfileData.ResistanceProfile[i]=Sprin8_Get_ResistanceDotMatrixData(k+PSprint8.Stage);
    }
    else
    {
      Prg_ProfileData.ResistanceProfile[i]=Data_Get_Resistance(TARGET_RESISTANCE);
    }
  }
  if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
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
      if(Sprint8_table.DefalultProfleType==PROFILE_TYPE_SPEED)
        Profile.ProfileData=Prg_ProfileData.SpeedProfile;
      else
        Profile.ProfileData=Prg_ProfileData.InclineProfile;
      
      break;
    default:
      if(Sprint8_table.DefalultProfleType==PROFILE_TYPE_SPEED)
        Profile.ProfileData=Prg_ProfileData.SpeedProfile;
      else
        Profile.ProfileData=Prg_ProfileData.InclineProfile;
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
      if(Sprint8_table.DefalultProfleType==PROFILE_TYPE_RESISTANCE)
        Profile.ProfileData=Prg_ProfileData.ResistanceProfile;
      else
        Profile.ProfileData=Prg_ProfileData.InclineProfile;
      break;
    default:
      if(Sprint8_table.DefalultProfleType==PROFILE_TYPE_RESISTANCE)
        Profile.ProfileData=Prg_ProfileData.ResistanceProfile;
      else
        Profile.ProfileData=Prg_ProfileData.InclineProfile;
      break;
    }  
  }
  Profile.Length=ProgramLibConfigPara->DotMatrixLength;
  return &Profile;
}



PROGRAM_PENDING_EVTS ProgramSprint8_Run(ProgramKeyEvts *keyevt)
{  
  UINT16 Remaining_Data;
  UINT16 DiffData;
  UINT16 TargetData;
  UINT16 ElapsedData;
  static UINT16 LastValue=0xffff;

  UINT16 Stage_Remaining_Time;
  PROGRAM_PENDING_EVTS pendingEvt;
  pendingEvt.evt=PROGRAM_NONE_EVENT;
  memset(&pendingEvt.WorkOutEvts,0,sizeof(pendingEvt.WorkOutEvts));
  if(Sprint8.Prg_CurrentStatus()<PS_WARMUP||Sprint8.Prg_CurrentStatus()>PS_COOLDOWN)return pendingEvt;
  TargetData=Data_Get_Time(TARGET_TIME);
  ElapsedData=Data_Get_Time(ELAPSED_TIME);
  Remaining_Data=TargetData-ElapsedData;
  if(LastValue!=ElapsedData)
  {
    LastValue=ElapsedData;
    if(Remaining_Data==0)
    {
      if(Sprint8.Prg_CurrentStatus()!=PS_COMPLETED)
      {
      Sprint8.Prg_NextStatus(PS_COMPLETED);
        pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
        pendingEvt.WorkOutEvts.Workout_Complete_Evt=true;
      }
    }
    else
    {
      if(ElapsedData==0)
      {
        PSprint8.State = WARM_UP;
	Sprint8.Prg_NextStatus(PS_WARMUP);
        pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
        pendingEvt.WorkOutEvts.Workout_Warmup_Evt=true;
      }
        DiffData = ElapsedData - PSprint8.LastPoint;
        if(DiffData >= PSprint8.SegmentTime)
        {
          PSprint8.LastPoint = ElapsedData;
          PSprint8.Stage++;
          Beep_Set_Beeps(1,3,1);                      
          if(PSprint8.LastPoint >= PSprint8.WarmupTime)
          {
            if(TargetData - PSprint8.LastPoint > PSprint8.CooldownTime)
            {
              if(PSprint8.Stage >= PSprint8.CoolDownStart)
              {
                PSprint8.Stage = PSprint8.RepeatStart;
              }
            }
            else
            {
              if(PSprint8.State != COOL_DOWN)
              {
                PSprint8.State = COOL_DOWN;
		Sprint8.Prg_NextStatus(PS_COOLDOWN);		
                pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
                pendingEvt.WorkOutEvts.Workout_Cooldown_Evt=true;
                PSprint8.Stage = PSprint8.CoolDownStart;
              }
            }
          }
      if((PSprint8.State != COOL_DOWN) & ((TargetData - PSprint8.LastPoint - PSprint8.SegmentTime) < PSprint8.CooldownTime))
      {
        PSprint8.SegmentTime = TargetData - PSprint8.LastPoint;
        PSprint8.SegmentTime -= PSprint8.CooldownTime;
      }
      if(PSprint8.LastPoint >= PSprint8.WarmupTime && PSprint8.State != COOL_DOWN)
      {
        PSprint8.State = REPEAT;
		Sprint8.Prg_NextStatus(PS_RUN);
        pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
      }      
	  PSprint8.SegmentTime =  Sprint8_table.StatusStageGoal[Sprint8_table.Segamount+PSprint8.Stage].StageGoal;
          Data_Set_StageTime(PSprint8.SegmentTime);
          if(Sprint8_table.SpeedTable!=NULL)
          {
              if(PSprint8.Stage%2)
              {
                Data_Set_SpeedOffset(0);
              }
			  Data_Set_Speed_Base_User(Sprint8_table.SpeedTable[Sprint8_table.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PSprint8.Stage],0);
            //Data_Set_Speed( Sprint8_table.SpeedTable[Sprint8_table.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PSprint8.Stage],STANDARD,0);
          }
          if(Sprint8_table.InclineTable!=NULL)
          {
              if(PSprint8.Stage%2)
              {
                Data_Set_InclineOffset(0);
              }
            Data_Set_Incline( Sprint8_table.InclineTable[Sprint8_table.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PSprint8.Stage],0);
          }
          if(Sprint8_table.ResistanceTable!=NULL)
          {
              if(PSprint8.Stage%2)
              {
                Data_Set_ResistanceOffset(0);
              }
            Data_Set_Resistance( Sprint8_table.ResistanceTable[Sprint8_table.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PSprint8.Stage],0);
          }
        }
    }
    
    Stage_Remaining_Time=Data_Get_Time(STAGE_TIME)-Data_Get_Time(TIME_IN_STAGE);
    if(PSprint8.Stage%2==0)//activerecovery
    {
      if(Remaining_Data>Data_Get_Time(COOLDOWN_TIME))
      {
        if(Stage_Remaining_Time==3)
        {
          pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
          pendingEvt.WorkOutEvts.Workout_Sprint8_Ready=true;  
        }
        else if(Stage_Remaining_Time==2)
        {
          pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
          pendingEvt.WorkOutEvts.Workout_Sprint8_Set=true;  
        }
        else if(Stage_Remaining_Time==1)
        {
            pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
            pendingEvt.WorkOutEvts.Workout_Sprint8_Go=true;
        }
        else if(Data_Get_Time(TIME_IN_STAGE)==0 && ElapsedData>Data_Get_Time(WARMUP_TIME))
        {
          pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
          pendingEvt.WorkOutEvts.Workout_Sprint8_ActiveRecoveryStage=true;        
        }

      }
    }
    else
    {
      if(Data_Get_Time(TIME_IN_STAGE)==0)
      {
        PSprint8.SprintStage++;
	Data_Set_Sprint8SprintStage(PSprint8.SprintStage);	
        pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
        pendingEvt.WorkOutEvts.Workout_Sprint8_SprintStage=true;
      }
    }
  }
  if(keyevt->Bit_Flag.Key_SpeedUp)
  {
    keyevt->Bit_Flag.Key_SpeedUp=false;
	Data_Set_Speed_Base_User(Data_Get_Speed(TARGET_SPEEDX10_AUTO)+1,1);
   // Data_Set_Speed(Data_Get_Speed(TARGET_SPEEDX10_AUTO)+1,SystemConfig_Get_Display_Unit(User_Get_UserSlot( ))==METRIC,1);
  }
  else if(keyevt->Bit_Flag.Key_SpeedDn)
  {
    keyevt->Bit_Flag.Key_SpeedDn=false;
	Data_Get_Speed(TARGET_SPEEDX10_AUTO)-1,1);
    //Data_Set_Speed(Data_Get_Speed(TARGET_SPEEDX10_AUTO)-1,SystemConfig_Get_Display_Unit(User_Get_UserSlot( ))==METRIC,1);
  } 
  if(keyevt->Bit_Flag.Key_InclineUp)
  {
    keyevt->Bit_Flag.Key_InclineUp=false;
    Data_Set_Incline(Program_Get_Incline(0)+5,1);
  }
  else if(keyevt->Bit_Flag.Key_InclineDn)
  {
    keyevt->Bit_Flag.Key_InclineDn=false;
    if(Program_Get_Incline(0))
      Data_Set_Incline(Program_Get_Incline(0)-5,1);
  }      
  if(keyevt->Bit_Flag.Key_ResistanceUp)
  {
    keyevt->Bit_Flag.Key_ResistanceUp=false;
    Data_Set_Resistance(Program_Get_Resistance(0)+1,1);
  }
  else if(keyevt->Bit_Flag.Key_ResistanceDn)
  {
    keyevt->Bit_Flag.Key_ResistanceDn=false;
    Data_Set_Resistance(Program_Get_Resistance(0)-1,1);
  }   
  return pendingEvt;
}
UINT16 Sprint8_ShowDataProcess(UINT8 ParaIndex)
{
	
}
void initSprint8(void)
{
  Sprint8.Prg_Initial = ProgramSprint8_Data_Initial;
  Sprint8.BeginWorkout=  ProgramSprint8_Data_BeginWorkout;
  Sprint8.Prg_Setup = ProgramSprint8_SetUp;
  Sprint8.Prg_ProfileShow=ProgramSprint8_ProfileShow;
  Sprint8.Prg_Workout = ProgramSprint8_Run;
  Sprint8.Prg_SetupData=ProgramSprint8_SetUpData;
  Sprint8.Prg_RunData=ProgramSprint8_RunData;
  
  Sprint8.Programid = PS_SPRINT8;
  Sprint8.SettingArray_Index=0;
Sprint8.Setting_Array=ProSprint8_SetUp;

  Program_Register_Function(&Sprint8);  
}
#else
void initSprint8(void){
}
#endif

