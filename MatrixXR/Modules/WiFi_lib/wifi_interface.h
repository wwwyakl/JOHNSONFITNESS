/*
 * wifi_interface.h
 *
 *  Created on: May 2, 2013
 *      Author: Brenden.Capps
 */

#ifndef WIFI_INTERFACE_H_
#define WIFI_INTERFACE_H_

#include "jisdef.h"
#ifdef __WIFI_FUNC__
#include "wifi_api.h"

#if 0
typedef union
{
  struct
  {    
    UINT16 Key_Up:1;
    UINT16 Key_Dn:1;
    
    UINT16 Key_Enter:1;
    UINT16 Key_Start:1;
    UINT16 Key_Stop:1;
    UINT16 Key_Reset:1;
    UINT16 Key_WiFi_Reset:1;
    UINT16 Key_ENG:1;
    UINT16 Key_QuickKey:1;
  }EventBits; 
  UINT16 All_KeyEvents;
}WiFiKeyEvts;

typedef enum
{
     NO_WIFI_EVENT=0x0,
     DISPLAY_ENG_EVENT,
     ALL_DOTSON_ENG_EVENT,
     ALL_DOTSOFF_ENG_EVENT,
     SEG1_ENG_EVENT,
     ENG_EVENT_TOTAL
}WIFI_EVENT;
#endif

#define WIFI_TEST_DELAY_FOR_RESET	20
#define WIFI_TEST_DELAY_FOR_RESULT	4000
#define WIFI_TEST_DELAY_FOR_TIMEOUT	12000

#define FACTORY_TEST_SSID "Wifi-Test"
#define FACTORY_TEST_PW   ""

#define HTTP_STATUS_OK 200
#define SYNCWORD       0xAA55

#define TOTAL_RESPONSE_NUMBER 10
#define XIDMAXLENGTH     14


#define DEFAULTXID "00000000000000"


#define WIFI_PROTOCOL_VERSION 5//E------>5
#define WIFI_LINGO_MODULE_SETTINGS_VERSION 1
#define WIFI_LINGO_GENERAL_VERSION 1
#define WIFI_LINGO_REMOTE_VERSION 1
#define WIFI_LINGO_SETTINGS_VERSION 1
#define WIFI_LINGO_TEST_VERSION 1

#define WIFI_LINGO_WORKOUT_TRACKING_VERSION 1
#define WIFI_LINGO_ASSET_MANAGEMENT_VERSION 1
#define WIFI_LINGO_NETWORK_TIME_VERSION 1
#define WIFI_LINGO_USER_MANAGEMENT_VERSION 1

#ifndef WIFI_SUCCESS
#define WIFI_SUCCESS 1
#endif
#ifndef WIFI_FAIL
#define WIFI_FAIL    2
#endif

typedef enum
{
	WIFI_LINGO_MODULE_SETTINGS = 0x01,
	WIFI_LINGO_GENERAL = 0x02,
	WIFI_LINGO_REMOTE,//0x03
	WIFI_LINGO_SETTINGS,//0x04
	WIFI_LINGO_TEST,//0x05

	WIFI_LINGO_WORKOUT_TRACKING = 0x80,
	WIFI_LINGO_ASSET_MANAGEMENT = 0x81,
	WIFI_LINGO_NETWORK_TIME = 0x82,
	WIFI_LINGO_USER_MANAGEMENT = 0x83,

} WIFI_COMMAND_LINGO;

typedef enum
{
	WIFI_MODULE_SETTINGS_ACKNOWLEDGMENT,
	WIFI_GET_WIFI_SETTINGS,
	WIFI_CONNECT_TO_AP,
} WIFI_LINGO_MODULE_SETTINGS_COMMANDS;//0x01

typedef enum
{
	WIFI_GENERAL_ACKNOWLEDGMENT,//0
	WIFI_GET_LINGO_VERSION,
	WIFI_GET_MACHINE_INFO,
	WIFI_SET_SPEED_CALIBRATION,
	WIFI_GET_SPEED_CALIBRATION,
	WIFI_SET_PWM_CAL,//5
	WIFI_GET_PWM_CAL,
	WIFI_SET_ECB_CAL,
	WIFI_GET_ECB_CAL,
	WIFI_SET_WATTS_TABLE,
	WIFI_GET_WATTS_TABLE,//0x0A

	WIFI_SET_DATA_SERVER,
	WIFI_GET_DATA_SERVER,

	WIFI_SET_MACHINE_STATE,//-0x0D-
	WIFI_GET_MACHINE_STATE,//0x0E

	WIFI_SET_USER_DATA,//0x0F
	WIFI_GET_USER_DATA,//-0x10-

	WIFI_SET_WORKOUT_DATA_RATE,//-0x11-
	WIFI_GET_WORKOUT_DATA,

	WIFI_GET_CURRENT_USER,//-0x13-

	WIFI_STOP_WORKOUT,//-0x14-

	WIFI_CHECK_FOR_UPDATE,//0X15
    WIFI_GET_MACHINE_INFO_PASSPORT,//-0x16-
    WIFI_GET_WORKOUT_DATA_PASSPORT,//-0x17-

} WIFI_LINGO_GENERAL_COMMANDS;//0x02

typedef enum
{
	WIFI_REMOTE_ACKNOWLEDGMENT,//0x00
	WIFI_SET_CURRENT_USER,

	WIFI_START,//0x02
	WIFI_PAUSE,//-0x03-
	WIFI_RESTART,//0x04
//	WIFI_CHANGE_PROGRAM,
	WIFI_SET_SPEED,//-0x05-
	WIFI_SET_INCLINE,//-0x06-
	WIFI_SET_RESISTANCE,//-0x07-
	WIFI_SET_RAW_ECB,
	WIFI_SET_RAW_PWM,//0x09
	WIFI_SELECT_PROGRAM,
	WIFI_KEY_PRESS,
    WIFI_SETUP_WORKOUT_PASSPORT = 0x12,//-0x12-
    WIFI_SET_GOAL_PASSPORT = 0x13,//-0x13-
} WIFI_LINGO_REMOTE_COMMANDS;//0x03

typedef enum
{
	WIFI_SETTINGS_ACKNOWLEDGMENT,
	WIFI_SET_DEFAULTS,
	WIFI_GET_DEFAULTS,

	WIFI_SET_MACHINE_TYPE,
	WIFI_GET_MACHINE_TYPE,

	WIFI_SET_REGION,
	WIFI_GET_REGION,
} WIFI_LINGO_SETTINGS_COMMANDS;//0x04

typedef enum
{
	WIFI_TEST_ACKNOWLEDGMENT,
	WIFI_SET_INCLINE_ADC,
	WIFI_SET_RPM,
	WIFI_SET_ECB,
	WIFI_SET_PWM,
	WIFI_BEEP,
	WIFI_SIM_SAFETY_KEY,

} WIFI_LINGO_TEST_COMMANDS;//0x05

typedef enum
{
	WIFI_ACK_OK,
	WIFI_ACK_CHECKSUM_ERROR,
	WIFI_ACK_UNABLE_TO_CONNECT,
	WIFI_ACK_INVALID_COMMAND_DURING_WORKOUT,
	WIFI_ACK_ANOTHER_DEVICE_HAS_CONTROL,
	WIFI_ACK_MACHINE_STATE_CHANGE_NOT_CONFIRMED,
	WIFI_ACK_INVALID_USER_ID,
	WIFI_ACK_NOT_IN_REMOTE_CONTROL,
	WIFI_ACK_INVALID_SPEED,
	WIFI_ACK_INVALID_INCLINE,
	WIFI_ACK_INVALID_RESISTANCE,
	WIFI_ACK_START_INVALID_WORKOUT_IN_PROGRESS,
	WIFI_ACK_STOP_INVALID_NO_WORKOUT_IN_PROGRESS,
	WIFI_ACK_PAUSE_INVALID_NO_WORKOUT_IN_PROGRESS,
	WIFI_ACK_CHANGE_WORKOUT_INVALID_NO_WORKOUT_IN_PROGRESS,
	WIFI_ACK_DEFAULT_OUT_OF_RANGE,
	WIFI_ACK_INVALID_TIME_SET,
	WIFI_ACK_NOT_IN_TEST_MODE,
	WIFI_ACK_ADC_OUT_OF_RANGE,
	WIFI_ACK_RPM_OUT_OF_RANGE,
	WIFI_ACK_ECB_OUT_OF_RANGE,
	WIFI_ACK_PWM_OUT_OF_RANGE,
	WIFI_ACK_NOT_IN_SETTINGS_MODE,
	WIFI_ACK_MACHINE_STATE_OUT_OF_RANGE,
	WIFI_ACK_INVALID_LINGO,
	WIFI_ACK_UNKNOWN_COMMAND,
	WIFI_ACK_NO_PWM_RESISTANCE,
	WIFI_ACK_NO_ECB_RESISTANCE,
	WIFI_ACK_NO_INCLINE,
	WIFI_ACK_NO_WATTS_TABLE,
	WIFI_ACK_MACHINE_TYPE_OUT_OF_RANGE,
	WIFI_ACK_MODEL_TYPE_OUT_OF_RANGE,
	WIFI_ACK_START_INVALID_BELT_MOVING,
	WIFI_ACK_RESTART_INVALID_WORKOUT_NOT_IN_PROGRESS,
	WIFI_ACK_RESTART_INVALID_WORKOUT_NOT_PAUSED,
	WIFI_ACK_WORKOUT_NOT_IN_PROGRESS,
	WIFI_ACK_UNSUPPORTED_COMMAND,
	WIFI_ACK_SPEED_CALIBRATION_OUT_OF_RANGE,
	WIFI_ACK_INVALID_USER_DATA,
	WIFI_ACK_START_PARAMS_OUT_OF_RANGE,
	WIFI_ACK_CHANGE_PROGRAM_PARAMS_OUT_OF_RANGE,
	WIFI_ACK_INVALID_DATE_TIME,
	WIFI_ACK_INVALID_DATA_SIZE,
	WIFI_ACK_SET_MACHINE_STATE_REJECTED,

} WIFI_ACK_RESPONSE;

typedef enum
{
	WIFI_WORKOUT_TRACKING_ACKNOWLEDGMENT,//0
	WIFI_WORKOUT_TRACKING_START_WORKOUT,//1
	WIFI_WORKOUT_TRACKING_WORKOUT_DATA,//2
	WIFI_WORKOUT_TRACKING_WORKOUT_COMPLETE,//3
	WIFI_WORKOUT_TRACKING_SYNC_USER,//4
	WIFI_WORKOUT_TRACKING_DATA_EX = 0x06,
	WIFI_WORKOUT_TRACKING_COMPLETE_EX= 0x07,
	WIFI_WORKOUT_TRACKING_SPRINT8_COMPLETE_EXTRA = 0x08,
	WIFI_WORKOUT_TRACKING_FITNESS_COMPLETE_EXTRA = 0x09,
} WIFI_LINGO_WORKOUT_TRACKING_MESSAGES;//0x80

typedef enum
{
	WIFI_ASSET_MANAGEMENT_ACKNOWLEDGMENT,//0
	WIFI_ASSET_MANAGEMENT_MACHINE_INFO,//1
	WIFI_ASSET_MANAGEMENT_MACHINE_REGISTRATION,//2
	WIFI_ASSET_MANAGEMENT_GET_DAPI_LOCATION,
	WIFI_ASSET_MANAGEMENT_GET_LATEST_VERSION = 0x05,
	WIFI_ASSET_MANAGEMENT_MACHINE_INFO_EX = 0x07,
	WIFI_ASSET_MANAGEMENT_UPDATE_MACHINE_STATE = 0x08,
	WIFI_ASSET_MANAGEMENT_ERROR_CODE_OCCURRED = 0x09,
	WIFI_ASSET_MANAGEMENT_SERVICE_CODE_OCCURRED = 0x0a,
	WIFI_ASSET_MANAGEMENT_GET_CONSOLE_CONFIG_URL = 0x0B,
	WIFI_ASSET_MANAGEMENT_GET_MESSAGE_CENTER_URL = 0x0c,
} WIFI_LINGO_ASSET_MANAGEMENT_MESSAGES;//0x81

typedef enum
{
	WIFI_NETWORK_TIME_ACKNOWLEDGMENT,//0
	WIFI_NETWORK_TIME_GET_TIME,//1
} WIFI_LINGO_NETWORK_TIME_MESSAGES;//0x82

typedef enum
{
	WIFI_USER_MANAGEMENT_ACKNOWLEDGMENT,//0
	WIFI_SYNC_USER_INFO,//1
	WIFI_USER_LOGIN,//2
	WIFI_USER_REGISTRATION,//3
	WIFI_USER_CHECK_XID,//4
	WIFI_USER_CLEAR_USERS,//5
	WIFI_USER_RESET_PASSWORD,//6
	WIFI_USER_SET_KEY_STORE,//7
	WIFI_USER_GET_KEY_STORE,//8
	WIFI_USER_GET_CALORIE_INFO,//9 
	WIFI_USER_UNLINK_USER,//0x0a
	WIFI_USER_GET_SOCAIL_MEDIA_CONNECTED=0x0e,
	WIFI_USER_SEND_SOCAIL_MEDIA = 0x0F,
	WIFI_USER_CHECK_UNIQUE_EMAIL=0x10,
	WIFI_USER_HAS_PERSONAL_TRAINER = 0x11,
	WIFI_USER_RFID_LOGIN = 0x12,
	WIFI_USER_RFID_ASSOCIATE = 0x13,
	WIFI_USER_RFID_DISASSOCIATE =0x14,
	WIFI_USER_GET_NUMBER_LEADER_BOARDS = 0x15,
} WIFI_LINGO_USER_MANAGEMENT_MESSAGES;//0x83

#pragma pack( push, 1 )




/*
typedef struct {
 UINT8 level;
 UINT16 cal;
 } PWM_CAL_POINT;
*/




typedef struct
{
	UINT8 messageType;
	UINT8 error;
} WIFIDataAck;

typedef struct
{
	char ssid[256];
	char securityKey[256];
} WIFIDataAPConnection;

typedef struct
{
	char server[256];
} WIFIDataDataServer;

typedef struct
{
	UINT8 lingo;
	UINT8 version;
} WIFIDataLingoVersion;

typedef struct
{
	char machineIdentification[36];
	UINT8 macAddress[6];
	UINT16 DapiVersion;
}WIFIDAPILocation;
typedef struct{
	UINT8 Secure;
	char Server[256];
	char Url[256];
	UINT16 Port;
}WIFIDAPIResponse;
/* For Dapi Version 10 Start */

typedef struct{
    char  MachineUUid[36];
    UCHAR  MacAddress[6];
    UINT16 BrandID;
    UINT16 MachineType;
    UINT16 ModelType;
    UINT16 ConsoleType;
    UCHAR  Deployment;
    char  SoftwareType[256];
    char  CurrentVersion[256];
}WIFIDATAGetLatestVersion;

typedef struct{
    char  SoftwareType[256];
    char  CurrentVersion[256];
    char  SoftwareUrl[256];
    char  MD5[32];
}WIFIDATAResponseLatestVersion;


typedef struct
{
    char machineIdentification[36];
    UINT8 macAddress[6];
    UINT16 brandID;
    UINT16 machineType;
    UINT16 modelType;
    UINT16 driveType;
    UINT16 consoleType;
    UINT8   csafeManufacture;
    UINT8   csafeID;
    UINT8   csafeModelID;
    char      serialNumber[MAX_CONSOLE_SERIAL_NUMBER];
    char      frameSerialNumber[MAX_FRAME_SERIAL_NUMBER];
    UINT16  clubID;
    UINT16  minMPHX10;
    UINT16 maxMPHX10;
    UINT16 minKPHX10;
    UINT16 maxKPHX10;
    UINT16 minInclineX10;
    UINT16 maxInclineX10;
    UINT16 minResistanceLevel;
    UINT16 maxResistanceLevel;
    char     softwareVersion[20];
    UINT16 dapiApiVersion;
    UINT8  LCBVendor;
    UINT16 LCBVersion;
    char     osVersion[20];
    UINT8  maxUsers;
    char     IpAddress[6];
    char     timeZone[65];
    UINT8  autoLogin;
    UINT16 lastUpdateYear;
    UINT8  lastUpdateMonth;
    UINT8  lastUpdateDay;
    UINT8  lastUpdateHour;
    UINT8  lastUpdateMinute;
    UINT8  lastUpdateSecond;
    char     lastUpdateTimezone[65];
}WIFIMachineInfoEX;

typedef struct{
    char     machineIdentification[36];
    UINT16 brandID;
    UINT16 machineType;
    UINT16 modelType;
    UINT16 driveType;
    UINT16 consoleType;
    UINT8   csafeManufacture;
    UINT8   csafeID;
    UINT8   csafeModelID;
    char      serialNumber[36];
    char      frameSerialNumber[36];
    UINT16 clubID;
    UINT16 minMPHX10;
    UINT16 maxMPHX10;
    UINT16 minKPHX10;
    UINT16 maxKPHX10;
    UINT16 minInclineX10;
    UINT16 maxInclineX10;
    UINT16 minResistanceLevel;
    UINT16 maxResistanceLevel;
    UINT8  maxUsers;
    char     timeZone[65];
    UINT8  autoLogin;
    UINT32 accumulatedDistanceMilesX100;
    UINT32 accumulatedTime;//seconds
    UINT32 lubeBeltMilesX10;
    UINT32 headphoneInsertions;
    UINT32 headphoneRemovals;
}WIFIMachineInfoEXResponse;

typedef struct{
    char      machineIdentification[36];
    char      MacAddress[6];
    UINT32  accumulatedDistanceMilesX100;
    UINT32  accumulatedTime;
    UINT16  motorTemperature;
    UINT16  busCurrent;
    UINT16  busVoltage;
    UINT32  lubeBeltMilesX10;
    UINT32  headphoneInsertions;
    UINT32  headphoneRemovals;
}WIFIUpdateMachineStats;

typedef struct{
    char      machineIdentification[36];
    char      MacAddress[6];
    UINT16  errorCode;
    UINT16  year;
    UINT8    month;
    UINT8    day;
    UINT8    hour;
    UINT8    minute;
    UINT8    second;    
}WIFIErrorCodeOccurred;

typedef struct{    
    char      machineIdentification[36];
    char      MacAddress[6];
    UINT16 serviceCode;
    UINT16 year;
    UINT8   month;
    UINT8   day;
    UINT8   hour;
    UINT8   minute;
    UINT8   second;
    char      serviceNote[1024];
}WIFIServiceCodeOccurred;
typedef struct
{

    char macAddress[6];
    char MachineID[36];
    char userExternalId[36];
    UINT16 programType;
    char programUUID[36];
    UINT16 goalType;
    UINT32 goalValueX100;
    UINT32 goalRemainingX100;
    UINT8   stageInterval;
    UINT16 stageGoalValueX100;
    UINT16 stageGoalRemainingX100;
    UINT8   workoutState;
    UINT16  timeInState;
    UINT16  timeInStage;
    UINT16  lapIndex;
    UINT16  splitIndex;
    UINT16  MPHX10;
    UINT16  inclineX10;
    UINT16  averagePace;
    UINT16  totalCalories;
    UINT16  caloriesPerHour;
    UINT16  metsX10;
    UINT16  watts;
    UINT16  milesX100;
    UINT16  heartRate;
    UINT16  averageHeartRate;
    UINT16  RPM;
    UINT16  programLevel;
    UINT16  resistanceLevel;
    UINT16  targetHeartRate;
    UINT16  targetWattsX10;
    UINT32  floorsX10;
    UINT32  totalSteps;
    UINT32  elevationX100;
    UINT16  VO2X10;
    UINT16  workoutStartYear;
    UINT8    workoutStartMonth;
    UINT8    workoutStartDay;
    UINT8    workoutStartHour;
    UINT8    workoutStartMinute;
    UINT8    workoutStartSecond;
} WIFIWorkoutDataEx;
typedef struct{
    char macAddress[6];
    char MachineID[36];
    char userExternalId[36];
    UINT16 programType;
    char programUUID[36];
    char programDescription[256];
    UINT16  workoutTime;
    UINT16  warmupTime;
    UINT16  cooldownTime;
    UINT16  totalCalories;
    UINT16  milesX100;
    UINT16  averageHeartRate;
    UINT32  totalFloorsX10;
    UINT32  totalSteps;
    UINT32 totalElevationX100;
    UINT16  workoutStartYear;
    UINT8    workoutStartMonth;
    UINT8    workoutStartDay;
    UINT8    workoutStartHour;
    UINT8    workoutStartMinute;
    UINT8    workoutStartSecond;
}WIFIWorkoutDataCompleteEx;

typedef struct{
    char macAddress[6];
    char MachineID[36];
    char userExternalId[36];
    UINT16 programType;
    char programUUID[36];
    char programDescription[256];
    UINT16 sprint1SweatScoreX10;    
    UINT16 sprint2SweatScoreX10;
    UINT16 sprint3SweatScoreX10;    
    UINT16 sprint4SweatScoreX10;
    UINT16 sprint5SweatScoreX10;    
    UINT16 sprint6SweatScoreX10;
    UINT16 sprint7SweatScoreX10;    
    UINT16 sprint8SweatScoreX10;
    UINT16 totalSweatScoreX10;
    UINT16  workoutStartYear;
    UINT8    workoutStartMonth;
    UINT8    workoutStartDay;
    UINT8    workoutStartHour;
    UINT8    workoutStartMinute;
    UINT8    workoutStartSecond;
}WIFIWorkoutDataSprint8CompleteExtra;


typedef struct{
    char macAddress[6];
    char MachineID[36];
    char userExternalId[36];
    UINT16 programType;
    char programUUID[36];
    char programDescription[256];
    UINT16 fitnessTestScore;
    UINT16  workoutStartYear;
    UINT8    workoutStartMonth;
    UINT8    workoutStartDay;
    UINT8    workoutStartHour;
    UINT8    workoutStartMinute;
    UINT8    workoutStartSecond;
}WIFIWorkoutDataFitnessTestCompleteExtra;


typedef struct{
    char MachineID[36];
    char macAddress[6];
    UINT8 userID;
    char RFIDTag[16];
    UINT8  RFIDCarrier;
}WIFIRFidLoginReq;
typedef struct{
    UINT8 userID;
    char    externalUserID[36];
    UINT8 gender;
    UINT16 userWeightX10;
    UINT8   userAge;
    UINT16 userBirthYear;
    UINT16 userBirthMonth;
    UINT16 userBirthDay;
    UINT8   userLanguageId;
    UINT8   displayUnits;
    char      userName[65];
    UINT8   acceptedTerms;
    UINT8   shareWorkoutData;
}WIFIRFidLoginResp;

typedef struct{
    char MachineID[36];
    char macAddress[6];
    UINT8 userID;
    char    externalUserID[36];
    char RFIDTag[16];
    UINT8  RFIDCarrier;
}WIFIRFidAssociateReq;

typedef struct{
    char MachineID[36];
    char macAddress[6];
    char RFIDTag[16];
    UINT8  RFIDCarrier;
}WIFIRFidDisassociateReq;

/*For Dapi Version 10 End*/

typedef struct
{
	UINT8 brand;
	UINT8 machineType;
	UINT8 modelType;
	UINT8 driveType;
	UINT8 csafeManufacture;
	UINT8 csafeId;
	UINT8 csafeModelId;
        char      serialNumber[MAX_CONSOLE_SERIAL_NUMBER];
        char      frameSerialNumber[MAX_FRAME_SERIAL_NUMBER];
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
	UINT16 wifiFirmwareVersion;
	UINT16 wifiUARTVersion;
	UINT16 btFirmwareVersion;
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
	char timeZone[65];
	UINT32 lubeMiles;
	UINT8  AutoLogin;

} WIFIDataMachineInfo;

typedef struct
{
    UINT8 machineType;
     char machineIdentification[36];
 	UINT16 minMPHX10;
	UINT16 maxMPHX10;
 	UINT16 minRPM;
	UINT16 maxRPM;
	UINT16 minInclineX10;
	UINT16 maxInclineX10;
    UINT16 minResistance;
    UINT16 maxResistance;
}PassportMachineInfo;

typedef struct
{
    UINT8 userId;
    char xID[14];
//    char passcode[14];
    UINT8 gender;
	UINT16 weightX10;
	UINT8 age;
	UINT16 birthYear;
    UINT8 birthMonth;
    UINT8 birthDay;
    UINT8 birthLanguageId;
    UINT8 displayUnits;
    char  userName[65];
    UINT8 acceptedTerms;
    UINT8 shareWorkoutData;
    
}PassportUserInfo;

typedef struct
{
    UINT32 WorkoutId;
    UINT8  WorkoutState;//5
    UINT8  WorkoutMode;//8
        UINT16 WorkoutTime;
    UINT16 WarmUpTime;
    UINT16 CoolDownTime;//12
    UINT16 TimeInState;
	
    UINT16 SpeedX100;
    UINT16 ActualSpeedX100;
    UINT16 AverageSpeedX100;
	
    UINT16 InclineX10;
    UINT16 TotalCalories;
    UINT16 CaloriesPerHour;
    UINT16 MetsX100;
    UINT16 Watts;
    UINT16 MilesX100;
	
    UINT16 HeartRate;
    UINT16 AverageHeartRate;
    UINT16 PeakHeartRate;
    UINT16 RPM;   
    UINT16 ResistanceLevel;
	
    UINT32 GoalX100;
    UINT32 GoalRemainingX100;
    UINT16 GoalType; 
	   
    UINT16 Elevation;  
    UINT16 VALocation;  
    UINT16 Steps;
    UINT16 Floors;
    UINT16 SPM;
    UINT16 AverageSPM;
}PassportGetWorkoutData;

typedef struct
{
	UINT8 machineType;
	UINT8 modelType;
	UINT8 csafeManufacture;
	UINT8 csafeId;
	UINT8 csafeModelId;
    char      serialNumber[MAX_CONSOLE_SERIAL_NUMBER];
    char      frameSerialNumber[MAX_FRAME_SERIAL_NUMBER];
	char machineIdentification[36];
	UINT16 clubId;
	UINT16 minMPHX10;
	UINT16 maxMPHX10;
	UINT16 minKPHX10;
	UINT16 maxKPHX10;
	UINT16 minInclineX10;
	UINT16 maxInclineX10;
	UINT32 accumulatedDistanceMilesX100;
	UINT32 accumulatedSeconds;
	char timeZone[65];
	UINT32 lubeMiles;
	char   AutoLogin;

} WIFIAssetManagementResponse;

typedef struct
{
	UINT16 maxMPHx10;
	UINT16 minMPHx10;
	UINT16 maxKPHx10;
	UINT16 minKPHx10;
	UINT16 maxInclinex10;
	UINT16 minInclinex10;
	UINT16 maxADC;
	UINT16 minADC;
	UINT16 rpmMPHHigh;
	UINT16 rpmMPHLow;
	UINT16 rpmKPHHigh;
	UINT16 rpmKPHLow;
} WIFIDataSpeedCalibration;

typedef struct
{
	UINT8 machineType;
	UINT8 modelType;
} WIFIModelSelection;

#if 0
typedef struct
{
	PWM_CAL_POINT calPoint[5];
} WIFIDataPWMCal;
#endif
typedef struct
{
	UINT16 ecbLevel[30];
} WIFIDataECBCal;

typedef struct
{
	UINT8 level;
	UINT16 wattsX10[26];
} WIFIDataWattsTable;

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
	char name[65];
} WIFIDataUserData;

typedef struct
{
	UINT16 programType;
	UINT16 workoutTime;
	UINT16 warmupTime;
	UINT16 cooldownTime;
	UINT8 units;
	UINT16 startSpeedX10;
	UINT16 startInclineX10;
	UINT8 startLevel;
} WIFIDataProgramStart;

typedef struct
{
	UINT16 programType;
	UINT16 workoutTime;
	UINT16 warmupTime;
	UINT16 cooldownTime;
	UINT8 startLevel;
} WIFIDataProgramChange;

typedef struct
{
	UINT8 userId[36];
	UINT16 programType;
	UINT16 workoutTime;
	UINT16 warmupTime;
	UINT16 cooldownTime;
	UINT16 startMPHX10;
	UINT16 startInclineX10;
	UINT8 startLevel;
} WIFIDataExternalProgramStart;

typedef struct
{
	UINT8 userId[36];
} WIFIDataExternalProgramStartResponse;

typedef struct
{
	UINT16 maxWorkoutTime;
	UINT16 defaultWorkoutTime;
	UINT16 defaultAge;
	UINT16 defaultWeightX10;
	UINT16 defaultWarmupTime;
	UINT16 defaultCooldownTime;
	UINT16 pauseTime;
	UINT8 gender;
	UINT8 volume;
	UINT8 brightness;
	UINT16 targetHR;
	UINT16 constantWatts;
	UINT8 energySaver;
	UINT8 demoMode;
	UINT8 languageId;
	UINT8 units;
	UINT16 maxMPHX10;
	UINT16 minMPHX10;
	UINT16 maxKPHX10;
	UINT16 minKPHX10;
	UINT16 maxInclineX10;
	UINT16 minInclineX10;
	UINT8 timeFormat;
} WIFIDataDefaults;

typedef struct
{
	UINT16 year;
	UINT8 month;
	UINT8 day;
	UINT8 hour;
	UINT8 minute;
	UINT8 second;
	UINT8 units;
	UINT8 timeFormat;
	//char timeZone[65];
} WIFIDataRegion;

typedef struct
{
	UINT8 machineType;
	UINT8 modelType;
	UINT8 CSAFEManufacture;
	UINT8 CSAFEId;
	UINT8 CSAFEModelId;
	char ConsoleSerialNumber[15];
	char FrameSerialNumber[15];
	char machineIdentification[36];
} WIFIMachineTypeData;

typedef struct
{

	unsigned char macAddress[6];
	char MachineID[36];
	char userExternalId[36];
	UINT16 programType;
	UINT8 workoutState;
	UINT8 stage;
	UINT16 stageTimeSeconds;
	UINT16 workoutTimeSeconds;
	UINT16 warmupTimeSeconds;
	UINT16 cooldownTimeSeconds;
	UINT16 timeInStateSeconds;
	UINT16 timeInStageSeconds;
	UINT16 mphx10;
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
	UINT16 goal;
	UINT16 year;
	UINT8 month;
	UINT8 day;
	UINT8 hour;
	UINT8 minute;
	UINT8 second;
} WIFIWorkoutData;

typedef struct
{

	unsigned char macAddress[6];
	char userExternalId[36];
	UINT32 workoutId;
	UINT16 programType;
	UINT8 workoutState;
	UINT8 stage;
	UINT16 stageTimeSeconds;
	UINT16 workoutTimeSeconds;
	UINT16 warmupTimeSeconds;
	UINT16 cooldownTimeSeconds;
	UINT16 timeInStateSeconds;
	UINT16 timeInStageSeconds;
	UINT16 mphx10;
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
	UINT16 goal;
	UINT16 year;
	UINT8 month;
	UINT8 day;
	UINT8 hour;
	UINT8 minute;
	UINT8 second;
	short int nextIncline;
	short int nextSpeed;
	short int nextResistance;
} WIFIWorkoutDataRemote;

typedef struct
{

	unsigned char macAddress[6];
	char MachineID[36];
	char userExternalId[36];
	//UINT32 workoutId;
	UINT16 programType;
	UINT16 workoutTimeSeconds;
	UINT16 warmupTimeSeconds;
	UINT16 cooldownTimeSeconds;
	UINT16 totalCalories;
	UINT16 milesx100;
	UINT16 averageHeartRate;
	UINT16 year;
	UINT8 month;
	UINT8 day;
	UINT8 hour;
	UINT8 minute;
	UINT8 second;
} WIFIWorkoutDataComplete;

typedef struct
{
	UINT16 speedX10;
	UINT8 metric;
} WIFISetSpeed;

typedef struct
{
	char timeZone[65];
} WIFITimeRequestRegion;

typedef struct
{
	UINT16 year;
	UINT8 month;
	UINT8 day;
	UINT8 hour;
	UINT8 minute;
	UINT8 second;
	UINT16 ms;
} WIFITimeResponseRegion;

typedef struct
{
	UINT8 userConsoleId;
	UINT8 UserSetup;
	char xid[XIDMAXLENGTH];
	char passcode[4];
	UINT8 macAddress[6];
	char machineIdentification[36];
	UINT16 lastChangeYear;
	UINT8 lastChangeMonth;
	UINT8 lastChangeDay;
	UINT8 lastChangeHour;
	UINT8 lastChangeMinute;
	UINT8 lastChangeSecond;

	UINT8 gender;
	UINT16 weightX10;
	UINT8 age;
	UINT16 year;
	UINT8 month;
	UINT8 day;
	UINT8 language;
	UINT8 DisplayUnits;
	char name[65];
	UINT8 AcceptedTerms;
	UINT8 WorkoutDataShare;

} WIFIExternalUserRequestData;

typedef struct
{
	UINT8 userConsoleId;
	UINT8 UserSetup;
	char xid[XIDMAXLENGTH];
	char passcode[4];
	UINT8 gender;
	UINT16 weightX10;
	UINT8 age;
	UINT16 year;
	UINT8 month;
	UINT8 day;
	UINT8 language;
	UINT8 DisplayUnits;
	char name[65];
	UINT8 AcceptedTerms;
	UINT8 WorkoutDataShare;
} WIFIExternalUserResponseData;

typedef struct
{
	UINT8 userId;
	char xid[XIDMAXLENGTH];
	char passcode[4];
	char macAddress[6];
	char MachineID[36];
} WIFIUserLoginRequest;

typedef struct
{
	UINT8 userId;
	char externalUserId[36];
	UINT8 gender;
	UINT16 weightX10;
	UINT8 age;
	UINT16 year;
	UINT8 month;
	UINT8 day;
	UINT8 language;
	UINT8 DisplayUnits;
	char userName[65];

} WIFIUserLoginResponse;

typedef struct
{
	UINT8 userId;
	char macAddress[6];
	char machineIdentification[36];
	UINT8 gender;
	UINT16 weightX10;
	UINT8 age;
	UINT16 year;
	UINT8 month;
	UINT8 day;
	UINT8 language;
	UINT8 DisplayUints;
	char userName[65];
	char email[65];
	char xid[XIDMAXLENGTH];
	char passcode[4];
	UINT8 AcceptedTerms;
	UINT8 WorkoutDataShare;
} WIFIUserRegisterRequest;

typedef struct
{
	UINT8 userId;
	char externalUserId[36];

} WIFIUserRegisterResponse;

typedef struct
{
	char xid[XIDMAXLENGTH];
} WIFIUserCheckXid;

typedef struct
{
	UINT8 units;
	UINT16 speedX10;
} WIFIChangeSpeed;

typedef struct
{
	UINT8 priority;
	UINT8 state;
} WIFIMachineStateRequest;

typedef struct
{
	UINT8 macAddress[6];
} WIFIMachineRegisterRequest;


typedef struct
{
	char machineIdentification[36];
} WIFIMachineRegisterResponse;

typedef struct
{
	UINT8 macAddress[6];
	char machineIdentification[36];
} WIFIClearUsers;
typedef struct
{
	UINT8 UserIndex;
	char machineIdentification[36];
}WIFIUnlinkUser;


typedef struct
{
	UINT8 state;
	UINT16 command;
	UINT16 error;
	UINT8 port;
} WifiResponseState;


typedef struct{
	UCHAR Brand;
	UCHAR MachineType;
	UCHAR ModeType;
	UCHAR DriveType;
	UCHAR CSafeManu;
	UCHAR CSafeID;
	UCHAR CSafeModel;
    char      serialNumber[MAX_CONSOLE_SERIAL_NUMBER];
    char      frameSerialNumber[MAX_FRAME_SERIAL_NUMBER];
}MachineInfor;

typedef union
{
	UINT8 UByte;
	char Byte;
	INT16 Word;
	UINT16 UWord;
	INT32 DoubleWord;
	UINT32 UDoubleWord;

	WIFIDataAck Ack;
	WIFIMachineStateRequest MachineStateRequest;
	WIFIDataAPConnection APConnection;
	WIFIDataDataServer DataServer;
	WIFIDataMachineInfo MachineInfo;
	WIFIDataSpeedCalibration SpeedCalibration;
#if 0
	WIFIDataPWMCal PWMCal;
	WIFIDataECBCal ECBCal;
#endif
	WIFIDataWattsTable WattsTable;
	WIFIDataUserData UserData;
	WIFIDataLingoVersion LingoVersion;
	WIFIDataProgramStart ProgramStart;
	WIFIDataProgramChange ProgramChange;
	WIFIDataDefaults Defaults;
	WIFIDataRegion Region;
	WIFIModelSelection ModelSelection;
	WIFIWorkoutData WorkoutData;//WorkoutData
	WIFIMachineTypeData MachineType;
	WIFITimeRequestRegion TimeRequest;
	WIFITimeResponseRegion TimeResponse;
	WIFIExternalUserRequestData UserDataRequest;
	WIFIExternalUserResponseData UserDataResponse;
	WIFISetSpeed SetSpeed;
	WIFIDataExternalProgramStart WorkoutTrackingProgramStart;
	WIFIDataExternalProgramStartResponse WorkoutTrackingProgramStartResponse;
	WIFIWorkoutDataComplete WorkoutTrackingDataComplete;
	WIFIWorkoutDataRemote WorkoutDataRemote;
	WIFIAssetManagementResponse AssetManagementResponse;
	WIFIDAPIResponse AMWIFIDAPIResponse;

	WIFIUserLoginRequest UserLoginRequest;
	WIFIUserLoginResponse UserLoginResponse;

	WIFIUserRegisterRequest UserRegisterRequest;
	WIFIUserRegisterResponse UserRegisterResponse;

	WIFIUserCheckXid UserCheckXid;

	WIFIChangeSpeed ChangeSpeed;

	WIFIMachineRegisterRequest RegisterRequest;
	WIFIMachineRegisterResponse RegisterResponse;
	WIFIDAPILocation   DAPILocation;

	WIFIClearUsers ClearUsers;
	WIFIUnlinkUser UnlinkUserData;
       WIFIDATAGetLatestVersion getlatestVersion;
       WIFIDATAResponseLatestVersion responseLatestVersion;

       WIFIMachineInfoEX   MachineInfoEx;
       WIFIMachineInfoEXResponse MachineInfoExResponse;
       WIFIUpdateMachineStats  MachineStats;
       WIFIErrorCodeOccurred    ErrorCodeOccurred;
       WIFIServiceCodeOccurred ServiceCodeOccurred;
       WIFIWorkoutDataEx    WorkoutDataEx;
       WIFIWorkoutDataCompleteEx WorkoutTrackingDataCompleteEx;
       WIFIWorkoutDataSprint8CompleteExtra WorkoutSprint8CompleteExtra;
       WIFIWorkoutDataFitnessTestCompleteExtra WorkoutFitnessTestCompleteExtra;
       WIFIRFidLoginReq  rfidLoginReq;
       WIFIRFidLoginResp rfidLoginResp;
       WIFIRFidAssociateReq rfidAssociateReq;
       WIFIRFidDisassociateReq rfidDisassociateReq;
       PassportMachineInfo passportData;
       PassportUserInfo pssportUserData;
       PassportGetWorkoutData passportWorkoutData;
       
} WifiCommandData;

typedef struct
{
	UINT16 syncWord;
	UINT16 transactionId;
	UINT8 lingoId;
	UINT8 messageId;
	UINT16 dataSize;
	UINT16 checksum;
} WifiHeader;
typedef struct _WIFI_COMMAND
{
	WifiHeader header;
	WifiCommandData data;

} WifiCommand;


enum WIFI_COMMAND_STATES
{
	WCS_IDLE,
	WCS_INIT_WIFI,
	WCS_GET_WIFI_INFO,
	WCS_GET_WIFI_SETTINGS,
	WCS_CONNECT_TO_AP,
	WCS_GET_LINGO_INFO,//5
	WCS_GET_CONNECTION_INFO,
	WCS_SET_DATA_SERVER,
	WCS_GET_DATA_SERVER,
	WCS_SET_AM_SERVER,
	WCS_GET_AM_SERVER,//10
	WCS_GET_MACHINE_INFO,
	WCS_GET_WORKOUT_DATA,
	WCS_ACK,
	WCS_GET_STATUS,
	WCS_GET_SPEED_CALIBRATION,//15
	WCS_GET_PWM_CALIBRATION,
	WCS_GET_ECB_CALIBRATION,
	WCS_GET_WATTS_TABLE,
	WCS_GET_MACHINE_STATE,
	WCS_GET_USER_DATA,//20
	WCS_GET_CURRENT_USER,
	WCS_GET_DEFAULTS,
	WCS_GET_MACHINE_TYPE,
	WCS_GET_REGION,
	WCS_CHECK_FOR_UPDATE,//25
	WCS_POST_WORKOUT_DATA,

	WCS_UPDATE_USER,
	WCS_COMPLETE_WORKOUT,
	WCS_UPDATE_TIME,
	WCS_GET_REMOTE_DATA,//30

	WCS_UPDATE_USER_INFO,
	WCS_UPDATE_MACHINE_INFO,
	WCS_SELECT_PROGRAM,
	WCS_KEY_PRESS,
	WCS_REGISTER_USER,//35
	WCS_CHECK_XID,
	WCS_LOGIN_USER,
	WCS_ENABLE_AP,
	WCS_DISABLE_AP,
	WCS_CONNECT_TO_ROUTER,//40
	WCS_DISCONNECT_TO_ROUTER,
	WCS_SYNC_CONSOLE_INFO,
	WCS_REGISTER_MACHINE,
	WCS_GET_DAPI_LOCATION,
	WCS_SET_MARIO_BOX,//45
	WCS_UNLINK_USER,

	WCS_GET_SIGNAL_STRENGTH,
	WCS_RESTORE_FACTORY,
	WCS_SET_AP_FOR_ENGTEST,
    
	/* For Dapi Version 10 start  */
    
	WCS_GET_LATEST_VERSION, //for RSCU //50
	WCS_SET_UPDATE_URL,//for RSCU 
	WCS_WORKOUT_SNAPSHOT_EX,
	WCS_WORKOUT_COMPLETE_EX,
	WCS_RFID_LOGIN,
	WCS_RFID_ASSOCIATE,//55
	WCS_RFID_DISASSOCIATE,
	WCS_UPDATE_MACHINE_STATS,
	WCS_OCCURRED_ERROR_CODE,
	WCS_OCCURRED_SERVICE_CODE,
	WCS_SET_TIMEZONE,//60
	WCS_RESEND_0502,
    WCS_LEAVE_PROGRAMMING_MODE,
    WCS_GET_MACHINE_INFO_PASSPORT,
    WCS_GET_WORKOUT_DATA_PASSPORT,
    WCS_SET_START_WORKOUT,
};

typedef enum{
    WIFI_STATUS_START,
    WIFI_STATUS_IDLE= WIFI_STATUS_START,
    WIFI_STATUS_OFFLINE,
    WIFI_STATUS_LOGIN,
    WIFI_STATUS_SYNC_USER,
    WIFI_STATUS_SYNC_SUCCESS,    
    WIFI_STATUS_SYNC_FAIL,
    WIFI_STATUS_LOGIN_SUCCESS,
    WIFI_STATUS_LOGIN_FAIL,
    WIFI_STATUS_DELETE_SUCCESS,
    WIFI_STATUS_DELETE_FAIL,
    WIFI_STATUS_WAITTING,
    WIFI_STATUS_TIMEOUT,
    WIFI_STATUS_TIMEZONE,
    WIFI_STATUS_ACTIVATED,
    //WIFI_STATUS_UPDATE_VALID,
    WIFI_STATUS_AP_MODE,
    WIFI_STATUS_RFID_ASSOCIATE_SUCCESS,
    WIFI_STATUS_RFID_ASSOCIATE_FAIL,    
    WIFI_STATUS_RFID_DISASSOCIATE_SUCCESS,
    WIFI_STATUS_RFID_DISASSOCIATE_FAIL,
//    WIFI_STATUS_RSCU_CHECKING,
    WIFI_STATUS_DOWNLOADING_SOFTWARE,
//    WIFI_STATUS_DOWNLOAD_COMPLETED,
    WIFI_STATUS_NO_MAC,
    WIFI_STATUS_NO_IP,
    WIFI_STATUS_TEST_SUCCESS,
    WIFI_STATUS_TEST_FAIL,
    WIFI_STATUS_AVALIABLE_UPDATE,
  //  WIFI_STATUS_DOWNLOADING,
    //WIFI_STATU_
    WIFI_STATUS_END
}WIFI_STATUS_TYPE;
typedef enum
{
    XR_MODEL_TYPE_START = 0,
    XR_TF30 = XR_MODEL_TYPE_START,
    XR_TF50,
    XR_T50,
    XR_R30,
    XR_U30,
    XR_SE30,
    XR_R50I,
    XR_U50I,
    XR_SE50I,
    XR_A30,
    XR_A50I,
    XR_T70_C2,
    XR_T70_C3,
    XR_T70_C1,
    XR_C50,
    XR_T30,
    XR_T75_C2,
    XR_T75_C3,
    XR_T75_C1,
    XR_AVAIABLE_MACHINES_TOTAL
}XR_AVAIABLE_MODELS;


#pragma pack(pop)
UINT8 User_logined();//lxl20160816

UINT8 WIFIIsAPMode(void);
UINT8 wifi_Get_Timezone(void);

void ConnectAPForFactoryTest( void );

void Wifi_Clear_Status(void);
VR_KEY  Wifi_Set_VirtualKey(VR_KEY vk);
void RegisterMachine(void);
void ProcessTCPData( int port, char* data, int data_size );
void RunWiFiInterface( UINT16 timer );
void ProcessHTTPPost( char* message, int length, UINT16 timer );
void WifiConnectionStatusChange( bool wifiConnected );
void CheckXidUnique( unsigned char* xid );
UCHAR Wifi_Get_EnggineTestResult(void);
UINT8 WIFIGetDeleteResult(void);
void wifi_ResetInit(void);
UINT8 WIFIIsOnline(void);

void WIFIClearStateBuffer(void);
void wifi_Poweron_Initial_Data(void);
void WIFISendSignalStrengthCommand(void);
void WIFISendLeave_Programming_Mode(void);
WIFI_STATUS_TYPE wifiGetStatus(void);
void wifiTestRSCU(UINT8 softtype,char* md5);
void ClearFactoryTestStatus( void );
void WIFI_get_status(void);

#endif
#endif /* WIFI_INTERFACE_H_ */
