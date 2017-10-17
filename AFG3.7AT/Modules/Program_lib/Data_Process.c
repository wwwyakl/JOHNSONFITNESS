#include "Data_Process.h"
#include "Program_Inner.h"
#include "Midware_Heart.h"
#include "Bluetooth_lib.h"
#include "page.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>


#define C_TIME_MAX 35999
#define C_CALORIES_MAX 9999
#define C_DISTANCE_MAX 65000
#define C_STEPS_MAX 10000
#define C_CLIMB_MAX 1708
#define SPEEDUP_TIME 37//30/0.8//30s
#define SPEEDDOWN_TIME 31//25/0.8//25s
#define DISTANCE_1MILE 1000
#define DISTANCE_2MILE 2000
#define DISTANCE_3KM 1864
#define DISTANCE_5KM 3107
#define DISTANCE_5MILE 5000
#define DISTANCE_10K 6215
#define DISTANCE_8MILE 8000
#define DISTANCE_15K 9322
#define DISTANCE_10MILE 10000
#define DISTANCE_15MILE 15000
#define DISTANCE_20MILE 20000
#define DISTANCE_HALF_MARATHON 13109
#define DISTANCE_MARATHON 26218
typedef struct {
  UCHAR Run : 1;
  UCHAR Sec : 1;
  UCHAR SecFlag : 1;
  UCHAR SaveData_Event: 1;
}DataEvent;
__no_init static  ACTIVE_WORKOUT Data;
__no_init static  DataEvent Data_Event;


static UINT16 w_Data_TimeMax = 1000;
UINT8 AutoRPM_Check=0;
UINT8 Auto_RPM=0;
__no_init static UINT16 w_CaloriesA,w_Calories_Data;
__no_init static UINT16 w_DistanceA,w_Distance_Data;
__no_init static UINT16 w_StepsA,w_Steps_Data;
__no_init static UINT16 w_ClimbA,w_Climb_Data;
__no_init static double _averageHR;
__no_init static double _averageSpeed;
__no_init static double _averageRPM;
__no_init static double _averageIncline;
__no_init static double _averageResistance;
__no_init static UINT32 _Counter;
__no_init static UINT32 _hrCounter;
//__no_init static UINT16 w_1ms_Timer;
__no_init LCB_SPEED_REF MotorRPM_Step;

void BikeEpAscent_WattsUpdate(void);
void Treadmill_Steps_Calculation(void);
void BikeEpAscent_Steps_Calculation(void);
bool Speed_RPM_Covert(void);
bool Incline_ADC_Covert(void);
UCHAR Data_1s_Process(void);
void Data_Process_Intilal_Data(void)
{
  UINT32 w_Dat;
  w_CaloriesA=w_Calories_Data=0;
  w_DistanceA=w_Distance_Data=0;
  w_StepsA=w_Steps_Data=0;
  w_ClimbA=w_Climb_Data=0;
//  w_1ms_Timer=0;
  _averageHR=0;
  _averageSpeed=0;
  _averageRPM=0;
  _averageIncline=0;
  _averageResistance=0;
  _Counter=0;
  _hrCounter=0;
  memset(&Data_Event,0,sizeof(DataEvent));
  memset(&Data,0,sizeof(ACTIVE_WORKOUT));
  Data.User.age=SystemConfig_Get_Int(DEFAULT_AGE_UINT16);
  Data.Time.accumulatedTimeSeconds=SystemConfig_Get_Long(ACCUMULATED_SECONDS_LONG);
  Data.Distance.accumulatedDistanceMilesX1000=SystemConfig_Get_Long(ACCUMULATED_MILESX100_LONG)*10;
  Data.Level.Max_Level=10;
  Data.Level.Min_Level=1;
  Data.Level.Target_Level=1;
  Data.User.user_Slot=0;
  Data_Event.Sec = 0;
  if(SystemConfig_Get_Display_Unit(User_Get_UserSlot( )) == STANDARD)//SystemConfig_Get_Char(UNIT_FLAG_UCHAR)==STANDARD)
  {
    Data.Speed.Max_Speedx1000=SystemConfig_Get_Int(MAX_MPHX10_UINT16)*100;
    Data.Speed.Min_Speedx1000=SystemConfig_Get_Int(MIN_MPHX10_UINT16)*100;
    Data.Speed.Max_Speedx10=SystemConfig_Get_Int(MAX_MPHX10_UINT16);
    Data.Speed.Min_Speedx10=SystemConfig_Get_Int(MIN_MPHX10_UINT16);	
    Data.RPM.Max_RPM=SystemConfig_Get_Int(MAX_MPH_RPM_UINT16);
    Data.RPM.Min_RPM=SystemConfig_Get_Int(MIN_MPH_RPM_UINT16);
  }
  else
  {
    w_Dat=SystemConfig_Get_Int(MAX_KPHX10_UINT16)*100;
    w_Dat=w_Dat*125000/201168+10;
    Data.Speed.Max_Speedx1000=(UINT16)w_Dat;
    w_Dat=SystemConfig_Get_Int(MIN_KPHX10_UINT16)*100;
    w_Dat=w_Dat*125000/201168+3;
    Data.Speed.Min_Speedx1000=w_Dat;
    Data.Speed.Max_Speedx10=SystemConfig_Get_Int(MAX_KPHX10_UINT16);
    Data.Speed.Min_Speedx10=SystemConfig_Get_Int(MIN_KPHX10_UINT16);	
    Data.RPM.Max_RPM=SystemConfig_Get_Int(MAX_KPH_RPM_UINT16);
    Data.RPM.Min_RPM=SystemConfig_Get_Int(MIN_KPH_RPM_UINT16);
  }
  Data.Time.target_Time = 0;
  Data.Time.remaining_Time = 0;
  Data.Time.elapsed_Time = 0;
  Data.Time.warmup_Time = 0;
  Data.Time.cooldown_Time  = 0; 
  Data.Time.time_In_Stage=0;
  Data.Time.stage_Time=Data.Time.target_Time;
  
  
  Data.Speed.Offset=0;
  Data.Incline.Offset=0;
  Data.Resistance.Offset=0;
  Data.Speed.target_Speedx1000=Data.Speed.current_Speedx1000=Data.Speed.Base=Data.Speed.Min_Speedx1000;
  Data.Incline.target_Inclinex10=Data.Incline.current_Inclinex10=Data.Incline.Base=0;
  Data.Resistance.target_Resistance=Data.Resistance.current_Resistance=Data.Resistance.Base=SystemConfig_Get_Char(MIN_RESISTANCE_UINT8);
  
}

void Data_1ms_ISR(void)
{ 
  static UINT16 w_Time_ms=0;

  if(!Data_Event.Run)
  {
    w_Time_ms=0;
    return;
  }
  if(w_Time_ms < w_Data_TimeMax)
  {
    ++w_Time_ms;
    return;
  }
  w_Time_ms=0;
  Data_Event.Sec = 1;
  Data_Event.SecFlag = 1;
  
}
void Data_Set_Event(DATA_EVENT Event)
{
  switch(Event)
  {
  case DATA_RUN_EVENT:
    Data_Event.Run=1;
    break;
  case DATA_PAUSE_EVENT:
    Data_Event.Run=0;
    Data.RPM.target_RPM=0;
    Data_Save_AccumulatedData();
    break;
  case DATA_SAVEDATA_EVENT:
    Data_Save_AccumulatedData();
    break;
  default:
    break;
  }
}
void Data_Set_AutoRPM(UINT8 by_Dat)
{
    if(by_Dat)
    {          
        Data.RPM.EPBI_RPM=Auto_RPM=by_Dat;
        AutoRPM_Check=1;
    }
    else
    {
        AutoRPM_Check=0;  
    }
}
void Data_Save_AccumulatedData(void)
{
    UINT32 Temp=(UINT32)(Data.Distance.accumulatedDistanceMilesX1000/10);
//  Data.Distance.accumulatedDistanceMilesX1000 +=Data.Distance.elapsed_Distancex1000/10; 
  SystemConfigProcess(SVK_ADD_ACCUMULATED_MILESX100,&Temp,sizeof(UINT32));
  SystemConfigProcess(SVK_ADD_ACCUMULATED_SECONDS,&Data.Time.accumulatedTimeSeconds,sizeof(UINT32));
  SystemConfigProcess(SVK_SAVE_ACCUMULATEDDATA2FLASH,NULL,0);
}
void Data_Process(void)
{
    USERS_INFO userinfo = SystemConfig_Get_UserInfo(User_Get_UserSlot());
    Midware_Heart_Process();
    if(Bluetooth_Get_HeartData()>10)
        Data.Heart.current_Heart=Bluetooth_Get_HeartData();
    else
        Data.Heart.current_Heart=Midware_Heart_Get_Heart();
    if(User_Get_UserSlot() == 0)
        Data.User.weight = User_Get_StandardWeight();
    else
        Data.User.weight = userinfo.weightStandardX100;
     MotorRPM_Step.MaxSpeed =  Data.RPM.Max_RPM;
  MotorRPM_Step.AddStep =  Data.RPM.Max_RPM/SPEEDUP_TIME;
  MotorRPM_Step.DecStep =  Data.RPM.Max_RPM/SPEEDDOWN_TIME;
  LCB_Set_SpeedRef(&MotorRPM_Step);
  Data_1s_Process();

  switch(SystemConfig_Get_Char(DRIVE_TYPE_UINT8))
  {
  case NO_MCB:
    break;
  case EP_0B_JIS:
  case DIGITAL_ECB:
  case ECB_JIS:
        if(AutoRPM_Check)
        {
        Data.RPM.EPBI_RPM=Auto_RPM;
        }
        else
        {
#ifdef TM
        Data.RPM.EPBI_RPM=LCB_Get_Data(G_EPBI_RPM);
#else
          RPM_Decoder( );
          Data.RPM.EPBI_RPM= RPM_Get_RPM( ); 
#endif
        }
      if(Data.Resistance.target_ECBCount !=Data.Resistance.current_ECBCount)
      {
#ifdef TM
        LCB_Send_Cmd(DS_ECB_LOCATION,Data.Resistance.target_ECBCount);
#else        
             ECB_Set_Resistance(Data_Get_Resistance(TARGET_RESISTANCE));
                ECB_Data_Updata();
#endif                
        Data.Resistance.current_ECBCount=Data.Resistance.target_ECBCount;
      }
        if(Data.Incline.target_Incline_ADC !=Data.Incline.current_Incline_ADC)
        {
          LCB_Send_Cmd(DS_INC_LOCATION,Data.Incline.target_Incline_ADC);
          Data.Incline.current_Incline_ADC=Data.Incline.target_Incline_ADC;
        }
    break;
  case ECB_INDUCTOR_BRAKE:
  case BIKE_0C_JIS:
  case INDUCTOR_BRAKE_INCLINE:
    if(AutoRPM_Check)
    {
        Data.RPM.EPBI_RPM=Auto_RPM;
    }
    else
    {
        Data.RPM.EPBI_RPM=LCB_Get_Data(G_EPBI_RPM);
    }
    if(Data.Resistance.target_PWM !=Data.Resistance.current_PWM)
    {
      LCB_Send_Cmd(DS_INDUCTION_PWM,Data.Resistance.target_PWM);
      Data.Resistance.current_PWM=Data.Resistance.target_PWM;
    }
      if(Data.Incline.target_Incline_ADC !=Data.Incline.current_Incline_ADC)
      {
        LCB_Send_Cmd(DS_INC_LOCATION,Data.Incline.target_Incline_ADC);
        Data.Incline.current_Incline_ADC=Data.Incline.target_Incline_ADC;
      }
    break;
  case MCB_TOPTEK:
  case MCB_DELTA:
  case MCB_DELTA_COMMOCIAL:
    Data.RPM.Motor_RPM=LCB_Get_Data(G_MOTOR_RPM);
    if(Data.RPM.target_RPM !=Data.RPM.current_RPM)
    {
      LCB_Send_Cmd(DS_MOT_RPM,Data.RPM.target_RPM);
      Data.RPM.current_RPM=Data.RPM.target_RPM;
    }
      if(Data.Incline.target_Incline_ADC !=Data.Incline.current_Incline_ADC)
      {
        LCB_Send_Cmd(DS_INC_LOCATION,Data.Incline.target_Incline_ADC);
        Data.Incline.current_Incline_ADC=Data.Incline.target_Incline_ADC;
      }
    break;
  default:
    Data.RPM.Motor_RPM=LCB_Get_Data(G_MOTOR_RPM);
    if((Data.RPM.target_RPM !=Data.RPM.current_RPM) || ((Data.RPM.target_RPM == 0) && (Data.RPM.Motor_RPM != 0) && (nCurrentPage != PAGE_ENG0_0_DisplayTest)))
    {
      LCB_Send_Cmd(DS_MOT_RPM,Data.RPM.target_RPM);
      Data.RPM.current_RPM=Data.RPM.target_RPM;
    }
      if(Data.Incline.target_Incline_ADC !=Data.Incline.current_Incline_ADC)
      {
        LCB_Send_Cmd(DS_INC_LOCATION,Data.Incline.target_Incline_ADC);
        Data.Incline.current_Incline_ADC=Data.Incline.target_Incline_ADC;
      }
    break;
  }
  
}
bool Incline_ADC_Covert(void)
{
  bool Result=false;
  if(SystemConfig_Get_Int(MAX_INCLINEADC_UINT16)<SystemConfig_Get_Int(MIN_INLCINEADC_UINT16))
  {
    return Result;
  }
  UINT32 AdcRange=SystemConfig_Get_Int(MAX_INCLINEADC_UINT16)-SystemConfig_Get_Int(MIN_INLCINEADC_UINT16);
  Data.Incline.target_Incline_ADC=SystemConfig_Get_Int(MIN_INLCINEADC_UINT16)+Data.Incline.target_Inclinex10*AdcRange/SystemConfig_Get_Int(MAX_INCLINEX10_UINT16);
  Result=true;
  return Result;
}

bool Speed_RPM_Covert(void)
{
    bool Result=false;
  if(Data.RPM.Max_RPM < Data.RPM.Min_RPM ||Data.Speed.Max_Speedx1000<Data.Speed.Min_Speedx1000)
  {
    return Result;
  }
  UINT32 RpmRange=Data.RPM.Max_RPM-Data.RPM.Min_RPM;
  UINT32 SpeedRange=Data.Speed.Max_Speedx1000-Data.Speed.Min_Speedx1000;
  Data.RPM.target_RPM=Data.RPM.Min_RPM+(Data.Speed.target_Speedx1000-Data.Speed.Min_Speedx1000)\
    *RpmRange/SpeedRange;    
  Result=true;
  return Result;
}



void Treadmill_Steps_Calculation(void)
{
  UINT32 l_Dat,Steps_DistanceData,l_Speed,l_Incline,l_Weight,l_Calories,l_Mets;
  UINT32 Climb_DistanceData;
  l_Dat=l_Speed=Data.Speed.target_Speedx1000;
  
  l_Dat=l_Dat*10/36;
  w_Distance_Data=l_Dat+1;
  
  Steps_DistanceData=w_Distance_Data;
  w_Steps_Data=2030*Steps_DistanceData/1000;
  
  l_Incline=Data.Incline.target_Inclinex10;
  Climb_DistanceData = w_Distance_Data;
  Climb_DistanceData *= l_Incline;
  Climb_DistanceData /= 1000;
  w_Climb_Data=Climb_DistanceData*5280/1000;
  
  l_Dat = Data.User.weight;
  l_Dat =l_Dat*215/474;
  ++l_Dat;
  l_Weight = l_Dat/100;
  
  if(l_Speed > 4000)
  {
    l_Dat = (l_Speed * 536) / 10;
    l_Calories=((unsigned long long)l_Incline * l_Speed * 2412) / 10000;
    l_Mets=((unsigned long long)l_Incline * l_Speed * 2414) / 10000;
    l_Calories +=l_Dat;
    l_Mets +=l_Dat;
  }
  else
  {
    l_Dat = (l_Speed * 268) / 10;
    l_Calories=(l_Incline * l_Speed * 4824) / 10000;
    l_Mets=(l_Incline * l_Speed * 4828) / 10000;
    l_Calories +=l_Dat;
    l_Mets +=l_Dat;
  }
  l_Calories += 35000;
  l_Calories *= l_Weight;
  l_Calories /= 12000;
  ++l_Calories;
  w_Calories_Data = (UINT16)l_Calories;  
  
  Data.Calories.CaloriesPerHour=l_Calories*36/100;
  Data.Watts.current_Watts=Data.Calories.CaloriesPerHour*10/42;
  
  l_Mets += 35000;
  l_Mets /= 3500;
  Data.Mets = (UINT16)l_Mets;  
}

UCHAR Data_1s_Process(void)
{
    
    if(!Data_Event.Run) return(0);
    if(!Data_Event.Sec) return(0);
    Data_Event.Sec = 0;
    if(Data.Heart.current_Heart)
    {
        _averageHR = (_averageHR * _hrCounter + Data.Heart.current_Heart) / ( _hrCounter + 1 );
        _hrCounter++;
    } 
    _averageSpeed = (_averageSpeed * _Counter + Program_Get_Speed(DATA_AUTO,0)) / ( _Counter + 1 );
    _averageRPM = (_averageRPM * _Counter + Data.RPM.EPBI_RPM) / ( _Counter + 1 );
    _averageIncline = (_averageIncline * _Counter + Data.Incline.target_Inclinex10) / ( _Counter + 1 );
    _averageResistance = (_averageResistance * _Counter + Data.Resistance.target_Resistance) / ( _Counter + 1 );
    _Counter++;
    Data.Heart.average_Heart=(UINT8)_averageHR;
    Data.Speed.average_Speedx10=(UINT16)_averageSpeed;  
    Data.Incline.average_Inclinex10=(UINT8)_averageIncline;
    Data.Resistance.average_Resistance=(UINT8)_averageResistance;
    Data.RPM.average_RPM=(UINT16)_averageRPM;
    if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
    {
        Treadmill_Steps_Calculation();
        Speed_RPM_Covert();
        Incline_ADC_Covert();
    }
    else
    {
        BikeEpAscent_WattsUpdate();
        BikeEpAscent_Steps_Calculation();
        Incline_ADC_Covert();
    }
    Data.Time.time_In_Stage++;
    if(Data.Time.target_Time)
    {
        if(Data.Time.elapsed_Time < Data.Time.target_Time) 
        {
            ++Data.Time.elapsed_Time;
            ++Data.Time.accumulatedTimeSeconds;
        }
        Data.Time.remaining_Time=Data.Time.target_Time-Data.Time.elapsed_Time;
    }
    else
    {
        if(Data.Time.elapsed_Time < C_TIME_MAX) 
        {
            ++Data.Time.elapsed_Time;
            ++Data.Time.accumulatedTimeSeconds;
        }
        Data.Time.remaining_Time=0;
    }
    w_StepsA+=w_Steps_Data;
    if(w_StepsA >=1000)
    {
        Data.Steps.elapsed_Steps+=(w_StepsA/1000);
        w_StepsA=(w_StepsA%1000);
        if(Data.Steps.elapsed_Steps > C_STEPS_MAX)
            Data.Steps.elapsed_Steps = C_STEPS_MAX;
    }
    w_ClimbA+=w_Climb_Data;
    if(w_ClimbA >=1000)
    {
        Data.Climb.elapsed_Climb+=(w_ClimbA/1000);
        w_ClimbA=(w_ClimbA%1000);
        if(Data.Climb.elapsed_Climb > C_CLIMB_MAX) 
            Data.Climb.elapsed_Climb = C_CLIMB_MAX;
    }
    w_DistanceA+=w_Distance_Data;
    if(w_DistanceA >=1000)
    {
        if(Data.Distance.elapsed_Distancex1000 <C_DISTANCE_MAX) 
            Data.Distance.accumulatedDistanceMilesX1000+=(w_DistanceA/1000);
        
        Data.Distance.elapsed_Distancex1000+=(w_DistanceA/1000);
        w_DistanceA=(w_DistanceA%1000);
        if(Data.Distance.elapsed_Distancex1000 > C_DISTANCE_MAX) 
            Data.Distance.elapsed_Distancex1000 = C_DISTANCE_MAX;
    }
    w_CaloriesA+=w_Calories_Data;
    if(w_CaloriesA >=10000)
    {
        Data.Calories.elapsed_Calories+=(w_CaloriesA/10000);
        w_CaloriesA%=10000;
        if(Data.Calories.elapsed_Calories > C_CALORIES_MAX) 
            Data.Calories.elapsed_Calories = C_CALORIES_MAX;
        if(Data.Calories.target_Calories)
            Data.Calories.remaining_Calories=Data.Calories.target_Calories-Data.Calories.elapsed_Calories;
    } 
    if(Data.Distance.target_Distancex1000)
    {
        if(Data.Distance.elapsed_Distancex1000>Data.Distance.target_Distancex1000)
        {
            Data.Distance.elapsed_Distancex1000=Data.Distance.target_Distancex1000;
        }
        Data.Distance.remaining_Distancex1000 = Data.Distance.target_Distancex1000 - Data.Distance.elapsed_Distancex1000;
    }
    if(Data.Calories.target_Calories)
    {
        if(Data.Calories.elapsed_Calories>Data.Calories.target_Calories)
        {
            Data.Calories.elapsed_Calories=Data.Calories.target_Calories;
        }
        Data.Calories.remaining_Calories = Data.Calories.target_Calories - Data.Calories.elapsed_Calories;
    }
    return(1);
}
void BikeEpAscent_WattsUpdate(void)
{
    UINT16 Temp_Value=0;
    if(Data.Resistance.target_Resistance<SystemConfig_Get_Char(MIN_RESISTANCE_UINT8))
    {
        Data.Resistance.target_Resistance=SystemConfig_Get_Char(MIN_RESISTANCE_UINT8);
    }
    
    Temp_Value=Watts_ParameterData->PWM_ECBCount_Table[Data.Resistance.target_Resistance-SystemConfig_Get_Char(MIN_RESISTANCE_UINT8)];
    if(Watts_ParameterData->ResistanceType==ECB_TYPE)
        Data.Resistance.target_ECBCount=Temp_Value;
    //  else
    //    Data.Resistance.target_PWM=Temp_Value;
    if(Data.RPM.EPBI_RPM)
    {
        if(Data.RPM.EPBI_RPM<=Watts_ParameterData->Minrpm)
        {
            Temp_Value=0;
        }
        else
        {
            Temp_Value=(Data.RPM.EPBI_RPM-Watts_ParameterData->Minrpm)/5;    
        }
        if(Temp_Value > Watts_ParameterData->Columns-1) 
        {
            Temp_Value=Watts_ParameterData->Columns-1;
        }
        Data.Watts.current_Watts = Watts_ParameterData->Watts_Table[(Data.Resistance.target_Resistance-SystemConfig_Get_Char(MIN_RESISTANCE_UINT8)) * Watts_ParameterData->Columns+ Temp_Value];
        if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==BIKE)
        {
            if(Data.Watts.current_Watts <= 13)
                Data.Speed.EPBI_Speedx1000 = (UINT16)(pow((52.63*((float)Data.Watts.current_Watts/13)),0.3333)*1000);  
            else	
                Data.Speed.EPBI_Speedx1000 = (UINT16)(pow((52.63*(Data.Watts.current_Watts-13)),0.3333)*1000);
        }
        else
        {
            //      Data.Speed.EPBI_Speedx1000 = Data.RPM.EPBI_RPM*1000/16;
            Data.Speed.EPBI_Speedx1000 = (Data.RPM.EPBI_RPM*70+Data_Get_Watts(CURRENT_WATT)*15);
        }
    }
    else
    {
        Data.Speed.EPBI_Speedx1000=0;
        Data.Watts.current_Watts=0;
    }
    
}


void BikeEpAscent_Steps_Calculation(void)
{
  UINT32 l_Incline,l_Weight;
  UINT32 Climb_DistanceData;
  UINT32 l_VO2Max;
  UINT32 l_Out;
  l_Out=Data.Speed.EPBI_Speedx1000;
  l_Out=l_Out*10/36;
  w_Distance_Data=l_Out+1;
  
  w_Steps_Data=(UINT32)Data.RPM.EPBI_RPM*1000/30;
  
  l_Incline=Data.Incline.target_Inclinex10;
  Climb_DistanceData = w_Distance_Data;
  Climb_DistanceData *= l_Incline;
  Climb_DistanceData /= 1000;
  w_Climb_Data=Climb_DistanceData*5280/1000;
  
  if(Data.Speed.EPBI_Speedx1000 == 0)
  {
    w_Distance_Data=0;
  }
  if(Data.Watts.current_Watts == 0)
  {
    w_Calories_Data = 0;
    return;
  }
  l_Weight = Data.User.weight;
  l_Weight =l_Weight*215/474;
  ++l_Weight;
  l_Weight = l_Weight/100;
  
  l_Out=Data.Watts.current_Watts*120;
  l_Out+=35*l_Weight;
  l_Out *=1000;
  l_VO2Max=l_Out;
  l_Out/=12000;
  w_Calories_Data=l_Out; 
  Data.Calories.CaloriesPerHour=w_Calories_Data*36/100;
  Data.VO2Max = l_VO2Max/l_Weight/1000;
  Data.Mets=l_VO2Max/l_Weight/35/100;
}

void Data_Set_Speed_Base_User(UINT16 by_Dat,UCHAR by_Mode)
{
  UINT32 temp =  by_Dat*100;
  UCHAR by_Unit = SystemConfig_Get_Display_Unit(User_Get_UserSlot( ));
  if(by_Unit==METRIC)
  {
    by_Dat = temp*125000/201168+10;
    by_Dat=GETMAXMINVALUE(by_Dat,Data.Speed.Max_Speedx1000,Data.Speed.Min_Speedx1000);
  }
  else
  {
    by_Dat=GETMAXMINVALUE(temp,Data.Speed.Max_Speedx1000,Data.Speed.Min_Speedx1000);
  }
  if(by_Mode)
  {
    Data.Speed.target_Speedx1000 =by_Dat;
    Data.Speed.Offset = Data.Speed.target_Speedx1000-Data.Speed.Base;
  }
  else
  {
    Data.Speed.Base=by_Dat;
  }
  
}

void Data_Set_Speed(UINT16 by_Dat,UCHAR by_Unit,UCHAR by_Mode)
{
  UINT32 temp =  by_Dat*100;
  if(by_Unit==METRIC)
  {
    by_Dat = temp*125000/201168;
    by_Dat=GETMAXMINVALUE(by_Dat,Data.Speed.Max_Speedx1000,Data.Speed.Min_Speedx1000);
  }
  else
  {
    by_Dat=GETMAXMINVALUE(temp,Data.Speed.Max_Speedx1000,Data.Speed.Min_Speedx1000);
  }
  if(by_Mode)
  {
    Data.Speed.target_Speedx1000 =by_Dat;
    Data.Speed.Offset = Data.Speed.target_Speedx1000-Data.Speed.Base;
  }
  else
  {
    Data.Speed.Base=by_Dat;
  }
  
}
void Data_Set_Incline(UINT16 by_Dat,UCHAR by_Mode)
{
    by_Dat = by_Dat - by_Dat%5;
    if(SystemConfig_Get_Char(DRIVE_TYPE_UINT8) == DIGITAL_ECB 
       || SystemConfig_Get_Char(DRIVE_TYPE_UINT8) == BIKE_0C_JIS)
        return;
    by_Dat=GETMAXMINVALUE(by_Dat,SystemConfig_Get_Int(MAX_INCLINEX10_UINT16),SystemConfig_Get_Int(MIN_INCLINEX10_UINT16));
    if(by_Mode)
    {
        Data.Incline.target_Inclinex10 = by_Dat;
        Data.Incline.Offset = Data.Incline.target_Inclinex10- Data.Incline.Base;
    }
    else
    {
        Data.Incline.Base=by_Dat;
    }
}
void Data_Set_Resistance(UINT16 by_Dat,UCHAR by_Mode)
{
  by_Dat =GETMAXMINVALUE(by_Dat, SystemConfig_Get_Char(MAX_RESISTANCE_UINT8), SystemConfig_Get_Char(MIN_RESISTANCE_UINT8));
  if(by_Mode)
  {
    Data.Resistance.target_Resistance=by_Dat;
    Data.Resistance.Offset = Data.Resistance.target_Resistance-Data.Resistance.Base;
  }
  else
  {
    Data.Resistance.Base = by_Dat;
  }
  
}
void Data_Set_Time(UINT16 by_Dat)
{
  Data.Time.target_Time =by_Dat;
  Data.Time.elapsedOld_Time = Data.Time.elapsed_Time;
  Data.Time.remaining_Time=Data.Time.target_Time-Data.Time.elapsed_Time;
}

void Data_Set_ElapsedOldTime(UINT16 by_Dat)
{
    Data.Time.elapsedOld_Time = by_Dat;
}

void Data_Set_WorkOutTime(UINT16 by_Dat)
{
  Data.Time.workout_Time =by_Dat;
  Data.Time.target_Time=Data.Time.warmup_Time+Data.Time.workout_Time+Data.Time.cooldown_Time;
}
void Data_Set_Weight(UINT16 by_Dat)
{
  UINT32 temp=by_Dat*100;
//  if(SystemConfig_Get_Char(UNIT_FLAG_UCHAR)==METRIC)
  	if(SystemConfig_Get_Display_Unit(User_Get_UserSlot( )) == METRIC)
  {
    by_Dat = temp*474/215;
  }
  by_Dat =GETMAXMINVALUE(by_Dat, SystemConfig_Get_Int(MAX_WEIGHT_STANDARD_UINT16), SystemConfig_Get_Int(MIN_WEIGHT_STANDARD_UINT16));
  Data.User.weight=by_Dat;
} 
void Data_Set_StageTime(UINT16 by_Dat)
{
  Data.Time.stage_Time =by_Dat;
  Data.Time.time_In_Stage=0;
}

void Data_Set_WarmUP_Time(UINT16 by_Dat)
{
  Data.Time.warmup_Time =by_Dat;
}

void Data_Set_CoolDown_Time(UINT16 by_Dat)
{
  Data.Time.cooldown_Time =by_Dat;
}
void Data_Set_WarmUP_Distance(UINT16 by_Dat)
{
  Data.Distance.WarmUp_Distancex1000=by_Dat;
}

void Data_Set_CoolDown_Distance(UINT16 by_Dat)
{
  Data.Distance.CoolDown_Distancex1000=by_Dat;
}
void Data_Set_remainingOld_Calories(UINT16 by_Dat)
{
  Data.Calories.remainingOld_Calories=by_Dat;
}

void Data_Set_WarmUP_Calories(UINT16 by_Dat)
{
  Data.Calories.WarmUp_Calories=by_Dat;
}
void Data_Set_CoolDown_Calories(UINT16 by_Dat)
{
  Data.Calories.CoolDown_Calories=by_Dat;
}
void Data_Set_Level(UINT16 by_Dat)
{
  by_Dat =GETMAXMINVALUE(by_Dat, Data.Level.Max_Level, Data.Level.Min_Level);
  Data.Level.Target_Level =by_Dat;
}
void Data_Set_Level_MaxVal(UINT16 by_Dat)
{
  Data.Level.Max_Level =by_Dat;
}
void Data_Set_Watt(UINT16 by_Dat)
{
  by_Dat =GETMAXMINVALUE(by_Dat, PARAMETER_MAX_WATT, PARAMETER_MIN_WATT);
  Data.Watts.target_Watts =by_Dat;
}
void Data_Set_TargetHeart(UINT8 by_Dat)
{
  Data.Heart.target_Heart=by_Dat;
}
void Data_Set_Distance(UINT16 by_Dat)
{
    switch(by_Dat)
    {
        default:
        case  TARGET_NONE:
        {
            Data.Distance.target_Distancex1000 =0;
        }
        break;
        case TARGET_1MILE:
        {
            Data.Distance.target_Distancex1000 =DISTANCE_1MILE;
        }
        break;
        case TARGET_2MILE:
        {
            Data.Distance.target_Distancex1000 =DISTANCE_2MILE;
        }
        break;
        case TARGET_5KM:
        {
            Data.Distance.target_Distancex1000 =DISTANCE_5KM;
        }
        break;
        case TARGET_5MILE:
        {
            Data.Distance.target_Distancex1000 =DISTANCE_5MILE;
        }
        break;
        case TARGET_10KM:
        {
            Data.Distance.target_Distancex1000 =DISTANCE_10K;
        }
        break;
// WWW #if 0
        case TARGET_8MILE:
        {
            Data.Distance.target_Distancex1000 =DISTANCE_8MILE;
        }
        break;
// WWW #endif  
        case TARGET_10MILE:
        {
            Data.Distance.target_Distancex1000 =DISTANCE_10MILE;
        }
        break;
// WWW #if 0
        case TARGET_15KM:
        {
            Data.Distance.target_Distancex1000 =DISTANCE_15K;
        }
        break;  
        case TARGET_20KM:
        {
            Data.Distance.target_Distancex1000 =DISTANCE_20MILE;
        }
        break;  
// WWW #endif  
        case TARGET_HALF_MARATHON:
        {
            Data.Distance.target_Distancex1000 =DISTANCE_HALF_MARATHON;
        }
        break;
// WWW #if 0
        case TARGET_15MILE:
        {
            Data.Distance.target_Distancex1000 =DISTANCE_15MILE;
        }
        break;
        case TARGET_20MILE:
        {
            Data.Distance.target_Distancex1000 =DISTANCE_20MILE;
        }
        break;
        case TARGET_MARATHON:
        {
            Data.Distance.target_Distancex1000 =DISTANCE_MARATHON;
        }
        break; 
// WWW #endif  
    }
    Data.Distance.remaining_Distancex1000=Data.Distance.target_Distancex1000-Data.Distance.elapsed_Distancex1000;
    Data.Distance.remainingOld_Distancex1000 = Data.Distance.remaining_Distancex1000;    //zsf
}
//void Data_Set_targetDistancex1000(UINT16 by_Dat)
//{
//  Data.Distance.target_Distancex1000=by_Dat;
//}
//void Data_Set_targetCalories(UINT16 by_Dat)
//{
//  Data.Calories.target_Calories=by_Dat;
//}
void _Set_Target_RPM(UINT16 by_Dat)
{
  Data.RPM.target_RPM=by_Dat;
}
void _Set_Target_ECBCount(UINT16 by_Dat)
{
  Data.Resistance.target_ECBCount=by_Dat;
}
void Data_Set_TargetPWMValue(UINT16 by_Dat)
{
  Data.Resistance.target_PWM=by_Dat;
}
void Data_Set_targetDistancex1000(UINT16 by_Dat)
{
  Data.Distance.target_Distancex1000=by_Dat;
  Data.Distance.remaining_Distancex1000 = Data.Distance.target_Distancex1000 - Data.Distance.elapsed_Distancex1000;
  Data.Distance.remainingOld_Distancex1000 = Data.Distance.remaining_Distancex1000;
}
void Data_Set_remainingOldDistancex1000(UINT16 by_Dat)
{
  Data.Distance.remainingOld_Distancex1000=by_Dat;
}
void Data_Set_targetCalories(UINT16 by_Dat)
{
    Data.Calories.target_Calories=by_Dat;
    Data.Calories.remaining_Calories = Data.Calories.target_Calories - Data.Calories.elapsed_Calories;
    Data.Calories.remainingOld_Calories = Data.Calories.remaining_Calories;
}
void Data_Set_Sprint8SprintStage(UINT8 by_Dat)
{
  Data.Sprint8SprintStage=by_Dat;
}
UINT8 Data_Get_Sprint8SprintStage(void)
{
  return(Data.Sprint8SprintStage);
}
UINT8 Data_Get_Laps(void)
{
  UINT32 ret = 0;
  ret = (unsigned long long)Data.Distance.elapsed_Distancex1000*201168/125000+2;//convert to Metric
  ret /= 400;
  if(ret > 188)
      ret %= 188;
  return((UINT8)ret);
}
UINT8 Data_Get_LapsCircle(void)
{
  UINT32 ret;
  UINT8 by_Dat;
  ret=(unsigned long long)Data.Distance.elapsed_Distancex1000*201168/125000+2;//convert to Metric
  ret %= 400;
//if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
//  by_Dat=ret/50;
//else 
  by_Dat=ret*10/125;

  return(by_Dat);
}
void Data_Set_Nx(UCHAR by_Dat)
{
  if(by_Dat>100||by_Dat==0)return;
  w_Data_TimeMax = 1000/by_Dat;
}

UINT16 Data_Get_Nx(void)
{
  return(1000/w_Data_TimeMax);
}

UINT16 Data_Get_Time(_TIME_PARA_INDEX Para_Index)
{
    UINT16 w_Dat=0;
    switch(Para_Index)
    {
        case TARGET_TIME:
        w_Dat=Data.Time.target_Time;
        break;
        case ELAPSED_TIME:
        w_Dat=Data.Time.elapsed_Time;
        break;
        case ELAPSED_OLD_TIME:
        w_Dat=Data.Time.elapsedOld_Time;
        break;
        case REMAINING_TIME:
        w_Dat=Data.Time.remaining_Time;
        break;
        case WARMUP_TIME:
        w_Dat=Data.Time.warmup_Time;
        break;
        case COOLDOWN_TIME:
        w_Dat=Data.Time.cooldown_Time;
        break;
        case WORKOUT_TIME:
        w_Dat=Data.Time.workout_Time;
        break;
        case AVERAGE_TIME:
        w_Dat=Data.Time.average_Time;
        break;
        case STAGE_TIME:
        w_Dat=Data.Time.stage_Time;
        break;
        case TIME_IN_STAGE:
        w_Dat=Data.Time.time_In_Stage;
        break;
        case STAGE:
        w_Dat=Data.Time.stage;
        break;
        default:break;
    }
    return w_Dat;
    
}
UINT16 Data_Get_Speed(_SPEED_PARA_INDEX Para_Index)
{
  UINT32 w_Dat=0;
  switch(Para_Index)
  {
  case TARGET_PACE:
    w_Dat=Data.Speed.target_Pace;
    break;
  case AVERAGE_PACE:
    w_Dat=Data.Speed.average_Pace;
    break;
  case EPBI_SPEEDX1000:
    w_Dat=Data.Speed.EPBI_Speedx1000;
    break;
  case EPBI_SPEEDX10_AUTO:
      w_Dat=Data.Speed.EPBI_Speedx1000;	
   // if(SystemConfig_Get_Char(UNIT_FLAG_UCHAR)==METRIC)
      if(SystemConfig_Get_Display_Unit(User_Get_UserSlot( )) == METRIC)
      {
          w_Dat=w_Dat*201168/125000+2;//convert to Metric
      }
	w_Dat /=100;
    break;
  case MOTOR_SPEED:
    w_Dat=Data.Speed.Motor_Speed;
    break;
  case TARGET_SPEEDX10_AUTO:
    Data.Speed.target_Speedx1000 = Data.Speed.Offset + Data.Speed.Base;
    if((Data.Speed.Offset + Data.Speed.Base) > Data.Speed.Max_Speedx1000)
    {
      Data.Speed.target_Speedx1000 = Data.Speed.Max_Speedx1000;
    }
    if((Data.Speed.Offset +Data.Speed.Base) < Data.Speed.Min_Speedx1000)
    {
      Data.Speed.target_Speedx1000  = Data.Speed.Min_Speedx1000;
    }
    w_Dat=Data.Speed.target_Speedx1000;
   // if(SystemConfig_Get_Char(UNIT_FLAG_UCHAR)==METRIC)
    if(SystemConfig_Get_Display_Unit(User_Get_UserSlot( )) == METRIC)
    {
        w_Dat=w_Dat*201168/125000+2;//convert to Metric
    }
	w_Dat /=100;
    
    break;
  case TARGET_SPEEDX10_MPH:
    Data.Speed.target_Speedx1000 = Data.Speed.Offset + Data.Speed.Base;
    if((Data.Speed.Offset + Data.Speed.Base) > Data.Speed.Max_Speedx1000)
    {
      Data.Speed.target_Speedx1000 = Data.Speed.Max_Speedx1000;
    }
    if((Data.Speed.Offset +Data.Speed.Base) < Data.Speed.Min_Speedx1000)
    {
      Data.Speed.target_Speedx1000  = Data.Speed.Min_Speedx1000;
    }
    w_Dat=Data.Speed.target_Speedx1000;
    w_Dat /=100;
    break;
  case TARGET_SPEEDX10_KPH:
    Data.Speed.target_Speedx1000 = Data.Speed.Offset + Data.Speed.Base;
    if((Data.Speed.Offset + Data.Speed.Base) > Data.Speed.Max_Speedx1000)
    {
      Data.Speed.target_Speedx1000 = Data.Speed.Max_Speedx1000;
    }
    if((Data.Speed.Offset +Data.Speed.Base) < Data.Speed.Min_Speedx1000)
    {
      Data.Speed.target_Speedx1000  = Data.Speed.Min_Speedx1000;
    }
    w_Dat=Data.Speed.target_Speedx1000;
    w_Dat=w_Dat*201168/125000+2;//convert to Metric
	w_Dat /=100;
    break;
  case TARGET_SPEEDX1000:
    Data.Speed.target_Speedx1000 = Data.Speed.Offset + Data.Speed.Base;
    if((Data.Speed.Offset + Data.Speed.Base) > Data.Speed.Max_Speedx1000)
    {
      Data.Speed.target_Speedx1000 = Data.Speed.Max_Speedx1000;
    }
    if((Data.Speed.Offset +Data.Speed.Base) < Data.Speed.Min_Speedx1000)
    {
      Data.Speed.target_Speedx1000  = Data.Speed.Min_Speedx1000;
    }
    w_Dat=Data.Speed.target_Speedx1000;
    break;
  case CURRENT_SPEEDX1000:
    w_Dat=Data.Speed.current_Speedx1000;
    break;
  case AVERAGE_SPEEDX10:
    w_Dat=Data.Speed.average_Speedx10;
    break;
  case MAX_SPEEDX1000:
    w_Dat=Data.Speed.Max_Speedx1000;
    break;
  case MIN_SPEEDX1000:
    w_Dat=Data.Speed.Min_Speedx1000;
    break;
  case MAX_SPEEDX10:
    w_Dat=Data.Speed.Max_Speedx10;
    break;
  case MIN_SPEEDX10:
    w_Dat=Data.Speed.Min_Speedx10;
    break;
 case SPEED_BASE:
    w_Dat=Data.Speed.Base;
    break;
  default:break;
  }
  return w_Dat;
}
UINT16 Data_Get_RPM(_RPM_PARA_INDEX Para_Index)
{
  UINT16 w_Dat=0;
  switch(Para_Index)
  {
  case TARGET_RPM:
    w_Dat=Data.RPM.target_RPM;
    break;
  case CURRENT_RPM:
    w_Dat=Data.RPM.current_RPM;
    break;
  case MOTOR_RPM:
    w_Dat=Data.RPM.Motor_RPM;
    break;
  case EPBI_RPM:
    w_Dat=Data.RPM.EPBI_RPM;
    break;
  case AVERAGE_RPM:
    w_Dat=Data.RPM.average_RPM;
    break;
  case MAX_RPM:
    w_Dat=Data.RPM.Max_RPM;
    break;
  case MIN_RPM:
    w_Dat=Data.RPM.Min_RPM;
    break;
  default:break;
  }
  return w_Dat;
}
UINT16 Data_Get_Incline(_INCLINE_PARA_INDEX Para_Index)
{
  UINT16 w_Dat=0;
  switch(Para_Index)
  {
  case TARGET_INCLINE_ADC:
    w_Dat=Data.Incline.target_Incline_ADC;
    break;
  case CURRENT_INCLINE_ADC:
    w_Dat=Data.Incline.current_Incline_ADC;
    break;
  case REAL_INCLINE_ADC:
    w_Dat=Data.Incline.real_Incline_ADC;
    break;
  case MAX_INCLINEADC:
    w_Dat=SystemConfig_Get_Int(MAX_INCLINEADC_UINT16);
    break;
  case MIN_INCLINEADC:
    w_Dat=SystemConfig_Get_Int(MIN_INLCINEADC_UINT16);
    break;
  case MAX_INCLINEX10:
    w_Dat=SystemConfig_Get_Int(MAX_INCLINEX10_UINT16);
    break;
  case MIN_INCLINEX10:
    w_Dat=SystemConfig_Get_Int(MIN_INCLINEX10_UINT16);
    break;
  case AVERAGE_INCLINEX10:
    w_Dat=Data.Incline.average_Inclinex10;
    break;
  case TARGET_INCLINEX10:
    Data.Incline.target_Inclinex10 = Data.Incline.Offset + Data.Incline.Base;
    if((Data.Incline.Offset + Data.Incline.Base) > SystemConfig_Get_Int(MAX_INCLINEX10_UINT16))
    {
      Data.Incline.target_Inclinex10 = SystemConfig_Get_Int(MAX_INCLINEX10_UINT16);
    }
    if((Data.Incline.Offset +Data.Incline.Base) < SystemConfig_Get_Int(MIN_INCLINEX10_UINT16))
    {
      Data.Incline.target_Inclinex10  = SystemConfig_Get_Int(MIN_INCLINEX10_UINT16);
    }
    w_Dat=Data.Incline.target_Inclinex10;
    break;
  case CURRENT_INCLINEX10:
    w_Dat=Data.Incline.current_Inclinex10;
    break;
  case INCLINE_BASE:
    w_Dat=Data.Incline.Base;;
    break;
  default:break;
  }
  return w_Dat;
}
UINT16 Data_Get_Calories(_CALORIES_PARA_INDEX Para_Index)
{
    UINT16 w_Dat=0;
    switch(Para_Index)
    {
        case TARGET_CALORIES:
        w_Dat=Data.Calories.target_Calories;
        break;
        case ELAPSED_CALORIES:
        w_Dat=Data.Calories.elapsed_Calories;
        break;
        case REMAINING_CALORIES:
        w_Dat=Data.Calories.remaining_Calories;
        break;
        case REMAINING_OLD_CALORIES:
        w_Dat=Data.Calories.remainingOld_Calories;
        break;
        case CALORIES_PERHOUR:
        w_Dat=Data.Calories.CaloriesPerHour;
        break;
        case COOLDOWN_CALORIES:
        w_Dat=Data.Calories.CoolDown_Calories;
        break;
        case CALORIES_PERSECOND:
           w_Dat = w_Calories_Data;
        break;
        default:break;
    }
    return w_Dat;
}
UINT16 Data_Get_Distance(_DISTANCE_PARA_INDEX Para_Index)
{
  UINT16 w_Dat=0;
  switch(Para_Index)
  {
  case TARGET_DISTANCE:
    w_Dat=Data.Distance.target_Distancex1000;
    break;
  case ELAPSED_DISTANCE:
    w_Dat=Data.Distance.elapsed_Distancex1000;
    break;
      case REMAINING_OLD_DISTANCE:
    w_Dat=Data.Distance.remainingOld_Distancex1000;
    break;
  case REMAINING_DISTANCE:
    w_Dat=Data.Distance.remaining_Distancex1000;
    break;
  case COOLDOWN_DISTANCE:
    w_Dat=Data.Distance.CoolDown_Distancex1000;
    break;	
  default:break;
  }
  return w_Dat;
}
UINT16 Data_Get_Steps(_STEPS_PARA_INDEX Para_Index)
{
  UINT16 w_Dat=0;
  switch(Para_Index)
  {
  case TARGET_STEPS:
    w_Dat=Data.Steps.target_Steps;
    break;
  case ELAPSED_STEPS:
    w_Dat=Data.Steps.elapsed_Steps;
    break;
  case REMAINING_STEPS:
    w_Dat=Data.Steps.remaining_Steps;
    break;
  default:break;
  }
  return w_Dat;
}
UINT16 Data_Get_Climb(_CLIMB_PARA_INDEX Para_Index)
{
  UINT16 w_Dat=0;
  switch(Para_Index)
  {
  case TARGET_CLIMB:
    w_Dat=Data.Climb.target_Climb;
    break;
  case ELAPSED_CLIMB:
    w_Dat=Data.Climb.elapsed_Climb;
    break;
  case REMAINING_CLIMB:
    w_Dat=Data.Climb.remaining_Climb;
    break;
  default:break;
  }
  return w_Dat;
}
UINT16 Data_Get_Heart(_HEART_PARA_INDEX Para_Index)
{
  UINT16 w_Dat=0;
  switch(Para_Index)
  {
  case TARGET_HEART:
    w_Dat=Data.Heart.target_Heart;
    break;
  case CURRENT_HEART:
    w_Dat=Data.Heart.current_Heart;
    break;
  case AVERAGE_HEART:
    w_Dat=Data.Heart.average_Heart;
    break;
  default:break;
  }
  return w_Dat;
}
UINT16 Data_Get_Watts(_WATTS_PARA_INDEX Para_Index)
{
  UINT16 w_Dat=0;
  switch(Para_Index)
  {
  case TARGET_WATT:
    w_Dat=Data.Watts.target_Watts;
    break;
  case CURRENT_WATT:
    w_Dat=Data.Watts.current_Watts;
    break;
  default:break;
  }
  return w_Dat;
}
void Data_Set_Step(UINT16 by_Dat)
{
    Data.Steps.target_Steps =by_Dat;
    Data.Steps.remaining_Steps=Data.Steps.target_Steps-Data.Steps.elapsed_Steps;
}
void Data_Set_Calories(UINT16 by_Dat)
{
  Data.Calories.target_Calories =by_Dat;
  Data.Calories.remaining_Calories=Data.Calories.target_Calories-Data.Calories.elapsed_Calories;
  Data.Calories.remainingOld_Calories = Data.Calories.remaining_Calories;    //zsf
}
UINT16 Data_Get_Resistance(_RESISTANCE_PARA_INDEX Para_Index)
{
  UINT16 w_Dat=0;
  switch(Para_Index)
  {
  case TARGET_PWM:
    w_Dat=Data.Resistance.target_PWM;
    break;
  case CURRENT_PWM:
    w_Dat=Data.Resistance.current_PWM;
    break;
  case REAL_PWM:
    w_Dat=Data.Resistance.real_PWM;
    break;
  case TARGET_ECBCOUNT:
    w_Dat=Data.Resistance.target_ECBCount;
    break;
  case REAL_ECBCOUNT:
    w_Dat=Data.Resistance.real_ECBCount;
    break;
  case TARGET_RESISTANCE:
    Data.Resistance.target_Resistance = Data.Resistance.Offset + Data.Resistance.Base;
    if((Data.Resistance.Offset + Data.Resistance.Base) > SystemConfig_Get_Char(MAX_RESISTANCE_UINT8))
    {
      Data.Resistance.target_Resistance = SystemConfig_Get_Char(MAX_RESISTANCE_UINT8);
    }
    if((Data.Resistance.Offset +Data.Resistance.Base) < SystemConfig_Get_Char(MIN_RESISTANCE_UINT8))
    {
      Data.Resistance.target_Resistance  = SystemConfig_Get_Char(MIN_RESISTANCE_UINT8);
    }
    w_Dat=Data.Resistance.target_Resistance;
    break;
  case CURRENT_RESISTANCE:
    w_Dat=Data.Resistance.current_Resistance;
    break;
  case AVERAGE_RESISTANCE:
    w_Dat=Data.Resistance.average_Resistance;
    break;
  case MAX_RESISTANCE:
    w_Dat=SystemConfig_Get_Char(MAX_RESISTANCE_UINT8);
    break;
  case MIN_RESISTANCE:
    w_Dat=SystemConfig_Get_Char(MIN_RESISTANCE_UINT8);
    break;
  case RESISTANCE_BASE:
    w_Dat=Data.Resistance.Base;
    break;
  default:break;
  }
  return w_Dat;
}
UINT16 Data_Get_Level(_LEVEL_PARA_INDEX Para_Index)
{
  UINT16 w_Dat=0;
  switch(Para_Index)
  {
  case MAX_LEVEL:
    w_Dat=Data.Level.Max_Level;
    break;
  case MIN_LEVEL:
    w_Dat=Data.Level.Min_Level;
    break;
  case TARGET_LEVEL:
    w_Dat=Data.Level.Target_Level;
    break;
  default:break;
  }
  return w_Dat;
}
UINT16 Data_Get_UserData(_USER_PARA_INDEX Para_Index)
{
  UINT16 w_Dat=0;
  switch(Para_Index)
  {
  case USER_WEIGHT:
    w_Dat=Data.User.weight;
    break;
  case USER_AGE:
    w_Dat=Data.User.age;
    break;
  case USER_GENDER:
    w_Dat=Data.User.gender;
    break;
  case USER_SLOT:
    w_Dat=Data.User.user_Slot;
    break;
  default:break;
  }
  return w_Dat;
}

UINT16 Data_Get_VO2Max(void)
{
  return Data.VO2Max;
}
UINT16 Data_Get_Mets(void)
{
  return Data.Mets;
}


UCHAR Data_Get_1s(void)
{
  if(Data_Event.SecFlag)
  {
    Data_Event.SecFlag=0;
    return 1;
  }
  return 0;
}
UCHAR Data_Get_DotmatrixSpeedData(UCHAR by_Dat)
{
  UINT32 by_Out;
  UINT16 by_temp;
  by_temp=by_Dat*100;
  if((Data.Speed.Offset + by_temp) > Data.Speed.Max_Speedx1000)
  {
    by_Out=Data.Speed.Max_Speedx1000;
  }
  else if((Data.Speed.Offset +by_temp) < Data.Speed.Min_Speedx1000)
  {
    by_Out=Data.Speed.Min_Speedx1000;
  }
  else
  {
    by_Out=Data.Speed.Offset + by_temp;
  }
   // if(SystemConfig_Get_Char(UNIT_FLAG_UCHAR)==METRIC)
		  	if(SystemConfig_Get_Display_Unit(User_Get_UserSlot( )) == METRIC)
  {
    by_Out=by_Out*201168/125000+2;
  }
  return (UCHAR)(by_Out/100);
}
UCHAR Data_Get_DotmatrixInclineData(UCHAR by_Dat)
{
  UCHAR by_Out;
  if((Data.Incline.Offset + by_Dat) > SystemConfig_Get_Int(MAX_INCLINEX10_UINT16))
  {
    return SystemConfig_Get_Int(MAX_INCLINEX10_UINT16);
  }
  if((Data.Incline.Offset + by_Dat) < SystemConfig_Get_Int(MIN_INCLINEX10_UINT16))
  {
    return SystemConfig_Get_Int(MIN_INCLINEX10_UINT16);
  }
  by_Out=Data.Incline.Offset + by_Dat;
  return (by_Out);
}

UCHAR Data_Get_DotmatrixResistanceData(UCHAR by_Dat)
{
  UCHAR by_Out;
  if((Data.Resistance.Offset + by_Dat) > SystemConfig_Get_Char(MAX_RESISTANCE_UINT8))
  {
    return SystemConfig_Get_Char(MAX_RESISTANCE_UINT8);
  }
  if((Data.Resistance.Offset + by_Dat) < SystemConfig_Get_Char(MIN_RESISTANCE_UINT8))
  {
    return SystemConfig_Get_Char(MIN_RESISTANCE_UINT8);
  }
  by_Out=Data.Resistance.Offset + by_Dat;
  return (by_Out);
}

void Data_Set_ResistanceOffset(INT16 by_Dat)
{
  Data.Resistance.Offset=by_Dat;
}
void Data_Set_InclineOffset(INT16 by_Dat)
{
  Data.Incline.Offset=by_Dat;
}
void Data_Set_SpeedOffset(INT16 by_Dat)
{
//  INT32 by_Out;
//  by_Out=by_Dat*100;
   // if(SystemConfig_Get_Char(UNIT_FLAG_UCHAR)==METRIC)
//		  	if(SystemConfig_Get_Display_Unit(User_Get_UserSlot( )) == METRIC)
//  {
//    by_Out=by_Out*201168/125000+2;
//  }
  Data.Speed.Offset=by_Dat;
}
INT16 Data_Get_ResistanceOffset(void)
{
  return Data.Resistance.Offset;
}
INT16 Data_Get_InclineOffset(void)
{
  return Data.Incline.Offset;
}

INT16 Data_Get_MphOffset(void)
{
  INT32 by_Out=Data.Speed.Offset;
  return by_Out/100;
}

INT16 Data_Get_SpeedOffset(void)
{
  INT32 by_Out=Data.Speed.Offset;
   // if(SystemConfig_Get_Char(UNIT_FLAG_UCHAR)==METRIC)
		  	if(SystemConfig_Get_Display_Unit(User_Get_UserSlot( )) == METRIC)
  {
    by_Out=by_Out*201168/125000+2;
  }
  return by_Out/100;
}
