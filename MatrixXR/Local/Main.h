#ifndef __MAIN_H
#define	__MAIN_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "em_emu.h"
#include "em_rmu.h"
#include "Global_Config.h"
#include "HAL_Lib.h"

#include "Display.h"
#include "Digital_wifi.h"
#include "LCB.h"
#include "page.h"
#include "Show.h"
#include "SystemConfig.h"
#include "UtilitiesBeep.h"
#include "UtilitiesFan.h"
#include "UtilitiesTimer.h"
#include "UtilitiesBor.h"
#include "UtilitiesRtc.h"
#include "UtilitiesErp.h"
#include "UtilitiesErrorcode.h"
#include "Programs.h"
#include "USBManager.h"
#include "Events_Handler.h"
#include "RF_lib.h"
#include "wifi_api.h"
#include "Auto_Model.h"
#include "SPI_Flash.h"
#include "User.h"
#include "HT16CXX.h"
#include "Show_Functions.h"
#include "Midware_Heart.h"

void Main_Initial_Data(INIT_TYPE type);
#endif
