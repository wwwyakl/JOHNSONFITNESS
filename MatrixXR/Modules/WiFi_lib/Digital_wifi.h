#ifndef _DIGITAL_wifi
#define _DIGITAL_wifi

#include "jisdef.h"
//#include  "SPI_Flash.h"

#ifdef __WIFI_FUNC__
#include "wifi_api.h"
#define YDRIVER   0x00
#define NDRIVER   0xff

#define M_TM    0
#define M_EPBI  1

#define NO_PAR   0

#define SOFTWARE_START_ADDRESS               0x0
#define SOFTWARE_END_ADDRESS                 0x3FFFF
#define SOFTWARE_SIZE                        0x40000
#define SOFTWARE_READ_SIZE                   256
#define PROGRAM_BUFFER_SIZE                  256

#define WIFI_DELAY 1000

#define WIFI_GET_WIFI_VERSION    0x0001 
#define WIFI_GET_UART_VERSION    0x0002
#define WIFI_GET_STATUS          0x0003
#define WIFI_GET_MAC_ADDRESS     0x0004
#define WIFI_GET_IP              0x0005
#define WIFI_CONNECT_AP          0x0006
#define WIFI_DISCONNECT_AP       0x0007
#define WIFI_SET_SSID            0x0008
#define WIFI_SET_SECURITY_TYPE   0x0009
#define WIFI_SET_SECURITY_KEY    0x000A
//#define WIFI_GET_ERASE_FLASH     0x0010
#define WIFI_GET_SSID            0x0011
#define WIFI_GET_SECURITY_TYPE   0x0012
#define WIFI_GET_SECURITY_KEY    0x0013
#define WIFI_GET_SIGNAL_STRENGTH 0x0014
#define WIFI_GET_MACHINE_UUID    0x0015
#define WIFI_CLEAR_ERROR_CODE    0x0016
#define WIFI_SET_MACHINE_INFO    0x0017
#define WIFI_SET_STA_INFOR            0x018
#define WIFI_GET_TCP_BUFFER_SIZE 0x0020
#define WIFI_SET_TCP_ADDRESS     0x0021
#define WIFI_GET_TCP_ADDRESS     0x0022
#define WIFI_SET_TCP_PORT        0x0023
#define WIFI_GET_TCP_PORT        0x0024
#define WIFI_TCP_CONNECT         0x0025
#define WIFI_TCP_DISCONNECT      0x0026
#define WIFI_SET_TCP_DATA        0x0027//set_tcp_data
#define WIFI_GET_TCP_DATA        0x0028//get_tcp_data
#define WIFI_SET_SSID_AP         0x0030
#define WIFI_SET_AP_SECURE_TYPE  0x0031
#define WIFI_SET_AP_SECURE_KEY   0x0032
#define WIFI_GET_SSID_AP         0x0033
#define WIFI_GET_AP_SECURE_TYPE  0x0034
#define WIFI_GET_AP_SECURE_KEY   0x0035
#define WIFI_SET_AP_INFOR          0x0036
#define WIFI_SET_UDP_PORT        0x0091
#define WIFI_GET_UDP_PORT        0x0092
#define WIFI_DOWNLOAD_PROGRAM    0x0200
#define WIFI_LEAVE_PROG_MODE   	 0x0201
#define WIFI_GET_PROG_STATUS   	 0x0202
#define WIFI_PROGRAM_READ        0x0203

#define WIFI_SET_HTTP_DATA       0x0500
#define WIFI_SEND_HTTP_DATA   	 0x0502
#define WIFI_GET_HTTP_RESPONSE   0x0503
#define WIFI_SET_WORKOUT_DATA_IP 0x0504


//#define WIFI_SET_WORKOUT_DATA_IP 0x0504
#define WIFI_SET_REMOTE_UPDATE   0x0600
#define WIFI_SET_BROADCAST_DATA  0x0700
#define WIFI_SET_AP_STA_MODE     0x0800
#define WIFI_RESET_WIFI          0x0900

// kmy
typedef union _UINT16
{ struct
 {
   UCHAR L_CHAR_VAL;
   UCHAR H_CHAR_VAL;
 }Dat;
 UINT16 INT_VAL;
} _UINT16;

typedef union _CRC16
{ struct
 {
   UCHAR CRC_H;
   UCHAR CRC_L;
 }Dat;
UINT16 CRC_HL;
}CRC16;


typedef union _TIME16
{ struct
 {
   UCHAR TIME_H;
   UCHAR TIME_L;
 }Dat;
UINT16 TIME_HL;
}TIME16;

typedef union _DIS16
{ struct
 {
   UCHAR DIS_H;
   UCHAR DIS_L;
 }Dat;
UINT16 DIS_HL;
}DIS16;


typedef union _CAL16
{ struct
 {
   UCHAR CAL_H;
   UCHAR CAL_L;
 }Dat;
UINT16 CAL_HL;
}CAL16;


typedef struct
{
  UCHAR START_NULL;
  UCHAR START_BYTE;
  _UINT16 COMMAND;
  _UINT16 LENGTH;
  UCHAR DATA[5 + 1024];
  _UINT16 CRC;
  UCHAR END_BYTE;
} MSG_RECV;


typedef enum _TCP_CONNECTION_MODE
{
  TCP_CLIENT = 0x11,
  TCP_SERVER = 0x22,
}TCP_CONNECTION_MODE;

typedef enum _WIFI_MODE
{
  AP_MODE_ON = 0x11,
  STA_MODE_ON = 0x33,
  DUAL_MODE_ON = 0x55,
  AP_MODE_SSID_HIDDEN = 0x66,
  AP_MODE_SSID_BROADCAST = 0x77,
}WIFI_MODE;

typedef enum _WIFI_SECURITY_TYPE
{
  WIFI_SECURITY_UNSECURED = 0,
  WIFI_SECURITY_WEP40,
  WIFI_SECURITY_WEP104,
  WIFI_SECURITY_WPA_PHRASE,
  WIFI_SECURITY_WPA2_PHRASE,
  WIFI_SECURITY_WPA_AUTO_PHRASE,
  WIFI_SECURITY_AUTO_DETECT,
  NUM_WIFI_SECURITY_TYPES
}WIFI_SECURITY_TYPE;

typedef struct _WIFI_STATUS {
  bool wifi_connected;
  bool error;
  bool unsupported_command;
  bool resend_package;
  bool buffer_full;
  bool data_ready;
  bool tcp_connection;
  bool ucb_lcb_updating;
  
  bool mcu_updating;
  bool ucb_updating;
  bool lcb_updating;
  bool lcb2_updating;
  bool tcp_connected[4];
  _UINT16 last_error;
} WIFI_STATUS;

typedef struct _WIFI_SECURITY_SETTINGS {
  char ssid[33];
  WIFI_SECURITY_TYPE security_type;
  UCHAR security_key[255];
  
} WIFI_SECURITY_SETTINGS;

typedef struct _WIFI_TCP_SETTINGS {
  _UINT16 tx_buffer_size;
  _UINT16 rx_buffer_size;
  unsigned char ip_address[20];
  _UINT16 port;
  TCP_CONNECTION_MODE mode;
} WIFI_TCP_SETTINGS;

typedef struct _WIFI_SETTINGS {
  WIFI_STATUS status;
  _UINT16 wifi_version;
  _UINT16 uart_version;
  unsigned char mac_address[6];
  unsigned char ip_address[6];
  WIFI_SECURITY_SETTINGS security;
  WIFI_SECURITY_SETTINGS ap_security;
  WIFI_TCP_SETTINGS tcp_settings[3];
  //char data_server[256];
  UINT16 udp_port;
  UINT8 signalStrength;
} WIFI_SETTINGS;

typedef enum {
	HTTP_GET,
	HTTP_POST
} HTTP_METHOD;

typedef enum {
	HTTP_APPLICATION_URLENCODED,
	HTTP_APPLICATION_JSON,
	HTTP_APPLICATION_XML,
	HTTP_APPLICATION_OCTET_STREAM,
	HTTP_APPLICATION_TEXT,
} HTTP_DATA_TYPE;

#pragma pack( push, 1 )
typedef struct _HTTP_Post_Command {
	UINT8 post;
	UINT8 dataType;
	char url[256];
} HTTP_Post_Command;
typedef struct{
	UINT8 Secure;
	char Server[256];
	char Url[256];
	UINT16 Port;
}DAPI_ADDRESS_INFO;
typedef struct 
{
  UINT16 DataLength;
  UINT32 StartAddress;
}RSCU_Read_Parameter;
#pragma pack( pop )



extern WIFI_SETTINGS wifi_settings;
void WiFi_Init_IO(void);
void Digital_10ms_Timeout_Int_wifi(void);
void Digital_Recv_Int_wifi(UCHAR by_RXBUF0);
void Digital_Setkey_wifi(UCHAR by_keyin);
UCHAR Digital_Getkey_wifi(void);
void Digital_Send_FIFO_wifi(void);;

UCHAR Digital_GetError_wifi(void);


bool SendGetWifiVersion();
bool SendGetUartVersion();
bool SendGetStatus();
bool SendGetMacAddress();
bool SendGetIP();
bool SendConnectAP();
bool SendDisconnectAP();
bool SendSetSSID( char *string, UINT16 string_length );
bool SendSetSecurityType( UINT8 type );
bool SendSetSecurityKey( char *string, UINT16 string_length );
bool SendGetSSID();
bool SendGetSecurityType();
bool SendGetSecurityKey();
bool SendGetTCPBufferSize( UINT8 index );
bool SendSetTCPAddress( UINT8 index, char *string, UINT16 string_length );
bool SendGetTCPAddress( UINT8 index );
bool SendSetTCPPort( UINT8 index, UINT16 port, UINT8 mode );
bool SendGetTCPPort( UINT8 index );
bool SendTCPConnect( UINT8 index );
bool SendTCPDisconnect( UINT8 index );
bool SendSetTCPData( UINT8 index, char *data, UINT16 data_size );
bool SendGetTCPData( UINT8 index );
bool SendSetUDPPort( UINT16 port );
bool SendGetUDPPort();
bool SendSetSSID_AP( char *string, UINT16 string_length );
bool SendGetSSID_AP();
bool SendSetSecurityType_AP( UINT8 type );
bool SendSetSecurityKey_AP( char *string, UINT16 string_length );
bool SendGetSecurityType_AP();
bool SendGetSecurityKey_AP();
bool SendSetBroacastData( char *string, UINT16 string_length );
bool SendSetApStaMode( UINT8 type );
bool SetHTTPData(char *string, UINT16 string_length);
bool SendHTTPData( HTTP_METHOD command, HTTP_DATA_TYPE dataType, char *url, UINT16 url_length, UINT8 Secure);

bool GetHTTPResponseData();
bool SendSetWorkoutIP( char *string, UINT16 string_length );
bool SendSetMachineInfo( char *string, UINT16 string_length );
bool ResetWiFi();
#ifdef __RSCU_UPDATE__
bool SendRemoteUpdate( UINT8 softwareType,char* url,char* md5 );
bool SendReadProgramData( UINT16 timer );
bool SendLeaveProgrammingMode(void);
bool SendGetProgramStatus( void );
bool SendDownloadProgram(void);
void SetProgramLastAddress(UINT32 by_Dat);
#else
bool SendRemoteUpdate( char* console, char* equipment, char* version, UINT16 lcbVersion, UINT16 wifiVersion, int clubId, char* serialNumber );
#endif

UINT16 Digital_WIFI_Get_10ms_Timer(void);


bool SendGetMachineUUID();
bool SendGetSignalStrength();
UCHAR GetHttpCmdCoounter ( void );
UCHAR DigitalGetWFChangeFinish( void );
#ifdef __WIFI_NO_RTC__
UINT16 GetNetworkDelay(void);
void SetNetworkDelay(UINT16 offset);
#endif

WIFI_EVENT_HANDLE Digital_Command_Proce_wifi(ReturnEventHandler flag);

void Digital_WIFI_Hardware_Reset(void);
void wifi_Recv_ISR_Func(void);
UCHAR getRemoteUpdate(void);
UCHAR GetWifiModuleDownloadStatus(void);
bool SendSetStaInfor(char *ssid, UINT8 securityType,char* pw);
bool SendSetApInfor(char *ssid, UINT8 securityType,char* pw);
UCHAR GetProgramReadStatus(void);
UINT8 Get_ProgramProgress(void);
bool DigitalGetRFstatus( void );
#endif

#endif



