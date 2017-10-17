#include"Pro_constant.h"

#ifdef __PROGRAM_CONSTANT__

__no_init static PROFILE Profile;
__no_init static const UCHAR *Setting_Array;
__no_init static UCHAR WattsAdjust_Time;
__no_init static UINT16 WorkoutTargetWatts;

void ProgramConstantWatts_Adjust(void);
UINT16 ProgramConstantWatts_DivWatt(UINT8 Res_Level);

void ProgramConstantWatts_Data_Initial(void)
{
    _ul_Segment_Time=0;
    w_Current_Segment=0;
    Data_Set_Time(1800);
    Data_Set_Watt(50);
    Data_Set_Resistance(SystemConfig_Get_Char(MIN_RESISTANCE_UINT8),1);
    Data_Set_Incline(0,1);      
    Data_Set_WarmUP_Time(0);
    Data_Set_CoolDown_Time(0);
    Setting_Array=PConstantWatt_SetUp;
    WattsAdjust_Time=0;
    WorkoutTargetWatts=Data_Get_Watts(TARGET_WATT);
    if(SystemConfig_Get_Char(DRIVE_TYPE_UINT8) == DIGITAL_ECB 
       || SystemConfig_Get_Char(DRIVE_TYPE_UINT8) == BIKE_0C_JIS)
        DefaultDotmatrix=RESISTANCE_TYPE;
    else
        DefaultDotmatrix=INCLINE_TYPE;
}


PROGRAM_PENDING_EVTS ProgramConstantWatts_SetUp(ProgramKeyEvts *keyevt)
{
  UINT32 ul_Temp = 0;
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
      if(SET_PROGRAM==Setting_Array[Setting_Index])
      {
        Data_Set_Time(0);
      }
      else if(SET_TIME==Setting_Array[Setting_Index])
      {
        Data_Set_Time(30*60);
      }
      else if(SET_WATTS==Setting_Array[Setting_Index])
      {
        Data_Set_Watt(50);
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
      ProgramConstantWatts_Data_Initial();
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
    else if(keyevt->EventBits.Key_Dn)
    {
      if(Program_Get_Time(DATA_MODE_NORMAL)>5*60)
      {
        Data_Set_Time(Program_Get_Time(DATA_MODE_NORMAL)-60);
      }
      else
      {
        Data_Set_Time(5*60);
      }
      pendingEvt.evt=TIME_CHANGE_EVENT;
    }

    break;
  case SET_WATTS:
    if(keyevt->EventBits.Key_StateChange)
    {
      keyevt->EventBits.Key_StateChange=false;
      pendingEvt.evt=WATTS_CHANGE_EVENT;
    }
    if(keyevt->EventBits.Key_Up)
    {
      Data_Set_Watt(Data_Get_Watts(TARGET_WATT)+5);
      pendingEvt.evt=WATTS_CHANGE_EVENT;
    }
    else if(keyevt->EventBits.Key_Dn)
    {
      Data_Set_Watt(Data_Get_Watts(TARGET_WATT)-5);
      pendingEvt.evt=WATTS_CHANGE_EVENT;
    }      
    break;
  case SET_PRESS_START:
    if(keyevt->EventBits.Key_StateChange)
    {
      keyevt->EventBits.Key_StateChange=false;
      pendingEvt.evt=SETTING_COMPLETE_EVENT;
    }    
    break;   
  }
  if(Data_Get_Time(TARGET_TIME) == 0) 
  {
      _ul_Segment_Time = 6000;  
  }
  else
  {
      ul_Temp = (UINT32)Data_Get_Time(TARGET_TIME);
      ul_Temp *= 100;               
      ul_Temp /= ProgramParameter->DotMatrixLength;
      _ul_Segment_Time = ul_Temp;
  }	 
  return pendingEvt; 
}

PROFILE *ProgramConstantWatts_GetShow(PROFILE_TAG tag_index,UCHAR* Dotmatrix_pos)
{
  *Dotmatrix_pos=w_Current_Segment%ProgramParameter->DotMatrixLength;

  for(UINT8 i = *Dotmatrix_pos; i < ProgramParameter->DotMatrixLength; i++)
  {
    DotmatrixData.InclineProfile[i]=Data_Get_Incline(TARGET_INCLINEX10);
    DotmatrixData.ResistanceProfile[i]=Data_Get_Resistance(TARGET_RESISTANCE);
  }
 
  	switch(tag_index)
  		{
  		case RESISTANCE_PROFILE:
			Profile.ProfileData=DotmatrixData.ResistanceProfile;
			break;
  		case INCLINE_PROFILE:
			Profile.ProfileData=DotmatrixData.InclineProfile;
			break;
  		case DEFAULT_PROFILE:
			Profile.ProfileData=DotmatrixData.ResistanceProfile;
			break;
		default:
			Profile.ProfileData=DotmatrixData.ResistanceProfile;
				break;
  		}  	
Profile.Length=ProgramParameter->DotMatrixLength;
  return &Profile;
}



PROGRAM_PENDING_EVTS ProgramConstantWatts_Run(ProgramKeyEvts *keyevt)
{  
  UINT32 ul_Temp = 0;
  static UINT16 WorkOut_Old_Time=1;
  PROGRAM_PENDING_EVTS pendingEvt;
  pendingEvt.evt=PROGRAM_NONE_EVENT;
  pendingEvt.WorkOutEvts.AllEvts=0;
  
  if(g_StateMachine<PS_WARMUP||g_StateMachine>PS_COOLDOWN)return pendingEvt;
  if(keyevt->EventBits.Key_ResistanceUp)
  {
    keyevt->EventBits.Key_ResistanceUp=false;
    Data_Set_Watt(Data_Get_Watts(TARGET_WATT)+5);
  }
  else if(keyevt->EventBits.Key_ResistanceDn)
  {
    keyevt->EventBits.Key_ResistanceDn=false;
    Data_Set_Watt(Data_Get_Watts(TARGET_WATT)-5);
  }  
  if(WorkOut_Old_Time!=Data_Get_Time(ELAPSED_TIME))
  {
      WorkOut_Old_Time=Data_Get_Time(ELAPSED_TIME);
//				pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
//	pendingEvt.WorkOutEvts.EvtBit.Workout_Refresh_Data=true;	

      if(Data_Get_Time(TARGET_TIME) == 0) //counting up
      {
          if(Data_Get_Time(ELAPSED_TIME) < MAX_WORKOUT_TIME -1) //data.c to handle this
          {
              ul_Temp = (UINT32)Data_Get_Time(ELAPSED_TIME);
              ul_Temp *= 100;
              ul_Temp /=  _ul_Segment_Time;
              ul_Temp %= ProgramParameter->DotMatrixLength;
              w_Current_Segment = (UINT8)ul_Temp; //This truncation is safe.
              g_StateMachine=PS_RUN;
              pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
          }
      }
      else   //counting down
      {
          if(Data_Get_Time(REMAINING_TIME) == 0)
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
              if(Data_Get_Time(ELAPSED_TIME))
              {
                  ul_Temp = Data_Get_Time(ELAPSED_TIME);
                  ul_Temp *= 100;
                  ul_Temp /= _ul_Segment_Time;
                  w_Current_Segment = (UINT8)ul_Temp; //safe truncation
              }
              else
                  w_Current_Segment = 0;
              g_StateMachine=PS_RUN;
              pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
          }
      }
  }
  ProgramConstantWatts_Adjust();
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
      Data_Set_Watt(Data_Get_Watts(TARGET_WATT)+5);
    }
    else if(keyevt->EventBits.Key_ResistanceDn)
    {
      keyevt->EventBits.Key_ResistanceDn=false;
      Data_Set_Watt(Data_Get_Watts(TARGET_WATT)-5);
    } 
    if(WorkoutTargetWatts!=Data_Get_Watts(TARGET_WATT))
    {
      WorkoutTargetWatts=Data_Get_Watts(TARGET_WATT);
      pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
      pendingEvt.WorkOutEvts.EvtBit.Workout_TargetWattChange_Evt=true;
    }
    return pendingEvt;
}

void initConstantWatts(void)
{
	struProgram ConstantWatts;
        
	ConstantWatts.Programid = PS_CONSTANT_WATTS;
	ConstantWatts.Initial = ProgramConstantWatts_Data_Initial;
	ConstantWatts.Setup = ProgramConstantWatts_SetUp;
	ConstantWatts.GetProfile=ProgramConstantWatts_GetShow;
	ConstantWatts.Workout = ProgramConstantWatts_Run;
	Program_Add_Array(ConstantWatts);

}

void ProgramConstantWatts_Adjust(void)
{
  UINT16 PWM_Out=0;
  UINT16 w_Watt_Target=Data_Get_Watts(TARGET_WATT);
  UINT16 w_Watt=Data_Get_Watts(CURRENT_WATT);
  UINT16 RpmValue=Data_Get_RPM(EPBI_RPM);
  UINT8 WattsLevel=Data_Get_Resistance(TARGET_RESISTANCE);
  UINT8 MinLevel=SystemConfig_Get_Char(MIN_RESISTANCE_UINT8);
  UINT8 MaxLevel=SystemConfig_Get_Char(MAX_RESISTANCE_UINT8);
  
  UINT16 w_watt_value2,w_watt_value1;
  w_watt_value1=ProgramConstantWatts_DivWatt(MinLevel);
  w_watt_value2=ProgramConstantWatts_DivWatt(MaxLevel);
  if(_Watts_Talbelist->ResistanceType==ECB_TYPE)
  {
    if(Data_Get_1s())
    {
      WattsAdjust_Time++;
    }
    if(WattsAdjust_Time<10)return;
    WattsAdjust_Time=0;
  }
  if(w_Watt_Target<=w_watt_value1)
  {
    if(_Watts_Talbelist->ResistanceType==INDUCTION_TYPE)
      Data_Set_TargetPWMValue(InductionOutput_30P[0]);
    Data_Set_Resistance(1,1);
  }
  else if(w_Watt_Target>=w_watt_value2)
  {
    if(_Watts_Talbelist->ResistanceType==INDUCTION_TYPE)
      Data_Set_TargetPWMValue(InductionOutput_30P[MaxLevel-MinLevel]);
    Data_Set_Resistance(Data_Get_Resistance(MAX_RESISTANCE),1);
  }
  else
  {
    WattsLevel=MaxLevel*(w_Watt_Target-w_watt_value1)/(w_watt_value2 - w_watt_value1);
    if(WattsLevel<MinLevel)
    {
      WattsLevel=MinLevel;
    }
    else if(WattsLevel>MaxLevel)
    {
      WattsLevel=MaxLevel;
    }
    if(w_Watt_Target==ProgramConstantWatts_DivWatt(WattsLevel))
    {
      if(_Watts_Talbelist->ResistanceType==INDUCTION_TYPE)
        Data_Set_TargetPWMValue(InductionOutput_30P[WattsLevel-MinLevel]);
      Data_Set_Resistance(WattsLevel,1);
    }
    else
    {
      if(ProgramConstantWatts_DivWatt(WattsLevel)<w_Watt_Target)
      {
        while(1)
        {
#ifdef WDT_ENABLE
          HAL_Clear_WatchDog();
#endif
          w_watt_value1 = ProgramConstantWatts_DivWatt(WattsLevel);
          w_watt_value2 = ProgramConstantWatts_DivWatt(WattsLevel+1);
          if(w_watt_value2 >= w_Watt_Target || w_watt_value1==w_watt_value2)
          {
            break;
          }
          WattsLevel++;
        }
        Data_Set_Resistance(WattsLevel,1);      
      }
      else
      {
        while(1)
        {
#ifdef WDT_ENABLE
          HAL_Clear_WatchDog();
#endif
          w_watt_value2 = ProgramConstantWatts_DivWatt(WattsLevel);
          w_watt_value1 = ProgramConstantWatts_DivWatt(WattsLevel-1);
          if(w_watt_value1 <= w_Watt_Target || w_watt_value2==w_watt_value1)
          {
            break;
          }
          WattsLevel--;
        }
        Data_Set_Resistance(WattsLevel,1);
      }
      if(_Watts_Talbelist->ResistanceType==INDUCTION_TYPE)
      {
        int pwm_small,pwm_big;
        pwm_small=InductionOutput_30P[WattsLevel-MinLevel];
        pwm_big=InductionOutput_30P[WattsLevel+1-MinLevel];
        PWM_Out = 
          ((int long)w_Watt_Target - w_watt_value1)*(pwm_big - pwm_small)
            /(w_watt_value2 - w_watt_value1)+pwm_small;
        Data_Set_TargetPWMValue(PWM_Out);
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
  
  if(by_Dat >= _Watts_Talbelist->Columns-1)
  {
    by_Dat=_Watts_Talbelist->Columns-1;   
    watt=_Watts_Talbelist->Watts_Table[(Res_Level-Min_Resistance) * _Watts_Talbelist->Columns + by_Dat];
  }
  else
  {
    int watt1 = _Watts_Talbelist->Watts_Table[(Res_Level-Min_Resistance) * (_Watts_Talbelist->Columns) + by_Dat];
    int watt2 = _Watts_Talbelist->Watts_Table[(Res_Level-Min_Resistance) * (_Watts_Talbelist->Columns) + by_Dat + 1];
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
#endif
