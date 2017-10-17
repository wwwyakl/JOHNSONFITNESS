#ifndef __REMOTE_USER_H__
#define __REMOTE_USER_H__
#include "remote_config.h"
#include "JISTypes.h"
#include "Remote_datatype.h"

#ifdef __REMOTE_USER_MANAGE__





SubProcessState RemoteUserProcess(RemoteDevicesInformation *deviceinfor, UINT8* buf, UINT16 *bufLength,const  DapiGlobalVariable* dapiGlobalVariable);
RemoteResponse RemoteUserReceive(SubReceiveDataType datatype, ADDSTATESFUNC addstatefunc, DapiGlobalVariable* dapiGlobalVariable );


#endif

#endif

