#include "remote_asset.h"

#ifdef __REMOTE_ASSET_MANAGE__
#include "SystemConfig.h"
#include "UtilitiesRtc.h"
#include "Remote_process.h"
#include "Remote_datatype.h"
#include "remote_port.h"

extern char* rscu_version[3];
const static char* softwaretype[SOFTWARE_TYPE_END]={
    "gui",
     "boot_loader",
     "non_gui_mcu",
     "non_gui_sub_mcu",
     "lcb_1",
     "lcb_2",
     "lcm",
     "wifi",
     "bluetooth",
     "rfid",
     "nfc"
};

UINT8  RemoteGetSoftwareTypeIndex(const char *st)
{
    UINT8 ret = 0xff;
    for(SOFTWARE_TYPE start =SOFTWARE_TYPE_START; start<SOFTWARE_TYPE_END; start++ )
    {
        if(strcmp(softwaretype[start], st) == 0)
        {
            ret = start;
        }
    }

    return ret;
}


SubProcessState RemoteAssetProcess(RemoteDevicesInformation *deviceinfor, UINT8* buf, UINT16 *bufLength, const  DapiGlobalVariable* dapiGlobalVariable)
{
    SubProcessState ret = SUB_PROCESS_STATE_NOT_SUPPORTED;
    //UINT8 flag = deviceinfor->currentState>>8;
    UINT8 command = (UINT8)(deviceinfor->currentState&0x00ff);
    MACHINE_INFO machineConfig=SystemConfig_Get_MachineInfo( );     
    SystemSettingType Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_MACHINE_UUID );
    RemoteHeader *remoteHeader = (RemoteHeader*)buf;


    remoteHeader->syncWord = 0xAA55;
    remoteHeader->lingoId = DAPI_LINGO_ASSET_MANAGEMENT;
    remoteHeader->messageId = command;


    switch(command)
    {        
        case REMOTE_ASSET_MANAGEMENT_GET_NETWORK_TIME:
        {            
            TimeRequestRegion *data =(TimeRequestRegion*)( buf + sizeof(RemoteHeader));
            switch(deviceinfor->currentSubState)
            {
                case 0:
                {
                    remoteHeader->lingoId = DAPI_LINGO_NETWORK_TIME;
                    remoteHeader->messageId = REMOTE_ASSET_MANAGEMENT_END - REMOTE_ASSET_MANAGEMENT_GET_NETWORK_TIME;
                    Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_TIME_ZONE );
                    memcpy(data->timeZone, machineConfig.timezone, Settings.StringMaxLength );
                    ret = SUB_PROCESS_STATE_NEXT_SUBSTATE;
                }
                break;
                case 1:
                {
                    ret = SUB_PROCESS_STATE_NEXT_STATE;
                }
                break;
            }            
        }
        break;
        case REMOTE_ASSET_MANAGEMENT_MACHINE_INFO_EX:
        {
            MachineInfoEX *data = (MachineInfoEX*)(buf + sizeof(RemoteHeader));
            switch(deviceinfor->currentSubState)
            {
                case 0:
                {
                    Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_SERIAL_NUMBER );
                    memcpy(data->serialNumber , machineConfig.consoleSerialNumber, Settings.StringMaxLength);
                    Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_FRAME_NUMBER );
                    memcpy(data->frameSerialNumber, machineConfig.frameSerialNumber , Settings.StringMaxLength );
                    Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_MACHINE_UUID );
                    memcpy( data->machineIdentification , machineConfig.machineUUID ,Settings.StringMaxLength );
                    memcpy(data->macAddress, deviceinfor->deviceMacAddress, 6);
                    data->brandID = machineConfig.brandId;
                    data->machineType = machineConfig.machineType;
                    data->modelType = machineConfig.machineModel;
                    data->driveType = machineConfig.driveType;
                    data->consoleType = machineConfig.consoleType;
                    data->csafeManufacture = machineConfig.csafeManufactureId;
                    data->csafeID = machineConfig.csafeId;
                    data->csafeModelID = machineConfig.csafeModeId;
                    data->clubID = machineConfig.clubId;
                    data->minMPHX10 = machineConfig.minSpeedMphX10;
                    data->maxMPHX10 = machineConfig.maxSpeedMphX10;
                    data->minKPHX10 = machineConfig.minSpeedKphX10;
                    data->maxKPHX10 = machineConfig.maxSpeedKphX10;
                    data->minInclineX10 = machineConfig.minInclineX10;
                    data->maxInclineX10 = machineConfig.maxInclineX10;
                    data->minResistanceLevel = machineConfig.minResistance;
                    data->maxResistanceLevel = machineConfig.maxResistance;
                    SystemConfig_Get_Version_Code(data->softwareVersion);
                    data->dapiApiVersion = SystemConfig_Get_Dapi_Version( );
                    data->LCBVendor = 0;
                    data->LCBVersion = 0;
                    memset(data->osVersion, '0', 20);
                    data->maxUsers = SystemConfig_Get_MaxUserNumber( );
                    memcpy(data->IpAddress, deviceinfor->deviceIpAddress, 6);
                    Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_TIME_ZONE );
                    memcpy(data->timeZone, machineConfig.timezone, Settings.StringMaxLength );
                    data->autoLogin = machineConfig.isAutoLogin;
                    data->lastUpdateYear = machineConfig.lastUpdateYear;
                    data->lastUpdateMonth = machineConfig.lastUpdateMonth;
                    data->lastUpdateDay = machineConfig.lastUpdateDay;
                    data->lastUpdateHour = machineConfig.lastUpdateHour;
                    data->lastUpdateMinute = machineConfig.lastUpdateMinute;
                    data->lastUpdateSecond = machineConfig.lastUpdateSecond;
                    memcpy(data->lastUpdateTimezone,machineConfig.lastUpdateTimezone,Settings.StringMaxLength);
                    ret = SUB_PROCESS_STATE_NEXT_SUBSTATE;
                }
                break;
                case 1:
                {
                    ret = SUB_PROCESS_STATE_NEXT_STATE;
                }
                break;
            }
        }
        break;
        case REMOTE_ASSET_MANAGEMENT_UPDATE_MACHINE_STATE:
        {
            UpdateMachineStatus *data =  (UpdateMachineStatus*)(buf + sizeof(RemoteHeader));
            
            switch(deviceinfor->currentSubState)
            {

                case 0:
                {
                    data->accumulatedDistanceMilesX100 = SystemConfig_Get_Long(ACCUMULATED_MILESX100_LONG);
                    data->accumulatedTime = SystemConfig_Get_Long(ACCUMULATED_SECONDS_LONG);
                    data->lubeBeltMilesX10 = SystemConfig_Get_Long(ACCUMULATED_LUBE_MILESX100_LONG)/10;
                    data->busCurrent = 0;
                    data->busVoltage = 0;
                    data->headphoneInsertions = 0;
                    data->headphoneRemovals = 0;
                    data->motorTemperature = 0;
                    memcpy( data->MacAddress, deviceinfor->deviceMacAddress, 6 );
                    memcpy( data->machineIdentification, machineConfig.machineUUID, Settings.StringMaxLength);
                    ret = SUB_PROCESS_STATE_NEXT_SUBSTATE;
                }
                break;
                case 1:
                {
                    ret = SUB_PROCESS_STATE_NEXT_STATE;
                }
                break;
            }
        }
        break;
        case REMOTE_ASSET_MANAGEMENT_MACHINE_INFO:
        {
            MachineInfoData *data =(MachineInfoData*)( buf + sizeof(RemoteHeader));
            switch(deviceinfor->currentSubState)
            {
                case 0:
                {
                    Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_SERIAL_NUMBER );
                    memcpy( data->serialNumber, machineConfig.consoleSerialNumber, Settings.StringMaxLength );
                    Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_FRAME_NUMBER );
                    memcpy( data->frameSerialNumber, machineConfig.frameSerialNumber, Settings.StringMaxLength );
                    Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_MACHINE_UUID );
                    memcpy( data->machineIdentification, machineConfig.machineUUID, Settings.StringMaxLength);
                    data->csafeId = machineConfig.csafeId;
                    data->csafeManufacture = machineConfig.csafeManufactureId;
                    data->csafeModelId = machineConfig.csafeModeId;
                    data->machineType = machineConfig.machineType;
                    data->modelType = machineConfig.machineModel;
                    data->minMPHX10 = machineConfig.minSpeedMphX10;
                    data->maxMPHX10 = machineConfig.maxSpeedMphX10;
                    data->minKPHX10 = machineConfig.minSpeedKphX10;
                    data->maxKPHX10 = machineConfig.maxSpeedKphX10;
                    data->minInclineX10 = machineConfig.minInclineX10;
                    data->maxInclineX10 = machineConfig.maxInclineX10;
                    data->minLevel = machineConfig.minResistance;
                    data->maxLevel = machineConfig.maxResistance;
                    data->accumulatedSeconds = SystemConfig_Get_Long(ACCUMULATED_SECONDS_LONG);
                    data->accumulatedDistanceMilesX100 = SystemConfig_Get_Long(ACCUMULATED_MILESX100_LONG);;
                    data->softwareVersion = (( (machineConfig.versionMajor<<8)|(machineConfig.versionMinor))<<8)|machineConfig.versionRevision;
                    data->wifiProtocolVersion = 0;
                    data->wifiFirmwareVersion = 0;
                    data->wifiUARTVersion = 0;
                    data->lcbVendor = 0;
                    data->lcbVersion = 0;
                    data->osVersion = 0;
                    data->numUsers = SystemConfig_Get_MaxUserNumber();
                    memcpy( data->macAddress, deviceinfor->deviceMacAddress, 6 );
                    memcpy( data->ipAddress, deviceinfor->deviceIpAddress, 6 );
                    data->errorCode = 0;
                    data->serviceCode = 0;
                    data->motorTemperature = 0;
                    data->busCurrent = 0;
                    data->busVoltage = 0;
                    data->lubeMiles =SystemConfig_Get_Long(ACCUMULATED_LUBE_MILESX100_LONG);
                    Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_TIME_ZONE );
                    memset( data->timeZone, 0, Settings.StringMaxLength);
                    memcpy( data->timeZone, machineConfig.timezone, Settings.StringMaxLength);
                    data->AutoLogin = machineConfig.isAutoLogin;
                    data->DapiVersion = SystemConfig_Get_Dapi_Version() ; 
                    ret = SUB_PROCESS_STATE_NEXT_SUBSTATE;
                }
                break;
                case 1:
                {
                    ret = SUB_PROCESS_STATE_NEXT_STATE;
                }
                break;
            }
        }
        break;
        case REMOTE_ASSET_MANAGEMENT_MACHINE_REGISTRATION:
        {
            MachineRegisterData *data = (MachineRegisterData*)(buf + sizeof(RemoteHeader));
            switch(deviceinfor->currentSubState)
            {
                case 0:
                {             
                    memcpy(data->macAddress, deviceinfor->deviceMacAddress, 6);
                    ret = SUB_PROCESS_STATE_NEXT_SUBSTATE;
                }
                break;
                case 1:
                {
                    ret = SUB_PROCESS_STATE_NEXT_STATE;
                }
                break;
            }
        }
        break;
        case REMOTE_ASSET_MANAGEMENT_GET_DAPI_LOCATION:
        {
            DAPILocation *data =(DAPILocation*)( buf + sizeof(RemoteHeader));
            switch(deviceinfor->currentSubState)
            {
                case 0:
                {             
                    memcpy(data->machineIdentification, machineConfig.machineUUID,Settings.StringMaxLength);
                    memcpy(data->macAddress, deviceinfor->deviceMacAddress, 6);
                    data->DapiVersion = SystemConfig_Get_Dapi_Version();	
                    ret = SUB_PROCESS_STATE_NEXT_SUBSTATE;
                }
                break;
                case 1:
                {
                    ret = SUB_PROCESS_STATE_NEXT_STATE;
                }
                break;
            }
        }
        break;
        case REMOTE_ASSET_MANAGEMENT_GET_LATEST_VERSION:
        {
            GetLatestVersionData *data =(GetLatestVersionData*)( buf + sizeof(RemoteHeader));
            switch(deviceinfor->currentSubState)
            {
                case 0:
                {                    
                    char tempbuf[10];
                    data->BrandID = machineConfig.brandId;
                    data->ConsoleType = machineConfig.consoleType;
                    data->Deployment = DEPLOYMENT_QA; 
                    memset( tempbuf , '\0', 10  );
                    sprintf(tempbuf, "%s.%s",rscu_version[0],rscu_version[1]);   
                    memcpy(data->CurrentVersion,tempbuf,strlen(tempbuf));
                    memcpy(data->MacAddress ,  deviceinfor->deviceMacAddress,6);
                    data->MachineType = machineConfig.machineType;
                    memcpy(data->MachineUUid,machineConfig.machineUUID,Settings.StringMaxLength);
                    data->ModelType = machineConfig.machineModel;
                    strcpy(data->SoftwareType,softwaretype[SOFTWARE_TYPE_NON_MCU]);
                    ret = SUB_PROCESS_STATE_NEXT_SUBSTATE;
                }
                break;
                case 1:
                {
                    ret = SUB_PROCESS_STATE_NEXT_STATE;
                }
                break;
            }
        }
        break;
        default:
            break;
    }

    return ret;
}


RemoteResponse RemoteAssetReceive(SubReceiveDataType datatype, ADDSTATESFUNC addstatefunc, DapiGlobalVariable* dapiGlobalVariable )
{
    RemoteResponse ret = REMOTE_RESPONSE_START;
    
    MACHINE_INFO machineConfig= SystemConfig_Get_MachineInfo( );  
    SystemSettingType Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_MACHINE_UUID );
    
    switch(datatype.command)
    {        
        case REMOTE_ASSET_MANAGEMENT_ACKNOWLEDGMENT:
        {            
            RemoteDataAck *ack =(RemoteDataAck*) datatype.data;
            switch(ack->messageType)
            {
                case REMOTE_ASSET_MANAGEMENT_MACHINE_INFO_EX:
                {
                }
                break;
                default:
                    break;
            }
        }
        break;
        case REMOTE_ASSET_MANAGEMENT_GET_NETWORK_TIME:
        {            
            DateTime_T now;
            TimeResponseRegion *response = (TimeResponseRegion*) datatype.data;            
            now.date = response->day;
            now.month = response->month;
            now.year = response->year- 2000;
            now.hours = response->hour;
            now.minutes = response->minute;
            now.seconds = response->second;
            RTC_Write_Time( &now );
        }
        break;
        case REMOTE_ASSET_MANAGEMENT_MACHINE_INFO_EX:
        {
            MachineInfoEXResponse *response = (MachineInfoEXResponse*) datatype.data;
            SystemSettingType settings =SystemConfig_Get_DefaultString(SETTING_DEFAULT_TIME_ZONE);
            if( strncmp( (char*)SystemConfig_Get_String(RTC_TIME_ZONE_STRING), response->timeZone, settings.StringMaxLength) != 0 )
            {
                addstatefunc( RCMD_ASSET_GET_NETWORK_TIME, 0, 0, 0 );
            }           
            memcpy(machineConfig.timezone , response->timeZone , settings.StringMaxLength );
            settings =SystemConfig_Get_DefaultString(SETTING_DEFAULT_MACHINE_UUID);
            memcpy(machineConfig.machineUUID, response->machineIdentification, settings.StringMaxLength );
            machineConfig.brandId = response->brandID;
            machineConfig.machineType = response->machineType;
            machineConfig.machineModel = response->modelType;
            machineConfig.driveType = response->driveType;
            machineConfig.consoleType = response->consoleType;
            machineConfig.csafeManufactureId = response->csafeManufacture;
            machineConfig.csafeId = response->csafeID;
            machineConfig.csafeModeId = response->csafeModelID;
            settings =SystemConfig_Get_DefaultString(SETTING_DEFAULT_SERIAL_NUMBER);
            memcpy(machineConfig.consoleSerialNumber, response->serialNumber, settings.StringMaxLength);
            settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_FRAME_NUMBER );
            memcpy( machineConfig.frameSerialNumber , response->frameSerialNumber, settings.StringMaxLength );
            machineConfig.clubId = response->clubID;
            machineConfig.minSpeedMphX10= response->minMPHX10;
            machineConfig.maxSpeedMphX10= response->maxMPHX10;
            machineConfig.minSpeedKphX10= response->minKPHX10;
            machineConfig.maxSpeedKphX10= response->maxKPHX10;
            machineConfig.maxInclineX10= response->maxInclineX10;
            machineConfig.minInclineX10= response->minInclineX10;
            machineConfig.minResistance = response->minResistanceLevel;
            machineConfig.maxResistance = response->maxResistanceLevel;
            machineConfig.maxUserNumber = response->maxUsers;
            machineConfig.isAutoLogin = response->autoLogin;
            machineConfig.headphoneInsertions = response->headphoneInsertions;
            machineConfig.headphoneRemovals = response->headphoneRemovals;
            if((response->accumulatedDistanceMilesX100 !=SystemConfig_Get_Long(ACCUMULATED_MILESX100_LONG))
                ||(response->accumulatedTime !=SystemConfig_Get_Long(ACCUMULATED_SECONDS_LONG))
                ||(response->lubeBeltMilesX10 != (SystemConfig_Get_Long( ACCUMULATED_LUBE_MILESX100_LONG )/10)))
            {
                UINT32 tempSaveData = response->accumulatedDistanceMilesX100;
                SystemConfigProcess(SVK_ADD_ACCUMULATED_MILESX100,&(tempSaveData),sizeof(tempSaveData));
                tempSaveData = response->accumulatedTime;
                SystemConfigProcess(SVK_ADD_ACCUMULATED_SECONDS , &tempSaveData,sizeof(tempSaveData));
                tempSaveData = response->lubeBeltMilesX10;
                SystemConfigProcess(SVK_ADD_ACCUMULATED_LUBE_MILESX100 , &tempSaveData,sizeof(tempSaveData));
                SystemConfigProcess(SVK_SAVE_ACCUMULATEDDATA2FLASH,NULL,0);
            }                        
            if(SystemConfig_Set_MachineInfo(machineConfig))
            {
                SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH, NULL,0);
            }
        }
        break;
        case REMOTE_ASSET_MANAGEMENT_MACHINE_INFO:
        {
            MachineInfoDataResponse *response = (MachineInfoDataResponse*) datatype.data;
            ACCUMULATED_DATA accumulaliteddata ;
            SystemSettingType settings =SystemConfig_Get_DefaultString(SETTING_DEFAULT_TIME_ZONE);
            if( strncmp( (char*)SystemConfig_Get_String(RTC_TIME_ZONE_STRING), response->timeZone, settings.StringMaxLength) != 0 )
            {
                addstatefunc( RCMD_ASSET_GET_NETWORK_TIME, 0, 0, 0 );
            }

            machineConfig.machineType = response->machineType;
            machineConfig.machineModel= response->modelType;
            machineConfig.csafeManufactureId = response->csafeManufacture;
            machineConfig.csafeId = response->csafeId;
            machineConfig.csafeModeId= response->csafeModelId;
            settings =SystemConfig_Get_DefaultString(SETTING_DEFAULT_SERIAL_NUMBER);
            memcpy( machineConfig.consoleSerialNumber, response->serialNumber, settings.StringMaxLength);
            settings =SystemConfig_Get_DefaultString(SETTING_DEFAULT_FRAME_NUMBER);
            memcpy( machineConfig.frameSerialNumber, response->frameSerialNumber, settings.StringMaxLength );
            settings =SystemConfig_Get_DefaultString(SETTING_DEFAULT_MACHINE_UUID);
            memcpy( machineConfig.machineUUID, response->machineIdentification, settings.StringMaxLength );
            machineConfig.clubId = response->clubId;
            machineConfig.minSpeedMphX10= response->minMPHX10;
            machineConfig.maxSpeedMphX10= response->maxMPHX10;
            machineConfig.minSpeedKphX10= response->minKPHX10;
            machineConfig.maxSpeedKphX10= response->maxKPHX10;
            machineConfig.maxInclineX10= response->maxInclineX10;
            machineConfig.minInclineX10= response->minInclineX10;
            accumulaliteddata.accumulatedDistanceMilesX100 = response->accumulatedDistanceMilesX100;
            accumulaliteddata.accumulatedTimeSeconds =  response->accumulatedSeconds;
            machineConfig.isAutoLogin = response->AutoLogin;
            settings = SystemConfig_Get_DefaultString(SETTING_DEFAULT_TIME_ZONE);
            memcpy( machineConfig.timezone,  response->timeZone, settings.StringMaxLength  );
            if(SystemConfig_Set_MachineInfo(machineConfig))
            {
                SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH, NULL,0);
            }
            if((accumulaliteddata.accumulatedDistanceMilesX100!=SystemConfig_Get_Long(ACCUMULATED_MILESX100_LONG))||(accumulaliteddata.accumulatedTimeSeconds!=SystemConfig_Get_Long(ACCUMULATED_SECONDS_LONG)))
            {
                SystemConfigProcess(SVK_ADD_ACCUMULATED_MILESX100,(void*)(accumulaliteddata.accumulatedDistanceMilesX100),sizeof(accumulaliteddata.accumulatedDistanceMilesX100));
                SystemConfigProcess(SVK_ADD_ACCUMULATED_SECONDS , (void*)(accumulaliteddata.accumulatedTimeSeconds),sizeof(accumulaliteddata.accumulatedTimeSeconds));
                SystemConfigProcess(SVK_SAVE_ACCUMULATEDDATA2FLASH,NULL,0);
            }
            
        }
        break;
        case REMOTE_ASSET_MANAGEMENT_MACHINE_REGISTRATION:
        {
            MachineRegisterResponse *response = (MachineRegisterResponse*)datatype.data;
            memcpy(machineConfig.machineUUID, response->machineIdentification, Settings.StringMaxLength);
            if(SystemConfig_Set_MachineInfo(machineConfig))
            {                        
                SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH, NULL,0);
            }
        }
        break;
        case REMOTE_ASSET_MANAGEMENT_GET_DAPI_LOCATION:
        {
            DAPILocalResponse *response =(DAPILocalResponse*) datatype.data;
            memcpy(dapiGlobalVariable->DapiInfo.Server, response->Server, 256);
            memcpy(dapiGlobalVariable->DapiInfo.Url, response->Url, 256);
            dapiGlobalVariable->DapiInfo.Port = response->Port;
            dapiGlobalVariable->DapiInfo.Secure = response->Secure;
            addstatefunc(REMOTE_WIFI_SET_MARIOBOX, 2, 0, (UCHAR*)&(dapiGlobalVariable->DapiInfo));
        }
        break;
        case REMOTE_ASSET_MANAGEMENT_GET_LATEST_VERSION:
        {
            ResponseLatestVersionData *response = (ResponseLatestVersionData*) datatype.data;
            MACHINE_INFO machineConfig=SystemConfig_Get_MachineInfo();
            memcpy(dapiGlobalVariable->dataInfor._softwareUpdate.SoftwareUrl, response->SoftwareUrl, 256);
            memcpy(dapiGlobalVariable->dataInfor._softwareUpdate.MD5, response->MD5, 32);
            addstatefunc(REMOTE_WIFI_SET_UPDATE_URL, 2, 0, (UCHAR*)&(dapiGlobalVariable->dataInfor._softwareUpdate));
        }
        break;
        default:
            break;
    }
    return ret;
}
#else
SubProcessState RemoteAssetProcess(RemoteDevicesInformation *deviceinfor, UINT8* buf, UINT16 *bufLength, const  DapiGlobalVariable* dapiGlobalVariable){return SUB_PROCESS_STATE_NOT_SUPPORTED;};
RemoteResponse RemoteAssetReceive(SubReceiveDataType datatype, ADDSTATESFUNC addstatefunc, DapiGlobalVariable* dapiGlobalVariable ){ return REMOTE_RESPONSE_START;};



#endif
