/****************************************************************************
*
*
*
*
*
*
****************************************************************************/
#include "remote_config.h"

#ifdef __REMOTE_LIB__
#include "remote_process.h"
#include "remote_datatype.h"
#include "global_config.h"
#include "crc.h"
#include "remote_port.h"
#include "remote_user.h"
#include "remote_asset.h"
#include "remote_general.h"
#include "remote_workouttracking.h"
#include "remote_control.h"



static UINT16 gNeedResend=0; 
static RemoteResponse RemoteReceiveMessage(UINT8* msg, UINT16 length,ADDSTATESFUNC addstatefunc);
UINT16 ConsonetransactionId=0;


__no_init static DapiGlobalVariable gDapiGlobalVariable;

//RemoteDevicesInformation*  gRemoteDevicesInformation[MAX_DEVICES_COUNT]={0};


SUBPROCESSFUNC gSubProcessFuncation[SUB_SYSTEM_TYPE_END]=
{
    RemoteGeneralProcess,
    RemoteControlProcess,
    NULL,
    RemoteWorkoutProcess,
    RemoteAssetProcess,
    RemoteUserProcess,
};

SUBRECEIVEFUNC gSubReceiveFuncation[SUB_SYSTEM_TYPE_END] = 
{
    RemoteGeneralReceive,
    RemoteControlReceive,
    NULL,
    RemoteWorkoutReceive,
    RemoteAssetReceive,
    RemoteUserReceive,
};
void RemoteDeviceLog(UINT8* msg, UINT8 strlen)
{
}

void RemoteDeviceInit(void)
{
    gNeedResend = 0;
}

void RemoteSetLoginParameter(UINT8 loginType, UINT8 slot, UINT8 *userinfor, UINT8 *passcod)
{
    if(loginType == 1)
    {
        UserLoginRequestData parameter;
        parameter.userId = slot;
        memcpy(parameter.xid, userinfor, 14);
        memcpy(parameter.passcode, passcod, 4);
        memcpy(&(gDapiGlobalVariable.dataInfor.loginInfor._loginRequest), &parameter, sizeof(UserLoginRequestData));
    }
    else if(loginType == 0)
    {
        RFIDData parameter;
        parameter.userIndex = slot;
        parameter.rfidCarrier =(RFIDCARRIER) (*passcod);
        memcpy(parameter.rfidTag,userinfor, 16 );
        memcpy(&(gDapiGlobalVariable.dataInfor.loginInfor.rfiddata), &parameter, sizeof(RFIDData));
    }
}


void RemoteChangeWorkoutStateCallback(RemoteWorkoutState state, ADDSTATESFUNC addstatefunc,UINT8 *devicesmac )
{
    RemoteChangeWorkotState(state, addstatefunc, &gDapiGlobalVariable, devicesmac);
}


bool  RemoteRegisterDevice(RemoteDevicesInformation *device)
{
    device->receiveMessage = RemoteReceiveMessage;
    device->ChangeWorkoutCallback = RemoteChangeWorkoutStateCallback;
    
    return true;
}
UINT16 RemoteProcessDataPre(UINT8* msg, UINT16 length)
{
	UINT16 ret = 0;
	RemoteHeader* header = (RemoteHeader*)msg; 
	if(header->dataSize == length)
		return ret;
	for(UINT16 i=0; i<length;i++)
	{
		if(((*(msg+i)) == 0xaa)&&(*(msg+i+1))==0x55)
		{
			ret = i;
			break;
		}
	}
	return ret;
}

RemoteResponse RemoteReceiveMessage(UINT8* msg, UINT16 length,ADDSTATESFUNC addstatefunc)
{
    RemoteResponse ret = REMOTE_RESPONSE_START;
    SubSystemType systemType = SUB_SYSTEM_TYPE_END ;
    SubReceiveDataType datatype;
	UINT8 *message = (msg+RemoteProcessDataPre(msg,length));
	
    RemoteHeader* header = (RemoteHeader*)message;    
    UINT8 command = header->messageId;
    ConsonetransactionId = header->transactionId;
    UINT16 crc = 0;
//    CRCBUFF=GenerateCRC_CCITT(STR , 3);
    if(header->dataSize>0)
    {
      crc=GenerateCRC_CCITT((message+sizeof(RemoteHeader)) , length-sizeof(RemoteHeader));
    }
    if(crc == header->checksum)
    {
        switch(header->lingoId)
        {
            case DAPI_LINGO_GENERAL:
            {
                    systemType = SUB_SYSTEM_TYPE_GENERAL;
            }
            break;
//            case DAPI_LINGO_INTERFACE:
//            {
//                systemType = SUB_SYSTEM_TYPE_INTERFACE;
//            }
            case DAPI_LINGO_REMOTE:
            {
                    systemType = SUB_SYSTEM_TYPE_CONTROL;
            }
            break;
            case DAPI_LINGO_SETTINGS:
            {
                    systemType = SUB_SYSTEM_TYPE_SYSTEMSETTINGS;
            }
            break;
            case DAPI_LINGO_WORKOUT_TRACKING:
            {
                    systemType = SUB_SYSTEM_TYPE_WORKOUTTRACKING;
            }
            break;
            case DAPI_LINGO_NETWORK_TIME:
            {
                ///command=????
                    systemType = SUB_SYSTEM_TYPE_ASSET;
                    command = REMOTE_ASSET_MANAGEMENT_GET_NETWORK_TIME;
            }
            break;
            case DAPI_LINGO_ASSET_MANAGEMENT:
            {
                    systemType = SUB_SYSTEM_TYPE_ASSET;
            }
            break;
            case DAPI_LINGO_USER_MANAGEMENT:
            {
                    systemType = SUB_SYSTEM_TYPE_USER;
            }
            break;
            default:
                break;
        }
    }
    if((systemType != SUB_SYSTEM_TYPE_END)&&(gSubReceiveFuncation[systemType] != NULL))
    {
        datatype.command =command;
        datatype.dataLength = header->dataSize;
        datatype.data = (UINT8*)(message+sizeof(RemoteHeader));
        ret = gSubReceiveFuncation[systemType](datatype ,addstatefunc,&gDapiGlobalVariable);
    }

	if(REMOTE_RESPONSE_SUCCESFUL != ret)
	{
		gNeedResend = 1; 
	}
	else 
    {
        if(command!=0)
            gNeedResend = header->lingoId<<8|command;
        else
        {
            RemoteDataAck *reponse =( RemoteDataAck *)datatype.data;
            
            gNeedResend = header->lingoId<<8|reponse->messageType;
        }
        
    }
    return ret;
    
}

RemoteEvent RemoteProcess(RemoteDevicesInformation *device)
{
        UINT8 buf[1024]={0};
        UINT16 bufLength;
        RemoteEvent ret = REMOTE_EVENT_NO_DEVICE;
        UINT8 flag = device->currentState>>8;
        SubProcessState response = SUB_PROCESS_STATE_NOT_SUPPORTED ;
        device->transactionId = ConsonetransactionId;
        bool sendReady = false;
        switch(flag)
        {
            case REMOTE_PROPERTY_GENERAL:
            {
                if(gSubProcessFuncation[SUB_SYSTEM_TYPE_GENERAL] == NULL )
                {

                    break;
                }
                response= gSubProcessFuncation[SUB_SYSTEM_TYPE_GENERAL](device,
                                                                     buf,&bufLength, &gDapiGlobalVariable);
            }
            break;
            case REMOTE_PROPERTY_CONTROL:
            {
                if(gSubProcessFuncation[SUB_SYSTEM_TYPE_CONTROL] == NULL )
                {

                    break;
                }
                response= gSubProcessFuncation[SUB_SYSTEM_TYPE_CONTROL](device,
                                                                     buf,&bufLength, &gDapiGlobalVariable);
            }
            break;
            case REMOTE_PROPERTY_SYSTEMSETTINGS:
            {
                if(gSubProcessFuncation[SUB_SYSTEM_TYPE_SYSTEMSETTINGS] == NULL )
                {

                    break;
                }
                response= gSubProcessFuncation[SUB_SYSTEM_TYPE_SYSTEMSETTINGS](device,
                                                                     buf,&bufLength, &gDapiGlobalVariable);
            }
            break;
            case REMOTE_PROPERTY_WORKOUTTRACKING:
            {
                if(gSubProcessFuncation[SUB_SYSTEM_TYPE_WORKOUTTRACKING] == NULL )
                {

                    break;
                }
                response= gSubProcessFuncation[SUB_SYSTEM_TYPE_WORKOUTTRACKING](device,
                                                                     buf,&bufLength, &gDapiGlobalVariable);
            }
            break;
            case REMOTE_PROPERTY_ASSET:
            {
                if(gSubProcessFuncation[SUB_SYSTEM_TYPE_ASSET] == NULL )
                {

                    break;
                }
                response= gSubProcessFuncation[SUB_SYSTEM_TYPE_ASSET](device,
                                                                     buf,&bufLength, &gDapiGlobalVariable);
            }
            break;
            case REMOTE_PROPERTY_USER:
            {
                if(gSubProcessFuncation[SUB_SYSTEM_TYPE_USER] == NULL )
                {
                    break;
                }
                response= gSubProcessFuncation[SUB_SYSTEM_TYPE_USER](device,
                                                                     buf,&bufLength, &gDapiGlobalVariable);
            }
            break;
            default:
                break;
        }
        switch( response )
        {
            case SUB_PROCESS_STATE_NOT_SUPPORTED:
            {
                //device->nextState();
                break;
            }
            break;
            case SUB_PROCESS_STATE_NEXT_SUBSTATE:
            {
                if(device->subStateSendMessage != NULL)
                {
                    sendReady = device->subStateSendMessage(buf,bufLength);
                    if( !sendReady )
                    {
                        break;
                    }
                }
                device->currentSubState ++;
            }
            break;
            case SUB_PROCESS_STATE_NEXT_STATE:
            {
                if(device->stateSendMessage != NULL)
                {
                    memset(buf,0,1024);
                    PushDataUrl *pushDataUrl =(PushDataUrl *) buf;
                    pushDataUrl->httpDataType = HTTP_APPLICATION_OCTET_STREAM;
                    pushDataUrl->httpMethod = HTTP_POST;
                    pushDataUrl->secure = gDapiGlobalVariable.DapiInfo.Secure;
                    memcpy(pushDataUrl->url, gDapiGlobalVariable.DapiInfo.Url, 256);
                    sendReady =device->stateSendMessage(buf , sizeof(PushDataUrl));
                    if( !sendReady )
                    {
                        break;
                    }
                }
                device->nextState();
            }
            break;
            case SUB_PROCESS_STATE_JUMP_STATE:
            {
                device->nextState();
            }
            break;
            case SUB_PROCESS_STATE_DIRECT_CONNECT:
            {
                if((device->directSendMessage != NULL)&&device->reSend>4)
                {
     
                    sendReady = device->directSendMessage(buf,bufLength);
                    if( !sendReady )
                    {
                        break;
                    }
					else
					{
						if((device->reSend != 0xA5))
							device->reSend = 0;
					}
					
                }
				if((device->reSend == 0xA5)||(gNeedResend == device->currentState))
				{
            		device->nextState();
					gNeedResend = 1;
				}
				else if(device->reSend>3)
				{
					device->reSendFuncation();
				}
            }
            break;
            default:
                break;
        }                
       return ret;
}



#endif
