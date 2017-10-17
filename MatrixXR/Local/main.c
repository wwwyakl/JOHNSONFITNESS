/*IAR Embedded Workbench for ARM, Evaluation version 6.30*/

#include "Main.h"
#include "Table_List.h"
#include "WattsTable.h"
#include "FACTORYDEFAULT.H"
#include "Strings.h"
#include  "EEPRom_Language.h"

void Main_Init_Model_Data(void);
static void Main_PowerOn_Initial(void);
static void Main_Initial_IO(void);
UINT32  TestTimer;
UINT8 FlashHaveLanguageFlag = 0;
extern unsigned short __checksum;
__root const unsigned char DeviceModel[32] @ ".ModelAddr" = THISMODEL;
__root const unsigned char DeviceSoftwareMVersion[8] @ ".MVerAddr" = C_S_VER;
__root const unsigned char DeviceSoftwareSVersion[8] @ ".SVerAddr" = C_S_NUM;
__root const unsigned char DeviceUpdateRegion[4] @ ".UpdateStep" = {0,0,0,0};

extern __no_init MAIN_STAGE Main_Stage;

#ifdef OUTPUT_DEBUG
UINT8 DBGPrintBuf[64];
#endif   

 void main(void)
 {
     if (__checksum==0) __checksum=1;
     
     HAL_Disable_MasterInterrupt();
     Main_Initial_IO(); 
     if(BOR_Get_Passcode() != C_PASSWORD)  
     {
         Main_PowerOn_Initial();
     }
     RMU_ResetCauseClear();
     HAL_BOR_Enable_ExtPower(Power_Channel_A,true);
     HAL_Enable_MasterInterrupt();	
	Digital_WIFI_Hardware_Reset();
	
    
	#ifdef  __WIFI_FUNC__
   		Digital_Initial_Data_wifi();
#endif

     USBManager_Initial_IO();
     USBManager_Initial_Data();  
		 
#ifdef WDT_ENABLE
     HAL_Clear_WatchDog();
#endif
//     while(1)
//     {
//         #ifdef WDT_ENABLE
//            HAL_Clear_WatchDog();
//         #endif
//         EventRet = USBManager_Process(EventRet);
//         if(USBManager_Get_Connect() != 1)
//         {
//             continue;
//         }
//         
//         
//         #ifdef WDT_ENABLE
//            HAL_Clear_WatchDog();
//         #endif             
//         Lang_WriteLanguageToEEPRom(_Lang_UcbMachineType_,"/MATRIX/FW/LANGUAGE.bin");
//         Lang_WriteLanguageToEEPRom(_Lang_UcbMachineType_,"/MATRIX/FWF/LANGUAGE.bin");
//         break;
//         
//     }
     
     USBManager_Updata_Language();
     FlashHaveLanguageFlag = Language_EEPROMCheck("MatrixXR");
    Language_LanguageChecking(String_Get_LanguageIndex());
#ifdef FUNCTION_AUTO_MODEL    
     if(SystemConfig_Get_Char(IS_MACHINE_CONFIGED_UCHAR)!=0xAA)
//     if(SystemConfig_Get_Char(IS_MACHINE_CONFIGED_UCHAR)!=LCB_Get_MCBType()) //power on check LCB type
     {
         nCurrentPage = PAGE_AUTO_MODELS;
     }

     while( nCurrentPage == PAGE_AUTO_MODELS)
     {
         Model_Set_VirtualKey(Event_VK_Converter(KeyBoard_Get_Code(0))); 
         LCB_Process();
         Auto_Model_Event_Handler(Model_Process()) ;
         Display_Updata_Data(true);
#ifdef WDT_ENABLE
         HAL_Clear_WatchDog();
#endif
     }
#endif
     
     User_Set_VirtualKey(VK_INITIAL); 
     ReturnEventHandler EventRet=RET_REFRESH_DISPLAY;	
	 if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==CLIMB_MILL)
     {
         LCB_Send_Cmd(DS_SET_STATUS,0xA1);	
         LCB_Send_Cmd(DS_SET_STATUS,0xB1);
         LCB_Set_DriverType(RETAIL_CLIMBMILL);
     }
     while(true)
     {  
        Page_Timer_Process();
		
         //KEY_EVENTS
       	Key_Events_Handler(KeyBoard_Get_Code(0));
		
		switch(Main_Stage)
		{
			case STAGE_USER:
			{
				EventRet = Event_UTL_BurnTest_Process(BurnTest_Process(EventRet),EventRet);
				EventRet = Event_UTL_ERP_Process(ERP_Process(EventRet),EventRet);
				EventRet = Event_UTL_Error_Code_Process(ERR_Process(EventRet),EventRet);
				EventRet = Events_USB_Handler(USBManager_Process(EventRet),EventRet);
#ifdef  __WIFI_FUNC__
		        EventRet = WIFI_Events_Handler(Digital_Command_Proce_wifi(EventRet),EventRet);
#endif
#ifdef RF_MODULE_EXIST_   
       			EventRet = Events_RF_Handler(RF_AP_Process(EventRet),EventRet);
#endif
				EventRet = Event_UTL_BlueBelt_Process(LubeBelt_Process(EventRet),EventRet);
				EventRet = User_Events_Handler(User_Process(EventRet),EventRet);
			}
			break;
			case STAGE_PRG_SETUP:
			{
				EventRet = Event_UTL_BurnTest_Process(BurnTest_Process(EventRet),EventRet);
				EventRet = Event_UTL_ERP_Process(ERP_Process(EventRet),EventRet);
				EventRet = Event_UTL_Error_Code_Process(ERR_Process(EventRet),EventRet);
				EventRet = Events_USB_Handler(USBManager_Process(EventRet),EventRet);
#ifdef  __WIFI_FUNC__
		        EventRet = WIFI_Events_Handler(Digital_Command_Proce_wifi(EventRet),EventRet);
#endif
#ifdef RF_MODULE_EXIST_   
       			EventRet = Events_RF_Handler(RF_AP_Process(EventRet),EventRet);
#endif	
				EventRet = Program_Events_Handler(Program_Process(EventRet),EventRet);
			}
			break;

			case STAGE_PRG_RUN:
			{
				EventRet = Event_UTL_Error_Code_Process(ERR_Process(EventRet),EventRet);
        
#ifdef RF_MODULE_EXIST_   
       			EventRet = Events_RF_Handler(RF_AP_Process(EventRet),EventRet);
#endif	                
				EventRet = Program_Events_Handler(Program_Process(EventRet),EventRet);
#ifdef  __WIFI_FUNC__
		        EventRet = WIFI_Events_Handler(Digital_Command_Proce_wifi(EventRet),EventRet);
#endif       
				EventRet = Event_UTL_HR_Blink_Process(NULL,EventRet);
			}
			break;

			case STAGE_ENG_MODE:
			{
				//EventRet = Event_UTL_Error_Code_Process(ERR_Process(EventRet),EventRet);
				EventRet = Events_ENG_Handler(Engmode_Process(EventRet),EventRet);
#ifdef  __WIFI_FUNC__
		        EventRet = WIFI_Events_Handler(Digital_Command_Proce_wifi(EventRet),EventRet);
#endif
				EventRet = Event_UTL_ERP_Process(ERP_Process(EventRet),EventRet);
			}
			break;
		}
			 
         //DISPLAY_UPDATE
        if(EventRet != RET_REFRESH_DISPLAY)	 
		{
			EventRet = RET_PROCESS_DISPLAY;
		}
         if(EventRet >RET_UNPROCESS_UNDISPLAY )
       	{	
			Display_Updata_Data(true);
       	}
         LCB_Process();
#ifdef WDT_ENABLE
         HAL_Clear_WatchDog();
#endif     
    }
}

void Main_BackUp_Flash(void)
{
#ifdef RF_MODULE_EXIST_   
	RF_Set_RF_Enable(0);
#endif
	SPI_Flash_Initial();
	SPI_Flash_Initial_Data();
	for(char i=4;i<8;i++)
	SPI_Flash_BlockErase(i);
	//EEROM_Initial();
	{
		unsigned long FlashAddress=APP_START_ADDRESS;
		unsigned long WriteCount=0;
		uint8_t Buffer1[2048]={0} ;
		do{
#ifdef WDT_ENABLE
			HAL_Clear_WatchDog();
#endif
			HAL_FLASH_Burst_Read(FlashAddress, (UINT8 *)(Buffer1), sizeof(Buffer1));
			if(FlashAddress<0x38000||FlashAddress==0x3e000)
			{
				SPI_Flash_WriteBlock(FlashAddress+_OldBin_BaseAddress_,sizeof(Buffer1),Buffer1);
			}
			FlashAddress += 2048 ;
			WriteCount++;
		}while(FlashAddress<FLASH_END_ADDRESS);
	}
#ifdef RF_MODULE_EXIST_   
	RF_Set_RF_Enable(1);
#endif
}


static void  Main_PowerOn_Initial(void)
{
	UINT8 isFirstBoot = 0;

	isFirstBoot = *((UINT8*)(SystemConfig_Get_FirstBoot_ADDRESS()));
	SystemConfig_Initial(); 
	Main_Initial_Data(TYPE_ALL_INIT);
	HAL_Enable_MasterInterrupt();        
	LCB_Power_On(true);        
	HAL_BOR_Enable_ExtPower(Power_Channel_A,true);
	if(isFirstBoot != 0xA5)
	{
		DisplayString(0,3,0,String_Get_String(STR_INITIAL_ID)); 
		Beep_Set_Beeps(6,10,5);
		while(Beep_Get_OK() == false)
		{
			Display_Updata_Data(true);
			LCB_Process();
#ifdef WDT_ENABLE
			HAL_Clear_WatchDog();
#endif
			Main_BackUp_Flash();
		}
	}
    SPI_Flash_Initial();
//    FlashHaveLanguageFlag = Language_EEPROMCheck("MatrixXR");
//    Language_LanguageChecking(String_Get_LanguageIndex());
    
	Display_Set_All(0);
	char versionBuffer1[20];
	char versionBuffer2[10] = THISMODEL;
    memset(versionBuffer1,'\0',20);
	//memset(versionBuffer2,'\0',10);	
    snprintf(versionBuffer1,8,"%s.%s   ",(const char *)C_S_VER,(const char *)C_S_NUM);
	
	strncpy(versionBuffer1+strlen(versionBuffer1),(const char *)versionBuffer2,20)	;
    DisplayString(0,3,0,(const char *)versionBuffer1); 
    //DisplayString(0,3,0,"Version Lube Belt Demo"); 
	DisplayString(41,0,0,String_Get_String(Get_User_Unit()==STANDARD?STR_STANDARD_ID: STR_METRIC_ID ));
    Beep_Set_Beeps(6,10,10);
    while(Beep_Get_OK() == false)
    { 
        Display_Updata_Data(true);
        LCB_Process();
#ifdef WDT_ENABLE
        HAL_Clear_WatchDog();
#endif    
    }
    BOR_Set_Passcode(C_PASSWORD); 
}

static void Main_Initial_IO(void)
{
	HAL_Config_Clock_Active();
	HAL_Init_ISR();
	BOR_Initial_IO();
	BurnTest_Initial_IO();
	//HAL_FAN_Initial_IO();
	HAL_HR_Initial_IO();
    HAL_SystemTicker_Alloc_ISR(Midware_Heart_1ms_Int);
	Err_Initial_IO();
	Beep_Initial_IO();
	//HAL_AMP_Initial_IO(1);
	//HAL_HT16XX_Initial_IO();
	HAL_LCD_Backlight_Initial_IO();
	HT16CXX_Initial_IO();	
	Keyboard_Initial_IO();
	LCB_Initial_IO();
    Midware_Heart_Initial_IO();
#ifdef  __WIFI_FUNC__
    WiFi_Init_IO();
#endif

#ifdef RF_MODULE_EXIST_   
    RF_AP_Initial_IO(); 
#endif

#ifdef OUTPUT_DEBUG     
    HAL_Monitor_Init_Uart();
#endif
   
	Erp_Regist_Functions();
	Timer_Regist_Functions();
	Show_Regist_Functions();
	Program_Regist_Functions();
	Engmode_Regist_Function();
    Auto_Regist_Function();
    USBManager_Regist_Function();
}


void Main_Init_Model_Data(void)
{
    KeyBoard_Set_KeyholdTable(HOLDTable,KEY_HOLD_NUMBER);
    if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)== TREADMILL)
    {
        KeyBoard_Set_TrueTable(&Table_TM[0][0],8,8);
        KeyBoard_Set_KeyComboTable(TM_COMboTable,5);
    }
    else if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==CLIMB_MILL)
    {
        KeyBoard_Set_TrueTable(&Table_C50[0][0],8,8);
        KeyBoard_Set_KeyComboTable(C50_COMboTable,5);
    }
    else
    {
        KeyBoard_Set_TrueTable(&Table_BK[0][0],8,8);
        KeyBoard_Set_KeyComboTable(EP_COMboTable,5);
    }  
    
    
    
    switch(SystemConfig_Get_Char(DRIVE_TYPE_UINT8))
    {
        case MCB_RETAIL_CLIMBMILL://to do:load C50 program para.
        {
//            Program_Initial_Data(&Program_ECB_EpBike_ParaConfig);
//            Program_Load_WattsTable(&UR30_WattsParameter);
            Program_Initial_Data(&Program_Climbmill_ParaConfig);
            Program_Load_WattsTable(&C50_WattsParameter);
        }
        break;
        case EP_0B_JIS:
        case DIGITAL_ECB:
        case ECB_JIS:
        {
            if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==BIKE)
            {
                Program_Initial_Data(&Program_ECB_EpBike_ParaConfig);
                Program_Load_WattsTable(&UR30_WattsParameter);
            }
            else if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==ASCENT||SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==CLIMB_MILL)
            {
                Program_Initial_Data(&Program_ECB_Ascent_ParaConfig);
                Program_Load_WattsTable(&A30_WattsParameter);
            }
            else if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==ELLIPTICAL)
			{
                Program_Initial_Data(&Program_ECB_EpBike_ParaConfig);
	            Program_Load_WattsTable(&E30_WattsParameter);
			}
        }
        break;
        case ECB_INDUCTOR_BRAKE:
        case BIKE_0C_JIS:
        case INDUCTOR_BRAKE_INCLINE:
        {
            if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==ELLIPTICAL||\
                SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==BIKE)
            {
                Program_Initial_Data(&Program_Induction_EpBike_ParaConfig);
            }
            else if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==ASCENT||SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==CLIMB_MILL)
            {
                Program_Initial_Data(&Program_Induction_Ascent_ParaConfig);
            }
            if(SystemConfig_Get_Char(DRIVE_TYPE_UINT8)==INDUCTOR_BRAKE_INCLINE)
            {
                Program_Load_WattsTable(&InductionBrakeIncline_WattsParameter);
            }
            else
            {
                Program_Load_WattsTable(&Induction_WattsParameter1);
            }
        }
        break;
        default:
        {
            Program_Initial_Data(&Program_TM_ParaConfig);
        }
        break;
    }
    LCB_Initial();
#ifdef FUNCTION_AUTO_MODEL 
    Model_Initial_Data(Models_Default,AVAIABLE_MACHINES_TOTAL,AvailableLanguage,AVAILABLE_LANGUAGES);
#endif
    Page_Init_Data();
	String_Init_Data();
}

void Main_Initial_Data(INIT_TYPE type)
{ 
	if( type== TYPE_ALL_INIT )
	{
		Beep_Initial_Data();
	    Beep_Set_Enable(true);
	    FAN_PWM fan_pwm={60,30,90,4};
	    Fan_Initial_Data(fan_pwm);
		Keyboard_Initial_Data();
		Main_Init_Model_Data();
		
#ifdef RF_MODULE_EXIST_
    		RF_AP_Initial_Data();
#endif
#ifdef  __WIFI_FUNC__
   		Digital_Initial_Data_wifi();
#endif
		Timer_Initial_Data();	
		Page_Init_Timer();
	}
	BurnTest_Initial_Data();
    ERP_Initial_Data();
    Err_Initial_Data();
	Wifi_Clear_Status();
	Page_Init_Data();
	Engmode_Initial_Data(1);
	Display_Initial_Data();
    Midware_Heart_Initial_Data();
    User_Initial_Data();
	LubeBelt_Initial_Data();
    Event_Handler_Init_Data();
	Main_Stage = STAGE_USER;
	nCurrentPage = PAGE_SELECTUSER;
	Event_Set_Virtual_key(VK_INITIAL);
}

