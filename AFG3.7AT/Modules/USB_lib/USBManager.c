#include "USBManager.h"
#include "SystemConfig.h"

#include "em_usb.h"
#include "ff.h"
#include "msdh.h"
#include "usbconfig.h"
#include "HAL_Lib.h"
//These parameters must match the icf file configuration. Do NOT touch.
#define FLASH_END_ADDRESS               0x0003ffff
#define information1ADDR		0x3e000
extern UINT8 PrintBuf[30];

//WWW
   UINT ToRead =1;
   UINT ToWrite =0;
   UINT OverRead =0; 
DWORD   BtUpdateFileSize;    //zsf
UINT8 BtLastSize;
unsigned char g_BtUpdateFileDate[256];

//List all possible directory
typedef enum tag_JohnsonDirs
{
    ROOT_DIR = 0,
    LS_DIR1,
    LS_DIR2,
    AFG_DIR1,
    AFG_DIR2,
    HF_DIR1,
    HF_DIR2,
    VF_DIR1,
    VF_DIR2,
    BT_DIR1, // WWW
    TOTAL_DIRS
}JOHNSON_DIRS;
const char ROOT_DIRECTORY[20] = {"."};
const char LS_DIRECTORY1[20] = {"/LS/FW"};
const char LS_DIRECTORY2[20] = {"/LS/FWF"};
const char AFG_DIRECTORY1[20] = {"/AFG/FW"};
const char AFG_DIRECTORY2[20] = {"/AFG/FWF"};
const char HF_DIRECTORY1[20] = {"/HORIZON/FW"};
const char HF_DIRECTORY2[20] = {"/HORIZON/FWF"};
const char VF_DIRECTORY1[20] = {"/VISION/FW"};
const char VF_DIRECTORY2[20] = {"/VISION/FWF"};
const char BT_DIRECTORY1[20] = {"/BT/FW"}; // WWW
const char FILE_NAME[20] =  {"/EFM.bin"};
const char BTfile_NAME[20] =  {"/BT.MVA"}; // WWW
const char *pDirs[TOTAL_DIRS] = {
    ROOT_DIRECTORY,
    LS_DIRECTORY1,
    LS_DIRECTORY2,
    AFG_DIRECTORY1,
    AFG_DIRECTORY2,
    HF_DIRECTORY1,
    HF_DIRECTORY2,
    VF_DIRECTORY1,
    VF_DIRECTORY2,
    BT_DIRECTORY1, // WWW
};

__no_init static union
{
    struct
    {
        UINT16 key_enter :1;
        UINT16 key_back:1;
    }fields;
    UINT16 event;
}USBManager_event;

//__no_init static unsigned char USB_Connect;
static unsigned char g_ucSectorBuf[512];
/* USB related data */
STATIC_UBUF(tmpBuf, 1024);
/* File system */
FATFS Fatfs;
FIL Fileinfo;
__no_init volatile USB_STATE g_eState;
__no_init static FRAME_STAGE gs_MainStateBuf;
int connectionResult;

extern void USB_Init_End(void);
char firmwarePathFilename[30];
void USBManager_Initial_IO(void)
{
    
    USBH_Init_TypeDef is = USBH_INIT_DEFAULT;
//    USB_Connect = 0;
    /* Initialize USB HOST stack */
    USBH_Init( &is );
    USB_Init_End();
}

void USBManager_Initial_Data(void)
{
    g_eState = STATE_NO_DEVICE;
    memset(firmwarePathFilename,0,30*sizeof(char));
    USBManager_event.event = 0;
    gs_MainStateBuf = FRAME_STAGE_START;
}

void USBManager_Set_VirtualKey(UINT16 vk)
{
    gs_MainStateBuf = (FRAME_STAGE)(vk & 0xFF00);

	UINT16 TempKey = (UINT8)(vk&0x00FF);
	if(TempKey == VK_ENTER ||VK_ENTER_CHANGEDIS== TempKey)
		USBManager_event.fields.key_enter = 1;
#ifdef _ALLOW_BACK_FEATURE_
	else if(TempKey == VK_BACK)
		USBManager_event.fields.key_back = 1;
#endif
}

USB_EVENTS USBManager_Process(void)
{
    USB_EVENTS g_USB_Events = EVT_USB_NONE;
    FRESULT FileResult;
    
    if(gs_MainStateBuf==FRAME_STATE_USBMAN ||gs_MainStateBuf == FRAME_STAGE_USER\
        || gs_MainStateBuf == FRAME_STAGE_PRGSETUP) 
    {
        switch(g_eState)
        {
            case STATE_NO_DEVICE:
            
            if(!USBH_DeviceConnected())
            {
                g_eState = STATE_NO_DEVICE;
//                USB_Connect = 0;  
                break;
            }
            connectionResult = USBH_WaitForDeviceConnectionB(tmpBuf, 1);
            if (connectionResult == USB_STATUS_OK)
            {
                
                if (MSDH_Init(tmpBuf, sizeof(tmpBuf)))
                {
                    /* Initialize filesystem */
                    FileResult = f_mount( 0, &Fatfs );
                    if (FileResult  == FR_OK)
                    {
                        g_eState = STATE_DEVICE_ENUM;
//                        USB_Connect = 1;
                        g_USB_Events = EVT_USB_CONNECTED; 
                    }
                }
            }
            
            break;
            case STATE_DEVICE_ENUM:
            {
                FileResult = FR_NO_FILE;
                UINT8 isFileFound = 0;
                for(char i = 0; i < TOTAL_DIRS; i ++)
                {   
                    //Seek all the possible directories to find EFM.bin file.
                    memset(firmwarePathFilename,'\0',30*sizeof(char));
                    strcpy(firmwarePathFilename,pDirs[i]);
                    if('B'==firmwarePathFilename[1])  // WWW
                      strcat(firmwarePathFilename,BTfile_NAME);// WWW
                    else  // WWW
                    strcat(firmwarePathFilename,FILE_NAME);
                    FileResult = f_open(&Fileinfo, firmwarePathFilename, FA_READ | FA_OPEN_EXISTING);
                    if(FR_OK == FileResult)
                    {
                        f_close(&Fileinfo);
                        isFileFound = 1;
                        if('B'==firmwarePathFilename[1])  // WWW
                        {
                            g_eState = STATE_FILE_DETECTED;  //WWW
                            g_USB_Events = EVT_BTUPDATEFILE_DETECTED;  //WWW
                        }
                        else
                            g_eState = STATE_DEVICE_READY;
                        break;
                    }
                }
                if(isFileFound == 0)
                {
                    g_USB_Events = EVT_FILE_NOT_EXIST;
                    g_eState = STATE_FILE_ERROR;
                }
            }
            break;
            case STATE_DEVICE_READY:
            {
                if(!USBH_DeviceConnected())
                {
                    g_eState = STATE_NO_DEVICE;
//                  USB_Connect = 0;  
                    g_USB_Events = EVT_USB_REMOVED;
                }
                else
                {
                    /* BT Update data to read  //WWW */
                    if('B'==firmwarePathFilename[1])
                    {   
                 
                        UsbBttoRead();
                        if(1==OverRead)
                        {
                            OverRead = 0;
                        }
                        else
                        {
                            g_eState = STATE_DEVICE_READY; 
                            break;
     //              g_eState = STATE_FILE_ERROR;
     //              g_USB_Events = EVT_FILE_BT_UPDATE_READFAILED;
                        }
                    }
                    else
                    {
                        UINT Readsize;
                        FileResult = FR_NO_FILE;
                        FileResult = f_open(&Fileinfo,firmwarePathFilename,FA_READ | FA_OPEN_EXISTING);
                        if(Fileinfo.fsize == 0 || Fileinfo.fsize > FLASH_END_ADDRESS+1)
                        {
                            g_eState = STATE_FILE_ERROR;
                            g_USB_Events = EVT_FILE_NOT_EXIST;
                            break;
                        }
                        char ucSum = 0;
                        unsigned int uDataWant = 0;
                        char ucChecksum[4];
                        while(FR_OK == f_read(&Fileinfo,g_ucSectorBuf,512,&Readsize))
                        {
                            if(uDataWant != 496) 
                            {
                                for(unsigned int i = 0; i<512; i++)
                                {
                                    ucSum += g_ucSectorBuf[i];
                                }
                            }
                            if(uDataWant == 512)   //whole flash area ends 0x3ffff
                            {
                                if(ucSum != ucChecksum[0])
                                {
                                    g_eState = STATE_FILE_ERROR;
                                    g_USB_Events = EVT_FILE_CRC_ERROR;
                                    
                                    
                                }
                                g_eState = STATE_FILE_DETECTED;
                                g_USB_Events = EVT_FILE_DETECTED;
                                
                                break;
                            }
                            if(uDataWant == 496)  //Application ends 0x3e000
                            {
                                for(unsigned int i = 0; i<512; i++)	
                                {
                                    if(i>= 48 && i<=55)
                                    {
                                        ;
                                    }else{
                                        ucSum += g_ucSectorBuf[i];
                                    }
                                }
                                for(unsigned int i = 0; i<4; i++)	ucChecksum[i] = g_ucSectorBuf[i+32+8+8];
                            }
                            uDataWant++;
                        }
                    }
                }
            }
            break;
            case STATE_FILE_DETECTED:
            if(!USBH_DeviceConnected())
            {
                g_eState = STATE_NO_DEVICE;
//                USB_Connect = 0;  
                g_USB_Events = EVT_USB_REMOVED;
            } 
            else
            {
                if(USBManager_event.fields.key_enter ==  1)
                {
                    FileResult = FR_NO_FILE;
                    FileResult = f_open(&Fileinfo,firmwarePathFilename,FA_READ | FA_OPEN_EXISTING);
                    if(Fileinfo.fsize == 0)
                    {
                        g_eState = STATE_FILE_ERROR;
                        g_USB_Events = EVT_FILE_NOT_EXIST;
                        break;
                    }
                    //WWW BT UPDATE
                    if('B'==firmwarePathFilename[1])  
                    {
                        g_eState = STATE_DEVICE_READY;
                        g_USB_Events = EVT_FILE_BT_UPDATE_CONFIRMED; 
                        //Bluetooth_Set_VirtualKey(VK_BT_UPDATE);
                    }
                    else  
                    {
                        SystemConfig_Set_UpdateFlag(USB_UPDATE_AVAILABLE);
                        g_USB_Events = EVT_FILE_UPDATE_CONFIRMED;
                    }
                }
#ifdef _ALLOW_BACK_FEATURE_
                else if(USBManager_event.fields.key_back ==  1)
                {
                    g_USB_Events = EVT_FILE_UPDATE_CANCELED;
                    g_eState = STATE_UNKNOWN_DEVICE;
                }
#endif
            }
            break;
            case STATE_UNKNOWN_DEVICE:
            case STATE_POWER_FAULT:
            case STATE_FILE_ERROR:
            default: 
            if(!USBH_DeviceConnected())
            {
                g_eState = STATE_NO_DEVICE;
//                USB_Connect = 0;  
            } 
            g_USB_Events = EVT_USB_NONE;
            break;
        }
    }
    USBManager_event.event = 0;
    return g_USB_Events;
}

USB_STATE USB_Get_State(void)
{
    return(g_eState);
}

//WWW
void UsbBttoRead()
{ 
   extern UINT ToRead ;
   extern UINT ToWrite ;
   extern UINT OverRead ;
   ULONG count = (&Fileinfo)->fsize / 256 ;
   UINT number = (&Fileinfo)->fsize % 256 ;
   unsigned char *p =g_BtUpdateFileDate;
   static ULONG i =0;
   UINT j =number ;
   UINT Readsize;
   
   BtUpdateFileSize = (&Fileinfo)->fsize;
   BtLastSize = number;
   //do{  
  if (0 == number )
  {   
      while( ToRead && (FR_OK == f_read(&Fileinfo,g_BtUpdateFileDate,256,&Readsize)))
      {
          if(i<(count-1))
          {
           ToRead = 0 ;
           ToWrite = 1 ;
           i++;
          }
          else
          {
           ToRead = 0 ;
           ToWrite = 1 ;
           i++;
           OverRead = 1 ;
           break;
          }
      }
  }
  else
  {
      while( ToRead && (FR_OK == f_read(&Fileinfo,g_BtUpdateFileDate,256,&Readsize)))
      {
         if(i<count)
         {
           ToRead = 0 ;
           ToWrite = 1 ;
           i++;
         }
         else
         {
            ToRead = 0 ;
            ToWrite = 1 ;
            i++;
            for( ; j <256;j++)
            {
              *(p+j) = 0 ;
            }
            OverRead = 1 ;
            break;
         }
      }
   }
   //}while(OverRead!=1);
}

