#include"Pro_manual.h"

#ifdef __PROGRAM_MANUAL__
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
}Manual;
__no_init static Manual g_Manual;
void Manual_Data_Initial(void)
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
void Manual_BeginWorkout(void)
{
    Data_Set_targetCalories(0);
    Data_Set_targetDistancex1000(0);
}
UINT16 Manual_SetUpData(UINT8 ParaIndex)
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
		default:break;
     	}
	 return w_Dat;
}
UINT16 Manual_RunData(UINT8 ParaIndex)
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
PROGRAM_PENDING_EVTS Manual_SetUp(ProgramKeyEvts *keyevt)
{
    UINT32 l_Dat = 0;
    PROGRAM_PENDING_EVTS pendingEvt;
    pendingEvt.evt=PROGRAM_NONE_EVENT;
    memset(&pendingEvt.WorkOutEvts,0,sizeof(pendingEvt.WorkOutEvts));
    if(keyevt->Bit_All == 0) return pendingEvt;
    
    if(keyevt->Bit_Flag.Key_Enter)
    {
        keyevt->Bit_Flag.Key_Enter=false;
        if(g_Manual.StruProgram.Setting_Array[g_Manual.StruProgram.SettingArray_Index] != SET_PRESS_START )
        {
            keyevt->Bit_Flag.Key_StateChange=true;
            g_Manual.StruProgram.SettingArray_Index++;
            if(SET_TIME==g_Manual.StruProgram.Setting_Array[g_Manual.StruProgram.SettingArray_Index])
            {
                Data_Set_Time(30*60);
            }
        }
    }
    else if(keyevt->Bit_Flag.Key_Back)
    {
        keyevt->Bit_Flag.Key_Back=false;
        if(g_Manual.StruProgram.Setting_Array[g_Manual.StruProgram.SettingArray_Index] != NO_SETTING )
        {
            keyevt->Bit_Flag.Key_StateChange=true;
            if(SET_PROGRAM==g_Manual.StruProgram.Setting_Array[g_Manual.StruProgram.SettingArray_Index])
            {
                Data_Set_Time(0);
            }
            else if(SET_TIME==g_Manual.StruProgram.Setting_Array[g_Manual.StruProgram.SettingArray_Index])
            {
                Data_Set_Time(0);
            }
            else if(SET_SPEED==g_Manual.StruProgram.Setting_Array[g_Manual.StruProgram.SettingArray_Index])
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
            else if(SET_INCLINE==g_Manual.StruProgram.Setting_Array[g_Manual.StruProgram.SettingArray_Index])
            {
                Data_Set_Incline(0,0);
            }
            else if(SET_RESISTANCE==g_Manual.StruProgram.Setting_Array[g_Manual.StruProgram.SettingArray_Index])
            {
                Data_Set_Resistance(Data_Get_Resistance(MIN_RESISTANCE),0);
            }
            g_Manual.StruProgram.SettingArray_Index--;
        }
    }
	g_Manual.ManualData.SettingState=g_Manual.StruProgram.Setting_Array[g_Manual.StruProgram.SettingArray_Index];
    switch(g_Manual.ManualData.SettingState)
    {
    case SET_PROGRAM:
      if(keyevt->Bit_Flag.Key_StateChange)
      {
        keyevt->Bit_Flag.Key_StateChange=false;
        pendingEvt.evt=PROGRAM_CHANGE_EVENT;
		g_Manual.StruProgram.Prg_NextStatus(PS_SELECTPRG);
        Manual_Data_Initial();
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
        if(Data_Get_Time(TARGET_TIME)==0)
        {
            Data_Set_Time(5*60);
        }
//              else if(Data_Get_Time(TARGET_TIME)<99*60)
      else if((Data_Get_Time(TARGET_TIME)!=0)&&(Data_Get_Time(TARGET_TIME)<99*60)) 
        {
            Data_Set_Time(Data_Get_Time(TARGET_TIME)+60);
        }            
      else  
        Data_Set_Time(0*60);  
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
      else  
        Data_Set_Time(99*60);
        pendingEvt.evt=TIME_CHANGE_EVENT;
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
    case SET_INCLINE:
      if(keyevt->Bit_Flag.Key_StateChange)
      {
        keyevt->Bit_Flag.Key_StateChange=false;
        pendingEvt.evt=INCLINE_CHANGE_EVENT;
      }
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
      break;
    case SET_RESISTANCE:
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
        g_Manual.ManualData.SegmentTime = 6000;  
    }
    else
    {
        l_Dat = (UINT32)Data_Get_Time(TARGET_TIME);
        l_Dat *= 100;               
        l_Dat /= ProgramLibConfigPara->DotMatrixLength;
        g_Manual.ManualData.SegmentTime = l_Dat;
    }	
    return pendingEvt; 
}

PROFILE *Manual_ProfileShow(PROFILE_TAG tag_index,UCHAR* Dotmatrix_pos)
{
    *Dotmatrix_pos=g_Manual.ManualData.Stage%ProgramLibConfigPara->DotMatrixLength;
    
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
			g_Manual.Profile.ProfileData=Prg_ProfileData.SpeedProfile;
			break;
            case INCLINE_PROFILE:
			g_Manual.Profile.ProfileData=Prg_ProfileData.InclineProfile;
			break;
            case DEFAULT_PROFILE:
			g_Manual.Profile.ProfileData=Prg_ProfileData.SpeedProfile;
			break;
			default:
			g_Manual.Profile.ProfileData=Prg_ProfileData.SpeedProfile;
            break;
  		}
  	}
    else
  	{
        switch(tag_index)
  		{
            case RESISTANCE_PROFILE:
			g_Manual.Profile.ProfileData=Prg_ProfileData.ResistanceProfile;
			break;
            case INCLINE_PROFILE:
			g_Manual.Profile.ProfileData=Prg_ProfileData.InclineProfile;
			break;
            case DEFAULT_PROFILE:
			g_Manual.Profile.ProfileData=Prg_ProfileData.ResistanceProfile;
			break;
			default:
			g_Manual.Profile.ProfileData=Prg_ProfileData.ResistanceProfile;
            break;
  		}  
	}
	g_Manual.Profile.Length=ProgramLibConfigPara->DotMatrixLength;
	return &g_Manual.Profile;
}



PROGRAM_PENDING_EVTS Manual_Run(ProgramKeyEvts *keyevt)
{  
    PROGRAM_PENDING_EVTS pendingEvt;
    static UINT16 LastValue=0xffff;
    UINT32 l_Dat = 0;
    pendingEvt.evt=PROGRAM_NONE_EVENT;
    memset(&pendingEvt.WorkOutEvts,0,sizeof(pendingEvt.WorkOutEvts));
    
    if(g_Manual.StruProgram.Prg_CurrentStatus()<PS_WARMUP
       ||g_Manual.StruProgram.Prg_CurrentStatus()>PS_COOLDOWN)
	{
		return pendingEvt;
	}
    {
        if(LastValue!=Data_Get_Time(ELAPSED_TIME))
        {
            LastValue=Data_Get_Time(ELAPSED_TIME);
            if(Data_Get_Time(TARGET_TIME) == 0) //counting up
            {
                if(Data_Get_Time(ELAPSED_TIME) < MAX_WORKOUT_TIME -1) //data.c to handle this
                {
                    l_Dat = (UINT32)Data_Get_Time(ELAPSED_TIME);
                    l_Dat *= 100;
                    l_Dat /=  g_Manual.ManualData.SegmentTime;
                    l_Dat %= ProgramLibConfigPara->DotMatrixLength;
                    g_Manual.ManualData.Stage = (UINT8)l_Dat; //This truncation is safe.
                    g_Manual.StruProgram.Prg_NextStatus(PS_RUN);
                }
            }
            else   //counting down
            {
               if(Data_Get_Time(REMAINING_TIME) == 0)
                {
                    if(g_Manual.StruProgram.Prg_CurrentStatus()!=PS_COMPLETED)
                    {
                        g_Manual.StruProgram.Prg_NextStatus(PS_COMPLETED);
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
                        l_Dat /= g_Manual.ManualData.SegmentTime;
                        g_Manual.ManualData.Stage = (UINT8)l_Dat; //safe truncation
                    }
                    else
                        g_Manual.ManualData.Stage = 0;
                    g_Manual.StruProgram.Prg_NextStatus(PS_RUN);
                    
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
    }
    return pendingEvt;
}

void initManual(void)
{
    
	g_Manual.StruProgram.Programid = PS_MANUAL;
	g_Manual.StruProgram.SettingArray_Index= 0;
	
	g_Manual.StruProgram.Prg_Initial = Manual_Data_Initial;
	g_Manual.StruProgram.BeginWorkout=Manual_BeginWorkout;
	g_Manual.StruProgram.Prg_Setup = Manual_SetUp;
	g_Manual.StruProgram.Prg_ProfileShow=Manual_ProfileShow;
	g_Manual.StruProgram.Prg_Workout = Manual_Run;
	g_Manual.StruProgram.Prg_SetupData=Manual_SetUpData;
	g_Manual.StruProgram.Prg_RunData=Manual_RunData;
	Program_Register_Function(&g_Manual.StruProgram);
	if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL )
	{
        g_Manual.StruProgram.Setting_Array=TM_Manual_SetUp;
	}
	else
    {
        if(SystemConfig_Get_Char(DRIVE_TYPE_UINT8) == EP_0B_JIS 
           || SystemConfig_Get_Char(DRIVE_TYPE_UINT8) == INDUCTOR_BRAKE_INCLINE)
            
            g_Manual.StruProgram.Setting_Array=EP_Manual_SetUp;                
        else            
            g_Manual.StruProgram.Setting_Array=EP_Manual_SetUp;
    }
    memset(&g_Manual.ManualData,0,sizeof(g_Manual.ManualData));
    g_Manual.ManualData.SegmentTime = 6000;  
    
}





#endif

