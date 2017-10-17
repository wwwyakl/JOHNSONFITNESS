#ifndef __PROGRAM_INNER_H_
#define __PROGRAM_INNER_H_
#include "programs.h"

typedef enum
{
  TIMER_PAUSE_5MIN,
  TIMER_WORKOUTCOMPLETE,
  TIMER_QUICKKEY,
  TIMER_20S_No_RPM,
  TIMER_5S_PedalStart,
  TIMER_ScanFunction,
  TIMER_500MS_READ_STATUS,
  TIMER_END
}PRG_ISR_TIMER;

typedef struct 
{
  UINT8 SpeedProfile[30];
  UINT8 InclineProfile[30];
  UINT8 ResistanceProfile[30];
  UINT8 HightProfile[35];
}PROFILE_INEER;
typedef union
{
  struct
  {
    UINT32 Key_Up:1;
    UINT32 Key_Dn:1;
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
    UINT32 Key_QuickKey:1;
    UINT32 Key_Initial:1;
    UINT32 Key_EnterChangeDisplay:1;
    UINT32 Key_EnterEngMode:1;
    UINT32 Key_WorkoutSummary:1;
    UINT32 Key_SpeedOver:1;
    UINT32 Key_SensorError:1;
    UINT32 Key_SafekeyError:1;
    UINT32 Key_SpeedLongFlag:1;
  }EventBits; 
  UINT32 All_KeyEvents;
}ProgramKeyEvts;
typedef struct 
{
  UINT16 Data;
  UINT16 Value;
  UINT16 Counter;
  UINT8 Index;
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
    }Evt;
    UINT8 Evts;
  }Key;
}QUICK_KEY;
typedef struct
{
	UCHAR Programid;
	void (*Initial)(void);
	PROFILE* (*GetProfile)(PROFILE_TAG tag_index,UCHAR *);
	PROGRAM_PENDING_EVTS (*Setup)(ProgramKeyEvts *keyevt);
	PROGRAM_PENDING_EVTS (*Workout)(ProgramKeyEvts *keyevt);
	
}struProgram;
void Program_Initial_Inner_Data(void);
void Program_Add_Array(struProgram pro);

extern __no_init const WattsParameterStruct *_Watts_Talbelist;
extern __no_init ProgramLibParaConfig *ProgramParameter;
extern __no_init  PROFILE_INEER DotmatrixData;
extern __no_init PROGRAM_STATUS g_StateMachine;
extern __no_init  UINT8 DefaultDotmatrix;

extern __no_init SETTING_ENUM SettingState;
extern __no_init UCHAR Setting_Index;
extern __no_init UINT32 _ul_Segment_Time;
extern __no_init UINT16 w_Current_Segment;
extern __no_init UCHAR Sprint8Stage;
extern __no_init  UINT16 InductionOutput_30P[30];

extern const unsigned char ProTable_SetUp[];
extern const unsigned char ProSprint8_SetUp[];
extern const unsigned char PConstantWatt_SetUp[];
extern const unsigned char TM_Manual_SetUp[];
extern const unsigned char EP_Manual_SetUp[];
extern const unsigned char C50_Landmark_SetUp[];
extern const unsigned char EP_Incline_Manual_SetUp[];

extern const unsigned char THR_ZONE_SetUp[];
extern const unsigned char Interval_HR_SetUp[];
extern const unsigned char Steps_HR_SetUp[];
#endif
