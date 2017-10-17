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
*  初始化程式
* @param[in]
*   array[]指向初始化结构体；
*   size是TABLE表个数
*  DotMatrix_length:点阵列数
* @param[out]：none
******************************************************************************/
void Program_Initial_Data(ProgramLibParaConfig *ProgramConfig);
/***************************************************************************//**
* @brief
*  初始化EP或BIKE的瓦特表，仅用于EP或BIKE
* @param[in]
*   Watts_Para：初始化瓦特表结构体；
* @param[out]：none
******************************************************************************/
void Program_Load_WattsTable(const WattsParameterStruct *Watts_Para);
/***************************************************************************//**
* @brief
* 此函数用于选择对应的程式
* @param[in]
*   prg_Type：程式名的枚举变量；
* @param[out]：none
******************************************************************************/
void Program_Select_ID(PROGRAM_ID prg_Type);
/***************************************************************************//**
* @brief
* 此函数用于取得当前的程式
* @param[in]
*   none
* @param[out]：
*返回程式的ID号
******************************************************************************/
PROGRAM_ID Program_Get_ProgramID(void);
/***************************************************************************//**
* @brief
* 此函数用于取得当前的程式
* @param[in]
*   none
* @param[out]：
*返回程式的当前状态
******************************************************************************/
PROGRAM_STATUS Program_Get_Status(void);
/***************************************************************************//**
* @brief
* 此函数用于设置程序状态
* @param[in]
*   prgstatus：状态设置变量
* @param[out]：none
******************************************************************************/
void Program_Set_Status(PROGRAM_STATUS prgstatus);

/***************************************************************************//**
* @brief
* 此函数用于设置虚拟按键
* @param[in]
*   vrKey：程式虚拟按键枚举变量
* @param[out]：
*返回是否执行OK
*true表示成功；false表示失败
******************************************************************************/
VR_KEY Program_Set_VirtualKey(VR_KEY vk);
/***************************************************************************//**
* @brief
* 此函数用于设置参数
* @param[in]：
vrKey：参数索引
*p：设置的值
* @param[out]：none
******************************************************************************/
void Program_Set_Parameter(UINT16 vrKey,void *p);
/***************************************************************************//**
* @brief
* 此函数用于取得时间
* @param[in]：
* @param[out]：返回时间
******************************************************************************/

UINT16 Program_Get_Time(UINT8 Data_Mode);

/***************************************************************************//**
* @brief
* 此函数用于取得目标时间
* @param[in]
*   none
* @param[out]：
*返回目标时间
******************************************************************************/
UINT16 Program_Get_WorkoutTime(void);
/***************************************************************************//**
* @brief
* 此函数用于取得当前时间
* @param[in]
*   none
* @param[out]：
*返回当前时间
******************************************************************************/
UINT16 Program_Get_WorkoutTime_Elapsed(void);
/***************************************************************************//**
* @brief
* 此函数用于取得剩余时间
* @param[in]
*   none
* @param[out]：
*返回剩余时间
******************************************************************************/
UINT16 Program_Get_WorkoutTime_Remaining(void);
/***************************************************************************//**
* @brief
* 此函数用于取得热身运动时间
* @param[in]
*   none
* @param[out]：
*返回热身运动时间
******************************************************************************/
UINT8 Program_Get_WorkoutTime_Warmup(void);
/***************************************************************************//**
* @brief
* 此函数用于取得缓和运动时间
* @param[in]
*   none
* @param[out]：
*返回缓和运动时间
******************************************************************************/
UINT8 Program_Get_WorkoutTime_Cooldown(void);
/***************************************************************************//**
* @brief
* 此函数用于取得当前运动的段数
* @param[in]
*   none
* @param[out]：
*返回当前运动的段数
******************************************************************************/
UINT8 Program_Get_Stage(void);
/***************************************************************************//**
* @brief
* 此函数用于取得当前段当前运动时间
* @param[in]
*   none
* @param[out]：
*返回当前段当前运动时间
******************************************************************************/
UINT16 Program_Get_timeInStage(void);
/***************************************************************************//**
* @brief
* 此函数用于取得当前段总运动时间
* @param[in]
*   none
* @param[out]：
*返回当前段总运动时间
******************************************************************************/
UINT16 Program_Get_stageTime(void);
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
UINT16 Program_Get_Speed(DATA_TYPE type,UINT8 Data_Mode);
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
UINT16 Program_Get_Speed_Average(DATA_TYPE type);
/***************************************************************************//**
* @brief
* 此函数用于取得升降值
* @param[in]
*pIsBlinking：暂不用
* @param[out]：
*返回升降值
******************************************************************************/
UINT8 Program_Get_Incline(UINT8 Data_Mode);
/***************************************************************************//**
* @brief
* 此函数用于取得平均升降值
* @param[in]
*none
* @param[out]：
*返回平均升降值
******************************************************************************/
UINT8 Program_Get_Incline_Average(void);
UINT16 Program_Get_Current_Incline_ADC(void);
/***************************************************************************//**
* @brief
* 此函数用于取得阻力值
* @param[in]
*pIsBlinking：暂不用
* @param[out]：
*返回阻力值
******************************************************************************/
UINT8 Program_Get_Resistance(UINT8 Data_Mode);
/***************************************************************************//**
* @brief
* 此函数用于取得平均阻力值
* @param[in]
* @param[out]：
*返回平均阻力值
******************************************************************************/
UINT8 Program_Get_Resistance_Average(void);
/***************************************************************************//**
* @brief
* 此函数用于取得Level值
* @param[in]
*pIsBlinking：暂不用
* @param[out]：
*返回Level值
******************************************************************************/
UINT8 Program_Get_WorkoutLevel(UINT8 Data_Mode);
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
UINT16 Program_Get_Distance(DATA_TYPE type);
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
UINT16 Program_Get_Distance_Remaining(DATA_TYPE type);
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
UINT16 Program_Get_Calories(bool isFourDigits);
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
UINT16 Program_Get_Calories_Remaining(bool isFourDigits);
/***************************************************************************//**
* @brief
* 此函数用于取得点阵位置
* @param[in]：None
* @param[out]：
*返回点阵当前位置
******************************************************************************/
UINT16 Program_Get_DotmatixPos(void);
/***************************************************************************//**
* @brief
* 此函数用于取得Pace
* @param[in]：None
* @param[out]：
*返回Pace
******************************************************************************/
UINT16 Program_Get_Pace(void);
/***************************************************************************//**
* @brief
* 此函数用于取得Average Pace
* @param[in]：None
* @param[out]：
*返回Average Pace
******************************************************************************/
UINT16 Program_Get_Pace_Average(void);
/***************************************************************************//**
* @brief
* 此函数用于取得心跳
* @param[in]：
hrTag：用于索引返回目标，当前，平均，还是自动的值
* @param[out]：
*返回心跳值
******************************************************************************/
UINT8 Program_Get_HR(HR_TAG hrTag);
/***************************************************************************//**
* @brief
* 此函数用于取得Watts值
* @param[in]
watts：用于索引目标，当前值
*pIsBlinking：暂不用
* @param[out]：
*返回Watts值
******************************************************************************/
UINT16 Program_Get_Watts(WATTS_TAG watts, UINT8 Data_Mode);
/***************************************************************************//**
* @brief
* 此函数用于取得METs值
* @param[in]：none
* @param[out]：
*返回METs值
******************************************************************************/
UINT16 Program_Get_METs(void);
/***************************************************************************//**
* @brief
* 此函数用于取得当前Steps值
* @param[in]：none
* @param[out]：
*返回当前Steps值
******************************************************************************/
UINT16 Program_Get_Steps(void);
/***************************************************************************//**
* @brief
* 此函数用于取得剩余Steps值
* @param[in]：none
* @param[out]：
*返回剩余Steps值
******************************************************************************/
UINT16 Program_Get_Steps_Remaining(void);
/***************************************************************************//**
* @brief
* 此函数用于取得RPM值
* @param[in]：none
* @param[out]：
*返回RPM值
******************************************************************************/
UINT16 Program_Get_RPM(void);
/***************************************************************************//**
* @brief
* 此函数用于取得VO2Max值
* @param[in]：none
* @param[out]：
*返回VO2Max值
******************************************************************************/
UINT16 Program_Get_VO2Max(void);
/***************************************************************************//**
* @brief
* 此函数用于取得点阵数据
* @param[in]：none
* @param[out]：
*返回点阵数据
******************************************************************************/
PROFILE* Program_Get_Profile(PROFILE_TAG index);
/***************************************************************************//**
* @brief
* 此函数用于设置加速倍率
* @param[in]：times：加速倍率
* @param[out]：none
******************************************************************************/
void Program_Set_nXTime(UINT8 times);
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
UINT16 Program_Get_Weight(DATA_TYPE type);
/***************************************************************************//**
* @brief
* 此函数用于取得当前用户
* @param[in]：none
* @param[out]：
*返回当前用户
******************************************************************************/
UINT8 Program_Get_User(void);
/***************************************************************************//**
* @brief
* 此函数用于取得目标程式的目标值
* @param[in]：none
* @param[out]：
*返回目标程式的目标值
******************************************************************************/
UINT16 Program_Get_Goal(void);
/***************************************************************************//**
* @brief
* 此函数用于取得目标程式的类型
* @param[in]：none
* @param[out]：
*返回目标程式的类型
******************************************************************************/
workoutGoalTypes Program_Get_Goal_Type(void);
/***************************************************************************//**
* @brief
* 此函数用于取得目标程式的剩余值
* @param[in]：none
* @param[out]：
*返回目标程式的剩余值
******************************************************************************/
UINT16 Program_Get_Goal_Remaining(void);
/***************************************************************************//**
* @brief
* 此函数用于取得程式的设置流程
* @param[in]：none
* @param[out]：
*返回程式的设置流程枚举
******************************************************************************/
SETTING_ENUM Program_Get_SettingFlow(void);
/***************************************************************************//**
* @brief
* 此函数用于Main.c调用
* @param[in]：none
* @param[out]：
*返回事件
******************************************************************************/
PROGRAM_PENDING_EVTS Program_Process(ReturnEventHandler flag);
/***************************************************************************//**
* @brief
* 此函数用于取得Program UUID
* @param[in]：none
* @param[out]：返回Program UUID
******************************************************************************/
char* Program_Get_UUID(void);
/***************************************************************************//**
* @brief
* 此函数用于取得Program Description
* @param[in]：* plength：描述长度
* @param[out]：返回Program UUDescription
******************************************************************************/
char* Program_Get_Description(UINT16* plength);
/***************************************************************************//**
* @brief
* 此函数用于取得Stage Goal Remaining
* @param[in]：none
* @param[out]：
返回Stage Goal Remaining
******************************************************************************/
UINT16 Program_Get_Stage_Goal_Remaining(void);
/***************************************************************************//**
* @brief
* 此函数用于取得Stage Goal
* @param[in]：none
* @param[out]：
返回Stage Goal
******************************************************************************/
UINT16 Program_Get_Stage_Goal(void);
/***************************************************************************//**
* @brief
* 此函数用于取得Lap
* @param[in]：none
* @param[out]：
返回Lap
******************************************************************************/
UINT16 Program_Get_Lap_Index(void);
/***************************************************************************//**
* @brief
* 此函数用于取得Split
* @param[in]：none
* @param[out]：
返回Split
******************************************************************************/
UINT16 Program_Get_Split_Index(void);
/***************************************************************************//**
* @brief
* 此函数用于取得Floor
* @param[in]：none
* @param[out]：
返回Floor
******************************************************************************/
UINT16 Program_Get_Floor(void);
/***************************************************************************//**
* @brief
* 此函数用于取得Elevation
* @param[in]：none
* @param[out]：
返回Elevation
******************************************************************************/
UINT16 Program_Get_Elevation(void);
/***************************************************************************//**
* @brief
* 此函数用于取得VO2
* @param[in]：none
* @param[out]：
返回VO2
******************************************************************************/
UINT16 Program_Get_VO2(void);
/***************************************************************************//**
* @brief
* 此函数用于取得Sprint8 Sweat Score
* @param[in]：index：索引
* @param[out]：
返回Sprint8 Sweat Score
******************************************************************************/

UINT16 Sprint8_Get_Sweat_Score(UINT8 index);
/***************************************************************************//**
* @brief
* 此函数用于取得Sprint8 Total Sweat Score
* @param[in]：none
* @param[out]：
返回Sprint8 Total Sweat Score
******************************************************************************/
UINT16 Sprint8_Get_Total_Sweat_Score(void);
/***************************************************************************//**
* @brief
* 此函数用于取得Fitness Test Sweat Score
* @param[in]：index：索引
* @param[out]：
返回Fitness Test Sweat Score
******************************************************************************/
UINT16 Fitness_Test_Get_Score(void);
/***************************************************************************//**
* @brief
* 此函数用于读取版本号
* @param[in]：*buffer用于读取版本
* @param[out]：返回版本总长度
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
