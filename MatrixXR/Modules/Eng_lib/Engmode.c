#include "Engmode_Inner.h"
#include "UtilitiesKeyboard.h"
#include "Events_Handler.h"
#include "Engmode.h"

//#include "Setting_Config.h"
#include <string.h>
__no_init static struEngmode EngArray[ENG_TOTALS];
__no_init static UCHAR engCurrentIndex;
__no_init static const UCHAR *EngSetting_Array;
__no_init static ENG_SETTING_ENUM EngSetting_Index;
__no_init static UCHAR EngKey_event;
__no_init static ENG_SETTING_ENUM EngSetting_Chart;
__no_init static ENG_EVENT Eng_event;


__no_init	static ENG_EVENT g_ENGPreEvent ;

__no_init ENGKeyEvts  EngKey_evt;
__no_init VR_KEY  VK_Value;
#define MAX_CEB_COUNT 310
#define MIN_CEB_COUNT 0
#define ENG_TIMER_CH 16

UINT8 ENG0Test_Number;
UINT8 ENG0Test_CharIndex;
UINT16 Eng_timer_counter;
UINT16 Eng1_Set_SpeedRPM;
UINT16 Eng1_Set_InclineADC;
UINT16 Eng1_Set_ECBCount;
UINT8 PROG_Mult_Times;
UINT8 PROG_Setted_RPM;
UINT8 AutoInclineTestStart=0;
UINT16 l_DATA=0;
UINT16 Debug_incline=0;
UINT8 EngMaxLevel=25;
UINT8 EngControlZoneData=200;
UINT16 EngRealControlZoneData=0;
UINT16 Eng1StControlZoneData=0;
UINT16 Eng2NdControlZoneData=0;
extern const UINT16 Models_Default[16*12];

__no_init struct 
{
    UINT16 Timer_Delay[ENG_TIMER_CH];
    UINT8  Timer_Channel[ENG_TIMER_CH];
}Eng_isr;


__no_init static union
{
     struct
     {
          uint64_t DisplayTest:1;
          uint64_t AllDotsOn:1;
          uint64_t AllDotsOff:1;
		  uint64_t DisNumber:1;	
		  uint64_t DisChar:1;	
		  uint64_t DisRowScan:1;	
		  uint64_t DisKeyValue:1;	
			
          //uint64_t Seg1:1;
          //uint64_t Seg2:1;
          //uint64_t Seg3:1;
          //eng1
          uint64_t HardwareTest:1;
          uint64_t BasicFunctions:1;
          uint64_t AutoADCFunctions:1;
          uint64_t InclineAutoTest:1;
          uint64_t InclinePassTest:1;
          uint64_t InclineFailTest:1;
          uint64_t ENG1RefreshData:1;
          uint64_t Usbport:1;
          uint64_t Usbtesting:1;
          uint64_t Wifi:1;
          uint64_t Wifi_Testing:1;
          //eng2
          uint64_t NotAvailable:1;
          //eng3
          uint64_t SwitchFunction:1;
          uint64_t StandardorMetric:1;
          uint64_t LanguageSelection:1;
          uint64_t MachineSelection:1;
          uint64_t ModelSelection:1;
          uint64_t EnergyMode:1;
          uint64_t FirstBoot:1;
          uint64_t SpeedSwitch:1;
          uint64_t ErrorCode:1;
          uint64_t ControlZone:1;
          uint64_t SecondControlZone:1;
          uint64_t ShowControlZoneData:1;
          uint64_t IrSensor:1;
          uint64_t MaxLevel:1;
          //eng4
          uint64_t INFORMATION:1;
          uint64_t AccumulatedInf:1;
          uint64_t UcbVersion:1;
          uint64_t McbVersion:1;
          uint64_t Rscu:1;
          //eng5
          uint64_t DAPI:1;
          uint64_t setdapi:1;
          //eng8
          uint64_t Test:1;
          uint64_t ProgramAccelerator:1;
          uint64_t ClearUserData:1;
          uint64_t ClearedUserData:1;
          uint64_t BurnCheck:1;
          uint64_t QuickStandbyMode:1; 
          uint64_t ERPTime:1; 
		  uint64_t Setted_RPM:1;
		  uint64_t SetOverSpeedValue:1;
          uint64_t Eng_Quitted:1;
     };
     uint64_t event;
}Engs_event;

typedef enum
{
    MODEL_TYPE_START = 0,
    TM_TF30 = MODEL_TYPE_START,
    TM_TF50,
    TM_T50,
    BK_R30,
    BK_U30,
    EP_SE30,
    BK_R50I,
    BK_U50I,
    EP_SE50I,
    AST_A30,
    AST_A50I,
    TM_T70_C2,
    TM_T70_C3,
    TM_T70_C1,
    CM_C50,
    TM_T30,
    TM_T75_C2,
    TM_T75_C3,
    TM_T75_C1,
    AVAIABLE_MACHINES_TOTAL
}AVAIABLE_MODELS;


const unsigned char ENG0_SetUp[]=
{
     //NO_SETTING,
     SET_ENG0_0_DisplayTest,
     SET_ENG0_0_TestItems
     /*SET_ENG0_1_AllDotsOn,
     SET_ENG0_2_AllDotsOff,
     SET_ENG0_3_Seg1,
     SET_ENG0_4_Seg2,
     SET_ENG0_5_Seg3,
     //SET_ENG0_Total*/
};
const unsigned char ENG1_SetUp[]=
{
            // NO_SETTING,
     SET_ENG1_0_HardwareTest,
     SET_ENG1_1_BasicFunctions,
     SET_ENG1_2_Usbport,
     SET_ENG1_3_Wifi
          //             SET_ENG1_Total
};
const unsigned char ENG2_SetUp[]=
{
          //NO_SETTING,
     SET_ENG2_0_NotAvailable,
     //          SET_ENG2_Total
};
const unsigned char ENG3_SetUp[]=
{
            // NO_SETTING,
     SET_ENG3_0_SwitchFunction,
     SET_ENG3_1_StandardorMetric,
     SET_ENG3_2_LanguageSelection,
     SET_ENG3_3_MachineSelection,
     SET_ENG3_4_ModelSelection,
     SET_ENG3_5_EnergyMode,
     SET_ENG3_6_FirstBoot,
     //        SET_ENG3_Total
};
const unsigned char ENG4_SetUp[]=
{
             //NO_SETTING,
     SET_ENG4_0_INFORMATION,
     SET_ENG4_1_AccumulatedInf,
     SET_ENG4_2_UcbVersion,
     SET_ENG4_3_McbVersion,
     //        SET_ENG4_Total
};
const unsigned char ENG5_SetUp[]=
{
             //NO_SETTING,
     SET_ENG5_0_DAPI,
     SET_ENG5_1_SetDapi,
     //        SET_ENG5_Total
};
const unsigned char ENG8_SetUp[]=
{
             //NO_SETTING,
     SET_ENG8_0_Test,
     SET_ENG8_1_ProgramAccelerator,
     SET_ENG8_2_ClearUserData,
     SET_ENG8_3_BurnCheck,
     SET_ENG8_4_QuickStandbyMode,
     //        SET_ENG8_Total
};
void initEng0mode(void);
void initEng1mode(void);
void initEng2mode(void);
void initEng3mode(void);
void initEng4mode(void);
void initEng5mode(void);
void initEng8mode(void);


/***************************************************************************//**
* @brief
*  初始化Engmode数据
* @note
* @param[in] 
*TableStruct *array[]:初始化TABEL表数组；
*size：Table表个数
* @param[out]：none
******************************************************************************/
void Engmode_Initial_Data(/*TableStruct *array[],*/UINT8 size)
{
	g_ENGPreEvent = ENG_EVENT_NONE;
	
     engCurrentIndex = 0;
     EngSetting_Index = ENG_SET_IDEL ;
     EngKey_evt.All_KeyEvents = 0 ;
     
     Eng_event=ENG_EVENT_NONE;
     EngKey_event=ENG_NONE_KEY_EVENT;
     Eng1_Set_InclineADC = 20;
     memset(EngArray,0,sizeof(struEngmode));
     memset(&Engs_event.event,0,sizeof(Engs_event.event));
     initEng0mode();
     initEng1mode();
     initEng2mode();
     initEng3mode();
     initEng4mode();
     initEng5mode();
     initEng8mode();
     Eng_Select_ID(ENG0_DIS);
}
UINT8 Eng_isr_TimerCounter(UINT8 by_Who,UINT16 w_Dat)
{
    UINT8 b_Out = 0;
    if(Eng_isr.Timer_Channel[by_Who] == 1 && Eng_isr.Timer_Delay[by_Who] >= w_Dat)
    {
        b_Out=1;
        Eng_isr.Timer_Channel[by_Who] = 0;
    }
    return b_Out;
}
void Eng_isr_TimerCounterClear(UINT8 by_Index)
{
    Eng_isr.Timer_Delay[by_Index]=0;
    Eng_isr.Timer_Channel[by_Index]=1;
}
void Eng_isr_TimerColse(UINT8 by_Dat)
{
    Eng_isr.Timer_Channel[by_Dat]=0;
}
void Eng_1ms_ISR(void)
{
    static UINT16 w_Timer_10ms=0;
    if(engCurrentIndex==ENG1_HW && (EngSetting_Index==SET_ENG1_1_HWTestStarted\
        ||EngSetting_Index ==SET_ENG1_1_BasicFunctions)||EngSetting_Index == SET_ENG1_4_AutoADC\
            ||EngSetting_Index ==SET_ENG3_8_1_ShowCotrolZoneData) 
    {
        if(Eng_timer_counter++>=1000)
        {
            Eng_timer_counter = 0 ;
            EngKey_evt.EventBits.Key_RefreshData =1 ;
        }
    }
    else
    {
        Eng_timer_counter = 0;
        EngKey_evt.EventBits.Key_RefreshData = 0 ;
    }
    if(++w_Timer_10ms<10)return;
    w_Timer_10ms=0;
    for(UINT8 i=0;i<ENG_TIMER_CH;i++)
    {
        if(Eng_isr.Timer_Channel[i]==1 && Eng_isr.Timer_Delay[i]<0xffff)
        {
            Eng_isr.Timer_Delay[i]++;
        }
    }
}

void Engmode_Regist_Function(void)
{
	HAL_SystemTicker_Alloc_ISR(Eng_1ms_ISR);
}


void Eng_Select_ID(ENG_ID erg_Type)
{
     engCurrentIndex = erg_Type;
     EngArray[engCurrentIndex].Initial();  
     switch(engCurrentIndex)
     {
     case ENG0_DIS:        EngSetting_Array = ENG0_SetUp;   /*EngSetting_Index=0;*/
     break;
     case ENG1_HW:         EngSetting_Array = ENG1_SetUp;   /*EngSetting_Index=0;*/ 
     break;
     case ENG2_AVAILABLE:  EngSetting_Array = ENG2_SetUp;   /*EngSetting_Index=0;*/
     break;
     case ENG3_SWITCH:     EngSetting_Array = ENG3_SetUp;   /*EngSetting_Index=0;*/
     break;
     case ENG4_INF:        EngSetting_Array = ENG4_SetUp;   /*EngSetting_Index=0;*/ 
     break;
     case ENG5_DAPI:        EngSetting_Array = ENG5_SetUp;   /*EngSetting_Index=0;*/ 
     break;
     case ENG8_TEST:       EngSetting_Array = ENG8_SetUp;   /*EngSetting_Index=0;*/ 
     break; 
     default: break; 
     }      
}
//ENG_ID Eng_Get_EngID(void)
//{
//     return (ENG_ID)EngArray[engCurrentIndex].Engid;
//}

ENG_SETTING_ENUM Engmode_SetUp(UCHAR key)
{
     switch(key)
     {
         case ENG_ENTER_KEY_EVENT:
         {
             EngSetting_Index++;   
             
             switch(engCurrentIndex)
             {
                 case ENG0_DIS:        if(EngSetting_Index >= sizeof(ENG0_SetUp))     EngSetting_Index =1; break;
                 case ENG1_HW:         if(EngSetting_Index >= sizeof(ENG1_SetUp))     EngSetting_Index =1;break;
                 case ENG2_AVAILABLE:  if(EngSetting_Index >= sizeof(ENG2_SetUp))     EngSetting_Index =0;break;
                 case ENG3_SWITCH:     if(EngSetting_Index >= sizeof(ENG3_SetUp))     EngSetting_Index =1;break;
                 case ENG4_INF:        if(EngSetting_Index >= sizeof(ENG4_SetUp))     EngSetting_Index =1;break;
                 case ENG5_DAPI:       if(EngSetting_Index >= sizeof(ENG5_SetUp))     EngSetting_Index =1;break;
                 case ENG8_TEST:       if(EngSetting_Index >= sizeof(ENG8_SetUp))     EngSetting_Index =1;break; 
                 default: EngSetting_Index =0; break; 
             } 
             
             break; 
         }
         default: break;
     }
     //}
     return (ENG_SETTING_ENUM)EngSetting_Array[EngSetting_Index];
}



void EngHw_Data_Initial(void)
{
     
}
UCHAR EngHw_Run(UCHAR key)
{  
     return 0;
}




UINT8 Engmode_Get_KeyEvent(void)
{
    switch(EngKey_event)
    {
        case ENG_BACK_KEY_EVENT:
            EngKey_event =ENG_NONE_KEY_EVENT;
            return ENG_BACK_KEY_EVENT;
            break;
        case ENG_ENTER_KEY_EVENT:
            EngKey_event =ENG_NONE_KEY_EVENT;
            return ENG_ENTER_KEY_EVENT;
            break;
        default:
            return ENG_NONE_KEY_EVENT;
            break;
    }
}


ENG_SETTING_ENUM Engmode_Get_SettingFlow(void)
{
     return EngSetting_Chart;
}



void Eng_Add_Array(struEngmode ero)
{
     if(ero.Engid<ENG_TOTALS)
     {
          memcpy(&EngArray[ero.Engid],&ero,sizeof(struEngmode));
     }
}



void Eng0_Data_Initial(void)
{
    
}
void Eng1_Data_Initial(void)
{
    
}

ENG_SETTING_ENUM Eng2_Setup(ENGKeyEvts keyevt)
{
    if(engCurrentIndex != ENG2_AVAILABLE) return(0);
    
    if(keyevt.All_KeyEvents ==0) return(0);
    
    Engs_event.event = 0;
    
    if(keyevt.EventBits.Key_Up||keyevt.EventBits.Key_SpeedUp||\
        keyevt.EventBits.Key_InclineUp||keyevt.EventBits.Key_ResistanceUp)
    {
        engCurrentIndex = ENG3_SWITCH;
        EngArray[engCurrentIndex].Initial();
        EngSetting_Index = SET_ENG3_0_SwitchFunction;
        Engs_event.SwitchFunction = true;
    }
    else if(keyevt.EventBits.Key_Dn||keyevt.EventBits.Key_SpeedDn||\
        keyevt.EventBits.Key_InclineDn||keyevt.EventBits.Key_ResistanceDn)
    {
        engCurrentIndex = ENG1_HW;
        EngArray[engCurrentIndex].Initial();
        EngSetting_Index = SET_ENG1_0_HardwareTest;
        Engs_event.HardwareTest = true;
    }
    else if(keyevt.EventBits.Key_Reset)
    {
        EngSetting_Index=0;            
        Engs_event.Eng_Quitted = true;
    } 
    EngKey_evt.All_KeyEvents = 0 ;
    return((ENG_SETTING_ENUM)EngSetting_Array[EngSetting_Index]);
}



ENG_SETTING_ENUM Eng0_Setup(ENGKeyEvts keyevt)
{
   if(engCurrentIndex != ENG0_DIS) return(0);
    
    if(keyevt.All_KeyEvents ==0) return(0);
    
    Engs_event.event = 0;
    switch(EngSetting_Index)
    {
        case SET_ENG0_0_DisplayTest:
        {
            if(keyevt.EventBits.Key_Up||keyevt.EventBits.Key_SpeedUp||\
                keyevt.EventBits.Key_InclineUp||keyevt.EventBits.Key_ResistanceUp)
            {
                engCurrentIndex = ENG1_HW;
                EngArray[engCurrentIndex].Initial();
                EngSetting_Index = SET_ENG1_0_HardwareTest;
                Engs_event.HardwareTest = true;
            }
            else if(keyevt.EventBits.Key_Dn||keyevt.EventBits.Key_SpeedDn||\
                keyevt.EventBits.Key_InclineDn||keyevt.EventBits.Key_ResistanceDn)
            {
                engCurrentIndex = ENG5_DAPI;
                EngArray[engCurrentIndex].Initial();
                EngSetting_Index = SET_ENG5_0_DAPI;
                Engs_event.DAPI = true;
            }
            else if(keyevt.EventBits.Key_Enter)
            {
                EngSetting_Index=SET_ENG0_0_TestItems;
//                Erase_EEPROM_Data(12) ;//ZDJ TEST
                Engs_event.AllDotsOn = true ;
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                EngSetting_Index=0;            
                Engs_event.Eng_Quitted = true;
            }
            else if(keyevt.EventBits.Key_ENG)
            {
                EngSetting_Index = SET_ENG0_0_DisplayTest;
                Engs_event.DisplayTest = true;
            }
        }
        break;
        case SET_ENG0_0_TestItems:
        {
			if(keyevt.EventBits.Key_Reset)
            {
                EngSetting_Index=0;
                EngSetting_Index = SET_ENG0_0_DisplayTest;
                Engs_event.DisplayTest = true ;
            }
            else if(keyevt.EventBits.Key_Start)
            {
                Engs_event.AllDotsOn = true ;
            }
			else if(keyevt.EventBits.Key_Stop)
            {
                Engs_event.AllDotsOff = true ;
            }
			else if(keyevt.EventBits.Key_SpeedUp||keyevt.EventBits.Key_ResistanceUp)
			{
				if(++ENG0Test_Number>=10)
					ENG0Test_Number = 0;
				Engs_event.DisNumber = true ;
			}
			else if(keyevt.EventBits.Key_SpeedDn||keyevt.EventBits.Key_ResistanceDn)
			{
				if(ENG0Test_Number>1)
					ENG0Test_Number --;
				else
					ENG0Test_Number = 9;
				Engs_event.DisNumber = true ;
			}
            else if(VK_Value !=VK_NULL)
            {
				Engs_event.DisKeyValue = true ;
			}
        }
        break;

        default:break;    
    }
    EngKey_evt.All_KeyEvents = 0 ;
    return((ENG_SETTING_ENUM)EngSetting_Array[EngSetting_Index]);
}

ENG_SETTING_ENUM Eng1_Setup(ENGKeyEvts keyevt)
{
    if(engCurrentIndex != ENG1_HW) return(0);
    
    if(keyevt.All_KeyEvents ==0) return(0);
    
    Engs_event.event = 0;
    
    switch(EngSetting_Index)
    {
        case SET_ENG1_0_HardwareTest:
        {
            if(keyevt.EventBits.Key_Up||keyevt.EventBits.Key_SpeedUp||\
                keyevt.EventBits.Key_InclineUp||keyevt.EventBits.Key_ResistanceUp)
            {
                if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
                {
                    engCurrentIndex = ENG2_AVAILABLE;
                    EngArray[engCurrentIndex].Initial();
                    EngSetting_Index = SET_ENG2_0_NotAvailable;
                    Engs_event.NotAvailable = true;
                }
                else
                {
                    engCurrentIndex = ENG3_SWITCH;
                    EngArray[engCurrentIndex].Initial();
                    EngSetting_Index = SET_ENG3_0_SwitchFunction;
                    Engs_event.SwitchFunction = true;
                }
            }
            else if(keyevt.EventBits.Key_Dn||keyevt.EventBits.Key_SpeedDn||\
                keyevt.EventBits.Key_InclineDn||keyevt.EventBits.Key_ResistanceDn)
            {
                engCurrentIndex = ENG0_DIS;
                EngArray[engCurrentIndex].Initial();
                EngSetting_Index = SET_ENG0_0_DisplayTest;
                Engs_event.DisplayTest = true;
            }
            else if(keyevt.EventBits.Key_Enter)
            {
                EngSetting_Index=SET_ENG1_1_BasicFunctions;
                Engs_event.BasicFunctions = true ;
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                EngSetting_Index=0;            
                Engs_event.Eng_Quitted = true;
            }
        }
        break;
        case SET_ENG1_1_BasicFunctions:
        {
            if(keyevt.EventBits.Key_Enter)
            {
//                EngSetting_Index=SET_ENG1_2_Usbport;
//                Engs_event.Usbport = true ;
                EngSetting_Index=SET_ENG1_3_Wifi;
                Engs_event.Wifi = true ;
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                EngSetting_Index = SET_ENG1_0_HardwareTest;
                Engs_event.HardwareTest = true ;
            }
            else if(keyevt.EventBits.Key_Start)
            {
                LCB_Send_Cmd(DS_MOT_RPM,Eng1_Set_SpeedRPM);
                LCB_Send_Cmd(DS_INC_LOCATION,Eng1_Set_InclineADC);
                LCB_Send_Cmd(DS_ECB_LOCATION,Eng1_Set_ECBCount);
                EngSetting_Index =SET_ENG1_1_HWTestStarted;
                Engs_event.BasicFunctions = true ;
            }
            else if(EngKey_evt.EventBits.Key_RefreshData)
            {
                Engs_event.BasicFunctions = true ;
            }
        }
        break;
        
        case SET_ENG1_1_HWTestStarted:
        {
            if(keyevt.EventBits.Key_Enter)
            {
                LCB_Send_Cmd(DS_MOT_RPM,0);
                LCB_Send_Cmd(DS_INC_ACTION,0);
                LCB_Send_Cmd(DS_ECB_LOCATION,0);
                
//                EngSetting_Index=SET_ENG1_2_Usbport;
//                Engs_event.Usbport = true ;
                EngSetting_Index=SET_ENG1_3_Wifi;
                Engs_event.Wifi = true ;
            }
            else if(keyevt.EventBits.Key_Start||keyevt.EventBits.Key_Stop)
            {
                LCB_Send_Cmd(DS_MOT_RPM,0);
                LCB_Send_Cmd(DS_INC_ACTION,0);
                LCB_Send_Cmd(DS_ECB_LOCATION,0);
                
                EngSetting_Index =SET_ENG1_1_HWTestStarted;
                Engs_event.BasicFunctions = true ;
            }
			else if(keyevt.EventBits.Key_Reset)
            {
            	LCB_Send_Cmd(DS_MOT_RPM,0);
                LCB_Send_Cmd(DS_INC_ACTION,0);
                LCB_Send_Cmd(DS_ECB_LOCATION,0);
                EngSetting_Index = SET_ENG1_0_HardwareTest;
                Engs_event.HardwareTest = true ;
            }			
            else if(keyevt.EventBits.Key_SpeedUp)
            {
                UINT16 Max_SpeedRPM_Value = (UINT16)((SystemConfig_Get_Int(MAX_KPH_RPM_UINT16))*1.1);
                Eng1_Set_SpeedRPM+=10;
                if(Eng1_Set_SpeedRPM>Max_SpeedRPM_Value)
                    Eng1_Set_SpeedRPM=Max_SpeedRPM_Value;
		
                LCB_Send_Cmd(DS_MOT_RPM,Eng1_Set_SpeedRPM);
                Engs_event.BasicFunctions = true ;
            }
            else if((keyevt.EventBits.Key_SpeedDn))
            {
                UINT16 Min_SpeedRPM_Value = SystemConfig_Get_Int(MIN_KPH_RPM_UINT16);
                if(Eng1_Set_SpeedRPM>Min_SpeedRPM_Value)
                    Eng1_Set_SpeedRPM-=10;
                else
                    Eng1_Set_SpeedRPM = Min_SpeedRPM_Value;
              
                LCB_Send_Cmd(DS_MOT_RPM,Eng1_Set_SpeedRPM);
                Engs_event.BasicFunctions = true ;
            }
            else if((keyevt.EventBits.Key_InclineUp))
            {
                UINT16 Max_InclineADC_Value = SystemConfig_Get_Int(MAX_INCLINEADC_UINT16);
                if(LCB_Get_MCBType()==MCB_DELTA_COMMOCIAL||(LCB_Get_MCBType()==MCB_TOPTEK)||(LCB_Get_MCBType()==MCB_DELTA))
                    Eng1_Set_InclineADC +=200;
                else
                    Eng1_Set_InclineADC +=10;
                if(Eng1_Set_InclineADC>(Max_InclineADC_Value+200))
                    Eng1_Set_InclineADC = Max_InclineADC_Value+200;
                LCB_Send_Cmd(DS_INC_LOCATION,Eng1_Set_InclineADC);
                Engs_event.BasicFunctions = true ;
            }
            else if((keyevt.EventBits.Key_InclineDn))
            {
                UINT16 Min_InclineADC_Value = SystemConfig_Get_Int(MIN_INLCINEADC_UINT16);
                if(Eng1_Set_InclineADC>Min_InclineADC_Value)
                {
                    if(LCB_Get_MCBType()==MCB_DELTA_COMMOCIAL||(LCB_Get_MCBType()==MCB_TOPTEK)||(LCB_Get_MCBType()==MCB_DELTA))
                        Eng1_Set_InclineADC -= 200;
                    else
                        Eng1_Set_InclineADC -= 10;
                }
                else
                    Eng1_Set_InclineADC = Min_InclineADC_Value;
                LCB_Send_Cmd(DS_INC_LOCATION,Eng1_Set_InclineADC);
                Engs_event.BasicFunctions = true ;
            }
            else if(EngKey_evt.EventBits.Key_ResistanceUp)
            {
                Eng1_Set_ECBCount +=1;
                if(Eng1_Set_ECBCount>MAX_CEB_COUNT)
                    Eng1_Set_ECBCount = MAX_CEB_COUNT;
                LCB_Send_Cmd(DS_ECB_LOCATION,Eng1_Set_ECBCount);
                Engs_event.BasicFunctions = true ;
            }
            else if(EngKey_evt.EventBits.Key_ResistanceDn)
            {
               if(Eng1_Set_ECBCount>MIN_CEB_COUNT)
                    Eng1_Set_ECBCount -= 1;
                else
                    Eng1_Set_ECBCount = MIN_CEB_COUNT;
                LCB_Send_Cmd(DS_ECB_LOCATION,Eng1_Set_ECBCount);
                Engs_event.BasicFunctions = true ;
            }
            else if(EngKey_evt.EventBits.Key_RefreshData)
            {
                Engs_event.BasicFunctions = true ;
            }
        }
        break;
        
        case SET_ENG1_2_Usbport:
        {
            if(keyevt.EventBits.Key_Enter)
            {
                EngSetting_Index=SET_ENG1_3_Wifi;
                Engs_event.Wifi = true ;
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                EngSetting_Index = SET_ENG1_0_HardwareTest;
                Engs_event.HardwareTest = true ;
            }
            //else if(keyevt.EventBits.Key_Start)
            //{
            //   Engs_event.Usbtesting = true ;
            //}
        }
        break;
        case SET_ENG1_3_Wifi:
        {
            if(keyevt.EventBits.Key_Enter)
            {
            	ClearFactoryTestStatus();
                EngSetting_Index=SET_ENG1_4_AutoADC;
                Engs_event.AutoADCFunctions = true ;
            }
            else if(keyevt.EventBits.Key_Reset)
            {
            	ClearFactoryTestStatus();
                EngSetting_Index = SET_ENG1_0_HardwareTest;
                Engs_event.HardwareTest = true ;
            }
            else if(keyevt.EventBits.Key_Start)
            {
            	ConnectAPForFactoryTest();
                Engs_event.Wifi_Testing = true ;
            }
            else if(keyevt.EventBits.Key_Stop)
            {
                
            }
        }
        break;
        case SET_ENG1_4_AutoADC:
        {
            if((keyevt.EventBits.Key_Enter)&&(AutoInclineTestStart==0))
            {
                EngSetting_Index=SET_ENG1_1_BasicFunctions;
                Engs_event.BasicFunctions = true ;
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                EngSetting_Index = SET_ENG1_0_HardwareTest;
                Engs_event.HardwareTest = true ;
            }
            else if(keyevt.EventBits.Key_Start)
            {
//                if((Digital_GetStatus() & 0x03) != 0)// S003-01
//                {
//                    //==>> 先让升降停止动作,确保RELAY的动作正确 
//                    LCB_Send_Cmd(DS_INC_ACTION,0);//==>升降停止
//                    // <=======================
//                }
//                LCB_Send_Cmd(DS_INC_ACTION,1);//==>升降上升
                LCB_Send_Cmd(DS_INC_LOCATION,255);
                LCB_Send_Cmd(DS_INC_ACTION,1);//==>升降上升
                Engs_event.InclineAutoTest = true ;
                AutoInclineTestStart = true;
            }
            else if(EngKey_evt.EventBits.Key_RefreshData)
            {
                if(AutoInclineTestStart)
                EngineerMode_AutoCalibration();   
//                Engs_event.InclineAutoTest = true ; 
            }
        }
        break;
        default:break;
    }
    EngKey_evt.All_KeyEvents = 0 ;
    return((ENG_SETTING_ENUM)EngSetting_Array[EngSetting_Index]);
}

ENG_SETTING_ENUM Eng3_Setup(ENGKeyEvts keyevt)
{
    if(engCurrentIndex != ENG3_SWITCH) return(0);
    
    if(keyevt.All_KeyEvents ==0) return(0);
    
    Engs_event.event = 0;
    
    switch(EngSetting_Index)
    {
        case SET_ENG3_0_SwitchFunction:
        {
            if(keyevt.EventBits.Key_Up||keyevt.EventBits.Key_SpeedUp||\
                keyevt.EventBits.Key_InclineUp||keyevt.EventBits.Key_ResistanceUp)
            {
                engCurrentIndex = ENG4_INF;
                EngArray[engCurrentIndex].Initial();
                EngSetting_Index = SET_ENG4_0_INFORMATION;
                Engs_event.INFORMATION = true;
            }
            else if(keyevt.EventBits.Key_Dn||keyevt.EventBits.Key_SpeedDn||\
                keyevt.EventBits.Key_InclineDn||keyevt.EventBits.Key_ResistanceDn)
            {
                if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
                {
                    engCurrentIndex = ENG2_AVAILABLE;
                    EngArray[engCurrentIndex].Initial();
                    EngSetting_Index = SET_ENG2_0_NotAvailable;
                    Engs_event.NotAvailable = true;
                }
                else
                {
                    engCurrentIndex = ENG1_HW;
                    EngArray[engCurrentIndex].Initial();
                    EngSetting_Index = SET_ENG1_0_HardwareTest;
                    Engs_event.HardwareTest = true;
                }
            }
            else if(keyevt.EventBits.Key_Enter)
            {
                EngSetting_Index=SET_ENG3_1_StandardorMetric;
                Engs_event.StandardorMetric = true ;
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                EngSetting_Index=0;            
                Engs_event.Eng_Quitted = true;
            }
            else if(keyevt.EventBits.Key_ENG)
            {
                engCurrentIndex=ENG8_TEST;
                EngSetting_Index = SET_ENG8_0_Test;
                Engs_event.Test = true ;
            }
        }
        break;
        case SET_ENG3_1_StandardorMetric:
        {
             if(keyevt.EventBits.Key_Up||keyevt.EventBits.Key_SpeedUp||\
                keyevt.EventBits.Key_InclineUp||keyevt.EventBits.Key_ResistanceUp||\
                keyevt.EventBits.Key_Dn||keyevt.EventBits.Key_SpeedDn||\
                keyevt.EventBits.Key_InclineDn||keyevt.EventBits.Key_ResistanceDn)
            {
                if(SystemConfig_Get_Char(UNIT_FLAG_UCHAR) == METRIC )
                {
                    UINT8 unit =STANDARD;
                    SystemConfigProcess(SVK_TOGGLE_UNIT,&unit,sizeof(unit));
                    SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
                }
                else
                {
                    UINT8 unit =METRIC;
                    SystemConfigProcess(SVK_TOGGLE_UNIT,&unit,sizeof(unit));
                    SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
                }
                Engs_event.StandardorMetric = true;
            }
            if(keyevt.EventBits.Key_Enter)
            {    
                EngSetting_Index=SET_ENG3_2_LanguageSelection;
                Engs_event.LanguageSelection = true ;
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                EngSetting_Index = SET_ENG3_0_SwitchFunction;
                Engs_event.SwitchFunction = true ;
            }
            else if(keyevt.EventBits.Key_ENG)
            {
                engCurrentIndex=ENG8_TEST;
                EngSetting_Index = SET_ENG8_0_Test;
                Engs_event.Test = true ;
            }
        }
        break;
        case SET_ENG3_2_LanguageSelection:
        {
            if(keyevt.EventBits.Key_Up||keyevt.EventBits.Key_SpeedUp||\
                keyevt.EventBits.Key_InclineUp||keyevt.EventBits.Key_ResistanceUp)
            {
                UINT8 ENGLanguageIndex = SystemConfig_Get_Char(LANGUAGE_ID_UINT8);
                if(++ENGLanguageIndex>=TOTAL_LANGUAGE)
                    ENGLanguageIndex = 0;
                SystemConfigProcess(SVK_SET_LANGUAGEID,&ENGLanguageIndex,sizeof(ENGLanguageIndex));
                SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
                Engs_event.LanguageSelection = true;
            }
            else if(keyevt.EventBits.Key_Dn||keyevt.EventBits.Key_SpeedDn||\
                keyevt.EventBits.Key_InclineDn||keyevt.EventBits.Key_ResistanceDn)
            {
                UINT8 ENGLanguageIndex = SystemConfig_Get_Char(LANGUAGE_ID_UINT8);
                if(ENGLanguageIndex>=1)
                    ENGLanguageIndex--;
                else
                    ENGLanguageIndex = TOTAL_LANGUAGE-1;
                
                SystemConfigProcess(SVK_SET_LANGUAGEID,&ENGLanguageIndex,sizeof(ENGLanguageIndex));
                SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
                Engs_event.LanguageSelection = true;
            }
            else if(keyevt.EventBits.Key_Enter)
            {
                EngSetting_Index=SET_ENG3_3_MachineSelection;
                Engs_event.MachineSelection = true ;
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                EngSetting_Index = SET_ENG3_0_SwitchFunction;
                Engs_event.SwitchFunction = true ;
            }
            else if(keyevt.EventBits.Key_ENG)
            {
                engCurrentIndex=ENG8_TEST;
                EngSetting_Index = SET_ENG8_0_Test;
                Engs_event.Test = true ;
            }
        }
        break;
        case SET_ENG3_3_MachineSelection:
        {
            if(keyevt.EventBits.Key_Enter)
            {
                EngSetting_Index=SET_ENG3_4_ModelSelection;
                Engs_event.ModelSelection = true ;
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                EngSetting_Index = SET_ENG3_0_SwitchFunction;
                Engs_event.SwitchFunction = true ;
            }
            else if(keyevt.EventBits.Key_Up||keyevt.EventBits.Key_SpeedUp||\
                keyevt.EventBits.Key_InclineUp||keyevt.EventBits.Key_ResistanceUp)
            {
                MACHINE_TYPE ENGMacheineIndex = SystemConfig_Get_Char(MACHINE_TYPE_UINT8);
                ENGMacheineIndex++;
			  UINT8 ParaIndex = 0;
//                if(ENGMacheineIndex == STEPPER)
//                {
//                    ENGMacheineIndex = CLIMB_MILL;
//                }
//                if(ENGMacheineIndex>CLIMB_MILL)
//                    ENGMacheineIndex = TREADMILL;
                if(ENGMacheineIndex>ASCENT)
                    ENGMacheineIndex = TREADMILL;
                if(ENGMacheineIndex==TREADMILL)
                    ParaIndex = 0;
                else if(ENGMacheineIndex==ELLIPTICAL)
                    ParaIndex = 5;
                else if(ENGMacheineIndex==BIKE)
                    ParaIndex = 4;
                else if(ENGMacheineIndex==ASCENT)
                    ParaIndex = 10;
                else if(ENGMacheineIndex==CLIMB_MILL)
                    ParaIndex = 14;
                MODEL_DEFAULTS tempmachineinfo;
                memcpy(&tempmachineinfo,&Models_Default[ParaIndex*sizeof(tempmachineinfo)/2],sizeof(tempmachineinfo));
                
                MACHINE_INFO machineinfo = SystemConfig_Get_MachineInfo();
                
                machineinfo.maxSpeedMphX10 = tempmachineinfo.maxSpeedSTDX10;
                machineinfo.minSpeedMphX10 = tempmachineinfo.minSpeedSTDX10;
                machineinfo.maxSpeedKphX10 = tempmachineinfo.maxSpeedKPHX10;
                machineinfo.minSpeedKphX10 = tempmachineinfo.minSpeedKPHX10;
                machineinfo.maxRpmMphX10 = tempmachineinfo.maxRPMSTD;
                machineinfo.minRpmMphX10 = tempmachineinfo.minRPMSTD;
                machineinfo.maxRpmKphX10 = tempmachineinfo.maxRPMKPH;
                machineinfo.minRpmKphX10 = tempmachineinfo.minRPMKPH;
                machineinfo.maxInclineX10 = tempmachineinfo.maxIncline;
                machineinfo.minInclineX10 = tempmachineinfo.minIncline;
                machineinfo.maxInclineADC= tempmachineinfo.maxInclineADC;
                machineinfo.minInclineADC= tempmachineinfo.minInclineADC;
                machineinfo.maxResistance= tempmachineinfo.maxResistance;
                machineinfo.minResistance= tempmachineinfo.minResistance;
                machineinfo.machineType= tempmachineinfo.machineType;
                machineinfo.driveType= tempmachineinfo.mcbType;
                machineinfo.machineModel= tempmachineinfo.machineModelDAPI;
                machineinfo.brandId= tempmachineinfo.brandid;
                     
                if(SystemConfig_Set_MachineInfo(machineinfo)) 
                {
                    SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
                }
                Engs_event.MachineSelection = true;
            }
            else if(keyevt.EventBits.Key_Dn||keyevt.EventBits.Key_SpeedDn||\
                keyevt.EventBits.Key_InclineDn||keyevt.EventBits.Key_ResistanceDn)
            {
                UINT8 ENGMacheineIndex = SystemConfig_Get_Char(MACHINE_TYPE_UINT8);
                UINT8 ParaIndex = 0;
//                if(ENGMacheineIndex>TREADMILL) 
//                    ENGMacheineIndex--;
//                else
//                    ENGMacheineIndex = CLIMB_MILL;
//                if(ENGMacheineIndex == STEPPER)
//                {
//                    ENGMacheineIndex = ASCENT;
//                }
                if(ENGMacheineIndex>TREADMILL) 
                    ENGMacheineIndex--;
                else
                    ENGMacheineIndex = ASCENT;
                if(ENGMacheineIndex==TREADMILL)
                    ParaIndex = TM_TF30;
                else if(ENGMacheineIndex==ELLIPTICAL)
                    ParaIndex = 5;
                else if(ENGMacheineIndex==BIKE)
                    ParaIndex = 4;
                else if(ENGMacheineIndex==ASCENT)
                    ParaIndex = 10;
                else if(ENGMacheineIndex==CLIMB_MILL)
                    ParaIndex = 14;
                MODEL_DEFAULTS tempmachineinfo;
                memcpy(&tempmachineinfo,&Models_Default[ParaIndex*sizeof(tempmachineinfo)/2],sizeof(tempmachineinfo));
                
                MACHINE_INFO machineinfo = SystemConfig_Get_MachineInfo();
                
                machineinfo.maxSpeedMphX10 = tempmachineinfo.maxSpeedSTDX10;
                machineinfo.minSpeedMphX10 = tempmachineinfo.minSpeedSTDX10;
                machineinfo.maxSpeedKphX10 = tempmachineinfo.maxSpeedKPHX10;
                machineinfo.minSpeedKphX10 = tempmachineinfo.minSpeedKPHX10;
                machineinfo.maxRpmMphX10 = tempmachineinfo.maxRPMSTD;
                machineinfo.minRpmMphX10 = tempmachineinfo.minRPMSTD;
                machineinfo.maxRpmKphX10 = tempmachineinfo.maxRPMKPH;
                machineinfo.minRpmKphX10 = tempmachineinfo.minRPMKPH;
                machineinfo.maxInclineX10 = tempmachineinfo.maxIncline;
                machineinfo.minInclineX10 = tempmachineinfo.minIncline;
                machineinfo.maxInclineADC= tempmachineinfo.maxInclineADC;
                machineinfo.minInclineADC= tempmachineinfo.minInclineADC;
                machineinfo.maxResistance= tempmachineinfo.maxResistance;
                machineinfo.minResistance= tempmachineinfo.minResistance;
                machineinfo.machineType= tempmachineinfo.machineType;
                machineinfo.driveType= tempmachineinfo.mcbType;
                machineinfo.machineModel= tempmachineinfo.machineModelDAPI;
                machineinfo.brandId= tempmachineinfo.brandid;
                     
                if(SystemConfig_Set_MachineInfo(machineinfo)) 
                {
                    SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
                }
                Engs_event.MachineSelection = true;
            }
            else if(keyevt.EventBits.Key_ENG)
            {
                engCurrentIndex=ENG8_TEST;
                EngSetting_Index = SET_ENG8_0_Test;
                Engs_event.Test = true ;
            }
        }
        break;
        case SET_ENG3_4_ModelSelection:
        {
            if(keyevt.EventBits.Key_Enter)
            {
                EngSetting_Index=SET_ENG3_5_EnergyMode;
                Engs_event.EnergyMode = true ;
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                EngSetting_Index = SET_ENG3_0_SwitchFunction;
                Engs_event.SwitchFunction = true ;
            }
            else if(keyevt.EventBits.Key_Up||keyevt.EventBits.Key_SpeedUp||\
                keyevt.EventBits.Key_InclineUp||keyevt.EventBits.Key_ResistanceUp)
            {
                AVAIABLE_MODELS temp_Machine=SystemConfig_Get_Char(MACHINE_TYPE_UINT8);
                UINT8 temp_Model = SystemConfig_Get_Char(MACHINE_MODLE_UINT8);
                UINT8 ParaNumber;
                if(temp_Machine==TREADMILL)
                {
                    if(temp_Model==1) ParaNumber = TM_TF50;
                    else if(temp_Model==2) ParaNumber = TM_T50;
                    else if(temp_Model==3) ParaNumber = TM_T70_C2;
                    else if(temp_Model==4) ParaNumber = TM_T75_C2;
                    else if(temp_Model==5) ParaNumber = TM_T30;
                    else if(temp_Model==6) ParaNumber = TM_TF30;
                }
                else if(temp_Machine==ELLIPTICAL)
                {
                    if(temp_Model==1) ParaNumber = EP_SE50I;
                    else if(temp_Model==2) ParaNumber = EP_SE30;
                }
                else if(temp_Machine==BIKE) 
                {
                    if(temp_Model==1) ParaNumber =  BK_U50I;
                    else if(temp_Model==2) ParaNumber = BK_R30;
                    else if(temp_Model==3) ParaNumber = BK_R50I;
                    else if(temp_Model==4) ParaNumber = BK_U30;
                }
                else if(temp_Machine==ASCENT)
                {
                    if(temp_Model==1) ParaNumber = AST_A50I;
                    else if(temp_Model==2) ParaNumber = AST_A30;
                }
                else if(temp_Machine==CLIMB_MILL)
                {
                    if(temp_Model==1) ParaNumber = CM_C50;
                }
                MODEL_DEFAULTS tempmachineinfo;
                memcpy(&tempmachineinfo,&Models_Default[ParaNumber*sizeof(tempmachineinfo)/2],sizeof(tempmachineinfo));
                
                MACHINE_INFO machineinfo = SystemConfig_Get_MachineInfo();
                
                machineinfo.maxSpeedMphX10 = tempmachineinfo.maxSpeedSTDX10;
                machineinfo.minSpeedMphX10 = tempmachineinfo.minSpeedSTDX10;
                machineinfo.maxSpeedKphX10 = tempmachineinfo.maxSpeedKPHX10;
                machineinfo.minSpeedKphX10 = tempmachineinfo.minSpeedKPHX10;
                machineinfo.maxRpmMphX10 = tempmachineinfo.maxRPMSTD;
                machineinfo.minRpmMphX10 = tempmachineinfo.minRPMSTD;
                machineinfo.maxRpmKphX10 = tempmachineinfo.maxRPMKPH;
                machineinfo.minRpmKphX10 = tempmachineinfo.minRPMKPH;
                machineinfo.maxInclineX10 = tempmachineinfo.maxIncline;
                machineinfo.minInclineX10 = tempmachineinfo.minIncline;
                machineinfo.maxInclineADC= tempmachineinfo.maxInclineADC;
                machineinfo.minInclineADC= tempmachineinfo.minInclineADC;
                machineinfo.maxResistance= tempmachineinfo.maxResistance;
                machineinfo.minResistance= tempmachineinfo.minResistance;
                machineinfo.machineType= tempmachineinfo.machineType;
                machineinfo.driveType= tempmachineinfo.mcbType;
                machineinfo.machineModel= tempmachineinfo.machineModelDAPI;
                machineinfo.brandId= tempmachineinfo.brandid;    
                if(SystemConfig_Set_MachineInfo(machineinfo)) 
                {
                    SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
                }
                Engs_event.ModelSelection = true;
            }
            else if(keyevt.EventBits.Key_Dn||keyevt.EventBits.Key_SpeedDn||\
                keyevt.EventBits.Key_InclineDn||keyevt.EventBits.Key_ResistanceDn)
            {
                AVAIABLE_MODELS temp_Machine=SystemConfig_Get_Char(MACHINE_TYPE_UINT8);
                UINT8 temp_Model = SystemConfig_Get_Char(MACHINE_MODLE_UINT8);
                UINT8 ParaNumber;
                
                if(temp_Machine==TREADMILL)
                {
                    if(temp_Model==1) ParaNumber = TM_T75_C2 ;
                    else if(temp_Model==2) ParaNumber = TM_TF30 ;
                    else if(temp_Model==3) ParaNumber = TM_TF50;
                    else if(temp_Model==4) ParaNumber = TM_T50;
                    else if(temp_Model==5) ParaNumber = TM_T70_C2;
                    else if(temp_Model==6) ParaNumber = TM_T30;
                }
                else if(temp_Machine==ELLIPTICAL)
                {
                    if(temp_Model==1) ParaNumber = EP_SE50I;
                    else if(temp_Model==2) ParaNumber = EP_SE30;
                }
                else if(temp_Machine==BIKE) 
                {
                    if(temp_Model==1) ParaNumber = BK_R50I;
                    else if(temp_Model==2) ParaNumber = BK_U30;
                    else if(temp_Model==3) ParaNumber = BK_U50I;
                    else if(temp_Model==4) ParaNumber = BK_R30;
                }
                else if(temp_Machine==ASCENT)
                {
                    if(temp_Model==1) ParaNumber = AST_A50I;
                    else if(temp_Model==2) ParaNumber = AST_A30;
                }
                else if(temp_Machine==CLIMB_MILL)
                {
                    if(temp_Model==1) ParaNumber = CM_C50;
                }
                MODEL_DEFAULTS tempmachineinfo;
                memcpy(&tempmachineinfo,&Models_Default[ParaNumber*sizeof(tempmachineinfo)/2],sizeof(tempmachineinfo));
                
                MACHINE_INFO machineinfo = SystemConfig_Get_MachineInfo();
                
                machineinfo.maxSpeedMphX10 = tempmachineinfo.maxSpeedSTDX10;
                machineinfo.minSpeedMphX10 = tempmachineinfo.minSpeedSTDX10;
                machineinfo.maxSpeedKphX10 = tempmachineinfo.maxSpeedKPHX10;
                machineinfo.minSpeedKphX10 = tempmachineinfo.minSpeedKPHX10;
                machineinfo.maxRpmMphX10 = tempmachineinfo.maxRPMSTD;
                machineinfo.minRpmMphX10 = tempmachineinfo.minRPMSTD;
                machineinfo.maxRpmKphX10 = tempmachineinfo.maxRPMKPH;
                machineinfo.minRpmKphX10 = tempmachineinfo.minRPMKPH;
                machineinfo.maxInclineX10 = tempmachineinfo.maxIncline;
                machineinfo.minInclineX10 = tempmachineinfo.minIncline;
                machineinfo.maxInclineADC= tempmachineinfo.maxInclineADC;
                machineinfo.minInclineADC= tempmachineinfo.minInclineADC;
                machineinfo.maxResistance= tempmachineinfo.maxResistance;
                machineinfo.minResistance= tempmachineinfo.minResistance;
                machineinfo.machineType= tempmachineinfo.machineType;
                machineinfo.driveType= tempmachineinfo.mcbType;
                machineinfo.machineModel= tempmachineinfo.machineModelDAPI;
                machineinfo.brandId= tempmachineinfo.brandid;   
                     
                if(SystemConfig_Set_MachineInfo(machineinfo)) 
                {
                    SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
                }
                Engs_event.ModelSelection = true;
            }
            else if(keyevt.EventBits.Key_ENG)
            {
                engCurrentIndex=ENG8_TEST;
                EngSetting_Index = SET_ENG8_0_Test;
                Engs_event.Test = true ;
            }
        }
        break;
        case SET_ENG3_5_EnergyMode:
        {
             if(keyevt.EventBits.Key_Up||keyevt.EventBits.Key_SpeedUp||\
                keyevt.EventBits.Key_InclineUp||keyevt.EventBits.Key_ResistanceUp||\
                keyevt.EventBits.Key_Dn||keyevt.EventBits.Key_SpeedDn||\
                keyevt.EventBits.Key_InclineDn||keyevt.EventBits.Key_ResistanceDn)
            {
                UINT8 tempsaverflag =SystemConfig_Get_Char(ENERGY_SAVER_FLAG_UCHAR);
                if( tempsaverflag == 1 )
                {
                    tempsaverflag =0;
                    SystemConfigProcess(SVK_TOGGLE_ERP,&tempsaverflag,sizeof(SVK_TOGGLE_ERP));
                    SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
                }
                else
                {
                    tempsaverflag = 1;
                    SystemConfigProcess(SVK_TOGGLE_ERP,&tempsaverflag,sizeof(SVK_TOGGLE_ERP));
                    SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
                }
                Engs_event.EnergyMode = true;
            }
            if(keyevt.EventBits.Key_Enter)
            {    
                EngSetting_Index=SET_ENG3_6_FirstBoot;
                Engs_event.FirstBoot = true ;
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                EngSetting_Index = SET_ENG3_0_SwitchFunction;
                Engs_event.SwitchFunction = true ;
            }
            else if(keyevt.EventBits.Key_ENG)
            {
                engCurrentIndex=ENG8_TEST;
                EngSetting_Index = SET_ENG8_0_Test;
                Engs_event.Test = true ;
            }
        }
        break;
        case SET_ENG3_6_FirstBoot:
        {
             if(keyevt.EventBits.Key_Up||keyevt.EventBits.Key_SpeedUp||\
                keyevt.EventBits.Key_InclineUp||keyevt.EventBits.Key_ResistanceUp||\
                keyevt.EventBits.Key_Dn||keyevt.EventBits.Key_SpeedDn||\
                keyevt.EventBits.Key_InclineDn||keyevt.EventBits.Key_ResistanceDn)
             {
                 UINT8 boot = SystemConfig_Get_Char(FIRST_BOOT_FLAG_UCHAR);
                 if(boot==0xA5)
                 {
                     boot =0xFF;
                     SystemConfigProcess(SVK_TOGGLE_FIRSTBOOT,&boot,sizeof(boot));
                     SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
                 }
                 else
                 {
                     boot =0xA5;
                     SystemConfigProcess(SVK_TOGGLE_FIRSTBOOT,&boot,sizeof(boot));
                     SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
                 }
                 Engs_event.FirstBoot = true;
             }
            if(keyevt.EventBits.Key_Enter)
            {    
//#ifdef FUNCTION_OVERSPEED_ON            
//                EngSetting_Index=SET_ENG3_7_OverSpeed;
//                Engs_event.SpeedSwitch = true ;
                
                
//#else
//		EngSetting_Index=SET_ENG3_1_StandardorMetric;
//                	Engs_event.StandardorMetric = true ;
//#endif
                if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8) == CLIMB_MILL)
                {
                    EngSetting_Index=SET_ENG3_7_ErrorCode;
                    LCB_Send_Cmd(DG_GET_LCB_DEVICE_DATE, 0x040000);
                    Engs_event.ErrorCode = true ;
                }
                else
                {
                    EngSetting_Index=SET_ENG3_1_StandardorMetric;
                    Engs_event.StandardorMetric = true ;
                }
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                EngSetting_Index = SET_ENG3_0_SwitchFunction;
                Engs_event.SwitchFunction = true ;
            }
            else if(keyevt.EventBits.Key_ENG)
            {
                engCurrentIndex=ENG8_TEST;
                EngSetting_Index = SET_ENG8_0_Test;
                Engs_event.Test = true ;
            }
        }
        break;
        case SET_ENG3_7_ErrorCode:
	    {
//             if(keyevt.EventBits.Key_Up||keyevt.EventBits.Key_SpeedUp||\
//                keyevt.EventBits.Key_InclineUp||keyevt.EventBits.Key_ResistanceUp||\
//                keyevt.EventBits.Key_Dn||keyevt.EventBits.Key_SpeedDn||\
//                keyevt.EventBits.Key_InclineDn||keyevt.EventBits.Key_ResistanceDn)
//             {
//                 UINT8 boot = SystemConfig_Get_Char(OVERSPEED_SWITCH_UINT8);
//                 if(boot==1)
//                 {
//                     boot =0;
//                     SystemConfigProcess(SVK_TOGGLE_OVERSPEEDFLAG,&boot,sizeof(boot));
//                     SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
//                 }
//                 else
//                 {
//                     boot =1;
//                     SystemConfigProcess(SVK_TOGGLE_OVERSPEEDFLAG,&boot,sizeof(boot));
//                     SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
//                 }
//                 Engs_event.SpeedSwitch = true;
//             }
//            if(keyevt.EventBits.Key_Enter)
//            {    
//                EngSetting_Index=SET_ENG3_1_StandardorMetric;
//                Engs_event.StandardorMetric = true ;
//            }
//            else if(keyevt.EventBits.Key_Reset)
//            {
//                EngSetting_Index = SET_ENG3_0_SwitchFunction;
//                Engs_event.SwitchFunction = true ;
//            }
//            else if(keyevt.EventBits.Key_ENG)
//            {
//                engCurrentIndex=ENG8_TEST;
//                EngSetting_Index = SET_ENG8_0_Test;
//                Engs_event.Test = true ;
//            }
            if(keyevt.EventBits.Key_Up||keyevt.EventBits.Key_SpeedUp||\
                keyevt.EventBits.Key_InclineUp||keyevt.EventBits.Key_ResistanceUp||\
                keyevt.EventBits.Key_Dn||keyevt.EventBits.Key_SpeedDn||\
                keyevt.EventBits.Key_InclineDn||keyevt.EventBits.Key_ResistanceDn)
            {
                if(SystemConfig_Get_Char(SHOW_ERROR_FLAG_UCHAR) == 1 )
                {
                  UINT8 error =0;
                  SystemConfigProcess(SVK_TOGGLE_SHOW_ERROR,&error,sizeof(error));
                  SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
                }
                else
                {
                  UINT8 error =1;
                  SystemConfigProcess(SVK_TOGGLE_SHOW_ERROR,&error,sizeof(error));
                  SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
                }
                Engs_event.ErrorCode = true;
            }
            if(keyevt.EventBits.Key_Enter)
            {    
                EngSetting_Index=SET_ENG3_8_ControlZone;
                LCB_Send_Cmd(DG_GET_LCB_DEVICE_DATE, 0x040000);
                Engs_event.ControlZone = true ;
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                EngSetting_Index = SET_ENG3_0_SwitchFunction;
                Engs_event.SwitchFunction = true ;
            }
            else if(keyevt.EventBits.Key_ENG)
            {
                engCurrentIndex=ENG8_TEST;
                EngSetting_Index = SET_ENG8_0_Test;
                Engs_event.Test = true ;
            }
        }
		break;
        case SET_ENG3_8_ControlZone:
        {
            if(keyevt.EventBits.Key_Up||keyevt.EventBits.Key_SpeedUp||\
                keyevt.EventBits.Key_InclineUp||keyevt.EventBits.Key_ResistanceUp||\
                keyevt.EventBits.Key_Dn||keyevt.EventBits.Key_SpeedDn||\
                keyevt.EventBits.Key_InclineDn||keyevt.EventBits.Key_ResistanceDn)
            {
                if(SystemConfig_Get_Char(CONTROL_ZONE_FLAG_UCHAR) == 1 )
                {
                    UINT8 OnOff_Flag =0;
                    SystemConfigProcess(SVK_CONTROL_ZONE,&OnOff_Flag,sizeof(OnOff_Flag));
                    SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
                }
                else
                {
                    UINT8 OnOff_Flag =1;
                    SystemConfigProcess(SVK_CONTROL_ZONE,&OnOff_Flag,sizeof(OnOff_Flag));
                    SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
                }
                if(( SystemConfig_Get_Char(CONTROL_ZONE_FLAG_UCHAR) == 0 )&&\
                    ( SystemConfig_Get_Char(IR_SENSOR_FLAG_UCHAR) == 0 ))
                {
                    LCB_Send_Cmd(DS_SET_STATUS, 0xA0);
                }
                if(( SystemConfig_Get_Char(CONTROL_ZONE_FLAG_UCHAR) == 0 )&&\
                    ( SystemConfig_Get_Char(IR_SENSOR_FLAG_UCHAR) == 1 ))
                {
                    LCB_Send_Cmd(DS_SET_STATUS, 0xA2);
                }
                if(( SystemConfig_Get_Char(CONTROL_ZONE_FLAG_UCHAR) == 1 )&&\
                    ( SystemConfig_Get_Char(IR_SENSOR_FLAG_UCHAR) == 0 ))
                {
                    LCB_Send_Cmd(DS_SET_STATUS, 0xA3);
                }
                if(( SystemConfig_Get_Char(CONTROL_ZONE_FLAG_UCHAR) == 1 )&&\
                    ( SystemConfig_Get_Char(IR_SENSOR_FLAG_UCHAR) == 1 ))
                {
                    LCB_Send_Cmd(DS_SET_STATUS, 0xA1);
                }
                Engs_event.ControlZone = true ;
            }
            if(keyevt.EventBits.Key_Enter)
            {    
//                EngSetting_Index=SET_ENG3_8_1_ShowCotrolZoneData;
                EngSetting_Index=SET_ENG3_8_0_SecondCotrolZone;
                LCB_Send_Cmd(DG_GET_TWO_CONTROL_DATA, 0x070000);
//                Engs_event.ShowControlZoneData = true ;
                Engs_event.SecondControlZone = true;
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                EngSetting_Index = SET_ENG3_0_SwitchFunction;
                
                Engs_event.SwitchFunction = true ;
            }
            else if(keyevt.EventBits.Key_ENG)
            {
                engCurrentIndex=ENG8_TEST;
                EngSetting_Index = SET_ENG8_0_Test;
                Engs_event.Test = true ;
            }
        }
        break;
        case SET_ENG3_8_0_SecondCotrolZone:
        {
            if(keyevt.EventBits.Key_Up||keyevt.EventBits.Key_SpeedUp||\
                keyevt.EventBits.Key_InclineUp||keyevt.EventBits.Key_ResistanceUp||\
                keyevt.EventBits.Key_Dn||keyevt.EventBits.Key_SpeedDn||\
                keyevt.EventBits.Key_InclineDn||keyevt.EventBits.Key_ResistanceDn)
            {
                if(SystemConfig_Get_Char(SECOND_CONTROL_ZONE_FLAG_UCHAR) == 1 )
                {
                    UINT8 OnOff_Flag =0;
                    SystemConfigProcess(SVK_SECOND_CONTROL_ZONE,&OnOff_Flag,sizeof(OnOff_Flag));
                    SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
                    LCB_Send_Cmd(DS_SET_STATUS, 0xA6);
                }
                else
                {
                    UINT8 OnOff_Flag =1;
                    SystemConfigProcess(SVK_SECOND_CONTROL_ZONE,&OnOff_Flag,sizeof(OnOff_Flag));
                    SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
                    LCB_Send_Cmd(DS_SET_STATUS, 0xA7);
                }
                Engs_event.SecondControlZone = true ;
            }
            if(keyevt.EventBits.Key_Enter)
            {    
                EngSetting_Index=SET_ENG3_8_1_ShowCotrolZoneData;
                LCB_Send_Cmd(DG_GET_TWO_CONTROL_DATA, 0x070000);
                Engs_event.ShowControlZoneData = true ;
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                EngSetting_Index = SET_ENG3_0_SwitchFunction;
                
                Engs_event.SwitchFunction = true ;
            }
            else if(keyevt.EventBits.Key_ENG)
            {
                engCurrentIndex=ENG8_TEST;
                EngSetting_Index = SET_ENG8_0_Test;
                Engs_event.Test = true ;
            }
        }
        break;
        case SET_ENG3_8_1_ShowCotrolZoneData:
        {
            if(keyevt.EventBits.Key_Up||keyevt.EventBits.Key_SpeedUp||\
                keyevt.EventBits.Key_InclineUp||keyevt.EventBits.Key_ResistanceUp)
            {
                if(EngControlZoneData<200)
                {
                    EngControlZoneData ++;
                }
                else
                {
                    EngControlZoneData = 4;
                }
                LCB_Send_Cmd(DS_SET_CONTROL_DATA,EngControlZoneData);
                
            }
            if(keyevt.EventBits.Key_Dn||keyevt.EventBits.Key_SpeedDn||\
                keyevt.EventBits.Key_InclineDn||keyevt.EventBits.Key_ResistanceDn)
            {
                if(EngControlZoneData>4)
                {
                    EngControlZoneData --;
                }
                else
                {
                    EngControlZoneData = 200;
                }
                LCB_Send_Cmd(DS_SET_CONTROL_DATA,EngControlZoneData);
            }
            if(keyevt.EventBits.Key_Enter)
            {    
                EngSetting_Index=SET_ENG3_9_IrSensor;
                LCB_Send_Cmd(DG_GET_LCB_DEVICE_DATE, 0x040000);
                Engs_event.IrSensor = true ;
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                EngSetting_Index = SET_ENG3_0_SwitchFunction;
                
                Engs_event.SwitchFunction = true ;
            }
            else if(keyevt.EventBits.Key_ENG)
            {
                engCurrentIndex=ENG8_TEST;
                EngSetting_Index = SET_ENG8_0_Test;
                Engs_event.Test = true ;
            }
            else if(EngKey_evt.EventBits.Key_RefreshData)
            {
                LCB_Send_Cmd(DG_GET_TWO_CONTROL_DATA, 0x070000);
                Eng1StControlZoneData = (LCB_Get_Data(G_GET_TWO_CONTROL_DATA)&0xffff0000)>>16;
                Eng2NdControlZoneData = LCB_Get_Data(G_GET_TWO_CONTROL_DATA);
//                LCB_Send_Cmd(DG_GET_LCB_DEVICE_DATE, 0x040000);       
//                EngRealControlZoneData = (LCB_Get_Data(G_GET_LCB_DEVICE_DATA)&0xffff0000)>>16;
                Engs_event.ShowControlZoneData = true ;
            }
        }
        break;
        case SET_ENG3_9_IrSensor:
        {
            if(keyevt.EventBits.Key_Up||keyevt.EventBits.Key_SpeedUp||\
                keyevt.EventBits.Key_InclineUp||keyevt.EventBits.Key_ResistanceUp||\
                keyevt.EventBits.Key_Dn||keyevt.EventBits.Key_SpeedDn||\
                keyevt.EventBits.Key_InclineDn||keyevt.EventBits.Key_ResistanceDn)
            {
                //待加入
                if(SystemConfig_Get_Char(IR_SENSOR_FLAG_UCHAR) == 1 )
                {
                    UINT8 OnOff_Flag =0;
                    SystemConfigProcess(SVK_IR_SENSOR,&OnOff_Flag,sizeof(OnOff_Flag));
                    SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
                }
                else
                {
                    UINT8 OnOff_Flag =1;
                    SystemConfigProcess(SVK_IR_SENSOR,&OnOff_Flag,sizeof(OnOff_Flag));
                    SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
                }
                if(( SystemConfig_Get_Char(CONTROL_ZONE_FLAG_UCHAR) == 0 )&&\
                    ( SystemConfig_Get_Char(IR_SENSOR_FLAG_UCHAR) == 0 ))
                {
                    LCB_Send_Cmd(DS_SET_STATUS, 0xA0);
                }
                if(( SystemConfig_Get_Char(CONTROL_ZONE_FLAG_UCHAR) == 0 )&&\
                    ( SystemConfig_Get_Char(IR_SENSOR_FLAG_UCHAR) == 1 ))
                {
                    LCB_Send_Cmd(DS_SET_STATUS, 0xA2);
                }
                if(( SystemConfig_Get_Char(CONTROL_ZONE_FLAG_UCHAR) == 1 )&&\
                    ( SystemConfig_Get_Char(IR_SENSOR_FLAG_UCHAR) == 0 ))
                {
                    LCB_Send_Cmd(DS_SET_STATUS, 0xA3);
                }
                if(( SystemConfig_Get_Char(CONTROL_ZONE_FLAG_UCHAR) == 1 )&&\
                    ( SystemConfig_Get_Char(IR_SENSOR_FLAG_UCHAR) == 1 ))
                {
                    LCB_Send_Cmd(DS_SET_STATUS, 0xA1);
                }
                Engs_event.IrSensor = true;
            }
            if(keyevt.EventBits.Key_Enter)
            {    
                EngSetting_Index=SET_ENG3_10_MaxLevel;
                Engs_event.MaxLevel = true ;
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                EngSetting_Index = SET_ENG3_0_SwitchFunction;
                Engs_event.SwitchFunction = true ;
            }
            else if(keyevt.EventBits.Key_ENG)
            {
                engCurrentIndex=ENG8_TEST;
                EngSetting_Index = SET_ENG8_0_Test;
                Engs_event.Test = true ;
            }
        }
        break;
        case SET_ENG3_10_MaxLevel:
        {
            if(keyevt.EventBits.Key_Up||keyevt.EventBits.Key_SpeedUp||\
                keyevt.EventBits.Key_InclineUp||keyevt.EventBits.Key_ResistanceUp)
            {
                //待加入
                if(EngMaxLevel<30)
                {
                    EngMaxLevel++;
                }
                else
                {
                    EngMaxLevel = 20;
                }
                Engs_event.MaxLevel = true ;
            }
            else if(keyevt.EventBits.Key_Dn||keyevt.EventBits.Key_SpeedDn||\
                keyevt.EventBits.Key_InclineDn||keyevt.EventBits.Key_ResistanceDn)
            {
                if(EngMaxLevel>20)
                {
                    EngMaxLevel--;
                }
                else
                {
                    EngMaxLevel = 30;
                }
                Engs_event.MaxLevel = true ;
            }
            if(keyevt.EventBits.Key_Enter)
            {    
                if(SystemConfig_Get_Char(MAX_RESISTANCE_UINT8) != EngMaxLevel )
                {
                    SystemConfigProcess(SVK_SET_MAX_RESISTANCE,&EngMaxLevel,sizeof(EngMaxLevel));
                    SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
                }
                EngSetting_Index=SET_ENG3_1_StandardorMetric;
                Engs_event.StandardorMetric = true ;
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                if(SystemConfig_Get_Char(MAX_RESISTANCE_UINT8) != EngMaxLevel )
                {
                    SystemConfigProcess(SVK_SET_MAX_RESISTANCE,&EngMaxLevel,sizeof(EngMaxLevel));
                    SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
                }
                EngSetting_Index = SET_ENG3_0_SwitchFunction;
                Engs_event.SwitchFunction = true ;
            }
            else if(keyevt.EventBits.Key_ENG)
            {
                engCurrentIndex=ENG8_TEST;
                EngSetting_Index = SET_ENG8_0_Test;
                Engs_event.Test = true ;
            }
        }
        break;
        default:break;
    }
    EngKey_evt.All_KeyEvents = 0 ;
    return((ENG_SETTING_ENUM)EngSetting_Array[EngSetting_Index]);
}
ENG_SETTING_ENUM Eng4_Setup(ENGKeyEvts keyevt)
{
   if(engCurrentIndex != ENG4_INF) return(0);
    
    if(keyevt.All_KeyEvents ==0) return(0);
    
    Engs_event.event = 0;
    switch(EngSetting_Index)
    {
        case SET_ENG4_0_INFORMATION:
        {
            if(keyevt.EventBits.Key_Up||keyevt.EventBits.Key_SpeedUp||\
                keyevt.EventBits.Key_InclineUp||keyevt.EventBits.Key_ResistanceUp)
            {
                engCurrentIndex = ENG5_DAPI;
                EngArray[engCurrentIndex].Initial();
                EngSetting_Index = SET_ENG5_0_DAPI;
                Engs_event.DAPI = true;
            }
            else if(keyevt.EventBits.Key_Dn||keyevt.EventBits.Key_SpeedDn||\
                keyevt.EventBits.Key_InclineDn||keyevt.EventBits.Key_ResistanceDn)
            {
                engCurrentIndex = ENG3_SWITCH;
                EngArray[engCurrentIndex].Initial();
                EngSetting_Index = SET_ENG3_0_SwitchFunction;
                Engs_event.SwitchFunction = true;
            }
            else if(keyevt.EventBits.Key_Enter)
            {
                EngSetting_Index=SET_ENG4_1_AccumulatedInf;
                Engs_event.AccumulatedInf = true ;
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                EngSetting_Index=0;            
                Engs_event.Eng_Quitted = true;
            }
        }
        break;
        case SET_ENG4_1_AccumulatedInf:
        {
            if(keyevt.EventBits.Key_Enter)
            {
                EngSetting_Index=SET_ENG4_2_UcbVersion;
                Engs_event.UcbVersion = true ;
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                EngSetting_Index = SET_ENG4_0_INFORMATION;
                Engs_event.INFORMATION = true ;
            }
        }
        break;
        case SET_ENG4_2_UcbVersion:
        {
            if(keyevt.EventBits.Key_Enter)
            {
                EngSetting_Index=SET_ENG4_3_McbVersion;
                Engs_event.McbVersion = true ;
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                EngSetting_Index = SET_ENG4_0_INFORMATION;
                Engs_event.INFORMATION = true ;
            }
        }
        break;
        case SET_ENG4_3_McbVersion:
        {
            if(keyevt.EventBits.Key_Enter)
            {
                EngSetting_Index = SET_ENG4_4_Rscu;
                Engs_event.Rscu = true ;
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                EngSetting_Index = SET_ENG4_0_INFORMATION;
                Engs_event.INFORMATION = true ;
            }
        }
        break;
        case SET_ENG4_4_Rscu:
        {
            if(keyevt.EventBits.Key_Up||keyevt.EventBits.Key_SpeedUp||\
                keyevt.EventBits.Key_InclineUp||keyevt.EventBits.Key_ResistanceUp||\
                keyevt.EventBits.Key_Dn||keyevt.EventBits.Key_SpeedDn||\
                keyevt.EventBits.Key_InclineDn||keyevt.EventBits.Key_ResistanceDn)
             {
                 UINT8 rscuStatu = SystemConfig_Get_Char(RSCU_FLAG_UCHAR);
                 if(rscuStatu==0xA5)
                 {
                     rscuStatu =0xFF;
                     SystemConfigProcess(SVK_RSCU,&rscuStatu,sizeof(rscuStatu));
                     SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
                 }
                 else
                 {
                     rscuStatu =0xA5;
                     SystemConfigProcess(SVK_RSCU,&rscuStatu,sizeof(rscuStatu));
                     SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
                 }
                 Engs_event.Rscu = true ;
             }
            else if(keyevt.EventBits.Key_Enter)
            {
                EngSetting_Index = SET_ENG4_1_AccumulatedInf;
                Engs_event.AccumulatedInf = true ;
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                EngSetting_Index = SET_ENG4_0_INFORMATION;
                Engs_event.INFORMATION = true ;
            }
        }
        break;
        default:break;    
    }
    EngKey_evt.All_KeyEvents = 0 ;
    return((ENG_SETTING_ENUM)EngSetting_Array[EngSetting_Index]);
}

ENG_SETTING_ENUM Eng5_Setup(ENGKeyEvts keyevt)
{
   if(engCurrentIndex != ENG5_DAPI) return(0);
    
    if(keyevt.All_KeyEvents ==0) return(0);
    
    Engs_event.event = 0;
    switch(EngSetting_Index)
    {
        case SET_ENG5_0_DAPI:
        {
            if(keyevt.EventBits.Key_Up||keyevt.EventBits.Key_SpeedUp||\
                keyevt.EventBits.Key_InclineUp||keyevt.EventBits.Key_ResistanceUp)
            {
                engCurrentIndex = ENG0_DIS;
                EngArray[engCurrentIndex].Initial();
                EngSetting_Index = SET_ENG0_0_DisplayTest;
                Engs_event.DisplayTest = true;
            }
            else if(keyevt.EventBits.Key_Dn||keyevt.EventBits.Key_SpeedDn||\
                keyevt.EventBits.Key_InclineDn||keyevt.EventBits.Key_ResistanceDn)
            {
                engCurrentIndex = ENG4_INF;
                EngArray[engCurrentIndex].Initial();
                EngSetting_Index = SET_ENG4_0_INFORMATION;
                Engs_event.INFORMATION = true;
            }
            else if(keyevt.EventBits.Key_Enter)
            {
                EngSetting_Index=SET_ENG5_1_SetDapi;
                Engs_event.setdapi = true ;
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                EngSetting_Index=0;            
                Engs_event.Eng_Quitted = true;
            }
        }
        break;
        case SET_ENG5_1_SetDapi:
        {
            if(keyevt.EventBits.Key_Up||keyevt.EventBits.Key_SpeedUp||\
                keyevt.EventBits.Key_InclineUp||keyevt.EventBits.Key_ResistanceUp)
            {
                MACHINE_INFO machineinfo = SystemConfig_Get_MachineInfo();
                
                if(strncmp( machineinfo.marioBox, JOHNSON_WIFI_SERVERIP_PRODUCTION, 256) == 0 )
                {
                    memset( machineinfo.marioBox, 0, 256 );
				    strcpy( machineinfo.marioBox,JOHNSON_WIFI_SERVERIP_QA);
                }
                else if(strncmp( machineinfo.marioBox,JOHNSON_WIFI_SERVERIP_QA, 256) == 0 )
                {
                    memset( machineinfo.marioBox, 0, 256 );
				    strcpy( machineinfo.marioBox,JOHNSON_WIFI_SERVERIP_STAGING);
                }
                else if(strncmp( machineinfo.marioBox,JOHNSON_WIFI_SERVERIP_STAGING, 256) == 0 )
                {
                    memset( machineinfo.marioBox, 0, 256 );
				    strcpy( machineinfo.marioBox,JOHNSON_WIFI_SERVERIP_DEV);
                }
                else
                {
                    memset( machineinfo.marioBox, 0, 256 );
				    strcpy( machineinfo.marioBox,JOHNSON_WIFI_SERVERIP_PRODUCTION);
				}
				if(SystemConfig_Set_MachineInfo(machineinfo ))
                {
                    SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH, NULL,0);
                }

                WifiConnectionStatusChange(0);
                WifiConnectionStatusChange(1);
                Engs_event.setdapi = true;
            }
            else if(keyevt.EventBits.Key_Dn||keyevt.EventBits.Key_SpeedDn||\
                keyevt.EventBits.Key_InclineDn||keyevt.EventBits.Key_ResistanceDn)
            {
                MACHINE_INFO machineinfo = SystemConfig_Get_MachineInfo();
                
                if(strncmp(machineinfo.marioBox, JOHNSON_WIFI_SERVERIP_PRODUCTION, 256) == 0 )
                {
                    memset( machineinfo.marioBox, 0, 256 );
				    strcpy( machineinfo.marioBox,JOHNSON_WIFI_SERVERIP_DEV);
                }
                else if(strncmp( machineinfo.marioBox,JOHNSON_WIFI_SERVERIP_QA, 256) == 0 )
                {
                    memset( machineinfo.marioBox, 0, 256 );
				    strcpy( machineinfo.marioBox,JOHNSON_WIFI_SERVERIP_PRODUCTION);
                }
                else if(strncmp( machineinfo.marioBox,JOHNSON_WIFI_SERVERIP_STAGING, 256) == 0 )
                {
                    memset( machineinfo.marioBox, 0, 256 );
				    strcpy( machineinfo.marioBox,JOHNSON_WIFI_SERVERIP_QA);
                }
                 else
                {
                    memset( machineinfo.marioBox, 0, 256 );
				    strcpy( machineinfo.marioBox,JOHNSON_WIFI_SERVERIP_STAGING);
				}
				if(SystemConfig_Set_MachineInfo(machineinfo ))
                {
                    SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH, NULL,0);
                }
                
                WifiConnectionStatusChange(0);
                WifiConnectionStatusChange(1);
                Engs_event.setdapi = true;
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                EngSetting_Index = SET_ENG5_0_DAPI;
                Engs_event.DAPI = true ;
            }
        }
        break;
        
        default:break;    
    }
    EngKey_evt.All_KeyEvents = 0 ;
    return((ENG_SETTING_ENUM)EngSetting_Array[EngSetting_Index]);
}

ENG_SETTING_ENUM Eng8_Setup(ENGKeyEvts keyevt)
{
    if(engCurrentIndex != ENG8_TEST) return(0);
    
    if(keyevt.All_KeyEvents ==0) return(0);
    
    Engs_event.event = 0;
    
    switch(EngSetting_Index)
    {
        case SET_ENG8_0_Test:
        {
            if(keyevt.EventBits.Key_Enter)
            {
                EngSetting_Index=SET_ENG8_1_ProgramAccelerator;
                Engs_event.ProgramAccelerator = true ;
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                engCurrentIndex = ENG3_SWITCH;
                EngArray[engCurrentIndex].Initial();
                EngSetting_Index = SET_ENG3_0_SwitchFunction;
                Engs_event.SwitchFunction = true;
            }
        }
        break;
        case SET_ENG8_1_ProgramAccelerator:
        {
            if(keyevt.EventBits.Key_Up||keyevt.EventBits.Key_SpeedUp||\
                keyevt.EventBits.Key_InclineUp||keyevt.EventBits.Key_ResistanceUp)
            {
                if(++PROG_Mult_Times>100)
                    PROG_Mult_Times = 1;
                Program_Set_nXTime(PROG_Mult_Times);
                Engs_event.ProgramAccelerator = true ;
            }
            else if(keyevt.EventBits.Key_Dn||keyevt.EventBits.Key_SpeedDn||\
                keyevt.EventBits.Key_InclineDn||keyevt.EventBits.Key_ResistanceDn)
            {
                UINT16 temp= Data_Get_Nx();
                if(PROG_Mult_Times<=1)
                    PROG_Mult_Times = 100;
                else
                    PROG_Mult_Times -=1;
                Program_Set_nXTime(PROG_Mult_Times);                
                Engs_event.ProgramAccelerator = true ;
            }
            else if(keyevt.EventBits.Key_Enter)
            {    
                EngSetting_Index=SET_ENG8_2_ClearUserData;
                Engs_event.ClearUserData = true ;
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                engCurrentIndex = ENG3_SWITCH;
                EngArray[engCurrentIndex].Initial();
                EngSetting_Index = SET_ENG3_0_SwitchFunction;
                Engs_event.SwitchFunction = true;
            }
        }
        break;
        case SET_ENG8_2_ClearUserData:
        {
            if(keyevt.EventBits.Key_Enter)
            {
                EngSetting_Index=SET_ENG8_3_BurnCheck;
                Engs_event.BurnCheck = true ;
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                engCurrentIndex=ENG3_SWITCH;
                EngSetting_Index = SET_ENG3_0_SwitchFunction;
                Engs_event.SwitchFunction = true ;
            }
            else if(keyevt.EventBits.Key_Save)
            {
            	RestoreWifiToFactoryDefaults();
                SystemConfigRestoreToFactory();
                Eng1_Set_SpeedRPM = 0 ;
                Eng1_Set_InclineADC = 0;
                Eng1_Set_ECBCount = 0 ;
                PROG_Mult_Times = 0 ;
                PROG_Setted_RPM = 0 ;
                PROG_Mult_Times = 1;
                EngSetting_Index = SET_ENG8_2_ClearUserData;
                Program_Set_nXTime(PROG_Mult_Times);
                Engs_event.ClearedUserData = true ;
            }
        }
        break;
        case SET_ENG8_3_BurnCheck:
        {
            if(keyevt.EventBits.Key_Enter)
            {
                EngSetting_Index=SET_ENG8_4_QuickStandbyMode;
                Engs_event.QuickStandbyMode = true ;
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                engCurrentIndex=ENG3_SWITCH;
                EngSetting_Index = SET_ENG3_0_SwitchFunction;
                Engs_event.SwitchFunction = true ;
            }
        }
        break;
        case SET_ENG8_4_QuickStandbyMode:
        {
            if(keyevt.EventBits.Key_Enter)
            {
                EngSetting_Index=SET_ENG8_5_ERPTime;
                Engs_event.ERPTime = true ;
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                engCurrentIndex=ENG3_SWITCH;
                EngSetting_Index = SET_ENG3_0_SwitchFunction;
                Engs_event.SwitchFunction = true ;
            }
            else if(keyevt.EventBits.Key_Start)
            {
            	
                ERP_Quick_In();
            }
        }
        break;
        case SET_ENG8_5_ERPTime:
        {
             if(keyevt.EventBits.Key_Up||keyevt.EventBits.Key_SpeedUp||\
                keyevt.EventBits.Key_InclineUp||keyevt.EventBits.Key_ResistanceUp)
            {
                UINT16 tempERPTime =SystemConfig_Get_Int(ERP_WAIT_MIN_UINT16);
                tempERPTime+=60;
                if(tempERPTime>900) 
                    tempERPTime = 60;

                SystemConfigProcess(SVK_SET_ERP_TIMES,&tempERPTime,sizeof(tempERPTime));
                SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);

                Engs_event.ERPTime = true;
            }
            else if(keyevt.EventBits.Key_Dn||keyevt.EventBits.Key_SpeedDn||\
                keyevt.EventBits.Key_InclineDn||keyevt.EventBits.Key_ResistanceDn)
            {
                UINT16 tempERPTime =SystemConfig_Get_Int(ERP_WAIT_MIN_UINT16);
                
                if(tempERPTime>60) 
                    tempERPTime-=60;
                else 
                    tempERPTime = 900;
                SystemConfigProcess(SVK_SET_ERP_TIMES,&tempERPTime,sizeof(tempERPTime));
                SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);

                Engs_event.ERPTime = true;
            }
            else if(keyevt.EventBits.Key_Enter)
            {   
				 if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
				 {
					EngSetting_Index = SET_ENG8_7_OverSpeedValue ;
					Engs_event.SetOverSpeedValue = true ;
				 }
				 else
				 {
					EngSetting_Index = SET_ENG8_6_RPM ;
	                Engs_event.Setted_RPM = true ;
				 }
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                engCurrentIndex=ENG3_SWITCH;
                EngSetting_Index = SET_ENG3_0_SwitchFunction;
                Engs_event.SwitchFunction = true ;
            }
        }
		break;
		case SET_ENG8_6_RPM:
		{
			if(keyevt.EventBits.Key_Enter)
            {    
				EngSetting_Index = SET_ENG8_1_ProgramAccelerator ;
                Engs_event.ProgramAccelerator = true ;
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                engCurrentIndex=ENG3_SWITCH;
                EngSetting_Index = SET_ENG3_0_SwitchFunction;
                Engs_event.SwitchFunction = true ;
            }
			else  if(keyevt.EventBits.Key_Up||keyevt.EventBits.Key_SpeedUp||\
                keyevt.EventBits.Key_InclineUp||keyevt.EventBits.Key_ResistanceUp)
            {
                if(PROG_Setted_RPM<140)
                PROG_Setted_RPM++;
                else
                    PROG_Setted_RPM = 1;
				//todo 
				//set program rpm
                Program_Set_RPM(PROG_Setted_RPM);

                Engs_event.Setted_RPM = true;
            }
            else if(keyevt.EventBits.Key_Dn||keyevt.EventBits.Key_SpeedDn||\
                keyevt.EventBits.Key_InclineDn||keyevt.EventBits.Key_ResistanceDn)
            {
                UINT16 tempERPTime =SystemConfig_Get_Int(ERP_WAIT_MIN_UINT16);
                if(PROG_Setted_RPM>1)
                 PROG_Setted_RPM--;
                else
                    PROG_Setted_RPM = 140;
                //todo 
				//set program rpm
                Program_Set_RPM(PROG_Setted_RPM);
                Engs_event.Setted_RPM = true;
            }
		}
        break;
		case SET_ENG8_7_OverSpeedValue:
        {
             if(keyevt.EventBits.Key_Up||keyevt.EventBits.Key_SpeedUp||\
                keyevt.EventBits.Key_InclineUp||keyevt.EventBits.Key_ResistanceUp)
            {
                UINT8 overspeedvalue = SystemConfig_Get_Char(OVERSPEED_VALUE);
                overspeedvalue+=1;
                if(overspeedvalue>MAX_OVERSPEED_VALUE) 
                    overspeedvalue = MIN_OVERSPEED_VALUE;

                SystemConfigProcess(SVK_TOGGLE_OVERSPEEDVALUE,&overspeedvalue,sizeof(overspeedvalue));
                SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);

                Engs_event.SetOverSpeedValue = true;
            }
            else if(keyevt.EventBits.Key_Dn||keyevt.EventBits.Key_SpeedDn||\
                keyevt.EventBits.Key_InclineDn||keyevt.EventBits.Key_ResistanceDn)
            {
                UINT8 overspeedvalue = SystemConfig_Get_Char(OVERSPEED_VALUE);
                
                if(overspeedvalue> MIN_OVERSPEED_VALUE) 
                    overspeedvalue-=1;
                else 
                    overspeedvalue = MAX_OVERSPEED_VALUE;
                SystemConfigProcess(SVK_TOGGLE_OVERSPEEDVALUE,&overspeedvalue,sizeof(overspeedvalue));
                SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);

                Engs_event.SetOverSpeedValue = true;
            }
            else if(keyevt.EventBits.Key_Enter)
            {   
				 if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
				 {
					EngSetting_Index = SET_ENG8_1_ProgramAccelerator ;
					Engs_event.ProgramAccelerator = true ;
				 }
				 else
				 {
					EngSetting_Index = SET_ENG8_6_RPM ;
	                Engs_event.Setted_RPM = true ;
				 }
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                engCurrentIndex=ENG3_SWITCH;
                EngSetting_Index = SET_ENG3_0_SwitchFunction;
                Engs_event.SwitchFunction = true ;
            }
        }
		break;
        default:break;
    }
    EngKey_evt.All_KeyEvents = 0 ;
    return((ENG_SETTING_ENUM)EngSetting_Array[EngSetting_Index]);
}

void initEng0mode(void)
{
     struEngmode eng0mode; 
     eng0mode.Engid = ENG0_DIS;
     eng0mode.Initial = Eng0_Data_Initial;
     eng0mode.Setup = Eng0_Setup;
     eng0mode.Run = NULL;
     Eng_Add_Array(eng0mode);
     EngSetting_Index=0;
     VK_Value = 0 ;
	 ENG0Test_Number = 0 ;
	 ENG0Test_CharIndex = 0 ;
}

void initEng1mode(void)
{
    struEngmode eng1mode; 
    eng1mode.Engid = ENG1_HW;
    eng1mode.Initial = Eng1_Data_Initial;
    eng1mode.Setup = Eng1_Setup;
    eng1mode.Run = EngHw_Run;
    Eng_Add_Array(eng1mode);
    EngSetting_Index=0;
    Eng_timer_counter=0;
    Eng1_Set_SpeedRPM=SystemConfig_Get_Int(MIN_KPH_RPM_UINT16);
    Eng1_Set_InclineADC=SystemConfig_Get_Int(MIN_INCLINEX10_UINT16);
    Eng1_Set_ECBCount=0;
}
void initEng2mode(void)
{
     struEngmode eng2mode; 
     eng2mode.Engid = ENG2_AVAILABLE;
     eng2mode.Initial = EngHw_Data_Initial;
     eng2mode.Setup = Eng2_Setup;
     eng2mode.Run = EngHw_Run;
     Eng_Add_Array(eng2mode);
     EngSetting_Index=0;
}
void initEng3mode(void)
{
     struEngmode eng3mode;
     eng3mode.Engid = ENG3_SWITCH;
     eng3mode.Initial = EngHw_Data_Initial;
    eng3mode.Setup = Eng3_Setup;
     eng3mode.Run = EngHw_Run;
     Eng_Add_Array(eng3mode);
     EngSetting_Index=0;
}
void initEng4mode(void)
{
     struEngmode eng4mode;
     eng4mode.Engid = ENG4_INF;
     eng4mode.Initial = EngHw_Data_Initial;
     eng4mode.Setup = Eng4_Setup;
     eng4mode.Run = EngHw_Run;
     Eng_Add_Array(eng4mode);
     EngSetting_Index=0;
}
void initEng5mode(void)
{
     struEngmode eng5mode;
     eng5mode.Engid = ENG5_DAPI;
     eng5mode.Initial = EngHw_Data_Initial;
     eng5mode.Setup = Eng5_Setup;
     eng5mode.Run = NULL;
     Eng_Add_Array(eng5mode);
     EngSetting_Index=0;
}
void initEng8mode(void)
{
     struEngmode eng8mode; 
     eng8mode.Engid = ENG8_TEST;
     eng8mode.Initial = EngHw_Data_Initial;
     eng8mode.Setup = Eng8_Setup;
     eng8mode.Run = NULL;
     Eng_Add_Array(eng8mode);
     EngSetting_Index=0;
     PROG_Mult_Times = 1;
}

VR_KEY Engmode_Set_VirtualKey( VR_KEY TempKey)
{
	if(TempKey == VK_ENG && EngSetting_Index == ENG_SET_IDEL)
	{
		Engmode_Initial_Data(1);
		Engs_event.DisplayTest = true;
		EngSetting_Index	= SET_ENG0_0_DisplayTest;
		TempKey = VK_PROCESSED ;
		return TempKey;
	}
     
     switch(TempKey)
     {
         //case EVR_NULL: break;
         case VK_INCLINEUP:
         EngKey_evt.EventBits.Key_InclineUp = true;
         break;
         case VK_SPEEDUP:
         EngKey_evt.EventBits.Key_SpeedUp = true;
         break; 
         case VK_UP:
         EngKey_evt.EventBits.Key_Up = true;
         break;
         case VK_RESISTANCEUP:
         EngKey_evt.EventBits.Key_ResistanceUp = true;
         break;
         
         case VK_RESISTANCEDOWN:
         EngKey_evt.EventBits.Key_ResistanceDn = true;
         break;
         case VK_INCLINEDOWN:
         EngKey_evt.EventBits.Key_InclineDn = true;
         break;
         case VK_SPEEDDOWN:
         EngKey_evt.EventBits.Key_SpeedDn = true;
         break;
         case VK_DOWN:
         EngKey_evt.EventBits.Key_Dn = true;
         break;
         
         case VK_ENTER:
         case VK_CHANGE_DISPLAY:
         case VK_ENTER_CHANGEDIS:
         EngKey_evt.EventBits.Key_Enter = true;
         break;
         case VK_START:
         case VK_PAUSE_START:
         EngKey_evt.EventBits.Key_Start = true;
         break;
         case VK_STOP:
         case VK_PAUSE:
         EngKey_evt.EventBits.Key_Stop = true;
         break;
         case VK_RESET:
         EngKey_evt.EventBits.Key_Reset = true;
         break;
         
         case VK_ENG:
         EngKey_evt.EventBits.Key_ENG = true;
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
         EngKey_evt.EventBits.Key_QuickKey = true;
         break;
         case VK_SAVE:
         EngKey_evt.EventBits.Key_Save = true;
         break;
         default:
         {
            if(engCurrentIndex == ENG0_DIS && EngSetting_Index == SET_ENG0_0_TestItems)
            {
                 EngKey_evt.EventBits.Key_QuickKey = true;   
            }
         }
         break;
     }
     
     VK_Value = TempKey;
		 
     return TempKey;
}
/*******************************************************************************
* Function Name  : EngineerMode_AutoCalibration()
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EngineerMode_AutoCalibration(void)
{
    unsigned char  by_Status=0xa0;
    static unsigned char  by_AutoCheck=1;
    static unsigned char  by_AutoCheckNumber=0;
    static unsigned short Old_ADC=0;
    static unsigned char  by_2msScan=0;
    
    
    MACHINE_INFO machineinfo = SystemConfig_Get_MachineInfo();
    Debug_incline = Data_Get_Incline(REAL_INCLINE_ADC);
    
    l_DATA = SystemConfig_Get_Int(MAX_INCLINEADC_UINT16);
    by_2msScan++;
    if(by_2msScan>=2)
    {
        by_2msScan = 0;
    if(1 == by_AutoCheck)
    {
        if(Data_Get_Incline(REAL_INCLINE_ADC) > Old_ADC+(2*128))
        {
            Old_ADC=Data_Get_Incline(REAL_INCLINE_ADC);
            Engs_event.InclineAutoTest = true ;
        }
        else
        {
            by_AutoCheckNumber++;
            if(by_AutoCheckNumber == 3)
            {
                if(Data_Get_Incline(REAL_INCLINE_ADC) > SystemConfig_Get_Int(MIN_INLCINEADC_UINT16)+(120*128))
//                if(Data_Get_Incline(REAL_INCLINE_ADC) > SystemConfig_Get_Int(MAX_INCLINEADC_UINT16))
                {//==>确保范围值
                    //                    Console_HighLowADC(Data_Set,1,Data_Get_Incline(CURRENT_INCLINE_ADC)-25*128);//(5*128)
                    machineinfo.maxInclineADC = Data_Get_Incline(REAL_INCLINE_ADC)-25*128;
                    Engs_event.InclinePassTest=true;
                }
                else
                {
                    by_AutoCheck = 1;
                    by_AutoCheckNumber = 0;
                    Engs_event.InclineFailTest=true;
                    Old_ADC = 0;
                    AutoInclineTestStart = 0;
//                    Led_Set_Reset();  
                }   
                LCB_Send_Cmd(DS_INC_ACTION,0);//==>升降停止
            }
            else if(by_AutoCheckNumber == 5)
            {
                by_AutoCheckNumber=0;
                by_AutoCheck = 2;
//                LCB_Send_Cmd(DS_INC_ACTION,0xff);//==>升降下降
                LCB_Send_Cmd(DS_INC_LOCATION,0);
            }
        }
        //            Eng_isr_TimerCounterClear(0);
    }
    else if(by_AutoCheck==2)
    {
        if(Data_Get_Incline(REAL_INCLINE_ADC) < Old_ADC-(2*128))
        {
            Old_ADC=Data_Get_Incline(REAL_INCLINE_ADC);
            Engs_event.InclineAutoTest = true ;
        }
        else
        {
            by_AutoCheckNumber++;
            if(by_AutoCheckNumber > 2)
            {
                if(Data_Get_Incline(REAL_INCLINE_ADC) < SystemConfig_Get_Int(MAX_INCLINEADC_UINT16)-(120*128))
                {//==>确保范围值
                    //                    Console_HighLowADC(Data_Set,0,Digtial_Get_InclineLocation()+13*128);//(9*128)
                    machineinfo.minInclineADC = Data_Get_Incline(REAL_INCLINE_ADC)+25*128;
                    if(SystemConfig_Set_MachineInfo(machineinfo)) 
                    {
                        SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
                    }
                    //                        Auto Calibration PASS
                    Engs_event.InclinePassTest=true;
                    Old_ADC = 0;
                    by_AutoCheckNumber=0;
                    AutoInclineTestStart = 0;
                    by_AutoCheck=1;
                }
                else
                {
                    by_AutoCheck=1;
                    Engs_event.InclineFailTest=true;
                    Old_ADC = 0;
                    AutoInclineTestStart = 0;
                    Led_Set_Reset();  
                }   
                Led_Set_Reset();  
                LCB_Send_Cmd(DS_INC_ACTION,0);//==>升降停止
            }
        }
    }
    else if(by_AutoCheck==3)
    {
        
        //Auto Calibration FAIL
    } 
    }
}
ENG_EVENT Engmode_Process(ReturnEventHandler flag)
{
	ENG_EVENT Result =ENG_EVENT_NONE;
	
	if(flag < RET_PROCESS_DISPLAY)
	return Result;
	
	if(flag == RET_REFRESH_DISPLAY)
	{
		return g_ENGPreEvent;
	}
	
	if(EngSetting_Index > ENG_SET_IDEL  && EngSetting_Index!=SET_ENG8_4_QuickStandbyMode && EngSetting_Index!=SET_ENG1_3_Wifi)
	{
		Result = ENG_EVENT_PROCESSED;
	}

     EngArray[engCurrentIndex].Setup(EngKey_evt);

	if(Engs_event.DisplayTest)
        Result=DISPLAY_ENG_EVENT;  
    else if(Engs_event.AllDotsOn)
        Result=ALL_DOTSON_ENG_EVENT;  
    else if(Engs_event.AllDotsOff)
        Result=ALL_DOTSOFF_ENG_EVENT;  
    else if(Engs_event.DisNumber)
        Result=ENG0_DIS_NUMBER;  
    else if(Engs_event.DisChar)
        Result=ENG0_DIS_CHAR; 
    else if(Engs_event.DisRowScan)
        Result=ENG0_DIS_ROWSCAN;
    else if(Engs_event.DisKeyValue)
        Result=ENG0_DIS_KEY_VALUE;
		
    else if(Engs_event.HardwareTest) 
        Result=HARDWARE_ENG_EVENT; 
    else if(Engs_event.BasicFunctions)
        Result=BASICFUNCTIONS_ENG_EVENT;
    else if(Engs_event.AutoADCFunctions)
        Result=AUTO_ADC_ENG_EVENT;
    else if(Engs_event.InclineAutoTest)
        Result=AUTO_INCLINE_ENG_EVENT;
    else if(Engs_event.InclinePassTest)
        Result=AUTO_INCLINE_PASS;
    else if(Engs_event.InclineFailTest)
        Result=AUTO_INCLINE_FAIL;
    else if(Engs_event.Usbport)
        Result=USB_ENG_EVENT;
    else if(Engs_event.Usbtesting)
        Result=USB_TESTING_ENG_EVENT;
    else if(Engs_event.Wifi)
        Result=WIFI_ENG_EVENT;
    else if(Engs_event.Wifi_Testing)
        Result=WIFI_TESTING_ENG_EVENT;
    else if(Engs_event.NotAvailable)
        Result=NOTAVAILABLE_ENG_EVENT;
    
    else if(Engs_event.SwitchFunction)
        Result=SWITCH_ENG_EVENT; 
    else if(Engs_event.StandardorMetric)
        Result=UNIT_ENG_EVENT;
    else if(Engs_event.LanguageSelection)
        Result=LANGUAGE_ENG_EVENT;  
    else if(Engs_event.MachineSelection)
        Result=MACHINE_ENG_EVENT;
    else if(Engs_event.ModelSelection)
        Result=MODEL_ENG_EVENT; 
    else if(Engs_event.EnergyMode)
        Result=ERP_ENG_EVENT; 
    else if(Engs_event.FirstBoot)
        Result=BOOT_ENG_EVENT;
//	else if(Engs_event.SpeedSwitch)
//        Result=OVERSPEEDFLAG_ENG_EVENT;
	else if(Engs_event.ErrorCode)
        Result=ERR_ENG_EVENT;
    else if(Engs_event.ControlZone)
        Result=CONTROL_ENG_EVENT;
    else if(Engs_event.SecondControlZone)
        Result=SECOND_CONTROL_ENG_EVENT;
    else if(Engs_event.ShowControlZoneData)
        Result=SHOW_CONTROL_ENG_EVENT;
    else if(Engs_event.IrSensor)
        Result=IR_SENSOR_ENG_EVENT;
    else if(Engs_event.MaxLevel)
        Result=MAX_LEVEL_ENG_EVENT;
    else if(Engs_event.INFORMATION)
        Result=INFORMATION_ENG_EVENT; 
    else if(Engs_event.AccumulatedInf)
        Result=ACCINF_ENG_EVENT;  
    else if(Engs_event.Rscu)
        Result=RSCU_ENG_EVENT;
    else  if(Engs_event.UcbVersion)
        Result=UCBVERSION_ENG_EVENT;
    else if(Engs_event.McbVersion)
        Result=MCBVERSION_ENG_EVENT;
    
    else if(Engs_event.DAPI)
        Result=DAPI_ENG_EVENT;
    else if(Engs_event.setdapi)
        Result=SETDAPI_ENG_EVENT;
    
    else if(Engs_event.Test)
        Result=TEST_ENG_EVETN;  
    else if(Engs_event.ProgramAccelerator)
        Result=QUICK_PRO_ENG_EVENT;

    else  if(Engs_event.ClearUserData)
        Result=CLEAR_ENG_EVENT;  
    else if(Engs_event.ClearedUserData)
        Result=CLEARED_ENG_EVENT;  
    else if(Engs_event.BurnCheck)
        Result=BURN_ENG_EVENT; 
    else if(Engs_event.ERPTime)
        Result=ERP_WAIT_TIME; 
    else if(Engs_event.QuickStandbyMode)
        Result=QUICK_ERP_ENG_EVENT;  
    else if(Engs_event.Eng_Quitted)
        Result=QUIT_ENG_EVENT;
    else if(Engs_event.ENG1RefreshData)
        Result=BASICFUNCTIONS_ENG_EVENT;
	else if(Engs_event.Setted_RPM)
		Result=SET_RPM;
	else if(Engs_event.SetOverSpeedValue)
		Result=OVERSPEEDVALUE_EVENT;
    if(Result >ENG_EVENT_PROCESSED)
  	{
		g_ENGPreEvent = Result;
	}
    Engs_event.event = 0 ;
    return Result;
}

UINT16 Engmode_Get_SetSpeedRPM(void)
{
    return(Eng1_Set_SpeedRPM);
}

UINT16 Engmode_Get_SetInclineADC(void)
{
    if(LCB_Get_MCBType()==MCB_DELTA_COMMOCIAL||(LCB_Get_MCBType()==MCB_TOPTEK)||(LCB_Get_MCBType()==MCB_DELTA))
        return(Eng1_Set_InclineADC/128);
    else
        return(Eng1_Set_InclineADC);
}
UINT16 Engmode_Get_SetECBCount(void)
{
    return(Eng1_Set_ECBCount);
}

UINT8 Engmode_Get_PROG_Mult_Times(void)
{
    return(PROG_Mult_Times);
}

VR_KEY Engmode_Get_KeyValue(void)
{
    return(VK_Value);
}

UINT8 Engmode_Get_Number(void)
{
    return(ENG0Test_Number);
}

UINT8 Engmode_Get_CharIndex(void)
{
    return(ENG0Test_CharIndex);
}
UINT8 Engmode_Get_MaxLevel(void)
{
    return(EngMaxLevel);
}
UINT8 Engmode_Get_ControlZoneData(void)
{
    return(EngControlZoneData);
}
UINT16 Engmode_Get_1StControlZoneData(void)
{
    return(Eng1StControlZoneData);
}                                    
UINT16 Engmode_Get_2NdControlZoneData(void)
{
    return(Eng2NdControlZoneData);
}                                     
UINT16 Engmode_Get_RealControlZoneData(void)
{
    return(EngRealControlZoneData);
}                                      