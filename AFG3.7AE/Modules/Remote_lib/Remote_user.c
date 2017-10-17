#include"remote_user.h"
#ifdef __REMOTE_USER_MANAGE__
#include "Global_Config.h"
#include "SystemConfig.h"
#include "Remote_datatype.h"
#include "Remote_process.h"
#include "remote_port.h"





SubProcessState RemoteUserProcess(RemoteDevicesInformation *deviceinfor, UINT8* buf, UINT16 *bufLength,const  DapiGlobalVariable* dapiGlobalVariable)
{
    SubProcessState ret = SUB_PROCESS_STATE_NOT_SUPPORTED;
//    UINT8 flag = deviceinfor->currentState>>8;
    UINT8 command = (UINT8)(deviceinfor->currentState&0x00ff);
    MACHINE_INFO machineConfig=SystemConfig_Get_MachineInfo( );     
    SystemSettingType Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_MACHINE_UUID );
    RemoteHeader *remoteHeader = (RemoteHeader*) buf;


    remoteHeader->syncWord = 0xAA55;
    remoteHeader->lingoId = DAPI_LINGO_USER_MANAGEMENT;
    remoteHeader->messageId = command;
    
    switch(command)
    {
        case REMOTE_USER_MANAGEMENT_ACKNOWLEDGMENT:
        {
        }
        break;
        case REMOTE_SYNC_USER_INFO:
        {
            UINT8 totalUser = SystemConfig_Get_MaxUserNumber();
            if(deviceinfor->currentSubState< totalUser )
            {
                ret = SUB_PROCESS_STATE_NEXT_STATE;
            }
            else
            {
                USERS_INFO userinfo = SystemConfig_Get_UserInfo( deviceinfor->currentSubState );
                UserSyncRequestData *requestdata =(UserSyncRequestData*)( buf+ sizeof(RemoteHeader));   
                
                *bufLength = sizeof(UserSyncRequestData)+sizeof(RemoteHeader);   
                requestdata->userConsoleId = deviceinfor->currentSubState;
                memcpy( requestdata->xid, userinfo.xid, XIDMAXLENGTH );
                memcpy( requestdata->passcode, userinfo.passcode, 4 );
                memcpy( requestdata->macAddress, deviceinfor->deviceMacAddress, 6 );
                memcpy( requestdata->machineIdentification, machineConfig.machineUUID,Settings.StringMaxLength);
                requestdata->lastChangeYear = userinfo.timeStampYear;
                requestdata->lastChangeMonth = userinfo.timeStampMonth;
                requestdata->lastChangeDay = userinfo.timeStampDate;
                requestdata->lastChangeHour = userinfo.timeStampHour;
                requestdata->lastChangeMinute = userinfo.timeStampMinute;
                requestdata->lastChangeSecond = userinfo.timeStampSeconds;
                requestdata->gender = userinfo.gender;
                requestdata->weightX10 = userinfo.weightStandardX100/10;
                requestdata->age = userinfo.age;
                requestdata->year = userinfo.year;
                requestdata->month = userinfo.month;
                requestdata->day = userinfo.date;
                requestdata->language = userinfo.languageId;
                requestdata->WorkoutDataShare = userinfo.isShareWorkouts;
                requestdata->DisplayUnits = userinfo.displayUnit;
                requestdata->AcceptedTerms = userinfo.isAcceptedTerms;
                memcpy( requestdata->name, userinfo.name, 65 );
                requestdata->UserSetup= userinfo.isSetup;                
                ret = SUB_PROCESS_STATE_NEXT_SUBSTATE;
            }
        }
        break;
        case REMOTE_USER_LOGIN:
        {
            switch(deviceinfor->currentSubState)
            {
                case 0:
                {                    
                    UserLoginRequest *userLoginRequest = (UserLoginRequest*)buf+ sizeof(RemoteHeader);
                    memcpy( userLoginRequest->xid, dapiGlobalVariable->dataInfor.loginInfor._loginRequest.xid, XIDMAXLENGTH );
                    memcpy( userLoginRequest->passcode, dapiGlobalVariable->dataInfor.loginInfor._loginRequest.passcode, 4 );
                    memcpy( userLoginRequest->macAddress, deviceinfor->deviceMacAddress, 6 );
                    memcpy( userLoginRequest->MachineID, machineConfig.machineUUID, Settings.StringMaxLength);
                    userLoginRequest->userId = dapiGlobalVariable->dataInfor.loginInfor._loginRequest.userId;
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
        case REMOTE_USER_UNLINK_USER:
        {
            switch(deviceinfor->currentSubState)
            {
                case 0:
                {
                    UnlinkUser *unlinkUser = ( UnlinkUser*)(buf+ sizeof(RemoteHeader));
                    memset( unlinkUser, 0, sizeof(UnlinkUser));
                    unlinkUser->UserIndex = deviceinfor->cmdParameter;
                    memcpy( unlinkUser->machineIdentification, machineConfig.machineUUID, Settings.StringMaxLength);
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
        case REMOTE_USER_RFID_LOGIN:
        {
            switch(deviceinfor->currentSubState)
            {
            case 0:
            {
                UserRFidLoginReq *rfidLoginReq =(UserRFidLoginReq*) buf+ sizeof(RemoteHeader);
                memcpy( rfidLoginReq->macAddress, deviceinfor->deviceMacAddress, 6 );
                memcpy( rfidLoginReq->MachineID, machineConfig.machineUUID, Settings.StringMaxLength);
                memcpy(rfidLoginReq->RFIDTag, dapiGlobalVariable->dataInfor.loginInfor.rfiddata.rfidTag, 16);
                rfidLoginReq->RFIDCarrier = dapiGlobalVariable->dataInfor.loginInfor.rfiddata.rfidCarrier;
                rfidLoginReq->userID = dapiGlobalVariable->dataInfor.loginInfor.rfiddata.userIndex;
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
        case REMOTE_USER_RFID_ASSOCIATE:
        {
            switch(deviceinfor->currentSubState)
            {
            case 0:
            {
                UserRFidAssociateReq *userRFidAssociateReq  = (UserRFidAssociateReq*)buf+ sizeof(RemoteHeader);
                memcpy( userRFidAssociateReq->macAddress, deviceinfor->deviceMacAddress, 6 );
                memcpy( userRFidAssociateReq->MachineID, machineConfig.machineUUID, Settings.StringMaxLength);
                memcpy(userRFidAssociateReq->RFIDTag, dapiGlobalVariable->dataInfor.loginInfor.rfiddata.rfidTag, 16);
                userRFidAssociateReq->userID = dapiGlobalVariable->dataInfor.loginInfor.rfiddata.userIndex;
                userRFidAssociateReq->RFIDCarrier = dapiGlobalVariable->dataInfor.loginInfor.rfiddata.rfidCarrier;
                memcpy(userRFidAssociateReq->externalUserID , dapiGlobalVariable->dataInfor.workoutInfor._UserExternalID+1,36);
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
        case REMOTE_USER_RFID_DISASSOCIATE:
        {
            switch(deviceinfor->currentSubState)
            {
            case 0:
            {
                UserRFidDisassociateReq *userRFidDisassociateReq  = (UserRFidDisassociateReq*)buf+ sizeof(RemoteHeader);
                memcpy( userRFidDisassociateReq->macAddress, deviceinfor->deviceMacAddress, 6 );
                memcpy( userRFidDisassociateReq->MachineID, machineConfig.machineUUID, Settings.StringMaxLength);
                memcpy(userRFidDisassociateReq->RFIDTag, dapiGlobalVariable->dataInfor.loginInfor.rfiddata.rfidTag, 16);
                userRFidDisassociateReq->RFIDCarrier = dapiGlobalVariable->dataInfor.loginInfor.rfiddata.rfidCarrier;
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


RemoteResponse RemoteUserReceive(SubReceiveDataType datatype, ADDSTATESFUNC addstatefunc, DapiGlobalVariable* dapiGlobalVariable )
{
    RemoteResponse ret = REMOTE_RESPONSE_START;

    switch(datatype.command)
    {
        case REMOTE_USER_MANAGEMENT_ACKNOWLEDGMENT:
        {
            RemoteDataAck *ack =(RemoteDataAck*) datatype.data;
            switch( ack->messageType )
            {
                case REMOTE_SYNC_USER_INFO:
                {
                }
                break;
                case REMOTE_USER_LOGIN:
                {
                }
                break;
                case REMOTE_USER_CLEAR_USERS:
                {
                }
                break;
                case REMOTE_USER_UNLINK_USER:
                {
                }
                break;
                case REMOTE_USER_RFID_LOGIN:
                {
                }
                break;
                case REMOTE_USER_RFID_ASSOCIATE:
                {
                }
                break;
                case REMOTE_USER_RFID_DISASSOCIATE:
                {
                }
                break;
                default:
                    break;
            }
        }
        break;
        case REMOTE_SYNC_USER_INFO:
        {
            UserSyncResponseData *responseData = (UserSyncResponseData*)datatype.data;
            UINT8 userIndex = responseData->userConsoleId;
            if( userIndex < SystemConfig_Get_MaxUserNumber( ))
            {
                USERS_INFO userinfo = SystemConfig_Get_UserInfo(userIndex);
                if(responseData->UserSetup == 0)
                {
                    if( strncmp( userinfo.xid, DEFAULTXID, XIDMAXLENGTH ) != 0 )
                    {
                        if(SystemConfig_Reset_User_Default(userIndex))
                        {
                            SystemConfigProcess(SVK_SAVE_USER2FLASH, &userIndex, sizeof(userIndex));
                        }
                    }
                }
                else
                {                
                    memcpy( userinfo.xid, responseData->xid, XIDMAXLENGTH );
                    memcpy( userinfo.passcode, responseData->passcode, 4 );
                    userinfo.weightStandardX100= responseData->weightX10*10+5;
                    userinfo.age = responseData->age;
                    userinfo.year = responseData->year;
                    userinfo.month = responseData->month;
                    userinfo.date= responseData->day;
                    userinfo.gender = responseData->gender;
                    userinfo.languageId= responseData->language;
                    userinfo.displayUnit= responseData->DisplayUnits;
                    userinfo.isAcceptedTerms= responseData->AcceptedTerms;
                    userinfo.isShareWorkouts= responseData->WorkoutDataShare;
                    memset( userinfo.name, 0, 65 );
                    strncpy( userinfo.name, responseData->name, 65 );
                    if(SystemConfig_Set_UserInfo(userinfo,userIndex))
                    { 
                        SystemConfigProcess(SVK_SAVE_USER2FLASH, &userIndex, sizeof(userIndex));
                    }
                }
            }
        }
        break;
        case REMOTE_USER_LOGIN:
        {
            UserLoginResponse *loginResponse = (UserLoginResponse*)datatype.data;
            UINT8 userId = loginResponse->userId;	
            if(userId < SystemConfig_Get_MaxUserNumber( ))
            {
                USERS_INFO userinfo = SystemConfig_Get_UserInfo(userId);
                MACHINE_INFO machineConfig=SystemConfig_Get_MachineInfo();
                memcpy(userinfo.xid, dapiGlobalVariable->dataInfor.loginInfor._loginRequest.xid, XIDMAXLENGTH );
                memcpy( userinfo.passcode, dapiGlobalVariable->dataInfor.loginInfor._loginRequest.passcode, 4 );
                userinfo.weightStandardX100 = loginResponse->weightX10*10+5;
                userinfo.age = loginResponse->age;
                userinfo.year = loginResponse->year;
                userinfo.month = loginResponse->month;
                userinfo.date = loginResponse->day;
                userinfo.gender = loginResponse->gender;
                userinfo.languageId= loginResponse->language;
                userinfo.displayUnit = loginResponse->DisplayUnits;
                dapiGlobalVariable->dataInfor.workoutInfor._UserExternalID[0] = userId;
                memcpy( dapiGlobalVariable->dataInfor.workoutInfor._UserExternalID+1,  loginResponse->externalUserId,  36 );
                userinfo.isAcceptedTerms = 0;
                userinfo.isSetup= 1;
                if(machineConfig.lastLoggedUser != userId )
                {
                        USERS_INFO Tempuserinfo = SystemConfig_Get_UserInfo( machineConfig.lastLoggedUser );
                        Tempuserinfo.isLoggedOn = 0;
                        if((SystemConfig_Set_UserInfo(Tempuserinfo,machineConfig.lastLoggedUser)))   
                        {
                            SystemConfigProcess(SVK_SAVE_USER2FLASH, &machineConfig.lastLoggedUser,sizeof(machineConfig.lastLoggedUser));
                        }
                }
                machineConfig.lastLoggedUser = userId;
                userinfo.isLoggedOn = 1;
	         machineConfig.unit = userinfo.displayUnit;             
               memset( userinfo.name, 0, 65 );
               memcpy( userinfo.name, loginResponse->userName, 65 );
               if(SystemConfig_Set_UserInfo(userinfo,userId))
               {
                   SystemConfigProcess(SVK_SAVE_USER2FLASH, &userId,sizeof(userId));                        
               }
               if(SystemConfig_Set_MachineInfo(machineConfig))
               {                        
                   SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH, NULL,0);
               }
            }
        }
        break;
        case REMOTE_USER_RFID_LOGIN:
        {
            UserRFidLoginResp *response = (UserRFidLoginResp *)datatype.data;
            UINT8 userId = response->userID;            
            if(userId < SystemConfig_Get_MaxUserNumber( ))
            {                
                USERS_INFO userinfo = SystemConfig_Get_UserInfo(userId);
                MACHINE_INFO machineConfig=SystemConfig_Get_MachineInfo();
                
                dapiGlobalVariable->dataInfor.workoutInfor._UserExternalID[0] = userId;
                memcpy( dapiGlobalVariable->dataInfor.workoutInfor._UserExternalID+1,  response->externalUserID,  36 );                
                userinfo.gender = response->gender;
                userinfo.weightStandardX100 = response->userWeightX10*10;
                userinfo.age = response->userAge;
                userinfo.year = response->userBirthYear;
                userinfo.month = response->userBirthMonth;
                userinfo.date= response->userBirthDay;
                userinfo.languageId= response->userLanguageId;
                userinfo.displayUnit= response->displayUnits;
                userinfo.isAcceptedTerms= response->acceptedTerms;
                userinfo.isShareWorkouts= response->shareWorkoutData;
                memset( userinfo.name, 0, 65 );
                memcpy( userinfo.name, response->userName, 65 );                
                if(machineConfig.lastLoggedUser != userId )
                {
                    USERS_INFO Tempuserinfo = SystemConfig_Get_UserInfo( machineConfig.lastLoggedUser );
                    Tempuserinfo.isLoggedOn = 0;
                    if((SystemConfig_Set_UserInfo(Tempuserinfo,machineConfig.lastLoggedUser)))   
                    {
                        SystemConfigProcess(SVK_SAVE_USER2FLASH, &machineConfig.lastLoggedUser,sizeof(machineConfig.lastLoggedUser));
                    }
                }
                machineConfig.lastLoggedUser = userId;
                userinfo.isLoggedOn = 1;
                machineConfig.unit = userinfo.displayUnit; 
                if((SystemConfig_Set_UserInfo(userinfo,userId)))
                {
                    SystemConfigProcess(SVK_SAVE_USER2FLASH, &userId,sizeof(userId));
                }
                if(SystemConfig_Set_MachineInfo(machineConfig))
                {
                    SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH, NULL,0);
                }
            }
        }
        break;
        default:
                break;
    }
    return ret;
}

#endif


