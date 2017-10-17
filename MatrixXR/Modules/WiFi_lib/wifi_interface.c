/*
* wifi_interface.c
*
*  Created on: May 2, 2013
*      Author: Brenden.Capps
*/

#include <wifi_interface.h>
#ifdef __WIFI_FUNC__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global_config.h"
#include "SystemConfig.h"
#include "Digital_wifi.h"
#include "UtilitiesRtc.h"
#include "MyStruct.h"
#include "CRC.h"
#include "wifi_event_handlers.h"
#include "system_config.h"
#include "wifi_api.h"
#include "programs.h"
#include  "SPI_Flash.h"
extern const char * timezone_string[];
extern const char* rscu_version[3];


#define LOCAL_WEB_TEST
#define WIFI_SYNC_WORD 0xAA55

#define MESSAGE	 "PASSPORT_2_RESPONSE;" 
#define MACHINE_NAME "XRT70;"
#define IP_ADDRESS "192.168.1.137;"
//#define PORT_NUMBER "30304"
#define PORT_NUMBER "30304;false"
char* response_message[4]={
    MESSAGE,//0
    MACHINE_NAME,//1
    IP_ADDRESS,//2
    PORT_NUMBER//3
};

static UINT16 wifi_response_index = 0;
static UINT16 wifi_next_response_index = 0;
static UINT8 wifi_num_pending_responses = 0;
static WifiResponseState wifi_response_state[TOTAL_RESPONSE_NUMBER] =
{0
};
static WifiCommand wifiResponse;
static int wifi_response_substate = 0;
static UINT16 transactionId;
static UINT16 dataTransactionId = 0;
//static UINT16 workout_data_rate[3] =
//	{ 100, 100, 100 };
static UINT16 workoutTrackingDataRate = 1;
static UINT8 workoutTrackingDataCount = 0;
//static UINT16 workoutPostTime = 0;
//static UINT8 _remoteControlPriority = 0;
static WIFI_REMOTE_MODE _remoteControlState = WIFI_IDLE;
static UINT8 _remoteControlIndex = 0;
static WORKOUT_STATE _workoutState = WIFI_WORKOUT_IDLE;
static UINT8 init_flag;


static WIFIUserLoginRequest _loginRequest;

typedef struct{
    UINT8 xidCheckResponse :2;
    UINT8 registrationCheckResponse:2;
    UINT8 loginCheckResponse:2;
    UINT8 DeleteUserCheckResponse:2;
    UINT8 userInfoUpdated:2;
    UINT8 RfidAssociateflag:2;
    UINT8 RfidDisassociateflag:2;
    UINT8 RfidLoginflag:2;
    UINT8 _feature:2;
}UserResponse;

typedef struct{
	UINT8 isOnLine:1;
	UINT8 needUpdateMachine:1;
    UINT8 isSendDapiLocation:1;
    UINT8 isSendMachineRegister:1;
    UINT8 isSendUpdateTime:1;
    UINT8 needCheckUpdate:1;
	UINT8 _fastrue:2;
}WifiFlag;

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

typedef struct{
    UINT8 userIndex;
    UINT8 rfidTag[16];
    RFIDCARRIER rfidCarrier;
}RFIDData;

static UserResponse _userDataResponse;
static RFIDData rfiddata;




__no_init static DAPI_ADDRESS_INFO DapiInfo;
__no_init static char _UserExternalID[36];
__no_init static WIFIDATAResponseLatestVersion _softwareUpdate;
__no_init static DateTime_T _workoutStartTime;
__no_init WIFI_STATUS_TYPE _wifiStatus;
__no_init WIFI_EVENT_HANDLE _wifiEventHandle;

UINT16 tcpDataBufferIndex[3] =
{ 0, 0, 0 };

char WiFiTestStage;
char WiFiTestingFlag;
//char WiFiReseted;
//char WiFiChecked;
//char WiFiCheckResult;
UINT16 WiFiTestDelay;

static bool init = false;
//static bool external_data_timer = false;
static WifiFlag gWifiFlag;
static UINT16 status_timer = 0;
//static UINT16 active_delay_timer = 0;
char tcpDataBuffer[3][sizeof(WifiCommand)];

typedef union{
    WIFIWorkoutData WorkdataTrack;
    WIFIWorkoutDataEx WorkdataTrackEx;
}WorkDataTrackUnion;

typedef union{
    WIFIWorkoutDataComplete wkDataComplete;
    WIFIWorkoutDataCompleteEx wkDataCompleteEx;
}WorkDataCompleteUnion;

static WorkDataTrackUnion workouData;
static WorkDataCompleteUnion wokoutCompleteData;
static WIFIWorkoutDataSprint8CompleteExtra completeSprint8Data;
static WIFIWorkoutDataFitnessTestCompleteExtra completeFitnessTestData;
DEPLOYMENT_TYPE remoteUpdateDeploymentType;
SOFTWARE_TYPE   remoteUpdateSoftwareType;





const char* softwaretype[SOFTWARE_TYPE_END]={
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

char* RfMachinetype[XR_AVAIABLE_MACHINES_TOTAL]={
    "XRTF30_",
    "XRTF50_",
    "XRT50_",
    "XRR30_",
    "XRU30_",
    "XRSE30_",
    "XRR50I_",
    "XRU50I_",
    "XRSE50I_",
    "XRA30_",
    "XRA50I_",
    "XRT70_C2_",
    "XRT70_C3_",
    "XRT70_C1_",
    "XRC50_",
    "XRT30_",
    "XRT75_C2_",
    "XRT75_C3_",
    "XRT75_C1_",
};
unsigned char RfMachineName[15];
unsigned char Pg2MacAddr[5];

#define MAX_TIMEZONE_NUMBER             78

__no_init static UINT16 g_TimeZone;
__no_init static bool g_hadKeyInput;
__no_init static FRAME_STAGE stage;


void AddWifiResponseState( UINT8 state, UINT16 command, UINT16 error, UINT8 port );


void Wifi_Clear_Status(void)
{
	_wifiStatus = WIFI_STATUS_IDLE;
}
UINT8 wifi_Get_Timezone(void)
{
    return g_TimeZone; 
}

#ifdef __WIFI_TEST_PETER__
UINT16 uartLostCounter,uartHadSent,recvResponse,uartSend502;
#endif

UINT8 User_logined()//lxl20160816
{
    char i=0;
    i= strlen(_UserExternalID);
    if(i>0)
        return 1;
    else
        return 0;
}

//VR_KEY
VR_KEY  Wifi_Set_VirtualKey(VR_KEY vk)
{
	VR_KEY TempKey = vk;
	VR_KEY realKey = vk;
	
	if(TempKey==VK_PROCESSED) return TempKey;
	
	if(_wifiStatus == WIFI_STATUS_TIMEZONE || _wifiStatus == WIFI_STATUS_AVALIABLE_UPDATE)
	{
		TempKey = VK_PROCESSED;
	}
    
	if( VK_WIFI_AP == realKey && _wifiStatus != WIFI_STATUS_TIMEZONE && _wifiStatus != WIFI_STATUS_AVALIABLE_UPDATE)
	{
		TempKey = VK_PROCESSED;
		_wifiStatus = WIFI_STATUS_TIMEZONE;
		_wifiEventHandle = WIFI_EVENT_INPUT_TIMEZONE;
	}
    
    if(_wifiStatus == WIFI_STATUS_TIMEZONE)
    {
	    switch(realKey)
	    {
        case VK_RESET:
	        {
                _wifiStatus = WIFI_STATUS_IDLE;
                _wifiEventHandle = WIFI_EVENT_TIMEZONE_COMPLETE;
                g_hadKeyInput = false;
	        }
	        break;
        case VK_ENTER:
        case VK_CHANGE_DISPLAY:
        case VK_ENTER_CHANGEDIS:
	        {
                MACHINE_INFO machineinfo = SystemConfig_Get_MachineInfo();
                memset( machineinfo.timezone, 0, 65 );
                g_TimeZone =g_TimeZone>MAX_TIMEZONE_NUMBER?MAX_TIMEZONE_NUMBER:g_TimeZone;
                strncpy( machineinfo.timezone, timezone_string[g_TimeZone-1],sizeof(machineinfo.timezone));
                if(SystemConfig_Set_MachineInfo(machineinfo))
                {
                    SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH, &machineinfo,sizeof(machineinfo));
                }
                RestoreWifiToFactoryDefaults();
                SystemConfig_Reset_User_Default(1);
                SystemConfig_Reset_User_Default(2);
                SystemConfig_Reset_User_Default(3);
                SystemConfig_Reset_User_Default(4);
	        }
	        break;
        case VK_NUM0:
        case VK_NUM1:
        case VK_NUM2:
        case VK_NUM3:
        case VK_NUM4:
        case VK_NUM5:
        case VK_NUM6:
        case VK_NUM7:
        case VK_NUM8:
        case VK_NUM9:
	        {
	            if(WIFI_STATUS_TIMEZONE != _wifiStatus )
	                break;
	            if(!g_hadKeyInput)
	            {
	                if(realKey == VK_NUM0)
	                    break;                
	                g_hadKeyInput = true;
	                g_TimeZone = ( realKey - VK_NUM0 );
	            }
	            else
	            {
	                g_TimeZone = g_TimeZone*10+( realKey - VK_NUM0 );
	                g_TimeZone = g_TimeZone%(100);
	                if(g_TimeZone == 0)
	                    g_TimeZone =1;
	            }
	            _wifiEventHandle = WIFI_EVENT_INPUT_TIMEZONE;
            }
	        break;
        case VK_INCLINEUP:
        case VK_SPEEDUP:	
        case VK_RESISTANCEUP:
        case VK_UP:
	        {
	            if(WIFI_STATUS_TIMEZONE != _wifiStatus )
	                break;
	            if(g_TimeZone == MAX_TIMEZONE_NUMBER )
	                g_TimeZone +=1;
	            g_TimeZone +=1;            
	            g_TimeZone = g_TimeZone%(MAX_TIMEZONE_NUMBER+1); 
	            _wifiEventHandle = WIFI_EVENT_INPUT_TIMEZONE;
	        }
	        break;
        case VK_INCLINEDOWN:
        case VK_SPEEDDOWN:		
        case VK_DOWN:
        case VK_RESISTANCEDOWN:
	        {
                if(WIFI_STATUS_TIMEZONE != _wifiStatus )
                    break;
	            if(g_TimeZone == 1 )
	            {
	                g_TimeZone = MAX_TIMEZONE_NUMBER;
	            }
	            else
	            {
	                g_TimeZone -=1;    
	            }
	            g_TimeZone = g_TimeZone%(MAX_TIMEZONE_NUMBER+1); 
	            _wifiEventHandle = WIFI_EVENT_INPUT_TIMEZONE;
	        }
	        break;
        default:
            break;
	    }
    }
	else if(_wifiStatus == WIFI_STATUS_AVALIABLE_UPDATE)
	{
		switch(realKey)
	    {
        case VK_BACK:
	        {
				WIFISendLeave_Programming_Mode();
				_wifiStatus = WIFI_STATUS_IDLE;
				_wifiEventHandle = WIFI_EVENT_UPDATE_CANCELED;
	        }
	        break;
        case VK_ENTER:
        case VK_CHANGE_DISPLAY:
        case VK_ENTER_CHANGEDIS:
	        {
#ifdef RF_MODULE_EXIST_   
                RF_Set_RF_Enable(0);
#endif
                SPI_Flash_Initial();
                SPI_Flash_Initial_Data();
                //EEROM_Initial();
                WIFIClearStateBuffer( );
                AddWifiResponseState( WCS_SET_UPDATE_URL, 0, 0, 0 );
                _wifiStatus = WIFI_STATUS_DOWNLOADING_SOFTWARE;
                _wifiEventHandle = WIFI_EVENT_UPDATE_PROGRESS;
                status_timer = Digital_WIFI_Get_10ms_Timer( );
			}
			break;
        default:break;
		}	
	}
    //	else if(_wifiStatus == WIFI_STATUS_DOWNLOADING_SOFTWARE)
    //	{
    //		if(realKey == VK_RESET)
    //        {
    //            #ifdef RF_MODULE_EXIST_   
    //                RF_Set_RF_Enable(1);
    //#endif
    //              _wifiStatus = WIFI_STATUS_IDLE;
    //              _wifiEventHandle = WIFI_EVENT_UPDATE_CANCELED;
    //				status_timer = Digital_WIFI_Get_10ms_Timer( );
    //        }
    //	}
	return TempKey;
}


void InitMessage( UINT8 lingoId, UINT8 messageId, UINT16 tranId, UINT16 dataSize )
{
	wifiResponse.header.syncWord = 0xAA55;
	wifiResponse.header.transactionId = tranId;//transactionId++;
	wifiResponse.header.dataSize = dataSize;
	wifiResponse.header.lingoId = lingoId;
	wifiResponse.header.messageId = messageId;
	wifiResponse.header.checksum = 0;
	if( dataSize > 0 )
		wifiResponse.header.checksum = GenerateCRC_CCITT( (UPtr8)&wifiResponse.data, dataSize );
    
}
void NextWiFiState()
{
	wifi_response_substate = 0;
	wifi_response_state[wifi_response_index].state = 0;
	wifi_response_index++;
	wifi_num_pending_responses--;
	if( wifi_response_index == TOTAL_RESPONSE_NUMBER )
		wifi_response_index = 0;
}

void AddWifiResponseState( UINT8 state, UINT16 command, UINT16 error, UINT8 port )
{
	if( wifi_num_pending_responses == TOTAL_RESPONSE_NUMBER )
	{
		if(state == WCS_WORKOUT_SNAPSHOT_EX )
		{
#ifdef __WIFI_TEST_PETER__
			uartLostCounter ++;
#endif
		}
		return;
	}
	
   if(state == WCS_GET_REMOTE_DATA )
   {
	char state_number=0;
	for(char i=0;i<TOTAL_RESPONSE_NUMBER;i++)
	{
		if(wifi_response_state[i].state == WCS_GET_REMOTE_DATA) state_number++;
		if(state_number>2)return;
	}
   }
	
	wifi_response_state[wifi_next_response_index].port = port;
	wifi_response_state[wifi_next_response_index].state = state;
	wifi_response_state[wifi_next_response_index].command = command;
	wifi_response_state[wifi_next_response_index++].error = error;
	wifi_num_pending_responses++;
	if( wifi_next_response_index == TOTAL_RESPONSE_NUMBER )
		wifi_next_response_index = 0;
}

static bool ResendSetupStep(UINT16 delay)
{
    if(delay < 3000)
        return false;
    WIFIClearStateBuffer();
    if( gWifiFlag.isSendDapiLocation )
    {
        AddWifiResponseState(WCS_GET_DAPI_LOCATION,0,0,0);
    }
    if(gWifiFlag.isSendMachineRegister)
    {
        AddWifiResponseState(WCS_REGISTER_MACHINE,0,0,0);
    }
    if(gWifiFlag.isSendUpdateTime)
    {
        AddWifiResponseState(WCS_UPDATE_TIME,0,0,0);
    }
    return true;
}


void ProcessHTTPPost( char* message, int length, UINT16 timer )
{
	if( length < sizeof(WifiHeader) + 3 )
		return;
    
    //	UINT16 statusCode = *((UINT16*)message);
	UINT8 dataType = message[2];
	UINT16 messageLocation = 3;
	WifiCommand* response;
    
	if( dataType != HTTP_APPLICATION_OCTET_STREAM )
		return;
    
	while( messageLocation < length - sizeof(WifiHeader) )
	{
		response = (WifiCommand*)( message + messageLocation );
		if( response->header.syncWord != SYNCWORD )
		{
			messageLocation++;
			continue;
		}
        
		messageLocation += response->header.dataSize + sizeof(WifiHeader);
		if( messageLocation > length )
			break;
        
		UINT16 checksum = GenerateCRC_CCITT( (UPtr8)&response->data, response->header.dataSize );
        
		if( response->header.checksum != checksum )
			continue;
        //         isAllowSendHttpReq = 1;
        MACHINE_INFO machineConfig=SystemConfig_Get_MachineInfo();
        
		switch( response->header.lingoId )
		{
		case WIFI_LINGO_WORKOUT_TRACKING://0x80
			switch( response->header.messageId )
			{
			case WIFI_WORKOUT_TRACKING_ACKNOWLEDGMENT:
				if( response->data.Ack.messageType == WIFI_WORKOUT_TRACKING_WORKOUT_COMPLETE )
				{  
					if( response->data.Ack.error != WIFI_ACK_OK )
					{
					}
				}
				if( response->data.Ack.messageType == WIFI_WORKOUT_TRACKING_DATA_EX)
				{
#ifdef __WIFI_TEST_PETER__
					recvResponse++;
#endif
				}
                
			}
			break;
		case WIFI_LINGO_ASSET_MANAGEMENT://0x81
			switch( response->header.messageId )
			{
			case WIFI_ASSET_MANAGEMENT_ACKNOWLEDGMENT:
                if( response->data.Ack.messageType  ==  WIFI_ASSET_MANAGEMENT_MACHINE_INFO_EX )
                {
                    gWifiFlag.isOnLine = 1;
                    AddWifiResponseState( WCS_GET_LATEST_VERSION, 0, 0, 0 );
                }
				break;
			case WIFI_ASSET_MANAGEMENT_MACHINE_INFO://0x01
                {         
                    ACCUMULATED_DATA accumulaliteddata ;
                    
                    
                    SystemSettingType settings =SystemConfig_Get_DefaultString(SETTING_DEFAULT_TIME_ZONE);
                    if( strncmp( (char*)SystemConfig_Get_String(RTC_TIME_ZONE_STRING), response->data.AssetManagementResponse.timeZone, settings.StringMaxLength) != 0 )
                        AddWifiResponseState( WCS_UPDATE_TIME, 0, 0, 0 );
                    
                    machineConfig.machineType = response->data.AssetManagementResponse.machineType;
                    machineConfig.machineModel= response->data.AssetManagementResponse.modelType;
                    machineConfig.csafeManufactureId = response->data.AssetManagementResponse.csafeManufacture;
                    machineConfig.csafeId = response->data.AssetManagementResponse.csafeId;
                    machineConfig.csafeModeId= response->data.AssetManagementResponse.csafeModelId;
                    settings =SystemConfig_Get_DefaultString(SETTING_DEFAULT_SERIAL_NUMBER);
                    memcpy( machineConfig.consoleSerialNumber, response->data.AssetManagementResponse.serialNumber, settings.StringMaxLength);
                    settings =SystemConfig_Get_DefaultString(SETTING_DEFAULT_FRAME_NUMBER);
                    memcpy( machineConfig.frameSerialNumber, response->data.AssetManagementResponse.frameSerialNumber, settings.StringMaxLength );
                    settings =SystemConfig_Get_DefaultString(SETTING_DEFAULT_MACHINE_UUID);
                    memcpy( machineConfig.machineUUID, response->data.AssetManagementResponse.machineIdentification, settings.StringMaxLength );
                    
                    machineConfig.clubId = response->data.AssetManagementResponse.clubId;
                    machineConfig.minSpeedMphX10= response->data.AssetManagementResponse.minMPHX10;
                    machineConfig.maxSpeedMphX10= response->data.AssetManagementResponse.maxMPHX10;
                    machineConfig.minSpeedKphX10= response->data.AssetManagementResponse.minKPHX10;
                    machineConfig.maxSpeedKphX10= response->data.AssetManagementResponse.maxKPHX10;
                    machineConfig.maxInclineX10= response->data.AssetManagementResponse.maxInclineX10;
                    machineConfig.minInclineX10= response->data.AssetManagementResponse.minInclineX10;
                    accumulaliteddata.accumulatedDistanceMilesX100 = response->data.AssetManagementResponse.accumulatedDistanceMilesX100;
                    accumulaliteddata.accumulatedTimeSeconds =  response->data.AssetManagementResponse.accumulatedSeconds;
                    machineConfig.isAutoLogin = response->data.AssetManagementResponse.AutoLogin;
                    settings = SystemConfig_Get_DefaultString(SETTING_DEFAULT_TIME_ZONE);
                    memcpy( machineConfig.timezone,  response->data.AssetManagementResponse.timeZone, settings.StringMaxLength  );
                    if(SystemConfig_Set_MachineInfo(machineConfig))
                    {
                        SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH, NULL,0);
                    }
#if 0
                    if((accumulaliteddata.accumulatedDistanceMilesX100!=SystemConfig_Get_Long(ACCUMULATED_MILESX100_LONG))||(accumulaliteddata.accumulatedTimeSeconds!=SystemConfig_Get_Long(ACCUMULATED_SECONDS_LONG)))
                    {
                        SystemConfigProcess(SVK_ADD_ACCUMULATED_MILESX100,&accumulaliteddata.accumulatedDistanceMilesX100,sizeof(accumulaliteddata.accumulatedDistanceMilesX100));
                        SystemConfigProcess(SVK_ADD_ACCUMULATED_SECONDS , &accumulaliteddata.accumulatedTimeSeconds,sizeof(accumulaliteddata.accumulatedTimeSeconds));
                        SystemConfigProcess(SVK_SAVE_ACCUMULATEDDATA2FLASH,NULL,0);
                    }
#endif
                }
				break;
			case WIFI_ASSET_MANAGEMENT_MACHINE_REGISTRATION://0x02
                {
                    
                    SystemSettingType settings = SystemConfig_Get_DefaultString(SETTING_DEFAULT_MACHINE_UUID);
                    machineConfig.isAPModeOn= 0;							
                    
					memcpy( machineConfig.machineUUID, response->data.RegisterResponse.machineIdentification, settings.StringMaxLength );
                    if(SystemConfig_Set_MachineInfo(machineConfig))
                    {
                        SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH, NULL,0);
                        gWifiFlag.needUpdateMachine = 1;
                    }
                    gWifiFlag.isSendMachineRegister = 0;
                    gWifiFlag.isSendUpdateTime = 1;
                    WIFIClearStateBuffer( );
                    AddWifiResponseState( WCS_UPDATE_TIME, 0, 0, 0 );
                    
                }
				break;
			case WIFI_ASSET_MANAGEMENT_GET_DAPI_LOCATION:
                {
                    
                    memset(&DapiInfo, 0, sizeof(DAPI_ADDRESS_INFO));
#ifdef __CLOSE_SSL__
					DapiInfo.Port = 80;
					DapiInfo.Secure = 0;                
#else
                    DapiInfo.Port = response->data.AMWIFIDAPIResponse.Port;
                    DapiInfo.Secure = response->data.AMWIFIDAPIResponse.Secure;   
#endif
                    gWifiFlag.isSendDapiLocation = 0;
                    SystemSettingType settings =SystemConfig_Get_DefaultString(SETTING_MARIOX_PRODUCTION_ADDRESS);
                    memcpy( DapiInfo.Server, response->data.AMWIFIDAPIResponse.Server, settings.StringMaxLength);
                    memcpy( DapiInfo.Url, response->data.AMWIFIDAPIResponse.Url, settings.StringMaxLength );		
                    WIFIClearStateBuffer( );
                    AddWifiResponseState( WCS_SET_MARIO_BOX, 0, 0, 0 );
                    machineConfig.isAPModeOn= 0;
                    //        if(SystemConfig_Set_MachineInfo(machineConfig))
                    //      {
                    //           SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH, NULL,0);
                    //      }
#ifdef OUTPUT_DEBUG
                    {
                        char buf[200]={0};
                        sprintf(buf,"%s:%d","dapi",Digital_WIFI_Get_10ms_Timer());
                        HAL_Monitor_Send_String(buf,sizeof(buf));
                    }
#endif   
                }
                break;
                
                /* For Dapi Version 10 start  */
                
            case WIFI_ASSET_MANAGEMENT_GET_LATEST_VERSION:
                {
                    memset(&_softwareUpdate,0,sizeof(WIFIDATAResponseLatestVersion));
                    
                    strcpy(_softwareUpdate.CurrentVersion,response->data.responseLatestVersion.CurrentVersion);
                    strcpy(_softwareUpdate.SoftwareType,response->data.responseLatestVersion.SoftwareType);
                    strcpy(_softwareUpdate.SoftwareUrl,response->data.responseLatestVersion.SoftwareUrl);     
                    strcpy(_softwareUpdate.MD5,response->data.responseLatestVersion.MD5); 
                    //                        char  Version[256];
                    //                        SystemConfig_Get_Version_Code(Version);
                    char tempbuf[256];
                    memset( tempbuf , '\0', 256  );
                    sprintf(tempbuf, "%s.%s",rscu_version[0],rscu_version[1]);
                    if(SystemConfig_Get_Char(RSCU_FLAG_UCHAR) == 0xA5)
                    {
                        if((strcmp(_softwareUpdate.CurrentVersion,tempbuf)!=0)&&(gWifiFlag.needCheckUpdate == 0) && SPI_Flash_Error()!=FLASH_ERROR)
                        {
                            //AddWifiResponseState( WCS_SET_UPDATE_URL, 0, 0, 0 );
                            if(WIFI_STATUS_IDLE == _wifiStatus )
                            {      
                                gWifiFlag.needCheckUpdate = 1;
                                _wifiEventHandle = WIFI_EVENT_AVALIABLE_UPDATE;
                                _wifiStatus = WIFI_STATUS_AVALIABLE_UPDATE;				
                            }
                            status_timer = Digital_WIFI_Get_10ms_Timer( );
                        }
                    }
                }
                break;
            case WIFI_ASSET_MANAGEMENT_MACHINE_INFO_EX:
                {
                    SystemSettingType settings =SystemConfig_Get_DefaultString(SETTING_DEFAULT_TIME_ZONE);
                    if( strncmp( (char*)SystemConfig_Get_String(RTC_TIME_ZONE_STRING), response->data.MachineInfoExResponse.timeZone, settings.StringMaxLength) != 0 )
                        AddWifiResponseState( WCS_UPDATE_TIME, 0, 0, 0 );
                    
                    if(!gWifiFlag.isOnLine)
                    {
                        AddWifiResponseState( WCS_GET_LATEST_VERSION, 0, 0, 0 );
                    }
                    gWifiFlag.isOnLine = 1;
                    
                    memcpy(machineConfig.timezone , response->data.MachineInfoExResponse.timeZone , settings.StringMaxLength );
                    settings =SystemConfig_Get_DefaultString(SETTING_DEFAULT_MACHINE_UUID);
                    memcpy(machineConfig.machineUUID, response->data.MachineInfoExResponse.machineIdentification, settings.StringMaxLength );
                    machineConfig.brandId = response->data.MachineInfoExResponse.brandID;
                    machineConfig.machineType = response->data.MachineInfoExResponse.machineType;
                    machineConfig.machineModel = response->data.MachineInfoExResponse.modelType;
                    machineConfig.driveType = response->data.MachineInfoExResponse.driveType;
                    machineConfig.consoleType = response->data.MachineInfoExResponse.consoleType;
                    machineConfig.csafeManufactureId = response->data.MachineInfoExResponse.csafeManufacture;
                    machineConfig.csafeId = response->data.MachineInfoExResponse.csafeID;
                    machineConfig.csafeModeId = response->data.MachineInfoExResponse.csafeModelID;
                    settings =SystemConfig_Get_DefaultString(SETTING_DEFAULT_SERIAL_NUMBER);
                    memcpy(machineConfig.consoleSerialNumber, response->data.MachineInfoExResponse.serialNumber, settings.StringMaxLength);
                    settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_FRAME_NUMBER );
                    memcpy( machineConfig.frameSerialNumber , response->data.MachineInfoExResponse.frameSerialNumber, settings.StringMaxLength );
                    machineConfig.clubId = response->data.MachineInfoExResponse.clubID;
                    machineConfig.minSpeedMphX10= response->data.MachineInfoExResponse.minMPHX10;
                    machineConfig.maxSpeedMphX10= response->data.MachineInfoExResponse.maxMPHX10;
                    machineConfig.minSpeedKphX10= response->data.MachineInfoExResponse.minKPHX10;
                    machineConfig.maxSpeedKphX10= response->data.MachineInfoExResponse.maxKPHX10;
                    machineConfig.maxInclineX10= response->data.MachineInfoExResponse.maxInclineX10;
                    machineConfig.minInclineX10= response->data.MachineInfoExResponse.minInclineX10;
                    machineConfig.minResistance = response->data.MachineInfoExResponse.minResistanceLevel;
                    machineConfig.maxResistance = response->data.MachineInfoExResponse.maxResistanceLevel;
                    machineConfig.maxUserNumber = response->data.MachineInfoExResponse.maxUsers;
                    machineConfig.isAutoLogin = response->data.MachineInfoExResponse.autoLogin;
                    machineConfig.headphoneInsertions = response->data.MachineInfoExResponse.headphoneInsertions;
                    machineConfig.headphoneRemovals = response->data.MachineInfoExResponse.headphoneRemovals;
#if 0
                    if((response->data.MachineInfoExResponse.accumulatedDistanceMilesX100 !=SystemConfig_Get_Long(ACCUMULATED_MILESX100_LONG))
                       ||(response->data.MachineInfoExResponse.accumulatedTime !=SystemConfig_Get_Long(ACCUMULATED_SECONDS_LONG))
                           ||(response->data.MachineInfoExResponse.lubeBeltMilesX10 != (SystemConfig_Get_Long( ACCUMULATED_LUBE_MILESX100_LONG )/10)))
                    {
                        UINT32 tempSaveData = response->data.MachineInfoExResponse.accumulatedDistanceMilesX100;
                        SystemConfigProcess(SVK_ADD_ACCUMULATED_MILESX100,&(tempSaveData),sizeof(tempSaveData));
                        tempSaveData = response->data.MachineInfoExResponse.accumulatedTime;
                        SystemConfigProcess(SVK_ADD_ACCUMULATED_SECONDS , &tempSaveData,sizeof(tempSaveData));
                        tempSaveData = response->data.MachineInfoExResponse.lubeBeltMilesX10;
                        SystemConfigProcess(SVK_ADD_ACCUMULATED_LUBE_MILESX100 , &tempSaveData,sizeof(tempSaveData));
                        SystemConfigProcess(SVK_SAVE_ACCUMULATEDDATA2FLASH,NULL,0);
                    }                        
#endif
                    if(SystemConfig_Set_MachineInfo(machineConfig))
                    {
                        SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH, NULL,0);
                    }
                }
                break;
                /* For Dapi Version 10 end  */
			}
			break;
		case WIFI_LINGO_NETWORK_TIME://0x82
			switch( response->header.messageId )
			{
			case WIFI_NETWORK_TIME_ACKNOWLEDGMENT:
				{
					if(response->data.Ack.messageType == WIFI_NETWORK_TIME_GET_TIME)
					{
                        gWifiFlag.isSendUpdateTime = 0;
					}
				}
				break;
			case WIFI_NETWORK_TIME_GET_TIME://0x01
                {
                    WIFIDataRegion region;
                    
                    SystemSettingType settings =SystemConfig_Get_DefaultString(SETTING_DEFAULT_TIME_ZONE);
                    region.year = response->data.TimeResponse.year;
                    region.month = response->data.TimeResponse.month;
                    region.day = response->data.TimeResponse.day;
                    region.hour = response->data.TimeResponse.hour;
                    region.minute = response->data.TimeResponse.minute;
                    region.second = response->data.TimeResponse.second;
                    region.units = machineConfig.unit;
                    region.timeFormat = machineConfig.is24HoursFormat;
                    //				memcpy( region.timeZone, machineConfig.timezone, settings.StringMaxLength );
                    gWifiFlag.isSendUpdateTime = 0;
					if(gWifiFlag.needUpdateMachine )
                    {
                        machineConfig.lastUpdateYear =response->data.TimeResponse.year;
                        machineConfig.lastUpdateMonth=response->data.TimeResponse.month;
                        machineConfig.lastUpdateDay=response->data.TimeResponse.day;
                        machineConfig.lastUpdateHour=response->data.TimeResponse.hour;
                        machineConfig.lastUpdateMinute=response->data.TimeResponse.minute;
                        machineConfig.lastUpdateSecond=response->data.TimeResponse.second;
                        gWifiFlag.needUpdateMachine =0;
                    }
                    if(SystemConfig_Set_MachineInfo(machineConfig))
                    {
                        SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH, NULL,0);
                    }
                    WIFIClearStateBuffer( );
                    AddWifiResponseState( WCS_UPDATE_MACHINE_INFO, 0, 0, 0 );
#ifdef __WIFI_NO_RTC__
                    SetNetworkDelay(0);
#endif
                    UpdateRTCTime( &region, 0);
                }
				break;
			}
			break;
		case WIFI_LINGO_USER_MANAGEMENT://0x83
			switch( response->header.messageId )
			{
			case WIFI_USER_MANAGEMENT_ACKNOWLEDGMENT:
                if(response->data.Ack.error == WIFI_ACK_OK)
                {
                    _wifiStatus = WIFI_STATUS_SYNC_SUCCESS;
                    _wifiEventHandle = WIFI_EVENT_SUCCESS;
                }
                else
                {
                    _wifiStatus = WIFI_STATUS_SYNC_FAIL;
                    _wifiEventHandle = WIFI_EVENT_FAIL;
                }
                switch( response->data.Ack.messageType )
                {
                case WIFI_USER_CHECK_XID:
                    _userDataResponse.xidCheckResponse = response->data.Ack.error == WIFI_ACK_OK ? WIFI_SUCCESS : WIFI_FAIL;
                    break;
                case WIFI_USER_LOGIN:                                    
                    _userDataResponse.loginCheckResponse = response->data.Ack.error == WIFI_ACK_OK ? WIFI_SUCCESS : WIFI_FAIL;
                    _wifiStatus = WIFI_STATUS_LOGIN_SUCCESS+_userDataResponse.loginCheckResponse-WIFI_SUCCESS;
                    break;
                case WIFI_USER_REGISTRATION:
                    _userDataResponse.registrationCheckResponse = response->data.Ack.error == WIFI_ACK_OK ? WIFI_SUCCESS : WIFI_FAIL;
                    break;
                case WIFI_USER_UNLINK_USER:
                    _userDataResponse.DeleteUserCheckResponse = response->data.Ack.error == WIFI_ACK_OK ? WIFI_SUCCESS : WIFI_FAIL;
                    _wifiStatus = WIFI_STATUS_DELETE_SUCCESS+_userDataResponse.DeleteUserCheckResponse-WIFI_SUCCESS;
                    break;
                case WIFI_USER_RFID_LOGIN:
                    _userDataResponse.RfidLoginflag= response->data.Ack.error == WIFI_ACK_OK ? WIFI_SUCCESS : WIFI_FAIL;
                    _wifiStatus = WIFI_STATUS_LOGIN_SUCCESS+_userDataResponse.RfidLoginflag-WIFI_SUCCESS;
                    break;
                case WIFI_USER_RFID_ASSOCIATE:
                    _userDataResponse.RfidAssociateflag= response->data.Ack.error == WIFI_ACK_OK ? WIFI_SUCCESS : WIFI_FAIL;                                    
                    _wifiStatus = WIFI_STATUS_RFID_ASSOCIATE_SUCCESS+_userDataResponse.RfidAssociateflag-WIFI_SUCCESS;                                    
                    break;
                case WIFI_USER_RFID_DISASSOCIATE:
                    _userDataResponse.RfidDisassociateflag= response->data.Ack.error == WIFI_ACK_OK ? WIFI_SUCCESS : WIFI_FAIL;                                    
                    _wifiStatus = WIFI_STATUS_RFID_DISASSOCIATE_SUCCESS+_userDataResponse.RfidDisassociateflag-WIFI_SUCCESS;                                    
                    break;
                }
                
                status_timer = Digital_WIFI_Get_10ms_Timer( );
				break;
            case WIFI_USER_RFID_LOGIN:
                {
                    if( response->data.rfidLoginResp.userID <= SystemConfig_Get_MaxUserNumber( ))
                    {
                        UINT8 userId = response->data.rfidLoginResp.userID;
                        USERS_INFO userinfo = SystemConfig_Get_UserInfo(userId);
                        memcpy( _UserExternalID,  response->data.rfidLoginResp.externalUserID,  36 );
                        userinfo.gender = response->data.rfidLoginResp.gender;
                        userinfo.weightStandardX100 = response->data.rfidLoginResp.userWeightX10*10;
                        userinfo.age = response->data.rfidLoginResp.userAge;
                        userinfo.year = response->data.rfidLoginResp.userBirthYear;
                        userinfo.month = response->data.rfidLoginResp.userBirthMonth;
                        userinfo.date= response->data.rfidLoginResp.userBirthDay;
                        userinfo.languageId= response->data.rfidLoginResp.userLanguageId;
                        userinfo.displayUnit= response->data.rfidLoginResp.displayUnits;
                        userinfo.isAcceptedTerms= response->data.rfidLoginResp.acceptedTerms;
                        userinfo.isShareWorkouts= response->data.rfidLoginResp.shareWorkoutData;
                        memset( userinfo.name, 0, 65 );
                        strncpy( userinfo.name, response->data.rfidLoginResp.userName, 65 );
                        for(UINT8 i = 1; i<= SystemConfig_Get_MaxUserNumber(); i++ )
                        {
                            USERS_INFO Tempuserinfo = SystemConfig_Get_UserInfo( i );
                            Tempuserinfo.isLoggedOn = 0;
                            if((SystemConfig_Set_UserInfo(Tempuserinfo,i)))   
                            {
                                SystemConfigProcess(SVK_SAVE_USER2FLASH, &i,sizeof(i));
                            }
                        }
                        userinfo.isLoggedOn = 1;
                        //machineConfig.unit = userinfo.displayUnit;
                        machineConfig.lastLoggedUser = userId;
                        _userDataResponse.loginCheckResponse = WIFI_SUCCESS;
                        _userDataResponse.RfidLoginflag = WIFI_SUCCESS;
                        _wifiStatus= WIFI_STATUS_LOGIN_SUCCESS;
                        _wifiEventHandle = WIFI_EVENT_SUCCESS;
                        status_timer = Digital_WIFI_Get_10ms_Timer( );
                        if((SystemConfig_Set_UserInfo(userinfo,userId)))
                        {
                            SystemConfigProcess(SVK_SAVE_USER2FLASH, &userId,sizeof(userId));
                        }
                        if(SystemConfig_Set_MachineInfo(machineConfig))
                        {
                            SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH, NULL,0);
                        }
                    }
                    else
                        _userDataResponse.RfidLoginflag= WIFI_FAIL;
                }
                break;
			case WIFI_SYNC_USER_INFO://0x01
				if(  response->data.UserDataResponse.userConsoleId <= SystemConfig_Get_MaxUserNumber() )
				{
                    
                    UINT8 userId = response->data.UserDataResponse.userConsoleId;
                    USERS_INFO userinfo = SystemConfig_Get_UserInfo(userId);
					if( response->data.UserDataResponse.UserSetup == 0 )
					{
                        if( strncmp( userinfo.xid, DEFAULTXID, XIDMAXLENGTH ) != 0 )
					 	{
					 		if(SystemConfig_Reset_User_Default(userId))
                            {                                
                                SystemConfigProcess(SVK_SAVE_USER2FLASH, &userId, sizeof(userId));
                            }
					 	}
					}
					else
					{
						userinfo.isSetup= response->data.UserDataResponse.UserSetup;
						memcpy( userinfo.xid, response->data.UserDataResponse.xid, XIDMAXLENGTH );
						memcpy( userinfo.passcode, response->data.UserDataResponse.passcode, 4 );
						userinfo.weightStandardX100= response->data.UserDataResponse.weightX10*10+5;
						userinfo.age = response->data.UserDataResponse.age;
						userinfo.year = response->data.UserDataResponse.year;
						userinfo.month = response->data.UserDataResponse.month;
						userinfo.date= response->data.UserDataResponse.day;
						userinfo.gender = response->data.UserDataResponse.gender;
						userinfo.languageId= response->data.UserDataResponse.language;
						userinfo.displayUnit= response->data.UserDataResponse.DisplayUnits;
						userinfo.isAcceptedTerms= response->data.UserDataResponse.AcceptedTerms;
						userinfo.isShareWorkouts= response->data.UserDataResponse.WorkoutDataShare;
						memset( userinfo.name, 0, 65 );
						strncpy( userinfo.name, response->data.UserDataResponse.name, 65 );
						if(userinfo.isLoggedOn)
						{
							//machineConfig.unit= userinfo.displayUnit;
						}
                        _userDataResponse.userInfoUpdated = 1;
                        if(SystemConfig_Set_UserInfo(userinfo,userId))
                        { 
                            SystemConfigProcess(SVK_SAVE_USER2FLASH, &userId, sizeof(userId));
                        }
						if(SystemConfig_Set_MachineInfo(machineConfig))
	                    {                        
	                        SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH, NULL,0);
	                    }						
					}
                    _wifiStatus= WIFI_STATUS_SYNC_SUCCESS;
                    _wifiEventHandle = WIFI_EVENT_SUCCESS;
                    status_timer = Digital_WIFI_Get_10ms_Timer( );                    
				}
				break;
			case WIFI_USER_LOGIN://0x02
				
				if(response->data.UserLoginResponse.userId <= SystemConfig_Get_MaxUserNumber() )
				{
					UINT8 userId = response->data.UserLoginResponse.userId;		
                    USERS_INFO userinfo = SystemConfig_Get_UserInfo(userId);                    
                    SystemSettingType settings =SystemConfig_Get_DefaultString(SETTING_DEFAULT_MACHINE_UUID);
					memcpy( _UserExternalID,  response->data.UserLoginResponse.externalUserId,  36 );
					memcpy(userinfo.xid, _loginRequest.xid, XIDMAXLENGTH );
					memcpy( userinfo.passcode, _loginRequest.passcode, 4 );
					userinfo.weightStandardX100 = response->data.UserLoginResponse.weightX10*10+5;
					userinfo.age = response->data.UserLoginResponse.age;
					userinfo.year = response->data.UserLoginResponse.year;
					userinfo.month = response->data.UserLoginResponse.month;
					userinfo.date = response->data.UserLoginResponse.day;
					userinfo.gender = response->data.UserLoginResponse.gender;
					userinfo.languageId= response->data.UserLoginResponse.language;
					userinfo.displayUnit = response->data.UserLoginResponse.DisplayUnits;
					userinfo.isAcceptedTerms = 0;
					userinfo.isSetup= 1;
					for(UINT8 i = 1; i<= SystemConfig_Get_MaxUserNumber(); i++ )
                    {
                        USERS_INFO Tempuserinfo = SystemConfig_Get_UserInfo( i );
                        Tempuserinfo.isLoggedOn = 0;
                        if((SystemConfig_Set_UserInfo(Tempuserinfo,i)))   
                        {
                            SystemConfigProcess(SVK_SAVE_USER2FLASH, &i,sizeof(i));
                        }
                    }
                    //userinfo,&FLASH_DATA.fields._userInfo[i].isLoggedOn = 0 ;
                    userinfo.isLoggedOn = 1;
					//machineConfig.unit = userinfo.displayUnit;
                    machineConfig.lastLoggedUser = userId;
					memset( userinfo.name, 0, 65 );
					strncpy( userinfo.name, response->data.UserLoginResponse.userName, 64 );
					_userDataResponse.loginCheckResponse = 1;
                    _wifiStatus= WIFI_STATUS_LOGIN_SUCCESS;
                    _wifiEventHandle = WIFI_EVENT_SUCCESS;
                    status_timer = Digital_WIFI_Get_10ms_Timer( );                    
                    if(SystemConfig_Set_UserInfo(userinfo,userId))
                    {
                        SystemConfigProcess(SVK_SAVE_USER2FLASH, &userId,sizeof(userId));                        
                    }
                    if(SystemConfig_Set_MachineInfo(machineConfig))
                    {                        
                        SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH, NULL,0);
                    }
				}
                
				break;
			case WIFI_USER_REGISTRATION://0x03
				_userDataResponse.registrationCheckResponse = 1;
				break;
                
			}
			break;
		}
	}
    
}

void WifiConnectionStatusChange( bool wifiConnected )
{
	if( wifiConnected )
	{
		if(gWifiFlag.isOnLine  == 0)
        {
            AddWifiResponseState( WCS_GET_WIFI_INFO, 0, 0, 0 );
        }
	}
	else
	{
		//gWifiFlag.isOnLine;
		memset( &wifi_settings.status, 0, sizeof(WIFI_STATUS) );
		memset( &wifi_settings.ip_address, 0, 6 );
#if 0
		memset( &wifi_settings.mac_address, 0, 6 );
		memset( &wifi_settings.security, 0, sizeof(WIFI_SECURITY_SETTINGS) );
		memset( &wifi_settings.ap_security, 0, sizeof(WIFI_SECURITY_SETTINGS) );
		memset( &wifi_settings.tcp_settings, 0, 3 * sizeof(WIFI_TCP_SETTINGS) );
#endif
		//memset( &wifi_settings.data_server, 0, 256 );        
		gWifiFlag.isOnLine = 0;
        _wifiStatus = WIFI_STATUS_OFFLINE;
        _wifiEventHandle = WIFI_EVENT_OFFLINE;
        status_timer = Digital_WIFI_Get_10ms_Timer( );
		wifi_settings.udp_port = 0;
	}
}

UCHAR Wifi_Get_EnggineTestResult(void)
{
    if(!WiFiTestingFlag) return 0;
    switch(WiFiTestStage)
    {
    case 1://delay for reset wifi
        {
            if(WiFiTestDelay>=WIFI_TEST_DELAY_FOR_RESET)
            {
                //ResetWiFi();
		        WiFiTestStage = 2;
            }
        } 
        break;
    case 2://delay 40s for check result
        {
            if(WiFiTestDelay>=WIFI_TEST_DELAY_FOR_RESULT)
            {
                WiFiTestStage = 3; 
            }
        }
        break;
    case 3://check OK result && wait for time out 
        {
            if(WiFiTestDelay>=WIFI_TEST_DELAY_FOR_TIMEOUT)
            {
                if( wifi_settings.mac_address[0] == 0 )
                {
                    _wifiEventHandle = WIFI_EVENT_NO_MAC;
                    _wifiStatus = WIFI_STATUS_NO_MAC ;
                }
                else if( wifi_settings.ip_address[0] == 0 )
                {
                    _wifiEventHandle = WIFI_EVENT_NO_IP;
                    _wifiStatus = WIFI_STATUS_NO_IP ;
                } 
                WiFiTestStage = 4; 
            }
            if( wifi_settings.ip_address[0] != 0 )
            {
                _wifiEventHandle = WIFI_EVENT_WIFI_IS_OK;
		        _wifiStatus = WIFI_STATUS_TEST_SUCCESS ;
                WiFiTestStage = 4; 
            }
        }
        break;
    case 4://RestoreWifiToFactoryDefaults
        {
            RestoreWifiToFactoryDefaults();
            WiFiTestStage = 5;
        }
        break;
    default:
    case 5:
        break;
    }
    return 0;
}
//--------------------------//
//--ProcessExternalMessage--//
//--------------------------//
void ProcessExternalMessage( int interface, WifiCommand* message )//decode message
{
    //	if( wifi_num_pending_responses == 10 )
    //		return;
    
    MACHINE_INFO machineConfig=SystemConfig_Get_MachineInfo();
#if 1
	switch( message->header.lingoId )
	{
	case WIFI_LINGO_MODULE_SETTINGS://WIFI_LINGO_MODULE_SETTINGS
        {
            switch( message->header.messageId )
            {
            case WIFI_GET_WIFI_SETTINGS:
                AddWifiResponseState( WCS_GET_WIFI_SETTINGS, 0, 0, interface );
                break;
            case WIFI_CONNECT_TO_AP:
                memset( wifi_settings.security.ssid, 0, 33 );
                strncpy( wifi_settings.security.ssid, message->data.APConnection.ssid, 32 );
                wifi_settings.security.security_type = WIFI_SECURITY_AUTO_DETECT;
                memset( wifi_settings.security.security_key, 0, 256 );
                memcpy( wifi_settings.security.security_key, message->data.APConnection.securityKey, 255 );
                AddWifiResponseState( WCS_CONNECT_TO_AP, 0, 0, interface );
                break;
            }
            break;
        }
        
	case WIFI_LINGO_GENERAL://WIFI_LINGO_GENERAL
        {
            switch( message->header.messageId )
            {
            case WIFI_GET_LINGO_VERSION://01
                {
                    AddWifiResponseState( WCS_GET_LINGO_INFO, message->data.UByte, 0, interface );
                    break;
                }
            case WIFI_GET_MACHINE_INFO:
                {
                    AddWifiResponseState( WCS_GET_MACHINE_INFO, 0, 0, interface );
                    break;
                }
            case WIFI_SET_SPEED_CALIBRATION://03
                {
                    if( _workoutState == WIFI_WORKOUT_RUNNING || _workoutState == WIFI_WORKOUT_PAUSED )
                    {
                        AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_GENERAL << 8 ) | WIFI_SET_SPEED_CALIBRATION, WIFI_ACK_INVALID_COMMAND_DURING_WORKOUT, interface );
                    }
                    else if( message->data.SpeedCalibration.maxMPHx10 > machineConfig.maxRpmMphX10
                            || message->data.SpeedCalibration.maxKPHx10 > machineConfig.maxRpmKphX10
                                || message->data.SpeedCalibration.minMPHx10 < machineConfig.minRpmMphX10
                                    || message->data.SpeedCalibration.minKPHx10 < machineConfig.minRpmKphX10
                                        || message->data.SpeedCalibration.maxInclinex10 > machineConfig.maxInclineX10
                                            || message->data.SpeedCalibration.minInclinex10 < machineConfig.minInclineX10
                                                || message->data.SpeedCalibration.maxADC > machineConfig.maxInclineADC
                                                    || message->data.SpeedCalibration.rpmKPHHigh > machineConfig.maxRpmKphX10
                                                        || message->data.SpeedCalibration.rpmMPHHigh > machineConfig.maxRpmMphX10
                                                            || message->data.SpeedCalibration.rpmMPHLow >= message->data.SpeedCalibration.rpmMPHHigh
                                                                || message->data.SpeedCalibration.rpmKPHLow >= message->data.SpeedCalibration.rpmKPHHigh )
                    {
                        AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_GENERAL << 8 ) | WIFI_SET_SPEED_CALIBRATION, WIFI_ACK_SPEED_CALIBRATION_OUT_OF_RANGE, interface );
                        
                    }
                    else
                    {
                        //				machineConfig->maxMPH_RPM = message->data.SpeedCalibration.rpmMPHHigh;
                        //				machineConfig->minMPH_RPM = message->data.SpeedCalibration.rpmMPHLow;
                        //				machineConfig->maxKPH_RPM = message->data.SpeedCalibration.rpmKPHHigh;
                        //				machineConfig->minKPH_RPM = message->data.SpeedCalibration.rpmKPHLow;
                        //				machineConfig->maxInclineADC = message->data.SpeedCalibration.maxADC;
                        //				machineConfig->minInclineADC = message->data.SpeedCalibration.minADC;
                        machineConfig.maxSpeedMphX10 = message->data.SpeedCalibration.maxMPHx10;
                        machineConfig.minSpeedMphX10 = message->data.SpeedCalibration.minMPHx10;
                        machineConfig.maxSpeedKphX10 = message->data.SpeedCalibration.maxKPHx10;
                        machineConfig.minSpeedKphX10 = message->data.SpeedCalibration.minKPHx10;
                        machineConfig.maxInclineX10 = message->data.SpeedCalibration.maxInclinex10;
                        machineConfig.minInclineX10 = message->data.SpeedCalibration.minInclinex10;
                        if(SystemConfig_Set_MachineInfo(machineConfig))
                        {
                            SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH, NULL,0);
                        }            
                        AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_GENERAL << 8 ) | WIFI_SET_SPEED_CALIBRATION, WIFI_ACK_OK, interface );
                    }
                    break;
                }
            case WIFI_GET_SPEED_CALIBRATION://04
                {
                    AddWifiResponseState( WCS_GET_SPEED_CALIBRATION, 0, 1, interface );
                    break;
                }
            case WIFI_SET_PWM_CAL:
                {
                    if( _workoutState == WIFI_WORKOUT_RUNNING || _workoutState == WIFI_WORKOUT_PAUSED )
                    {
                        AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_GENERAL << 8 ) | WIFI_SET_PWM_CAL, WIFI_ACK_INVALID_COMMAND_DURING_WORKOUT, interface );
                    }
                    else /*if( Information_GetDriveType() == 0 )
                    {
                        UpdatePWMCalibration( message->data.PWMCal.calPoint );
                        AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_GENERAL << 8 ) | WIFI_SET_PWM_CAL, WIFI_ACK_OK, interface );
                }
			else*/
                        AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_GENERAL << 8 ) | WIFI_SET_PWM_CAL, WIFI_ACK_NO_PWM_RESISTANCE, interface );
                    break;
                }
            case WIFI_GET_PWM_CAL:
                {
                    AddWifiResponseState( WCS_GET_PWM_CALIBRATION, 0, 1, interface );
                    break;
                }
            case WIFI_SET_ECB_CAL:
                {
                    if( _workoutState == WIFI_WORKOUT_RUNNING || _workoutState == WIFI_WORKOUT_PAUSED )
                    {
                        AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_GENERAL << 8 ) | WIFI_SET_ECB_CAL, WIFI_ACK_INVALID_COMMAND_DURING_WORKOUT, interface );
                    }
                    else/* if( Information_GetDriveType() == 1 )
                    {
                        for( int i = 0; i < 30; i++ )
                    {
                        machineConfig->ecbCal[i] = message->data.ECBCal.ecbLevel[i];
                }
                        SAVE_MACHINE_CONFIG_TO_FLASH
                        AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_GENERAL << 8 ) | WIFI_SET_ECB_CAL, WIFI_ACK_OK, interface );
                }
			else*/
                        AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_GENERAL << 8 ) | WIFI_SET_PWM_CAL, WIFI_ACK_NO_ECB_RESISTANCE, interface );
                    break;
                }
            case WIFI_GET_ECB_CAL:
                {
                    AddWifiResponseState( WCS_GET_ECB_CALIBRATION, 0, 1, interface );
                    break;
                }
            case WIFI_SET_WATTS_TABLE:
                {
                    if( _workoutState == WIFI_WORKOUT_RUNNING || _workoutState == WIFI_WORKOUT_PAUSED )
                    {
                        AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_GENERAL << 8 ) | WIFI_SET_WATTS_TABLE, WIFI_ACK_INVALID_COMMAND_DURING_WORKOUT, interface );
                    }
                    else if( message->data.WattsTable.level < 1 || message->data.WattsTable.level > 30 )
                    {
                        AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_GENERAL << 8 ) | WIFI_SET_WATTS_TABLE, WIFI_ACK_INVALID_RESISTANCE, interface );
                    }
                    else if( machineConfig.machineType == 1 )
                        AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_GENERAL << 8 ) | WIFI_SET_PWM_CAL, WIFI_ACK_NO_WATTS_TABLE, interface );
                    else
                    {
                        AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_GENERAL << 8 ) | WIFI_SET_MACHINE_STATE, WIFI_ACK_OK, interface );
                    }
                    break;
                }
            case WIFI_GET_WATTS_TABLE://0A
                {
                    AddWifiResponseState( WCS_GET_WATTS_TABLE, 0, 0, interface );
                    break;
                }
            case WIFI_SET_DATA_SERVER:
                {
                    if( _workoutState == WIFI_WORKOUT_RUNNING || _workoutState == WIFI_WORKOUT_PAUSED )
                        AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_GENERAL << 8 ) | WIFI_SET_DATA_SERVER, WIFI_ACK_INVALID_COMMAND_DURING_WORKOUT, interface );
                    else
                    {
                        //				memset( machineConfig->MarioBox, 0, 256 );
                        //				strncpy( machineConfig->MarioBox, message->data.DataServer.server, 255 );
                        if(SystemConfig_Set_MachineInfo(machineConfig ))
                        {
                            SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH, NULL,0);
                        }
                        AddWifiResponseState( WCS_SET_DATA_SERVER, 0, 0, interface );
                    }
                    break;
                }
            case WIFI_GET_DATA_SERVER:
                {
                    AddWifiResponseState( WCS_GET_DATA_SERVER, 0, 0, interface );
                    break;
                }
            case WIFI_SET_MACHINE_STATE://0x0D
                {
                    if( _workoutState == WIFI_WORKOUT_RUNNING || _workoutState == WIFI_WORKOUT_PAUSED )
                    	{
                        AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_GENERAL << 8 ) | WIFI_SET_MACHINE_STATE, WIFI_ACK_INVALID_COMMAND_DURING_WORKOUT, interface );
                    //			else if( _remoteControlPriority >= message->data.MachineStateRequest.priority && _remoteControlState != WIFI_IDLE
                    //                    && _remoteControlIndex != interface )
                    //				AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_GENERAL << 8 ) | WIFI_SET_PWM_CAL, WIFI_ACK_ANOTHER_DEVICE_HAS_CONTROL, interface );
                    	}
					else
                    {
                        if( GrantRemoteAccess() )
                        {
                            _remoteControlIndex = interface;
                            _remoteControlState = (WIFI_REMOTE_MODE)message->data.MachineStateRequest.state;
                            //					_remoteControlPriority = message->data.MachineStateRequest.priority;
                            
                            AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_GENERAL << 8 ) | WIFI_SET_MACHINE_STATE, WIFI_ACK_OK, interface );
                        }
                        else
                            AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_GENERAL << 8 ) | WIFI_SET_MACHINE_STATE, WIFI_ACK_SET_MACHINE_STATE_REJECTED, interface );
                        
                    }
                    break;
                }
            case WIFI_GET_MACHINE_STATE:
                {
                    AddWifiResponseState( WCS_GET_MACHINE_STATE, 0, 0, interface );
                    break;
                }
            case WIFI_SET_USER_DATA://0x0F
                {
                    if( _workoutState == WIFI_WORKOUT_RUNNING || _workoutState == WIFI_WORKOUT_PAUSED )
                        AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_GENERAL << 8 ) | WIFI_SET_USER_DATA, WIFI_ACK_INVALID_COMMAND_DURING_WORKOUT, interface );
                    else if( message->data.UserData.userConsoleId > /*MAX_USERS*/4 )
                    {
                        AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_GENERAL << 8 ) | WIFI_SET_USER_DATA, WIFI_ACK_INVALID_USER_ID, interface );
                    }
                    else
                    {             
                        //                        USERS_INFO userinfo= SystemConfig_Get_UserInfo(message->data.UserData.userConsoleId);
                        //                        memcpy( userinfo.xid, message->data.UserData.xid, XIDMAXLENGTH );
                        //                        memcpy( userinfo.passcode, message->data.UserData.passcode, 4 );
                        //                        //				userinfo.weightX10 = message->data.UserData.weightX10;
                        //                        userinfo.age = message->data.UserData.age;
                        //                        userinfo.year = message->data.UserData.year;
                        //                        userinfo.month = message->data.UserData.month;
                        //                        //				userinfo.day = message->data.UserData.day;
                        //                        userinfo.gender = message->data.UserData.gender;
                        //                        //				userinfo.language = message->data.UserData.langauge;
                        //                        
                        //                        memset( userinfo.name, 0, 65 );
                        //                        memcpy( userinfo.name, message->data.UserData.name, 64 );
                        
                        //				if( SystemConfig_Set_UserInfo(userinfo,UCHAR message->data.UserData.userConsoleId))
                        //                {
                        //                    SystemConfigProcess(SVK_SAVE_USER2FLASH, &message->data.UserData.userConsoleId, sizeof(UCHAR));
                        //                }
                        AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_GENERAL << 8 ) | WIFI_SET_MACHINE_STATE, WIFI_ACK_OK, interface );
                    }
                    break;
                }
            case WIFI_GET_USER_DATA://0x10
                {
                    // if( message->data.UByte >= 0 && message->data.UByte <= /*MAX_USERS*/4 )
                    AddWifiResponseState( WCS_GET_USER_DATA, message->data.UByte, WIFI_ACK_OK, interface );
                    //else
                    //      AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_GENERAL << 8 ) | WIFI_GET_USER_DATA, WIFI_ACK_INVALID_USER_ID, interface );
                    break;
                }
            case WIFI_SET_WORKOUT_DATA_RATE://0x11
                {
                    //			workout_data_rate[interface - 1] = message->data.UWord * 100;
                    AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_GENERAL << 8 ) | WIFI_SET_WORKOUT_DATA_RATE, WIFI_ACK_OK, interface );
                    break;
                }
            case WIFI_GET_WORKOUT_DATA:
                {
                    AddWifiResponseState( WCS_GET_WORKOUT_DATA, transactionId, 0, interface );
                    break;
                }
            case WIFI_GET_CURRENT_USER://0x13
                {
                    AddWifiResponseState( WCS_GET_CURRENT_USER, 0, 0, interface );
                    break;
                }
            case WIFI_STOP_WORKOUT://0x14
                {
                    AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_REMOTE << 8 ) | WIFI_STOP_WORKOUT, StopWorkout(), interface );
                    // HAL_Reset_System(); 
					break;
                }
            case WIFI_CHECK_FOR_UPDATE:
                {
                    AddWifiResponseState( WCS_CHECK_FOR_UPDATE, 0, 0, interface );
                    break;
                }
            case WIFI_GET_MACHINE_INFO_PASSPORT://0x16
                {
                    AddWifiResponseState( WCS_GET_MACHINE_INFO_PASSPORT, 0, 0, interface );
                    break;
                }
            case WIFI_GET_WORKOUT_DATA_PASSPORT://0x17
                {
                    AddWifiResponseState( WCS_GET_WORKOUT_DATA_PASSPORT, 0, 0, interface );
                    break;
                }
            }
            break;
        }
    case WIFI_LINGO_REMOTE://WIFI_LINGO_REMOTE
        {
            //            if( _remoteControlState != WIFI_REMOTE_CONTROL || _remoteControlIndex != interface )
            //            {
            //                AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_REMOTE << 8 ) | message->header.messageId, WIFI_ACK_NOT_IN_REMOTE_CONTROL, interface );
            //                break;
            //            }
            switch( message->header.messageId )
            {
            case WIFI_SET_CURRENT_USER:
                {
                    if( _workoutState == WIFI_WORKOUT_RUNNING || _workoutState == WIFI_WORKOUT_PAUSED )
                        AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_REMOTE << 8 ) | WIFI_SET_USER_DATA, WIFI_ACK_INVALID_COMMAND_DURING_WORKOUT, interface );
                    else if(/* message->data.UByte >= 0 &&*/ message->data.UByte < /*MAX_USERS*/4 )
                    {
                        //OnChangedUser( message->data.UByte );
                        USERS_INFO userinfo = SystemConfig_Get_UserInfo( message->data.UByte );
                        Login( message->data.UByte, (UCHAR*)userinfo.xid, (UCHAR*)userinfo.passcode );
                        AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_REMOTE << 8 ) | WIFI_SET_CURRENT_USER, WIFI_ACK_OK, interface );
                    }
                    else
                        AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_REMOTE << 8 ) | WIFI_SET_CURRENT_USER, WIFI_ACK_INVALID_USER_ID, interface );
                    break;
                }
            case WIFI_START://02
                {
                    AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_REMOTE << 8 ) | WIFI_START, StartWorkout( &message->data.ProgramStart ), interface );
                    break;
                }
            case WIFI_PAUSE://03
                {
                    AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_REMOTE << 8 ) | WIFI_PAUSE, PauseWorkout(), interface );
                    break;
                }
            case WIFI_RESTART://04
                {
                    AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_REMOTE << 8 ) | WIFI_RESTART, RestartWorkout(), interface );
                    break;
                }
            case WIFI_SET_SPEED://05
                {
                    AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_REMOTE << 8 ) | WIFI_SET_SPEED,
                                         SetSpeed( message->data.SetSpeed.speedX10, message->data.SetSpeed.metric ), interface );
                    break;
                }
            case WIFI_SET_INCLINE://06
                {
                    AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_REMOTE << 8 ) | WIFI_SET_INCLINE, SetIncline( message->data.UWord ), interface );
                    break;
                }
            case WIFI_SET_RESISTANCE://07
                {
                    AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_REMOTE << 8 ) | WIFI_SET_RESISTANCE, SetResistance( message->data.UByte ), interface );
                    break;
                }
            case WIFI_SET_RAW_ECB:
                {
                    AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_REMOTE << 8 ) | WIFI_SET_RAW_ECB, SetECB( message->data.UWord ), interface );
                    break;
                }
            case WIFI_SET_RAW_PWM:
                {
                    AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_REMOTE << 8 ) | WIFI_SET_RAW_PWM, SetPWM( message->data.UWord ), interface );
                    break;
                }
                //            case WIFI_CHANGE_PROGRAM:
                //                {
                //                    AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_REMOTE << 8 ) | WIFI_CHANGE_PROGRAM, ChangeProgram( &message->data.ProgramStart ), interface );
                //                    break;
                //                }
            case WIFI_KEY_PRESS:
                {
                    AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_REMOTE << 8 ) | WIFI_KEY_PRESS, PressKey( message->data.UByte ), interface );
                    break;
                }
            case WIFI_SELECT_PROGRAM:
                {
                    AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_REMOTE << 8 ) | WIFI_SELECT_PROGRAM, ProgramSelected( message->data.UWord ), interface );
                    break;
                }
            case WIFI_SETUP_WORKOUT_PASSPORT: // 0x12
                {
                    AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_REMOTE << 8 ) | WIFI_SETUP_WORKOUT_PASSPORT, Setup_Workout_Passport( message->data.UWord ), interface );
                    break;
                }
            case WIFI_SET_GOAL_PASSPORT: // 0x13
                {
                    AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_REMOTE << 8 ) | WIFI_SET_GOAL_PASSPORT, Set_Goal_Passport( message->data.UWord ), interface );
                    break;
                }
            }
            break;
        }
    case WIFI_LINGO_SETTINGS://WIFI_LINGO_SETTINGS
        {
            if( _remoteControlState != WIFI_SETTINGS_MODE || _remoteControlIndex != interface )
                AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_SETTINGS << 8 ) | message->header.messageId, WIFI_ACK_NOT_IN_SETTINGS_MODE, interface );
            else if( _workoutState == WIFI_WORKOUT_RUNNING || _workoutState == WIFI_WORKOUT_PAUSED )
                AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_SETTINGS << 8 ) | message->header.messageId, WIFI_ACK_INVALID_COMMAND_DURING_WORKOUT, interface );
            else
            {
                switch( message->header.messageId )
                {
                case WIFI_SET_DEFAULTS:
                    {
                        machineConfig.maxSpeedMphX10 = message->data.Defaults.maxMPHX10;
                        machineConfig.minSpeedMphX10 = message->data.Defaults.minMPHX10;
                        machineConfig.maxSpeedKphX10 = message->data.Defaults.maxKPHX10;
                        machineConfig.minSpeedKphX10 = message->data.Defaults.minKPHX10;
                        machineConfig.maxInclineX10 = message->data.Defaults.maxInclineX10;
                        machineConfig.minInclineX10 = message->data.Defaults.minInclineX10;
                        //				machineConfig->maxWorkoutTimeMinutes = message->data.Defaults.maxWorkoutTime;
                        //				machineConfig->defaultWorkoutTimeMinutes = message->data.Defaults.defaultWorkoutTime;
                        //				machineConfig->defaultAge = message->data.Defaults.defaultAge;
                        //				machineConfig->defaultWeightX10 = message->data.Defaults.defaultWeightX10;
                        //				machineConfig->defaultWarmupTimeMinutes = message->data.Defaults.defaultWarmupTime;
                        //				machineConfig->defaultCooldownTimeMinutes = message->data.Defaults.defaultCooldownTime;
                        //				machineConfig->pauseTimeMinutes = message->data.Defaults.pauseTime;
                        //				machineConfig->gender = message->data.Defaults.gender;
                        //				machineConfig->volume = message->data.Defaults.volume;
                        //				machineConfig->brightness = message->data.Defaults.brightness;
                        //				machineConfig->targetHR = message->data.Defaults.targetHR;
                        //				machineConfig->constantWatts = message->data.Defaults.constantWatts;
                        //				machineConfig->energySaver = message->data.Defaults.energySaver;
                        //				machineConfig->languageId = message->data.Defaults.languageId;
                        //				machineConfig->units = message->data.Defaults.units;
                        
                        //				machineConfig->demoMode = message->data.Defaults.demoMode;
                        //				machineConfig.timeFormat = message->data.Defaults.timeFormat;
                        if(SystemConfig_Set_MachineInfo(machineConfig))
                        {
                            SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH, NULL,0);
                        }            
                        AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_SETTINGS << 8 ) | WIFI_LINGO_SETTINGS, WIFI_ACK_OK, interface );
                        break;
                    }
                case WIFI_GET_DEFAULTS:
                    {
                        AddWifiResponseState( WCS_GET_DEFAULTS, 0, 0, interface );
                        break;
                    }
                case WIFI_SET_MACHINE_TYPE:
                    {
                        machineConfig.machineType = message->data.MachineType.machineType;
                        machineConfig.machineModel = message->data.MachineType.modelType;
                        machineConfig.csafeId = message->data.MachineType.CSAFEManufacture;
                        machineConfig.csafeManufactureId = message->data.MachineType.CSAFEId;
                        machineConfig.csafeModeId = message->data.MachineType.CSAFEModelId;
                        memcpy( machineConfig.consoleSerialNumber, message->data.MachineType.ConsoleSerialNumber, 15 );
                        memcpy( machineConfig.frameSerialNumber, message->data.MachineType.FrameSerialNumber, 15 );
                        AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_SETTINGS << 8 ) | WIFI_SET_MACHINE_TYPE, WIFI_ACK_OK, interface );
                        if(SystemConfig_Set_MachineInfo(machineConfig))
                        {
                            SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH, NULL,0);
                        }     
                        break;
                    }
                case WIFI_GET_MACHINE_TYPE:
                    {
                        AddWifiResponseState( WCS_GET_MACHINE_TYPE, 0, 0, interface );
                        break;
                    }
                case WIFI_SET_REGION:
                    {
                        AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_SETTINGS << 8 ) | WIFI_SET_REGION, WIFI_ACK_OK,
                                             
                                             UpdateRTCTime( &message->data.Region, 0 ) ? 0 : WIFI_ACK_INVALID_TIME_SET );
                        
                        break;
                    }
                case WIFI_GET_REGION:
                    {
                        AddWifiResponseState( WCS_GET_REGION, 0, 0, interface );
                        break;
                    }
                }
            }
            break;
        }
    case WIFI_LINGO_TEST://WIFI_LINGO_TEST
        {
            if( _remoteControlState != WIFI_TEST_MODE || _remoteControlIndex != interface )
                AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_SETTINGS << 8 ) | message->header.messageId, WIFI_ACK_NOT_IN_TEST_MODE, interface );
            else if( _workoutState == WIFI_WORKOUT_RUNNING || _workoutState == WIFI_WORKOUT_PAUSED )
                AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_SETTINGS << 8 ) | message->header.messageId, WIFI_ACK_INVALID_COMMAND_DURING_WORKOUT, interface );
            else
            {
                switch( message->header.messageId )
                {
                case WIFI_SET_INCLINE_ADC:
                    AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_REMOTE << 8 ) | WIFI_SET_INCLINE_ADC, SetInclineADC( message->data.UWord ), interface );
                    break;
                case WIFI_SET_RPM:
                    AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_REMOTE << 8 ) | WIFI_SET_RPM, SetRPM( message->data.UWord ), interface );
                    break;
                case WIFI_SET_ECB:
                    AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_REMOTE << 8 ) | WIFI_SET_ECB, SetECB( message->data.UWord ), interface );
                    break;
                case WIFI_SET_PWM:
                    AddWifiResponseState( WCS_ACK, ( WIFI_LINGO_REMOTE << 8 ) | WIFI_SET_PWM, SetPWM( message->data.UWord ), interface );
                    break;
                    
                case WIFI_BEEP:
                    
                    AddWifiResponseState( WCS_ACK, 0, 0, interface );
                    break;
                }
            }
            break;
        }
	}
#endif
}



int AddTCPDataToBuffer( int buffer, char* data, int data_size )
{
	if( data_size + tcpDataBufferIndex[buffer] > sizeof(WifiCommand) )
	{
		memcpy( tcpDataBuffer[buffer] + tcpDataBufferIndex[buffer], data, sizeof(WifiCommand) - tcpDataBufferIndex[buffer] );
		return sizeof(WifiCommand) - tcpDataBufferIndex[buffer];
	}
	else
		memcpy( tcpDataBuffer[buffer] + tcpDataBufferIndex[buffer], data, data_size );
	return data_size;
}
void wifi_ResetInit(void)
{
    init = false;
    wifi_response_substate = 0;
    memset( &DapiInfo , 0, sizeof(DAPI_ADDRESS_INFO) );
    _wifiStatus = WIFI_STATUS_IDLE;
    gWifiFlag.isOnLine = 0;
    gWifiFlag.needUpdateMachine = 0;
    gWifiFlag.needCheckUpdate = 0;
    init_flag=0;
}
void wifi_Poweron_Initial_Data(void)
{
	MACHINE_INFO machineinfo = SystemConfig_Get_MachineInfo();
	int i;
	memset( _UserExternalID, 0, 36 );
	g_hadKeyInput = false;
	for(i=0;i<MAX_TIMEZONE_NUMBER;i++)
	{
		if(strcmp(machineinfo.timezone,timezone_string[i])==0)
            break;
	}
	if(i == MAX_TIMEZONE_NUMBER )
        i = 22;
	if( SystemConfig_Get_Char( AP_MODE_FLAG_UCHAR ))
    {
        _wifiStatus  = WIFI_STATUS_AP_MODE;
        _wifiEventHandle = WIFI_EVENT_AP_MODE;
    }
	g_TimeZone = i+1; 
    
	WiFiTestingFlag = 0;
	WiFiTestStage = 0 ;
	WiFiTestDelay = 0;
}


void ProcessWifiStatus(UINT16 timer)
{
	UINT16 TempDelay =TIME_ACTIVE_DELAY  ;
	if((_wifiStatus==WIFI_STATUS_LOGIN )||( _wifiStatus==WIFI_STATUS_WAITTING)||(_wifiStatus == WIFI_STATUS_SYNC_USER))
	{
		TempDelay = TIME_OUT_DELAY;
	}    
    if(( SystemConfig_Get_Char( AP_MODE_FLAG_UCHAR ) &&( _wifiStatus!=WIFI_STATUS_ACTIVATED))\
        || _wifiStatus==WIFI_STATUS_TIMEZONE ||  _wifiStatus== WIFI_STATUS_AVALIABLE_UPDATE||_wifiStatus == WIFI_STATUS_DOWNLOADING_SOFTWARE)
    {
        return ;
    }
    
    if(_wifiStatus != WIFI_STATUS_IDLE && _wifiStatus != WIFI_STATUS_TIMEZONE)
    {
        if( (TempDelay + status_timer ) < timer)
        {
            if(TempDelay == TIME_OUT_DELAY)
            {
                _wifiStatus = WIFI_STATUS_TIMEOUT;
                _wifiEventHandle = WIFI_EVENT_TIMEOUT;
                status_timer = Digital_WIFI_Get_10ms_Timer( );
            }
            else
            {
            	if(_wifiStatus == WIFI_STATUS_ACTIVATED)
				{
					_wifiEventHandle = WIFI_EVENT_TIMEZONE_COMPLETE;
				}
				else
				{
					_wifiEventHandle = WIFI_EVENT_IDLE;
				}
                _wifiStatus = WIFI_STATUS_IDLE;
            }
        }
        
    }
}
#ifdef __WIFI_TEST_PETER__
#define __HIGH_SPEED__
#endif

UINT16 UserSlot;
UINT16 UserSlot_backup;
UCHAR UserName_backup[30]={0};
UINT8  tcp_connected_backup;
UINT16 time_out = 0;
void RunWiFiInterface( UINT16 timer )//RunWiFiInterface
{
	static UINT16 data_timer = 0;     
	static UINT16  WaitForWIFIWriteFlash = 0;
    static UINT16  delayTimer=0;
    static UINT16  delayHeart=0;
	
    if(timer < delayTimer+ 8)
    {
        UINT16 temp = timer - delayTimer;
        if (temp>8)
        {
            delayTimer = timer;
        }
        return;
    }
	
//	if(wifi_settings.status.tcp_connected[1] == 1)
//	{
//		time_out++;
//		if(time_out> 100)
//		{
//		//init = false;
//		time_out = 0;/*HAL_Reset_System();*/
//		}
//	}
//
//	if((wifi_settings.status.tcp_connected[1] == 0)&&(tcp_connected_backup == 1))
//	{
//		//init = false;
//		//time_out = 0;
//	}
	tcp_connected_backup  =  wifi_settings.status.tcp_connected[1] ;
	
    delayTimer = timer;
    //bool updateDataTransaction = false;
    MACHINE_INFO machineConfig=SystemConfig_Get_MachineInfo( );
    ACCUMULATED_DATA accumulalteddata;
    accumulalteddata.accumulatedDistanceMilesX100 = SystemConfig_Get_Long(ACCUMULATED_MILESX100_LONG);
    accumulalteddata.accumulatedLubeMilesX100 = SystemConfig_Get_Long(ACCUMULATED_LUBE_MILESX100_LONG);
    accumulalteddata.accumulatedTimeSeconds = SystemConfig_Get_Long(ACCUMULATED_SECONDS_LONG);
    
    ProcessWifiStatus( timer );
    
    
    
	if( !init )
	{
		WaitForWIFIWriteFlash = 0;
        memset( &_userDataResponse , 0 , sizeof(_userDataResponse));
		init = true;
		AddWifiResponseState( WCS_INIT_WIFI, 0, 0, 0 );//by ckm
//                if((gWifiFlag.isOnLine==1)||(wifi_settings.status.tcp_connection==1)||(wifi_settings.status.wifi_connected==1))
//                  AddWifiResponseState( WCS_GET_WIFI_INFO, 0, 0, 0 );//lxl20170809
	}
    
	if(wifi_settings.ip_address[0] !=0 )
	{
        
        if(((gWifiFlag.isSendDapiLocation)
            ||(gWifiFlag.isSendMachineRegister)
                ||(gWifiFlag.isSendUpdateTime))
           &&(gWifiFlag.isOnLine == 0))
        {
            if(ResendSetupStep(timer - WaitForWIFIWriteFlash))
            {
                WaitForWIFIWriteFlash = timer;
            }
        }
        
        
		if( !HAL_WIFI_Get_INT())
		{
            AddWifiResponseState( WCS_GET_REMOTE_DATA, 0, 0, 0 ); 
		}
	}
    
#if 1
    if( timer < data_timer ) data_timer = 0 ;
     
    if( timer - data_timer >= 100 * workoutTrackingDataRate )
    {
        if( _workoutState == WIFI_WORKOUT_RUNNING || _workoutState == WIFI_WORKOUT_PAUSED )
        {
            AddWifiResponseState( WCS_GET_WORKOUT_DATA_PASSPORT, dataTransactionId, 0, 1 );  //0x02 0x17                    
            dataTransactionId++;
        }
        else//setup interface
        {
            UserSlot  =    User_Get_UserSlot();
            USERS_INFO userinfo = SystemConfig_Get_UserInfo( User_Get_UserSlot());
//            UCHAR UserNameLen = 
            if((memcmp(userinfo.name,UserName_backup,strlen(userinfo.name)))||((UserSlot !=UserSlot_backup)))
            {
                Set_g_SetUser_InfoSync(0x00);
                AddWifiResponseState( WCS_GET_USER_DATA, dataTransactionId, 0, 1 );  //0x02 0x10                    
                dataTransactionId++;
            }            
//            if((UserSlot !=UserSlot_backup)||(Get_g_SetUser_InfoSync()==0x55))
//            {
//                Set_g_SetUser_InfoSync(0x00);
//                AddWifiResponseState( WCS_GET_USER_DATA, dataTransactionId, 0, 1 );  //0x02 0x10                    
//                dataTransactionId++;
//            }
            else//heart 
            {
                if(delayHeart > 3)
                {
                    delayHeart = 0;
                    AddWifiResponseState( WCS_IDLE, dataTransactionId, 0, 1 );  //0x01 0x01
                }
                delayHeart++;
            }
            UserSlot_backup  =  UserSlot;
            strncpy(UserName_backup,userinfo.name,strlen(userinfo.name));
        }
        data_timer = timer;
    }
#else
	if( WIFIIsOnline( ) && (User_Get_UserSlot() != 0))//Guest do not send workout data
	{
		if( timer < data_timer ) data_timer = 0 ;
        
		if( timer - data_timer >= 100 * workoutTrackingDataRate )
		{
			if( _workoutState == WIFI_WORKOUT_RUNNING || _workoutState == WIFI_WORKOUT_PAUSED )
			{
			    if(SystemConfig_Get_Dapi_Version( ) >9)
                {
                    AddWifiResponseState( WCS_WORKOUT_SNAPSHOT_EX, dataTransactionId, 0, 0 );
                }
                else
                {
			        AddWifiResponseState( WCS_POST_WORKOUT_DATA, dataTransactionId, 0, 0 );
                }                        
                dataTransactionId++;
                //updateDataTransaction = true;                
                // if( updateDataTransaction )
			}
			data_timer = timer;
		}
        
	}
#endif
	switch( wifi_response_state[wifi_response_index].state )
	{
    case WCS_IDLE:
		switch( wifi_response_substate )
		{
		case 0:
			wifi_response_substate++;
			memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
			/*
			if( _remoteControlIndex == wifi_response_index )
				wifiResponse.data.UByte = _remoteControlState;
			else
				wifiResponse.data.UByte = WIFI_IDLE;
			InitMessage( WIFI_LINGO_GENERAL, WIFI_GET_MACHINE_STATE, transactionId, sizeof(UINT8) );
			*/
			break;
		case 1://0x0027
		char buff[4] = {0};
		       buff[0] = 0x01;buff[1] = 0x01;buff[2] = 0x01;buff[3] = 0x01;
			if( SendSetTCPData( wifi_response_state[wifi_response_index].port, /*(char*)&wifiResponse*/(char*)&buff, /*sizeof(WifiHeader) + wifiResponse.header.dataSize*/sizeof("1234") ) )
				wifi_response_substate++;
			break;
		case 2:
			NextWiFiState();
			break;
		}
		break;
		
        /* For Dapi Version 10 Start */
        /*
    case WCS_OCCURRED_ERROR_CODE:
        switch( wifi_response_substate )
        {
    case 0:
        SystemSettingType Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_MACHINE_UUID );
        wifi_response_substate++;
        memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
        memcpy(wifiResponse.data.ErrorCodeOccurred.machineIdentification , machineConfig.machineUUID , Settings.StringMaxLength );
        memcpy( wifiResponse.data.ErrorCodeOccurred.MacAddress , wifi_settings.mac_address , 6 );
        break;
    }
        break;
        */
    case WCS_UPDATE_MACHINE_STATS:
        switch( wifi_response_substate )
        {
        case 0:
            {
                SystemSettingType Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_MACHINE_UUID );
                wifi_response_substate++;
                memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
                memcpy(wifiResponse.data.MachineStats.machineIdentification , machineConfig.machineUUID , Settings.StringMaxLength );
                memcpy( wifiResponse.data.MachineStats.MacAddress , wifi_settings.mac_address , 6 );
                wifiResponse.data.MachineStats.accumulatedDistanceMilesX100 = SystemConfig_Get_Long( ACCUMULATED_MILESX100_LONG );
                wifiResponse.data.MachineStats.accumulatedTime = SystemConfig_Get_Long( ACCUMULATED_SECONDS_LONG );
                wifiResponse.data.MachineStats.motorTemperature = 0;
                wifiResponse.data.MachineStats.busCurrent = 0;
                wifiResponse.data.MachineStats.busVoltage = 0;
                wifiResponse.data.MachineStats.lubeBeltMilesX10 = SystemConfig_Get_Long( ACCUMULATED_LUBE_MILESX100_LONG );
                wifiResponse.data.MachineStats.headphoneInsertions = machineConfig.headphoneInsertions;
                wifiResponse.data.MachineStats.headphoneRemovals = machineConfig.headphoneRemovals;
                InitMessage( WIFI_LINGO_ASSET_MANAGEMENT, WIFI_ASSET_MANAGEMENT_UPDATE_MACHINE_STATE, wifi_response_state[wifi_response_index].command,
                            sizeof(WIFIUpdateMachineStats) );
            }
#ifndef __HIGH_SPEED__
            break;
#endif
        case 1:
            if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
                wifi_response_substate++;
            break;
        case 2:
            if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)DapiInfo.Url, strlen( DapiInfo.Url ), DapiInfo.Secure ) )
                NextWiFiState();
            break;
        }
        break;
    case WCS_RFID_DISASSOCIATE:
        switch( wifi_response_substate )
        {
        case 0:
            {
                SystemSettingType Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_MACHINE_UUID );
                _userDataResponse.RfidDisassociateflag = 2;
                wifi_response_substate++;
                memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
                memcpy(wifiResponse.data.rfidDisassociateReq.MachineID , machineConfig.machineUUID , Settings.StringMaxLength);
                memcpy(wifiResponse.data.rfidDisassociateReq.macAddress, wifi_settings.mac_address, 6 );
                memcpy(wifiResponse.data.rfidDisassociateReq.RFIDTag, rfiddata.rfidTag , 16);
                wifiResponse.data.rfidDisassociateReq.RFIDCarrier = rfiddata.rfidCarrier;
                InitMessage( WIFI_LINGO_USER_MANAGEMENT, WIFI_USER_RFID_DISASSOCIATE, wifi_response_state[wifi_response_index].command,
                            sizeof(WIFIRFidDisassociateReq) );
            }
#ifndef __HIGH_SPEED__
            break;
#endif
        case 1://0x0500
            if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
                wifi_response_substate++;
            break;
        case 2:
            if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)DapiInfo.Url, strlen( DapiInfo.Url ), DapiInfo.Secure ) )
                NextWiFiState();
            break;
        }
        break;    
    case WCS_RFID_ASSOCIATE:
        switch( wifi_response_substate )
        {
        case 0:
            {
                SystemSettingType Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_MACHINE_UUID );
                _userDataResponse.RfidAssociateflag = 2;
                wifi_response_substate++;
                memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
                memcpy(wifiResponse.data.rfidAssociateReq.externalUserID, _UserExternalID, 36);
                memcpy(wifiResponse.data.rfidAssociateReq.MachineID , machineConfig.machineUUID , Settings.StringMaxLength);
                memcpy(wifiResponse.data.rfidAssociateReq.macAddress, wifi_settings.mac_address, 6 );
                wifiResponse.data.rfidAssociateReq.userID = rfiddata.userIndex;
                memcpy(wifiResponse.data.rfidAssociateReq.RFIDTag, rfiddata.rfidTag , 16);
                wifiResponse.data.rfidAssociateReq.RFIDCarrier = rfiddata.rfidCarrier;
                InitMessage( WIFI_LINGO_USER_MANAGEMENT, WIFI_USER_RFID_ASSOCIATE, wifi_response_state[wifi_response_index].command,
                            sizeof(WIFIRFidAssociateReq) );
            }
#ifndef __HIGH_SPEED__
            break;
#endif
        case 1://0x0500
            if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
                wifi_response_substate++;
            break;
        case 2:
            if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)DapiInfo.Url, strlen( DapiInfo.Url ), DapiInfo.Secure ) )
                NextWiFiState();
            break;
        }
        break;
    case WCS_RFID_LOGIN:
        switch(wifi_response_substate)
        {
        case 0:
            {
                SystemSettingType Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_MACHINE_UUID );
                _userDataResponse.RfidLoginflag = 2;
                wifi_response_substate++;
                memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
                memcpy(wifiResponse.data.rfidLoginReq.MachineID , machineConfig.machineUUID , Settings.StringMaxLength);
                memcpy(wifiResponse.data.rfidLoginReq.macAddress, wifi_settings.mac_address, 6 );
                wifiResponse.data.rfidLoginReq.userID = rfiddata.userIndex;
                memcpy(wifiResponse.data.rfidLoginReq.RFIDTag, rfiddata.rfidTag , 16);
                wifiResponse.data.rfidLoginReq.RFIDCarrier = rfiddata.rfidCarrier;
                InitMessage( WIFI_LINGO_USER_MANAGEMENT, WIFI_USER_RFID_LOGIN, wifi_response_state[wifi_response_index].command,
                            sizeof(WIFIRFidLoginReq) );
            }
#ifndef __HIGH_SPEED__
            break;
#endif
        case 1://0x0500
            if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
                wifi_response_substate++;
            break;
        case 2:
            if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)DapiInfo.Url, strlen( DapiInfo.Url ), DapiInfo.Secure ) )
                NextWiFiState();
            break;
        }
        break;
        
    case WCS_WORKOUT_COMPLETE_EX:
        switch( wifi_response_substate )
        {
        case 0:
            
            wifi_response_substate++;
            memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
            
            memcpy(&wifiResponse.data.WorkoutDataEx, &(workouData.WorkdataTrackEx), sizeof(WIFIWorkoutDataEx));
            InitMessage( WIFI_LINGO_WORKOUT_TRACKING, WIFI_WORKOUT_TRACKING_DATA_EX, wifi_response_state[wifi_response_index].command,
                        sizeof(WIFIWorkoutDataEx) );
#ifndef __HIGH_SPEED__
            break;
#endif
        case 1://0x0500
            if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
            {
#ifdef __WIFI_TEST_PETER__
                uartHadSent++;
#endif
                wifi_response_substate++;
            }
            break;
        case 2:
            memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
            memcpy(&wifiResponse.data.WorkoutTrackingDataCompleteEx,&wokoutCompleteData.wkDataCompleteEx,sizeof(WIFIWorkoutDataCompleteEx));
            InitMessage( WIFI_LINGO_WORKOUT_TRACKING, WIFI_WORKOUT_TRACKING_COMPLETE_EX, transactionId, sizeof(WIFIWorkoutDataCompleteEx) );
            wifi_response_substate++;
#ifndef __HIGH_SPEED__
            break;
#endif
        case 3://0x0500
            if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
            {
#ifdef __WIFI_TEST_PETER__
                uartHadSent++;
#endif
                wifi_response_substate++;
            }
            break;
        case 4:
            if(wokoutCompleteData.wkDataCompleteEx.programType == PS_SPRINT8)
            {
                memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
                memcpy(&wifiResponse.data.WorkoutSprint8CompleteExtra,&completeSprint8Data,sizeof(WIFIWorkoutDataSprint8CompleteExtra));
                InitMessage( WIFI_LINGO_WORKOUT_TRACKING, WIFI_WORKOUT_TRACKING_SPRINT8_COMPLETE_EXTRA, transactionId, sizeof(WIFIWorkoutDataSprint8CompleteExtra) );
            }
            if(wokoutCompleteData.wkDataCompleteEx.programType  == PS_FITNESS_TEST)                    
            {
                memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
                memcpy(&wifiResponse.data.WorkoutFitnessTestCompleteExtra,&completeFitnessTestData,sizeof(WIFIWorkoutDataFitnessTestCompleteExtra));
                InitMessage( WIFI_LINGO_WORKOUT_TRACKING, WIFI_WORKOUT_TRACKING_FITNESS_COMPLETE_EXTRA, transactionId, sizeof(WIFIWorkoutDataFitnessTestCompleteExtra) );
            }                    
            wifi_response_substate ++;
        case 5:
            if((wokoutCompleteData.wkDataCompleteEx.programType == PS_SPRINT8) || (wokoutCompleteData.wkDataCompleteEx.programType  == PS_FITNESS_TEST))
            {                
                if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
                    wifi_response_substate++;
            }
            else
            {
                wifi_response_substate++;
            }
            break;
        case 6:
            if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)DapiInfo.Url, strlen( DapiInfo.Url ), DapiInfo.Secure ) )
            {
#ifdef __WIFI_TEST_PETER__
                uartSend502++;
#endif
                wifi_response_substate++;
            }
            break;
        case 7:
            NextWiFiState();
            break;
        }
        break;
    case WCS_WORKOUT_SNAPSHOT_EX:
		switch( wifi_response_substate )
		{
		case 0:
            {
                SystemSettingType Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_MACHINE_UUID );
                wifi_response_substate++;
                memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
                GetWorkoutDataEx( &(wifiResponse.data.WorkoutDataEx ));//Workout_Data
                
                memcpy( wifiResponse.data.WorkoutDataEx.userExternalId, _UserExternalID, 36 );
                memcpy( wifiResponse.data.WorkoutDataEx.macAddress, wifi_settings.mac_address, 6 );
                memcpy(wifiResponse.data.WorkoutDataEx.MachineID,machineConfig.machineUUID, Settings.StringMaxLength);
                wifiResponse.data.WorkoutDataEx.workoutStartYear = _workoutStartTime.year ;
                wifiResponse.data.WorkoutDataEx.workoutStartYear = wifiResponse.data.WorkoutDataEx.workoutStartYear+ 2000;
                wifiResponse.data.WorkoutDataEx.workoutStartMonth= (UINT8)_workoutStartTime.month;
                wifiResponse.data.WorkoutDataEx.workoutStartDay= (UINT8)_workoutStartTime.date;
                wifiResponse.data.WorkoutDataEx.workoutStartHour= (UINT8)_workoutStartTime.hours;
                wifiResponse.data.WorkoutDataEx.workoutStartMinute= (UINT8)_workoutStartTime.minutes;
                wifiResponse.data.WorkoutDataEx.workoutStartSecond= (UINT8)_workoutStartTime.seconds;
                
                InitMessage( WIFI_LINGO_WORKOUT_TRACKING, WIFI_WORKOUT_TRACKING_DATA_EX, wifi_response_state[wifi_response_index].command,sizeof(WIFIWorkoutDataEx) );
            }
#ifndef __HIGH_SPEED__
			break;
#endif
		case 1://0x0500
			if( SetHTTPData( (char*)&wifiResponse, wifiResponse.header.dataSize + sizeof(WifiHeader) ) )
			{
				workoutTrackingDataCount++;
#ifdef __WIFI_TEST_PETER__
				uartHadSent++;
#endif
				wifi_response_substate++;
			}
			break;
		case 2:
			if( workoutTrackingDataCount >= 4095 / sizeof(WIFIWorkoutDataEx) - 4 )
			{
				if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)DapiInfo.Url, strlen( DapiInfo.Url ) ,DapiInfo.Secure ) )
				{
					wifi_response_substate++;
					//workoutPostTime = timer;
					workoutTrackingDataCount = 0;
#ifdef __WIFI_TEST_PETER__
                    uartSend502++;
#endif
				}
			}
			else
				wifi_response_substate++;
			break;
		case 3:
			NextWiFiState();
			break;
		}
        break;
        
    case WCS_SET_UPDATE_URL:
        switch(wifi_response_substate)
        {
        case 0:  
            {
                UINT8 tempSoftwareType=0;
                _wifiStatus=WIFI_STATUS_DOWNLOADING_SOFTWARE;
                _wifiEventHandle = WIFI_EVENT_UPDATE_PROGRESS;
                for(SOFTWARE_TYPE i = SOFTWARE_TYPE_START;i<SOFTWARE_TYPE_END;i++)
                {
                    if(strcmp(_softwareUpdate.SoftwareType,softwaretype[i])==0)
                    {
                        tempSoftwareType = i;
                        break;
                    }
                }
                
                if( SendRemoteUpdate(tempSoftwareType , _softwareUpdate.SoftwareUrl,_softwareUpdate.MD5) )
                {
                    wifi_response_substate ++;
                    for(char i= _NewBin_StartBlock_;i < _NewBin_EndBlock_ ;i++)
                    {
#ifdef WDT_ENABLE
                        HAL_Clear_WatchDog();
#endif
                        SPI_Flash_BlockErase(i);
                    }
                }  
            }
            break;
        case 1:
            if( SendDownloadProgram() )
            {              
                WaitForWIFIWriteFlash = timer;
                wifi_response_substate++;
            }
            break;
        case 2:
            if(getRemoteUpdate() == 1)
            {
                wifi_response_substate++;
                WaitForWIFIWriteFlash = timer;
            }
            else if(GetWifiModuleDownloadStatus())
            {
                wifi_response_substate = 6;
            }
            else
            {
                if(((timer - WaitForWIFIWriteFlash)%30)==0)
                {
                    _wifiStatus=WIFI_STATUS_DOWNLOADING_SOFTWARE;
                    status_timer = Digital_WIFI_Get_10ms_Timer( );
                    SendGetProgramStatus();
                }
            }
            if(timer-WaitForWIFIWriteFlash > 18000)
            {
                _wifiStatus=WIFI_STATUS_TIMEOUT;
                _wifiEventHandle = WIFI_EVENT_TIMEOUT;
                wifi_response_substate = 6;
            }
            break;
        case 3:
            if(!GetProgramReadStatus())
            {  
                SendReadProgramData(timer);
                if(Get_ProgressChanged())
                {
                    _wifiEventHandle = WIFI_EVENT_UPDATE_PROGRESS;
                }
                if(timer-WaitForWIFIWriteFlash > 24000)
                {
                    _wifiStatus=WIFI_STATUS_TIMEOUT;
                    _wifiEventHandle = WIFI_EVENT_TIMEOUT;
                    wifi_response_substate = 6;
                }
            }
            else
            {
                if(SendLeaveProgrammingMode())
                {
                    wifi_response_substate++;
                }
            }
            break;
        case 4: 
            {
                SystemConfig_Set_UpdateFlag(WIFI_UPDATE_AVAILABLE);
                _wifiEventHandle = WIFI_EVENT_UPDATE_COMPLETED;
                status_timer = Digital_WIFI_Get_10ms_Timer( );
                NextWiFiState();
            }
            //reboot
            break;
        case 6:
            {
                WaitForWIFIWriteFlash = timer;			
                _wifiStatus=WIFI_STATUS_TIMEOUT;
                _wifiEventHandle = WIFI_EVENT_TIMEOUT;
                wifi_response_substate++;
            }
            break;
        case 7:
            {
                if(timer-WaitForWIFIWriteFlash > 500)
                {
                    _wifiEventHandle = WIFI_EVENT_UPDATE_FAIL;
                    NextWiFiState();
                }
            }
            break;
        default:
            break;
        }
        break;
        
    case WCS_GET_LATEST_VERSION:
        {
            switch( wifi_response_substate )
            {
            case 0:
                {
                    wifi_response_substate++;
                }
                break;
            case 1:
                {
                    SystemSettingType Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_MACHINE_UUID );
                    
                    //                        _wifiStatus=WIFI_STATUS_RSCU_CHECKING;
                    //                        _wifiEventHandle = WIFI_EVENT_RSCU_CHECKING;
                    wifi_response_substate++;
                    memset( &wifiResponse.data, 0, sizeof(WIFIDATAGetLatestVersion) );
                    wifiResponse.data.getlatestVersion.BrandID = machineConfig.brandId;
                    wifiResponse.data.getlatestVersion.ConsoleType = machineConfig.consoleType;
                    //                        wifiResponse.data.getlatestVersion.Deployment = remoteUpdateDeploymentType;
                    //                        SystemConfig_Get_Version_Code(wifiResponse.data.getlatestVersion.CurrentVersion); 
                    wifiResponse.data.getlatestVersion.Deployment = DEPLOYMENT_QA; 
                    char tempbuf[10];
                    memset( tempbuf , '\0', 10  );
                    sprintf(tempbuf, "%s.%s",rscu_version[0],rscu_version[1]);   
                    memcpy(wifiResponse.data.getlatestVersion.CurrentVersion,tempbuf,sizeof(tempbuf));
                    memcpy(wifiResponse.data.getlatestVersion.MacAddress ,  wifi_settings.mac_address,6);
                    wifiResponse.data.getlatestVersion.MachineType = machineConfig.machineType;
                    memcpy(wifiResponse.data.getlatestVersion.MachineUUid,machineConfig.machineUUID,Settings.StringMaxLength);
                    wifiResponse.data.getlatestVersion.ModelType = machineConfig.machineModel;
                    strcpy(wifiResponse.data.getlatestVersion.SoftwareType,softwaretype[SOFTWARE_TYPE_NON_MCU]);
                    InitMessage( WIFI_LINGO_ASSET_MANAGEMENT, WIFI_ASSET_MANAGEMENT_GET_LATEST_VERSION, transactionId, sizeof(WIFIDATAGetLatestVersion) );
                }
#ifndef __HIGH_SPEED__
                break;
#endif
            case 2:
                {
                    if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
                        wifi_response_substate++;
                }
                break;
            case 3:
                {
                    if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)DapiInfo.Url, strlen( DapiInfo.Url ) , DapiInfo.Secure) )
                    {
                        wifi_response_substate++;
                    }
                }
                break;
            case 4:
                NextWiFiState();
                break;
                
            }
        }
        break;
        
        
        /* For Dapi Version 10 end */
        
	case WCS_GET_WIFI_INFO://used by WifiConnection_Status_Change
		switch( wifi_response_substate )
		{
		case 0:
			if( SendGetIP() )//0x0005
				wifi_response_substate++;
			break;
        case 1:
			if( SendGetMacAddress() )//0x0034
				wifi_response_substate++;
			break;
		case 2:
			if( SendGetSSID() )//0x0011
			{
				if( wifi_settings.ip_address[0] == 0 
                   && wifi_settings.ip_address[1] == 0 
                       && wifi_settings.ip_address[2] == 0
                           && wifi_settings.ip_address[3] == 0 )
                    wifi_response_substate = 0;
                
				else
					wifi_response_substate++;
			}
			break;
		case 3:
			if( SendGetSecurityType() )//0x0012
				wifi_response_substate++;
            
            machineConfig.isAPModeOn= 0;
            if(SystemConfig_Set_MachineInfo(machineConfig))
            {
                SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH, NULL,0);
            }
			break;
		case 4:
			if( SendGetSecurityKey() )//0x0013
				wifi_response_substate++;
			break;
		case 5:
			if( SendGetTCPBufferSize( 1 ) )//0x0020
				wifi_response_substate++;
			break;
		case 6:
			if( SendGetTCPAddress( 1 ) )//0x0022
				wifi_response_substate++;
			break;
		case 7:
			if( SendGetTCPPort( 1 ) )//0x0024
				wifi_response_substate++;
			break;
		case 8:
			if( SendGetTCPBufferSize( 2 ) )//0x0020
				wifi_response_substate++;
			break;
		case 9:
			if( SendGetTCPAddress( 2 ) )//0x0022
				wifi_response_substate++;
			break;
            
		case 10:
			if( SendGetTCPPort( 2 ) )//0x0024
				wifi_response_substate++;
			break;
		case 11:
			if( SendGetTCPBufferSize( 3 ) )//0x0020
				wifi_response_substate++;
			break;
        case 12:
            if( SendGetTCPAddress( 3 ) )//0x0022
				wifi_response_substate++;
			break;
		case 13:
			if( SendGetTCPPort( 3 ) )//0x0024
				wifi_response_substate++;
			break;
		case 14:
			if( SendSetWorkoutIP( (char*)machineConfig.marioBox, strlen( machineConfig.marioBox)+1) )//0x0504
				wifi_response_substate++;
			break;
		case 15:
			if( SendTCPConnect( 1 ) )//0x0025
				wifi_response_substate++;
			break;
		case 16:
			if(SendTCPConnect( 2 ) )//0x0025
				wifi_response_substate++;
			break;
		case 17:
			if( SendTCPConnect( 3 ) )//0x0025
				wifi_response_substate++;
			break;
            
		case 18:
            {
                
                SystemSettingType settings = SystemConfig_Get_DefaultString(SETTING_DEFAULT_MACHINE_UUID);
                memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
                wifiResponse.data.DAPILocation.DapiVersion = SystemConfig_Get_Dapi_Version();					
                memcpy( wifiResponse.data.DAPILocation.macAddress, wifi_settings.mac_address, 6 );
                memcpy( wifiResponse.data.DAPILocation.machineIdentification, settings.strings, settings.StringMaxLength);
                InitMessage( WIFI_LINGO_ASSET_MANAGEMENT, WIFI_ASSET_MANAGEMENT_GET_DAPI_LOCATION, transactionId, sizeof(WIFIDAPILocation) );
                wifi_response_substate ++;
                
                
            }
            break;
		case 19:
            {
                if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
                    wifi_response_substate++;
            }
            break;
		case 20:
            {
                if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)DapiInfo.Url, strlen( DapiInfo.Url ) , 0 ) )
                {
                    gWifiFlag.isSendDapiLocation = 1;
                    WaitForWIFIWriteFlash = timer;
                    NextWiFiState();
                }
            }
            break;
            
            
		}
        
		break;
        
	case WCS_INIT_WIFI://used by init wifi   //by ckm
		switch( wifi_response_substate )
		{
		case 0:
            {
                MachineInfor machineinfo;                    
                SystemSettingType Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_SERIAL_NUMBER );
                memset( &machineinfo, 0, sizeof( MachineInfor ) );
                machineinfo.Brand = machineConfig.brandId;
                machineinfo.CSafeID = machineConfig.csafeId;
                machineinfo.CSafeManu = machineConfig.csafeManufactureId;
                machineinfo.DriveType = machineConfig.driveType;
                machineinfo.ModeType = machineConfig.machineModel;
                machineinfo.MachineType = machineConfig.machineType;
                machineinfo.CSafeModel = machineConfig.csafeModeId;
                memcpy( machineinfo.serialNumber, machineConfig.consoleSerialNumber, Settings.StringMaxLength);
                Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_FRAME_NUMBER );
                memcpy( machineinfo.frameSerialNumber, machineConfig.frameSerialNumber, Settings.StringMaxLength );
                if( SendSetMachineInfo( (char*)&machineinfo, sizeof( MachineInfor )))
                    wifi_response_substate++;
            }
			break;
		case 1:
			if( SendGetWifiVersion() )
				wifi_response_substate++;
			break;
		case 2:
			if( SendGetUartVersion() )//0x0002
				wifi_response_substate++;
			break;
		case 3:
			if( SendGetMacAddress() )//0x0004
				wifi_response_substate++;
			break;
		case 4:
			if( SendGetSecurityType_AP() )//0x0034
				wifi_response_substate++;
			break;
		case 5:
			if( SendGetSecurityKey_AP() )//0x0035
				wifi_response_substate++;
			break;
		case 6:
			if( SendGetSSID_AP() )//0x0033
				wifi_response_substate++;
			break;
        case 7:  
            if(SendSetUDPPort(8180))//0X0091
				wifi_response_substate++;
			break;
        case 8:
            if(SendTCPDisconnect(1))//0x0026
                //if(SendGetUDPPort())//0X0092
				wifi_response_substate++;
			break;
		case 9:
			if(SendSetTCPPort( 1, 30304, 0x11 ) )//0x0023
				wifi_response_substate++;
			break;
            //        case 10:
            //			if(SendSetTCPPort( 2, 30305, 0x11 ) )//0x0023
            //				wifi_response_substate++;
            //			break;
            //        case 11:
            //			if(SendSetTCPPort( 3, 30306, 0x11 ) )//0x0023
            //				wifi_response_substate++;
            //			break;
     	case 10:
            if(SendGetTCPPort(1))//0x0024
                wifi_response_substate++;
			break;
        case 11:// settings.strings, strlen( settings.strings ) + 1 
            //            if(SendSetBroacastData(response_message,strlen( response_message ) + 1))//0x0700
            char ResponseMessage[80];
            //              strcpy(ResponseMessage,pDirs[i]);
            memset(ResponseMessage,'\0',80*sizeof(char));
            strcat(ResponseMessage,MESSAGE);
            
            
            XR_AVAIABLE_MODELS temp_Machine=SystemConfig_Get_Char(MACHINE_TYPE_UINT8);
                UINT8 temp_Model = SystemConfig_Get_Char(MACHINE_MODLE_UINT8);
                UINT8 ParaNumber;
                if(temp_Machine==TREADMILL)
                {
                    if(temp_Model==1) ParaNumber = XR_TF30;
                    else if(temp_Model==2) ParaNumber = XR_TF50;
                    else if(temp_Model==3) ParaNumber = XR_T50;
                    else if(temp_Model==4) ParaNumber = XR_T70_C2;
                    else if(temp_Model==5) ParaNumber = XR_T75_C2;
                    else if(temp_Model==6) ParaNumber = XR_T30;
                }
                else if(temp_Machine==ELLIPTICAL)
                {
                    if(temp_Model==1) ParaNumber = XR_SE30;
                    else if(temp_Model==2) ParaNumber = XR_SE50I;
                }
                else if(temp_Machine==BIKE) 
                {
                    if(temp_Model==1) ParaNumber =  XR_U30;
                    else if(temp_Model==2) ParaNumber = XR_U50I;
                    else if(temp_Model==3) ParaNumber = XR_R30;
                    else if(temp_Model==4) ParaNumber = XR_R50I;
                }
                else if(temp_Machine==ASCENT)
                {
                    if(temp_Model==1) ParaNumber = XR_A30;
                    else if(temp_Model==2) ParaNumber = XR_A50I;
                }
                else if(temp_Machine==CLIMB_MILL)
                {
                    if(temp_Model==1) ParaNumber = XR_C50;
                }
                
//            strcat(ResponseMessage,response_message[1]/*MACHINE_NAME*/);
//            strcpy(Pg2MacAddr,wifi_settings.mac_address+4);
                sprintf(Pg2MacAddr,"%2X%2X",
                        wifi_settings.mac_address[4],wifi_settings.mac_address[5]);
                for(char i=0;i<4;i++)
                {
                    if(Pg2MacAddr[i] == 0x20)   
                    {
                        Pg2MacAddr[i] = 0x30;
                    }
                }
                Pg2MacAddr[4]=';';
//            sprintf(Pg2MacAddr,"%4x",&Pg2MacAddr);
            strcpy(RfMachineName,RfMachinetype[ParaNumber]/*MACHINE_NAME*/);
                strcat(RfMachineName,Pg2MacAddr);
            strcat(ResponseMessage,RfMachineName/*MACHINE_NAME*/);    
//            strcat(ResponseMessage,RfMachinetype[ParaNumber]/*MACHINE_NAME*/);
            //            strcat(ResponseMessage,response_message[2]/*wifi_settings.ip_address*/);
            strcat(ResponseMessage,response_message[3]/*wifi_settings.tcp_settings[0].port*/);
            if(SendSetBroacastData(ResponseMessage, strlen(ResponseMessage) + 1 ))//0x0700
            	wifi_response_substate++;
			break;
        case 12:
            //if(SendSetBroacastData(ResponseMessage, strlen(ResponseMessage) + 1 ))//0x0700
            //if(SendSetTCPAddress( 1, "0.0.0.0", strlen( "0.0.0.0" ) + 1) )//0x0021
            wifi_response_substate++;
            break;
            //        case 15:
            //            if( SendGetTCPAddress( 1 ) )//0x0022
            //                wifi_response_substate++;
            //            			break;
        case 13:  
            //if(SendTCPDisconnect(1))//0x0026
            //if(SendSetBroacastData(ResponseMessage, strlen(ResponseMessage) + 1 ))//0x0700
            wifi_response_substate++;
			break;
        case 14: 
            //if(SendSetBroacastData(ResponseMessage, strlen(ResponseMessage) + 1 ))//0x0700
            wifi_response_substate++;
			break;
        case 15:  
            if(SendTCPConnect(1))//0x0025
            	wifi_response_substate++;
			break;
            
        case 16://0x0900
			NextWiFiState();
			break;
            /*
            default:
			{
            NextWiFiState();
        }
            break;
            */
		}
		break;
	case WCS_SET_AP_FOR_ENGTEST:
		switch( wifi_response_substate )
		{
        case 0:
			{
				if( SendDisconnectAP( ) )
				{
					wifi_response_substate++;
				}
			}
			break;
        case 1:
			{
				if( SendSetSSID( FACTORY_TEST_SSID, strlen( FACTORY_TEST_SSID )+1 ) )
				{
					wifi_response_substate++;
				}
			}
			break;
        case 2:
			{
				/*
                Set WI-FI Security Type in STA mode  (DataT = DataR)
                0: None
                1: WEP_40Bits
                2: WEP_104Bits
                3: WPA_PHRASE
                4: WPA2_PHRASE
                5: WPA_AUTO_PHRASE
                6: Auto Detect Security
				*/
				if( SendSetSecurityType( 5 ) )
				{
					wifi_response_substate++;
				}
			}
			break;
        case 3:
			{
				if( SendSetSecurityKey( FACTORY_TEST_PW, strlen( FACTORY_TEST_PW )+1  ) )
				{
					wifi_response_substate++;
				}
			}
			break;
        case 4:
			{
				if( SendSetApStaMode( 0x33 ) )//0x0800
					wifi_response_substate++;
			}
			break;
        case 5:
			{
				if( ResetWiFi() )
					wifi_response_substate++;
			}
			break;
        case 6:
			
		default:
			{
				NextWiFiState();
            }
            break;
		}
		break;
	case WCS_GET_WIFI_SETTINGS:
		switch( wifi_response_substate )
		{
		case 0:
			wifi_response_substate++;
			memset( &wifiResponse.data, 0, sizeof(WIFIDataAPConnection) );
			memset( wifiResponse.data.APConnection.securityKey, 0, 256 );
			memcpy( wifiResponse.data.APConnection.securityKey, wifi_settings.security.security_key, 255 );
			memset( wifiResponse.data.APConnection.ssid, 0, 256 );
			strncpy( wifiResponse.data.APConnection.ssid, wifi_settings.security.ssid, 255 );
			InitMessage( WIFI_LINGO_MODULE_SETTINGS, WIFI_GET_WIFI_SETTINGS, transactionId, sizeof(WIFIDataAPConnection) );
#ifndef __HIGH_SPEED__
			break;
#endif
		case 1://0x0027
			if( SendSetTCPData( wifi_response_state[wifi_response_index].port, (char*)&wifiResponse, wifiResponse.header.dataSize + sizeof(WifiHeader) ) )
				wifi_response_substate++;
			break;
		case 2:
			NextWiFiState();
			break;
		default:
			{
				NextWiFiState();
            }
            break;
		}
		break;
	case WCS_CONNECT_TO_AP:
		switch( wifi_response_substate )
		{
		case 0:
			wifi_response_substate++;
			memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
			wifiResponse.data.Ack.messageType = WIFI_CONNECT_TO_AP;
			wifiResponse.data.Ack.error = WIFI_ACK_OK;
			InitMessage( WIFI_LINGO_MODULE_SETTINGS, WIFI_MODULE_SETTINGS_ACKNOWLEDGMENT, transactionId, sizeof(WIFIDataAck) );
#ifndef __HIGH_SPEED__
			break;
#endif
		case 1:
			if( SendSetTCPData( wifi_response_state[wifi_response_index].port, (char*)&wifiResponse, wifiResponse.header.dataSize + sizeof(WifiHeader) ) )
				wifi_response_substate++;
			break;
            
		case 2:
            if( SendDisconnectAP() )//0x0007
				wifi_response_substate++;
            break;
		case 3://0x0008
            if( SendSetSSID( wifi_settings.security.ssid, strlen( wifi_settings.security.ssid ) + 1 ) )
				wifi_response_substate++;
            break;
		case 4:
            if( SendSetSecurityType( wifi_settings.security.security_type  ) )
				wifi_response_substate++;
            break;
		case 5:
            if( SendSetSecurityKey( (char*)wifi_settings.security.security_key, strlen( (char*)wifi_settings.security.security_key ) + 1 ) )
				wifi_response_substate++;
            break;
		case 6:
            if( SendSetApStaMode( 0x33 ) )//0x0800
				wifi_response_substate++;
            break;
		case 7:
            if( SendConnectAP() )//0x0006
				wifi_response_substate++;
            break;
		case 8:
            if( ResetWiFi() )//0x0900
				NextWiFiState();
            break;
		default:
			{
				NextWiFiState();
            }
            break;
		}
		break;
	case WCS_GET_LINGO_INFO:
		switch( wifi_response_substate )
		{
		case 0:
			wifi_response_substate++;
			memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
			wifiResponse.data.LingoVersion.lingo = wifi_response_state[wifi_response_index].command;
			switch( wifiResponse.data.LingoVersion.lingo )
			{
			case WIFI_LINGO_MODULE_SETTINGS:
				wifiResponse.data.LingoVersion.version = WIFI_LINGO_MODULE_SETTINGS_VERSION;
				break;
			case WIFI_LINGO_GENERAL:
				wifiResponse.data.LingoVersion.version = WIFI_LINGO_GENERAL_VERSION;
				break;
            case WIFI_LINGO_REMOTE:
                wifiResponse.data.LingoVersion.version = WIFI_LINGO_REMOTE_VERSION;
                break;
            case WIFI_LINGO_SETTINGS:
                wifiResponse.data.LingoVersion.version = WIFI_LINGO_SETTINGS_VERSION;
                break;
            case WIFI_LINGO_TEST:
                wifiResponse.data.LingoVersion.version = WIFI_LINGO_TEST_VERSION;
                break;
			}
			InitMessage( WIFI_LINGO_GENERAL, WIFI_GET_LINGO_VERSION, transactionId, sizeof(WIFIDataLingoVersion) );
#ifndef __HIGH_SPEED__
			break;
#endif
		case 1://0x0027
			if( SendSetTCPData( wifi_response_state[wifi_response_index].port, (char*)&wifiResponse, wifiResponse.header.dataSize + sizeof(WifiHeader) ) )
				wifi_response_substate++;
			break;
		case 2:
			NextWiFiState();
			break;
		default:
			{
				NextWiFiState();
            }
            break;
			
		}
		break;
	case WCS_GET_DAPI_LOCATION:
        {
            switch( wifi_response_substate )
            {
			case 0:
                {
                    
                    SystemSettingType settings = SystemConfig_Get_DefaultString(SETTING_DEFAULT_MACHINE_UUID);
                    wifiResponse.data.DAPILocation.DapiVersion = SystemConfig_Get_Dapi_Version();				
                    memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
                    memcpy( wifiResponse.data.DAPILocation.macAddress, wifi_settings.mac_address, 6 );
                    memcpy( wifiResponse.data.DAPILocation.machineIdentification, settings.strings, settings.StringMaxLength);
                    InitMessage( WIFI_LINGO_ASSET_MANAGEMENT, WIFI_ASSET_MANAGEMENT_GET_DAPI_LOCATION, transactionId, sizeof(WIFIDAPILocation) );
                    wifi_response_substate ++;
                }
#ifndef __HIGH_SPEED__
                break;
#endif
			case 1:
                {
                    if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
                        wifi_response_substate++;
                }
                break;
			case 2:
                {
                    if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)DapiInfo.Url, strlen( DapiInfo.Url ) , 0 ) )
                        NextWiFiState();
                }
                break;
            default:
                {
                    NextWiFiState();
                }
                break;
            }
        }
        break;
	case WCS_SET_DATA_SERVER:
		switch( wifi_response_substate )
		{
		case 0:
            {
                char IpAndPort[300] = {0};
                sprintf(IpAndPort,"%s:%d",DapiInfo.Server, DapiInfo.Port );
                if( SendSetWorkoutIP( (char*)IpAndPort, strlen( IpAndPort ) + 1 ) )
                {
                    
                    wifi_response_substate++;
                    memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
                    wifiResponse.data.Ack.messageType = WCS_SET_DATA_SERVER;
                    wifiResponse.data.Ack.error = WIFI_ACK_OK;
                    InitMessage( WIFI_LINGO_GENERAL, WIFI_GENERAL_ACKNOWLEDGMENT, transactionId, sizeof(WIFIDataAck) );
                }
            }
#ifndef __HIGH_SPEED__
			break;
#endif
		case 1://0x0027
			if( SendSetTCPData( wifi_response_state[wifi_response_index].port, (char*)&wifiResponse, wifiResponse.header.dataSize + sizeof(WifiHeader) ) )
				wifi_response_substate++;
			break;
            
		case 2:
			NextWiFiState();
			break;
		default:
			{
				NextWiFiState();
            }
            break;
		}
		break;
	case WCS_GET_DATA_SERVER:
        
		switch( wifi_response_substate )
		{
		case 0:
			wifi_response_substate++;
			memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
			memset( wifiResponse.data.DataServer.server, 0, 256 );
			strncpy( wifiResponse.data.DataServer.server, DapiInfo.Server, 255 );
			InitMessage( WIFI_LINGO_GENERAL, WIFI_GET_DATA_SERVER, transactionId, sizeof(WIFIDataDataServer) );
#ifndef __HIGH_SPEED__
			break;
#endif
		case 1://0x0027
			if( SendSetTCPData( wifi_response_state[wifi_response_index].port, (char*)&wifiResponse, wifiResponse.header.dataSize + sizeof(WifiHeader) ) )
				wifi_response_substate++;
			break;
		case 2:
			NextWiFiState();
			break;
		default:
			{
				NextWiFiState();
            }
            break;
			
		}
		break;
        
	case WCS_GET_MACHINE_INFO:
		switch( wifi_response_substate )
		{
		case 0:
            {
                SystemSettingType Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_SERIAL_NUMBER );
                wifi_response_substate++;
                memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
                memcpy( wifiResponse.data.MachineInfo.serialNumber, machineConfig.consoleSerialNumber, Settings.StringMaxLength);
                Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_FRAME_NUMBER );
                memcpy( wifiResponse.data.MachineInfo.frameSerialNumber, machineConfig.frameSerialNumber, Settings.StringMaxLength);
                Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_MACHINE_UUID );
                memcpy( wifiResponse.data.MachineInfo.machineIdentification, machineConfig.machineUUID, Settings.StringMaxLength);
                wifiResponse.data.MachineInfo.csafeId = machineConfig.csafeId;
                wifiResponse.data.MachineInfo.csafeManufacture = machineConfig.csafeManufactureId;
                wifiResponse.data.MachineInfo.csafeModelId = machineConfig.csafeModeId;
                wifiResponse.data.MachineInfo.driveType = machineConfig.driveType;
                wifiResponse.data.MachineInfo.brand = machineConfig.brandId;
                
                wifiResponse.data.MachineInfo.machineType = machineConfig.machineType;
                wifiResponse.data.MachineInfo.modelType = machineConfig.machineType;
                wifiResponse.data.MachineInfo.minMPHX10 = machineConfig.minSpeedMphX10;
                wifiResponse.data.MachineInfo.maxMPHX10 = machineConfig.maxSpeedMphX10;
                wifiResponse.data.MachineInfo.minKPHX10 = machineConfig.minSpeedKphX10;
                wifiResponse.data.MachineInfo.maxKPHX10 = machineConfig.maxSpeedKphX10;
                wifiResponse.data.MachineInfo.minInclineX10 = machineConfig.minInclineX10;
                wifiResponse.data.MachineInfo.maxInclineX10 = machineConfig.maxInclineX10;
                wifiResponse.data.MachineInfo.minLevel = machineConfig.minResistance;
                wifiResponse.data.MachineInfo.maxLevel = machineConfig.maxResistance;
                wifiResponse.data.MachineInfo.accumulatedSeconds = accumulalteddata.accumulatedTimeSeconds;
                wifiResponse.data.MachineInfo.accumulatedDistanceMilesX100 = accumulalteddata.accumulatedDistanceMilesX100;
                wifiResponse.data.MachineInfo.softwareVersion =(( (machineConfig.versionMajor<<8)|(machineConfig.versionMinor))<<8)|machineConfig.versionRevision;
                wifiResponse.data.MachineInfo.wifiProtocolVersion = 0;
                wifiResponse.data.MachineInfo.wifiFirmwareVersion = wifi_settings.wifi_version.INT_VAL;
                wifiResponse.data.MachineInfo.wifiUARTVersion = wifi_settings.uart_version.INT_VAL;
                wifiResponse.data.MachineInfo.btFirmwareVersion = 0;
                wifiResponse.data.MachineInfo.btUARTVersion = 0;
                // TODO: Get LCB Vendor and LCB Version from MCB.
                wifiResponse.data.MachineInfo.lcbVendor = 0;
                wifiResponse.data.MachineInfo.lcbVersion = 0;
                wifiResponse.data.MachineInfo.osVersion = 0;
                wifiResponse.data.MachineInfo.numUsers = SystemConfig_Get_MaxUserNumber();
                memcpy( wifiResponse.data.MachineInfo.macAddress, wifi_settings.mac_address, 6 );
                memcpy( wifiResponse.data.MachineInfo.ipAddress, wifi_settings.ip_address, 6 );
                wifiResponse.data.MachineInfo.errorCode = 0;
                wifiResponse.data.MachineInfo.serviceCode = 0;
                wifiResponse.data.MachineInfo.motorTemperature = 0;
                wifiResponse.data.MachineInfo.busCurrent = 0;
                wifiResponse.data.MachineInfo.busVoltage = 0;
                wifiResponse.data.MachineInfo.lubeMiles =accumulalteddata.accumulatedLubeMilesX100;
                Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_TIME_ZONE );
                memset( wifiResponse.data.MachineInfo.timeZone, 0, Settings.StringMaxLength);
                strcpy( wifiResponse.data.MachineInfo.timeZone, machineConfig.timezone );
                wifiResponse.data.MachineInfo.AutoLogin = machineConfig.isAutoLogin;//temp
                
                InitMessage( WIFI_LINGO_GENERAL, WIFI_GET_MACHINE_INFO, transactionId, sizeof(WIFIDataMachineInfo) );
            }
#ifndef __HIGH_SPEED__
			break;
#endif
		case 1://0x0027
			if( SendSetTCPData( wifi_response_state[wifi_response_index].port, (char*)&wifiResponse, wifiResponse.header.dataSize + sizeof(WifiHeader) ) )
				wifi_response_substate++;
			break;
		case 2:
			NextWiFiState();
			break;
        default:
			{
				NextWiFiState();
            }
            break;
            
		}
		break;
	case WCS_GET_WORKOUT_DATA:
		switch( wifi_response_substate )
		{
		case 0:
            
			wifi_response_substate++;
			memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
            
			GetWorkoutDataRemote( &wifiResponse.data.WorkoutDataRemote );//WorkoutDataRemote
			InitMessage( WIFI_LINGO_GENERAL, WIFI_GET_WORKOUT_DATA, wifi_response_state[wifi_response_index].command, sizeof(WIFIWorkoutDataRemote) );
#ifndef __HIGH_SPEED__
			break;
#endif
		case 1://0x0027
			if( SendSetTCPData( wifi_response_state[wifi_response_index].port, (char*)&wifiResponse, wifiResponse.header.dataSize + sizeof(WifiHeader) ) )
				wifi_response_substate++;
			break;
		case 2:
			NextWiFiState();
			break;
		default:
			{
				NextWiFiState();
            }
            break;
			
		}
		break;
        
	case WCS_POST_WORKOUT_DATA:
		switch( wifi_response_substate )
		{
		case 0:
            {
                SystemSettingType Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_MACHINE_UUID );
                wifi_response_substate++;
                memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
                memcpy( wifiResponse.data.WorkoutData.userExternalId, _UserExternalID, 36 );
                memcpy( wifiResponse.data.WorkoutData.macAddress, wifi_settings.mac_address, 6 );
                memcpy(wifiResponse.data.WorkoutData.MachineID,machineConfig.machineUUID, Settings.StringMaxLength);
                wifiResponse.data.WorkoutData.year = _workoutStartTime.year ;
                wifiResponse.data.WorkoutData.year = wifiResponse.data.WorkoutData.year+ 2000;
                wifiResponse.data.WorkoutData.month = (UINT8)_workoutStartTime.month;
                wifiResponse.data.WorkoutData.day = (UINT8)_workoutStartTime.date;
                wifiResponse.data.WorkoutData.hour = (UINT8)_workoutStartTime.hours;
                wifiResponse.data.WorkoutData.minute = (UINT8)_workoutStartTime.minutes;
                wifiResponse.data.WorkoutData.second = (UINT8)_workoutStartTime.seconds;
                GetWorkoutData( &wifiResponse.data.WorkoutData );//Workout_Data
                InitMessage( WIFI_LINGO_WORKOUT_TRACKING, WIFI_WORKOUT_TRACKING_WORKOUT_DATA, wifi_response_state[wifi_response_index].command,sizeof(WIFIWorkoutData) );
            }
#ifndef __HIGH_SPEED__
			break;
#endif
		case 1://0x0500
			if( SetHTTPData( (char*)&wifiResponse, wifiResponse.header.dataSize + sizeof(WifiHeader) ) )
			{
				workoutTrackingDataCount++;
				wifi_response_substate++;
			}
			break;
		case 2:
			if( workoutTrackingDataCount >= 4095 / sizeof(WIFIWorkoutData) - 1 )
			{
				if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)DapiInfo.Url, strlen( DapiInfo.Url ) ,DapiInfo.Secure ) )
				{
					//workoutPostTime = timer;
					wifi_response_substate++;
					workoutTrackingDataCount = 0;
				}
			}
			else
				wifi_response_substate++;
			break;
		case 3:
			NextWiFiState();
			break;
		}
		break;
	case WCS_ACK:
		switch( wifi_response_substate )
		{
		case 0:
			wifi_response_substate++;
			memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
			wifiResponse.data.Ack.messageType = wifi_response_state[wifi_response_index].command & 0xff;
			wifiResponse.data.Ack.error = wifi_response_state[wifi_response_index].error;
			InitMessage( ( wifi_response_state[wifi_response_index].command >> 8 ) & 0xff, WIFI_GENERAL_ACKNOWLEDGMENT, transactionId, sizeof(WIFIDataAck) );
#ifndef __HIGH_SPEED__
			break;
#endif
		case 1://0x0027
			if( SendSetTCPData( wifi_response_state[wifi_response_index].port, (char*)&wifiResponse, wifiResponse.header.dataSize + sizeof(WifiHeader) ) )
				wifi_response_substate++;
			break;
		case 2:
			NextWiFiState();
			break;	
		}
		break;
	case WCS_GET_REMOTE_DATA:
		switch( wifi_response_substate )
		{
		case 0:
		case 1:
		case 2:
			if( wifi_settings.status.tcp_connected[wifi_response_substate + 1] == 0 )
                        {wifi_response_substate++;
//                         AddWifiResponseState( WCS_GET_WIFI_INFO, 0, 0, 0 );//lxl20170809
                        }
			else if( SendGetTCPData( wifi_response_substate + 1 ) )//0x0028
				wifi_response_substate++;
			break;
		case 3:
			if( GetHTTPResponseData() )//0x0503
				wifi_response_substate++;
			break;
            
		case 4:
			NextWiFiState();
			break;
		}
		break;
	case WCS_GET_STATUS:
#if 0
		if( _remoteControlIndex != 0 && wifi_settings.status.tcp_connected[_remoteControlIndex] == 0 )
		{
            
			_remoteControlState = WIFI_IDLE;
			_remoteControlIndex = 0;
            
			if( _workoutState != WIFI_WORKOUT_IDLE && _workoutState != WIFI_WORKOUT_STOP )
			{
				USERS_INFO userinfo = SystemConfig_Get_UserInfo( 0 );
				memset( &userinfo, 0, sizeof(USERS_INFO) );
				userinfo.weightStandardX100= 15000;
				DateTime_T now;
#ifdef __WIFI_NO_RTC__
                now= ReadNetworkTime(GetNetworkDelay());
#else
                RTC_Read_Time( &now );
#endif
				
				userinfo.year = now.year + 2000 - 30;
				if( now.month == 2 && now.date == 29 )
					userinfo.date = 28;
				else
					userinfo.date = now.date;
				userinfo.month = now.month;
				userinfo.gender = 0;
				userinfo.languageId= 0;
			}
		}
#endif
		switch( wifi_response_substate )
		{
		case 0:
			if( SendGetStatus() )//0x0003
				wifi_response_substate++;
			break;
		case 1:
			NextWiFiState();
			break;
		}
        
		break;
        
	case WCS_GET_SPEED_CALIBRATION:
		switch( wifi_response_substate )
		{
		case 0:
			wifi_response_substate++;
#if 0
			memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
			wifiResponse.data.SpeedCalibration.maxMPHx10 = machineConfig.maxSpeedMphX10;
			wifiResponse.data.SpeedCalibration.minMPHx10 = machineConfig.minSpeedMphX10;
			wifiResponse.data.SpeedCalibration.maxKPHx10 = machineConfig.maxSpeedKphX10;
			wifiResponse.data.SpeedCalibration.minKPHx10 = machineConfig.minSpeedKphX10;
			wifiResponse.data.SpeedCalibration.minInclinex10 = machineConfig.minInclineX10;
			wifiResponse.data.SpeedCalibration.maxInclinex10 = machineConfig.maxInclineX10;
			wifiResponse.data.SpeedCalibration.minADC = machineConfig->minInclineADC;
			wifiResponse.data.SpeedCalibration.maxADC = machineConfig->maxInclineADC;
			wifiResponse.data.SpeedCalibration.rpmMPHLow = machineConfig->minMPH_RPM;
			wifiResponse.data.SpeedCalibration.rpmMPHHigh = machineConfig->maxMPH_RPM;
			wifiResponse.data.SpeedCalibration.rpmKPHLow = machineConfig->minKPH_RPM;
			wifiResponse.data.SpeedCalibration.rpmKPHHigh = machineConfig->maxKPH_RPM;
			InitMessage( WIFI_LINGO_GENERAL, WIFI_GET_SPEED_CALIBRATION, transactionId, sizeof(WIFIDataSpeedCalibration) );
#endif
#ifndef __HIGH_SPEED__
			break;
#endif
		case 1://0x0027
			if( SendSetTCPData( wifi_response_state[wifi_response_index].port, (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
				wifi_response_substate++;
			break;
		case 2:
			NextWiFiState();
			break;
		}
		break;
	case WCS_UNLINK_USER:
		{
			switch( wifi_response_substate )
			{
            case 0:
				{					
                    SystemSettingType Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_MACHINE_UUID );
					memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
					wifiResponse.data.UnlinkUserData.UserIndex = wifi_response_state[wifi_response_index].command;
					memcpy( wifiResponse.data.UnlinkUserData.machineIdentification , machineConfig.machineUUID , Settings.StringMaxLength );
					InitMessage( WIFI_LINGO_USER_MANAGEMENT, WIFI_USER_UNLINK_USER, transactionId, sizeof(WIFIUnlinkUser) );
					wifi_response_substate++;
				}
#ifndef __HIGH_SPEED__
				break;
#endif
            case 1:
				{
					if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
						wifi_response_substate++;
				}
				break;
            case 2:
				{
					if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)DapiInfo.Url, strlen( DapiInfo.Url ) ,DapiInfo.Secure ) )
					{
						NextWiFiState( );
					}
				}
				break;
			}
		}
		break;
	case WCS_GET_PWM_CALIBRATION:
		switch( wifi_response_substate )
		{
		case 0:
			wifi_response_substate++;
			memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
#if 0
			for( int i = 0; i < 5; i++ )
			{
				wifiResponse.data.PWMCal.calPoint[i].level = 1;
				wifiResponse.data.PWMCal.calPoint[i].cal = 0;
			}
			InitMessage( WIFI_LINGO_GENERAL, WIFI_GET_PWM_CAL, transactionId, sizeof(WIFIDataPWMCal) );
#endif
#ifndef __HIGH_SPEED__
			break;
#endif
		case 1://0x0027
			if( SendSetTCPData( wifi_response_state[wifi_response_index].port, (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
				wifi_response_substate++;
			break;
		case 2:
			NextWiFiState();
			break;
        default:
			NextWiFiState();
            break;
		}
		break;
	case WCS_GET_ECB_CALIBRATION:
		switch( wifi_response_substate )
		{
		case 0:
			wifi_response_substate++;
#if 0
			memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
			for( int i = 0; i < 30; i++ )
			{
				wifiResponse.data.ECBCal.ecbLevel[i] = machineConfig->ecbCal[i];
			}
			InitMessage( WIFI_LINGO_GENERAL, WIFI_GET_ECB_CAL, transactionId, sizeof(WIFIDataECBCal) );
#endif
			break;
		case 1:
			if( SendSetTCPData( wifi_response_state[wifi_response_index].port, (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
				wifi_response_substate++;
			break;
		case 2:
			NextWiFiState();
			break;
        default:
			NextWiFiState();
            break;
		}
		break;
	case WCS_GET_WATTS_TABLE:
		if( wifi_response_substate == 30 * 2 )
			NextWiFiState();
		else if( wifi_response_substate % 2 == 0 )
		{
			wifi_response_substate++;
#if 0
			memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
			wifiResponse.data.WattsTable.level = wifi_response_substate / 2 + 1;
			InitMessage( WIFI_LINGO_GENERAL, WIFI_GET_WATTS_TABLE, transactionId, sizeof(WIFIDataWattsTable) );
#endif
		}
		else
		{
			if( SendSetTCPData( wifi_response_state[wifi_response_index].port, (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
				wifi_response_substate++;
		}
		break;
	case WCS_GET_MACHINE_STATE:
		switch( wifi_response_substate )
		{
		case 0:
			wifi_response_substate++;
			memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
			if( _remoteControlIndex == wifi_response_index )
				wifiResponse.data.UByte = _remoteControlState;
			else
				wifiResponse.data.UByte = WIFI_IDLE;
			InitMessage( WIFI_LINGO_GENERAL, WIFI_GET_MACHINE_STATE, transactionId, sizeof(UINT8) );
			break;
		case 1://0x0027
			if( SendSetTCPData( wifi_response_state[wifi_response_index].port, (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
				wifi_response_substate++;
			break;
		case 2:
			NextWiFiState();
			break;
		}
		break;
	case WCS_GET_USER_DATA:
		switch( wifi_response_substate )
		{
		case 0:
            {
		    	DateTime_T t;
                USERS_INFO userinfo = SystemConfig_Get_UserInfo( User_Get_UserSlot()/*wifi_response_state[wifi_response_index].command*/);
#ifdef __WIFI_NO_RTC__
                t = ReadNetworkTime(GetNetworkDelay());
#else
                RTC_Read_Time( &t );
#endif
                
                memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
                memcpy( wifiResponse.data.pssportUserData.xID, (char*)userinfo.xid, XIDMAXLENGTH );
                //                memcpy( wifiResponse.data.pssportUserData.passcode, (char*)userinfo.passcode, 4 );
                wifiResponse.data.pssportUserData.userId = User_Get_UserSlot();//wifi_response_state[wifi_response_index].command;
                wifiResponse.data.pssportUserData.weightX10 = userinfo.weightStandardX100/10;
                wifiResponse.data.pssportUserData.age = t.year+2000-userinfo.year;
                wifiResponse.data.pssportUserData.birthYear = userinfo.year;
                wifiResponse.data.pssportUserData.birthMonth= userinfo.month;
                wifiResponse.data.pssportUserData.birthDay = userinfo.date;
                wifiResponse.data.pssportUserData.birthLanguageId= userinfo.languageId;
//                wifiResponse.data.pssportUserData.displayUnits = 0;
//                wifiResponse.data.pssportUserData.displayUnits = machineConfig.unit;
                wifiResponse.data.pssportUserData.displayUnits = Get_User_Unit();
                memcpy( wifiResponse.data.pssportUserData.userName, userinfo.name, 64 );
			if(strcmp(wifiResponse.data.pssportUserData.userName,"")==0)
			{
                if(1==User_Get_UserSlot())
                    memcpy( wifiResponse.data.pssportUserData.userName, "User1", 64 );
                if(2==User_Get_UserSlot())
                    memcpy( wifiResponse.data.pssportUserData.userName, "User2", 64 );
                if(3==User_Get_UserSlot())
                    memcpy( wifiResponse.data.pssportUserData.userName, "User3", 64 );
                if(4==User_Get_UserSlot())
                    memcpy( wifiResponse.data.pssportUserData.userName, "User4", 64 );
			}
                wifiResponse.data.pssportUserData.acceptedTerms = 0;
                wifiResponse.data.pssportUserData.shareWorkoutData = 0;
                //                wifiResponse.data.UserData.month = userinfo.month;
                //                wifiResponse.data.UserData.day = userinfo.date;
                wifiResponse.data.pssportUserData.gender = userinfo.gender;
                //                wifiResponse.data.UserData.langauge = userinfo.languageId;
                //                memcpy( wifiResponse.data.UserData.name, userinfo.name, 64 );
                InitMessage( WIFI_LINGO_GENERAL, WIFI_GET_USER_DATA, transactionId, sizeof(PassportUserInfo) );
                wifi_response_substate++;
            }
			break;
		case 1://0x0027
			if( SendSetTCPData( wifi_response_state[wifi_response_index].port, (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
				wifi_response_substate++;
			break;
		case 2:
			NextWiFiState();
			break;
		}
		break;
	case WCS_GET_CURRENT_USER:
		switch( wifi_response_substate )
		{
		case 0:
			wifi_response_substate++;
			memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
			wifiResponse.data.UByte = User_Get_UserSlot();//Program_Get_User();
            InitMessage( WIFI_LINGO_GENERAL, WIFI_GET_CURRENT_USER, transactionId, sizeof(UINT8) );
			break;
		case 1://0x0027
			if( SendSetTCPData( wifi_response_state[wifi_response_index].port, (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
				wifi_response_substate++;
			break;
		case 2:
			NextWiFiState();
			break;
		}
		break;
        
	case WCS_GET_MACHINE_INFO_PASSPORT:
		switch( wifi_response_substate )
		{
		case 0:
			wifi_response_substate++;
			memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
            
            SystemSettingType Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_SERIAL_NUMBER );
            wifiResponse.data.passportData.machineType = machineConfig.machineType;
            Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_MACHINE_UUID );
            memcpy( wifiResponse.data.passportData.machineIdentification , machineConfig.machineUUID ,Settings.StringMaxLength );
            wifiResponse.data.passportData.minMPHX10 = machineConfig.minSpeedMphX10;
            wifiResponse.data.passportData.maxMPHX10 = machineConfig.maxSpeedMphX10;
//            wifiResponse.data.passportData.minRPM=  machineConfig.minRpmMphX10;
//            wifiResponse.data.passportData.maxRPM= machineConfig.maxRpmMphX10;
            wifiResponse.data.passportData.minRPM=  0;
            wifiResponse.data.passportData.maxRPM= 240;
            wifiResponse.data.passportData.minInclineX10 = machineConfig.minInclineX10;
            wifiResponse.data.passportData.maxInclineX10 = machineConfig.maxInclineX10;
            wifiResponse.data.passportData.minResistance = machineConfig.minResistance;
            wifiResponse.data.passportData.maxResistance = machineConfig.maxResistance; 
            
            InitMessage( WIFI_LINGO_GENERAL, WIFI_GET_MACHINE_INFO_PASSPORT, transactionId, sizeof(PassportMachineInfo) );
			break;
		case 1://0x0027
			if( SendSetTCPData( wifi_response_state[wifi_response_index].port, (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
				wifi_response_substate++;
			break;
		case 2:
			NextWiFiState();
			break;
		}
		break;
        
    case WCS_SET_START_WORKOUT:
		switch( wifi_response_substate )
		{
		case 0:
			wifi_response_substate++;
			memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
			//wifiResponse.data.UByte =1;// Program_Get_User();
            InitMessage( WIFI_LINGO_REMOTE, WIFI_START, transactionId, 0 );
			break;
		case 1://0x0027
			if( SendSetTCPData( wifi_response_state[wifi_response_index].port, (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
				wifi_response_substate++;
			break;
		case 2:
			NextWiFiState();
			break;
		}
		break;
        
    case WCS_GET_WORKOUT_DATA_PASSPORT:
		switch( wifi_response_substate )
		{
		case 0:
			wifi_response_substate++;
			memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
            
            SystemSettingType Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_SERIAL_NUMBER );
            wifiResponse.data.passportData.machineType = machineConfig.machineType;
            Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_MACHINE_UUID );
            memcpy( wifiResponse.data.passportData.machineIdentification , machineConfig.machineUUID ,Settings.StringMaxLength );

				
            wifiResponse.data.passportWorkoutData.WorkoutId  = Program_Get_ProgramID();
            
            //0: Stop
            //1: Run
            //2: Pause
            //3: Idle
            //4: Complete	
            if( Program_Get_Status() == PS_IDLE)
            {
                wifiResponse.data.passportWorkoutData.WorkoutState = 0;//stop (0)
            }
            if( Program_Get_Status() == PS_RUN)
            {
                wifiResponse.data.passportWorkoutData.WorkoutState = 1;//run (1)
            }
            if( Program_Get_Status() == PS_PAUSED)
            {
                wifiResponse.data.passportWorkoutData.WorkoutState = 2;//pause (2)
            }
            if( Program_Get_Status() ==PS_COMPLETED)
            {
                wifiResponse.data.passportWorkoutData.WorkoutState = 0;//complete (4)
            }	
            
            
            //0: None
            //1: Warmup
            //2: Workout
            //3: Cooldown
            if( Program_Get_Status() == PS_IDLE)
            {
                wifiResponse.data.passportWorkoutData.WorkoutMode = 0;
            }
            if( Program_Get_Status() == PS_WARMUP)
            {
                wifiResponse.data.passportWorkoutData.WorkoutMode = 1;
            }
            if( Program_Get_Status() == PS_RUN)
            {
                wifiResponse.data.passportWorkoutData.WorkoutMode = 2;
            }
            if( Program_Get_Status() ==PS_COOLDOWN)
            {
                wifiResponse.data.passportWorkoutData.WorkoutMode = 3;
            }
            
            
            wifiResponse.data.passportWorkoutData.WorkoutTime = Program_Get_WorkoutTime_Elapsed();
            
            UINT16 WarmUpTime = Program_Get_WorkoutTime_Warmup();
            wifiResponse.data.passportWorkoutData.WarmUpTime  = WarmUpTime;
			UINT16  CoolDownTime = Program_Get_WorkoutTime_Cooldown();
            wifiResponse.data.passportWorkoutData.CoolDownTime =  CoolDownTime;
            
            wifiResponse.data.passportWorkoutData.TimeInState = 0;//Program_Get_WorkoutTime_Elapsed();//????
            
            UINT16 SpeedX100 = Program_Get_Speed(DATA_AUTO,0);//stanard
            if(Get_User_Unit()==METRIC)
            {
                SpeedX100 = SpeedX100*1250000/201168;
            }
//            SpeedX100 = SpeedX100*10;
            else
            SpeedX100 = SpeedX100*10;
//            if(SpeedX100 > 100 && SpeedX100 < 9000)
//            {
//                if((SpeedX100 % 100)>=5)
//                {
//                    SpeedX100 = SpeedX100/10+1;   
//                }
//                else
//                {
//                    SpeedX100 = SpeedX100/10;
//                }
//            }
            wifiResponse.data.passportWorkoutData.SpeedX100 = SpeedX100;
            wifiResponse.data.passportWorkoutData.ActualSpeedX100 = SpeedX100;
            wifiResponse.data.passportWorkoutData.AverageSpeedX100 = SpeedX100;
            UINT16 InclineX10 = Program_Get_Incline(0);
            wifiResponse.data.passportWorkoutData.InclineX10 = InclineX10;
            wifiResponse.data.passportWorkoutData.TotalCalories = Program_Get_Calories(1);
            wifiResponse.data.passportWorkoutData.CaloriesPerHour = Program_Get_Calories(1);
            wifiResponse.data.passportWorkoutData.MetsX100 = Program_Get_METs()*10;
            wifiResponse.data.passportWorkoutData.Watts = Program_Get_Watts(AUTO_WATTS,0);
            wifiResponse.data.passportWorkoutData.MilesX100 = Program_Get_Distance(DATA_STANARD);
            
            UINT16 HeartRate = Program_Get_HR(CURRENT_HR);
            wifiResponse.data.passportWorkoutData.HeartRate = HeartRate;
			
            wifiResponse.data.passportWorkoutData.AverageHeartRate = 0;
            wifiResponse.data.passportWorkoutData.PeakHeartRate = 0;
            
            wifiResponse.data.passportWorkoutData.RPM = Program_Get_RPM();
            UINT16 ResistanceLevel =	Program_Get_Resistance(0);
            wifiResponse.data.passportWorkoutData.ResistanceLevel = ResistanceLevel;
            
            UINT32 GoalX100 = Program_Get_WorkoutTime();
            GoalX100 = GoalX100*100;	
            wifiResponse.data.passportWorkoutData.GoalX100 = GoalX100;
            UINT32 GoalRemainingX100 = Program_Get_WorkoutTime_Remaining();
            GoalRemainingX100 = GoalRemainingX100*100;	
            wifiResponse.data.passportWorkoutData.GoalRemainingX100 = GoalRemainingX100;
            //0: None
            //1: Time Seconds
            //2: Distance Miles
            //3: Calorie
            //4: Floors
            //5: Elevation
            
			wifiResponse.data.passportWorkoutData.GoalType = 1;// Time Seconds
			
            
            wifiResponse.data.passportWorkoutData.Elevation =  Program_Get_Elevation();//5
            wifiResponse.data.passportWorkoutData.VALocation = 0;
            
            wifiResponse.data.passportWorkoutData.Steps = Program_Get_Steps();
            wifiResponse.data.passportWorkoutData.Floors = 0;
            wifiResponse.data.passportWorkoutData.SPM = 0;
            wifiResponse.data.passportWorkoutData.AverageSPM = 0;
            
			/*
            RF_AP_Set_Data_To_Tx(TX_MODE_INUSE,TX_INUSE_UNIT_MODE,Get_User_Unit());
            RF_AP_Set_Data_To_Tx(TX_MODE_INUSE,TX_INUSE_MACHINE_MODE,SystemConfig_Get_Char(MACHINE_TYPE_UINT8));
            RF_AP_Set_Data_To_Tx(TX_MODE_INUSE,TX_INUSE_WORKOUT_TIME,Program_Get_WorkoutTime_Remaining());
            RF_AP_Set_Data_To_Tx(TX_MODE_INUSE,TX_INUSE_DISTANCE,Program_Get_Distance(DATA_AUTO));
            RF_AP_Set_Data_To_Tx(TX_MODE_INUSE,TX_INUSE_CALORIES,Program_Get_Calories(1));
            RF_AP_Set_Data_To_Tx(TX_MODE_INUSE,TX_INUSE_HEART_RATE,Program_Get_HR(CURRENT_HR));
            RF_AP_Set_Data_To_Tx(TX_MODE_INUSE,TX_INUSE_SPEED,Program_Get_Speed(DATA_AUTO,0));
            RF_AP_Set_Data_To_Tx(TX_MODE_INUSE,TX_INUSE_TARGET_WORKOUT_TIME,Program_Get_WorkoutTime());
            
            
            RF_AP_Set_Data_To_Tx(TX_MODE_INUSE,TX_INUSE_INCLINE,Program_Get_Incline(0)/10);
            UINT32 l_Dat = Program_Get_Pace();
            l_Dat = (l_Dat/60*100)+(l_Dat%60);		
            RF_AP_Set_Data_To_Tx(TX_MODE_INUSE,TX_INUSE_CURRENT_PACE,l_Dat);
            RF_AP_Set_Data_To_Tx(TX_MODE_INUSE,TX_INUSE_RESISTANCE,Program_Get_Resistance(0));
            RF_AP_Set_Data_To_Tx(TX_MODE_INUSE,TX_INUSE_RPM,Program_Get_RPM());
            RF_AP_Set_Data_To_Tx(TX_MODE_INUSE,TX_INUSE_WATTS,Program_Get_Watts(CURRENT_WATTS,0));
            */
            
            //            wifiResponse.data.passportWorkoutData.WorkoutId = machineConfig.minSpeedMphX10;
            //            wifiResponse.data.passportWorkoutData.maxMPHX10 = machineConfig.maxSpeedMphX10;
            //            wifiResponse.data.passportWorkoutData.minInclineX10 = machineConfig.minInclineX10;
            //            wifiResponse.data.passportWorkoutData.maxInclineX10 = machineConfig.maxInclineX10;
            //            wifiResponse.data.passportWorkoutData.minResistanceX10 = machineConfig.minResistance;
            //            wifiResponse.data.passportWorkoutData.maxResistanceX10 = machineConfig.maxResistance; 
            
            InitMessage( WIFI_LINGO_GENERAL, WIFI_GET_WORKOUT_DATA_PASSPORT, transactionId, sizeof(PassportGetWorkoutData) );
			break;
		case 1://0x0027
			if( SendSetTCPData( wifi_response_state[wifi_response_index].port, (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
				wifi_response_substate++;
			break;
		case 2:
			NextWiFiState();
			break;
		}
		break;
        
	case WCS_GET_DEFAULTS:
		switch( wifi_response_substate )
		{
		case 0:
			wifi_response_substate++;
			memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
			wifiResponse.data.Defaults.maxMPHX10 = machineConfig.maxSpeedMphX10;
			wifiResponse.data.Defaults.minMPHX10 = machineConfig.minSpeedMphX10;
			wifiResponse.data.Defaults.maxKPHX10 = machineConfig.maxSpeedKphX10;
			wifiResponse.data.Defaults.minKPHX10 = machineConfig.minSpeedKphX10;
			wifiResponse.data.Defaults.maxInclineX10 = machineConfig.maxInclineX10;
			wifiResponse.data.Defaults.minInclineX10 = machineConfig.minInclineX10;
            //			wifiResponse.data.Defaults.maxWorkoutTime = workoutdata.maxWorkoutMinutes *60;
            //			wifiResponse.data.Defaults.defaultWorkoutTime = workoutdata.defaultWorkoutMinutes;
			wifiResponse.data.Defaults.defaultAge = machineConfig.defaultAge;
			wifiResponse.data.Defaults.defaultWeightX10 = machineConfig.defaultWeightLBSX100;
            //			wifiResponse.data.Defaults.defaultWarmupTime = workoutdata.defaultWarmupMinutes;
            //			wifiResponse.data.Defaults.defaultCooldownTime = workoutdata.defaultCooldownMinutes;
            //			wifiResponse.data.Defaults.pauseTime = workoutdata.defaultPauseMinutes;
			wifiResponse.data.Defaults.gender = machineConfig.defaultGender;
			wifiResponse.data.Defaults.volume = machineConfig.defaultVolume;
			wifiResponse.data.Defaults.brightness = machineConfig.defaulitBrightness;
			wifiResponse.data.Defaults.targetHR = machineConfig.targetHR;
            //			wifiResponse.data.Defaults.constantWatts = workoutdata.defaultWattsTarget;
			wifiResponse.data.Defaults.energySaver = machineConfig.isEnergySaverModeOn;
			wifiResponse.data.Defaults.languageId = machineConfig.languageId;
			wifiResponse.data.Defaults.units = machineConfig.unit;
			wifiResponse.data.Defaults.demoMode = machineConfig.isDemoModeOn;
			wifiResponse.data.Defaults.timeFormat = machineConfig.is24HoursFormat;
            InitMessage( WIFI_LINGO_SETTINGS, WIFI_GET_DEFAULTS, transactionId, sizeof(WIFIDataDefaults) );
			break;
		case 1://0x0027
			if( SendSetTCPData( wifi_response_state[wifi_response_index].port, (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
				wifi_response_substate++;
			break;
		case 2:
			NextWiFiState();
			break;
		}
		break;
	case WCS_GET_MACHINE_TYPE:
        
		switch( wifi_response_substate )
		{
		case 0:
            {
                SystemSettingType Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_SERIAL_NUMBER );
                wifi_response_substate++;
                memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
                wifiResponse.data.MachineType.machineType = machineConfig.machineType;
                wifiResponse.data.MachineType.modelType = machineConfig.machineModel;
                wifiResponse.data.MachineType.CSAFEManufacture = machineConfig.csafeManufactureId;
                wifiResponse.data.MachineType.CSAFEId = machineConfig.csafeId;
                wifiResponse.data.MachineType.CSAFEModelId = machineConfig.csafeModeId;
                memcpy( wifiResponse.data.MachineType.ConsoleSerialNumber, machineConfig.consoleSerialNumber , Settings.StringMaxLength);
                Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_FRAME_NUMBER );
                memcpy( wifiResponse.data.MachineType.FrameSerialNumber, machineConfig.frameSerialNumber, Settings.StringMaxLength );
                Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_MACHINE_UUID );
                memcpy( wifiResponse.data.MachineInfo.machineIdentification, machineConfig.machineUUID, Settings.StringMaxLength );
                InitMessage( WIFI_LINGO_SETTINGS, WIFI_GET_MACHINE_TYPE, transactionId, sizeof(WIFIMachineTypeData) );
            }
			break;
		case 1://0x0027
			if( SendSetTCPData( wifi_response_state[wifi_response_index].port, (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
				wifi_response_substate++;
			break;
		case 2:
			NextWiFiState();
			break;
		}
		break;
        
	case WCS_CHECK_FOR_UPDATE:
		switch( wifi_response_substate )
		{
		case 0:
            {
                SystemSettingType Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_SERIAL_NUMBER );
                char serialNumber[16];
                memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
                memset( serialNumber, 0, Settings.StringMaxLength);
                strncpy( serialNumber, machineConfig.consoleSerialNumber, Settings.StringMaxLength );
#ifdef __DIGITAL_SYSTEM__
                if( SendRemoteUpdate( CONSOLE_SERIES, EQUIPMENT_TYPE, SOFTWARE_VERSION, Digital_Get_Data( G_New_Version ), wifi_settings.wifi_version.INT_VAL,
                                     machineConfig->clubId, serialNumber ) )
                {
                    wifi_response_substate++;
                    wifiResponse.data.Ack.messageType = WCS_CHECK_FOR_UPDATE;
                    wifiResponse.data.Ack.error = WIFI_ACK_OK;
                    InitMessage( ( wifi_response_state[wifi_response_index].command >> 8 ) & 0xff, WIFI_GENERAL_ACKNOWLEDGMENT, transactionId,
                                sizeof(WIFIDataAck) );
                }
#endif
            }
			break;
		case 1://0x0027
			if( SendSetTCPData( wifi_response_state[wifi_response_index].port, (char*)&wifiResponse, wifiResponse.header.dataSize + sizeof(WifiHeader) ) )
				wifi_response_substate++;
			break;
            
		case 2:
			NextWiFiState();
			break;
		}
		break;
	case WCS_GET_REGION:
        
		switch( wifi_response_substate )
		{
		case 0:
            {
                DateTime_T now;
#ifdef __WIFI_NO_RTC__
                now = ReadNetworkTime(GetNetworkDelay());
#else
                RTC_Read_Time( &now );
#endif
                wifi_response_substate++;
                memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
                wifiResponse.data.Region.year = now.year + 2000;
                wifiResponse.data.Region.month = now.month;
                wifiResponse.data.Region.day = now.date;
                wifiResponse.data.Region.hour = now.hours;
                wifiResponse.data.Region.minute = now.minutes;
                wifiResponse.data.Region.second = now.seconds;
                wifiResponse.data.Region.timeFormat = machineConfig.is24HoursFormat;
                //			strcpy( wifiResponse.data.Region.timeZone, machineConfig.timezone);
                
                InitMessage( WIFI_LINGO_SETTINGS, WIFI_GET_REGION, transactionId, sizeof(WIFIDataRegion) );
            }
			break;
		case 1://0x0027
			if( SendSetTCPData( wifi_response_state[wifi_response_index].port, (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
				wifi_response_substate++;
			break;
		case 2:
			NextWiFiState();
			break;
		}
		break;
        
	case WCS_UPDATE_USER:
		switch( wifi_response_substate )
		{
		case 0:
            {
                UINT8 userIndex = wifi_response_state[wifi_response_index].command;
                USERS_INFO userinfo = SystemConfig_Get_UserInfo( userIndex );
                memset( &wifiResponse.data, 0, sizeof( WifiCommandData ) );
                wifi_response_substate++;
                wifiResponse.data.UserDataRequest.userConsoleId = userIndex;
                memcpy( wifiResponse.data.UserDataRequest.xid, userinfo.xid, XIDMAXLENGTH );
                memcpy( wifiResponse.data.UserDataRequest.passcode, userinfo.passcode, 4 );
                memcpy( wifiResponse.data.UserDataRequest.macAddress, wifi_settings.mac_address, 6 );
                memcpy( wifiResponse.data.UserDataRequest.machineIdentification, machineConfig.machineUUID,36);
                wifiResponse.data.UserDataRequest.lastChangeYear = userinfo.timeStampYear;
                wifiResponse.data.UserDataRequest.lastChangeMonth = userinfo.timeStampMonth;
                wifiResponse.data.UserDataRequest.lastChangeDay = userinfo.timeStampDate;
                wifiResponse.data.UserDataRequest.lastChangeHour = userinfo.timeStampHour;
                wifiResponse.data.UserDataRequest.lastChangeMinute = userinfo.timeStampMinute;
                wifiResponse.data.UserDataRequest.lastChangeSecond = userinfo.timeStampSeconds;
                wifiResponse.data.UserDataRequest.gender = userinfo.gender;
                wifiResponse.data.UserDataRequest.weightX10 = userinfo.weightStandardX100/10;
                wifiResponse.data.UserDataRequest.age = userinfo.age;
                wifiResponse.data.UserDataRequest.year = userinfo.year;
                wifiResponse.data.UserDataRequest.month = userinfo.month;
                wifiResponse.data.UserDataRequest.day = userinfo.date;
                wifiResponse.data.UserDataRequest.language = userinfo.languageId;
                wifiResponse.data.UserDataRequest.WorkoutDataShare = userinfo.isShareWorkouts;
                wifiResponse.data.UserDataRequest.DisplayUnits = userinfo.displayUnit;
                wifiResponse.data.UserDataRequest.AcceptedTerms = userinfo.isAcceptedTerms;
                memcpy( wifiResponse.data.UserDataRequest.name, userinfo.name, 65 );
                wifiResponse.data.UserDataRequest.UserSetup= userinfo.isSetup;
                InitMessage( WIFI_LINGO_USER_MANAGEMENT, WIFI_SYNC_USER_INFO, transactionId, sizeof(WIFIExternalUserRequestData) );
#if 0
                UINT8 userId = wifi_response_state[wifi_response_index].command;
                USERS_INFO userinfo = SystemConfig_Get_UserInfo( wifi_response_state[wifi_response_index].command );
                memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
                wifiResponse.data.UserDataRequest.userConsoleId = userId;
                
                wifiResponse.data.UserDataRequest.lastChangeYear = userinfo.timeStampYear;
                wifiResponse.data.UserDataRequest.lastChangeMonth = userinfo.timeStampMonth;
                wifiResponse.data.UserDataRequest.lastChangeDay = userinfo.timeStampDate;
                wifiResponse.data.UserDataRequest.lastChangeHour = userinfo.timeStampHour;
                wifiResponse.data.UserDataRequest.lastChangeMinute = userinfo.timeStampMinute;
                wifiResponse.data.UserDataRequest.lastChangeSecond = userinfo.timeStampSeconds;
                
                wifiResponse.data.UserDataRequest.weightX10 = userinfo.weightStandardX100/10;
                wifiResponse.data.UserDataRequest.age = userinfo.age;
                wifiResponse.data.UserDataRequest.year = userinfo.year;
                wifiResponse.data.UserDataRequest.month = userinfo.month;
                wifiResponse.data.UserDataRequest.day = userinfo.date;
                wifiResponse.data.UserDataRequest.gender = userinfo.gender;
                
                strcpy( wifiResponse.data.UserDataRequest.name, userinfo.name);
                wifi_response_substate++;
                InitMessage( WIFI_LINGO_WORKOUT_TRACKING, WIFI_WORKOUT_TRACKING_SYNC_USER, transactionId, sizeof(WIFIExternalUserRequestData) );
#endif
            }
#ifndef __HIGH_SPEED__
			break;
#endif
		case 1://0x0500
			if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
				wifi_response_substate++;
			break;
		case 2://0x0502
			if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)DapiInfo.Url, strlen( DapiInfo.Url ), DapiInfo.Secure ) )
				wifi_response_substate++;
			break;
		case 3:
			NextWiFiState();
			break;
        default:
			NextWiFiState();
            break;
		}
		break;
        
	case WCS_COMPLETE_WORKOUT:
		switch( wifi_response_substate )
		{
		case 0:            
            
			wifi_response_substate++;
			memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
            
            memcpy(&wifiResponse.data.WorkoutData, &(workouData.WorkdataTrack), sizeof(WIFIWorkoutData));
			InitMessage( WIFI_LINGO_WORKOUT_TRACKING, WIFI_WORKOUT_TRACKING_WORKOUT_DATA, wifi_response_state[wifi_response_index].command,
                        sizeof(WIFIWorkoutData) );
#ifndef __HIGH_SPEED__
			break;
#endif
		case 1://0x0500
			if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
				wifi_response_substate++;
			break;
		case 2:
			memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
            memcpy(&wifiResponse.data.WorkoutTrackingDataComplete,&wokoutCompleteData.wkDataComplete,sizeof(WIFIWorkoutDataComplete));
			InitMessage( WIFI_LINGO_WORKOUT_TRACKING, WIFI_WORKOUT_TRACKING_WORKOUT_COMPLETE, transactionId, sizeof(WIFIWorkoutDataComplete) );
			wifi_response_substate++;
#ifndef __HIGH_SPEED__
			break;
#endif
		case 3://0x0500
			if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
				wifi_response_substate++;
			break;
		case 4:
			if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)DapiInfo.Url, strlen( DapiInfo.Url ), DapiInfo.Secure ) )
				wifi_response_substate++;
			break;
		case 5:
			NextWiFiState();
			break;
		}
		break;
	case WCS_SET_MARIO_BOX:
        {
            switch(wifi_response_substate)
            {
            case 0:
                {
                    char IpAndPort[300] = {0};
                    sprintf(IpAndPort,"%s:%d",DapiInfo.Server, DapiInfo.Port );
                    if( SendSetWorkoutIP( (char*)IpAndPort, strlen( IpAndPort ) + 1 ) )//0x0504	
                        wifi_response_substate ++;
                }
                break;
            case 1:
                wifi_response_substate++;
                memcpy( wifiResponse.data.RegisterRequest.macAddress, wifi_settings.mac_address, 6 );
                
                InitMessage( WIFI_LINGO_ASSET_MANAGEMENT, WIFI_ASSET_MANAGEMENT_MACHINE_REGISTRATION, transactionId, sizeof(WIFIMachineRegisterRequest) );
                break;
            case 2:
                if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
                    wifi_response_substate++;
                break;
            case 3:
                if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)DapiInfo.Url, strlen( DapiInfo.Url ), DapiInfo.Secure ) )
                {
                    gWifiFlag.isSendMachineRegister= 1;
                    WaitForWIFIWriteFlash = timer;
                    wifi_response_substate++;
                }
                break;
                case 4://lxl20170809
			NextWiFiState();
			break;
			}
        }
        break;
	case WCS_SYNC_CONSOLE_INFO://used by WifiConnection_Status_Change
#if 0
		if( wifi_response_substate > 3 )
		{
			int userIndex = ( wifi_response_substate - 4 ) / 2;
			int stepIndex = ( wifi_response_substate - 4 ) % 2;
            
			if( userIndex == SystemConfig_Get_MaxUserNumber( ) )
			{
				if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)DapiInfo.Url, strlen( DapiInfo.Url ) ,DapiInfo.Secure) )//meng
				{
					NextWiFiState();
				}
			}
			else
			{
				if( stepIndex == 0 )
				{
                    USERS_INFO userinfo = SystemConfig_Get_UserInfo( userIndex );
                    memset(&(wifiResponse.data.UserDataRequest),0,sizeof(WIFIExternalUserRequestData));
					wifiResponse.data.UserDataRequest.userConsoleId = userIndex;
					memcpy( wifiResponse.data.UserDataRequest.xid, userinfo.xid, XIDMAXLENGTH );
					memcpy( wifiResponse.data.UserDataRequest.passcode, userinfo.passcode, 4 );
					memcpy( wifiResponse.data.UserDataRequest.macAddress, wifi_settings.mac_address, 6 );
                    
					memcpy( wifiResponse.data.UserDataRequest.machineIdentification, machineConfig.machineUUID, 36 );
					wifiResponse.data.UserDataRequest.lastChangeYear = userinfo.timeStampYear;
					wifiResponse.data.UserDataRequest.lastChangeMonth = userinfo.timeStampMonth;
					wifiResponse.data.UserDataRequest.lastChangeDay = userinfo.timeStampDate;
					wifiResponse.data.UserDataRequest.lastChangeHour = userinfo.timeStampHour;
					wifiResponse.data.UserDataRequest.lastChangeMinute = userinfo.timeStampMinute;
					wifiResponse.data.UserDataRequest.lastChangeSecond = userinfo.timeStampSeconds;
					wifiResponse.data.UserDataRequest.gender = userinfo.gender;
					wifiResponse.data.UserDataRequest.weightX10 = userinfo.weightStandardX10;
                    
					
					wifiResponse.data.UserDataRequest.age = userinfo.age;
					wifiResponse.data.UserDataRequest.year = userinfo.year;
					wifiResponse.data.UserDataRequest.month = userinfo.month;
					wifiResponse.data.UserDataRequest.day = userinfo.date;
					wifiResponse.data.UserDataRequest.language = userinfo.languageId;
					memcpy( wifiResponse.data.UserDataRequest.name, userinfo.name, 65 );
					wifiResponse.data.UserDataRequest.AcceptedTerms = userinfo.isAcceptedTerms;
					wifiResponse.data.UserDataRequest.DisplayUnits = userinfo.displayUnit;
					wifiResponse.data.UserDataRequest.WorkoutDataShare = userinfo.isShareWorkouts;
                    
					wifiResponse.data.UserDataRequest.UserSetup= userinfo.isSetup;
					InitMessage( WIFI_LINGO_USER_MANAGEMENT, WIFI_SYNC_USER_INFO, transactionId, sizeof(WIFIExternalUserRequestData) );
					wifi_response_substate++;
				}
				else
				{//0x0500
					if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
						wifi_response_substate++;
				}
			}
		}
#endif
		switch( wifi_response_substate )
		{
		case 0:
			{
                memset(&(wifiResponse.data.TimeRequest),0,sizeof(WIFITimeRequestRegion));
				strcpy( wifiResponse.data.TimeRequest.timeZone, machineConfig.timezone );
				InitMessage( WIFI_LINGO_NETWORK_TIME, WIFI_NETWORK_TIME_GET_TIME, transactionId, sizeof(WIFITimeRequestRegion) );
				wifi_response_substate++;
			}
#ifndef __HIGH_SPEED__
			break;
#endif
		case 1:
		case 3://0x0500
			if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
				wifi_response_substate++;
			break;
		case 2:
			wifi_response_substate++;	
#if 0
            if( SystemConfig_Get_Dapi_Version()  > 9 )
            {
                SystemSettingType Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_SERIAL_NUMBER );
                memset( &wifiResponse.data.MachineInfoEx , 0, sizeof(WIFIMachineInfoEX) );
                memcpy(wifiResponse.data.MachineInfoEx.serialNumber , machineConfig.consoleSerialNumber, Settings.StringMaxLength);
                Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_FRAME_NUMBER );
                memcpy(wifiResponse.data.MachineInfoEx.frameSerialNumber, machineConfig.frameSerialNumber , Settings.StringMaxLength );
                Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_MACHINE_UUID );
                memcpy( wifiResponse.data.MachineInfoEx.machineIdentification , machineConfig.machineUUID ,Settings.StringMaxLength );
                memcpy(wifiResponse.data.MachineInfoEx.macAddress, wifi_settings.mac_address, 6);
                wifiResponse.data.MachineInfoEx.brandID = machineConfig.brandId;
                wifiResponse.data.MachineInfoEx.machineType = machineConfig.machineType;
                wifiResponse.data.MachineInfoEx.modelType = machineConfig.machineModel;
                wifiResponse.data.MachineInfoEx.driveType = machineConfig.driveType;
                wifiResponse.data.MachineInfoEx.consoleType = machineConfig.consoleType;
                wifiResponse.data.MachineInfoEx.csafeManufacture = machineConfig.csafeManufactureId;
                wifiResponse.data.MachineInfoEx.csafeID = machineConfig.csafeId;
                wifiResponse.data.MachineInfoEx.csafeModelID = machineConfig.csafeModeId;
                wifiResponse.data.MachineInfoEx.clubID = machineConfig.clubId;
                wifiResponse.data.MachineInfoEx.minMPHX10 = machineConfig.minSpeedMphX10;
                wifiResponse.data.MachineInfoEx.maxMPHX10 = machineConfig.maxSpeedMphX10;
                wifiResponse.data.MachineInfoEx.minKPHX10 = machineConfig.minSpeedKphX10;
                wifiResponse.data.MachineInfoEx.maxKPHX10 = machineConfig.maxSpeedKphX10;
                wifiResponse.data.MachineInfoEx.minInclineX10 = machineConfig.minInclineX10;
                wifiResponse.data.MachineInfoEx.maxInclineX10 = machineConfig.maxInclineX10;
                wifiResponse.data.MachineInfoEx.minResistanceLevel = machineConfig.minResistance;
                wifiResponse.data.MachineInfoEx.maxResistanceLevel = machineConfig.maxResistance;
                SystemConfig_Get_Version_Code(wifiResponse.data.MachineInfoEx.softwareVersion);
                //                strncpy(wifiResponse.data.MachineInfoEx.softwareVersion,SystemConfig_Get_Version( ), 20);
                wifiResponse.data.MachineInfoEx.dapiApiVersion = SystemConfig_Get_Dapi_Version( );
                wifiResponse.data.MachineInfoEx.LCBVendor = 0;
                wifiResponse.data.MachineInfoEx.LCBVersion = 0;
                memset(wifiResponse.data.MachineInfoEx.osVersion, '0', 20);
                wifiResponse.data.MachineInfoEx.maxUsers = SystemConfig_Get_MaxUserNumber( );
                memcpy(wifiResponse.data.MachineInfoEx.IpAddress, wifi_settings.ip_address, 6);
                Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_TIME_ZONE );
                memcpy(wifiResponse.data.MachineInfoEx.timeZone, machineConfig.timezone, Settings.StringMaxLength );
                wifiResponse.data.MachineInfoEx.autoLogin = machineConfig.isAutoLogin;
                wifiResponse.data.MachineInfoEx.lastUpdateYear = machineConfig.lastUpdateYear;
                wifiResponse.data.MachineInfoEx.lastUpdateMonth = machineConfig.lastUpdateMonth;
                wifiResponse.data.MachineInfoEx.lastUpdateDay = machineConfig.lastUpdateDay;
                wifiResponse.data.MachineInfoEx.lastUpdateHour = machineConfig.lastUpdateHour;
                wifiResponse.data.MachineInfoEx.lastUpdateMinute = machineConfig.lastUpdateMinute;
                wifiResponse.data.MachineInfoEx.lastUpdateSecond = machineConfig.lastUpdateSecond;
                memcpy(wifiResponse.data.MachineInfoEx.lastUpdateTimezone,machineConfig.lastUpdateTimezone,Settings.StringMaxLength);
                InitMessage( WIFI_LINGO_ASSET_MANAGEMENT, WIFI_ASSET_MANAGEMENT_MACHINE_INFO_EX, transactionId, sizeof(WIFIMachineInfoEX) );
            }
            else
			{
                SystemSettingType Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_SERIAL_NUMBER );
				memset( &wifiResponse.data.MachineInfo, 0, sizeof( WIFIDataMachineInfo ) );
				wifiResponse.data.MachineInfo.brand = machineConfig.brandId;
				wifiResponse.data.MachineInfo.machineType = machineConfig.machineType;
				wifiResponse.data.MachineInfo.modelType = machineConfig.machineModel;
				wifiResponse.data.MachineInfo.driveType = machineConfig.driveType;
				wifiResponse.data.MachineInfo.csafeId = machineConfig.csafeId;
				wifiResponse.data.MachineInfo.csafeManufacture = machineConfig.csafeManufactureId;
				wifiResponse.data.MachineInfo.csafeModelId = machineConfig.csafeModeId;
				memcpy( wifiResponse.data.MachineInfo.serialNumber, machineConfig.consoleSerialNumber , Settings.StringMaxLength);
                Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_FRAME_NUMBER );
				memcpy( wifiResponse.data.MachineInfo.frameSerialNumber, machineConfig.frameSerialNumber ,  Settings.StringMaxLength);
                Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_MACHINE_UUID );
				memcpy( wifiResponse.data.MachineInfo.machineIdentification, machineConfig.machineUUID, Settings.StringMaxLength);
				wifiResponse.data.MachineInfo.clubId = machineConfig.clubId;
				wifiResponse.data.MachineInfo.minMPHX10 = machineConfig.minSpeedMphX10;
				wifiResponse.data.MachineInfo.maxMPHX10 = machineConfig.maxSpeedMphX10;
				wifiResponse.data.MachineInfo.minKPHX10 = machineConfig.minSpeedKphX10;
				wifiResponse.data.MachineInfo.maxKPHX10 = machineConfig.maxSpeedKphX10;
				wifiResponse.data.MachineInfo.minInclineX10 = machineConfig.minInclineX10;
				wifiResponse.data.MachineInfo.maxInclineX10 = machineConfig.maxInclineX10;
				wifiResponse.data.MachineInfo.minLevel = machineConfig.minResistance;
				wifiResponse.data.MachineInfo.maxLevel = machineConfig.maxResistance;
				wifiResponse.data.MachineInfo.accumulatedSeconds = accumulalteddata.accumulatedTimeSeconds;
				wifiResponse.data.MachineInfo.accumulatedDistanceMilesX100 = accumulalteddata.accumulatedDistanceMilesX100;
				wifiResponse.data.MachineInfo.softwareVersion = (( (machineConfig.versionMajor<<8)|(machineConfig.versionMinor))<<8)|machineConfig.versionRevision;
				wifiResponse.data.MachineInfo.DapiVersion = SystemConfig_Get_Dapi_Version() ; 
				wifiResponse.data.MachineInfo.wifiProtocolVersion = WIFI_PROTOCOL_VERSION;
				wifiResponse.data.MachineInfo.wifiFirmwareVersion = wifi_settings.wifi_version.INT_VAL;
				wifiResponse.data.MachineInfo.wifiUARTVersion = wifi_settings.uart_version.INT_VAL;
				wifiResponse.data.MachineInfo.btFirmwareVersion =0;
				wifiResponse.data.MachineInfo.btUARTVersion = 0;
				wifiResponse.data.MachineInfo.lcbVendor = 0;
				wifiResponse.data.MachineInfo.lcbVersion = 0;
				wifiResponse.data.MachineInfo.ioVersion = 0;
				wifiResponse.data.MachineInfo.osVersion = 0;
				wifiResponse.data.MachineInfo.numUsers = SystemConfig_Get_MaxUserNumber();
				memcpy( wifiResponse.data.MachineInfo.macAddress, wifi_settings.mac_address, 6 );
				memcpy( wifiResponse.data.MachineInfo.ipAddress, wifi_settings.ip_address, 6 );
                
				wifiResponse.data.MachineInfo.errorCode = 0;
				wifiResponse.data.MachineInfo.serviceCode = 0;
				wifiResponse.data.MachineInfo.motorTemperature = 0;
				wifiResponse.data.MachineInfo.busCurrent = 0;
				wifiResponse.data.MachineInfo.busVoltage = 0;
				wifiResponse.data.MachineInfo.lubeMiles =accumulalteddata.accumulatedLubeMilesX100;
                
				strcpy( wifiResponse.data.MachineInfo.timeZone, machineConfig.timezone);
				wifiResponse.data.MachineInfo.AutoLogin = machineConfig.isAutoLogin;//temp
				InitMessage( WIFI_LINGO_ASSET_MANAGEMENT, WIFI_ASSET_MANAGEMENT_MACHINE_INFO, transactionId, sizeof(WIFIDataMachineInfo) );
			}
#endif
			break;
        case 4:
            if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)DapiInfo.Url, strlen( DapiInfo.Url ) ,DapiInfo.Secure) )//meng
            {
                NextWiFiState();
			}
            break;
            
		}
		break;
        
	case WCS_UPDATE_TIME:
		switch( wifi_response_substate )
		{
		case 0:
			memset( &wifiResponse.data, 0, sizeof( WifiCommandData ) );
			strcpy( wifiResponse.data.TimeRequest.timeZone, machineConfig.timezone );
			InitMessage( WIFI_LINGO_NETWORK_TIME, WIFI_NETWORK_TIME_GET_TIME, transactionId, sizeof(WIFITimeRequestRegion) );
			wifi_response_substate++;
#ifndef __HIGH_SPEED__
			break;
#endif
		case 1://0x0500
			if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
				wifi_response_substate++;
			break;
		case 2://0x0502
			if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)DapiInfo.Url, strlen( DapiInfo.Url ), DapiInfo.Secure) )
			{
                gWifiFlag.isSendUpdateTime= 1;
                WaitForWIFIWriteFlash = timer;
				wifi_response_substate++;
			}
			break;
		case 3:
			NextWiFiState();
			break;
		}
		break;
        
	case WCS_UPDATE_USER_INFO:
        {
            int userIndex = wifi_response_substate / 2 + 1;
            int stepIndex = wifi_response_substate % 2;
            
            if( userIndex >= SystemConfig_Get_MaxUserNumber() )
            {
                if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)DapiInfo.Url, strlen( DapiInfo.Url ) ,DapiInfo.Secure) )
                    NextWiFiState();
            }
            else
            {
                SystemSettingType Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_MACHINE_UUID );
                if( stepIndex == 0 )
                {
                    USERS_INFO userinfo = SystemConfig_Get_UserInfo( userIndex );
                    memset( &wifiResponse.data, 0, sizeof( WifiCommandData ) );
                    wifi_response_substate++;
                    wifiResponse.data.UserDataRequest.userConsoleId = userIndex;
                    memcpy( wifiResponse.data.UserDataRequest.xid, userinfo.xid, XIDMAXLENGTH );
                    memcpy( wifiResponse.data.UserDataRequest.passcode, userinfo.passcode, 4 );
                    memcpy( wifiResponse.data.UserDataRequest.macAddress, wifi_settings.mac_address, 6 );
                    memcpy( wifiResponse.data.UserDataRequest.machineIdentification, machineConfig.machineUUID,Settings.StringMaxLength);
                    wifiResponse.data.UserDataRequest.lastChangeYear = userinfo.timeStampYear;
                    wifiResponse.data.UserDataRequest.lastChangeMonth = userinfo.timeStampMonth;
                    wifiResponse.data.UserDataRequest.lastChangeDay = userinfo.timeStampDate;
                    wifiResponse.data.UserDataRequest.lastChangeHour = userinfo.timeStampHour;
                    wifiResponse.data.UserDataRequest.lastChangeMinute = userinfo.timeStampMinute;
                    wifiResponse.data.UserDataRequest.lastChangeSecond = userinfo.timeStampSeconds;
                    wifiResponse.data.UserDataRequest.gender = userinfo.gender;
                    wifiResponse.data.UserDataRequest.weightX10 = userinfo.weightStandardX100/10;
                    wifiResponse.data.UserDataRequest.age = userinfo.age;
                    wifiResponse.data.UserDataRequest.year = userinfo.year;
                    wifiResponse.data.UserDataRequest.month = userinfo.month;
                    wifiResponse.data.UserDataRequest.day = userinfo.date;
                    wifiResponse.data.UserDataRequest.language = userinfo.languageId;
                    wifiResponse.data.UserDataRequest.WorkoutDataShare = userinfo.isShareWorkouts;
                    wifiResponse.data.UserDataRequest.DisplayUnits = userinfo.displayUnit;
                    wifiResponse.data.UserDataRequest.AcceptedTerms = userinfo.isAcceptedTerms;
                    memcpy( wifiResponse.data.UserDataRequest.name, userinfo.name, 65 );
                    wifiResponse.data.UserDataRequest.UserSetup= userinfo.isSetup;
                    InitMessage( WIFI_LINGO_USER_MANAGEMENT, WIFI_SYNC_USER_INFO, transactionId, sizeof(WIFIExternalUserRequestData) );
                    
                }
                else
                {
                    if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
                        wifi_response_substate++;
                }
            }
        }
		break;
        // case WCS_COMMIT_USER_EDIT:
        //break;
        
	case WCS_UPDATE_MACHINE_INFO:
		switch( wifi_response_substate )
		{
		case 0:       
            {
                SystemSettingType Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_SERIAL_NUMBER );
                wifi_response_substate++;
                if( SystemConfig_Get_Dapi_Version()  > 9 )
                {
                    memset( &wifiResponse.data.MachineInfoEx , 0, sizeof(WIFIMachineInfoEX) );
                    memcpy(wifiResponse.data.MachineInfoEx.serialNumber , machineConfig.consoleSerialNumber, Settings.StringMaxLength);
                    Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_FRAME_NUMBER );
                    memcpy(wifiResponse.data.MachineInfoEx.frameSerialNumber, machineConfig.frameSerialNumber , Settings.StringMaxLength );
                    Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_MACHINE_UUID );
                    memcpy( wifiResponse.data.MachineInfoEx.machineIdentification , machineConfig.machineUUID ,Settings.StringMaxLength );
                    memcpy(wifiResponse.data.MachineInfoEx.macAddress, wifi_settings.mac_address, 6);
                    wifiResponse.data.MachineInfoEx.brandID = machineConfig.brandId;
                    wifiResponse.data.MachineInfoEx.machineType = machineConfig.machineType;
                    wifiResponse.data.MachineInfoEx.modelType = machineConfig.machineModel;
                    wifiResponse.data.MachineInfoEx.driveType = machineConfig.driveType;
                    wifiResponse.data.MachineInfoEx.consoleType = machineConfig.consoleType;
                    wifiResponse.data.MachineInfoEx.csafeManufacture = machineConfig.csafeManufactureId;
                    wifiResponse.data.MachineInfoEx.csafeID = machineConfig.csafeId;
                    wifiResponse.data.MachineInfoEx.csafeModelID = machineConfig.csafeModeId;
                    wifiResponse.data.MachineInfoEx.clubID = machineConfig.clubId;
                    wifiResponse.data.MachineInfoEx.minMPHX10 = machineConfig.minSpeedMphX10;
                    wifiResponse.data.MachineInfoEx.maxMPHX10 = machineConfig.maxSpeedMphX10;
                    wifiResponse.data.MachineInfoEx.minKPHX10 = machineConfig.minSpeedKphX10;
                    wifiResponse.data.MachineInfoEx.maxKPHX10 = machineConfig.maxSpeedKphX10;
                    wifiResponse.data.MachineInfoEx.minInclineX10 = machineConfig.minInclineX10;
                    wifiResponse.data.MachineInfoEx.maxInclineX10 = machineConfig.maxInclineX10;
                    wifiResponse.data.MachineInfoEx.minResistanceLevel = machineConfig.minResistance;
                    wifiResponse.data.MachineInfoEx.maxResistanceLevel = machineConfig.maxResistance;
                    SystemConfig_Get_Version_Code(wifiResponse.data.MachineInfoEx.softwareVersion);
                    //strncpy(wifiResponse.data.MachineInfoEx.softwareVersion,SystemConfig_Get_Version( ), 20);
                    wifiResponse.data.MachineInfoEx.dapiApiVersion = SystemConfig_Get_Dapi_Version( );
                    wifiResponse.data.MachineInfoEx.LCBVendor = 0;
                    wifiResponse.data.MachineInfoEx.LCBVersion = 0;
                    memset(wifiResponse.data.MachineInfoEx.osVersion, '0', 20);
                    wifiResponse.data.MachineInfoEx.maxUsers = SystemConfig_Get_MaxUserNumber( );
                    memcpy(wifiResponse.data.MachineInfoEx.IpAddress, wifi_settings.ip_address, 6);
                    Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_TIME_ZONE );
                    memcpy(wifiResponse.data.MachineInfoEx.timeZone, machineConfig.timezone, Settings.StringMaxLength );
                    wifiResponse.data.MachineInfoEx.autoLogin = machineConfig.isAutoLogin;
                    wifiResponse.data.MachineInfoEx.lastUpdateYear = machineConfig.lastUpdateYear;
                    wifiResponse.data.MachineInfoEx.lastUpdateMonth = machineConfig.lastUpdateMonth;
                    wifiResponse.data.MachineInfoEx.lastUpdateDay = machineConfig.lastUpdateDay;
                    wifiResponse.data.MachineInfoEx.lastUpdateHour = machineConfig.lastUpdateHour;
                    wifiResponse.data.MachineInfoEx.lastUpdateMinute = machineConfig.lastUpdateMinute;
                    wifiResponse.data.MachineInfoEx.lastUpdateSecond = machineConfig.lastUpdateSecond;
                    memcpy(wifiResponse.data.MachineInfoEx.lastUpdateTimezone,machineConfig.lastUpdateTimezone,Settings.StringMaxLength);
                    InitMessage( WIFI_LINGO_ASSET_MANAGEMENT, WIFI_ASSET_MANAGEMENT_MACHINE_INFO_EX, transactionId, sizeof(WIFIMachineInfoEX) );
                }
                else
                {
                    memset( &wifiResponse.data.MachineInfo, 0, sizeof( WIFIDataMachineInfo ) );
                    memcpy( wifiResponse.data.MachineInfo.serialNumber, machineConfig.consoleSerialNumber, Settings.StringMaxLength );
                    Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_FRAME_NUMBER );
                    memcpy( wifiResponse.data.MachineInfo.frameSerialNumber, machineConfig.frameSerialNumber, Settings.StringMaxLength );
                    Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_MACHINE_UUID );
                    memcpy( wifiResponse.data.MachineInfo.machineIdentification, machineConfig.machineUUID, Settings.StringMaxLength);
                    wifiResponse.data.MachineInfo.csafeId = machineConfig.csafeId;
                    wifiResponse.data.MachineInfo.csafeManufacture = machineConfig.csafeManufactureId;
                    wifiResponse.data.MachineInfo.csafeModelId = machineConfig.csafeModeId;
                    
                    wifiResponse.data.MachineInfo.machineType = machineConfig.machineType;
                    wifiResponse.data.MachineInfo.modelType = machineConfig.machineModel;
                    wifiResponse.data.MachineInfo.minMPHX10 = machineConfig.minSpeedMphX10;
                    wifiResponse.data.MachineInfo.maxMPHX10 = machineConfig.maxSpeedMphX10;
                    wifiResponse.data.MachineInfo.minKPHX10 = machineConfig.minSpeedKphX10;
                    wifiResponse.data.MachineInfo.maxKPHX10 = machineConfig.maxSpeedKphX10;
                    wifiResponse.data.MachineInfo.minInclineX10 = machineConfig.minInclineX10;
                    wifiResponse.data.MachineInfo.maxInclineX10 = machineConfig.maxInclineX10;
                    wifiResponse.data.MachineInfo.minLevel = machineConfig.minResistance;
                    wifiResponse.data.MachineInfo.maxLevel = machineConfig.maxResistance;
                    wifiResponse.data.MachineInfo.accumulatedSeconds = accumulalteddata.accumulatedTimeSeconds;
                    wifiResponse.data.MachineInfo.accumulatedDistanceMilesX100 = accumulalteddata.accumulatedDistanceMilesX100;
                    wifiResponse.data.MachineInfo.softwareVersion = (( (machineConfig.versionMajor<<8)|(machineConfig.versionMinor))<<8)|machineConfig.versionRevision;
                    wifiResponse.data.MachineInfo.wifiProtocolVersion = WIFI_PROTOCOL_VERSION;
                    wifiResponse.data.MachineInfo.wifiFirmwareVersion = wifi_settings.wifi_version.INT_VAL;
                    wifiResponse.data.MachineInfo.wifiUARTVersion = wifi_settings.uart_version.INT_VAL;
                    wifiResponse.data.MachineInfo.lcbVendor = 0;
                    wifiResponse.data.MachineInfo.lcbVersion = 0;
                    wifiResponse.data.MachineInfo.osVersion = 0;
                    wifiResponse.data.MachineInfo.numUsers = SystemConfig_Get_MaxUserNumber();
                    memcpy( wifiResponse.data.MachineInfo.macAddress, wifi_settings.mac_address, 6 );
                    memcpy( wifiResponse.data.MachineInfo.ipAddress, wifi_settings.ip_address, 6 );
                    
                    wifiResponse.data.MachineInfo.errorCode = 0;
                    wifiResponse.data.MachineInfo.serviceCode = 0;
                    wifiResponse.data.MachineInfo.motorTemperature = 0;
                    wifiResponse.data.MachineInfo.busCurrent = 0;
                    wifiResponse.data.MachineInfo.busVoltage = 0;
                    wifiResponse.data.MachineInfo.lubeMiles =accumulalteddata.accumulatedLubeMilesX100;
                    Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_TIME_ZONE );
                    
                    memset( wifiResponse.data.MachineInfo.timeZone, 0, Settings.StringMaxLength);
                    memcpy( wifiResponse.data.MachineInfo.timeZone, machineConfig.timezone, Settings.StringMaxLength);
                    wifiResponse.data.MachineInfo.AutoLogin = machineConfig.isAutoLogin;//temp
                    wifiResponse.data.MachineInfo.DapiVersion = SystemConfig_Get_Dapi_Version() ; 
                    InitMessage( WIFI_LINGO_ASSET_MANAGEMENT, WIFI_ASSET_MANAGEMENT_MACHINE_INFO, transactionId, sizeof(WIFIDataMachineInfo) );
                }
            }
#ifndef __HIGH_SPEED__
			break;
#endif
		case 1://0x0500
			if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
				wifi_response_substate++;
			break;
		case 2://0x0502
			if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)DapiInfo.Url, strlen( DapiInfo.Url ), DapiInfo.Secure ) )
				NextWiFiState();
			break;
		}
		break;
        
	case WCS_SELECT_PROGRAM:
        
		switch( wifi_response_substate )
		{
		case 0:
			wifi_response_substate++;
			wifiResponse.data.UWord = wifi_response_state[wifi_response_index].command;
            InitMessage( WIFI_LINGO_REMOTE, WIFI_SELECT_PROGRAM, transactionId, sizeof(UINT16) );
			break;
		case 1:
		case 2:
		case 3:
			if( wifi_settings.status.tcp_connected[wifi_response_substate] )
			{//0x0027
				if( SendSetTCPData( wifi_response_substate, (char*)&wifiResponse, wifiResponse.header.dataSize + sizeof(WifiHeader) ) )
					wifi_response_substate++;
			}
			else
				wifi_response_substate++;
            
			if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )//0x0500
				wifi_response_substate++;
			break;
		case 4:
			NextWiFiState();
			break;
        default:
			NextWiFiState();
            break;
		}
		break;
        
	case WCS_KEY_PRESS:
		switch( wifi_response_substate )
		{
		case 0:
			wifi_response_substate++;
			wifiResponse.data.UWord = wifi_response_state[wifi_response_index].command;
            InitMessage( WIFI_LINGO_REMOTE, WIFI_KEY_PRESS, transactionId, sizeof(UINT16) );
			break;
		case 1:
		case 2:
		case 3:
			if( wifi_settings.status.tcp_connected[wifi_response_substate] )
			{
				if( SendSetTCPData( wifi_response_substate, (char*)&wifiResponse, wifiResponse.header.dataSize + sizeof(WifiHeader) ) )
					wifi_response_substate++;
			}
			else
				wifi_response_substate++;
            
			if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )//0x0500
				wifi_response_substate++;
			break;
		case 4:
			NextWiFiState();
			break;
		}
		break;
        
	case WCS_REGISTER_USER:
		switch( wifi_response_substate )
		{
		case 0:
            /*
			wifi_response_substate++;
			memset( &wifiResponse.data, 0, sizeof(WifiCommandData) );
			wifiResponse.data.UserRegisterRequest.userId = _loginRequest.userId;
			memcpy( wifiResponse.data.UserRegisterRequest.xid, _loginRequest.xid, XIDMAXLENGTH );
			memcpy( wifiResponse.data.UserRegisterRequest.passcode, _loginRequest.passcode, 4 );
			memcpy( wifiResponse.data.UserRegisterRequest.macAddress, wifi_settings.mac_address, 6 );
			memcpy( wifiResponse.data.UserRegisterRequest.machineIdentification, machineConfig.machineUUID, 36 );
			memset( wifiResponse.data.UserRegisterRequest.userName, 0, 65 );
			memset( wifiResponse.data.UserRegisterRequest.email, 0, 65 );
			strcpy( wifiResponse.data.UserRegisterRequest.userName, "fufangpeng" );
			strcpy( wifiResponse.data.UserRegisterRequest.email, "fufangpeng@johnsonfitness.com");
			wifiResponse.data.UserRegisterRequest.gender = userFlashConfig[_loginRequest.userId]->gender;
			wifiResponse.data.UserRegisterRequest.weightX10 = userFlashConfig[_loginRequest.userId]->weightX10;
			wifiResponse.data.UserRegisterRequest.age = Information_GetUserAge( _loginRequest.userId );
			wifiResponse.data.UserRegisterRequest.year = userFlashConfig[_loginRequest.userId]->year;
			wifiResponse.data.UserRegisterRequest.month = userFlashConfig[_loginRequest.userId]->month;
			wifiResponse.data.UserRegisterRequest.day = userFlashConfig[_loginRequest.userId]->day;
			wifiResponse.data.UserRegisterRequest.language = userFlashConfig[_loginRequest.userId]->language;
            wifiResponse.data.UserRegisterRequest.AcceptedTerms = userFlashConfig[_loginRequest.userId]->AcceptedTerms;
            wifiResponse.data.UserRegisterRequest.WorkoutDataShare= userFlashConfig[_loginRequest.userId]->WorkoutDataShare;
            wifiResponse.data.UserRegisterRequest.DisplayUints = Information_Get_Data( IM_SPEED_MODE );
            */
			InitMessage( WIFI_LINGO_USER_MANAGEMENT, WIFI_USER_REGISTRATION, transactionId, sizeof(WIFIUserRegisterRequest) );
			break;
		case 1://0x0500
			if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
				wifi_response_substate++;
			break;
		case 2://0x0502
			if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)DapiInfo.Url, strlen( DapiInfo.Url ), DapiInfo.Secure ) )
				wifi_response_substate++;
			break;
		case 3:
			NextWiFiState();
			break;
		}
		break;
        
	case WCS_RESTORE_FACTORY:
		
		switch( wifi_response_substate )
		{
#if 0
        case 0:
            {
                SystemSettingType Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_MACHINE_UUID );
                wifi_response_substate++;
                
                memcpy( wifiResponse.data.ClearUsers.macAddress, wifi_settings.mac_address, 6 );
                memcpy( wifiResponse.data.ClearUsers.machineIdentification, machineConfig.machineUUID, Settings.StringMaxLength );
                InitMessage( WIFI_LINGO_USER_MANAGEMENT, WIFI_USER_CLEAR_USERS, transactionId, sizeof( WIFIClearUsers ) );
            }
            break;
        case 1://0x0500
            if(gWifiFlag.isOnLine == 0)
            {
                wifi_response_substate++;
            }
            else
            {
                if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
                    wifi_response_substate++;
            }
            break;
        case 2://0x0502
            if(gWifiFlag.isOnLine == 0)
            {
                wifi_response_substate++;
            }
            else
            {
                if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)DapiInfo.Url, strlen( DapiInfo.Url ),DapiInfo.Secure ) )
                    wifi_response_substate++;
            }
            break;
		case 3:
            {
                SystemSettingType settings = SystemConfig_Get_DefaultString(SETTING_DEFAULT_SSID);
                if(SendSetApInfor((char*)settings.strings,0,""))
                {
                    WaitForWIFIWriteFlash = timer;
                    wifi_response_substate++;
                    if(WiFiTest_Operation==1)
                    {
                        //_wifiStatus = WIFI_STATUS_AP_MODE;
                        WiFiTest_Operation=0;
                    }
                    else
                    {
                        _wifiStatus = WIFI_STATUS_ACTIVATED;
                        _wifiEventHandle = WIFI_EVENT_ACTIVATED;
                    }
                }
            }
            break;
            
		case 4:
			if( timer - WaitForWIFIWriteFlash >= 200 )
			{
				if( SendSetApStaMode( 0x11 ) )
				{
					WaitForWIFIWriteFlash = timer;
					wifi_response_substate++;
				}			
			}
			break;
		case 5:
			//if( SendSetApStaMode( 0x11 ) )
			if( timer - WaitForWIFIWriteFlash >= 200 )
            {
				wifi_response_substate++;
            }
			break;
        case 6:
			if( ResetWiFi() )
			{
                Digital_WIFI_Hardware_Reset();
                SystemSettingType settings = SystemConfig_Get_DefaultString(SETTING_DEFAULT_MACHINE_UUID);
				machineConfig.isAPModeOn= 1;
                strncpy( machineConfig.machineUUID, settings.strings,settings.StringMaxLength);
				memset(&DapiInfo , 0 , sizeof( DAPI_ADDRESS_INFO ));
				if(SystemConfig_Set_MachineInfo( machineConfig))
                {
                    SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH, NULL,0);
                }
                // init = false;
				wifi_response_substate++;
			}
			break;
        case 7:
            {
                
                init = false;
                NextWiFiState( );
            }
            break;
#else
		case 0:
			{
				SystemSettingType Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_MACHINE_UUID );
				wifi_response_substate++;
				{
					//if(stage == FRMAE_STATE_WIFI)
					if(WiFiTestingFlag==0)
					{
						_wifiEventHandle = WIFI_EVENT_ACTIVATED;
						_wifiStatus = WIFI_STATUS_ACTIVATED;
					}
				}
				
				memcpy( wifiResponse.data.ClearUsers.macAddress, wifi_settings.mac_address, 6 );
				memcpy( wifiResponse.data.ClearUsers.machineIdentification, machineConfig.machineUUID, Settings.StringMaxLength );
				InitMessage( WIFI_LINGO_USER_MANAGEMENT, WIFI_USER_CLEAR_USERS, transactionId, sizeof( WIFIClearUsers ) );
            }
#ifndef __HIGH_SPEED__
			break;
#endif
		case 1://0x0500
			if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
				wifi_response_substate++;
			break;
		case 2://0x0502
			if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)DapiInfo.Url, strlen( DapiInfo.Url ),DapiInfo.Secure ) )
            {
                WaitForWIFIWriteFlash = timer;
				wifi_response_substate++;
            }
			break;
		case 3:
            if( timer - WaitForWIFIWriteFlash >= 1000 )
            {
				if( SendSetSecurityKey_AP( "", 0 ) )
				{
					wifi_response_substate++;
					WaitForWIFIWriteFlash = 0;
				}
            }
			break;
		case 4:
            
            if( SendSetSecurityType_AP( 0 ) )
            {
                wifi_response_substate++;
                WaitForWIFIWriteFlash = timer;
            }
			break;
		case 5:
            if(timer - WaitForWIFIWriteFlash >= 200)
            {
                SystemSettingType settings = SystemConfig_Get_DefaultString(SETTING_DEFAULT_SSID);
				if( SendSetSSID_AP( settings.strings, strlen( settings.strings ) + 1 ) )
				{				
					wifi_response_substate++;
					WaitForWIFIWriteFlash = timer;
				}
            }
			break;
		case 6:
            if(timer - WaitForWIFIWriteFlash >= 200)
            {
                if( SendSetApStaMode( 0x11 ) )
                {			 			    
                    wifi_response_substate++;
                    WaitForWIFIWriteFlash = 0;
                }
            }
            break;
		case 7:
			if( ResetWiFi() )
			{
                
                SystemSettingType settings = SystemConfig_Get_DefaultString(SETTING_DEFAULT_MACHINE_UUID);
				machineConfig.isAPModeOn= 1;
                strncpy( machineConfig.machineUUID, settings.strings,settings.StringMaxLength);
				memset(&DapiInfo , 0 , sizeof( DAPI_ADDRESS_INFO ));
				if(SystemConfig_Set_MachineInfo( machineConfig))
                {
                    SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH, NULL,0);
                }
				init = false;
                
				wifi_response_substate++;
			}
			break;
            
		case 8:
			NextWiFiState();
			break;
#endif
            
		}
		break;
        
	case WCS_CHECK_XID:
		switch( wifi_response_substate )
		{
		case 0:
			wifi_response_substate++;
			memset( &wifiResponse.data.UserCheckXid, 0, sizeof(WIFIUserCheckXid) );
			memcpy( wifiResponse.data.UserCheckXid.xid, _loginRequest.xid, XIDMAXLENGTH );
            
			InitMessage( WIFI_LINGO_USER_MANAGEMENT, WIFI_USER_CHECK_XID, transactionId, sizeof(WIFIUserCheckXid) );
			break;
		case 1://0x0500
			if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
				wifi_response_substate++;
			break;
		case 2://0x0502
			if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)DapiInfo.Url, strlen( DapiInfo.Url ), DapiInfo.Secure ) )
				wifi_response_substate++;
			break;
		case 3:
			NextWiFiState();
			break;
		}
		break;
	case WCS_LOGIN_USER:
		switch( wifi_response_substate )
		{
		case 0:
            {
                SystemSettingType Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_MACHINE_UUID );
                wifi_response_substate++;
                memset( &wifiResponse.data.UserLoginRequest, 0, sizeof(WIFIUserLoginRequest) );
                memcpy( wifiResponse.data.UserLoginRequest.xid, _loginRequest.xid, XIDMAXLENGTH );
                memcpy( wifiResponse.data.UserLoginRequest.passcode, _loginRequest.passcode, 4 );
                memcpy( wifiResponse.data.UserLoginRequest.macAddress, wifi_settings.mac_address, 6 );
                memcpy( wifiResponse.data.UserLoginRequest.MachineID, machineConfig.machineUUID, Settings.StringMaxLength);
                wifiResponse.data.UserLoginRequest.userId = _loginRequest.userId;
                
                InitMessage( WIFI_LINGO_USER_MANAGEMENT, WIFI_USER_LOGIN, transactionId, sizeof(WIFIUserLoginRequest) );
            }
#ifndef __HIGH_SPEED__
			break;
#endif
		case 1://0x0500
			if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
				wifi_response_substate++;
			break;
		case 2://0x0502
			if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)DapiInfo.Url, strlen( DapiInfo.Url ), DapiInfo.Secure ) )
				wifi_response_substate++;
			break;
		case 3:
			NextWiFiState();
			break;
            
		}
		break;
        
	case WCS_ENABLE_AP:
		switch( wifi_response_substate )
		{
		case 0://0x0030
			if( SendSetSSID_AP( wifi_settings.ap_security.ssid, strlen( wifi_settings.ap_security.ssid ) + 1 ) )
				wifi_response_substate++;
			break;
		case 1:
			if( SendSetApStaMode( 0x55 ) )
				wifi_response_substate++;
			break;
		case 2:
			NextWiFiState();
			break;
        default:
			NextWiFiState();
            break;
		}
		break;
        
	case WCS_DISABLE_AP:
		switch( wifi_response_substate )
		{
		case 0:
			if( SendSetApStaMode( 0x33 ) )
				wifi_response_substate++;
			break;
		case 2:
			NextWiFiState();
			break;
        default:
			NextWiFiState();
            break;
		}
		break;
	case WCS_CONNECT_TO_ROUTER://connect to RD AP
		switch( wifi_response_substate )
		{
#if 0
		case 0:
            {
                if(SendSetStaInfor((char*)wifi_settings.security.ssid,wifi_settings.security.security_type,(char*)wifi_settings.security.security_key))
                {
                    WaitForWIFIWriteFlash = timer;
                    wifi_response_substate++;
                }
            }
            break;
            
		case 1:
			if( timer-WaitForWIFIWriteFlash >=160 )
			{
				if( SendSetApStaMode( 0x33 ) )
				{
                    WaitForWIFIWriteFlash = timer;
					wifi_response_substate++;
				}			
			}
			break;
		case 2:
			
			if( timer-WaitForWIFIWriteFlash >=160 )
            {
                if( ResetWiFi())//SendConnectAP() )
                    wifi_response_substate++;
            }
			break;
            //case 3:
#else
		case 0:
			if( SendSetSSID( wifi_settings.security.ssid, strlen( wifi_settings.security.ssid ) + 1 ) )
			{
				WaitForWIFIWriteFlash = timer;
				wifi_response_substate++;
			}			
			break;
		case 1:
			if( timer-WaitForWIFIWriteFlash >160 )
			{
				if( SendSetSecurityKey( (char*)wifi_settings.security.security_key, strlen( (char*)wifi_settings.security.security_key ) + 1 ) )
				{
					WaitForWIFIWriteFlash = timer;
					wifi_response_substate++;
				}			
			}
			break;
		case 2:
			if(timer-WaitForWIFIWriteFlash >160 )
			{
				if(SendSetSecurityType(wifi_settings.security.security_type))
				{
					WaitForWIFIWriteFlash = timer;
					wifi_response_substate++;
				}			
			}			
			break;
            
		case 3:
			if( timer-WaitForWIFIWriteFlash >160  )
			{
				if( SendSetApStaMode( 0x33 ) )
				{
					wifi_response_substate++;
				}			
			}
			break;
		case 4:
			if( SendConnectAP() )
				wifi_response_substate++;
			break;
#endif
        default:
			NextWiFiState();
            break;
		}
		break;
	case WCS_DISCONNECT_TO_ROUTER://disconnect to RD AP
		switch( wifi_response_substate )
		{
		case 0:
			if( SendDisconnectAP() )
				wifi_response_substate++;
			break;
		case 1:
			NextWiFiState();
			break;
		}
		break;
	case WCS_REGISTER_MACHINE:
		switch( wifi_response_substate )
		{
		case 0:
			wifi_response_substate++;
			memcpy( wifiResponse.data.RegisterRequest.macAddress, wifi_settings.mac_address, 6 );
            
			InitMessage( WIFI_LINGO_ASSET_MANAGEMENT, WIFI_ASSET_MANAGEMENT_MACHINE_REGISTRATION, transactionId, sizeof(WIFIMachineRegisterRequest) );
			break;
		case 1:
			if( SetHTTPData( (char*)&wifiResponse, sizeof(WifiHeader) + wifiResponse.header.dataSize ) )
				wifi_response_substate++;
			break;
		case 2:
			if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)DapiInfo.Url, strlen( DapiInfo.Url ), DapiInfo.Secure ) )
            {
    			gWifiFlag.isSendMachineRegister= 1;
                WaitForWIFIWriteFlash = timer;
				wifi_response_substate++;
            }
			break;
		case 3:
			NextWiFiState();
            break;
		}
		break;
	case WCS_GET_SIGNAL_STRENGTH:
		switch( wifi_response_substate )
		{
		case 0:
			if( SendGetSignalStrength() )
				wifi_response_substate++;
			break;
		case 1:
			NextWiFiState();
			break;
		}
		break;
	case WCS_LEAVE_PROGRAMMING_MODE:
		switch( wifi_response_substate )
		{
		case 0:
			if( SendLeaveProgrammingMode() )
				wifi_response_substate++;
			break;
		case 1:
			NextWiFiState();
			break;
		}
		break;
    case WCS_RESEND_0502:
        /*
        if( (_workoutState != WIFI_WORKOUT_RUNNING)&& (_workoutState != WIFI_WORKOUT_PAUSED ))
        {
  		wifi_response_substate++;
  	}
        */
        
        switch( wifi_response_substate )
        {
        case 0:
            if( SendHTTPData( HTTP_POST, HTTP_APPLICATION_OCTET_STREAM, (char*)DapiInfo.Url, strlen( DapiInfo.Url ), DapiInfo.Secure ) )
                wifi_response_substate++;
            break;
        case 1:
            NextWiFiState();
            break;
        }
        break;
	}
}

void ProcessTCPData( int port, char* data, int data_size )//0x0028  receive data
{
    
	UINT16 readSize = 0;
    
	if( port < 1 || port > 3 )return;
    
	 time_out = 0; 
      
    
	while( data_size > 0 || tcpDataBufferIndex[port - 1] >= sizeof(WifiHeader) )
	{
		if( data_size > 0 )
		{
			readSize = AddTCPDataToBuffer( port - 1, data, data_size );
			data_size -= readSize;
			data += readSize;
			tcpDataBufferIndex[port - 1] += readSize;
		}
        
		for( int i = 0; i < tcpDataBufferIndex[port - 1] - 1; i++ )
		{
			WifiHeader* header = (WifiHeader*)( tcpDataBuffer[port - 1] + i );
			if( header->syncWord == 0xAA55 )
			{
				if( i != 0 )
				{
					for( int j = 0; /*j < i &&*/ i + j < tcpDataBufferIndex[port - 1]; j++ )
						tcpDataBuffer[port - 1][j] = tcpDataBuffer[port - 1][i + j];
					tcpDataBufferIndex[port - 1] -= i;
				}
				break;
			}
		}
        
		if( tcpDataBufferIndex[port - 1] >= sizeof(WifiHeader) )
		{
			WifiCommand* command = (WifiCommand*)tcpDataBuffer[port - 1];
			int messageSize = command->header.dataSize + sizeof(WifiHeader);
            
			if( command->header.dataSize + sizeof(WifiHeader) <= tcpDataBufferIndex[port - 1] )
			{
				transactionId = command->header.transactionId;
				ProcessExternalMessage( port, command );//--decode--
				for( int i = 0; /*i < messageSize &&*/ i + messageSize < tcpDataBufferIndex[port - 1]; i++ )
					tcpDataBuffer[port - 1][i] = tcpDataBuffer[port - 1][i + messageSize];
				tcpDataBufferIndex[port - 1] -= messageSize;
			}
			else
            {
                tcpDataBuffer[port - 1][0] = 0x11;
				break;
            }
		}
	}
    
}

void UpdateNetworkTime()
{
	AddWifiResponseState( WCS_UPDATE_TIME, 0, 0, 0 );
}

void UpdateUserInfo()
{
	if( gWifiFlag.isOnLine )
	{
		_userDataResponse.userInfoUpdated = 0;
		AddWifiResponseState( WCS_UPDATE_USER_INFO, 0, 0, 0 );
        _wifiStatus = WIFI_STATUS_SYNC_USER;
        _wifiEventHandle = WIFI_EVENT_SYNC_USER;            
        status_timer = Digital_WIFI_Get_10ms_Timer( );
	}
	else
		_userDataResponse.userInfoUpdated = 2;
}

UINT8 CheckUserInfoUpdated()
{
	return _userDataResponse.userInfoUpdated;
    
}

void UpdateMachineInfo()
{
	AddWifiResponseState( WCS_UPDATE_MACHINE_INFO, 0, 0, 0 );
}

void KeyPressed( UINT8 keycode )
{
	AddWifiResponseState( WCS_KEY_PRESS, keycode, 0, 0 );
}

WIFI_REMOTE_MODE GetWifiMode()
{
	return _remoteControlState;
    
}

void RegisterUser( unsigned char userId, unsigned char* xid, unsigned char* passcode )
{
	memcpy( _loginRequest.xid, xid, XIDMAXLENGTH );
	memcpy( _loginRequest.passcode, passcode, 4 );
	_loginRequest.userId = userId;
	_userDataResponse.registrationCheckResponse = 0;
    
	AddWifiResponseState( WCS_REGISTER_USER, 0, 0, 0 );
    
}

UINT8 CheckRegistration()
{
	return _userDataResponse.registrationCheckResponse;
}

#if 0
void CheckXidUnique( unsigned char* xid )
{
	memcpy( _loginRequest.xid, xid, XIDMAXLENGTH );
	_userDataResponse.xidCheckResponse = 0;
	AddWifiResponseState( WCS_CHECK_XID, 0, 0, 0 );
    
}

UINT8 CheckXidUniqueResult()
{
	return _userDataResponse.xidCheckResponse;
    
}
#endif

void Login( unsigned char userId, unsigned char* xid, unsigned char* passcode )
{
	if( wifi_settings.status.wifi_connected && xid[0] != 0 )
	{
		memcpy( _loginRequest.xid, xid, XIDMAXLENGTH );
		memcpy( _loginRequest.passcode, passcode, 4 );
		_loginRequest.userId = userId;
        /* This function returns the result of the login.  A 0 means it is still processing.
        * A 1 means success and anything else is an error.*/
		_userDataResponse.loginCheckResponse = 0;
		AddWifiResponseState( WCS_LOGIN_USER, 0, 0, 0 );
        _wifiStatus= WIFI_STATUS_LOGIN;
        _wifiEventHandle = WIFI_EVENT_LOGIN;
        status_timer = Digital_WIFI_Get_10ms_Timer( );
	}
	else
        /* This function returns the result of the login.  A 0 means it is still processing.
        * A 1 means success and anything else is an error.*/
		_userDataResponse.loginCheckResponse = 2;
    
}

UINT8 CheckLoginResult()
{
    /* This function returns the result of the login.  A 0 means it is still processing.
    * A 1 means success and anything else is an error.*/
	return _userDataResponse.loginCheckResponse;
}

void ChangeWorkoutState( WORKOUT_STATE state )
{
    MACHINE_INFO machineConfig = SystemConfig_Get_MachineInfo( );
    
#if 1
	if( ( _workoutState==WIFI_WORKOUT_PAUSED||_workoutState == WIFI_WORKOUT_IDLE || _workoutState == WIFI_WORKOUT_STOP ) && state == WIFI_WORKOUT_RUNNING )
	{
        AddWifiResponseState( WCS_SET_START_WORKOUT, 0, 0, 1 );
#ifdef __WIFI_TEST_PETER__
        uartLostCounter = 0;
        uartHadSent = 0;
        recvResponse = 0;
        uartSend502 = 0;
        InitTest( );
#endif
#endif
        
#ifdef __WIFI_NO_RTC__
        _workoutStartTime = ReadNetworkTime(GetNetworkDelay());
#else
        RTC_Read_Time( &_workoutStartTime );
#endif
        memset( &wokoutCompleteData , 0 , sizeof(wokoutCompleteData) );            
        memset( &workouData, 0 , sizeof(workouData) );
	}
	else if( ( _workoutState == WIFI_WORKOUT_RUNNING || _workoutState == WIFI_WORKOUT_PAUSED ) && state == WIFI_WORKOUT_STOP )
    {
        AddWifiResponseState( WCS_WORKOUT_COMPLETE_EX, 0, 0, 1 );
        SystemSettingType Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_MACHINE_UUID );
        if( SystemConfig_Get_Dapi_Version( )>9)
        {
            GetWorkoutDataEx( &workouData.WorkdataTrackEx );
            memcpy( workouData.WorkdataTrackEx.userExternalId, _UserExternalID, 36 );
            memcpy( workouData.WorkdataTrackEx.macAddress, wifi_settings.mac_address, 6 );
            memcpy(workouData.WorkdataTrackEx.MachineID,machineConfig.machineUUID, Settings.StringMaxLength);
            workouData.WorkdataTrackEx.workoutStartYear = _workoutStartTime.year ;
            workouData.WorkdataTrackEx.workoutStartYear = workouData.WorkdataTrackEx.workoutStartYear+ 2000;
            workouData.WorkdataTrackEx.workoutStartMonth= (UINT8)_workoutStartTime.month;
            workouData.WorkdataTrackEx.workoutStartDay= (UINT8)_workoutStartTime.date;
            workouData.WorkdataTrackEx.workoutStartHour= (UINT8)_workoutStartTime.hours;
            workouData.WorkdataTrackEx.workoutStartMinute= (UINT8)_workoutStartTime.minutes;
            workouData.WorkdataTrackEx.workoutStartSecond= (UINT8)_workoutStartTime.seconds;			
            GetWorkoutDataCompleteEx(&wokoutCompleteData.wkDataCompleteEx);
            memcpy( wokoutCompleteData.wkDataCompleteEx.userExternalId, _UserExternalID, 36 );
            memcpy( wokoutCompleteData.wkDataCompleteEx.macAddress, wifi_settings.mac_address, 6 );
            memcpy(wokoutCompleteData.wkDataCompleteEx.MachineID,machineConfig.machineUUID, Settings.StringMaxLength);
            wokoutCompleteData.wkDataCompleteEx.workoutStartYear = _workoutStartTime.year ;
            wokoutCompleteData.wkDataCompleteEx.workoutStartYear = wokoutCompleteData.wkDataCompleteEx.workoutStartYear + 2000;
            wokoutCompleteData.wkDataCompleteEx.workoutStartMonth= (UINT8)_workoutStartTime.month;
            wokoutCompleteData.wkDataCompleteEx.workoutStartDay= (UINT8)_workoutStartTime.date;
            wokoutCompleteData.wkDataCompleteEx.workoutStartHour= (UINT8)_workoutStartTime.hours;
            wokoutCompleteData.wkDataCompleteEx.workoutStartMinute= (UINT8)_workoutStartTime.minutes;
            wokoutCompleteData.wkDataCompleteEx.workoutStartSecond= (UINT8)_workoutStartTime.seconds;
            
            if( Program_Get_ProgramID( ) == PS_SPRINT8 )
            {
                memcpy( completeSprint8Data.userExternalId, _UserExternalID, 36 );
                memcpy( completeSprint8Data.macAddress, wifi_settings.mac_address, 6 );
                memcpy(completeSprint8Data.MachineID,machineConfig.machineUUID, Settings.StringMaxLength);
                completeSprint8Data.workoutStartYear = _workoutStartTime.year ;
                completeSprint8Data.workoutStartYear = completeSprint8Data.workoutStartYear + 2000;
                completeSprint8Data.workoutStartMonth= (UINT8)_workoutStartTime.month;
                completeSprint8Data.workoutStartDay= (UINT8)_workoutStartTime.date;
                completeSprint8Data.workoutStartHour= (UINT8)_workoutStartTime.hours;
                completeSprint8Data.workoutStartMinute= (UINT8)_workoutStartTime.minutes;
                completeSprint8Data.workoutStartSecond= (UINT8)_workoutStartTime.seconds;
                GetWorkoutDataSprint8Extra(&completeSprint8Data);
            }
            if( Program_Get_ProgramID( ) == PS_FITNESS_TEST )
            {
                memcpy( completeFitnessTestData.userExternalId, _UserExternalID, 36 );
                memcpy( completeFitnessTestData.macAddress, wifi_settings.mac_address, 6 );
                memcpy(completeFitnessTestData.MachineID,machineConfig.machineUUID, Settings.StringMaxLength);
                completeFitnessTestData.workoutStartYear = _workoutStartTime.year ;
                completeFitnessTestData.workoutStartYear = completeFitnessTestData.workoutStartYear + 2000;
                completeFitnessTestData.workoutStartMonth= (UINT8)_workoutStartTime.month;
                completeFitnessTestData.workoutStartDay= (UINT8)_workoutStartTime.date;
                completeFitnessTestData.workoutStartHour= (UINT8)_workoutStartTime.hours;
                completeFitnessTestData.workoutStartMinute= (UINT8)_workoutStartTime.minutes;
                completeFitnessTestData.workoutStartSecond= (UINT8)_workoutStartTime.seconds;
                GetWorkoutDataFitnessTestExtra(&completeFitnessTestData);
            }
            AddWifiResponseState( WCS_WORKOUT_COMPLETE_EX, 0, 0, 0 );
            //  AddWifiResponseState( WCS_UPDATE_MACHINE_STATS, 0, 0, 0 );
        }
        else
        {
            memcpy( workouData.WorkdataTrack.userExternalId, _UserExternalID, 36 );				
            memcpy( workouData.WorkdataTrack.macAddress, wifi_settings.mac_address, 6 );
            memcpy(workouData.WorkdataTrack.MachineID,machineConfig.machineUUID,Settings.StringMaxLength );
            workouData.WorkdataTrack.year = _workoutStartTime.year;
            workouData.WorkdataTrack.year = workouData.WorkdataTrack.year+ 2000;
            workouData.WorkdataTrack.month = (UINT8)_workoutStartTime.month;
            workouData.WorkdataTrack.day = (UINT8)_workoutStartTime.date;
            workouData.WorkdataTrack.hour = (UINT8)_workoutStartTime.hours;
            workouData.WorkdataTrack.minute = (UINT8)_workoutStartTime.minutes;
            workouData.WorkdataTrack.second = (UINT8)_workoutStartTime.seconds;
            GetWorkoutData( &workouData.WorkdataTrack );
            memcpy( wokoutCompleteData.wkDataComplete.userExternalId, _UserExternalID, 36 );				
            memcpy( wokoutCompleteData.wkDataComplete.macAddress, wifi_settings.mac_address, 6 );
            memcpy( wokoutCompleteData.wkDataComplete.MachineID, machineConfig.machineUUID, Settings.StringMaxLength);
            wokoutCompleteData.wkDataComplete.year = _workoutStartTime.year ;
            wokoutCompleteData.wkDataComplete.year  = wokoutCompleteData.wkDataComplete.year+2000;
            wokoutCompleteData.wkDataComplete.month = (UINT8)_workoutStartTime.month;
            wokoutCompleteData.wkDataComplete.day = (UINT8)_workoutStartTime.date;
            wokoutCompleteData.wkDataComplete.hour = (UINT8)_workoutStartTime.hours;
            wokoutCompleteData.wkDataComplete.minute = (UINT8)_workoutStartTime.minutes;
            wokoutCompleteData.wkDataComplete.second = (UINT8)_workoutStartTime.seconds;
            GetWorkoutDataComplete( &wokoutCompleteData.wkDataComplete );
            AddWifiResponseState( WCS_COMPLETE_WORKOUT, 0, 0, 0 );
        }
    }
	_workoutState = state;
}

void EnableAP( char* ssid, UINT16 ssid_length )
{
	memset( wifi_settings.ap_security.ssid, 0, 33 );
	if( ssid_length > 33 )
		strncpy( wifi_settings.ap_security.ssid, ssid, 33 );
	else
		strncpy( wifi_settings.ap_security.ssid, ssid, ssid_length );
	AddWifiResponseState( WCS_ENABLE_AP, 0, 0, 0 );
}

void DisableAP()
{
	AddWifiResponseState( WCS_DISABLE_AP, 0, 0, 0 );
}

void ConnectWIFI( const char* ssid, UINT16 ssid_length, const char* security_key, UINT16 security_key_length )
{
	memset( wifi_settings.security.security_key, 0, 255 );
	if( security_key_length > 255 )
		strncpy( (char *)wifi_settings.security.security_key, security_key, 255 );
	else
		strncpy((char *) wifi_settings.security.security_key, security_key, security_key_length );
    
	memset( wifi_settings.security.ssid, 0, 33 );
	if( ssid_length > 33 )
		strncpy((char *) wifi_settings.security.ssid, ssid, 33 );
	else
		strncpy( (char *)wifi_settings.security.ssid, ssid, ssid_length );
	wifi_settings.security.security_type = WIFI_SECURITY_UNSECURED;
	AddWifiResponseState( WCS_CONNECT_TO_ROUTER, 0, 0, 0 );
    
}

void DisconnectWIFI()
{
	AddWifiResponseState( WCS_DISCONNECT_TO_ROUTER, 0, 0, 0 );
}

void ClearFactoryTestStatus( void )
{
	WiFiTestingFlag = 0;
	WiFiTestStage = 0 ;
	WiFiTestDelay = 0;
	_wifiStatus = WIFI_STATUS_IDLE;
}
void ConnectAPForFactoryTest( void )
{
	WiFiTestingFlag = 1;
	WIFIClearStateBuffer();
	ConnectWIFI(FACTORY_TEST_SSID,strlen(FACTORY_TEST_SSID),FACTORY_TEST_PW,strlen(FACTORY_TEST_PW));
	WiFiTestDelay = 0 ;
	WiFiTestStage  = 1 ;
}

void RegisterMachine(void)
{
	AddWifiResponseState( WCS_REGISTER_MACHINE, 0, 0, 0 );
}

void RestoreWifiToFactoryDefaults()
{
	///memset(&wifi_settings,0,sizeof(WIFI_SETTINGS));
    
    memset( &wifi_settings.ip_address, 0, 6 );
    memset( &wifi_settings.security, 0, sizeof(WIFI_SECURITY_SETTINGS) );
    memset( &wifi_settings.ap_security, 0, sizeof(WIFI_SECURITY_SETTINGS) );
    memset( &wifi_settings.tcp_settings, 0, 3 * sizeof(WIFI_TCP_SETTINGS) );
    
#if 0
    memset( &wifi_settings.mac_address, 0, 6 );
#endif
    WIFIClearStateBuffer();
    
    
    status_timer = Digital_WIFI_Get_10ms_Timer( );
    gWifiFlag.isOnLine = 0;//after Set AP,cancel update user info     
	AddWifiResponseState( WCS_RESTORE_FACTORY, 0, 0, 0 );
}

UINT8 WIFIIsOnline(void)
{
	return gWifiFlag.isOnLine;
    //return ((DapiInfo.Url[0]!=0)&&(DapiInfo.Url[0]!=0xFF));
}
UINT8 WIFIIsAPMode(void)
{
    MACHINE_INFO machineConfig = SystemConfig_Get_MachineInfo( );
    if(WIFIIsOnline( ))
    {
    	machineConfig.isAPModeOn= 0;
    	return machineConfig.isAPModeOn;
    }
    else
        return (machineConfig.isAPModeOn==1);
}

UINT8 WIFISignalStrength(void)
{
    if(WIFIIsOnline( ))
    {
        if(wifi_settings.status.wifi_connected)
        {	
            //external_data_timer = true;
            if(wifi_settings.signalStrength == 0)
                return 3;
            else
                return (wifi_settings.signalStrength);
        }
        else
        {
            //memset( &DapiInfo , 0, sizeof(DAPI_ADDRESS_INFO) );
            init = false;
            return 0;
        }
    }
    else
    {
        return 0;
    }
}
UINT8 WIFIGetDeleteResult(void)
{
	UINT8 Result = _userDataResponse.DeleteUserCheckResponse;
	return Result;
}
void WIFIDeleteUser(char index)
{
    _userDataResponse.DeleteUserCheckResponse = 0;
    AddWifiResponseState( WCS_UNLINK_USER, index, 0, 0 );
}

void WIFIClearStateBuffer(void)
{
    memset(wifi_response_state,0x00,sizeof(WifiResponseState)*TOTAL_RESPONSE_NUMBER);
    wifi_response_index=0;
    wifi_next_response_index = 0;
    wifi_response_substate=0;
    wifi_num_pending_responses = 0;
}

void WIFISendSignalStrengthCommand(void)
{    
    //  if( (_workoutState != WIFI_WORKOUT_RUNNING)&& (_workoutState != WIFI_WORKOUT_PAUSED )
    //		&&( _wifiStatus != WIFI_STATUS_DOWNLOADING_SOFTWARE))
    AddWifiResponseState( WCS_GET_SIGNAL_STRENGTH , 0 , 0 , 0 );
}
void WIFISendLeave_Programming_Mode(void)
{    
    WIFIClearStateBuffer();
    AddWifiResponseState( WCS_LEAVE_PROGRAMMING_MODE , 0 , 0 , 0 );
}
MARIO_BOX_TYPE WifiGetMarioBox(UINT8 index)
{
    UCHAR temp = index%MARIO_BOX_TYPE_END;
    return (MARIO_BOX_TYPE)(MARIO_BOX_PRODUCTION+temp);
}


UINT8 WifiSetMarioBox(MARIO_BOX_TYPE index)
{  
    UCHAR temp = index%MARIO_BOX_TYPE_END;        
    SystemSettingType systemsettings = SystemConfig_Get_DefaultString((SystemSettingString)(SETTING_MARIOX_PRODUCTION_ADDRESS+temp));
    MACHINE_INFO machineConfig=SystemConfig_Get_MachineInfo( );  
    
    strcpy(machineConfig.marioBox , systemsettings.strings);
    SystemConfig_Set_MachineInfo( machineConfig );
    Digital_WIFI_Hardware_Reset();
    return index;
    
}

MARIO_BOX_TYPE WifiGetCurrentMarioBox(void)
{     
    SystemSettingType systemsettings ;
    MACHINE_INFO machineConfig=SystemConfig_Get_MachineInfo( );
    UCHAR ret = MARIO_BOX_TYPE_START;    
    for(;ret<MARIO_BOX_TYPE_END;ret++)
    {
        systemsettings = SystemConfig_Get_DefaultString((SystemSettingString)(SETTING_MARIOX_PRODUCTION_ADDRESS+ret));
        if( strcmp(systemsettings.strings, machineConfig.marioBox ) == 0 )
        {
            break;
        }
    }
    
    return (MARIO_BOX_TYPE)ret;
}
WIFI_STATUS_TYPE wifiGetStatus(void)
{
    WIFI_STATUS_TYPE ret = _wifiStatus;
    //  _wifiStatus = WIFI_EVENT_IDLE;
    return ret;
}

WIFI_EVENT_HANDLE wifiGetEvent(void)
{
	WIFI_EVENT_HANDLE ret = _wifiEventHandle;
	//if(stage ==FRAME_STAGE_PRGRUN )
    if(Program_Get_Status()> PS_SETUP )
	{
		ret = WIFI_EVENT_IDLE;
	}
	else
	{
		_wifiEventHandle = WIFI_EVENT_IDLE;
	}
	return ret;
}



/* For Dapi Version 10 Start */

void wifiSetFirmwareUpdate(DEPLOYMENT_TYPE deploy , SOFTWARE_TYPE software)
{
    remoteUpdateDeploymentType = deploy;
    remoteUpdateSoftwareType = software;
    AddWifiResponseState( WCS_GET_LATEST_VERSION, 0, 0, 0);
}

void wifiSetRFidLogin(UINT8 userIndex, UINT8* rfidTag, RFIDCARRIER rfidCarrier)
{
    rfiddata.userIndex = userIndex;
    memcpy(rfiddata.rfidTag,rfidTag,16);
    rfiddata.rfidCarrier = rfidCarrier;    
    AddWifiResponseState( WCS_RFID_LOGIN, 0, 0, 0);
}
void wifiSetRFidAssociate(UINT8 userIndex, UINT8* rfidTag, RFIDCARRIER rfidCarrier)
{
    rfiddata.userIndex = userIndex;
    memcpy(rfiddata.rfidTag,rfidTag,16);
    rfiddata.rfidCarrier = rfidCarrier;    
    AddWifiResponseState( WCS_RFID_ASSOCIATE, 0, 0, 0);
}

void wifiSetRFidDisassociate(UINT8* rfidTag, RFIDCARRIER rfidCarrier)
{
    memcpy(rfiddata.rfidTag,rfidTag,16);
    rfiddata.rfidCarrier = rfidCarrier;    
    AddWifiResponseState( WCS_RFID_DISASSOCIATE, 0, 0, 0);
}

void wifiSetErrorCode(UINT16 errorCode)
{
    
    AddWifiResponseState(WCS_OCCURRED_ERROR_CODE,errorCode,0,0);
}

void wifiSetServiceCode(UINT16 serviceCode)
{
    
    AddWifiResponseState(WCS_OCCURRED_SERVICE_CODE,serviceCode,0,0);
}

/*
void wifiSetTimeZone(UCHAR timeZoneIndex)
{
MACHINE_INFO machineConfig = SystemConfig_Get_MachineInfo();
SystemSettingType Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_TIME_ZONE );
memset(machineConfig.timezone,0,Settings.StringMaxLength);
memcpy(machineConfig.timezone,SystemConfig_Get_Timezone(timeZoneIndex),Settings.StringMaxLength);
SystemConfig_Get_MachineInfo(machineConfig);
}
*/
/* For Dapi Version 10 End */
void WifiEditUser(UINT8 userindex,UINT16 weightX100,UINT8 age,UINT8 gender)
{
    USERS_INFO userinfo = SystemConfig_Get_UserInfo( userindex );
    DateTime_T now;
#ifdef __WIFI_NO_RTC__
    now= ReadNetworkTime(GetNetworkDelay());
#else
    RTC_Read_Time(&now);
#endif
    
    userinfo.age = age;
    userinfo.weightStandardX100 = weightX100;
    userinfo.gender = gender;
    userinfo.timeStampYear= now.year + 2000;
    userinfo.timeStampMonth= now.month;
    userinfo.timeStampDate= now.date;
    userinfo.timeStampHour= now.hours;
    userinfo.timeStampMinute= now.minutes;
    userinfo.timeStampSeconds= now.seconds;
    if( SystemConfig_Set_UserInfo(userinfo,userindex))
    {
        SystemConfigProcess(SVK_SAVE_USER2FLASH, &userindex,sizeof(userindex));
        //        AddWifiResponseState(WCS_UPDATE_USER,userindex,0,0);
        AddWifiResponseState( WCS_UPDATE_USER_INFO, 0, 0, 0 );
    }
    
}
UINT8 WiFi_Get_Update_Percent(void)
{
	return Get_ProgramProgress();
}
void wifiTestRSCU(UINT8 softtype,char* md5)
{
    memset(&_softwareUpdate,0,sizeof(WIFIDATAResponseLatestVersion));
    strcpy(_softwareUpdate.SoftwareType,softwaretype[softtype]);
    
    //   strcpy(_softwareUpdate.SoftwareUrl,"http://192.168.129.121/filezilla/upload/EFM.bin");
    //  strcpy(_softwareUpdate.SoftwareUrl,"https://raw.githubusercontent.com/rupesh1mb/CopyFileTest/master/EFM.bin");
    //    strcpy(_softwareUpdate.SoftwareUrl,"https://s3-us-west-2.amazonaws.com/jhtee1.jhtdev.co/EFM.bin");
    strcpy(_softwareUpdate.SoftwareUrl,"http://dev-software.jfit.co/568f16f45014c54c068b456f");
    strncpy(_softwareUpdate.MD5,md5,32);
    AddWifiResponseState(WCS_SET_UPDATE_URL,0,0,0);
}

void WIFI_get_status(void)
{
	AddWifiResponseState(WCS_GET_STATUS, 0, 0, 0);
}

#ifdef __WIFI_TEST_PETER__
UINT16 WIFI_Get_uartLostCounter(void)
{
	return uartLostCounter;
}
UINT16 WIFI_Get_uartHadSent(void)
{
	return uartHadSent;
}
UINT16 WIFI_Get_recvResponse(void)
{
	return recvResponse;
}
#endif

#endif
/* http://192.168.129.121/filezilla/upload/EFM.bin
02 68 74 74 70 3A 2F 2F 31 39 32 2E 31 36 38 2E 31 32 39 2E 31 32 31 2F 66 69 6C 65 7A 69 6C 6C 61 2F 75 70 6C 6F 61 64 2F 45 46 4D 2E 62 69 6E 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 61 38 64 62 36 63 30 61 65 36 34 37 61 35 32 65 66 31 34 65 36 31 34 31 38 63 36 64 34 37 31 35
*/
/*
02 68 74 74 70 3A 2F 2F 31 39 32 2E 31 36 38 2E 31 32 39 2E 31 32 31 2F 66 69 6C 65 7A 69 6C 6C 61 2F 75 70 6C 6F 61 64 2F 45 46 4D 2E 62 69 6E 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 32 34 38 37 34 32 38 31 34 34 63 31 30 36 37 39 62 61 39 65 34 64 30 38 35 33 38 39 33 36 35 32
*/
/*640000600000*/

/*https://s3-us-west-2.amazonaws.com/jhtee1.jhtdev.co/EFM.bin
02 68 74 74 70 73 3A 2F 2F 73 33 2D 75 73 2D 77 65 73 74 2D 32 2E 61 6D 61 7A 6F 6E 61 77 73 2E 63 6F 6D 2F 6A 68 74 65 65 31 2E 6A 68 74 64 65 76 2E 63 6F 2F 45 46 4D 2E 62 69 6E 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 61 38 64 62 36 63 30 61 65 36 34 37 61 35 32 65 66 31 34 65 36 31 34 31 38 63 36 64 34 37 31 35
*/
