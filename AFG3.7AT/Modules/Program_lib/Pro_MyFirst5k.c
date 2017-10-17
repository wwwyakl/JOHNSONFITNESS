#include"Pro_MyFirst5k.h"
#include "Bluetooth_lib.h"

#ifdef __PROGRAM_MYFIRST_5K__

#define MyFirst5K_NoWeekMissed  0
#define MyFirst5K_1WeekMissed   1
#define MyFirst5K_2WeekMissed   2

typedef  struct
{
  UINT8 MyFirst5k_Status;
  UINT8 MyFirst5k_Time;
}MyFirst5kRunDataStru;
typedef  struct
{
  const MyFirst5kRunDataStru *MyFirst5kRunData;
  UINT8 MyFirst5k_WorkoutDataTotalSegment;
}MyFirst5kWorkoutData;
typedef enum
{
  NORMAL=0,
  FASTER,
  SLOWER,
}SPEED_STATE;
typedef struct
{
  UINT16 lastUpdateYear;
  UINT16 Stage_Time;
  UINT16 WorkoutTargetTime;
  UINT8  WorkoutTotalSegment;
  UINT8  WorkoutTotalCount;
  UINT8  Stage;
  UINT8  State;
  UINT8  WorkoutDataArrayIndex;
  UINT8  Week;
  UINT8  Workout;
  UINT8  WarmupCooldown_Speed;
  UINT8  Walk_Speed;
  UINT8  Jog_Speed;
  UINT8  Reset_Counts;
  UINT8  lastUpdateDay;
  UINT8  lastUpdateMonth;
  UINT8  SpeedCheckTime;
  UINT8  SettingState;
  UINT8  GoodLuckFlag;
  SPEED_STATE  EpBikeSpeedState;

}MyFirst5kData;

typedef struct{
  struProgram StruProgram;
  MyFirst5kData MyFirst5k_Data;
  PROFILE Profile;
}MyFirst5K;

__no_init static MyFirst5K g_MyFirst5K;
__no_init static MyFirst5K g_MyFirst5KBuff;
static DateTime_T Rtc_Time;
static UINT8 MyFirst5k_Get_Result(void);
const MyFirst5kRunDataStru MyFirst5kRunWeek1Workout123[]=
{
  {WARMUP  ,10},
  {JOG     ,2 },
  {WALK    ,3 },
  {JOG     ,2 },
  {WALK    ,3 },
  {JOG     ,2 },
  {WALK    ,3 },
  {JOG     ,2 },
  {WALK    ,3 },
  {JOG     ,2 },
  {WALK    ,3 },
  {JOG     ,2 },
  {WALK    ,3 },
  {JOG     ,2 },
  {WALK    ,3 },
  {JOG     ,2 },
  {WALK    ,3 },
  {COOLDOWN,10},
};
const MyFirst5kRunDataStru MyFirst5kRunWeek2Workout123[]=
{
  {WARMUP  ,10},
  {JOG     ,3 },
  {WALK    ,4 },
  {JOG     ,3 },
  {WALK    ,4 },
  {JOG     ,3 },
  {WALK    ,4 },
  {JOG     ,3 },
  {WALK    ,4 },
  {JOG     ,3 },
  {WALK    ,4 },
  {JOG     ,3 },
  {WALK    ,4 },
  {COOLDOWN,10},
};
const MyFirst5kRunDataStru MyFirst5kRunWeek3Workout123[]=
{
  {WARMUP  ,10},
  {JOG     ,3 },
  {WALK    ,3 },
  {JOG     ,6 },
  {WALK    ,6 },
  {JOG     ,3 },
  {WALK    ,3 },
  {JOG     ,6 },
  {WALK    ,6 },
  {COOLDOWN,10},
};
const MyFirst5kRunDataStru MyFirst5kRunWeek4Workout123[]=
{
  {WARMUP  ,10},
  {JOG     ,6 },
  {WALK    ,3 },
  {JOG     ,10},
  {WALK    ,5 },
  {JOG     ,6 },
  {WALK    ,3 },
  {JOG     ,10},
  {COOLDOWN,10},
  
};
const MyFirst5kRunDataStru MyFirst5kRunWeek5Workout123[]=
{
  {WARMUP  ,10},
  {JOG     ,10},
  {WALK    ,6 },
  {JOG     ,10},
  {WALK    ,6 },
  {JOG     ,10},
  {COOLDOWN,10},
};
const MyFirst5kRunDataStru MyFirst5kRunWeek6Workout1[]=
{
  {WARMUP  ,10},
  {JOG     ,10},
  {WALK    ,6 },
  {JOG     ,16},
  {WALK    ,6 },
  {JOG     ,10},
  {COOLDOWN,10},
  
};
const MyFirst5kRunDataStru MyFirst5kRunWeek6Workout2[]=
{
  {WARMUP  ,10},
  {JOG     ,20},
  {WALK    ,6 },
  {JOG     ,20},
  {COOLDOWN,10},
};
const MyFirst5kRunDataStru MyFirst5kRunWeek6Workout3[]=
{
  {WARMUP  ,10},
  {JOG     ,50},
  {COOLDOWN,10},
};
const MyFirst5kRunDataStru MyFirst5kRunWeek7Workout123[]=
{
  {WARMUP  ,10},
  {JOG     ,50},
  {COOLDOWN,10},
};
const MyFirst5kRunDataStru MyFirst5kRunWeek8Workout123[]=
{
  {WARMUP  ,10},
  {JOG     ,56},
  {COOLDOWN,10},
};
const MyFirst5kRunDataStru MyFirst5kRunWeek9Workout123[]=
{
  {WARMUP  ,10},
  {JOG     ,60},
  {COOLDOWN,10},
};

const MyFirst5kWorkoutData MyFirst5kWorkoutDataArray[]=
{
  //Week1
  {MyFirst5kRunWeek1Workout123,sizeof(MyFirst5kRunWeek1Workout123)/sizeof(MyFirst5kRunDataStru)},
  {MyFirst5kRunWeek1Workout123,sizeof(MyFirst5kRunWeek1Workout123)/sizeof(MyFirst5kRunDataStru)},
  {MyFirst5kRunWeek1Workout123,sizeof(MyFirst5kRunWeek1Workout123)/sizeof(MyFirst5kRunDataStru)},
  //Week2
  {MyFirst5kRunWeek2Workout123,sizeof(MyFirst5kRunWeek2Workout123)/sizeof(MyFirst5kRunDataStru)},
  {MyFirst5kRunWeek2Workout123,sizeof(MyFirst5kRunWeek2Workout123)/sizeof(MyFirst5kRunDataStru)},
  {MyFirst5kRunWeek2Workout123,sizeof(MyFirst5kRunWeek2Workout123)/sizeof(MyFirst5kRunDataStru)},
  //Week3
  {MyFirst5kRunWeek3Workout123,sizeof(MyFirst5kRunWeek3Workout123)/sizeof(MyFirst5kRunDataStru)},
  {MyFirst5kRunWeek3Workout123,sizeof(MyFirst5kRunWeek3Workout123)/sizeof(MyFirst5kRunDataStru)},
  {MyFirst5kRunWeek3Workout123,sizeof(MyFirst5kRunWeek3Workout123)/sizeof(MyFirst5kRunDataStru)},
  //Week4
  {MyFirst5kRunWeek4Workout123,sizeof(MyFirst5kRunWeek4Workout123)/sizeof(MyFirst5kRunDataStru)},
  {MyFirst5kRunWeek4Workout123,sizeof(MyFirst5kRunWeek4Workout123)/sizeof(MyFirst5kRunDataStru)},
  {MyFirst5kRunWeek4Workout123,sizeof(MyFirst5kRunWeek4Workout123)/sizeof(MyFirst5kRunDataStru)},
  //Week5
  {MyFirst5kRunWeek5Workout123,sizeof(MyFirst5kRunWeek5Workout123)/sizeof(MyFirst5kRunDataStru)},
  {MyFirst5kRunWeek5Workout123,sizeof(MyFirst5kRunWeek5Workout123)/sizeof(MyFirst5kRunDataStru)},
  {MyFirst5kRunWeek5Workout123,sizeof(MyFirst5kRunWeek5Workout123)/sizeof(MyFirst5kRunDataStru)},
  //Week6
  {MyFirst5kRunWeek6Workout1  ,sizeof(MyFirst5kRunWeek6Workout1)  /sizeof(MyFirst5kRunDataStru)},
  {MyFirst5kRunWeek6Workout2  ,sizeof(MyFirst5kRunWeek6Workout2)  /sizeof(MyFirst5kRunDataStru)},
  {MyFirst5kRunWeek6Workout3  ,sizeof(MyFirst5kRunWeek6Workout3)  /sizeof(MyFirst5kRunDataStru)},
  //Week7
  {MyFirst5kRunWeek7Workout123,sizeof(MyFirst5kRunWeek7Workout123)/sizeof(MyFirst5kRunDataStru)},
  {MyFirst5kRunWeek7Workout123,sizeof(MyFirst5kRunWeek7Workout123)/sizeof(MyFirst5kRunDataStru)},
  {MyFirst5kRunWeek7Workout123,sizeof(MyFirst5kRunWeek7Workout123)/sizeof(MyFirst5kRunDataStru)},
  //Week8
  {MyFirst5kRunWeek8Workout123,sizeof(MyFirst5kRunWeek8Workout123)/sizeof(MyFirst5kRunDataStru)},
  {MyFirst5kRunWeek8Workout123,sizeof(MyFirst5kRunWeek8Workout123)/sizeof(MyFirst5kRunDataStru)},
  {MyFirst5kRunWeek8Workout123,sizeof(MyFirst5kRunWeek8Workout123)/sizeof(MyFirst5kRunDataStru)},
  //Week9
  {MyFirst5kRunWeek9Workout123,sizeof(MyFirst5kRunWeek9Workout123)/sizeof(MyFirst5kRunDataStru)},
  {MyFirst5kRunWeek9Workout123,sizeof(MyFirst5kRunWeek9Workout123)/sizeof(MyFirst5kRunDataStru)},
  {MyFirst5kRunWeek9Workout123,sizeof(MyFirst5kRunWeek9Workout123)/sizeof(MyFirst5kRunDataStru)},
};
void ProMyFirst5k_Set_Speeed(UINT16 Speed_Data)
{
//    if(SystemConfig_Get_Display_Unit(User_Get_UserSlot( )) == STANDARD)
//    {
        g_MyFirst5K.MyFirst5k_Data.Walk_Speed = Speed_Data & 0xFF;
        g_MyFirst5K.MyFirst5k_Data.Jog_Speed = (Speed_Data>>8)&0xFF;
//    }
//    else
//    {
//        g_MyFirst5K.MyFirst5k_Data.Walk_Speed = (Speed_Data & 0xFF)*1609/1000;
//        g_MyFirst5K.MyFirst5k_Data.Jog_Speed = ((Speed_Data>>8)&0xFF)*1609/1000;
//    }
}
void ProMyFirst5k_AppData_Refresh(void)
{
    MYFIRST5K_CONFIG ProMyFirst5k_Info = SystemConfig_Get_MyFirst5kInfo(User_Get_UserSlot());;
    if(SystemConfig_Get_Display_Unit(User_Get_UserSlot( )) == STANDARD)
    {
        g_MyFirst5K.MyFirst5k_Data.Walk_Speed = ProMyFirst5k_Info.Walk_Speed;
        g_MyFirst5K.MyFirst5k_Data.Jog_Speed = ProMyFirst5k_Info.Jog_Speed;
    }
    else
    {
        g_MyFirst5K.MyFirst5k_Data.Walk_Speed = ((UINT32)ProMyFirst5k_Info.Walk_Speed*1609/1000);
        g_MyFirst5K.MyFirst5k_Data.Jog_Speed = ((UINT32)ProMyFirst5k_Info.Jog_Speed*1609/1000);
        if(g_MyFirst5K.MyFirst5k_Data.Walk_Speed<Data_Get_Speed(MIN_SPEEDX10))g_MyFirst5K.MyFirst5k_Data.Walk_Speed=Data_Get_Speed(MIN_SPEEDX10);
        else if(g_MyFirst5K.MyFirst5k_Data.Walk_Speed>Data_Get_Speed(MAX_SPEEDX10))g_MyFirst5K.MyFirst5k_Data.Walk_Speed=Data_Get_Speed(MAX_SPEEDX10);
        if(g_MyFirst5K.MyFirst5k_Data.Jog_Speed<Data_Get_Speed(MIN_SPEEDX10))g_MyFirst5K.MyFirst5k_Data.Jog_Speed=Data_Get_Speed(MIN_SPEEDX10);
        else if(g_MyFirst5K.MyFirst5k_Data.Jog_Speed>Data_Get_Speed(MAX_SPEEDX10))g_MyFirst5K.MyFirst5k_Data.Jog_Speed=Data_Get_Speed(MAX_SPEEDX10);
    }
    
    g_MyFirst5K.MyFirst5k_Data.WarmupCooldown_Speed = g_MyFirst5K.MyFirst5k_Data.Walk_Speed*3/4;
    if(g_MyFirst5K.MyFirst5k_Data.WarmupCooldown_Speed < 10)
    {
        g_MyFirst5K.MyFirst5k_Data.WarmupCooldown_Speed = 10;
    }
    
    g_MyFirst5K.MyFirst5k_Data.Week = ProMyFirst5k_Info.Week;
    g_MyFirst5K.MyFirst5k_Data.Workout = ProMyFirst5k_Info.Workout;
   
    g_MyFirst5KBuff.MyFirst5k_Data.Week = ProMyFirst5k_Info.Week;
    g_MyFirst5KBuff.MyFirst5k_Data.Workout = ProMyFirst5k_Info.Workout;

    g_MyFirst5K.MyFirst5k_Data.Reset_Counts = ProMyFirst5k_Info.Reset_Counts;
    g_MyFirst5K.MyFirst5k_Data.lastUpdateYear = ProMyFirst5k_Info.lastUpdateYear;
    g_MyFirst5K.MyFirst5k_Data.lastUpdateMonth = ProMyFirst5k_Info.lastUpdateMonth;
    g_MyFirst5K.MyFirst5k_Data.lastUpdateDay = ProMyFirst5k_Info.lastUpdateDay;
    g_MyFirst5K.MyFirst5k_Data.WorkoutDataArrayIndex = (g_MyFirst5K.MyFirst5k_Data.Week-1)*3+(g_MyFirst5K.MyFirst5k_Data.Workout-1);
    g_MyFirst5K.MyFirst5k_Data.WorkoutTotalSegment = MyFirst5kWorkoutDataArray[g_MyFirst5K.MyFirst5k_Data.WorkoutDataArrayIndex].MyFirst5k_WorkoutDataTotalSegment;
    g_MyFirst5K.MyFirst5k_Data.WorkoutTotalCount = sizeof(MyFirst5kWorkoutDataArray)/sizeof(MyFirst5kWorkoutData);  
    g_MyFirst5K.MyFirst5k_Data.Stage_Time = MyFirst5kWorkoutDataArray[g_MyFirst5K.MyFirst5k_Data.WorkoutDataArrayIndex].MyFirst5kRunData[0].MyFirst5k_Time*30;
    g_MyFirst5K.MyFirst5k_Data.State = MyFirst5kWorkoutDataArray[g_MyFirst5K.MyFirst5k_Data.WorkoutDataArrayIndex].MyFirst5kRunData[0].MyFirst5k_Status;
    g_MyFirst5K.MyFirst5k_Data.WorkoutTargetTime = 0;
    for(char i=0;i<g_MyFirst5K.MyFirst5k_Data.WorkoutTotalSegment;i++)
    {
        g_MyFirst5K.MyFirst5k_Data.WorkoutTargetTime +=MyFirst5kWorkoutDataArray[g_MyFirst5K.MyFirst5k_Data.WorkoutDataArrayIndex].MyFirst5kRunData[i].MyFirst5k_Time*30;
        if(MyFirst5kWorkoutDataArray[g_MyFirst5K.MyFirst5k_Data.WorkoutDataArrayIndex].MyFirst5kRunData[i].MyFirst5k_Status==WARMUP)
        {
            Data_Set_WarmUP_Time(MyFirst5kWorkoutDataArray[g_MyFirst5K.MyFirst5k_Data.WorkoutDataArrayIndex].MyFirst5kRunData[i].MyFirst5k_Time*30);
        }
        else if(MyFirst5kWorkoutDataArray[g_MyFirst5K.MyFirst5k_Data.WorkoutDataArrayIndex].MyFirst5kRunData[i].MyFirst5k_Status==COOLDOWN)
        {
            Data_Set_CoolDown_Time(MyFirst5kWorkoutDataArray[g_MyFirst5K.MyFirst5k_Data.WorkoutDataArrayIndex].MyFirst5kRunData[i].MyFirst5k_Time*30);
        }
    }
}
void ProMyFirst5k_Save_Info(void)
{
    MYFIRST5K_CONFIG ProMyFirst5kInfo = SystemConfig_Get_MyFirst5kInfo(User_Get_UserSlot());
    
    ProMyFirst5kInfo.Workout = g_MyFirst5K.MyFirst5k_Data.Workout;
    ProMyFirst5kInfo.Week = g_MyFirst5K.MyFirst5k_Data.Week;
    if(SystemConfig_Get_Display_Unit(User_Get_UserSlot( )) == STANDARD)
    {
        ProMyFirst5kInfo.Walk_Speed = g_MyFirst5K.MyFirst5k_Data.Walk_Speed;
        ProMyFirst5kInfo.Jog_Speed = g_MyFirst5K.MyFirst5k_Data.Jog_Speed;
    }
    else
    {
        ProMyFirst5kInfo.Walk_Speed = ((UINT32)g_MyFirst5K.MyFirst5k_Data.Walk_Speed*12500000/201168+50)/100;
        ProMyFirst5kInfo.Jog_Speed = ((UINT32)g_MyFirst5K.MyFirst5k_Data.Jog_Speed*12500000/201168+50)/100;
    }
    if(g_MyFirst5K.MyFirst5k_Data.Workout == 2)
    {
        if(g_MyFirst5K.MyFirst5k_Data.Week == 1)
        {
            ProMyFirst5kInfo.OldWalk_SpeedBuff = ProMyFirst5kInfo.Walk_Speed;
            ProMyFirst5kInfo.OldJog_SpeedBuff = ProMyFirst5kInfo.Jog_Speed;
            
            ProMyFirst5kInfo.Walk_SpeedBuff = ProMyFirst5kInfo.Walk_Speed;
            ProMyFirst5kInfo.Jog_SpeedBuff = ProMyFirst5kInfo.Jog_Speed;
        }
        else
        {
            ProMyFirst5kInfo.OldWalk_SpeedBuff = ProMyFirst5kInfo.Walk_SpeedBuff;
            ProMyFirst5kInfo.OldJog_SpeedBuff = ProMyFirst5kInfo.Jog_SpeedBuff;
            
            ProMyFirst5kInfo.Walk_SpeedBuff = ProMyFirst5kInfo.Walk_Speed;
            ProMyFirst5kInfo.Jog_SpeedBuff = ProMyFirst5kInfo.Jog_Speed;
        }
    }
    ProMyFirst5kInfo.Reset_Counts = g_MyFirst5K.MyFirst5k_Data.Reset_Counts;
    ProMyFirst5kInfo.lastUpdateYear = g_MyFirst5K.MyFirst5k_Data.lastUpdateYear;
    ProMyFirst5kInfo.lastUpdateMonth = g_MyFirst5K.MyFirst5k_Data.lastUpdateMonth;
    ProMyFirst5kInfo.lastUpdateDay = g_MyFirst5K.MyFirst5k_Data.lastUpdateDay;
    
    if(SystemConfig_Set_MyFirst5kInfo(ProMyFirst5kInfo,User_Get_UserSlot()))
    {
        SystemConfigProcess(SVK_SAVE_MYFIRST5KPRG2FLASH, NULL,0);
    }
}

void MyFirst5k_Data_Initial(void)
{
    MYFIRST5K_CONFIG ProMyFirst5k_Info = SystemConfig_Get_MyFirst5kInfo(User_Get_UserSlot());;
    if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL )
    {
        g_MyFirst5K.StruProgram.Setting_Array=TM_ProMyFirst5k_SetUp;
    }
    else
    {
        g_MyFirst5K.StruProgram.Setting_Array=EP_ProMyFirst5k_SetUp;
    }
    memset(&g_MyFirst5K.MyFirst5k_Data,0,sizeof(MyFirst5kData));
    //д╜н^ию
    if(SystemConfig_Get_Display_Unit(User_Get_UserSlot( )) == STANDARD)
    {
        g_MyFirst5K.MyFirst5k_Data.Walk_Speed = ProMyFirst5k_Info.Walk_Speed;
        g_MyFirst5K.MyFirst5k_Data.Jog_Speed = ProMyFirst5k_Info.Jog_Speed;
    }
    else
    {
        g_MyFirst5K.MyFirst5k_Data.Walk_Speed = ((UINT32)ProMyFirst5k_Info.Walk_Speed*1609/1000);
        g_MyFirst5K.MyFirst5k_Data.Jog_Speed = ((UINT32)ProMyFirst5k_Info.Jog_Speed*1609/1000);
        if(g_MyFirst5K.MyFirst5k_Data.Walk_Speed<Data_Get_Speed(MIN_SPEEDX10))g_MyFirst5K.MyFirst5k_Data.Walk_Speed=Data_Get_Speed(MIN_SPEEDX10);
        else if(g_MyFirst5K.MyFirst5k_Data.Walk_Speed>Data_Get_Speed(MAX_SPEEDX10))g_MyFirst5K.MyFirst5k_Data.Walk_Speed=Data_Get_Speed(MAX_SPEEDX10);
        if(g_MyFirst5K.MyFirst5k_Data.Jog_Speed<Data_Get_Speed(MIN_SPEEDX10))g_MyFirst5K.MyFirst5k_Data.Jog_Speed=Data_Get_Speed(MIN_SPEEDX10);
        else if(g_MyFirst5K.MyFirst5k_Data.Jog_Speed>Data_Get_Speed(MAX_SPEEDX10))g_MyFirst5K.MyFirst5k_Data.Jog_Speed=Data_Get_Speed(MAX_SPEEDX10);
    }
    
    g_MyFirst5K.MyFirst5k_Data.WarmupCooldown_Speed = g_MyFirst5K.MyFirst5k_Data.Walk_Speed*3/4;
    if(g_MyFirst5K.MyFirst5k_Data.WarmupCooldown_Speed < 10)
    {
        g_MyFirst5K.MyFirst5k_Data.WarmupCooldown_Speed = 10;
    }
    
    g_MyFirst5K.MyFirst5k_Data.Week = ProMyFirst5k_Info.Week;
    g_MyFirst5K.MyFirst5k_Data.Workout = ProMyFirst5k_Info.Workout;
   
    g_MyFirst5KBuff.MyFirst5k_Data.Week = ProMyFirst5k_Info.Week;
    g_MyFirst5KBuff.MyFirst5k_Data.Workout = ProMyFirst5k_Info.Workout;

    g_MyFirst5K.MyFirst5k_Data.Reset_Counts = ProMyFirst5k_Info.Reset_Counts;
    g_MyFirst5K.MyFirst5k_Data.lastUpdateYear = ProMyFirst5k_Info.lastUpdateYear;
    g_MyFirst5K.MyFirst5k_Data.lastUpdateMonth = ProMyFirst5k_Info.lastUpdateMonth;
    g_MyFirst5K.MyFirst5k_Data.lastUpdateDay = ProMyFirst5k_Info.lastUpdateDay;
    g_MyFirst5K.MyFirst5k_Data.WorkoutDataArrayIndex = (g_MyFirst5K.MyFirst5k_Data.Week-1)*3+(g_MyFirst5K.MyFirst5k_Data.Workout-1);
    g_MyFirst5K.MyFirst5k_Data.WorkoutTotalSegment = MyFirst5kWorkoutDataArray[g_MyFirst5K.MyFirst5k_Data.WorkoutDataArrayIndex].MyFirst5k_WorkoutDataTotalSegment;
    g_MyFirst5K.MyFirst5k_Data.WorkoutTotalCount = sizeof(MyFirst5kWorkoutDataArray)/sizeof(MyFirst5kWorkoutData);  
    g_MyFirst5K.MyFirst5k_Data.Stage_Time = MyFirst5kWorkoutDataArray[g_MyFirst5K.MyFirst5k_Data.WorkoutDataArrayIndex].MyFirst5kRunData[0].MyFirst5k_Time*30;
    g_MyFirst5K.MyFirst5k_Data.State = MyFirst5kWorkoutDataArray[g_MyFirst5K.MyFirst5k_Data.WorkoutDataArrayIndex].MyFirst5kRunData[0].MyFirst5k_Status;
    for(char i=0;i<g_MyFirst5K.MyFirst5k_Data.WorkoutTotalSegment;i++)
    {
        g_MyFirst5K.MyFirst5k_Data.WorkoutTargetTime +=MyFirst5kWorkoutDataArray[g_MyFirst5K.MyFirst5k_Data.WorkoutDataArrayIndex].MyFirst5kRunData[i].MyFirst5k_Time*30;
        if(MyFirst5kWorkoutDataArray[g_MyFirst5K.MyFirst5k_Data.WorkoutDataArrayIndex].MyFirst5kRunData[i].MyFirst5k_Status==WARMUP)
        {
            Data_Set_WarmUP_Time(MyFirst5kWorkoutDataArray[g_MyFirst5K.MyFirst5k_Data.WorkoutDataArrayIndex].MyFirst5kRunData[i].MyFirst5k_Time*30);
        }
        else if(MyFirst5kWorkoutDataArray[g_MyFirst5K.MyFirst5k_Data.WorkoutDataArrayIndex].MyFirst5kRunData[i].MyFirst5k_Status==COOLDOWN)
        {
            Data_Set_CoolDown_Time(MyFirst5kWorkoutDataArray[g_MyFirst5K.MyFirst5k_Data.WorkoutDataArrayIndex].MyFirst5kRunData[i].MyFirst5k_Time*30);
        }
    }
    if(Bluetooth_Get_ConnectStatus())
    {
        UINT8 Result=MyFirst5k_Get_Result();
        if(Result==MyFirst5K_1WeekMissed)
        {
            Bluetooth_Set_VirtualKey((UINT16)VK_1WEEKMISSED);
            if(g_MyFirst5K.MyFirst5k_Data.Week>1)
            {
                g_MyFirst5K.MyFirst5k_Data.Week -= 1;
            }
            g_MyFirst5K.MyFirst5k_Data.Workout = 1; 
            //read Walk_Speed,Jog_Speed(last week)
            MYFIRST5K_CONFIG ProMyFirst5kInfo = SystemConfig_Get_MyFirst5kInfo(User_Get_UserSlot());
            if(SystemConfig_Get_Display_Unit(User_Get_UserSlot( )) == STANDARD)
            {
                g_MyFirst5K.MyFirst5k_Data.Walk_Speed = ProMyFirst5kInfo.OldWalk_SpeedBuff;
                g_MyFirst5K.MyFirst5k_Data.Jog_Speed = ProMyFirst5kInfo.OldJog_SpeedBuff;
            }
            else
            {
                g_MyFirst5K.MyFirst5k_Data.Walk_Speed = (UINT32)ProMyFirst5kInfo.OldWalk_SpeedBuff*1609/1000;
                g_MyFirst5K.MyFirst5k_Data.Jog_Speed = (UINT32)ProMyFirst5kInfo.OldJog_SpeedBuff*1609/1000;
            }
            
            RTC_Read_Time(&Rtc_Time);
            g_MyFirst5K.MyFirst5k_Data.lastUpdateYear=2000+Rtc_Time.year;
            g_MyFirst5K.MyFirst5k_Data.lastUpdateMonth=Rtc_Time.month;
            g_MyFirst5K.MyFirst5k_Data.lastUpdateDay=Rtc_Time.date;
            //save to flash
            ProMyFirst5k_Save_Info();
//            MyFirst5k_Data_Initial();
            ProMyFirst5k_AppData_Refresh();
        }
        else if(Result==MyFirst5K_2WeekMissed)
        {
            Bluetooth_Set_VirtualKey((UINT16)VK_2WEEKMISSED);
            g_MyFirst5K.MyFirst5k_Data.Week = 1;
            g_MyFirst5K.MyFirst5k_Data.Workout = 1;
            if(SystemConfig_Get_Display_Unit(User_Get_UserSlot( )) == STANDARD)
            {
                g_MyFirst5K.MyFirst5k_Data.Walk_Speed = 20;
                g_MyFirst5K.MyFirst5k_Data.Jog_Speed = 35;
            }
            else
            {
                g_MyFirst5K.MyFirst5k_Data.Walk_Speed = 32;
                g_MyFirst5K.MyFirst5k_Data.Jog_Speed = 56;
            }
            RTC_Read_Time(&Rtc_Time);
            g_MyFirst5K.MyFirst5k_Data.lastUpdateYear=2000+Rtc_Time.year;
            g_MyFirst5K.MyFirst5k_Data.lastUpdateMonth=Rtc_Time.month;
            g_MyFirst5K.MyFirst5k_Data.lastUpdateDay=Rtc_Time.date;
            //save to flash
            ProMyFirst5k_Save_Info();
            ProMyFirst5k_AppData_Refresh();
//            MyFirst5k_Data_Initial();
        }
    }
    Data_Set_StageTime(g_MyFirst5K.MyFirst5k_Data.Stage_Time);
    Data_Set_Time(g_MyFirst5K.MyFirst5k_Data.WorkoutTargetTime);
    Data_Set_Speed(g_MyFirst5K.MyFirst5k_Data.Walk_Speed,SystemConfig_Get_Display_Unit(User_Get_UserSlot( )),0);
	Data_Set_Incline(Data_Get_Incline(MIN_INCLINEX10),0);   
	Data_Set_Resistance(Data_Get_Resistance(MIN_RESISTANCE),0);
    g_MyFirst5K.MyFirst5k_Data.GoodLuckFlag = 2;
}

void MyFirst5k_BeginWorkout(void)
{
    g_MyFirst5K.MyFirst5k_Data.WarmupCooldown_Speed = g_MyFirst5K.MyFirst5k_Data.Walk_Speed*3/4;
    if(g_MyFirst5K.MyFirst5k_Data.WarmupCooldown_Speed < 10)
    {
        g_MyFirst5K.MyFirst5k_Data.WarmupCooldown_Speed = 10;
    }
	Data_Set_StageTime(g_MyFirst5K.MyFirst5k_Data.Stage_Time);	
	Data_Set_Time(g_MyFirst5K.MyFirst5k_Data.WorkoutTargetTime);
	Data_Set_Speed(g_MyFirst5K.MyFirst5k_Data.WarmupCooldown_Speed,SystemConfig_Get_Display_Unit(User_Get_UserSlot( )),0);
	Data_Set_Incline(Program_Get_Incline(0),0);   
	Data_Set_Resistance(Program_Get_Resistance(0),0);
    
//    ProMyFirst5k_Save_Info();
}
UINT16 MyFirst5k_SetUpData(UINT8 ParaIndex)
{
	UINT16 w_Dat=0;
    switch(ParaIndex)
	{
		case INDEX_INCLINE:
        w_Dat=Data_Get_Incline(TARGET_INCLINEX10);
        break;
		case INDEX_SPEED:
        if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)!=TREADMILL)
        {
            switch(g_MyFirst5K.MyFirst5k_Data.SettingState)
            {
                case SET_WALK_SPEED:
                w_Dat=g_MyFirst5K.MyFirst5k_Data.Walk_Speed;
                break;
                case SET_JOG_SPEED:
                w_Dat=g_MyFirst5K.MyFirst5k_Data.Jog_Speed;
                break;
            }
        }
        else
        {
            switch(g_MyFirst5K.MyFirst5k_Data.SettingState)
            {
                case SET_WALK_SPEED:
                case SET_JOG_SPEED:
                w_Dat=Data_Get_Speed(TARGET_SPEEDX10_AUTO);
                break;
            }
        }
        break;
		case INDEX_RESISTANCE:
        w_Dat=Data_Get_Resistance(TARGET_RESISTANCE);
        break;
		case INDEX_WEEK:
        w_Dat=  g_MyFirst5K.MyFirst5k_Data.Week;
        break;
		case INDEX_WORKOUT:
        w_Dat=g_MyFirst5K.MyFirst5k_Data.Workout;
        break;
        case INDEX_WEEKBUFF:
        w_Dat=  g_MyFirst5KBuff.MyFirst5k_Data.Week;
        break;
		case INDEX_WORKOUTBUFF:
        w_Dat=g_MyFirst5KBuff.MyFirst5k_Data.Workout;
        break;
		default:break;
    }
    return w_Dat;
}
UINT16 MyFirst5k_RunData(UINT8 ParaIndex)
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
        case INDEX_SPEED:
                UINT16 Workoutstate=MyFirst5kWorkoutDataArray[g_MyFirst5K.MyFirst5k_Data.WorkoutDataArrayIndex].MyFirst5kRunData[g_MyFirst5K.MyFirst5k_Data.Stage].MyFirst5k_Status;
                switch(Workoutstate)
                {
                    case JOG:
                    g_MyFirst5K.MyFirst5k_Data.Jog_Speed=Data_Get_Speed(TARGET_SPEEDX10_AUTO);
                    break;
                    case WALK:
                    g_MyFirst5K.MyFirst5k_Data.Walk_Speed=Data_Get_Speed(TARGET_SPEEDX10_AUTO);	
                    break;
                }        
                break;
     	}
	 return w_Dat;
}
PROFILE *MyFirst5k_ProfileShow(PROFILE_TAG tag_index,UCHAR* Dotmatrix_pos)
{
  UINT16 by_SEG;
  UINT8 by_Dat;
  UINT8 i,k;

  by_SEG = g_MyFirst5K.MyFirst5k_Data.WorkoutTotalSegment-1\
    -g_MyFirst5K.MyFirst5k_Data.Stage;
  if(g_MyFirst5K.MyFirst5k_Data.WorkoutTotalSegment<ProgramLibConfigPara->DotMatrixLength)
  {
    *Dotmatrix_pos = g_MyFirst5K.MyFirst5k_Data.Stage;
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
    if(k+g_MyFirst5K.MyFirst5k_Data.Stage<g_MyFirst5K.MyFirst5k_Data.WorkoutTotalSegment)
    {
      switch(MyFirst5kWorkoutDataArray[g_MyFirst5K.MyFirst5k_Data.WorkoutDataArrayIndex].MyFirst5kRunData[k+g_MyFirst5K.MyFirst5k_Data.Stage].MyFirst5k_Status)
      {
      case WARMUP:
        by_Dat=g_MyFirst5K.MyFirst5k_Data.WarmupCooldown_Speed;
        break;
      case JOG:
        by_Dat=g_MyFirst5K.MyFirst5k_Data.Jog_Speed;
        break;
      case WALK:
        by_Dat=g_MyFirst5K.MyFirst5k_Data.Walk_Speed;
        break;
      case COOLDOWN:
        if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
        {
          by_Dat=g_MyFirst5K.MyFirst5k_Data.WarmupCooldown_Speed;
        }
        else 
        {
          if(MyFirst5kWorkoutDataArray[g_MyFirst5K.MyFirst5k_Data.WorkoutDataArrayIndex].MyFirst5kRunData[g_MyFirst5K.MyFirst5k_Data.WorkoutTotalSegment-2].MyFirst5k_Status==JOG)
            by_Dat=g_MyFirst5K.MyFirst5k_Data.Jog_Speed;
          else if(MyFirst5kWorkoutDataArray[g_MyFirst5K.MyFirst5k_Data.WorkoutDataArrayIndex].MyFirst5kRunData[g_MyFirst5K.MyFirst5k_Data.WorkoutTotalSegment-2].MyFirst5k_Status==WALK)
            by_Dat=g_MyFirst5K.MyFirst5k_Data.Walk_Speed;
        }
        break;
      }
        Prg_ProfileData.InclineProfile[i]=Data_Get_Incline(TARGET_INCLINEX10);
        Prg_ProfileData.ResistanceProfile[i]=Data_Get_Resistance(TARGET_RESISTANCE);
    }
    else
    {
      by_Dat=0xff;
      Prg_ProfileData.InclineProfile[i]=0xff;
      Prg_ProfileData.ResistanceProfile[i]=0xff;
    }
    Prg_ProfileData.SpeedProfile[i]=by_Dat;
  }
  if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
  	{
  	switch(tag_index)
  		{
  		case SPEED_PROFILE:
			g_MyFirst5K.Profile.ProfileData=Prg_ProfileData.SpeedProfile;
			break;
  		case INCLINE_PROFILE:
			g_MyFirst5K.Profile.ProfileData=Prg_ProfileData.InclineProfile;
			break;
  		case DEFAULT_PROFILE:
			g_MyFirst5K.Profile.ProfileData=Prg_ProfileData.SpeedProfile;
			break;
			default:
			g_MyFirst5K.Profile.ProfileData=Prg_ProfileData.SpeedProfile;
				break;
  		}
  	}
  else
  	{
  	switch(tag_index)
  		{
  		case RESISTANCE_PROFILE:
			g_MyFirst5K.Profile.ProfileData=Prg_ProfileData.ResistanceProfile;
			break;
  		case INCLINE_PROFILE:
			g_MyFirst5K.Profile.ProfileData=Prg_ProfileData.InclineProfile;
			break;
  		case DEFAULT_PROFILE:
			g_MyFirst5K.Profile.ProfileData=Prg_ProfileData.ResistanceProfile;
			break;
			default:
			g_MyFirst5K.Profile.ProfileData=Prg_ProfileData.ResistanceProfile;
				break;
  		}  
	}  
  g_MyFirst5K.Profile.Length=ProgramLibConfigPara->DotMatrixLength;
  return &g_MyFirst5K.Profile;
}
PROGRAM_PENDING_EVTS MyFirst5k_SetUp(ProgramKeyEvts *keyevt)
{
    PROGRAM_PENDING_EVTS pendingEvt;
    pendingEvt.evt=PROGRAM_NONE_EVENT;
    memset(&pendingEvt.WorkOutEvts,0,sizeof(pendingEvt.WorkOutEvts));
    if(keyevt->Bit_All == 0) return pendingEvt;
    if(keyevt->Bit_Flag.Key_Enter)
    {
        keyevt->Bit_Flag.Key_Enter=false;
        if(g_MyFirst5K.StruProgram.Setting_Array[g_MyFirst5K.StruProgram.SettingArray_Index] != SET_PRESS_START )
        {
            keyevt->Bit_Flag.Key_StateChange=true;
            g_MyFirst5K.StruProgram.SettingArray_Index++;
        }
    }
    else if(keyevt->Bit_Flag.Key_Back)
    {
        keyevt->Bit_Flag.Key_Back=false;
        if(g_MyFirst5K.StruProgram.Setting_Array[g_MyFirst5K.StruProgram.SettingArray_Index] != NO_SETTING )
        {
            keyevt->Bit_Flag.Key_StateChange=true;
            g_MyFirst5K.StruProgram.SettingArray_Index--;
        }
    }
//    else if(keyevt->Bit_Flag.Key_Reset)
//    {
//        ProMyFirst5k_Save_Info();
//    }
    g_MyFirst5K.MyFirst5k_Data.SettingState=g_MyFirst5K.StruProgram.Setting_Array[g_MyFirst5K.StruProgram.SettingArray_Index];
    switch(g_MyFirst5K.MyFirst5k_Data.SettingState)
    {
        case SET_PROGRAM:
        if(keyevt->Bit_Flag.Key_StateChange)
        {
            keyevt->Bit_Flag.Key_StateChange=false;
            pendingEvt.evt=PROGRAM_CHANGE_EVENT;
            g_MyFirst5K.StruProgram.Prg_NextStatus(PS_SELECTPRG);
        }
        break;
        case SET_WAIT_STAGE:
        {
            UINT8 Result=MyFirst5k_Get_Result();
            if(Result==MyFirst5K_1WeekMissed)
            {
                pendingEvt.evt=MYFIRST5K_1WEEKMISSEDREPEATLASTWEEK;
                if(g_MyFirst5K.MyFirst5k_Data.Week>1)
                {
                    g_MyFirst5K.MyFirst5k_Data.Week -= 1;
                }
                g_MyFirst5K.MyFirst5k_Data.Workout = 1; 
                //read Walk_Speed,Jog_Speed(last week)
                MYFIRST5K_CONFIG ProMyFirst5kInfo = SystemConfig_Get_MyFirst5kInfo(User_Get_UserSlot());
                if(SystemConfig_Get_Display_Unit(User_Get_UserSlot( )) == STANDARD)
                {
                    g_MyFirst5K.MyFirst5k_Data.Walk_Speed = ProMyFirst5kInfo.OldWalk_SpeedBuff;
                    g_MyFirst5K.MyFirst5k_Data.Jog_Speed = ProMyFirst5kInfo.OldJog_SpeedBuff;
                }
                else
                {
                    g_MyFirst5K.MyFirst5k_Data.Walk_Speed = (UINT32)ProMyFirst5kInfo.OldWalk_SpeedBuff*1609/1000;
                    g_MyFirst5K.MyFirst5k_Data.Jog_Speed = (UINT32)ProMyFirst5kInfo.OldJog_SpeedBuff*1609/1000;
                }
                //save to flash
                ProMyFirst5k_Save_Info();
                MyFirst5k_Data_Initial();
            }
            else if(Result==MyFirst5K_2WeekMissed)
            {
                pendingEvt.evt=MYFIRST5K_2WEEKMISSEDSTARTOVER;
                g_MyFirst5K.MyFirst5k_Data.Week = 1;
                g_MyFirst5K.MyFirst5k_Data.Workout = 1;
                if(SystemConfig_Get_Display_Unit(User_Get_UserSlot( )) == STANDARD)
                {
                    g_MyFirst5K.MyFirst5k_Data.Walk_Speed = 20;
                    g_MyFirst5K.MyFirst5k_Data.Jog_Speed = 35;
                }
                else
                {
                    g_MyFirst5K.MyFirst5k_Data.Walk_Speed = 32;
                    g_MyFirst5K.MyFirst5k_Data.Jog_Speed = 56;
                }
                RTC_Read_Time(&Rtc_Time);
                g_MyFirst5K.MyFirst5k_Data.lastUpdateYear=2000+Rtc_Time.year;
                g_MyFirst5K.MyFirst5k_Data.lastUpdateMonth=Rtc_Time.month;
                g_MyFirst5K.MyFirst5k_Data.lastUpdateDay=Rtc_Time.date;
                //save to flash
                ProMyFirst5k_Save_Info();
                MyFirst5k_Data_Initial();
            }
            else if(keyevt->Bit_Flag.Key_StateChange)
            {
                keyevt->Bit_Flag.Key_StateChange=false;
                pendingEvt.evt=WEEK_WORKOUT_CHANGE_EVENT;
            }  
        }
        break;
        case SET_WALK_SPEED:
        if(keyevt->Bit_Flag.Key_StateChange)
        {
            keyevt->Bit_Flag.Key_StateChange=false;
            Data_Set_Speed(g_MyFirst5K.MyFirst5k_Data.Walk_Speed,SystemConfig_Get_Display_Unit(User_Get_UserSlot( )),0);
            pendingEvt.evt=WALKSPEED_CHANGE_EVENT;
        }      
        if(keyevt->Bit_Flag.Key_Up)
        {
            if(g_MyFirst5K.MyFirst5k_Data.Walk_Speed<Data_Get_Speed(MAX_SPEEDX10))
                g_MyFirst5K.MyFirst5k_Data.Walk_Speed +=1;
            else
                g_MyFirst5K.MyFirst5k_Data.Walk_Speed =Data_Get_Speed(MAX_SPEEDX10);
            Data_Set_Speed(g_MyFirst5K.MyFirst5k_Data.Walk_Speed,SystemConfig_Get_Display_Unit(User_Get_UserSlot( )),0);
        }
        else if(keyevt->Bit_Flag.Key_Dn)
        {
            if(g_MyFirst5K.MyFirst5k_Data.Walk_Speed>Data_Get_Speed(MIN_SPEEDX10))
                g_MyFirst5K.MyFirst5k_Data.Walk_Speed -=1;
            else
                g_MyFirst5K.MyFirst5k_Data.Walk_Speed =Data_Get_Speed(MIN_SPEEDX10);
        Data_Set_Speed(g_MyFirst5K.MyFirst5k_Data.Walk_Speed,SystemConfig_Get_Display_Unit(User_Get_UserSlot( )),0);
        }
        else if(keyevt->Bit_Flag.Key_QuickKey==true)
        {
            g_MyFirst5K.MyFirst5k_Data.Walk_Speed=Data_Get_Speed(TARGET_SPEEDX1000)/100;
        }
        pendingEvt.evt=WALKSPEED_CHANGE_EVENT;

        g_MyFirst5K.MyFirst5k_Data.WarmupCooldown_Speed=g_MyFirst5K.MyFirst5k_Data.Walk_Speed*3/4;
        if(g_MyFirst5K.MyFirst5k_Data.WarmupCooldown_Speed<10)
        {
            g_MyFirst5K.MyFirst5k_Data.WarmupCooldown_Speed=10;
        }
        break;    
        case SET_JOG_SPEED:
        if(keyevt->Bit_Flag.Key_StateChange)
        {
            keyevt->Bit_Flag.Key_StateChange=false;
            Data_Set_Speed(g_MyFirst5K.MyFirst5k_Data.Jog_Speed,SystemConfig_Get_Display_Unit(User_Get_UserSlot( )),0);
            pendingEvt.evt=JOGSPEED_CHANGE_EVENT;
        }      
        if(keyevt->Bit_Flag.Key_Up)
        {
            if(g_MyFirst5K.MyFirst5k_Data.Jog_Speed<Data_Get_Speed(MAX_SPEEDX10))
                g_MyFirst5K.MyFirst5k_Data.Jog_Speed +=1;
            else
                g_MyFirst5K.MyFirst5k_Data.Jog_Speed =Data_Get_Speed(MAX_SPEEDX10);
            Data_Set_Speed(g_MyFirst5K.MyFirst5k_Data.Jog_Speed,SystemConfig_Get_Display_Unit(User_Get_UserSlot( )),0);
        }
        else if(keyevt->Bit_Flag.Key_Dn)
        {
            if(g_MyFirst5K.MyFirst5k_Data.Jog_Speed>Data_Get_Speed(MIN_SPEEDX10))
                g_MyFirst5K.MyFirst5k_Data.Jog_Speed -=1;
            else
                g_MyFirst5K.MyFirst5k_Data.Jog_Speed =Data_Get_Speed(MIN_SPEEDX10);
            Data_Set_Speed(g_MyFirst5K.MyFirst5k_Data.Jog_Speed,SystemConfig_Get_Display_Unit(User_Get_UserSlot( )),0);
        } 
        else if(keyevt->Bit_Flag.Key_QuickKey==true)
        {
            g_MyFirst5K.MyFirst5k_Data.Jog_Speed=Data_Get_Speed(TARGET_SPEEDX1000)/100;
        }
        pendingEvt.evt=JOGSPEED_CHANGE_EVENT;
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
            pendingEvt.evt=RESISTANCE_CHANGE_EVENT;
        }
        if(keyevt->Bit_Flag.Key_Up)
        {
            Data_Set_Resistance(Program_Get_Resistance(0)+1,0);
            pendingEvt.evt=RESISTANCE_CHANGE_EVENT;
        }
        else if(keyevt->Bit_Flag.Key_Dn)
        {
            Data_Set_Resistance(Program_Get_Resistance(0)-1,0);
            pendingEvt.evt=RESISTANCE_CHANGE_EVENT;
        } 
        case SET_PRESS_START:
        if(keyevt->Bit_Flag.Key_StateChange)
        {
            keyevt->Bit_Flag.Key_StateChange=false;
            pendingEvt.evt=SETTING_COMPLETE_EVENT;
        }    
        break;
    }
    return pendingEvt; 
    
}
UINT16 UserSpeedx10;
        UINT16 TargetSpeed_xPercent;
        UINT16 TargetSpeedx10;
        UINT16 Workoutstate;
PROGRAM_PENDING_EVTS MyFirst5k_Run(ProgramKeyEvts *keyevt)
{
    UINT16 Target_Data;
    UINT16 Elapsed_Data;
    UINT16 Remaining_Data;
    static UINT16 LastValue=0xffff;
    
    PROGRAM_PENDING_EVTS pendingEvt;
    pendingEvt.evt=PROGRAM_NONE_EVENT;
    memset(&pendingEvt.WorkOutEvts,0,sizeof(pendingEvt.WorkOutEvts));
    if(g_MyFirst5K.StruProgram.Prg_CurrentStatus()<PS_WARMUP||g_MyFirst5K.StruProgram.Prg_CurrentStatus()>PS_COOLDOWN)return pendingEvt;
    Target_Data=Data_Get_Time(TARGET_TIME);
    Elapsed_Data=Data_Get_Time(ELAPSED_TIME);
    Remaining_Data=Target_Data-Elapsed_Data;
    if(g_MyFirst5K.MyFirst5k_Data.State==JOG || g_MyFirst5K.MyFirst5k_Data.State==WALK)
        g_MyFirst5K.MyFirst5k_Data.WarmupCooldown_Speed=g_MyFirst5K.MyFirst5k_Data.Walk_Speed*3/4;
	if(g_MyFirst5K.MyFirst5k_Data.WarmupCooldown_Speed<10)
	{
		g_MyFirst5K.MyFirst5k_Data.WarmupCooldown_Speed=10;
	}
    
    g_MyFirst5K.MyFirst5k_Data.State=MyFirst5kWorkoutDataArray[g_MyFirst5K.MyFirst5k_Data.WorkoutDataArrayIndex].MyFirst5kRunData[g_MyFirst5K.MyFirst5k_Data.Stage].MyFirst5k_Status;
    if(LastValue!=Elapsed_Data)
    {
        
        LastValue=Elapsed_Data;
        if(Remaining_Data==0)
        {
            if(g_MyFirst5K.StruProgram.Prg_CurrentStatus()!=PS_COMPLETED)
            {
                g_MyFirst5K.StruProgram.Prg_NextStatus(PS_COMPLETED);
                pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
                pendingEvt.WorkOutEvts.Workout_Complete_Evt=true;
                RTC_Read_Time(&Rtc_Time);
                g_MyFirst5K.MyFirst5k_Data.lastUpdateYear=2000+Rtc_Time.year;
                g_MyFirst5K.MyFirst5k_Data.lastUpdateMonth=Rtc_Time.month;
                g_MyFirst5K.MyFirst5k_Data.lastUpdateDay=Rtc_Time.date;
                if(g_MyFirst5K.MyFirst5k_Data.WorkoutDataArrayIndex<26)
                {
                    g_MyFirst5K.MyFirst5k_Data.GoodLuckFlag = 0;
                    switch(g_MyFirst5K.MyFirst5k_Data.Workout)
                    {
                        case 1:
                        case 2:
                        pendingEvt.WorkOutEvts.Workout_MyFirst5k_NextWorkoutIn2Days=true;
                        break;
                        case 3:
                        pendingEvt.WorkOutEvts.Workout_MyFirst5k_NextWorkoutIn3Days=true;
                        break;
                    }
                    if(g_MyFirst5K.MyFirst5k_Data.Workout<3)
                    {
//                        g_MyFirst5KBuff.MyFirst5k_Data.Workout = g_MyFirst5K.MyFirst5k_Data.Workout;
                        g_MyFirst5K.MyFirst5k_Data.Workout++;
                        //save to flash
                        ProMyFirst5k_Save_Info();
                    }
                    else
                    {
//                        g_MyFirst5KBuff.MyFirst5k_Data.Week = g_MyFirst5K.MyFirst5k_Data.Week;
                        g_MyFirst5K.MyFirst5k_Data.Week++;
                        g_MyFirst5K.MyFirst5k_Data.Workout=1;
                        g_MyFirst5K.MyFirst5k_Data.Walk_Speed += 1;
                        g_MyFirst5K.MyFirst5k_Data.Jog_Speed +=2;
                        if(Data_Get_Speed(MAX_SPEEDX10)<g_MyFirst5K.MyFirst5k_Data.Walk_Speed)
                            g_MyFirst5K.MyFirst5k_Data.Walk_Speed=Data_Get_Speed(MAX_SPEEDX10);
                        if(Data_Get_Speed(MAX_SPEEDX10)<g_MyFirst5K.MyFirst5k_Data.Jog_Speed)
                            g_MyFirst5K.MyFirst5k_Data.Jog_Speed=Data_Get_Speed(MAX_SPEEDX10);
                        //save to flash
                        ProMyFirst5k_Save_Info();
                    }
                }
                else
                {
                    pendingEvt.WorkOutEvts.Workout_MyFirst5k_CongratulationsGoodLuckWithYour5k=true;
                    g_MyFirst5K.MyFirst5k_Data.GoodLuckFlag = 1;
                    g_MyFirst5KBuff.MyFirst5k_Data.Week = g_MyFirst5K.MyFirst5k_Data.Week;
                    g_MyFirst5KBuff.MyFirst5k_Data.Workout = g_MyFirst5K.MyFirst5k_Data.Workout;
                    g_MyFirst5K.MyFirst5k_Data.Week = 1;
                    g_MyFirst5K.MyFirst5k_Data.Workout = 1;
                    if(SystemConfig_Get_Display_Unit(User_Get_UserSlot( )) == STANDARD)
                    {
                        g_MyFirst5K.MyFirst5k_Data.Walk_Speed = 20;
                        g_MyFirst5K.MyFirst5k_Data.Jog_Speed =35;
                    }
                    else
                    {
                        g_MyFirst5K.MyFirst5k_Data.Walk_Speed = 32;
                        g_MyFirst5K.MyFirst5k_Data.Jog_Speed = 56;
                    }
                    //save to flash
                    ProMyFirst5k_Save_Info();
                }
                
                // pendingEvt.WorkOutEvts.Workout_Complete_Evt=true;
            }
        }
        else 
        {
            if(Elapsed_Data==0)
            {
                g_MyFirst5K.StruProgram.Prg_NextStatus(PS_WARMUP);
                pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
//                pendingEvt.WorkOutEvts.Workout_Warmup_Evt=true;
            }
            else if(Elapsed_Data==5)
            {
                pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
                pendingEvt.WorkOutEvts.Workout_Warmup_Evt=true;
            }
            else if(Elapsed_Data==Data_Get_Time(WARMUP_TIME))
            {
                g_MyFirst5K.StruProgram.Prg_NextStatus(PS_RUN);
                pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
//                pendingEvt.WorkOutEvts.Workout_Run_Evt=true;
            }
            else if(Elapsed_Data==Data_Get_Time(WARMUP_TIME)+5)
            {
                pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
                pendingEvt.WorkOutEvts.Workout_Run_Evt=true;
            }
            else if(Remaining_Data==Data_Get_Time(COOLDOWN_TIME))  
            {
                if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)!=TREADMILL)
                {
                    Data_Set_Resistance(Program_Get_Resistance(0)/2,0);
                }
                g_MyFirst5K.StruProgram.Prg_NextStatus(PS_COOLDOWN);
                pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
//                pendingEvt.WorkOutEvts.Workout_Cooldown_Evt=true;
            }
            else if(Remaining_Data==(Data_Get_Time(COOLDOWN_TIME)-5))
            {
                pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
                pendingEvt.WorkOutEvts.Workout_Cooldown_Evt=true;
            }
            if(Data_Get_Time(TIME_IN_STAGE)==Data_Get_Time(STAGE_TIME))
            {
                if(g_MyFirst5K.MyFirst5k_Data.Stage<g_MyFirst5K.MyFirst5k_Data.WorkoutTotalSegment)
                {
                    g_MyFirst5K.MyFirst5k_Data.Stage++;
                    g_MyFirst5K.MyFirst5k_Data.Stage_Time=MyFirst5kWorkoutDataArray[g_MyFirst5K.MyFirst5k_Data.WorkoutDataArrayIndex].MyFirst5kRunData[g_MyFirst5K.MyFirst5k_Data.Stage].MyFirst5k_Time*30;
                    g_MyFirst5K.MyFirst5k_Data.State=MyFirst5kWorkoutDataArray[g_MyFirst5K.MyFirst5k_Data.WorkoutDataArrayIndex].MyFirst5kRunData[g_MyFirst5K.MyFirst5k_Data.Stage].MyFirst5k_Status;
                    Data_Set_StageTime(g_MyFirst5K.MyFirst5k_Data.Stage_Time);	
                    
                    if(g_MyFirst5K.MyFirst5k_Data.State==JOG)
                    {
                        if(Data_Get_Speed(TARGET_SPEEDX10_AUTO) != g_MyFirst5K.MyFirst5k_Data.Jog_Speed)
                        {
                            Beep_Set_Beeps(1,3,1);  
                            Data_Set_Speed(g_MyFirst5K.MyFirst5k_Data.Jog_Speed,SystemConfig_Get_Display_Unit(User_Get_UserSlot( )),0);
                        }
                    }
                    else if(g_MyFirst5K.MyFirst5k_Data.State==WALK)
                    {
                        if(Data_Get_Speed(TARGET_SPEEDX10_AUTO) != g_MyFirst5K.MyFirst5k_Data.Walk_Speed)
                        {
                            Beep_Set_Beeps(1,3,1);  
                            Data_Set_Speed(g_MyFirst5K.MyFirst5k_Data.Walk_Speed,SystemConfig_Get_Display_Unit(User_Get_UserSlot( )),0);
                        }  
                    }
                    else
                    {
                        if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
                        {
                            if(Data_Get_Speed(TARGET_SPEEDX10_AUTO) != g_MyFirst5K.MyFirst5k_Data.WarmupCooldown_Speed)
                            {
                                Beep_Set_Beeps(1,3,1);
                            }
                            Data_Set_Speed(g_MyFirst5K.MyFirst5k_Data.WarmupCooldown_Speed,SystemConfig_Get_Display_Unit(User_Get_UserSlot( )),0);
                        }
                        else
                        {
                            if(MyFirst5kWorkoutDataArray[g_MyFirst5K.MyFirst5k_Data.WorkoutDataArrayIndex].MyFirst5kRunData[g_MyFirst5K.MyFirst5k_Data.WorkoutTotalSegment-2].MyFirst5k_Status==JOG)
                            {  
                                if(Data_Get_Speed(TARGET_SPEEDX10_AUTO) != g_MyFirst5K.MyFirst5k_Data.Jog_Speed)
                                {
                                    Beep_Set_Beeps(1,3,1);
                                }
                                Data_Set_Speed(g_MyFirst5K.MyFirst5k_Data.Jog_Speed,SystemConfig_Get_Display_Unit(User_Get_UserSlot( )),0);
                            }
                            else if(MyFirst5kWorkoutDataArray[g_MyFirst5K.MyFirst5k_Data.WorkoutDataArrayIndex].MyFirst5kRunData[g_MyFirst5K.MyFirst5k_Data.WorkoutTotalSegment-2].MyFirst5k_Status==WALK)
                            {  
                                if(Data_Get_Speed(TARGET_SPEEDX10_AUTO) != g_MyFirst5K.MyFirst5k_Data.Walk_Speed)
                                {
                                    Beep_Set_Beeps(1,3,1);
                                }
                                Data_Set_Speed(g_MyFirst5K.MyFirst5k_Data.Walk_Speed,SystemConfig_Get_Display_Unit(User_Get_UserSlot( )),0);
                            }
                        }
                    }	
                }
            }
        }
    } 
    if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)!=TREADMILL && Elapsed_Data>Data_Get_Time(WARMUP_TIME)+5)
    {
        UINT16 UserSpeedx10=Data_Get_Speed(EPBI_SPEEDX10_AUTO);
        UINT16 TargetSpeed_xPercent;
        UINT16 TargetSpeedx10;
        UINT16 Workoutstate=MyFirst5kWorkoutDataArray[g_MyFirst5K.MyFirst5k_Data.WorkoutDataArrayIndex].MyFirst5kRunData[g_MyFirst5K.MyFirst5k_Data.Stage].MyFirst5k_Status;
        switch(Workoutstate)
        {
            case JOG:
            TargetSpeedx10=	g_MyFirst5K.MyFirst5k_Data.Jog_Speed;
            TargetSpeed_xPercent=g_MyFirst5K.MyFirst5k_Data.Jog_Speed/5;//20%
            break;
            case WALK:
            TargetSpeedx10=	g_MyFirst5K.MyFirst5k_Data.Walk_Speed;	
            TargetSpeed_xPercent=g_MyFirst5K.MyFirst5k_Data.Walk_Speed*3/10;//3//30%
            break;
        }
        if( Workoutstate== JOG || Workoutstate== WALK)
        {
            switch(g_MyFirst5K.MyFirst5k_Data.EpBikeSpeedState)
            {
                case NORMAL:
                if(UserSpeedx10>=TargetSpeedx10+TargetSpeed_xPercent)
                {
                    g_MyFirst5K.MyFirst5k_Data.EpBikeSpeedState=FASTER;
                    pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
                    pendingEvt.WorkOutEvts.Workout_MyFirst5k_SlowDown=true;
//                    Bluetooth_Set_VirtualKey(VK_SHOWSLOWDDOWN);
                }
                else if(UserSpeedx10+TargetSpeed_xPercent<=TargetSpeedx10)
                {
                    g_MyFirst5K.MyFirst5k_Data.EpBikeSpeedState=SLOWER;
                    pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
                    pendingEvt.WorkOutEvts.Workout_MyFirst5k_SpeedUp=true;
//                    Bluetooth_Set_VirtualKey(VK_SHOWSPEEDUP);
                }		
                break;
                case FASTER:
                if(UserSpeedx10+TargetSpeed_xPercent<=TargetSpeed_xPercent)
                {
                    g_MyFirst5K.MyFirst5k_Data.EpBikeSpeedState=SLOWER;
                    pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
                    pendingEvt.WorkOutEvts.Workout_MyFirst5k_SpeedUp=true;
//                    Bluetooth_Set_VirtualKey(VK_SHOWSPEEDUP);
                }
                else if((UserSpeedx10+TargetSpeed_xPercent>TargetSpeedx10) &&  (UserSpeedx10<TargetSpeedx10+TargetSpeed_xPercent))
                {
                    g_MyFirst5K.MyFirst5k_Data.EpBikeSpeedState=NORMAL;
                }
                break;
                case SLOWER:
                if(UserSpeedx10>=TargetSpeedx10+TargetSpeed_xPercent)
                {
                    g_MyFirst5K.MyFirst5k_Data.EpBikeSpeedState=FASTER;
                    pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
                    pendingEvt.WorkOutEvts.Workout_MyFirst5k_SlowDown=true;
//                    Bluetooth_Set_VirtualKey(VK_SHOWSLOWDDOWN);
                }
                else if((UserSpeedx10+TargetSpeed_xPercent>TargetSpeedx10) &&  (UserSpeedx10<TargetSpeedx10+TargetSpeed_xPercent))
                {
                    g_MyFirst5K.MyFirst5k_Data.EpBikeSpeedState=NORMAL;
                }
                break;
            }
        }
    }
    if(keyevt->Bit_Flag.Key_SpeedUp)
    {
        keyevt->Bit_Flag.Key_SpeedUp=false;
        if(g_MyFirst5K.MyFirst5k_Data.State==JOG)
        {
            if(g_MyFirst5K.MyFirst5k_Data.Jog_Speed<Data_Get_Speed(MAX_SPEEDX10))
                g_MyFirst5K.MyFirst5k_Data.Jog_Speed +=1;
            else
                g_MyFirst5K.MyFirst5k_Data.Jog_Speed =Data_Get_Speed(MAX_SPEEDX10);
            Data_Set_Speed(g_MyFirst5K.MyFirst5k_Data.Jog_Speed,SystemConfig_Get_Display_Unit(User_Get_UserSlot( )),0);
        }
        else if(g_MyFirst5K.MyFirst5k_Data.State==WALK)
        {
            if(g_MyFirst5K.MyFirst5k_Data.Walk_Speed<Data_Get_Speed(MAX_SPEEDX10))
                g_MyFirst5K.MyFirst5k_Data.Walk_Speed +=1;
            else
                g_MyFirst5K.MyFirst5k_Data.Walk_Speed =Data_Get_Speed(MAX_SPEEDX10);
            Data_Set_Speed(g_MyFirst5K.MyFirst5k_Data.Walk_Speed,SystemConfig_Get_Display_Unit(User_Get_UserSlot( )),0);
        }
        else
            Data_Set_Speed(Data_Get_Speed(TARGET_SPEEDX10_AUTO)+1,SystemConfig_Get_Display_Unit(User_Get_UserSlot( )),0);
        
    }
    else if(keyevt->Bit_Flag.Key_SpeedDn)
    {
        keyevt->Bit_Flag.Key_SpeedDn=false;
        if(g_MyFirst5K.MyFirst5k_Data.State==JOG)
        {
            if(g_MyFirst5K.MyFirst5k_Data.Jog_Speed>Data_Get_Speed(MIN_SPEEDX10))
                g_MyFirst5K.MyFirst5k_Data.Jog_Speed -=1;
            else
                g_MyFirst5K.MyFirst5k_Data.Jog_Speed =Data_Get_Speed(MIN_SPEEDX10);
            Data_Set_Speed(g_MyFirst5K.MyFirst5k_Data.Jog_Speed,SystemConfig_Get_Display_Unit(User_Get_UserSlot( )),0);
        }
        else if(g_MyFirst5K.MyFirst5k_Data.State==WALK)
        {
            if(g_MyFirst5K.MyFirst5k_Data.Walk_Speed>Data_Get_Speed(MIN_SPEEDX10))
                g_MyFirst5K.MyFirst5k_Data.Walk_Speed -=1;
            else
                g_MyFirst5K.MyFirst5k_Data.Walk_Speed =Data_Get_Speed(MIN_SPEEDX10);
            Data_Set_Speed(g_MyFirst5K.MyFirst5k_Data.Walk_Speed,SystemConfig_Get_Display_Unit(User_Get_UserSlot( )),0);
        }
        else
            Data_Set_Speed(Data_Get_Speed(TARGET_SPEEDX10_AUTO)-1,SystemConfig_Get_Display_Unit(User_Get_UserSlot( )),0);
    }
    if(keyevt->Bit_Flag.Key_SPDQuickKey==true)
    {
        if(g_MyFirst5K.MyFirst5k_Data.State==WALK)
        {
            if(Data_Get_Speed(TARGET_SPEEDX10_AUTO)!=g_MyFirst5K.MyFirst5k_Data.Walk_Speed)
            {
                g_MyFirst5K.MyFirst5k_Data.Walk_Speed=Data_Get_Speed(TARGET_SPEEDX10_AUTO);
            }
        }
        else if(g_MyFirst5K.MyFirst5k_Data.State==JOG)
        {
            if(Data_Get_Speed(TARGET_SPEEDX10_AUTO)!=g_MyFirst5K.MyFirst5k_Data.Jog_Speed)
            {
                g_MyFirst5K.MyFirst5k_Data.Jog_Speed=Data_Get_Speed(TARGET_SPEEDX10_AUTO);
            }
        }
        else
        {
            if(Data_Get_Speed(TARGET_SPEEDX10_AUTO)!=g_MyFirst5K.MyFirst5k_Data.WarmupCooldown_Speed)
            {
                g_MyFirst5K.MyFirst5k_Data.WarmupCooldown_Speed=Data_Get_Speed(TARGET_SPEEDX10_AUTO);
            }
        }
    }
    if(keyevt->Bit_Flag.Key_InclineUp)
    {
        keyevt->Bit_Flag.Key_InclineUp=false;
        Data_Set_Incline(Program_Get_Incline(0)+5,0);
    }
    else if(keyevt->Bit_Flag.Key_InclineDn)
    {
        keyevt->Bit_Flag.Key_InclineDn=false;
        if(Program_Get_Incline(0))
            Data_Set_Incline(Program_Get_Incline(0)-5,0);
    }      
    if(keyevt->Bit_Flag.Key_ResistanceUp)
    {
        keyevt->Bit_Flag.Key_ResistanceUp=false;
        Data_Set_Resistance(Program_Get_Resistance(0)+1,0);
    }
    else if(keyevt->Bit_Flag.Key_ResistanceDn)
    {
        keyevt->Bit_Flag.Key_ResistanceDn=false;
        Data_Set_Resistance(Program_Get_Resistance(0)-1,0);
    }
    return pendingEvt;
    
}
//MyFirst5K ProMyFirst5k_Get_WeekWorkoutBuff(void)
//{
//    return g_MyFirst5KBuff;
//}
void initMyFirst5k(void)
{
	g_MyFirst5K.StruProgram.Programid= PS_MYFIRST5K;
	g_MyFirst5K.StruProgram.SettingArray_Index=0;
	g_MyFirst5K.StruProgram.Prg_Initial=MyFirst5k_Data_Initial;
	g_MyFirst5K.StruProgram.BeginWorkout=MyFirst5k_BeginWorkout;
	g_MyFirst5K.StruProgram.Prg_ProfileShow=MyFirst5k_ProfileShow;
	g_MyFirst5K.StruProgram.Prg_Setup=MyFirst5k_SetUp;
	g_MyFirst5K.StruProgram.Prg_Workout=MyFirst5k_Run;
	g_MyFirst5K.StruProgram.Prg_SetupData=MyFirst5k_SetUpData;
	g_MyFirst5K.StruProgram.Prg_RunData=MyFirst5k_RunData;

  Program_Register_Function(&g_MyFirst5K.StruProgram);
}

static UCHAR Rtc_Is_LeapYear(UINT16 year)
{
    return(((year %4 ==0 && year%100 !=0) || !(year %400)));
}

static UCHAR Rtc_Get_NumberOfDay(char by_Month,UINT16 by_Year)
{
    const static UCHAR aby_NDM[12]={31,28,31,30,31,30,31,31,30,31,30,31};
    UCHAR by_NumberOfDay;
    if(Rtc_Is_LeapYear(by_Year) && by_Month==2 )
    {
        by_NumberOfDay = 29;
    }
    else
    {
        by_NumberOfDay = aby_NDM[by_Month-1];
    }
    return(by_NumberOfDay);
}
UINT8 ProMyFirst5k_Get_GoodLuckFlag(void)
{
   return g_MyFirst5K.MyFirst5k_Data.GoodLuckFlag;
}
static UINT8 MyFirst5k_Get_Result(void)
{   
    UINT16 result=MyFirst5K_NoWeekMissed;
    UINT16 w_RtcYear;
    
    if(g_MyFirst5K.MyFirst5k_Data.Week == 1 && g_MyFirst5K.MyFirst5k_Data.Workout == 1)return result;
    RTC_Read_Time(&Rtc_Time);
    w_RtcYear =2000+Rtc_Time.year;
//     if(g_MyFirst5K.MyFirst5k_Data.lastUpdateYear>w_RtcYear)
//     {
//         result= MyFirst5K_2WeekMissed;
//     }
//     else
//     {      
       if(w_RtcYear-g_MyFirst5K.MyFirst5k_Data.lastUpdateYear>1)
       {
           result= MyFirst5K_2WeekMissed;
       }
       else if(w_RtcYear-g_MyFirst5K.MyFirst5k_Data.lastUpdateYear==1)
       {
             if(Rtc_Time.month+12-g_MyFirst5K.MyFirst5k_Data.lastUpdateMonth>1)
             {
                 result= MyFirst5K_2WeekMissed;
             }
             else if(Rtc_Time.month+12-g_MyFirst5K.MyFirst5k_Data.lastUpdateMonth==1)
             {
                 if(Rtc_Time.date+31-g_MyFirst5K.MyFirst5k_Data.lastUpdateDay>14)
                 {
                     result= MyFirst5K_2WeekMissed;
                 }
                 else if(Rtc_Time.date+31-g_MyFirst5K.MyFirst5k_Data.lastUpdateDay>7)
                 {
                     result= MyFirst5K_1WeekMissed;
                 }
             }
       }
       else
       {
            if(Rtc_Time.month-g_MyFirst5K.MyFirst5k_Data.lastUpdateMonth>1)
             {
                 result= MyFirst5K_2WeekMissed;
             }
             else if(Rtc_Time.month-g_MyFirst5K.MyFirst5k_Data.lastUpdateMonth==1)
             {
                 if(Rtc_Time.date+Rtc_Get_NumberOfDay(g_MyFirst5K.MyFirst5k_Data.lastUpdateMonth,w_RtcYear)-g_MyFirst5K.MyFirst5k_Data.lastUpdateDay>14)
                 {
                     result= MyFirst5K_2WeekMissed;
                 }
                 else if(Rtc_Time.date+Rtc_Get_NumberOfDay(g_MyFirst5K.MyFirst5k_Data.lastUpdateMonth,w_RtcYear)-g_MyFirst5K.MyFirst5k_Data.lastUpdateDay>7)
                 {
                     result= MyFirst5K_1WeekMissed;
                 }
             }
            else if(Rtc_Time.month-g_MyFirst5K.MyFirst5k_Data.lastUpdateMonth==0)
            {
                 if(Rtc_Time.date-g_MyFirst5K.MyFirst5k_Data.lastUpdateDay>14)
                 {
                     result= MyFirst5K_2WeekMissed;
                 }
                 else if(Rtc_Time.date-g_MyFirst5K.MyFirst5k_Data.lastUpdateDay>7)
                 {
                     result= MyFirst5K_1WeekMissed;
                 }
                     
            }          
       }
//     }
     return result;
}

void ProMyFirst5k_Set_DefaultData(void)
{
    g_MyFirst5KBuff.MyFirst5k_Data.Week = 1;
    g_MyFirst5KBuff.MyFirst5k_Data.Workout = 1;
    g_MyFirst5K.MyFirst5k_Data.Week = 1;
    g_MyFirst5K.MyFirst5k_Data.Workout = 1;
    g_MyFirst5K.MyFirst5k_Data.Walk_Speed = 20;
    g_MyFirst5K.MyFirst5k_Data.Jog_Speed =35;
}

#endif
