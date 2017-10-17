#ifndef __USBMANAGER_H__
#define __USBMANAGER_H__
#include "JIStypes.h"

#ifndef EFM32LG380F256
#error "This library only supports EFM32LG380F256 chip"
#else

#ifndef _ALLOW_BACK_FEATURE_
#define _ALLOW_BACK_FEATURE_
#endif 

typedef enum tag_UsbEvents{
    EVT_USB_NONE = 0,
    EVT_USB_REMOVED,
    EVT_USB_CONNECTED,
    EVT_USB_ERROR,
    EVT_FILE_SIZE_ERROR,
    EVT_FILE_NOT_EXIST,
    EVT_FILE_CRC_ERROR,
    EVT_FILE_DETECTED,
    EVT_FILE_UPDATE_CANCELED,
    EVT_FILE_UPDATE_CONFIRMED,
    EVT_FILE_BT_UPDATE_CONFIRMED, // WWW
    EVT_FILE_BT_UPDATE_READFAILED, // WWW
    EVT_BTUPDATEFILE_DETECTED, // WWW
    EVT_USB_TOTAL
}USB_EVENTS;

typedef enum
{
    STATE_NO_DEVICE = 0,
    STATE_DEVICE_ENUM,
    STATE_DEVICE_READY,
    STATE_UNKNOWN_DEVICE,
    STATE_POWER_FAULT,
    STATE_FILE_ERROR,
    STATE_FILE_DETECTED
  
}USB_STATE;

void USBManager_Initial_IO(void);
void USBManager_Initial_Data(void);
USB_EVENTS USBManager_Process(void);
USB_STATE USB_Get_State(void);
void USBManager_Set_VirtualKey(UINT16 vk);
void UsbBttoRead();
#endif

#endif