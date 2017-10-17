#ifndef __REMOTE_ASSET_H__
#define __REMOTE_ASSET_H__

#include "remote_config.h"
#include "JISTypes.h"
#include "Global_Config.h"
#include "remote_datatype.h"

//#ifdef __REMOTE_ASSET_MANAGE__







SubProcessState RemoteAssetProcess(RemoteDevicesInformation *deviceinfor, UINT8* buf, UINT16 *bufLength, const  DapiGlobalVariable* dapiGlobalVariable);
RemoteResponse RemoteAssetReceive(SubReceiveDataType datatype, ADDSTATESFUNC addstatefunc, DapiGlobalVariable* dapiGlobalVariable );

//#endif
#endif

