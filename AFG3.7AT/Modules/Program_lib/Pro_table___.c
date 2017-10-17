#include"Pro_table.h"
#ifdef __PROGRAM_TABLE__

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
  UINT16 targetTime;
  UINT16 targetDistance;
  UINT16 targetCalories;
  UINT8 RepeatStart;
  UINT8 CoolDownStart;
  UINT8 WorkoutTotalSegment;
  UINT8 Stage;
  UINT8 State;
}P_Table;
__no_init static struProgram Pro_table;
__no_init static P_Table PTable;
__no_init static PROFILE Profile;

__no_init static Pro_RunPara Ptable_RunPara;
void ProgramTable_LookUpParameter(void)
{
  char loopState = 0, returnVal = 0;
  if(Ptable_RunPara.WorkoutRunType == FOLLOW_TABLE)
  {
  PTable.WorkoutTotalSegment=Ptable_RunPara.Segamount;
    for(char i = 0; i<Ptable_RunPara.Segamount; i++)
    {
      returnVal=Ptable_RunPara.StatusStageGoal[i].Status;
      if(loopState==0 && returnVal==Repeat)
      {
        PTable.WarmupTime = PTable.WorkoutTotalTime;
        PTable.RepeatStart = i;
        loopState = 1;
      }
      if(loopState==1 && returnVal==CoolDown)
      {
        PTable.CooldownTime += Ptable_RunPara.StatusStageGoal[Ptable_RunPara.Segamount+i].StageGoal;
        if(PTable.CoolDownStart == 0) 	PTable.CoolDownStart = i;
      }
      PTable.WorkoutTotalTime  += Ptable_RunPara.StatusStageGoal[Ptable_RunPara.Segamount+i].StageGoal;
    }
    Data_Set_WarmUP_Time(PTable.WarmupTime);
    Data_Set_CoolDown_Time(PTable.CooldownTime);
    Data_Set_Time(PTable.WorkoutTotalTime);
  }
  else if(Ptable_RunPara.WorkoutRunType == TOTAL_GOAL_AVERAGE)
  {
  PTable.WorkoutTotalSegment=Ptable_RunPara.Segamount;
    Data_Set_WarmUP_Time(0);
    Data_Set_CoolDown_Time(0);
  }
  else if(Ptable_RunPara.WorkoutRunType == TOTALGOAL_WITH_REPEAT)
	{

	}
}

void ProgramTable_Data_Initial(void)
{
  UINT16 i;
  memset(&PTable,0,sizeof(P_Table));
  Ptable_RunPara = *(ProgramLibConfigPara->Pro_Config[0].RunParameter);  
  for(i=0;i<ProgramLibConfigPara->ProgramTotalNum;i++)
  {
    if(ProgramLibConfigPara->Pro_Config[i].ProgramFirstID==Program_Get_ProgramID())
    {
      Ptable_RunPara = *(ProgramLibConfigPara->Pro_Config[i].RunParameter);
      break;
    }
  }
//  PTable.targetTime=1800;
//    PTable.targetDistance=1000;
//    PTable.targetCalories=100;
  if(Ptable_RunPara.WorkoutGoalType==GOAL_TYPES_DISTANCE)
//  	Data_Set_targetDistancex1000(1000);
      PTable.targetDistance=1000;

  else if(Ptable_RunPara.WorkoutGoalType==GOAL_TYPES_TIME)
//	  Data_Set_Time(30*60);
    PTable.targetTime=1800;

  else if(Ptable_RunPara.WorkoutGoalType==GOAT_TYPES_CALORIES)
//	  Data_Set_targetCalories(100);
      PTable.targetCalories=100;
  PTable.State = WARM_UP;
  Data_Set_Level(1);
  Data_Set_Level_MaxVal(Ptable_RunPara.Levelamount);
Data_Set_Speed(Data_Get_Speed(MIN_SPEEDX10),SystemConfig_Get_Char(UNIT_FLAG_UCHAR),1); 
Data_Set_Resistance(SystemConfig_Get_Char(MIN_RESISTANCE_UINT8),1);
Data_Set_Incline(0,1);      

  ProgramTable_LookUpParameter();
  if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
  {
    if(Ptable_RunPara.DefalultProfleType==PROFILE_TYPE_SPEED)
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
    if(Ptable_RunPara.DefalultProfleType==PROFILE_TYPE_RESISTANCE)
    {
      ProfileIndex=DefaultDotmatrix=PROFILE_TYPE_RESISTANCE;
    }
    else
    {
      ProfileIndex=DefaultDotmatrix=PROFILE_TYPE_INCLINE;
    }
  }
}
void ProgramTable_BeginWorkout(void)
{

}
UINT16 ProgramTable_SetUpData(UINT8 ParaIndex)
{
	UINT16 w_Dat=0;
     switch(ParaIndex)
	{

		case INDEX_TIME:
			w_Dat=Data_Get_Time(TARGET_TIME);
		break;
		default:break;
     	}
 	 return w_Dat;
}
UINT16 ProgramTable_RunData(UINT8 ParaIndex)
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
PROGRAM_PENDING_EVTS ProgramTable_SetUp(ProgramKeyEvts *keyevt)
{
  PROGRAM_PENDING_EVTS pendingEvt;
  pendingEvt.evt=PROGRAM_NONE_EVENT;
  memset(&pendingEvt.WorkOutEvts,0,sizeof(pendingEvt.WorkOutEvts));
  
  if(keyevt->Bit_Flag.Key_Enter)
  {
    keyevt->Bit_Flag.Key_Enter=false;
    if(Pro_table.Setting_Array[Pro_table.SettingArray_Index] != SET_PRESS_START )
    {
      keyevt->Bit_Flag.Key_StateChange=true;
      Pro_table.SettingArray_Index++;
    }
  }
  else if(keyevt->Bit_Flag.Key_Back)
  {
    keyevt->Bit_Flag.Key_Back=false;
    if(Pro_table.Setting_Array[Pro_table.SettingArray_Index] != NO_SETTING )
    {
      keyevt->Bit_Flag.Key_StateChange=true;
      if(SET_TIME==Pro_table.Setting_Array[Pro_table.SettingArray_Index])
      {
        Data_Set_Time(30*60);
      }
      else if(SET_LEVEL==Pro_table.Setting_Array[Pro_table.SettingArray_Index])
      {
        Data_Set_Level(Data_Get_Level(MIN_LEVEL));
      }
      Pro_table.SettingArray_Index--;
    }
  }
  switch(Pro_table.Setting_Array[Pro_table.SettingArray_Index])
  {
  case SET_PROGRAM:
    if(keyevt->Bit_Flag.Key_StateChange)
    {
      keyevt->Bit_Flag.Key_StateChange=false;
      pendingEvt.evt=PROGRAM_CHANGE_EVENT;
	        Pro_table.Prg_NextStatus(PS_SELECTPRG);

      ProgramTable_Data_Initial();
    }      
    break;
	
  case SET_TIME:
    if(keyevt->Bit_Flag.Key_StateChange)
    {
      keyevt->Bit_Flag.Key_StateChange=false;
      pendingEvt.evt=TIME_CHANGE_EVENT;
    }
    if(keyevt->Bit_Flag.Key_Up)
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
    else if(keyevt->Bit_Flag.Key_Dn)
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
  case SET_DISTANCE:
    if(keyevt->Bit_Flag.Key_StateChange)
    {
      keyevt->Bit_Flag.Key_StateChange=false;
      pendingEvt.evt=DISTANCE_CHANGE_EVENT;
    }
    if(keyevt->Bit_Flag.Key_Up)
    {
      pendingEvt.evt=DISTANCE_CHANGE_EVENT;
    }
    else if(keyevt->Bit_Flag.Key_Dn)
    {
      pendingEvt.evt=DISTANCE_CHANGE_EVENT;
    }
    break;
  case SET_CALORIES:
    if(keyevt->Bit_Flag.Key_StateChange)
    {
      keyevt->Bit_Flag.Key_StateChange=false;
      pendingEvt.evt=CALORIES_CHANGE_EVENT;
    }
    if(keyevt->Bit_Flag.Key_Up)
    {
      pendingEvt.evt=CALORIES_CHANGE_EVENT;
    }
    else if(keyevt->Bit_Flag.Key_Dn)
    {
      pendingEvt.evt=CALORIES_CHANGE_EVENT;
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
    if(Ptable_RunPara.SpeedTable!=NULL)
    {
      Data_Set_Speed( Ptable_RunPara.SpeedTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.Stage],STANDARD,0);
    }
    if(Ptable_RunPara.InclineTable!=NULL)
    {
      Data_Set_Incline( Ptable_RunPara.InclineTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.Stage],0);
    }
    if(Ptable_RunPara.ResistanceTable!=NULL)
    {
      Data_Set_Resistance( Ptable_RunPara.ResistanceTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.Stage],0);
    }
if(Ptable_RunPara.WorkoutRunType == FOLLOW_TABLE)
  PTable.SegmentTime = Ptable_RunPara.StatusStageGoal[Ptable_RunPara.Segamount+PTable.Stage].StageGoal;
else if(Ptable_RunPara.WorkoutRunType == TOTAL_GOAL_AVERAGE)
  PTable.SegmentTime= Data_Get_Time(TARGET_TIME)/Ptable_RunPara.Segamount;
Data_Set_StageTime(PTable.SegmentTime);  
  return pendingEvt; 
}
PROFILE *ProgramTable_ProfileShow(PROFILE_TAG tag_index,UCHAR* Dotmatrix_pos)
{
  
  UINT16 by_SEG;
  UINT8 i,k;
  by_SEG = PTable.WorkoutTotalSegment- 1 - PTable.Stage;
  if(PTable.WorkoutTotalSegment<ProgramLibConfigPara->DotMatrixLength)
  {
    *Dotmatrix_pos = PTable.Stage;
  }
  else
  {
	  if(by_SEG < ProgramLibConfigPara->DotMatrixLength)
	  {
	    *Dotmatrix_pos = ProgramLibConfigPara->DotMatrixLength - 1 - by_SEG;
	  }
	  else
	  {
	    *Dotmatrix_pos = 0;
	  }
  }
  for( i = *Dotmatrix_pos,k=0; i < ProgramLibConfigPara->DotMatrixLength; i++,k++)
  {
    if(Ptable_RunPara.SpeedTable!=NULL)
    {
      Prg_ProfileData.SpeedProfile[i]=Data_Get_DotmatrixSpeedData( Ptable_RunPara.SpeedTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+k+PTable.Stage]);
    }
    else
    {
      Prg_ProfileData.SpeedProfile[i]=Data_Get_Speed(TARGET_SPEEDX10_AUTO);
    }
    
    if(Ptable_RunPara.InclineTable!=NULL)
    {
      Prg_ProfileData.InclineProfile[i]=Data_Get_DotmatrixInclineData( Ptable_RunPara.InclineTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+k+PTable.Stage]);
    }
    else
    {
      Prg_ProfileData.InclineProfile[i]=Data_Get_Incline(TARGET_INCLINEX10);
    }
    if(Ptable_RunPara.ResistanceTable!=NULL)
    {
      Prg_ProfileData.ResistanceProfile[i]=Data_Get_DotmatrixResistanceData( Ptable_RunPara.ResistanceTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+k+PTable.Stage]);
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
      if(Ptable_RunPara.DefalultProfleType==PROFILE_TYPE_SPEED)
        Profile.ProfileData=Prg_ProfileData.SpeedProfile;
      else
        Profile.ProfileData=Prg_ProfileData.InclineProfile;
      
      break;
    default:
      if(Ptable_RunPara.DefalultProfleType==PROFILE_TYPE_SPEED)
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
      if(Ptable_RunPara.DefalultProfleType==PROFILE_TYPE_RESISTANCE)
        Profile.ProfileData=Prg_ProfileData.ResistanceProfile;
      else
        Profile.ProfileData=Prg_ProfileData.InclineProfile;
      break;
    default:
      if(Ptable_RunPara.DefalultProfleType==PROFILE_TYPE_RESISTANCE)
        Profile.ProfileData=Prg_ProfileData.ResistanceProfile;
      else
        Profile.ProfileData=Prg_ProfileData.InclineProfile;
      break;
    }  
  }
  Profile.Length=ProgramLibConfigPara->DotMatrixLength;
  return &Profile;
}



PROGRAM_PENDING_EVTS ProgramTable_Run(ProgramKeyEvts *keyevt)
{  
  UINT16 Remaining_Data;
  UINT16 DiffData;
  UINT16 TargetData;
  UINT16 ElapsedData;
  UINT16 Stage_Remaining_Time;
  static UINT16 LastValue=0xffff;
  
  PROGRAM_PENDING_EVTS pendingEvt;
  pendingEvt.evt=PROGRAM_NONE_EVENT;
  memset(&pendingEvt.WorkOutEvts,0,sizeof(pendingEvt.WorkOutEvts));
  if(Pro_table.Prg_CurrentStatus()<PS_WARMUP||Pro_table.Prg_CurrentStatus()>PS_COOLDOWN)return pendingEvt;
  TargetData=Data_Get_Time(TARGET_TIME);
  ElapsedData=Data_Get_Time(ELAPSED_TIME);
  Remaining_Data=TargetData-ElapsedData;
  if(LastValue!=ElapsedData)
  {
    LastValue=ElapsedData;
    if(Remaining_Data==0)
    {
      if(Pro_table.Prg_CurrentStatus()!=PS_COMPLETED)
      {
        Pro_table.Prg_NextStatus(PS_COMPLETED);
        pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
        pendingEvt.WorkOutEvts.Workout_Complete_Evt=true;
      }
    }
    else
    {
        DiffData = ElapsedData - PTable.LastPoint;
      if(Ptable_RunPara.WorkoutRunType == FOLLOW_TABLE)
      {
        if(ElapsedData==0)
        {
          PTable.State = WARM_UP;
          Pro_table.Prg_NextStatus(PS_WARMUP);	  
          pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
          pendingEvt.WorkOutEvts.Workout_Warmup_Evt=true;
        }
        if(DiffData >= PTable.SegmentTime)
        {
          PTable.LastPoint = ElapsedData;
          PTable.Stage++;
          Beep_Set_Beeps(1,3,1);            
          if(PTable.LastPoint >= PTable.WarmupTime)
          {
            if(TargetData - PTable.LastPoint > PTable.CooldownTime)
            {
              if(PTable.Stage >= PTable.CoolDownStart)
              {
                PTable.Stage = PTable.RepeatStart;
              }
            }
            else
            {
              if(PTable.State != COOL_DOWN)
              {
                PTable.State = COOL_DOWN;
		Pro_table.Prg_NextStatus(PS_COOLDOWN);		
                pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
                pendingEvt.WorkOutEvts.Workout_Cooldown_Evt=true;
                PTable.Stage = PTable.CoolDownStart;
              }
            }
          }	
          PTable.SegmentTime =  Ptable_RunPara.StatusStageGoal[Ptable_RunPara.Segamount+PTable.Stage].StageGoal;
          Data_Set_StageTime(PTable.SegmentTime);
          if(Ptable_RunPara.SpeedTable!=NULL)
          {
            Data_Set_Speed( Ptable_RunPara.SpeedTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.Stage],STANDARD,0);
          }
          if(Ptable_RunPara.InclineTable!=NULL)
          {
            Data_Set_Incline( Ptable_RunPara.InclineTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.Stage],0);
          }
          if(Ptable_RunPara.ResistanceTable!=NULL)
          {
            Data_Set_Resistance( Ptable_RunPara.ResistanceTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.Stage],0);
          }  
        }	
		if((PTable.State != COOL_DOWN) & ((TargetData - PTable.LastPoint - PTable.SegmentTime) < PTable.CooldownTime))
		{
			PTable.SegmentTime = TargetData - PTable.LastPoint;
			PTable.SegmentTime -= PTable.CooldownTime;
		}
		if(PTable.LastPoint >= PTable.WarmupTime && PTable.State != COOL_DOWN)
		{
			PTable.State = REPEAT;
			Pro_table.Prg_NextStatus(PS_RUN);
			pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
		}
      }
      else if(Ptable_RunPara.WorkoutRunType == TOTAL_GOAL_AVERAGE)
      {
        if(DiffData >= PTable.SegmentTime)
        {
          PTable.LastPoint = ElapsedData;
          PTable.Stage++;
          Beep_Set_Beeps(1,3,1);
          if(PTable.Stage<Ptable_RunPara.Segamount-1)
            PTable.SegmentTime= TargetData/Ptable_RunPara.Segamount;
          else
            PTable.SegmentTime=TargetData%PTable.SegmentTime+PTable.SegmentTime;
          Data_Set_StageTime(PTable.SegmentTime);
          if(Ptable_RunPara.SpeedTable!=NULL)
          {
            Data_Set_Speed( Ptable_RunPara.SpeedTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.Stage],STANDARD,0);
          }
          if(Ptable_RunPara.InclineTable!=NULL)
          {
            Data_Set_Incline( Ptable_RunPara.InclineTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.Stage],0);
          }
          if(Ptable_RunPara.ResistanceTable!=NULL)
          {
            Data_Set_Resistance( Ptable_RunPara.ResistanceTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.Stage],0);
          }  
        }
      }
    }
          if(Ptable_RunPara.SpeedTable!=NULL)
          {
            Data_Set_Speed( Ptable_RunPara.SpeedTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.Stage],STANDARD,0);
          }
          if(Ptable_RunPara.InclineTable!=NULL)
          {
            Data_Set_Incline( Ptable_RunPara.InclineTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.Stage],0);
          }
          if(Ptable_RunPara.ResistanceTable!=NULL)
          {
            Data_Set_Resistance( Ptable_RunPara.ResistanceTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.Stage],0);
          }  
  }
  
  if(keyevt->Bit_Flag.Key_SpeedUp)
  {
    keyevt->Bit_Flag.Key_SpeedUp=false;
    Data_Set_Speed(Data_Get_Speed(TARGET_SPEEDX10_AUTO)+1,SystemConfig_Get_Char(UNIT_FLAG_UCHAR)==METRIC?METRIC:STANDARD,1);
  }
  else if(keyevt->Bit_Flag.Key_SpeedDn)
  {
    keyevt->Bit_Flag.Key_SpeedDn=false;
    Data_Set_Speed(Data_Get_Speed(TARGET_SPEEDX10_AUTO)-1,SystemConfig_Get_Char(UNIT_FLAG_UCHAR)==METRIC?METRIC:STANDARD,1);
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

void initTable(void)
{
  Pro_table.Prg_Initial = ProgramTable_Data_Initial;
  Pro_table.BeginWorkout=  ProgramTable_BeginWorkout;
  Pro_table.Prg_Setup = ProgramTable_SetUp;
  Pro_table.Prg_ProfileShow=ProgramTable_ProfileShow;
  Pro_table.Prg_Workout = ProgramTable_Run;
  Pro_table.Prg_SetupData=ProgramTable_SetUpData;
  Pro_table.Prg_RunData = ProgramTable_RunData;

  Pro_table.Programid = PS_DISTANCE_GOAL;
  Pro_table.SettingArray_Index=0;
  Pro_table.Setting_Array=ProTable_SetUp;
  Program_Register_Function(&Pro_table);
  Pro_table.Programid = PS_CALORIES_GOAL;
  Program_Register_Function(&Pro_table);
  Pro_table.Programid = PS_ROLLING_HILLS;
  Program_Register_Function(&Pro_table);
  Pro_table.Programid = PS_FATBURN;
  Program_Register_Function(&Pro_table);
  
}





#endif

