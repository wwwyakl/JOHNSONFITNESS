#ifndef __PROGRAMS_H__
#define __PROGRAMS_H__
#include "Data_Process.h"
#include "SystemConfig.h"
#include "LCB.h"
#include "UtilitiesKeyboard.h"
#include "wifi_interface.h"
#include "User.h"
#include "LCB.h"

#define MAX_WORKOUT_TIME  36000

#define __PROGRAM_MANUAL__
#define __PROGRAM_HRC__
#define __PROGRAM_TABLE__
#define __PROGRAM_CONSTANT__
#define __PROGRAM_SPRINT8__
#define __PROGRAM_GOAL__

#define BIT0                (0x00000001)
#define BIT1                (0x00000002)
#define BIT2                (0x00000004)
#define BIT3                (0x00000008)
#define BIT4                (0x00000010)
#define BIT5                (0x00000020)
#define BIT6                (0x00000040)
#define BIT7                (0x00000080)
#define BIT8                (0x00000100)
#define BIT9                (0x00000200)
#define BITA                (0x00000400)
#define BITB                (0x00000800)
#define BITC                (0x00001000)
#define BITD                (0x00002000)
#define BITE                (0x00004000)
#define BITF                (0x00008000)

enum{
    PROFILE_TYPE_NULL=0,
    SPEED_TYPE,
    INCLINE_TYPE,
    RESISTANCE_TYPE,
    HIGHT_TYPE,
    RUN_HIGHT_TYPE,
    PROFILE_TYPE_END
};

typedef enum{
    GOAL_TYPES_START,
        
    GOAL_TYPES_NONE = GOAL_TYPES_START,
    GOAL_TYPES_TIME,/*unit Seconds*/
    GOAL_TYPES_DISTANCE,/*unit KM*/
    GOAT_TYPES_CALORIES,/*unit kCal*/
    GOAL_TYPES_FLOORS,/*unit floors*/
    GOAL_TYPES_CUSTOM = 6,
    GOAL_TYPES_STEPS,

    GOAL_TYPES_END
}workoutGoalTypes;
typedef enum{
 HOME_NORMAL,
 TIME_AVARAGE_TABLE_FIX,
 TIME_TABLE_FIX   
}TableType;
typedef struct 
{
  UINT8 *ProfileData;
  UINT8 Length;
}PROFILE;

typedef struct{
   UINT8 Table_Type;	
   UINT8 ProgramID;
   UINT8 Max_Level;
   UINT8 Max_Segment;
   UINT8 DefalultProfle_Type;	
   const UINT8 *StatusTime;
   const UINT8 *Speed_table;
   const UINT8 *Incline_table;
   const UINT8 *Resistance_table;
}TableStruct;
typedef struct
{
//  UINT8 IsExistIncline;
  const UINT8 *ProgramidArray;
  UINT8 ProgramNum;
  const TableStruct **TablePara;
  UINT8 PtableNum;
  UINT8 DotMatrixLength;
}ProgramLibParaConfig;

typedef struct{
UCHAR ResistanceType;
UCHAR Minrpm;
UCHAR Maxrpm;
UCHAR Rows;
UCHAR Columns;
const UINT16 *PWM_ECBCount_Table;
const UINT16 *Watts_Table;	 
}WattsParameterStruct;

typedef enum
{
	NO_SETTING = 0,
	SET_PROGRAM,
	SET_TIME,
	SET_SPEED,
	SET_INCLINE,
	SET_RESISTANCE,
    SET_TARGET,
	SET_LEVEL,
	SET_HEART,
	SET_WATTS,
	
	SET_WORKINTERVAL_TIME,
	SET_WORKINTERVAL_HEART,
	SET_RESTINTERVAL_TIME,
	SET_RESTINTERVAL_HEART,

	SET_SEGMENT_TIME,
	SET_SEGMENT_1_HEART,
	SET_SEGMENT_2_HEART,
	SET_SEGMENT_3_HEART,
	SET_SEGMENT_4_HEART,
        SET_PRESS_START
}SETTING_ENUM;

typedef enum
{
	PROGRAM_NONE_EVENT=0x0,
	PROGRAM_EVENT_PROCESSED,
	PROGRAM_EVENT_ENTER_ENGMODE,
	BACK_END_EVENT,
	PROGRAM_CHANGE_EVENT,
	TIME_CHANGE_EVENT,
    TARGET_CHANGE_EVENT,
	HEART_CHANGE_EVENT,
	WATTS_CHANGE_EVENT,
	CALORIES_CHANGE_EVENT,
	DISTANCE_CHANGE_EVENT,
	STEPS_CHANGE_EVENT,
	CLIMB_CHANGE_EVENT,
	LEVEL_CHANGE_EVENT,
	CLIMBMILL_LEVEL_CHANGE_EVENT,
	SPEED_CHANGE_EVENT,
	INCLINE_CHANGE_EVENT,
	RESISTANCE_CHANGE_EVENT,
	SETTING_COMPLETE_EVENT,
	THREE_EVENT,
	TWO_EVENT,
	ONE_EVENT,

	MOTOR_SPEED_OVER_EVENT,		
	WORKOUT_DATA_REFRESH_EVENT,
	PROGRAM_INITIAL_EVENT,
	PROGRAM_RESET_EVENT,
	PROGRAM_RUN_EVENT,
	PROGRAM_PAUSE_EVENT,
	PROGRAM_ENGINEER_EVENT,
	PROGRAM_TOTAL_EVENT
}PROGRAM_EVENTS;

typedef union{
  struct{
    UINT32 Workout_Refresh_Data:1;
    UINT32 Workout_Speed_Evt:1;
    UINT32 Workout_Incline_Evt:1;
    UINT32 Workout_Resistance_Evt:1;
    
    UINT32 Workout_Complete_Evt:1;	
    UINT32 Workout_Warmup_Evt:1;
    UINT32 Workout_Cooldown_Evt:1;
    UINT32 Workout_Run_Evt:1;
    
    UINT32 Workout_Profile_Evt:1;
    UINT32 Workout_ChangeDisplay:1;
    UINT32 Workout_TargetWattChange_Evt:1;
    UINT32 Workout_TargetHeartChange_Evt:1;
    
    UINT32 Workout_QuickKeySpd_Evt:1;
    UINT32 Workout_QuickKeyRes_Evt:1;
    UINT32 Workout_HR_ChangeToZero:1;
    UINT32 Workout_HR_IsNotZero:1;
    
    UINT32 Workout_Quick_Recover:1;
    UINT32 Workout_Sprint8_Ready:1;
    UINT32 Workout_Sprint8_Set:1;    
    UINT32 Workout_Sprint8_Go:1;
    UINT32 Workout_Sprint8_RampUp:1;
    
    UINT32 Workout_Sprint8_SprintStage:1;
    UINT32 Workout_Sprint8_ActiveRecoveryStage:1;
    UINT32 Workout_Climbmill_ResChange:1;
  }EvtBit;
  UINT32 AllEvts;
}WorkOutEVTS;
typedef struct {
	PROGRAM_EVENTS evt;
	WorkOutEVTS  WorkOutEvts;
}PROGRAM_PENDING_EVTS;
typedef enum
{
	DATA_MODE_NORMAL=0,
	QUICKKEY_NORMAL,
	DATA_MODE_END
}DATA_MODE;


typedef enum
{
	DATA_STANARD,
	DATA_METRIC,
	DATA_AUTO,
	DATA_END
}DATA_TYPE;

typedef enum
{
  FARE= 0,
  NOTBAD = 1,
  GOOD = 2,
  EXCELLENT = 3
}WORKOUT_RATING;
typedef enum
{
  TARGET_HR= 0,
  CURRENT_HR = 1,
  AVERGE_HR,
  AUTO_HR,
  HR_TAG_TOTAL,
}HR_TAG;
typedef enum
{
  TARGET_WATTS= 0,
  CURRENT_WATTS = 1,
  AUTO_WATTS,
  WATTS_TAG_TOTAL,
}WATTS_TAG;

typedef enum
{
  EP_BI_RPM= 0,
  TM_TARGET_RPM = 1,
  TM_REAL_RPM,
  RPM_TAG_TOTAL,
}RPM_TAG;
typedef enum
{
  SPEED_PROFILE= 0,
  INCLINE_PROFILE,
  RESISTANCE_PROFILE,
  DEFAULT_PROFILE,
  HIGHT_PROFILE,
  PROFILE_TOTAL,
}PROFILE_TAG;

void Program_Regist_Functions(void);
/***************************************************************************//**
* @brief
*  ��ʼ����ʽ
* @param[in]
*   array[]ָ���ʼ���ṹ�壻
*   size��TABLE�����
*  DotMatrix_length:��������
* @param[out]��none
******************************************************************************/
void Program_Initial_Data(ProgramLibParaConfig *ProgramConfig);
/***************************************************************************//**
* @brief
*  ��ʼ��EP��BIKE�����ر�������EP��BIKE
* @param[in]
*   Watts_Para����ʼ�����ر�ṹ�壻
* @param[out]��none
******************************************************************************/
void Program_Load_WattsTable(const WattsParameterStruct *Watts_Para);
/***************************************************************************//**
* @brief
* �˺�������ѡ���Ӧ�ĳ�ʽ
* @param[in]
*   prg_Type����ʽ����ö�ٱ�����
* @param[out]��none
******************************************************************************/
void Program_Select_ID(PROGRAM_ID prg_Type);
/***************************************************************************//**
* @brief
* �˺�������ȡ�õ�ǰ�ĳ�ʽ
* @param[in]
*   none
* @param[out]��
*���س�ʽ��ID��
******************************************************************************/
PROGRAM_ID Program_Get_ProgramID(void);
/***************************************************************************//**
* @brief
* �˺�������ȡ�õ�ǰ�ĳ�ʽ
* @param[in]
*   none
* @param[out]��
*���س�ʽ�ĵ�ǰ״̬
******************************************************************************/
PROGRAM_STATUS Program_Get_Status(void);
/***************************************************************************//**
* @brief
* �˺����������ó���״̬
* @param[in]
*   prgstatus��״̬���ñ���
* @param[out]��none
******************************************************************************/
void Program_Set_Status(PROGRAM_STATUS prgstatus);

/***************************************************************************//**
* @brief
* �˺��������������ⰴ��
* @param[in]
*   vrKey����ʽ���ⰴ��ö�ٱ���
* @param[out]��
*�����Ƿ�ִ��OK
*true��ʾ�ɹ���false��ʾʧ��
******************************************************************************/
VR_KEY Program_Set_VirtualKey(VR_KEY vk);
/***************************************************************************//**
* @brief
* �˺����������ò���
* @param[in]��
vrKey����������
*p�����õ�ֵ
* @param[out]��none
******************************************************************************/
void Program_Set_Parameter(UINT16 vrKey,void *p);
/***************************************************************************//**
* @brief
* �˺�������ȡ��ʱ��
* @param[in]��
* @param[out]������ʱ��
******************************************************************************/

UINT16 Program_Get_Time(UINT8 Data_Mode);

/***************************************************************************//**
* @brief
* �˺�������ȡ��Ŀ��ʱ��
* @param[in]
*   none
* @param[out]��
*����Ŀ��ʱ��
******************************************************************************/
UINT16 Program_Get_WorkoutTime(void);
/***************************************************************************//**
* @brief
* �˺�������ȡ�õ�ǰʱ��
* @param[in]
*   none
* @param[out]��
*���ص�ǰʱ��
******************************************************************************/
UINT16 Program_Get_WorkoutTime_Elapsed(void);
/***************************************************************************//**
* @brief
* �˺�������ȡ��ʣ��ʱ��
* @param[in]
*   none
* @param[out]��
*����ʣ��ʱ��
******************************************************************************/
UINT16 Program_Get_WorkoutTime_Remaining(void);
/***************************************************************************//**
* @brief
* �˺�������ȡ�������˶�ʱ��
* @param[in]
*   none
* @param[out]��
*���������˶�ʱ��
******************************************************************************/
UINT8 Program_Get_WorkoutTime_Warmup(void);
/***************************************************************************//**
* @brief
* �˺�������ȡ�û����˶�ʱ��
* @param[in]
*   none
* @param[out]��
*���ػ����˶�ʱ��
******************************************************************************/
UINT8 Program_Get_WorkoutTime_Cooldown(void);
/***************************************************************************//**
* @brief
* �˺�������ȡ�õ�ǰ�˶��Ķ���
* @param[in]
*   none
* @param[out]��
*���ص�ǰ�˶��Ķ���
******************************************************************************/
UINT8 Program_Get_Stage(void);
/***************************************************************************//**
* @brief
* �˺�������ȡ�õ�ǰ�ε�ǰ�˶�ʱ��
* @param[in]
*   none
* @param[out]��
*���ص�ǰ�ε�ǰ�˶�ʱ��
******************************************************************************/
UINT16 Program_Get_timeInStage(void);
/***************************************************************************//**
* @brief
* �˺�������ȡ�õ�ǰ�����˶�ʱ��
* @param[in]
*   none
* @param[out]��
*���ص�ǰ�����˶�ʱ��
******************************************************************************/
UINT16 Program_Get_stageTime(void);
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
UINT16 Program_Get_Speed(DATA_TYPE type,UINT8 Data_Mode);
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
UINT16 Program_Get_Speed_Average(DATA_TYPE type);
/***************************************************************************//**
* @brief
* �˺�������ȡ������ֵ
* @param[in]
*pIsBlinking���ݲ���
* @param[out]��
*��������ֵ
******************************************************************************/
UINT8 Program_Get_Incline(UINT8 Data_Mode);
/***************************************************************************//**
* @brief
* �˺�������ȡ��ƽ������ֵ
* @param[in]
*none
* @param[out]��
*����ƽ������ֵ
******************************************************************************/
UINT8 Program_Get_Incline_Average(void);
UINT16 Program_Get_Current_Incline_ADC(void);
/***************************************************************************//**
* @brief
* �˺�������ȡ������ֵ
* @param[in]
*pIsBlinking���ݲ���
* @param[out]��
*��������ֵ
******************************************************************************/
UINT8 Program_Get_Resistance(UINT8 Data_Mode);
/***************************************************************************//**
* @brief
* �˺�������ȡ��ƽ������ֵ
* @param[in]
* @param[out]��
*����ƽ������ֵ
******************************************************************************/
UINT8 Program_Get_Resistance_Average(void);
/***************************************************************************//**
* @brief
* �˺�������ȡ��Levelֵ
* @param[in]
*pIsBlinking���ݲ���
* @param[out]��
*����Levelֵ
******************************************************************************/
UINT8 Program_Get_WorkoutLevel(UINT8 Data_Mode);
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
UINT16 Program_Get_Distance(DATA_TYPE type);
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
UINT16 Program_Get_Distance_Remaining(DATA_TYPE type);
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
UINT16 Program_Get_Calories(bool isFourDigits);
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
UINT16 Program_Get_Calories_Remaining(bool isFourDigits);
/***************************************************************************//**
* @brief
* �˺�������ȡ�õ���λ��
* @param[in]��None
* @param[out]��
*���ص���ǰλ��
******************************************************************************/
UINT16 Program_Get_DotmatixPos(void);
/***************************************************************************//**
* @brief
* �˺�������ȡ��Pace
* @param[in]��None
* @param[out]��
*����Pace
******************************************************************************/
UINT16 Program_Get_Pace(void);
/***************************************************************************//**
* @brief
* �˺�������ȡ��Average Pace
* @param[in]��None
* @param[out]��
*����Average Pace
******************************************************************************/
UINT16 Program_Get_Pace_Average(void);
/***************************************************************************//**
* @brief
* �˺�������ȡ������
* @param[in]��
hrTag��������������Ŀ�꣬��ǰ��ƽ���������Զ���ֵ
* @param[out]��
*��������ֵ
******************************************************************************/
UINT8 Program_Get_HR(HR_TAG hrTag);
/***************************************************************************//**
* @brief
* �˺�������ȡ��Wattsֵ
* @param[in]
watts����������Ŀ�꣬��ǰֵ
*pIsBlinking���ݲ���
* @param[out]��
*����Wattsֵ
******************************************************************************/
UINT16 Program_Get_Watts(WATTS_TAG watts, UINT8 Data_Mode);
/***************************************************************************//**
* @brief
* �˺�������ȡ��METsֵ
* @param[in]��none
* @param[out]��
*����METsֵ
******************************************************************************/
UINT16 Program_Get_METs(void);
/***************************************************************************//**
* @brief
* �˺�������ȡ�õ�ǰStepsֵ
* @param[in]��none
* @param[out]��
*���ص�ǰStepsֵ
******************************************************************************/
UINT16 Program_Get_Steps(void);
/***************************************************************************//**
* @brief
* �˺�������ȡ��ʣ��Stepsֵ
* @param[in]��none
* @param[out]��
*����ʣ��Stepsֵ
******************************************************************************/
UINT16 Program_Get_Steps_Remaining(void);
/***************************************************************************//**
* @brief
* �˺�������ȡ��RPMֵ
* @param[in]��none
* @param[out]��
*����RPMֵ
******************************************************************************/
UINT16 Program_Get_RPM(void);
/***************************************************************************//**
* @brief
* �˺�������ȡ��VO2Maxֵ
* @param[in]��none
* @param[out]��
*����VO2Maxֵ
******************************************************************************/
UINT16 Program_Get_VO2Max(void);
/***************************************************************************//**
* @brief
* �˺�������ȡ�õ�������
* @param[in]��none
* @param[out]��
*���ص�������
******************************************************************************/
PROFILE* Program_Get_Profile(PROFILE_TAG index);
/***************************************************************************//**
* @brief
* �˺����������ü��ٱ���
* @param[in]��times�����ٱ���
* @param[out]��none
******************************************************************************/
void Program_Set_nXTime(UINT8 times);
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
UINT16 Program_Get_Weight(DATA_TYPE type);
/***************************************************************************//**
* @brief
* �˺�������ȡ�õ�ǰ�û�
* @param[in]��none
* @param[out]��
*���ص�ǰ�û�
******************************************************************************/
UINT8 Program_Get_User(void);
/***************************************************************************//**
* @brief
* �˺�������ȡ��Ŀ���ʽ��Ŀ��ֵ
* @param[in]��none
* @param[out]��
*����Ŀ���ʽ��Ŀ��ֵ
******************************************************************************/
UINT16 Program_Get_Goal(void);
/***************************************************************************//**
* @brief
* �˺�������ȡ��Ŀ���ʽ������
* @param[in]��none
* @param[out]��
*����Ŀ���ʽ������
******************************************************************************/
workoutGoalTypes Program_Get_Goal_Type(void);
/***************************************************************************//**
* @brief
* �˺�������ȡ��Ŀ���ʽ��ʣ��ֵ
* @param[in]��none
* @param[out]��
*����Ŀ���ʽ��ʣ��ֵ
******************************************************************************/
UINT16 Program_Get_Goal_Remaining(void);
/***************************************************************************//**
* @brief
* �˺�������ȡ�ó�ʽ����������
* @param[in]��none
* @param[out]��
*���س�ʽ����������ö��
******************************************************************************/
SETTING_ENUM Program_Get_SettingFlow(void);
/***************************************************************************//**
* @brief
* �˺�������Main.c����
* @param[in]��none
* @param[out]��
*�����¼�
******************************************************************************/
PROGRAM_PENDING_EVTS Program_Process(ReturnEventHandler flag);
/***************************************************************************//**
* @brief
* �˺�������ȡ��Program UUID
* @param[in]��none
* @param[out]������Program UUID
******************************************************************************/
char* Program_Get_UUID(void);
/***************************************************************************//**
* @brief
* �˺�������ȡ��Program Description
* @param[in]��* plength����������
* @param[out]������Program UUDescription
******************************************************************************/
char* Program_Get_Description(UINT16* plength);
/***************************************************************************//**
* @brief
* �˺�������ȡ��Stage Goal Remaining
* @param[in]��none
* @param[out]��
����Stage Goal Remaining
******************************************************************************/
UINT16 Program_Get_Stage_Goal_Remaining(void);
/***************************************************************************//**
* @brief
* �˺�������ȡ��Stage Goal
* @param[in]��none
* @param[out]��
����Stage Goal
******************************************************************************/
UINT16 Program_Get_Stage_Goal(void);
/***************************************************************************//**
* @brief
* �˺�������ȡ��Lap
* @param[in]��none
* @param[out]��
����Lap
******************************************************************************/
UINT16 Program_Get_Lap_Index(void);
/***************************************************************************//**
* @brief
* �˺�������ȡ��Split
* @param[in]��none
* @param[out]��
����Split
******************************************************************************/
UINT16 Program_Get_Split_Index(void);
/***************************************************************************//**
* @brief
* �˺�������ȡ��Floor
* @param[in]��none
* @param[out]��
����Floor
******************************************************************************/
UINT16 Program_Get_Floor(void);
/***************************************************************************//**
* @brief
* �˺�������ȡ��Elevation
* @param[in]��none
* @param[out]��
����Elevation
******************************************************************************/
UINT16 Program_Get_Elevation(void);
/***************************************************************************//**
* @brief
* �˺�������ȡ��VO2
* @param[in]��none
* @param[out]��
����VO2
******************************************************************************/
UINT16 Program_Get_VO2(void);
/***************************************************************************//**
* @brief
* �˺�������ȡ��Sprint8 Sweat Score
* @param[in]��index������
* @param[out]��
����Sprint8 Sweat Score
******************************************************************************/

UINT16 Sprint8_Get_Sweat_Score(UINT8 index);
/***************************************************************************//**
* @brief
* �˺�������ȡ��Sprint8 Total Sweat Score
* @param[in]��none
* @param[out]��
����Sprint8 Total Sweat Score
******************************************************************************/
UINT16 Sprint8_Get_Total_Sweat_Score(void);
/***************************************************************************//**
* @brief
* �˺�������ȡ��Fitness Test Sweat Score
* @param[in]��index������
* @param[out]��
����Fitness Test Sweat Score
******************************************************************************/
UINT16 Fitness_Test_Get_Score(void);
/***************************************************************************//**
* @brief
* �˺������ڶ�ȡ�汾��
* @param[in]��*buffer���ڶ�ȡ�汾
* @param[out]�����ذ汾�ܳ���
******************************************************************************/
UINT8 Program_Get_Vision(UINT8 *buffer);
UINT8 Program_Is_Working(void);
UINT16 Sprint8_Get_SprintStage(void);
UINT8 Program_Get_DefaultDotMatrix(void);
void Program_Set_RPM(UINT8 by_Dat);
UINT16 Program_Get_Stairs();
UINT16 Program_Get_SPM();
UINT16 Program_Get_Height();
UINT16 Program_Get_Height_Percent();
#endif
