#include"Pro_table.h"
#ifdef __PROGRAM_TABLE__

#define WARM_UP	0
#define COOL_DOWN	1
#define REPEAT	2

typedef struct{
	UINT8 Lable;
	UINT16 Data;
}DistanceTargetStru;
const DistanceTargetStru DistanceTargetArray[D_END]={
	{D_1_MILE,1000},
	{D_2_MILES,2000},
	{D_5_K,3125},
	{D_5_MILES,5000},
	{D_10_K,6250},
	{D_8_MILES,8000},
	{D_15_K,9375},
	{D_10_MILES,10000},
	{D_20_K,12500},
	{D_HALF_MARATHON,13100},
	{D_15_MILES,15000},
	{D_20_MILES,20000},
	{D_MARATHON,26200},	
};
typedef struct{
  UINT16 WorkoutTotalSegment;
  UINT16 TableTotalGoal;
  UINT16 StageTime;
  UINT16 SegmentGoal;
  UINT16 WarmupGoal;
  UINT16 CooldownGoal;
  UINT16 LastPoint;//last time point
  UINT16 targetTime;
  UINT16 targetCalories;
  UINT8 RepeatStart;
  UINT8 CoolDownStart;
  UINT8 Segamount;
  UINT8 Stage;
  UINT8 State;
  UINT8 DistanceIndex;
  UINT8 TableIndex;
  UINT8 SpeedBase;
  UINT8 InclineBase;
  UINT8 ResistanceBase;
}P_Table;
__no_init static struProgram Pro_table;
__no_init static P_Table PTable;
__no_init static PROFILE Profile;

__no_init static Pro_RunPara Ptable_RunPara;
void ProgramTable_LookUpParameter(void)
{
  char loopState = 0, returnVal = 0;
  UINT16 w_Dat=0;
  PTable.WarmupGoal=0;
  PTable.CooldownGoal=0;
  PTable.TableTotalGoal=0;
  PTable.WorkoutTotalSegment=0;
  if(Ptable_RunPara.WorkoutRunType == FOLLOW_TABLE)
  {
    PTable.Segamount=Ptable_RunPara.Segamount;
    for(char i = 0; i<Ptable_RunPara.Segamount; i++)
    {
      returnVal=Ptable_RunPara.StatusStageGoal[i].Status;
      if(loopState==0 && returnVal==Repeat)
      {
        PTable.WarmupGoal = PTable.TableTotalGoal;
        PTable.RepeatStart = i;
        loopState = 1;
      }
      if(loopState==1 && returnVal==CoolDown)
      {
        PTable.CooldownGoal += Ptable_RunPara.StatusStageGoal[i].StageGoal;
        if(PTable.CoolDownStart == 0) 	PTable.CoolDownStart = i;
      }
      PTable.TableTotalGoal  += Ptable_RunPara.StatusStageGoal[i].StageGoal;
    }
    Data_Set_SpeedOffset(0);
    Data_Set_InclineOffset(0);
    Data_Set_ResistanceOffset(0);
    Data_Set_WarmUP_Time(PTable.WarmupGoal);
    Data_Set_CoolDown_Time(PTable.CooldownGoal);
    Data_Set_Time(PTable.TableTotalGoal);
  }
  else if(Ptable_RunPara.WorkoutRunType == TOTAL_GOAL_AVERAGE)
  {
    PTable.Segamount=Ptable_RunPara.Segamount;
    Data_Set_WarmUP_Time(0);
    Data_Set_CoolDown_Time(0);
  }
  else if(Ptable_RunPara.WorkoutRunType == TOTALGOAL_WITH_REPEAT)
  {
    PTable.Segamount=Ptable_RunPara.Segamount;
    for(char i = 0; i<Ptable_RunPara.Segamount; i++)
    {
      returnVal=Ptable_RunPara.StatusStageGoal[i].Status;
      if(loopState==0 && returnVal==Repeat)
      {
        PTable.WarmupGoal = PTable.TableTotalGoal;
        PTable.RepeatStart = i;
        loopState = 1;
      }
      if(loopState==1 && returnVal==CoolDown)
      {
        PTable.CooldownGoal += Ptable_RunPara.StatusStageGoal[i].StageGoal;
        if(PTable.CoolDownStart == 0) 	PTable.CoolDownStart = i;
      }
      PTable.TableTotalGoal  += Ptable_RunPara.StatusStageGoal[i].StageGoal;
    }
    if(Ptable_RunPara.StageGoalType==STAGEGOAL_TYPE_DISTANCE)
    {
      Data_Set_WarmUP_Distance(PTable.WarmupGoal);
      Data_Set_CoolDown_Distance(PTable.CooldownGoal);
    }
    else if(Ptable_RunPara.StageGoalType==STAGEGOAL_TYPE_TIME)
    {
      Data_Set_WarmUP_Time(PTable.WarmupGoal);
      Data_Set_CoolDown_Time(PTable.CooldownGoal);
    }
    else if(Ptable_RunPara.StageGoalType==STAGEGOAL_TYPE_CALORIES)
    {
      Data_Set_WarmUP_Calories(PTable.WarmupGoal);
      Data_Set_CoolDown_Calories(PTable.CooldownGoal);
    }
    if(Ptable_RunPara.WorkoutGoalType==GOAL_TYPES_DISTANCE)
    {
      for(UINT8 i=0;DistanceTargetArray[PTable.DistanceIndex].Data>w_Dat;i++)
      {
        if(PTable.CoolDownStart !=0 )
        {
		if(PTable.CoolDownStart==i &&(Data_Get_Distance(COOLDOWN_DISTANCE)+w_Dat>DistanceTargetArray[PTable.DistanceIndex].Data))
		{
			i=PTable.RepeatStart; 
		}
		else
		{
			i=PTable.CoolDownStart; 
		}
        }
        else
        {
		if(PTable.Segamount==i)
		{
			i=PTable.RepeatStart; 
		}
        }
        w_Dat +=Ptable_RunPara.StatusStageGoal[i].StageGoal;
        PTable.WorkoutTotalSegment++;
      }
    }
    else if(Ptable_RunPara.WorkoutGoalType==GOAL_TYPES_TIME)
    {
      for(UINT8 i=0;PTable.targetTime>w_Dat;i++)
      {
        if(PTable.CoolDownStart !=0)   //zsf
        {
		    if (Data_Get_Time(COOLDOWN_TIME)+w_Dat >= PTable.targetTime)
            {
                i=PTable.CoolDownStart;
            }
            
            if(PTable.CoolDownStart==i)
            {
                if (Data_Get_Time(COOLDOWN_TIME)+w_Dat < PTable.targetTime)
                {
                    i=PTable.RepeatStart; 
                }
                else
                {
                    i=PTable.CoolDownStart; 
                }
            }
        }
        else
        {
          if(PTable.Segamount==i)
          {
            i=PTable.RepeatStart; 
          }
        }
        w_Dat +=Ptable_RunPara.StatusStageGoal[i].StageGoal;
        PTable.WorkoutTotalSegment++;
      }
    }
    else if(Ptable_RunPara.WorkoutGoalType==GOAL_TYPES_CALORIES)
    {
      for(UINT8 i=0;PTable.targetCalories>w_Dat;i++)
      {
        if(PTable.CoolDownStart !=0)
        {
		if(PTable.CoolDownStart==i &&(Data_Get_Calories(COOLDOWN_CALORIES)+w_Dat>PTable.targetCalories))
		{
			i=PTable.RepeatStart; 
		}
		else
		{
			i=PTable.CoolDownStart; 
		}
        }
        else
        {
          if(PTable.Segamount==i)
          {
            i=PTable.RepeatStart; 
          }
        }
        w_Dat +=Ptable_RunPara.StatusStageGoal[i].StageGoal;
        PTable.WorkoutTotalSegment++;
      }
    }
  }
  if(Ptable_RunPara.WorkoutRunType == FOLLOW_TABLE)
  {
      PTable.SegmentGoal = Ptable_RunPara.StatusStageGoal[PTable.Stage].StageGoal;
      Data_Set_StageTime(PTable.SegmentGoal);  
  }
  else if(Ptable_RunPara.WorkoutRunType == TOTAL_GOAL_AVERAGE)
  {
      PTable.SegmentGoal= Data_Get_Time(TARGET_TIME)/Ptable_RunPara.Segamount;
      Data_Set_StageTime(PTable.SegmentGoal);  
  }
  else if(Ptable_RunPara.WorkoutRunType == TOTALGOAL_WITH_REPEAT)
  {
	  PTable.SegmentGoal = Ptable_RunPara.StatusStageGoal[PTable.Stage].StageGoal;
      if(Ptable_RunPara.StageGoalType==STAGEGOAL_TYPE_TIME)
      {
          Data_Set_StageTime(PTable.SegmentGoal);  
      }
  }
}

void ProgramTable_Data_Initial(void)
{
    UINT16 i;
    memset(&PTable,0,sizeof(P_Table));
    
    Ptable_RunPara = *(ProgramLibConfigPara->Pro_Config[0].RunParameter);  
//WWW    for(i=0;i<ProgramLibConfigPara->ProgramTotalNum-1;i++)
         for(i=0;i<ProgramLibConfigPara->ProgramTotalNum;i++) //WWW
    {
        if(ProgramLibConfigPara->Pro_Config[i].ProgramFirstID==Program_Get_ProgramID())
        {
            Ptable_RunPara = *(ProgramLibConfigPara->Pro_Config[i].RunParameter);
            break;
        }
    }
    if(Ptable_RunPara.WorkoutGoalType==GOAL_TYPES_DISTANCE)
	{
        if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
            Pro_table.Setting_Array = TM_ProTable_DistanceSetUp;
        else
            Pro_table.Setting_Array = EP_ProTable_DistanceSetUp;
	}
    else if(Ptable_RunPara.WorkoutGoalType==GOAL_TYPES_TIME)
  	{
        Pro_table.Setting_Array=ProTable_TimeSetUp;
  	}
    else if(Ptable_RunPara.WorkoutGoalType==GOAL_TYPES_CALORIES)
  	{
        if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
            Pro_table.Setting_Array = TM_ProTable_CaloriesSetUp;
        else
            Pro_table.Setting_Array = EP_ProTable_CaloriesSetUp;
  	}
    if(Ptable_RunPara.WorkoutGoalType==GOAL_TYPES_TIME)
        PTable.targetTime=1800;
    else if(Ptable_RunPara.WorkoutGoalType==GOAL_TYPES_CALORIES)
        PTable.targetCalories=100;
    PTable.State = WARM_UP;
    Data_Set_Level(1);
    Data_Set_Level_MaxVal(Ptable_RunPara.Levelamount);
    Data_Set_Speed(Data_Get_Speed(MIN_SPEEDX10),SystemConfig_Get_Display_Unit(User_Get_UserSlot( )),0); 
    Data_Set_Resistance(SystemConfig_Get_Char(MIN_RESISTANCE_UINT8),0);
    Data_Set_Incline(0,0);
    
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
    
    if(Ptable_RunPara.WorkoutGoalType==GOAL_TYPES_DISTANCE)
	{
        Data_Set_targetDistancex1000(DistanceTargetArray[PTable.DistanceIndex].Data);
        Data_Set_Time(0);
        Data_Set_targetCalories(0);
	}
    else if(Ptable_RunPara.WorkoutGoalType==GOAL_TYPES_TIME)
  	{
        Data_Set_targetDistancex1000(0);
        Data_Set_Time(PTable.targetTime);
        Data_Set_targetCalories(0);
  	}
    else if(Ptable_RunPara.WorkoutGoalType==GOAL_TYPES_CALORIES)
  	{
        Data_Set_targetDistancex1000(0);
        Data_Set_Time(0);
        Data_Set_targetCalories(PTable.targetCalories);
  	}
    if(Ptable_RunPara.SpeedTable!=NULL)
    {
         PTable.SpeedBase=Ptable_RunPara.SpeedTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.Stage];
  
        Data_Set_Speed( PTable.SpeedBase,STANDARD,0);
    }
    if(Ptable_RunPara.InclineTable!=NULL)
    {
       PTable.InclineBase=Ptable_RunPara.InclineTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.Stage];
        Data_Set_Incline( PTable.InclineBase,0);
    }
    if(Ptable_RunPara.ResistanceTable!=NULL)
    {
      PTable.ResistanceBase=Ptable_RunPara.ResistanceTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.Stage];
      Data_Set_Resistance( PTable.ResistanceBase,0);
    }
    ProgramTable_LookUpParameter();
}
UINT16 ProgramTable_SetUpData(UINT8 ParaIndex)
{
	UINT16 w_Dat=0;
     switch(ParaIndex)
	{

		case INDEX_TIME:
			w_Dat=PTable.targetTime;
		break;
//		case INDEX_DISTANCE:
//			w_Dat=DistanceTargetArray[PTable.DistanceIndex].Data;
//		break;
//		case INDEX_CALORIES:
//			w_Dat=PTable.targetCalories;
//		break;
//		case INDEX_DISTANCE_LABLE:
//			w_Dat=DistanceTargetArray[PTable.DistanceIndex].Lable;
//		break;
        case INDEX_SPEED:
			w_Dat=Data_Get_Speed(TARGET_SPEEDX10_AUTO);
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
  if(keyevt->Bit_All == 0) return pendingEvt;
  
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
      if(PTable.targetTime<99*60)
      {
        PTable.targetTime +=60;
      }
      else
      {
        PTable.targetTime=99*60;
      }
      pendingEvt.evt=TIME_CHANGE_EVENT;
    }
    else if(keyevt->Bit_Flag.Key_Dn)
    {
      if(PTable.targetTime>15*60)
      {
        PTable.targetTime -=60;
      }
      else
      {
        PTable.targetTime=15*60;
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
     if(PTable.DistanceIndex<D_MARATHON)
      PTable.DistanceIndex++;
     else
      PTable.DistanceIndex=D_MARATHON;
      pendingEvt.evt=DISTANCE_CHANGE_EVENT;
    }
    else if(keyevt->Bit_Flag.Key_Dn)
    {
     if(PTable.DistanceIndex>D_1_MILE)
      PTable.DistanceIndex--;
     else
      PTable.DistanceIndex=D_1_MILE;
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
       if(PTable.targetCalories<980)
		PTable.targetCalories +=20;
	else
		PTable.targetCalories =980;
	   	
      pendingEvt.evt=CALORIES_CHANGE_EVENT;
    }
    else if(keyevt->Bit_Flag.Key_Dn)
    {
       if(PTable.targetCalories>20)
		PTable.targetCalories -=20;
	else
		PTable.targetCalories =20;
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
  case SET_SPEED:
      if(keyevt->Bit_Flag.Key_StateChange)
      {
          keyevt->Bit_Flag.Key_StateChange=false;
          pendingEvt.evt=SPEED_CHANGE_EVENT;
      }
      if(keyevt->Bit_Flag.Key_Up)
      {
          Data_Set_Speed(Data_Get_Speed(TARGET_SPEEDX10_AUTO)+1,SystemConfig_Get_Display_Unit(User_Get_UserSlot( ))==METRIC?METRIC:STANDARD,0);
      }
      else if(keyevt->Bit_Flag.Key_Dn)
      {
          Data_Set_Speed(Data_Get_Speed(TARGET_SPEEDX10_AUTO)-1,SystemConfig_Get_Display_Unit(User_Get_UserSlot( ))==METRIC?METRIC:STANDARD,0);
      }
      else if(keyevt->Bit_Flag.Key_QuickKey==true)
      {
          Data_Set_Speed(Data_Get_Speed(TARGET_SPEEDX10_AUTO),SystemConfig_Get_Display_Unit(User_Get_UserSlot( ))==METRIC?METRIC:STANDARD,0);
      }
      pendingEvt.evt=SPEED_CHANGE_EVENT;
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
    PTable.SpeedBase=Ptable_RunPara.SpeedTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.Stage];
    
    Data_Set_Speed( PTable.SpeedBase,STANDARD,0);
  }
  if(Ptable_RunPara.InclineTable!=NULL)
  {
    PTable.InclineBase=Ptable_RunPara.InclineTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.Stage];
      Data_Set_Incline( PTable.InclineBase,0);
  }
  if(Ptable_RunPara.ResistanceTable!=NULL)
  {
    PTable.ResistanceBase=Ptable_RunPara.ResistanceTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.Stage];
      Data_Set_Resistance( PTable.ResistanceBase,0);
  }
  
  ProgramTable_LookUpParameter();
  return pendingEvt; 
}
PROFILE *ProgramTable_ProfileShow(PROFILE_TAG tag_index,UCHAR* Dotmatrix_pos)
{
  
  UINT16 by_SEG;
  UINT8 i,k,Index;
  UINT8 segment;
  
  if(Ptable_RunPara.WorkoutRunType == TOTALGOAL_WITH_REPEAT)
  {
  
    by_SEG=PTable.WorkoutTotalSegment-1-PTable.Stage;
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
    
    if (PTable.WorkoutTotalSegment < ProgramLibConfigPara->DotMatrixLength)   //zsf
    {
        segment = PTable.WorkoutTotalSegment;
    }
    else
    {
        segment = ProgramLibConfigPara->DotMatrixLength;
    }
	  for( i = *Dotmatrix_pos,k=0; i < segment; i++,k++)
	  {
        Index=k+PTable.TableIndex;
		if(PTable.CoolDownStart !=0)
		{
#ifdef	TM	
		    if(k>(PTable.WorkoutTotalSegment-1-PTable.Stage)-2)   //zsf
            {
                if((PTable.WorkoutTotalSegment%2==1)&&(PTable.WorkoutTotalSegment-1-PTable.Stage)<2)
                    Index+=1;
                Index=((PTable.WorkoutTotalSegment-Index)%2)+PTable.CoolDownStart; ////2 is cooldown segments
            }
#else
			if(k>(PTable.WorkoutTotalSegment-1-PTable.Stage)-4)  //
            {            
                if((PTable.WorkoutTotalSegment%4==1)&&(PTable.WorkoutTotalSegment-1-PTable.Stage)<4)
                    Index+=1;
 //             Index=(4-(PTable.WorkoutTotalSegment-Index)%4)%4+PTable.CoolDownStart; ////4 is cooldown segments
                Index=k-(PTable.WorkoutTotalSegment-1-PTable.Stage)+4 - 1+PTable.CoolDownStart; ////4 is cooldown segments
          }
#endif		  
            else
            {
                if(PTable.CoolDownStart<=Index)
		        {
				    Index -=PTable.RepeatStart; 
				    Index=Index%(PTable.CoolDownStart-PTable.RepeatStart)+PTable.RepeatStart;
                }
		    }
		}
		else
		{
			if(PTable.Segamount<=Index)
			{
				Index -=PTable.RepeatStart; 
				Index=Index%(PTable.Segamount-PTable.RepeatStart)+PTable.RepeatStart;
			}
		}
        Index +=Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1);
	    if(Ptable_RunPara.SpeedTable!=NULL)
	    {
	      Prg_ProfileData.SpeedProfile[i]=Data_Get_DotmatrixSpeedData( Ptable_RunPara.SpeedTable[Index]);
	    }
	    else
	    {
	      Prg_ProfileData.SpeedProfile[i]=Data_Get_Speed(TARGET_SPEEDX10_AUTO);
	    }
	    
	    if(Ptable_RunPara.InclineTable!=NULL)
	    {
	      Prg_ProfileData.InclineProfile[i]=Data_Get_DotmatrixInclineData( Ptable_RunPara.InclineTable[Index]);
	    }
	    else
	    {
	      Prg_ProfileData.InclineProfile[i]=Data_Get_Incline(TARGET_INCLINEX10);
	    }
	    if(Ptable_RunPara.ResistanceTable!=NULL)
	    {
	      Prg_ProfileData.ResistanceProfile[i]=Data_Get_DotmatrixResistanceData( Ptable_RunPara.ResistanceTable[Index]);
	    }
	    else
	    {
	      Prg_ProfileData.ResistanceProfile[i]=Data_Get_Resistance(TARGET_RESISTANCE);
	    }
	  }
  }
  else
  {
	    by_SEG = PTable.Segamount- 1 - PTable.Stage;
	    if(PTable.Segamount<ProgramLibConfigPara->DotMatrixLength)
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
    UINT16 CooldownData;
//    UINT16 Stage_Remaining_Time;
    static UINT16 LastValue=0xffff;
    
    PROGRAM_PENDING_EVTS pendingEvt;
    pendingEvt.evt=PROGRAM_NONE_EVENT;
    memset(&pendingEvt.WorkOutEvts,0,sizeof(pendingEvt.WorkOutEvts));
    if(Pro_table.Prg_CurrentStatus()<PS_WARMUP||Pro_table.Prg_CurrentStatus()>PS_COOLDOWN)return pendingEvt;
    
    if(Ptable_RunPara.WorkoutGoalType==GOAL_TYPES_DISTANCE)
    {
        TargetData=Data_Get_Distance(TARGET_DISTANCE);
        ElapsedData=Data_Get_Distance(ELAPSED_DISTANCE);
        CooldownData=	Data_Get_Distance(COOLDOWN_DISTANCE);
    }
    else if(Ptable_RunPara.WorkoutGoalType==GOAL_TYPES_TIME)
    {
        TargetData=Data_Get_Time(TARGET_TIME);
        ElapsedData=Data_Get_Time(ELAPSED_TIME);
        CooldownData=	Data_Get_Time(COOLDOWN_TIME);
    }
    else if(Ptable_RunPara.WorkoutGoalType==GOAL_TYPES_CALORIES)
    {
        TargetData=Data_Get_Calories(TARGET_CALORIES);
        ElapsedData=Data_Get_Calories(ELAPSED_CALORIES);
        CooldownData=Data_Get_Calories(COOLDOWN_CALORIES);
    }
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
//                    pendingEvt.WorkOutEvts.Workout_Warmup_Evt=true;
                }
                if(DiffData >= PTable.SegmentGoal)
                {
                    PTable.LastPoint = ElapsedData;
                    PTable.Stage++;
                    Beep_Set_Beeps(1,3,1);            
                    if(PTable.LastPoint >= PTable.WarmupGoal)
                    {
                        if(TargetData - PTable.LastPoint > PTable.CooldownGoal)
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
//                                pendingEvt.WorkOutEvts.Workout_Cooldown_Evt=true;
                                PTable.Stage = PTable.CoolDownStart;
                            }
                        }
                    }	
                    PTable.SegmentGoal =  Ptable_RunPara.StatusStageGoal[Ptable_RunPara.Segamount+PTable.Stage].StageGoal;
                    Data_Set_StageTime(PTable.SegmentGoal);
                    if(Ptable_RunPara.SpeedTable!=NULL)
                    {
                      if(PTable.SpeedBase != Ptable_RunPara.SpeedTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.Stage])
                      {
                        PTable.SpeedBase = Ptable_RunPara.SpeedTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.Stage];
                          Data_Set_Speed( PTable.SpeedBase,STANDARD,0);
                        Beep_Set_Beeps(1,3,1);
                      }
                    }
                    if(Ptable_RunPara.InclineTable!=NULL)
                    {
                      if(PTable.InclineBase != Ptable_RunPara.InclineTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.Stage])
                      {
                        PTable.InclineBase = Ptable_RunPara.InclineTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.Stage];
                          Data_Set_Incline( PTable.InclineBase,0);
                        Beep_Set_Beeps(1,3,1);
                      }
                    }
                    if(Ptable_RunPara.ResistanceTable!=NULL)
                    {
                      if(PTable.ResistanceBase != Ptable_RunPara.ResistanceTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.Stage])
                      {
                        PTable.ResistanceBase = Ptable_RunPara.ResistanceTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.Stage];
                          Data_Set_Resistance( PTable.ResistanceBase,0);
                        Beep_Set_Beeps(1,3,1);
                      }                      
                    }                      
                }	
                if((PTable.State != COOL_DOWN) & ((TargetData - PTable.LastPoint - PTable.SegmentGoal) < PTable.CooldownGoal))
                {
                    PTable.SegmentGoal = TargetData - PTable.LastPoint;
                    PTable.SegmentGoal -= PTable.CooldownGoal;
                }
                if(PTable.LastPoint >= PTable.WarmupGoal && PTable.State != COOL_DOWN)
                {
                    PTable.State = REPEAT;
                    Pro_table.Prg_NextStatus(PS_RUN);
                    pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
                }
            }
            else if(Ptable_RunPara.WorkoutRunType == TOTAL_GOAL_AVERAGE)
            {
                if(DiffData >= PTable.SegmentGoal)
                {
                    PTable.LastPoint = ElapsedData;
                    PTable.Stage++;
                    
                    if(PTable.Stage<Ptable_RunPara.Segamount-1)
                        PTable.SegmentGoal= TargetData/Ptable_RunPara.Segamount;
                    else
                        PTable.SegmentGoal=TargetData%PTable.SegmentGoal+PTable.SegmentGoal;
                    Data_Set_StageTime(PTable.SegmentGoal);
                    if(Ptable_RunPara.SpeedTable!=NULL)
                    {
                        if(PTable.SpeedBase != Ptable_RunPara.SpeedTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.Stage])
                        {
                          PTable.SpeedBase = Ptable_RunPara.SpeedTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.Stage];
                            Data_Set_Speed( PTable.SpeedBase,STANDARD,0);
                            Beep_Set_Beeps(1,3,1);
                        }
                    }
                    if(Ptable_RunPara.InclineTable!=NULL)
                    {
                        if(PTable.InclineBase != Ptable_RunPara.InclineTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.Stage])
                        {
                          PTable.InclineBase = Ptable_RunPara.InclineTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.Stage];
                            Data_Set_Incline( PTable.InclineBase,0);
                            Beep_Set_Beeps(1,3,1);
                        }
                    }
                    if(Ptable_RunPara.ResistanceTable!=NULL)
                    {
                      if(PTable.ResistanceBase != Ptable_RunPara.ResistanceTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.Stage])
                      {
                        PTable.ResistanceBase = Ptable_RunPara.ResistanceTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.Stage];
                          Data_Set_Resistance( PTable.ResistanceBase,0);
                        Beep_Set_Beeps(1,3,1);
                      }                      
                    }  
                }
            }
            else if(Ptable_RunPara.WorkoutRunType == TOTALGOAL_WITH_REPEAT)
            {
                if(ElapsedData==0)
                {
                    PTable.State = WARM_UP;
                    Pro_table.Prg_NextStatus(PS_WARMUP);	  
                    pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
//                    pendingEvt.WorkOutEvts.Workout_Warmup_Evt=true;
                }
                if(DiffData >= PTable.SegmentGoal)
                {
                    PTable.LastPoint = ElapsedData;
                    PTable.Stage++;
                    PTable.TableIndex++;
//                    Beep_Set_Beeps(1,3,1);
                    if(PTable.CoolDownStart !=0 )
                    {
//            if(PTable.CoolDownStart==PTable.TableIndex&&Remaining_Data>CooldownData)
//            {
//              PTable.TableIndex=PTable.RepeatStart; 
//            }
//            else
//            {
//              PTable.TableIndex=PTable.CoolDownStart; 
//            }
            //WWW
            
       {
            if(PTable.LastPoint >= PTable.WarmupGoal && PTable.State != COOL_DOWN)
            {
              PTable.State = REPEAT;
              Pro_table.Prg_NextStatus(PS_RUN);
              pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
            }
            if(Data_Get_Time(COOLDOWN_TIME)==Remaining_Data)
            {
              PTable.State = COOL_DOWN;
		pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
          pendingEvt.WorkOutEvts.Workout_Cooldown_Evt=true;	  
              PTable.TableIndex=PTable.CoolDownStart; 
              PTable.SegmentGoal = Ptable_RunPara.StatusStageGoal[PTable.TableIndex].StageGoal; 
            }
            else if(PTable.CoolDownStart==PTable.TableIndex && Data_Get_Time(COOLDOWN_TIME)<Remaining_Data) //if()
            {
              PTable.TableIndex=PTable.RepeatStart;
          
                if(Remaining_Data-Data_Get_Time(COOLDOWN_TIME)<Ptable_RunPara.StatusStageGoal[PTable.TableIndex].StageGoal)    //zsf
                {
                  PTable.SegmentGoal =Remaining_Data-Data_Get_Time(COOLDOWN_TIME);    //zsf
                }
                else
                PTable.SegmentGoal = Ptable_RunPara.StatusStageGoal[PTable.TableIndex].StageGoal; 
              
            }
            else
            PTable.SegmentGoal = Ptable_RunPara.StatusStageGoal[PTable.TableIndex].StageGoal; 
          }
          
            
          }
          else
          {
            if(PTable.Segamount==PTable.TableIndex)
            {
              PTable.TableIndex=PTable.RepeatStart; 
            }
            PTable.SegmentGoal = Ptable_RunPara.StatusStageGoal[PTable.TableIndex].StageGoal; //WWW
          }
          
 //WWW         PTable.SegmentGoal = Ptable_RunPara.StatusStageGoal[PTable.TableIndex].StageGoal;  

                    if(Ptable_RunPara.StageGoalType==STAGEGOAL_TYPE_TIME)
                    {
                        Data_Set_StageTime(PTable.SegmentGoal);  
                    }
                    if(Ptable_RunPara.SpeedTable!=NULL)
                    {
                      if(PTable.SpeedBase != Ptable_RunPara.SpeedTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.TableIndex])
                        {
                         PTable.SpeedBase = Ptable_RunPara.SpeedTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.TableIndex];
                           Data_Set_Speed( PTable.SpeedBase,STANDARD,0);
                            Beep_Set_Beeps(1,3,1);
                        }
                    }
                    if(Ptable_RunPara.InclineTable!=NULL)
                    {
                     
                      if(PTable.InclineBase != Ptable_RunPara.InclineTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.TableIndex])
                      {
                        PTable.InclineBase = Ptable_RunPara.InclineTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.TableIndex];
                        Data_Set_Incline( PTable.InclineBase,0);
                        Beep_Set_Beeps(1,3,1);
                      }
                    }
                    if(Ptable_RunPara.ResistanceTable!=NULL)
                    {
                      if(PTable.ResistanceBase != Ptable_RunPara.ResistanceTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.TableIndex])
                      {
                        PTable.ResistanceBase = Ptable_RunPara.ResistanceTable[Ptable_RunPara.Segamount*(Data_Get_Level(TARGET_LEVEL)-1)+PTable.TableIndex];
                        Data_Set_Resistance( PTable.ResistanceBase,0);
                        Beep_Set_Beeps(1,3,1);
                      }
                    }  
                }
            }
        } 
    }
    
    if(keyevt->Bit_Flag.Key_SpeedUp)
    {
        keyevt->Bit_Flag.Key_SpeedUp=false;
        Data_Set_Speed(Data_Get_Speed(TARGET_SPEEDX10_AUTO)+1,SystemConfig_Get_Display_Unit(User_Get_UserSlot( ))==METRIC?METRIC:STANDARD,1);
    }
    else if(keyevt->Bit_Flag.Key_SpeedDn)
    {
        keyevt->Bit_Flag.Key_SpeedDn=false;
        Data_Set_Speed(Data_Get_Speed(TARGET_SPEEDX10_AUTO)-1,SystemConfig_Get_Display_Unit(User_Get_UserSlot( ))==METRIC?METRIC:STANDARD,1);
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
void Pro_table_Set_Data(UINT16 w_Data)
{
    switch(Program_Get_ProgramID())
    {
        case PS_DISTANCE_GOAL:
        {
            switch(w_Data)
            {
                case 1000:
                PTable.DistanceIndex = D_1_MILE;
                break;
                case 2000:
                PTable.DistanceIndex = D_2_MILES;
                break;                
                case 3125:
                PTable.DistanceIndex = D_5_K;
                break;    
                case 5000:
                PTable.DistanceIndex = D_5_MILES;
                break;
                case 6250:
                PTable.DistanceIndex = D_10_K;
                break; 
                case 8000:
                PTable.DistanceIndex = D_8_MILES;
                break; 
                case 9375:
                PTable.DistanceIndex = D_15_K;
                break; 
                case 10000:
                PTable.DistanceIndex = D_10_MILES;
                break;
                case 12500:
                PTable.DistanceIndex = D_20_K;
                break; 
                case 13100:
                PTable.DistanceIndex = D_HALF_MARATHON;
                break; 
                case 15000:
                PTable.DistanceIndex = D_15_MILES;
                break;
                case 20000:
                PTable.DistanceIndex = D_20_MILES;
                break; 
                case 26200:
                PTable.DistanceIndex = D_MARATHON;
                break;   
            }
        }
        break;
        case PS_FATBURN:
        case PS_HILL_CLIMB:
        PTable.targetTime = w_Data;
        break;

        case PS_CALORIES_GOAL:
        PTable.targetCalories = w_Data;
        break;
    }
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
  Pro_table.Setting_Array=ProTable_TimeSetUp; 
  Pro_table.SettingArray_Index=0;
  
  Pro_table.Programid = PS_WEIGHT_LOSS;
  Program_Register_Function(&Pro_table);
  Pro_table.Programid = PS_INTERVALS;
  Program_Register_Function(&Pro_table);
  
//  Pro_table.Programid = PS_DISTANCE_GOAL;
//  Program_Register_Function(&Pro_table);
//  Pro_table.Programid = PS_CALORIES_GOAL;
//  Program_Register_Function(&Pro_table);
//  Pro_table.Programid = PS_ROLLING_HILLS;
//  Program_Register_Function(&Pro_table);
//  Pro_table.Programid = PS_FATBURN;
//  Program_Register_Function(&Pro_table);
//  Pro_table.Programid = PS_HILL_CLIMB;
//  Program_Register_Function(&Pro_table);
  
}





#endif

