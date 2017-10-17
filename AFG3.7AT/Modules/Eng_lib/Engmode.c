#include "Engmode_Inner.h"
#include "UtilitiesKeyboard.h"
#include "Events_Handler.h"
#include "Engmode.h"

//#include "Setting_Config.h"
#include <string.h>
//__no_init static PROGRAM_STATUS Prg_Status,Old_Status;  
__no_init static struEngmode EngArray[ENG_TOTALS];
//__no_init static UCHAR engArrayIndex;
__no_init static UCHAR engCurrentIndex;
//__no_init static UCHAR Profile_Pos;
__no_init static const UCHAR *EngSetting_Array;
__no_init static ENG_SETTING_ENUM EngSetting_Index;
__no_init static UCHAR EngKey_event;
__no_init static ENG_SETTING_ENUM EngSetting_Chart;
//__no_init static ENG_EVENT Eng_event;
static UINT8 unit = 0;
__no_init VR_KEY  VK_Value;
__no_init ENGKeyEvts  EngKey_evt;

#define MAX_CEB_COUNT 310
#define MIN_CEB_COUNT 0

static UINT8 ENG0Test_Number;
UINT16 Eng_timer_counter;
UINT16 Eng1_Set_SpeedRPM;
UINT16 Eng1_Set_InclineADC;
UINT16 Eng1_Set_ECBCount;
UINT8 PROG_Mult_Times;
UINT8 PROG_Setted_RPM;
extern const UINT16 Models_Default[16*12];
extern UINT8 Bluetooth_Get_ConnectStatus(void);

__no_init static union
{
     struct
     {
          uint64_t DisplayTest:1;
          uint64_t AllDotsOn:1;
          uint64_t AllDotsOff:1;
		  uint64_t DisNumber:1;	
		  uint64_t DisKeyValue:1;	
          //eng1
          uint64_t HardwareTest:1;
          uint64_t BasicFunctions:1;
          uint64_t ENG1RefreshData:1;
          uint64_t Usbport:1;
          uint64_t Usbtesting:1;
          uint64_t Wifi:1;
          uint64_t Wifi_Testing:1;
		uint64_t Show_Mac:1;
        uint64_t Show_BtVersion:1;
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
          //eng4
          uint64_t INFORMATION:1;
          uint64_t AccumulatedInf:1;
          uint64_t UcbVersion:1;
          uint64_t McbVersion:1;
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
    AVAIABLE_MACHINES_TOTAL
}AVAIABLE_MODELS;


const unsigned char ENG0_SetUp[]=
{
     //NO_SETTING,
     SET_ENG0_0_DisplayTest,
     SET_ENG0_0_TestItems,
     //SET_ENG0_Total
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
//     engArrayIndex = 0;
     engCurrentIndex = 0;
     EngSetting_Index = ENG_NO_SETTING ;
     EngKey_evt.All_KeyEvents = 0 ;
     
//     Eng_event=NO_ENG_EVENT;
     EngKey_event=ENG_NONE_KEY_EVENT;
     
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

void Eng_1ms_ISR(void)
{
    if(engCurrentIndex==ENG1_HW && (EngSetting_Index==SET_ENG1_1_HWTestStarted\
        ||EngSetting_Index ==SET_ENG1_1_BasicFunctions)) 
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
}

void Engmode_Regist_Function(void)
{
    Hal_Regist_ISR(MS_ISR,ENG_ISR_IDX,Eng_1ms_ISR);
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

//ENG_SETTING_ENUM Engmode_SetUp(UCHAR key)
//{
//     switch(key)
//     {
//         case ENG_ENTER_KEY_EVENT:
//         {
//             EngSetting_Index++;   
//             
//             switch(engCurrentIndex)
//             {
//                 case ENG0_DIS:        if(EngSetting_Index >= sizeof(ENG0_SetUp))     EngSetting_Index =SET_ENG0_1_AllDotsOn; break;
//                 case ENG1_HW:         if(EngSetting_Index >= sizeof(ENG1_SetUp))     EngSetting_Index =SET_ENG0_1_AllDotsOn;break;
//                 case ENG2_AVAILABLE:  if(EngSetting_Index >= sizeof(ENG2_SetUp))     EngSetting_Index =ENG_NO_SETTING;break;
//                 case ENG3_SWITCH:     if(EngSetting_Index >= sizeof(ENG3_SetUp))     EngSetting_Index =SET_ENG0_1_AllDotsOn;break;
//                 case ENG4_INF:        if(EngSetting_Index >= sizeof(ENG4_SetUp))     EngSetting_Index =SET_ENG0_1_AllDotsOn;break;
//                 case ENG5_DAPI:       if(EngSetting_Index >= sizeof(ENG5_SetUp))     EngSetting_Index =SET_ENG0_1_AllDotsOn;break;
//                 case ENG8_TEST:       if(EngSetting_Index >= sizeof(ENG8_SetUp))     EngSetting_Index =SET_ENG0_1_AllDotsOn;break; 
//                 default: EngSetting_Index =ENG_NO_SETTING; break; 
//             } 
//             
//             break; 
//         }
//         default: break;
//     }
//     //}
//     return (ENG_SETTING_ENUM)EngSetting_Array[EngSetting_Index];
//}

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


ENG_EVENT Engmode_Get_ProEvent(void)
{
    ENG_EVENT Result = NO_ENG_EVENT;
    if(Engs_event.DisplayTest)
        Result=DISPLAY_ENG_EVENT;  
    else if(Engs_event.AllDotsOn)
        Result=ALL_DOTSON_ENG_EVENT;  
    else if(Engs_event.AllDotsOff)
        Result=ALL_DOTSOFF_ENG_EVENT;  
    else if(Engs_event.DisNumber)
        Result=ENG0_DIS_NUMBER;  
    else if(Engs_event.DisKeyValue)
        Result=ENG0_DIS_KEY_VALUE;
    
    else if(Engs_event.HardwareTest) 
        Result=HARDWARE_ENG_EVENT; 
    else if(Engs_event.BasicFunctions)
        Result=BASICFUNCTIONS_ENG_EVENT;
    else if(Engs_event.Usbport)
        Result=USB_ENG_EVENT;
    else if(Engs_event.Usbtesting)
        Result=USB_TESTING_ENG_EVENT;
    else if(Engs_event.Wifi)
        Result=WIFI_ENG_EVENT;
    else if(Engs_event.Wifi_Testing)
        Result=WIFI_TESTING_ENG_EVENT;
	else if(Engs_event.Show_Mac )
        Result=ENG_SHOW_MAC_EVENT;
	else if(Engs_event.Show_BtVersion)
        Result=ENG_SHOW_BT_VERSION_EVENT;
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
    
    else if(Engs_event.INFORMATION)
        Result=INFORMATION_ENG_EVENT; 
    else if(Engs_event.AccumulatedInf)
        Result=ACCINF_ENG_EVENT;  
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
//    else if(Engs_event.ENG1RefreshData)
//        Result=BASICFUNCTIONS_ENG_EVENT;
	else if(Engs_event.Setted_RPM)
		Result=SET_RPM;
    else 
        Result = NO_ENG_EVENT;
    
    Engs_event.event = 0 ;
    return Result;
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
    if(engCurrentIndex != ENG2_AVAILABLE) return(ENG_NO_SETTING);
    
    if(keyevt.All_KeyEvents ==0) return(ENG_NO_SETTING);
    
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
        EngSetting_Index=ENG_NO_SETTING;            
        Engs_event.Eng_Quitted = true;
    } 
    EngKey_evt.All_KeyEvents = 0 ;
    return((ENG_SETTING_ENUM)EngSetting_Array[EngSetting_Index]);
}



ENG_SETTING_ENUM Eng0_Setup(ENGKeyEvts keyevt)
{
   if(engCurrentIndex != ENG0_DIS) return(ENG_NO_SETTING);
    
    if(keyevt.All_KeyEvents ==0) return(ENG_NO_SETTING);
    
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
                engCurrentIndex = ENG4_INF;
                EngArray[engCurrentIndex].Initial();
                EngSetting_Index = SET_ENG4_0_INFORMATION;
                Engs_event.INFORMATION = true;
            }
            else if(keyevt.EventBits.Key_Enter)
            {
                EngSetting_Index=SET_ENG0_0_TestItems;
                Engs_event.AllDotsOn = true ;
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                EngSetting_Index = ENG_NO_SETTING;
                Engs_event.Eng_Quitted = true;
            }
        }
        break;
        case SET_ENG0_0_TestItems:
        {
            if(keyevt.EventBits.Key_Start)
            {
                Engs_event.AllDotsOn = true;
            }
            else if(keyevt.EventBits.Key_Stop)
            {
                Engs_event.AllDotsOff = true;
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                EngSetting_Index = ENG_NO_SETTING;
                Engs_event.DisplayTest = true;
            }
			else if(keyevt.EventBits.Key_SpeedUp||keyevt.EventBits.Key_InclineUp)
			{
				if(ENG0Test_Number>=9)
					ENG0Test_Number = 0;
                else
                    ENG0Test_Number++;
				Engs_event.DisNumber = true;
			}
			else if(keyevt.EventBits.Key_SpeedDn||keyevt.EventBits.Key_InclineDn)
			{
				if(ENG0Test_Number > 0)
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
    if(engCurrentIndex != ENG1_HW) return(ENG_NO_SETTING);
    
    if(keyevt.All_KeyEvents ==0) return(ENG_NO_SETTING);
    
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
                EngSetting_Index=ENG_NO_SETTING;            
                Engs_event.Eng_Quitted = true;
            }
        }
        break;
        case SET_ENG1_1_BasicFunctions:
        {
            if(keyevt.EventBits.Key_Enter)
            {
				//EngSetting_Index=SET_ENG1_2_Usbport;
				// Engs_event.Usbport = true ;
				EngSetting_Index = SET_ENG1_4_SHOWMAC ;
				Engs_event.Show_Mac = true ;					
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
#ifdef TM				
                LCB_Send_Cmd(DS_ECB_LOCATION,Eng1_Set_ECBCount);
#else
                ECB_Set_Resistance(Data_Get_Resistance(TARGET_RESISTANCE));  
                ECB_Data_Updata();
#endif
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
//                LCB_Send_Cmd(DS_MOT_RPM,0);
//                LCB_Send_Cmd(DS_INC_ACTION,0);
//                LCB_Send_Cmd(DS_ECB_LOCATION,0);
//                
//                EngSetting_Index=SET_ENG1_2_Usbport;
//                Engs_event.Usbport = true;
            }
            else if(keyevt.EventBits.Key_Start||keyevt.EventBits.Key_Stop)
            {
                LCB_Send_Cmd(DS_MOT_RPM,0);
                LCB_Send_Cmd(DS_INC_ACTION,0);
#ifdef TM				
                LCB_Send_Cmd(DS_ECB_LOCATION,0);
#else
                ECB_Set_Resistance(1); 
                ECB_Data_Updata();
#endif		
                
                EngSetting_Index =SET_ENG1_1_BasicFunctions;
                Engs_event.BasicFunctions = true ;
            }
			else if(keyevt.EventBits.Key_Reset)
            {
            	LCB_Send_Cmd(DS_MOT_RPM,0);
                LCB_Send_Cmd(DS_INC_ACTION,0);
#ifdef TM				
                LCB_Send_Cmd(DS_ECB_LOCATION,0);
#else
                ECB_Set_Resistance(1); 
                ECB_Data_Updata();
#endif
                EngSetting_Index = SET_ENG1_0_HardwareTest;
                Engs_event.HardwareTest = true ;
            }			
            else if(keyevt.EventBits.Key_SpeedUp)
            {
                UINT16 Max_SpeedRPM_Value = (UINT16)(SystemConfig_Get_Int(MAX_KPH_RPM_UINT16)*1.1);
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
                Eng1_Set_InclineADC +=10;
                if(Eng1_Set_InclineADC>Max_InclineADC_Value)
                    Eng1_Set_InclineADC = Max_InclineADC_Value;
                LCB_Send_Cmd(DS_INC_LOCATION,Eng1_Set_InclineADC);
                Engs_event.BasicFunctions = true ;
            }
            else if((keyevt.EventBits.Key_InclineDn))
            {
                UINT16 Min_InclineADC_Value = SystemConfig_Get_Int(MIN_INLCINEADC_UINT16);
                if(Eng1_Set_InclineADC>Min_InclineADC_Value)
                    Eng1_Set_InclineADC -= 10;
                else
                    Eng1_Set_InclineADC = Min_InclineADC_Value;
                LCB_Send_Cmd(DS_INC_LOCATION,Eng1_Set_InclineADC);
                Engs_event.BasicFunctions = true;
            }
            else if(EngKey_evt.EventBits.Key_ResistanceUp)
            {
#ifdef TM
                Eng1_Set_ECBCount +=1;
                if(Eng1_Set_ECBCount>MAX_CEB_COUNT)
                    Eng1_Set_ECBCount = MAX_CEB_COUNT;
                LCB_Send_Cmd(DS_ECB_LOCATION,Eng1_Set_ECBCount);
#else
        if(Program_Get_Resistance(0)<=9)
        Data_Set_Resistance(Program_Get_Resistance(0)+1,0);
        else
        Data_Set_Resistance(10,0);  
        
        ECB_Set_Resistance(Data_Get_Resistance(TARGET_RESISTANCE)); 
        ECB_Data_Updata();

#endif		
                Engs_event.BasicFunctions = true ;
            }
            else if(EngKey_evt.EventBits.Key_ResistanceDn)
            {
#ifdef TM
               if(Eng1_Set_ECBCount>MIN_CEB_COUNT)
                    Eng1_Set_ECBCount -= 1;
                else
                    Eng1_Set_ECBCount = MIN_CEB_COUNT;
                LCB_Send_Cmd(DS_ECB_LOCATION,Eng1_Set_ECBCount);
#else
        if(Program_Get_Resistance(0)>=2)
        Data_Set_Resistance(Program_Get_Resistance(0)-1,0);
        else
        Data_Set_Resistance(1,0); 
        
        ECB_Set_Resistance(Data_Get_Resistance(TARGET_RESISTANCE)); 
        ECB_Data_Updata();
#endif		
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
            else if(keyevt.EventBits.Key_Start)
            {
                Engs_event.Usbtesting = true ;
            }
        }
        break;
#ifdef __WIFI_FUNC__
        case SET_ENG1_3_Wifi:
        {
            if(keyevt.EventBits.Key_Enter)
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
            	ConnectAPForFactoryTest();
                Engs_event.Wifi_Testing = true ;
            }
            else if(keyevt.EventBits.Key_Stop)
            {
                
            }
        }
        break;
#endif       
		case SET_ENG1_4_SHOWMAC:
		{
			 if(keyevt.EventBits.Key_Enter)
	            {
	                EngSetting_Index=SET_ENG1_4_SHOWBTVERSION;
	                Engs_event.Show_BtVersion = true ;
	            }
	            else if(keyevt.EventBits.Key_Reset)
	            {
	                EngSetting_Index = SET_ENG1_0_HardwareTest;
	                Engs_event.HardwareTest = true ;
	            }
		}
		break;
        case SET_ENG1_4_SHOWBTVERSION:
		{
			 if(keyevt.EventBits.Key_Enter)
	            {
	                EngSetting_Index=SET_ENG1_1_BasicFunctions;
	                Engs_event.BasicFunctions = true ;
	            }
	            else if(keyevt.EventBits.Key_Reset)
	            {
	                EngSetting_Index = SET_ENG1_0_HardwareTest;
	                Engs_event.HardwareTest = true ;
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
    if(engCurrentIndex != ENG3_SWITCH) return(ENG_NO_SETTING);
    
    if(keyevt.All_KeyEvents ==0) return(ENG_NO_SETTING);
    
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
                EngSetting_Index=ENG_NO_SETTING;            
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
                 //if(unit == STANDARD)
               if(SystemConfig_Get_Char(UNIT_FLAG_UCHAR)== STANDARD) //WWW
                     unit = METRIC;
                 else
                     unit = STANDARD;
                 char username[6]={'u','s','e','r',' ',0};
                 char username1[6]={'U','S','E','R',' ',0};
                 for(char i=0;i < 6;i++)
                 {
                     if(i == 0)//GUEST
                     {
                         SystemConfigProcess(SVK_TOGGLE_UNIT,&unit,sizeof(unit));
                         SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
                     }
                     else//USER 1 2 3 4 5
                     {
                         USERS_INFO usersinfo = SystemConfig_Get_UserInfo(i);
                         username[5] = 0x30+i;
                         username1[5] = 0x30+i;
                         if(strcmp(usersinfo.name,username) == 0 || strcmp(usersinfo.name,username1) == 0)
                         {
                             usersinfo.displayUnit = unit;
                             if( SystemConfig_Set_UserInfo(usersinfo, i))
                             {
                                 SystemConfigProcess(SVK_SAVE_USER2FLASH, &i, sizeof(UCHAR));
                             }
                         }
                     }
                 }
                 Engs_event.StandardorMetric = true;
             }
            if(keyevt.EventBits.Key_Enter)
            {    
            EngSetting_Index=SET_ENG3_2_LanguageSelection;
//                EngSetting_Index=SET_ENG3_5_EnergyMode;
//                Engs_event.EnergyMode = true;
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
//WWW                EngSetting_Index=SET_ENG3_3_MachineSelection;
//WWW                Engs_event.MachineSelection = true ;
              EngSetting_Index=SET_ENG3_5_EnergyMode;//WWW
                Engs_event.EnergyMode = true ;//WWW
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
                if(ENGMacheineIndex>ASCENT)
                    ENGMacheineIndex = TREADMILL;
                
//                UINT8 Default_Modeltype;
                if(ENGMacheineIndex==TREADMILL)
                    ENGMacheineIndex = 0;
                else if(ENGMacheineIndex==ELLIPTICAL)
                    ENGMacheineIndex = 5;
                else if(ENGMacheineIndex==BIKE)
                    ENGMacheineIndex = 4;
                else if(ENGMacheineIndex==ASCENT)
                    ENGMacheineIndex = 10;
                
                MODEL_DEFAULTS tempmachineinfo;
                memcpy(&tempmachineinfo,&Models_Default[ENGMacheineIndex*sizeof(tempmachineinfo)/2],sizeof(tempmachineinfo));
                
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
                
                if(ENGMacheineIndex>TREADMILL) 
                    ENGMacheineIndex--;
                else
                    ENGMacheineIndex = ASCENT;
                
//                UINT8 Default_Modeltype;
                if(ENGMacheineIndex==TREADMILL)
                    ENGMacheineIndex = 0;
                else if(ENGMacheineIndex==ELLIPTICAL)
                    ENGMacheineIndex = 5;
                else if(ENGMacheineIndex==BIKE)
                    ENGMacheineIndex = 4;
                else if(ENGMacheineIndex==ASCENT)
                    ENGMacheineIndex = 10;
                
                MODEL_DEFAULTS tempmachineinfo;
                memcpy(&tempmachineinfo,&Models_Default[ENGMacheineIndex*sizeof(tempmachineinfo)/2],sizeof(tempmachineinfo));
                
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
                MACHINE_TYPE temp_Machine=(MACHINE_TYPE)SystemConfig_Get_Char(MACHINE_TYPE_UINT8);
                UINT8 temp_Model = SystemConfig_Get_Char(MACHINE_MODLE_UINT8);
                UINT8 ParaNumber;
                if(temp_Machine==TREADMILL)
                {
                    if(temp_Model==1) ParaNumber = TM_TF50;
                    else if(temp_Model==2) ParaNumber = TM_T50;
                    else if(temp_Model==3) ParaNumber = TM_T70_C2;
                    else if(temp_Model==4) ParaNumber = TM_TF30;
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
                MACHINE_TYPE temp_Machine=(MACHINE_TYPE)SystemConfig_Get_Char(MACHINE_TYPE_UINT8);
                UINT8 temp_Model = SystemConfig_Get_Char(MACHINE_MODLE_UINT8);
                UINT8 ParaNumber;
                
                if(temp_Machine==TREADMILL)
                {
                    if(temp_Model==1) ParaNumber = TM_T70_C2 ;
                    else if(temp_Model==2) ParaNumber = TM_TF30 ;
                    else if(temp_Model==3) ParaNumber = TM_TF50;
                    else if(temp_Model==4) ParaNumber = TM_T50;
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
                while(Beep_Get_OK() == false);
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
                Beep_Initial_IO();
                Engs_event.EnergyMode = true;
            }
            if(keyevt.EventBits.Key_Enter)
            {    
                EngSetting_Index=SET_ENG3_6_FirstBoot;
                Engs_event.FirstBoot = true;
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
                 while(Beep_Get_OK() == false);
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
                 Beep_Initial_IO();
                 Engs_event.FirstBoot = true;
             }
            if(keyevt.EventBits.Key_Enter)
            {    
                EngSetting_Index=SET_ENG3_1_StandardorMetric;
                Engs_event.StandardorMetric = true ;
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
        
        default:break;
    }
    EngKey_evt.All_KeyEvents = 0 ;
    return((ENG_SETTING_ENUM)EngSetting_Array[EngSetting_Index]);
}
ENG_SETTING_ENUM Eng4_Setup(ENGKeyEvts keyevt)
{
   if(engCurrentIndex != ENG4_INF) return(ENG_NO_SETTING);
    
    if(keyevt.All_KeyEvents ==0) return(ENG_NO_SETTING);
    
    Engs_event.event = 0;
    switch(EngSetting_Index)
    {
        case SET_ENG4_0_INFORMATION:
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
                EngSetting_Index=ENG_NO_SETTING;            
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
   if(engCurrentIndex != ENG5_DAPI) return(ENG_NO_SETTING);
    
    if(keyevt.All_KeyEvents ==0) return(ENG_NO_SETTING);
    
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
                EngSetting_Index=ENG_NO_SETTING;            
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
				    strcpy( machineinfo.marioBox,JOHNSON_WIFI_SERVERIP_PRODUCTION);
                }
                
				if(SystemConfig_Set_MachineInfo(machineinfo ))
                {
                    SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH, NULL,0);
                }
#ifdef __WIFI_FUNC__               
                WifiConnectionStatusChange(0);
                WifiConnectionStatusChange(1);
#endif 
                Engs_event.setdapi = true;
            }
            else if(keyevt.EventBits.Key_Dn||keyevt.EventBits.Key_SpeedDn||\
                keyevt.EventBits.Key_InclineDn||keyevt.EventBits.Key_ResistanceDn)
            {
                MACHINE_INFO machineinfo = SystemConfig_Get_MachineInfo();
                
                if(strncmp(machineinfo.marioBox, JOHNSON_WIFI_SERVERIP_PRODUCTION, 256) == 0 )
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
				    strcpy( machineinfo.marioBox,JOHNSON_WIFI_SERVERIP_PRODUCTION);
                }
                
				if(SystemConfig_Set_MachineInfo(machineinfo ))
                {
                    SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH, NULL,0);
                }
#ifdef __WIFI_FUNC__               
                WifiConnectionStatusChange(0);
                WifiConnectionStatusChange(1);
#endif                
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
    if(engCurrentIndex != ENG8_TEST) return(ENG_NO_SETTING);
    
    if(keyevt.All_KeyEvents ==0) return(ENG_NO_SETTING);
    
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
                if(PROG_Mult_Times >= 100)
                    PROG_Mult_Times = 1;
                else
                    PROG_Mult_Times +=1;
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
                EngSetting_Index=SET_ENG8_4_QuickStandbyMode;
                Engs_event.QuickStandbyMode = true ;
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                engCurrentIndex=ENG3_SWITCH;
                EngSetting_Index = SET_ENG3_0_SwitchFunction;
                Engs_event.SwitchFunction = true ;
            }
            else if(keyevt.EventBits.Key_Start)
            {
                while(Beep_Get_OK() == false);
                SystemConfigRestoreToFactory();
                Beep_Initial_IO();
                EngSetting_Index = SET_ENG8_2_ClearUserData;
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
                EngSetting_Index=SET_ENG8_1_ProgramAccelerator;
                Engs_event.ProgramAccelerator = true ;
            }
            else if(keyevt.EventBits.Key_Reset)
            {
                engCurrentIndex=ENG3_SWITCH;
                EngSetting_Index = SET_ENG3_0_SwitchFunction;
                Engs_event.SwitchFunction = true ;
            }
            else if(keyevt.EventBits.Key_Start)
            {
                EngSetting_Index = SET_ENG8_5_ERPTime;
                Engs_event.ERPTime = true ;
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
                PROG_Setted_RPM++;
				//todo 
				//set program rpm
                Engs_event.Setted_RPM = true;
            }
            else if(keyevt.EventBits.Key_Dn||keyevt.EventBits.Key_SpeedDn||\
                keyevt.EventBits.Key_InclineDn||keyevt.EventBits.Key_ResistanceDn)
            {
                UINT16 tempERPTime =SystemConfig_Get_Int(ERP_WAIT_MIN_UINT16);
                
                 PROG_Setted_RPM--;
                //todo 
				//set program rpm
                Engs_event.Setted_RPM = true;
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
     VK_Value = VK_NULL;
     eng0mode.Run = NULL;
     Eng_Add_Array(eng0mode);
     EngSetting_Index=ENG_NO_SETTING;
}

void initEng1mode(void)
{
    struEngmode eng1mode; 
    eng1mode.Engid = ENG1_HW;
    eng1mode.Initial = Eng1_Data_Initial;
    eng1mode.Setup = Eng1_Setup;
    eng1mode.Run = EngHw_Run;
    Eng_Add_Array(eng1mode);
    EngSetting_Index=ENG_NO_SETTING;
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
     EngSetting_Index=ENG_NO_SETTING;
}
void initEng3mode(void)
{
     struEngmode eng3mode;
     eng3mode.Engid = ENG3_SWITCH;
     eng3mode.Initial = EngHw_Data_Initial;
    eng3mode.Setup = Eng3_Setup;
     eng3mode.Run = EngHw_Run;
     Eng_Add_Array(eng3mode);
     EngSetting_Index=ENG_NO_SETTING;
}
void initEng4mode(void)
{
     struEngmode eng4mode;
     eng4mode.Engid = ENG4_INF;
     eng4mode.Initial = EngHw_Data_Initial;
     eng4mode.Setup = Eng4_Setup;
     eng4mode.Run = EngHw_Run;
     Eng_Add_Array(eng4mode);
     EngSetting_Index=ENG_NO_SETTING;
}
void initEng5mode(void)
{
     struEngmode eng5mode;
     eng5mode.Engid = ENG5_DAPI;
     eng5mode.Initial = EngHw_Data_Initial;
     eng5mode.Setup = Eng5_Setup;
     eng5mode.Run = NULL;
     Eng_Add_Array(eng5mode);
     EngSetting_Index=ENG_NO_SETTING;
}
void initEng8mode(void)
{
     struEngmode eng8mode; 
     eng8mode.Engid = ENG8_TEST;
     eng8mode.Initial = EngHw_Data_Initial;
     eng8mode.Setup = Eng8_Setup;
     eng8mode.Run = NULL;
     Eng_Add_Array(eng8mode);
     EngSetting_Index=ENG_NO_SETTING;
     PROG_Mult_Times = 1;
}

bool Engmode_Set_VirtualKey(UINT16 vk)
{
     UINT16 TempKey = vk & 0xFF00 ;
     
     if((TempKey == FRAME_STAGE_USER)||(TempKey == FRAME_STAGE_PRGSETUP))
     {
         UINT8 keyValue = (UINT8)(vk&0x00FF);
         if(keyValue == VK_ENG && Bluetooth_Get_ConnectStatus() == 0)
         {
             Engmode_Initial_Data(1);
             Engs_event.DisplayTest = true;
         }
     }
     if(TempKey != FRAME_STAGE_ENG)  return 0;
     
     TempKey = (UINT8)(vk&0x00FF);
     if(TempKey == VK_NULL) return 0;
     switch(TempKey)
     {
         case VK_FAN:
         EngKey_evt.EventBits.Key_Fan = true;
         break;
         case VK_BLUETOOTH:
         EngKey_evt.EventBits.Key_BlueTooth = true;
         break;
         case VK_INCLINEUP:
         EngKey_evt.EventBits.Key_InclineUp = true;
         break;
         case VK_SPEEDUP_LONG_PRESS:
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
          
         case VK_QK_INCLINE_0:
         case VK_QK_INCLINE_5:
         case VK_QK_INCLINE_10:
         case VK_QK_INCLINE_15:
         case VK_QK_INCLINE_20:
         case VK_QK_INCLINE_25:
         case VK_QK_INCLINE_40:
         case VK_QK_INCLINE_60:
         case VK_QK_INCLINE_65:
         case VK_QK_INCLINE_80:
         case VK_QK_INCLINE_100:
         case VK_QK_INCLINE_120:
         case VK_QK_INCLINE_150:  
         case VK_QK_SPEED1:
         case VK_QK_SPEED2:
         case VK_QK_SPEED3:
         case VK_QK_SPEED4:
         case VK_QK_SPEED5:
         case VK_QK_SPEED6:
         case VK_QK_SPEED7:
         case VK_QK_SPEED8:
         case VK_QK_SPEED9:
         case VK_QK_SPEED10:
         case VK_QK_SPEED11:
         case VK_QK_SPEED12:
#ifdef EP
     case VK_QK_RESISTACE_1:
     case VK_QK_RESISTACE_2:
     case VK_QK_RESISTACE_3:
     case VK_QK_RESISTACE_4:
     case VK_QK_RESISTACE_5:
     case VK_QK_RESISTACE_6:
     case VK_QK_RESISTACE_7:
     case VK_QK_RESISTACE_8:
     case VK_QK_RESISTACE_9:
     case VK_QK_RESISTACE_10:
#endif
            EngKey_evt.EventBits.Key_QuickKey = true;
         break;
         case VR_MANUAL:
            EngKey_evt.EventBits.Key_Fan = true;
         break;
         case VR_DISTANCE:
            EngKey_evt.EventBits.Key_Fan = true;
         break;
         case VR_WEIGHT_LOSS:
            EngKey_evt.EventBits.Key_Fan = true;
         break;
         case VR_CALORIES:
            EngKey_evt.EventBits.Key_Fan = true;
         break;
         case VR_INTERVALS:
            EngKey_evt.EventBits.Key_Fan = true;
         break;
         default:
         break;
     }
     VK_Value = TempKey;
     return false;
}

ENG_EVENT Engmode_Process(void)
{
     EngArray[engCurrentIndex].Setup(EngKey_evt);

     return Engmode_Get_ProEvent();
}

UINT16 Engmode_Get_SetSpeedRPM(void)
{
    return(Eng1_Set_SpeedRPM);
}

UINT16 Engmode_Get_SetInclineADC(void)
{
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

UINT8 Engmode_Get_PROG_Setted_RPM(void)
{
    return(PROG_Setted_RPM);
}

VR_KEY Engmode_Get_KeyValue(void)
{
    return(VK_Value);
}

UINT8 Engmode_Get_Number(void)
{
    return(ENG0Test_Number);
}

UINT8 Engmode_Get_Uint(void)
{
    return unit;
}