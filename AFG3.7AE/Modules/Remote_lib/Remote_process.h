/****************************************************************************
*
*
*
*
*
*
****************************************************************************/
#ifndef __REMOTE_PROCESS_H__
#define __REMOTE_PROCESS_H__
#include "remote_config.h"

#include "Remote_user.h"
#include "Remote_workoutTracking.h"
#include "Remote_asset.h"
#ifdef __REMOTE_LIB__



void RemoteSetLoginParameter(UINT8 loginType, UINT8 slot, UINT8 *userinfor, UINT8 *passcod);
RemoteEvent RemoteProcess(RemoteDevicesInformation *device);
bool  RemoteRegisterDevice(RemoteDevicesInformation *device);
#endif

#endif
