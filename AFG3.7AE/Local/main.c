/*IAR Embedded Workbench for ARM, Evaluation version 6.30*/

#include "Main.h"
#include "Table_List.h"
#include "WattsTable.h"
#include "Bluetooth_lib.h"

#include "FACTORYDEFAULT.H"
#include "USBManager.h"
#include "User.h"
#include "Auto_Model.h"
#if (defined EP)||(defined AE)
#include "RPM.h"
#include "ECB.h" 
#endif
#ifdef AE
#include "Incline.h" 
#endif

void Main_Init_Model_Data(void);
static void Main_PowerOn_Initial(void);
static void Main_Initial_Data(void);
static void Main_Initial_IO(void);
UINT32  TestTimer;


extern void Bluetooth_Events_Handler(BLUETOOTH_EVENT Bluetooth_Event);
extern unsigned short __checksum;
__root const unsigned char DeviceModel[32] @ ".ModelAddr" = THISMODEL;
__root const unsigned char DeviceSoftwareMVersion[8] @ ".MVerAddr" = C_S_VER;
__root const unsigned char DeviceSoftwareSVersion[8] @ ".SVerAddr" = C_S_NUM;
__root const unsigned char DeviceUpdateRegion[4] @ ".UpdateStep" = {0,0,0,0};

__no_init UINT8 BLINK_DISPLAY_CHANNEL;
__no_init UINT8 STATIC_DISPLAY_CHANNEL;
__no_init UINT8 MODAL_WINDOW_CHANNEL;
__no_init UINT8 REFRESH_WINDOW_CHANNEL;
__no_init UINT8 BTHRTimerChannel ;

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
     nCurrentPage = PAGE_SELECTUSER;
     Event_Handler_SetNewState( FRAME_STAGE_USER );
     Event_Set_Virtual_key(VK_INITIAL);
     BLINK_DISPLAY_CHANNEL = Timer_Clear();
     STATIC_DISPLAY_CHANNEL = Timer_Clear();
     MODAL_WINDOW_CHANNEL = Timer_Clear();
     REFRESH_WINDOW_CHANNEL = Timer_Clear();
     BTHRTimerChannel = Timer_Clear();
   }
   HAL_BOR_Enable_ExtPower(Power_Channel_A,true);
   HAL_Enable_MasterInterrupt();	
   HAL_Set_IO_Output(USB_CRTL_PORT,USB_CRTL_PIN);
   HAL_Set_IO_High(USB_CRTL_PORT,USB_CRTL_PIN);
   USBManager_Initial_IO();
   USBManager_Initial_Data();
   //     while(1)
   //     {
   //         Display_Set_All(1);
   //         Display_Bottom_Text(0,"NOPQRSTUVWXYZ");
   //         Display_Updata_Data(true);
   //     }
   //     initialBluetoothLib();
   //     Wlt2564_Reset();
   while(true)
   {  
     if(Timer_Counter(BLINK_DISPLAY_CHANNEL+0x80,5))
     {
       Events_Blink_Timer_Handler();
     }
     
     if(Timer_Counter(STATIC_DISPLAY_CHANNEL+0x80,10))
     {
       TestTimer = 0;
       Event_Static_Timer_Handler();
     }
     if(Timer_Counter(MODAL_WINDOW_CHANNEL+0x80,10))
     {
       Event_Modal_Timer_Handler();
     }
     
     if(Timer_Counter(REFRESH_WINDOW_CHANNEL+0x80,2))
     {
       Event_Refresh_Timer_Handler();
     }
     //KEY_EVENTS
     Key_Events_Handler(KeyBoard_Get_Code(0));
     
     //UTL_EVENTS
     UTL_Events_Handler(UTL_Process()); 
     
     //USB EVENTS   
     USB_Events_Handler(USBManager_Process());
     
     //User Setup Thread
     User_Events_Handler(User_Process());
     
     //PROGRAM_EVENTS
     Program_Events_Handler(Program_Process());
     
     //WIFI_EVENTS
#ifdef  __WIFI_FUNC__
     WIFI_Events_Handler(Digital_Command_Proce_wifi());
#endif
     
#ifdef RF_MODULE_EXIST_   
     RF_Events_Handler(RF_AP_Process());
#endif
     //demo events
     Demo_Events_Handler();
     
     RTC_Event_Handler(RTCSetMode_Process());
     //Eng Process
     ENG_Events_Handler(Engmode_Process());
     Bluetooth_Events_Handler(BluetoothProcess());
     //DISPLAY_UPDATE         
     Display_Updata_Data(true);
     //         LED_Updata(true);
   }
   
 }



static void Main_PowerOn_Initial(void)
{
    UINT8 isFirstBoot = 0;
    
    isFirstBoot = *((UINT8*)(SystemConfig_Get_FirstBoot_ADDRESS()));
    
    SystemConfig_Initial();           //TODO: 在Poweron读Flash不能开中断        
    
    Main_Initial_Data();
    Display_Set_All(false);
    LED_Set_All(false);
//    LED_Updata(true);
    Display_Updata_Data(true);
    HAL_Enable_MasterInterrupt();        
    LCB_Power_On(true);               //TODO：LCB需自行阻塞直到超时或者命令完成
#ifdef OUTPUT_DEBUG
    HAL_Monitor_Send_String("LCB_Power_On_Finish:",strlen("LCB_Power_On_Finish:"));
    memset(DBGPrintBuf,'\0',64);
    sprintf(DBGPrintBuf,"%4u",TestTimer);
    HAL_Monitor_Send_String(DBGPrintBuf,strlen(DBGPrintBuf));
#endif             
    HAL_BOR_Enable_ExtPower(Power_Channel_A,true);
    
    if(isFirstBoot != 0xA5)
    {
        g_wndBottomTextBox._isHidden = 1;
        g_wndBottomTextBox._string_id = STR_INITIAL_ID;
        g_wndBottomTextBox.pfShow=ShowCommonTextBox;
        g_wndBottomTextBox.pfShow(&g_wndBottomTextBox);
        Display_Updata_Data(true);
        for(UINT32 i=0;i < 0x3fffff;i++);
        Beep_Set_Beeps(6,10,5);
        while(Beep_Get_OK() == false)
        {
//            LED_Updata(true);
            LCB_Process();
        }
    }
#ifdef OUTPUT_DEBUG
    memset(DBGPrintBuf,'\0',64);
    snprintf(DBGPrintBuf,64,"==Johnson Framework V1.0==\n");
    HAL_Monitor_Send_String(DBGPrintBuf,strlen(DBGPrintBuf));
#endif

	char versionBuffer1[16];
    memset(versionBuffer1,'\0',20);
    snprintf(versionBuffer1,13,"V %s.%3s",C_S_VER,C_S_NUM);
    DisplayString(0,13,0,versionBuffer1);
    Display_Updata_Data(true);
    for(UINT32 i=0;i < 0x3fffff;i++);
    Beep_Set_Beeps(6,10,10);
    while(Beep_Get_OK() == false)
    { 
//        LED_Updata(true);
        LCB_Process();
    }
    BOR_Set_Passcode(C_PASSWORD); 
}

static void Main_Initial_IO(void)
{
    HAL_Config_Clock_Active();
 
    BOR_Initial_IO();
    HAL_LFT_Initial_IO();
    HAL_HR_Initial_IO();
    Hal_Regist_ISR(MS_ISR,Heart_ISR1_IDX,Midware_Heart_1ms_Int);
    HAL_SafetyKey_Initial_IO();
    Fan_Initial_IO();
    Beep_Initial_IO();
    HAL_AMP_Initial_IO(1);
    HAL_TLC59282_Initial_IO();
    HAL_Switch_Initial_IO();
    HAL_LCD_Backlight_Initial_IO();
    
#if (defined EP)||(defined AE)    
    HAL_RPM_Initial_IO();
    HAL_ECB_Initial_IO();
    HAL_ADC_SINGLE_Initial();
    HAL_RESISTANCE_VR_ADCInitial();
#endif     
#ifdef AE       
    HAL_Incline_Initial_IO();
    HAL_INCLINE_VR_ADCInitial();
#endif
    
    Display_Regist_1msISR();
    HAL_RTC_Initial_IO();
    Keyboard_Initial_IO();
	//HAL_Key_Initial_IO(KEYMODE_I2C); 
#ifdef RF_MODULE_EXIST_   
    HAL_RF_Initial_IO(); 
#endif
#ifdef  __WIFI_FUNC__
    HAL_WIFI_Initial_IO();
#endif
    HAL_BT_Initial_IO();
#ifdef TM
    HAL_DLCB_Initial_IO();
#endif
#ifdef OUTPUT_DEBUG     
    HAL_Monitor_Init_Uart();
#endif
#ifdef TM
    Hal_Regist_ISR(UART_RX_ISR,BEEP_ISR_IDX, Digital_Cmd_Rx_Int);    //Rx
    Hal_Regist_ISR(UART_TX_ISR,BEEP_ISR_IDX, Digital_Cmd_Tx_Int);    //TX
    Hal_Regist_ISR(MS_ISR, LCB_ISR_IDX, LCB_1ms_Int); 
#endif  
#ifdef  __WIFI_FUNC__
    Hal_Regist_ISR(MS_ISR,WIFI_ISR_IDX,Digital_10ms_Timeout_Int_wifi);
#endif
    Beep_Regist_Functions();
    Bor_Regist_Functions();
    Erp_Regist_Functions();
    Err_Regist_Functions();
    Timer_Regist_Functions();
#ifdef RF_MODULE_EXIST_
    RF_Regist_Functions();
#endif
    Register_Program_ISR();
    Engmode_Regist_Function();
    LED_Regist_Functions();
    BT_Regist_Functions();
    Midware_Heart_Regist_Functions();    //zsf
}


void Main_Init_Model_Data(void)
{
    KeyBoard_Set_TrueTable(&Table[0][0],8,8);
    
    KeyBoard_Set_KeyholdTable(HOLDTable,11);
    if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)== TREADMILL)
    {
        KeyBoard_Set_KeyComboTable(TM_COMboTable,5);
    }
    else
    {
#ifdef AE
      KeyBoard_Set_KeyComboTable(AE_COMboTable,5);
#else
      KeyBoard_Set_KeyComboTable(EP_COMboTable,5);      
#endif       
    }     
    Timer_Initial_Data();
    if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)== TREADMILL)
    {
        Program_Initial_Data(&PrgLib_TM_Config);
    }
    else
    {
#ifdef AE
      Program_Initial_Data(&PrgLib_AE_Config);
#else
      Program_Initial_Data(&PrgLib_EP_Config);
#endif
    }
    Program_Load_WattsTable(&WattsParameter);
//    switch(SystemConfig_Get_Char(DRIVE_TYPE_UINT8))
//    {
//    case EP_0B_JIS:
//    case DIGITAL_ECB:
//    case ECB_JIS:
//        if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==ELLIPTICAL||\
//            SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==BIKE)
//        {
//            Program_Initial_Data(&Program_ECB_EpBike_ParaConfig);
//        }
//        else if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==ASCENT)
//        {
//            Program_Initial_Data(&Program_Induction_EpBike_ParaConfig);
//        }
//        Program_Load_WattsTable(&ECB_WattsParameter1);
//        break;
//    case ECB_INDUCTOR_BRAKE:
//    case BIKE_0C_JIS:
//    case INDUCTOR_BRAKE_INCLINE:
//        if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==ELLIPTICAL||\
//            SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==BIKE)
//        {
//            Program_Initial_Data(&Program_ECB_Ascent_ParaConfig);
//        }
//        else if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==ASCENT)
//        {
//            Program_Initial_Data(&Program_Induction_Ascent_ParaConfig);
//        }
//        Program_Load_WattsTable(&Induction_WattsParameter1);
//        
//        break;
//    default:
//        Program_Initial_Data(&Program_TM_ParaConfig);
//        break;
//    }
    
    LCB_Initial();
#ifdef FUNCTION_AUTO_MODEL 
    Model_Initial_Data(Models_Default,AVAIABLE_MACHINES_TOTAL,AvailableLanguage,AVAILABLE_LANGUAGES);
#endif
    Page_Init_Data();
#ifdef  __WIFI_FUNC__
    if(WIFIIsAPMode())
        g_wndWiFiIcon._blinkCount = 0xAA ;
#endif
	String_Init_Data();
}

void Main_Initial_Data(void)
{ 
    Beep_Initial_Data();
    Beep_Set_Enable(true);
    FAN_PWM fan_pwm={60,30,90,4};
    Fan_Initial_Data(fan_pwm);
    HAL_FAN_Set_Duty(0);
#ifdef  AE 
    Incline_Initial_Data();
#endif  
#if (defined EP)||(defined AE)     
    RPM_Initial_Data();   
    RPM_AUTO_Check(0);
    RPM_AutoRPM_Clear();
    Hal_Regist_ISR(MS_ISR, RPM_Calculate_ISR_IDX, RPM_Calculate_Counter_Int);
    Hal_Regist_ISR(MS_ISR, GPIO_ODD_IRQ_ISR_IDX, GPIO_ODD_IRQHandler);
    ECB_Initial_Data();
#endif
#if defined( AE)
    Hal_Regist_ISR(MS_ISR, ADC0_IRQHandler_ISR_IDX, ADC0_1MS_Handler);
#elif defined (EP)
    Hal_Regist_ISR(MS_ISR, HAL_VR2_Get_CrrntChk_ISR_IDX, HAL_VR2_Get_CrrntChk_IO);
#endif
   
    Midware_Heart_Initial_Data();
    Keyboard_Initial_Data();
    Main_Init_Model_Data();
    Display_Initial_Data();
    ERP_Initial_Data();
    Err_Initial_Data();
#ifdef RF_MODULE_EXIST_
    RF_AP_Initial_Data();
#endif
#ifdef  __WIFI_FUNC__
    Digital_Initial_Data_wifi();
#endif
    Engmode_Initial_Data(1);
    User_Initial_Data();
    initialBluetoothLib();
    Event_Handler_Init_Data();
    RTCSetModeInitialData();
}
