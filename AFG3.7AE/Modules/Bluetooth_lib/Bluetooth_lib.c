#include "remote_config.h"
#include "wlt2564_command.h"
#include "Bluetooth_lib.h"
#include "Remote_datatype.h"
#include "Remote_port.h"
#include "remote_general.h"
#include "Events_Handler.h"
//#include "UtilitiesAMP.h"
//#ifdef __BLUETOOTH_FUNC__

#define MAX_BT_STATES_NUMBER 20

typedef struct{
    UINT16 state;    
    UINT16 intParameter;
    UINT8   argsno;
    UINT8* pointParameter;
}BluetoothStateStruct;


typedef struct{
    RemoteDevicesInformation remoteDevice;
    BluetoothStateStruct BluetoothStates[MAX_BT_STATES_NUMBER];
    UINT8 currentIndex;
    UINT8 nextIndex;
    UINT8 statesCounter;
    UINT16 timerCounter;
    UINT16 SendTimeOutCounter;
    UINT16 scanDelaytimer;
    UINT16 scanDelay1s;
}BluetoothDataStruct;
typedef struct{
UINT8 GETMAC_FLAG:1;
UINT8 GETBLENAME_FLAG:1;
UINT8 SETBLENAME_FLAG:1;
UINT8 GETAMPNAME_FLAG:1;
UINT8 SETAMPNAME_FLAG:1;
}CMDSend;
static CMDSend CmdSend;
UINT16 Delay1S=0;
bool WorkoutCompleteSync=0;
unsigned char BleName[14]="AFG Sport_";
#ifdef TM
unsigned char AmpName[19]="3.7AT SPEAKERS "; 
#elif (defined AE)
unsigned char AmpName[19]="3.7AE SPEAKERS "; 
#else
unsigned char AmpName[19]="3.7HT SPEAKERS "; 
#endif
__no_init static BluetoothDataStruct gBluetoothData;
UINT8 BluetoothMode;
__no_init static UINT8 startScan;
__no_init static bool b_SyncSuccessFlag;
extern UINT8 gBtCounter;
extern void WltReciveInterrupt(void);
extern void WltSendInterrupt(void);
char AmpNameFlag=0;


void UpdateBluetoothProcess(void);    //zsf

void BT_Set_SyncFlag(bool b_Data)
{
    b_SyncSuccessFlag = b_Data;
}
bool BT_Get_SyncFlag(void)
{
    return b_SyncSuccessFlag;
}
static void BTNextState(void)
{
    gBluetoothData.remoteDevice.currentSubState = 0;  
    gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].state=0;
	gBluetoothData.remoteDevice.reSend  = 0xAA;
    gBluetoothData.remoteDevice.currentState = gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].state;
    gBluetoothData.remoteDevice.argscounter = gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].argsno;
    gBluetoothData.remoteDevice.cmdParameter= gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].intParameter;
    gBluetoothData.remoteDevice.parameterPoint= gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].pointParameter;
    
    gBluetoothData.currentIndex ++;
    gBluetoothData.statesCounter --;
    if(gBluetoothData.currentIndex  == MAX_BT_STATES_NUMBER )
    {
        gBluetoothData.currentIndex = 0;
    }
}

static bool AddBTState(UINT16 state, UINT8 argsno, UINT16 parameter, UINT8 *parameterPoint )
{
    bool ret = false;
     if((gBtCounter>20)&&(gBluetoothData.statesCounter>1))
        HAL_BT_Enable_UARTI(1);
    if(gBluetoothData.statesCounter  == MAX_BT_STATES_NUMBER)
    {
        ret = false;
        return ret;
    }
    ret = true;
    gBluetoothData.BluetoothStates[gBluetoothData.nextIndex].state = state;
    gBluetoothData.BluetoothStates[gBluetoothData.nextIndex].argsno = argsno;
    gBluetoothData.BluetoothStates[gBluetoothData.nextIndex].intParameter = parameter;
    gBluetoothData.BluetoothStates[gBluetoothData.nextIndex].pointParameter = parameterPoint;
    gBluetoothData.statesCounter ++;
    gBluetoothData.nextIndex ++;
    if(gBluetoothData.nextIndex  == MAX_BT_STATES_NUMBER )
    {
        gBluetoothData.nextIndex = 0;
    }
	if(state == 0x0000)
	{
		ret = false;
	}

    return ret;
}

bool WltProcessRecive(UINT16 dataLength)
{
   UINT8 databuf[1024];
    UINT16 length=0;
    bool ret = false;
    length=WltGetReciveData(databuf);
    if(length==0)
    {
        return ret;
    }
    gBluetoothData.remoteDevice.receiveMessage(databuf,length,AddBTState);
    return ret;
}

bool BluetoothStartUpdate(UINT8 update)      //zsf
{
    bool ret =false;
    
    if (0 == update)
        ret = AddBTState(REMOTE_COMMAND_BT_GETUPDATEINFO,0,0, NULL);
    else
        ret = AddBTState(REMOTE_COMMAND_BT_UARTUPDATE,0,0, NULL);
    return ret;
}

bool BluetoothSetAdvertisele(void)      //zsf
{
    bool ret =false;
    
    ret = AddBTState(REMOTE_COMMAND_BT_STOPADVERTISELE,0,0, NULL);
    return ret;
}

bool BluetoothSetDiscoveryMode(void)      //zsf
{
    bool ret =false;
    
    ret = AddBTState(REMOTE_COMMAND_BT_SETDISCOVERYMODE,0,0, NULL);
    return ret;
}

bool BluetoothSetConnetMode(void)      //zsf
{
    bool ret =false;
    
    ret = AddBTState(REMOTE_COMMAND_BT_SETCONNECTMODE,0,0, NULL);
    return ret;
}

BLUETOOTH_EVENT BluetoothProcess()
{
    BLUETOOTH_EVENT evt = BLUETOOTH_EVT_NONE;
    static UINT8 updateCNT = 0;
    
    if(Event_Handler_GetNewState() != FRAME_STAGE_ENG && Event_Handler_GetNewState() != FRAME_STAGE_AGINGTEST)
    {
        if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
            Show_LEDLight(LED12_BT,(LedType)Bluetooth_Get_BT_LED_Status());
        else
            Show_LEDLight(LED13_BT_EP,(LedType)Bluetooth_Get_BT_LED_Status());
    }
    
    WltProcessRecive(0);
    if(gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].state<=REMOTE_COMMAND_BT_START)
    {
        gBluetoothData.remoteDevice.currentState = gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].state;
        gBluetoothData.remoteDevice.argscounter = gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].argsno;
        gBluetoothData.remoteDevice.cmdParameter= gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].intParameter;
        gBluetoothData.remoteDevice.parameterPoint= gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].pointParameter;
        RemoteProcess(&gBluetoothData.remoteDevice);
    }
    else
    {
        switch(gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].state)
        {
             case REMOTE_COMMAND_BT_SETBLENAME: 
                wlt2564SendCommand(BTCMD_SETBLENAME,BleName,sizeof(BleName));
                gBluetoothData.remoteDevice.currentSubState = 0;  
                gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].state=0;
                gBluetoothData.remoteDevice.reSend  = 0xAA;
                gBluetoothData.remoteDevice.currentState = gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].state;
                gBluetoothData.remoteDevice.argscounter = gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].argsno;
                gBluetoothData.remoteDevice.cmdParameter= gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].intParameter;
                gBluetoothData.remoteDevice.parameterPoint= gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].pointParameter;
                
                gBluetoothData.currentIndex ++;
                gBluetoothData.statesCounter --;
                if(gBluetoothData.currentIndex  == MAX_BT_STATES_NUMBER )
                {
                    gBluetoothData.currentIndex = 0;
                }
            break;
            case REMOTE_COMMAND_BT_GETBLENAME: 
                wlt2564SendCommand(BTCMD_GETBLENAME,NULL,0);
                gBluetoothData.remoteDevice.currentSubState = 0;  
                gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].state=0;
                gBluetoothData.remoteDevice.reSend  = 0xAA;
                gBluetoothData.remoteDevice.currentState = gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].state;
                gBluetoothData.remoteDevice.argscounter = gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].argsno;
                gBluetoothData.remoteDevice.cmdParameter= gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].intParameter;
                gBluetoothData.remoteDevice.parameterPoint= gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].pointParameter;
                
                gBluetoothData.currentIndex ++;
                gBluetoothData.statesCounter --;
                if(gBluetoothData.currentIndex  == MAX_BT_STATES_NUMBER )
                {
                    gBluetoothData.currentIndex = 0;
                }
            break;
            case REMOTE_COMMAND_BT_GETLOCALNAME: 
                wlt2564SendCommand(BTCMD_GETLOCALNAME,NULL,0);
                gBluetoothData.remoteDevice.currentSubState = 0;  
                gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].state=0;
                gBluetoothData.remoteDevice.reSend  = 0xAA;
                gBluetoothData.remoteDevice.currentState = gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].state;
                gBluetoothData.remoteDevice.argscounter = gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].argsno;
                gBluetoothData.remoteDevice.cmdParameter= gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].intParameter;
                gBluetoothData.remoteDevice.parameterPoint= gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].pointParameter;
                
                gBluetoothData.currentIndex ++;
                gBluetoothData.statesCounter --;
                if(gBluetoothData.currentIndex  == MAX_BT_STATES_NUMBER )
                {
                    gBluetoothData.currentIndex = 0;
                }
            break;
            
            case REMOTE_COMMAND_BT_SWITCHMODE:
                if(gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].intParameter != 0)
                {
                    wlt2564SendCommand(BTCMD_MASTERMODE,NULL,0);
                    gBluetoothData.remoteDevice.currentSubState = 0;  
                    gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].state=0;
                    gBluetoothData.remoteDevice.reSend  = 0xAA;
                    gBluetoothData.remoteDevice.currentState = gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].state;
                    gBluetoothData.remoteDevice.argscounter = gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].argsno;
                    gBluetoothData.remoteDevice.cmdParameter= gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].intParameter;
                    gBluetoothData.remoteDevice.parameterPoint= gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].pointParameter;
                    
                    gBluetoothData.currentIndex ++;
                    gBluetoothData.statesCounter --;
                    if(gBluetoothData.currentIndex  == MAX_BT_STATES_NUMBER )
                    {
                        gBluetoothData.currentIndex = 0;
                    }
                    AddBTState(REMOTE_COMMAND_BT_STARTSCANNING,1,0, NULL);

				  evt = BLUETOOTH_MODE_MASTER;					
                }
                else
                { 
                    wlt2564SendCommand(BTCMD_SLAVERMODE,NULL,0);
                    gBluetoothData.remoteDevice.currentSubState = 0;  
                    gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].state=0;
                    gBluetoothData.remoteDevice.reSend  = 0xAA;
                    gBluetoothData.remoteDevice.currentState = gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].state;
                    gBluetoothData.remoteDevice.argscounter = gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].argsno;
                    gBluetoothData.remoteDevice.cmdParameter= gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].intParameter;
                    gBluetoothData.remoteDevice.parameterPoint= gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].pointParameter;
                    
                    gBluetoothData.currentIndex ++;
                    gBluetoothData.statesCounter --;
                    if(gBluetoothData.currentIndex  == MAX_BT_STATES_NUMBER )
                    {
                        gBluetoothData.currentIndex = 0;
                    }
				  evt = BLUETOOTH_MODE_SLAVE;	
                }
            break;
            case REMOTE_COMMAND_BT_STARTSCANNING:
            {
                wlt2564StartScaning();
                gBluetoothData.remoteDevice.currentSubState = 0;  
                    gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].state=0;
                    gBluetoothData.remoteDevice.reSend  = 0xAA;
                    gBluetoothData.remoteDevice.currentState = gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].state;
                    gBluetoothData.remoteDevice.argscounter = gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].argsno;
                    gBluetoothData.remoteDevice.cmdParameter= gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].intParameter;
                    gBluetoothData.remoteDevice.parameterPoint= gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].pointParameter;
                    
                    gBluetoothData.currentIndex ++;
                    gBluetoothData.statesCounter --;
                    if(gBluetoothData.currentIndex  == MAX_BT_STATES_NUMBER )
                    {
                        gBluetoothData.currentIndex = 0;
                    }
            }
            break;
            case REMOTE_COMMAND_BT_STOPSCANNING:
            {
                wlt2564SendCommand(BTCMD_STOPSCANNING,NULL,0);
                gBluetoothData.remoteDevice.currentSubState = 0;  
                gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].state=0;
                gBluetoothData.remoteDevice.reSend  = 0xAA;
                gBluetoothData.remoteDevice.currentState = gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].state;
                gBluetoothData.remoteDevice.argscounter = gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].argsno;
                gBluetoothData.remoteDevice.cmdParameter= gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].intParameter;
                gBluetoothData.remoteDevice.parameterPoint= gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].pointParameter;
                
                gBluetoothData.currentIndex ++;
                gBluetoothData.statesCounter --;
                if(gBluetoothData.currentIndex  == MAX_BT_STATES_NUMBER )
                {
                    gBluetoothData.currentIndex = 0;
                }
            }
            break;
            case REMOTE_COMMAND_BT_SETAMPNAME:
                wlt2564SendCommand(BTCMD_SETAMPNAME,AmpName,sizeof(AmpName));
                gBluetoothData.remoteDevice.currentSubState = 0;  
                gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].state=0;
                gBluetoothData.remoteDevice.reSend  = 0xAA;
                gBluetoothData.remoteDevice.currentState = gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].state;
                gBluetoothData.remoteDevice.argscounter = gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].argsno;
                gBluetoothData.remoteDevice.cmdParameter= gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].intParameter;
                gBluetoothData.remoteDevice.parameterPoint= gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].pointParameter;
                
                gBluetoothData.currentIndex ++;
                gBluetoothData.statesCounter --;
                if(gBluetoothData.currentIndex  == MAX_BT_STATES_NUMBER )
                {
                    gBluetoothData.currentIndex = 0;
                }
            break;
            case REMOTE_COMMAND_BT_GETUPDATEINFO:     //zsf
                wlt2564SendCommand(BTCMD_GETUPDATEINFO,NULL,0);
                gBluetoothData.remoteDevice.currentSubState = 0;  
                gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].state=0;
                gBluetoothData.remoteDevice.reSend  = 0xAA;
                gBluetoothData.remoteDevice.currentState = gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].state;
                gBluetoothData.remoteDevice.argscounter = gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].argsno;
                gBluetoothData.remoteDevice.cmdParameter= gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].intParameter;
                gBluetoothData.remoteDevice.parameterPoint= gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].pointParameter;
                
                gBluetoothData.currentIndex ++;
                gBluetoothData.statesCounter --;
                if(gBluetoothData.currentIndex  == MAX_BT_STATES_NUMBER )
                {
                    gBluetoothData.currentIndex = 0;
                }
            break;
            case REMOTE_COMMAND_BT_STOPADVERTISELE:     //zsf
                wlt2564SendCommand(BTCMD_SETADVERTISELE,NULL,0);
                gBluetoothData.remoteDevice.currentSubState = 0;  
                gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].state=0;
                gBluetoothData.remoteDevice.reSend  = 0xAA;
                gBluetoothData.remoteDevice.currentState = gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].state;
                gBluetoothData.remoteDevice.argscounter = gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].argsno;
                gBluetoothData.remoteDevice.cmdParameter= gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].intParameter;
                gBluetoothData.remoteDevice.parameterPoint= gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].pointParameter;
                
                gBluetoothData.currentIndex ++;
                gBluetoothData.statesCounter --;
                if(gBluetoothData.currentIndex  == MAX_BT_STATES_NUMBER )
                {
                    gBluetoothData.currentIndex = 0;
                }
            break;
            case REMOTE_COMMAND_BT_SETDISCOVERYMODE:     //zsf
                wlt2564SendCommand(BTCMD_SETDISCOVERYMODE,NULL,0);
                gBluetoothData.remoteDevice.currentSubState = 0;  
                gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].state=0;
                gBluetoothData.remoteDevice.reSend  = 0xAA;
                gBluetoothData.remoteDevice.currentState = gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].state;
                gBluetoothData.remoteDevice.argscounter = gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].argsno;
                gBluetoothData.remoteDevice.cmdParameter= gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].intParameter;
                gBluetoothData.remoteDevice.parameterPoint= gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].pointParameter;
                
                gBluetoothData.currentIndex ++;
                gBluetoothData.statesCounter --;
                if(gBluetoothData.currentIndex  == MAX_BT_STATES_NUMBER )
                {
                    gBluetoothData.currentIndex = 0;
                }
            break;
            case REMOTE_COMMAND_BT_SETCONNECTMODE:     //zsf
                wlt2564SendCommand(BTCMD_SETCONNECTMODE,NULL,0);
                gBluetoothData.remoteDevice.currentSubState = 0;  
                gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].state=0;
                gBluetoothData.remoteDevice.reSend  = 0xAA;
                gBluetoothData.remoteDevice.currentState = gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].state;
                gBluetoothData.remoteDevice.argscounter = gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].argsno;
                gBluetoothData.remoteDevice.cmdParameter= gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].intParameter;
                gBluetoothData.remoteDevice.parameterPoint= gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].pointParameter;
                
                gBluetoothData.currentIndex ++;
                gBluetoothData.statesCounter --;
                if(gBluetoothData.currentIndex  == MAX_BT_STATES_NUMBER )
                {
                    gBluetoothData.currentIndex = 0;
                }
            break;
            case REMOTE_COMMAND_BT_UARTUPDATE:    //zsf
            {
                wlt2564SendCommand(BTCMD_UARTUPDATE,NULL,0);
                gBluetoothData.remoteDevice.currentSubState = 0;  
                gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].state=0;
                gBluetoothData.remoteDevice.reSend  = 0xAA;
                gBluetoothData.remoteDevice.currentState = gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].state;
                gBluetoothData.remoteDevice.argscounter = gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].argsno;
                gBluetoothData.remoteDevice.cmdParameter= gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].intParameter;
                gBluetoothData.remoteDevice.parameterPoint= gBluetoothData.BluetoothStates[gBluetoothData.currentIndex].pointParameter;
                
                gBluetoothData.currentIndex ++;
                gBluetoothData.statesCounter --;
                if(gBluetoothData.currentIndex  == MAX_BT_STATES_NUMBER )
                {
                    gBluetoothData.currentIndex = 0;
                }
            }
            break;
            default:break;
        }
    }
    switch(wlt2564_Get_CmdStatu())
    {
        case 0:
        {
            
        }
        break;
        case 1:
        {
            if(CmdSend.GETMAC_FLAG==0)
            {
                CmdSend.GETMAC_FLAG = 1;
                wlt2564_Set_CmdStatu(0);
                //GetWlt2564MAC();
                GetWlt2564MAC();
            }
        }
        break;
        case 2:
        {
            if(CmdSend.GETBLENAME_FLAG==0)
            {
                CmdSend.GETBLENAME_FLAG = 1;
                wlt2564_Set_CmdStatu(0);
                BluetoothGetBleName(); 
            }
        }
        break;
        case 3:
        {
            if(CmdSend.SETBLENAME_FLAG==0)
            {
                CmdSend.SETBLENAME_FLAG = 1;
                wlt2564_Set_CmdStatu(0);
                SetBleNameFlag(1);
                BluetoothSetBleName(0);
            }
        }
        break;
        case SEND_CMD_SETBLENAME_COMPLETE:
        {
            if(CmdSend.GETAMPNAME_FLAG==0)
            {
                CmdSend.GETAMPNAME_FLAG = 1;
                wlt2564_Set_CmdStatu(0);
                SetBleNameFlag(0);
                BluetoothGetLOCALName();
            }
        }
        break;
        case SEND_CMD_GETAMPNAME_COMPLETE:
        {
            if(CmdSend.SETAMPNAME_FLAG==0)
            {
                CmdSend.SETAMPNAME_FLAG = 1;
                wlt2564_Set_CmdStatu(0);
                BluetoothSetAmpName();
            }
        }
        break;
        default:
        break;
    }
    

    if ((Event_Handler_GetNewState() != FRAME_STATE_USBMAN) && wlt2564_Get_UpdateStatu())    //bt更新过程中U盘拔出
    {
        HAL_Reset_System(); 
    }
    switch(wlt2564_Get_UpdateStatu())   //zsf
    {
        case 0:
        {
            
        }
        break;      
        case UPDATE_CMD_GETUPDATEINFO_COMPLETE:    //zsf
        {
            wlt2564_Set_UpdateStatu(0);
            SetUpdateFlag(UARTUPDATE_CMD_ADVERTISELE);
            BluetoothSetAdvertisele();
        }
        break;
        case STOP_CMD_ADVERTISELE_COMPLETE:    //zsf
        {
            wlt2564_Set_UpdateStatu(0);
            SetUpdateFlag(UARTUPDATE_CMD_SETDISCOVERYMODE);
            BluetoothSetDiscoveryMode();
        }
        break;
        case STOP_CMD_SETDISCOVERYMODE_COMPLETE:    //zsf
        {
            wlt2564_Set_UpdateStatu(0);
            SetUpdateFlag(UARTUPDATE_CMD_SETCONNECTMODE);
            BluetoothSetConnetMode();
        }
        break;
        case STOP_CMD_SETCONNECTMODE_COMPLETE:    //zsf
        {
            wlt2564_Set_UpdateStatu(0);
            SetUpdateFlag(UARTUPDATE_CMD_SEND);
            BluetoothStartUpdate(1);
        }
        break;
        case UPDATE_CMD_UARTUPDATE_COMPLETE:    //zsf
        {
            SetUpdateFlag(UARTUPDATE_DATA_START);
            UpdateBluetoothProcess();
            updateCNT++;
            if(updateCNT >= 50)
            {
                updateCNT = 0;
                evt = BLUETOOTH_UPDATE_START;
            }
        }
        break;
        case UARTUPDATE_COMPLETE:    //zsf
        {
            SetUpdateFlag(0);
            wlt2564_Set_UpdateStatu(0);
            initialBluetoothLib();
            evt = BLUETOOTH_UPDATE_END;
        }
        break;
        case UARTUPDATE_FAIL:    //zsf
        {          
            evt = BLUETOOTH_UPDATE_FAIL;
        }
        default:
        break;
    }
    //}
	return evt;
}



bool BluetoothSetUser(UINT8 slot)
{
    bool ret =false;
    ret = AddBTState(RCMD_CONTROL_SET_CURRENT_USER,1,slot, NULL);
    return ret;
}

bool BluetoothStartWorkout(void)
{
    bool ret =false;
    ret = AddBTState(RCMD_CONTROL_START,0,0, NULL);
    return ret;
}


bool BluetoothStopWorkout(void)
{
    bool ret =false;
    ret = AddBTState(RCMD_GENERAL_STOP_WORKOUT,0,0, NULL);
    return ret;
}
void BluetoothGetWorkoutData(void)
{
    AddBTState(RCMD_GENERAL_GET_WORKOUT_DATA, 0, 0, NULL);   
}

bool BluetoothPauseWorkout(UINT8 pause)
{
    bool ret =false;
    ret = AddBTState(RCMD_CONTROL_PAUSE,0,pause, NULL);
    return ret;
}

bool BluetoothSetPopUpInformation(UINT8 popUpType)
{
    bool ret =false;
    ret = AddBTState(RCMD_CONTROL_KEY_PRESS,0,popUpType, NULL);
    return ret;
}
bool BluetoothSetHRCustom(UINT8 userSlot,UINT8 *buf)
{
    bool ret =false;
    ret = AddBTState(RCMD_GENERAL_SET_CUSTOM_HR_PROGRAM,userSlot,0, buf);
    return ret;
}
bool BluetoothSetCustom()
{
    bool ret =false;
    ret = AddBTState(RCMD_GENERAL_SET_CUSTOM_PROGRAM,0,0, NULL);
    return ret;
}
bool BluetoothSetMyFirst5K(UINT8 userSlot)
{
    bool ret =false;
    ret = AddBTState(RCMD_GENERAL_SET_MY_FIRST_5K,userSlot,0, 0);
    return ret;
}
bool BluetoothSetSpeed()///
{
    bool ret = false;
    
    return ret;
}

bool BluetoothSetIncline()
{
    bool ret = false;
    
    return ret;
}

bool BluetoothSetRes()
{
    bool ret = false;
    
    return ret;
}

bool BluetoothSetProgram(UINT8 programType)
{
    bool ret =false;
    ret = AddBTState(RCMD_CONTROL_SELECT_PROGRAM, 1, programType, NULL);
    return ret;
}
bool BluetoothSwitchMode(UINT8 Mode)
{
    bool ret =false;
    ret = AddBTState(REMOTE_COMMAND_BT_SWITCHMODE,1,Mode, NULL);
    return ret;
}
bool BluetoothSetBleName(UINT8 Mode)
{
    bool ret =false;
    ret = AddBTState(REMOTE_COMMAND_BT_SETBLENAME,1,Mode, NULL);
    return ret;      
}
bool BluetoothGetBleName(void)
{
    bool ret =false;
    ret = AddBTState(REMOTE_COMMAND_BT_GETBLENAME,1,0, NULL);
    return ret;      
}
bool BluetoothGetLOCALName(void)
{
    bool ret =false;
    ret = AddBTState(REMOTE_COMMAND_BT_GETLOCALNAME,1,0, NULL);
    return ret;      
}
bool BluetoothSetAmpName(void)
{
    bool ret =false;
    ret = AddBTState(REMOTE_COMMAND_BT_SETAMPNAME,1,0, NULL);
    return ret;      
}
bool BluetoothStartScaning()
{
    bool ret =false;
    ret = AddBTState(REMOTE_COMMAND_BT_STARTSCANNING,1,0, NULL);
    return ret;     
}
bool BluetoothDeleteUser(UINT8 slot)
{
    bool ret =false;
    ret = AddBTState(RCMD_GENERAL_SET_USER_DATA,1,slot, NULL);
    return ret;
}

void BluetoothSetDirectSendFuncation(SENDFUNC pfuc)
{
    gBluetoothData.remoteDevice.directSendMessage = pfuc;
}

char* BluetoothGetMacAddress(void)
{
    return gBluetoothData.remoteDevice.deviceMacAddress;
}

static void reSendFunc(void)
{
	gBluetoothData.remoteDevice.currentSubState = 0;
}
//UINT16 testtimer1=0;
//UINT16 testtimer2=0;
//UINT16 testtimer3=0;
static void BT_1ms_ISR(void)
{
    extern UINT8 gWaitOkTimeOut;
//    UINT8 times20;
//    if(times20>50)
//    {
//        times20 = 0;
//        AMP_PROCESS();
//    }
//    else
//    {
//        times20++;   
//    }
    if(gWaitOkTimeOut<60)
    {
        gWaitOkTimeOut++;

    }
    if(gBtCounter<30)
    {
        gBtCounter++;
    }
    if(++gBluetoothData.timerCounter>999)
    {
        gBluetoothData.timerCounter=0;
        if(((Program_Get_Status()>=PS_WAIT_3S)
			&&(Program_Get_Status() != PS_COMPLETED)
			&&(Program_Get_Status() != PS_PAUSED))
			&&(BT_Get_SyncFlag())
			)
        {
            if(1==Bluetooth_Get_ConnectStatus())
            {
//               AddBTState(RCMD_GENERAL_GET_WORKOUT_DATA, 0, 0, NULL);
            }
        }
		if((gBluetoothData.remoteDevice.reSend != 0xA5)||(gBluetoothData.remoteDevice.reSend  != 0xAA))
			gBluetoothData.remoteDevice.reSend ++;
    }
    if(WorkoutCompleteSync == 1)
    {
        if(++Delay1S>1000) 
        {
            WorkoutCompleteSync = 0;
            Delay1S = 0;
            BluetoothStopWorkout();
            
        }
    }
    if(++gBluetoothData.SendTimeOutCounter>3000)
    {
        gBluetoothData.SendTimeOutCounter=0;
        WltSetSendReady();
    }
}
void initialBluetoothLib()
{
    memset(&gBluetoothData, 0, sizeof(BluetoothDataStruct));
    memset(&CmdSend,0, sizeof(CmdSend));
    BluetoothMode=0;
	startScan=1;
    BT_Set_SyncFlag(false);
    gBluetoothData.scanDelaytimer=0;
    wlt2564Initial();
    //gBluetoothData.remoteDevice.devicename = "WLT 2564";
    gBluetoothData.remoteDevice.deviceBufSize = 0;
    gBluetoothData.remoteDevice.subStateSendMessage = NULL;
    gBluetoothData.remoteDevice.stateSendMessage = NULL;
	gBluetoothData.remoteDevice.reSendFuncation = reSendFunc;
//    gBluetoothData.remoteDevice.directSendMessage = NULL;
    gBluetoothData.remoteDevice.nextState = BTNextState;
    gBluetoothData.remoteDevice.addResponseState = AddBTState;
    RemoteRegisterDevice(&gBluetoothData.remoteDevice);
    gBluetoothData.remoteDevice.currentState = REMOTE_PROPERTY_GENERAL;
//    	Hal_Regist_ISR(MS_ISR,BT_ISR_IDX,BT_1ms_ISR);
//	BluetoothSetBleName(0);
    extern UINT8 appStart;    //zsf
    appStart = 0;    //zsf
}
void ResetinitialBluetoothLib()
{
    memset(&gBluetoothData, 0, sizeof(BluetoothDataStruct));
    wlt2564ResetInitial();
    //gBluetoothData.remoteDevice.devicename = "WLT 2564";
    gBluetoothData.remoteDevice.deviceBufSize = 0;
    gBluetoothData.remoteDevice.subStateSendMessage = NULL;
    gBluetoothData.remoteDevice.stateSendMessage = NULL;
	gBluetoothData.remoteDevice.reSendFuncation = reSendFunc;
//    gBluetoothData.remoteDevice.directSendMessage = NULL;
    gBluetoothData.remoteDevice.nextState = BTNextState;
    gBluetoothData.remoteDevice.addResponseState = AddBTState;
    RemoteRegisterDevice(&gBluetoothData.remoteDevice);
    gBluetoothData.remoteDevice.currentState = REMOTE_PROPERTY_GENERAL;
//    	Hal_Regist_ISR(MS_ISR,BT_ISR_IDX,BT_1ms_ISR);
//	BluetoothSetBleName(0);
}
void BT_Regist_Functions(void)
{
    Hal_Regist_ISR(MS_ISR,BT_ISR_IDX,BT_1ms_ISR);
    Hal_Regist_ISR(UART0_RX_ISR , NULL , WltReciveInterrupt);
    Hal_Regist_ISR(UART0_TX_ISR , NULL , WltSendInterrupt);
}

void  Bluetooth_Set_VirtualKey(UINT16 vk)
{
    FRAME_STAGE stage = (FRAME_STAGE)(vk&0xFF00);
    UINT8 realKey = vk&0x00ff;
    
    if(realKey == VK_BT_UPDATE)    //zsf
    {
      if(BluetoothMode == 1 ) return;
      BluetoothStartUpdate(0); 
    }
    if((realKey != VK_NULL) && wlt2564_Get_UpdateStatu() == UARTUPDATE_FAIL)     //zsf  提示bt更新失败时按任意键退出
    {
        Event_Handler_SetNewState(FRAME_STAGE_USER);
        User_Set_VirtualKey(VK_INITIAL);
        HAL_Reset_System(); 
    }
        
	if(Bluetooth_Get_ConnectStatus() ==0 && realKey != VK_BT_CHANGMODE) return;
    switch(realKey)
    {
    	case VK_BT_CHANGMODE:
        {
		   if( stage != FRAME_STAGE_PRGRUN )		
		   {
                if(Bluetooth_Get_ConnectStatus() && !b_SyncSuccessFlag)break;
	            if(BluetoothMode==0)
	            {
	                BluetoothMode=1;
	                wlt2564_Set_Mode(1);
	            }
	            else
	            {
	                AddBTState(REMOTE_COMMAND_BT_STOPSCANNING,1,0, NULL);
	                BluetoothMode=0;
	                wlt2564_Set_Mode(0);
	            }
	            BluetoothSwitchMode(BluetoothMode);    
		   	}
        }
        break;
        case VK_PAUSE_START:
		if(BluetoothMode == 1 || stage == FRMAE_STATE_DEMOMODE) return;
//        clear_buffer();
        if(Program_Get_Status() <= PS_PAUSED)
        {
            if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
            {
                if(Data_Get_RPM(MOTOR_RPM) || stage == FRAME_STAGE_SAFFKEYOFF)
                {
                    break;
                }
            }
            if(Program_Get_Status() < PS_RESTART_321)  
            {
                if(1==Bluetooth_Get_ConnectStatus())
                {
                    BluetoothStartWorkout();
                }
            }
            else
            {
                BluetoothPauseWorkout(1);
            }
        }
        else
        {
            BluetoothPauseWorkout(0);
        }
        break;	
        case VK_START:
#ifdef TM
		if(BluetoothMode == 1 || stage == FRMAE_STATE_DEMOMODE || Data_Get_RPM(MOTOR_RPM) != 0) return;    //zsf
#else
		if(BluetoothMode == 1 || stage == FRMAE_STATE_DEMOMODE) return;
#endif		
        if(Program_Get_Status() < PS_RESTART_321)  
        {
            BluetoothStartWorkout();
        }
        else
        {
            if(Program_Get_Status() == PS_RESTART_321)
                BluetoothPauseWorkout(1);  
        }
        break;	
		case VK_PAUSE:
		if(BluetoothMode == 1 || stage == FRMAE_STATE_DEMOMODE) return;
        if(PS_WARMUP <= Program_Get_Status() && Program_Get_Status() <= PS_COOLDOWN)
            BluetoothPauseWorkout(0);
        break;	
        case VK_WORKOUT_COMPLETE:
        case VK_RESET:
		if(BluetoothMode == 1 ) return;	
        if(PS_WAIT_3S <= Program_Get_Status() && Program_Get_Status() <= PS_COOLDOWN)
        {
            WorkoutCompleteSync = 1;
            BluetoothGetWorkoutData();
        }
        break;	  
        case VK_SAFEKEYOFF:
        BluetoothSetPopUpInformation(0x50);       
        break;
        case VK_SAFEKEYON:
        BluetoothSetPopUpInformation(0x51);
        break;
        case VK_DEMO:    //zsf
        BluetoothSetPopUpInformation(0x52);
        break;
        case VK_ENG:    //zsf
        BluetoothSetPopUpInformation(0x53);
        break;
        case VK_1WEEKMISSED:
		if(BluetoothMode == 1 ) return;
//            BluetoothSetPopUpInformation(0x05,0xa5);
        break;
        case VK_2WEEKMISSED:
		if(BluetoothMode == 1 ) return;
//            BluetoothSetPopUpInformation(0x06,0xa5);
        break;
	}
}
UINT8 Bluetooth_Get_HeartData(void)
{
	return wlt2564_Get_HeartData();
}
UINT8 Bluetooth_Get_ConnectStatus(void)
{
	return wlt2564_Get_ConnectStatus();
}
UINT8 Bluetooth_Get_BeltConnectStatus(void)
{
	return wlt2564GetChestBeltConnectStatus();
}
UINT8 Bluetooth_Get_BT_LED_Status(void)
{
    return  wlt2564_Get_BT_LED_Statu();
}
void CleargBluetoothData(void)
{
//    memset(&gBluetoothData.currentIndex, 0, 3);
    ResetinitialBluetoothLib();
    BT_Set_SyncFlag(false);
}
//#endif
