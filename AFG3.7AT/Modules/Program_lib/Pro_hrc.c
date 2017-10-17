#include"Pro_hrc.h"

#ifdef __PROGRAM_HRC__

#define HRC_WARMUP_TIME 60*4
#define HRC_COOLDOWN_TIME 60*4

#define CHECK_TIME_2S 2	
#define CHECK_TIME_3S 3	
#define CHECK_TIME_5S 5	
#define CHECK_TIME_8S 8	

#define CHECK_TIME_10S 10	
#define CHECK_TIME_35S 35	

#define THR_TOTAL_SEGMENT 14

__no_init static PROFILE Profile;

typedef struct
{
  UINT8 TargetHeart;
}THR;

typedef struct
{
  UINT16 IntervalTime;
  UINT16 RestIntervalTime;
  UINT8 IntervalHeart;
  UINT8 RestIntervalHeart;
}INTERVALS_HR;
typedef struct
{
  UINT16 SegmentTime;
  UINT8 Segment1Heart;
  UINT8 Segment2Heart;
  UINT8 Segment3Heart;
  UINT8 Segment4Heart;
  
}STEPS_HR;
typedef union
{
  THR thr;
  INTERVALS_HR IntervalsHR;
  STEPS_HR StepsHR;
}HRPro_Parameter;
typedef struct
{
  HRPro_Parameter HRProParameter;
  INT16 HeartDiffForSpeedAdjust;
  INT16 HeartDiffForInclineAdjust;
  INT16 HeartDiffForResistanceAdjust;
  UINT16 StageTime;
  UINT16 WorkoutRemainingTime;
  UINT16 WorkoutTargetTime;
  UINT8 WorkoutTotalSegment;
  UINT8 Max_Heart;
  UINT8 Stage;
  UINT8 NoHRSignal60s;
  UINT8 SlowDownTimeCounter;
  UINT8 HeartStatusForSpeedAdjust;
  UINT8 HeartStatusForInclineAdjust;
  UINT8 HeartStatusForResistanceAdjust;
  UINT8 SpeedAdjustTimeCount;
  UINT8 SpeedAdjustLimitedTime;
  UINT8 InclineAdjustTimeCount;
  UINT8 InclineAdjustLimitedTime;
  UINT8 ResistanceAdjustTimeCount;
  UINT8 ResistanceAdjustLimitedTime;
  UINT8 HeartMore25;
  UINT8 SettingState;
  UINT8 RemainSegment;

}Hrc_Data;
typedef struct
{
  struProgram ProHrc;
  Hrc_Data HrcData;
}Hrc;
__no_init static Hrc g_Hrc;

void PHrc_Speed_Adjust(void)
{
  if(g_Hrc.HrcData.SpeedAdjustTimeCount<100)g_Hrc.HrcData.SpeedAdjustTimeCount++;
  g_Hrc.HrcData.HeartDiffForSpeedAdjust=Data_Get_Heart(CURRENT_HEART)-Data_Get_Heart(TARGET_HEART);
    if(g_Hrc.HrcData.HeartDiffForSpeedAdjust>=16)
    {
      g_Hrc.HrcData.SpeedAdjustLimitedTime=CHECK_TIME_2S;
    }
    else if(g_Hrc.HrcData.HeartDiffForSpeedAdjust>=12 || g_Hrc.HrcData.HeartDiffForSpeedAdjust<=-12)
    {
      g_Hrc.HrcData.SpeedAdjustLimitedTime=CHECK_TIME_3S;
    }
    else if(g_Hrc.HrcData.HeartDiffForSpeedAdjust>=7 || g_Hrc.HrcData.HeartDiffForSpeedAdjust<=-7)
    {
      g_Hrc.HrcData.SpeedAdjustLimitedTime=CHECK_TIME_5S;
    }
    else
    {
      g_Hrc.HrcData.SpeedAdjustLimitedTime=CHECK_TIME_8S;
    }  
  if(g_Hrc.HrcData.SpeedAdjustLimitedTime>g_Hrc.HrcData.SpeedAdjustTimeCount)
  {
    return;
  }
  g_Hrc.HrcData.SpeedAdjustTimeCount=0;
    if(g_Hrc.HrcData.HeartDiffForSpeedAdjust>=25)
    {
      g_Hrc.HrcData.HeartStatusForSpeedAdjust=HC_MORE_25_SPD;
    }
    else if(g_Hrc.HrcData.HeartDiffForSpeedAdjust>=16)
    {
      g_Hrc.HrcData.HeartStatusForSpeedAdjust=HC_MORE_16_24_SPD;
    }
    else if(g_Hrc.HrcData.HeartDiffForSpeedAdjust>=12)
    {
      g_Hrc.HrcData.HeartStatusForSpeedAdjust=HC_MORE_12_15_SPD;
    }
    else if(g_Hrc.HrcData.HeartDiffForSpeedAdjust>=7)
    {
      g_Hrc.HrcData.HeartStatusForSpeedAdjust=HC_MORE_7_11_SPD;
    }
    else if(g_Hrc.HrcData.HeartDiffForSpeedAdjust>=4)
    {
      g_Hrc.HrcData.HeartStatusForSpeedAdjust=HC_MORE_4_6_SPD;
    }
   else if(g_Hrc.HrcData.HeartDiffForSpeedAdjust<=-12)
    {
      g_Hrc.HrcData.HeartStatusForSpeedAdjust=HC_LESS_12_SPD;
    }
    else if(g_Hrc.HrcData.HeartDiffForSpeedAdjust<=-7)
    {
      g_Hrc.HrcData.HeartStatusForSpeedAdjust=HC_LESS_7_11_SPD;
    }
    else if(g_Hrc.HrcData.HeartDiffForSpeedAdjust<=-4)
    {
      g_Hrc.HrcData.HeartStatusForSpeedAdjust=HC_LESS_4_6_SPD;
    }
    else
    {
      g_Hrc.HrcData.HeartStatusForSpeedAdjust=HC_NONE_SPD;
    }    
  switch(g_Hrc.HrcData.HeartStatusForSpeedAdjust)
  {
  case HC_NONE_SPD:
    break;
  case HC_LESS_4_6_SPD:
  case HC_LESS_7_11_SPD:
  case HC_LESS_12_SPD:
    if(Data_Get_Speed(TARGET_SPEEDX10_AUTO)<Data_Get_Speed(MAX_SPEEDX10))
    {
        Beep_Set_Beeps(1,3,1);
		Data_Set_Speed_Base_User(Data_Get_Speed(TARGET_SPEEDX10_AUTO)+1,1);
        //Data_Set_Speed(Data_Get_Speed(TARGET_SPEEDX10_AUTO)+1,SystemConfig_Get_Display_Unit(User_Get_UserSlot( )),1);
    }
    break;
  case HC_MORE_4_6_SPD:
  case HC_MORE_7_11_SPD:
  case HC_MORE_12_15_SPD:
  case HC_MORE_16_24_SPD:
    if(Data_Get_Speed(TARGET_SPEEDX10_AUTO)>Data_Get_Speed(MIN_SPEEDX10))
    {
        Beep_Set_Beeps(1,3,1);
		Data_Set_Speed_Base_User(Data_Get_Speed(TARGET_SPEEDX10_AUTO)-1,1);
       // Data_Set_Speed(Data_Get_Speed(TARGET_SPEEDX10_AUTO)-1,SystemConfig_Get_Display_Unit(User_Get_UserSlot( )),1);
    }
    break;
  case HC_MORE_25_SPD:
  	Data_Set_Speed_Base_User(0,1);
    //Data_Set_Speed(0,SystemConfig_Get_Display_Unit(User_Get_UserSlot( )),1);
    Program_Set_VirtualKey((UINT16)VK_RESET | FRAME_STAGE_PRGRUN);
    break;
    
  }
}
void PHrc_Incline_Adjust(void)
{
  if(g_Hrc.HrcData.InclineAdjustTimeCount<100)g_Hrc.HrcData.InclineAdjustTimeCount++;
  g_Hrc.HrcData.HeartDiffForInclineAdjust=Data_Get_Heart(CURRENT_HEART)-Data_Get_Heart(TARGET_HEART);
    if(g_Hrc.HrcData.HeartDiffForInclineAdjust>=11 || g_Hrc.HrcData.HeartDiffForInclineAdjust<=-20)
    {
      g_Hrc.HrcData.InclineAdjustLimitedTime=CHECK_TIME_10S;
    }
    else
    {
      g_Hrc.HrcData.InclineAdjustLimitedTime=CHECK_TIME_35S;
    }  
  if(g_Hrc.HrcData.InclineAdjustLimitedTime>g_Hrc.HrcData.InclineAdjustTimeCount)
  {
    return;
  }
  g_Hrc.HrcData.InclineAdjustTimeCount=0;

    if(g_Hrc.HrcData.HeartDiffForInclineAdjust>=25)
    {
      g_Hrc.HrcData.HeartStatusForInclineAdjust=HC_MORE_25_INC;
    }
    else if(g_Hrc.HrcData.HeartDiffForInclineAdjust>=20)
    {
      g_Hrc.HrcData.HeartStatusForInclineAdjust=HC_MORE_20_24_INC;
    }
    else if(g_Hrc.HrcData.HeartDiffForInclineAdjust>=11)
    {
      g_Hrc.HrcData.HeartStatusForInclineAdjust=HC_MORE_11_19_INC;
    }
    else if(g_Hrc.HrcData.HeartDiffForInclineAdjust>=6)
    {
      g_Hrc.HrcData.HeartStatusForInclineAdjust=HC_MORE_6_10_INC;
    }
    else if(g_Hrc.HrcData.HeartDiffForInclineAdjust<=-20)
    {
      g_Hrc.HrcData.HeartStatusForInclineAdjust=HC_LESS_20_INC;
    }
    else if(g_Hrc.HrcData.HeartDiffForInclineAdjust<=-6)
    {
      g_Hrc.HrcData.HeartStatusForInclineAdjust=HC_LESS_6_19_INC;
    }
    else
    {
      g_Hrc.HrcData.HeartStatusForInclineAdjust=HC_NONE_INC;
    }   
  switch(g_Hrc.HrcData.HeartStatusForInclineAdjust)
  {
  case HC_NONE_INC:
    break;
  case HC_LESS_6_19_INC:
  case HC_LESS_20_INC:
    if(Data_Get_Incline(TARGET_INCLINEX10)<Data_Get_Incline(MAX_INCLINEX10))
    {
        Data_Set_Incline(Data_Get_Incline(TARGET_INCLINEX10)+5,1);
        Beep_Set_Beeps(1,3,1);
    }
    break;
  case HC_MORE_6_10_INC:
  case HC_MORE_11_19_INC:
//  case HC_MORE_20_24_INC:
    if(Data_Get_Incline(TARGET_INCLINEX10)) 
    {
        Beep_Set_Beeps(1,3,1);
        Data_Set_Incline(Data_Get_Incline(TARGET_INCLINEX10)-5,1);
    }
    break;
  case HC_MORE_20_24_INC:
    if(Data_Get_Incline(TARGET_INCLINEX10)) 
    {
        Beep_Set_Beeps(1,3,1);
        Data_Set_Incline(Data_Get_Incline(TARGET_INCLINEX10)/2,1);
    }
    break;
  case HC_MORE_25_INC:
      Beep_Set_Beeps(1,3,1);
    Data_Set_Incline(0,1);
    g_Hrc.HrcData.HeartMore25=1;
    break;
  } 
 
}
void PHrc_Resistance_Adjust(void)
{
  if(g_Hrc.HrcData.ResistanceAdjustTimeCount<100)g_Hrc.HrcData.ResistanceAdjustTimeCount++;
  g_Hrc.HrcData.HeartDiffForResistanceAdjust=Data_Get_Heart(CURRENT_HEART)-Data_Get_Heart(TARGET_HEART);
     if(g_Hrc.HrcData.HeartDiffForResistanceAdjust>=11 || g_Hrc.HrcData.HeartDiffForResistanceAdjust<=-20)
    {
      g_Hrc.HrcData.ResistanceAdjustLimitedTime=CHECK_TIME_10S;
    }
    else
    {
      g_Hrc.HrcData.ResistanceAdjustLimitedTime=CHECK_TIME_35S;
    }
  if(g_Hrc.HrcData.ResistanceAdjustLimitedTime>g_Hrc.HrcData.ResistanceAdjustTimeCount)
  {
    return;
  }
  g_Hrc.HrcData.ResistanceAdjustTimeCount=0;

    if(g_Hrc.HrcData.HeartDiffForResistanceAdjust>=25)
    {
      g_Hrc.HrcData.HeartStatusForResistanceAdjust=HC_MORE_25_RES;
    }
    else if(g_Hrc.HrcData.HeartDiffForResistanceAdjust>=20)
    {
      g_Hrc.HrcData.HeartStatusForResistanceAdjust=HC_MORE_20_24_RES;
    }
    else if(g_Hrc.HrcData.HeartDiffForResistanceAdjust>=11)
    {
      g_Hrc.HrcData.HeartStatusForResistanceAdjust=HC_MORE_11_19_RES;
    }
    else if(g_Hrc.HrcData.HeartDiffForResistanceAdjust>=6)
    {
      g_Hrc.HrcData.HeartStatusForResistanceAdjust=HC_MORE_6_10_RES;
    }
    else if(g_Hrc.HrcData.HeartDiffForResistanceAdjust<=-20)
    {
      g_Hrc.HrcData.HeartStatusForResistanceAdjust=HC_LESS_20_RES;
    }
    else if(g_Hrc.HrcData.HeartDiffForResistanceAdjust<=-6)
    {
      g_Hrc.HrcData.HeartStatusForResistanceAdjust=HC_LESS_6_19_RES;
    }
    else
    {
      g_Hrc.HrcData.HeartStatusForResistanceAdjust=HC_NONE_RES;
    }    
  switch(g_Hrc.HrcData.HeartStatusForResistanceAdjust)
  {
  case HC_NONE_RES:
    break;
  case HC_LESS_6_19_RES:
  case HC_LESS_20_RES:
      if(Data_Get_Resistance(TARGET_RESISTANCE)<Data_Get_Resistance(MAX_RESISTANCE))
      {    
          Beep_Set_Beeps(1,3,1);
          Data_Set_Resistance(Data_Get_Resistance(TARGET_RESISTANCE)+1,1);
      }
      break;
  case HC_MORE_6_10_RES:
  case HC_MORE_11_19_RES:
//  case HC_MORE_20_24_RES:
    if(Data_Get_Resistance(TARGET_RESISTANCE)>Data_Get_Resistance(MIN_RESISTANCE))
    {
        Beep_Set_Beeps(1,3,1);
        Data_Set_Resistance(Data_Get_Resistance(TARGET_RESISTANCE)-1,1);
    }
    break;
    case HC_MORE_20_24_RES:
    if(Data_Get_Resistance(TARGET_RESISTANCE)>Data_Get_Resistance(MIN_RESISTANCE))
    {
        Beep_Set_Beeps(1,3,1);
        Data_Set_Resistance(Data_Get_Resistance(TARGET_RESISTANCE)/2,1);
    }
    break;
  case HC_MORE_25_RES:
    Data_Set_Resistance(0,1);
    g_Hrc.HrcData.HeartMore25=1;
    break;
  } 
}
void PHrc_Get_HRTarget(void)
{
  UINT8 by_Dat;
  switch(Program_Get_ProgramID())
  {
  case PS_THR_ZONE:
    Data_Set_TargetHeart(g_Hrc.HrcData.HRProParameter.thr.TargetHeart);
    break;
  case PS_INTERVALS_HR:
    by_Dat=(g_Hrc.HrcData.Stage-1)%2;
    switch(by_Dat)
    {
    case 0:
      Data_Set_TargetHeart(g_Hrc.HrcData.HRProParameter.IntervalsHR.IntervalHeart);
      break;
    case 1:
      Data_Set_TargetHeart(g_Hrc.HrcData.HRProParameter.IntervalsHR.RestIntervalHeart);
      break;
    }
    break;
  case PS_STEPS_THR:
    switch(by_Dat)
    {
    case 0:
      Data_Set_TargetHeart(g_Hrc.HrcData.HRProParameter.StepsHR.Segment1Heart);
      break;
    case 1:
      Data_Set_TargetHeart(g_Hrc.HrcData.HRProParameter.StepsHR.Segment2Heart);
      break;
    case 2:
      Data_Set_TargetHeart(g_Hrc.HrcData.HRProParameter.StepsHR.Segment3Heart);
      break;
    case 3:
      Data_Set_TargetHeart(g_Hrc.HrcData.HRProParameter.StepsHR.Segment4Heart);
      break;
    }   
    break;
  }
}
void PHrc_Cul_Data(void)
{
  if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
  {
      if(Data_Get_Heart(CURRENT_HEART)==0)
      {
          if(g_Hrc.HrcData.NoHRSignal60s<60)
          {
              g_Hrc.HrcData.NoHRSignal60s++;
              g_Hrc.HrcData.SlowDownTimeCounter=9;
          }
          else
          {
              if(++g_Hrc.HrcData.SlowDownTimeCounter>9)
              {
                  g_Hrc.HrcData.SlowDownTimeCounter=0;
                  if(Data_Get_Speed(TARGET_SPEEDX10_AUTO)>10+Data_Get_Speed(MIN_SPEEDX10))
                  {
                      Data_Set_Speed_Base_User(Data_Get_Speed(TARGET_SPEEDX10_AUTO)-10,1);
                      //Data_Set_Speed(Data_Get_Speed(TARGET_SPEEDX10_KPH)-10,METRIC,1);
                      Beep_Set_Beeps(1,3,1);
                  }
                  else if(Data_Get_Speed(TARGET_SPEEDX10_AUTO) > Data_Get_Speed(MIN_SPEEDX10))
                  {
                      Beep_Set_Beeps(1,3,1);
					  Data_Set_Speed_Base_User(Data_Get_Speed(MIN_SPEEDX10),1);
                      //Data_Set_Speed(Data_Get_Speed(MIN_SPEEDX10),SystemConfig_Get_Display_Unit(User_Get_UserSlot( )),1);
                  }
              }
          }
          return;
      }
      else
      {
          g_Hrc.HrcData.NoHRSignal60s=0;
      }
  }
  if(g_Hrc.ProHrc.Prg_CurrentStatus()<=PS_WARMUP||g_Hrc.ProHrc.Prg_CurrentStatus()>=PS_COOLDOWN)
    return;
  PHrc_Get_HRTarget();
  if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
    //PHrc_Speed_Adjust();
      PHrc_Incline_Adjust();
  else
    if(Data_Get_Heart(CURRENT_HEART))
      PHrc_Resistance_Adjust();
}







void ProgramHrc_Data_Initial(void)
{
  UINT16 w_Dat;
  UINT16 SegTime;
  memset(&g_Hrc.HrcData,0,sizeof(Hrc_Data));
  g_Hrc.HrcData.Max_Heart=190;
  g_Hrc.HrcData.HeartStatusForSpeedAdjust=HC_NONE_SPD;
  g_Hrc.HrcData.HeartStatusForInclineAdjust=HC_NONE_INC;
  g_Hrc.HrcData.HeartStatusForResistanceAdjust=HC_NONE_RES;
  g_Hrc.HrcData.WorkoutTargetTime=30*60;
  g_Hrc.HrcData.RemainSegment=ProgramLibConfigPara->DotMatrixLength-3;
  Data_Set_WarmUP_Time(HRC_WARMUP_TIME);
  Data_Set_CoolDown_Time(HRC_COOLDOWN_TIME);
  Data_Set_Time(g_Hrc.HrcData.WorkoutTargetTime);
   Data_Set_Speed_Base_User(Data_Get_Speed(MIN_SPEEDX10),0);
  //Data_Set_Speed(Data_Get_Speed(MIN_SPEEDX10),SystemConfig_Get_Display_Unit(User_Get_UserSlot( )),0); 
  Data_Set_Resistance(SystemConfig_Get_Char(MIN_RESISTANCE_UINT8),0);
  Data_Set_Incline(0,0);      
  if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
    ProfileIndex=DefaultDotmatrix=PROFILE_TYPE_SPEED;
  else
    ProfileIndex=DefaultDotmatrix=PROFILE_TYPE_RESISTANCE;
  switch(Program_Get_ProgramID())
  {
  case PS_THR_ZONE:
    g_Hrc.ProHrc.Setting_Array=THR_ZONE_SetUp;
    w_Dat=Data_Get_Time(TARGET_TIME)-  Data_Get_Time(WARMUP_TIME)-Data_Get_Time(COOLDOWN_TIME);
    g_Hrc.HrcData.HRProParameter.thr.TargetHeart=80;
    Data_Set_TargetHeart(g_Hrc.HrcData.HRProParameter.thr.TargetHeart);
    g_Hrc.HrcData.WorkoutTotalSegment=ProgramLibConfigPara->DotMatrixLength;
    g_Hrc.HrcData.StageTime=w_Dat/g_Hrc.HrcData.RemainSegment;
    g_Hrc.HrcData.WorkoutRemainingTime=w_Dat%g_Hrc.HrcData.RemainSegment;
    break;
  case PS_INTERVALS_HR:
    g_Hrc.ProHrc.Setting_Array=Interval_HR_SetUp;	
    g_Hrc.HrcData.HRProParameter.IntervalsHR.IntervalHeart=g_Hrc.HrcData.HRProParameter.IntervalsHR.RestIntervalHeart=80;
    Data_Set_TargetHeart(g_Hrc.HrcData.HRProParameter.IntervalsHR.IntervalHeart);
    w_Dat=Data_Get_Time(TARGET_TIME)-Data_Get_Time(WARMUP_TIME)-Data_Get_Time(COOLDOWN_TIME);
    SegTime=g_Hrc.HrcData.HRProParameter.IntervalsHR.IntervalTime+g_Hrc.HrcData.HRProParameter.IntervalsHR.RestIntervalTime;
    g_Hrc.HrcData.WorkoutTotalSegment=w_Dat/SegTime;
    g_Hrc.HrcData.WorkoutTotalSegment *=2;
    w_Dat %=SegTime;
    if(w_Dat>g_Hrc.HrcData.HRProParameter.IntervalsHR.IntervalTime)
      g_Hrc.HrcData.WorkoutTotalSegment +=2;
    else
      g_Hrc.HrcData.WorkoutTotalSegment +=1;
    g_Hrc.HrcData.WorkoutTotalSegment +=3;
    break;
  case PS_STEPS_THR:
    g_Hrc.ProHrc.Setting_Array=Steps_HR_SetUp;	
    g_Hrc.HrcData.HRProParameter.StepsHR.Segment1Heart=80;
    g_Hrc.HrcData.HRProParameter.StepsHR.Segment2Heart=80;
    g_Hrc.HrcData.HRProParameter.StepsHR.Segment3Heart=80;
    g_Hrc.HrcData.HRProParameter.StepsHR.Segment4Heart=80;
    Data_Set_TargetHeart(g_Hrc.HrcData.HRProParameter.StepsHR.Segment1Heart);
    
    w_Dat=Data_Get_Time(TARGET_TIME)-Data_Get_Time(WARMUP_TIME)-Data_Get_Time(COOLDOWN_TIME);
    SegTime=g_Hrc.HrcData.HRProParameter.StepsHR.SegmentTime;
    g_Hrc.HrcData.WorkoutTotalSegment =w_Dat/SegTime;
    w_Dat %=SegTime;
    if(w_Dat>g_Hrc.HrcData.HRProParameter.StepsHR.SegmentTime)
    {
      g_Hrc.HrcData.WorkoutTotalSegment +=1;
    }
    g_Hrc.HrcData.WorkoutTotalSegment +=3;
    break;
  }	
  Data_Set_StageTime(HRC_WARMUP_TIME);
}
void ProgramHrc_BeginWorkout(void)
{
  switch(Program_Get_ProgramID())
  {
  case PS_THR_ZONE:
  	Data_Set_TargetHeart(g_Hrc.HrcData.HRProParameter.thr.TargetHeart);
    break;
  case PS_INTERVALS_HR:
	Data_Set_TargetHeart(g_Hrc.HrcData.HRProParameter.IntervalsHR.IntervalHeart);
    break;
  case PS_STEPS_THR:
    Data_Set_TargetHeart(g_Hrc.HrcData.HRProParameter.StepsHR.Segment1Heart);
    break;
  }
    Data_Set_Time(g_Hrc.HrcData.WorkoutTargetTime);

  }
UINT16 ProgramHrc_SetUpData(UINT8 ParaIndex)
{
	UINT16 w_Dat=0;
     switch(ParaIndex)
	{
		case INDEX_HEART:
			switch(g_Hrc.HrcData.SettingState)
			{
			  case SET_HEART:
			  	w_Dat=g_Hrc.HrcData.HRProParameter.thr.TargetHeart;
			  	break;
			  case SET_WORKINTERVAL_HEART:
			  	w_Dat=g_Hrc.HrcData.HRProParameter.IntervalsHR.IntervalHeart;
			  	break;
			  case SET_RESTINTERVAL_HEART:
			  	w_Dat=g_Hrc.HrcData.HRProParameter.IntervalsHR.RestIntervalHeart;
			  	break;
			  case SET_SEGMENT_1_HEART:
			  	w_Dat=g_Hrc.HrcData.HRProParameter.StepsHR.Segment1Heart;	
			  	break;
			  case SET_SEGMENT_2_HEART:
			  	w_Dat=g_Hrc.HrcData.HRProParameter.StepsHR.Segment2Heart;
			  	break;
			  case SET_SEGMENT_3_HEART:
			  	w_Dat=g_Hrc.HrcData.HRProParameter.StepsHR.Segment3Heart;
			  	break;
			  case SET_SEGMENT_4_HEART:
			  	w_Dat=g_Hrc.HrcData.HRProParameter.StepsHR.Segment4Heart;
			  	break;
			}
			break;
		case INDEX_TIME:
			switch(g_Hrc.HrcData.SettingState)
			{
			  case SET_TIME:
			  	w_Dat=g_Hrc.HrcData.WorkoutTargetTime;
			  	break;
			  case SET_WORKINTERVAL_TIME:
			  	w_Dat=g_Hrc.HrcData.HRProParameter.IntervalsHR.IntervalTime;
			  	break;
			  case SET_RESTINTERVAL_TIME:
			  	w_Dat=g_Hrc.HrcData.HRProParameter.IntervalsHR.RestIntervalTime;
			  	break;
			  case SET_SEGMENT_TIME:
			  	w_Dat=g_Hrc.HrcData.HRProParameter.StepsHR.SegmentTime;	
			  	break;
			}
			break;
		default:break;
	 }
	 return w_Dat;
}
UINT16 ProgramHrc_RunData(UINT8 ParaIndex)
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


PROGRAM_PENDING_EVTS ProgramHrc_SetUp(ProgramKeyEvts *keyevt)
{
  UINT16 w_Dat;
  UINT16 SegTime;
  
  PROGRAM_PENDING_EVTS pendingEvt;
  pendingEvt.evt=PROGRAM_NONE_EVENT;
  memset(&pendingEvt.WorkOutEvts,0,sizeof(pendingEvt.WorkOutEvts));
    if(keyevt->Bit_All == 0) return pendingEvt;
  if(keyevt->Bit_Flag.Key_Enter)
  {
    keyevt->Bit_Flag.Key_Enter=false;
    if(g_Hrc.ProHrc.Setting_Array[g_Hrc.ProHrc.SettingArray_Index] != SET_PRESS_START )
    {
      keyevt->Bit_Flag.Key_StateChange=true;
      g_Hrc.ProHrc.SettingArray_Index++;
    }
  }
  else if(keyevt->Bit_Flag.Key_Back)
  {
    keyevt->Bit_Flag.Key_Back=false;
    if(g_Hrc.ProHrc.Setting_Array[g_Hrc.ProHrc.SettingArray_Index] != NO_SETTING )
    {
      keyevt->Bit_Flag.Key_StateChange=true;
      if(SET_TIME==g_Hrc.ProHrc.Setting_Array[g_Hrc.ProHrc.SettingArray_Index])
      {
        g_Hrc.HrcData.WorkoutTargetTime=30*60;
        Data_Set_Time(g_Hrc.HrcData.WorkoutTargetTime);
      }
      else if(SET_HEART==g_Hrc.ProHrc.Setting_Array[g_Hrc.ProHrc.SettingArray_Index])
      {
        g_Hrc.HrcData.HRProParameter.thr.TargetHeart=80;
        Data_Set_TargetHeart(g_Hrc.HrcData.HRProParameter.thr.TargetHeart);
      }
      g_Hrc.ProHrc.SettingArray_Index--;
    }
  }
  g_Hrc.HrcData.SettingState=g_Hrc.ProHrc.Setting_Array[g_Hrc.ProHrc.SettingArray_Index];
  switch(g_Hrc.HrcData.SettingState)
  {
  case SET_PROGRAM:
    if(keyevt->Bit_Flag.Key_StateChange)
    {
      keyevt->Bit_Flag.Key_StateChange=false;
      pendingEvt.evt=PROGRAM_CHANGE_EVENT;
      g_Hrc.ProHrc.Prg_NextStatus(PS_SELECTPRG);
      ProgramHrc_Data_Initial();
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
      if(g_Hrc.HrcData.WorkoutTargetTime<99*60)
      {
        g_Hrc.HrcData.WorkoutTargetTime +=60;
      }
      else
      {
        g_Hrc.HrcData.WorkoutTargetTime =99*60;
      }
      pendingEvt.evt=TIME_CHANGE_EVENT;
    }
    else if(keyevt->Bit_Flag.Key_Dn)
    {
      if(g_Hrc.HrcData.WorkoutTargetTime>15*60)
      {
        g_Hrc.HrcData.WorkoutTargetTime -=60;
      }
      else
      {
        g_Hrc.HrcData.WorkoutTargetTime =15*60;
      }
      pendingEvt.evt=TIME_CHANGE_EVENT;
    }
    Data_Set_Time(g_Hrc.HrcData.WorkoutTargetTime);
    break;
    
  case SET_WORKINTERVAL_TIME:
    if(keyevt->Bit_Flag.Key_StateChange)
    {
      keyevt->Bit_Flag.Key_StateChange=false;
      pendingEvt.evt=TIME_CHANGE_EVENT;
    }
    if(keyevt->Bit_Flag.Key_Up)
    {
      if(g_Hrc.HrcData.HRProParameter.IntervalsHR.IntervalTime<5*60)
        g_Hrc.HrcData.HRProParameter.IntervalsHR.IntervalTime+=60;
      else
        g_Hrc.HrcData.HRProParameter.IntervalsHR.IntervalTime=5*60;
      pendingEvt.evt=TIME_CHANGE_EVENT;
    }
    else if(keyevt->Bit_Flag.Key_Dn)
    {
      if(g_Hrc.HrcData.HRProParameter.IntervalsHR.IntervalTime>2*60)
        g_Hrc.HrcData.HRProParameter.IntervalsHR.IntervalTime-=60;
      else
        g_Hrc.HrcData.HRProParameter.IntervalsHR.IntervalTime=2*60;
      pendingEvt.evt=TIME_CHANGE_EVENT;
    }
    break;
  case SET_RESTINTERVAL_TIME:
    if(keyevt->Bit_Flag.Key_StateChange)
    {
      keyevt->Bit_Flag.Key_StateChange=false;
      pendingEvt.evt=TIME_CHANGE_EVENT;
    }
    if(keyevt->Bit_Flag.Key_Up)
    {
      if(g_Hrc.HrcData.HRProParameter.IntervalsHR.RestIntervalTime<5*60)
        g_Hrc.HrcData.HRProParameter.IntervalsHR.RestIntervalTime+=60;
      else
        g_Hrc.HrcData.HRProParameter.IntervalsHR.RestIntervalTime=5*60;
      pendingEvt.evt=TIME_CHANGE_EVENT;
    }
    else if(keyevt->Bit_Flag.Key_Dn)
    {
      if(g_Hrc.HrcData.HRProParameter.IntervalsHR.RestIntervalTime>2*60)
        g_Hrc.HrcData.HRProParameter.IntervalsHR.RestIntervalTime-=60;
      else
        g_Hrc.HrcData.HRProParameter.IntervalsHR.RestIntervalTime=2*60;
      pendingEvt.evt=TIME_CHANGE_EVENT;
    }
    break;
  case SET_SEGMENT_TIME:
    if(keyevt->Bit_Flag.Key_StateChange)
    {
      keyevt->Bit_Flag.Key_StateChange=false;
      pendingEvt.evt=TIME_CHANGE_EVENT;
    }
    if(keyevt->Bit_Flag.Key_Up)
    {
      if(g_Hrc.HrcData.HRProParameter.StepsHR.SegmentTime<10*60)
        g_Hrc.HrcData.HRProParameter.StepsHR.SegmentTime+=60;
      else
        g_Hrc.HrcData.HRProParameter.StepsHR.SegmentTime=10*60;
      pendingEvt.evt=TIME_CHANGE_EVENT;
      
    }
    else if(keyevt->Bit_Flag.Key_Dn)
    {
      if(g_Hrc.HrcData.HRProParameter.StepsHR.SegmentTime<3*60)
        g_Hrc.HrcData.HRProParameter.StepsHR.SegmentTime-=60;
      else
        g_Hrc.HrcData.HRProParameter.StepsHR.SegmentTime=3*60;
      pendingEvt.evt=TIME_CHANGE_EVENT;
      
    }
    break;
  case SET_HEART:
    if(keyevt->Bit_Flag.Key_StateChange)
    {
      keyevt->Bit_Flag.Key_StateChange=false;
        Data_Set_TargetHeart(g_Hrc.HrcData.HRProParameter.thr.TargetHeart);
      pendingEvt.evt=HEART_CHANGE_EVENT;
      
    }
    if(keyevt->Bit_Flag.Key_Up)
    {
      if(g_Hrc.HrcData.HRProParameter.thr.TargetHeart<g_Hrc.HrcData.Max_Heart)
        g_Hrc.HrcData.HRProParameter.thr.TargetHeart+=5;
      else
        g_Hrc.HrcData.HRProParameter.thr.TargetHeart=g_Hrc.HrcData.Max_Heart;
        Data_Set_TargetHeart(g_Hrc.HrcData.HRProParameter.thr.TargetHeart);
      pendingEvt.evt=HEART_CHANGE_EVENT;
    }
    else if(keyevt->Bit_Flag.Key_Dn)
    {
      if(g_Hrc.HrcData.HRProParameter.thr.TargetHeart==g_Hrc.HrcData.Max_Heart)
      {
        if(g_Hrc.HrcData.HRProParameter.thr.TargetHeart%5 == 0)
          g_Hrc.HrcData.HRProParameter.thr.TargetHeart-= 5;
        else
          g_Hrc.HrcData.HRProParameter.thr.TargetHeart = (g_Hrc.HrcData.HRProParameter.thr.TargetHeart/5 + 1) * 5;
      }
      else
      {
        if(g_Hrc.HrcData.HRProParameter.thr.TargetHeart-5>50)
          g_Hrc.HrcData.HRProParameter.thr.TargetHeart-=5;
        else 
          g_Hrc.HrcData.HRProParameter.thr.TargetHeart=50;
      }
        Data_Set_TargetHeart(g_Hrc.HrcData.HRProParameter.thr.TargetHeart);
      pendingEvt.evt=HEART_CHANGE_EVENT;
    }
    break;
  case SET_WORKINTERVAL_HEART:
    if(keyevt->Bit_Flag.Key_StateChange)
    {
      keyevt->Bit_Flag.Key_StateChange=false;
      pendingEvt.evt=HEART_CHANGE_EVENT;
    }
    if(keyevt->Bit_Flag.Key_Up)
    {
      if(g_Hrc.HrcData.HRProParameter.IntervalsHR.IntervalHeart<g_Hrc.HrcData.Max_Heart)
        g_Hrc.HrcData.HRProParameter.IntervalsHR.IntervalHeart+=5;
      else
        g_Hrc.HrcData.HRProParameter.IntervalsHR.IntervalHeart=g_Hrc.HrcData.Max_Heart;
      pendingEvt.evt=HEART_CHANGE_EVENT;
    }
    else if(keyevt->Bit_Flag.Key_Dn)
    {
      if(g_Hrc.HrcData.HRProParameter.IntervalsHR.IntervalHeart==g_Hrc.HrcData.Max_Heart)
      {
        if(g_Hrc.HrcData.HRProParameter.IntervalsHR.IntervalHeart%5 == 0)
          g_Hrc.HrcData.HRProParameter.IntervalsHR.IntervalHeart-= 5;
        else
          g_Hrc.HrcData.HRProParameter.IntervalsHR.IntervalHeart = (g_Hrc.HrcData.HRProParameter.IntervalsHR.IntervalHeart/5 + 1) * 5;
      }
      else
      {
        if(g_Hrc.HrcData.HRProParameter.IntervalsHR.IntervalHeart-5>50)
          g_Hrc.HrcData.HRProParameter.IntervalsHR.IntervalHeart-=5;
        else 
          g_Hrc.HrcData.HRProParameter.IntervalsHR.IntervalHeart=50;
      }
      pendingEvt.evt=HEART_CHANGE_EVENT;
    }
    break;
  case SET_RESTINTERVAL_HEART:
    if(keyevt->Bit_Flag.Key_StateChange)
    {
      keyevt->Bit_Flag.Key_StateChange=false;
      pendingEvt.evt=HEART_CHANGE_EVENT;
    }
    if(keyevt->Bit_Flag.Key_Up)
    {
      if(g_Hrc.HrcData.HRProParameter.IntervalsHR.RestIntervalHeart<g_Hrc.HrcData.Max_Heart)
        g_Hrc.HrcData.HRProParameter.IntervalsHR.RestIntervalHeart+=5;
      else
        g_Hrc.HrcData.HRProParameter.IntervalsHR.RestIntervalHeart=g_Hrc.HrcData.Max_Heart;
      pendingEvt.evt=HEART_CHANGE_EVENT;
    }
    else if(keyevt->Bit_Flag.Key_Dn)
    {
      if(g_Hrc.HrcData.HRProParameter.IntervalsHR.RestIntervalHeart==g_Hrc.HrcData.Max_Heart)
      {
        if(g_Hrc.HrcData.HRProParameter.IntervalsHR.RestIntervalHeart%5 == 0)
          g_Hrc.HrcData.HRProParameter.IntervalsHR.RestIntervalHeart-= 5;
        else
          g_Hrc.HrcData.HRProParameter.IntervalsHR.RestIntervalHeart = (g_Hrc.HrcData.HRProParameter.IntervalsHR.IntervalHeart/5 + 1) * 5;
      }
      else
      {
        if(g_Hrc.HrcData.HRProParameter.IntervalsHR.RestIntervalHeart-5>50)
          g_Hrc.HrcData.HRProParameter.IntervalsHR.RestIntervalHeart-=5;
        else 
          g_Hrc.HrcData.HRProParameter.IntervalsHR.RestIntervalHeart=50;
      }
      pendingEvt.evt=HEART_CHANGE_EVENT;
    }
    break;
  case SET_SEGMENT_1_HEART:
    if(keyevt->Bit_Flag.Key_StateChange)
    {
      keyevt->Bit_Flag.Key_StateChange=false;
      pendingEvt.evt=HEART_CHANGE_EVENT;
    }
    if(keyevt->Bit_Flag.Key_Up)
    {
      if(g_Hrc.HrcData.HRProParameter.StepsHR.Segment1Heart<g_Hrc.HrcData.Max_Heart)
        g_Hrc.HrcData.HRProParameter.StepsHR.Segment1Heart+=5;
      else
        g_Hrc.HrcData.HRProParameter.StepsHR.Segment1Heart=g_Hrc.HrcData.Max_Heart;
      pendingEvt.evt=HEART_CHANGE_EVENT;
    }
    else if(keyevt->Bit_Flag.Key_Dn)
    {
      if(g_Hrc.HrcData.HRProParameter.StepsHR.Segment1Heart==g_Hrc.HrcData.Max_Heart)
      {
        if(g_Hrc.HrcData.HRProParameter.StepsHR.Segment1Heart%5 == 0)
          g_Hrc.HrcData.HRProParameter.StepsHR.Segment1Heart-= 5;
        else
          g_Hrc.HrcData.HRProParameter.StepsHR.Segment1Heart = (g_Hrc.HrcData.HRProParameter.StepsHR.Segment1Heart/5 + 1) * 5;
      }
      else
      {
        if(g_Hrc.HrcData.HRProParameter.StepsHR.Segment1Heart-5>50)
          g_Hrc.HrcData.HRProParameter.StepsHR.Segment1Heart-=5;
        else 
          g_Hrc.HrcData.HRProParameter.StepsHR.Segment1Heart=50;
      }
      pendingEvt.evt=HEART_CHANGE_EVENT;
    }	  	
    break;
  case SET_SEGMENT_2_HEART:
    if(keyevt->Bit_Flag.Key_StateChange)
    {
      keyevt->Bit_Flag.Key_StateChange=false;
      pendingEvt.evt=HEART_CHANGE_EVENT;
    }
    if(keyevt->Bit_Flag.Key_Up)
    {
      if(g_Hrc.HrcData.HRProParameter.StepsHR.Segment2Heart<g_Hrc.HrcData.Max_Heart)
        g_Hrc.HrcData.HRProParameter.StepsHR.Segment2Heart+=5;
      else
        g_Hrc.HrcData.HRProParameter.StepsHR.Segment2Heart=g_Hrc.HrcData.Max_Heart;
      pendingEvt.evt=HEART_CHANGE_EVENT;
    }
    else if(keyevt->Bit_Flag.Key_Dn)
    {
      if(g_Hrc.HrcData.HRProParameter.StepsHR.Segment2Heart==g_Hrc.HrcData.Max_Heart)
      {
        if(g_Hrc.HrcData.HRProParameter.StepsHR.Segment2Heart%5 == 0)
          g_Hrc.HrcData.HRProParameter.StepsHR.Segment2Heart-= 5;
        else
          g_Hrc.HrcData.HRProParameter.StepsHR.Segment2Heart = (g_Hrc.HrcData.HRProParameter.StepsHR.Segment2Heart/5 + 1) * 5;
      }
      else
      {
        if(g_Hrc.HrcData.HRProParameter.StepsHR.Segment2Heart-5>50)
          g_Hrc.HrcData.HRProParameter.StepsHR.Segment2Heart-=5;
        else 
          g_Hrc.HrcData.HRProParameter.StepsHR.Segment2Heart=50;
      }
      pendingEvt.evt=HEART_CHANGE_EVENT;
    }
    break;
  case SET_SEGMENT_3_HEART:
    if(keyevt->Bit_Flag.Key_StateChange)
    {
      keyevt->Bit_Flag.Key_StateChange=false;
      pendingEvt.evt=HEART_CHANGE_EVENT;
    }
    if(keyevt->Bit_Flag.Key_Up)
    {
      if(g_Hrc.HrcData.HRProParameter.StepsHR.Segment3Heart<g_Hrc.HrcData.Max_Heart)
        g_Hrc.HrcData.HRProParameter.StepsHR.Segment3Heart+=5;
      else
        g_Hrc.HrcData.HRProParameter.StepsHR.Segment3Heart=g_Hrc.HrcData.Max_Heart;
      pendingEvt.evt=HEART_CHANGE_EVENT;
    }
    else if(keyevt->Bit_Flag.Key_Dn)
    {
      if(g_Hrc.HrcData.HRProParameter.StepsHR.Segment3Heart==g_Hrc.HrcData.Max_Heart)
      {
        if(g_Hrc.HrcData.HRProParameter.StepsHR.Segment3Heart%5 == 0)
          g_Hrc.HrcData.HRProParameter.StepsHR.Segment3Heart-= 5;
        else
          g_Hrc.HrcData.HRProParameter.StepsHR.Segment3Heart = (g_Hrc.HrcData.HRProParameter.StepsHR.Segment3Heart/5 + 1) * 5;
      }
      else
      {
        if(g_Hrc.HrcData.HRProParameter.StepsHR.Segment3Heart-5>50)
          g_Hrc.HrcData.HRProParameter.StepsHR.Segment3Heart-=5;
        else 
          g_Hrc.HrcData.HRProParameter.StepsHR.Segment3Heart=50;
      }
      pendingEvt.evt=HEART_CHANGE_EVENT;
    }
    break;
  case SET_SEGMENT_4_HEART:
    if(keyevt->Bit_Flag.Key_StateChange)
    {
      keyevt->Bit_Flag.Key_StateChange=false;
      pendingEvt.evt=HEART_CHANGE_EVENT;
    }
    if(keyevt->Bit_Flag.Key_Up)
    {
      if(g_Hrc.HrcData.HRProParameter.StepsHR.Segment4Heart<g_Hrc.HrcData.Max_Heart)
        g_Hrc.HrcData.HRProParameter.StepsHR.Segment4Heart+=5;
      else
        g_Hrc.HrcData.HRProParameter.StepsHR.Segment4Heart=g_Hrc.HrcData.Max_Heart;
      pendingEvt.evt=HEART_CHANGE_EVENT;
    }
    else if(keyevt->Bit_Flag.Key_Dn)
    {
      if(g_Hrc.HrcData.HRProParameter.StepsHR.Segment4Heart==g_Hrc.HrcData.Max_Heart)
      {
        if(g_Hrc.HrcData.HRProParameter.StepsHR.Segment4Heart%5 == 0)
          g_Hrc.HrcData.HRProParameter.StepsHR.Segment4Heart-= 5;
        else
          g_Hrc.HrcData.HRProParameter.StepsHR.Segment4Heart = (g_Hrc.HrcData.HRProParameter.StepsHR.Segment4Heart/5 + 1) * 5;
      }
      else
      {
        if(g_Hrc.HrcData.HRProParameter.StepsHR.Segment4Heart-5>50)
          g_Hrc.HrcData.HRProParameter.StepsHR.Segment4Heart-=5;
        else 
          g_Hrc.HrcData.HRProParameter.StepsHR.Segment4Heart=50;
      }
      pendingEvt.evt=HEART_CHANGE_EVENT;
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
  switch(Program_Get_ProgramID())
  {
  case PS_THR_ZONE:
    w_Dat=Data_Get_Time(TARGET_TIME)-Data_Get_Time(WARMUP_TIME)-Data_Get_Time(COOLDOWN_TIME);
    g_Hrc.HrcData.StageTime=w_Dat/g_Hrc.HrcData.RemainSegment;
    g_Hrc.HrcData.WorkoutRemainingTime=w_Dat%g_Hrc.HrcData.RemainSegment;
    break;
  case PS_INTERVALS_HR:
    w_Dat=Data_Get_Time(TARGET_TIME)-Data_Get_Time(WARMUP_TIME)-Data_Get_Time(COOLDOWN_TIME);
    SegTime=g_Hrc.HrcData.HRProParameter.IntervalsHR.IntervalTime+g_Hrc.HrcData.HRProParameter.IntervalsHR.RestIntervalTime;
    g_Hrc.HrcData.WorkoutTotalSegment=w_Dat/SegTime;
    g_Hrc.HrcData.WorkoutTotalSegment *=2;
    w_Dat %=SegTime;
    if(w_Dat>g_Hrc.HrcData.HRProParameter.IntervalsHR.IntervalTime)
      g_Hrc.HrcData.WorkoutTotalSegment +=2;
    else
      g_Hrc.HrcData.WorkoutTotalSegment +=1;
    g_Hrc.HrcData.WorkoutTotalSegment +=3;
    break;
  case PS_STEPS_THR:
    w_Dat=Data_Get_Time(TARGET_TIME)-Data_Get_Time(WARMUP_TIME)-Data_Get_Time(COOLDOWN_TIME);
    SegTime=g_Hrc.HrcData.HRProParameter.StepsHR.SegmentTime;
    g_Hrc.HrcData.WorkoutTotalSegment =w_Dat/SegTime;
    w_Dat %=SegTime;
    if(w_Dat>g_Hrc.HrcData.HRProParameter.StepsHR.SegmentTime)
    {
      g_Hrc.HrcData.WorkoutTotalSegment +=1;
    }
    g_Hrc.HrcData.WorkoutTotalSegment +=3;
    break;
  }
  
  return pendingEvt; 
}

PROFILE *ProgramHrc_ProfileShow(PROFILE_TAG tag_index,UCHAR* Dotmatrix_pos)
{
  if(ProgramLibConfigPara->DotMatrixLength+g_Hrc.HrcData.Stage<=g_Hrc.HrcData.WorkoutTotalSegment)
  {
    *Dotmatrix_pos=0;
  }
  else
  {
    *Dotmatrix_pos=ProgramLibConfigPara->DotMatrixLength+g_Hrc.HrcData.Stage-g_Hrc.HrcData.WorkoutTotalSegment;
  }
  for(UCHAR by_Loop=*Dotmatrix_pos;by_Loop<ProgramLibConfigPara->DotMatrixLength;by_Loop++)
  {
    Prg_ProfileData.SpeedProfile[by_Loop]=Data_Get_Speed(TARGET_SPEEDX10_AUTO);
    Prg_ProfileData.InclineProfile[by_Loop]=Data_Get_Incline(TARGET_INCLINEX10);
    Prg_ProfileData.ResistanceProfile[by_Loop]=Data_Get_Resistance(TARGET_RESISTANCE);
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
      Profile.ProfileData=Prg_ProfileData.SpeedProfile;
      break;
    default:
      Profile.ProfileData=Prg_ProfileData.SpeedProfile;
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
      Profile.ProfileData=Prg_ProfileData.ResistanceProfile;
      break;
    default:
      Profile.ProfileData=Prg_ProfileData.ResistanceProfile;
      break;
    }  
  }
  Profile.Length=ProgramLibConfigPara->DotMatrixLength;
  return &Profile;
}



PROGRAM_PENDING_EVTS ProgramHrc_Run(ProgramKeyEvts *keyevt)
{  
  UINT16 w_Dat;
  static UINT16 LastValue=0xffff;
  PROGRAM_PENDING_EVTS pendingEvt;
  pendingEvt.evt=PROGRAM_NONE_EVENT;
  memset(&pendingEvt.WorkOutEvts,0,sizeof(pendingEvt.WorkOutEvts));
  if(g_Hrc.ProHrc.Prg_CurrentStatus()<PS_WARMUP||g_Hrc.ProHrc.Prg_CurrentStatus()>PS_COOLDOWN)return pendingEvt;
  
  if(LastValue!=Data_Get_Time(ELAPSED_TIME))
  {
    LastValue=Data_Get_Time(ELAPSED_TIME);
    PHrc_Cul_Data();
    if(Data_Get_Time(ELAPSED_TIME)>=Data_Get_Time(TARGET_TIME)||(g_Hrc.HrcData.HeartMore25==1))
    {
      if(g_Hrc.ProHrc.Prg_CurrentStatus()!=PS_COMPLETED)
      {
        g_Hrc.ProHrc.Prg_NextStatus(PS_COMPLETED);
        pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
        pendingEvt.WorkOutEvts.Workout_Complete_Evt=true;
      }
    }
    else
    {
      if(Data_Get_Time(ELAPSED_TIME)==0)
      {
        g_Hrc.ProHrc.Prg_NextStatus(PS_WARMUP);
        pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
//        pendingEvt.WorkOutEvts.Workout_Warmup_Evt=true;
      }
      else if(Data_Get_Time(ELAPSED_TIME)==5)
      {
          pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
          pendingEvt.WorkOutEvts.Workout_Warmup_Evt=true;
      }
      else if(Data_Get_Time(ELAPSED_TIME)==HRC_WARMUP_TIME)
      {
        g_Hrc.ProHrc.Prg_NextStatus(PS_RUN);
        pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
//        pendingEvt.WorkOutEvts.Workout_Run_Evt=true;
      }
      else if(Data_Get_Time(ELAPSED_TIME)==HRC_WARMUP_TIME+5)
      {
        pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
        pendingEvt.WorkOutEvts.Workout_Run_Evt=true;
      }
      else if(Data_Get_Time(REMAINING_TIME)==HRC_COOLDOWN_TIME)
      {
        g_Hrc.ProHrc.Prg_NextStatus(PS_COOLDOWN);
        if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
        {
            Data_Set_Incline(Data_Get_Incline(TARGET_INCLINEX10)/2,1);
//          Data_Set_Speed(Data_Get_Speed(TARGET_SPEEDX10_AUTO)/2,SystemConfig_Get_Char(UNIT_FLAG_UCHAR)==METRIC?METRIC:STANDARD,1);
        }
        else
        {
          Data_Set_Resistance(Program_Get_Resistance(0)/2,1);
        }
        pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
//        pendingEvt.WorkOutEvts.Workout_Cooldown_Evt=true;
      }
      else if(Data_Get_Time(REMAINING_TIME) == (HRC_COOLDOWN_TIME-5))
      {
          pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
          pendingEvt.WorkOutEvts.Workout_Cooldown_Evt=true;
      }
      else if(Data_Get_Time(REMAINING_TIME)==HRC_COOLDOWN_TIME/2)
      {
        if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
        {
            Data_Set_Incline(0,1);
//          Data_Set_Speed(0,SystemConfig_Get_Char(UNIT_FLAG_UCHAR)==METRIC?METRIC:STANDARD,1);
        }
        else
        {
          Data_Set_Resistance(0,1);
        }
        pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
      }
      if(Data_Get_Time(TIME_IN_STAGE)==Data_Get_Time(STAGE_TIME))
      {
        g_Hrc.HrcData.Stage++;
        switch(Program_Get_ProgramID())
        {
        case PS_THR_ZONE:
          w_Dat=g_Hrc.HrcData.StageTime;
          if(g_Hrc.HrcData.WorkoutRemainingTime!=0)
          {
            if(g_Hrc.HrcData.Stage==g_Hrc.HrcData.RemainSegment)
            {
              w_Dat +=g_Hrc.HrcData.WorkoutRemainingTime;
            }
          }
          break;
        case PS_INTERVALS_HR:
          w_Dat=(g_Hrc.HrcData.Stage-1)%2;
          switch(w_Dat)
          {
          case 0:
            w_Dat=g_Hrc.HrcData.HRProParameter.IntervalsHR.IntervalTime;
            break;
          case 1:
            w_Dat=g_Hrc.HrcData.HRProParameter.IntervalsHR.RestIntervalTime;
            break;
          }                 
          break;
        case PS_STEPS_THR:
          w_Dat=g_Hrc.HrcData.HRProParameter.StepsHR.SegmentTime;
          break;
        }             
        if(Data_Get_Time(REMAINING_TIME)>HRC_COOLDOWN_TIME)
        {
          Data_Set_StageTime(w_Dat);
        }
        else 
          Data_Set_StageTime(HRC_COOLDOWN_TIME/2);
        
      }
    }
  }
  if(keyevt->Bit_Flag.Key_SpeedUp)
  {
    keyevt->Bit_Flag.Key_SpeedUp=false;
	 Data_Set_Speed_Base_User(Data_Get_Speed(TARGET_SPEEDX10_AUTO)+1,1);
    //Data_Set_Speed(Data_Get_Speed(TARGET_SPEEDX10_AUTO)+1,SystemConfig_Get_Display_Unit(User_Get_UserSlot( ))==METRIC?METRIC:STANDARD,1);
  }
  else if(keyevt->Bit_Flag.Key_SpeedDn)
  {
    keyevt->Bit_Flag.Key_SpeedDn=false;
	Data_Set_Speed_Base_User(Data_Get_Speed(TARGET_SPEEDX10_AUTO)-1,1);
    //Data_Set_Speed(Data_Get_Speed(TARGET_SPEEDX10_AUTO)-1,SystemConfig_Get_Display_Unit(User_Get_UserSlot( ))==METRIC?METRIC:STANDARD,1);
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
void ProHrc_Set_TargetTime(UINT16 w_Data)
{
    g_Hrc.HrcData.WorkoutTargetTime = w_Data;
}
void ProHrc_Set_TargetHR(UINT8 by_Dat)
{
  g_Hrc.HrcData.HRProParameter.thr.TargetHeart = by_Dat;
}
void initHrc(void)
{
  g_Hrc.ProHrc.Prg_Initial= ProgramHrc_Data_Initial;
  g_Hrc.ProHrc.BeginWorkout=ProgramHrc_BeginWorkout;
  g_Hrc.ProHrc.Prg_Setup = ProgramHrc_SetUp;
  g_Hrc.ProHrc.Prg_ProfileShow=ProgramHrc_ProfileShow;
  g_Hrc.ProHrc.Prg_Workout = ProgramHrc_Run;
    g_Hrc.ProHrc.Prg_SetupData= ProgramHrc_SetUpData;
    g_Hrc.ProHrc.Prg_RunData= ProgramHrc_RunData;
g_Hrc.ProHrc.SettingArray_Index= 0;
  g_Hrc.ProHrc.Programid = PS_THR_ZONE;
  g_Hrc.ProHrc.Setting_Array=THR_ZONE_SetUp;
  Program_Register_Function(&g_Hrc.ProHrc);
  g_Hrc.ProHrc.Programid = PS_STEPS_THR;
  g_Hrc.ProHrc.Setting_Array=Steps_HR_SetUp;	
  Program_Register_Function(&g_Hrc.ProHrc);
  g_Hrc.ProHrc.Programid = PS_INTERVALS_HR;
  g_Hrc.ProHrc.Setting_Array=Interval_HR_SetUp;
  Program_Register_Function(&g_Hrc.ProHrc);
}




















#endif

