#include "remote_config.h"

#include "wlt2564_command.h"
#include "Remote_datatype.h"
#include "Remote_port.h"
#include "remote_general.h"
#include "HAL_Lib.h"
#ifdef __BLUETOOTH_WLT2654__

#define MAX_WLT_BUFFER 1024
#define BLE_DEVADDRESS_LENGTH 14
//extern UINT16 testtimer3;
//UINT16 COUNTFLAG=0;
void WltReciveInterrupt(void);
void WltSendInterrupt(void);
void WltReceiveInterruptFuncation(UINT8 ch);
UINT16 WltGetReciveData(UINT8* data);
//extern UINT16 testtimer1;
//extern UINT16 testtimer2;
extern unsigned char BleName[14];
extern unsigned char AmpName[19]; 
typedef struct{
    UINT16 total;
    UINT16 processIndex;
    unsigned char DeviceAddress[BLE_DEVADDRESS_LENGTH];
    unsigned char buf[MAX_WLT_BUFFER];
}BluetoothData;
typedef struct{
UINT8 HeartData;
    UINT8 Mode:1;
    UINT8 AppConnected:1;
    UINT8 Scanning:1;
    UINT8 ReceiveSacnOk:1;
    UINT8 HeartOk:1;
    UINT8 AmpConnected:1;
    UINT8 SetBleNameFlag:1;
    UINT8 InitialCmdStatu:4;
    UINT8 BtBeatConnect:1;
    UINT8 UpdateCmdStatu:3;  //zsf
    UINT8 uartUpdateCmdFlag:4;  //zsf
}WLT2564Status;
__no_init static UINT16 gWaitFlag;
__no_init static bool gSendReady;
UINT8 gWaitOkTimeOut;
unsigned char BT_MacAddress[4];
unsigned char BT_Version[8];
unsigned char BT_UpdateInfo[5];    //zsf
//__no_init UCHAR BT_BEAT_ConnectFlag;
//__no_init static UINT16 processDataIndex;
//__no_init UCHAR BT2564H_SendReady;

//static UINT8 debugbuf[MAX_WLT_BUFFER] = {0};

static WLT2564Status wlt2564Status;
static BluetoothData gSend,gRecive;
unsigned char BlueheartAddressLength[16];


UINT8 BT_UpdatePercent;   //zsf
DWORD updatedLength = 0;   //zsf
extern unsigned char g_BtUpdateFileDate[256];  //zsf
UINT8 updateRecOK = 1;  //zsf

const static char* BluetoothCMD[BTCMD_END]={
    "AT+HELP\r\n",
    "AT+REGISTERSPPLE\r\n",
    "AT+GETMAC\r\n",
    "AT+STOPADV\r\n",
    "AT+STARTADV\r\n",
    "AT+GETUPDATEINFO\r\n",
    "AT+SPPLESEND=",
    "AT+ECHO=0\r\n",
    "AT+REGISTERAVRCP=1\r\n",
    "AT+AUDINIT\r\n",
    "AT+SETDISCOVERYMODE=0\r\n",  //zsf
    "AT+PINCODE=0000\r\n" ,
    "AT+MASTER",           
    "AT+SLAVER",
    "AT+STARTSCANNING",
    "AT+STOPSCANNING",
    "AT+CONNECTLE=",
    "AT+SETBLENAME=",
    "AT+SETLOCALNAME=",
    "AT+GETBLENAME",
    "AT+MRESET\r\n",
    "AT+GETLOCALNAME",
    "AT+ADVERTISELE=0",    //ZSF
    "AT+SETCONNECTMODE=0",    //ZSF
    "AT+UARTUPDATE",  //zsf
};

const static char* BluetoothRecive[BTRECIVE_END]={
    "+IND=BLECONNECTED,",
    "Start WLT2564M_",  
    "+IND=BLEDISCONNECTED", 
    "+IND=B",    //zsf
    "+IND=SPPLEREAD,",
    "+IND=SPPLELen:",
    "+ERR",
#ifdef TM
    "+OK=3.7AT SPEAKERS",
#else
    "+OK=3.7HT SPEAKERS",
#endif
    "+OK=WLT2564M",
    "+OK=",
    "+OK",
    "0x",
    "+IND=AC",
    "+IND=AD",  
    "+IND=AFG Pro_7.2AT ",
    "+IND=AFG Sport_",
    "+IND=Enter AT Cmd mode",
    "+IND=Slaver",
};
//static bool  wlt2564SendData(UINT8* buf, UINT16 length);

INT16 FindSubString(const UINT8 *str, UINT16 strLen, const char *targetString,UINT16 targetLenth,UINT16 startp)
{
    UINT16 j,i;
    INT16 ret = -1;
    if(targetLenth > strLen)
    {
        return ret;
    }
    for(i =startp; i<strLen- targetLenth; i++)
    {
    	if((i%MAX_WLT_BUFFER == startp)&&(i!=startp))
			return ret;
        if(str[i%MAX_WLT_BUFFER] != targetString[0] )
        {
            continue;
        }
        j = 1;
        for(;j<targetLenth;j++)
        {
            if(str[(i+j)%MAX_WLT_BUFFER] != targetString[j] )
            {
                break;
            }
        }
        if(j == targetLenth)
        {
            ret = (i+j)%MAX_WLT_BUFFER -1;
            if( ret == -1 )
            {
                ret = 0;   
            }
            return ret;
        }
	 
    }
    return ret;
}

void wlt2564ResetInitial(void)
{
    memset(&gSend,0, sizeof(BluetoothData));
    memset(&gRecive,0, sizeof(BluetoothData));
    gSendReady=true;
    gWaitFlag=0;
    gWaitOkTimeOut = 0x55;
	BluetoothSetDirectSendFuncation(wlt2564SendData);
}
void wlt2564Initial(void)
{
    memset(&gSend,0, sizeof(BluetoothData));
    memset(&gRecive,0, sizeof(BluetoothData));
    memset(&wlt2564Status,0, sizeof(WLT2564Status));
    gSendReady=true;
    gWaitFlag=0;
    gWaitOkTimeOut = 0x55;
	wlt2564Status.Mode=0;
    wlt2564Status.SetBleNameFlag = 0;
    wlt2564Status.InitialCmdStatu = 0;
    wlt2564Status.UpdateCmdStatu = 0;  //zsf
    wlt2564Status.uartUpdateCmdFlag = 0;    //zsf
	BluetoothSetDirectSendFuncation(wlt2564SendData);
}

bool  wlt2564SendUpdateData(UINT8* buf, UINT16 length)    //zsf   
{
    bool ret = false;
    if(gSendReady)
    {
        memset(gSend.buf, 0, sizeof(gSend.buf));
        gSendReady = false;
        gSend.processIndex = 0;        
        ret = true;
        memcpy(gSend.buf, buf, length);
        gSend.total = length;
        gWaitFlag = length;
        HAL_BT_Enable_UARTI(1)	;
    }
    return ret;
}

bool  wlt2564SendData(UINT8* buf, UINT16 length)
{
    bool ret = false;
    if(gSendReady )
    {
        char str[4];
        UINT16 index = strlen(BluetoothCMD[BTCMD_SENDDATA]);
        memset(gSend.buf, 0, sizeof(gSend.buf));
        gSendReady = false;
        gSend.processIndex = 0;        
        ret = true;
        sprintf(str, "%d", length);
        memcpy(gSend.buf, BluetoothCMD[BTCMD_SENDDATA], index);
        memcpy(gSend.buf+index, str, strlen(str));
        index += strlen(str);
        *(gSend.buf+index) = ',';
        index ++;
        memcpy(gSend.buf+index, gRecive.DeviceAddress, BLE_DEVADDRESS_LENGTH);
        index += BLE_DEVADDRESS_LENGTH;
        *(gSend.buf+index) = '\r';
        index ++;
        *(gSend.buf+index) = '\n';
        index ++;
        gWaitFlag = index;
        memcpy(gSend.buf+index, buf, length);
        index += length;
        *(gSend.buf+index) = '\r';
        index ++;
        *(gSend.buf+index) = '\n';
        index ++;
        gSend.total = index;
        HAL_BT_Enable_UARTI(1)	;
    }
    return ret;
}
bool  wlt2564SendCommand(UINT8 Cmd,UINT8* buf, UINT16 length)
{
    bool ret = false;
    if(Cmd==BTCMD_SENDDATA ||Cmd>=BTCMD_END)return ret;
    if(gSendReady )
    {
        UINT16 index = strlen(BluetoothCMD[Cmd]);
        gSendReady = false;
        memset(gSend.buf, 0, sizeof(gSend.buf));
        gSend.processIndex =0;
        memcpy(gSend.buf, BluetoothCMD[Cmd], index);
        memcpy(gSend.buf+index, buf, length);
        index += length;
        *(gSend.buf+index) = '\r';
        index ++;
        *(gSend.buf+index) = '\n';
        index ++;
        gWaitFlag = index;
        gSend.total = index;
        HAL_BT_Enable_UARTI(1);
        ret = true;
    }
    return ret;
}
void WltReciveInterrupt(void)
{
    WltReceiveInterruptFuncation(HAL_BT_GetData());
}
void WltSetSendReady(void)
{
    if(gSendReady==false)
    {
        gSendReady=true;
    }
}
UINT8 gBtCounter;

void WltSendInterrupt(void)
{
    if( gWaitFlag != gSend.processIndex )
    {
        gBtCounter = 0;
        gWaitOkTimeOut = 0xaa;
        HAL_BT_SendData(gSend.buf[gSend.processIndex++]);
    }
    else
    {
        if(gWaitFlag == gSend.total)
        {
            gSendReady=true;
            gWaitOkTimeOut = 0x55;
        }
        else
        {
           gWaitOkTimeOut = 0;
        }
        HAL_BT_Enable_UARTI(0);
    }
}
void WltReceiveInterruptFuncation(UINT8 ch)
{
    gRecive.buf[gRecive.total] = ch;
    gRecive.total++;
    if( gRecive.total >= MAX_WLT_BUFFER )
    {
        gRecive.total = 0;
    }

    //解析返回数据
    
//     WltGetReciveData(gRecive.processIndex);
    /*
    WltGetReciveData(BluetoothData.processIndex);
    memset(gHiflyRecive.buf, 0, gHiflyRecive.totalIndex);
    gHiflyRecive.totalIndex =0;
    gHiflyRecive.processedIndex = 0;
    */
}

UINT16 WltGetReciveData(UINT8* data)
{
  INT16 adposization = 0;
  INT16 dataPosization = 0;
  //    UINT16 adposizationbackup = 0;
  //    UINT16 okPosization = 0;
  //    UINT16 errPosization = 0;
  UINT16 receivePosization = 0;
  UINT16 connectPosization = 0;
  UINT16 blenamePosization = 0;
  UINT16 btUpdateInfoPosization = 0;  //zsf
  //    UINT16 disconnectPosization = 0;
  UINT16 othercount = 0;
  UINT16 totalcount = 0;
  UINT16 okcount = 0;
  UINT16 errorcount = 0;
  UINT16 macaddressPosization = 0;
  UINT16 retdatalength = 0;
  UINT16 btgetversionPosization = 0;
  BTRECIVE i = BTRECIVE_START;
  //    static UCHAR DataIndex = 0;
  static UINT8 packetbuffer[500] = {0};
  static UINT16 packetdatalength=0;
  static UINT16 packetdataindex=0;
  static UINT16 packetstartflag=0;
  extern UINT ToRead;
  
  if((gWaitOkTimeOut!=0X55)&&(gWaitOkTimeOut!=0XAA)&&(gWaitOkTimeOut>10)&&(gSendReady == 0))
  {
    gWaitFlag = gSend.total;
    HAL_BT_Enable_UARTI(1); 
  }
  
  if (GetUpdateFlag() >= UARTUPDATE_DATA_START)     //zsf
  {
      UINT16 checkSumIndex = 0;
      UINT16 dataIndex = 0;
      
      if ((gRecive.buf[gRecive.processIndex]==0x5A) && ((gRecive.total - gRecive.processIndex == 5) || (1024 - gRecive.processIndex + gRecive.total == 5)))
      {
          updateRecOK = 1;
        
          checkSumIndex = gRecive.processIndex + 1;
          if (checkSumIndex > 1023)
          {
              checkSumIndex  -= 1024;
          }
      
          dataIndex = gRecive.processIndex + 4;
          if (dataIndex > 1023)
          {
              dataIndex  -= 1024;
          } 
      
          if (gRecive.buf[checkSumIndex] + gRecive.buf[dataIndex] == 0xFF)
          {
              if (gRecive.buf[gRecive.processIndex]==0x5A)
              {
                  if (gRecive.buf[checkSumIndex] == 0xFF)   
                  {
                      SetUpdateFlag(UARTUPDATE_DATA_OK);
                      ToRead = 1;
                  }
                  else if (gRecive.buf[checkSumIndex]== 0x01)
                  {
                      SetUpdateFlag(UARTUPDATE_DATA_COMPLETE);
                      wlt2564Status.UpdateCmdStatu = UARTUPDATE_COMPLETE;
                  }
                  else
                  {
                      SetUpdateFlag(UARTUPDATE_DATA_ERR);
                  }
                  retdatalength = 5;
                  gRecive.processIndex += 5;
                  if (gRecive.processIndex > 1023)
                  {
                      gRecive.processIndex = gRecive.processIndex - 1024;
                  }
              
              } 
          }
      }   
      
      return retdatalength;
  }
  
  adposization = FindSubString(gRecive.buf,MAX_WLT_BUFFER*2,"\r\n",2,gRecive.processIndex);
  if(adposization == -1)
    return retdatalength;
  //    adposizationbackup=adposization;
  adposization +=1;
  for(; i<BTRECIVE_END; i++)
  {
    dataPosization =FindSubString(gRecive.buf,MAX_WLT_BUFFER*2,BluetoothRecive[i],strlen(BluetoothRecive[i]),gRecive.processIndex);
    if((BTCMD_RECVDATA == i)&&(dataPosization != -1)){
    }
    if(dataPosization>=0 && dataPosization<1024)
      break;
  }
  
  totalcount = totalcount+1;
  if( (dataPosization == -1)||(i == BTRECIVE_END ))
  {
    if(adposization>gRecive.processIndex)
    {
      memset(gRecive.buf+gRecive.processIndex, 0, adposization- gRecive.processIndex);
    }
    else
    {
      memset(gRecive.buf+gRecive.processIndex, 0, MAX_WLT_BUFFER-gRecive.processIndex);
      memset(gRecive.buf, 0, adposization);
    }
    gRecive.processIndex=adposization;
    othercount++;
    return retdatalength;
  }
  
  dataPosization +=1;
  
  switch(i)
  {
  case BTRECIVE_OK:
    {
      //            okPosization=dataPosization;
      okcount++;
      if(wlt2564Status.Mode)
      {
        if(wlt2564Status.Scanning)	
        {
          wlt2564Status.Scanning=false;
          wlt2564Status.ReceiveSacnOk=true;
        }
      }
      if(gWaitFlag != gSend.total)
      {
        gWaitFlag = gSend.total;
        HAL_BT_Enable_UARTI(1);
      }
      if(GetBleNameFlag())
      {
        wlt2564_Set_CmdStatu(0);
        SetBleNameFlag(0);
        wlt2564Status.InitialCmdStatu = SEND_CMD_SETBLENAME_COMPLETE;
      }
      if(GetUpdateFlag() == UARTUPDATE_CMD_SEND)   //zsf
      {
        wlt2564_Set_CmdStatu(0);       
        wlt2564Status.UpdateCmdStatu = UPDATE_CMD_UARTUPDATE_COMPLETE;
      }
      if(GetUpdateFlag() == UARTUPDATE_CMD_ADVERTISELE)   //zsf
      {
        wlt2564_Set_CmdStatu(0);       
        wlt2564Status.UpdateCmdStatu = STOP_CMD_ADVERTISELE_COMPLETE;
      }
      if(GetUpdateFlag() == UARTUPDATE_CMD_SETDISCOVERYMODE)   //zsf
      {
        wlt2564_Set_CmdStatu(0);       
        wlt2564Status.UpdateCmdStatu = STOP_CMD_SETDISCOVERYMODE_COMPLETE;
      }
      if(GetUpdateFlag() == UARTUPDATE_CMD_SETCONNECTMODE)   //zsf
      {
        wlt2564_Set_CmdStatu(0);       
        wlt2564Status.UpdateCmdStatu = STOP_CMD_SETCONNECTMODE_COMPLETE;
      }
    }
    break;
  case BTRECIVE_ERR:
    {
      errorcount++;
      //            errPosization=dataPosization;
    }
    break;
  case BTCMD_RECVDATA:
  {
      receivePosization=dataPosization;
      UINT8 length = 0;
      int j=0;
      UINT8* pRecvData;
      UINT8 ReceveData[1024] = {0};
      if(dataPosization>adposization)
      {        
          memcpy(ReceveData,gRecive.buf+receivePosization,MAX_WLT_BUFFER-receivePosization);
          memcpy(ReceveData+MAX_WLT_BUFFER-receivePosization,gRecive.buf,receivePosization);
      }
      else
      {
          memcpy(ReceveData,gRecive.buf+receivePosization,adposization- receivePosization);
      }
      pRecvData = ReceveData;
      for(; j<3; j++)
      {
          if(*(pRecvData+j) == ',')
              break;
          length *= 10;
          length = length +*(pRecvData+j)-'0';;
      }
      pRecvData = pRecvData +j+1; 
      if(wlt2564Status.Mode)
      {
          pRecvData +=1;
          wlt2564Status.HeartData=*pRecvData;
          wlt2564Status.HeartOk=1;
      }
      else
      {
          if((*(pRecvData)==0x55&&*(pRecvData+1)==0xAA)&&(packetstartflag == 0))
          {
              packetdatalength =(*(pRecvData+7))<<8;
              packetdatalength +=*(pRecvData+6);
              packetdatalength += 10;
              packetstartflag=1;
              packetdataindex=0;
              memset(packetbuffer,0,500);
          }
          if (adposization - (receivePosization+j+1) < length)    //zsf
          {
              memcpy(pRecvData,gRecive.buf+receivePosization+j+1,length);
          }
          if(packetstartflag==1)
          {
              memcpy(packetbuffer+packetdataindex, pRecvData, length);
              packetdataindex+=length;
              if(packetdatalength>packetdataindex)
              {
                  if(length<15)
                  {
                      retdatalength=packetdataindex;
                      memcpy(data, packetbuffer, retdatalength);
                      memset(packetbuffer,0,500);
                      packetstartflag=0;
                      
                  }
                  else
                  {
                      retdatalength=0;
                  }
                  
              } 
              else
              {
                  retdatalength=packetdatalength;
                  memcpy(data, packetbuffer, retdatalength);
                  memset(packetbuffer,0,500);
                  packetstartflag=0;
              }
          }
          else
          {
              retdatalength=0;
          }
      }
  }
    break;
  case BTCMD_CONNECTED:
    {
      connectPosization=dataPosization;
      packetdatalength=0;
      packetdataindex=0;
      packetstartflag=0;
      memset(packetbuffer,0,500);
      // ResetinitialBluetoothLib();
      if(wlt2564Status.Mode)
        wlt2564Status.BtBeatConnect=1;
      else
        wlt2564Status.AppConnected=1;
      if(dataPosization>adposization)
      {
        memcpy(gRecive.DeviceAddress,gRecive.buf+dataPosization,MAX_WLT_BUFFER-dataPosization);
        memcpy(gRecive.DeviceAddress+(MAX_WLT_BUFFER-dataPosization),gRecive.buf,adposization);
      }
      else
      {
        memcpy(gRecive.DeviceAddress,gRecive.buf+dataPosization,BLE_DEVADDRESS_LENGTH);
      }
      memset(gRecive.buf,0,1024);
    }
    break;
  case BTCMD_GET_VERSION:
    {
      btgetversionPosization=dataPosization;
      memcpy(BT_Version,gRecive.buf+btgetversionPosization,8);
      BT_Version[2] = ' ';
    }
    break;
  case BTCMD_AC:
    wlt2564Status.AmpConnected=1;
    break;
  case BTCMD_DISCONNECTED:
    {
      CleargBluetoothData();
      wlt2564Status.AppConnected=0;
      wlt2564Status.BtBeatConnect=0;
      wlt2564Status.HeartOk=0;
      wlt2564Status.HeartData = 0;
      if(wlt2564Status.Mode)
      {
        wlt2564SendCommand(BTCMD_CONNECTLE,BlueheartAddressLength,16);
      }
    }
    break;
  case BTCMD_AD:
    wlt2564Status.AmpConnected=0;
    break;
  case BTCMD_SETBLENAMEFLAG:
    macaddressPosization = dataPosization;
    strncat(BT_MacAddress,gRecive.buf+macaddressPosization,4);
    wlt2564Status.InitialCmdStatu = SEND_CMD_SETBLENAME_COMPLETE;  
    
    break;
  case BTCMD_BLENAMEWRONG:
    blenamePosization = dataPosization;
    strncat(BleName,gRecive.buf+blenamePosization,4);
    strncat(AmpName,gRecive.buf+blenamePosization,4);
    strncat(BT_MacAddress,gRecive.buf+blenamePosization,4);
    memset(gRecive.buf,0,1024);
    wlt2564Status.InitialCmdStatu = SEND_CMD_GETBLENAME_COMPLETE;
    break;
  case BTCMD_INITIAL:
    {
      wlt2564Status.InitialCmdStatu = SEND_CMD_INITIAL;
    }
    break;
  case BTCMD_INITIALCOMPLETE:
    {
      if(wlt2564Status.InitialCmdStatu == SEND_CMD_INITIAL)
        wlt2564Status.InitialCmdStatu = SEND_CMD_INITIAL_COMPLETE;
    }
    break;
  case BTCMD_MACADDRESS:
    {
      //            macaddressPosization = dataPosization;
      if((wlt2564Status.AppConnected)||(gRecive.buf[macaddressPosization+7]!=0x0D))
      {
        //                sprintf(BT_MacAddress,"----");
      }
      else
      {
        //                sprintf(BT_MacAddress,"%X%X",gRecive.buf[macaddressPosization+4],gRecive.buf[macaddressPosization+5]);
      }
      wlt2564Status.InitialCmdStatu = SEND_CMD_GETMAC_COMPLETE;
    }
    break;
  case BTCMD_GETDEALOCALNAME:
    {
      wlt2564Status.InitialCmdStatu = SEND_CMD_GETAMPNAME_COMPLETE;
    }
    break;
  case BTCMD_GETCHANGEDLOCALNAME:
    {
      wlt2564Status.InitialCmdStatu = SEND_CMD_START;
    }
    break;
  case BTCMD_GET_UPDATEINFO:    //zsf
    {
      btUpdateInfoPosization = dataPosization;
      strncat(BT_UpdateInfo,gRecive.buf+btUpdateInfoPosization,5);
      if (BT_UpdateInfo[0] == '4') 
      {
         if (((BT_UpdateInfo[2] == '2') && (BT_UpdateInfo[4] >= '2')) || (BT_UpdateInfo[2] > '2'))
         {
             wlt2564Status.UpdateCmdStatu = UPDATE_CMD_GETUPDATEINFO_COMPLETE;
         }
      }
      else if (BT_UpdateInfo[0] > '4' && BT_UpdateInfo[0] <= '9')
      {
          wlt2564Status.UpdateCmdStatu = UPDATE_CMD_GETUPDATEINFO_COMPLETE;
      }              
    }
    break;
  default:
    break;
  }
  if(wlt2564Status.Mode)
  {
    if(i == BTRECIVE_0X)
    {
      if(wlt2564Status.ReceiveSacnOk)
      {
        wlt2564Status.ReceiveSacnOk=false;
        if(dataPosization>adposization)
        {
          memcpy(BlueheartAddressLength,gRecive.buf+dataPosization-2,MAX_WLT_BUFFER-dataPosization);
          memcpy(BlueheartAddressLength+(MAX_WLT_BUFFER+2-dataPosization),gRecive.buf,adposization);
        }
        else
        {
          memcpy(BlueheartAddressLength,gRecive.buf+dataPosization-2,16);
        }
        wlt2564SendCommand(BTCMD_CONNECTLE,BlueheartAddressLength,16);
      }
    }
  }
  if(adposization>gRecive.processIndex)
  {
    memset(gRecive.buf+gRecive.processIndex, 0, adposization- gRecive.processIndex);
  }
  else
  {
    memset(gRecive.buf+gRecive.processIndex, 0, MAX_WLT_BUFFER-gRecive.processIndex);
    memset(gRecive.buf, 0, adposization);
  }
  gRecive.processIndex=adposization;
  return retdatalength;
}
UINT8 wlt2564_Get_Mode(void)
{
   return wlt2564Status.Mode;
}
void wlt2564_Set_Mode(unsigned char modetype)
{
    wlt2564Status.Mode = modetype;
}
UINT8 wlt2564_Get_HeartData(void)
{
    UINT8 ret = 0 ;
    //if(wlt2564Status.Mode == 1 )
        ret = wlt2564Status.HeartData;
    return ret ;
}
void wlt2564_Set_HeartData(UINT8 heartrate)
{
   wlt2564Status.HeartData = heartrate;
}
UINT8 wlt2564_Get_ConnectStatus(void)
{
    if(wlt2564Status.Mode == 0 )
        return wlt2564Status.AppConnected;
    else
        return 0;
}
UINT8 wlt2564GetAmpConnectStatus(void)
{
    return wlt2564Status.AmpConnected;
}
UINT8 wlt2564GetChestBeltConnectStatus(void)
{
    if(wlt2564Status.Mode == 1 )
       return wlt2564Status.BtBeatConnect;
    else
       return 0;
}
UINT8 wlt2564_Get_CmdStatu()
{
    return wlt2564Status.InitialCmdStatu;
}
void wlt2564_Set_CmdStatu(unsigned char i)
{
    wlt2564Status.InitialCmdStatu = i;
}

UINT8 wlt2564_Get_UpdateStatu()    //zsf
{
    return wlt2564Status.UpdateCmdStatu;
}
void wlt2564_Set_UpdateStatu(unsigned char i)    //zsf
{
    wlt2564Status.UpdateCmdStatu = i;
}

UINT8 wlt2564_Get_BT_LED_Statu(void)
{
    if((wlt2564Status.AppConnected==1)||(wlt2564Status.AmpConnected==1)||(wlt2564Status.BtBeatConnect==1))
        return 1;
    else 
        return 0;
}
bool wlt2564MasterSlaverModeSwitch(char  mode)
{
    bool ret = false;
    if(mode!=0)
    {
        if(wlt2564SendCommand(BTCMD_MASTERMODE,NULL,0))
        {
            ret = true;
        }
    }
    else
    {
        if(wlt2564SendCommand(BTCMD_SLAVERMODE,NULL,0))
        {
            ret = true;
        }
    }
    return ret;
}
bool wlt2564StartScaning(void)
{
  bool ret = false;
    if(wlt2564SendCommand(BTCMD_STARTSCANNING,NULL,0))
    {
	wlt2564Status.Scanning=true;
      ret = true;
    }
    return ret;
}
bool wlt2564StopScaning(void)
{
    bool ret = false;
    if(wlt2564SendCommand(BTCMD_STOPSCANNING,NULL,0))
    {
        ret = true;
    }
    return ret;
}
void Wlt2564_Reset(void)
{
    wlt2564SendCommand(BTCMD_MRESET,NULL,0);
}
void GetWlt2564MAC(void)
{
    wlt2564SendCommand(BTCMD_GETMAC,NULL,0);
}
UINT8 GetBleNameFlag()
{
    return wlt2564Status.SetBleNameFlag;   
}
void SetBleNameFlag(char i)
{
    wlt2564Status.SetBleNameFlag = i;
}

UINT8 GetUpdateFlag()    //zsf
{
    return wlt2564Status.uartUpdateCmdFlag;   
}
void SetUpdateFlag(char i)   //zsf
{
    wlt2564Status.uartUpdateCmdFlag = i;
}

UINT8 *BT_Get_BTMac(void)
{
    return BT_MacAddress;
}
UINT8 *BT_Get_BT_Version(void)
{
    return BT_Version;
}

void UpdateBluetoothProcess(void)     //zsf
{
    static UINT8 buf[260]={0};
    UpdateHeader *updateHeader =  (UpdateHeader*)buf;
    unsigned char checkSum = 0;   
    static UINT16 bufLength = 0;
    static UINT8 errCnt = 0;
    int i = 0;
    //extern static unsigned char g_BtUpdateFileDate[256];
    UINT16 readLength = 0;
    extern UINT ToWrite;
    extern DWORD   BtUpdateFileSize;
    extern UINT OverRead;
    extern UINT8 BtLastSize;
    
    //extern UINT ToRead;
    
    if (ToWrite && (gSendReady == 1) && (1 == updateRecOK))
    {
      updateRecOK = 0;
      ToWrite = 0;
      
      switch (GetUpdateFlag())
      {
        case UARTUPDATE_DATA_START:
        case UARTUPDATE_DATA_OK: 
            errCnt = 0;
            updateHeader->syncWord = 0xA5;
            updateHeader->checksum= 0;
            updateHeader->dataSize= 0;
            
            if (OverRead)
              readLength = BtLastSize;
            else
              readLength = 256;
            for (i = 0; i < readLength; i++)
            {
               //if (g_BtUpdateFileDate[i] != '\0')
               //{
                   buf[i + 4] = g_BtUpdateFileDate[i]; 
                   checkSum += g_BtUpdateFileDate[i];
               //}
               //else                
                    //break;
            }
            buf[2] = i>>8;
            buf[3] = i;
        
	    if(updateHeader->dataSize>0)
		updateHeader->checksum = 0xFF - checkSum;
        
            bufLength=sizeof(UpdateHeader)+i;
                
            wlt2564SendUpdateData(buf, bufLength);
            
            updatedLength += bufLength;
            BT_UpdatePercent = updatedLength * 100/BtUpdateFileSize;
            if (BT_UpdatePercent > 100)
              BT_UpdatePercent = 100;
            
            
            //ToRead = 1;
            break;
        case UARTUPDATE_DATA_ERR:
            errCnt++;
            if (errCnt < 3)
                wlt2564SendUpdateData(buf, bufLength);
            if (OverRead)
                SetUpdateFlag(UARTUPDATE_DATA_FAIL);
                wlt2564Status.UpdateCmdStatu = UARTUPDATE_FAIL;
            break;

        default:
            break;
        }
    }
}
#endif
