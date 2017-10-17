#include "Programs.h"
#ifndef __SETTING_CONFIG_H_
#define __SETTING_CONFIG_H_
const unsigned char TM_Manual_SetUp[]=
{
	SET_PROGRAM,
	SET_TIME,
	SET_SPEED,
	SET_INCLINE,
    SET_PRESS_START
        
};
const unsigned char TM_Goal_DistanceSetUp[]=
{
    SET_PROGRAM,
	SET_GOAL,
	SET_SPEED,
    SET_PRESS_START
};
const unsigned char EP_Goal_SetUp[]=
{
    SET_PROGRAM,
	SET_GOAL,
	SET_RESISTANCE,
    SET_PRESS_START
};
const unsigned char EP_Incline_Goal_SetUp[]=
{
    SET_PROGRAM,
	SET_GOAL,
	SET_RESISTANCE,
    SET_INCLINE,
    SET_PRESS_START
};
const unsigned char TM_Calories_SetUp[]=
{
    SET_PROGRAM,
    SET_CALORIES,
    SET_SPEED,
    SET_PRESS_START
};
const unsigned char EP_Manual_SetUp[]=
{
	SET_PROGRAM,
	SET_TIME,
	SET_RESISTANCE,
    SET_PRESS_START
};
const unsigned char EP_Incline_Manual_SetUp[]=
{
	SET_PROGRAM,
	SET_TIME,
	SET_RESISTANCE,
	SET_INCLINE,
    SET_PRESS_START
};
const unsigned char EP_Calories_SetUp[]=
{
	SET_PROGRAM,
	SET_TIME,
	SET_RESISTANCE,
    SET_PRESS_START
};
const unsigned char EP_Incline_Calories_SetUp[]=
{
	SET_PROGRAM,
	SET_TIME,
	SET_RESISTANCE,
	SET_INCLINE,
    SET_PRESS_START
};
const unsigned char THR_ZONE_SetUp[]=
{
	SET_PROGRAM,
	SET_TIME,
	SET_HEART,
    SET_PRESS_START
};
const unsigned char Interval_HR_SetUp[]=
{
    SET_PROGRAM,
	SET_WORKINTERVAL_TIME,
	SET_WORKINTERVAL_HEART,
	SET_RESTINTERVAL_TIME,
	SET_RESTINTERVAL_HEART,
	SET_TIME,
    SET_PRESS_START
        
};
const unsigned char Steps_HR_SetUp[]=
{
    SET_PROGRAM,
	SET_SEGMENT_TIME,
	SET_SEGMENT_1_HEART,
	SET_SEGMENT_2_HEART,
	SET_SEGMENT_3_HEART,
	SET_SEGMENT_4_HEART,
	SET_TIME,
    SET_PRESS_START
};
const unsigned char ProTable_TimeSetUp[]=
{
    SET_PROGRAM,
	SET_TIME,
	SET_LEVEL,
    SET_PRESS_START
};
const unsigned char TM_ProTable_DistanceSetUp[]=
{
    SET_PROGRAM,
	SET_DISTANCE,
	SET_LEVEL,
    SET_SPEED,
    SET_PRESS_START
};
const unsigned char TM_Distance_SetUp[]=
{
    SET_PROGRAM,
	SET_DISTANCE,
    SET_SPEED,
    SET_PRESS_START
};
const unsigned char EP_Distance_SetUp[]=
{
    SET_PROGRAM,
	SET_DISTANCE,
	SET_RESISTANCE,
    SET_PRESS_START
};
const unsigned char EP_Incline_Distance_SetUp[]=
{
    SET_PROGRAM,
	SET_DISTANCE,
	SET_LEVEL,
    SET_INCLINE,
    SET_PRESS_START
};
const unsigned char TM_ProTable_CaloriesSetUp[]=
{
    SET_PROGRAM,
	SET_CALORIES,
	SET_LEVEL,
    SET_SPEED,
    SET_PRESS_START
};
const unsigned char TM_CaloriesSetUp[]=
{
    SET_PROGRAM,
	SET_CALORIES,
	SET_LEVEL,
    SET_SPEED,
    SET_PRESS_START
};
const unsigned char EP_ProTable_DistanceSetUp[]=
{
    SET_PROGRAM,
	SET_DISTANCE,
	SET_LEVEL,
    SET_PRESS_START
};
const unsigned char EP_ProTable_CaloriesSetUp[]=
{
    SET_PROGRAM,
	SET_CALORIES,
	SET_LEVEL,
    SET_PRESS_START
};

const unsigned char ProSprint8_SetUp[]=
{
    SET_PROGRAM,
	SET_LEVEL,
    SET_PRESS_START
};

const unsigned char PConstantWatt_SetUp[]=
{
    SET_PROGRAM,
	SET_TIME,
	SET_WATTS,
    SET_PRESS_START
};
const unsigned char EP_ProMyFirst5k_SetUp[]=
{
    SET_PROGRAM,
    SET_WAIT_STAGE,
	SET_WALK_SPEED,
	SET_JOG_SPEED,
	SET_RESISTANCE,
    SET_PRESS_START
};
const unsigned char TM_ProMyFirst5k_SetUp[]=
{
    SET_PROGRAM,
    SET_WAIT_STAGE,
	SET_WALK_SPEED,
	SET_JOG_SPEED,
	SET_INCLINE,
    SET_PRESS_START
};
const unsigned char TM_CustomTime_SetUp[]=
{
    SET_PROGRAM,
    SET_SUBPROGRAM,
    SET_WAIT_STAGE,
    SET_CUSTOM_SEGMENT_TIME,
    SET_SPEED,
    SET_INCLINE,
    SET_PRESS_START,
};

const unsigned char TM_CustomDistance_SetUp[]=
{
    SET_PROGRAM,
    SET_SUBPROGRAM,
    SET_WAIT_STAGE,
    SET_DISTANCE,
    SET_SPEED,
    SET_INCLINE,
    SET_PRESS_START
};

const unsigned char EP_CustomTime_SetUp[]=
{
    SET_PROGRAM,
    SET_SUBPROGRAM,
    SET_WAIT_STAGE,
	SET_CUSTOM_SEGMENT_TIME,
	SET_RESISTANCE,
    SET_INCLINE,
    SET_PRESS_START
};

const unsigned char EP_CustomDistance_SetUp[]=
{
    SET_PROGRAM,
    SET_SUBPROGRAM,
    SET_WAIT_STAGE,
	SET_DISTANCE,
	SET_RESISTANCE,
    SET_INCLINE,
    SET_PRESS_START
};

const unsigned char TM_CustomHr_SetUp[]=
{
    SET_PROGRAM,
    SET_WAIT_STAGE,
    SET_CUSTOMHR_SEGMENT_TIME,
    SET_CUSTOMHR_SEGMENT_HEART,
    SET_TOTAL_TIME,
    SET_PRESS_START,
};

#endif
