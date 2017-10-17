#include "Global_Config.h"
#ifndef __TABLELIST_H_
#define __TABLELIST_H_

#define DOTMATRIX_LENGTH 16

#ifdef __PROGRAM_TABLE__
//TM
const STATUS_STAGEGOAL Distance_StatusStageGoal_TM[]={

{WarmUp,	100},
{WarmUp,	100},
{Repeat,	100},
{Repeat,	100},
{Repeat,	100},
{Repeat,	100},
{Repeat,	100},
{Repeat,	100},
{Repeat,	100},
{Repeat,	100},
{Repeat,	100},
{Repeat,	100},
{Repeat,	100},
{Repeat,	100},
{Repeat,	100},
{Repeat,	100},
{Repeat,	100},
};
const STATUS_STAGEGOAL Calories_StatusStageGoal_TM[]={

{WarmUp,	20},
{WarmUp,	20},
{Repeat,	20},
{Repeat,	20},
{Repeat,	20},
{Repeat,	20},
{Repeat,	20},
{Repeat,	20},
{Repeat,	20},
{Repeat,	20},
{Repeat,	20},
{Repeat,	20},
{Repeat,	20},
{Repeat,	20},
{Repeat,	20},
{Repeat,	20},
{Repeat,	20},
};
const UINT8 DistanceCalories_Incline_TM[]={

  0,	  0,	  0,	  0,	  0,	  0,	  0,	  0,	  0,	  0,	  0,	  0,	  0,	  0,	  0,	  0,	  0,
  0,	  5,	 10,	 30,	 20,	 30,	 20,	 30,	 25,	 35,	 25,	 30,	 20,	 30,	 20,	 30,	 10,
  5,	 10,	 15,	 35,	 15,	 35,	 25,	 35,	 25,	 40,	 25,	 35,	 25,	 35,	 15,	 35,	 15,
  5,	 10,	 15,	 35,	 25,	 35,	 25,	 35,	 30,	 40,	 30,	 35,	 25,	 35,	 25,	 35,	 15,
 10,	 15,	 20,	 40,	 20,	 40,	 30,	 40,	 30,	 45,	 30,	 40,	 30,	 40,	 20,	 40,	 20,
 10,	 15,	 20,	 40,	 30,	 40,	 30,	 40,	 35,	 45,	 35,	 40,	 30,	 40,	 30,	 40,	 20,
 15,	 20,	 25,	 45,	 25,	 45,	 35,	 45,	 35,	 50,	 35,	 45,	 35,	 45,	 25,	 45,	 25,
 15,	 20,	 25,	 45,	 35,	 45,	 35,	 45,	 40,	 50,	 40,	 45,	 35,	 45,	 35,	 45,	 25,
 20,	 25,	 30,	 50,	 30,	 50,	 40,	 50,	 40,	 55,	 40,	 50,	 40,	 50,	 30,	 50,	 30,
 20,	 25,	 30,	 50,	 40,	 50,	 40,	 50,	 45,	 55,	 45,	 50,	 40,	 50,	 40,	 50,	 30,
};
const UINT8 WeightLoss_Speed_TM[]={
5,  15, 20, 25, 30, 35, 40, 35, 30, 25, 23, 15,
5,	19,	25,	30,	35,	40,	45,	40,	35,	30, 26, 18,
5,	23,	30,	35,	40,	45,	50,	45,	40,	35, 30, 20,
10,	26,	35,	40,	45,	50,	55,	50,	45,	40, 34, 23,
10,	30,	40,	45,	50,	55,	60,	55,	50,	45, 38, 25,
10,	34,	45,	50,	55,	60,	65,	60,	55,	50, 41, 28,
14,	38,	50,	55,	60,	65,	70,	65,	60,	55, 45, 30,
14,	41,	55,	60,	65,	70,	75,	70,	65,	60, 49, 33,
14,	45,	60,	65,	70,	75,	80,	75,	70,	65, 53, 35,
14,	49,	65,	70,	75,	80,	85,	80,	75,	70, 56, 38,
};
const UINT8 WeightLoss_Incline_TM[]={
0,	5,	15,	15,	10,	5,	5,	5,	10,	15, 5, 0,
0,	5,	15,	15,	10,	5,	10,	5,	10,	15, 5, 0,
5,	10,	20,	20,	15,	10,	10,	10,	15,	20, 10,5,
5,	10,	20,	20,	15,	10,	10,	10,	15,	20, 10,5, 
10,	15,	25,	25,	20,	15,	15,	15,	20,	25, 15,10,
10,	15,	25,	25,	20,	15,	15,	15,	20,	25, 15,10,
15,	20,	30,	30,	25,	20,	20,	20,	25,	30, 20,15,
15,	20,	30,	30,	25,	20,	20,	20,	25,	30, 20,15,
20,	25,	35,	35,	30,	25,	25,	25,	30,	35, 25,20,
20,	25,	35,	35,	30,	25,	25,	25,	30,	35, 25,20,

};

const STATUS_STAGEGOAL WeightLoss_StatusStageGoal_TM[]={

{WarmUp,	120},
{WarmUp,	120},
{Repeat,	30},
{Repeat,	30},
{Repeat,	30},
{Repeat,	30},
{Repeat,	30},
{Repeat,	30},
{Repeat,	30},
{Repeat,	30},
{CoolDown,	120},     //zsf
{CoolDown,	120},    //zsf
};

const Pro_RunPara WeightLoss_Para_TM=
{
    TOTALGOAL_WITH_REPEAT,
    GOAL_TYPES_TIME,
    STAGEGOAL_TYPE_TIME,
    10,
    12,
    PROFILE_TYPE_SPEED,
    WeightLoss_StatusStageGoal_TM,
    WeightLoss_Speed_TM,
    WeightLoss_Incline_TM,
    NULL,
};
const UINT8 Intervals_Speed_TM[]={
	5,	15,	20,	40,   15,   10,
	5,	15,	20,	45,   15,   10,
	5,	19,	25,	50,   19,   13,
	10,	19,	25,	55,   19,   13,
	10,	23,	30,	60,   23,   15,
	10,	23,	30,	65,   23,   15,
	14,	26,	35,	70,   26,   18,
	14,	26,	35,	75,   26,   18,
	14,	30,	40,	80,   30,   20,
	14,	30,	40,	85,   30,   20,
};
const STATUS_STAGEGOAL Intervals_StatusStageGoal_TM[]={

	{WarmUp,	120},
	{WarmUp,	120},
	{Repeat,	90},
	{Repeat,	30},
        {CoolDown,	120},     //zsf
        {CoolDown,	120},    //zsf
};
const Pro_RunPara Intervals_Para_TM=
{
    TOTALGOAL_WITH_REPEAT,
    GOAL_TYPES_TIME,
    STAGEGOAL_TYPE_TIME,
    10,
    6,
    PROFILE_TYPE_SPEED,
    Intervals_StatusStageGoal_TM,
    Intervals_Speed_TM,
    NULL,
    NULL,
};


const Pro_RunPara Distance_Para_TM={
     TOTALGOAL_WITH_REPEAT,
    GOAL_TYPES_DISTANCE,
    STAGEGOAL_TYPE_DISTANCE,
    10,
    17,
    PROFILE_TYPE_INCLINE,
    Distance_StatusStageGoal_TM,
    NULL,
    DistanceCalories_Incline_TM,
    NULL
};
const Pro_RunPara Calories_Para_TM={
     TOTALGOAL_WITH_REPEAT,
    GOAL_TYPES_CALORIES,
    STAGEGOAL_TYPE_CALORIES,
    10,
    17,
    PROFILE_TYPE_INCLINE,
    Calories_StatusStageGoal_TM,
    NULL,
    DistanceCalories_Incline_TM,
    NULL
};
const STATUS_STAGEGOAL FatBurn_StatusStageGoal_TM[]={

{WarmUp,	120},
{WarmUp,	120},
{Repeat,	30},
{Repeat,	30},
{Repeat,	30},
{Repeat,	30},
{Repeat,	30},
{Repeat,	30},
{Repeat,	30},
{Repeat,	30},

};
const UINT8 FatBurn_Speed_TM[]={
5,	15,	20,	25,	30,	35,	40,	35,	30,	25,
5,	19,	25,	30,	35,	40,	45,	40,	35,	30,
5,	23,	30,	35,	40,	45,	50,	45,	40,	35,
10,	26,	35,	40,	45,	50,	55,	50,	45,	40,
10,	30,	40,	45,	50,	55,	60,	55,	50,	45,
10,	34,	45,	50,	55,	60,	65,	60,	55,	50,
14,	38,	50,	55,	60,	65,	70,	65,	60,	55,
14,	41,	55,	60,	65,	70,	75,	70,	65,	60,
14,	45,	60,	65,	70,	75,	80,	75,	70,	65,
14,	49,	65,	70,	75,	80,	85,	80,	75,	70,
};
const UINT8 FatBurn_Incline_TM[]={
0,	5,	15,	15,	10,	5,	5,	5,	10,	15,
0,	5,	15,	15,	10,	5,	10,	5,	10,	15,
5,	10,	20,	20,	15,	10,	10,	10,	15,	20,
5,	10,	20,	20,	15,	10,	10,	10,	15,	20,
10,	15,	25,	25,	20,	15,	15,	15,	20,	25,
10,	15,	25,	25,	20,	15,	15,	15,	20,	25,
15,	20,	30,	30,	25,	20,	20,	20,	25,	30,
15,	20,	30,	30,	25,	20,	20,	20,	25,	30,
20,	25,	35,	35,	30,	25,	25,	25,	30,	35,
20,	25,	35,	35,	30,	25,	25,	25,	30,	35,

};
const Pro_RunPara FatBurn_Para_TM={
     TOTALGOAL_WITH_REPEAT,
    GOAL_TYPES_TIME,
    STAGEGOAL_TYPE_TIME,
    10,
    10,
    PROFILE_TYPE_SPEED,
    FatBurn_StatusStageGoal_TM,
    FatBurn_Speed_TM,
    FatBurn_Incline_TM,
    NULL
};



const STATUS_STAGEGOAL HillClimb_StatusStageGoal_TM[]={

{WarmUp,	120},
{WarmUp,	120},
{Repeat,	30},
{Repeat,	30},
{Repeat,	30},
{Repeat,	30},
{Repeat,	30},
{Repeat,	30},
{Repeat,	30},
{Repeat,	30},
{Repeat,	30},
{Repeat,	30},
};
const UINT8 HillClimb_Incline_TM[]={
0,	0,	10,	15,	20,	25,	30,	30,	25,	20,	15,	10,
0,	0,	15,	20,	25,	30,	35,	35,	30,	25,	20,	15,
0,	10,	20,	25,	30,	35,	40,	40,	35,	30,	25,	20,
0,	15,	25,	30,	35,	40,	45,	45,	40,	35,	30,	25,
0,	15,	30,	35,	40,	45,	50,	50,	45,	40,	35,	30,
0,	15,	35,	40,	45,	50,	55,	55,	50,	45,	40,	35,
0,	15,	40,	45,	50,	55,	60,	60,	55,	50,	45,	40,
0,	20,	45,	50,	55,	60,	65,	65,	60,	55,	50,	45,
0,	20,	50,	55,	60,	65,	70,	70,	65,	60,	55,	50,
0,	20,	55,	60,	65,	70,	75,	75,	70,	65,	60,	55,
};

const Pro_RunPara HillClimb_Para_TM={
     TOTALGOAL_WITH_REPEAT,
    GOAL_TYPES_TIME,
    STAGEGOAL_TYPE_TIME,
    10,
    12,
    PROFILE_TYPE_INCLINE,
    HillClimb_StatusStageGoal_TM,
    NULL,
    HillClimb_Incline_TM,
    NULL
};
const STATUS_STAGEGOAL Sprint_StatusStageGoal_TM[]={

	{WarmUp,	120},
	{WarmUp,	120},
	{Repeat,	90},
	{Repeat,	30},
};
const UINT8 Sprint_Incline_TM[]={
	5,	15,	20,	40,
	5,	15,	20,	45,
	5,	19,	25,	50,
	10,	19,	25,	55,
	10,	23,	30,	60,
	10,	23,	30,	65,
	14,	26,	35,	70,
	14,	26,	35,	75,
	14,	30,	40,	80,
	14,	30,	40,	85,

};

const Pro_RunPara Sprint_Para_TM={
     TOTALGOAL_WITH_REPEAT,
    GOAL_TYPES_TIME,
    STAGEGOAL_TYPE_TIME,
    10,
    4,
    PROFILE_TYPE_INCLINE,
    Sprint_StatusStageGoal_TM,
    NULL,
    Sprint_Incline_TM,
    NULL
};
const UINT8 SubPrgIdArray[]=
{
    PS_CUSTOM_TIME,
    PS_CUSTOM_DISTANCE,
};

const PrgConfig TM_PrgConfig[]=
{
    {PS_MANUAL,NULL,0,NULL},
    {PS_WEIGHT_LOSS,NULL,0,&WeightLoss_Para_TM},
    {PS_INTERVALS,NULL,0,&Intervals_Para_TM},
    {PS_DISTANCE_GOAL,NULL,0,&Distance_Para_TM},
    {PS_CALORIES_GOAL,NULL,0,&Calories_Para_TM},
//    {PS_FATBURN,NULL,0,&FatBurn_Para_TM},
//    {PS_HILL_CLIMB,NULL,0,&HillClimb_Para_TM},
//    {PS_THR_ZONE,NULL,0,NULL},
//    {PS_MYFIRST5K,NULL,0,NULL},
//    {PS_CUSTOM_TIME,SubPrgIdArray,sizeof(SubPrgIdArray)/sizeof(SubPrgIdArray[0]),NULL},
    
//    {PS_SPRINTS,NULL,0,NULL},
//    {PS_CUSTOM_HR,NULL,0,NULL},
//    {PS_CUSTOM_DISTANCE,NULL,0,NULL},
//    {PS_CUSTOM,NULL,0,NULL},
};
ProgramLibPara PrgLib_TM_Config=
{
  TM_PrgConfig,
  sizeof(TM_PrgConfig)/sizeof(TM_PrgConfig[0]),
  DOTMATRIX_LENGTH
};

//EP

const STATUS_STAGEGOAL Distance_StatusStageGoal_EP[]={

{WarmUp,	100},
{WarmUp,	100},
{Repeat,	100},
{Repeat,	100},
{Repeat,	100},
{Repeat,	100},
{Repeat,	100},
{Repeat,	100},
{Repeat,	100},
{Repeat,	100},
{Repeat,	100},
{Repeat,	100},
{Repeat,	100},
{Repeat,	100},
};
const STATUS_STAGEGOAL Calories_StatusStageGoal_EP[]={

{WarmUp,	20},
{WarmUp,	20},
{Repeat,	20},
{Repeat,	20},
{Repeat,	20},
{Repeat,	20},
{Repeat,	20},
{Repeat,	20},
{Repeat,	20},
{Repeat,	20},
{Repeat,	20},
{Repeat,	20},
{Repeat,	20},
{Repeat,	20},
};
const STATUS_STAGEGOAL WeightLoss_StatusStageGoal_EP[]={

{WarmUp,	60},
{WarmUp,	60},
{WarmUp,	60},
{WarmUp,	60},
{Repeat,	60},
{Repeat,	60},
{Repeat,	60},
{Repeat,	60},
{Repeat,	60},
{Repeat,	60},
{Repeat,	60},
{Repeat,	60},
{Repeat,	60},
{Repeat,	60},
{Repeat,	60},
{Repeat,	60},
{CoolDown,	60},
{CoolDown,	60},
{CoolDown,	60},
{CoolDown,	60},
//{CoolDown,	120},
//{CoolDown,	120},
};
const STATUS_STAGEGOAL Intervals_StatusStageGoal_EP[]={
    {WarmUp,	60},
{WarmUp,	60},
{WarmUp,	60},
{WarmUp,	60},
// {Repeat,	60},
// {Repeat,	60},
{Repeat,	30},
{Repeat,	90},
{CoolDown,	60},//
{CoolDown,	60},//
{CoolDown,	60},//
{CoolDown,	60},//
};
const UINT8 DistanceCalories_Resistance_EP[]={

//1,	2,	3,	4,	5,	6,	7,	8,	9,	10,	11,	12,	13,	14,
	1,	1,	2,	2,	1,	3,	5,	2,	4,	5,	2,	3,	4,	1,
	1,	2,	2,	3,	3,	5,	7,	4,	6,	7,	4,	5,	6,	3,
	2,	2,	3,	3,	4,	6,	8,	5,	7,	8,	5,	6,	7,	4,
	2,	3,	3,	4,	6,	8,	10,	7,	9,	10,	7,	8,	9,	6,
	3,	4,	4,	4,	8,	10,	12,	9,	11,	12,	9,	10,	11,	8,
	3,	5,	4,	5,	10,	12,	14,	11,	13,	14,	11,	12,	13,	10,
	4,	5,	5,	5,	11,	13,	15,	12,	14,	15,	12,	13,	14,	11,
	4,	5,	5,	5,	13,	15,	17,	14,	16,	17,	14,	15,	16,	13,
	4,	5,	5,	5,	15,	17,	19,	16,	18,	19,	16,	17,	18,	15,
	4,	5,	6,	6,	16,	18,	20,	17,	19,	20,	17,	18,	19,	16,
};
const UINT8 Weightloss_Resistance_EP[]={

//1,	2,	3,	4,	5,	6,	7,	8,	9,	10,	11,	12,	13,	14,
    1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1,
    1, 1, 1, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1,
    1, 1, 1, 2, 3, 3, 3, 3, 4, 4, 4, 4, 3, 3, 3, 3, 2, 1, 1, 1,
    1, 1, 1, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 1, 1, 1,
    1, 2, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 4, 4, 4, 4, 3, 3, 2, 1,
    1, 2, 3, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 4, 4, 4, 3, 2, 1,
    1, 2, 3, 4, 5, 5, 5, 5, 6, 6, 6, 6, 5, 5, 5, 5, 4, 3, 2, 1,
    1, 2, 3, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 3, 2, 1,
    2, 2, 3, 5, 6, 6, 6, 6, 7, 7, 7, 7, 6, 6, 6, 6, 5, 3, 2, 2,
    2, 2, 4, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 4, 2, 2,
};
const UINT8 Intervals_Resistance_EP[]={
1, 1, 1, 1, 3, 1, 1, 1, 1, 1,
1, 1, 1, 2, 4, 1, 2, 1, 1, 1,
1, 1, 1, 2, 5, 2, 2, 1, 1, 1,
1, 1, 1, 3, 5, 3, 3, 1, 1, 1,
1, 2, 3, 3, 6, 3, 3, 3, 2, 1,
1, 2, 3, 4, 7, 4, 4, 3, 2, 1,
1, 2, 3, 4, 7, 5, 4, 3, 2, 1,
1, 2, 3, 5, 8, 5, 5, 3, 2, 1,
2, 2, 3, 5, 9, 6, 5, 3, 2, 2,
2, 2, 4, 6, 10,6, 6, 4, 2, 2,
};
const Pro_RunPara WeightLoss_Para_EP=
{
    TOTALGOAL_WITH_REPEAT,
    GOAL_TYPES_TIME,
    STAGEGOAL_TYPE_TIME,
    10,
//    10,
    20,//
//    PROFILE_TYPE_SPEED,
    PROFILE_TYPE_RESISTANCE,//
    WeightLoss_StatusStageGoal_EP,
    NULL,
    NULL,
    Weightloss_Resistance_EP,
};
const Pro_RunPara Intervals_Para_EP=
{
    TOTALGOAL_WITH_REPEAT,
    GOAL_TYPES_TIME,
    STAGEGOAL_TYPE_TIME,
    10,
    10,
//    PROFILE_TYPE_SPEED,
    PROFILE_TYPE_RESISTANCE,//
    Intervals_StatusStageGoal_EP,
    NULL,
    NULL,
    Intervals_Resistance_EP,
};
const Pro_RunPara Distance_Para_EP={
     TOTALGOAL_WITH_REPEAT,
    GOAL_TYPES_DISTANCE,
    STAGEGOAL_TYPE_DISTANCE,
    10,
    14,
    PROFILE_TYPE_RESISTANCE,
    Distance_StatusStageGoal_EP,
    NULL,
    NULL,
    DistanceCalories_Resistance_EP,
};
const Pro_RunPara Calories_Para_EP={
     TOTALGOAL_WITH_REPEAT,
    GOAL_TYPES_CALORIES,
    STAGEGOAL_TYPE_CALORIES,
    10,
    14,
    PROFILE_TYPE_RESISTANCE,
    Calories_StatusStageGoal_EP,
    NULL,
    NULL,
    DistanceCalories_Resistance_EP,
};
const STATUS_STAGEGOAL FatBurn_StatusStageGoal_EP[]={

{WarmUp,	60},
{WarmUp,	60},
{WarmUp,	60},
{WarmUp,	60},
{Repeat,	60},
{Repeat,	60},
{Repeat,	60},
{Repeat,	60},
{Repeat,	60},
{Repeat,	60},
{Repeat,	60},
{Repeat,	60},
{Repeat,	60},
{Repeat,	60},
{Repeat,	60},
{Repeat,	60},
};
const UINT8 FatBurn_Resistance_EP[]={
//1,	2,	3,	4,	5,	6,	7,	8,	9,	10,	11,	12,	13,	14,	15,	16,
	1,	2,	2,	3,	4,	3,	4,	5,	5,	3,	4,	6,	6,	5,	4,	3,
	2,	3,	3,	3,	5,	4,	5,	6,	6,	4,	5,	7,	7,	6,	5,	4,
	2,	4,	4,	5,	7,	6,	7,	8,	8,	6,	7,	9,	9,	8,	7,	6,
	3,	4,	5,	6,	8,	7,	8,	9,	9,	7,	8,	10,	10,	9,	8,	7,
	4,	6,	7,	7,	10,	9,	10,	11,	11,	9,	10,	12,	12,	11,	10,	9,
	4,	6,	7,	8,	11,	10,	11,	12,	12,	10,	11,	13,	13,	12,	11,	10,
	5,	7,	9,	9,	13,	12,	13,	14,	14,	12,	13,	15,	15,	14,	13,	12,
	5,	8,	9,	10,	14,	13,	14,	15,	15,	13,	14,	16,	16,	15,	14,	13,
	6,	9,	11,	12,	16,	15,	16,	17,	17,	15,	16,	18,	18,	17,	16,	15,
	7,	10,	12,	13,	18,	17,	18,	19,	19,	17,	18,	20,	20,	19,	18,	17,
};

const Pro_RunPara FatBurn_Para_EP={
     TOTALGOAL_WITH_REPEAT,
    GOAL_TYPES_TIME,
    STAGEGOAL_TYPE_TIME,
    10,
    16,
    PROFILE_TYPE_RESISTANCE,
    FatBurn_StatusStageGoal_EP,
    NULL,
    NULL,
    FatBurn_Resistance_EP,
};



const STATUS_STAGEGOAL HillClimb_StatusStageGoal_EP[]={

{WarmUp,	60},
{WarmUp,	60},
{WarmUp,	60},
{WarmUp,	60},
{Repeat,	60},
{Repeat,	60},
{Repeat,	60},
{Repeat,	60},
{Repeat,	60},
{Repeat,	60},
};
const UINT8 HillClimb_Incline_EP[]={
//1,	2,	3,	4,	5,	6,	7,	8,	9,	10,
	0,	10,	10,	20,	20,	20,	30,	30,	30,	30,
	0,	10,	10,	20,	30,	30,	30,	30,	40,	40,
	0,	10,	10,	30,	30,	30,	40,	40,	50,	50,
	0,	10,	10,	30,	40,	40,	50,	50,	50,	50,
	10,	20,	30,	40,	50,	50,	50,	50,	60,	60,
	10,	20,	30,	50,	50,	50,	60,	60,	70,	70,
	10,	20,	30,	50,	60,	60,	70,	70,	80,	80,
	10,	20,	30,	60,	70,	70,	80,	80,	80,	80,
	20,	30,	30,	70,	80,	80,	80,	80,	90,	90,
	20,	30,	50,	80,	80,	80,	90,	90,	90,	90,
};
const UINT8 HillClimb_Resistance_EP[]={
//1,	2,	3,	4,	5,	6,	7,	8,	9,	10,
	1,	2,	2,	3,	2,	3,	4,	5,	4,	3,
	1,	2,	2,	3,	4,	5,	6,	7,	6,	5,
	1,	2,	2,	4,	6,	7,	8,	9,	8,	7,
	1,	2,	2,	5,	8,	9,	10,	11,	10,	9,
	2,	3,	5,	6,	10,	11,	12,	13,	12,	11,
	2,	3,	5,	7,	12,	13,	14,	15,	14,	13,
	2,	3,	5,	8,	14,	15,	16,	17,	16,	15,
	2,	3,	5,	9,	15,	16,	17,	18,	17,	16,
	3,	4,	5,	10,	16,	17,	18,	19,	18,	17,
	3,	4,	8,	11,	17,	18,	19,	20,	19,	18,
};


const Pro_RunPara HillClimb_Para_EP={
     TOTALGOAL_WITH_REPEAT,
    GOAL_TYPES_TIME,
    STAGEGOAL_TYPE_TIME,
    10,
    10,
    PROFILE_TYPE_RESISTANCE,
    HillClimb_StatusStageGoal_EP,
    NULL,
    HillClimb_Incline_EP,
    HillClimb_Resistance_EP,
};
const STATUS_STAGEGOAL Sprint_StatusStageGoal_EP[]={

	{WarmUp,	60},
	{WarmUp,	60},
	{WarmUp,	60},
	{WarmUp,	60},
	{Repeat,	30},
	{Repeat,	90},
};
const UINT8 Sprint_Resistance_EP[]={
//1,	2,	3,	4,	5,	6,
	1,	2,	2,	3,	4,	1,
	1,	2,	2,	3,	6,	2,
	1,	2,	2,	4,	7,	3,
	1,	2,	2,	5,	9,	5,
	2,	3,	5,	6,	11,	6,
	2,	3,	5,	7,	13,	7,
	2,	3,	5,	8,	14,	8,
	2,	3,	5,	9,	16,	9,
	3,	4,	5,	10,	18,	11,
	3,	4,	8,	11,	19,	12,
};

const Pro_RunPara Sprint_Para_EP={
     TOTALGOAL_WITH_REPEAT,
    GOAL_TYPES_TIME,
    STAGEGOAL_TYPE_TIME,
    10,
    17,
    PROFILE_TYPE_INCLINE,
    Sprint_StatusStageGoal_EP,
    NULL,
    NULL,
    Sprint_Resistance_EP,
};

const PrgConfig EP_PrgConfig[]=
{
    {PS_MANUAL,NULL,0,NULL},
    {PS_WEIGHT_LOSS,NULL,0,&WeightLoss_Para_EP},
    {PS_INTERVALS,NULL,0,&Intervals_Para_EP},
    {PS_DISTANCE_GOAL,NULL,0,&Distance_Para_EP},
    {PS_CALORIES_GOAL,NULL,0,&Calories_Para_EP},
//  {PS_THR_ZONE,NULL,0,NULL},
//  {PS_MYFIRST5K,NULL,0,NULL},
//  {PS_CUSTOM_TIME,SubPrgIdArray,sizeof(SubPrgIdArray)/sizeof(SubPrgIdArray[0]),NULL},
//
//	{PS_SPRINTS,NULL,0,NULL},
//    {PS_CUSTOM_HR,NULL,0,NULL},
//    {PS_CUSTOM_DISTANCE,NULL,0,NULL},
//    {PS_CUSTOM,NULL,0,NULL},
};
ProgramLibPara PrgLib_EP_Config=
{
  EP_PrgConfig,
  sizeof(EP_PrgConfig)/sizeof(EP_PrgConfig[0]),
  DOTMATRIX_LENGTH
};

const UINT8 Weightloss_Resistance_AE[]={
   //1,	         2,	           3,            4,            5,	        6,7,8,9,10,11,12,13,14,
    1, 2, 2, 3,  3, 3, 4, 4,   5, 5, 5, 5,   4, 4, 3, 3,   3, 2, 2, 1,//1,
    1, 2, 2, 3,  4, 4, 5, 5,   6, 6, 6, 6,   5, 5, 4, 4,   3, 2, 2, 1,//2,
    1, 2, 2, 4,  5, 5, 6, 6,   7, 7, 7, 7,   6, 6, 5, 5,   4, 2, 2, 1,
    1, 2, 2, 5,  6, 6, 7, 7,   8, 8, 8, 8,   7, 7, 6, 6,   5, 2, 2, 1,
    2, 3, 5, 6,  7, 7, 8, 8,   9, 9, 9, 9,   8, 8, 7, 7,   6, 5, 3, 2,
    2, 3, 5, 7,  8, 8, 9, 9,   10, 10, 10, 10,   9, 9, 8, 8,   7, 5, 3, 2,
    2, 3, 5, 8,  9, 9, 10, 10,   11, 11, 11, 11,   10, 10, 9, 9,   8, 5, 3, 2,
    2, 3, 5, 9,  10, 10, 11, 11,   12, 12, 12, 12,   11, 11, 10, 10,   9, 5, 3, 2,
    3, 4, 5, 10,  11, 11, 12, 12,   13, 13, 13, 13,   12, 12, 11, 11,   10, 5, 4, 3,
    3, 4, 8, 11,  12, 12, 13, 13,   14, 14, 14, 14,   13, 13, 12, 12,   11, 8, 4, 3,
    3, 6, 10, 12,  13, 13, 14, 14,   15, 15, 15, 15,   14, 14, 13, 13,   12, 10, 6, 3,//11,
    3, 6, 10, 13,  14, 14, 15, 15,   16, 16, 16, 16,   15, 15, 14, 14,   13, 10, 6, 3,
    5, 9, 13, 14,  15, 15, 16, 16,   17, 17, 17, 17,   16, 16, 15, 15,   14, 13, 9, 5,
    5, 9, 13, 15,  16, 16, 17, 17,   18, 18, 18, 18,   17, 17, 16, 16,   15, 13, 9, 5,
    5, 9, 13, 16,  17, 17, 18, 18,   19, 19, 19, 19,   18, 18, 17, 17,   16, 13, 9, 5,
    5, 9, 13, 17,  18, 18, 19, 19,   20, 20, 20, 20,   19, 19, 18, 18,   17, 13, 9, 5,//16,
};

const UINT8 Intervals_Resistance_AE[]={
   //1,   2,     3,        4,    5,     6,7,8,9,10,
    1, 1, 1, 1,  5,   1,   1, 1, 1, 1,//1,
    1, 1, 1, 2,  6,   2,   2, 1, 1, 1,//2,
    1, 1, 1, 2,  7,   3,   2, 1, 1, 1,
    1, 1, 1, 3,  8,   4,   3, 1, 1, 1,
    1, 2, 3, 3,  9,   5,   3, 3, 2, 1,
    1, 2, 3, 4,  10,  6,   4, 3, 2, 1,
    1, 2, 3, 4,  11,  7,   4, 3, 2, 1,
    1, 2, 3, 5,  12,  8,   5, 3, 2, 1,
    2, 2, 3, 5,  13,  9,   5, 3, 2, 2,
    2, 2, 4, 6,  14, 10,   6, 4, 2, 2,
    2, 3, 4, 6,  15, 11,   6, 4, 3, 2,//11,
    2, 3, 4, 7,  16, 12,   7, 4, 3, 2,
    2, 3, 4, 7,  17, 13,   7, 4, 3, 2,
    2, 3, 5, 8,  18, 14,   8, 5, 3, 2,
    2, 3, 5, 8,  19, 15,   8, 5, 3, 2,
    2, 3, 5, 8,  20, 16,   8, 5, 3, 2,//16,
};

const Pro_RunPara WeightLoss_Para_AE=
{
    TOTALGOAL_WITH_REPEAT,
    GOAL_TYPES_TIME,
    STAGEGOAL_TYPE_TIME,
    16,
//    10,
    20,//
//    PROFILE_TYPE_SPEED,
    PROFILE_TYPE_RESISTANCE,//
    WeightLoss_StatusStageGoal_EP,
    NULL,
    NULL,
    Weightloss_Resistance_AE,
};

const Pro_RunPara Intervals_Para_AE=
{
    TOTALGOAL_WITH_REPEAT,
    GOAL_TYPES_TIME,
    STAGEGOAL_TYPE_TIME,
    16,
    10,
//    PROFILE_TYPE_SPEED,
    PROFILE_TYPE_RESISTANCE,//
    Intervals_StatusStageGoal_EP,
    NULL,
    NULL,
    Intervals_Resistance_AE,
};

const PrgConfig AE_PrgConfig[]=
{
    {PS_MANUAL,NULL,0,NULL},
    {PS_WEIGHT_LOSS,NULL,0,&WeightLoss_Para_AE},
    {PS_INTERVALS,NULL,0,&Intervals_Para_AE},
    {PS_DISTANCE_GOAL,NULL,0,&Distance_Para_EP},
    {PS_CALORIES_GOAL,NULL,0,&Calories_Para_EP},
//  {PS_THR_ZONE,NULL,0,NULL},
//  {PS_MYFIRST5K,NULL,0,NULL},
//  {PS_CUSTOM_TIME,SubPrgIdArray,sizeof(SubPrgIdArray)/sizeof(SubPrgIdArray[0]),NULL},
//
//	{PS_SPRINTS,NULL,0,NULL},
//    {PS_CUSTOM_HR,NULL,0,NULL},
//    {PS_CUSTOM_DISTANCE,NULL,0,NULL},
//    {PS_CUSTOM,NULL,0,NULL},
};

ProgramLibPara PrgLib_AE_Config=
{
  AE_PrgConfig,
  sizeof(AE_PrgConfig)/sizeof(AE_PrgConfig[0]),
  DOTMATRIX_LENGTH
};




#endif


#endif

