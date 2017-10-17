#include"Pro_hrc.h"

#ifdef __PROGRAM_HRC__


#define HR_WARMUP_TIME 240
#define HR_COOLDOWN_TIME 240

#define CHECK_TIME_2S 2	
#define CHECK_TIME_3S 3	
#define CHECK_TIME_5S 5	
#define CHECK_TIME_8S 8	

#define CHECK_TIME_10S 10	
#define CHECK_TIME_35S 35	

#define THR_TOTAL_SEGMENT 14

__no_init UINT16 w_Total_Segment;
__no_init static PROFILE Profile;
__no_init UINT16 w_Dat,w_SetTotal_Time;

__no_init static const unsigned char *Setting_Array;
__no_init static UCHAR Hrc_TargetHeart;
__no_init static UCHAR Hrc_Heart[4];
__no_init static UINT16 Hrc_Time[2];
__no_init static HC_STATUS Hrc_Status_Speed,Hrc_Status_Incline,Hrc_Status_Resistance;
__no_init static struct 
{
  UINT8 No_Heart;
  UINT8 TenSecondCounter;
  UINT8 Spd_Time_Count;
  UINT8 Inc_Time_Count;
  UINT8 Res_Time_Count;
  UINT8 Spd_Time_Limit;
  UINT8 Inc_Time_Limit;
  UINT8 Res_Time_Limit;
}CheckTime;
__no_init static struct 
{
  UINT8 Max_Heart;
  UINT8 WorkoutModTime;
  UINT8 Heart_Big25_WorkoutEnd:1;
}PHrc;
void PHrc_Add_Time(void)
{

  switch(Program_Get_ProgramID())
  {
  case PS_THR_ZONE:
      switch(Setting_Array[Setting_Index])
      {
      case SET_TIME:
        if(Data_Get_Time(WORKOUT_TIME)==99*60)
        {
          Data_Set_WorkOutTime(99*60);
        }
        else
        {
          Data_Set_WorkOutTime(Data_Get_Time(WORKOUT_TIME)+60);
        }
        break;
      }
      break;
  case PS_INTERVALS_HR:
      switch(Setting_Array[Setting_Index])
      {
      case SET_TIME:
        if(Data_Get_Time(TARGET_TIME)==99*60)
        {
          Data_Set_Time(99*60);
        }
        else
        {
          Data_Set_Time(Data_Get_Time(TARGET_TIME)+60);
        }
        break;
      case SET_WORKINTERVAL_TIME:
        if(Hrc_Time[0]<5*60)
          Hrc_Time[0]+=60;
        else
          Hrc_Time[0]=5*60;
        break;
      case SET_RESTINTERVAL_TIME:
        if(Hrc_Time[1]<5*60)
          Hrc_Time[1]+=60;
        else
          Hrc_Time[1]=5*60;
        break;
      }
      break;
  case PS_STEPS_THR:
      switch(Setting_Array[Setting_Index])
      {
      case SET_TIME:
        if(Data_Get_Time(TARGET_TIME)==99*60)
        {
          Data_Set_Time(99*60);
        }
        else
        {
          Data_Set_Time(Data_Get_Time(TARGET_TIME)+60);
        }
        break;
      case SET_SEGMENT_TIME:
        if(Hrc_Time[0]<10*60)
          Hrc_Time[0]+=60;
        else
          Hrc_Time[0]=10*60;
        break;
      }
      break;
  } 
}
void PHrc_Dec_Time(void)
{

  switch(Program_Get_ProgramID())
  {
  case PS_THR_ZONE:
      switch(Setting_Array[Setting_Index])
      {
      case SET_TIME:
        if(Data_Get_Time(WORKOUT_TIME)==5*60)
        {
          Data_Set_WorkOutTime(5*60);
        }
        else
        {
          Data_Set_WorkOutTime(Data_Get_Time(WORKOUT_TIME)-60);
        }
        break;
      }
      break;
  case PS_INTERVALS_HR:
      switch(Setting_Array[Setting_Index])
      {
      case SET_TIME:
        if(Data_Get_Time(TARGET_TIME)==5*60)
        {
          Data_Set_Time(5*60);
        }
        else
        {
          Data_Set_Time(Data_Get_Time(TARGET_TIME)-60);
        }
        break;
      case SET_WORKINTERVAL_TIME:
        if(Hrc_Time[0]<2*60)
          Hrc_Time[0]-=60;
        else
          Hrc_Time[0]=2*60;
        break;
      case SET_RESTINTERVAL_TIME:
        if(Hrc_Time[1]<2*60)
          Hrc_Time[1]-=60;
        else
          Hrc_Time[1]=2*60;
        break;
      }
      break;
  case PS_STEPS_THR:
      switch(Setting_Array[Setting_Index])
      {
      case SET_TIME:
        if(Data_Get_Time(TARGET_TIME)==5*60)
        {
          Data_Set_Time(5*60);
        }
        else
        {
          Data_Set_Time(Data_Get_Time(TARGET_TIME)-60);
        }
        break;
      case SET_SEGMENT_TIME:
        if(Hrc_Time[0]<3*60)
          Hrc_Time[0]-=60;
        else
          Hrc_Time[0]=3*60;
        break;
      }
      break;
  } 
}
void PHrc_Add_Heart(void)
{

  switch(Program_Get_ProgramID())
  {
  case PS_THR_ZONE:
      switch(Setting_Array[Setting_Index])
      {
      case SET_HEART:
//        if(Hrc_Heart[0]<PHrc.Max_Heart)
//          Hrc_Heart[0]+=5;
        if(Hrc_Heart[0]+5<PHrc.Max_Heart)
          Hrc_Heart[0]+=5;
        else
          Hrc_Heart[0]=PHrc.Max_Heart;
        break;
      }
      break;
  case PS_INTERVALS_HR:
      switch(Setting_Array[Setting_Index])
      {
      case SET_WORKINTERVAL_HEART:
        if(Hrc_Heart[0]<PHrc.Max_Heart)
          Hrc_Heart[0]+=5;
        break;
      case SET_RESTINTERVAL_HEART:
        if(Hrc_Heart[1]<PHrc.Max_Heart)
          Hrc_Heart[1]+=5;
        break;
      }
      break;
  case PS_STEPS_THR:
      switch(Setting_Array[Setting_Index])
      {
      case SET_SEGMENT_1_HEART:
        if(Hrc_Heart[0]<PHrc.Max_Heart)
          Hrc_Heart[0]+=5;
        break;
      case SET_SEGMENT_2_HEART:
        if(Hrc_Heart[1]<PHrc.Max_Heart)
          Hrc_Heart[1]+=5;
        break;
      case SET_SEGMENT_3_HEART:
        if(Hrc_Heart[2]<PHrc.Max_Heart)
          Hrc_Heart[2]+=5;
        break;
      case SET_SEGMENT_4_HEART:
        if(Hrc_Heart[3]<PHrc.Max_Heart)
          Hrc_Heart[3]+=5;
        break;
      }
      break;
  } 
}
void PHrc_Dec_Heart(void)
{

  switch(Program_Get_ProgramID())
  {
  case PS_THR_ZONE:
      switch(Setting_Array[Setting_Index])
      {
      case SET_HEART:
        if(Hrc_Heart[0]==PHrc.Max_Heart)
        {
          if(Hrc_Heart[0]%5 == 0)
          {
            Hrc_Heart[0] -= 5;
          }
          else
          {
            Hrc_Heart[0] = ((Hrc_Heart[0])/5 + 1) * 5;
          }
        }
        else
        {
        if(Hrc_Heart[0]-5>50)
          Hrc_Heart[0]-=5;
        else
          Hrc_Heart[0]=50;
        }
        break;
      }
      break;
  case PS_INTERVALS_HR:
      switch(Setting_Array[Setting_Index])
      {
      case SET_WORKINTERVAL_HEART:
        if(Hrc_Heart[0]>50)
          Hrc_Heart[0]-=5;
        break;
      case SET_RESTINTERVAL_HEART:
        if(Hrc_Heart[1]>50)
          Hrc_Heart[1]-=5;
        break;
      }
      break;
  case PS_STEPS_THR:
      switch(Setting_Array[Setting_Index])
      {
      case SET_SEGMENT_1_HEART:
        if(Hrc_Heart[0]>50)
          Hrc_Heart[0]-=5;
        break;
      case SET_SEGMENT_2_HEART:
        if(Hrc_Heart[1]>50)
          Hrc_Heart[1]-=5;
        break;
      case SET_SEGMENT_3_HEART:
        if(Hrc_Heart[2]>50)
          Hrc_Heart[2]-=5;
        break;
      case SET_SEGMENT_4_HEART:
        if(Hrc_Heart[3]>50)
          Hrc_Heart[3]-=5;
        break;
      }
      break;
  } 
}
UCHAR PHrc_Get_Heart(UCHAR by_Index)
{
  return Hrc_Heart[by_Index];
}
UINT16 PHrc_Get_Time(UCHAR by_Index)
{
  return Hrc_Time[by_Index];
}
void PHrc_Speed_Adjust(void)
{
  UINT8 by_Dat;
  if(CheckTime.Spd_Time_Count<100)CheckTime.Spd_Time_Count++;
  if(Data_Get_Heart(CURRENT_HEART)<Hrc_TargetHeart)
  {
    by_Dat=Hrc_TargetHeart-Data_Get_Heart(CURRENT_HEART);
    if(by_Dat>=12)
    {
      CheckTime.Spd_Time_Limit=CHECK_TIME_3S;
    }
    else if(by_Dat>=7)
    {
      CheckTime.Spd_Time_Limit=CHECK_TIME_5S;
    }
    else
    {
      CheckTime.Spd_Time_Limit=CHECK_TIME_8S;
    }    
  }
  else
  {
    by_Dat=Data_Get_Heart(CURRENT_HEART)-Hrc_TargetHeart;
    if(by_Dat>=16)
    {
      CheckTime.Spd_Time_Limit=CHECK_TIME_2S;
    }
    else if(by_Dat>=12)
    {
      CheckTime.Spd_Time_Limit=CHECK_TIME_3S;
    }
    else if(by_Dat>=7)
    {
      CheckTime.Spd_Time_Limit=CHECK_TIME_5S;
    }
    else
    {
      CheckTime.Spd_Time_Limit=CHECK_TIME_8S;
    } 
  }
  if(CheckTime.Spd_Time_Limit>CheckTime.Spd_Time_Count)
  {
    return;
  }
  CheckTime.Spd_Time_Count=0;
  if(Data_Get_Heart(CURRENT_HEART)<Hrc_TargetHeart)
  {
    by_Dat=Hrc_TargetHeart-Data_Get_Heart(CURRENT_HEART);
    if(by_Dat>=12)
    {
      Hrc_Status_Speed=HC_LESS_12_SPD;
    }
    else if(by_Dat>=7)
    {
      Hrc_Status_Speed=HC_LESS_7_11_SPD;
    }
    else if(by_Dat>=4)
    {
      Hrc_Status_Speed=HC_LESS_4_6_SPD;
    }
    else
    {
      Hrc_Status_Speed=HC_NONE_SPD;
    }
  }
  else
  {
    by_Dat=Data_Get_Heart(CURRENT_HEART)-Hrc_TargetHeart;
    if(by_Dat>=25)
    {
      Hrc_Status_Speed=HC_MORE_25_SPD;
    }
    else if(by_Dat>=16)
    {
      Hrc_Status_Speed=HC_MORE_16_24_SPD;
    }
    else if(by_Dat>=12)
    {
      Hrc_Status_Speed=HC_MORE_12_15_SPD;
    }
    else if(by_Dat>=7)
    {
      Hrc_Status_Speed=HC_MORE_7_11_SPD;
    }
    else if(by_Dat>=4)
    {
      Hrc_Status_Speed=HC_MORE_4_6_SPD;
    }
    else
    {
      Hrc_Status_Speed=HC_NONE_SPD;
    }  
  }
  switch(Hrc_Status_Speed)
  {
    case HC_NONE_SPD:
      break;
    case HC_LESS_4_6_SPD:
    case HC_LESS_7_11_SPD:
    case HC_LESS_12_SPD:
// 	Program_Set_VirtualKey(VK_SPEEDUP);
    Beep_Set_Beeps(1,3,1);
      Data_Set_Speed(Program_Get_Speed(DATA_AUTO,0)+1,Get_User_Unit()==METRIC?METRIC:STANDARD,1);
      break;
    case HC_MORE_4_6_SPD:
    case HC_MORE_7_11_SPD:
    case HC_MORE_12_15_SPD:
    case HC_MORE_16_24_SPD:
// 	Program_Set_VirtualKey(VK_SPEEDDOWN);
    Beep_Set_Beeps(1,3,1);
      Data_Set_Speed(Program_Get_Speed(DATA_AUTO,0)-1,Get_User_Unit()==METRIC?METRIC:STANDARD,1);
      break;
    case HC_MORE_25_SPD:
      Data_Set_Speed(0,Get_User_Unit()==METRIC?METRIC:STANDARD,1);
      Program_Set_VirtualKey(VK_RESET);
      
      break;
      
  }
}
void PHrc_Incline_Adjust(void)
{
  UINT8 by_Dat;
  if(CheckTime.Inc_Time_Count<100)CheckTime.Inc_Time_Count++;
  if(Data_Get_Heart(CURRENT_HEART)<Hrc_TargetHeart)
  {
    by_Dat=Hrc_TargetHeart-Data_Get_Heart(CURRENT_HEART);  
    if(by_Dat>=20)
    {
      CheckTime.Inc_Time_Limit=CHECK_TIME_10S;
    }
    else
    {
      CheckTime.Inc_Time_Limit=CHECK_TIME_35S;
    }
  }
  else
  {
    by_Dat=Data_Get_Heart(CURRENT_HEART)-Hrc_TargetHeart;
    if(by_Dat>=20)
    {
      CheckTime.Inc_Time_Limit=CHECK_TIME_10S;
    }
    else if(by_Dat>=11)
    {
      CheckTime.Inc_Time_Limit=CHECK_TIME_10S;
    }
    else
    {
      CheckTime.Inc_Time_Limit=CHECK_TIME_35S;
    }
  }
  if(CheckTime.Inc_Time_Limit>CheckTime.Inc_Time_Count)
  {
    return;
  }
  CheckTime.Inc_Time_Count=0;
  if(Data_Get_Heart(CURRENT_HEART)<Hrc_TargetHeart)
  {
    by_Dat=Hrc_TargetHeart-Data_Get_Heart(CURRENT_HEART);
    if(by_Dat>=20)
    {
      Hrc_Status_Incline=HC_LESS_20_INC;
    }
    else if(by_Dat>=6)
    {
      Hrc_Status_Incline=HC_LESS_6_19_INC;
    }
    else
    {
      Hrc_Status_Incline=HC_NONE_INC;
    }
  }
  else
  {
    by_Dat=Data_Get_Heart(CURRENT_HEART)-Hrc_TargetHeart;
    if(by_Dat>=25)
    {
      Hrc_Status_Incline=HC_MORE_25_INC;
    }
    else if(by_Dat>=20)
    {
      Hrc_Status_Incline=HC_MORE_20_24_INC;
    }
    else if(by_Dat>=11)
    {
      Hrc_Status_Incline=HC_MORE_11_19_INC;
    }
    else if(by_Dat>=6)
    {
      Hrc_Status_Incline=HC_MORE_6_10_INC;
    }
    else
    {
      Hrc_Status_Incline=HC_NONE_INC;
    }  
  }
  switch(Hrc_Status_Incline)
  {
    case HC_NONE_INC:
      break;
    case HC_LESS_6_19_INC:
    case HC_LESS_20_INC:
// 	Program_Set_VirtualKey(VK_INCLINEUP);
      Data_Set_Incline(Program_Get_Incline(0)+5,1);
      break;
    case HC_MORE_6_10_INC:
    case HC_MORE_11_19_INC:
    case HC_MORE_20_24_INC:
//	Program_Set_VirtualKey(VK_INCLINEDOWN);
      Data_Set_Incline(Program_Get_Incline(0)-5,1);
      break;
  case HC_MORE_25_INC:
	Data_Set_Incline(0,1);
      break;
  }
}
void PHrc_Resistance_Adjust(void)
{
  UINT8 by_Dat;
  if(CheckTime.Res_Time_Count<100)CheckTime.Res_Time_Count++;
  if(Data_Get_Heart(CURRENT_HEART)<Hrc_TargetHeart)
  {
    by_Dat=Hrc_TargetHeart-Data_Get_Heart(CURRENT_HEART);  
    if(by_Dat>=20)
    {
      CheckTime.Res_Time_Limit=CHECK_TIME_10S;
    }
    else
    {
      CheckTime.Res_Time_Limit=CHECK_TIME_35S;
    }
  }
  else
  {
    by_Dat=Data_Get_Heart(CURRENT_HEART)-Hrc_TargetHeart;
    if(by_Dat>=20)
    {
      CheckTime.Res_Time_Limit=CHECK_TIME_10S;
    }
    else if(by_Dat>=11)
    {
      CheckTime.Res_Time_Limit=CHECK_TIME_10S;
    }
    else
    {
      CheckTime.Res_Time_Limit=CHECK_TIME_35S;
    }
  }
  if(CheckTime.Res_Time_Limit>CheckTime.Res_Time_Count)
  {
    return;
  }
  CheckTime.Res_Time_Count=0;
  if(Data_Get_Heart(CURRENT_HEART)<Hrc_TargetHeart)
  {
    by_Dat=Hrc_TargetHeart-Data_Get_Heart(CURRENT_HEART);
    if(by_Dat>=20)
    {
      Hrc_Status_Resistance=HC_LESS_20_RES;
    }
    else if(by_Dat>=6)
    {
      Hrc_Status_Resistance=HC_LESS_6_19_RES;
    }
    else
    {
      Hrc_Status_Resistance=HC_NONE_RES;
    }
  }
  else
  {
    by_Dat=Data_Get_Heart(CURRENT_HEART)-Hrc_TargetHeart;
    if(by_Dat>=25)
    {
      Hrc_Status_Resistance=HC_MORE_25_RES;
    }
    else if(by_Dat>=20)
    {
      Hrc_Status_Resistance=HC_MORE_20_24_RES;
    }
    else if(by_Dat>=11)
    {
      Hrc_Status_Resistance=HC_MORE_11_19_RES;
    }
    else if(by_Dat>=6)
    {
      Hrc_Status_Resistance=HC_MORE_6_10_RES;
    }
    else
    {
      Hrc_Status_Resistance=HC_NONE_RES;
    }  
  }
  switch(Hrc_Status_Resistance)
  {
    case HC_NONE_RES:
      break;
    case HC_LESS_6_19_RES:
    case HC_LESS_20_RES:
    Beep_Set_Beeps(1,3,1);
//	Program_Set_VirtualKey(VK_RESISTANCEUP);
      Data_Set_Resistance(Program_Get_Resistance(0)+1,1);
      break;
    case HC_MORE_6_10_RES:
    case HC_MORE_11_19_RES:
    case HC_MORE_20_24_RES:
//	Program_Set_VirtualKey(VK_RESISTANCEUP);
    Beep_Set_Beeps(1,3,1);
      Data_Set_Resistance(Program_Get_Resistance(0)-1,1);
      break;
    case HC_MORE_25_RES:
	Data_Set_Resistance(0,1);
        PHrc.Heart_Big25_WorkoutEnd=1;
      break;
  } 
}
UCHAR PHrc_Get_HRTarget(void)
{
  switch(Program_Get_ProgramID())
  {
  case PS_THR_ZONE:
    Hrc_TargetHeart=Hrc_Heart[0];
      break;
  case PS_INTERVALS_HR:
      Hrc_TargetHeart=Hrc_Heart[(w_Current_Segment-1)%2];
    break;
  case PS_STEPS_THR:
      Hrc_TargetHeart=Hrc_Heart[(w_Current_Segment-1)%4];
    break;
  }
  return Hrc_TargetHeart;
}
void PHrc_Cul_Data(void)
{
  if(Data_Get_1s())
  {
      if(Data_Get_Heart(CURRENT_HEART)==0)
      {
          if(CheckTime.No_Heart<60)
          {
              CheckTime.No_Heart++;
              CheckTime.TenSecondCounter=0;
          }
          else
          {
              if(++CheckTime.TenSecondCounter>9)
              {
                  CheckTime.TenSecondCounter=0;
                  
                  if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
                  {
                      if(Get_User_Unit()==METRIC)
                      {
                          if(Program_Get_Speed(DATA_METRIC,0)>18)
                          {
                              Data_Set_Speed(Program_Get_Speed(DATA_METRIC,0)-10,METRIC,1);
                          }
                          else
                          {
                              Data_Set_Speed(8,METRIC,1);
                          }
                      }
                      else
                      {
                          if(Program_Get_Speed(DATA_METRIC,0)>11)
                          {
                              Data_Set_Speed(Program_Get_Speed(DATA_STANARD,0)-6,STANDARD,1);
                          }
                          else
                          {
                              Data_Set_Speed(5,STANDARD,1);
                          }
                      }
                  }
                  else 
                  {
                      Data_Set_Resistance(Program_Get_Resistance(0)-1,1);  
                  }
              }
          }
          return;
      }
      else
      {
          CheckTime.No_Heart=0;
      }
      if(g_StateMachine<=PS_WARMUP||g_StateMachine>=PS_COOLDOWN)
          return;
      PHrc_Get_HRTarget();
      if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
          PHrc_Speed_Adjust();
      //    PHrc_Incline_Adjust();
      else
          PHrc_Resistance_Adjust();
  }
}







void ProgramHrc_Data_Initial(void)
{
  _ul_Segment_Time=0;
  w_Current_Segment=0;
  w_Total_Segment=0;
//  memset(&DotmatrixData,0x00,sizeof(PROFILE_INEER));
//  Setting_Index=1;
  Hrc_TargetHeart=Hrc_Heart[0]=Hrc_Heart[1]=Hrc_Heart[2]=Hrc_Heart[3]=80;
  Hrc_Time[0]=Hrc_Time[1]=180;
    Data_Set_WarmUP_Time(HR_WARMUP_TIME);
  Data_Set_CoolDown_Time(HR_COOLDOWN_TIME);
  Data_Set_Time(1800);
  Data_Set_StageTime(HR_WARMUP_TIME);
  CheckTime.No_Heart=0;
  CheckTime.TenSecondCounter=0;
  CheckTime.Spd_Time_Count=0;
  CheckTime.Inc_Time_Count=0;
  CheckTime.Res_Time_Count=0;
  CheckTime.Spd_Time_Limit=0;
  CheckTime.Inc_Time_Limit=0;
  CheckTime.Res_Time_Limit=0;    
  PHrc.Heart_Big25_WorkoutEnd=0;
  PHrc.Max_Heart=190;
  PHrc.WorkoutModTime=0;
  if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
      DefaultDotmatrix=SPEED_TYPE;
  else
      DefaultDotmatrix=RESISTANCE_TYPE;
  Hrc_Status_Speed=HC_NONE_SPD;
  Hrc_Status_Incline=HC_NONE_INC;
  Hrc_Status_Resistance=HC_NONE_RES;
  switch(Program_Get_ProgramID())
  {
  case PS_THR_ZONE:
      Setting_Array=THR_ZONE_SetUp;
      PHrc.Max_Heart=220-User_Get_Age();
      Data_Set_WorkOutTime(30*60);
      break;
  case PS_INTERVALS_HR:
      Setting_Array=Interval_HR_SetUp;
    break;
  case PS_STEPS_THR:
      Setting_Array=Steps_HR_SetUp;
    break;
  }  

    switch(Program_Get_ProgramID())
    {
    case PS_THR_ZONE:
      w_Total_Segment=THR_TOTAL_SEGMENT;
      Hrc_Time[0]=Data_Get_Time(WORKOUT_TIME)/11;
      PHrc.WorkoutModTime=Data_Get_Time(WORKOUT_TIME)%11;
      break;
    case PS_INTERVALS_HR:
      w_Dat=(Data_Get_Time(TARGET_TIME)-HR_WARMUP_TIME-HR_COOLDOWN_TIME);
      w_SetTotal_Time=Hrc_Time[0]+Hrc_Time[1];
      w_Total_Segment=w_Dat/w_SetTotal_Time;
      w_Total_Segment *=2;
      w_Dat %=w_SetTotal_Time;
      if(w_Dat>Hrc_Time[0])
        w_Total_Segment +=2;
      else
        w_Total_Segment +=1;
      w_Total_Segment +=3;
      break;
    case PS_STEPS_THR:
      w_Dat=(Data_Get_Time(TARGET_TIME)-HR_WARMUP_TIME-HR_COOLDOWN_TIME);
      w_SetTotal_Time=Hrc_Time[0];
      w_Total_Segment =w_Dat/w_SetTotal_Time;
      w_Dat %=w_SetTotal_Time;
      if(w_Dat>Hrc_Time[0])
      {
        w_Total_Segment +=1;
      }
      w_Total_Segment +=3;
      break;
    }	
}


PROGRAM_PENDING_EVTS ProgramHrc_SetUp(ProgramKeyEvts *keyevt)
{
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
            if(Program_Get_ProgramID()==PS_THR_ZONE)
              Data_Set_WorkOutTime(30*60);
            else 
              Data_Set_Time(1800);
          }
          else if(SET_HEART==Setting_Array[Setting_Index])
          {
              Hrc_Heart[0]=Hrc_Heart[1]=Hrc_Heart[2]=Hrc_Heart[3]=80;
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
        ProgramHrc_Data_Initial();
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
      PHrc_Add_Time();
        pendingEvt.evt=TIME_CHANGE_EVENT;
      }
      else if(keyevt->EventBits.Key_Dn)
      {
            PHrc_Dec_Time();
        pendingEvt.evt=TIME_CHANGE_EVENT;
      }
      break;
    case SET_HEART:
      if(keyevt->EventBits.Key_StateChange)
      {
        keyevt->EventBits.Key_StateChange=false;
        pendingEvt.evt=HEART_CHANGE_EVENT;

      }
      if(keyevt->EventBits.Key_Up)
      {
      PHrc_Add_Heart();
        pendingEvt.evt=HEART_CHANGE_EVENT;
      }
      else if(keyevt->EventBits.Key_Dn)
      {
            PHrc_Dec_Heart();
        pendingEvt.evt=HEART_CHANGE_EVENT;
      }
      break;
    case SET_PRESS_START:
      if(keyevt->EventBits.Key_StateChange)
      {
        keyevt->EventBits.Key_StateChange=false;
        pendingEvt.evt=SETTING_COMPLETE_EVENT;
      }    
    }
    switch(Program_Get_ProgramID())
    {
    case PS_THR_ZONE:
      w_Total_Segment=THR_TOTAL_SEGMENT;
      Hrc_Time[0]=Data_Get_Time(WORKOUT_TIME)/11;
      PHrc.WorkoutModTime=Data_Get_Time(WORKOUT_TIME)%11;
      break;
    case PS_INTERVALS_HR:
      w_Dat=(Data_Get_Time(TARGET_TIME)-HR_WARMUP_TIME-HR_COOLDOWN_TIME);
      w_SetTotal_Time=Hrc_Time[0]+Hrc_Time[1];
      w_Total_Segment=w_Dat/w_SetTotal_Time;
      w_Total_Segment *=2;
      w_Dat %=w_SetTotal_Time;
      if(w_Dat>Hrc_Time[0])
        w_Total_Segment +=2;
      else
        w_Total_Segment +=1;
      w_Total_Segment +=3;
      break;
    case PS_STEPS_THR:
      w_Dat=(Data_Get_Time(TARGET_TIME)-HR_WARMUP_TIME-HR_COOLDOWN_TIME);
      w_SetTotal_Time=Hrc_Time[0];
      w_Total_Segment =w_Dat/w_SetTotal_Time;
      w_Dat %=w_SetTotal_Time;
      if(w_Dat>Hrc_Time[0])
      {
        w_Total_Segment +=1;
      }
      w_Total_Segment +=3;
      break;
    }	
    
    return pendingEvt; 
}

PROFILE *ProgramHrc_GetShow(PROFILE_TAG tag_index,UCHAR* Dotmatrix_pos)
{
  if(ProgramParameter->DotMatrixLength+w_Current_Segment<=w_Total_Segment)
  {
    *Dotmatrix_pos=0;
  }
  else
  {
    *Dotmatrix_pos=ProgramParameter->DotMatrixLength+w_Current_Segment-w_Total_Segment;
  }
  for(UCHAR by_Loop=*Dotmatrix_pos;by_Loop<ProgramParameter->DotMatrixLength;by_Loop++)
  {
    DotmatrixData.SpeedProfile[by_Loop]=Program_Get_Speed(DATA_AUTO,0);
    DotmatrixData.InclineProfile[by_Loop]=Data_Get_Incline(TARGET_INCLINEX10);
    DotmatrixData.ResistanceProfile[by_Loop]=Data_Get_Resistance(TARGET_RESISTANCE);
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



PROGRAM_PENDING_EVTS ProgramHrc_Run(ProgramKeyEvts *keyevt)
{  
  UINT16 w_Dat;
  static UINT16 WorkOut_Old_Time=1;
  PROGRAM_PENDING_EVTS pendingEvt;
  pendingEvt.evt=PROGRAM_NONE_EVENT;
  pendingEvt.WorkOutEvts.AllEvts=0;
  if(g_StateMachine<PS_WARMUP||g_StateMachine>PS_COOLDOWN)return pendingEvt;
 
  if(WorkOut_Old_Time!=Data_Get_Time(ELAPSED_TIME))
  {
      WorkOut_Old_Time=Data_Get_Time(ELAPSED_TIME);
//				pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
//	pendingEvt.WorkOutEvts.EvtBit.Workout_Refresh_Data=true;	

      PHrc_Cul_Data();
      if(Data_Get_Time(ELAPSED_TIME)>=Data_Get_Time(TARGET_TIME)||(PHrc.Heart_Big25_WorkoutEnd==1))
      {
          if(g_StateMachine!=PS_COMPLETED)
          {
              g_StateMachine=PS_COMPLETED;
              //      Data_Set_Event(DATA_COMPLETE_EVENT);
              pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
              pendingEvt.WorkOutEvts.EvtBit.Workout_Complete_Evt=true;
          }
      }
      else
      {
        if(Data_Get_Time(ELAPSED_TIME)<HR_WARMUP_TIME)
          {
              g_StateMachine=PS_WARMUP;
        }
        else if(Data_Get_Time(REMAINING_TIME)<=HR_COOLDOWN_TIME)
        {
          g_StateMachine=PS_COOLDOWN;
        }
        else
        {
          g_StateMachine=PS_RUN;
        }
        if(Data_Get_Time(ELAPSED_TIME)==0)
        {
              pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
              pendingEvt.WorkOutEvts.EvtBit.Workout_Warmup_Evt=true;
          }
          else if(Data_Get_Time(ELAPSED_TIME)==HR_WARMUP_TIME)
          {
          
              pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
              pendingEvt.WorkOutEvts.EvtBit.Workout_Run_Evt=true;
          }
          else if(Data_Get_Time(REMAINING_TIME)==HR_COOLDOWN_TIME)
          {
              if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
              {
                  Data_Set_Speed(Program_Get_Speed(DATA_AUTO,0)/2,Get_User_Unit()==METRIC?METRIC:STANDARD,1);
              }
              else
              {
                  Data_Set_Resistance(Program_Get_Resistance(0)/2,1);
              }
              pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
              pendingEvt.WorkOutEvts.EvtBit.Workout_Cooldown_Evt=true;
          }
          else if(Data_Get_Time(REMAINING_TIME)==HR_COOLDOWN_TIME/2)
          {
             if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
              {
                  Data_Set_Speed(0,Get_User_Unit()==METRIC?METRIC:STANDARD,1);
              }
              else
              {
                  Data_Set_Resistance(0,1);
              }
              pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
          }
          if(Data_Get_Time(TIME_IN_STAGE)==Data_Get_Time(STAGE_TIME))
          {
              w_Current_Segment++;
              switch(Program_Get_ProgramID())
              {
                  case PS_THR_ZONE:
                  w_Dat=Hrc_Time[0];
                  if(PHrc.WorkoutModTime!=0)
                  {
                    if(w_Current_Segment==11)
                    {
                      w_Dat +=PHrc.WorkoutModTime;
                    }
                  }
                  break;
                  case PS_INTERVALS_HR:
                  w_Dat=Hrc_Time[(w_Current_Segment-1)%2];
                  break;
                  case PS_STEPS_THR:
                  w_Dat=Hrc_Time[(w_Current_Segment-1)%4];
                  break;
              }  
              
              if(Data_Get_Time(REMAINING_TIME)>HR_COOLDOWN_TIME)
              {
                Data_Set_StageTime(w_Dat);
                
              }
              else 
                  Data_Set_StageTime(HR_COOLDOWN_TIME/2);
                  
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

void initHrc(void)
{
	struProgram Hrc;
//        Setting_Index=0;
	Hrc.Initial = ProgramHrc_Data_Initial;
	Hrc.Setup = ProgramHrc_SetUp;
	Hrc.GetProfile=ProgramHrc_GetShow;
	Hrc.Workout = ProgramHrc_Run;
	Hrc.Programid = PS_THR_ZONE;
	Program_Add_Array(Hrc);
        Hrc.Programid = PS_STEPS_THR;
	Program_Add_Array(Hrc);
        Hrc.Programid = PS_INTERVALS_HR;
	Program_Add_Array(Hrc);
}




















#endif

