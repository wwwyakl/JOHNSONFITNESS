#include "Program_Inner.h"
#include "HAL_Lib.h"
#include "Setting_Config.h"
#include "UtilitiesErrorcode.h"
#include"Pro_manual.h"
#include"Pro_hrc.h"
#include"Pro_table.h"
#include"Pro_constant.h"
#include"Pro_goals.h"
#include <string.h>
#include "Digital_wifi.h"
#define PROGRAM_VISION "1.0.2"
#define SETPARANUM 7
#define PRG_TIMER_CH 16
const struct
{
    struct
    {
        unsigned char Used : 1;   // 0:��ʹ�����ּ�, 1:ʹ��
        unsigned char Digits : 7; // ����λ��
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
    {"00000000-0000-0000-0000-000000000000"},//PS_FITNESS_TEST
    {"00000000-0000-0000-0000-000000000000"},//PS_LANDMARK
};
const int aby_pwm_Offset[30]=
{
//L1	L2	L3	L4	L5	L6	L7	L8	L9	L10	L11	L12	L13	L14	L15	L16	L17	L18	L19	L20	L21	L22	L23	L24	L25	L26	L27	L28	L29	L30
  10,	13,	17,	20,	24,	27,	29,	31,	33,	35,	37,	39,	41,	43,	45,	47,	50,	52,	54,	57,	59,	61,	64,	66,	68,	71,	73,	75,	78,	80,
};
__no_init  PROFILE_INEER DotmatrixData;
__no_init static struProgram ProgramArray[PS_TOTALS];
__no_init static UCHAR proCurrentIndex;
__no_init static UCHAR Profile_Pos;
__no_init static UCHAR WorkoutResistanceData,WorkoutInclineData;
__no_init static UINT16 WorkoutSpeedData;//,WorkoutTargetHeart;
__no_init const WattsParameterStruct *_Watts_Talbelist;
__no_init PROGRAM_STATUS g_StateMachine;
__no_init ProgramLibParaConfig *ProgramParameter;
__no_init QUICK_KEY QuickKey;
__no_init static UINT8 Heart_Satus;
__no_init  UINT8 DefaultDotmatrix;
__no_init  UINT16 InductionOutput_5P[5];
__no_init  UINT16 InductionOutput_30P[30];
__no_init static PROGRAM_PENDING_EVTS g_PRGPreEvent;
UINT16 Debug_read_statu = 0;
UCHAR T75_Overspeed_Flag = 0;
UCHAR ShowOverspeedStringTime;
bool Key_SensorError_Flag;
bool Key_SafekeyError_Flag;
__no_init struct 
{
    UINT16 Timer_Delay[PRG_TIMER_CH];
    UINT8  Timer_Channel[PRG_TIMER_CH];
}Prg_isr;
__no_init struct 
{
    UINT16 Scan:1;
    UINT16 ReadPWM:1;
}Prg_Mode;

__no_init char ValueLimit;
__no_init struct 
{
    UINT16 Counter;
    UINT8 Count;
    UINT8 Event:1;
}Watit3s;
__no_init UINT32 _ul_Segment_Time;
__no_init UINT16 w_Current_Segment;
__no_init UCHAR Setting_Index;
__no_init UCHAR Sprint8Stage;

__no_init SETTING_ENUM SettingState;
__no_init char Program_index;
static ProgramKeyEvts key_event;

void Program_1ms_ISR(void);
UINT8 Prg_isr_TimerCounter(UINT8 by_Who,UINT16 w_Dat);
void Prg_isr_TimerCounterClear(UINT8 by_Index);
void Prg_isr_TimerColse(UINT8 by_Dat);

UINT8 Prg_isr_TimerCounter(UINT8 by_Who,UINT16 w_Dat)
{
    UINT8 b_Out = 0;
    if(Prg_isr.Timer_Channel[by_Who] == 1 && Prg_isr.Timer_Delay[by_Who] >= w_Dat)
    {
        b_Out=1;
        Prg_isr.Timer_Channel[by_Who] = 0;
    }
    return b_Out;
}
void Prg_isr_TimerCounterClear(UINT8 by_Index)
{
    Prg_isr.Timer_Delay[by_Index]=0;
    Prg_isr.Timer_Channel[by_Index]=1;
}
void Prg_isr_TimerColse(UINT8 by_Dat)
{
    Prg_isr.Timer_Channel[by_Dat]=0;
}
void Program_1ms_ISR(void)
{
    static UINT16 w_Timer_100ms=0;
    if(g_StateMachine==PS_WAIT_3S)
    {
        if(!Watit3s.Event)
        {
            if(Watit3s.Counter++>850)
            {
                Watit3s.Count++;
                Watit3s.Counter=0;
                Watit3s.Event=1;
            }
        }
    }  
    if(++w_Timer_100ms<100)return;
    w_Timer_100ms=0;
    for(UINT8 i=0;i<PRG_TIMER_CH;i++)
    {
        if(Prg_isr.Timer_Channel[i]==1 && Prg_isr.Timer_Delay[i]<0xffff)
        {
            Prg_isr.Timer_Delay[i]++;
        }
    }
    if(ShowOverspeedStringTime<30)
    {
        if(T75_Overspeed_Flag)
        {
            ShowOverspeedStringTime++;
        }
    }
}

void Program_Initial_Inner_Data(void)
{
    Data_Process_Intilal_Data();
    memset(&DotmatrixData.SpeedProfile,Program_Get_Speed(DATA_AUTO,0),sizeof(DotmatrixData.SpeedProfile));
    memset(&DotmatrixData.InclineProfile,0,sizeof(DotmatrixData.InclineProfile));
    memset(&DotmatrixData.ResistanceProfile,Data_Get_Resistance(MIN_RESISTANCE),sizeof(DotmatrixData.ResistanceProfile));
    memset(ProgramArray,0,sizeof(struProgram));
    memset(&QuickKey,0,sizeof(QUICK_KEY));
    memset(&Prg_isr,0,sizeof(Prg_isr));
    memset(&Prg_Mode,0,sizeof(Prg_Mode));
    memset(&InductionOutput_30P,0,sizeof(InductionOutput_30P));
    Prg_Mode.ReadPWM=1;
    //    QuickKey.Setting = 0;
    ValueLimit=0;
    Watit3s.Counter=0;
    Watit3s.Count=0;
    Watit3s.Event=1;
		Heart_Satus=0;
        Sprint8Stage=0;
    Setting_Index=0;
    proCurrentIndex = 0;
    Profile_Pos=0;
    Program_index=0;
    _ul_Segment_Time=6000;
    w_Current_Segment=0;
    g_StateMachine=PS_IDLE;
    SettingState=SET_PROGRAM;
    if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
        DefaultDotmatrix=SPEED_TYPE;
    else
        DefaultDotmatrix=RESISTANCE_TYPE;
    WorkoutSpeedData=Data_Get_Speed(TARGET_SPEED);
    WorkoutInclineData=Data_Get_Incline(TARGET_INCLINEX10);
    WorkoutResistanceData=Data_Get_Resistance(TARGET_RESISTANCE);
    memset(&g_PRGPreEvent,0,sizeof(PROGRAM_PENDING_EVTS));
#ifdef __PROGRAM_MANUAL__
    initManual();
#endif
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
#ifdef __PROGRAM_GOAL__
	initLandmark();
#endif
}
void Program_Add_Array(struProgram pro)
{
    if(pro.Programid<PS_TOTALS)
    {
        memcpy(&ProgramArray[pro.Programid],&pro,sizeof(struProgram));
    }
}
void Program_Add_Progam(void)
{
	if(Program_index!=ProgramParameter->ProgramNum-1)
        Program_index++;
    else
        Program_index=0;
	proCurrentIndex=ProgramParameter->ProgramidArray[Program_index];
	ProgramArray[proCurrentIndex].Initial();  
	
}
void Program_Dec_Progam(void)
{
	if(Program_index!=0)
		Program_index--;
    else
        Program_index=ProgramParameter->ProgramNum-1;
	proCurrentIndex=ProgramParameter->ProgramidArray[Program_index];
	ProgramArray[proCurrentIndex].Initial();  
	
}
void Program_Set_RPM(UINT8 by_Dat)
{
    Data_Set_FixedRPM(by_Dat);
}
/***************************************************************************//**
* @brief
*ע���жϷ���  
* @param[in]:none
* @param[out]:none
******************************************************************************/

void Program_Regist_Functions(void)
{
	HAL_SystemTicker_Alloc_ISR(Data_1ms_ISR);
	HAL_SystemTicker_Alloc_ISR(Program_1ms_ISR);
}
/***************************************************************************//**
* @brief
*  ��ʼ����ʽ
* @param[in]
*   array[]ָ���ʼ���ṹ�壻
*   size��TABLE�����
*  DotMatrix_length:��������
* @param[out]��none
******************************************************************************/
void Program_Initial_Data(ProgramLibParaConfig *ProgramConfig)
{
  ProgramParameter=ProgramConfig;
  Program_Initial_Inner_Data();
}


/***************************************************************************//**
* @brief
*  ��ʼ��EP��BIKE�����ر�������EP��BIKE
* @param[in]
*   Watts_Para����ʼ�����ر�ṹ�壻
* @param[out]��none
******************************************************************************/
void Program_Load_WattsTable(const WattsParameterStruct *Watts_Para)
{
    _Watts_Talbelist=Watts_Para;
}

/***************************************************************************//**
* @brief
* �˺�������ѡ���Ӧ�ĳ�ʽ
* @param[in]
*   prg_Type����ʽ����ö�ٱ�����
* @param[out]��none
******************************************************************************/
void Program_Select_ID(PROGRAM_ID prg_Type)
{
    UINT8 i;
    proCurrentIndex = prg_Type;
    for(i=0;i<ProgramParameter->ProgramNum-1;i++)
  	{
        if(ProgramParameter->ProgramidArray[i]==proCurrentIndex)
            break;
  	}
    Program_index=i;
    ProgramArray[proCurrentIndex].Initial();  
    
}
/***************************************************************************//**
* @brief
* �˺�������ȡ�õ�ǰ�ĳ�ʽ
* @param[in]
*   none
* @param[out]��
*���س�ʽ��ID��
******************************************************************************/
PROGRAM_ID Program_Get_ProgramID(void)
{
    return (PROGRAM_ID)ProgramArray[proCurrentIndex].Programid;
}
/***************************************************************************//**
* @brief
* �˺�������ȡ�õ�ǰ�ĳ�ʽ
* @param[in]
*   none
* @param[out]��
*���س�ʽ�ĵ�ǰ״̬
******************************************************************************/
PROGRAM_STATUS Program_Get_Status(void)
{
    return g_StateMachine;
}
/***************************************************************************//**
* @brief
* �˺����������ó���״̬
* @param[in]
*   prgstatus��״̬���ñ���
* @param[out]��none
******************************************************************************/
void Program_Set_Status(PROGRAM_STATUS prgstatus)
{
    g_StateMachine=prgstatus;
}
/***************************************************************************//**
* @brief
* �˺��������������ⰴ��
* @param[in]
*   vrKey����ʽ���ⰴ��ö�ٱ���
* @param[out]��
*�����Ƿ�ִ��OK
*true��ʾ�ɹ���false��ʾʧ��
******************************************************************************/
VR_KEY Program_Set_VirtualKey(VR_KEY vk)
{
 	VR_KEY TempKey = vk;
	if(TempKey==VK_PROCESSED  ||TempKey == VK_CLEAR_BOOT ) return TempKey;
	
	if(g_StateMachine == PS_IDLE )
	{
		if(TempKey!=VK_START && TempKey!= VK_PAUSE_START && TempKey !=VK_INITIAL)
		return TempKey;
	}
//	if(( TempKey == VK_IRSENSOR_ERROR)||(TempKey == VK_CONTROLZONE_ERROR)||(TempKey == VK_UCBEMERG_ERROR)||(TempKey == VK_NO_SPM_ERROR)||(TempKey == VK_HANDRAILEMERG_ERROR))
    if( TempKey == VK_IRSENSOR_ERROR)
	{
		if(g_StateMachine>PS_WAIT_3S ) 
		{
			key_event.EventBits.Key_SensorError = true ;
            Key_SensorError_Flag = true ;
		}
	}
//    if(( TempKey == VK_IRSENSOR_ERROR_RELEASED)||(TempKey == VK_CONTROLZONE_ERROR_RELEASED)||(TempKey == VK_UCBEMERG_ERROR_RELEASED)||(TempKey == VK_HANDRAILEMERG_ERROR_RELEASED))
	if( TempKey == VK_IRSENSOR_ERROR_RELEASED)
    {
		if(g_StateMachine == PS_PAUSED)
		{
            Key_SensorError_Flag = false ;
		}
	}
	if(TempKey ==VK_INITIAL  )
	{
		Program_Initial_Inner_Data();
		key_event.EventBits.Key_Initial = true ;
		TempKey = VK_PROCESSED;
		return(TempKey);
	}
	if((TempKey == VK_SAFETY_KEY_OFF)||(TempKey == VK_UCBEMERG_ERROR_RELEASED))
    {
          if(g_StateMachine>PS_WAIT_3S ) 
          {
              Data_Set_Event(DATA_SAVEDATA_EVENT);
              ChangeWorkoutState(WIFI_WORKOUT_STOP);
          }
		g_StateMachine=PS_IDLE;				
        if(!DigitalGetRFstatus())Program_Initial_Inner_Data();//debug by ckm
		return(TempKey);		
    }
	if(TempKey ==VK_ENG &&  g_StateMachine<=PS_SETUP)
	{
		key_event.EventBits.Key_EnterEngMode = true ;
		return(TempKey);
	}
	if(TempKey ==VK_WIFI_AP &&  g_StateMachine<=PS_SETUP)
	{
		Program_Initial_Inner_Data();
		return(TempKey);	
	}
		
    if(TempKey == VK_NULL || g_StateMachine==PS_WAIT_3S) return TempKey;
	if(TempKey ==VK_ENTER||TempKey ==VK_ENTER_CHANGEDIS)
	{
		Prg_Mode.Scan=false;
		Prg_isr_TimerColse(TIMER_ScanFunction);
	}
    
    if(TempKey>=VK_NUM0 && TempKey<=VK_NUM9)
    {
        key_event.EventBits.Key_QuickKey=true;
        QuickKey.Data = TempKey - VK_NUM0;
        if(g_StateMachine<=PS_SETUP)
        {
            switch(SettingState)
            {
                case SET_TIME:
                QuickKey.Key.Evt.Time=1;
                break;
                case SET_SPEED:
                QuickKey.Key.Evt.Spd=1;
                break;
                case SET_INCLINE:
                QuickKey.Key.Evt.Inl=1;
                break;
                case SET_RESISTANCE:
                QuickKey.Key.Evt.Res=1;
                break;
                case SET_LEVEL:
                QuickKey.Key.Evt.Level=1;
                break;
                case SET_WATTS:
                QuickKey.Key.Evt.Watt=1;
                break;
            }
        }
        else if(g_StateMachine>=PS_WARMUP &&g_StateMachine<=PS_COOLDOWN)
        {
            if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
            {
                QuickKey.Key.Evt.Spd=1;
            }
            else
            {
                QuickKey.Key.Evt.Res=1;
            }
        }
        
        QuickKey.Counter = 0 ;
    }
    else if(TempKey== VK_ENTER_CHANGEDIS)
    {
        if(g_StateMachine<=PS_SETUP)
        {
            key_event.EventBits.Key_Enter=true;
        }
        else if(g_StateMachine>=PS_COMPLETED && g_StateMachine<=PS_COOLDOWN)
        {
            if(QuickKey.Key.Evt.Spd || QuickKey.Key.Evt.Res)
            {
                key_event.EventBits.Key_Enter=true;
            }
            else
            {
                key_event.EventBits.Key_ChangeDisplay=true;
            }
            
        }
        
    }
    else if(TempKey==VK_ENTER)
    {
        key_event.EventBits.Key_Enter=true;
    }
    else if(TempKey==VK_BACK)
    {
        if(QuickKey.Key.Evts!=0)
        {
            memset(&QuickKey,0,sizeof(QUICK_KEY));
        }
        else
        {
            key_event.EventBits.Key_Back=true;
        }
    }
    else
    {
        memset(&QuickKey,0,sizeof(QUICK_KEY));
        
        switch(TempKey)
        {
            case VK_SPEEDUP_LONG_PRESS:
            if(g_StateMachine>=PS_WARMUP && g_StateMachine<=PS_COOLDOWN)
            {
              if(Get_User_Unit()==METRIC)
              {
                if(ValueLimit<15)
                {
                    ValueLimit++;
                    Data_Set_Speed(Program_Get_Speed(DATA_METRIC,0)+1,DATA_METRIC,1);
                }
              }
              else
              {
                if(ValueLimit<9)
                {
                    ValueLimit++;
                    Data_Set_Speed(Program_Get_Speed(DATA_STANARD,0)+1,STANDARD,1);
                }
                }
            }
            else
            {
                key_event.EventBits.Key_Up=true;
            }
            break;
            case VK_RESISTANCEUP_LONGPRESSED:
            if(g_StateMachine>=PS_WARMUP && g_StateMachine<=PS_COOLDOWN)
            {
                if(ValueLimit<2)
                {
                    ValueLimit++;
                    Data_Set_Resistance(Program_Get_Resistance(0)+1,1);
                }
            }
            else
            {
                key_event.EventBits.Key_Up=true;
            }
            break;
            case VK_SPEEDUP_RELEASE:
            ValueLimit=0;
            break;     
//            case VK_SPEEDUP_RELEASE:
//            ValueLimit=0;
//            break; 
            case VK_UP:
            key_event.EventBits.Key_Up=true;
            break;
            case VK_DOWN:
            key_event.EventBits.Key_Dn=true;
            break;
            case VK_INCLINEUP:
              if(g_StateMachine>=PS_WARMUP && g_StateMachine<=PS_COOLDOWN)
              {
            key_event.EventBits.Key_InclineUp=true;
              }
              else
              {
                key_event.EventBits.Key_Up=true;
              }
            break;
            case VK_INCLINEDOWN:
              if(g_StateMachine>=PS_WARMUP && g_StateMachine<=PS_COOLDOWN)
              {
            key_event.EventBits.Key_InclineDn=true;
              }
              else
              {
                key_event.EventBits.Key_Dn=true;
              }             
            break;
            case VK_SPEEDUP:
              if(g_StateMachine>=PS_WARMUP && g_StateMachine<=PS_COOLDOWN)
              {
            key_event.EventBits.Key_SpeedUp=true;
              }
              else
              {
                key_event.EventBits.Key_Up=true;
              }
            break;
            case VK_SPEEDDOWN:
              if(g_StateMachine>=PS_WARMUP && g_StateMachine<=PS_COOLDOWN)
              {
            key_event.EventBits.Key_SpeedDn=true;
              }
              else
              {
                key_event.EventBits.Key_Dn=true;
              }            
            break;
            case VK_RESISTANCEUP:
              if(g_StateMachine>=PS_WARMUP && g_StateMachine<=PS_COOLDOWN)
              {
            key_event.EventBits.Key_ResistanceUp=true;
              }
              else
              {
                key_event.EventBits.Key_Up=true;
              }            
            break;
            case VK_RESISTANCEDOWN:
              if(g_StateMachine>=PS_WARMUP && g_StateMachine<=PS_COOLDOWN)
              {
            key_event.EventBits.Key_ResistanceDn=true;
              }
              else
              {
                key_event.EventBits.Key_Dn=true;
              }           
            break;
            case VK_PAUSE_START:
            if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==CLIMB_MILL)
                Prg_isr_TimerCounterClear(TIMER_500MS_READ_STATUS);
            if(g_StateMachine <= PS_SETUP)
            {
                if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
                {
                    if(Data_Get_RPM(MOTOR_RPM))break;
                }
                if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==CLIMB_MILL)
                {
                    Clearg_ErrorCode();
                    if((LCB_Get_Data(G_READ_STATUS)&0X0001)==1)break;
                }
//                ChangeWorkoutState(WIFI_WORKOUT_RUNNING);
//                LCB_Send_Cmd(DS_SET_STATUS,0x00);
//                LCB_Send_Cmd(DS_SET_SPM,Data_Get_Resistance(TARGET_ECBCOUNT));
                else
                {
                    LCB_Send_Cmd(DS_INC_LOCATION,Data_Get_Incline(TARGET_INCLINE_ADC));
                    LCB_Send_Cmd(DS_ECB_ZERO,0);
                }
                WorkoutSpeedData=Data_Get_Speed(TARGET_SPEED);
                WorkoutInclineData=Data_Get_Incline(TARGET_INCLINEX10);
                WorkoutResistanceData=Data_Get_Resistance(TARGET_RESISTANCE);
                g_StateMachine=PS_WAIT_3S;
                
//                if(DigitalGetRFstatus())Data_Set_Time(30*60);//by ckm debug
                if(DigitalGetRFstatus())Data_Get_Time(TARGET_TIME);//by zdj change
                if((DigitalGetRFstatus())&&(Data_Get_Time(TARGET_TIME)==0))
                {
                    Data_Set_Time(30*60);
                }
            }
            else if(g_StateMachine==PS_PAUSED)
            {
                if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==CLIMB_MILL)
                {
                    if((LCB_Get_Data(G_READ_STATUS)&0X0001)==1)break;
                    LCB_Send_Cmd(DS_SET_SPM,(Data_Get_Resistance(TARGET_ECBCOUNT))*10);
                    if(Key_SensorError_Flag==1)
//                        return TempKey;
                        break;
                }
                if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
                {
                    if(Data_Get_RPM(MOTOR_RPM))break;
                }
//                ChangeWorkoutState(WIFI_WORKOUT_RUNNING);
                if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)!=CLIMB_MILL)
                {
                    LCB_Send_Cmd(DS_INC_LOCATION,Data_Get_Incline(TARGET_INCLINE_ADC));
                    LCB_Send_Cmd(DS_ECB_LOCATION,Data_Get_Resistance(TARGET_ECBCOUNT));
                    LCB_Send_Cmd(DS_INDUCTION_PWM,Data_Get_Resistance(TARGET_PWM));
                }
                g_StateMachine=PS_WAIT_3S;
//                 Data_Set_Time(30*60);//just debug
            }  
            else if(g_StateMachine<=PS_COOLDOWN && g_StateMachine>=PS_WARMUP)
            {
                Prg_isr_TimerCounterClear(TIMER_5S_PedalStart);
                key_event.EventBits.Key_Pause=true;
                
            }    
            break;
            case VK_START:
            if(g_StateMachine <= PS_SETUP)
            {
                if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
                {
                    if(Data_Get_RPM(MOTOR_RPM))break;
                }
//                ChangeWorkoutState(WIFI_WORKOUT_RUNNING);
                if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)!=CLIMB_MILL)
                {
                    LCB_Send_Cmd(DS_INC_LOCATION,Data_Get_Incline(TARGET_INCLINE_ADC));                
                    LCB_Send_Cmd(DS_ECB_ZERO,0);
                }
                WorkoutSpeedData=Data_Get_Speed(TARGET_SPEED);
                WorkoutInclineData=Data_Get_Incline(TARGET_INCLINEX10);
                WorkoutResistanceData=Data_Get_Resistance(TARGET_RESISTANCE);
                g_StateMachine=PS_WAIT_3S;
                
            }
            else if(g_StateMachine==PS_PAUSED)
            {
                if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
                {
                    if(Data_Get_RPM(MOTOR_RPM))break;
                }
//                ChangeWorkoutState(WIFI_WORKOUT_RUNNING);
                if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)!=CLIMB_MILL)
                {
                    LCB_Send_Cmd(DS_INC_LOCATION,Data_Get_Incline(TARGET_INCLINE_ADC));
                    LCB_Send_Cmd(DS_ECB_LOCATION,Data_Get_Resistance(TARGET_ECBCOUNT));
                    LCB_Send_Cmd(DS_INDUCTION_PWM,Data_Get_Resistance(TARGET_PWM));
                }
                g_StateMachine=PS_WAIT_3S;
                
            }
            break;
            case VK_PAUSE: 
            if(g_StateMachine<=PS_COOLDOWN && g_StateMachine>=PS_WARMUP)
            {
                Prg_isr_TimerCounterClear(TIMER_5S_PedalStart);
                key_event.EventBits.Key_Pause=true;
                if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==CLIMB_MILL)
                    Prg_isr_TimerCounterClear(TIMER_500MS_READ_STATUS);
            }
            break;
            case VK_COOLDOWN: 
            if(g_StateMachine<=PS_COOLDOWN && g_StateMachine>=PS_WARMUP)
            {
                g_StateMachine=PS_COOLDOWN;
                key_event.EventBits.Key_CoolDn=true;
                
            }
            break;
            case VK_RESET: 
            if((g_StateMachine<=PS_COOLDOWN && g_StateMachine>=PS_WARMUP)||g_StateMachine==PS_PAUSED)
        	{
				key_event.EventBits.Key_WorkoutSummary=true;
        	}
			else
			{
			    key_event.EventBits.Key_Reset=true;
			}
            break;
            case VK_CHANGE_DISPLAY: 
            key_event.EventBits.Key_ChangeDisplay=true;
            break;
            case VK_SCAN: 
            Prg_Mode.Scan=true;
			Prg_isr_TimerCounterClear(TIMER_ScanFunction);	
            break;
            case VK_INITIAL: 
            g_StateMachine=PS_IDLE;
            ChangeWorkoutState(WIFI_WORKOUT_STOP);
            if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)!=CLIMB_MILL)
            {
                LCB_Send_Cmd(DS_ECB_ZERO,0);
            }
            key_event.EventBits.Key_Initial=true;
            Program_Initial_Inner_Data();
            break;
			case VK_SPEEDOVER:
			{
				key_event.EventBits.Key_SpeedOver=true;
			}
			break;
            default:
            break;
            
        }
    }
	TempKey = VK_PROCESSED;
	
    return TempKey;
}
/***************************************************************************//**
* @brief
* �˺����������ò���
* @param[in]��
vrKey����������
*p�����õ�ֵ
* @param[out]��none
******************************************************************************/

void Program_Set_Parameter(UINT16 vrKey,void *p)
{
    UINT16 ret=0;
    switch(vrKey)
    {
        case VK_SET_WORKOUT_TIME:
        {
            memcpy(&(ret),p,sizeof(UINT16));
            if(ProgramArray[proCurrentIndex].Programid == PS_MANUAL||ProgramArray[proCurrentIndex].Programid == PS_CONSTANT_WATTS\
                ||ProgramArray[proCurrentIndex].Programid == PS_THR_ZONE||ProgramArray[proCurrentIndex].Programid == PS_INTERVALS_HR||ProgramArray[proCurrentIndex].Programid == PS_STEPS_THR)
            {
                if(ret>0&&ret<5*60)
                    ret=0;
            }
            else
            {
                if(ret<5*60)ret=5*60;
            }   
            if(ret>99*60)
            {
                ret=99*60;
            }
            ret /=60;
            ret *=60;
            Data_Set_Time(ret);
        }
        break;
        case VK_SET_SPEED:
        {
            memcpy(&(ret),p,sizeof(UINT16));
            Data_Set_Speed(ret,Get_User_Unit()==METRIC?METRIC:STANDARD,1);      
        }
        break;
        case VK_SET_INCLINE:
        {
            memcpy(&(ret),p,sizeof(UINT16));
            Data_Set_Incline(ret,1);        
        }
        break;	
        case VK_SET_RESISTANCE:
        {
            memcpy(&(ret),p,sizeof(UINT16));
            Data_Set_Resistance(ret,1);  
        }
        break;
        case VK_SET_WEIGHT:
        {
            memcpy(&(ret),p,sizeof(UINT16));
            Data_Set_Weight(ret);      
        }
        break;	
        case VK_SET_WATTS:
        {
            memcpy(&(ret),p,sizeof(UINT16));
            ret=(ret*2+5)/10*5;//covert to 25,30,...,400
            
            Data_Set_Watt(ret);      
        }
        break;    
        case VK_SET_LEVEL:
        {
            memcpy(&(ret),p,sizeof(UINT16));
            Data_Set_Level(ret);      
        }
        break;    
        default:break;
    }  
}

/***************************************************************************//**
* @brief
* �˺�������ȡ��ʱ��
* @param[in]��
* @param[out]������ʱ��
******************************************************************************/
UINT16 Program_Get_Time(UINT8 Data_Mode)
{
    UINT16 w_Dat=0;
    if(g_StateMachine<=PS_SETUP)
    {
        if(QuickKey.Key.Evt.Time==1 && Data_Mode==QUICKKEY_NORMAL)
        {
            w_Dat=QuickKey.Value*60;
        }
        else
        {
            switch(SettingState)
            {
                case SET_TIME:
                  if(ProgramArray[proCurrentIndex].Programid == PS_THR_ZONE)
                  {
                    w_Dat=Data_Get_Time(WORKOUT_TIME);
                  }
                  else
                  {
                    w_Dat=Data_Get_Time(TARGET_TIME);
                  }
                break;
                case SET_WORKINTERVAL_TIME:
                w_Dat=PHrc_Get_Time(0);
                break;
                case SET_RESTINTERVAL_TIME:
                w_Dat=PHrc_Get_Time(1);
                break;
                case SET_SEGMENT_TIME:
                w_Dat=PHrc_Get_Time(0);
                break;
                default:
                w_Dat=Data_Get_Time(TARGET_TIME);
                break;
            }
        }
    }
    else if(g_StateMachine == PS_COMPLETED)
    {
      w_Dat=Data_Get_Time(ELAPSED_TIME);
    }
    else
    {
      if(ProgramArray[proCurrentIndex].Programid == PS_SPRINT8)
      {
        w_Dat=Data_Get_Time(STAGE_TIME)-Data_Get_Time(TIME_IN_STAGE);
      }
      else if(ProgramArray[proCurrentIndex].Programid == PS_THR_ZONE)
      {
        if(Data_Get_Time(ELAPSED_TIME)<Data_Get_Time(WARMUP_TIME))
        {
          w_Dat=Data_Get_Time(WARMUP_TIME)-Data_Get_Time(ELAPSED_TIME);
        }
        else if(Data_Get_Time(REMAINING_TIME)<=Data_Get_Time(COOLDOWN_TIME))
        {
          w_Dat=Data_Get_Time(REMAINING_TIME);
        }
        else
        {
          w_Dat=Data_Get_Time(WORKOUT_TIME)+Data_Get_Time(WARMUP_TIME)-Data_Get_Time(ELAPSED_TIME);
        }
      }
    else
    {
        if(Data_Get_Time(TARGET_TIME))
        {
            w_Dat=Data_Get_Time(REMAINING_TIME);
        }
        else
        {
            w_Dat=Data_Get_Time(ELAPSED_TIME);
        }
        }
    }
    return w_Dat;
}
/***************************************************************************//**
* @brief
* �˺�������ȡ��Ŀ��ʱ��
* @param[in]
*   none
* @param[out]��
*����Ŀ��ʱ��
******************************************************************************/

UINT16 Program_Get_WorkoutTime(void)
{
    return Data_Get_Time(TARGET_TIME);
}
/***************************************************************************//**
* @brief
* �˺�������ȡ�õ�ǰʱ��
* @param[in]
*   none
* @param[out]��
*���ص�ǰʱ��
******************************************************************************/
UINT16 Program_Get_WorkoutTime_Elapsed(void)
{
    return Data_Get_Time(ELAPSED_TIME);
}
/***************************************************************************//**
* @brief
* �˺�������ȡ��ʣ��ʱ��
* @param[in]
*   none
* @param[out]��
*����ʣ��ʱ��
******************************************************************************/
UINT16 Program_Get_WorkoutTime_Remaining(void)
{
    return Data_Get_Time(REMAINING_TIME);
}
/***************************************************************************//**
* @brief
* �˺�������ȡ�������˶�ʱ��
* @param[in]
*   none
* @param[out]��
*���������˶�ʱ��
******************************************************************************/

UINT8 Program_Get_WorkoutTime_Warmup(void)
{
    return Data_Get_Time(WARMUP_TIME);
}
/***************************************************************************//**
* @brief
* �˺�������ȡ�û����˶�ʱ��
* @param[in]
*   none
* @param[out]��
*���ػ����˶�ʱ��
******************************************************************************/

UINT8 Program_Get_WorkoutTime_Cooldown(void)
{
    return Data_Get_Time(COOLDOWN_TIME);   
}
/***************************************************************************//**
* @brief
* �˺�������ȡ�õ�ǰ�˶��Ķ���
* @param[in]
*   none
* @param[out]��
*���ص�ǰ�˶��Ķ���
******************************************************************************/

UINT8 Program_Get_Stage(void)
{
    return Data_Get_Time(STAGE);   
}
/***************************************************************************//**
* @brief
* �˺�������ȡ�õ�ǰ�ε�ǰ�˶�ʱ��
* @param[in]
*   none
* @param[out]��
*���ص�ǰ�ε�ǰ�˶�ʱ��
******************************************************************************/

UINT16 Program_Get_timeInStage(void)
{
    return Data_Get_Time(TIME_IN_STAGE);   
}
/***************************************************************************//**
* @brief
* �˺�������ȡ�õ�ǰ�����˶�ʱ��
* @param[in]
*   none
* @param[out]��
*���ص�ǰ�����˶�ʱ��
******************************************************************************/

UINT16 Program_Get_stageTime(void)
{
    return Data_Get_Time(STAGE_TIME);   
}
/***************************************************************************//**
* @brief
* �˺�������ȡ���ٶ�ֵ
* @param[in]
*   type��	
DATA_STANARD��ȡ��Ӣ���ٶ�
DATA_METRIC��ȡ�ù����ٶ�
DATA_AUTO��ȡ�õ�ǰ�ٶȣ�����Ŀǰϵͳ��Ӣ�Ʒ���ֵ
*pIsBlinking���ݲ���
* @param[out]��
*�����ٶ�ֵ
******************************************************************************/

UINT16 Program_Get_Speed(DATA_TYPE type,UINT8 Data_Mode)
{
    UINT32 ret;
    
    if(QuickKey.Key.Evt.Spd==1 && Data_Mode==QUICKKEY_NORMAL)
    {
        ret=QuickKey.Value*100; 
    }
    else
    {
        if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
        {
            ret=Data_Get_Speed(TARGET_SPEED); 
        }
        else
        {
            ret =Data_Get_Speed(EPBI_SPEED); 
        }
        if(type==DATA_METRIC)
        {
            ret=ret*201168/125000+2;//convert to Metric
        }
        else if(type==DATA_AUTO)
        {
            if(Get_User_Unit()==METRIC)
            {
                ret=ret*201168/125000+2;//convert to Metric
            }
        }
    }
    return (UINT16)(ret/100);
}
/***************************************************************************//**
* @brief
* �˺�������ȡ��ƽ���ٶ�ֵ
* @param[in]
*   type��	
DATA_STANARD��ȡ��Ӣ��ƽ���ٶ�
DATA_METRIC��ȡ�ù���ƽ���ٶ�
DATA_AUTO��ȡ�õ�ǰƽ���ٶȣ�����Ŀǰϵͳ��Ӣ�Ʒ���ֵ
* @param[out]��
*����ƽ���ٶ�ֵ
******************************************************************************/

UINT16 Program_Get_Speed_Average(DATA_TYPE type)
{
    UINT32 ret;
    ret=Data_Get_Speed(AVERAGE_SPEED); 
    
    if(type==DATA_METRIC)
    {
        ret=ret*201168/125000+2;//convert to Metric
    }
    else if(type==DATA_AUTO)
    {
        if(Get_User_Unit()==METRIC)
        {
            ret=ret*201168/125000+2;//convert to Metric
        }
    }
    return ret;  
}
/***************************************************************************//**
* @brief
* �˺�������ȡ������ֵ
* @param[in]
*pIsBlinking���ݲ���
* @param[out]��
*��������ֵ
******************************************************************************/

UINT8 Program_Get_Incline(UINT8 Data_Mode)
{
    if(QuickKey.Key.Evt.Inl==1 &&Data_Mode==QUICKKEY_NORMAL)
    {
        return ((UINT8)QuickKey.Value);
    }
    else
    {
        return ((UINT8)Data_Get_Incline(TARGET_INCLINEX10));  
    }  
}
/***************************************************************************//**
* @brief
* �˺�������ȡ��ƽ������ֵ
* @param[in]
*none
* @param[out]��
*����ƽ������ֵ
******************************************************************************/

UINT8 Program_Get_Incline_Average(void)
{
    return ((UINT8)Data_Get_Incline(AVERAGE_INCLINEX10));    
}
/***************************************************************************//**
* @brief
* �˺�������ȡ�õ�ǰ����ADCֵ
* @param[in]
*none
* @param[out]��
*���ص�ǰ����ADCֵ
******************************************************************************/
UINT16 Program_Get_Current_Incline_ADC(void)
{
    return (Data_Get_Incline(REAL_INCLINE_ADC));    
}
/***************************************************************************//**
* @brief
* �˺�������ȡ������ֵ
* @param[in]
*pIsBlinking���ݲ���
* @param[out]��
*��������ֵ
******************************************************************************/

UINT8 Program_Get_Resistance(UINT8 Data_Mode)
{
    if(QuickKey.Key.Evt.Res==1 &&Data_Mode==QUICKKEY_NORMAL)
    {
        return ((UINT8)QuickKey.Value);
    }
    else
    { 
        return ((UINT8)Data_Get_Resistance(TARGET_RESISTANCE));   
    }  
}
/***************************************************************************//**
* @brief
* �˺�������ȡ��ƽ������ֵ
* @param[in]
* @param[out]��
*����ƽ������ֵ
******************************************************************************/

UINT8 Program_Get_Resistance_Average(void)
{
    return ((UINT8)Data_Get_Resistance(AVERAGE_RESISTANCE));   
}
/***************************************************************************//**
* @brief
* �˺�������ȡ��Levelֵ
* @param[in]
*pIsBlinking���ݲ���
* @param[out]��
*����Levelֵ
******************************************************************************/
UINT8 Program_Get_WorkoutLevel(UINT8 Data_Mode)
{
    if(QuickKey.Key.Evt.Level==1 || (SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==CLIMB_MILL && QuickKey.Key.Evt.Res)\
        && Data_Mode==QUICKKEY_NORMAL )
    {
        return ((UINT8)QuickKey.Value);
    }
    else
    {
        return ((UINT8)Data_Get_Level(TARGET_LEVEL));   
    }
}

UINT16 Program_Get_Stairs()
{
    UINT16 ret ;
    ret = Data_Get_Stairs(ELAPSED_STAIRS);
    return ret ;
}
UINT16 Program_Get_Height()
{
    UINT16 ret ;
    ret = Data_Get_Stairs(ELAPSED_HEIGHT);
    return ret ;
}
UINT16 Program_Get_Height_Percent()
{
    UINT16 ret ;
    ret = Data_Get_Stairs(ELAPSED_HEIGHT)*100/Data_Get_Stairs(TARGET_HEIGHT);
    return ret ;
}
/***************************************************************************//**
* @brief
* �˺�������ȡ�õ�ǰ����
* @param[in]
*   type��	
DATA_STANARD��ȡ��Ӣ�ƾ���
DATA_METRIC��ȡ�ù��ƾ���
DATA_AUTO��ȡ�õ�ǰ���룬����Ŀǰϵͳ��Ӣ�Ʒ���ֵ
*pIsBlinking���ݲ���
* @param[out]��
*���ص�ǰ����ֵ
******************************************************************************/

UINT16 Program_Get_Distance(DATA_TYPE type)
{
    UINT32 ret=Data_Get_Distance(ELAPSED_DISTANCE);
    if(DATA_METRIC==type)
    {
        ret=(unsigned long long)ret*201168/125000+2;//convert to Metric
    }
    else if( DATA_AUTO == type)
    {
        if( Get_User_Unit() ==  METRIC )
        {
            ret = (unsigned long long)ret*201168/125000+2;//convert to Metric
        }
    }
    
    return ret/10;
}
/***************************************************************************//**
* @brief
* �˺�������ȡ��ʣ�����
* @param[in]
*   type��	
DATA_STANARD��ȡ��Ӣ�ƾ���
DATA_METRIC��ȡ�ù��ƾ���
DATA_AUTO��ȡ�õ�ǰ���룬����Ŀǰϵͳ��Ӣ�Ʒ���ֵ
* @param[out]��
*����ʣ�����ֵ
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
        if( Get_User_Unit() ==  METRIC )
        {
            ret = (unsigned long long)ret*201168/125000;//convert to Metric
            
        }
    }
    return ret/10;
}
/***************************************************************************//**
* @brief
* �˺�������ȡ�õ�ǰ��·��
* @param[in]
*   isFourDigits��
false���������ֵΪ999
true���������ֵΪ9999
* @param[out]��
*���ص�ǰ��·��
******************************************************************************/

UINT16 Program_Get_Calories(bool isFourDigits)
{
    UINT16 by_Dat; 
    by_Dat=Data_Get_Calories(ELAPSED_CALORIES);
    if(isFourDigits==false)
    {
        if(by_Dat>999)
            by_Dat=999;      
    }
    return by_Dat; 
}
/***************************************************************************//**
* @brief
* �˺�������ȡ��ʣ�࿨·��
* @param[in]
*   isFourDigits��
false���������ֵΪ999
true���������ֵΪ9999
* @param[out]��
*����ʣ�࿨·��
******************************************************************************/

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
* �˺�������ȡ�õ���λ��
* @param[in]��None
* @param[out]��
*���ص���ǰλ��
******************************************************************************/
UINT16 Program_Get_DotmatixPos(void)
{
    return Profile_Pos;    
}
/***************************************************************************//**
* @brief
* �˺�������ȡ��Pace
* @param[in]��None
* @param[out]��
*����Pace
******************************************************************************/

UINT16 Program_Get_Pace(void)
{
    UINT16 w_Dat=0;
    w_Dat=36000/Program_Get_Speed(DATA_AUTO,0);//s/mile or s/km
    return w_Dat;
}
/***************************************************************************//**
* @brief
* �˺�������ȡ��Average Pace
* @param[in]��None
* @param[out]��
*����Average Pace
******************************************************************************/

UINT16 Program_Get_Pace_Average(void)
{
    UINT16 w_Dat=0;
    w_Dat=36000/Program_Get_Speed_Average(DATA_AUTO);
    return w_Dat;
}
/***************************************************************************//**
* @brief
* �˺�������ȡ������
* @param[in]��
hrTag��������������Ŀ�꣬��ǰ��ƽ���������Զ���ֵ
* @param[out]��
*��������ֵ
******************************************************************************/

UINT8 Program_Get_HR(HR_TAG hrTag)
{
    UINT8 by_Dat=0;
    if(hrTag==TARGET_HR)
    {
        by_Dat=PHrc_Get_HRTarget();
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
        if(g_StateMachine<=PS_SETUP)
        {
            switch(SettingState)
            {
                case SET_HEART:
                by_Dat=PHrc_Get_Heart(0);
                break;  
                case SET_WORKINTERVAL_HEART:
                by_Dat=PHrc_Get_Heart(0);
                break;
                case SET_RESTINTERVAL_HEART:
                by_Dat=PHrc_Get_Heart(1);
                break;
                case SET_SEGMENT_1_HEART:
                by_Dat=PHrc_Get_Heart(0);
                break;
                case SET_SEGMENT_2_HEART:
                by_Dat=PHrc_Get_Heart(1);
                break;   
                case SET_SEGMENT_3_HEART:
                by_Dat=PHrc_Get_Heart(2);
                break;
                case SET_SEGMENT_4_HEART:
                by_Dat=PHrc_Get_Heart(3);
                break;  
                default:
                by_Dat=PHrc_Get_HRTarget();
                break;
            }
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
* �˺�������ȡ��Wattsֵ
* @param[in]
watts����������Ŀ�꣬��ǰֵ
*pIsBlinking���ݲ���
* @param[out]��
*����Wattsֵ
******************************************************************************/

UINT16 Program_Get_Watts(WATTS_TAG watts, UINT8 Data_Mode)
{
    UINT16 w_Dat=0;
    if(QuickKey.Key.Evt.Watt==1 && Data_Mode==QUICKKEY_NORMAL)
    {
        return QuickKey.Value;
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
            if(ProgramArray[proCurrentIndex].Programid==PS_CONSTANT_WATTS)
                w_Dat=Data_Get_Watts(TARGET_WATT);
            else
            {
			    if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
			    {
			        w_Dat=Data_Get_Watts(CURRENT_WATT);
			    }
			    else
			    {
                    if(Program_Get_Status()!=PS_PAUSED)
                        w_Dat=Data_Get_Watts(CURRENT_WATT);
					else
                        w_Dat=0;
                    
                }							
            }
        }
    }
    
    return w_Dat;
}
/***************************************************************************//**
* @brief
* �˺�������ȡ��METsֵ
* @param[in]��none
* @param[out]��
*����METsֵ
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
* �˺�������ȡ�õ�ǰStepsֵ
* @param[in]��none
* @param[out]��
*���ص�ǰStepsֵ
******************************************************************************/

UINT16 Program_Get_Steps(void)
{
    return Data_Get_Steps(ELAPSED_STEPS);
}
/***************************************************************************//**
* @brief
* �˺�������ȡ��ʣ��Stepsֵ
* @param[in]��none
* @param[out]��
*����ʣ��Stepsֵ
******************************************************************************/

UINT16 Program_Get_Steps_Remaining(void)
{
    return Data_Get_Steps(REMAINING_STEPS);
}
/***************************************************************************//**
* @brief
* �˺�������ȡ��RPMֵ
* @param[in]��none
* @param[out]��
*����RPMֵ
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
* �˺�������ȡ��VO2Maxֵ
* @param[in]��none
* @param[out]��
*����VO2Maxֵ
******************************************************************************/

UINT16 Program_Get_VO2Max(void)
{
    return Data_Get_VO2Max();
}

/***************************************************************************//**
* @brief
* �˺�������ȡ�õ�������
* @param[in]��none
* @param[out]��
*���ص�������
******************************************************************************/

PROFILE* Program_Get_Profile(PROFILE_TAG index)
{
    
    return ProgramArray[proCurrentIndex].GetProfile(index,&Profile_Pos);
}
/***************************************************************************//**
* @brief
* �˺����������ü��ٱ���
* @param[in]��times�����ٱ���
* @param[out]��none
******************************************************************************/

void Program_Set_nXTime(UINT8 times)
{
    Data_Set_Nx(times);
}
/***************************************************************************//**
* @brief
* �˺�������ȡ�õ�ǰ����
* @param[in]
*   type��	
DATA_STANARD��ȡ��Ӣ��
DATA_METRIC��ȡ�ù���
DATA_AUTO��ȡ�õ�ǰ������Ŀǰϵͳ��Ӣ�Ʒ���ֵ
* @param[out]��
*���ص�ǰ����ֵ
******************************************************************************/
UINT16 Program_Get_Weight(DATA_TYPE type)
{
    UINT32 ret=Data_Get_UserData(USER_WEIGHT);//value is X100
    if(type==DATA_METRIC)
    {
        ret=ret*215/474+1;//convert to kg
    }
    else if(type==DATA_AUTO)
    {
        if(Get_User_Unit()==METRIC)
        {
            ret=ret*215/474+1;//convert to kg
        }
    }
    return ret/100;//Return real value
}
/***************************************************************************//**
* @brief
* �˺�������ȡ�õ�ǰ�û�
* @param[in]��none
* @param[out]��
*���ص�ǰ�û�
******************************************************************************/
UINT8 Program_Get_User(void)
{
    return Data_Get_UserData(USER_SLOT);
}
/***************************************************************************//**
* @brief
* �˺�������ȡ��Ŀ���ʽ��Ŀ��ֵ
* @param[in]��none
* @param[out]��
*����Ŀ���ʽ��Ŀ��ֵ
******************************************************************************/

UINT16 Program_Get_Goal(void)
{
    return 0;
}
/***************************************************************************//**
* @brief
* �˺�������ȡ��Ŀ���ʽ������
* @param[in]��none
* @param[out]��
*����Ŀ���ʽ������
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
            retTypes = GOAT_TYPES_CALORIES;
        }
        break;
    }
    return retTypes;
}
/***************************************************************************//**
* @brief
* �˺�������ȡ��Ŀ���ʽ��ʣ��ֵ
* @param[in]��none
* @param[out]��
*����Ŀ���ʽ��ʣ��ֵ
******************************************************************************/
UINT16 Program_Get_Goal_Remaining(void)
{
    UINT16 ret = 0;
    
    return ret;
}
/***************************************************************************//**
* @brief
* �˺�������ȡ�ó�ʽ����������
* @param[in]��none
* @param[out]��
*���س�ʽ����������ö��
******************************************************************************/

//SETTING_ENUM Program_Get_SettingFlow(void)
//{
//  Setting_Chart=ProgramArray[proCurrentIndex].Setup(Program_Get_KeyEvent());
//  return Setting_Chart;
//}
/***************************************************************************//**
* @brief
* �˺�������Main.c����
* @param[in]��none
* @param[out]��
*�����¼�
******************************************************************************/

PROGRAM_PENDING_EVTS Program_Process(ReturnEventHandler flag)
{	
	PROGRAM_PENDING_EVTS pendingEvt;
    pendingEvt.evt=PROGRAM_NONE_EVENT;
    pendingEvt.WorkOutEvts.AllEvts=0;
		
	if(flag < RET_PROCESS_DISPLAY)
		return pendingEvt;
	if(flag == RET_REFRESH_DISPLAY && g_StateMachine!=PS_IDLE)
	{
		return g_PRGPreEvent;
	}
	if(key_event.EventBits.Key_EnterEngMode == 1)
	{
		pendingEvt.evt = PROGRAM_EVENT_ENTER_ENGMODE;
		
		return pendingEvt;
	}

  
    if(SystemConfig_Get_Char(DRIVE_TYPE_UINT8)==BIKE_0C_JIS || SystemConfig_Get_Char(DRIVE_TYPE_UINT8)==INDUCTOR_BRAKE_INCLINE)
    {
      if(Prg_Mode.ReadPWM)
      {
        Prg_Mode.ReadPWM=0;
        LCB_Get_Induct(InductionOutput_5P);
//        InductionOutput_5P[0]=410;
//        InductionOutput_5P[2]=860;
//        InductionOutput_5P[4]=1238;
        if(InductionOutput_5P[0]<InductionOutput_5P[2] && InductionOutput_5P[2]<InductionOutput_5P[4])
        {
          int by_Dat=0;
          unsigned int w_K=0;
          unsigned int w_B=0;
          char by_Loop=0;
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
            InductionOutput_30P[i]=_Watts_Talbelist->PWM_ECBCount_Table[i];
        }
        if(SystemConfig_Get_Char(DRIVE_TYPE_UINT8)==BIKE_0C_JIS)
        {
           for(char i=0;i<30;i++)
            InductionOutput_30P[i] +=aby_pwm_Offset[i];
        }
      }
      if(ProgramArray[proCurrentIndex].Programid != PS_CONSTANT_WATTS)
        Data_Set_TargetPWMValue(InductionOutput_30P[Data_Get_Resistance(TARGET_RESISTANCE)-SystemConfig_Get_Char(MIN_RESISTANCE_UINT8)]);
    }
    switch(g_StateMachine)
    {
        case PS_IDLE:  //select id
        if(key_event.EventBits.Key_Initial)
        {
            pendingEvt.evt = PROGRAM_INITIAL_EVENT;
            g_StateMachine = PS_SELECTPRG;
        }
        if(key_event.EventBits.Key_Enter)
        {
            pendingEvt.evt = PROGRAM_CHANGE_EVENT;
            g_StateMachine = PS_SELECTPRG;
        }
//        key_event.All_KeyEvents=0;
        break;
        case PS_SELECTPRG:
        if(key_event.EventBits.Key_Up)
        {
            Program_Add_Progam();
            pendingEvt.evt=PROGRAM_CHANGE_EVENT;
            pendingEvt.WorkOutEvts.AllEvts=0;           
        }
        else if(key_event.EventBits.Key_Dn)
        {
            Program_Dec_Progam();      
            pendingEvt.evt=PROGRAM_CHANGE_EVENT;
            pendingEvt.WorkOutEvts.AllEvts=0;
        }
        else if(key_event.EventBits.Key_Back)
        {
            pendingEvt.evt = BACK_END_EVENT;
		    pendingEvt.WorkOutEvts.AllEvts=0;	
            g_StateMachine = PS_IDLE;
            Program_Initial_Inner_Data();
        }		
        else if(key_event.EventBits.Key_Enter)
        {
            pendingEvt = ProgramArray[proCurrentIndex].Setup(&key_event);
            g_StateMachine = PS_SETUP;
        }
//        key_event.All_KeyEvents=0;
        
        break;
        case PS_SETUP:
        if(key_event.EventBits.Key_QuickKey)
        {
            key_event.EventBits.Key_QuickKey=false;
            for(char i=0;i<SETPARANUM;i++)
			{
                if(SettingState==DefaultNumber[i].SetState)
                {
                    if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==CLIMB_MILL && SettingState == SET_RESISTANCE )
                        pendingEvt.evt = CLIMBMILL_LEVEL_CHANGE_EVENT ;
                    else
                        pendingEvt.evt=DefaultNumber[i].EventChange;
                    if(DefaultNumber[i].NumericKey.Used==1)
                    {
                        if(QuickKey.Index<DefaultNumber[i].NumericKey.Digits)
                        {
                            QuickKey.Index++;
                            if(QuickKey.Value!=0)
                                QuickKey.Value *=10;
                            QuickKey.Value +=QuickKey.Data;
                        }
                    }
                }
			}
        }
        else if(key_event.EventBits.Key_Enter)
        {
            if(QuickKey.Key.Evts!=0)
            {						
                switch(SettingState)
					
                {
                    case SET_TIME:
                      if(QuickKey.Value>=5 && QuickKey.Value<=99)
                      {
                        if(ProgramArray[proCurrentIndex].Programid==PS_THR_ZONE)
                          Data_Set_WorkOutTime(QuickKey.Value * 60);
                        else
                          Data_Set_Time(QuickKey.Value * 60);
                      }
                      else
                      {
                        key_event.EventBits.Key_Enter=false;
                      }
                      break;
                    case SET_SPEED:
                    if(Get_User_Unit()==METRIC)
					{
                        if(QuickKey.Value <= SystemConfig_Get_Int(MAX_KPHX10_UINT16)&&QuickKey.Value >= SystemConfig_Get_Int(MIN_KPHX10_UINT16))
						{
                            Data_Set_Speed(QuickKey.Value,METRIC,1);
						}
						else
						{
                            key_event.EventBits.Key_Enter=false;
						}
					}
					
					else
					{
                        if(QuickKey.Value <= SystemConfig_Get_Int(MAX_MPHX10_UINT16)&&QuickKey.Value >= SystemConfig_Get_Int(MIN_MPHX10_UINT16))
						{
                            Data_Set_Speed(QuickKey.Value,STANDARD,1);
                        }
                        
                        else
                        {
                            key_event.EventBits.Key_Enter=false;
                        }
                    }
                    break;
                    case SET_INCLINE:
					
                    if(QuickKey.Value%5==0 && QuickKey.Value <= SystemConfig_Get_Int(MAX_INCLINEX10_UINT16))
					{
                        Data_Set_Incline(QuickKey.Value,1);
                    }
					else
					{
                        key_event.EventBits.Key_Enter=false;
					}
                    break;
                    case SET_RESISTANCE:
                    if(QuickKey.Value >= SystemConfig_Get_Char(MIN_RESISTANCE_UINT8) && QuickKey.Value <= SystemConfig_Get_Char(MAX_RESISTANCE_UINT8))
					{
                        Data_Set_Resistance(QuickKey.Value,1);
                    }
					else
					{
                        key_event.EventBits.Key_Enter=false;
					}
                    break;
                    case SET_LEVEL:
                    if(QuickKey.Value >= Data_Get_Level(MIN_LEVEL) && QuickKey.Value <= Data_Get_Level(MAX_LEVEL))
					{
                        Data_Set_Level(QuickKey.Value);
                    }
                    else
                    {
                        key_event.EventBits.Key_Enter=false;
                    }
                    break;
                    case SET_WATTS:
                    if(QuickKey.Value%5==0 && QuickKey.Value >= 25 && QuickKey.Value <= 400)
                    {
                        Data_Set_Watt(QuickKey.Value);
					}
					else
					{
                        key_event.EventBits.Key_Enter=false;
					}
                    break;
                }
                memset(&QuickKey,0,sizeof(QUICK_KEY));
                if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==CLIMB_MILL && SettingState == SET_RESISTANCE )
                    pendingEvt.evt = CLIMBMILL_LEVEL_CHANGE_EVENT ;
                else
                    pendingEvt = ProgramArray[proCurrentIndex].Setup(&key_event);
            }
            else
                pendingEvt = ProgramArray[proCurrentIndex].Setup(&key_event);
        }
        else
            pendingEvt = ProgramArray[proCurrentIndex].Setup(&key_event);
        break;
        case PS_WAIT_3S:
        if(Watit3s.Event)
        {
            ChangeWorkoutState(WIFI_WORKOUT_RUNNING);// PG2 DELAY 
            Watit3s.Event=0;
            if(Watit3s.Count==0)
            {
                pendingEvt.evt= THREE_EVENT;
            }
            else if(Watit3s.Count==1)
            {
                pendingEvt.evt= TWO_EVENT;
                
            }
            else if(Watit3s.Count==2)
            {
                pendingEvt.evt= ONE_EVENT;
            }
            else if(Watit3s.Count==3)
            {
                if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==CLIMB_MILL)
                {
                    LCB_Send_Cmd(DS_SET_STATUS,0x00);
                    LCB_Send_Cmd(DS_SET_SPM,Data_Get_Resistance(TARGET_ECBCOUNT)*10);
                }
                else 
                {
                    LCB_Send_Cmd(DS_MOT_RPM,Data_Get_RPM(MIN_RPM)*10);
                }
                Prg_isr_TimerCounterClear(TIMER_20S_No_RPM);
                Watit3s.Counter=0;
                Watit3s.Count=0;
                Watit3s.Event=1;
                pendingEvt.evt= PROGRAM_RUN_EVENT;
                Data_Set_Event(DATA_RUN_EVENT);
                ChangeWorkoutState(WIFI_WORKOUT_RUNNING);
                g_StateMachine=PS_WARMUP;
            }       
        }
//        key_event.All_KeyEvents=0;
        
        break;
        case PS_OVERSPEEDSTOP:
		{
			UINT8 _isIncZero = 0;
			UINT16 _realtimeIncADC = LCB_Get_Data(G_INC_LOCATION);
			UINT16 _minIncADC = SystemConfig_Get_Int(MIN_INLCINEADC_UINT16);
			if((SystemConfig_Get_Char(MACHINE_MODLE_UINT8)==4)||(SystemConfig_Get_Char(MACHINE_MODLE_UINT8)==5))
            {
                LCB_Initial();
                LCB_Send_Cmd(DS_INITIAL,0);
                LCB_Send_Cmd(DS_WORK_STATUS,0);
                LCB_Send_Cmd(DS_WORK_STATUS,0x01);
                LCB_Send_Cmd(DS_INC_LOCATION,Data_Get_Incline(MIN_INCLINEADC));
//                Program_Initial_Inner_Data();
//                pendingEvt.WorkOutEvts.AllEvts=0;
                T75_Overspeed_Flag = 1;
                if(ShowOverspeedStringTime>=30)
                {
                    ShowOverspeedStringTime = 0;
                    T75_Overspeed_Flag = 0;
                    g_StateMachine=PS_COMPLETED;
                    Prg_isr_TimerCounterClear(TIMER_WORKOUTCOMPLETE);
                    pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
                    pendingEvt.WorkOutEvts.EvtBit.Workout_Complete_Evt=true;
                }
//                T75_Overspeed_Flag = 1;
            }
            else
            {
                if( _realtimeIncADC > _minIncADC && _realtimeIncADC -_minIncADC <  INCLINE_ZERO_SHIFT  ||_realtimeIncADC <= _minIncADC)
                {
                    _isIncZero = 1 ;
                }
                if( LCB_Get_Data(G_MOTOR_RPM) == 0  && 1 == _isIncZero  )
                {
                    g_StateMachine=PS_COMPLETED;
                    Prg_isr_TimerCounterClear(TIMER_WORKOUTCOMPLETE);
                    pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
                    pendingEvt.WorkOutEvts.EvtBit.Workout_Complete_Evt=true;
    //			    if(SystemConfig_Get_Char(MACHINE_MODLE_UINT8)==6)
    //                {
    //                    LCB_Initial();
    //                }
                }	
            }
//            if( LCB_Get_Data(G_MOTOR_RPM) == 0/*  && 1 == _isIncZero  */)
//			{
//		        LCB_Initial();
////		        ChangeWorkoutState(WIFI_WORKOUT_STOP);
//                LCB_Send_Cmd(DS_INITIAL,0);
//                LCB_Send_Cmd(DS_WORK_STATUS,0);
//                LCB_Send_Cmd(DS_WORK_STATUS,0x01);
//                LCB_Send_Cmd(DS_INC_LOCATION,Data_Get_Incline(MIN_INCLINEADC));
////				g_StateMachine=PS_COMPLETED;
////				Prg_isr_TimerCounterClear(TIMER_WORKOUTCOMPLETE);
//                Program_Initial_Inner_Data();
////				pendingEvt.evt=MOTOR_WORKOUTINIT_LCB_EVENT;
//				pendingEvt.WorkOutEvts.AllEvts=0;
//			}
		}
		break;
        case PS_WARMUP:
        case PS_RUN:
        case PS_COOLDOWN:
        {
            pendingEvt =  ProgramArray[proCurrentIndex].Workout(&key_event);
            if(key_event.EventBits.Key_WorkoutSummary || key_event.EventBits.Key_SpeedOver )
            {
                ChangeWorkoutState(WIFI_WORKOUT_STOP);
                Data_Set_Event(DATA_PAUSE_EVENT);
                if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)!=CLIMB_MILL)
                {
                    LCB_Send_Cmd(DS_INC_LOCATION,Data_Get_Incline(MIN_INCLINEADC));
                    LCB_Send_Cmd(DS_ECB_ZERO,0);
                    LCB_Send_Cmd(DS_MOT_RPM,0);
                    LCB_Send_Cmd(DS_INDUCTION_PWM,0);
                }
                if(key_event.EventBits.Key_WorkoutSummary)
                {
                    g_StateMachine=PS_COMPLETED;
                    Prg_isr_TimerCounterClear(TIMER_WORKOUTCOMPLETE);
                    pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
                    pendingEvt.WorkOutEvts.EvtBit.Workout_Complete_Evt=true;
                }
                else
                {
                    g_StateMachine=PS_OVERSPEEDSTOP;
                    pendingEvt.evt=MOTOR_SPEED_OVER_EVENT;
                    key_event.All_KeyEvents=0;
                    QuickKey.Key.Evts  = 0 ;
                    memset(&Prg_Mode,0,sizeof(Prg_Mode));
                }
            }
            
            if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
            {
                if(WorkoutSpeedData!=Data_Get_Speed(TARGET_SPEED))
                {
                    WorkoutSpeedData=Data_Get_Speed(TARGET_SPEED);
                    pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
                    pendingEvt.WorkOutEvts.EvtBit.Workout_Speed_Evt=true;
                }
            }
            else
            {
                if(WorkoutResistanceData!=Data_Get_Resistance(TARGET_RESISTANCE))
                {
                    WorkoutResistanceData=Data_Get_Resistance(TARGET_RESISTANCE);
                    pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
                    if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==CLIMB_MILL)
                        pendingEvt.WorkOutEvts.EvtBit.Workout_Climbmill_ResChange=true;
                    else
                        pendingEvt.WorkOutEvts.EvtBit.Workout_Resistance_Evt=true;
                }
            }
            if(WorkoutInclineData!=Data_Get_Incline(TARGET_INCLINEX10))
            {
                WorkoutInclineData=Data_Get_Incline(TARGET_INCLINEX10);
                pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
                pendingEvt.WorkOutEvts.EvtBit.Workout_Incline_Evt=true;
            }
            
            if(key_event.EventBits.Key_Enter)
            {
                if(QuickKey.Key.Evt.Spd || QuickKey.Key.Evt.Res)
                {
                    if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
                    {
                        if(Get_User_Unit()==METRIC)
                        {
                            if(QuickKey.Value <= SystemConfig_Get_Int(MAX_KPHX10_UINT16)&&QuickKey.Value >= SystemConfig_Get_Int(MIN_KPHX10_UINT16))
                            {
                                Data_Set_Speed(QuickKey.Value,METRIC,1);
                            }
                        }
                        else
                        {
                            if(QuickKey.Value <= SystemConfig_Get_Int(MAX_MPHX10_UINT16)&&QuickKey.Value >= SystemConfig_Get_Int(MIN_MPHX10_UINT16))
                            {
                                Data_Set_Speed(QuickKey.Value,STANDARD,1);
                            }
                        }      
                        if(WorkoutSpeedData!=Data_Get_Speed(TARGET_SPEED))
                        {
                            WorkoutSpeedData=Data_Get_Speed(TARGET_SPEED);
                            pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
                            pendingEvt.WorkOutEvts.EvtBit.Workout_Quick_Recover=true;
                        }
                        
                    }
                    else
                    {
                        if(QuickKey.Value >= SystemConfig_Get_Char(MIN_RESISTANCE_UINT8) && QuickKey.Value <= SystemConfig_Get_Char(MAX_RESISTANCE_UINT8))
                        {
                            Data_Set_Resistance(QuickKey.Value,1);
                        }
                        if(WorkoutResistanceData!=Data_Get_Resistance(TARGET_RESISTANCE))
                        {
                            WorkoutResistanceData=Data_Get_Resistance(TARGET_RESISTANCE);
                            pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
                            pendingEvt.WorkOutEvts.EvtBit.Workout_Quick_Recover=true;
                        }
                    }
                    memset(&QuickKey,0,sizeof(QUICK_KEY));
                }
            }
            if(key_event.EventBits.Key_QuickKey)
            {
                Prg_isr_TimerCounterClear(TIMER_QUICKKEY);
                if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
                {
                    if(DefaultNumber[1].NumericKey.Used==1)
                    {
                        if(QuickKey.Index<3)
                        {
                            QuickKey.Index++;
                            if(QuickKey.Value!=0)
                                QuickKey.Value *=10;
                            QuickKey.Value +=QuickKey.Data;
                        }
                    }
                    key_event.EventBits.Key_QuickKey=false;
                    pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
                    pendingEvt.WorkOutEvts.EvtBit.Workout_QuickKeySpd_Evt=true;
                }
                else
                {
                    if(ProgramArray[proCurrentIndex].Programid != PS_CONSTANT_WATTS)
                    { 
                        if(DefaultNumber[1].NumericKey.Used==1)
                        {
                            if(QuickKey.Index<2)
                            {
                                QuickKey.Index++;
                                if(QuickKey.Value!=0)
                                    QuickKey.Value *=10;
                                QuickKey.Value +=QuickKey.Data;
                            }
                        }
                        key_event.EventBits.Key_QuickKey=false;
                        pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
                        if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==CLIMB_MILL)
                        {
                            pendingEvt.WorkOutEvts.EvtBit.Workout_Climbmill_ResChange=true;
                        }
                        else   
                        {
                            pendingEvt.WorkOutEvts.EvtBit.Workout_QuickKeyRes_Evt=true;
                        }
                    }
                }
            }
            if(QuickKey.Key.Evt.Spd || QuickKey.Key.Evt.Res)
            {
                if(Prg_isr_TimerCounter(TIMER_QUICKKEY,50))
                {
                    memset(&QuickKey,0,sizeof(QUICK_KEY));
                    QuickKey.Key.Evt.Recover = true ;
                }
            }
            if(QuickKey.Key.Evt.Recover)
            {
                QuickKey.Key.Evt.Recover =false;
                pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
                pendingEvt.WorkOutEvts.EvtBit.Workout_Quick_Recover=true;
            }
            if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)!=TREADMILL && SystemConfig_Get_Char(MACHINE_TYPE_UINT8)!=CLIMB_MILL )
            {
                if(Data_Get_RPM(EPBI_RPM)>=10)
                {
                    Prg_isr_TimerCounterClear(TIMER_20S_No_RPM);
                }
                else
                {
                    if(Prg_isr_TimerCounter(TIMER_20S_No_RPM,210))
                    {
                        Prg_isr_TimerCounterClear(TIMER_5S_PedalStart);
                        Program_Set_VirtualKey(VK_PAUSE | FRAME_STAGE_PRGRUN);
                    }
                }
            }
            if(key_event.EventBits.Key_Pause || key_event.EventBits.Key_SensorError)//
            {
                
                Prg_isr_TimerCounterClear(TIMER_PAUSE_5MIN);
                Prg_isr_TimerColse(TIMER_QUICKKEY);
                ChangeWorkoutState(WIFI_WORKOUT_PAUSED);
                Data_Set_Event(DATA_PAUSE_EVENT);
                if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==CLIMB_MILL)
                {
                    LCB_Send_Cmd(DS_SET_STATUS,0x01);
                    LCB_Send_Cmd(DS_SET_SPM,0);
                }
                else
                {
                    LCB_Send_Cmd(DS_INC_LOCATION,Data_Get_Incline(MIN_INCLINEADC));
                    LCB_Send_Cmd(DS_INC_ACTION,0x00);
                    LCB_Send_Cmd(DS_ECB_LOCATION,0);
                    LCB_Send_Cmd(DS_MOT_RPM,0);
                    LCB_Send_Cmd(DS_INDUCTION_PWM,0); 
                }

                g_StateMachine=PS_PAUSED;
//                if(key_event.EventBits.Key_Pause)
                {
                    pendingEvt.evt=PROGRAM_PAUSE_EVENT;
                    pendingEvt.WorkOutEvts.AllEvts=0;
                }
                key_event.EventBits.Key_Pause=false;
                key_event.EventBits.Key_SensorError = false ;
            }
        }
        case PS_COMPLETED:
		{
			if(Heart_Satus)
			{
				if(Data_Get_Heart(CURRENT_HEART)==0)
				{
					pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
					pendingEvt.WorkOutEvts.EvtBit.Workout_HR_ChangeToZero=true;
					Heart_Satus=0;
				}
			}
			else if(Heart_Satus==0)
			{
				if(Data_Get_Heart(CURRENT_HEART)>40)
				{
					pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
					pendingEvt.WorkOutEvts.EvtBit.Workout_HR_IsNotZero=true;
					Heart_Satus=1;
				}
			}
			if(Prg_isr_TimerCounter(TIMER_WORKOUTCOMPLETE,300))
			{
				key_event.EventBits.Key_Reset=true;
			}   
			if( key_event.EventBits.Key_ChangeDisplay)
			{
				key_event.EventBits.Key_ChangeDisplay=false;
				pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
				pendingEvt.WorkOutEvts.EvtBit.Workout_ChangeDisplay=true;
			}
			if(Prg_Mode.Scan)
			{
				if(Prg_isr_TimerCounter(TIMER_ScanFunction,100))
				{
					Prg_isr_TimerCounterClear(TIMER_ScanFunction);
					pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
					pendingEvt.WorkOutEvts.EvtBit.Workout_ChangeDisplay=true;
				}
			}     
			if(pendingEvt.WorkOutEvts.EvtBit.Workout_Complete_Evt)
			{
				ChangeWorkoutState(WIFI_WORKOUT_STOP);
				Data_Set_Event(DATA_PAUSE_EVENT);
				Prg_isr_TimerCounterClear(TIMER_WORKOUTCOMPLETE);
                if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==CLIMB_MILL)
                {
                    LCB_Send_Cmd(DS_SET_STATUS,0x01);
            	    LCB_Send_Cmd(DS_SET_SPM,0);
                }
                else
                {
                    LCB_Send_Cmd(DS_INC_LOCATION,Data_Get_Incline(MIN_INCLINEADC));
                    LCB_Send_Cmd(DS_ECB_ZERO,0);
                    LCB_Send_Cmd(DS_MOT_RPM,0);
                    LCB_Send_Cmd(DS_INDUCTION_PWM,0);
				}
			}
		}
        break;
        case PS_PAUSED:
//       	if(key_event.EventBits.Key_WorkoutSummary)
//		{
//			g_StateMachine=PS_COMPLETED;
//            ChangeWorkoutState(WIFI_WORKOUT_STOP);
//            Data_Set_Event(DATA_PAUSE_EVENT);
//            Prg_isr_TimerCounterClear(TIMER_WORKOUTCOMPLETE);
//            if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)!=CLIMB_MILL)
//            {
//                LCB_Send_Cmd(DS_INC_LOCATION,Data_Get_Incline(MIN_INCLINEADC));
//                LCB_Send_Cmd(DS_ECB_ZERO,0);
//                LCB_Send_Cmd(DS_MOT_RPM,0);
//                LCB_Send_Cmd(DS_INDUCTION_PWM,0);
//            }
//            pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
//			pendingEvt.WorkOutEvts.EvtBit.Workout_Complete_Evt=true;
//		}
	  	if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)!=TREADMILL && SystemConfig_Get_Char(MACHINE_TYPE_UINT8)!=CLIMB_MILL )
        {
            if(Data_Get_RPM(EPBI_RPM)>=10)
            {
                if(Prg_isr_TimerCounter(TIMER_5S_PedalStart,60))
                {
                    Prg_isr_TimerColse(TIMER_5S_PedalStart);
                    Program_Set_VirtualKey(VK_START | FRAME_STAGE_PRGRUN);
                }
            }
            else
            {
                Prg_isr_TimerCounterClear(TIMER_5S_PedalStart);
            }
        }    
        if(Prg_isr_TimerCounter(TIMER_PAUSE_5MIN,3000))
        {
//            ChangeWorkoutState(WIFI_WORKOUT_STOP);
//            if(Prg_isr_TimerCounter(TIMER_PAAUSE_DELAY1S,10))
//            {
                key_event.EventBits.Key_WorkoutSummary = true;
//                key_event.EventBits.Key_Reset=true;
//            }
//            key_event.EventBits.Key_Reset=true;
//            Program_Set_VirtualKey(VK_RESET | FRAME_STAGE_PRGRUN);
        } 
        break;
        default:
        break;
    }
    if(Data_Process())
    {
//      if(pendingEvt.evt==PROGRAM_NONE_EVENT)
//      {
//        pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
//        pendingEvt.WorkOutEvts.EvtBit.Workout_Refresh_Data=true;	
//      }
    }
if(key_event.EventBits.Key_WorkoutSummary)
		{
			g_StateMachine=PS_COMPLETED;
            ChangeWorkoutState(WIFI_WORKOUT_STOP);
            Data_Set_Event(DATA_PAUSE_EVENT);
            Prg_isr_TimerCounterClear(TIMER_WORKOUTCOMPLETE);
            if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)!=CLIMB_MILL)
            {
                LCB_Send_Cmd(DS_INC_LOCATION,Data_Get_Incline(MIN_INCLINEADC));
                LCB_Send_Cmd(DS_ECB_ZERO,0);
                LCB_Send_Cmd(DS_MOT_RPM,0);
                LCB_Send_Cmd(DS_INDUCTION_PWM,0);
            }
            pendingEvt.evt=WORKOUT_DATA_REFRESH_EVENT;
			pendingEvt.WorkOutEvts.EvtBit.Workout_Complete_Evt=true;
		}
	if(key_event.EventBits.Key_Reset)
	{
		if(g_StateMachine>PS_SETUP)
		{
			Data_Set_Event(DATA_SAVEDATA_EVENT);
		}
		g_StateMachine=PS_IDLE;
		key_event.EventBits.Key_Reset=false;
		ChangeWorkoutState(WIFI_WORKOUT_STOP);
        if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)!=CLIMB_MILL)
        {
            LCB_Send_Cmd(DS_INDUCTION_PWM,0);
            LCB_Send_Cmd(DS_ECB_ZERO,0);
            LCB_Send_Cmd(DS_MOT_RPM,0);
            LCB_Send_Cmd(DS_INC_LOCATION,Data_Get_Incline(MIN_INCLINEADC));
        }
		if(!DigitalGetRFstatus())Program_Initial_Inner_Data();//debug by ckm
		pendingEvt.evt=PROGRAM_RESET_EVENT;
		pendingEvt.WorkOutEvts.AllEvts=0;
	}    
    key_event.All_KeyEvents=0;
    if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
    {
        LCB_Set_DriverType(LCB_MOTOR);
    }
    else if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==CLIMB_MILL)
    {
        LCB_Set_DriverType(RETAIL_CLIMBMILL);
    }
    else 
    {
        LCB_Set_DriverType(LCB_EP_BIKE);
    }
	if(g_StateMachine > PS_IDLE && pendingEvt.evt == PROGRAM_NONE_EVENT )
	{
		pendingEvt.evt = PROGRAM_EVENT_PROCESSED;
	}
	if(pendingEvt.evt > PROGRAM_EVENT_PROCESSED)	
	{
		g_PRGPreEvent.evt = pendingEvt.evt;
	}
	return pendingEvt;
    
}
/***************************************************************************//**
* @brief
* �˺�������ȡ��Program UUID
* @param[in]��none
* @param[out]������Program UUID
******************************************************************************/

char* Program_Get_UUID(void)
{
    return (char*)(ProgramUUID[proCurrentIndex]);
}
/***************************************************************************//**
* @brief
* �˺�������ȡ��Program Description
* @param[in]��* plength����������
* @param[out]������Program UUDescription
******************************************************************************/

char* Program_Get_Description(UINT16* plength)
{
    *plength = 0;
    return NULL;
}
/***************************************************************************//**
* @brief
* �˺�������ȡ��Stage Goal Remaining
* @param[in]��none
* @param[out]��
����Stage Goal Remaining
******************************************************************************/
UINT16 Program_Get_Stage_Goal_Remaining(void)
{
    UINT16 ret = 0;
    
    return ret;
}
/***************************************************************************//**
* @brief
* �˺�������ȡ��Stage Goal
* @param[in]��none
* @param[out]��
����Stage Goal
******************************************************************************/
UINT16 Program_Get_Stage_Goal(void)
{
    UINT16 ret = 0;
    
    return ret;
}
/***************************************************************************//**
* @brief
* �˺�������ȡ��Lap
* @param[in]��none
* @param[out]��
����Lap
******************************************************************************/
UINT16 Program_Get_Lap_Index(void)
{
    UINT16 ret = 0;
    
    return ret;
}
/***************************************************************************//**
* @brief
* �˺�������ȡ��Split
* @param[in]��none
* @param[out]��
����Split
******************************************************************************/
UINT16 Program_Get_Split_Index(void)
{
    UINT16 ret = 0;
    
    return ret;
}
/***************************************************************************//**
* @brief
* �˺�������ȡ��Floor
* @param[in]��none
* @param[out]��
����Floor
******************************************************************************/

UINT16 Program_Get_Floor(void)
{
    UINT16 ret = 0;
    
    return ret;
}
/***************************************************************************//**
* @brief
* �˺�������ȡ��Elevation
* @param[in]��none
* @param[out]��
����Elevation
******************************************************************************/

UINT16 Program_Get_Elevation(void)
{
    UINT16 ret = 0;
    
    return ret;
}

/***************************************************************************//**
* @brief
* �˺�������ȡ��VO2
* @param[in]��none
* @param[out]��
����VO2
******************************************************************************/

UINT16 Program_Get_VO2(void)
{
    UINT16 ret = 0;
    return ret;
}

/***************************************************************************//**
* @brief
* �˺�������ȡ��Sprint8 Sweat Score
* @param[in]��index������
* @param[out]��
����Sprint8 Sweat Score
******************************************************************************/
UINT16 Sprint8_Get_Sweat_Score(UINT8 index)
{
    UINT16 ret = 0;
    
    return ret;
}
/***************************************************************************//**
* @brief
* �˺�������ȡ��Sprint8 Total Sweat Score
* @param[in]��none
* @param[out]��
����Sprint8 Total Sweat Score
******************************************************************************/

UINT16 Sprint8_Get_Total_Sweat_Score(void)
{
    UINT16 ret = 0;
    
    return ret;
}
/***************************************************************************//**
* @brief
* �˺�������ȡ��Fitness Test Sweat Score
* @param[in]��index������
* @param[out]��
����Fitness Test Sweat Score
******************************************************************************/

UINT16 Fitness_Test_Get_Score(void)
{
    UINT16 ret = 0;
    
    return ret;
}
/***************************************************************************//**
* @brief
* �˺������ڶ�ȡ�汾��
* @param[in]��*buffer���ڶ�ȡ�汾
* @param[out]�����ذ汾�ܳ���
******************************************************************************/

UINT8 Program_Get_Vision(UINT8 *buffer)
{
    UINT8 string[8] = PROGRAM_VISION;
    memcpy(buffer,string,sizeof(string));
    return(strlen(PROGRAM_VISION));
}


UINT8 Program_Is_Working(void)
{
    if(g_StateMachine<PS_WARMUP)
        
        return 0;
    else
        return 1;
}

UINT16 Sprint8_Get_SprintStage(void)
{

    return Sprint8Stage;
}

UINT8 Program_Get_DefaultDotMatrix(void)
{
    return DefaultDotmatrix;
}







