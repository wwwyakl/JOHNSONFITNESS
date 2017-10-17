#ifndef __Engmode_H__
#define __Engmode_H__


#include "wifi_interface.h"

#include "Data_Process.h"
#include "SystemConfig.h"
#include "HAL_Lib.h"
#include "LCB.h"

#define TOTAL_LANGUAGE 8

void Engmode_Initial_Data(/*TableStruct *array[],*/UINT8 size);

typedef union
{
  struct
  {    
    UINT16 Key_Up:1;
    UINT16 Key_Dn:1;
    UINT16 Key_SpeedUp:1;
    UINT16 Key_SpeedDn:1;
    
    UINT16 Key_InclineUp:1;
    UINT16 Key_InclineDn:1;
    UINT16 Key_ResistanceUp:1;
    UINT16 Key_ResistanceDn:1;
    
    UINT16 Key_Enter:1;
    UINT16 Key_Start:1;
    UINT16 Key_Stop:1;
    UINT16 Key_Reset:1;
    
    UINT16 Key_ENG:1;
    UINT16 Key_QuickKey:1;
    UINT16 Key_RefreshData:1;
    UINT16 Key_Save:1;
  }EventBits; 
  UINT16 All_KeyEvents;
}ENGKeyEvts;

typedef enum
{
     ENG0_DIS,
     ENG1_HW,
     ENG2_AVAILABLE,
     ENG3_SWITCH,
     ENG4_INF,
     ENG5_DAPI,
     ENG8_TEST,
     ENG_TOTALS
}ENG_ID;

typedef enum
{
	 ENG_SET_START,
     ENG_SET_IDEL = ENG_SET_START ,	
     SET_ENG0_0_DisplayTest,
     SET_ENG0_0_TestItems,
     /*SET_ENG0_1_AllDotsOn,
     SET_ENG0_2_AllDotsOff,
     SET_ENG0_3_Seg1,
     SET_ENG0_4_Seg2,
     SET_ENG0_5_Seg3,*/
     //eng1
     SET_ENG1_0_HardwareTest,
     SET_ENG1_1_BasicFunctions,
     SET_ENG1_1_HWTestStarted,
     SET_ENG1_2_Usbport,
     SET_ENG1_3_Wifi,
     SET_ENG1_4_AutoADC,
     //eng2
     SET_ENG2_0_NotAvailable,
     //eng3
     SET_ENG3_0_SwitchFunction,
     SET_ENG3_1_StandardorMetric,
     SET_ENG3_2_LanguageSelection,
     SET_ENG3_3_MachineSelection,
     SET_ENG3_4_ModelSelection,
     SET_ENG3_5_EnergyMode,
     SET_ENG3_6_FirstBoot,
//     SET_ENG3_7_OverSpeed,
     SET_ENG3_7_ErrorCode,
     SET_ENG3_8_ControlZone,
     SET_ENG3_8_0_SecondCotrolZone,
     SET_ENG3_8_1_ShowCotrolZoneData,
     SET_ENG3_9_IrSensor,

     SET_ENG3_10_MaxLevel,
     //eng4
     SET_ENG4_0_INFORMATION,
     SET_ENG4_1_AccumulatedInf,
     SET_ENG4_2_UcbVersion,
     SET_ENG4_3_McbVersion,
     SET_ENG4_4_Rscu, 
     //eng5
     SET_ENG5_0_DAPI,
     SET_ENG5_1_SetDapi,

     //eng8
     SET_ENG8_0_Test,
     SET_ENG8_1_ProgramAccelerator,
     SET_ENG8_2_ClearUserData,
     SET_ENG8_3_BurnCheck,
     SET_ENG8_4_QuickStandbyMode,
     SET_ENG8_5_ERPTime,
     SET_ENG8_6_RPM,
     SET_ENG8_7_OverSpeedValue,
     SET_ENG_TOTAL
          
}ENG_SETTING_ENUM;






typedef enum
{
     ENG_EVENT_NONE=0x0,
	 ENG_EVENT_PROCESSED,	 	 	
     //eng0
     DISPLAY_ENG_EVENT,
     ALL_DOTSON_ENG_EVENT,
     ALL_DOTSOFF_ENG_EVENT,
     ENG0_DIS_NUMBER,
     ENG0_DIS_CHAR,
     ENG0_DIS_ROWSCAN,
     ENG0_DIS_KEY_VALUE,
     //SEG1_ENG_EVENT,
     //SEG2_ENG_EVENT,
     //SEG3_ENG_EVENT,
     //eng1
     HARDWARE_ENG_EVENT,
     BASICFUNCTIONS_ENG_EVENT,
     AUTO_ADC_ENG_EVENT,
     AUTO_INCLINE_ENG_EVENT,
     AUTO_INCLINE_PASS,
     AUTO_INCLINE_FAIL,
     USB_ENG_EVENT,
     USB_TESTING_ENG_EVENT,
     WIFI_ENG_EVENT,
     WIFI_TESTING_ENG_EVENT,
     //eng2
     NOTAVAILABLE_ENG_EVENT,
     //eng3
     SWITCH_ENG_EVENT,
     UNIT_ENG_EVENT,
     LANGUAGE_ENG_EVENT,
     MACHINE_ENG_EVENT,
     MODEL_ENG_EVENT,
     ERP_ENG_EVENT,
     BOOT_ENG_EVENT,
     ERR_ENG_EVENT,
     CONTROL_ENG_EVENT,
     SECOND_CONTROL_ENG_EVENT,
     SHOW_CONTROL_ENG_EVENT,
     IR_SENSOR_ENG_EVENT,
     MAX_LEVEL_ENG_EVENT,
//     OVERSPEEDFLAG_ENG_EVENT,
     //eng4
     INFORMATION_ENG_EVENT,
     ACCINF_ENG_EVENT,
     RSCU_ENG_EVENT,
     UCBVERSION_ENG_EVENT,
     MCBVERSION_ENG_EVENT,
     
     //eng5
     DAPI_ENG_EVENT,
     SETDAPI_ENG_EVENT,
     //eng8
     TEST_ENG_EVETN,
     QUICK_PRO_ENG_EVENT,
     CLEAR_ENG_EVENT,
     CLEARED_ENG_EVENT,
     BURN_ENG_EVENT,
     ERP_WAIT_TIME,
     QUICK_ERP_ENG_EVENT,
     QUIT_ENG_EVENT,
     SET_RPM,
     OVERSPEEDVALUE_EVENT,
     ENG_EVENT_TOTAL
}ENG_EVENT;
typedef enum
{
  TIMER_10MS,
  TIMER_END
}ENG_ISR_TIMER;

void Engmode_Regist_Function(void);
void Eng_Select_ID(ENG_ID erg_Type);
VR_KEY Engmode_Set_VirtualKey( VR_KEY TempKey);
ENG_SETTING_ENUM Engmode_Get_SettingFlow(void);
ENG_EVENT Engmode_Process(ReturnEventHandler flag);
UINT16 Engmode_Get_SetECBCount(void);
UINT16 Engmode_Get_SetInclineADC(void);
UINT16 Engmode_Get_SetSpeedRPM(void);
UINT8 Engmode_Get_PROG_Mult_Times(void);
VR_KEY Engmode_Get_KeyValue(void);
UINT8 Engmode_Get_Number(void);
UINT8 Engmode_Get_CharIndex(void);
UINT8 Engmode_Get_MaxLevel(void);
UINT8 Engmode_Get_ControlZoneData(void);
void EngineerMode_AutoCalibration(void);
UINT16 Engmode_Get_1StControlZoneData(void);
UINT16 Engmode_Get_2NdControlZoneData(void);
UINT16 Engmode_Get_RealControlZoneData(void);
#endif
