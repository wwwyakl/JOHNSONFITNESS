#include"Pro_constant.h"

#ifdef __PROGRAM_CONSTANT__
typedef struct{
  UINT16 StageTime;
  UINT16 SegmentTime;
  UINT16 WorkoutTargetTime;
  UINT8 WorkoutTotalSegment;
  UINT8 Stage;
  UINT8 CheckWattsTime;
}Constant;

__no_init static struProgram ConstantWatts;

__no_init static PROFILE Profile;
__no_init static Constant g_Constant;
void ProgramConstantWatts_Adjust(void);
UINT16 ProgramConstantWatts_DivWatt(UINT8 Res_Level);

void ProgramConstantWatts_Data_Initial(void)
{
memset(&g_Constant,0,sizeof(Constant));
    Data_Set_Watt(50);
    Data_Set_Time(30*60);
    Data_Set_Resistance(SystemConfig_Get_Char(MIN_RESISTANCE_UINT8),0);
    Data_Set_Incline(0,0);      
    Data_Set_WarmUP_Time(0);
    Data_Set_CoolDown_Time(0);
    if(SystemConfig_Get_Char(DRIVE_TYPE_UINT8) == DIGITAL_ECB 
       || SystemConfig_Get_Char(DRIVE_TYPE_UINT8) == BIKE_0C_JIS)
        ProfileIndex=DefaultDotmatrix=PROFILE_TYPE_RESISTANCE;
    else
        ProfileIndex=DefaultDotmatrix=PROFILE_TYPE_INCLINE;
}
void ProgramConstantWatts_BeginWorkout(void)
{

}
UINT16 ProgramConstantWatts_SetUpData(UINT8 ParaIndex)
{
	UINT16 w_Dat=0;
     switch(ParaIndex)
	{
		case INDEX_TIME:
			break;
     	}
	 return w_Dat;
}
UINT16 ProgramConstantWatts_RunData(UINT8 ParaIndex)
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
				w_Dat=Data_Get_Watts(TARGET_WATT);
			break;
			}
	 return w_Dat;
}
PROGRAM_PENDING_EVTS ProgramConstantWatts_SetUp(ProgramKeyEvts *keyevt)
{
  UINT32 l_Dat = 0;
  PROGRAM_PENDING_EVTS pendingEvt;
  pendingEvt.evt=PROGRAM_NONE_EVENT;
  memset(&pendingEvt.WorkOutEvts,0,sizeof(pendingEvt.WorkOutEvts));
   if(keyevt->Bit_All == 0) return pendingEvt;

  if(keyevt->Bit_Flag.Key_Enter)
  {
    keyevt->Bit_Flag.Key_Enter=false;
    if(ConstantWatts.Setting_Array[ConstantWatts.SettingArray_Index] != SET_PRESS_START )
    {
      keyevt->Bit_Flag.Key_StateChange=true;
      ConstantWatts.SettingArray_Index++;
    }
  }
  else if(keyevt->Bit_Flag.Key_Back)
  {
    keyevt->Bit_Flag.Key_Back=false;
    if(ConstantWatts.Setting_Array[ConstantWatts.SettingArray_Index] != NO_SETTING )
    {
      keyevt->Bit_Flag.Key_StateChange=true;
      if(SET_PROGRAM==ConstantWatts.Setting_Array[ConstantWatts.SettingArray_Index])
      {
        Data_Set_Time(0);
      }
      else if(SET_TIME==ConstantWatts.Setting_Array[ConstantWatts.SettingArray_Index])
      {
        Data_Set_Time(30*60);
      }
      else if(SET_WATTS==ConstantWatts.Setting_Array[ConstantWatts.SettingArray_Index])
      {
        Data_Set_Watt(50);
      }
      ConstantWatts.SettingArray_Index--;
    }
  }
  switch(ConstantWatts.Setting_Array[ConstantWatts.SettingArray_Index])
  {
  case SET_PROGRAM:
    if(keyevt->Bit_Flag.Key_StateChange)
    {
      keyevt->Bit_Flag.Key_StateChange=false;
      pendingEvt.evt=PROGRAM_CHANGE_EVENT;
	  ConstantWatts.Prg_NextStatus(PS_SELECTPRG);
      ProgramConstantWatts_Data_Initial();
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
      if(Program_Get_Time(DATA_MODE_NORMAL)<99*60)
      {
        Data_Set_Time(Program_Get_Time(DATA_MODE_NORMAL)+60);
      }
      else
      {
        Data_Set_Time(99*60);
      }
      pendingEvt.evt=TIME_CHANGE_EVENT;
    }
    else if(keyevt->Bit_Flag.Key_Dn)
    {
      if(Program_Get_Time(DATA_MODE_NORMAL)>15*60)
      {
        Data_Set_Time(Program_Get_Time(DATA_MODE_NORMAL)-60);
      }
      else
      {
        Data_Set_Time(15*60);
      }
      pendingEvt.evt=TIME_CHANGE_EVENT;
    }

    break;
  case SET_WATTS:
    if(keyevt->Bit_Flag.Key_StateChange)
    {
      keyevt->Bit_Flag.Key_StateChange=false;
      pendingEvt.evt=WATTS_CHANGE_EVENT;
    }
    if(keyevt->Bit_Flag.Key_Up)
    {
      Data_Set_Watt(Data_Get_Watts(TARGET_WATT)+5);
      pendingEvt.evt=WATTS_CHANGE_EVENT;
    }
    else if(keyevt->Bit_Flag.Key_Dn)
    {
      Data_Set_Watt(Data_Get_Watts(TARGET_WATT)-5);
      pendingEvt.evt=WATTS_CHANGE_EVENT;
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
  if(Data_Get_Time(TARGET_TIME) == 0) 
  {
      g_Constant.SegmentTime = 6000;  
  }
  else
  {
      l_Dat = (UINT32)Data_Get_Time(TARGET_TIME);
      l_Dat *= 100;               
      l_Dat /= ProgramLibConfigPara->DotMatrixLength;
      g_Constant.SegmentTime = l_Dat;
  }	 
  return pendingEvt; 
}

PROFILE *ProgramConstantWatts_ProfileShow(PROFILE_TAG tag_index,UCHAR* Dotmatrix_pos)
{
  *Dotmatrix_pos=g_Constant.Stage%ProgramLibConfigPara->DotMatrixLength;

  for(UINT8 i = *Dotmatrix_pos; i < ProgramLibConfigPara->DotMatrixLength; i++)
  {
    Prg_ProfileData.InclineProfile[i]=Data_Get_Incline(TARGET_INCLINEX10);
    Prg_ProfileData.ResistanceProfile[i]=Data_Get_Resistance(TARGET_RESISTANCE);
  }
 
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
Profile.Length=ProgramLibConfigPara->DotMatrixLength;
  return &Profile;
}



PROGRAM_PENDING_EVTS ProgramConstantWatts_Run(ProgramKeyEvts *keyevt)
{  
  UINT32 l_Dat = 0;
  static UINT16 LastValue=0xffff;
  PROGRAM_PENDING_EVTS pendingEvt;
  pendingEvt.evt=PROGRAM_NONE_EVENT;
  memset(&pendingEvt.WorkOutEvts,0,sizeof(pendingEvt.WorkOutEvts));
  
  if(ConstantWatts.Prg_CurrentStatus()<PS_WARMUP||ConstantWatts.Prg_CurrentStatus()>PS_COOLDOWN)return pendingEvt;
  if(LastValue!=Data_Get_Time(ELAPSED_TIME))
  {
      LastValue=Data_Get_Time(ELAPSED_TIME);
      if(Data_Get_Time(TARGET_TIME) == 0) //counting up
      {
          if(Data_Get_Time(ELAPSED_TIME) < MAX_WORKOUT_TIME -1) //data.c to handle this
          {
              l_Dat = (UINT32)Data_Get_Time(ELAPSED_TIME);
              l_Dat *= 100;
              l_Dat /=  g_Constant.SegmentTime;
              l_Dat %= ProgramLibConfigPara->DotMatrixLength;
              g_Constant.Stage = (UINT8)l_Dat; //This truncation is safe.
	  	  ConstantWatts.Prg_NextStatus(PS_RUN);

              pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
          }
      }
      else   //counting down
      {
          if(Data_Get_Time(REMAINING_TIME) == 0)
          {
              if(ConstantWatts.Prg_CurrentStatus()!=PS_COMPLETED)
              {
	  	  ConstantWatts.Prg_NextStatus(PS_COMPLETED);
                  pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
                  pendingEvt.WorkOutEvts.Workout_Complete_Evt=true;
              }
          }
          else
          {
              if(Data_Get_Time(ELAPSED_TIME))
              {
                  l_Dat = Data_Get_Time(ELAPSED_TIME);
                  l_Dat *= 100;
                  l_Dat /= g_Constant.SegmentTime;
                  g_Constant.Stage = (UINT8)l_Dat; //safe truncation
              }
              else
                  g_Constant.Stage = 0;
  	  	  ConstantWatts.Prg_NextStatus(PS_RUN);

              pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
          }
      }
	  ProgramConstantWatts_Adjust();
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
      Data_Set_Watt(Data_Get_Watts(TARGET_WATT)+5);
      pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
      pendingEvt.WorkOutEvts.Workout_TargetWattChange_Evt=true;
    }
    else if(keyevt->Bit_Flag.Key_ResistanceDn)
    {
      keyevt->Bit_Flag.Key_ResistanceDn=false;
      Data_Set_Watt(Data_Get_Watts(TARGET_WATT)-5);
      pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
      pendingEvt.WorkOutEvts.Workout_TargetWattChange_Evt=true;
    } 
    return pendingEvt;
}


void ProgramConstantWatts_Adjust(void)
{
  UINT16 pwmOutpput=0;
  UINT16 TargetWatt=Data_Get_Watts(TARGET_WATT);
  UINT16 CurrentWatt=Data_Get_Watts(CURRENT_WATT);
  UINT16 RpmValue=Data_Get_RPM(EPBI_RPM);
  UINT8 ResistanceLevel=Data_Get_Resistance(TARGET_RESISTANCE);
  UINT8 MinResistanceLevel=SystemConfig_Get_Char(MIN_RESISTANCE_UINT8);
  UINT8 MaxResistanceLevel=SystemConfig_Get_Char(MAX_RESISTANCE_UINT8);
  
  UINT16 w_watt_value2,w_watt_value1;
  w_watt_value1=ProgramConstantWatts_DivWatt(MinResistanceLevel);
  w_watt_value2=ProgramConstantWatts_DivWatt(MaxResistanceLevel);
  if(Watts_ParameterData->ResistanceType==ECB_TYPE)
  {
    g_Constant.CheckWattsTime++;
    if(g_Constant.CheckWattsTime<10)return;
    g_Constant.CheckWattsTime=0;
  }
  if(TargetWatt<=w_watt_value1)
  {
    if(Watts_ParameterData->ResistanceType==INDUCTION_TYPE)
      Data_Set_TargetPWMValue(InductionOutput_30P[0]);
    Data_Set_Resistance(1,1);
  }
  else if(TargetWatt>=w_watt_value2)
  {
    if(Watts_ParameterData->ResistanceType==INDUCTION_TYPE)
      Data_Set_TargetPWMValue(InductionOutput_30P[MaxResistanceLevel-MinResistanceLevel]);
    Data_Set_Resistance(Data_Get_Resistance(MAX_RESISTANCE),1);
  }
  else
  {
    ResistanceLevel=MaxResistanceLevel*(TargetWatt-w_watt_value1)/(w_watt_value2 - w_watt_value1);
    if(ResistanceLevel<MinResistanceLevel)
    {
      ResistanceLevel=MinResistanceLevel;
    }
    else if(ResistanceLevel>MaxResistanceLevel)
    {
      ResistanceLevel=MaxResistanceLevel;
    }
    if(TargetWatt==ProgramConstantWatts_DivWatt(ResistanceLevel))
    {
      if(Watts_ParameterData->ResistanceType==INDUCTION_TYPE)
        Data_Set_TargetPWMValue(InductionOutput_30P[ResistanceLevel-MinResistanceLevel]);
      Data_Set_Resistance(ResistanceLevel,1);
    }
    else
    {
      if(ProgramConstantWatts_DivWatt(ResistanceLevel)<TargetWatt)
      {
        while(1)
        {
#ifdef WDT_ENABLE
          HAL_Clear_WatchDog();
#endif
          w_watt_value1 = ProgramConstantWatts_DivWatt(ResistanceLevel);
          w_watt_value2 = ProgramConstantWatts_DivWatt(ResistanceLevel+1);
          if(w_watt_value2 >= TargetWatt || w_watt_value1==w_watt_value2)
          {
            break;
          }
          ResistanceLevel++;
        }
        Data_Set_Resistance(ResistanceLevel,1);      
      }
      else
      {
        while(1)
        {
#ifdef WDT_ENABLE
          HAL_Clear_WatchDog();
#endif
          w_watt_value2 = ProgramConstantWatts_DivWatt(ResistanceLevel);
          w_watt_value1 = ProgramConstantWatts_DivWatt(ResistanceLevel-1);
          if(w_watt_value1 <= TargetWatt || w_watt_value2==w_watt_value1)
          {
            break;
          }
          ResistanceLevel--;
        }
        Data_Set_Resistance(ResistanceLevel,1);
      }
      if(Watts_ParameterData->ResistanceType==INDUCTION_TYPE)
      {
        int pwm_small,pwm_big;
        pwm_small=InductionOutput_30P[ResistanceLevel-MinResistanceLevel];
        pwm_big=InductionOutput_30P[ResistanceLevel+1-MinResistanceLevel];
        pwmOutpput = 
          ((int long)TargetWatt - w_watt_value1)*(pwm_big - pwm_small)
            /(w_watt_value2 - w_watt_value1)+pwm_small;
        Data_Set_TargetPWMValue(pwmOutpput);
      }
    }
  }
}
UINT16 ProgramConstantWatts_DivWatt(UINT8 Res_Level)
{
  UINT8 Min_Resistance=SystemConfig_Get_Char(MIN_RESISTANCE_UINT8);
 if(Res_Level<Min_Resistance)
 {
   Res_Level=Min_Resistance;
 }
  
  UINT16 watt = 0;
  UINT16 by_RPM = Data_Get_RPM(EPBI_RPM);
  UINT16 by_Dat = 0;
  if(by_RPM > 14)
  {
    if(by_RPM<=20)
      by_Dat=(by_RPM-10)/10;
    else
      by_Dat=(by_RPM-15)/5;
  }                           
  else
  {
    by_Dat=0;
  }
  
  if(by_Dat >= Watts_ParameterData->Columns-1)
  {
    by_Dat=Watts_ParameterData->Columns-1;   
    watt=Watts_ParameterData->Watts_Table[(Res_Level-Min_Resistance) * Watts_ParameterData->Columns + by_Dat];
  }
  else
  {
    int watt1 = Watts_ParameterData->Watts_Table[(Res_Level-Min_Resistance) * (Watts_ParameterData->Columns) + by_Dat];
    int watt2 = Watts_ParameterData->Watts_Table[(Res_Level-Min_Resistance) * (Watts_ParameterData->Columns) + by_Dat + 1];
    int left = 0;
    if(by_RPM>15)
    {
      left = by_RPM%5;
      watt = (watt2 - watt1)*left/5 + watt1;
    }
    else
    {
      watt = watt1;
    }
  }
  
  return watt;
  
}

void initConstantWatts(void)
{
        
	ConstantWatts.Programid = PS_CONSTANT_WATTS;
	ConstantWatts.SettingArray_Index=0;
	ConstantWatts.Setting_Array=PConstantWatt_SetUp;

	ConstantWatts.Prg_Initial = ProgramConstantWatts_Data_Initial;
	ConstantWatts.BeginWorkout=	ProgramConstantWatts_BeginWorkout;
	ConstantWatts.Prg_Setup = ProgramConstantWatts_SetUp;
	ConstantWatts.Prg_ProfileShow=ProgramConstantWatts_ProfileShow;
	ConstantWatts.Prg_Workout = ProgramConstantWatts_Run;
	ConstantWatts.Prg_SetupData=ProgramConstantWatts_SetUpData;
	ConstantWatts.Prg_RunData=ProgramConstantWatts_RunData;
	Program_Register_Function(&ConstantWatts);

}
#else
void initConstantWatts(void){
}

#endif
