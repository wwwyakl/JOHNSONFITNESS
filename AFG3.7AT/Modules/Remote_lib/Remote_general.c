#include "remote_datatype.h"
#include "remote_general.h"
#include "crc.h"
#include "Events_Handler.h"
#ifdef __REMOTE_GENERAL__
#include "Remote_process.h"
#include "Remote_datatype.h"
#include "remote_port.h"
#include "page.h"
#include"Pro_MyFirst5k.h"
#include "Bluetooth_lib.h"
extern bool BT_Get_SyncFlag(void);

UINT16 Customcrc=0;
//USERS_INFO userinfo_bak;
struct
{
    char FLAG:1;
}CUSTOM_SYNC;
extern void ProCustom_Set_APPData(SetCustomprogram *CustomAPPInfo);
static StopWorkoutData g_StopWorkout;
void RemoteStopWorkotState(void);

static int StopWorkout(void)
{
    //RemoteStopWorkotState();
    if ((Program_Get_Status()==PS_RUN) || (Program_Get_Status()==PS_WARMUP) || (Program_Get_Status()==PS_COOLDOWN) || (Program_Get_Status()==PS_PAUSED))    //zsf
    {
        return Program_Set_VirtualKey((UINT16)VK_RESET|FRAME_STAGE_PRGRUN );
    }
    return 0;
}
extern MyFirst5kDat Program_Get_MyFirst5k_WeekWorkoutBuff(void);
extern UINT16 ProCustomDistance_Get_TotalTime(void);

static void GetStopWorkoutData( StopWorkoutData* data )
{
	MyFirst5kDat First5kData;
    data->workouttime=Program_Get_WorkoutTime_Elapsed();
    data->workoucalorie=Program_Get_Calories(true);
    data->Workoutdistance=Program_Get_Distance(DATA_AUTO);
    data->WorkoutMaxspeed=Data_Get_Speed(MAX_SPEEDX10);
    data->WorkoutAveragespeed=Program_Get_Speed_Average(DATA_AUTO);
    data->WorkoutMaxHeartrate = 210-User_Get_Age();
    data->WorkoutAverageheartrate=Program_Get_HR(AVERGE_HR);
    data->WorkoutElevation=Program_Get_Incline(1);
    data->WorkoutUnits=SystemConfig_Get_Display_Unit(User_Get_UserSlot());
    data->Workoutstartyear=0;
    data->Workoutstartmonth=0;
    data->Workoutstartday=0;
    data->Workoutstarthour=0;
    data->Workoutstartminute=0;
    data->Workoutstartsecond=0;
    UINT32 l_Dat;
    l_Dat = Program_Get_Pace_Average();
    l_Dat = (l_Dat/60*100)+(l_Dat%60);
    data->Workoutpace=l_Dat;
	if(Program_Get_ProgramID()==PS_MYFIRST5K)
    {
        First5kData=Program_Get_MyFirst5k_WeekWorkoutBuff();
        data->MyFirst5K_WEEK=First5kData.Week;
        data->MyFirst5K_WORKOUT=First5kData.Workout;
        if(data->MyFirst5K_WORKOUT == 1 || data->MyFirst5K_WORKOUT == 2)
            data->MyFirst5K_NEXT_WORKOUT_IN_DAYS = 2;
        else if(data->MyFirst5K_WORKOUT == 3)
            data->MyFirst5K_NEXT_WORKOUT_IN_DAYS = 3;
        data->MyFirst5K_FINISHED = ProMyFirst5k_Get_GoodLuckFlag();
    }
	else
    {
        data->MyFirst5K_WEEK=0;
        data->MyFirst5K_WORKOUT=0;
        data->MyFirst5K_NEXT_WORKOUT_IN_DAYS=0;
        data->MyFirst5K_FINISHED=0;
    }
}
void RemoteStopWorkotState(void)
{
      GetStopWorkoutData(&g_StopWorkout);
}

static void GetWorkoutDataRemote( WorkoutDataRemote* data )
{
    float mphCov = 0;
    strncpy(data->userExternalId , 0,36);
    data->programType = Program_Get_ProgramID( );
    data->workoutState=Program_Get_Status( ) -PS_WARMUP;

    data->stage=Program_Get_Stage();
    data->workoutTimeSeconds = Program_Get_WorkoutTime_Elapsed();
    data->stageTimeSeconds=Program_Get_stageTime();
    switch(data->workoutState)
    {
        case 0:
        {
            data->timeInStateSeconds= Program_Get_WorkoutTime_Elapsed();
            data->warmupTimeSeconds=data-> workoutTimeSeconds=data->timeInStateSeconds;
            data->cooldownTimeSeconds=0;
        }
        break;
        case 1:
        {
            data->timeInStateSeconds = Program_Get_WorkoutTime_Elapsed() - Program_Get_WorkoutTime_Warmup( );
            data->warmupTimeSeconds=Program_Get_WorkoutTime_Warmup();
            data->workoutTimeSeconds=Program_Get_WorkoutTime_Elapsed();//data->timeInStateSeconds;
            data->cooldownTimeSeconds=0;
        }
        break;
        case 2:
        {
            data->timeInStateSeconds =  Program_Get_WorkoutTime_Cooldown() - (Program_Get_WorkoutTime() - Program_Get_WorkoutTime_Elapsed());
            data->warmupTimeSeconds=Program_Get_WorkoutTime_Warmup();
            data->workoutTimeSeconds=Program_Get_WorkoutTime_Elapsed();//Program_Get_WorkoutTime()-Program_Get_WorkoutTime_Warmup()-Program_Get_WorkoutTime_Cooldown();
            data->cooldownTimeSeconds=data->timeInStateSeconds;
        }     
        break;
    }    
    data->timeInStageSeconds=Program_Get_timeInStage( );
    data->mphx100=Program_Get_Speed(DATA_AUTO, 0 )*100;
    if(SystemConfig_Get_Display_Unit(User_Get_UserSlot( ))==METRIC)    //zsf
    {      
        mphCov = 201168.0/125000;
        data->mphx100=(data->mphx100-2)/mphCov;//convert to STANARD
    }
    
    data->inclinex10=Program_Get_Incline( 0 );
    
    UINT32 l_Dat;
    l_Dat = Program_Get_Pace_Average();
    l_Dat = (l_Dat/60*100)+(l_Dat%60);
    data->averagePace=l_Dat;
    
    data->totalCalories= Program_Get_Calories( true );
    data->caloriesPerHour = Program_Get_CaloriesHR();//Program_Get_Calories(0)*3600/Program_Get_WorkoutTime_Elapsed();
    data->metsX10= Program_Get_METs( );
    data->watts = Program_Get_Watts(CURRENT_WATTS ,0 );
    data->milesx100= Program_Get_Distance(DATA_STANARD);
    data->heartRate= Program_Get_HR(CURRENT_HR);
    data->averageHeartRate= Program_Get_HR(AVERGE_HR);
    data->rpm= Program_Get_RPM( );
    data->programLevel= Program_Get_WorkoutLevel(0);
    data->resistanceLevel= Program_Get_Resistance( 0 );
    data->thr= Program_Get_HR(TARGET_HR);
    data->targetWattsX10= Program_Get_Watts(TARGET_WATTS , 0 );
    l_Dat = Program_Get_Pace();
    l_Dat = (l_Dat/60*100)+(l_Dat%60);
    data->nextIncline=l_Dat;
    data->nextSpeed=0;
    data->nextResistance=0;
    if(data->programType == PS_CUSTOM_DISTANCE)
    {
        if(data->mphx100)
        {
            data->totalTime = ProCustomDistance_Get_TotalTime();
        }
        else
            data->totalTime = 99*60;
    }
    else if(data->programType == PS_DISTANCE_GOAL)
    {
        if(data->mphx100)
        {
            UINT16 time = Data_Get_Time(ELAPSED_OLD_TIME);
            UINT16 timenew = (UINT32)Data_Get_Distance(REMAINING_OLD_DISTANCE)*36/Program_Get_Speed(DATA_STANARD, 0 );          
            if(timenew)
            {
                data->totalTime = timenew+time;
            }
            else
            {
                data->totalTime = time+time;
            }
        }
        else
            data->totalTime = 99*60;
    }
    else
    {
        if(data->programType == PS_CALORIES_GOAL)
        {
            if(Program_Get_Calories_PerSecond())
            {              
                UINT16 time = Data_Get_Time(ELAPSED_OLD_TIME);
                UINT16 timenew =  (UINT32)Data_Get_Calories(REMAINING_OLD_CALORIES)*10000/Program_Get_Calories_PerSecond();
                if(timenew)
                {
                    data->totalTime = timenew+time;
                }
                else
                {
                    data->totalTime = time+time;
                }
            }
            else
            {
                data->totalTime = 99*60;
            }
        }
        else
        {
            data->totalTime=Program_Get_WorkoutTime();
        }
    }
}
static void GetSyncUserData( UINT8 userslot,SyncUserData* data )
{
    USERS_INFO userinfo= SystemConfig_Get_UserInfo(userslot);
    MYFIRST5K_CONFIG	myfirst5kinfo=SystemConfig_Get_MyFirst5kInfo(userslot);
    CUSTOM_UNION CustomUserInfo;
    CUSTOM_CONFIG custominfo = SystemConfig_Get_CustomInfo(userslot);
    CustomUserInfo.CUSTOM_INFO.userID = userslot;
    CustomUserInfo.CUSTOM_INFO.TimeCustomSegnumber = custominfo.Custom_Time.customSegmentNumber;
    CustomUserInfo.CUSTOM_INFO.DistanceCustomSegnumber = custominfo.Custom_Distance.customSegmentNumber;
//    CustomUserInfo.CUSTOM_INFO.Units = custominfo.units;
    
    for(char i = 0;i < MAX_CUSTOM_SEGMENTS;i++)
    {
        CustomUserInfo.CUSTOM_INFO.customtimeDATA[i].TimeWorkoutTime = custominfo.Custom_Time.customWorkoutTime[i];
        CustomUserInfo.CUSTOM_INFO.customtimeDATA[i].TimeSpeedX10 = custominfo.Custom_Time.customSpeedX10[i];
        CustomUserInfo.CUSTOM_INFO.customtimeDATA[i].TimeInclineX10 = custominfo.Custom_Time.customInclineX10[i];
        CustomUserInfo.CUSTOM_INFO.customdisDATA[i].TimeWorkoutTime = custominfo.Custom_Distance.customWorkoutDistance[i];
        CustomUserInfo.CUSTOM_INFO.customdisDATA[i].TimeSpeedX10 = custominfo.Custom_Distance.customSpeedX10[i];
        CustomUserInfo.CUSTOM_INFO.customdisDATA[i].TimeInclineX10 = custominfo.Custom_Distance.customInclineX10[i];       
    }
    Customcrc = GenerateCRC_CCITT(CustomUserInfo.custominfobuf , sizeof(CustomUserInfo));
    data->userslot=userslot;
    strncpy(data->name,userinfo.name,65);
    data->weightX10=(userinfo.weightStandardX100)/10;
    data->year=userinfo.year;
    data->month=userinfo.month;
    data->day=userinfo.date;
    data->displayUnit=userinfo.displayUnit;
    data->customprogramcrc=Customcrc;
    data->customheartratecrc=0;
    data->myfirst5kweek=myfirst5kinfo.Week;
    data->myfirst5kworkout=myfirst5kinfo.Workout;
    data->myfirst5kwalkspeed=myfirst5kinfo.Walk_Speed;
    data->myfirst5kjogspeed=myfirst5kinfo.Jog_Speed;
    data->myfirst5kresetcount=myfirst5kinfo.Reset_Counts;
    memset(data->MMFtokenID,0,40);
    memset(data->MFPtokenID,0,320);
    memset(data->MFPuserID,0,15);

    memcpy(data->MMFtokenID,userinfo.MMFtokenID,40);
    memcpy(data->MFPtokenID,userinfo.MFPtokenID,320);
    memcpy(data->MFPuserID,userinfo.MFPuserID,15);
}
static void GetWorkoutSummarData( UINT8 userslot,WorkoutSummaryData* data[10] )
{
    USERS_INFO userinfo= SystemConfig_Get_UserInfo(userslot);
    MYFIRST5K_CONFIG	myfirst5kinfo=SystemConfig_Get_MyFirst5kInfo(userslot);
    
    data[0]->workout1time=userinfo.WorkoutSummaryData[0].workouttime;
}
static UINT16 SetUserInfo(SetUserData *userData)
{
    if(nCurrentPage != PAGE_ERP_COUNTDOWN && nCurrentPage != PAGE_WORK_TM1 && nCurrentPage != PAGE_WORK_TM2&& nCurrentPage != PAGE_WORK_BK1 && nCurrentPage != PAGE_WORK_BK2 && nCurrentPage != PAGE_ENG0_0_DisplayTest)    //zsf
    {
        if(userData->userConsoleId >SystemConfig_Get_MaxUserNumber())
        {
            return 1;
        }
        else
        {
            USERS_INFO userinfo= SystemConfig_Get_UserInfo(userData->userConsoleId);
            memcpy( userinfo.xid, userData->xid, XIDMAXLENGTH );
            memcpy( userinfo.passcode, userData->passcode, 4 );
            userinfo.weightStandardX100 = userData->weightX10*10;
            userinfo.age = userData->age;
            userinfo.year = userData->year;
            userinfo.month = userData->month;
            userinfo.date = userData->day;
            userinfo.gender = userData->gender;
            userinfo.languageId = userData->langauge;
            userinfo.displayUnit = userData->displayUnit;
            memcpy( userinfo.name, userData->name, 65 );
//            memcpy( userinfo.MMFtokenID, userData->MMFtokenID, 40 );
//            memcpy( userinfo.MFPtokenID, userData->MFPtokenID, 320 );
//            memcpy( userinfo.MFPuserID, userData->MFPuserID, 15 );
            
            if( SystemConfig_Set_UserInfo(userinfo, userData->userConsoleId))
            {
                SystemConfigProcess(SVK_SAVE_USER2FLASH, &userData->userConsoleId, sizeof(UCHAR));
            }
            if(userData->userConsoleId == 0)
            {            
                //int result=memcmp(&userinfo,&userinfo_bak,sizeof(USERS_INFO));
                //if (result != 0)
                static UINT8 displayUnit = 0;
                if (displayUnit != userinfo.displayUnit)
                {
                    //memcpy(&userinfo_bak,&userinfo,sizeof(USERS_INFO));
                    displayUnit = userinfo.displayUnit;
                    SystemConfigProcess(SVK_TOGGLE_UNIT,&userinfo.displayUnit,sizeof(userinfo.displayUnit));
                    SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
                }
            }
        }
        User_Set_VirtualKey((UINT16)VK_UPDATE|FRAME_STAGE_USER);
        User_Set_UserSlot(userData->userConsoleId);
        ERP_Clear_time();
        BT_Set_SyncFlag(true);
        return 0;
    }
    BT_Set_SyncFlag(true);
    return 1;
}

SubProcessState RemoteGeneralProcess(RemoteDevicesInformation *deviceinfor, UINT8* buf, UINT16 *bufLength,const  DapiGlobalVariable* dapiGlobalVariable)
{
    SubProcessState ret = SUB_PROCESS_STATE_NOT_SUPPORTED;
    UINT8 flag = deviceinfor->currentState>>8;
    UINT8 command = (UINT8)(deviceinfor->currentState&0x00ff);
    MACHINE_INFO machineConfig=SystemConfig_Get_MachineInfo( );  
    RemoteHeader *remoteHeader = ( RemoteHeader *) buf;


    remoteHeader->syncWord = 0xAA55;
    remoteHeader->lingoId = DAPI_LINGO_GENERAL;
    remoteHeader->messageId = command;
    remoteHeader->dataSize= 0;
    remoteHeader->checksum= 0;
    remoteHeader->transactionId = deviceinfor->transactionId;
	

    if((flag & REMOTE_PROPERTY_GENERAL) == 0)
    {
        return ret;
    }    

    switch(command)
    {
        case GENERAL_ACKNOWLEDGMENT:
        {
            RemoteDataAck *data =( RemoteDataAck *) (buf + sizeof(RemoteHeader));
            remoteHeader->dataSize=sizeof(RemoteDataAck);
            data->messageType=deviceinfor->argscounter;
            data->error=deviceinfor->cmdParameter;
            deviceinfor->reSend = 0xA5;
            ret = SUB_PROCESS_STATE_DIRECT_CONNECT;
        }
        break;
        case GENERAL_GET_LING_VERSION:
        {
        }
        break;
        case GENERAL_GET_MACHINE_INFO:
        {
            MachineInfo *data =( MachineInfo *) (buf + sizeof(RemoteHeader));
            remoteHeader->dataSize=sizeof(MachineInfo);
            SystemSettingType Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_SERIAL_NUMBER );
            memcpy( data->serialNumber, machineConfig.consoleSerialNumber, Settings.StringMaxLength);
            Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_FRAME_NUMBER );
            memcpy( data->frameSerialNumber, machineConfig.frameSerialNumber, Settings.StringMaxLength);
            Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_MACHINE_UUID );
            memcpy( data->machineIdentification, machineConfig.machineUUID, Settings.StringMaxLength);
            data->csafeId = machineConfig.csafeId;
            data->csafeManufacture = machineConfig.csafeManufactureId;
            data->csafeModelId = machineConfig.csafeModeId;
            data->driveType = machineConfig.driveType;
            data->brand = machineConfig.brandId;
//            data->machineType = machineConfig.machineModel;
//            data->modelType = machineConfig.machineType;
            if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
            {
                data->machineType = 0;
                data->modelType = 3;
            }
            else
            {
                data->machineType = 1;
                data->modelType = 0;
            }
            data->minMPHX10 = machineConfig.minSpeedMphX10;
            data->maxMPHX10 = machineConfig.maxSpeedMphX10;
            data->minKPHX10 = machineConfig.minSpeedKphX10;
            data->maxKPHX10 = machineConfig.maxSpeedKphX10;
            data->minInclineX10 = machineConfig.minInclineX10;
            data->maxInclineX10 = machineConfig.maxInclineX10;
            data->minLevel = machineConfig.minResistance;
            data->maxLevel = machineConfig.maxResistance;
            data->lubeMiles = SystemConfig_Get_Long(ACCUMULATED_LUBE_MILESX100_LONG)/10;
            data->accumulatedSeconds = SystemConfig_Get_Long(ACCUMULATED_SECONDS_LONG);
            data->accumulatedDistanceMilesX100 = SystemConfig_Get_Long(ACCUMULATED_MILESX100_LONG);
            data->softwareVersion =(( (machineConfig.versionMajor<<8)|(machineConfig.versionMinor))<<8)|machineConfig.versionRevision;
            data->wifiProtocolVersion = 0;
            deviceinfor->reSend = 0xAA;
            data->wifiUARTVersion = 0;            
            data->btUARTVersion = 0;
            data->lcbVendor = 0;
            data->lcbVersion = 0;
            data->osVersion = 0;
            data->numUsers = SystemConfig_Get_MaxUserNumber();
            memcpy( data->macAddress, deviceinfor->deviceMacAddress, 6 );
            memcpy( data->ipAddress, deviceinfor->deviceIpAddress, 6 );
            if(Program_Get_Status() < PS_WAIT_3S)
            {
                data->errorCode = 0;
            }
            else if(Program_Get_Status() <= PS_COOLDOWN)
            {
                if(PS_PAUSED == Program_Get_Status())
                {
                    data->errorCode |= 0x0200;
                }
                else if(PS_COMPLETED != Program_Get_Status())
                {
                    data->errorCode |= 0x0100;
                }
            }
            if(FRAME_STAGE_SAFFKEYOFF == Event_Handler_GetNewState())
            {
                data->errorCode |= 0x0001;
            }
//            if(FRMAE_STATE_DEMOMODE == Event_Handler_GetNewState())    //zsf
//            {
//                data->errorCode |= 0x0001;
//            }
//            if(FRAME_STAGE_ENG == Event_Handler_GetNewState())    //zsf
//            {
//                data->errorCode |= 0x0001;
//            }
            data->serviceCode = 0;
            data->motorTemperature = 0;
            data->busCurrent = 0;
            data->busVoltage = 0;
            data->AutoLogin = machineConfig.isAutoLogin;
            ret = SUB_PROCESS_STATE_DIRECT_CONNECT;
        }
        break;
        case GENERAL_SET_SPEED_CALIBRATION:
        {
        }
        break;
        case GENERAL_GET_SPEED_CALIBRATION:
        {
        }
        break;
        case GENERAL_SET_PWM_CAL:
        {
        }
        break;
        case GENERAL_GET_PWM_CAL:
        {
        }
        break;
        case GENERAL_SET_ECB_CAL:
        {
        }
        break;
        case GENERAL_GET_ECB_CAL:
        {
        }
        break;
        case GENERAL_SET_WATTS_TABLE:
        {
        }
        break;
        case GENERAL_GET_WATTS_TABLE:
        {
        }
        break;
        case GENERAL_SET_DATA_SERVER:
        {
//          DateServer *data =( DateServer *) (buf + sizeof(RemoteHeader));
//          remoteHeader->dataSize= sizeof(DateServer);
//          data->status = deviceinfor->cmdParameter;
////          deviceinfor->reSend = 0xA5;
//          ret = SUB_PROCESS_STATE_DIRECT_CONNECT;
        }
        break;
        case GENERAL_GET_DATA_SERVER:
        {
        }
        break;
        case GENERAL_SET_MACHINE_STATE:
        {
        }
        break;
        case GENERAL_GET_MACHINE_STATE:
        {
        }
        break;
        case GENERAL_SET_USER_DATA:
        {
        }
        break;
        case GENERAL_GET_USER_DATA:
        {
        }
        break;
        case GENERAL_SET_WORKOUT_DATA_RATE:
        {
        }
        break;
        case GENERAL_GET_WORKOUT_DATA:
        {

            WorkoutDataRemote *data = ( WorkoutDataRemote*)(buf + sizeof(RemoteHeader));
            remoteHeader->dataSize=sizeof(WorkoutDataRemote);
            GetWorkoutDataRemote( data );
            deviceinfor->reSend = 0xA5;
            ret = SUB_PROCESS_STATE_DIRECT_CONNECT;
        }
        break;
        case GENERAL_GET_CURRENT_USER:
        {
            UINT8 *data = buf + sizeof(RemoteHeader);
            remoteHeader->dataSize=sizeof(UINT8);
            *data = deviceinfor->cmdParameter;
            ret = SUB_PROCESS_STATE_DIRECT_CONNECT;
        }
        break;
        case GENERAL_STOP_WORKOUT:
        {
//            StopWorkoutData *data = ( StopWorkoutData*)(buf + sizeof(RemoteHeader));
//            remoteHeader->dataSize=sizeof(StopWorkoutData);
//            memcpy(data,&g_StopWorkout,sizeof(StopWorkoutData));
//			GetStopWorkoutData(data);
            Show_ClearLedLight();    //zsf
            Program_Initial_Inner_Data();    //zsf
            Program_NextStatus(PS_IDLE);
            deviceinfor->reSend = 0xA5;
            ret = SUB_PROCESS_STATE_DIRECT_CONNECT;
        }
        break;
        case GENERAL_CHECK_FOR_UPDATE:
        {
        }
        break;
        case GENERAL_SYNC_USER_DATA:
        {
            SyncUserData *data =( SyncUserData *) (buf + sizeof(RemoteHeader));
            remoteHeader->dataSize=sizeof(SyncUserData);
//            deviceinfor->reSend = 0xA5;
            GetSyncUserData(deviceinfor->argscounter,data); 		
            ret = SUB_PROCESS_STATE_DIRECT_CONNECT;
        }
        break;
        case GENERAL_PUSH_WORKOUT_SUMMARY:
        {
            PushWorkoutSummary *data =( PushWorkoutSummary *) (buf + sizeof(RemoteHeader));
            WorkoutSummaryData *workoutsummary[10]={( WorkoutSummaryData *) (buf + sizeof(RemoteHeader))};
            memset(data->name,0,65);
            memset(data->MMFtokenID,0,40);
            memset(data->MFPtokenID,0,320);
            memset(data->MFPuserID,0,15);
            
//            deviceinfor->reSend = 0xA5;
            data->userslot=deviceinfor->argscounter;
            USERS_INFO userinfo= SystemConfig_Get_UserInfo(deviceinfor->argscounter);
            memcpy(data->name,userinfo.name,65);
            memcpy(data->MMFtokenID,userinfo.MMFtokenID,40);
            memcpy(data->MFPtokenID,userinfo.MFPtokenID,320);
            memcpy(data->MFPuserID,userinfo.MFPuserID,15);
            if(userinfo.WorkoutSummaryDataCount <= 3)
                data->workoutscounter = userinfo.WorkoutSummaryDataCount;
            else
                data->workoutscounter = 3;
			for(int i=0; i<data->workoutscounter; i++)
			{
				//memcpy(*(data+sizeof(PushWorkoutSummary)+sizeof(WorkoutSummaryData)*i),,sizeof(WorkoutSummaryData));
			    data->worksummary[i].workout1time=userinfo.WorkoutSummaryData[i].workouttime;
                data->worksummary[i].workout1calorie=userinfo.WorkoutSummaryData[i].workoutcalorie;
                data->worksummary[i].workout1distance=userinfo.WorkoutSummaryData[i].workoutdistance;
                data->worksummary[i].workout1maxspeed=userinfo.WorkoutSummaryData[i].workoutmaxspeed;
                data->worksummary[i].workout1averagespeed=userinfo.WorkoutSummaryData[i].workoutaveragespeed;
                data->worksummary[i].workout1maxheartrate=userinfo.WorkoutSummaryData[i].workoutmaxheartrate;
                data->worksummary[i].workout1averageheartrate=userinfo.WorkoutSummaryData[i].workoutaverageheartrate;
                data->worksummary[i].workout1elevation=userinfo.WorkoutSummaryData[i].workoutelevation;
                data->worksummary[i].workout1units=userinfo.WorkoutSummaryData[i].workoutunits;
                data->worksummary[i].workout1startyear=userinfo.WorkoutSummaryData[i].workoutstartyear;
                data->worksummary[i].workout1startmonth=userinfo.WorkoutSummaryData[i].workoutstartmonth;
                data->worksummary[i].workout1startday=userinfo.WorkoutSummaryData[i].workoutstartday;
                data->worksummary[i].workout1starthour=userinfo.WorkoutSummaryData[i].workoutstarthour;
                data->worksummary[i].workout1startminute=userinfo.WorkoutSummaryData[i].workoutstartminute;
                data->worksummary[i].workout1startsecond=userinfo.WorkoutSummaryData[i].workoutstartsecond; 
                
            }
            userinfo.WorkoutSummaryDataCount = 0;//AeIsEy?YEI¡¦?oo??AaCa3y
            if(SystemConfig_Set_UserInfo(userinfo,data->userslot))
            {
                SystemConfigProcess(SVK_SAVE_USER2FLASH, &data->userslot, sizeof(data->userslot));
            }
            
            remoteHeader->dataSize=sizeof(PushWorkoutSummary)-sizeof(WorkoutSummaryData)*10+sizeof(WorkoutSummaryData)*data->workoutscounter;
            ret = SUB_PROCESS_STATE_DIRECT_CONNECT;
        }
        break;
        case GENERAL_SET_POPUP_INFORMATION:
        {
            PopupInformation *data =( PopupInformation *) (buf + sizeof(RemoteHeader));
            remoteHeader->dataSize=sizeof(PopupInformation);
            data->popuptype=deviceinfor->argscounter;
            data->disappeartime=deviceinfor->cmdParameter;
//            if(data->popuptype == 0x05 || data->popuptype == 0x06)
                deviceinfor->reSend = 0xA5;
            ret = SUB_PROCESS_STATE_DIRECT_CONNECT;
        }
        break;
        case GENERAL_SET_CUSTOM_HR_PROGRAM:
        { 
            SetCustomHRprogram *data =( SetCustomHRprogram *) (buf + sizeof(RemoteHeader));
            remoteHeader->dataSize=sizeof(SetCustomHRprogram);
            data->User_slot=deviceinfor->argscounter;
            memcpy(data+1,deviceinfor->parameterPoint,sizeof(CustomHRParameter));
            ret = SUB_PROCESS_STATE_DIRECT_CONNECT;
        }
        break;
        case GENERAL_SET_CUSTOM_PROGRAM:
        { 
            CUSTOM_CONFIG custominfo = SystemConfig_Get_CustomInfo(User_Get_UserSlot());
            SetCustomprogram *data =( SetCustomprogram *) (buf + sizeof(RemoteHeader));
            remoteHeader->dataSize=sizeof(SetCustomprogram);
            data->User_slot=User_Get_UserSlot();
            data->TimeCustomSegnumber=custominfo.Custom_Time.customSegmentNumber;
            data->DistanceCustomSegnumber=custominfo.Custom_Distance.customSegmentNumber;
            data->Units = custominfo.units;
            for(char i = 0;i < MAX_CUSTOM_SEGMENTS;i++)
            {
                data->Time[i].Goal = custominfo.Custom_Time.customWorkoutTime[i];
                data->Time[i].speed = custominfo.Custom_Time.customSpeedX10[i];
                data->Time[i].incline = custominfo.Custom_Time.customInclineX10[i];
                data->Distance[i].Goal = custominfo.Custom_Distance.customWorkoutDistance[i];
                data->Distance[i].speed = custominfo.Custom_Distance.customSpeedX10[i];
                data->Distance[i].incline = custominfo.Custom_Distance.customInclineX10[i];       
            }
//            memcpy(data->Time,tempdata,16*sizeof(CustomParameter));
//            memcpy(data->Distance,tempdata+16,16*sizeof(CustomParameter));
            ret = SUB_PROCESS_STATE_DIRECT_CONNECT;
        }
        break;
        case GENERAL_SET_MY_FIRST_5K:
        {
//            SetMyFirst5K *tempdata=(SetMyFirst5K *)deviceinfor->parameterPoint;
            SetMyFirst5K *data =( SetMyFirst5K *) (buf + sizeof(RemoteHeader));
            remoteHeader->dataSize=sizeof(SetMyFirst5K);
            data->userSlot=deviceinfor->argscounter;
            MYFIRST5K_CONFIG ProMyFirst5k_Info = SystemConfig_Get_MyFirst5kInfo(data->userSlot);;
            data->week = ProMyFirst5k_Info.Week;
            data->workout = ProMyFirst5k_Info.Workout;
            data->walkSpeed = ProMyFirst5k_Info.Walk_Speed;
            data->jogSpeed = ProMyFirst5k_Info.Jog_Speed;
            data->resetCount = ProMyFirst5k_Info.Reset_Counts; 
            
            ret = SUB_PROCESS_STATE_DIRECT_CONNECT;
        }
        break;
        default:
        break;
    }
	if(remoteHeader->dataSize>0)
		remoteHeader->checksum=GenerateCRC_CCITT((buf+sizeof(RemoteHeader)) , remoteHeader->dataSize);
		
    *bufLength=sizeof(RemoteHeader)+remoteHeader->dataSize;
    return ret;

    
}
RemoteResponse RemoteGeneralReceive(SubReceiveDataType datatype, ADDSTATESFUNC addstatefunc, DapiGlobalVariable* dapiGlobalVariable )
{
    RemoteResponse ret = REMOTE_RESPONSE_START;
    ret = REMOTE_RESPONSE_SUCCESFUL;
    
    switch(datatype.command)
    {
        case GENERAL_ACKNOWLEDGMENT:
        {
            RemoteDataAck *reponse =( RemoteDataAck *)datatype.data;

			if(reponse->error)
			{
                ret = REMOTE_RESPONSE_START;
	            addstatefunc(REMOTE_PROPERTY_GENERAL<<8|reponse->messageType, 0, 0, NULL );
			}
        }
        break;
        case GENERAL_GET_LING_VERSION:
        {
        }
        break;
        case GENERAL_GET_MACHINE_INFO:
        {
            addstatefunc(REMOTE_PROPERTY_GENERAL<<8|GENERAL_GET_MACHINE_INFO, 1, 0, NULL );
//            for( char i=0;i<=SystemConfig_Get_MaxUserNumber();i++)
//            {
//                addstatefunc(REMOTE_PROPERTY_GENERAL<<8|GENERAL_SYNC_USER_DATA, i, 0, NULL );
//            }
//            for( char i=1;i<=SystemConfig_Get_MaxUserNumber();i++)
//            {
//                addstatefunc(REMOTE_PROPERTY_GENERAL<<8|GENERAL_PUSH_WORKOUT_SUMMARY, i, 0, NULL );
//            }
//            addstatefunc(REMOTE_PROPERTY_CONTROL<<8|CONTROL_GET_NETWORK_TIME, 0, 0, NULL );
        }
        break;
        case GENERAL_SET_SPEED_CALIBRATION:
        {
        }
        break;
        case GENERAL_GET_SPEED_CALIBRATION:
        {
        }
        break;
        case GENERAL_SET_PWM_CAL:
        {
        }
        break;
        case GENERAL_GET_PWM_CAL:
        {
        }
        break;
        case GENERAL_SET_ECB_CAL:
        {
        }
        break;
        case GENERAL_GET_ECB_CAL:
        {
        }
        break;
        case GENERAL_SET_WATTS_TABLE:
        {
        }
        break;
        case GENERAL_GET_WATTS_TABLE:
        {
        }
        break;
        case GENERAL_SET_DATA_SERVER:
        {
        }
        break;
        case GENERAL_GET_DATA_SERVER:
        {
        }
        break;
        case GENERAL_SET_MACHINE_STATE:
        {
        }
        break;
        case GENERAL_GET_MACHINE_STATE:
        {
        }
        break;
        case GENERAL_SYNC_USER_DATA:
        {
            UINT16 AppCrcValue=0;
            SyncUserData *reponse = (SyncUserData *)datatype.data;
            USERS_INFO userinfo= SystemConfig_Get_UserInfo(reponse->userslot);
            userinfo.weightStandardX100 = reponse->weightX10*10;
            userinfo.year = reponse->year;
            userinfo.month = reponse->month;
            userinfo.date = reponse->day;
            userinfo.displayUnit = reponse->displayUnit;
            memcpy(userinfo.name,reponse->name,sizeof(reponse->name));
            
            memset(userinfo.MMFtokenID,0,40);
            memset(userinfo.MFPtokenID,0,320);
            memset(userinfo.MFPuserID,0,15);
            
            memcpy(userinfo.MMFtokenID,reponse->MMFtokenID,40);
            memcpy(userinfo.MFPtokenID,reponse->MFPtokenID,320);
            memcpy(userinfo.MFPuserID,reponse->MFPuserID,15);
            if(reponse->userslot == 0)
            {
                SystemConfigProcess(SVK_TOGGLE_UNIT,&userinfo.displayUnit,sizeof(userinfo.displayUnit));
                SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
            }
            else if((SystemConfig_Set_UserInfo(userinfo,reponse->userslot)))
            {
                SystemConfigProcess(SVK_SAVE_USER2FLASH, &reponse->userslot,sizeof(reponse->userslot));
            }
            AppCrcValue = reponse->customprogramcrc;
            if(Customcrc == AppCrcValue)
            {
                CUSTOM_SYNC.FLAG = 0;
            }
            else
            {
                CUSTOM_SYNC.FLAG = 1;
            }
        }
        break;
        case GENERAL_SET_USER_DATA:
        {
            
            SetUserData *reponse = (SetUserData *)datatype.data;
            UINT8 LONGLENGTH = datatype.dataLength;
            addstatefunc(REMOTE_PROPERTY_GENERAL<<8|GENERAL_ACKNOWLEDGMENT, GENERAL_SET_MACHINE_STATE, SetUserInfo(reponse), NULL );
        }
        break;
        case GENERAL_GET_USER_DATA:
        {
          
        }
        break;
        case GENERAL_SET_WORKOUT_DATA_RATE:
        {
        }
        break;
        case GENERAL_GET_WORKOUT_DATA:
        {
//            StopWorkoutData *reponse = (StopWorkoutData *)datatype.data;
//            Beep_Set_Beeps(1,3,1);
            addstatefunc(REMOTE_PROPERTY_GENERAL<<8|GENERAL_GET_WORKOUT_DATA, 0, 0, NULL );
        }
        break;
        case GENERAL_GET_CURRENT_USER:
        {
           // addstatefunc(REMOTE_PROPERTY_GENERAL<<8|GENERAL_GET_WORKOUT_DATA, 1, translateid, NULL );
        }
        break;
        case GENERAL_STOP_WORKOUT:
        {
            
            StopWorkoutData *reponse = (StopWorkoutData *)datatype.data;
            
//                userinfo.weightStandardX100 = reponse->weightX10;
//                userinfo.age = reponse->age;
//                userinfo.year = reponse->year;
//                userinfo.month = reponse->month;
//                userinfo.date = reponse->day;
//                userinfo.gender = reponse->gender;
//                userinfo.languageId = reponse->langauge;
            //Beep_Set_Beeps(6,10,10);   //zsf
            StopWorkout( );
            addstatefunc(REMOTE_PROPERTY_GENERAL<<8|GENERAL_ACKNOWLEDGMENT, GENERAL_STOP_WORKOUT, 0, NULL );
        }
        break;
        case GENERAL_CHECK_FOR_UPDATE:
        {
            
        }
        break;
        case GENERAL_SET_CUSTOM_HR_PROGRAM:
        { 
            SetCustomHRprogram *reponse = (SetCustomHRprogram *)datatype.data;
		
             UINT8 ack = 0;
//            if(Program_Get_Status() != PS_IDLE ||Program_Get_Status() != PS_SELECTPRG)
//            {
//                ack =1;
//            }
//            else 
                if(reponse->User_slot >SystemConfig_Get_MaxUserNumber())
            {
                ack =1;
            }
            else
            {
            	//to do
            }
			addstatefunc(REMOTE_PROPERTY_GENERAL<<8|GENERAL_ACKNOWLEDGMENT, GENERAL_SET_CUSTOM_HR_PROGRAM, ack, NULL );			
        }
        break;
        case GENERAL_SET_CUSTOM_PROGRAM:
        { 
            SetCustomprogram *reponse = (SetCustomprogram *)datatype.data;
             UINT8 ack = 0;
//            if(Program_Get_Status() != PS_IDLE ||Program_Get_Status() != PS_SETUP)
//            {
//                ack =1;
//            }
//            else 
                if(reponse->User_slot >SystemConfig_Get_MaxUserNumber())
            {
                ack =1;
            }
            else
            {
                ProCustom_Set_APPData(reponse);
                CUSTOM_CONFIG ProCustomInfo = SystemConfig_Get_CustomInfo(reponse->User_slot);
                ProCustomInfo.Custom_Time.customSegmentNumber=reponse->TimeCustomSegnumber;
                ProCustomInfo.Custom_Distance.customSegmentNumber=reponse->DistanceCustomSegnumber;
                ProCustomInfo.units = reponse->Units;
                for(char i = 0;i < MAX_CUSTOM_SEGMENTS;i++)
                {
                    ProCustomInfo.Custom_Time.customWorkoutTime[i] = reponse->Time[i].Goal;
                    ProCustomInfo.Custom_Time.customSpeedX10[i] = reponse->Time[i].speed;
                    ProCustomInfo.Custom_Time.customInclineX10[i] = reponse->Time[i].incline;
                    
                    ProCustomInfo.Custom_Distance.customWorkoutDistance[i] = reponse->Distance[i].Goal;
                    ProCustomInfo.Custom_Distance.customSpeedX10[i] = reponse->Distance[i].speed;
                    ProCustomInfo.Custom_Distance.customInclineX10[i] = reponse->Distance[i].incline;   
                }
                if(SystemConfig_Set_CustomInfo(ProCustomInfo,reponse->User_slot))
                {
                    SystemConfigProcess(SVK_SAVE_CUSTOMPRG2FLASH, NULL,0);
                }
            }
			addstatefunc(REMOTE_PROPERTY_GENERAL<<8|GENERAL_ACKNOWLEDGMENT, GENERAL_SET_CUSTOM_PROGRAM, ack, NULL );
        }
        break;
        case GENERAL_SET_MY_FIRST_5K:
        {
            SetMyFirst5K *reponse = (SetMyFirst5K *)datatype.data;
            UINT8 ack = 0;
            //            if(Program_Get_Status() != PS_IDLE && Program_Get_Status() != PS_SELECTPRG)
            //            {
            //                ack =1;
            //            }
            //            else 
            if(reponse->userSlot >SystemConfig_Get_MaxUserNumber())
            {
                ack =1;
            }
            else
            {
                MYFIRST5K_CONFIG ProMyFirst5kInfo = SystemConfig_Get_MyFirst5kInfo(User_Get_UserSlot());
                
                ProMyFirst5kInfo.Workout = reponse->workout;
                ProMyFirst5kInfo.Week = reponse->week;
                ProMyFirst5kInfo.Walk_Speed = reponse->walkSpeed;
                ProMyFirst5kInfo.Jog_Speed = reponse->jogSpeed;
                ProMyFirst5kInfo.Walk_SpeedBuff = ProMyFirst5kInfo.Walk_Speed;
                ProMyFirst5kInfo.Jog_SpeedBuff = ProMyFirst5kInfo.Jog_Speed;
                ProMyFirst5kInfo.OldWalk_SpeedBuff = ProMyFirst5kInfo.Walk_Speed;
                ProMyFirst5kInfo.OldJog_SpeedBuff = ProMyFirst5kInfo.Jog_Speed;
                ProMyFirst5kInfo.Reset_Counts = reponse->resetCount;
                ProMyFirst5k_Set_DefaultData();
                if(SystemConfig_Set_MyFirst5kInfo(ProMyFirst5kInfo,reponse->userSlot))
                {
                    SystemConfigProcess(SVK_SAVE_MYFIRST5KPRG2FLASH, NULL,0);
                }
            }
			addstatefunc(REMOTE_PROPERTY_GENERAL<<8|GENERAL_ACKNOWLEDGMENT, GENERAL_SET_MY_FIRST_5K, ack, NULL );
        }
        break;
        default:
            break;
			}

    return ret;
}
#endif
