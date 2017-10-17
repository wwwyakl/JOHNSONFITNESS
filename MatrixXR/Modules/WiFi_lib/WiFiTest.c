#include "WiFiTest.h"
#ifdef __WIFI_FUNC__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "Information.h"

#define NUM_TESTS 17


#define UNSECURED_WIFI_SSID "JHTGuest"
#define UNSECURED_SECURITY_KEY ""
#define UNSECURED_SECURITY_TYPE 0

#define WEP40_WIFI_SSID "JHT_RD_WEP_40"
#define WEP40_SECURITY_KEY "12345"
//char WEP40_SECURITY_KEY[5] = { 0x12, 0x34, 0x5a, 0xbc, 0xde };
#define WEP40_SECURITY_TYPE 1

#define WEP108_WIFI_SSID "JHT_RD_WEP_108"
#define WEP108_SECURITY_KEY "123456789abcd"
#define WEP108_SECURITY_TYPE 2

#define WPA_WIFI_SSID "JHT_RD_WPA"
#define WPA_SECURITY_KEY "12345678"
#define WPA_SECURITY_TYPE 3

#define WPA2_WIFI_SSID "JHT_RD_WPA"
#define WPA2_SECURITY_KEY "12345678"
#define WPA2_SECURITY_TYPE 4


#define WPA_AUTO_WIFI_SSID "JHT_RD_WPA"
#define WPA_AUTO_SECURITY_KEY "12345678"
#define WPA_AUTO_SECURITY_TYPE WPA_SECURITY_TYPE


#define DEFAULT_SSID WPA_WIFI_SSID
#define DEFAULT_SECURITY_KEY WPA_SECURITY_KEY
#define DEFAULT_SECURITY_TYPE 5

#define TEST_SERVER_IP "www.google.com"
#define TEST_SERVER_IP_2 "www.bing.com"
#define TEST_SERVER_IP_3 "172.16.212.95"
#define TEST_SERVER_PORT 80
#define TEST_SERVER_PORT_2 80
#define TEST_SERVER_PORT_3 80
#define TEST_SERVER_SEND_DATA "GET / HTTP/1.0\r\n\r\n"
#define TEST_SERVER_SEND_DATA_2 "GET / HTTP/1.0\r\n\r\n"
#define TEST_SERVER_SEND_DATA_3 "POST /workoutData/data.php HTTP/1.0\r\nFROM: ls8.0@livestrong.com\r\nUser-Agent:HTTPTool/1.0\r\nContent-Type: application/json\r\nContent-Length: 26\r\n\r\n{\"var1\":\"value1\",\"var2\":3}"

#define TEST_CONSOLE_SERVER_PORT 30304
#define TEST_CONSOLE_SERVER_PORT_2 30305
#define TEST_CONSOLE_SERVER_PORT_3 30306
#define TEST_CONSOLE_SERVER_SEND_DATA "Hello From Interface 1"
#define TEST_CONSOLE_SERVER_SEND_DATA_2 "Hello From Interface 2"
#define TEST_CONSOLE_SERVER_SEND_DATA_3 "Hello From Interface 3"

#define REMOTE_UPDATE_STRING "172.16.212.95/update/update.php?c=LS8.0T&e=T&uhv=1&usv=1"

#define AP_TEST_SSID "JHTNA_CARDIO"
#define AP_TEST_MODE 0x77

#define WORKOUT_DATA_SERVER "172.16.212.95/workoutData/data.php"
#define WORKOUT_DATA "{\"var1\":\"value1\",\"var2\":3}"

static int test_number = 0;
static int test_state = 0;
static UINT16 test_start_time = 0;
static UINT16 test_timeout = 0;
static UINT16 fail_timer = 0;
static bool test_active[NUM_TESTS] = 
{
  false,
  true,
  false,
  false,
  false,
  false,
  false,
  false,
  false,
  false,
  false,
  false,
  false,
  false,
  false,
  false,
  false
};


static char* test_text[NUM_TESTS] = 
{
  "Set Configuration",
  "Get Configuration",
  "Connect to Unsecured AP Test",
  "Connect to WEP-40 Test",
  "Connect to WEP-104 Test",
  "Connect to WPA Test",
  "Connect to WPA2 Test",
  "Connect to WPA2 Auto Test",
  "Connect with Misconfigured Settings",
  "Reset Configuration",
  "Setup TCP as Clients",
  "Test TCP as Clients",
  "Set TCP as Servers",
  "Test TCP as Servers",
  "Test Workout Data",
  "Test Remote Update",
  "Configure AP Mode"
  
};

void WiFiTest1( UINT16 timer );
void WiFiTest2( UINT16 timer );
void WiFiTest3( UINT16 timer );
void WiFiTest4( UINT16 timer );
void WiFiTest5( UINT16 timer );
void WiFiTest6( UINT16 timer );
void WiFiTest7( UINT16 timer );
void WiFiTest8( UINT16 timer );
void WiFiTest9( UINT16 timer );
void WiFiTest10( UINT16 timer );
void WiFiTest11( UINT16 timer );
void WiFiTest12( UINT16 timer );
void WiFiTest13( UINT16 timer );
void WiFiTest14( UINT16 timer );
void WiFiTest15( UINT16 timer );
void WiFiTest16( UINT16 timer );
void WiFiTest17( UINT16 timer );


bool WiFiTest1ParseData( MSG_RECV* msg );
bool WiFiTest2ParseData( MSG_RECV* msg );
bool WiFiTest3ParseData( MSG_RECV* msg );
bool WiFiTest4ParseData( MSG_RECV* msg );
bool WiFiTest5ParseData( MSG_RECV* msg );
bool WiFiTest6ParseData( MSG_RECV* msg );
bool WiFiTest7ParseData( MSG_RECV* msg );
bool WiFiTest8ParseData( MSG_RECV* msg );
bool WiFiTest9ParseData( MSG_RECV* msg );
bool WiFiTest10ParseData( MSG_RECV* msg );
bool WiFiTest11ParseData( MSG_RECV* msg );
bool WiFiTest12ParseData( MSG_RECV* msg );
bool WiFiTest13ParseData( MSG_RECV* msg );
bool WiFiTest14ParseData( MSG_RECV* msg );
bool WiFiTest15ParseData( MSG_RECV* msg );
bool WiFiTest16ParseData( MSG_RECV* msg );
bool WiFiTest17ParseData( MSG_RECV* msg );


#ifdef __WIFI_ENG_TEST__
static unsigned char testitem = 0;
void WifiTestInitial(void)
{
    testitem = 0;
}


    
#endif



void InitTestFramework()
{
	test_number = 0;
}

bool TestingWifi()
{
	return test_number < NUM_TESTS;
}




#ifdef __WIFI_TEST_PRINTF__

void RunNextTestStep( UINT16 timer )
{
  
  if( test_number < NUM_TESTS && test_state == 0 )
  {
    test_start_time = timer;
    test_timeout = 20 * 100;
    printf( "Test %d %s:\n", test_number+1, test_text[test_number] );
    
    if( !test_active[test_number] )
    {
      test_number++;
      printf( "Disabled\n" );
      return;
    }
    else
      test_state++;
  }
  else if( test_state > 1 && timer - test_start_time > test_timeout ) 
  {
    printf( "Failed.  Timeout reached.\n" );
    test_state = 0;
    test_number++;    
  }
  

  switch( test_number )
  {
  case 0:
    WiFiTest1( timer );
    break;
  case 1:
    WiFiTest2( timer );
    break;
  case 2:
    WiFiTest3( timer );
    break;
  case 3:
    WiFiTest4( timer );
    break;
  case 4:
    WiFiTest5( timer );
    break;
  case 5:
    WiFiTest6( timer );
    break;
  case 6:
    WiFiTest7( timer );
    break;
  case 7:
    WiFiTest8( timer );
    break;
  case 8:
    WiFiTest9( timer );
    break;
  case 9:
    WiFiTest10( timer );
    break;
  case 10:
    WiFiTest11( timer );
    break;
  case 11:
    WiFiTest12( timer );
    break;
  case 12:
    WiFiTest13( timer );
    break;
  case 13:
    WiFiTest14( timer );
    break;
  case 14:
    WiFiTest15( timer );
    break;
    
  case 15:
    WiFiTest16( timer );
    break;
  case 16:
    WiFiTest17( timer );
    break;
  default:
    break;
  }

}

bool WiFiTestParseData( MSG_RECV* msg )
{
  
  if( msg->COMMAND.INT_VAL == 0x1000 )
  {
    test_number = 20;
    test_state = 0;
    return true;
  }
  
    
  switch( test_number )
  {
  case 0:
    return WiFiTest1ParseData( msg );
  case 1:
    return WiFiTest2ParseData( msg );
  case 2:
    return WiFiTest3ParseData( msg );
  case 3:
    return WiFiTest4ParseData( msg );
  case 4:
    return WiFiTest5ParseData( msg );
  case 5:
    return WiFiTest6ParseData( msg );
  case 6:
    return WiFiTest7ParseData( msg );
  case 7:
    return WiFiTest8ParseData( msg );
  case 8:
    return WiFiTest9ParseData( msg );
  case 9:
    return WiFiTest10ParseData( msg );
  case 10:
    return WiFiTest11ParseData( msg );
  case 11:
    return WiFiTest12ParseData( msg );
  case 12:
    return WiFiTest13ParseData( msg );
  case 13:
    return WiFiTest14ParseData( msg );
  case 14:
    return WiFiTest15ParseData( msg );
  case 15:
    return WiFiTest16ParseData( msg );
  case 16:
    return WiFiTest17ParseData( msg );
  /*case 17:
    return WiFiTest18ParseData( msg );
  case 18:
    return WiFiTest19ParseData( msg );
  case 19:
    return WiFiTest20ParseData( msg );
  case 20:
    return WiFiTest21ParseData( msg );
  case 21:
    return WiFiTest22ParseData( msg );*/
  default:
    return false;
  }
  //return false;
}


#define NEXT_STATE { test_start_time = timer; test_state++; }
#define NEXT_TEST  { test_state = 0; test_number++; }


void WiFiTest1( UINT16 timer )
{
  
  switch( test_state )
  {
  case 1:
    if( SendDisconnectAP() ) { NEXT_STATE }
    break;
  case 2:
    if( SendSetSSID_AP( WIFISSID, strlen( WIFISSID ) + 1 ) ) { NEXT_STATE }
    break;
  case 3:
    if( SendSetSecurityType_AP( 0 ) ) { NEXT_STATE }
    break;
  case 4:
    if( SendSetSecurityKey_AP( "", strlen( "" ) ) ) { NEXT_STATE }
    break;

  case 5:
    if( SendSetApStaMode( 0x11 ) ) { NEXT_STATE }
    break;
  case 6:
	  NEXT_STATE;
    //if( SendConnectAP() ) { NEXT_STATE }
    break;
  case 7:
    if( ResetWiFi() ) { NEXT_STATE }
    break;
  case 8:
    if( timer - test_start_time > 15 * 100  ) { NEXT_TEST }
    break;
  }
   
}


bool WiFiTest1ParseData( MSG_RECV* msg )
{
  return true;
}


void WiFiTest2( UINT16 timer )
{
  
  switch( test_state )
  {
  case 1:
    if( SendGetWifiVersion() ) { NEXT_STATE }
    break;
  case 2:
    if( SendGetUartVersion() ) { NEXT_STATE }
    break;
  case 3:
    if( SendGetMacAddress() ) { NEXT_STATE }
    break;
  case 4:
    if( SendGetIP() ) { NEXT_STATE }
    break;
  case 5:
    if( SendGetSSID() ) { NEXT_STATE }
    break;
  case 6:
    if( SendGetSecurityType() ) { NEXT_STATE }
    break;
  case 7:
    if( SendGetSecurityKey() ) { NEXT_STATE }
    break;
  case 8:
    if( SendGetSSID_AP() ) { NEXT_STATE }
    break;
  case 9:
    if( SendGetSecurityType_AP() ) { NEXT_STATE }
    break;
  case 10:
    if( SendGetSecurityKey_AP() ) { NEXT_STATE }
    break;
  case 11:
    if( SendGetUDPPort() ) { NEXT_STATE }
    break;
  case 12:
    if( SendGetTCPBufferSize( 1 ) ) { NEXT_STATE }
    break;
  case 13:
    if( SendSetTCPAddress( 1, "0.0.0.0", strlen( "0.0.0.0" ) + 1 ) ) { NEXT_STATE }
    break;
  case 14:
    if( SendGetTCPAddress( 1 ) ) { NEXT_STATE }
    break;
  case 15:
    if( SendSetTCPPort( 1, 30304, 0x11 ) ) { NEXT_STATE }
    break;
  case 16:
    if( SendGetTCPPort( 1 ) ) { NEXT_STATE }
    break;
  case 17:
    if( SendGetTCPBufferSize( 2 ) ) { NEXT_STATE }
    break;
  case 18:
    if( SendGetTCPAddress( 2 ) ) { NEXT_STATE }
    break;
  case 19:
    if( SendGetTCPPort( 2 ) ) { NEXT_STATE }
    break;
  case 20:
    if( SendGetTCPBufferSize( 3 ) ) { NEXT_STATE }
    break;
  case 21:
    if( SendGetTCPAddress( 3 ) ) { NEXT_STATE }
    break;
  case 22:
    if( SendGetTCPPort( 3 ) ) { NEXT_STATE }
    break;
  case 23:
    {
      char deviceInfo[255];
      memset( deviceInfo, 0, 255 );
      sprintf( 
        deviceInfo, 
        "LS10.0T %d.%d.%d.%d:%d 123456",
        wifi_settings.ip_address[2], 
        wifi_settings.ip_address[3], 
        wifi_settings.ip_address[4], 
        wifi_settings.ip_address[5], 
        wifi_settings.tcp_settings[0].port.INT_VAL
      );
      if( SendSetBroacastData( (CHAR*)deviceInfo, strlen( deviceInfo ) + 1 ) ) { NEXT_STATE }
      break;
    }
  case 24:
    if( SendTCPConnect( 1 ) ) { NEXT_STATE }
    break;
  case 25:
    if( SendTCPConnect( 2 ) ) { NEXT_STATE }
    break;
  case 26:
    if( SendTCPConnect( 3 ) ) { NEXT_TEST }
    break;
  }
   
}


bool WiFiTest2ParseData( MSG_RECV* msg )
{
  return true;
}



void TestConnectToAP( CHAR* ssid, int ssidLength, CHAR* securityKey, int keyLength, int securityType, UINT16 timer )
{
  
  if( test_state == 1 && SendSetSSID( ssid, ssidLength ) )
    NEXT_STATE
  else if( test_state == 2 && SendSetSecurityKey( securityKey, keyLength ) )
    NEXT_STATE
  else if( test_state == 3 && SendSetSecurityType( securityType ) )
    NEXT_STATE
  else if( test_state == 4 && SendConnectAP() )
    NEXT_STATE
  else if( test_state == 5 && ResetWiFi() )
    NEXT_STATE
  else if( test_state == 6 && timer - test_start_time > 15 * 100 )
    NEXT_STATE
  else if( test_state == 7 )
    SendGetStatus();
  else if( test_state == 8 && SendGetIP() )
    NEXT_STATE
    
  
}

void WiFiConnectionTestParseData( MSG_RECV* msg )
{
  if( msg->COMMAND.INT_VAL == WIFI_GET_STATUS && test_state == 7 )
  {
    if( wifi_settings.status.wifi_connected )
      test_state++;
  }
  else if( msg->COMMAND.INT_VAL == WIFI_GET_IP && test_state == 9 )
  {
    for( int i = 0; i < 6; i++ )
      if( wifi_settings.ip_address[i] != 0 )
      {
        NEXT_TEST; 
        printf( "Passed\n" );
        return;
      }
    printf( "Failed.  IP address is 0.0.0.0\n" );
    NEXT_TEST;
  }
}


void WiFiTest3( UINT16 timer )
{
  TestConnectToAP( 
    UNSECURED_WIFI_SSID, 
    strlen( UNSECURED_WIFI_SSID ), 
    UNSECURED_SECURITY_KEY, 
    strlen( UNSECURED_SECURITY_KEY ), 
    UNSECURED_SECURITY_TYPE,
    timer 
  );
}


bool WiFiTest3ParseData( MSG_RECV* msg )
{
  WiFiConnectionTestParseData( msg );
  return true;
}

void WiFiTest4( UINT16 timer )
{
  TestConnectToAP( 
    WEP40_WIFI_SSID, 
    strlen( WEP40_WIFI_SSID ), 
    WEP40_SECURITY_KEY, 
    5, 
    WEP40_SECURITY_TYPE,
    timer 
  );
}


bool WiFiTest4ParseData( MSG_RECV* msg )
{
  
  WiFiConnectionTestParseData( msg );
  return true;
}

void WiFiTest5( UINT16 timer )
{
  TestConnectToAP( 
    WEP108_WIFI_SSID, 
    strlen( WEP108_WIFI_SSID ), 
    WEP108_SECURITY_KEY, 
    strlen( WEP108_SECURITY_KEY ), 
    WEP108_SECURITY_TYPE,
    timer 
  );
 
}


bool WiFiTest5ParseData( MSG_RECV* msg )
{
  
  WiFiConnectionTestParseData( msg );
  return true;
}

void WiFiTest6( UINT16 timer )
{
  
  TestConnectToAP( 
    WPA_WIFI_SSID, 
    strlen( WPA_WIFI_SSID ), 
    WPA_SECURITY_KEY, 
    strlen( WPA_SECURITY_KEY ), 
    WPA_SECURITY_TYPE,
    timer
  );
  
}


bool WiFiTest6ParseData( MSG_RECV* msg )
{
  WiFiConnectionTestParseData( msg );
  return true;
}

void WiFiTest7( UINT16 timer )
{
  
  TestConnectToAP( 
    WPA2_WIFI_SSID, 
    strlen( WPA2_WIFI_SSID ), 
    WPA2_SECURITY_KEY, 
    strlen( WPA2_SECURITY_KEY ), 
    WPA2_SECURITY_TYPE,
    timer 
  );
  
}


bool WiFiTest7ParseData( MSG_RECV* msg )
{
  WiFiConnectionTestParseData( msg );
  return true;
}

void WiFiTest8( UINT16 timer )
{ 
  TestConnectToAP( 
    WPA_AUTO_WIFI_SSID, 
    strlen( WPA_AUTO_WIFI_SSID ), 
    WPA_AUTO_SECURITY_KEY, 
    strlen( WPA_AUTO_SECURITY_KEY ), 
    WPA_AUTO_SECURITY_TYPE,
    timer 
  );
}


bool WiFiTest8ParseData( MSG_RECV* msg )
{
  return true;
}

void WiFiTest9( UINT16 timer )
{
  if( test_state == 6 )
    fail_timer = test_start_time = timer;
  TestConnectToAP( "Office", strlen( "Office" ), "JHTNAWireless2", strlen( "JHTNAWireless2" ), 5, timer );
  
  if( test_state == 7 && timer - fail_timer > 6 * 100 )
  {
    printf( "Passed.\n" );
    test_state = 0;
    test_number++;
  }  
  
  
}


bool WiFiTest9ParseData( MSG_RECV* msg )
{
  
  if( msg->COMMAND.INT_VAL == WIFI_GET_STATUS && test_state == 6 )
  {
    UINT16 status = *(int*)(msg->DATA + 4);
    if( status & 0x0001 != 0 )
    {
      printf( "Failed.  Connected with misconfigured settings.\n" );
      test_state = 0;
      test_number++;
    }
  }
  
   
  return true;
}


void WiFiTest10( UINT16 timer )
{
  
  switch( test_state )
  {
  case 1:
    if( SendSetSSID( DEFAULT_SSID, strlen( DEFAULT_SSID ) ) ) { NEXT_STATE }
    break;
  case 2:
    if( SendSetSecurityType( DEFAULT_SECURITY_TYPE ) ) { NEXT_STATE }
    break;
  case 3:
    if( SendSetSecurityKey( DEFAULT_SECURITY_KEY, strlen( DEFAULT_SECURITY_KEY ) ) ) { NEXT_STATE }
    break;
  case 4:
    if( SendConnectAP() ) { NEXT_STATE }
    break;
  case 5:
    if( SendSetApStaMode( 0x55 ) ) { NEXT_STATE }
    break;
  case 6:
    if( ResetWiFi() ) { NEXT_STATE }
    break;
  case 7:
    if( timer - test_start_time > 15 * 100  ) { NEXT_STATE }
    break;
  case 8:
    if( SendGetStatus() ) { NEXT_STATE }
    break;
  case 9:
    if( SendGetIP() ) { NEXT_STATE }
    break;
  }
  
}


bool WiFiTest10ParseData( MSG_RECV* msg )
{
  if( test_state == 10 )
  {
    NEXT_TEST
  }
  return true;
}


void WiFiTest11( UINT16 timer )
{
  if( test_state == 1 && SendTCPDisconnect( 1 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
      
  if( test_state == 3 && SendTCPDisconnect( 2 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
     
  if( test_state == 5 && SendTCPDisconnect( 3 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
  if( test_state == 7 && SendSetTCPAddress( 1, TEST_SERVER_IP, strlen( TEST_SERVER_IP ) + 1 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
  
  if( test_state == 9 && SendSetTCPAddress( 2, TEST_SERVER_IP_2, strlen( TEST_SERVER_IP_2 ) + 1 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
      
  if( test_state == 11 && SendSetTCPAddress( 3, TEST_SERVER_IP_3, strlen( TEST_SERVER_IP_3 ) + 1 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
  
  if( test_state == 13 && SendGetTCPAddress( 1 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
  
  if( test_state == 15 && SendGetTCPAddress( 2 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
  if( test_state == 17 && SendGetTCPAddress( 3 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
   
      
   
  if( test_state == 19 && SendSetTCPPort( 1, TEST_SERVER_PORT, 0x22 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
      
  
  if( test_state == 21 && SendSetTCPPort( 2, TEST_SERVER_PORT_2, 0x22 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
  
  if( test_state == 23 && SendSetTCPPort( 3, TEST_SERVER_PORT_3, 0x22 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
  if( test_state == 25 && SendGetTCPPort( 1 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }    
  
  if( test_state == 27 && SendGetTCPPort( 2 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }    
  if( test_state == 29 && SendGetTCPPort( 3 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }        
      
   
}


bool WiFiTest11ParseData( MSG_RECV* msg )
{
  if( msg->COMMAND.INT_VAL == WIFI_TCP_DISCONNECT && test_state == 2 )
  {
    test_state++;
    return true;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_TCP_DISCONNECT && test_state == 4 )
  {
    test_state++;
    return true;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_TCP_DISCONNECT && test_state == 6 )
  {
    test_state++;
    return true;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_SET_TCP_ADDRESS && test_state == 8 )
  {
    test_state++;
    return true;
  }
  
  
  else if( msg->COMMAND.INT_VAL == WIFI_SET_TCP_ADDRESS && test_state == 10 )
  {
    test_state++;
    return true;
  }
  
  
  else if( msg->COMMAND.INT_VAL == WIFI_SET_TCP_ADDRESS && test_state == 12 )
  {
    test_state++;
    return true;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_ADDRESS && test_state == 14 )
  {
    if( strncmp( (char*)wifi_settings.tcp_settings[0].ip_address, TEST_SERVER_IP, strlen( TEST_SERVER_IP ) ) != 0 )
    {
        printf( "Failed.  Get IP address %s does not match Set IP address of %s.\n", wifi_settings.tcp_settings[0].ip_address, TEST_SERVER_IP );
        NEXT_TEST
        return false;
    }
    else
      test_state++;
  }
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_ADDRESS && test_state == 16 )
  {
    if( strncmp( (char*)wifi_settings.tcp_settings[1].ip_address, TEST_SERVER_IP_2, strlen( TEST_SERVER_IP_2 ) ) != 0 )
    {
        printf( "Failed.  Get IP address %s does not match Set IP address of %s.\n", wifi_settings.tcp_settings[1].ip_address, TEST_SERVER_IP_2 );
        NEXT_TEST
        return false;
    }
    else
      test_state++;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_ADDRESS && test_state == 18 )
  {
    if( strncmp( (char*)wifi_settings.tcp_settings[2].ip_address, TEST_SERVER_IP_3, strlen( TEST_SERVER_IP_3 ) ) != 0 )
    {
        printf( "Failed.  Get IP address %s does not match Set IP address of %s.\n", wifi_settings.tcp_settings[2].ip_address, TEST_SERVER_IP_3 );
        NEXT_TEST
        return false;
    }
    else
      test_state++;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_SET_TCP_PORT && test_state == 20 )
  {
    test_state++;
    return true;
  }
  
  
  else if( msg->COMMAND.INT_VAL == WIFI_SET_TCP_PORT && test_state == 22 )
  {
    test_state++;
    return true;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_SET_TCP_PORT && test_state == 24 )
  {
    test_state++;
    return true;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_PORT && test_state == 26 )
  {
    if( wifi_settings.tcp_settings[0].port.INT_VAL != TEST_SERVER_PORT || wifi_settings.tcp_settings[0].mode != 0x22 )
    {
        printf( "Failed.  Get TCP port 1 does not match Set Port.\n" );
        test_number++;
        test_state = 0;
        return false;
    }
    else
      test_state++;
  }
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_PORT && test_state == 28 )
  {
    if( wifi_settings.tcp_settings[1].port.INT_VAL != TEST_SERVER_PORT_2 || wifi_settings.tcp_settings[1].mode != 0x22 )
    {
        printf( "Failed.  Get TCP port 1 does not match Set Port.\n" );
        NEXT_TEST;
        return false;
    }
    else
      test_state++;
  }
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_PORT && test_state == 30 )
  {
    if( wifi_settings.tcp_settings[2].port.INT_VAL != TEST_SERVER_PORT_3 || wifi_settings.tcp_settings[1].mode != 0x22 )
    {
        printf( "Failed.  Get TCP port 1 does not match Set Port.\n" );
    }
    else
    {
      printf( "Passed.\n" );
    }
    NEXT_TEST
  }
  return true;
}
      
      

void WiFiTest12( UINT16 timer )
{
  
  switch( test_state )
  {
  case 1:
    if( SendTCPConnect( 1 ) ) { NEXT_STATE }
    break;
  case 2:
    if( timer - test_start_time > 1 * 100  ) { NEXT_STATE }
    break;
  case 3:
    SendGetStatus();
    break;
  case 4:
    if( SendSetTCPData( 1, TEST_SERVER_SEND_DATA, strlen( TEST_SERVER_SEND_DATA ) ) )
    {
      NEXT_STATE
    }
    break;
  case 5:
  case 6:
    SendGetTCPData( 1 );
    break;
  case 7:
    if( SendTCPDisconnect( 1 ) ) { NEXT_STATE }
    break;
  case 8:
    if( SendTCPConnect( 2 ) ) { NEXT_STATE }
    break;
  case 9:
    if( timer - test_start_time > 1 * 100  ) { NEXT_STATE }
    break;
  case 10:
    SendGetStatus();
    break;
  case 11:
    if( SendSetTCPData( 2, TEST_SERVER_SEND_DATA_2, strlen( TEST_SERVER_SEND_DATA_2 ) ) )
    {
      NEXT_STATE
    }
    break;
  case 12:
  case 13:
    SendGetTCPData( 2 );
    break;
  case 14:
    if( SendTCPDisconnect( 2 ) ) { NEXT_STATE }
    break;
  case 15:
    if( SendTCPConnect( 3 ) ) { NEXT_STATE }
    break;
  case 16:
    if( timer - test_start_time > 1 * 100  ) { NEXT_STATE }
    break;
  case 17:
    SendGetStatus();
    break;
  case 18:
    if( SendSetTCPData( 3, TEST_SERVER_SEND_DATA_3, strlen( TEST_SERVER_SEND_DATA_3 ) ) )
    {
      NEXT_STATE
    }
    break;
  case 19:
  case 20:
    SendGetTCPData( 3 );
    break;
  case 21:
    if( SendTCPDisconnect( 3 ) ) { printf( "Passed.\n" ); NEXT_STATE }
    break;
  }    
   
}


bool WiFiTest12ParseData( MSG_RECV* msg )
{
  //3, 9, 15
  // 5, 11, 17
  if( test_state == 22 )
    NEXT_TEST
  else if( msg->COMMAND.INT_VAL == WIFI_GET_STATUS && test_state == 3 )
  {
    if( wifi_settings.status.tcp_connected[1] )
      test_state++;
  }
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_DATA && test_state == 5 )
  {
    if( msg->DATA[4] == 1 && msg->LENGTH.INT_VAL > 5 )
    {
      test_state++;
    }
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_DATA && test_state == 6 )
  {
    if( msg->DATA[4] == 1 && msg->LENGTH.INT_VAL <= 5 )
    {
      test_state++;
    }
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_GET_STATUS && test_state == 10 )
  {
    if( wifi_settings.status.tcp_connected[2] )
      test_state++;
  }
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_DATA && test_state == 12 )
  {
    if( msg->DATA[4] == 2 && msg->LENGTH.INT_VAL > 5 )
    {
      test_state++;
    }
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_DATA && test_state == 13 )
  {
    if( msg->DATA[4] == 2 && msg->LENGTH.INT_VAL <= 5 )
    {
      test_state++;
    }
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_GET_STATUS && test_state == 17 )
  {
    if( wifi_settings.status.tcp_connected[3] )
      test_state++;
  }
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_DATA && test_state == 19 )
  {
    if( msg->DATA[4] == 3 && msg->LENGTH.INT_VAL > 5 )
    {
      test_state++;
    }
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_DATA && test_state == 20 )
  {
    if( msg->DATA[4] == 3 && msg->LENGTH.INT_VAL <= 5 )
    {
      test_state++;
    }
  }
  
  return true;
}


void WiFiTest13( UINT16 timer )
{
  if( test_state == 1 && SendTCPDisconnect( 1 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
      
  if( test_state == 3 && SendTCPDisconnect( 2 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
     
  if( test_state == 5 && SendTCPDisconnect( 3 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
  if( test_state == 7 && SendSetTCPAddress( 1, "0.0.0.0", strlen( "0.0.0.0" ) + 1 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
  
  if( test_state == 9 && SendSetTCPAddress( 2, "0.0.0.0", strlen( "0.0.0.0" ) + 1 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
      
  if( test_state == 11 && SendSetTCPAddress( 3, "0.0.0.0", strlen( "0.0.0.0" ) + 1 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
  
  if( test_state == 13 && SendGetTCPAddress( 1 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
  
  if( test_state == 15 && SendGetTCPAddress( 2 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
  if( test_state == 17 && SendGetTCPAddress( 3 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
   
      
   
  if( test_state == 19 && SendSetTCPPort( 1, TEST_CONSOLE_SERVER_PORT, 0x11 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
      
  
  if( test_state == 21 && SendSetTCPPort( 2, TEST_CONSOLE_SERVER_PORT_2, 0x11 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
  
  if( test_state == 23 && SendSetTCPPort( 3, TEST_CONSOLE_SERVER_PORT_3, 0x11 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
  if( test_state == 25 && SendGetTCPPort( 1 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }    
  
  if( test_state == 27 && SendGetTCPPort( 2 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }    
  if( test_state == 29 && SendGetTCPPort( 3 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }        
      
   
}


bool WiFiTest13ParseData( MSG_RECV* msg )
{
  if( msg->COMMAND.INT_VAL == WIFI_TCP_DISCONNECT && test_state == 2 )
  {
    test_state++;
    return true;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_TCP_DISCONNECT && test_state == 4 )
  {
    test_state++;
    return true;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_TCP_DISCONNECT && test_state == 6 )
  {
    test_state++;
    return true;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_SET_TCP_ADDRESS && test_state == 8 )
  {
    test_state++;
    return true;
  }
  
  
  else if( msg->COMMAND.INT_VAL == WIFI_SET_TCP_ADDRESS && test_state == 10 )
  {
    test_state++;
    return true;
  }
  
  
  else if( msg->COMMAND.INT_VAL == WIFI_SET_TCP_ADDRESS && test_state == 12 )
  {
    test_state++;
    return true;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_ADDRESS && test_state == 14 )
  {
    if( strncmp( (char*)wifi_settings.tcp_settings[0].ip_address, "0.0.0.0", strlen( "0.0.0.0" ) ) != 0 )
    {
        printf( "Failed.  Get IP address %s does not match Set IP address of %s.\n", wifi_settings.tcp_settings[0].ip_address, TEST_SERVER_IP );
        NEXT_TEST
        return false;
    }
    else
      test_state++;
  }
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_ADDRESS && test_state == 16 )
  {
    if( strncmp( (char*)wifi_settings.tcp_settings[1].ip_address, "0.0.0.0", strlen( "0.0.0.0" ) ) != 0 )
    {
        printf( "Failed.  Get IP address %s does not match Set IP address of %s.\n", wifi_settings.tcp_settings[1].ip_address, TEST_SERVER_IP_2 );
        NEXT_TEST
        return false;
    }
    else
      test_state++;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_ADDRESS && test_state == 18 )
  {
    if( strncmp( (char*)wifi_settings.tcp_settings[2].ip_address, "0.0.0.0", strlen( "0.0.0.0" ) ) != 0 )
    {
        printf( "Failed.  Get IP address %s does not match Set IP address of %s.\n", wifi_settings.tcp_settings[2].ip_address, TEST_SERVER_IP_3 );
        NEXT_TEST
        return false;
    }
    else
      test_state++;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_SET_TCP_PORT && test_state == 20 )
  {
    test_state++;
    return true;
  }
  
  
  else if( msg->COMMAND.INT_VAL == WIFI_SET_TCP_PORT && test_state == 22 )
  {
    test_state++;
    return true;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_SET_TCP_PORT && test_state == 24 )
  {
    test_state++;
    return true;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_PORT && test_state == 26 )
  {
    if( wifi_settings.tcp_settings[0].port.INT_VAL != TEST_CONSOLE_SERVER_PORT || wifi_settings.tcp_settings[0].mode != 0x11 )
    {
        printf( "Failed.  Get TCP port 1 does not match Set Port.\n" );
        test_number++;
        test_state = 0;
        return false;
    }
    else
      test_state++;
  }
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_PORT && test_state == 28 )
  {
    if( wifi_settings.tcp_settings[1].port.INT_VAL != TEST_CONSOLE_SERVER_PORT_2 || wifi_settings.tcp_settings[1].mode != 0x11 )
    {
        printf( "Failed.  Get TCP port 1 does not match Set Port.\n" );
        NEXT_TEST;
        return false;
    }
    else
      test_state++;
  }
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_PORT && test_state == 30 )
  {
    if( wifi_settings.tcp_settings[2].port.INT_VAL != TEST_CONSOLE_SERVER_PORT_3 || wifi_settings.tcp_settings[1].mode != 0x11 )
    {
        printf( "Failed.  Get TCP port 1 does not match Set Port.\n" );
    }
    else
    {
      printf( "Passed.\n" );
    }
    NEXT_TEST
  }
  return true;
}

void WiFiTest14( UINT16 timer )
{
  
  switch( test_state )
  {
  case 1:
    if( SendTCPConnect( 1 ) ) { NEXT_STATE }
    break;
  case 2:
    if( timer - test_start_time > 1 * 100  ) 
    {
      printf(
             "Connect to %d.%d.%d.%d port %d\n", 
             wifi_settings.ip_address[2],
             wifi_settings.ip_address[3],
             wifi_settings.ip_address[4],
             wifi_settings.ip_address[5],
             TEST_CONSOLE_SERVER_PORT
      );
      NEXT_STATE 
    }
    break;
  case 3:
    SendGetStatus();
    if( timer - test_start_time > 10 * 100  ) 
    {
      printf( "No connection made on TCP Interface 1\n" );
      test_state = 6;
    }
    break;
  case 4:
    if( SendSetTCPData( 1, TEST_CONSOLE_SERVER_SEND_DATA, strlen( TEST_CONSOLE_SERVER_SEND_DATA ) ) )
    {
      NEXT_STATE
    }
    break;
  case 5:
    SendGetTCPData( 1 );
    break;
  case 6:
    if( SendTCPDisconnect( 1 ) ) { NEXT_STATE }
    break;
  case 7:
    if( SendTCPConnect( 2 ) ) { NEXT_STATE }
    break;
  case 8:
    if( timer - test_start_time > 1 * 100  ) 
    { 
      printf(
             "Connect to %d.%d.%d.%d port %d\n", 
             wifi_settings.ip_address[2],
             wifi_settings.ip_address[3],
             wifi_settings.ip_address[4],
             wifi_settings.ip_address[5],
             TEST_CONSOLE_SERVER_PORT_2
      );
      NEXT_STATE 
    }
    break;
  case 9:
    SendGetStatus();
    
    if( timer - test_start_time > 10 * 100  ) 
    {
      printf( "No connection made on TCP Interface 2\n" );
      test_state = 13;
    }
    break;
    
  case 10:
    if( SendSetTCPData( 2, TEST_CONSOLE_SERVER_SEND_DATA_2, strlen( TEST_CONSOLE_SERVER_SEND_DATA_2 ) ) )
    {
      NEXT_STATE
    }
    break;
  case 11:
    SendGetTCPData( 2 );
    break;
  case 12:
    if( SendTCPDisconnect( 2 ) ) { NEXT_STATE }
    break;
  case 13:
    if( SendTCPConnect( 3 ) ) { NEXT_STATE }
    break;
  case 14:
    if( timer - test_start_time > 1 * 100  ) 
    { 
      printf(
             "Connect to %d.%d.%d.%d port %d\n", 
             wifi_settings.ip_address[2],
             wifi_settings.ip_address[3],
             wifi_settings.ip_address[4],
             wifi_settings.ip_address[5],
             TEST_CONSOLE_SERVER_PORT_3
      );
      NEXT_STATE 
    }
    break;
  case 15:
    SendGetStatus();
    
    if( timer - test_start_time > 10 * 100  ) 
    {
      printf( "No connection made on TCP Interface 2\n" );
      test_state = 18;
    }
    break;
  case 16:
    if( SendSetTCPData( 3, TEST_CONSOLE_SERVER_SEND_DATA_3, strlen( TEST_SERVER_SEND_DATA_3 ) ) )
    {
      NEXT_STATE
    }
    break;
  case 17:
    SendGetTCPData( 3 );
    break;
  case 18:
    if( SendTCPDisconnect( 3 ) ) { printf( "Passed.\n" ); NEXT_STATE }
    break;
  }    
   
}


bool WiFiTest14ParseData( MSG_RECV* msg )
{
  //3, 9, 15
  // 5, 11, 17
  if( test_state == 19 )
    NEXT_TEST
  else if( msg->COMMAND.INT_VAL == WIFI_GET_STATUS && test_state == 3 )
  {
    if( wifi_settings.status.tcp_connected[1] )
      test_state++;
  }
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_DATA && test_state == 5 )
  {
    if( msg->DATA[4] == 1 && msg->LENGTH.INT_VAL > 5 )
    {
      printf( "Received data: %s\n", msg->DATA + 5 );
      test_state++;
    }
  }
  
  
  else if( msg->COMMAND.INT_VAL == WIFI_GET_STATUS && test_state == 9 )
  {
    if( wifi_settings.status.tcp_connected[2] )
      test_state++;
  }
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_DATA && test_state == 11 )
  {
    if( msg->DATA[4] == 2 && msg->LENGTH.INT_VAL > 5 )
    {
      printf( "Received data: %s\n", msg->DATA + 5 );
      test_state++;
    }
  }
  
  
  else if( msg->COMMAND.INT_VAL == WIFI_GET_STATUS && test_state == 15 )
  {
    if( wifi_settings.status.tcp_connected[3] )
      test_state++;
  }
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_DATA && test_state == 17 )
  {
    if( msg->DATA[4] == 3 && msg->LENGTH.INT_VAL > 5 )
    {
      printf( "Received data: %s\n", msg->DATA + 5 );
      test_state++;
    }
  }
  
  
  return true;
}

void WiFiTest15( UINT16 timer )
{
  if( test_state == 1 && SendSetWorkoutIP( WORKOUT_DATA_SERVER, strlen( WORKOUT_DATA_SERVER ) + 1 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
      
  if( test_state == 3 && SetHTTPData( WORKOUT_DATA, strlen( WORKOUT_DATA ) + 1 ) )
  {
    test_start_time = timer;
    test_state++;
  }
  
  if( test_state == 5 )// && SendHTTPData( ) )
  {
    test_start_time = timer;
    test_state++;
  }
  
   
}


bool WiFiTest15ParseData( MSG_RECV* msg )
{
  if( msg->COMMAND.INT_VAL == WIFI_SET_WORKOUT_DATA_IP && test_state == 2 )
  {
    test_state++;
    return true;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_SET_HTTP_DATA && test_state == 4 )
  {
    test_state++;
    return false;
  }
  else if( msg->COMMAND.INT_VAL == WIFI_SEND_HTTP_DATA && test_state == 6 )
  {
    test_state = 0;
    test_number++;
    return false;
  }
  return true;
}


void WiFiTest16( UINT16 timer )
{
//  if( test_state == 1 && SendRemoteUpdate( REMOTE_UPDATE_STRING, strlen( REMOTE_UPDATE_STRING ) + 1 ) )
//  {
    NEXT_STATE
//  }
      
}


bool WiFiTest16ParseData( MSG_RECV* msg )
{
  if( test_state == 2 )
    NEXT_TEST
  return true;
}
      


void WiFiTest17( UINT16 timer )
{
  if( test_state == 1 && SendSetSSID_AP( AP_TEST_SSID, strlen( AP_TEST_SSID ) + 1 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
      
  else if( test_state == 3 && SendGetSSID_AP() )
  {
    test_start_time = timer;
    test_state++;
  }
                                         
                                            
  else if( test_state == 5 && SendSetSecurityType_AP( 0 ) )
  {
    test_start_time = timer;
    test_state++;
  }
                                         
                                            
  else if( test_state == 7 && SendGetSecurityType_AP() )
  {
    test_start_time = timer;
    test_state++;
  }
                                         
                                         
  else if( test_state == 9 && SendSetSecurityKey_AP( "", 0 ) )
  {
    test_start_time = timer;
    test_state++;
  }
                                         
                                            
  else if( test_state == 11 && SendGetSecurityKey_AP() )
  {
    test_start_time = timer;
    test_state++;
  }
      
  else if( test_state == 13 && SendSetApStaMode( AP_TEST_MODE ) )
  {
    test_start_time = timer;
    test_state++;
  }
  
  else if( test_state == 15 && ResetWiFi() )
  {
    test_start_time = timer;
    test_state++;
  }
  
   
}


bool WiFiTest17ParseData( MSG_RECV* msg )
{
  if( msg->COMMAND.INT_VAL == WIFI_SET_SSID_AP && test_state == 2 )
  {
    test_state++;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_GET_SSID_AP && test_state == 4 )
  {
    if( strncmp( (char*)(msg->DATA + 4), AP_TEST_SSID, strlen( AP_TEST_SSID ) ) != 0 )
      printf( "Error.  AP SSID does not match what was set\n" );
    test_state++;
  }
  else if( msg->COMMAND.INT_VAL == WIFI_SET_AP_SECURE_TYPE && test_state == 6 )
  {
    test_state++;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_GET_AP_SECURE_TYPE && test_state == 8 )
  {
    if( msg->DATA[4] != 0 )
      printf( "Error.  Expected security type 0, but got %d\n", msg->DATA[4] );
    test_state++;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_SET_AP_SECURE_KEY && test_state == 10 )
  {
    test_state++;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_GET_AP_SECURE_KEY && test_state == 12 )
  {
    if( msg->LENGTH.INT_VAL != 4 )
      printf( "Error.  Expected security key to be blank, but got %d bytes\n", msg->LENGTH.INT_VAL );
    test_state++;
  }
  
  
  else if( msg->COMMAND.INT_VAL == WIFI_SET_AP_STA_MODE && test_state == 14 )
  {
    test_state++;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_RESET_WIFI && test_state == 16 )
  {
    test_state = 0;
    test_number++;
  }
  return true;
}
#else
void RunNextTestStep( UINT16 timer )
{
  
  if( test_number < NUM_TESTS && test_state == 0 )
  {
    test_start_time = timer;
    test_timeout = 20 * 100;
    
    if( !test_active[test_number] )
    {
      test_number++;
      return;
    }
    else
      test_state++;
  }
  else if( test_state > 1 && timer - test_start_time > test_timeout ) 
  {
    test_state = 0;
    test_number++;    
  }
  

  switch( test_number )
  {
  case 0:
    WiFiTest1( timer );
    break;
  case 1:
    WiFiTest2( timer );
    break;
  case 2:
    WiFiTest3( timer );
    break;
  case 3:
    WiFiTest4( timer );
    break;
  case 4:
    WiFiTest5( timer );
    break;
  case 5:
    WiFiTest6( timer );
    break;
  case 6:
    WiFiTest7( timer );
    break;
  case 7:
    WiFiTest8( timer );
    break;
  case 8:
    WiFiTest9( timer );
    break;
  case 9:
    WiFiTest10( timer );
    break;
  case 10:
    WiFiTest11( timer );
    break;
  case 11:
    WiFiTest12( timer );
    break;
  case 12:
    WiFiTest13( timer );
    break;
  case 13:
    WiFiTest14( timer );
    break;
  case 14:
    WiFiTest15( timer );
    break;
    
  case 15:
    WiFiTest16( timer );
    break;
  case 16:
    WiFiTest17( timer );
    break;
  default:
    break;
  }

}

bool WiFiTestParseData( MSG_RECV* msg )
{
  
  if( msg->COMMAND.INT_VAL == 0x1000 )
  {
    test_number = 20;
    test_state = 0;
    return true;
  }
  
    
  switch( test_number )
  {
  case 0:
    return WiFiTest1ParseData( msg );
  case 1:
    return WiFiTest2ParseData( msg );
  case 2:
    return WiFiTest3ParseData( msg );
  case 3:
    return WiFiTest4ParseData( msg );
  case 4:
    return WiFiTest5ParseData( msg );
  case 5:
    return WiFiTest6ParseData( msg );
  case 6:
    return WiFiTest7ParseData( msg );
  case 7:
    return WiFiTest8ParseData( msg );
  case 8:
    return WiFiTest9ParseData( msg );
  case 9:
    return WiFiTest10ParseData( msg );
  case 10:
    return WiFiTest11ParseData( msg );
  case 11:
    return WiFiTest12ParseData( msg );
  case 12:
    return WiFiTest13ParseData( msg );
  case 13:
    return WiFiTest14ParseData( msg );
  case 14:
    return WiFiTest15ParseData( msg );
  case 15:
    return WiFiTest16ParseData( msg );
  case 16:
    return WiFiTest17ParseData( msg );
  /*case 17:
    return WiFiTest18ParseData( msg );
  case 18:
    return WiFiTest19ParseData( msg );
  case 19:
    return WiFiTest20ParseData( msg );
  case 20:
    return WiFiTest21ParseData( msg );
  case 21:
    return WiFiTest22ParseData( msg );*/
  default:
    return false;
  }
  //return false;
}


#define NEXT_STATE { test_start_time = timer; test_state++; }
#define NEXT_TEST  { test_state = 0; test_number++; }


void WiFiTest1( UINT16 timer )
{
  
  switch( test_state )
  {
  case 1:
    if( SendDisconnectAP() ) { NEXT_STATE }
    break;
  case 2:
    if( SendSetSSID_AP( WIFISSID, strlen( WIFISSID ) + 1 ) ) { NEXT_STATE }
    break;
  case 3:
    if( SendSetSecurityType_AP( 0 ) ) { NEXT_STATE }
    break;
  case 4:
    if( SendSetSecurityKey_AP( "", strlen( "" ) ) ) { NEXT_STATE }
    break;

  case 5:
    if( SendSetApStaMode( 0x11 ) ) { NEXT_STATE }
    break;
  case 6:
	  NEXT_STATE;
    //if( SendConnectAP() ) { NEXT_STATE }
    break;
  case 7:
    if( ResetWiFi() ) { NEXT_STATE }
    break;
  case 8:
    if( timer - test_start_time > 15 * 100  ) { NEXT_TEST }
    break;
  }
   
}


bool WiFiTest1ParseData( MSG_RECV* msg )
{
  return true;
}


void WiFiTest2( UINT16 timer )
{
  
  switch( test_state )
  {
  case 1:
    if( SendGetWifiVersion() ) { NEXT_STATE }
    break;
  case 2:
    if( SendGetUartVersion() ) { NEXT_STATE }
    break;
  case 3:
    if( SendGetMacAddress() ) { NEXT_STATE }
    break;
  case 4:
    if( SendGetIP() ) { NEXT_STATE }
    break;
  case 5:
    if( SendGetSSID() ) { NEXT_STATE }
    break;
  case 6:
    if( SendGetSecurityType() ) { NEXT_STATE }
    break;
  case 7:
    if( SendGetSecurityKey() ) { NEXT_STATE }
    break;
  case 8:
    if( SendGetSSID_AP() ) { NEXT_STATE }
    break;
  case 9:
    if( SendGetSecurityType_AP() ) { NEXT_STATE }
    break;
  case 10:
    if( SendGetSecurityKey_AP() ) { NEXT_STATE }
    break;
  case 11:
    if( SendGetUDPPort() ) { NEXT_STATE }
    break;
  case 12:
    if( SendGetTCPBufferSize( 1 ) ) { NEXT_STATE }
    break;
  case 13:
    if( SendSetTCPAddress( 1, "0.0.0.0", strlen( "0.0.0.0" ) + 1 ) ) { NEXT_STATE }
    break;
  case 14:
    if( SendGetTCPAddress( 1 ) ) { NEXT_STATE }
    break;
  case 15:
    if( SendSetTCPPort( 1, 30304, 0x11 ) ) { NEXT_STATE }
    break;
  case 16:
    if( SendGetTCPPort( 1 ) ) { NEXT_STATE }
    break;
  case 17:
    if( SendGetTCPBufferSize( 2 ) ) { NEXT_STATE }
    break;
  case 18:
    if( SendGetTCPAddress( 2 ) ) { NEXT_STATE }
    break;
  case 19:
    if( SendGetTCPPort( 2 ) ) { NEXT_STATE }
    break;
  case 20:
    if( SendGetTCPBufferSize( 3 ) ) { NEXT_STATE }
    break;
  case 21:
    if( SendGetTCPAddress( 3 ) ) { NEXT_STATE }
    break;
  case 22:
    if( SendGetTCPPort( 3 ) ) { NEXT_STATE }
    break;
  case 23:
    {
      char deviceInfo[255];
      memset( deviceInfo, 0, 255 );
      sprintf( 
        deviceInfo, 
        "LS10.0T %d.%d.%d.%d:%d 123456",
        wifi_settings.ip_address[2], 
        wifi_settings.ip_address[3], 
        wifi_settings.ip_address[4], 
        wifi_settings.ip_address[5], 
        wifi_settings.tcp_settings[0].port.INT_VAL
      );
      if( SendSetBroacastData( (char*)deviceInfo, strlen( deviceInfo ) + 1 ) ) { NEXT_STATE }
      break;
    }
  case 24:
    if( SendTCPConnect( 1 ) ) { NEXT_STATE }
    break;
  case 25:
    if( SendTCPConnect( 2 ) ) { NEXT_STATE }
    break;
  case 26:
    if( SendTCPConnect( 3 ) ) { NEXT_TEST }
    break;
  }
   
}


bool WiFiTest2ParseData( MSG_RECV* msg )
{
  return true;
}



void TestConnectToAP( CHAR* ssid, int ssidLength, CHAR* securityKey, int keyLength, int securityType, UINT16 timer )
{
  
  if( test_state == 1 && SendSetSSID( ssid, ssidLength ) )
    NEXT_STATE
  else if( test_state == 2 && SendSetSecurityKey( securityKey, keyLength ) )
    NEXT_STATE
  else if( test_state == 3 && SendSetSecurityType( securityType ) )
    NEXT_STATE
  else if( test_state == 4 && SendConnectAP() )
    NEXT_STATE
  else if( test_state == 5 && ResetWiFi() )
    NEXT_STATE
  else if( test_state == 6 && timer - test_start_time > 15 * 100 )
    NEXT_STATE
  else if( test_state == 7 )
    SendGetStatus();
  else if( test_state == 8 && SendGetIP() )
    NEXT_STATE
    
  
}

void WiFiConnectionTestParseData( MSG_RECV* msg )
{
  if( msg->COMMAND.INT_VAL == WIFI_GET_STATUS && test_state == 7 )
  {
    if( wifi_settings.status.wifi_connected )
      test_state++;
  }
  else if( msg->COMMAND.INT_VAL == WIFI_GET_IP && test_state == 9 )
  {
    for( int i = 0; i < 6; i++ )
      if( wifi_settings.ip_address[i] != 0 )
      {
        NEXT_TEST; 
        return;
      }
    NEXT_TEST;
  }
}


void WiFiTest3( UINT16 timer )
{
  TestConnectToAP( 
    UNSECURED_WIFI_SSID, 
    strlen( UNSECURED_WIFI_SSID ), 
    UNSECURED_SECURITY_KEY, 
    strlen( UNSECURED_SECURITY_KEY ), 
    UNSECURED_SECURITY_TYPE,
    timer 
  );
}


bool WiFiTest3ParseData( MSG_RECV* msg )
{
  WiFiConnectionTestParseData( msg );
  return true;
}

void WiFiTest4( UINT16 timer )
{
  TestConnectToAP( 
    WEP40_WIFI_SSID, 
    strlen( WEP40_WIFI_SSID ), 
    WEP40_SECURITY_KEY, 
    5, 
    WEP40_SECURITY_TYPE,
    timer 
  );
}


bool WiFiTest4ParseData( MSG_RECV* msg )
{
  
  WiFiConnectionTestParseData( msg );
  return true;
}

void WiFiTest5( UINT16 timer )
{
  TestConnectToAP( 
    WEP108_WIFI_SSID, 
    strlen( WEP108_WIFI_SSID ), 
    WEP108_SECURITY_KEY, 
    strlen( WEP108_SECURITY_KEY ), 
    WEP108_SECURITY_TYPE,
    timer 
  );
 
}


bool WiFiTest5ParseData( MSG_RECV* msg )
{
  
  WiFiConnectionTestParseData( msg );
  return true;
}

void WiFiTest6( UINT16 timer )
{
  
  TestConnectToAP( 
    WPA_WIFI_SSID, 
    strlen( WPA_WIFI_SSID ), 
    WPA_SECURITY_KEY, 
    strlen( WPA_SECURITY_KEY ), 
    WPA_SECURITY_TYPE,
    timer
  );
  
}


bool WiFiTest6ParseData( MSG_RECV* msg )
{
  WiFiConnectionTestParseData( msg );
  return true;
}

void WiFiTest7( UINT16 timer )
{
  
  TestConnectToAP( 
    WPA2_WIFI_SSID, 
    strlen( WPA2_WIFI_SSID ), 
    WPA2_SECURITY_KEY, 
    strlen( WPA2_SECURITY_KEY ), 
    WPA2_SECURITY_TYPE,
    timer 
  );
  
}


bool WiFiTest7ParseData( MSG_RECV* msg )
{
  WiFiConnectionTestParseData( msg );
  return true;
}

void WiFiTest8( UINT16 timer )
{ 
  TestConnectToAP( 
    WPA_AUTO_WIFI_SSID, 
    strlen( WPA_AUTO_WIFI_SSID ), 
    WPA_AUTO_SECURITY_KEY, 
    strlen( WPA_AUTO_SECURITY_KEY ), 
    WPA_AUTO_SECURITY_TYPE,
    timer 
  );
}


bool WiFiTest8ParseData( MSG_RECV* msg )
{
  return true;
}

void WiFiTest9( UINT16 timer )
{
  if( test_state == 6 )
    fail_timer = test_start_time = timer;
  TestConnectToAP( "Office", strlen( "Office" ), "JHTNAWireless2", strlen( "JHTNAWireless2" ), 5, timer );
  
  if( test_state == 7 && timer - fail_timer > 6 * 100 )
  {
    test_state = 0;
    test_number++;
  }  
  
  
}


bool WiFiTest9ParseData( MSG_RECV* msg )
{
  
  if( msg->COMMAND.INT_VAL == WIFI_GET_STATUS && test_state == 6 )
  {
    UINT16 status = *(int*)(msg->DATA + 4);
    if( status & 0x0001 != 0 )
    {
      test_state = 0;
      test_number++;
    }
  }
  
   
  return true;
}


void WiFiTest10( UINT16 timer )
{
  
  switch( test_state )
  {
  case 1:
    if( SendSetSSID( DEFAULT_SSID, strlen( DEFAULT_SSID ) ) ) { NEXT_STATE }
    break;
  case 2:
    if( SendSetSecurityType( DEFAULT_SECURITY_TYPE ) ) { NEXT_STATE }
    break;
  case 3:
    if( SendSetSecurityKey( DEFAULT_SECURITY_KEY, strlen( DEFAULT_SECURITY_KEY ) ) ) { NEXT_STATE }
    break;
  case 4:
    if( SendConnectAP() ) { NEXT_STATE }
    break;
  case 5:
    if( SendSetApStaMode( 0x55 ) ) { NEXT_STATE }
    break;
  case 6:
    if( ResetWiFi() ) { NEXT_STATE }
    break;
  case 7:
    if( timer - test_start_time > 15 * 100  ) { NEXT_STATE }
    break;
  case 8:
    if( SendGetStatus() ) { NEXT_STATE }
    break;
  case 9:
    if( SendGetIP() ) { NEXT_STATE }
    break;
  }
  
}


bool WiFiTest10ParseData( MSG_RECV* msg )
{
  if( test_state == 10 )
  {
    NEXT_TEST
  }
  return true;
}


void WiFiTest11( UINT16 timer )
{
  if( test_state == 1 && SendTCPDisconnect( 1 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
      
  if( test_state == 3 && SendTCPDisconnect( 2 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
     
  if( test_state == 5 && SendTCPDisconnect( 3 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
  if( test_state == 7 && SendSetTCPAddress( 1, TEST_SERVER_IP, strlen( TEST_SERVER_IP ) + 1 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
  
  if( test_state == 9 && SendSetTCPAddress( 2, TEST_SERVER_IP_2, strlen( TEST_SERVER_IP_2 ) + 1 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
      
  if( test_state == 11 && SendSetTCPAddress( 3, TEST_SERVER_IP_3, strlen( TEST_SERVER_IP_3 ) + 1 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
  
  if( test_state == 13 && SendGetTCPAddress( 1 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
  
  if( test_state == 15 && SendGetTCPAddress( 2 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
  if( test_state == 17 && SendGetTCPAddress( 3 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
   
      
   
  if( test_state == 19 && SendSetTCPPort( 1, TEST_SERVER_PORT, 0x22 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
      
  
  if( test_state == 21 && SendSetTCPPort( 2, TEST_SERVER_PORT_2, 0x22 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
  
  if( test_state == 23 && SendSetTCPPort( 3, TEST_SERVER_PORT_3, 0x22 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
  if( test_state == 25 && SendGetTCPPort( 1 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }    
  
  if( test_state == 27 && SendGetTCPPort( 2 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }    
  if( test_state == 29 && SendGetTCPPort( 3 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }        
      
   
}


bool WiFiTest11ParseData( MSG_RECV* msg )
{
  if( msg->COMMAND.INT_VAL == WIFI_TCP_DISCONNECT && test_state == 2 )
  {
    test_state++;
    return true;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_TCP_DISCONNECT && test_state == 4 )
  {
    test_state++;
    return true;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_TCP_DISCONNECT && test_state == 6 )
  {
    test_state++;
    return true;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_SET_TCP_ADDRESS && test_state == 8 )
  {
    test_state++;
    return true;
  }
  
  
  else if( msg->COMMAND.INT_VAL == WIFI_SET_TCP_ADDRESS && test_state == 10 )
  {
    test_state++;
    return true;
  }
  
  
  else if( msg->COMMAND.INT_VAL == WIFI_SET_TCP_ADDRESS && test_state == 12 )
  {
    test_state++;
    return true;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_ADDRESS && test_state == 14 )
  {
    if( strncmp( (char*)wifi_settings.tcp_settings[0].ip_address, TEST_SERVER_IP, strlen( TEST_SERVER_IP ) ) != 0 )
    {
        NEXT_TEST
        return false;
    }
    else
      test_state++;
  }
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_ADDRESS && test_state == 16 )
  {
    if( strncmp( (char*)wifi_settings.tcp_settings[1].ip_address, TEST_SERVER_IP_2, strlen( TEST_SERVER_IP_2 ) ) != 0 )
    {
        NEXT_TEST
        return false;
    }
    else
      test_state++;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_ADDRESS && test_state == 18 )
  {
    if( strncmp( (char*)wifi_settings.tcp_settings[2].ip_address, TEST_SERVER_IP_3, strlen( TEST_SERVER_IP_3 ) ) != 0 )
    {
        NEXT_TEST
        return false;
    }
    else
      test_state++;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_SET_TCP_PORT && test_state == 20 )
  {
    test_state++;
    return true;
  }
  
  
  else if( msg->COMMAND.INT_VAL == WIFI_SET_TCP_PORT && test_state == 22 )
  {
    test_state++;
    return true;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_SET_TCP_PORT && test_state == 24 )
  {
    test_state++;
    return true;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_PORT && test_state == 26 )
  {
    if( wifi_settings.tcp_settings[0].port.INT_VAL != TEST_SERVER_PORT || wifi_settings.tcp_settings[0].mode != 0x22 )
    {
        test_number++;
        test_state = 0;
        return false;
    }
    else
      test_state++;
  }
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_PORT && test_state == 28 )
  {
    if( wifi_settings.tcp_settings[1].port.INT_VAL != TEST_SERVER_PORT_2 || wifi_settings.tcp_settings[1].mode != 0x22 )
    {
        NEXT_TEST;
        return false;
    }
    else
      test_state++;
  }
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_PORT && test_state == 30 )
  {
    if( wifi_settings.tcp_settings[2].port.INT_VAL != TEST_SERVER_PORT_3 || wifi_settings.tcp_settings[1].mode != 0x22 )
    {
    }
    else
    {
    }
    NEXT_TEST
  }
  return true;
}
      
      

void WiFiTest12( UINT16 timer )
{
  
  switch( test_state )
  {
  case 1:
    if( SendTCPConnect( 1 ) ) { NEXT_STATE }
    break;
  case 2:
    if( timer - test_start_time > 1 * 100  ) { NEXT_STATE }
    break;
  case 3:
    SendGetStatus();
    break;
  case 4:
    if( SendSetTCPData( 1, TEST_SERVER_SEND_DATA, strlen( TEST_SERVER_SEND_DATA ) ) )
    {
      NEXT_STATE
    }
    break;
  case 5:
  case 6:
    SendGetTCPData( 1 );
    break;
  case 7:
    if( SendTCPDisconnect( 1 ) ) { NEXT_STATE }
    break;
  case 8:
    if( SendTCPConnect( 2 ) ) { NEXT_STATE }
    break;
  case 9:
    if( timer - test_start_time > 1 * 100  ) { NEXT_STATE }
    break;
  case 10:
    SendGetStatus();
    break;
  case 11:
    if( SendSetTCPData( 2, TEST_SERVER_SEND_DATA_2, strlen( TEST_SERVER_SEND_DATA_2 ) ) )
    {
      NEXT_STATE
    }
    break;
  case 12:
  case 13:
    SendGetTCPData( 2 );
    break;
  case 14:
    if( SendTCPDisconnect( 2 ) ) { NEXT_STATE }
    break;
  case 15:
    if( SendTCPConnect( 3 ) ) { NEXT_STATE }
    break;
  case 16:
    if( timer - test_start_time > 1 * 100  ) { NEXT_STATE }
    break;
  case 17:
    SendGetStatus();
    break;
  case 18:
    if( SendSetTCPData( 3, TEST_SERVER_SEND_DATA_3, strlen( TEST_SERVER_SEND_DATA_3 ) ) )
    {
      NEXT_STATE
    }
    break;
  case 19:
  case 20:
    SendGetTCPData( 3 );
    break;
  case 21:
    if( SendTCPDisconnect( 3 ) ) {  NEXT_STATE }
    break;
  }    
   
}


bool WiFiTest12ParseData( MSG_RECV* msg )
{
  //3, 9, 15
  // 5, 11, 17
  if( test_state == 22 )
    NEXT_TEST
  else if( msg->COMMAND.INT_VAL == WIFI_GET_STATUS && test_state == 3 )
  {
    if( wifi_settings.status.tcp_connected[1] )
      test_state++;
  }
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_DATA && test_state == 5 )
  {
    if( msg->DATA[4] == 1 && msg->LENGTH.INT_VAL > 5 )
    {
      test_state++;
    }
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_DATA && test_state == 6 )
  {
    if( msg->DATA[4] == 1 && msg->LENGTH.INT_VAL <= 5 )
    {
      test_state++;
    }
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_GET_STATUS && test_state == 10 )
  {
    if( wifi_settings.status.tcp_connected[2] )
      test_state++;
  }
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_DATA && test_state == 12 )
  {
    if( msg->DATA[4] == 2 && msg->LENGTH.INT_VAL > 5 )
    {
      test_state++;
    }
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_DATA && test_state == 13 )
  {
    if( msg->DATA[4] == 2 && msg->LENGTH.INT_VAL <= 5 )
    {
      test_state++;
    }
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_GET_STATUS && test_state == 17 )
  {
    if( wifi_settings.status.tcp_connected[3] )
      test_state++;
  }
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_DATA && test_state == 19 )
  {
    if( msg->DATA[4] == 3 && msg->LENGTH.INT_VAL > 5 )
    {
      test_state++;
    }
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_DATA && test_state == 20 )
  {
    if( msg->DATA[4] == 3 && msg->LENGTH.INT_VAL <= 5 )
    {
      test_state++;
    }
  }
  
  return true;
}


void WiFiTest13( UINT16 timer )
{
  if( test_state == 1 && SendTCPDisconnect( 1 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
      
  if( test_state == 3 && SendTCPDisconnect( 2 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
     
  if( test_state == 5 && SendTCPDisconnect( 3 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
  if( test_state == 7 && SendSetTCPAddress( 1, "0.0.0.0", strlen( "0.0.0.0" ) + 1 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
  
  if( test_state == 9 && SendSetTCPAddress( 2, "0.0.0.0", strlen( "0.0.0.0" ) + 1 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
      
  if( test_state == 11 && SendSetTCPAddress( 3, "0.0.0.0", strlen( "0.0.0.0" ) + 1 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
  
  if( test_state == 13 && SendGetTCPAddress( 1 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
  
  if( test_state == 15 && SendGetTCPAddress( 2 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
  if( test_state == 17 && SendGetTCPAddress( 3 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
   
      
   
  if( test_state == 19 && SendSetTCPPort( 1, TEST_CONSOLE_SERVER_PORT, 0x11 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
      
  
  if( test_state == 21 && SendSetTCPPort( 2, TEST_CONSOLE_SERVER_PORT_2, 0x11 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
  
  if( test_state == 23 && SendSetTCPPort( 3, TEST_CONSOLE_SERVER_PORT_3, 0x11 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
  if( test_state == 25 && SendGetTCPPort( 1 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }    
  
  if( test_state == 27 && SendGetTCPPort( 2 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }    
  if( test_state == 29 && SendGetTCPPort( 3 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }        
      
   
}


bool WiFiTest13ParseData( MSG_RECV* msg )
{
  if( msg->COMMAND.INT_VAL == WIFI_TCP_DISCONNECT && test_state == 2 )
  {
    test_state++;
    return true;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_TCP_DISCONNECT && test_state == 4 )
  {
    test_state++;
    return true;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_TCP_DISCONNECT && test_state == 6 )
  {
    test_state++;
    return true;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_SET_TCP_ADDRESS && test_state == 8 )
  {
    test_state++;
    return true;
  }
  
  
  else if( msg->COMMAND.INT_VAL == WIFI_SET_TCP_ADDRESS && test_state == 10 )
  {
    test_state++;
    return true;
  }
  
  
  else if( msg->COMMAND.INT_VAL == WIFI_SET_TCP_ADDRESS && test_state == 12 )
  {
    test_state++;
    return true;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_ADDRESS && test_state == 14 )
  {
    if( strncmp( (char*)wifi_settings.tcp_settings[0].ip_address, "0.0.0.0", strlen( "0.0.0.0" ) ) != 0 )
    {
        NEXT_TEST
        return false;
    }
    else
      test_state++;
  }
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_ADDRESS && test_state == 16 )
  {
    if( strncmp( (char*)wifi_settings.tcp_settings[1].ip_address, "0.0.0.0", strlen( "0.0.0.0" ) ) != 0 )
    {
        NEXT_TEST
        return false;
    }
    else
      test_state++;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_ADDRESS && test_state == 18 )
  {
    if( strncmp( (char*)wifi_settings.tcp_settings[2].ip_address, "0.0.0.0", strlen( "0.0.0.0" ) ) != 0 )
    {
        NEXT_TEST
        return false;
    }
    else
      test_state++;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_SET_TCP_PORT && test_state == 20 )
  {
    test_state++;
    return true;
  }
  
  
  else if( msg->COMMAND.INT_VAL == WIFI_SET_TCP_PORT && test_state == 22 )
  {
    test_state++;
    return true;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_SET_TCP_PORT && test_state == 24 )
  {
    test_state++;
    return true;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_PORT && test_state == 26 )
  {
    if( wifi_settings.tcp_settings[0].port.INT_VAL != TEST_CONSOLE_SERVER_PORT || wifi_settings.tcp_settings[0].mode != 0x11 )
    {
        test_number++;
        test_state = 0;
        return false;
    }
    else
      test_state++;
  }
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_PORT && test_state == 28 )
  {
    if( wifi_settings.tcp_settings[1].port.INT_VAL != TEST_CONSOLE_SERVER_PORT_2 || wifi_settings.tcp_settings[1].mode != 0x11 )
    {
        NEXT_TEST;
        return false;
    }
    else
      test_state++;
  }
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_PORT && test_state == 30 )
  {
    if( wifi_settings.tcp_settings[2].port.INT_VAL != TEST_CONSOLE_SERVER_PORT_3 || wifi_settings.tcp_settings[1].mode != 0x11 )
    {
    }
    else
    {
    }
    NEXT_TEST
  }
  return true;
}

void WiFiTest14( UINT16 timer )
{
  
  switch( test_state )
  {
  case 1:
    if( SendTCPConnect( 1 ) ) { NEXT_STATE }
    break;
  case 2:
    if( timer - test_start_time > 1 * 100  ) 
    {

      NEXT_STATE 
    }
    break;
  case 3:
    SendGetStatus();
    if( timer - test_start_time > 10 * 100  ) 
    {
      test_state = 6;
    }
    break;
  case 4:
    if( SendSetTCPData( 1, TEST_CONSOLE_SERVER_SEND_DATA, strlen( TEST_CONSOLE_SERVER_SEND_DATA ) ) )
    {
      NEXT_STATE
    }
    break;
  case 5:
    SendGetTCPData( 1 );
    break;
  case 6:
    if( SendTCPDisconnect( 1 ) ) { NEXT_STATE }
    break;
  case 7:
    if( SendTCPConnect( 2 ) ) { NEXT_STATE }
    break;
  case 8:
    if( timer - test_start_time > 1 * 100  ) 
    { 
     
      NEXT_STATE 
    }
    break;
  case 9:
    SendGetStatus();
    
    if( timer - test_start_time > 10 * 100  ) 
    {
      test_state = 13;
    }
    break;
    
  case 10:
    if( SendSetTCPData( 2, TEST_CONSOLE_SERVER_SEND_DATA_2, strlen( TEST_CONSOLE_SERVER_SEND_DATA_2 ) ) )
    {
      NEXT_STATE
    }
    break;
  case 11:
    SendGetTCPData( 2 );
    break;
  case 12:
    if( SendTCPDisconnect( 2 ) ) { NEXT_STATE }
    break;
  case 13:
    if( SendTCPConnect( 3 ) ) { NEXT_STATE }
    break;
  case 14:
    if( timer - test_start_time > 1 * 100  ) 
    { 
      
      NEXT_STATE 
    }
    break;
  case 15:
    SendGetStatus();
    
    if( timer - test_start_time > 10 * 100  ) 
    {
      test_state = 18;
    }
    break;
  case 16:
    if( SendSetTCPData( 3, TEST_CONSOLE_SERVER_SEND_DATA_3, strlen( TEST_SERVER_SEND_DATA_3 ) ) )
    {
      NEXT_STATE
    }
    break;
  case 17:
    SendGetTCPData( 3 );
    break;
  case 18:
    if( SendTCPDisconnect( 3 ) ) {NEXT_STATE }
    break;
  }    
   
}


bool WiFiTest14ParseData( MSG_RECV* msg )
{
  //3, 9, 15
  // 5, 11, 17
  if( test_state == 19 )
    NEXT_TEST
  else if( msg->COMMAND.INT_VAL == WIFI_GET_STATUS && test_state == 3 )
  {
    if( wifi_settings.status.tcp_connected[1] )
      test_state++;
  }
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_DATA && test_state == 5 )
  {
    if( msg->DATA[4] == 1 && msg->LENGTH.INT_VAL > 5 )
    {
      test_state++;
    }
  }
  
  
  else if( msg->COMMAND.INT_VAL == WIFI_GET_STATUS && test_state == 9 )
  {
    if( wifi_settings.status.tcp_connected[2] )
      test_state++;
  }
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_DATA && test_state == 11 )
  {
    if( msg->DATA[4] == 2 && msg->LENGTH.INT_VAL > 5 )
    {
      test_state++;
    }
  }
  
  
  else if( msg->COMMAND.INT_VAL == WIFI_GET_STATUS && test_state == 15 )
  {
    if( wifi_settings.status.tcp_connected[3] )
      test_state++;
  }
  else if( msg->COMMAND.INT_VAL == WIFI_GET_TCP_DATA && test_state == 17 )
  {
    if( msg->DATA[4] == 3 && msg->LENGTH.INT_VAL > 5 )
    {
      test_state++;
    }
  }
  
  
  return true;
}

void WiFiTest15( UINT16 timer )
{
  if( test_state == 1 && SendSetWorkoutIP( WORKOUT_DATA_SERVER, strlen( WORKOUT_DATA_SERVER ) + 1 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
      
  if( test_state == 3 && SetHTTPData( WORKOUT_DATA, strlen( WORKOUT_DATA ) + 1 ) )
  {
    test_start_time = timer;
    test_state++;
  }
  
  if( test_state == 5 )// && SendHTTPData( ) )
  {
    test_start_time = timer;
    test_state++;
  }
  
   
}


bool WiFiTest15ParseData( MSG_RECV* msg )
{
  if( msg->COMMAND.INT_VAL == WIFI_SET_WORKOUT_DATA_IP && test_state == 2 )
  {
    test_state++;
    return true;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_SET_HTTP_DATA && test_state == 4 )
  {
    test_state++;
    return false;
  }
  else if( msg->COMMAND.INT_VAL == WIFI_SEND_HTTP_DATA && test_state == 6 )
  {
    test_state = 0;
    test_number++;
    return false;
  }
  return true;
}


void WiFiTest16( UINT16 timer )
{
//  if( test_state == 1 && SendRemoteUpdate( REMOTE_UPDATE_STRING, strlen( REMOTE_UPDATE_STRING ) + 1 ) )
//  {
    NEXT_STATE
//  }
      
}


bool WiFiTest16ParseData( MSG_RECV* msg )
{
  if( test_state == 2 )
    NEXT_TEST
  return true;
}
      


void WiFiTest17( UINT16 timer )
{
  if( test_state == 1 && SendSetSSID_AP( AP_TEST_SSID, strlen( AP_TEST_SSID ) + 1 ) )
  {
    fail_timer = test_start_time = timer;
    test_start_time = timer;
    test_state++;
  }
      
  else if( test_state == 3 && SendGetSSID_AP() )
  {
    test_start_time = timer;
    test_state++;
  }
                                         
                                            
  else if( test_state == 5 && SendSetSecurityType_AP( 0 ) )
  {
    test_start_time = timer;
    test_state++;
  }
                                         
                                            
  else if( test_state == 7 && SendGetSecurityType_AP() )
  {
    test_start_time = timer;
    test_state++;
  }
                                         
                                         
  else if( test_state == 9 && SendSetSecurityKey_AP( "", 0 ) )
  {
    test_start_time = timer;
    test_state++;
  }
                                         
                                            
  else if( test_state == 11 && SendGetSecurityKey_AP() )
  {
    test_start_time = timer;
    test_state++;
  }
      
  else if( test_state == 13 && SendSetApStaMode( AP_TEST_MODE ) )
  {
    test_start_time = timer;
    test_state++;
  }
  
  else if( test_state == 15 && ResetWiFi() )
  {
    test_start_time = timer;
    test_state++;
  }
  
   
}


bool WiFiTest17ParseData( MSG_RECV* msg )
{
  if( msg->COMMAND.INT_VAL == WIFI_SET_SSID_AP && test_state == 2 )
  {
    test_state++;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_GET_SSID_AP && test_state == 4 )
  {
    if( strncmp( (char*)(msg->DATA + 4), AP_TEST_SSID, strlen( AP_TEST_SSID ) ) != 0 )
    {
    }
    test_state++;
  }
  else if( msg->COMMAND.INT_VAL == WIFI_SET_AP_SECURE_TYPE && test_state == 6 )
  {
    test_state++;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_GET_AP_SECURE_TYPE && test_state == 8 )
  {
    if( msg->DATA[4] != 0 )
    {
    }
    test_state++;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_SET_AP_SECURE_KEY && test_state == 10 )
  {
    test_state++;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_GET_AP_SECURE_KEY && test_state == 12 )
  {
    if( msg->LENGTH.INT_VAL != 4 )
    {
    }
    test_state++;
  }
  
  
  else if( msg->COMMAND.INT_VAL == WIFI_SET_AP_STA_MODE && test_state == 14 )
  {
    test_state++;
  }
  
  else if( msg->COMMAND.INT_VAL == WIFI_RESET_WIFI && test_state == 16 )
  {
    test_state = 0;
    test_number++;
  }
  return true;
}
#endif

#endif


