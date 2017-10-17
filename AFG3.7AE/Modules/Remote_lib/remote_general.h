#ifndef __REMOTE_GENERAL_H__
    #define __REMOTE_GENERAL_H__
    
#include "remote_config.h"
#include "remote_datatype.h"

#ifdef __REMOTE_GENERAL__


#pragma pack( push, 1 )
typedef struct
{
	UINT8 userConsoleId;
	char xid[XIDMAXLENGTH];
	char passcode[4];
	UINT8 gender;
	UINT16 weightX10;
	UINT8 age;
	UINT16 year;
	UINT8 month;
	UINT8 day;
	UINT8 langauge;
    UINT8 displayUnit;
	char name[65];
    UINT8 AcceptedTerms;
    UINT8 ShareWorkoutData;
//    char MMFtokenID[40];
//    char MFPtokenID[320];
//    char MFPuserID[15];
} SetUserData;

typedef struct
{
    char userExternalId[36];	
    UINT8 programType;
    UINT8 workoutState;
    UINT8 stage;
    UINT16 stageTimeSeconds;
    UINT16 workoutTimeSeconds;
    UINT16 warmupTimeSeconds;
    UINT16 cooldownTimeSeconds;
    UINT16 timeInStateSeconds;
    UINT16 timeInStageSeconds;
    UINT16 mphx100;
    UINT16 inclinex10;
    UINT16 averagePace;
    UINT16 totalCalories;
    UINT16 caloriesPerHour;
    UINT16 metsX10;
    UINT16 watts;
    UINT16 milesx100;
    UINT16 heartRate;
    UINT16 averageHeartRate;
    UINT16 rpm;
    UINT16 programLevel;
    UINT16 resistanceLevel;
    UINT16 thr;
    UINT16 targetWattsX10;
    UINT16 nextIncline;
    UINT16 nextSpeed;
    UINT8 nextResistance;
    UINT16 totalTime;
} WorkoutDataRemote;

typedef struct
{
	UINT8 brand;
	UINT8 machineType;
	UINT8 modelType;
	UINT8 driveType;
	UINT8 csafeManufacture;
	UINT8 csafeId;
	UINT8 csafeModelId;
	char serialNumber[15];
	char frameSerialNumber[15];
	char machineIdentification[36];
	UINT16 clubId;
	UINT16 minMPHX10;
	UINT16 maxMPHX10;
	UINT16 minKPHX10;
	UINT16 maxKPHX10;
	UINT16 minInclineX10;
	UINT16 maxInclineX10;
	UINT16 minLevel;
	UINT16 maxLevel;
	UINT32 accumulatedDistanceMilesX100;
	UINT32 accumulatedSeconds;
	UINT32 softwareVersion;
	UINT16 DapiVersion;
	UINT16 wifiProtocolVersion;
	UINT16 wifiUARTVersion;
	UINT16 btUARTVersion;
	UINT8 lcbVendor;
	UINT16 lcbVersion;
	UINT16 ioVersion;
	UINT16 osVersion;
	UINT8 numUsers;
	UINT8 macAddress[6];
	UINT8 ipAddress[6];
	UINT16 errorCode;
	UINT16 serviceCode;
	UINT16 motorTemperature;
	UINT16 busCurrent;
	UINT16 busVoltage;
	//char timeZone[65];
	UINT32 lubeMiles;
	UINT8  AutoLogin;

} MachineInfo;
typedef struct
{
    UINT8 userslot;
	char name[65];
    UINT16 weightX10;
    UINT16 year;
    UINT8 month;
    UINT8 day;
    UINT8 displayUnit;
    UINT16 customprogramcrc;
    UINT16 customheartratecrc;
    UINT8 myfirst5kweek;
    UINT8 myfirst5kworkout;
    UINT8 myfirst5kwalkspeed;
    UINT8 myfirst5kjogspeed;
    UINT16 myfirst5kresetcount;
    char MMFtokenID[40];
    char MFPtokenID[320];
    char MFPuserID[15];
} SyncUserData;
typedef struct{
    UINT16 workout1time;
    UINT16 workout1calorie;
    UINT16 workout1distance;
    UINT8 workout1maxspeed;
    UINT8 workout1averagespeed;
    UINT8 workout1maxheartrate;
    UINT8 workout1averageheartrate;
    UINT8 workout1elevation;
    UINT8 workout1units;
    UINT16 workout1startyear;
    UINT8 workout1startmonth;
    UINT8 workout1startday;
    UINT8 workout1starthour;
    UINT8 workout1startminute;
    UINT8 workout1startsecond;
}WorkoutSummaryData;

typedef struct
{
	char name[65];
    UINT8 userslot;
    char MMFtokenID[40];
    char MFPtokenID[320];
    char MFPuserID[15];
    UINT8 workoutscounter;
    
    WorkoutSummaryData worksummary[10];
} PushWorkoutSummary;
typedef struct
{
    UINT16 workouttime;
    UINT16 workoucalorie;
    UINT16 Workoutdistance;
    UINT8  WorkoutMaxspeed;
    UINT8  WorkoutAveragespeed;
    UINT8  WorkoutMaxHeartrate;
    UINT8  WorkoutAverageheartrate;
    UINT8  WorkoutElevation;
    UINT8  WorkoutUnits;
    UINT16 Workoutstartyear;
    UINT8  Workoutstartmonth;
    UINT8  Workoutstartday;
    UINT8  Workoutstarthour;
    UINT8  Workoutstartminute;
    UINT8  Workoutstartsecond;
    UINT16 Workoutpace;
    UINT8  MyFirst5K_WEEK;
    UINT8  MyFirst5K_WORKOUT;
    UINT8  MyFirst5K_NEXT_WORKOUT_IN_DAYS;
    UINT8  MyFirst5K_FINISHED;
}StopWorkoutData;
typedef struct
{
    UINT16 Total_time;
    UINT8 Segment1_age;
    UINT16 Segment1_time;
    UINT8 Segment1_target_HR;
    UINT8 Segment2_age;
    UINT16 Segment2_time;
    UINT8 Segment2_target_HR;
    UINT8 Segment3_age;
    UINT16 Segment3_time;
    UINT8 Segment3_target_HR;
    UINT8 Segment4_age;
    UINT16 Segment4_time;
    UINT8 Segment4_target_HR;
}CustomHRParameter;
typedef struct
{
    UINT8 User_slot;
    
    CustomHRParameter Parameter;
}SetCustomHRprogram;
typedef struct
{
    UINT16 Goal;
    UINT16 speed;
    UINT8 incline;    
}CustomParameter;
typedef struct
{
    UINT8 User_slot;
    UINT8 TimeCustomSegnumber;
    UINT8 DistanceCustomSegnumber;
    UINT8 Units;
    CustomParameter Time[16];
    CustomParameter Distance[16];
}SetCustomprogram;
typedef struct
{
   UINT8 userSlot;
   UINT8 week;
   UINT8 workout;
   UINT8 walkSpeed;
   UINT8 jogSpeed;
   UINT16 resetCount;
}SetMyFirst5K;
typedef struct
{
    UINT8 popuptype;
    UINT8 disappeartime;
}PopupInformation;
typedef struct
{
    UINT8 messageType;
    UINT16 workouttime;
    UINT16 workoucalorie;
    UINT16 Workoutdistance;
    UINT8  WorkoutMaxspeed;
    UINT8  WorkoutAveragespeed;
    UINT8  WorkoutMaxHeartrate;
    UINT8  WorkoutAverageheartrate;
    UINT8  WorkoutElevation;
    UINT8  WorkoutUnits;
    UINT16 Workoutstartyear;
    UINT8  Workoutstartmonth;
    UINT8  Workoutstartday;
    UINT8  Workoutstarthour;
    UINT8  Workoutstartminute;
    UINT8  Workoutstartsecond;
    UINT16 Workoutpace;
    UINT8  MyFirst5K_WEEK;
    UINT8  MyFirst5K_WORKOUT;
    UINT8  MyFirst5K_NEXT_WORKOUT_IN_DAYS;
    UINT8  MyFirst5K_FINISHED;
} StopDataAck;
//typedef union
//{
//    struct
//    {
//        UINT8 userID;
//        UINT16 TimeWorkoutTime[MAX_CUSTOM_SEGMENTS];
//        UINT8 TimeSpeedX10[MAX_CUSTOM_SEGMENTS];
//        UINT8 TimeInclineX10[MAX_CUSTOM_SEGMENTS];
//        UINT16 DistanceWorkoutDistance[MAX_CUSTOM_SEGMENTS];
//        UINT8 DistanceSpeedX10[MAX_CUSTOM_SEGMENTS];
//        UINT8 DistanceInclineX10[MAX_CUSTOM_SEGMENTS];
//    }CUSTOM_INFO;
//    UINT8 custominfobuf[sizeof(CUSTOM_INFO)];
//}CUSTOM_UNION;
typedef struct
{
    UINT16 TimeWorkoutTime;
    UINT16 TimeSpeedX10;
    UINT8 TimeInclineX10; 
}CUSTOM_TIME_DATA;
typedef union
{
    struct
    {
        UINT8 userID;
        UINT8 TimeCustomSegnumber;
        UINT8 DistanceCustomSegnumber;
//        UINT8 Units;
        CUSTOM_TIME_DATA customtimeDATA[MAX_CUSTOM_SEGMENTS];
        CUSTOM_TIME_DATA customdisDATA[MAX_CUSTOM_SEGMENTS];
    }CUSTOM_INFO;
    UINT8 custominfobuf[sizeof(CUSTOM_INFO)];
}CUSTOM_UNION;
#pragma pack( pop)

SubProcessState RemoteGeneralProcess(RemoteDevicesInformation *deviceinfor, UINT8* buf, UINT16 *bufLength,const  DapiGlobalVariable* dapiGlobalVariable);
RemoteResponse RemoteGeneralReceive(SubReceiveDataType datatype, ADDSTATESFUNC addstatefunc, DapiGlobalVariable* dapiGlobalVariable );



#endif
#endif
