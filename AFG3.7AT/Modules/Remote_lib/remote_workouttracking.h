#ifndef __REMOTE_WORKOUT_TRACKING_H__
#define __REMOTE_WORKOUT_TRACKING_H__
#include "remote_config.h"
#include "JISTypes.h"
#include "remote_datatype.h"


#ifdef __REMOTE_WORKOUT_TRACKING__



SubProcessState RemoteWorkoutProcess(RemoteDevicesInformation *deviceinfor, UINT8* buf, UINT16 *bufLength, const  DapiGlobalVariable* dapiGlobalVariable);
RemoteResponse RemoteWorkoutReceive(SubReceiveDataType datatype, ADDSTATESFUNC addstatefunc, DapiGlobalVariable* dapiGlobalVariable );
void RemoteChangeWorkotState(RemoteWorkoutState state, ADDSTATESFUNC addstatefunc, DapiGlobalVariable* dapiGlobalVariable, UINT8 *deviceMac );

#endif

#endif
