#ifndef __DATA_PROC_H__
#define __DATA_PROC_H__

#include "Global_Config.h"

#define GETMAXMINVALUE(val,max,min) ((val)>(max)?(max):((val)<(min)?(min):(val)))


typedef enum{
    ECB_TYPE,
    INDUCTION_TYPE
}_ResistanceType;
typedef struct 
{ 
  UINT16 target_Time;
  UINT16 elapsed_Time;
  UINT16 remaining_Time;  
  UINT16 warmup_Time;
  UINT16 cooldown_Time;
  UINT16 workout_Time;
  UINT16 average_Time;
  UINT16 stage_Time;
  UINT16 time_In_Stage;
  UINT32 accumulatedTimeSeconds;
  UINT32 accumulatedLubeTime;
  UINT32 lubebeltDistance;
  UINT8  stage;
}TIME_PARAMETER_TypeDef;
typedef struct 
{ 
  UINT16 target_Pace;
  UINT16 average_Pace;
  UINT16 EPBI_Speed;
  UINT16 Motor_Speed;
  UINT16 target_Speed;
  UINT16 current_Speed;
  UINT16 average_Speed;
  UINT16 Max_Speed;
  UINT16 Min_Speed;
  INT16 Offset;
  UINT16 Base;
}SPEED_PARAMETER_Typedef; 
typedef struct 
{ 
  UINT16 target_RPM;
  UINT16 current_RPM;
  UINT16 Motor_RPM;
  UINT16 EPBI_RPM;
  UINT16 average_RPM;
  UINT16 Max_RPM;
  UINT16 Min_RPM;
}RPM_PARAMETER_Typedef; 
typedef struct 
{ 
  UINT16 target_Incline_ADC; 
  UINT16 current_Incline_ADC; 
  UINT16 real_Incline_ADC;
  UINT8 average_Inclinex10;  
  UINT8 target_Inclinex10;
  UINT8 current_Inclinex10;
  UINT8 Base;
  INT16 Offset;
}INCLINE_PARAMETER_Typedef; 
typedef struct 
{ 
  UINT16 target_Calories;
  UINT16 elapsed_Calories;  
  UINT16 remaining_Calories;
  UINT16 CaloriesPerHour;
}CALORIES_PARAMETER_TypeDef;

typedef struct 
{
    unsigned long long accumulatedStairs;
    unsigned long long LubeChainStairs;
  UINT16 elapsed_stairs;
  UINT16 target_stairs;
  UINT16 remain_stairs;
  UINT16 elapsed_height;
  UINT16 elapsed_floor;
  UINT16 target_height;
  UINT16 remain_height;
}STAIRS_PARAMETER_TypeDef;
typedef struct 
{ 
  unsigned long long accumulatedDistanceMilesX1000;
  unsigned long long LubeBeltDistanceMilesX1000;
  unsigned long long LubeBeltTime;
  UINT16 target_Distance;
  UINT16 elapsed_Distance;  
  UINT16 remaining_Distance;
}DISTANCE_PARAMETER_TypeDef;
typedef struct 
{ 
  UINT16 target_Steps;
  UINT16 elapsed_Steps;  
  UINT16 remaining_Steps;
}STEPS_PARAMETER_TypeDef;
typedef struct 
{ 
  UINT16 target_Climb;
  UINT16 elapsed_Climb;  
  UINT16 remaining_Climb;
}CLIMB_PARAMETER_TypeDef;
typedef struct 
{ 
  UINT8 target_Heart;
  UINT8 current_Heart;
  UINT8 average_Heart;
}HEART_PARAMETER_TypeDef; 
typedef struct 
{ 
  UINT16 target_Watts;
  UINT16 current_Watts;  
}WATTS_PARAMETER_TypeDef; 
typedef struct 
{ 
  UINT16 target_PWM;
  UINT16 current_PWM;
  UINT16 real_PWM;
  UINT16 target_ECBCount;
  UINT16 current_ECBCount;
  
  UINT16 real_ECBCount;
  UINT8 target_Resistance;
  UINT8 current_Resistance;
  UINT8 average_Resistance;
  UINT8 Base;
  INT8  Offset;
}RESISTANCE_PARAMETER_TypeDef; 
typedef struct 
{ 
  UINT8  Max_Level;
  UINT8  Min_Level;
  UINT8  Target_Level; 
}LEVEL_PARAMETER_TypeDef; 
typedef struct 
{
  UINT16 weight;  
  UINT8 age;  
  UINT8 gender;  
  UINT8 user_Slot;
}USER_PARAMETER_TypeDef; 

typedef struct 
{ 
  TIME_PARAMETER_TypeDef Time;
  SPEED_PARAMETER_Typedef Speed;
  RPM_PARAMETER_Typedef RPM;
  INCLINE_PARAMETER_Typedef Incline;
  CALORIES_PARAMETER_TypeDef Calories;
  DISTANCE_PARAMETER_TypeDef Distance;
  STEPS_PARAMETER_TypeDef Steps;
  CLIMB_PARAMETER_TypeDef Climb;
  HEART_PARAMETER_TypeDef Heart;
  RESISTANCE_PARAMETER_TypeDef Resistance;
  WATTS_PARAMETER_TypeDef Watts;
  USER_PARAMETER_TypeDef User;
  LEVEL_PARAMETER_TypeDef Level;
  STAIRS_PARAMETER_TypeDef Stairs;
  UINT16 Mets;
  UINT16 VO2Max;
  UINT16 Goal;
 
}ACTIVE_WORKOUT;

typedef enum
{
	TARGET_STAIRS,
	ELAPSED_STAIRS,
	REMAINING_STAIRS,
	TARGET_HEIGHT,
	ELAPSED_HEIGHT,
	REMAINING_HEIGHT,
	ELAPSED_FLOOR
}_STAIR_PARA_INDEX;
typedef enum
{
	TARGET_TIME,
	ELAPSED_TIME,
	REMAINING_TIME,  
	WARMUP_TIME,
	COOLDOWN_TIME,
	WORKOUT_TIME,
	AVERAGE_TIME,
	STAGE_TIME,
	TIME_IN_STAGE,
	STAGE
}_TIME_PARA_INDEX;
typedef enum 
{ 
	TARGET_PACE,
	AVERAGE_PACE,
	EPBI_SPEED,
	MOTOR_SPEED,
	TARGET_SPEED,
	CURRENT_SPEED,
	AVERAGE_SPEED,
	MAX_SPEED,
	MIN_SPEED,
	SPEED_OFFSET,
	SPEED_BASE,
}_SPEED_PARA_INDEX; 
typedef enum 
{ 
	TARGET_RPM,
	CURRENT_RPM,
	MOTOR_RPM,
	EPBI_RPM,
	AVERAGE_RPM,
	MAX_RPM,
	MIN_RPM,
}_RPM_PARA_INDEX; 
typedef enum 
{ 
	TARGET_INCLINE_ADC, 
	CURRENT_INCLINE_ADC, 
	REAL_INCLINE_ADC,
	MAX_INCLINEADC,
	MIN_INCLINEADC,
	MAX_INCLINEX10,  
	MIN_INCLINEX10,  
	AVERAGE_INCLINEX10,  
	TARGET_INCLINEX10,
	CURRENT_INCLINEX10,
	INCLINE_BASE,
	INCLINE_OFFSET,
}_INCLINE_PARA_INDEX; 
typedef enum 
{ 
	TARGET_CALORIES,
	ELAPSED_CALORIES,  
	REMAINING_CALORIES,
}_CALORIES_PARA_INDEX;
typedef enum 
{ 
	TARGET_DISTANCE,
	ELAPSED_DISTANCE,  
	REMAINING_DISTANCE,
}_DISTANCE_PARA_INDEX;
typedef enum 
{ 
	TARGET_STEPS,
	ELAPSED_STEPS,  
	REMAINING_STEPS,
}_STEPS_PARA_INDEX;
typedef enum 
{ 
	TARGET_CLIMB,
	ELAPSED_CLIMB,  
	REMAINING_CLIMB,
}_CLIMB_PARA_INDEX;
typedef enum 
{ 
	TARGET_HEART,
	CURRENT_HEART,
	AVERAGE_HEART,
}_HEART_PARA_INDEX;
typedef enum 
{ 
	TARGET_WATT,
	CURRENT_WATT,  
}_WATTS_PARA_INDEX;
typedef enum 
{ 
	TARGET_PWM,
	CURRENT_PWM,
	REAL_PWM,
	TARGET_ECBCOUNT,
	CURRENT_ECBCOUNT,
	REAL_ECBCOUNT,
	TARGET_RESISTANCE,
	CURRENT_RESISTANCE,
	AVERAGE_RESISTANCE,
	MAX_RESISTANCE,
	MIN_RESISTANCE,
	RESISTANCE_BASE,
	RESIISTANCE_OFFSET,
}_RESISTANCE_PARA_INDEX; 
typedef enum 
{ 
	MAX_LEVEL,
	MIN_LEVEL,
	TARGET_LEVEL, 
}_LEVEL_PARA_INDEX; 
typedef enum 
{
	USER_WEIGHT,  
	USER_AGE,  
	USER_GENDER,  
	USER_SLOT,
}_USER_PARA_INDEX; 

typedef enum
{
	UINTMODE,
	MACHINETYPE,
}MACHINEINFO;
typedef enum
{
    NO_DATA_EVENT=0x0,
//    DATA_COMPLETE_EVENT,
    DATA_RUN_EVENT,
    DATA_PAUSE_EVENT,
    DATA_SAVEDATA_EVENT,
    DATA_EVENT_END
}DATA_EVENT;

void Data_Set_Speed(UINT16 by_Dat,UCHAR by_Unit,UCHAR by_Mode);
void Data_Set_Incline(UINT16 by_Dat,UCHAR by_Mode);
void Data_Set_Resistance(UINT16 by_Dat,UCHAR by_Mode);
void Data_Set_Time(UINT16 by_Dat);
void Data_Set_Weight(UINT16 by_Dat);
void Data_Set_StageTime(UINT16 by_Dat);

void Data_Set_WarmUP_Time(UINT16 by_Dat);
void Data_Set_CoolDown_Time(UINT16 by_Dat);
void Data_Set_Level(UINT16 by_Dat);
void Data_Set_Level_MaxVal(UINT16 by_Dat);
void Data_Set_Watt(UINT16 by_Dat);
void Data_Set_Nx(UCHAR by_Dat);
void Data_Set_TargetPWMValue(UINT16 by_Dat);
void Data_Set_WorkOutTime(UINT16 by_Dat);

UCHAR Data_Get_DotmatrixSpeedData(UCHAR by_Dat);
UCHAR Data_Get_DotmatrixInclineData(UCHAR by_Dat);
UCHAR Data_Get_DotmatrixResistanceData(UCHAR by_Dat);

void Data_Process_Intilal_Data(void);

void Data_1ms_ISR(void);
UINT8 Data_Process(void);
UINT16 Data_Get_Time(_TIME_PARA_INDEX Para_Index);
UINT16 Data_Get_Speed(_SPEED_PARA_INDEX Para_Index);
UINT16 Data_Get_RPM(_RPM_PARA_INDEX Para_Index);
UINT16 Data_Get_Incline(_INCLINE_PARA_INDEX Para_Index);
UINT16 Data_Get_Calories(_CALORIES_PARA_INDEX Para_Index);
UINT16 Data_Get_Distance(_DISTANCE_PARA_INDEX Para_Index);
UINT16 Data_Get_Steps(_STEPS_PARA_INDEX Para_Index);
UINT16 Data_Get_Climb(_CLIMB_PARA_INDEX Para_Index);
UINT16 Data_Get_Heart(_HEART_PARA_INDEX Para_Index);
UINT16 Data_Get_Watts(_WATTS_PARA_INDEX Para_Index);
UINT16 Data_Get_Resistance(_RESISTANCE_PARA_INDEX Para_Index);
UINT16 Data_Get_Level(_LEVEL_PARA_INDEX Para_Index);
UINT16 Data_Get_UserData(_USER_PARA_INDEX Para_Index);
UINT16 Data_Get_VO2Max(void);
UINT16 Data_Get_Mets(void);
UCHAR Data_Get_1s(void);
UINT16 Data_Get_Nx(void);
void Data_Save_AccumulatedData(UINT8 isReset);
void Data_Set_Event(DATA_EVENT Data_Event);
void Data_Clear_ResistanceOffset(void);
void Data_Clear_InclineOffset(void);
void Data_Clear_SpeedOffset(void);
void Data_Set_FixedRPM(UINT8 by_Dat);
void Data_Refresh_BeltDistanceData(void);
UINT32 Data_Get_BeltDistance(void);
UINT32 Data_Get_Lube_Time(void);
void ClimbMill_Update(void);
void Climbmill_Stairs_Calculation(void);
UINT16 Data_Get_Stairs( _STAIR_PARA_INDEX index);
void Data_Set_Hight(UINT16 w_Dat);
#endif
