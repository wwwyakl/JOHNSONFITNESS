#ifndef __MAIN_H
#define	__MAIN_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


//#include "hw_ints.h"
//#include "hw_memmap.h"
//#include "hw_types.h"
//#include "interrupt.h"
//#include "sysctl.h"
//#include "systick.h"
//#include "udma.h"
//#include "usb.h"
//#include "usblib.h"
//#include "usbmsc.h"
//#include "usbhost.h"
//#include "usbhmsc.h"
//#include "include.h"
#include "LED.h"
#include "Display.h"
//#include "Digital_wifi.h"
//#include "dataCenter.h"
#include "HAL_Lib.h"
#include "LCB.h"
#include "page.h"
#include "Show.h"
#include "Global_Config.h"
#include "SystemConfig.h"
#include "UtilitiesBeep.h"
#include "UtilitiesFan.h"
#include "UtilitiesTimer.h"
#include "Midware_Heart.h"

#include "HT16CXX.h"
#include "UtilitiesBor.h"
#include "UtilitiesRtc.h"
#include "UtilitiesErp.h"
#include "UtilitiesErrorcode.h"

#include "Programs.h"

//#include "Fitlinx.h"
#include "USBManager.h"
//#include "Xml_Driver.h"

#include "Events_Handler.h"
#include "RF_lib.h"
//#include "wifi_api.h"

void Main_Init_Model_Data(void);
#endif
