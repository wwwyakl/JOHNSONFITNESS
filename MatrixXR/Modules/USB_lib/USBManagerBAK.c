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



//List all possible directory
typedef enum tag_JohnsonDirs
{
    ROOT_DIR = 0,
    MATRIX_DIR1,
    MATRIX_DIR2,
    AFG_DIR1,
    AFG_DIR2,
    HF_DIR1,
    HF_DIR2,
    VF_DIR1,
    VF_DIR2,
    TOTAL_DIRS
}JOHNSON_DIRS;
const char ROOT_DIRECTORY[20] = {"."};
const char MATRIX_DIRECTORY1[20] = {"/MATRIX/FW"};
const char MATRIX_DIRECTORY2[20] = {"/MATRIX/FWF"};
const char AFG_DIRECTORY1[20] = {"/AFG/FW"};
const char AFG_DIRECTORY2[20] = {"/AFG/FWF"};
const char HF_DIRECTORY1[20] = {"/HORIZON/FW"};
const char HF_DIRECTORY2[20] = {"/HORIZON/FWF"};
const char VF_DIRECTORY1[20] = {"/VISION/FW"};
const char VF_DIRECTORY2[20] = {"/VISION/FWF"};
const char FILE_NAME[30] =  {".bin"};
const char EFM_FILE_NAME[30] =  {"/EFM.bin"};
const char CFG_FILE_NAME[20] =  {"/update.cfg"};
const char *pDirs[TOTAL_DIRS] = {
    ROOT_DIRECTORY,
    MATRIX_DIRECTORY1,
    MATRIX_DIRECTORY2,
    AFG_DIRECTORY1,
    AFG_DIRECTORY2,
    HF_DIRECTORY1,
    HF_DIRECTORY2,
    VF_DIRECTORY1,
    VF_DIRECTORY2,
};

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


__no_init static union
{
    struct
    {
        UINT16 key_enter :1;
        UINT16 key_back:1;
    }fields;
    UINT16 event;
}USBManager_event;

__no_init static unsigned char USB_Connect;
static unsigned char g_ucSectorBuf[512];
/* USB related data */
STATIC_UBUF(tmpBuf, 1024);
/* File system */
FATFS Fatfs;
FIL Fileinfo;
__no_init volatile USB_STATE g_eState;
int connectionResult;

extern void USB_Init_End(void);
char firmwarePathFilename[50];

void USBManager_Initial_IO(void)
{
	HAL_Set_IO_Output(USB_CRTL_PORT,USB_CRTL_PIN);
	HAL_Set_IO_High(USB_CRTL_PORT,USB_CRTL_PIN);	
	USBH_Init_TypeDef is = USBH_INIT_DEFAULT;
	USB_Connect = 0;
	/* Initialize USB HOST stack */
	USBH_Init( &is );
	USB_Init_End();
}

void USBManager_Initial_Data(void)
{
    g_eState = STATE_NO_DEVICE;
    memset(firmwarePathFilename,0,50*sizeof(char));
    USBManager_event.event = 0;
}

VR_KEY USBManager_Set_VirtualKey(VR_KEY vk)
{
	VR_KEY ret = vk;
	if( g_eState == STATE_FILE_DETECTED )
	{
		if(ret == VK_ENTER ||VK_ENTER_CHANGEDIS== ret)
		{
			USBManager_event.fields.key_enter = 1;
		}
 #ifdef _ALLOW_BACK_FEATURE_
	else if(ret == VK_BACK)
		{
			USBManager_event.fields.key_back = 1;
		}
#endif
		ret = VK_PROCESSED;
	}
	
	return ret;
 
}

USB_EVENTS USBManager_Process(ReturnEventHandler flag)
{
	static USB_EVENTS g_PreEvent = EVT_USB_NONE;
	
	USB_EVENTS g_USB_Events = EVT_USB_NONE;
	if(flag < RET_PROCESS_DISPLAY)
		return g_USB_Events;
	if(g_PreEvent!=EVT_USB_NONE)
	{
		flag = RET_PROCESS_DISPLAY;
	}
	if(g_eState == STATE_FILE_DETECTED )
	{
		g_USB_Events = EVT_USB_PROCESSED;
	}

	
    FRESULT FileResult;
    UINT *stream;
    char list[50];
    char list_backup[50];    

        switch(g_eState)
        {
        case STATE_NO_DEVICE:
             
             if(!USBH_DeviceConnected())
             {
                  g_eState = STATE_NO_DEVICE;
                  USB_Connect = 0;  
                  break;
             }
             connectionResult = USBH_WaitForDeviceConnectionB(tmpBuf, 1);
             if (connectionResult == USB_STATUS_OK)
             {
#ifdef WDT_ENABLE
                HAL_Clear_WatchDog();
                HAL_Disable_WatchDog();
#endif
                  if (MSDH_Init(tmpBuf, sizeof(tmpBuf)))
                  {
                       /* Initialize filesystem */
                       FileResult = f_mount( 0, &Fatfs );
                       if (FileResult  == FR_OK)
                       {
                            g_eState = STATE_DEVICE_ENUM;
                            USB_Connect = 1;
                            g_USB_Events = EVT_USB_CONNECTED; 
                       }
                  }
#ifdef WDT_ENABLE
                HAL_Enable_WatchDog();
                HAL_Clear_WatchDog();
#endif
             }
             
             break;
        case STATE_DEVICE_ENUM:
             {
                  FileResult = FR_NO_FILE;
                  UINT8 isFileFound = 0;
                  UINT8  l=0;
                  //step 1 open update.cfg read config and create xxx.bin
                  for(char i = 0; i < TOTAL_DIRS; i ++)
                  {   
                       //Seek all the possible directories to find EFM.bin file.
                       memset(firmwarePathFilename,'\0',50*sizeof(char));
                       strcpy(firmwarePathFilename,pDirs[i]);
                       strcat(firmwarePathFilename,CFG_FILE_NAME/*FILE_NAME*/);//---update.cfg---
                       FileResult = f_open(&Fileinfo, firmwarePathFilename, FA_READ | FA_OPEN_EXISTING);
                       
                       if(FR_OK == FileResult)
                       {   
                            memset( list , '\0', 30);
                            memset( list_backup , '\0', 30);
                            f_read(&Fileinfo,list,30,stream);
                            
                            list_backup[0]=0x2f;  //  strcpy(FILE_NAME,pDirs[i]);
                            int j=1;
                            int k=2;
                            for(unsigned int i = 0; i<30; i++)	
                            {
                                 if(list[i] == 0x0d){
                                 }
                                 else if(list[i] == 0x0a){
                                      if(k ==2)
                                      {
                                           list_backup[j] =0x20;//
                                           j++;        k--;
                                      }
                                      else if(k ==1)
                                      {
                                           list_backup[j] =0x2e;//
                                           j++;        k--;
                                      }
                                 }
                                 else{
                                      list_backup[j] = list[i];
                                      j++;
                                 }
                            }
                            f_close(&Fileinfo);
                            strcat(list_backup,FILE_NAME);
                            l=1;
                       }
#ifdef WDT_ENABLE
                       HAL_Clear_WatchDog();
#endif
                       f_close(&Fileinfo);
                  }
                  
                  //step 2 use config inf open xxx.bin
                  for(char i = 0; i < TOTAL_DIRS; i ++)
                  {   
                       //Seek all the possible directories to find EFM.bin file.
                       memset(firmwarePathFilename,'\0',50*sizeof(char));
                       strcpy(firmwarePathFilename,pDirs[i]);
                       if(l==1)
                       strcat(firmwarePathFilename,list_backup);
                       else
                       strcat(firmwarePathFilename,EFM_FILE_NAME);
 
                       FileResult = f_open(&Fileinfo, firmwarePathFilename, FA_READ | FA_OPEN_EXISTING);
                       
                       if(FR_OK == FileResult)
                       {   
                            f_close(&Fileinfo);
                            isFileFound = 1;
                            g_eState = STATE_DEVICE_READY;
                             break;
                       }
#ifdef WDT_ENABLE
                    HAL_Clear_WatchDog();
#endif
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
#ifdef WDT_ENABLE
                      HAL_Clear_WatchDog();
#endif
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
             break;
        case STATE_FILE_DETECTED:
             if(!USBH_DeviceConnected())
             {
                  g_eState = STATE_NO_DEVICE;
                  USB_Connect = 0;  
                  g_USB_Events = EVT_USB_REMOVED;
             } 
             else
             {
                  if(USBManager_event.fields.key_enter ==  1)
                  {
                       SystemConfig_Set_UpdateFlag(USB_UPDATE_AVAILABLE);
                       g_USB_Events = EVT_FILE_UPDATE_CONFIRMED;
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
                  USB_Connect = 0;  
             } 
             g_USB_Events = EVT_USB_NONE;
             break;
        }
				
    USBManager_event.event = 0;
	if(g_USB_Events >EVT_USB_PROCESSED)	
	{
		g_PreEvent = g_USB_Events;
	}
    return g_USB_Events;
}

