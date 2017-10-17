#include "JISdef.h"
#include "Global_Config.h"

#define SYSTEMCONFIG_VISION_CODE "1.0.5"

#ifndef SYSTEMCONFIG_H__
#define SYSTEMCONFIG_H__

#define MAX_CUSTOM_SEGMENTS                 15

#define MAX_SUPPORTED_USERS                 5
#define MAX_SUPPORTED_CUSTOM_PRGS           4


#define D_CUSTOM_SPEED			    5
#define D_CUSTOM_TIME			    30*60
#define D_CUSTOM_INCLINE	            0
#define D_CUSTOM_RESISTANCE		    1


#define USB_UPDATE_AVAILABLE                 0x81
#define WIFI_UPDATE_AVAILABLE                0x01

extern const SystemSettingType SystemSettingArray[];

typedef enum
{
    //------------------1 byte returned data----------------
    FIRST_BOOT_FLAG_UCHAR = 0,
	IS_MACHINE_CONFIGED_UCHAR,
    MACHINE_TYPE_UINT8,
    MACHINE_MODLE_UINT8,
    BRAND_ID_UINT8,
    LANGUAGE_ID_UINT8,
    UNIT_FLAG_UCHAR,
    DRIVE_TYPE_UINT8,
    DISPLAY_MODE_UCHAR,
    FIRMWARE_MAJOR_UCHAR,
    FIRMWARE_MINOR_UCHAR,
    FIRMWARE_REVISION_UCHAR,
    FIRMWARE_FORCE_UPDATE_UCHAR,
    NEW_FIRMWARE_EXISTS_UCHAR,
    MAX_RESISTANCE_UINT8,
    MIN_RESISTANCE_UINT8,
    DEMO_MODE_FLAG_UCHAR,
    ENERGY_SAVER_FLAG_UCHAR,
    BUZZER_FLAG_UCHAR,
    RTC_USED_FLAG_UCHAR,
    RTC_TIME_FORMAT_UCHAR,
    DEFALUT_VOLUME_UINT8,
    CSAFE_ID_UINT8,
    CSAFE_MANUFACTURE_ID_UINT8,
    CSAFE_MODEL_ID_UINT8,
    CLUB_ID_UINT8,
    AUTO_LOGIN_FLAG_UCHAR,
    AP_MODE_FLAG_UCHAR,	
    RF_CHANNEL_UCHAR,
    RF_ADDRESS_UCHAR,
    MYKEY_MIN_SPEED_UINT8,
    MYKEY_MID_SPEED_UINT8,
    MYKEY_MAX_SPEED_UINT8,
    MYKEY_MIN_INCLINE_UINT8,
    MYKEY_MID_INCLINE_UINT8,
    MYKEY_MAX_INCLINE_UINT8,
    MYKEY_MIN_RESISTANCE_UINT8,
    MYKEY_MID_RESISTANCE_UINT8,
    MYKEY_MAX_RESISTANCE_UINT8,
    DAYS_TO_ACCHIEVE_GOAL_UINT8,
    OVERSPEED_SWITCH_UINT8,
    OVERSPEED_VALUE,
    SHOW_ERROR_FLAG_UCHAR,
    CONTROL_ZONE_FLAG_UCHAR,
    SECOND_CONTROL_ZONE_FLAG_UCHAR,
    IR_SENSOR_FLAG_UCHAR,
    RSCU_FLAG_UCHAR,
    TOTAL_ITEMS_INT8,
    //----------------2 bytes --------------------------
    MAX_WEIGHT_STANDARD_UINT16,
    MIN_WEIGHT_STANDARD_UINT16,
    MAX_WEIGHT_METRIC_UINT16,
    MIN_WEIGHT_METRIC_UINT16,
    MAX_MPHX10_UINT16,
    MIN_MPHX10_UINT16,
    MAX_KPHX10_UINT16,
    MIN_KPHX10_UINT16,
    MAX_MPH_RPM_UINT16,
    MIN_MPH_RPM_UINT16,
    MAX_KPH_RPM_UINT16,
    MIN_KPH_RPM_UINT16,
    MAX_INCLINEX10_UINT16,
    MIN_INCLINEX10_UINT16,
    MAX_INCLINEADC_UINT16,
    MIN_INLCINEADC_UINT16,
    ACCUMULATED_DATASAVEDTIMES_UINT16,
    ACCUMULATED_AGING_HOURS_UINT16,
    DEFAULT_BRIGHTNESS_UINT16,    
    DEFAULT_WORKOUT_MIN_UINT16,
    MAX_WORKOUT_MIN_UINT16,
    DEFAULT_WARMUP_MIN_UINT16,
    DEFAULT_COOLDOWN_MIN_UINT16,
    DEFAULT_PAUSE_MIN_UINT16,
    DEFAULT_HR_TARGET_UINT16,
    DEFAULT_PHR_TARGET_UINT16,
	MAX_AGE_UINT16,
    DEFAULT_AGE_UINT16,
	MIN_AGE_UINT16,
    DEFAULT_WEIGHTLBSX100_UINT16,
    DEFAULT_WEIGHTKGX100_UINT16,
    DEFAULT_WATTS_TARGET_UINT16,
    CUSTOM1_WORKOUT_MIN_UINT16,
    CUSTOM2_WORKOUT_MIN_UINT16,
    CUSTOM3_WOKROUT_MIN_UINT16,
    CUSTOM4_WORKOUT_MIN_UINT16,
    TIME_GOAL_MIN_UINT16,
    DISTANCE_GOAL_MIN_UINT16,
    CALORIES_GOAL_MIN_UINT16,
    //STAMP_OF_GOAL_RTC,
    DEMO_WAIT_MIN_UINT16,
    ERP_WAIT_MIN_UINT16,
    TOTAL_ITEMS_INT16,
    
    //-----------------------4 bytes---------------------
    ACCUMULATED_MILESX100_LONG,
    ACCUMULATED_SECONDS_LONG,
    ACCUMULATED_LUBE_MILESX100_LONG,
    ACCUMULATED_LUBE_TIME,
    TOTAL_ITEMS_LONG,


    //----------------------Array--------------------------
    CONSOLE_SERIAL_NUMBER_ARRAY,
    FRAME_SERIAL_NUMBER_ARRAY,
    MARIO_BOX_ADDRESS_STRING,
    MACHINE_UUIND_ARRAY,
    RTC_TIME_ZONE_STRING,
    CUSTOM1_SEGMENTS_ARRAY_SPEEDX10,
    CUSTOM2_SEGMENTS_ARRAY_SPEEDX10,
    CUSTOM3_SEGMENTS_ARRAY_SPEEDX10,
    CUSTOM4_SEGMENTS_ARRAY_SPEEDX10,
    CUSTOM1_SEGMENTS_ARRAY_INCLINEX10,
    CUSTOM2_SEGMENTS_ARRAY_INCLINEX10,
    CUSTOM3_SEGMENTS_ARRAY_INCLINEX10,
    CUSTOM4_SEGMENTS_ARRAY_INCLINEX10,
    CUSTOM1_SEGMENTS_ARRAY_RESISTANCE,
    CUSTOM2_SEGMENTS_ARRAY_RESISTANCE,
    CUSTOM3_SEGMENTS_ARRAY_RESISTANCE,
    CUSTOM4_SEGMENTS_ARRAY_RESISTANCE,
    SAVED_WORKOUT1_ARRAY,
    SAVED_WORKOUT2_ARRAY,
    SAVED_WORKOUT3_ARRAY,
    SAVED_WORKOUT4_ARRAY,
    USER1_INFO_STRING,
    USER2_INFO_STRING,
    USER3_INFO_STRING,
    USER4_INFO_STRING,
    USER5_INFO_STRING,
    ALL_SYSTEM_CONFIG_TYPES
}SYSTEM_CONFIG_TYPE;


typedef enum{
	SVK_NONE = 0,
	SVK_RESETDATA,
	SVK_TOGGLE_FIRSTBOOT,
	SVK_TOGGLE_OVERSPEEDFLAG,
	SVK_TOGGLE_OVERSPEEDVALUE,
	SVK_TOGGLE_IS_MACHINE_CONFIGED,
    SVK_TOGGLE_SHOW_ERROR,
    SVK_CONTROL_ZONE,
    SVK_SECOND_CONTROL_ZONE,
    SVK_IR_SENSOR,
    SVK_RSCU,
	SVK_MACHINETYPE_UP,
	SVK_MACHINETYPE_DOWN,
	SVK_SET_MACHINETYPE,
	SVK_SET_MACHINEMODEL,
	SVK_BRANDID_UP,
	SVK_BRANDID_DOWN,
	SVK_LANGUAGEID_UP,
	SVK_LANGUAGEID_DOWN,
	SVK_SET_LANGUAGEID,
	SVK_TOGGLE_UNIT,
	SVK_DRIVETYPE_UP,
	SVK_DRIVETYPE_DOWN,
	SVK_SET_DRIVETYPE,
	SVK_MAXWEIGHT_UP,
	SVK_MAXWEIGHT_DOWN,
	SVK_DISPLAYMODE_UP,
	SVK_DISPLAYMODE_DOWN,
	SVK_FIRMWARE_MAJOR_UP,
	SVK_FIRMWARE_MAJOR_DOWN,
	SVK_FIRMWARE_MINOR_UP,
	SVK_FIRMWARE_MINOR_DOWN,
	SVK_FIRMWARE_REV_UP,
	SVK_FIRMWARE_REV_DOWN,	
	SVK_SET_MAX_WEIGHT_STANDARD,
	SVK_SET_MIN_WEIGHT_STANDARD,
	SVK_SET_MAX_WEIGHT_METRIC,
	SVK_SET_MIN_WEIGHT_METRIC,
	SVK_SET_MAX_MPHX10,
	SVK_SET_MIN_MPHX10,
 	SVK_SET_MAX_KPHX10,
	SVK_SET_MIN_KPHX10,
	SVK_SET_MAX_MPH_RPM,
	SVK_SET_MIN_MPH_RPM,
	SVK_SET_MAX_KPH_RPM,
	SVK_SET_MIN_KPH_RPM,
	SVK_SET_MAX_INCLINEX10,
	SVK_SET_MIN_INCLINEX10,
	SVK_SET_MAX_INCLINEADC,
	SVK_SET_MIN_INLCINEADC,
	SVK_SET_MAX_RESISTANCE,
	SVK_SET_MIN_RESISTANCE,
	SVK_MAXSPEED_UP,
	SVK_MAXSPEED_DOWN,
	SVK_MAXSPEED_CONFIRM,
	SVK_MINSPEED_UP,
	SVK_MINSPEED_DOWN,
	SVK_MINSPEED_CONFIRM,
	SVK_MAXRPM_UP,
	SVK_MAXRPM_DOWN,
	SVK_MAXRPM_CONFIRM,
	SVK_MINRPM_UP,
	SVK_MINRPM_DOWN,
	SVK_MINRPM_CONFIRM,
	SVK_MAXINCLINE_UP,
	SVK_MAXINCLINE_DOWN,
	SVK_MININCLINE_UP,
	SVK_MININCLINE_DOWN,
	SVK_MAXINCLINE_ADC_UP,
	SVK_MAXINCLINE_ADC_DOWN,
	SVK_MININCLINE_ADC_UP,
	SVK_MININCLINE_ADC_DOWN,
	SVK_NUM0,
	SVK_NUM1,
	SVK_NUM2,
	SVK_NUM3,
	SVK_NUM4,
	SVK_NUM5,
	SVK_NUM6,
	SVK_NUM7,
	SVK_NUM8,
	SVK_NUM9,
	SVK_TOGGLE_DEMO,
	SVK_TOGGLE_ERP,
	SVK_TOGGLE_BUZZER,
	SVK_TOGGLE_RTC,
	SVK_TOGGLE_TIMEFORMAT,
	SVK_DEFAULT_VOL_UP,
	SVK_DEFAULT_VOL_DOWN,
	SVK_CONSOLE_SERIAL_UP,
	SVK_CONSOLE_SERIAL_DOWN,
	SVK_CONSOLE_SERIAL_NEXT,
	SVK_FRAME_SERIAL_UP,
	SVK_FRAME_SERIAL_DOWN,
	SVK_FRAME_SERIAL_NEXT,
	SVK_CSAFEID_UP,
	SVK_CSAFEID_DOWN,
	SVK_CSAFEID_CONFIRM,
	SVK_CSAFEMANUFACTUREID_UP,
	SVK_CSAFEMANUFACTUREID_DOWN,
	SVK_CSAFEMANUFACTUREID_CONFIRM,
	SVK_CSAFEMODELID_UP,
	SVK_CSAFEMODELID_DOWN,
	SVK_CSAFEMODELID_CONFIRM,
	SVK_CLUBID_UP,
	SVK_CLUBID_DOWN,
	SVK_CLUBID_CONFIRM,
	SVK_TOGGLE_AUTOLOGIN,
	SVK_TOGGLE_APMODE,
	SVK_DEFAULT_WORKOUT_TIME_UP,
	SVK_DEFAULT_WORKOUT_TIME_DOWN,
	SVK_DEFAULT_WORKOUT_TIME_CONFIRM,
	SVK_ACCAGING_HOURS,
	SVK_ADD_ACCUMULATED_MILESX100,
	SVK_ADD_ACCUMULATED_SECONDS,
	SVK_ADD_ACCUMULATED_LUBE_MILESX100,
    SVK_ADD_ACCUMULATED_LUBE_TIME,
	SVK_SET_RF_CHANNEL,
	SVK_SET_RF_ADDRESS,
	SVK_SET_ERP_TIMES,
	SVK_SET_DEMO_TIMES,
	SVK_SAVE_USER2FLASH,
	SVK_SAVE_MACHINEINFO2FLASH,
	SVK_SAVE_ACCUMULATEDDATA2FLASH,
	SVK_SAVE_CUSTOMPRG2FLASH,
	SVK_TOATAL
}SYSTEM_VIRTUAL_KEY;


/*
@ Brief: Data will be stored to FLASH_DEVICE_INFO_ADDRESS
*        0x39400
*/
#pragma pack( push, 1 )
typedef struct{
     //------------------General-------------------------
     UINT8 machineType;
     UINT8 machineModel;
     UINT8 consoletype;
     UINT8 brandId;
     UINT8 languageId;
     UCHAR unit;
     UINT8 driveType;
     UINT16 maxUserWeightLbs;
     UINT16 minUserWeightLbs;
     UINT16 maxUserWeightKg;
     UINT16 minUserWeightKg;
     UCHAR displayMode;
     UCHAR versionMajor;
     UCHAR versionMinor;
     UCHAR versionRevision;
     UCHAR isforceUpdate;
     UCHAR isFirmwareAvailable;
     UINT16 maxSpeedMphX10;
     UINT16 minSpeedMphX10;
     UINT16 maxSpeedKphX10;
     UINT16 minSpeedKphX10;
     UINT16 maxRpmMphX10;
     UINT16 minRpmMphX10;
     UINT16 maxRpmKphX10;
     UINT16 minRpmKphX10;
     UINT16 maxInclineX10;
     UINT16 minInclineX10;
     UINT16 maxInclineADC;
     UINT16 minInclineADC;
     UINT8 maxResistance;
     UINT8 minResistance;
     UCHAR isDemoModeOn;
     UCHAR isEnergySaverModeOn;
     UCHAR isBuzzerOn;
     UCHAR wasRTCPreset;
     UCHAR is24HoursFormat;
     UINT16 accumulatedAgingHours;
     UINT8 defaultVolume;
     UINT16 defaulitBrightness;
     UINT16 MaxAge;
     UINT16 defaultAge;
     UINT16 MinAge;
     UINT16 defaultWeightLBSX100;
     UINT16 defaultWeightKGX100;
     UINT8 defaultGender;
     UINT8 targetHR;
     char consoleSerialNumber[MAX_CONSOLE_SERIAL_NUMBER];
     char frameSerialNumber[MAX_CONSOLE_SERIAL_NUMBER];
     //--------------------------------Csafe-------------------------------------------
     UINT8 csafeId;
     UINT8 csafeManufactureId;
     UINT8 csafeModeId;
     UINT8 clubId;   //for commercial & specialty products
     //--------------------------------WiFi------------------------------------------------
     UCHAR isAutoLogin;
     char marioBox[MAX_MAIRO_BOX];
     char machineUUID[MAX_MACHINE_UUID];
     UCHAR isAPModeOn;	
     char timezone[MAX_TIME_ZONE];
     UINT8 lastLoggedUser;
     //-------------------RF------------------------------------------------
     UCHAR rfChannel;
     UCHAR rfAddress;
     //    UCHAR timeFormat;
     UINT16 erpOnWaitMinutes;
     UINT16 demoOnWaitMinutes;
     //-------------------DAPI 10.0--------------------
     UINT32 headphoneInsertions;
     UINT32 headphoneRemovals;
     UINT16 lastUpdateYear;
     UINT8  lastUpdateMonth;
     UINT8  lastUpdateDay;
     UINT8  lastUpdateHour;
     UINT8  lastUpdateMinute;
     UINT8  lastUpdateSecond;
     char lastUpdateTimezone[65];
     UINT16 consoleType;
     UINT8   maxUserNumber;
     //------------------First Boot Flag------------------------------------------
     UCHAR IsMachineConfiged;
     UCHAR firstBootFlag;   // 0x1£¬cleared; other first boot.
     UINT8 overspeedswitch;
     UINT8 overspeedvalue;
     UCHAR IsShowError;
     UCHAR controlZoneFlag;
     UCHAR secondControlZoneFlag;
     UCHAR irSensorFlag;
     UCHAR rscuFlag;
     UINT8 machinedataEnd;//this is only a flag,must be at the end!!!
}
MACHINE_INFO;

/*
* @brief Data will be stored at 0x38000 ~ 0x39000
*/	
typedef struct tag_WorkoutInfo
{
  	UINT16 defaultWorkoutMinutes;
  	UINT16 maxWorkoutMinutes;
  	UINT16 defaultWarmupMinutes;
  	UINT16 defaultCooldownMinutes;
  	UINT16 defaultPauseMinutes;
  	UINT16 defaultTHRTarget;
  	UINT8 defaultPTHRTarget;
  	UINT16 defaultWattsTarget;
        UINT8 myKeyMinSpeedX10;
	UINT8 myKeyMidSpeedX10;
	UINT8 myKeyMaxSpeedX10;
	UINT8 myKeyMinInclineX10;
	UINT8 myKeyMidInlcineX10;
	UINT8 myKeyMaxInclineX10;
	UINT8 myKeyMinResistance;
	UINT8 myKeyMidResistance;
	UINT8 myKeyMaxResistance;
	UINT16 savedWorkout1[32];
	UINT16 savedWorkout2[32];
	UINT16 savedWorkout3[32];
	UINT16 savedWorkout4[32];
	UINT16 timeGoalMinutes;
	UINT16 distanceGoal;
	UINT16 caloriesGoal;
//        RTC timestamp;
	UINT8 daysToAchieveGoal;
}WORKOUT_INFO;



/*
* @brief Data will be stored at 0x38000 ~ 0x39000
*/
typedef struct{
    UINT8 isUserDataDirty;
    UINT8 userConsoleId;   //to determine the current user.
    char xid[14];
    char passcode[4];
    UINT16 weightStandardX100;
    UINT8 age;   
    UINT16 year;
    UINT8 month;
    UINT8 date;
    UINT8 gender;          //0 as male; 1 as female
    UINT16 timeStampYear;
    UINT8 timeStampMonth;
    UINT8 timeStampDate;
    UINT8 timeStampHour;
    UINT8 timeStampMinute;
    UINT8 timeStampSeconds;
    UINT8 displayUnit;
    char name[65];
    UINT8 languageId;
    UINT8 isAcceptedTerms;
    UINT8 isShareWorkouts;
    UINT8 isLoggedOn; 
    UINT8 isSetup;
}USERS_INFO;


/*
* @brief Data will be stored at 0x39800
*/
typedef struct
{
    UINT16 customWorkoutTime;
    UINT8 customSpeedX10[MAX_CUSTOM_SEGMENTS];
    UINT8 customInclineX10[MAX_CUSTOM_SEGMENTS];
    UINT8 customresistance[MAX_CUSTOM_SEGMENTS];
}CUSTOM_CONFIG;


/*
* @brief Data will be stored at 0x0FE00000
*/
typedef struct{
	UINT8 isAccumulatedDataDirty;
	UINT32 accumulatedDistanceMilesX100;
 	UINT32 accumulatedTimeSeconds;
 	UINT32 accumulatedLubeMilesX100;
    UINT32 accumulatedLubeTime;
}ACCUMULATED_DATA;
#pragma pack( pop )

/*
* @brief Public APIs *********************************************************
*/
void SystemConfig_Initial(void);
UCHAR SystemConfig_Get_Char(SYSTEM_CONFIG_TYPE type);
UINT16 SystemConfig_Get_Int(SYSTEM_CONFIG_TYPE type);
UINT32 SystemConfig_Get_Long(SYSTEM_CONFIG_TYPE type);
UCHAR* SystemConfig_Get_String(SYSTEM_CONFIG_TYPE type);
UINT8  SystemConfigProcess(SYSTEM_VIRTUAL_KEY vk, void *p, UINT16 size);
void SystemConfigRestoreToFactory(void);
char SystemConfig_Get_MaxUserNumber(void);
UINT8 SystemConfig_Get_Version_Code(UINT8 *buffer);
UCHAR SystemConfig_Set_MachineInfo(MACHINE_INFO machineinfo);
UCHAR SystemConfig_Reset_User_Default(UCHAR index);
MACHINE_INFO SystemConfig_Get_MachineInfo(void);
UCHAR SystemConfig_Set_UserInfo(USERS_INFO userinfo , UCHAR index);
USERS_INFO SystemConfig_Get_UserInfo(UCHAR index);
SystemSettingType SystemConfig_Get_DefaultString(SystemSettingString  index);
UCHAR SystemConfig_Get_Dapi_Version(void);
UINT8 SystemConfig_Get_Console_Version(char *pVersion);
UINT8 SystemConfig_Set_UpdateFlag(UINT8 flag);
UINT32 SystemConfig_Get_FirstBoot_ADDRESS(void);
#endif
