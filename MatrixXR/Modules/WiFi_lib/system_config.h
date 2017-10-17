/*
 * system_config.h
 *
 *  Created on: May 23, 2013
 *      Author: brenden.capps
 */

#ifndef SYSTEM_CONFIG_H_
#define SYSTEM_CONFIG_H_

#include "jisdef.h"
#ifdef __WIFI_FUNC__

#define MIN_RESISTANCE_LEVEL 1
#define MAX_RESISTANCE_LEVEL 30
#define MAX_ECB_COUNT 320
#define MAX_PWM_COUNT 1800

#define CONSOLE_SERIES "LS"
#define EQUIPMENT_TYPE "T"
#define SOFTWARE_VERSION "0.1.0.0"
#define SOFTWARE_VERSION_INT 0x00010000
#define UPDATE_SERVER_LOCATION "/update"
#define DATA_SERVER_LOCATION "/data"

  #if defined (__ADVENTURE3__) || defined (__ADVENTURE5__)
#define BRAND HORIZON
#elif defined (__JOHNSON6__) || defined (__JOHNSON8__)
#define BRAND JOHNSON
#endif

#endif

#endif /* SYSTEM_CONFIG_H_ */
