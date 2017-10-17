#ifndef __CRC_H
#define __CRC_H
//#include "hw.H"
#include "jisdef.h"

#ifdef __BLUETOOTH_FUNC__//__WIFI_FUNC__
//#include "MyStruct.H"
#include "JIStypes.h"
#define DCrc_ResetValue							0xFFFF


UINT16 GenerateCRC_CCITT_F1( PUINT8 PUPtr8, UINT16 UINT16 );//kill F1
UINT16 GenerateCRC_CCITT( PUINT8 PUPtr8, UINT16 UINT16 );
#endif
#endif
