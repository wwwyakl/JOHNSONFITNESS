/****************************************************************************
*
*
*
*
*
*
****************************************************************************/

#ifndef __REMOTE_DATATYPE_H__
    #define __REMOTE_DATATYPE_H__

#include "remote_config.h"

#ifdef __REMOTE_LIB__
#include <stdbool.h>
#include "JISTypes.h"
#include "Global_Config.h"
#include "SystemConfig.h"
#include "Programs.h"


typedef enum{
    REMOTE_EVENT_START,
    REMOTE_EVENT_NO_DEVICE =  REMOTE_EVENT_START,
    REMOTE_EVENT_IDLE,
    REMOTE_EVENT_END
}RemoteEvent;

typedef enum{
    SUB_PROCESS_STATE_START,
    SUB_PROCESS_STATE_NOT_SUPPORTED=SUB_PROCESS_STATE_START,
    SUB_PROCESS_STATE_UNPROCESSED,
    SUB_PROCESS_STATE_NEXT_SUBSTATE,
    SUB_PROCESS_STATE_JUMP_STATE,
    SUB_PROCESS_STATE_NEXT_STATE,
    SUB_PROCESS_STATE_DIRECT_CONNECT,
    SUB_PROCESS_STATE_END
}SubProcessState;

typedef enum{
    REMOTE_RESPONSE_START,
    REMOTE_RESPONSE_WAIT,
    REMOTE_RESPONSE_SUCCESFUL,
    REMOTE_RESPONSE_FAIL,
    REMOTE_RESPONSE_TIMEOUT,
    
    REMOTE_RESPONSE_END
}RemoteResponse;

typedef enum {
	HTTP_GET,
	HTTP_POST
} HttpMethod;

typedef enum {
	HTTP_APPLICATION_URLENCODED,
	HTTP_APPLICATION_JSON,
	HTTP_APPLICATION_XML,
	HTTP_APPLICATION_OCTET_STREAM,
	HTTP_APPLICATION_TEXT,
} HttpDataType;

typedef struct
{
	UINT16 syncWord;
	UINT16 transactionId;
	UINT8 lingoId;
	UINT8 messageId;
	UINT16 dataSize;
	UINT16 checksum;
} RemoteHeader;


typedef struct
{
	UINT8 syncWord;
	UINT8 checksum;        
	UINT16 dataSize;
} UpdateHeader;      //zsf


typedef enum
{
    DAPI_LINGO_MODULE_SETTINGS = 0x00,
    DAPI_LINGO_GENERAL,//0x02
//    DAPI_LINGO_INTERFACE,
    DAPI_LINGO_REMOTE=0x02,//0x03
    DAPI_LINGO_SETTINGS=0X04,//0x04
    DAPI_LINGO_TEST,//0x05
    DAPI_LINGO_WORKOUT_TRACKING = 0x80,
    DAPI_LINGO_ASSET_MANAGEMENT,//0x81
    DAPI_LINGO_NETWORK_TIME,//0x82
    DAPI_LINGO_USER_MANAGEMENT,//0x83
} DapiCommanLingo;
typedef enum{
    DEPLOYMENT_TYPE_START,
    DEPLOYMENT_DEVELOPMENT = DEPLOYMENT_TYPE_START,
    DEPLOYMENT_INTERNAL_TESTING,
    DEPLOYMENT_QA,
    DEPLOYMENT_PRODUCTION,
    DEPLOYMENT_TYPE_END
}DEPLOYMENT_TYPE;

typedef enum{
    SOFTWARE_TYPE_START,
    SOFTWARE_TYPE_GUI = SOFTWARE_TYPE_START,
    SOFTWARE_TYPE_BOOTLOADER,
    SOFTWARE_TYPE_NON_MCU,
    SOFTWARE_TYPE_NON_SUB_MCU,
    SOFTWARE_TYPE_LCB1,
    SOFTWARE_TYPE_LCB2,
    SOFTWARE_TYPE_LCM,
    SOFTWARE_TYPE_WIFI,
    SOFTWARE_TYPE_BLUETOOTH,
    SOFTWARE_TYPE_RFID,
    SOFTWARE_TYPE_NFC,
    SOFTWARE_TYPE_END
}SOFTWARE_TYPE;





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
}GetLatestVersionData;

typedef struct{
    char  SoftwareType[256];
    char  CurrentVersion[256];
    char  SoftwareUrl[256];
    char  MD5[32];
}ResponseLatestVersionData;

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
    UINT8     softwareVersion[20];
    UINT16 dapiApiVersion;
    UINT8  LCBVendor;
    UINT16 LCBVersion;
    UINT8     osVersion[20];
    UINT8  maxUsers;
    UINT8     IpAddress[6];
    UINT8     timeZone[65];
    UINT8  autoLogin;
    UINT16 lastUpdateYear;
    UINT8  lastUpdateMonth;
    UINT8  lastUpdateDay;
    UINT8  lastUpdateHour;
    UINT8  lastUpdateMinute;
    UINT8  lastUpdateSecond;
    char     lastUpdateTimezone[65];
}MachineInfoEX;

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
}MachineInfoEXResponse;

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
}UpdateMachineStatus;

typedef struct
{
	char machineIdentification[36];
	UINT8 macAddress[6];
	UINT16 DapiVersion;
}DAPILocation;
typedef struct{
	UINT8 Secure;
	char Server[256];
	char Url[256];
	UINT16 Port;
}DAPILocalResponse;
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

} MachineInfoData;

typedef struct
{
    UINT8 machineType;
    UINT8 modelType;
    UINT8 csafeManufacture;
    UINT8 csafeId;
    UINT8 csafeModelId;
    char  serialNumber[MAX_CONSOLE_SERIAL_NUMBER];
    char  frameSerialNumber[MAX_FRAME_SERIAL_NUMBER];
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
} MachineInfoDataResponse;


typedef struct
{
	UINT8 macAddress[6];
} MachineRegisterData;


typedef struct
{
	char machineIdentification[36];
} MachineRegisterResponse;


typedef struct
{
	char timeZone[65];
} TimeRequestRegion;

typedef struct
{
	UINT16 year;
	UINT8 month;
	UINT8 day;
	UINT8 hour;
	UINT8 minute;
	UINT8 second;
	UINT16 ms;
} TimeResponseRegion;

typedef enum
{
    SUB_SYSTEM_TYPE_START,
    SUB_SYSTEM_TYPE_GENERAL = SUB_SYSTEM_TYPE_START,
    SUB_SYSTEM_TYPE_CONTROL,
    SUB_SYSTEM_TYPE_SYSTEMSETTINGS,
    SUB_SYSTEM_TYPE_WORKOUTTRACKING,
    SUB_SYSTEM_TYPE_ASSET,
    SUB_SYSTEM_TYPE_USER,
    SUB_SYSTEM_TYPE_END
}SubSystemType;

typedef enum
{
	WORKOUT_IDLE,
	WORKOUT_RUNNING,
	WORKOUT_PAUSED,
	WORKOUT_STOP
} RemoteWorkoutState;


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
} WorkoutData;
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
} WorkoutDataEx;
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
}WorkoutDataCompleteEx;

typedef struct
{

	unsigned char macAddress[6];
	char MachineID[36];
	char userExternalId[36];
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
} WorkoutDataComplete;


typedef union{
    WorkoutData WorkdataTrack;
    WorkoutDataEx WorkdataTrackEx;
}WorkDataTrackUnion;

typedef union{
    WorkoutDataComplete wkDataComplete;
    WorkoutDataCompleteEx wkDataCompleteEx;
}WorkDataCompleteUnion;

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
}WorkoutDataSprint8CompleteExtra;


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
}WorkoutDataFitnessTestCompleteExtra;

typedef enum{
    RFIDCARRIERSTART,
    LEGIC_RF_STANARD = RFIDCARRIERSTART,
    ISO_15693,
    ISO_14443_A,
    ISO_14443_B,
    INSIDE_SECURE,
    INSIDE_SECURE_1,
    RFIDCARRIEREND,
    EM4001,
    EM4100,
}RFIDCARRIER;
typedef enum{
    NO_HEART_RATE,
    SAFETY_KEY_ON,
    SAFETY_KEY_OFF,
}POPUPTYPE;
typedef struct{
    UINT8 userIndex;
    UINT8 rfidTag[16];
    RFIDCARRIER rfidCarrier;
}RFIDData;

typedef struct
{
	UINT8 userId;
	char xid[14];
	char passcode[4];
} UserLoginRequestData;


typedef struct{
    char MachineID[36];
    char macAddress[6];
    UINT8 userID;
    char RFIDTag[16];
    UINT8  RFIDCarrier;
}UserRFidLoginReq;

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
}UserRFidLoginResp;

typedef struct{
    char MachineID[36];
    char macAddress[6];
    UINT8 userID;
    char    externalUserID[36];
    char RFIDTag[16];
    UINT8  RFIDCarrier;
}UserRFidAssociateReq;

typedef struct{
    char MachineID[36];
    char macAddress[6];
    char RFIDTag[16];
    UINT8  RFIDCarrier;
}UserRFidDisassociateReq;


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

} UserSyncRequestData;

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
} UserSyncResponseData;

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

} UserLoginResponse;

typedef struct
{
	UINT8 UserIndex;
	char machineIdentification[36];
}UnlinkUser;


typedef struct
{
	UINT8 macAddress[6];
	char machineIdentification[36];
} ClearUsers;

typedef struct
{
	UINT8 userId;
	char xid[XIDMAXLENGTH];
	char passcode[4];
	char macAddress[6];
	char MachineID[36];
} UserLoginRequest;




typedef struct{
	UINT8 Secure;
	char Server[256];
	char Url[256];
	UINT16 Port;
}DAPI_ADDRESS_INFO;


typedef union{
            RFIDData rfiddata;
            UserLoginRequestData _loginRequest;
}UserLoginInfor;

typedef union{
               WorkoutDataSprint8CompleteExtra completeSprint8Data;
               WorkoutDataFitnessTestCompleteExtra completeFitnessTestData;
}ProgramExtraInfor;


typedef struct{
    char _UserExternalID[1+36];
    WorkDataTrackUnion workouData;
    WorkDataCompleteUnion wokoutCompleteData;
    ProgramExtraInfor  programExtraInfor;
}WorkoutInfor;

typedef union{
    UserLoginInfor loginInfor;
    WorkoutInfor   workoutInfor;
    ResponseLatestVersionData _softwareUpdate;
}Datainfor;

typedef struct{
    Datainfor     dataInfor;
    DAPI_ADDRESS_INFO DapiInfo;
}DapiGlobalVariable;



typedef struct{
    HttpMethod httpMethod;
    HttpDataType httpDataType;
    char              url[256];
    UINT8           secure;
}PushDataUrl;


typedef struct
{
	UINT8 messageType;
	UINT8 error;
} RemoteDataAck;

typedef bool (*ADDSTATESFUNC)( UINT16 state, UINT8 argsno, UINT16 parameter, UINT8 *parameterPoint );


typedef struct{
    char deviceMacAddress[6];
    char deviceIpAddress[6];
    void(*nextState)(void);
    bool (*addResponseState)( UINT16 state, UINT8 argsno, UINT16 parameter, UINT8 *parameterPoint );
    bool (*subStateSendMessage)(UINT8* buf, UINT16 length);    
    bool (*stateSendMessage)(UINT8* buf, UINT16 length);
    bool (*directSendMessage)(UINT8* buf, UINT16 length);
    RemoteResponse (*receiveMessage)(UINT8* buf, UINT16 length, ADDSTATESFUNC addstatefunc);    
    void (*ChangeWorkoutCallback)(RemoteWorkoutState state, ADDSTATESFUNC addstatefunc, UINT8 *deviceMac );
	void (*reSendFuncation)(void);
    UINT16 currentState;
    UINT16 cmdParameter;
    UINT16 delayTimer;
    UINT16 deviceBufSize;
    UINT16 initialStepTimeOut;
    UINT8  *parameterPoint;
    UINT8   argscounter;
    UINT8  currentSubState;
    UINT8  deviceProperty;    
    UINT8  initialStep;
	UINT8  reSend;
    UINT16 transactionId;
}RemoteDevicesInformation;


typedef struct{
    UINT8 command;
    UINT8* data;
    UINT16 dataLength;
}SubReceiveDataType;





typedef bool (*SENDFUNC)(UINT8* buf, UINT16 bufLength);
typedef RemoteResponse (*SUBRECEIVEFUNC)(SubReceiveDataType datatype, ADDSTATESFUNC addstatefunc, DapiGlobalVariable* dapiGlobalVariable );

typedef SubProcessState (*SUBPROCESSFUNC)(RemoteDevicesInformation *deviceinfor , UINT8* buf, UINT16* length, const  DapiGlobalVariable* dapiGlobalVariable );







#endif
#endif
