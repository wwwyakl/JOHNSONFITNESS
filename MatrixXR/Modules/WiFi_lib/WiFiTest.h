#ifndef _WIFI_TEST_
#define _WIFI_TEST_
#include "jisdef.h"

#ifdef __WIFI_FUNC__
#include "Digital_Wifi.h"
#include "wifi_api.h"
void RunNextTestStep( UINT16 timer );
bool WiFiTestParseData( MSG_RECV* msg );
bool TestingWifi();
#endif

#endif
