#include "Digital_wifi.h"
#ifdef __WIFI_FUNC__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SystemConfig.h"
#include "Hal_lib.h"
#include "System_config.h"
#include "UtilitiesRtc.h"
#include "dmactrl.h"
#include "em_dma.h"
#include "em_prs.h"
#include "CRC.H"
#include "Wifi_interface.h"
#include "Wifi_event_handlers.h"
#include  "SPI_Flash.h"

#define DRVE_START_wifi      0XF1
#define DRVE_COMML      0X01
#define DRVE_COMMH      0X05
#define DRVE_LENL      0X0C
#define DRVE_LENH      0X00f
#define DRVE_END         0XF2

#define Null    0xff
#define WIFI_MAX_TIMER  400//1000

typedef enum
{
	RStart_wifi,
	RCommdL,
	RCommdH,
	RLentL,
	RLentH,
	RData,
	RCRCL,
	RCRCH, //RCRCHH,//
	REND,
	RIDLE
} RECV_STR;

typedef enum
{
	DStart,
	DCommdL,
	DCommdH, //
	DLentL,
	DLentH, //
	DData,
	DCRCL,
	DCRCH, //
	DEND,
	DIDLE
} DRVE_STR;



typedef struct
{
	UCHAR Cmd;
	UCHAR D_Len;
	UCHAR U_Len;
	UCHAR Index;
} A_CMD;

typedef struct
{
	UCHAR Index;
	UCHAR Width;
} A_Data;


__no_init static WIFI_EVENT_HANDLE gWiFiPerEvent;

__no_init WIFI_SETTINGS wifi_settings;
static UINT16 by_10ms_timer = 0;
#ifdef __WIFI_NO_RTC__
    static UINT16 gIntNetworkTimeDelay;
#endif
static UINT16 by_10ms_timer2 = 0;
static UCHAR  wifiPoweronFinish = 0;
static UCHAR  wifiStateChangeFinish = 0;
__no_init static 
struct{
  UCHAR BinfileDownloadToWifiModuleOK:1;
  UCHAR BinfileReadToSPIFlashOK:1;
  UCHAR ReadFisrt:1;
  UCHAR Read256BytesOK:1;    
  UCHAR BinfileDownloadToWifiModuleFail:1;
  UINT32 ReadBinCurrentAddress;
  UINT32 RecvBinAddress;
	UINT16 ReadTimeoutCounter;
  UCHAR ReadBuffer[SOFTWARE_READ_SIZE];
}RSCU;
#if 0
__no_init static UCHAR key_backup;
#endif

#ifdef __WIFI_RESEND_0502__
static UINT16 startTime;
static bool hadSend0502;
#endif



__no_init static UINT16 wifi_send_data_index;
__no_init static UINT16 by_RX0_wifi;
#ifndef __WIFI_DMA__
__no_init static UINT16 by_xMs_Timer_wifi;
#endif
__no_init RECV_STR R_State_wifi;
#ifndef __WIFI_RX_DMA__
static MSG_RECV _MSG_BUFFER;
#endif
static MSG_RECV _MSG_SEND;
static DRVE_STR Local_Dstate = DStart;
static int send_ready = 0;

extern char WiFiTestingFlag;
extern UINT16 WiFiTestDelay;

#ifdef __WIFI_DMA__
volatile bool dataAvailable = false;

#define DMA_CHANNEL_RX       0
#define DMA_CHANNEL_TX       1
#define DMA_CHANNELS         2
#define RX_BUFFER_SIZE       1024 /* Even numbers only */
#define TX_BUFFER_SIZE       RX_BUFFER_SIZE

#define RX_TIMEOUT_MS     2000
#ifdef __WIFI_RX_DMA__
struct 
{
  volatile char buffer[RX_BUFFER_SIZE];
  volatile int startIndex;  /* Position of first unread data */
  volatile int stopIndex;   /* Position after last unread data */
} rxBuffer = {.startIndex = 0, .stopIndex = 0};
#endif
/* DMA callback structure */
UCHAR rcComplete;

#ifdef __WIFI_RX_DMA__
void rxDmaComplete(unsigned int channel, bool primary, void *user);
#endif
void txDmaComplete(unsigned int channel, bool primary, void *user);

#ifdef __WIFI_RX_DMA__
void Process_Message(MSG_RECV currentMessage);
#endif

#ifdef __WIFI_RX_DMA__
__no_init char WIFI_RX_Channel;
#endif

__no_init char WIFI_TX_Channel;
extern WIFI_EVENT_HANDLE _wifiEventHandle;


void txDmaComplete(unsigned int channel, bool primary, void *user)
{
    send_ready = 0;
}

#ifdef __WIFI_RX_DMA__


void rxDmaComplete(unsigned int channel, bool primary, void *user)
{ 
  (void) user;
  
  HAL_DMA_ActivateBasic(WIFI_RX_Channel,HAL_WIFI_Get_RX_DataSrc(),\
      (void*)&(rxBuffer.buffer),1024,0);

  rcComplete = 1;
}
#endif

void sendUartData(void *buffer, int bytes)
{
    HAL_DMA_ActivateBasic(WIFI_TX_Channel, (void*)(buffer),\
       HAL_WIFI_Get_TX_DataSrc(), bytes,1);
}
#endif


void WiFi_Init_IO(void)
{
	HAL_WIFI_Initial_IO();
	HAL_SystemTicker_Alloc_ISR(Digital_10ms_Timeout_Int_wifi);
#ifdef __WIFI_DMA__
    #if !defined(__WIFI_RX_DMA__)
	HAL_WiFi_Regist_ISR(NULL,wifi_Recv_ISR_Func);
    #endif
#else
	HAL_WiFi_Regist_ISR(Digital_Send_FIFO_wifi, wifi_Recv_ISR_Func)
#endif

}

UCHAR DigitalGetWFChangeFinish( void )
{
	return wifiStateChangeFinish;
}


bool DigitalGetRFstatus( void )
{
	
	return wifi_settings.status.tcp_connected[1];
}

void Digital_Initial_Data_wifi( void )
{
    memset(&wifi_settings,0,sizeof(WIFI_SETTINGS));
	//by_xMs_Timer_wifi = 0;
    gWiFiPerEvent = WIFI_EVENT_IDLE;
	
	R_State_wifi = RStart_wifi;
    
	by_RX0_wifi = 0;
	_MSG_SEND.START_BYTE = 0xF1;
	_MSG_SEND.END_BYTE = 0xF2;
	send_ready = 0;
        RSCU.BinfileReadToSPIFlashOK = 0;
        RSCU.ReadFisrt = 1;
        RSCU.Read256BytesOK =0;    
        RSCU.ReadBinCurrentAddress = 0;
        RSCU.RecvBinAddress = 0;
        RSCU.BinfileDownloadToWifiModuleOK = 0;
        RSCU.BinfileDownloadToWifiModuleFail = 0;
        memset(RSCU.ReadBuffer,0,SOFTWARE_READ_SIZE);
	wifi_ResetInit();
    wifi_Poweron_Initial_Data();
#ifdef __WIFI_DMA__
    DMA_PARA WiFi_Dma_cfg;
    
    
    WIFI_TX_Channel=HAL_DMA_Malloc_Channel(WIFI_TX_Channel);
    
    
#ifdef __WIFI_RX_DMA__
    WIFI_RX_Channel=HAL_DMA_Malloc_Channel(WIFI_RX_Channel);
    WiFi_Dma_cfg.channel = WIFI_RX_Channel;    
    WiFi_Dma_cfg.DescrCfg.dstInc  = dmaDataInc1;
    WiFi_Dma_cfg.DescrCfg.srcInc  = dmaDataIncNone;
    WiFi_Dma_cfg.DescrCfg.size    = dmaDataSize1;
    WiFi_Dma_cfg.DescrCfg.arbRate = dmaArbitrate1;
    WiFi_Dma_cfg.DescrCfg.hprot   = 0;
    WiFi_Dma_cfg.size = 1024;
    WiFi_Dma_cfg.callback  = NULL;
    WiFi_Dma_cfg.dataselect = HAL_WIFI_Get_RX_Select();

    WiFi_Dma_cfg.datasrc = HAL_WIFI_Get_RX_DataSrc();
    WiFi_Dma_cfg.datadst = (void*)&(rxBuffer.buffer);
    
    HAL_DMA_Setup_Channel(WiFi_Dma_cfg,1);
#endif

    WiFi_Dma_cfg.channel = WIFI_TX_Channel;
    WiFi_Dma_cfg.DescrCfg.dstInc  = dmaDataIncNone ;
    WiFi_Dma_cfg.DescrCfg.srcInc  = dmaDataInc1;
    WiFi_Dma_cfg.DescrCfg.size    = dmaDataSize1;
    WiFi_Dma_cfg.DescrCfg.arbRate = dmaArbitrate1;
    WiFi_Dma_cfg.DescrCfg.hprot   = 0;
    WiFi_Dma_cfg.size = 1;
    WiFi_Dma_cfg.callback  = txDmaComplete;
    WiFi_Dma_cfg.dataselect = HAL_WIFI_Get_TX_Select();
    WiFi_Dma_cfg.datasrc =  NULL;//(void*)&(rxBuffer.buffer );
    WiFi_Dma_cfg.datadst = HAL_WIFI_Get_TX_DataSrc();
    
    HAL_DMA_Setup_Channel(WiFi_Dma_cfg,0);    
#endif
}


#ifdef __WIFI_RX_DMA__
void Process_Recv_Message(void )//not used
{
    static UINT16 index = 0;
    UINT16 append;
    UINT16 preIndex = index;
 MSG_RECV currentMessage;

    for(append = index;append<RX_BUFFER_SIZE;append++)
    {
       if( rxBuffer.buffer[append] == 0xF1)
        {
            index = append;
            break;
        }
    }

    if(append==RX_BUFFER_SIZE)
    {
        if(rcComplete == 1 )
            return;
        rcComplete = 1;
        HAL_DMA_ActivateBasic(WIFI_RX_Channel,HAL_WIFI_Get_RX_DataSrc(),\
            (void*)&(rxBuffer.buffer),1024,0);
       index = 0;
        return;
    }
    rcComplete = 0;
    memset(&currentMessage,0,sizeof(MSG_RECV));
    if(rxBuffer.buffer[index] == 0XF1)
    {
        UINT16 length = 0,i,j = 2;
        currentMessage.START_BYTE = 0XF1;
        for( i =index+1; ((rxBuffer.buffer[i]!=0XF2)&&(i<RX_BUFFER_SIZE-1));i++,length++,j++)
        {
            if(rxBuffer.buffer[i] == 0XF0)
            {
                *(UCHAR*)((UCHAR*)&currentMessage+j)=rxBuffer.buffer[i];
                i++;
                length++;
                *(UCHAR*)((UCHAR*)&currentMessage+j) +=rxBuffer.buffer[i];
            }
            else
            {
                *(UCHAR*)((UCHAR*)&currentMessage+j) = rxBuffer.buffer[i];
            }
        }       
        if(i == RX_BUFFER_SIZE-1 )
        {
            index = preIndex;
            return;
        }
        currentMessage.CRC.Dat.L_CHAR_VAL = *(UCHAR*)((UCHAR*)&currentMessage+length);        
        currentMessage.CRC.Dat.H_CHAR_VAL = *(UCHAR*)((UCHAR*)&currentMessage+length+1);
        currentMessage.END_BYTE = 0XF2;        
        
        memset((void*)&(rxBuffer.buffer[index]),0,length+2);
        index =index + length+2;
        Process_Message(currentMessage);//not used
    }   
}
#endif

void Digital_WIFI_Hardware_Reset(void)
{
    HAL_WIFI_Set_RST( 0 );
    HAL_Time_Delay(3000);
    HAL_WIFI_Set_RST(1);

}

UINT16 Digital_WIFI_Get_10ms_Timer(void)
{
    return by_10ms_timer;
}
extern WIFI_EVENT_HANDLE _wifiEventHandle;
WIFI_EVENT_HANDLE Digital_Command_Proce_wifi(ReturnEventHandler flag)
{
    static UINT8 signalStrength;
    WIFI_EVENT_HANDLE event= WIFI_EVENT_IDLE;
    WIFI_EVENT_HANDLE ret = WIFI_EVENT_IDLE;
    
    extern WIFI_STATUS_TYPE _wifiStatus;
	if(flag == RET_REFRESH_DISPLAY && (_wifiStatus == WIFI_STATUS_TIMEZONE || _wifiStatus == WIFI_STATUS_AVALIABLE_UPDATE))
	{
		return gWiFiPerEvent;
	}
	
	if( wifiPoweronFinish > 4 )
    {   
    	Wifi_Get_EnggineTestResult();
	     RunWiFiInterface( by_10ms_timer );
         #ifdef __WIFI_RX_DMA__
            Process_Recv_Message();
         #endif
        if((by_10ms_timer%2000)==0)   
        {        
            WIFI_get_status( );//
			//WIFISendSignalStrengthCommand();
        }   
        event = wifiGetEvent( );
         if(event == WIFI_EVENT_IDLE )
        {
             if(signalStrength != WIFISignalStrength())
            {                
                signalStrength = WIFISignalStrength();
                event = WIFI_SIGNAL_CHANGED;
                _wifiEventHandle = WIFI_SIGNAL_CHANGED;
            }
        }
     }
    #ifdef __WIFI_RESEND_0502__
        if(hadSend0502)
        {
            if(by_10ms_timer - startTime > RESEND_WIFI_0502_X10MS )
            {
                startTime = by_10ms_timer;
                AddWifiResponseState( WCS_RESEND_0502, 0, 0, 0 );
            }
        }
    #endif
    if( by_10ms_timer2 > 10000 )
    {
        Digital_WIFI_Hardware_Reset( );
    }   
   #ifdef __WIFI_NO_RTC__
    if(GetNetworkDelay()>30000)
    {
        ReadNetworkTime(GetNetworkDelay());
        gIntNetworkTimeDelay = 0;
    }
   #endif
   ret = (event==WIFI_EVENT_IDLE?wifiGetEvent( ):event);
	 
	 if( ret == WIFI_EVENT_INPUT_TIMEZONE || ret == WIFI_EVENT_AVALIABLE_UPDATE)  
 	{
 		gWiFiPerEvent = ret;
	}
	 
	 if(flag < RET_PROCESS_DISPLAY) ret = WIFI_EVENT_IDLE;
	 	
   return ret;
}
#ifdef __WIFI_NO_RTC__
UINT16 GetNetworkDelay(void)
{
    return gIntNetworkTimeDelay;
}
void SetNetworkDelay(UINT16 offset)
{
  gIntNetworkTimeDelay = offset;
}
#endif

void Digital_10ms_Timeout_Int_wifi( void )
{

	static bool ReadyFlag =0;
       static UCHAR delay10ms = 0;
       #ifdef __WIFI_NO_RTC__
        static UCHAR networkdelay = 0;
       #endif

       if(delay10ms < 9)
        {
            delay10ms++;
            return ;
        }
       delay10ms = 0;
		if(WiFiTestingFlag)
		{
			WiFiTestDelay++;
			if(WiFiTestDelay>=WIFI_TEST_DELAY_FOR_TIMEOUT)
				WiFiTestDelay = WIFI_TEST_DELAY_FOR_TIMEOUT;
		}
#ifdef __WIFI_NO_RTC__
       if(networkdelay< 99 )
        {
            networkdelay ++;            
        }
       else
        {
            networkdelay = 0;
            gIntNetworkTimeDelay +=1;
        }
#endif
	by_10ms_timer++;

	by_10ms_timer2 ++;    
	if(ReadyFlag != HAL_WIFI_Get_RDY())
	{
		ReadyFlag = HAL_WIFI_Get_RDY();
		by_10ms_timer2 = 0;
		if( wifiPoweronFinish < 5)
		{
			wifiPoweronFinish ++;
                    if( wifiPoweronFinish > 4 )
                    {
                        wifi_ResetInit();
                    }
		}
		if( wifiStateChangeFinish == 0)
		{
			wifiStateChangeFinish = 1;
		}
	}
	if(by_10ms_timer2 > WIFI_MAX_TIMER )
	{
		if( wifiPoweronFinish )
		{
			wifiPoweronFinish = 0;
			by_10ms_timer2 = 0;
			wifi_ResetInit();        
            #ifdef __WIFI_NO_RTC__
                networkdelay = 0;
                gIntNetworkTimeDelay = 0;
            #endif
			wifiStateChangeFinish = 0;
		}		
	}
#ifndef __WIFI_DMA__
	if( send_ready == 1 )
	{
		if( by_xMs_Timer_wifi >= 50 )
		{
			send_ready = 0;
			R_State_wifi = RStart_wifi;
			by_xMs_Timer_wifi = 0;
		}
		else
			++by_xMs_Timer_wifi;
	}
	else
		by_xMs_Timer_wifi = 0;
#endif

}

bool StartSendingMessage()//send message
{
	if( send_ready == 0 )
	{
		send_ready = 1;
		//by_xMs_Timer_wifi = 0;
		_MSG_SEND.START_BYTE = 0xF1;
		_MSG_SEND.END_BYTE = 0xF2;
		_MSG_SEND.CRC.INT_VAL = GenerateCRC_CCITT_F1( (UCHAR*)&_MSG_SEND.START_BYTE, _MSG_SEND.LENGTH.INT_VAL + 5 );
             #ifdef __WIFI_DMA__
                int length = _MSG_SEND.LENGTH.INT_VAL+8;
                UCHAR tempData[1024]={0};
                int i,j;
               *(UINT16*)((UCHAR*)&_MSG_SEND+_MSG_SEND.LENGTH.INT_VAL+6) = _MSG_SEND.CRC.INT_VAL;               
               *(UCHAR*)((UCHAR*)&_MSG_SEND+_MSG_SEND.LENGTH.INT_VAL+8) = 0xF2;    
                *(tempData) = 0xF1;
               for(j=1,i =1; i< length-1; i++,j++ )
                {
                    UCHAR ch = *(UCHAR*)((UCHAR*)&_MSG_SEND +i+1);
                    if((ch==0xF0)||(ch==0xF1)||(ch==0xF2))
                    {
                        *(tempData+j) = 0xF0;
                          j++;
                        *(tempData+j) =ch - 0xF0;
                    }
                    else
                    {
                        *(tempData+j) = ch;
                    }
                }
               *(tempData + j ) =0xF2;
               length = j+1;
               memset((void*)&_MSG_SEND,0, sizeof(MSG_RECV));
               memcpy((void*)&_MSG_SEND,tempData,length);
               sendUartData((void*)&_MSG_SEND,length );
             #else              
        		Local_Dstate = DStart;
        		HAL_WIFI_Enable_UARTI( 1 );
             #endif
		return true;
	}
	return false;
}


bool SendWifiCommand( UINT16 message_id, char* data, int data_size )
{
	if( send_ready > 0 )
		return false;
	_MSG_SEND.COMMAND.INT_VAL = message_id;
	
	_MSG_SEND.LENGTH.INT_VAL = data_size;

	if( data_size > 0 && data != NULL )
	{
		memset( _MSG_SEND.DATA, 0, data_size +2 );
		memcpy( _MSG_SEND.DATA, data, data_size );
	}
	return StartSendingMessage();
}

bool SendSetMachineInfo( char *string, UINT16 string_length )
{
	return SendWifiCommand( WIFI_SET_MACHINE_INFO, string, string_length );
}


bool SendGetWifiVersion()
{
	return SendWifiCommand( WIFI_GET_WIFI_VERSION, NULL, 0 );
}
bool SendGetUartVersion()
{
	return SendWifiCommand( WIFI_GET_UART_VERSION, NULL, 0 );
}

bool SendGetStatus()
{
	return SendWifiCommand( WIFI_GET_STATUS, NULL, 0 );
}
bool SendGetMacAddress()
{
	return SendWifiCommand( WIFI_GET_MAC_ADDRESS, NULL, 0 );
}
bool SendGetIP()
{
	return SendWifiCommand( WIFI_GET_IP, NULL, 0 );
}
bool SendConnectAP()
{
	return SendWifiCommand( WIFI_CONNECT_AP, NULL, 0 );
}
bool SendDisconnectAP()
{
	return SendWifiCommand( WIFI_DISCONNECT_AP, NULL, 0 );
}
bool SendSetSSID( char*string, UINT16 string_length )
{
	return SendWifiCommand( WIFI_SET_SSID, string, string_length );
}
bool SendSetSecurityType( UINT8 type )
{
	return SendWifiCommand( WIFI_SET_SECURITY_TYPE, (char*)&type, sizeof(UINT8) );
}
bool SendSetSecurityKey( char *string, UINT16 string_length )
{
	return SendWifiCommand( WIFI_SET_SECURITY_KEY, string, string_length );
}
bool SendGetSSID()
{
	return SendWifiCommand( WIFI_GET_SSID, NULL, 0 );
}
bool SendGetSecurityType()
{
	return SendWifiCommand( WIFI_GET_SECURITY_TYPE, NULL, 0 );
}
bool SendGetSecurityKey()
{
	return SendWifiCommand( WIFI_GET_SECURITY_KEY, NULL, 0 );
}
bool SendSetApInfor(char *ssid, UINT8 securityType,char* pw)
{
	if( send_ready > 0 )
		return false;
    char data[97]={0};
    memcpy(data,ssid,strlen(ssid));
    data[32] = securityType;
    memcpy((data+33),pw,strlen(pw));
    return SendWifiCommand(WIFI_SET_STA_INFOR,data,97);
}

bool SendSetStaInfor(char *ssid, UINT8 securityType,char* pw)
{
	if( send_ready > 0 )
		return false;
    char data[97]={0};
    memcpy(data,ssid,strlen(ssid));
    data[32] = securityType;
    memcpy((data+33),pw,strlen(pw));
    return SendWifiCommand(WIFI_SET_AP_INFOR,data,97);
}

bool SendGetTCPBufferSize( UINT8 index )
{
	return SendWifiCommand( WIFI_GET_TCP_BUFFER_SIZE, (char*)&index, sizeof(UINT8) );
}
bool SendSetTCPAddress( UINT8 index, char *string, UINT16 string_length )
{
	if( send_ready > 0 )
		return false;
    char data[40];
    data[0] = index;
    memcpy( (data + 1), string, string_length );
   return SendWifiCommand( WIFI_SET_TCP_ADDRESS,  data,  string_length + 1 );
}
bool SendGetTCPAddress( UINT8 index )
{
	return SendWifiCommand( WIFI_GET_TCP_ADDRESS, (char*)&index, sizeof(UINT8) );
}
bool SendSetTCPPort( UINT8 index, UINT16 port, UINT8 mode )
{
	if( send_ready > 0 )
		return false;
    char data[4] ;
    data[0] = index;    
	*(UINT16*)( data + 1 ) = port;
    data[3] = mode;
    return SendWifiCommand( WIFI_SET_TCP_PORT, data,  4 );
}
bool SendGetTCPPort( UINT8 index )
{
	return SendWifiCommand( WIFI_GET_TCP_PORT, (char*)&index, sizeof(UINT8) );
}
bool SendSetUDPPort( UINT16 port )//0X0091
{
	return SendWifiCommand( WIFI_SET_UDP_PORT, (char*)&port, sizeof(UINT16) );
}
bool SendGetUDPPort()//0X0092
{
	return SendWifiCommand( WIFI_GET_UDP_PORT, NULL, 0 );
}
bool SendTCPConnect( UINT8 index )//0x0025
{
	return SendWifiCommand( WIFI_TCP_CONNECT, (char*)&index, sizeof(UINT8) );
}
bool SendTCPDisconnect( UINT8 index )
{
	return SendWifiCommand( WIFI_TCP_DISCONNECT, (char*)&index, sizeof(UINT8) );
}

bool SendGetMachineUUID()
{
	return SendWifiCommand( WIFI_GET_MACHINE_UUID, NULL, 0 );
}

bool SendGetSignalStrength()
{
	return SendWifiCommand( WIFI_GET_SIGNAL_STRENGTH, NULL, 0 );
}

bool SendSetTCPData( UINT8 index, char *data, UINT16 data_size )//0x0027
{
    if( send_ready > 0 )
        return false;

    char adata[512];
    adata[0] = index;
    memcpy( adata + 1, data, data_size );

    return SendWifiCommand( WIFI_SET_TCP_DATA, adata, data_size + 1 );
}
bool SendGetTCPData( UINT8 index )//0x0028 not used
{
	return SendWifiCommand( WIFI_GET_TCP_DATA, (char*)&index, sizeof(UINT8) );
}

bool SendSetSSID_AP( char *string, UINT16 string_length )
{
	return SendWifiCommand( WIFI_SET_SSID_AP, string, string_length );
}
bool SendGetSSID_AP()
{
	return SendWifiCommand( WIFI_GET_SSID_AP, NULL, 0 );
}
bool SendSetSecurityType_AP( UINT8 type )
{
	return SendWifiCommand( WIFI_SET_AP_SECURE_TYPE, (char*)&type, sizeof(UINT8) );
}
bool SendSetSecurityKey_AP( char *string, UINT16 string_length )
{
	return SendWifiCommand( WIFI_SET_AP_SECURE_KEY, string, string_length );
}
bool SendGetSecurityType_AP()
{
	return SendWifiCommand( WIFI_GET_AP_SECURE_TYPE, NULL, 0 );
}
bool SendGetSecurityKey_AP()
{
	return SendWifiCommand( WIFI_GET_AP_SECURE_KEY, NULL, 0 );
}
bool SendSetBroacastData( char *string, UINT16 string_length )//0x0700
{
	return SendWifiCommand( WIFI_SET_BROADCAST_DATA, string, string_length );
}
bool SendSetApStaMode( UINT8 type )
{
	return SendWifiCommand( WIFI_SET_AP_STA_MODE, (char*)&type, sizeof(UINT8) );
}

bool SendSetWorkoutIP( char *string, UINT16 string_length )
{
	if( send_ready > 0 )
		return false;

char data[512]={0};


strncpy( data, string, string_length );
strncpy( (char*)( data + string_length ), (char*)DATA_SERVER_LOCATION, strlen( DATA_SERVER_LOCATION ) );    

return SendWifiCommand( WIFI_SET_WORKOUT_DATA_IP, data, strlen( data ) + 1 );


}

#ifdef __RSCU_UPDATE__
bool SendRemoteUpdate( UINT8 softwareType,char* url,char* md5 )
{
	if( send_ready > 0 )
		return false;
    char data[512]={0};
    data[0]  =softwareType;

    strncpy(&(data[1]),url,256);
    
    strncpy(&(data[257]),md5,32);

    return SendWifiCommand( WIFI_SET_REMOTE_UPDATE, data, 256 +32+1 );

}
bool SendDownloadProgram(void)
{
  union 
  {
    struct
    {
      U32 DataLength;
      U32 StartAddress;
      U32 EndAddress;
    }Parameter;
    char data[12];
  }DownloadFormat;
  DownloadFormat.Parameter.DataLength = SOFTWARE_SIZE;
  DownloadFormat.Parameter.StartAddress = SOFTWARE_START_ADDRESS;
  DownloadFormat.Parameter.EndAddress = SOFTWARE_END_ADDRESS;
  if( send_ready > 0 )
    return false;
  return SendWifiCommand( WIFI_DOWNLOAD_PROGRAM, DownloadFormat.data, 4+4+4 );
}
bool SendLeaveProgrammingMode(void)
{  
  if( send_ready > 0 )
    return false;
  return SendWifiCommand( WIFI_LEAVE_PROG_MODE, NULL,0);
}
bool SendGetProgramStatus( void )
{
  if( send_ready > 0 )
		return false;
        return SendWifiCommand( WIFI_GET_PROG_STATUS, NULL, 0 );
}
bool SendReadProgramData( UINT16 timer )
{
  union 
  {
    RSCU_Read_Parameter Parameter;
    char data[6];
  }ReadFormat;
  if( send_ready > 0)
    		return false; 
  if(RSCU.Read256BytesOK)
  {    
//      if(RSCU.RecvBinAddress == RSCU.ReadBinCurrentAddress)
      {
          if(RSCU.ReadBinCurrentAddress == 0x3FF00)
              RSCU.ReadBuffer[SOFTWARE_READ_SIZE - 1] = 0xFF;
          
          SPI_Flash_WriteBlock(_NewBin_BaseAddress_+RSCU.ReadBinCurrentAddress,SOFTWARE_READ_SIZE, RSCU.ReadBuffer); 
          RSCU.Read256BytesOK=0;
          RSCU.ReadBinCurrentAddress += SOFTWARE_READ_SIZE; 
      }
            
      if(RSCU.ReadBinCurrentAddress >= SOFTWARE_END_ADDRESS)
      {
          RSCU.BinfileReadToSPIFlashOK = 1;
          RSCU.ReadBinCurrentAddress = 0; 
          return false;
      }
  }
  ReadFormat.Parameter.DataLength = SOFTWARE_READ_SIZE;
  if(RSCU.ReadBinCurrentAddress == 0x3FF00)
    ReadFormat.Parameter.DataLength -= 1;
  ReadFormat.Parameter.StartAddress = RSCU.ReadBinCurrentAddress;
#ifdef WIFI_UPDATE_TEST
  HAL_Set_IO_High(ERP_LED_PORT, ERP_LED_PIN);
#endif
  return SendWifiCommand( WIFI_PROGRAM_READ, ReadFormat.data, 2+4 );
}
#else
bool SendRemoteUpdate( char* console, char* equipment, char* version, UINT16 lcbVersion, UINT16 wifiVersion, int clubId, char* serialNumber )
{
	if( send_ready > 0 )
		return false;

    MACHINE_INFO machineConfig = SystemConfig_Get_MachineInfo( );
    UCHAR data[512]={0};
	sprintf( (char*)data, "%s%s?c=%s&e=%s&usv=%s&lsv=%d&wsv=%d&cid=%d&cs=%s", machineConfig.marioBox, UPDATE_SERVER_LOCATION, console, equipment,
		version, lcbVersion, wifiVersion, clubId, serialNumber );


    return SendWifiCommand( WIFI_SET_REMOTE_UPDATE, data, strlen( data ) + 1 );

}
#endif

bool ResetWiFi()
{
	return SendWifiCommand( WIFI_RESET_WIFI, NULL, 0 );
}

bool SetHTTPData( char *string, UINT16 string_length )//0x0500
{
	return SendWifiCommand( WIFI_SET_HTTP_DATA, string, string_length );
}

//0x0502
bool SendHTTPData( HTTP_METHOD command, HTTP_DATA_TYPE dataType, char *url, UINT16 url_length, UINT8 Secure)
{
    if( (send_ready > 0 ))
		return false;

    
#ifdef __WIFI_RESEND_0502__
     startTime =  by_10ms_timer;
    hadSend0502 = true;
#endif
    char data[515];

    data[0] = Secure;
    data[1] = command;
    data[2] = dataType;
    
    if( url_length > 0 && url != NULL )
        memcpy( data + 3, url, url_length );
    return SendWifiCommand( WIFI_SEND_HTTP_DATA,  data, url_length+3 );

}

bool GetHTTPResponseData()//0x0503
{
	return SendWifiCommand( WIFI_GET_HTTP_RESPONSE, NULL, 0 );
}

bool SendByte( bool *stupidByteShift, UCHAR data )
{
	if( !*stupidByteShift && ( data == 0xF0 || data == 0xF1 || data == 0xF2 ) )
	{
		*stupidByteShift = true;
		HAL_WIFI_SendData( 0xF0 );
		return false;
	}
	else if( *stupidByteShift )
	{
		HAL_WIFI_SendData( data - 0xF0 );
		*stupidByteShift = false;
		return true;
	}
	else
	{
		*stupidByteShift = false;
		HAL_WIFI_SendData( data );
		return true;
	}
}

void Digital_Send_FIFO_wifi( void )  //发送数据到wifi
{
	static bool stupidByteShift = false;

	switch( Local_Dstate )
	{
	case DStart:
		HAL_WIFI_SendData( _MSG_SEND.START_BYTE );
		Local_Dstate = DCommdL;
		break;
	case DCommdL:
		if( SendByte( &stupidByteShift, _MSG_SEND.COMMAND.Dat.L_CHAR_VAL ) )
			Local_Dstate = DCommdH;
		break;
	case DCommdH:
		if( SendByte( &stupidByteShift, _MSG_SEND.COMMAND.Dat.H_CHAR_VAL ) )
			Local_Dstate = DLentL;
		break;
	case DLentL:
		if( SendByte( &stupidByteShift, _MSG_SEND.LENGTH.Dat.L_CHAR_VAL ) )
			Local_Dstate = DLentH;
		break;
	case DLentH:
		if( SendByte( &stupidByteShift, _MSG_SEND.LENGTH.Dat.H_CHAR_VAL ) )
		{
			if( _MSG_SEND.LENGTH.INT_VAL == 0 )
				Local_Dstate = DCRCL;
			else
				Local_Dstate = DData;
		}
		wifi_send_data_index = 0;
		break;
	case DData:
		if( SendByte( &stupidByteShift, _MSG_SEND.DATA[wifi_send_data_index] ) )
		{
			wifi_send_data_index++;
			if( wifi_send_data_index >= _MSG_SEND.LENGTH.INT_VAL )
				Local_Dstate = DCRCL;
		}
		break;
	case DCRCL:
		if( SendByte( &stupidByteShift, _MSG_SEND.CRC.Dat.L_CHAR_VAL ) )
			Local_Dstate = DCRCH;
		break;
	case DCRCH:
		if( SendByte( &stupidByteShift, _MSG_SEND.CRC.Dat.H_CHAR_VAL ) )
			Local_Dstate = DEND;
		break;
	case DEND:
		HAL_WIFI_SendData( _MSG_SEND.END_BYTE );
		Local_Dstate = DIDLE;
		break;
	case DIDLE:
		HAL_WIFI_Enable_UARTI( 0 );
              send_ready = 0;
	default:
		Local_Dstate = DStart;
            send_ready = 0;
		break;
	}

}

void strcpy_s( char* dest, char* src, int destSize, int srcSize )
{
	memset( dest, 0, destSize );
    if(srcSize <= 0)
        return;
	if( srcSize > destSize - 1 )
		strncpy( dest, src, destSize - 1 );
	else
		strncpy( dest, src, srcSize );
}

UCHAR Digital_GetError_wifi(void)
{
 	return  wifi_settings.status.error; 
}


#ifdef __WIFI_RX_DMA__
void Process_Message(MSG_RECV currentMessage)//not used
{
	currentMessage.DATA[currentMessage.LENGTH.INT_VAL] = 0;

	if( currentMessage.LENGTH.INT_VAL >= 4 )
	{
		if( ( wifi_settings.status.wifi_connected && ( currentMessage.DATA[0] & 0x01 ) == 0 )
			|| ( !wifi_settings.status.wifi_connected && ( currentMessage.DATA[0] & 0x01 ) != 0 ) )
                {
                    WifiConnectionStatusChange( ( currentMessage.DATA[0] & 0x01 ) != 0 );
                }
		wifi_settings.status.wifi_connected = ( currentMessage.DATA[0] & 0x01 ) != 0;
		wifi_settings.status.error = ( currentMessage.DATA[0] & 0x02 ) != 0;
		wifi_settings.status.unsupported_command = ( currentMessage.DATA[0] & 0x04 ) != 0;
		wifi_settings.status.resend_package = ( currentMessage.DATA[0] & 0x08 ) != 0;
		wifi_settings.status.buffer_full = ( currentMessage.DATA[0] & 0x10 ) != 0;
		wifi_settings.status.data_ready = ( currentMessage.DATA[0] & 0x20 ) != 0;
		wifi_settings.status.tcp_connection = ( currentMessage.DATA[0] & 0x40 ) != 0;
		wifi_settings.status.ucb_lcb_updating = ( currentMessage.DATA[0] & 0x80 ) != 0;
                
		wifi_settings.status.mcu_updating = ( currentMessage.DATA[1] & 0x01 ) != 0;
		wifi_settings.status.ucb_updating = ( currentMessage.DATA[1] & 0x02 ) != 0;
		wifi_settings.status.lcb_updating = ( currentMessage.DATA[1] & 0x04 ) != 0;
		wifi_settings.status.lcb2_updating = ( currentMessage.DATA[1] & 0x08 ) != 0;
		wifi_settings.status.tcp_connected[0] = ( currentMessage.DATA[1] & 0x10 ) != 0;
		wifi_settings.status.tcp_connected[1] = ( currentMessage.DATA[1] & 0x20 ) != 0;
		wifi_settings.status.tcp_connected[2] = ( currentMessage.DATA[1] & 0x40 ) != 0;
		wifi_settings.status.tcp_connected[3] = ( currentMessage.DATA[1] & 0x80 ) != 0;

		wifi_settings.status.last_error.Dat.L_CHAR_VAL = currentMessage.DATA[2];
		wifi_settings.status.last_error.Dat.H_CHAR_VAL = currentMessage.DATA[3];
	}

	switch( currentMessage.COMMAND.INT_VAL )
	{
	case WIFI_GET_WIFI_VERSION:
		wifi_settings.wifi_version.Dat.L_CHAR_VAL = currentMessage.DATA[4];
		wifi_settings.wifi_version.Dat.H_CHAR_VAL = currentMessage.DATA[5];
		break;
	case WIFI_GET_UART_VERSION:
		wifi_settings.uart_version.Dat.L_CHAR_VAL = currentMessage.DATA[4];
		wifi_settings.uart_version.Dat.H_CHAR_VAL = currentMessage.DATA[5];
		break;
	case WIFI_GET_STATUS:
		break;
	case WIFI_GET_MAC_ADDRESS:
		memcpy( wifi_settings.mac_address, currentMessage.DATA + 4, 6 );
		break;
	case WIFI_GET_IP://0x0005
		memcpy( wifi_settings.ip_address, currentMessage.DATA + 4, 6 );
		break;
	case WIFI_SET_SSID:
		break;
	case WIFI_SET_SECURITY_TYPE:
		break;
	case WIFI_SET_SECURITY_KEY:
		break;
	case WIFI_GET_SSID://0x0011
		strcpy_s( wifi_settings.security.ssid, (char*)( currentMessage.DATA + 4 ), 33, currentMessage.LENGTH.INT_VAL - 4 );
		break;
	case WIFI_GET_SECURITY_TYPE:
		wifi_settings.security.security_type = (WIFI_SECURITY_TYPE)currentMessage.DATA[4];
		break;
	case WIFI_GET_SECURITY_KEY:
		if( currentMessage.LENGTH.INT_VAL == 4 )
			memset( (char*)wifi_settings.security.security_key, 0, 255 );
		else
			strcpy_s( (char*)wifi_settings.security.security_key, (char*)( currentMessage.DATA + 4 ), 255, currentMessage.LENGTH.INT_VAL - 4 );
		break;
	case WIFI_GET_MACHINE_UUID:
		break;
	case WIFI_GET_SIGNAL_STRENGTH:
		if( currentMessage.LENGTH.INT_VAL == 5 )
			wifi_settings.signalStrength = currentMessage.DATA[4];
		break;
	case WIFI_CONNECT_AP:
		break;
	case WIFI_DISCONNECT_AP:
		break;
	case WIFI_SET_TCP_ADDRESS:
		break;
	case WIFI_GET_TCP_ADDRESS:
		if( currentMessage.DATA[4] < 4 && currentMessage.DATA[4] > 0 )
		{
			strcpy_s( (char*)wifi_settings.tcp_settings[currentMessage.DATA[4] - 1].ip_address, (char*)( currentMessage.DATA + 5 ), 20,
				currentMessage.LENGTH.INT_VAL - 5 );
		}
		break;
	case WIFI_SET_TCP_PORT:
		break;
	case WIFI_GET_TCP_PORT://0x0024
		if( currentMessage.DATA[4] < 4 && currentMessage.DATA[4] > 0 )
		{
			wifi_settings.tcp_settings[currentMessage.DATA[4] - 1].port.INT_VAL = *(UINT16*)( currentMessage.DATA + 5 );
			wifi_settings.tcp_settings[currentMessage.DATA[4] - 1].mode = (TCP_CONNECTION_MODE)( currentMessage.DATA[7] );
		}
		break;
	case WIFI_TCP_CONNECT:
		break;
	case WIFI_TCP_DISCONNECT:
		break;
	case WIFI_SET_TCP_DATA:
		break;
	case WIFI_GET_TCP_DATA://0x0028
		ProcessTCPData( currentMessage.DATA[4], (char*)( currentMessage.DATA + 5 ), currentMessage.LENGTH.INT_VAL - 5 );
		break;
	case WIFI_GET_TCP_BUFFER_SIZE:

		if( currentMessage.DATA[4] < 4 && currentMessage.DATA[4] > 0 )
		{
			wifi_settings.tcp_settings[currentMessage.DATA[4] - 1].tx_buffer_size.INT_VAL = *(UINT16*)( currentMessage.DATA + 5 );
			wifi_settings.tcp_settings[currentMessage.DATA[4] - 1].rx_buffer_size.INT_VAL = *(UINT16*)( currentMessage.DATA + 7 );
		}
		break;
	case WIFI_SET_UDP_PORT:
		break;
	case WIFI_GET_UDP_PORT:
		wifi_settings.udp_port = *(UINT16*)( currentMessage.DATA + 4 );
		break;
	case WIFI_SET_SSID_AP:
		break;
	case WIFI_SET_AP_SECURE_TYPE://0x0031
		break;
	case WIFI_SET_AP_SECURE_KEY:
		break;
	case WIFI_GET_SSID_AP:
		strcpy_s( wifi_settings.ap_security.ssid, (char*)( currentMessage.DATA + 4 ), 33, currentMessage.LENGTH.INT_VAL - 4 );

		break;
	case WIFI_GET_AP_SECURE_TYPE:
		wifi_settings.ap_security.security_type = (WIFI_SECURITY_TYPE)currentMessage.DATA[4];
		break;
	case WIFI_GET_AP_SECURE_KEY:
		strcpy_s( (char*)wifi_settings.ap_security.security_key, (char*)( currentMessage.DATA + 4 ), 255, currentMessage.LENGTH.INT_VAL - 4 );
		break;
	case WIFI_SET_BROADCAST_DATA:
		break;
	case WIFI_SET_AP_STA_MODE:
		break;
	case WIFI_SET_MACHINE_INFO:
	{
		
	}
	break;
	case WIFI_GET_HTTP_RESPONSE://0x0503
		ProcessHTTPPost( (char*)( currentMessage.DATA + 4 ), currentMessage.LENGTH.INT_VAL - 4, by_10ms_timer );
		break;
	default: // kmy3
		break;
	}

}
void Digital_Recv_Int_wifi( UCHAR by_RXBUF0 ) 
{
}
void wifi_Recv_ISR_Func(void)
{
}

#else
#ifdef __WIFI_TEST_PETER__
UINT16 setHttpDataResponse,setHttpDataResponseError, pushHttpDataResponse,pushHttpDataResponseError;
void InitTest(void)
{
	setHttpDataResponse = 0;
	pushHttpDataResponse = 0;
        pushHttpDataResponseError = 0;
        setHttpDataResponseError = 0;
}
#endif
void Process_Message()//receive message
{
	_MSG_BUFFER.DATA[_MSG_BUFFER.LENGTH.INT_VAL] = 0;

	if( _MSG_BUFFER.LENGTH.INT_VAL >= 4 )
	{
		if( ( wifi_settings.status.wifi_connected && ( _MSG_BUFFER.DATA[0] & 0x01 ) == 0 )
			|| ( !wifi_settings.status.wifi_connected && ( _MSG_BUFFER.DATA[0] & 0x01 ) != 0 ) )
                {
                    WifiConnectionStatusChange( ( _MSG_BUFFER.DATA[0] & 0x01 ) != 0 );
                }
		wifi_settings.status.wifi_connected = ( _MSG_BUFFER.DATA[0] & 0x01 ) != 0;
		wifi_settings.status.error = ( _MSG_BUFFER.DATA[0] & 0x02 ) != 0;
		wifi_settings.status.unsupported_command = ( _MSG_BUFFER.DATA[0] & 0x04 ) != 0;
		wifi_settings.status.resend_package = ( _MSG_BUFFER.DATA[0] & 0x08 ) != 0;
		wifi_settings.status.buffer_full = ( _MSG_BUFFER.DATA[0] & 0x10 ) != 0;
		wifi_settings.status.data_ready = ( _MSG_BUFFER.DATA[0] & 0x20 ) != 0;
		wifi_settings.status.tcp_connection = ( _MSG_BUFFER.DATA[0] & 0x40 ) != 0;
		wifi_settings.status.ucb_lcb_updating = ( _MSG_BUFFER.DATA[0] & 0x80 ) != 0;
                
		wifi_settings.status.mcu_updating = ( _MSG_BUFFER.DATA[1] & 0x01 ) != 0;
		wifi_settings.status.ucb_updating = ( _MSG_BUFFER.DATA[1] & 0x02 ) != 0;
		wifi_settings.status.lcb_updating = ( _MSG_BUFFER.DATA[1] & 0x04 ) != 0;
		wifi_settings.status.lcb2_updating = ( _MSG_BUFFER.DATA[1] & 0x08 ) != 0;
		wifi_settings.status.tcp_connected[0] = ( _MSG_BUFFER.DATA[1] & 0x10 ) != 0;
		wifi_settings.status.tcp_connected[1] = ( _MSG_BUFFER.DATA[1] & 0x20 ) != 0;
		wifi_settings.status.tcp_connected[2] = ( _MSG_BUFFER.DATA[1] & 0x40 ) != 0;
//        if(wifi_settings.status.tcp_connected[2] == 1)
//        {
//            wifi_settings.status.tcp_connected[2] = 0;
//        }
		wifi_settings.status.tcp_connected[3] = ( _MSG_BUFFER.DATA[1] & 0x80 ) != 0;

		wifi_settings.status.last_error.Dat.L_CHAR_VAL = _MSG_BUFFER.DATA[2];
		wifi_settings.status.last_error.Dat.H_CHAR_VAL = _MSG_BUFFER.DATA[3];
	}

	switch( _MSG_BUFFER.COMMAND.INT_VAL )
	{
	case WIFI_GET_WIFI_VERSION:
		wifi_settings.wifi_version.Dat.L_CHAR_VAL = _MSG_BUFFER.DATA[4];
		wifi_settings.wifi_version.Dat.H_CHAR_VAL = _MSG_BUFFER.DATA[5];
                {
       #ifdef OUTPUT_DEBUG
                char buf[200]={0};
                sprintf(buf,"%s:%d","start",by_10ms_timer);
    HAL_Monitor_Send_String(buf,sizeof(buf));
    #endif       
                }
		break;
	case WIFI_SET_HTTP_DATA:
	{
		#ifdef __WIFI_TEST_PETER__
			setHttpDataResponse++;
		#endif
                        if(wifi_settings.status.error)
                        {
                            setHttpDataResponseError ++;
                            setHttpDataResponse --;
                        }
	}
	break;
	case WIFI_SEND_HTTP_DATA://0x0502
	{
        #ifdef __WIFI_TEST_PETER__
                pushHttpDataResponse++;
        #endif
                if(wifi_settings.status.error)
                {
                    pushHttpDataResponseError ++;
                    pushHttpDataResponse --;
                }
        #ifdef __WIFI_RESEND_0502__
                hadSend0502 = false;                
                if(wifi_settings.status.last_error.INT_VAL !=0)
                {
                    hadSend0502 = true;
                }
        #endif               
    }
		break;
	case WIFI_GET_UART_VERSION:
		wifi_settings.uart_version.Dat.L_CHAR_VAL = _MSG_BUFFER.DATA[4];
		wifi_settings.uart_version.Dat.H_CHAR_VAL = _MSG_BUFFER.DATA[5];
		break;
	case WIFI_GET_STATUS:
		break;
	case WIFI_GET_MAC_ADDRESS:
		memcpy( wifi_settings.mac_address, _MSG_BUFFER.DATA + 4, 6 );
                
		break;
	case WIFI_GET_IP://0x0005
		memcpy( wifi_settings.ip_address, _MSG_BUFFER.DATA + 4, 6 );
                       #ifdef OUTPUT_DEBUG
                {
                char buf[200]={0};
                sprintf(buf,"%s:%d","router",by_10ms_timer);
    HAL_Monitor_Send_String(buf,sizeof(buf));
                }
    #endif   
		break;
	case WIFI_SET_SSID:
		break;
	case WIFI_SET_SECURITY_TYPE:
		break;
	case WIFI_SET_SECURITY_KEY:
		break;
	case WIFI_GET_SSID://0x0011
		strcpy_s( wifi_settings.security.ssid, (char*)( _MSG_BUFFER.DATA + 4 ), 33, _MSG_BUFFER.LENGTH.INT_VAL - 4 );
		break;
	case WIFI_GET_SECURITY_TYPE:
		wifi_settings.security.security_type = (WIFI_SECURITY_TYPE)_MSG_BUFFER.DATA[4];
		break;
	case WIFI_GET_SECURITY_KEY:
		if( _MSG_BUFFER.LENGTH.INT_VAL == 4 )
			memset( (char*)wifi_settings.security.security_key, 0, 255 );
		else
			strcpy_s( (char*)wifi_settings.security.security_key, (char*)( _MSG_BUFFER.DATA + 4 ), 255, _MSG_BUFFER.LENGTH.INT_VAL - 4 );
		break;
	case WIFI_GET_MACHINE_UUID:
		break;
	case WIFI_GET_SIGNAL_STRENGTH:
		if( _MSG_BUFFER.LENGTH.INT_VAL == 5 )
			wifi_settings.signalStrength = _MSG_BUFFER.DATA[4];
		break;
	case WIFI_CONNECT_AP:
		break;
	case WIFI_DISCONNECT_AP:
		break;
	case WIFI_SET_TCP_ADDRESS:
		break;
	case WIFI_GET_TCP_ADDRESS:
		if( _MSG_BUFFER.DATA[4] < 4 && _MSG_BUFFER.DATA[4] > 0 )
		{
			strcpy_s( (char*)wifi_settings.tcp_settings[_MSG_BUFFER.DATA[4] - 1].ip_address, (char*)( _MSG_BUFFER.DATA + 5 ), 20,
				_MSG_BUFFER.LENGTH.INT_VAL - 5 );
		}
		break;
	case WIFI_SET_TCP_PORT:
		break;
	case WIFI_GET_TCP_PORT://0x0024
		if( _MSG_BUFFER.DATA[4] < 4 && _MSG_BUFFER.DATA[4] > 0 )
		{
			wifi_settings.tcp_settings[_MSG_BUFFER.DATA[4] - 1].port.INT_VAL = *(UINT16*)( _MSG_BUFFER.DATA + 5 );
			wifi_settings.tcp_settings[_MSG_BUFFER.DATA[4] - 1].mode = (TCP_CONNECTION_MODE)( _MSG_BUFFER.DATA[7] );
		}
		break;
	case WIFI_TCP_CONNECT:
		break;
	case WIFI_TCP_DISCONNECT:
		break;
	case WIFI_SET_TCP_DATA:
		break;
	case WIFI_GET_TCP_DATA://0x0028
		ProcessTCPData( _MSG_BUFFER.DATA[4], (char*)( _MSG_BUFFER.DATA + 5 ), _MSG_BUFFER.LENGTH.INT_VAL - 5 );
		break;
	case WIFI_GET_TCP_BUFFER_SIZE:

		if( _MSG_BUFFER.DATA[4] < 4 && _MSG_BUFFER.DATA[4] > 0 )
		{
			wifi_settings.tcp_settings[_MSG_BUFFER.DATA[4] - 1].tx_buffer_size.INT_VAL = *(UINT16*)( _MSG_BUFFER.DATA + 5 );
			wifi_settings.tcp_settings[_MSG_BUFFER.DATA[4] - 1].rx_buffer_size.INT_VAL = *(UINT16*)( _MSG_BUFFER.DATA + 7 );
		}
		break;
	case WIFI_SET_UDP_PORT:
		break;
	case WIFI_GET_UDP_PORT:
		wifi_settings.udp_port = *(UINT16*)( _MSG_BUFFER.DATA + 4 );
		break;
	case WIFI_SET_SSID_AP:
		break;
	case WIFI_SET_AP_SECURE_TYPE://0x0031
		break;
	case WIFI_SET_AP_SECURE_KEY:
		break;
	case WIFI_GET_SSID_AP:
		strcpy_s( wifi_settings.ap_security.ssid, (char*)( _MSG_BUFFER.DATA + 4 ), 33, _MSG_BUFFER.LENGTH.INT_VAL - 4 );

		break;
	case WIFI_GET_AP_SECURE_TYPE:
		wifi_settings.ap_security.security_type = (WIFI_SECURITY_TYPE)_MSG_BUFFER.DATA[4];
		break;
	case WIFI_GET_AP_SECURE_KEY:
		strcpy_s( (char*)wifi_settings.ap_security.security_key, (char*)( _MSG_BUFFER.DATA + 4 ), 255, _MSG_BUFFER.LENGTH.INT_VAL - 4 );
		break;
	case WIFI_SET_BROADCAST_DATA:
		break;
	case WIFI_SET_AP_STA_MODE:
		break;
	case WIFI_SET_MACHINE_INFO:
	{
		
	}
	break;
	case WIFI_GET_HTTP_RESPONSE://0x0503
		ProcessHTTPPost( (char*)( _MSG_BUFFER.DATA + 4 ), _MSG_BUFFER.LENGTH.INT_VAL - 4, by_10ms_timer );
		break;
        case WIFI_GET_PROG_STATUS:
          if(_MSG_BUFFER.DATA[4] == 0x11 )
          {
            RSCU.BinfileDownloadToWifiModuleOK = 1;
          }
          else if(_MSG_BUFFER.DATA[4] == 0x44 )
          {
            RSCU.BinfileDownloadToWifiModuleFail = 1;
          }
          break;
        case WIFI_PROGRAM_READ:
          {          
            memcpy(&RSCU.RecvBinAddress,_MSG_BUFFER.DATA+4,4);
            if(RSCU.RecvBinAddress == RSCU.ReadBinCurrentAddress && RSCU.Read256BytesOK == 0)
            {
              memcpy(RSCU.ReadBuffer,_MSG_BUFFER.DATA+8,SOFTWARE_READ_SIZE);
              RSCU.Read256BytesOK=1;               
            }
#ifdef WIFI_UPDATE_TEST
            HAL_Set_IO_Low(ERP_LED_PORT, ERP_LED_PIN); 
#endif
            
          }
		break;
	default: // kmy3
		break;
	}

}

void wifi_Recv_ISR_Func(void)
{
    Digital_Recv_Int_wifi(HAL_WIFI_GetData());
}


void Digital_Recv_Int_wifi( UCHAR by_RXBUF0 )//receive data 
{
	static UINT16 data_index = 0;

	static bool stupidByteShift = false;

	if( by_RXBUF0 == 0xF0 )
	{
		stupidByteShift = true;
		return;
	}
	else if( stupidByteShift )
	{
		by_RXBUF0 += 0xF0;
		stupidByteShift = false;
	}

	switch( R_State_wifi )
	{
	case RStart_wifi:
		if( by_RXBUF0 != DRVE_START_wifi )
		{
			R_State_wifi = RStart_wifi;
		}
		else
		{
			by_RX0_wifi = 0;
			_MSG_BUFFER.START_BYTE = by_RXBUF0;
			R_State_wifi = RCommdL;
			by_RX0_wifi++;
		}
		break;

	case RCommdL:
		_MSG_BUFFER.COMMAND.Dat.L_CHAR_VAL = by_RXBUF0;
		by_RX0_wifi++;
		R_State_wifi = RCommdH;
		break;
	case RCommdH:
		_MSG_BUFFER.COMMAND.Dat.H_CHAR_VAL = by_RXBUF0;
		by_RX0_wifi++;
		R_State_wifi = RLentL;
		break;
	case RLentL:
		_MSG_BUFFER.LENGTH.Dat.L_CHAR_VAL = by_RXBUF0;
		by_RX0_wifi++;
		R_State_wifi = RLentH;
		break;
	case RLentH:
		_MSG_BUFFER.LENGTH.Dat.H_CHAR_VAL = by_RXBUF0;
		by_RX0_wifi++;
		R_State_wifi = RData;
		data_index = 0;
		if( _MSG_BUFFER.LENGTH.INT_VAL == 0 )
		{
			R_State_wifi = RCRCL;
		}
		if( _MSG_BUFFER.LENGTH.INT_VAL > 4096 + 5 )
		{
			R_State_wifi = RStart_wifi;
		}
		break;
	case RData:
		_MSG_BUFFER.DATA[data_index] = by_RXBUF0;
		by_RX0_wifi++;
		data_index++;
		if( data_index >= _MSG_BUFFER.LENGTH.INT_VAL )
		{
			R_State_wifi = RCRCL;
		}
		break;
	case RCRCL:
		_MSG_BUFFER.CRC.Dat.L_CHAR_VAL = by_RXBUF0;
		by_RX0_wifi++;
		R_State_wifi = RCRCH;
		break;
	case RCRCH:
		_MSG_BUFFER.CRC.Dat.H_CHAR_VAL = by_RXBUF0;
		by_RX0_wifi++;
		R_State_wifi = REND;
		break;
	case REND:
		_MSG_BUFFER.END_BYTE = by_RXBUF0;
		by_RX0_wifi++;
		//send_ready = 0; 
		R_State_wifi = RStart_wifi;
                /*****************************/
		Process_Message();
                /*****************************/
		break;
	default:
		R_State_wifi = RStart_wifi;
		break;
	}

}
#endif

UCHAR getRemoteUpdate(void)
{
  return RSCU.BinfileDownloadToWifiModuleOK;
}
UCHAR GetWifiModuleDownloadStatus(void)
{
  return RSCU.BinfileDownloadToWifiModuleFail;
}
UCHAR GetProgramReadStatus(void)
        {
  return RSCU.BinfileReadToSPIFlashOK;
        }
UINT8 Get_ProgramProgress(void)
{
	UINT8 by_Dat=0;	
	by_Dat=RSCU.ReadBinCurrentAddress/2621;
	return by_Dat;
}
UINT8 Get_ProgressChanged(void)
{
	static UINT8 by_Data=0;	
	if(Get_ProgramProgress()==by_Data)
	{
		by_Data++;
		return 1;
	}
	return 0;
}
#ifdef __WIFI_TEST_PETER__
UINT16 WIFI_Get_setHttpDataResponse(void)
{
	return setHttpDataResponse;
}
UINT16 WIFI_Get_pushHttpDataResponse(void)
{
	return pushHttpDataResponse;
}
#endif

#endif
