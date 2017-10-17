#include "Program_Inner.h"
#include "HAL_Lib.h"
#include "Setting_Config.h"
#include"Pro_manual.h"
#include"Pro_goal.h"
#include"Pro_hrc.h"
#include"Pro_table.h"
#include"Pro_constant.h"
#include"ProSprint8.h"
#include"Pro_MyFirst5k.h"
#include "Pro_Custom.h"
#include "Bluetooth_lib.h"
#include "page.h"
#include <string.h>
//#include "Pro_distance.h"
//#include "Pro_calories.h"

#define PROGRAM_VISION "1.0.2"
#define SETPARANUM 8
#define TIMER_CH_NUM 16
typedef struct
{
    UINT16 ScanMode:1;
    UINT16 ReadPWM:1;
    UINT16 HRSignalStatus:1;	
}PrgAction_Flag;
typedef struct
{
    UINT16 Timer_Delay[TIMER_CH_NUM];
    UINT8  Timer_Channel[TIMER_CH_NUM];
}PrgTimer_Isr;
typedef struct
{
    UINT16 TimerDelay;
    UINT8 TimerCount;
    UINT8 WaitFlag:1;
}Prg321_Isr;
typedef struct
{
    UINT8 LessOneMileCounter;
}Prg_SpecialFunctionData;
const struct
{
    struct
    {
        unsigned char Used : 1;   // 0:不使用数字键, 1:使用
        unsigned char Digits : 7; // 数字位数
    }NumericKey;    
    unsigned char SetState;
    unsigned char EventChange;
}DefaultNumber[SETPARANUM] = 
{
    {1,2,SET_TIME,TIME_CHANGE_EVENT},              
    {1,3,SET_SPEED,SPEED_CHANGE_EVENT},              
    {1,3,SET_INCLINE,INCLINE_CHANGE_EVENT},              
    {1,2,SET_RESISTANCE,RESISTANCE_CHANGE_EVENT},              
    {1,2,SET_LEVEL,LEVEL_CHANGE_EVENT},              
    {1,3,SET_HEART,HEART_CHANGE_EVENT},              
    {1,3,SET_WATTS,WATTS_CHANGE_EVENT},              
};

const char ProgramUUID[PS_TOTALS][37]=
{
    {"00000000-0000-0000-0000-000000000000"},//PS_MANUAL
    {"00000000-0000-0000-0000-000000000000"},//PS_INTERVALS
    {"00000000-0000-0000-0000-000000000000"},//PS_WEIGHTLOSS
    {"00000000-0000-0000-0000-000000000000"},//PS_ROLLING_HILLS
    {"00000000-0000-0000-0000-000000000000"},//PS_SPRINT8
    {"00000000-0000-0000-0000-000000000000"},//PS_THR_ZONE
    {"00000000-0000-0000-0000-000000000000"},//PS_INTERVALS_HR
    {"00000000-0000-0000-0000-000000000000"},//PS_STEPS_THR
    {"00000000-0000-0000-0000-000000000000"},//PS_SPRING_CLASSIC
    {"00000000-0000-0000-0000-000000000000"},//PS_INTERVAL_PLUS
    {"00000000-0000-0000-0000-000000000000"},//PS_WEIGHTLOSS_PLUS
    {"00000000-0000-0000-0000-000000000000"},//PS_DISTANCE_GOAL
    {"00000000-0000-0000-0000-000000000000"},//PS_CALORIES_GOAL
    {"00000000-0000-0000-0000-000000000000"},//PS_STEPS_GOAL
    {"00000000-0000-0000-0000-000000000000"},//PS_CLIMB_GOAL
    {"00000000-0000-0000-0000-000000000000"},//PS_5K
    {"00000000-0000-0000-0000-000000000000"},//PS_10K
    {"00000000-0000-0000-0000-000000000000"},//PS_20K
    {"00000000-0000-0000-0000-000000000000"},//PS_40K
    {"00000000-0000-0000-0000-000000000000"},//PS_PTHR
    {"00000000-0000-0000-0000-000000000000"},//PS_THR_INTERVALS
    {"00000000-0000-0000-0000-000000000000"},//PS_CONSTANT_WATTS
    {"00000000-0000-0000-0000-000000000000"},//PS_WATT_INTERVALS
    {"00000000-0000-0000-0000-000000000000"},//PS_GERKIN
    {"00000000-0000-0000-0000-000000000000"},//PS_PEB
    {"00000000-0000-0000-0000-000000000000"},//PS_FATBURN
};
const int aby_pwm_Offset[30]=
{
    //L1	L2	L3	L4	L5	L6	L7	L8	L9	L10	L11	L12	L13	L14	L15	L16	L17	L18	L19	L20	L21	L22	L23	L24	L25	L26	L27	L28	L29	L30
    10,	13,	17,	20,	24,	27,	29,	31,	33,	35,	37,	39,	41,	43,	45,	47,	50,	52,	54,	57,	59,	61,	64,	66,	68,	71,	73,	75,	78,	80,
};
__no_init static PROGRAM_STATUS Prg_StateMachine,Prg_StateMachineBackup;

__no_init static PrgAction_Flag PrgActionFlag;
__no_init static PrgTimer_Isr PrgTimerIsr;
__no_init static Prg321_Isr Prg321Isr;
__no_init static Prg_SpecialFunctionData PrgSpecialFunctionData;

__no_init static struProgram *ProgramArray[PS_TOTALS];
__no_init static UCHAR proCurrentIndex;
__no_init static char ProgramidArrayIndex;
__no_init static char ProgramSubidArrayIndex;

__no_init UINT16 Target_calories;
__no_init TARGET_DIS_ENUM Target_distance;
__no_init UINT16 Target_steps;

__no_init ProgramLibPara *ProgramLibConfigPara;
__no_init const WattsParameterStruct *Watts_ParameterData;
__no_init  Prg_ProfileStruct Prg_ProfileData;

__no_init static UCHAR Profile_Pos;
__no_init static UCHAR ResistanceDataBackup,InclineDataBackup;
__no_init static UINT16 SpeedDataBackup;
__no_init NumberKeyData NumberKey_Data;
__no_init  UINT8 DefaultDotmatrix;
__no_init  UINT8 ProfileIndex;

__no_init  UINT16 InductionOutput_5P[5];
__no_init  UINT16 InductionOutput_30P[30];
extern VR_PAGE  nCurrentPage;
extern VR_PAGE  g_BackPage;
extern void Pro_table_Set_Data(UINT16 w_Data);
extern void ProMyFirst5k_Set_Speeed(UINT16 Speed_Data);

static ProgramKeyEvts PrgKeyEvts;

void Program_1ms_ISR(void);
UINT8 Prg_isr_TimerCounter(UINT8 by_Who,UINT16 w_Dat);
void Prg_isr_TimerCounterClear(UINT8 by_Index);
void Prg_isr_TimerColse(UINT8 by_Dat);

UINT8 Prg_isr_TimerCounter(UINT8 by_Who,UINT16 w_Dat)
{
    UINT8 b_Out = 0;
    if(PrgTimerIsr.Timer_Channel[by_Who] == 1 && PrgTimerIsr.Timer_Delay[by_Who] >= w_Dat)
    {
        b_Out=1;
        PrgTimerIsr.Timer_Channel[by_Who] = 0;
    }
    return b_Out;
}
void Prg_isr_TimerCounterClear(UINT8 by_Index)
{
    PrgTimerIsr.Timer_Delay[by_Index]=0;
    PrgTimerIsr.Timer_Channel[by_Index]=1;
}
void Prg_isr_TimerColse(UINT8 by_Dat)
{
    PrgTimerIsr.Timer_Channel[by_Dat]=0;
}
void Program_1ms_ISR(void)
{
    static UINT16 w_Timer_100ms=0;
    if(Program_Get_Status()==PS_WAIT_3S||Program_Get_Status()==PS_RESTART_321)
    {
        if(!Prg321Isr.WaitFlag)
        {
            if(Prg321Isr.TimerDelay++>850)
            {
                Prg321Isr.TimerCount++;
                Prg321Isr.TimerDelay=0;
                Prg321Isr.WaitFlag=1;
            }
        }
    }  
    if(++w_Timer_100ms<100)return;
    w_Timer_100ms=0;
    for(UINT8 i=0;i<TIMER_CH_NUM;i++)
    {
        if(PrgTimerIsr.Timer_Channel[i]==1 && PrgTimerIsr.Timer_Delay[i]<0xffff)
        {
            PrgTimerIsr.Timer_Delay[i]++;
        }
    }
}
void Program_Read_5PointData(void)
{
    int by_Dat=0;
    unsigned int w_K=0;
    unsigned int w_B=0;
    char by_Loop=0;
    if(PrgActionFlag.ReadPWM)
    {
        PrgActionFlag.ReadPWM=0;
        LCB_Get_Induct(InductionOutput_5P);
        if(InductionOutput_5P[0]<InductionOutput_5P[2] && InductionOutput_5P[2]<InductionOutput_5P[4])
        {
            InductionOutput_30P[0]=InductionOutput_5P[0];
            InductionOutput_30P[14]=InductionOutput_5P[2];
            InductionOutput_30P[29]=InductionOutput_5P[4];
            if((InductionOutput_30P[5]==0) && (InductionOutput_30P[14]!=0) && (InductionOutput_30P[21]==0))
            {
                if(SystemConfig_Get_Char(DRIVE_TYPE_UINT8)==INDUCTOR_BRAKE_INCLINE)
                {
                    w_K=(int)((InductionOutput_30P[14]-InductionOutput_30P[0])/(14-0));
                    w_B=(int)(InductionOutput_30P[0]-w_K);
                    InductionOutput_30P[5] = w_K*6+w_B;
                    
                    w_K=(int)((InductionOutput_30P[29]-InductionOutput_30P[14])/(29-14)*10/12);
                    w_B=(int)(InductionOutput_30P[14]-w_K);
                    InductionOutput_30P[21] = w_K*8+w_B;
                }
                else
                {
                    w_K=(int)((InductionOutput_30P[14]-InductionOutput_30P[0])/(14-0)*13/10);
                    w_B=(int)(InductionOutput_30P[0]-w_K);
                    InductionOutput_30P[5] = w_K*6+w_B;
                    
                    w_K=(int)((InductionOutput_30P[29]-InductionOutput_30P[14])/(29-14));
                    w_B=(int)(InductionOutput_30P[14]-w_K);
                    InductionOutput_30P[21] = w_K*8+w_B;
                }
                
            }
            by_Dat=(InductionOutput_30P[5]-InductionOutput_30P[0])/5; 
            for(by_Loop=1;by_Loop<5;by_Loop++)
                InductionOutput_30P[by_Loop]=InductionOutput_30P[by_Loop-1]+by_Dat;
            
            by_Dat=(InductionOutput_30P[14]-InductionOutput_30P[5])/9; 
            for(by_Loop=6;by_Loop<14;by_Loop++)
                InductionOutput_30P[by_Loop]=InductionOutput_30P[by_Loop-1]+by_Dat;
            
            by_Dat=(InductionOutput_30P[21]-InductionOutput_30P[14])/7; 
            for(by_Loop=15;by_Loop<21;by_Loop++)
                InductionOutput_30P[by_Loop]=InductionOutput_30P[by_Loop-1]+by_Dat;
            
            by_Dat=(InductionOutput_30P[29]-InductionOutput_30P[21])/8; 
            for(by_Loop=22;by_Loop<29;by_Loop++)
                InductionOutput_30P[by_Loop]=InductionOutput_30P[by_Loop-1]+by_Dat;     
        }
        else
        {
            for(char i=0;i<30;i++)
                InductionOutput_30P[i]=Watts_ParameterData->PWM_ECBCount_Table[i];
        }
        if(SystemConfig_Get_Char(DRIVE_TYPE_UINT8)==BIKE_0C_JIS)
        {
            for(char i=0;i<30;i++)
                InductionOutput_30P[i] +=aby_pwm_Offset[i];
        }
    }
    if(ProgramArray[proCurrentIndex]->Programid != PS_CONSTANT_WATTS)
    {
		if(Data_Get_Resistance(TARGET_RESISTANCE)<SystemConfig_Get_Char(MIN_RESISTANCE_UINT8))
		{
			Data_Set_TargetPWMValue(InductionOutput_30P[0]);
		}
		else
		{
			Data_Set_TargetPWMValue(InductionOutput_30P[Data_Get_Resistance(TARGET_RESISTANCE)-SystemConfig_Get_Char(MIN_RESISTANCE_UINT8)]);
		}
    }
}

void Program_Initial_Inner_Data(void)
{
    Data_Process_Intilal_Data();
    memset(&Prg_ProfileData.SpeedProfile,Data_Get_Speed(TARGET_SPEEDX10_AUTO),sizeof(Prg_ProfileData.SpeedProfile));
    memset(&Prg_ProfileData.InclineProfile,0,sizeof(Prg_ProfileData.InclineProfile));
    memset(&Prg_ProfileData.ResistanceProfile,Data_Get_Resistance(MIN_RESISTANCE),sizeof(Prg_ProfileData.ResistanceProfile));
    memset(&ProgramArray,0,sizeof(struProgram));
    memset(&NumberKey_Data,0,sizeof(NumberKeyData));
    memset(&PrgTimerIsr,0,sizeof(PrgTimer_Isr));
    memset(&PrgActionFlag,0,sizeof(PrgAction_Flag));
    memset(&Prg321Isr,0,sizeof(Prg321_Isr));
    memset(&PrgSpecialFunctionData,0,sizeof(Prg_SpecialFunctionData));
    memset(&InductionOutput_30P,0,sizeof(InductionOutput_30P));
	
    PrgActionFlag.ReadPWM=1;
    Prg321Isr.WaitFlag=1;
    PrgSpecialFunctionData.LessOneMileCounter=0;
    proCurrentIndex = 0;
    Profile_Pos=0;
    Target_distance=TARGET_1MILE;
    Target_steps=1000;
    Target_calories=100;
    ProgramidArrayIndex=0;
	ProgramSubidArrayIndex=0;
    if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
        ProfileIndex=DefaultDotmatrix=PROFILE_TYPE_SPEED;
    else
        ProfileIndex=DefaultDotmatrix=PROFILE_TYPE_RESISTANCE;
    SpeedDataBackup=Data_Get_Speed(TARGET_SPEEDX1000);
    InclineDataBackup=Data_Get_Incline(TARGET_INCLINEX10);
    ResistanceDataBackup=Data_Get_Resistance(TARGET_RESISTANCE);
    
#ifdef __PROGRAM_MANUAL__
    initManual();
#endif
#ifdef __PROGRAM_GOAL__
    initGoal();
#endif    
//#ifdef __PROGRAM_DISTANCE__
//    initDistance();
//#endif    
//#ifdef __PROGRAM_CALORIES__
//    initCalories();
//#endif
#ifdef __PROGRAM_HRC__
    initHrc();
#endif
#ifdef __PROGRAM_TABLE__
    initTable();
#endif
#ifdef __PROGRAM_CONSTANT__
    initConstantWatts();
#endif
#ifdef __PROGRAM_SPRINT8__
    initSprint8();
#endif
#ifdef __PROGRAM_MYFIRST_5K__
    initMyFirst5k();
#endif
#ifdef __PROGRAM_CUSTOM__
    initCustom();
#endif
    Prg_StateMachine=Prg_StateMachineBackup=PS_IDLE;
    
    extern UINT8 appStart;    //zsf
    appStart = 0;    //zsf
}
void Program_NextStatus(PROGRAM_STATUS status)
{
	Prg_StateMachine = status;
}
void Program_Register_Function(struProgram *pro)
{
    if(pro->Programid<PS_TOTALS)
    {
    	pro->Prg_NextStatus = Program_NextStatus;
    	pro->Prg_CurrentStatus = Program_Get_Status;
        ProgramArray[pro->Programid] = pro;
    }
}

void Program_Add_Program(void)
{
	if(ProgramidArrayIndex < ProgramLibConfigPara->ProgramTotalNum-1)
        ProgramidArrayIndex++;
    else
        ProgramidArrayIndex=0;
    
	proCurrentIndex=ProgramLibConfigPara->Pro_Config[ProgramidArrayIndex].ProgramFirstID;
	ProgramArray[proCurrentIndex]->Prg_Initial();  
    
	ProfileIndex=DefaultDotmatrix;
	ProgramSubidArrayIndex=0;
}
void Program_Dec_Program(void)
{
	if(ProgramidArrayIndex > 0)
		ProgramidArrayIndex--;
    else
        ProgramidArrayIndex=ProgramLibConfigPara->ProgramTotalNum-1;
	proCurrentIndex=ProgramLibConfigPara->Pro_Config[ProgramidArrayIndex].ProgramFirstID;
	ProgramArray[proCurrentIndex]->Prg_Initial();  
	ProfileIndex=DefaultDotmatrix;
	ProgramSubidArrayIndex=0;
	
}
void Program_Add_SubProgram(void)
{
	if(ProgramSubidArrayIndex!=ProgramLibConfigPara->Pro_Config[ProgramidArrayIndex].ProgramSubidNum-1)
        ProgramSubidArrayIndex++;
    else
        ProgramSubidArrayIndex=0;
    
	proCurrentIndex=ProgramLibConfigPara->Pro_Config[ProgramidArrayIndex].ProgramSubID[ProgramSubidArrayIndex];
	ProgramArray[proCurrentIndex]->Prg_Initial();  
    
	ProfileIndex=DefaultDotmatrix;
	
}
void Program_Dec_SubProgram(void)
{
	if(ProgramSubidArrayIndex!=0)
		ProgramSubidArrayIndex--;
    else
        ProgramSubidArrayIndex=ProgramLibConfigPara->Pro_Config[ProgramidArrayIndex].ProgramSubidNum-1;
	proCurrentIndex=ProgramLibConfigPara->Pro_Config[ProgramidArrayIndex].ProgramSubID[ProgramSubidArrayIndex];
	ProgramArray[proCurrentIndex]->Prg_Initial();  
	ProfileIndex=DefaultDotmatrix;
	
}
void Program_Set_RPM(UINT8 by_Dat)
{
    Data_Set_AutoRPM(by_Dat);
}
/***************************************************************************//**
* @brief
*注册中断服务  
* @param[in]:none
* @param[out]:none
******************************************************************************/

void Register_Program_ISR(void)
{
    Hal_Regist_ISR(MS_ISR,DATA_ISR_IDX,Data_1ms_ISR);
    Hal_Regist_ISR(MS_ISR,PRG_ISR_IDX,Program_1ms_ISR);
}

/***************************************************************************//**
* @brief
*  初始化程式
* @param[in]
*   array[]指向初始化结构体；
*   size是TABLE表个数
*  DotMatrix_length:点阵列数
* @param[out]：none
******************************************************************************/
void Program_Initial_Data(ProgramLibPara *ProgramLib)
{
    ProgramLibConfigPara=ProgramLib;
    Program_Initial_Inner_Data();
}



/***************************************************************************//**
* @brief
*  初始化EP或BIKE的瓦特表，仅用于EP或BIKE
* @param[in]
*   Watts_Para：初始化瓦特表结构体；
* @param[out]：none
******************************************************************************/
void Program_Load_WattsTable(const WattsParameterStruct *Watts_Para)
{
    Watts_ParameterData=Watts_Para;
}

/***************************************************************************//**
* @brief
* 此函数用于选择对应的程式
* @param[in]
*   prg_Type：程式名的枚举变量；
* @param[out]：none
******************************************************************************/
void Program_Select_ID(PROGRAM_ID prg_Type)
{
    UINT8 i;
//    if(Bluetooth_Get_ConnectStatus()&& Program_Get_Status()!=PS_PAUSED)
//    {
//        Program_Initial_Inner_Data();
//    }
    proCurrentIndex = prg_Type;
    for(i=0;i<ProgramLibConfigPara->ProgramTotalNum/*-1*/;i++)
  	{
		if(ProgramLibConfigPara->Pro_Config[i].ProgramFirstID==proCurrentIndex)
		{
			ProgramidArrayIndex=i;
			ProgramArray[proCurrentIndex]->Prg_Initial();  
			break;
		}
  	}
	ProfileIndex=DefaultDotmatrix;
	ProgramSubidArrayIndex=0;
    
}
/***************************************************************************//**
* @brief
* 此函数用于取得当前的程式
* @param[in]
*   none
* @param[out]：
*返回程式的ID号
******************************************************************************/
PROGRAM_ID Program_Get_ProgramID(void)
{
    if(ProgramLibConfigPara->Pro_Config[ProgramidArrayIndex].ProgramSubID!=NULL)
        return (PROGRAM_ID)ProgramLibConfigPara->Pro_Config[ProgramidArrayIndex].ProgramSubID[ProgramSubidArrayIndex];
    else
        return (PROGRAM_ID)ProgramLibConfigPara->Pro_Config[ProgramidArrayIndex].ProgramFirstID;   
	//return (PROGRAM_ID)ProgramArray[proCurrentIndex]->Programid;
}
/***************************************************************************//**
* @brief
* 此函数用于取得当前的程式
* @param[in]
*   none
* @param[out]：
*返回程式的当前状态
******************************************************************************/
PROGRAM_STATUS Program_Get_Status(void)
{
    return Prg_StateMachine;
}
/***************************************************************************//**
* @brief
* 此函数用于设置程序状态
* @param[in]
*   prgstatus：状态设置变量
* @param[out]：none
******************************************************************************/
void Program_Set_Status(PROGRAM_STATUS prgstatus)
{
    Prg_StateMachine=prgstatus;
}
/***************************************************************************//**
* @brief
* 此函数用于设置虚拟按键
* @param[in]
*   vrKey：程式虚拟按键枚举变量
* @param[out]：
*返回是否执行OK
*true表示成功；false表示失败
******************************************************************************/
bool Program_Set_VirtualKey(UINT16 vrKey)
{
  bool Result=false;
  UINT16 _stage = vrKey & 0xFF00 ;
  UINT8 TempKey = (UINT8)(vrKey&0x00FF);
  
  if(_stage == FRAME_STAGE_USER)
  { 
    if(TempKey == VK_PAUSE_START || TempKey == VK_START)
    {
      _stage= FRAME_STAGE_PRGRUN;
    }
  }
  if(TempKey == VK_SAFEKEYON || TempKey == VK_SAFEKEYOFF || TempKey == VK_INITIAL || TempKey == VK_DEMO_ERP_WAKEUP)
  {
    if(VK_SAFEKEYOFF==TempKey)
    {
      if(_stage ==FRAME_STAGE_PRGRUN) 
      {
        Data_Set_Event(DATA_SAVEDATA_EVENT);
      }
    }
    Program_Initial_Inner_Data();
  }
  if(((_stage != FRAME_STAGE_PRGSETUP)&&(_stage != FRAME_STAGE_PRGRUN))
     ||TempKey == VK_NULL || Program_Get_Status()==PS_WAIT_3S||Program_Get_Status()==PS_RESTART_321)  
  {
    return Result;
  }
  //  if(wlt2564_Get_ConnectStatus())
  //  {
  //      if(TempKey == VK_UPDATE)
  //      {
  //          Program_NextStatus(PS_SELECTPRG);
  //          PrgKeyEvts.Bit_Flag.key_Updata = 1;
  //      }
  //      return Result;
  //  }
  switch(TempKey)
  {
  case VK_UPDATE: 
    Program_NextStatus(PS_SELECTPRG);
    PrgKeyEvts.Bit_Flag.key_update = 1;   
    Result=true;
    break;
  case VK_RESET:
    PrgKeyEvts.Bit_Flag.Key_Reset=true;
    Result=true;
    break;
  case VK_INITIAL: 
    PrgKeyEvts.Bit_Flag.Key_Initial=true;
    Result=true;
    break;
  case VK_ENTER: 
  case VK_ENTER_CHANGEDIS:
    if(Bluetooth_Get_ConnectStatus() == 0)
    {
      PrgKeyEvts.Bit_Flag.Key_Enter=true;
      PrgActionFlag.ScanMode=false;
      Prg_isr_TimerColse(TIMER_ScanFunction);
    }
    break;
  case VK_BACK: 
    if(NumberKey_Data.Events.Bits_All!=0)
    {
      memset(&NumberKey_Data,0,sizeof(NumberKeyData));
    }
    else
    {
      PrgKeyEvts.Bit_Flag.Key_Back=true;
    }          
    break;
  default:break;
  }
  switch(Program_Get_Status())
  {
  case PS_IDLE:
  case PS_SELECTPRG:
  case PS_SETUP:
    {
      switch(TempKey)
      {
      case VK_START:
      case VK_PAUSE_START:
        if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
        {
          if(Data_Get_RPM(MOTOR_RPM))
          {
            Result = true;
            break;
          }
        }
        while(Beep_Get_OK() == false);
        ProgramArray[proCurrentIndex]->BeginWorkout();

        LCB_Send_Cmd(DS_INC_LOCATION,Data_Get_Incline(TARGET_INCLINE_ADC));

#ifdef AE        
//          Incline_Set_Incline(Data_Get_Incline(TARGET_INCLINEX10));// INCLINE FIRST
            Incline_Set_Zero( );
//          Incline_Updata_Incline();
#endif
#ifdef TM
        LCB_Send_Cmd(DS_ECB_ZERO,0);
#else
        ECB_Set_Zero();
////        ECB_Data_Updata();
#endif
        SpeedDataBackup=Data_Get_Speed(TARGET_SPEEDX1000);
        InclineDataBackup=Data_Get_Incline(TARGET_INCLINEX10);
        ResistanceDataBackup=Data_Get_Resistance(TARGET_RESISTANCE);
        Program_NextStatus(PS_WAIT_3S);
        //                Result=true;
        if(Bluetooth_Get_ConnectStatus() == 0)
        {
          UINT8 UserID = User_Get_UserSlot();
          USERS_INFO userinfo = SystemConfig_Get_UserInfo(UserID);
          UINT8 j = userinfo.WorkoutSummaryDataCount;
          userinfo.WorkoutSummaryData[j].workoutunits = SystemConfig_Get_Display_Unit(UserID);//SystemConfig_Get_Char(UNIT_FLAG_UCHAR);
          DateTime_T Rtc_Time;
          RTC_Read_Time(&Rtc_Time);
          userinfo.WorkoutSummaryData[j].workoutstartyear = Rtc_Time.year + 2000;
          userinfo.WorkoutSummaryData[j].workoutstartmonth = Rtc_Time.month;
          userinfo.WorkoutSummaryData[j].workoutstartday = Rtc_Time.date;
          userinfo.WorkoutSummaryData[j].workoutstarthour = Rtc_Time.hours;
          userinfo.WorkoutSummaryData[j].workoutstartminute = Rtc_Time.minutes;
          userinfo.WorkoutSummaryData[j].workoutstartsecond = Rtc_Time.seconds;
          if(SystemConfig_Set_UserInfo(userinfo,UserID))
          {
            SystemConfigProcess(SVK_SAVE_USER2FLASH, &UserID, sizeof(UserID));
          }
        }
        break;
      case VK_SPEEDUP_LONG_PRESS:
        if(0 == Bluetooth_Get_ConnectStatus())
          PrgKeyEvts.Bit_Flag.Key_Up=true;
        break;
      case VK_UP:
      case VK_INCLINEUP:
      case VK_SPEEDUP:
      case VK_RESISTANCEUP:
        //              if(Bluetooth_Get_ConnectStatus() == 0)
        PrgKeyEvts.Bit_Flag.Key_Up=true;
        break;
      case VK_DOWN:
      case VK_INCLINEDOWN:
      case VK_SPEEDDOWN:
      case VK_RESISTANCEDOWN:
        //              if(Bluetooth_Get_ConnectStatus() == 0)
        PrgKeyEvts.Bit_Flag.Key_Dn=true;
        break;
      case VK_ENTER_CHANGEDIS:
        PrgKeyEvts.Bit_Flag.Key_Enter=true;
        break;
      case VR_MANUAL:
        Program_Initial_Inner_Data();
        PrgKeyEvts.Bit_Flag.Key_Manual = true;
        Program_NextStatus(PS_SELECTPRG);            
        break;
      case VR_WEIGHT_LOSS:
        Program_Initial_Inner_Data();
        PrgKeyEvts.Bit_Flag.Key_WeightLoss = true;
        Program_NextStatus(PS_SELECTPRG);             
        break;
      case VR_INTERVALS:
        Program_Initial_Inner_Data();
        PrgKeyEvts.Bit_Flag.Key_Intervals = true;
        Program_NextStatus(PS_SELECTPRG);         
        break;
      case VR_DISTANCE:
        Program_Initial_Inner_Data();
        PrgKeyEvts.Bit_Flag.Key_Distance = true;
        Program_NextStatus(PS_SELECTPRG);
        break;
      case VR_CALORIES:
        Program_Initial_Inner_Data();
        PrgKeyEvts.Bit_Flag.Key_Calories = true;
        Program_NextStatus(PS_SELECTPRG);
        break;
      case VK_NUM0:
      case VK_NUM1:
      case VK_NUM2:
      case VK_NUM3:
      case VK_NUM4:
      case VK_NUM5:
      case VK_NUM6:
      case VK_NUM7:
      case VK_NUM8:
      case VK_NUM9:
        {
          PrgKeyEvts.Bit_Flag.Key_NumberKey=true;
          NumberKey_Data.InputValue = TempKey - VK_NUM0;
          NumberKey_Data.Timer_Counter = 0;
          switch(ProgramArray[proCurrentIndex]->Setting_Array[ProgramArray[proCurrentIndex]->SettingArray_Index])
          {
          case SET_TIME:
            NumberKey_Data.Events.Bit.Time=1;
            break;
          case SET_SPEED:
            NumberKey_Data.Events.Bit.Spd=1;
            break;
          case SET_INCLINE:
            NumberKey_Data.Events.Bit.Inl=1;
            break;
          case SET_RESISTANCE:
            NumberKey_Data.Events.Bit.Res=1;
            break;
          case SET_LEVEL:
            NumberKey_Data.Events.Bit.Level=1;
            break;
          case SET_WATTS:
            NumberKey_Data.Events.Bit.Watt=1;
            break;
          }	
        }
        break;
      default:
        switch(ProgramArray[proCurrentIndex]->Setting_Array[ProgramArray[proCurrentIndex]->SettingArray_Index])
        {
        case SET_WALK_SPEED:
        case SET_JOG_SPEED:
        case SET_SPEED:
          if(TempKey>=VK_QK_SPEED1 && TempKey<=VK_QK_SPEED20)
          {
            PrgKeyEvts.Bit_Flag.Key_QuickKey=true;
            TempKey=(TempKey-VK_QK_SPEED1+1)*10;
            Data_Set_Speed_Base_User(TempKey,0);
            //Data_Set_Speed(TempKey,SystemConfig_Get_Display_Unit(User_Get_UserSlot( ))==METRIC,0);
          }
          break;
        case SET_INCLINE:
#ifdef AE
          if(TempKey>=VK_QK_INCLINE_0 && TempKey<=VK_QK_INCLINE_200)
          {
            PrgKeyEvts.Bit_Flag.Key_QuickKey=true;
            TempKey=(TempKey-VK_QK_INCLINE_0)*5;
            Data_Set_Incline(TempKey,0);
          }
#else
          if(TempKey>=VK_QK_INCLINE_0 && TempKey<=VK_QK_INCLINE_150)
          {
            PrgKeyEvts.Bit_Flag.Key_QuickKey=true;
            TempKey=(TempKey-VK_QK_INCLINE_0)*5;
            Data_Set_Incline(TempKey,0);
          }
#endif
          break;
        case SET_RESISTANCE:
          if(TempKey>=VK_QK_RESISTACE_1 && TempKey<=VK_QK_RESISTACE_30)
          {
            PrgKeyEvts.Bit_Flag.Key_QuickKey=true;
            TempKey=TempKey-VK_QK_RESISTACE_1+1;
            Data_Set_Resistance(TempKey,0);
          }
          break;
        }	
        break;
      }
    }
    break;
  case PS_WAIT_3S:
    break;
  case PS_PAUSED:
    switch(TempKey)
    {
    case VK_START:
    case VK_PAUSE_START:
      if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
      {
        if(Data_Get_RPM(MOTOR_RPM))
        {
          Result=true;
          break;
        }
      }
      
	  LCB_Send_Cmd(DS_INC_LOCATION,Data_Get_Incline(TARGET_INCLINE_ADC));
#ifdef AE
          Incline_Set_Incline(Data_Get_Incline(TARGET_INCLINEX10));
//           Incline_Updata_Incline();
//          Incline_Proce_AE();
#endif
#ifdef TM	    
      LCB_Send_Cmd(DS_ECB_LOCATION,Data_Get_Resistance(TARGET_ECBCOUNT));
#else			
      ECB_Set_Resistance(Data_Get_Resistance(TARGET_RESISTANCE)); 
////      ECB_Data_Updata();
#endif			
      LCB_Send_Cmd(DS_INDUCTION_PWM,Data_Get_Resistance(TARGET_PWM));
      Program_NextStatus(PS_RESTART_321);
      //            Result=true;
      break;
    case VK_CHANGE_DISPLAY: 
      PrgKeyEvts.Bit_Flag.Key_ChangeDisplay=true;
      Result=true;
      break;
    default:
      break;
    }
    break;
  case PS_COMPLETED:
    switch(TempKey)
    {
    case VK_CHANGE_DISPLAY: 
      PrgKeyEvts.Bit_Flag.Key_ChangeDisplay=true;
      Result=true;
      break;
    case VK_ENTER_CHANGEDIS:
      if(NumberKey_Data.Events.Bit.Spd || NumberKey_Data.Events.Bit.Res)
      {
        PrgKeyEvts.Bit_Flag.Key_Enter=true;
      }
      else
      {
        PrgKeyEvts.Bit_Flag.Key_ChangeDisplay=true;
      }
      break;
    default:
      break;
    }
    break;
  case PS_WARMUP:
  case PS_RUN:
  case PS_COOLDOWN:
    switch(TempKey)
    {
    case VK_SPEEDUP_LONG_PRESS:
      if(SystemConfig_Get_Display_Unit(User_Get_UserSlot( ))==METRIC)
      {
        if(PrgSpecialFunctionData.LessOneMileCounter<15)
        {
          PrgSpecialFunctionData.LessOneMileCounter++;
          if(Program_Get_ProgramID() != PS_CUSTOM_TIME && Program_Get_ProgramID() != PS_CUSTOM_DISTANCE && Program_Get_ProgramID() != PS_MYFIRST5K)
            Data_Set_Speed_Base_User(Data_Get_Speed(TARGET_SPEEDX10_KPH)+1,1);
          else
            Data_Set_Speed_Base_User(Data_Get_Speed(TARGET_SPEEDX10_KPH)+1,0);
          ProgramArray[proCurrentIndex]->Prg_RunData(INDEX_SPEED);
          //Data_Set_Speed(Data_Get_Speed(TARGET_SPEEDX10_KPH)+1,DATA_METRIC,1);
        }
      }
      else
      {
        if(PrgSpecialFunctionData.LessOneMileCounter<9)
        {
          PrgSpecialFunctionData.LessOneMileCounter++;
          if(Program_Get_ProgramID() != PS_CUSTOM_TIME && Program_Get_ProgramID() != PS_CUSTOM_DISTANCE && Program_Get_ProgramID() != PS_MYFIRST5K)
            Data_Set_Speed_Base_User(Data_Get_Speed(TARGET_SPEEDX10_MPH)+1,1);
          else
            Data_Set_Speed_Base_User(Data_Get_Speed(TARGET_SPEEDX10_MPH)+1,0);
          ProgramArray[proCurrentIndex]->Prg_RunData(INDEX_SPEED);
          //  Data_Set_Speed(Data_Get_Speed(TARGET_SPEEDX10_MPH)+1,STANDARD,1);
        }
      }
      break;
    case VK_SPEEDUP_RElEASE:
      PrgSpecialFunctionData.LessOneMileCounter=0;
      break;      
    case VK_INCLINEUP:
      PrgKeyEvts.Bit_Flag.Key_InclineUp=true;
      ProgramArray[proCurrentIndex]->Prg_RunData(INDEX_INCLINE);
      break;
    case VK_INCLINEDOWN:
      PrgKeyEvts.Bit_Flag.Key_InclineDn=true;
      ProgramArray[proCurrentIndex]->Prg_RunData(INDEX_INCLINE);
      break;
    case VK_SPEEDUP:
      PrgKeyEvts.Bit_Flag.Key_SpeedUp=true;
      ProgramArray[proCurrentIndex]->Prg_RunData(INDEX_SPEED);
      break;
    case VK_SPEEDDOWN:
      PrgKeyEvts.Bit_Flag.Key_SpeedDn=true;
      ProgramArray[proCurrentIndex]->Prg_RunData(INDEX_SPEED);
      break;
    case VK_RESISTANCEUP:
      PrgKeyEvts.Bit_Flag.Key_ResistanceUp=true;
      ProgramArray[proCurrentIndex]->Prg_RunData(INDEX_RESISTANCE);
      break;
    case VK_RESISTANCEDOWN:
      PrgKeyEvts.Bit_Flag.Key_ResistanceDn=true;
      ProgramArray[proCurrentIndex]->Prg_RunData(INDEX_RESISTANCE);
      break;
    case VK_PAUSE_START:
    case VK_PAUSE: 
      Prg_isr_TimerCounterClear(TIMER_5S_PedalStart);
      PrgKeyEvts.Bit_Flag.Key_Pause=true;
      Result=true;
      break;
    case VK_COOLDOWN: 
      Program_NextStatus(PS_COOLDOWN);
      PrgKeyEvts.Bit_Flag.Key_CoolDn=true;
      Result=true;
      break;
    case VK_CHANGE_DISPLAY: 
      PrgKeyEvts.Bit_Flag.Key_ChangeDisplay=true;
      Result=true;
    case VK_ENTER_CHANGEDIS:
      if(NumberKey_Data.Events.Bit.Spd || NumberKey_Data.Events.Bit.Res)
      {
        PrgKeyEvts.Bit_Flag.Key_Enter=true;
      }
      else
      {
        PrgKeyEvts.Bit_Flag.Key_ChangeDisplay=true;
      }
      break;
      break;
    case VK_SCAN: 
      PrgActionFlag.ScanMode=true;
      Prg_isr_TimerCounterClear(TIMER_ScanFunction);	
      Result=true;
      break;
    case VK_NUM0:
    case VK_NUM1:
    case VK_NUM2:
    case VK_NUM3:
    case VK_NUM4:
    case VK_NUM5:
    case VK_NUM6:
    case VK_NUM7:
    case VK_NUM8:
    case VK_NUM9:
      PrgKeyEvts.Bit_Flag.Key_NumberKey=true;
      NumberKey_Data.InputValue = TempKey - VK_NUM0;
      if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
      {
        NumberKey_Data.Events.Bit.Spd=1;
      }
      else
      {
        NumberKey_Data.Events.Bit.Res=1;
      }
      NumberKey_Data.Timer_Counter = 0 ;
      break;
    default:
      if(TempKey>=VK_QK_SPEED1 && TempKey<=VK_QK_SPEED20)
      {
        PrgKeyEvts.Bit_Flag.Key_SPDQuickKey=true;
        TempKey=(TempKey-VK_QK_SPEED1+1)*10;
        if(Program_Get_ProgramID() != PS_CUSTOM_TIME && Program_Get_ProgramID() != PS_CUSTOM_DISTANCE && Program_Get_ProgramID() != PS_MYFIRST5K)
          Data_Set_Speed_Base_User(TempKey,1);
        else
          Data_Set_Speed_Base_User(TempKey,0);
      }
      else if(TempKey>=VK_QK_RESISTACE_1 && TempKey<=VK_QK_RESISTACE_30)
      {
        PrgKeyEvts.Bit_Flag.Key_RESQuickKey=true;
        TempKey=TempKey-VK_QK_RESISTACE_1+1;
        if(Program_Get_ProgramID() != PS_CUSTOM_TIME && Program_Get_ProgramID() != PS_CUSTOM_DISTANCE && Program_Get_ProgramID() != PS_MYFIRST5K)
          Data_Set_Resistance(TempKey,1);
        else
          Data_Set_Resistance(TempKey,0);
      }
#ifdef TM
      else if(TempKey>=VK_QK_INCLINE_0 && TempKey<=VK_QK_INCLINE_150)
      {
        PrgKeyEvts.Bit_Flag.Key_INCQuickKey=true;
        TempKey=(TempKey-VK_QK_INCLINE_0)*5;
        if(Program_Get_ProgramID() != PS_CUSTOM_TIME && Program_Get_ProgramID() != PS_CUSTOM_DISTANCE && Program_Get_ProgramID() != PS_MYFIRST5K)
          Data_Set_Incline(TempKey,1);
        else
          Data_Set_Incline(TempKey,0);
      }
#else      
       else if(TempKey>=VK_QK_INCLINE_0 && TempKey<=VK_QK_INCLINE_200)
      {
        PrgKeyEvts.Bit_Flag.Key_INCQuickKey=true;
        TempKey=(TempKey-VK_QK_INCLINE_0)*5;
        if(Program_Get_ProgramID() != PS_CUSTOM_TIME && Program_Get_ProgramID() != PS_CUSTOM_DISTANCE && Program_Get_ProgramID() != PS_MYFIRST5K)
          Data_Set_Incline(TempKey,1);
        else
          Data_Set_Incline(TempKey,0);
      }
#endif
      break;
    }
    break;
  }	
  return Result;
}
/***************************************************************************//**
* @brief
* 此函数用于设置参数
* @param[in]：
vrKey：参数索引
*p：设置的值
* @param[out]：none
******************************************************************************/

void Program_Set_Parameter(UINT16 vrKey,void *p)
{
    UINT16 ret=0;
    switch(vrKey)
    {
        case VK_SET_WORKOUT_TIME:
        {
            memcpy((void*)&(ret),p,sizeof(UINT16));
			//ret = *((UINT16*))p;
          //  UINT16 ElapsedTime = Data_Get_Time(ELAPSED_TIME);
            if(Program_Get_ProgramID() == PS_MANUAL)
            {
                extern UINT8 appStart;    //zsf
                if(appStart == 0)    //zsf
                {
                    if(ret>0&&ret<5*60)
                        ret=0;
                }
                Data_Set_Time(ret);
            }
            else if(Program_Get_ProgramID() == PS_THR_ZONE)
            {
                ProHrc_Set_TargetTime(ret);
            }
            else if(Program_Get_ProgramID() == PS_HILL_CLIMB||Program_Get_ProgramID() == PS_FATBURN)
            {
                Pro_table_Set_Data(ret);
            }
        }
        break;
        case VK_SET_WORKOUT_DISTANCE:
        {
          //  UINT16 ElapsedDistance = Data_Get_Distance(ELAPSED_DISTANCE);
            memcpy((void*)&(ret),p,sizeof(UINT16));
            if(Program_Get_ProgramID() == PS_DISTANCE_GOAL)
            {
                Pro_table_Set_Data(ret);
            }
        }
        break;
        case VK_SET_WORKOUT_CALORIES:
        {
            //UINT16 ElapsedCalories = Data_Get_Calories(ELAPSED_CALORIES);
            memcpy((void*)&(ret),p,sizeof(UINT16));
            if(Program_Get_ProgramID() == PS_CALORIES_GOAL)
            {
                Pro_table_Set_Data(ret);
            }
        }
        break;
        case VK_SET_WORKOUT_TARGETHR:
        {
            memcpy((void*)&(ret),p,sizeof(UINT16));
            if(Program_Get_ProgramID() == PS_THR_ZONE)
            {
                ProHrc_Set_TargetHR(ret);
            }
        }
        break;
        case VK_LONG_SET_SPEED:
        {
            memcpy((void*)&(ret),p,sizeof(UINT16));
            if(Program_Get_ProgramID() != PS_CUSTOM_TIME && Program_Get_ProgramID() != PS_CUSTOM_DISTANCE && Program_Get_ProgramID() != PS_MYFIRST5K)
                Data_Set_Speed_Base_User(ret,1);
            else
                Data_Set_Speed_Base_User(ret,0);
            ProgramArray[proCurrentIndex]->Prg_RunData(INDEX_SPEED);
        }
        break;
        case VK_SET_SPEED:
        {
            memcpy((void*)&(ret),p,sizeof(UINT16));
            if (Program_Get_Speed(DATA_AUTO,0) != ret)
            {           
                if(Prg_StateMachine != PS_SELECTPRG)
                {
                    if(Program_Get_ProgramID() != PS_CUSTOM_TIME && Program_Get_ProgramID() != PS_CUSTOM_DISTANCE && Program_Get_ProgramID() != PS_MYFIRST5K)
                        Data_Set_Speed_Base_User(ret,1);
                    else
                        Data_Set_Speed_Base_User(ret,0);
                    ProgramArray[proCurrentIndex]->Prg_RunData(INDEX_SPEED);
                }
                else
                {
	            Data_Set_Speed_Base_User(ret,0);
                }
                Beep_Set_Beeps(1,3,1);
            }
        }
        break;
        case VK_SET_MYFIRST5K_SPEED:
        {
            memcpy((void*)&(ret),p,sizeof(UINT16));
            ProMyFirst5k_Set_Speeed(ret);
        }
        break;
        case VK_LONG_SET_INCLINE:
        {
            memcpy((void*)&(ret),p,sizeof(UINT16));
            if(Program_Get_ProgramID() != PS_CUSTOM_TIME && Program_Get_ProgramID() != PS_CUSTOM_DISTANCE && Program_Get_ProgramID() != PS_MYFIRST5K)
                Data_Set_Incline(ret,1);
            else
                Data_Set_Incline(ret,0);
            ProgramArray[proCurrentIndex]->Prg_RunData(INDEX_INCLINE);
        }
        break;
        case VK_SET_INCLINE:
        {
            memcpy((void*)&(ret),p,sizeof(UINT16));
            if (Program_Get_Incline(0) != ret)
            { 
                if(Prg_StateMachine != PS_SELECTPRG)
                {
                    if(Program_Get_ProgramID() != PS_CUSTOM_TIME && Program_Get_ProgramID() != PS_CUSTOM_DISTANCE && Program_Get_ProgramID() != PS_MYFIRST5K)
                        Data_Set_Incline(ret,1);
                    else
                        Data_Set_Incline(ret,0);
                    ProgramArray[proCurrentIndex]->Prg_RunData(INDEX_INCLINE);
                }
                else
                    Data_Set_Incline(ret,0);
                Beep_Set_Beeps(1,3,1);
            }
        }
        break;	
        case VK_LONG_SET_RESISTANCE:
        {
            memcpy((void*)&(ret),p,sizeof(UINT16));
            if(Program_Get_ProgramID() != PS_CUSTOM_TIME && Program_Get_ProgramID() != PS_CUSTOM_DISTANCE)
                Data_Set_Resistance(ret,1);
            else
                Data_Set_Resistance(ret,0);
            ProgramArray[proCurrentIndex]->Prg_RunData(INDEX_RESISTANCE);
        }
        case VK_SET_RESISTANCE:
        {
            memcpy((void*)&(ret),p,sizeof(UINT16));
            if (Program_Get_Resistance(0) != ret)
            {
                if(Prg_StateMachine != PS_SELECTPRG)
                {
                    if(Program_Get_ProgramID() != PS_CUSTOM_TIME && Program_Get_ProgramID() != PS_CUSTOM_DISTANCE)
                        Data_Set_Resistance(ret,1);
                    else
                        Data_Set_Resistance(ret,0);
                    ProgramArray[proCurrentIndex]->Prg_RunData(INDEX_RESISTANCE);
                }
                else
                    Data_Set_Resistance(ret,0);
                Beep_Set_Beeps(1,3,1);
            }
        }
        break;
        case VK_SET_WEIGHT:
        {
            memcpy((void*)&(ret),p,sizeof(UINT16));
            Data_Set_Weight(ret);      
        }
        break;	
        case VK_SET_WATTS:
        {
            memcpy((void*)&(ret),p,sizeof(UINT16));
            ret=(ret*2+5)/10*5;//covert to 25,30,...,400
            
            Data_Set_Watt(ret);      
        }
        break;    
        case VK_SET_LEVEL:
        {
            memcpy((void*)&(ret),p,sizeof(UINT16));
            Data_Set_Level(ret);      
        }
        break;    
        default:break;
    }  
}

/***************************************************************************//**
* @brief
* 此函数用于取得时间
* @param[in]：
* @param[out]：返回时间
******************************************************************************/
UINT16 Program_Get_Time(UINT8 Data_Mode)
{
    UINT16 w_Dat=0;
	if(NumberKey_Data.Events.Bit.Time==1 && Data_Mode==QUICKKEY_NORMAL)
	{
		w_Dat=NumberKey_Data.ShowValue*60;
	}
	else
	{
		if(Program_Get_Status()<=PS_SETUP)
		{
			w_Dat=ProgramArray[proCurrentIndex]->Prg_SetupData(INDEX_TIME);
		}
		else if(Program_Get_Status() == PS_COMPLETED)
		{
			w_Dat=Data_Get_Time(ELAPSED_TIME);
		}
		else
		{
			w_Dat=ProgramArray[proCurrentIndex]->Prg_RunData(INDEX_TIME);
		}
	}
    return w_Dat;
}
/***************************************************************************//**
* @brief
* 此函数用于取得目标时间
* @param[in]
*   none
* @param[out]：
*返回目标时间
******************************************************************************/

UINT16 Program_Get_WorkoutTime(void)
{
    return Data_Get_Time(TARGET_TIME);
}
/***************************************************************************//**
* @brief
* 此函数用于取得当前时间
* @param[in]
*   none
* @param[out]：
*返回当前时间
******************************************************************************/
UINT16 Program_Get_WorkoutTime_Elapsed(void)
{
    return Data_Get_Time(ELAPSED_TIME);
}
/***************************************************************************//**
* @brief
* 此函数用于取得剩余时间
* @param[in]
*   none
* @param[out]：
*返回剩余时间
******************************************************************************/
UINT16 Program_Get_WorkoutTime_Remaining(void)
{
    return Data_Get_Time(REMAINING_TIME);
}
/***************************************************************************//**
* @brief
* 此函数用于取得热身运动时间
* @param[in]
*   none
* @param[out]：
*返回热身运动时间
******************************************************************************/

UINT16 Program_Get_WorkoutTime_Warmup(void)
{
    return Data_Get_Time(WARMUP_TIME);
}
/***************************************************************************//**
* @brief
* 此函数用于取得缓和运动时间
* @param[in]
*   none
* @param[out]：
*返回缓和运动时间
******************************************************************************/

UINT16 Program_Get_WorkoutTime_Cooldown(void)
{
    return Data_Get_Time(COOLDOWN_TIME);   
}
/***************************************************************************//**
* @brief
* 此函数用于取得当前运动的段数
* @param[in]
*   none
* @param[out]：
*返回当前运动的段数
******************************************************************************/

UINT8 Program_Get_Stage(void)
{
    return Data_Get_Time(STAGE);   
}
/***************************************************************************//**
* @brief
* 此函数用于取得当前段当前运动时间
* @param[in]
*   none
* @param[out]：
*返回当前段当前运动时间
******************************************************************************/

UINT16 Program_Get_timeInStage(void)
{
    return Data_Get_Time(TIME_IN_STAGE);   
}
/***************************************************************************//**
* @brief
* 此函数用于取得当前段总运动时间
* @param[in]
*   none
* @param[out]：
*返回当前段总运动时间
******************************************************************************/

UINT16 Program_Get_stageTime(void)
{
    return Data_Get_Time(STAGE_TIME);   
}
/***************************************************************************//**
* @brief
* 此函数用于取得速度值
* @param[in]
*   type：	
DATA_STANARD：取得英制速度
DATA_METRIC：取得公制速度
DATA_AUTO：取得当前速度，根据目前系统公英制返回值
*pIsBlinking：暂不用
* @param[out]：
*返回速度值
******************************************************************************/

UINT16 Program_Get_Speed(DATA_TYPE type,UINT8 Data_Mode)
{
    UINT32 ret;
    
    if(NumberKey_Data.Events.Bit.Spd==1 && Data_Mode==QUICKKEY_NORMAL)
    {
        ret=NumberKey_Data.ShowValue; 
    }
    else
    {
		if(Program_Get_Status()<=PS_SETUP)
		{
			ret=ProgramArray[proCurrentIndex]->Prg_SetupData(INDEX_SPEED);
		}
		else
		{
            if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
            {
                ret=Data_Get_Speed(TARGET_SPEEDX1000); 
            }
            else
            {
                ret =Data_Get_Speed(EPBI_SPEEDX1000); 
            }
            if(type==DATA_METRIC)
            {
                ret=ret*201168/125000+2;//convert to Metric
            }
            else if(type==DATA_AUTO)
            {
                if(SystemConfig_Get_Display_Unit(User_Get_UserSlot( ))==METRIC)
                {
                    ret=ret*201168/125000+2;//convert to Metric
                }
            }
			ret = ret/100;
		}
    }
    return (UINT16)(ret);
}
/***************************************************************************//**
* @brief
* 此函数用于取得平均速度值
* @param[in]
*   type：	
DATA_STANARD：取得英制平均速度
DATA_METRIC：取得公制平均速度
DATA_AUTO：取得当前平均速度，根据目前系统公英制返回值
* @param[out]：
*返回平均速度值
******************************************************************************/

UINT16 Program_Get_Speed_Average(DATA_TYPE type)
{
    UINT32 ret;
    ret=Data_Get_Speed(AVERAGE_SPEEDX10);//DATA_AUTO
    
    if(type == DATA_STANARD)
    {
        if(SystemConfig_Get_Display_Unit(User_Get_UserSlot( ))==METRIC)
        {
            ret=(ret*12500000/201168+10)/100;//convert to mile
        }
    }
    return ret;  
}
/***************************************************************************//**
* @brief
* 此函数用于取得升降值
* @param[in]
*pIsBlinking：暂不用
* @param[out]：
*返回升降值
******************************************************************************/

UINT8 Program_Get_Incline(UINT8 Data_Mode)
{
    if(NumberKey_Data.Events.Bit.Inl==1 &&Data_Mode==QUICKKEY_NORMAL)
    {
        return ((UINT8)NumberKey_Data.ShowValue);
    }
    else
    {
        return ((UINT8)Data_Get_Incline(TARGET_INCLINEX10));  
    }  
}
/***************************************************************************//**
* @brief
* 此函数用于取得平均升降值
* @param[in]
*none
* @param[out]：
*返回平均升降值
******************************************************************************/

UINT8 Program_Get_Incline_Average(void)
{
    return ((UINT8)Data_Get_Incline(AVERAGE_INCLINEX10));    
}
/***************************************************************************//**
* @brief
* 此函数用于取得阻力值
* @param[in]
*pIsBlinking：暂不用
* @param[out]：
*返回阻力值
******************************************************************************/

UINT8 Program_Get_Resistance(UINT8 Data_Mode)
{
    if(NumberKey_Data.Events.Bit.Res==1 &&Data_Mode==QUICKKEY_NORMAL)
    {
        return ((UINT8)NumberKey_Data.ShowValue);
    }
    else
    { 
        return ((UINT8)Data_Get_Resistance(TARGET_RESISTANCE));   
    }  
}
/***************************************************************************//**
* @brief
* 此函数用于取得平均阻力值
* @param[in]
* @param[out]：
*返回平均阻力值
******************************************************************************/

UINT8 Program_Get_Resistance_Average(void)
{
    return ((UINT8)Data_Get_Resistance(AVERAGE_RESISTANCE));   
}
/***************************************************************************//**
* @brief
* 此函数用于取得Level值
* @param[in]
*pIsBlinking：暂不用
* @param[out]：
*返回Level值
******************************************************************************/

UINT8 Program_Get_WorkoutLevel(UINT8 Data_Mode)
{
    if(NumberKey_Data.Events.Bit.Level==1 &&Data_Mode==QUICKKEY_NORMAL)
    {
        return ((UINT8)NumberKey_Data.ShowValue);
    }
    else
    {
        return ((UINT8)Data_Get_Level(TARGET_LEVEL));   
    }
}
/***************************************************************************//**
* @brief
* 此函数用于取得当前距离
* @param[in]
*   type：	
DATA_STANARD：取得英制距离
DATA_METRIC：取得公制距离
DATA_AUTO：取得当前距离，根据目前系统公英制返回值
*pIsBlinking：暂不用
* @param[out]：
*返回当前距离值
******************************************************************************/

UINT16 Program_Get_Distance(DATA_TYPE type)
{
    UINT32 ret;
    
    if(Program_Get_Status()<=PS_SETUP)
    {
        ret=ProgramArray[proCurrentIndex]->Prg_SetupData(INDEX_DISTANCE);
    }
    else
        ret=Data_Get_Distance(ELAPSED_DISTANCE);
    if(DATA_METRIC==type)
    {
        ret=(unsigned long long)ret*201168/125000+2;//convert to Metric
    }
    else if( DATA_AUTO == type)
    {
        if( SystemConfig_Get_Display_Unit(User_Get_UserSlot( ))==METRIC )
        {
            ret = (unsigned long long)ret*201168/125000+2;//convert to Metric
        }
    }
    return ret/10;
}
/***************************************************************************//**
* @brief
* 此函数用于取得剩余距离
* @param[in]
*   type：	
DATA_STANARD：取得英制距离
DATA_METRIC：取得公制距离
DATA_AUTO：取得当前距离，根据目前系统公英制返回值
* @param[out]：
*返回剩余距离值
******************************************************************************/

UINT16 Program_Get_Distance_Remaining(DATA_TYPE type)
{
    UINT32 ret=Data_Get_Distance(REMAINING_DISTANCE);
    if(DATA_METRIC==type)
    {
        ret=(unsigned long long)ret*201168/125000;//convert to Metric
    }
    else if( DATA_AUTO == type)
    {
        if( SystemConfig_Get_Display_Unit(User_Get_UserSlot( ))==METRIC )
        {
            ret = (unsigned long long)ret*201168/125000;//convert to Metric
            
        }
    }
    return ret/10;
}

UINT16 Program_Get_Distance_Target(void)
{
    return Data_Get_Distance(TARGET_DISTANCE);
}

/***************************************************************************//**
* @brief
* 此函数用于取得当前卡路里
* @param[in]
*   isFourDigits：
false：返回最大值为999
true：返回最大值为9999
* @param[out]：
*返回当前卡路里
******************************************************************************/

UINT16 Program_Get_Calories(bool isFourDigits)
{
    UINT16 by_Dat; 
    if(Program_Get_Status()<=PS_SETUP)
    {
        by_Dat=ProgramArray[proCurrentIndex]->Prg_SetupData(INDEX_CALORIES);
    }
    else
	    by_Dat=Data_Get_Calories(ELAPSED_CALORIES);
    if(isFourDigits==false)
    {
        if(by_Dat>999)
            by_Dat=999;
    }
    return by_Dat; 
}
UINT16 Program_Get_CaloriesHR(void)
{
    return Data_Get_Calories(CALORIES_PERHOUR); 
}

UINT16 Program_Get_Calories_PerSecond(void)
{
    return Data_Get_Calories(CALORIES_PERSECOND); 
}
/***************************************************************************//**
* @brief
* 此函数用于取得剩余卡路里
* @param[in]
*   isFourDigits：
false：返回最大值为999
true：返回最大值为9999
* @param[out]：
*返回剩余卡路里
******************************************************************************/
UINT16 Program_Get_Calories_Target(void)
{
    return Data_Get_Calories(TARGET_CALORIES); 
}

UINT16 Program_Get_Calories_Remaining(bool isFourDigits)
{
    UINT16 by_Dat; 
    by_Dat=Data_Get_Calories(REMAINING_CALORIES);
    if(isFourDigits==false)
    {
        if(by_Dat>999)
            by_Dat=999;      
    }
    return by_Dat; 
}
/***************************************************************************//**
* @brief
* 此函数用于取得点阵位置
* @param[in]：None
* @param[out]：
*返回点阵当前位置
******************************************************************************/
UINT16 Program_Get_DotmatixPos(void)
{
    return Profile_Pos;    
}
/***************************************************************************//**
* @brief
* 此函数用于取得Pace
* @param[in]：None
* @param[out]：
*返回Pace
******************************************************************************/

UINT16 Program_Get_Pace(void)
{
    UINT16 w_Dat=0;
    if(Program_Get_Status() > PS_SETUP)
        w_Dat=36000/Data_Get_Speed(TARGET_SPEEDX10_AUTO);
    return w_Dat;
}
/***************************************************************************//**
* @brief
* 此函数用于取得Average Pace
* @param[in]：None
* @param[out]：
*返回Average Pace
******************************************************************************/

UINT16 Program_Get_Pace_Average(void)
{
    UINT16 w_Dat=0;
    w_Dat=36000/Program_Get_Speed_Average(DATA_AUTO);
    return w_Dat;
}
/***************************************************************************//**
* @brief
* 此函数用于取得心跳
* @param[in]：
hrTag：用于索引返回目标，当前，平均，还是自动的值
* @param[out]：
*返回心跳值
******************************************************************************/

UINT8 Program_Get_HR(HR_TAG hrTag)
{
    UINT8 by_Dat=0;
    if(hrTag==TARGET_HR)
    {
        by_Dat=Data_Get_Heart(TARGET_HEART);
    }
    else if(hrTag==CURRENT_HR)
    {
        by_Dat=Data_Get_Heart(CURRENT_HEART);
    }
    else if(hrTag==AVERGE_HR)
    {
        by_Dat=Data_Get_Heart(AVERAGE_HEART);
    }
    else if(hrTag==AUTO_HR)
    {
        if(Program_Get_Status()<=PS_SETUP)
        {
            by_Dat=ProgramArray[proCurrentIndex]->Prg_SetupData(INDEX_HEART);
        }
        else
        {
            by_Dat=Data_Get_Heart(CURRENT_HEART);
        }
    }
    return by_Dat;
}
/***************************************************************************//**
* @brief
* 此函数用于取得Watts值
* @param[in]
watts：用于索引目标，当前值
*pIsBlinking：暂不用
* @param[out]：
*返回Watts值
******************************************************************************/

UINT16 Program_Get_Watts(WATTS_TAG watts, UINT8 Data_Mode)
{
    UINT16 w_Dat=0;
    if(NumberKey_Data.Events.Bit.Watt==1 && Data_Mode==QUICKKEY_NORMAL)
    {
        return NumberKey_Data.ShowValue;
    }
    else
    {
        if(TARGET_WATTS==watts)
        {
            w_Dat=Data_Get_Watts(TARGET_WATT);
        }
        else if(CURRENT_WATTS==watts)
        {
            w_Dat=Data_Get_Watts(CURRENT_WATT);
        }
        else if(AUTO_WATTS==watts)
        {
            if(Program_Get_Status()<=PS_SETUP)
            {
                w_Dat=Data_Get_Watts(TARGET_WATT);
            }
            else if(Program_Get_Status()==PS_PAUSED)
	      	{
		      	w_Dat=0;
	      	}
            else
            {
			    if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
			    {
			        w_Dat=Data_Get_Watts(CURRENT_WATT);
			    }
			    else
			    {
                    w_Dat=ProgramArray[proCurrentIndex]->Prg_RunData(INDEX_WATT);
                }							
            }
        }
    }
    
    return w_Dat;
}
/***************************************************************************//**
* @brief
* 此函数用于取得METs值
* @param[in]：none
* @param[out]：
*返回METs值
******************************************************************************/
UINT16 Program_Get_METs(void)
{
    if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
    {
        return Data_Get_Mets();
    }
    else
    {
        if(Program_Get_Status()!=PS_PAUSED)
            return Data_Get_Mets();
		else
            return 0;
        
    }
}
/***************************************************************************//**
* @brief
* 此函数用于取得当前Steps值
* @param[in]：none
* @param[out]：
*返回当前Steps值
******************************************************************************/

UINT16 Program_Get_Steps(void)
{
    return Data_Get_Steps(ELAPSED_STEPS);
}
/***************************************************************************//**
* @brief
* 此函数用于取得剩余Steps值
* @param[in]：none
* @param[out]：
*返回剩余Steps值
******************************************************************************/

UINT16 Program_Get_Steps_Remaining(void)
{
    return Data_Get_Steps(REMAINING_STEPS);
}
/***************************************************************************//**
* @brief
* 此函数用于取得RPM值
* @param[in]：none
* @param[out]：
*返回RPM值
******************************************************************************/
UINT16 Program_Get_RPM(void)
{
    UINT16 ret=0;
    if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
    {
        ret=Data_Get_RPM(MOTOR_RPM);
    }
    else
    {
        if(Program_Get_Status()!=PS_PAUSED)
            ret=Data_Get_RPM(EPBI_RPM);
		else
            ret=0;
        
    }
    
    return ret;
}

/***************************************************************************//**
* @brief
* 此函数用于取得VO2Max值
* @param[in]：none
* @param[out]：
*返回VO2Max值
******************************************************************************/

UINT16 Program_Get_VO2Max(void)
{
    return Data_Get_VO2Max();
}

/***************************************************************************//**
* @brief
* 此函数用于取得点阵数据
* @param[in]：none
* @param[out]：
*返回点阵数据
******************************************************************************/

PROFILE* Program_Get_Profile(PROFILE_TAG index)
{
    return ProgramArray[proCurrentIndex]->Prg_ProfileShow(index,&Profile_Pos);
}
/***************************************************************************//**
* @brief
* 此函数用于设置加速倍率
* @param[in]：times：加速倍率
* @param[out]：none
******************************************************************************/

void Program_Set_nXTime(UINT8 times)
{
    Data_Set_Nx(times);
}
/***************************************************************************//**
* @brief
* 此函数用于取得当前体重
* @param[in]
*   type：	
DATA_STANARD：取得英制
DATA_METRIC：取得公制
DATA_AUTO：取得当前，根据目前系统公英制返回值
* @param[out]：
*返回当前体重值
******************************************************************************/
UINT16 Program_Get_Weight(DATA_TYPE type)
{
    UINT32 ret=Data_Get_UserData(USER_WEIGHT);//value is X100
    if(type==DATA_METRIC)
    {
        ret=ret*215/474+10;//convert to kg
    }
    else if(type==DATA_AUTO)
    {
        if(SystemConfig_Get_Display_Unit(User_Get_UserSlot( ))==METRIC)
        {
            ret=ret*215/474+10;//convert to kg
        }
    }
    return ret/100;//Return real value
}
/***************************************************************************//**
* @brief
* 此函数用于取得当前用户
* @param[in]：none
* @param[out]：
*返回当前用户
******************************************************************************/
UINT8 Program_Get_User(void)
{
    return Data_Get_UserData(USER_SLOT);
}
/***************************************************************************//**
* @brief
* 此函数用于取得目标程式的目标值
* @param[in]：none
* @param[out]：
*返回目标程式的目标值
******************************************************************************/

UINT16 Program_Get_Goal(void)
{
    return 0;
}
/***************************************************************************//**
* @brief
* 此函数用于取得目标程式的类型
* @param[in]：none
* @param[out]：
*返回目标程式的类型
******************************************************************************/
workoutGoalTypes Program_Get_Goal_Type(void)
{
    workoutGoalTypes retTypes = GOAL_TYPES_NONE;
    switch(proCurrentIndex)
    {
        case PS_MANUAL:
        {
            retTypes = GOAL_TYPES_TIME;
        }
        break;
        case PS_10K:
        {
            retTypes = GOAL_TYPES_DISTANCE;
        }
        break;
        case PS_CALORIES_GOAL:
        {
            retTypes = GOAL_TYPES_CALORIES;
        }
        break;
    }
    return retTypes;
}
/***************************************************************************//**
* @brief
* 此函数用于取得目标程式的剩余值
* @param[in]：none
* @param[out]：
*返回目标程式的剩余值
******************************************************************************/
UINT16 Program_Get_Goal_Remaining(void)
{
    UINT16 ret = 0;
    
    return ret;
}
/***************************************************************************//**
* @brief
* 此函数用于取得程式的设置流程
* @param[in]：none
* @param[out]：
*返回程式的设置流程枚举
******************************************************************************/

//SETTING_ENUM Program_Get_SettingFlow(void)
//{
//  Setting_Chart=ProgramArray[proCurrentIndex]->Setup(Program_Get_KeyEvent());
//  return Setting_Chart;
//}
/***************************************************************************//**
* @brief
* 此函数用于Main.c调用
* @param[in]：none
* @param[out]：
*返回事件
******************************************************************************/

PROGRAM_PENDING_EVTS Program_Process(void)
{
  PROGRAM_PENDING_EVTS pendingEvt;
  pendingEvt.evt=PROGRAM_NONE_EVENT;
  memset(&pendingEvt.WorkOutEvts,0,sizeof(pendingEvt.WorkOutEvts));
  Data_Process();
  if(SystemConfig_Get_Char(DRIVE_TYPE_UINT8)==BIKE_0C_JIS || 
     SystemConfig_Get_Char(DRIVE_TYPE_UINT8)==INDUCTOR_BRAKE_INCLINE)
  {
    Program_Read_5PointData();	
  }
  if(PrgKeyEvts.Bit_Flag.Key_Initial)
  {
#ifdef  __WIFI_FUNC__
    ChangeWorkoutState(WIFI_WORKOUT_STOP);
#endif
#ifdef TM
    LCB_Send_Cmd(DS_ECB_ZERO,0);
#else
    ECB_Set_Zero();
    ////    ECB_Data_Updata();
#endif
    Program_Initial_Inner_Data();
    Program_NextStatus(PS_SELECTPRG);
    PrgKeyEvts.Bit_Flag.Key_Initial=false;
    pendingEvt.evt = PROGRAM_INITIAL_EVENT;
  }
  
  ////	ProfileIndex
  switch(Program_Get_Status())
  {
  case PS_IDLE:  
    break;
  case PS_SELECTPRG:
    if(PrgKeyEvts.Bit_Flag.Key_Manual)
    {
      nCurrentPage = PAGE_PROGRAM;
      pendingEvt.evt=PROGRAM_INITIAL_EVENT;
    }
    else if(PrgKeyEvts.Bit_Flag.Key_WeightLoss)
    {
      Program_Select_ID(PS_WEIGHT_LOSS);
      nCurrentPage = PAGE_PROGRAM;
      pendingEvt.evt=PROGRAM_CHANGE_EVENT;
    }
    else if(PrgKeyEvts.Bit_Flag.Key_Intervals)
    {
      Program_Select_ID(PS_INTERVALS);
      nCurrentPage = PAGE_PROGRAM;
      pendingEvt.evt=PROGRAM_CHANGE_EVENT;
    }
    else if(PrgKeyEvts.Bit_Flag.Key_Distance)
    {
      Program_Select_ID(PS_DISTANCE_GOAL);
      nCurrentPage = PAGE_PROGRAM;
      pendingEvt.evt=PROGRAM_CHANGE_EVENT;
    }
    else if(PrgKeyEvts.Bit_Flag.Key_Calories)
    {
      Program_Select_ID(PS_CALORIES_GOAL);
      nCurrentPage = PAGE_PROGRAM;
      pendingEvt.evt=PROGRAM_CHANGE_EVENT;
    }
    else if(PrgKeyEvts.Bit_Flag.key_update && nCurrentPage != PAGE_ERP_COUNTDOWN)
    {
      switch(Program_Get_ProgramID())
      {
      case PS_MANUAL:
        nCurrentPage = PAGE_PROGRAM;
        pendingEvt.evt=PROGRAM_INITIAL_EVENT;
        break;
      case PS_HILL_CLIMB:
      case PS_DISTANCE_GOAL:
      case PS_THR_ZONE:
      case PS_CALORIES_GOAL:
      case PS_MYFIRST5K:
      case PS_FATBURN:
      case PS_CUSTOM_HR:
        nCurrentPage = PAGE_PROGRAM;
        pendingEvt.evt=PROGRAM_CHANGE_EVENT;
        break;
      case PS_CUSTOM:
        nCurrentPage = PAGE_PROGRAM;
        Program_Select_ID(PS_CUSTOM_TIME);
        pendingEvt.evt=PROGRAM_CHANGE_EVENT;
        break;
      case PS_CUSTOM_TIME:
      case PS_CUSTOM_DISTANCE:
        nCurrentPage = PAGE_PROGRAM;
        pendingEvt.evt=PROGRAM_CHANGE_EVENT;
        break;
      }
    }
    else if(PrgKeyEvts.Bit_Flag.Key_Up)
    {
      switch(ProgramArray[proCurrentIndex]->Setting_Array[ProgramArray[proCurrentIndex]->SettingArray_Index])
      {
      case SET_PROGRAM:
        Program_Add_Program();
        pendingEvt.evt=PROGRAM_CHANGE_EVENT;
        break;
      case SET_SUBPROGRAM:
        Program_Add_SubProgram();
        pendingEvt.evt=SUBPROGRAM_CHANGE_EVENT;
        break;
      }
      
    }
    else if(PrgKeyEvts.Bit_Flag.Key_Dn)
    {
      switch(ProgramArray[proCurrentIndex]->Setting_Array[ProgramArray[proCurrentIndex]->SettingArray_Index])
      {
      case SET_PROGRAM:
        Program_Dec_Program();
        pendingEvt.evt=PROGRAM_CHANGE_EVENT;
        break;
      case SET_SUBPROGRAM:
        Program_Dec_SubProgram();
        pendingEvt.evt=SUBPROGRAM_CHANGE_EVENT;
        break;
      }
    }
    else if(PrgKeyEvts.Bit_Flag.Key_Back)
    {
      switch(ProgramArray[proCurrentIndex]->Setting_Array[ProgramArray[proCurrentIndex]->SettingArray_Index])
      {
      case SET_PROGRAM:
        Program_Initial_Inner_Data();
        pendingEvt.evt=BACK_END_EVENT;
        break;
      case SET_SUBPROGRAM:
        pendingEvt.evt=PROGRAM_CHANGE_EVENT;
        break;
      }
    }		
    else if(PrgKeyEvts.Bit_Flag.Key_Enter)
    {
      if(ProgramLibConfigPara->Pro_Config[ProgramidArrayIndex].ProgramSubID==NULL ||
         ProgramArray[proCurrentIndex]->Setting_Array[ProgramArray[proCurrentIndex]->SettingArray_Index]==SET_SUBPROGRAM)
      {
        Program_NextStatus(PS_SETUP);
        pendingEvt = ProgramArray[proCurrentIndex]->Prg_Setup(&PrgKeyEvts);
      }
      else
      {
        ProgramArray[proCurrentIndex]->SettingArray_Index++;
        switch(ProgramArray[proCurrentIndex]->Setting_Array[ProgramArray[proCurrentIndex]->SettingArray_Index])
        {
        case SET_SUBPROGRAM:
          pendingEvt.evt=SUBPROGRAM_CHANGE_EVENT;
          break;
        }
      }
    }
    break;
  case PS_SETUP:
    if(PrgKeyEvts.Bit_Flag.Key_NumberKey)
    {
      PrgKeyEvts.Bit_Flag.Key_NumberKey=false;
      for(char i=0;i<SETPARANUM;i++)
      {
        if(ProgramArray[proCurrentIndex]->Setting_Array[ProgramArray[proCurrentIndex]->SettingArray_Index]==DefaultNumber[i].SetState)
        {
          if(DefaultNumber[i].NumericKey.Used==1)
          {
            if(NumberKey_Data.ShowBitNumber<DefaultNumber[i].NumericKey.Digits)
            {
              NumberKey_Data.ShowBitNumber++;
              if(NumberKey_Data.ShowValue!=0)
                NumberKey_Data.ShowValue *=10;
              NumberKey_Data.ShowValue +=NumberKey_Data.InputValue;
            }
          }
        }
      }
    }
    else if(PrgKeyEvts.Bit_Flag.Key_Enter)
    {
      if(NumberKey_Data.Events.Bits_All!=0)
      {						
        switch(ProgramArray[proCurrentIndex]->Setting_Array[ProgramArray[proCurrentIndex]->SettingArray_Index])
          
        {
        case SET_TIME:
          if(NumberKey_Data.ShowValue>=15 && NumberKey_Data.ShowValue<=99)
          {
            Data_Set_Time(NumberKey_Data.ShowValue * 60);
          }
          else
          {
            PrgKeyEvts.Bit_Flag.Key_Enter=false;
          }
          break;
        case SET_SPEED:
          if(NumberKey_Data.ShowValue <= Data_Get_Speed(MAX_SPEEDX10)&&NumberKey_Data.ShowValue >= Data_Get_Speed(MIN_SPEEDX10))
          {
            Data_Set_Speed_Base_User(NumberKey_Data.ShowValue,1);
            //Data_Set_Speed(NumberKey_Data.ShowValue,SystemConfig_Get_Display_Unit(User_Get_UserSlot( ))==METRIC,1);
          }
          else
          {
            PrgKeyEvts.Bit_Flag.Key_Enter=false;
          }
#if 0
          if(SystemConfig_Get_Display_Unit(User_Get_UserSlot()==METRIC)
          {
            if(NumberKey_Data.ShowValue <= SystemConfig_Get_Int(MAX_KPHX10_UINT16)&&NumberKey_Data.ShowValue >= SystemConfig_Get_Int(MIN_KPHX10_UINT16))
            {
              Data_Set_Speed(NumberKey_Data.ShowValue,METRIC,1);
            }
            else
            {
              PrgKeyEvts.Bit_Flag.Key_Enter=false;
            }
          }
             
          else
          {
            if(NumberKey_Data.ShowValue <= SystemConfig_Get_Int(MAX_MPHX10_UINT16)&&NumberKey_Data.ShowValue >= SystemConfig_Get_Int(MIN_MPHX10_UINT16))
            {
              Data_Set_Speed(NumberKey_Data.ShowValue,STANDARD,1);
            }
            
            else
            {
              PrgKeyEvts.Bit_Flag.Key_Enter=false;
            }
          }
#endif
             break;
        case SET_INCLINE:
          
          if(NumberKey_Data.ShowValue%5==0 && NumberKey_Data.ShowValue <= SystemConfig_Get_Int(MAX_INCLINEX10_UINT16))
          {
            Data_Set_Incline(NumberKey_Data.ShowValue,1);
          }
          else
          {
            PrgKeyEvts.Bit_Flag.Key_Enter=false;
          }
          break;
        case SET_RESISTANCE:
          if(NumberKey_Data.ShowValue >= SystemConfig_Get_Char(MIN_RESISTANCE_UINT8) && NumberKey_Data.ShowValue <= SystemConfig_Get_Char(MAX_RESISTANCE_UINT8))
          {
            Data_Set_Resistance(NumberKey_Data.ShowValue,1);
          }
          else
          {
            PrgKeyEvts.Bit_Flag.Key_Enter=false;
          }
          break;
        case SET_LEVEL:
          if(NumberKey_Data.ShowValue >= Data_Get_Level(MIN_LEVEL) && NumberKey_Data.ShowValue <= Data_Get_Level(MAX_LEVEL))
          {
            Data_Set_Level(NumberKey_Data.ShowValue);
          }
          else
          {
            PrgKeyEvts.Bit_Flag.Key_Enter=false;
          }
          break;
        case SET_WATTS:
          if(NumberKey_Data.ShowValue%5==0 && NumberKey_Data.ShowValue >= 25 && NumberKey_Data.ShowValue <= 400)
          {
            Data_Set_Watt(NumberKey_Data.ShowValue);
          }
          else
          {
            PrgKeyEvts.Bit_Flag.Key_Enter=false;
          }
          break;
        }
             memset(&NumberKey_Data,0,sizeof(NumberKeyData));
             
      }
             
    }
             pendingEvt = ProgramArray[proCurrentIndex]->Prg_Setup(&PrgKeyEvts);
             break;
  case PS_WAIT_3S:
  case PS_RESTART_321:
    if(Prg321Isr.WaitFlag)
    {
      Prg321Isr.WaitFlag=0;
      if(Prg321Isr.TimerCount==0)
      {
        pendingEvt.evt= THREE_EVENT;
      }
      else if(Prg321Isr.TimerCount==1)
      {
        pendingEvt.evt= TWO_EVENT;
        LCB_Send_Cmd(DS_MOT_RPM,Data_Get_RPM(MIN_RPM));
        
        Prg_isr_TimerCounterClear(TIMER_20S_No_RPM);
      }
      else if(Prg321Isr.TimerCount==2)
      {
        pendingEvt.evt= ONE_EVENT;
      }
      else if(Prg321Isr.TimerCount==3)
      {                
        Prg321Isr.TimerDelay=0;
        Prg321Isr.TimerCount=0;
        Prg321Isr.WaitFlag=1;
        pendingEvt.evt= PROGRAM_RUN_EVENT;
        Data_Set_Event(DATA_RUN_EVENT);
#ifdef  __WIFI_FUNC__
        ChangeWorkoutState(WIFI_WORKOUT_RUNNING);
#endif
        if(Prg_StateMachine==PS_WAIT_3S)
          Program_NextStatus(PS_WARMUP);
        else
          Program_NextStatus(Prg_StateMachineBackup);
        
      }       
    }
    break;
  case PS_WARMUP:
  case PS_RUN:
  case PS_COOLDOWN:
    extern UINT8 appStart;    //zsf
    extern UINT16 appProId;    //zsf
    
    if (appStart == 0)
    {
      Show_ProgramLED(Program_Get_ProgramID());    //zsf
    }
    else
    {
      if (appProId == 0)
        Show_ProgramLED(appProId);    //zsf
      else
        Show_ProgramLED(10);    //zsf
    }
    
    pendingEvt =  ProgramArray[proCurrentIndex]->Prg_Workout(&PrgKeyEvts);
    if(pendingEvt.WorkOutEvts.Workout_Complete_Evt==false)
    {
      if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
      {
        if(SpeedDataBackup!=Data_Get_Speed(TARGET_SPEEDX1000))
        {
          SpeedDataBackup=Data_Get_Speed(TARGET_SPEEDX1000);
          pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
          pendingEvt.WorkOutEvts.Workout_Speed_Evt=true;
        }
      }
      else
      {
        if(ResistanceDataBackup!=Data_Get_Resistance(TARGET_RESISTANCE))
        {
          ResistanceDataBackup=Data_Get_Resistance(TARGET_RESISTANCE);
          pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
          pendingEvt.WorkOutEvts.Workout_Resistance_Evt=true;
        }
        static UINT16 lw_EP_OldSpeed =  0xffff;
        if(lw_EP_OldSpeed != Data_Get_Speed(EPBI_SPEEDX1000))
        {
          if(Program_Get_ProgramID() == PS_DISTANCE_GOAL)
          {
            Data_Set_ElapsedOldTime(Data_Get_Time(ELAPSED_TIME));
            Data_Set_remainingOldDistancex1000(Data_Get_Distance(REMAINING_DISTANCE));
          }
          else if(Program_Get_ProgramID() == PS_CALORIES_GOAL)
          {
            Data_Set_ElapsedOldTime(Data_Get_Time(ELAPSED_TIME));
            Data_Set_remainingOld_Calories(Data_Get_Calories(REMAINING_CALORIES));
          }
          else if(Program_Get_ProgramID() == PS_CUSTOM_DISTANCE)
          {
            Data_Set_ElapsedOldTime(Data_Get_Time(ELAPSED_TIME));
            ProCustom_Set_SegRemainingDistance();
          }
          lw_EP_OldSpeed = Data_Get_Speed(EPBI_SPEEDX1000);
        }
      }
      if(InclineDataBackup!=Data_Get_Incline(TARGET_INCLINEX10))
      {
        InclineDataBackup=Data_Get_Incline(TARGET_INCLINEX10);
        pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
        pendingEvt.WorkOutEvts.Workout_Incline_Evt=true;
      }
    }
    if(PrgKeyEvts.Bit_Flag.Key_Enter)
    {
      
      if(NumberKey_Data.Events.Bit.Spd || NumberKey_Data.Events.Bit.Res)
      {
        if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
        {
          if(SystemConfig_Get_Display_Unit(User_Get_UserSlot( ))==METRIC)
          {
            if(NumberKey_Data.ShowValue <= SystemConfig_Get_Int(MAX_KPHX10_UINT16)&&NumberKey_Data.ShowValue >= SystemConfig_Get_Int(MIN_KPHX10_UINT16))
            {
              Data_Set_Speed_Base_User(NumberKey_Data.ShowValue,1);
              // Data_Set_Speed(NumberKey_Data.ShowValue,METRIC,1);
            }
          }
          else
          {
            if(NumberKey_Data.ShowValue <= SystemConfig_Get_Int(MAX_MPHX10_UINT16)&&NumberKey_Data.ShowValue >= SystemConfig_Get_Int(MIN_MPHX10_UINT16))
            {
              Data_Set_Speed_Base_User(NumberKey_Data.ShowValue,1);
              //Data_Set_Speed(NumberKey_Data.ShowValue,STANDARD,1);
            }
          }      
          if(SpeedDataBackup!=Data_Get_Speed(TARGET_SPEEDX1000))
          {
            SpeedDataBackup=Data_Get_Speed(TARGET_SPEEDX1000);
            pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
            pendingEvt.WorkOutEvts.Workout_Quick_Recover=true;
          }
          
        }
        else
        {
          if(NumberKey_Data.ShowValue >= SystemConfig_Get_Char(MIN_RESISTANCE_UINT8) && NumberKey_Data.ShowValue <= SystemConfig_Get_Char(MAX_RESISTANCE_UINT8))
          {
            Data_Set_Resistance(NumberKey_Data.ShowValue,1);
          }
          if(ResistanceDataBackup!=Data_Get_Resistance(TARGET_RESISTANCE))
          {
            ResistanceDataBackup=Data_Get_Resistance(TARGET_RESISTANCE);
            pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
            pendingEvt.WorkOutEvts.Workout_Quick_Recover=true;
          }
        }
        memset(&NumberKey_Data,0,sizeof(NumberKeyData));
      }
    }
    if(PrgKeyEvts.Bit_Flag.Key_NumberKey)
    {
      Prg_isr_TimerCounterClear(TIMER_QUICKKEY);
      if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
      {
        if(DefaultNumber[1].NumericKey.Used==1)
        {
          if(NumberKey_Data.ShowBitNumber<3)
          {
            NumberKey_Data.ShowBitNumber++;
            if(NumberKey_Data.ShowValue!=0)
              NumberKey_Data.ShowValue *=10;
            NumberKey_Data.ShowValue +=NumberKey_Data.InputValue;
          }
        }
        PrgKeyEvts.Bit_Flag.Key_NumberKey=false;
        pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
        pendingEvt.WorkOutEvts.Workout_QuickKeySpd_Evt=true;
      }
      else
      {
        if(ProgramArray[proCurrentIndex]->Programid != PS_CONSTANT_WATTS)
        {
          if(DefaultNumber[1].NumericKey.Used==1)
          {
            if(NumberKey_Data.ShowBitNumber<2)
            {
              NumberKey_Data.ShowBitNumber++;
              if(NumberKey_Data.ShowValue!=0)
                NumberKey_Data.ShowValue *=10;
              NumberKey_Data.ShowValue +=NumberKey_Data.InputValue;
            }
          }
          PrgKeyEvts.Bit_Flag.Key_NumberKey=false;
          pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
          pendingEvt.WorkOutEvts.Workout_QuickKeyRes_Evt=true;
        }
      }
    }
    if(NumberKey_Data.Events.Bit.Spd || NumberKey_Data.Events.Bit.Res)
    {
      if(Prg_isr_TimerCounter(TIMER_QUICKKEY,50))
      {
        memset(&NumberKey_Data,0,sizeof(NumberKeyData));
        NumberKey_Data.Events.Bit.Recover = true ;
      }
    }
    if(NumberKey_Data.Events.Bit.Recover)
    {
      NumberKey_Data.Events.Bit.Recover =false;
      pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
      pendingEvt.WorkOutEvts.Workout_Quick_Recover=true;
    }
    if(PrgKeyEvts.Bit_Flag.Key_Pause)
    {
      PrgKeyEvts.Bit_Flag.Key_Pause=false;
      Prg_isr_TimerCounterClear(TIMER_PAUSE_5MIN);
      Prg_isr_TimerColse(TIMER_QUICKKEY);
#ifdef  __WIFI_FUNC__
      ChangeWorkoutState(WIFI_WORKOUT_PAUSED);
#endif
      Data_Set_Event(DATA_PAUSE_EVENT);
      LCB_Send_Cmd(DS_INC_LOCATION,Data_Get_Incline(MIN_INCLINEADC));
      LCB_Send_Cmd(DS_INC_ACTION,0x00);

#ifdef  AE      
      Incline_Set_Incline(Data_Get_Incline(TARGET_INCLINEX10));//Incline_Set_Incline(Data_Get_Incline(MIN_INCLINEX10));
      //          Incline_Updata_Incline();
#endif   
#ifdef  TM	  
      LCB_Send_Cmd(DS_ECB_LOCATION,0);
#else
      ECB_Set_Resistance(Data_Get_Resistance(TARGET_RESISTANCE));//ECB_Set_Resistance(1);
      ////      ECB_Data_Updata();
#endif
      LCB_Send_Cmd(DS_MOT_RPM,0);
      LCB_Send_Cmd(DS_INDUCTION_PWM,0); 
      Prg_StateMachineBackup=Program_Get_Status();
      Program_NextStatus(PS_PAUSED);
      
      pendingEvt.evt=PROGRAM_PAUSE_EVENT;
    }
  case PS_COMPLETED:
    if(PrgActionFlag.HRSignalStatus)
    {
      if(Data_Get_Heart(CURRENT_HEART)==0)
      {
        pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
        pendingEvt.WorkOutEvts.Workout_HR_ChangeToZero=true;
        PrgActionFlag.HRSignalStatus=0;
        
      }
    }
    else if(PrgActionFlag.HRSignalStatus==0)
    {
      if(Data_Get_Heart(CURRENT_HEART)>40)
      {
        pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
        pendingEvt.WorkOutEvts.Workout_HR_IsNotZero=true;
        PrgActionFlag.HRSignalStatus=1;
        
      }
    }
    
    //        PrgKeyEvts.All_KeyEvents=0;
    if(Bluetooth_Get_ConnectStatus())
    {
      if(Program_Get_Status()==PS_COMPLETED && Prg_isr_TimerCounter(TIMER_WORKOUTCOMPLETE,1))
      {
        Program_Set_VirtualKey((UINT16)VK_RESET | FRAME_STAGE_PRGRUN);
      }
    }
    else if(Prg_isr_TimerCounter(TIMER_WORKOUTCOMPLETE,600))
    {
      Program_Set_VirtualKey((UINT16)VK_RESET | FRAME_STAGE_PRGRUN);
    }
    if(pendingEvt.WorkOutEvts.Workout_Complete_Evt)
    {
#ifdef  __WIFI_FUNC__
      ChangeWorkoutState(WIFI_WORKOUT_STOP);
#endif
      RemoteStopWorkotState();
      Data_Set_Event(DATA_PAUSE_EVENT);
      Prg_isr_TimerCounterClear(TIMER_WORKOUTCOMPLETE);
      LCB_Send_Cmd(DS_INC_LOCATION,Data_Get_Incline(MIN_INCLINEADC));
#ifdef  AE
      Incline_Set_Incline(Data_Get_Incline(MIN_INCLINEX10));
      //          Incline_Updata_Incline();
      //          Incline_Proce_AE();
#endif
#ifdef  TM	  
      LCB_Send_Cmd(DS_ECB_ZERO,0);
#else
      ECB_Set_Zero(); 
      ////      ECB_Data_Updata();
#endif
      LCB_Send_Cmd(DS_MOT_RPM,0);
      LCB_Send_Cmd(DS_INDUCTION_PWM,0);
    }
    if( PrgKeyEvts.Bit_Flag.Key_ChangeDisplay)
    {
      PrgKeyEvts.Bit_Flag.Key_ChangeDisplay=false;
      pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
      pendingEvt.WorkOutEvts.Workout_ChangeDisplay=true;
    }
    if(PrgActionFlag.ScanMode)
    {
      if(Prg_isr_TimerCounter(TIMER_ScanFunction,50))
      {
        Prg_isr_TimerCounterClear(TIMER_ScanFunction);
        pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
        pendingEvt.WorkOutEvts.Workout_ChangeDisplay=true;
      }
    }     
    break;
  case PS_PAUSED:
    //        PrgKeyEvts.All_KeyEvents=0;
    //    if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)!=TREADMILL)
    //    {
    //      if(Data_Get_RPM(EPBI_RPM)>=10)
    //      {
    //        if(Prg_isr_TimerCounter(TIMER_5S_PedalStart,60))
    //        {
    //          Prg_isr_TimerColse(TIMER_5S_PedalStart);
    //          Program_Set_VirtualKey(VK_START | FRAME_STAGE_PRGRUN);
    //        }
    //      }
    //      else
    //      {
    //        Prg_isr_TimerCounterClear(TIMER_5S_PedalStart);
    //      }
    //    }    
    if(Prg_isr_TimerCounter(TIMER_PAUSE_5MIN,3000))
    {
      Bluetooth_Set_VirtualKey(VK_RESET);
      Program_Set_VirtualKey((UINT16)VK_RESET | FRAME_STAGE_PRGRUN);
    }   
    break;
  default:
    break;
  }
             if(PrgKeyEvts.Bit_Flag.Key_Reset)
             {
               if(Program_Get_Status()>PS_SETUP)
               {//user offline summery
                 if(Bluetooth_Get_ConnectStatus() == 0)
                 {
                   UINT8 UserID = User_Get_UserSlot();
                   USERS_INFO userinfo = SystemConfig_Get_UserInfo(UserID);
                   UINT8 j = userinfo.WorkoutSummaryDataCount % 3;
                   userinfo.WorkoutSummaryData[j].workouttime = Data_Get_Time(ELAPSED_TIME);
                   userinfo.WorkoutSummaryData[j].workoutcalorie = Data_Get_Calories(ELAPSED_CALORIES);
                   userinfo.WorkoutSummaryData[j].workoutdistance = Data_Get_Distance(ELAPSED_DISTANCE);
                   userinfo.WorkoutSummaryData[j].workoutmaxspeed = Data_Get_Speed(MAX_SPEEDX10);
                   userinfo.WorkoutSummaryData[j].workoutaveragespeed = Data_Get_Speed(AVERAGE_SPEEDX10);
                   userinfo.WorkoutSummaryData[j].workoutmaxheartrate = Data_Get_Heart(CURRENT_HEART);
                   userinfo.WorkoutSummaryData[j].workoutaverageheartrate = Data_Get_Heart(AVERAGE_HEART);
                   userinfo.WorkoutSummaryData[j].workoutelevation = Data_Get_Incline(CURRENT_INCLINEX10);
                   //                if(userinfo.WorkoutSummaryDataCount < 2)
                   userinfo.WorkoutSummaryDataCount++;
                   //                else 
                   //                    userinfo.WorkoutSummaryDataCount = 0;
                   if(SystemConfig_Set_UserInfo(userinfo,UserID))
                   {
                     SystemConfigProcess(SVK_SAVE_USER2FLASH, &UserID, sizeof(UserID));
                   }
                 }
                 Data_Set_Event(DATA_SAVEDATA_EVENT);
               }
               //RemoteStopWorkotState();
               
               PrgKeyEvts.Bit_Flag.Key_Reset=false;
#ifdef  __WIFI_FUNC__
               ChangeWorkoutState(WIFI_WORKOUT_STOP);
#endif
               unsigned char Err_state_flag = UTL_GetErrState();
               if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)!=TREADMILL)
                 Err_state_flag = +1;
               
               if(Err_state_flag != UTL_GetErrState())  //if(2 != UTL_GetErrState())
               {
                  LCB_Send_Cmd(DS_INDUCTION_PWM,0);
#ifdef  TM                               	  
                 LCB_Send_Cmd(DS_ECB_ZERO,0);
#else
                 ECB_Set_Zero(); 
////                 ECB_Data_Updata();
#endif
                 LCB_Send_Cmd(DS_MOT_RPM,0);
                 LCB_Send_Cmd(DS_INC_LOCATION,Data_Get_Incline(MIN_INCLINEADC));

#ifdef  AE
                 Incline_Set_Incline(Data_Get_Incline(MIN_INCLINEX10));
                 //          Incline_Updata_Incline();
                 //          Incline_Proce_AE();
#endif
               }
               if(wlt2564_Get_ConnectStatus() == 0 ||(Program_Get_Status()<PS_WAIT_3S)||(Program_Get_Status()>PS_COOLDOWN))    //zsf
               {
                 Program_Initial_Inner_Data();
                 Program_NextStatus(PS_IDLE);
               }
               pendingEvt.evt=PROGRAM_RESET_EVENT;
             }    
             PrgKeyEvts.Bit_All=0;
             if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
             {
               LCB_Set_DriverType(LCB_MOTOR);
             }
             else
             {
               LCB_Set_DriverType(LCB_EP_BIKE);
             }
             
             return pendingEvt;
}
/***************************************************************************//**
* @brief
* 此函数用于取得Program UUID
* @param[in]：none
* @param[out]：返回Program UUID
******************************************************************************/

char* Program_Get_UUID(void)
{
    return (char*)(ProgramUUID[proCurrentIndex]);
}
/***************************************************************************//**
* @brief
* 此函数用于取得Program Description
* @param[in]：* plength：描述长度
* @param[out]：返回Program UUDescription
******************************************************************************/

char* Program_Get_Description(UINT16* plength)
{
    *plength = 0;
    return NULL;
}
/***************************************************************************//**
* @brief
* 此函数用于取得Stage Goal Remaining
* @param[in]：none
* @param[out]：
返回Stage Goal Remaining
******************************************************************************/
UINT16 Program_Get_Stage_Goal_Remaining(void)
{
    UINT16 ret = 0;
    
    return ret;
}
/***************************************************************************//**
* @brief
* 此函数用于取得Stage Goal
* @param[in]：none
* @param[out]：
返回Stage Goal
******************************************************************************/
UINT16 Program_Get_Stage_Goal(void)
{
    UINT16 ret = 0;
    
    return ret;
}
/***************************************************************************//**
* @brief
* 此函数用于取得Lap
* @param[in]：none
* @param[out]：
返回Lap
******************************************************************************/
UINT16 Program_Get_Lap_Index(void)
{
    UINT16 ret = 0;
    
    return ret;
}
/***************************************************************************//**
* @brief
* 此函数用于取得Split
* @param[in]：none
* @param[out]：
返回Split
******************************************************************************/
UINT16 Program_Get_Split_Index(void)
{
    UINT16 ret = 0;
    
    return ret;
}
/***************************************************************************//**
* @brief
* 此函数用于取得Floor
* @param[in]：none
* @param[out]：
返回Floor
******************************************************************************/

UINT16 Program_Get_Floor(void)
{
    UINT16 ret = 0;
    
    return ret;
}
/***************************************************************************//**
* @brief
* 此函数用于取得Elevation
* @param[in]：none
* @param[out]：
返回Elevation
******************************************************************************/

UINT16 Program_Get_Elevation(void)
{
    UINT16 ret = 0;
    
    return ret;
}

/***************************************************************************//**
* @brief
* 此函数用于取得VO2
* @param[in]：none
* @param[out]：
返回VO2
******************************************************************************/

UINT16 Program_Get_VO2(void)
{
    UINT16 ret = 0;
    return ret;
}

/***************************************************************************//**
* @brief
* 此函数用于取得Sprint8 Sweat Score
* @param[in]：index：索引
* @param[out]：
返回Sprint8 Sweat Score
******************************************************************************/
UINT16 Sprint8_Get_Sweat_Score(UINT8 index)
{
    UINT16 ret = 0;
    
    return ret;
}
/***************************************************************************//**
* @brief
* 此函数用于取得Sprint8 Total Sweat Score
* @param[in]：none
* @param[out]：
返回Sprint8 Total Sweat Score
******************************************************************************/

UINT16 Sprint8_Get_Total_Sweat_Score(void)
{
    UINT16 ret = 0;
    
    return ret;
}
/***************************************************************************//**
* @brief
* 此函数用于取得Fitness Test Sweat Score
* @param[in]：index：索引
* @param[out]：
返回Fitness Test Sweat Score
******************************************************************************/

UINT16 Fitness_Test_Get_Score(void)
{
    UINT16 ret = 0;
    
    return ret;
}
/***************************************************************************//**
* @brief
* 此函数用于读取版本号
* @param[in]：*buffer用于读取版本
* @param[out]：返回版本总长度
******************************************************************************/

UINT8 Program_Get_Vision(UINT8 *buffer)
{
    UINT8 string[8] = PROGRAM_VISION;
    memcpy(buffer,string,sizeof(string));
    return(strlen(PROGRAM_VISION));
}


UINT8 Program_Is_Working(void)
{
    if(Program_Get_Status()<PS_WARMUP)
        
        return 0;
    else
        return 1;
}

UINT16 Sprint8_Get_SprintStage(void)
{
    return Data_Get_Sprint8SprintStage();
}

UINT8 Program_Get_DefaultDotMatrix(void)
{
    return DefaultDotmatrix;
}

MyFirst5kDat Program_Get_MyFirst5k_WeekWorkout(void)
{
    MyFirst5kDat Temp_MyFirst5kData;
    Temp_MyFirst5kData.Week=ProgramArray[proCurrentIndex]->Prg_SetupData(INDEX_WEEK);
    Temp_MyFirst5kData.Workout=ProgramArray[proCurrentIndex]->Prg_SetupData(INDEX_WORKOUT);
    return Temp_MyFirst5kData;
}

MyFirst5kDat Program_Get_MyFirst5k_WeekWorkoutBuff(void)
{
    MyFirst5kDat ProMyFirst5k_DataBuff; 
    ProMyFirst5k_DataBuff.Week= ProgramArray[proCurrentIndex]->Prg_SetupData(INDEX_WEEKBUFF);
    ProMyFirst5k_DataBuff.Workout= ProgramArray[proCurrentIndex]->Prg_SetupData(INDEX_WORKOUTBUFF);

    return ProMyFirst5k_DataBuff;
}
UINT8 Program_Get_CustomSegment(void)
{
    UINT8 lby_Segment;
    
    lby_Segment = ProgramArray[proCurrentIndex]->Prg_SetupData(INDEX_SEGMENT);
    return (lby_Segment+1);
}


UINT8 Program_Get_Laps(void)
{   
    return Data_Get_Laps();
}
UINT8 Program_Get_LapsCircle(void)
{   
    UINT8 by_Dat=0;
    if(Program_Get_Status()<=PS_SETUP)
    {
        by_Dat=0xff;//?竚场??び獹
    }
    else
    {
        by_Dat=Data_Get_LapsCircle();
    }
    return by_Dat;
}
UINT8 Program_Get_DistanceLable(void)
{   
    UINT8 by_Dat=0;
    if(Program_Get_Status()<=PS_SETUP)
    {
        by_Dat=ProgramArray[proCurrentIndex]->Prg_SetupData(INDEX_DISTANCE_LABLE);;
    }
    return by_Dat;
}
TARGET_DIS_ENUM Program_Get_DistanceID(void)
{
    return Target_distance;
}             
             