#include "USBManager.h"
#include "SystemConfig.h"

#include "em_usb.h"
#include "ff.h"
#include "msdh.h"
#include "usbconfig.h"
#include "HAL_Lib.h"
#include  "EEPRom_Language.h"
//These parameters must match the icf file configuration. Do NOT touch.
#define FLASH_END_ADDRESS               0x0003ffff
UINT8 isFileFound = 0;
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
    LANGUAGE_DIR1,
    LANGUAGE_DIR2,
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
const char LANGUAGE_DIRECTORY1[20] = {"/LANGUAGE/FW"};
const char LANGUAGE_DIRECTORY2[20] = {"/LANGUAGE/FWF"};
const char FILE_NAME[30] =  {".bin"};
const char EFM_FILE_NAME[30] =  {"/EFM.bin"};
const char CFG_FILE_NAME[20] =  {"/update.cfg"};
const char LANGUAGE_FILE_NAME[30] =  {"/LANGUAGE.bin"};
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
    LANGUAGE_DIRECTORY1,
    LANGUAGE_DIRECTORY2
};

typedef enum
{
    STATE_NO_DEVICE = 0,
    STATE_DEVICE_ENUM,
    STATE_DEVICE_MENU,
    STATE_DEVICE_LANGUAGE,
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
UCHAR Key_enter_statu = 0;
__no_init static unsigned char USB_Connect;
__no_init static unsigned char USB_Try_ConnectCNT;
UCHAR USB_Try_ConnectStartFlag=0;
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
char languagePathFilename[30];
char efmPathFilename[30];

void USBManager_Initial_IO(void)
{
	HAL_Set_IO_Output(USB_CRTL_PORT,USB_CRTL_PIN);
	HAL_Set_IO_High(USB_CRTL_PORT,USB_CRTL_PIN);	
	USBH_Init_TypeDef is = USBH_INIT_DEFAULT;
	USB_Connect = 0;
    USB_Try_ConnectCNT = 0;
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
                    g_eState = STATE_DEVICE_MENU;
                    USB_Connect = 1;
                    USB_Try_ConnectStartFlag = 0;
                    USB_Try_ConnectCNT = 0;
                    g_USB_Events = EVT_USB_CONNECTED; 
                }
            }
#ifdef WDT_ENABLE
            HAL_Enable_WatchDog();
            HAL_Clear_WatchDog();
#endif
        }
        
        break;
        case STATE_DEVICE_MENU:
        {
            FileResult = FR_NO_FILE;
            if(!USBH_DeviceConnected())
            {
                g_eState = STATE_NO_DEVICE;
                USB_Connect = 0;  
                g_USB_Events = EVT_USB_REMOVED;
                isFileFound = 0;
            } 
            //step 2 use config inf open xxx.bin
            for(char i = 0; i < TOTAL_DIRS; i ++)
            {   
                //Seek all the possible directories to find EFM.bin file.
                memset(firmwarePathFilename,'\0',50*sizeof(char));
                strcpy(firmwarePathFilename,pDirs[i]);
                
                strcat(firmwarePathFilename,LANGUAGE_FILE_NAME);
                
                FileResult = f_open(&Fileinfo, firmwarePathFilename, FA_READ | FA_OPEN_EXISTING);
                
                if(FR_OK == FileResult)
                {   
                    f_close(&Fileinfo);
                    isFileFound = isFileFound + 0x01;
                    isFileFound |= 0x10;
                    strcpy(languagePathFilename,firmwarePathFilename);
                    break;
                }
#ifdef WDT_ENABLE
                HAL_Clear_WatchDog();
#endif
            }
            for(char i = 0; i < TOTAL_DIRS; i ++)
            {   
                //Seek all the possible directories to find EFM.bin file.
                memset(firmwarePathFilename,'\0',50*sizeof(char));
                strcpy(firmwarePathFilename,pDirs[i]);
                
                strcat(firmwarePathFilename,EFM_FILE_NAME);
                
                FileResult = f_open(&Fileinfo, firmwarePathFilename, FA_READ | FA_OPEN_EXISTING);
                
                if(FR_OK == FileResult)
                {   
                    f_close(&Fileinfo);
                    isFileFound = isFileFound + 0x01;
                    isFileFound |= 0x20;
                    strcpy(efmPathFilename,firmwarePathFilename);
//                    g_eState = STATE_FILE_DETECTED;  
//                    g_USB_Events = EVT_FILE_LANGUAGE_UPDATE;  

                    break;
                }
#ifdef WDT_ENABLE
                HAL_Clear_WatchDog();
#endif
            }
            g_eState = STATE_DEVICE_LANGUAGE;
            if(isFileFound == 0)
            {
                g_USB_Events = EVT_FILE_NOT_EXIST;
                g_eState = STATE_FILE_ERROR;
            }
            else if((isFileFound&0x0f) >= 0x02) 
            {
                Key_enter_statu = 1;
            }
            else if((isFileFound&0x0f) < 0x02)
            {
                if((isFileFound&0xf0) == 0x10);
                if((isFileFound&0xf0) == 0x20);
            }
        }
        break;
        case STATE_DEVICE_LANGUAGE:
        {
            FileResult = FR_NO_FILE;
            memset(firmwarePathFilename,'\0',50*sizeof(char));
            strcpy(firmwarePathFilename,languagePathFilename);
            FileResult = f_open(&Fileinfo, firmwarePathFilename, FA_READ | FA_OPEN_EXISTING);
            if(FR_OK == FileResult)
            {   
                f_close(&Fileinfo);
                g_eState = STATE_FILE_DETECTED;  
            }
            else
            {
                f_close(&Fileinfo);
                g_eState = STATE_DEVICE_ENUM;  
            }
//            for(char i = 0; i < TOTAL_DIRS; i ++)
//            {   
//                //Seek all the possible directories to find EFM.bin file.
//                memset(firmwarePathFilename,'\0',50*sizeof(char));
//                strcpy(firmwarePathFilename,pDirs[i]);
//                
//                strcat(firmwarePathFilename,LANGUAGE_FILE_NAME);
//                
//                FileResult = f_open(&Fileinfo, firmwarePathFilename, FA_READ | FA_OPEN_EXISTING);
//                
//                if(FR_OK == FileResult)
//                {   
//                    f_close(&Fileinfo);
//                    g_eState = STATE_FILE_DETECTED;  
////                    g_USB_Events = EVT_FILE_LANGUAGE_UPDATE;  
//                    break;
//                }
//#ifdef WDT_ENABLE
//                HAL_Clear_WatchDog();
//#endif
//            }
            if((isFileFound&0x0f) >= 0x02) 
            {
                
            }
            else if((isFileFound&0x0f) < 0x02)
            {
                if((isFileFound&0xf0) == 0x10)
                    g_USB_Events = EVT_FILE_DETECTED;
                if((isFileFound&0xf0) == 0x20)
                    g_USB_Events = EVT_FILE_DETECTED;
            }
        }
        break;
        case STATE_DEVICE_ENUM:
        {
            UINT8  l=0;
            if(!USBH_DeviceConnected())
            {
                g_eState = STATE_NO_DEVICE;
                USB_Connect = 0;  
                g_USB_Events = EVT_USB_REMOVED;
                isFileFound = 0;
            } 
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
                {
                    memset(firmwarePathFilename,'\0',50*sizeof(char));
                    strcpy(firmwarePathFilename,efmPathFilename);
//                    strcat(firmwarePathFilename,EFM_FILE_NAME);
                }
                FileResult = f_open(&Fileinfo, firmwarePathFilename, FA_READ | FA_OPEN_EXISTING);
                
                if(FR_OK == FileResult)
                {   
                    f_close(&Fileinfo);
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
            isFileFound = 0;
        } 
        else
        {
            if((isFileFound&0x0f) >= 0x02) 
            {
                if(Key_enter_statu == 1)
                {
                    if(Lang_WriteLanguageToEEPRom(_Lang_UcbMachineType_,firmwarePathFilename) == _Lang_Success_)
                    {
                        Key_enter_statu = 0;
    //                    g_USB_Events = EVT_FILE_UPDATE_CONFIRMED;
                        g_eState = STATE_DEVICE_ENUM;
                        isFileFound -= 1;
                        isFileFound &= 0xef;
                    }
                }            
            }
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
//                if('L'==firmwarePathFilename[1])  
//                {
//                    if(Key_enter_statu == 0)
//                    {
//                        Key_enter_statu = 1;
//                        
//                        UINT8 boot = SystemConfig_Get_Char(FIRST_BOOT_FLAG_UCHAR);
//                        if(boot==0xA5)
//                        {
//                            boot =0xFF;
//                            SystemConfigProcess(SVK_TOGGLE_FIRSTBOOT,&boot,sizeof(boot));
//                            SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
//                        }
//                        g_USB_Events = EVT_FILE_LANGUAGE_UPDATEING;    
//                    }
//                }
                if((isFileFound&0xf0)==0x20)
                {
                    SystemConfig_Set_UpdateFlag(USB_UPDATE_AVAILABLE);
                    g_USB_Events = EVT_FILE_UPDATE_CONFIRMED;
                }
                else if((isFileFound&0xf0)==0x10)//更新LANGUAGE提示信息
                {
                    if(Lang_WriteLanguageToEEPRom(_Lang_UcbMachineType_,firmwarePathFilename) == _Lang_Success_)
                    {
                        Key_enter_statu = 0;
    //                    g_USB_Events = EVT_FILE_UPDATE_CONFIRMED;
                        g_eState = STATE_DEVICE_ENUM;
                        isFileFound -= 1;
                        isFileFound &= 0xef;
                        UINT8 boot = SystemConfig_Get_Char(FIRST_BOOT_FLAG_UCHAR);
                        if(boot==0xA5)
                        {
                            boot =0xFF;
                            SystemConfigProcess(SVK_TOGGLE_FIRSTBOOT,&boot,sizeof(boot));
                            SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
                        }
                        g_USB_Events = EVT_FILE_UPDATE_CONFIRMED;
                    }  
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
void USBManager_Updata_Language(void)
{
    ReturnEventHandler EventRet=RET_REFRESH_DISPLAY;
    USB_Try_ConnectStartFlag = 1;
     while(1)
     {
         #ifdef WDT_ENABLE
            HAL_Clear_WatchDog();
         #endif
         USBManager_Process(EventRet);
         LCB_Process();
//         if(USBManager_Get_Connect() != 1)
         //if(g_eState == STATE_NO_DEVICE)
           //  break;
         if(USB_Try_ConnectCNT >=30)
         {
             USB_Try_ConnectCNT = 0;
             break;
         }
         if(USB_Connect != 1)
         {
             continue;
         }
         
         
         #ifdef WDT_ENABLE
            HAL_Clear_WatchDog();
         #endif             
         //Lang_WriteLanguageToEEPRom(_Lang_UcbMachineType_,"/Matrix/FWF/Language.bin");
         Lang_WriteLanguageToEEPRom(_Lang_UcbMachineType_,"/MATRIX/FW/LANGUAGE.bin");
         Lang_WriteLanguageToEEPRom(_Lang_UcbMachineType_,"/MATRIX/FWF/LANGUAGE.bin");
         break;
         
     }
}
void USBManager_1ms_INT(void)
{
    static UINT8 w_100ms_Time;
    w_100ms_Time++;
    if(w_100ms_Time>100)
    {
        w_100ms_Time = 0;   
        if(USB_Try_ConnectStartFlag == 1)
            USB_Try_ConnectCNT ++;
        else
            USB_Try_ConnectCNT = 0;
    }
}
void USBManager_Regist_Function(void)
{
    HAL_SystemTicker_Alloc_ISR(USBManager_1ms_INT);   
}
