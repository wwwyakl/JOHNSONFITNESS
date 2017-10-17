#ifndef __USBMANAGER_H__
#define __USBMANAGER_H__
#ifndef EFM32LG380F256
#error "This library only supports EFM32LG380F256 chip"
#else

#ifndef _ALLOW_BACK_FEATURE_
#define _ALLOW_BACK_FEATURE_
#endif 
#include "Global_Config.h"

typedef enum tag_UsbEvents{
    EVT_USB_NONE = 0,
	EVT_USB_PROCESSED,		
    EVT_USB_REMOVED,
    EVT_USB_CONNECTED,
    EVT_USB_ERROR,
    EVT_FILE_SIZE_ERROR,
    EVT_FILE_NOT_EXIST,
    EVT_FILE_CRC_ERROR,
    EVT_FILE_DETECTED,
    EVT_FILE_LANGUAGE_UPDATE,
    EVT_FILE_LANGUAGE_UPDATEING,
    EVT_FILE_UPDATE_CANCELED,
    EVT_FILE_UPDATE_CONFIRMED,
    EVT_USB_TOTAL
}USB_EVENTS;
#define _Lang_UcbMachineType_ "MatrixXR"//"2015-T1x"

void USBManager_Initial_IO(void);
void USBManager_Initial_Data(void);
USB_EVENTS USBManager_Process(ReturnEventHandler flag);
VR_KEY USBManager_Set_VirtualKey(VR_KEY vk);
void USBManager_Updata_Language(void);
void USBManager_Regist_Function(void);
void USBManager_Regist_Function(void);
#endif

#endif
