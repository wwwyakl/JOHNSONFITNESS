#ifndef __PROGRAM_INNER_H_
#define __PROGRAM_INNER_H_
#include "programs.h"
//#define PRO_SETUP_DOUBLE_DISPLAY
typedef enum
{
  TIMER_PAUSE_5MIN,
  TIMER_WORKOUTCOMPLETE,
  TIMER_QUICKKEY,
  TIMER_20S_No_RPM,
  TIMER_5S_PedalStart,
  TIMER_ScanFunction,
  TIMER_END
}PRG_ISR_TIMER;
typedef enum
{
  INDEX_START,
  INDEX_TIME,
  INDEX_INCLINE,
  INDEX_SPEED,
  INDEX_RESISTANCE,  
  INDEX_DISTANCE,
  INDEX_SEGMENT,
  INDEX_CALORIES,
  INDEX_STEPS,
  INDEX_CLIMB,
  INDEX_HEART,
  INDEX_WATT,
  INDEX_WEEK,
  INDEX_WORKOUT,
  INDEX_WEEKBUFF,
  INDEX_WORKOUTBUFF,
  INDEX_DISTANCE_LABLE,
  INDEX_END,
}DataIndex;

typedef struct 
{
  UINT8 SpeedProfile[30];
  UINT8 InclineProfile[30];
  UINT8 ResistanceProfile[30];
}Prg_ProfileStruct;
typedef struct
{
    UINT32 Key_Up:1;
    UINT32 Key_Dn:1;
    UINT32 key_update:1;
    UINT32 Key_Enter:1;
    UINT32 Key_Back:1;
    UINT32 Key_SpeedUp:1;
    UINT32 Key_SpeedDn:1;
    UINT32 Key_InclineUp:1;
    UINT32 Key_InclineDn:1;
    UINT32 Key_ResistanceUp:1;
    UINT32 Key_ResistanceDn:1;
    UINT32 Key_CoolDn:1;
    UINT32 Key_StateChange:1;
    UINT32 Key_Pause:1;
    UINT32 Key_Reset:1;
    UINT32 Key_ChangeDisplay:1;
    UINT32 Key_NumberKey:1;
    UINT32 Key_Initial:1;
    UINT32 Key_EnterChangeDisplay:1;
    UINT32 Key_QuickKey:1;
    UINT32 Key_SPDQuickKey:1;
    UINT32 Key_INCQuickKey:1;
    UINT32 Key_RESQuickKey:1;
    UINT32 Key_Manual:1;
    UINT32 Key_WeightLoss:1;
    UINT32 Key_Intervals:1;
    UINT32 Key_Distance:1;
    UINT32 Key_Calories:1;
}KeyEvtBitFlag;
typedef union
{
	KeyEvtBitFlag Bit_Flag; 
	UINT32 Bit_All;
}ProgramKeyEvts;
typedef struct 
{
  UINT16 InputValue;
  UINT16 ShowValue;
  UINT16 Timer_Counter;
  UINT8 ShowBitNumber;
  union
  {
    struct
    {
      UINT8 Spd:1;
      UINT8 Inl:1;
      UINT8 Res:1;
      UINT8 Time:1;
      UINT8 Level:1;
      UINT8 Heart:1;
      UINT8 Watt:1;
      UINT8 Recover:1;
    }Bit;
    UINT8 Bits_All;
  }Events;
}NumberKeyData;
typedef struct
{
	UINT8 Programid;
	const UINT8  *Setting_Array;
	UINT8 SettingArray_Index;
	void (*Prg_Initial)(void);
	void(*BeginWorkout)(void);
	PROFILE* (*Prg_ProfileShow)(PROFILE_TAG index,UCHAR *);
	PROGRAM_PENDING_EVTS (*Prg_Setup)(ProgramKeyEvts *keyevt);
	PROGRAM_PENDING_EVTS (*Prg_Workout)(ProgramKeyEvts *keyevt);
	void (*Prg_NextStatus)(PROGRAM_STATUS NextPrgStatus);
	PROGRAM_STATUS (*Prg_CurrentStatus)(void);
	UINT16 (*Prg_SetupData)(UINT8 ParaIndex);
	UINT16 (*Prg_RunData)(UINT8 ParaIndex);

}struProgram;
void Program_Initial_Inner_Data(void);
void Program_Register_Function(struProgram *pro);

extern __no_init const WattsParameterStruct *Watts_ParameterData;
extern __no_init ProgramLibPara *ProgramLibConfigPara;;
extern __no_init  Prg_ProfileStruct Prg_ProfileData;
extern __no_init  UINT8 DefaultDotmatrix;
extern __no_init  UINT8 ProfileIndex;
extern __no_init  UINT16 InductionOutput_30P[30];

extern const unsigned char ProTable_TimeSetUp[];
extern const unsigned char TM_ProTable_CaloriesSetUp[];
extern const unsigned char EP_ProTable_CaloriesSetUp[];
extern const unsigned char TM_ProTable_DistanceSetUp[];
extern const unsigned char EP_ProTable_DistanceSetUp[];

extern const unsigned char ProSprint8_SetUp[];
extern const unsigned char PConstantWatt_SetUp[];
extern const unsigned char TM_Manual_SetUp[];
extern const unsigned char EP_Manual_SetUp[];
extern const unsigned char EP_Incline_Manual_SetUp[];
extern const unsigned char TM_Goal_DistanceSetUp[];
extern const unsigned char EP_Goal_SetUp[];
extern const unsigned char TM_Calories_SetUp[];
extern const unsigned char EP_Incline_Calories_SetUp[];
extern const unsigned char EP_Calories_SetUp[];
extern const unsigned char TM_Distance_SetUp[];
extern const unsigned char EP_Incline_Distance_SetUp[];
extern const unsigned char EP_Distance_SetUp[];

extern const unsigned char THR_ZONE_SetUp[];
extern const unsigned char Interval_HR_SetUp[];
extern const unsigned char Steps_HR_SetUp[];

extern const unsigned char TM_ProMyFirst5k_SetUp[];
extern const unsigned char EP_ProMyFirst5k_SetUp[];
extern const unsigned char TM_CustomTime_SetUp[];
extern const unsigned char EP_CustomTime_SetUp[];
extern const unsigned char TM_CustomDistance_SetUp[];
extern const unsigned char EP_CustomDistance_SetUp[];

extern const unsigned char TM_CustomHr_SetUp[];

#endif
