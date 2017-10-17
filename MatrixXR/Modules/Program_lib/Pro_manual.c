#include"Pro_manual.h"

#ifdef __PROGRAM_MANUAL__

__no_init static PROFILE Profile;
__no_init static const UCHAR *Setting_Array;

void Manual_Data_Initial(void)
{
//  _ul_Segment_Time=0;
  w_Current_Segment=0;
  Data_Set_Speed(Get_User_Unit()==METRIC?8:5,Get_User_Unit()==METRIC?METRIC:STANDARD,1); 
  Data_Set_Resistance(SystemConfig_Get_Char(MIN_RESISTANCE_UINT8),1);
  Data_Set_Incline(0,1);      
  Data_Set_WarmUP_Time(0);
  Data_Set_CoolDown_Time(0);
    if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
        DefaultDotmatrix=SPEED_TYPE;
    else
        DefaultDotmatrix=RESISTANCE_TYPE;
//  memset(&DotmatrixData,0x00,sizeof(PROFILE_INEER));
}

PROGRAM_PENDING_EVTS Manual_SetUp(ProgramKeyEvts *keyevt)
{
      UINT32 ul_Temp = 0;
  PROGRAM_PENDING_EVTS pendingEvt;
  pendingEvt.evt=PROGRAM_NONE_EVENT;
  pendingEvt.WorkOutEvts.AllEvts=0;
  //if(g_StateMachine!=PS_SETUP) return pendingEvt; 
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
        if(SET_PROGRAM==Setting_Array[Setting_Index])
        {
          Data_Set_Time(0);
        }
        else if(SET_TIME==Setting_Array[Setting_Index])
        {
          Data_Set_Time(0);
        }
        else if(SET_SPEED==Setting_Array[Setting_Index])
        {
          Data_Set_Speed(Get_User_Unit()==METRIC?SystemConfig_Get_Int(MIN_KPHX10_UINT16):SystemConfig_Get_Int(MIN_MPHX10_UINT16),Get_User_Unit()==METRIC?METRIC:STANDARD,1);
        }
        else if(SET_INCLINE==Setting_Array[Setting_Index])
        {
          Data_Set_Incline(0,1);
        }
        else if(SET_RESISTANCE==Setting_Array[Setting_Index])
        {
          Data_Set_Resistance(Data_Get_Resistance(MIN_RESISTANCE),1);
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
        Manual_Data_Initial();
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
    case SET_SPEED:
      if(keyevt->EventBits.Key_StateChange)
      {
        keyevt->EventBits.Key_StateChange=false;
        pendingEvt.evt=SPEED_CHANGE_EVENT;
      }
      if(keyevt->EventBits.Key_Up)
      {
//        if(Data_Get_Speed(TARGET_SPEED)<Data_Get_Speed(MAX_SPEED))
          Data_Set_Speed(Program_Get_Speed(DATA_AUTO,0)+1,Get_User_Unit()==METRIC?METRIC:STANDARD,1);
//        else
//          Data_Set_Speed(Get_User_Unit()==METRIC?SystemConfig_Get_Int(MIN_KPHX10_UINT16):SystemConfig_Get_Int(MIN_MPHX10_UINT16),Get_User_Unit()==METRIC?METRIC:STANDARD,1);
        
        pendingEvt.evt=SPEED_CHANGE_EVENT;
      }
      else if(keyevt->EventBits.Key_Dn)
      {
        
//        if(Data_Get_Speed(TARGET_SPEED)>Data_Get_Speed(MIN_SPEED))
          Data_Set_Speed(Program_Get_Speed(DATA_AUTO,0)-1,Get_User_Unit()==METRIC?METRIC:STANDARD,1);
//        else
//          Data_Set_Speed(Get_User_Unit()==METRIC?SystemConfig_Get_Int(MAX_KPHX10_UINT16):SystemConfig_Get_Int(MAX_MPHX10_UINT16),Get_User_Unit()==METRIC?METRIC:STANDARD,1);
        pendingEvt.evt=SPEED_CHANGE_EVENT;
      }      
      break;
    case SET_INCLINE:
      if(keyevt->EventBits.Key_StateChange)
      {
        keyevt->EventBits.Key_StateChange=false;
        pendingEvt.evt=INCLINE_CHANGE_EVENT;
      }
      if(keyevt->EventBits.Key_Up)
      {
        if(Data_Get_Incline(TARGET_INCLINEX10)<Data_Get_Incline(MAX_INCLINEX10))
          Data_Set_Incline(Program_Get_Incline(0)+5,1);
//        else
//          Data_Set_Incline(0,1);
        
        pendingEvt.evt=INCLINE_CHANGE_EVENT;
      }
      else if(keyevt->EventBits.Key_Dn)
      {
        if(Program_Get_Incline(0))
          Data_Set_Incline(Program_Get_Incline(0)-5,1);
//        else
//          Data_Set_Incline(Data_Get_Incline(MAX_INCLINEX10),1);
        pendingEvt.evt=INCLINE_CHANGE_EVENT;
      }   
      break;
    case SET_RESISTANCE:
      if(keyevt->EventBits.Key_StateChange)
      {
        keyevt->EventBits.Key_StateChange=false;
        if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==CLIMB_MILL)
        {
            pendingEvt.evt=CLIMBMILL_LEVEL_CHANGE_EVENT;
        }
        else
        {
            pendingEvt.evt=RESISTANCE_CHANGE_EVENT;
        }
      }
      if(keyevt->EventBits.Key_Up)
      {
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

PROFILE *Manual_GetShow(PROFILE_TAG tag_index,UCHAR* Dotmatrix_pos)
{
  *Dotmatrix_pos=w_Current_Segment%ProgramParameter->DotMatrixLength;

  for(UINT8 i = *Dotmatrix_pos; i < ProgramParameter->DotMatrixLength; i++)
  {
    DotmatrixData.SpeedProfile[i]=Program_Get_Speed(DATA_AUTO,0);
    DotmatrixData.InclineProfile[i]=Data_Get_Incline(TARGET_INCLINEX10);
    DotmatrixData.ResistanceProfile[i]=Data_Get_Resistance(TARGET_RESISTANCE);
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
			Profile.ProfileData=DotmatrixData.SpeedProfile;
			break;
			default:
			Profile.ProfileData=DotmatrixData.SpeedProfile;
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
			Profile.ProfileData=DotmatrixData.ResistanceProfile;
			break;
			default:
			Profile.ProfileData=DotmatrixData.ResistanceProfile;
				break;
  		}  
	}
	Profile.Length=ProgramParameter->DotMatrixLength;
	return &Profile;
}



PROGRAM_PENDING_EVTS Manual_Run(ProgramKeyEvts *keyevt)
{  
  PROGRAM_PENDING_EVTS pendingEvt;
  static UINT16 WorkOut_Old_Time=1;
  pendingEvt.evt=PROGRAM_NONE_EVENT;
  pendingEvt.WorkOutEvts.AllEvts=0;
  
  UINT32 ul_Temp = 0;
  if(g_StateMachine<PS_WARMUP||g_StateMachine>PS_COOLDOWN)return pendingEvt;
  {
      if(WorkOut_Old_Time!=Data_Get_Time(ELAPSED_TIME))
      {
          WorkOut_Old_Time=Data_Get_Time(ELAPSED_TIME);
//						pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
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
              }
          }
          else   //counting down
          {
              if(Data_Get_Time(REMAINING_TIME) == 0)
              {
                  if(g_StateMachine!=PS_COMPLETED)
                  {
                      g_StateMachine=PS_COMPLETED;
                      //          Data_Set_Event(DATA_COMPLETE_EVENT);
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
  }
return pendingEvt;
}

void initManual(void)
{
	struProgram manual;
        
	manual.Programid = PS_MANUAL;
	manual.Initial = Manual_Data_Initial;
	manual.Setup = Manual_SetUp;
	manual.GetProfile=Manual_GetShow;
	manual.Workout = Manual_Run;
	
	Program_Add_Array(manual);
//        Setting_Index=0;
	if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL )
	{
          Setting_Array=TM_Manual_SetUp;
	}
	else
        {
    if(SystemConfig_Get_Char(DRIVE_TYPE_UINT8) == EP_0B_JIS 
               || SystemConfig_Get_Char(DRIVE_TYPE_UINT8) == INDUCTOR_BRAKE_INCLINE)
                
          Setting_Array=EP_Incline_Manual_SetUp;                
            else            
                Setting_Array=EP_Manual_SetUp;
        }
}





#endif

