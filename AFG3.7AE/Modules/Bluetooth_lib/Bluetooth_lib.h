#ifndef __BLUETOOTH_LIB__H__
    #define __BLUETOOTH_LIB__H__
#include "JIStypes.h"
#include "Remote_process.h"
#include "LED.h"

typedef enum
{
    BLUETOOTH_EVT_NONE  = 0,
    BLUETOOTH_CHANGE_USER,
    BLUETOOTH_CHANGE_PROGRAM,
    BLUETOOTH_MODE_MASTER,
    BLUETOOTH_MODE_SLAVE,
    BLUETOOTH_UPDATE_START, 
    BLUETOOTH_UPDATE_END,   
    BLUETOOTH_UPDATE_FAIL,
    BLUETOOTH_EVENT_ALL   
}BLUETOOTH_EVENT;

void BT_Set_SyncFlag(bool b_Data);
bool BT_Get_SyncFlag(void);
void  Bluetooth_Set_VirtualKey(UINT16 vk);
void initialBluetoothLib();
void ResetinitialBluetoothLib();
bool BluetoothSetBleName(UINT8 Mode);
BLUETOOTH_EVENT BluetoothProcess();
UINT8 Bluetooth_Get_HeartData(void);
UINT8 Bluetooth_Get_ConnectStatus(void);
UINT8 Bluetooth_Get_BT_LED_Status(void);
void CleargBluetoothData(void);
void BluetoothSetDirectSendFuncation(SENDFUNC pfuc);
bool BluetoothStartScaning();
void BT_Regist_Functions(void);
bool BluetoothGetBleName(void);
bool BluetoothGetLOCALName(void);
bool BluetoothSetAmpName(void);
UINT8 Bluetooth_Get_BeltConnectStatus(void);
#endif
