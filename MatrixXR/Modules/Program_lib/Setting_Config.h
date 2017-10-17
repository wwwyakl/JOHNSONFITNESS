#include "Programs.h"
#ifndef __SETTING_CONFIG_H_
#define __SETTING_CONFIG_H_
const unsigned char TM_Manual_SetUp[]=
{
    //NO_SETTING,
	SET_PROGRAM,
	SET_TIME,
	SET_SPEED,
	SET_INCLINE,
    SET_PRESS_START
        
};
const unsigned char EP_Manual_SetUp[]=
{
//        NO_SETTING, 
	SET_PROGRAM,
	SET_TIME,
	SET_RESISTANCE,
        SET_PRESS_START
};
const unsigned char C50_Landmark_SetUp[]=
{
//        NO_SETTING, 
	SET_PROGRAM,
    SET_TARGET,
//	SET_TIME,
	SET_RESISTANCE,
        SET_PRESS_START
};
const unsigned char EP_Incline_Manual_SetUp[]=
{
//        NO_SETTING, 
	SET_PROGRAM,
	SET_TIME,
	SET_RESISTANCE,
	SET_INCLINE,
        SET_PRESS_START
};
const unsigned char THR_ZONE_SetUp[]=
{
//        NO_SETTING,
	SET_PROGRAM,
	SET_TIME,
	SET_HEART,
        SET_PRESS_START
};
const unsigned char Interval_HR_SetUp[]=
{
//        NO_SETTING,
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
//	NO_SETTING,
        SET_PROGRAM,
	SET_SEGMENT_TIME,
	SET_SEGMENT_1_HEART,
	SET_SEGMENT_2_HEART,
	SET_SEGMENT_3_HEART,
	SET_SEGMENT_4_HEART,
	SET_TIME,
        SET_PRESS_START
};
const unsigned char ProTable_SetUp[]=
{
//	NO_SETTING,
        SET_PROGRAM,
	SET_TIME,
	SET_LEVEL,
        SET_PRESS_START
};
const unsigned char ProSprint8_SetUp[]=
{
//	NO_SETTING,
        SET_PROGRAM,
	SET_LEVEL,
        SET_PRESS_START
};

const unsigned char PConstantWatt_SetUp[]=
{
//	NO_SETTING,
        SET_PROGRAM,
	SET_TIME,
	SET_WATTS,
        SET_PRESS_START
};

#endif
