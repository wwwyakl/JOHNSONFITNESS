#include "JIStypes.h"
#include "Global_Config.h"
#ifndef _FACTORYDEFAULT_H__
#define _FACTORYDEFAULT_H__

typedef enum
{
    MODEL_TYPE_START = 0,
    TM_TF30 = MODEL_TYPE_START,
    TM_TF50,
    TM_T50,
    BK_R30,
    BK_U30,
    EP_SE30,
    BK_R50I,
    BK_U50I,
    EP_SE50I,
    AST_A30,
    AST_A50I,
    TM_T70_C2,
    TM_T70_C3,
    TM_T70_C1,
    AVAIABLE_MACHINES_TOTAL
}AVAIABLE_MODELS;

#ifdef TM
#define C_S_VER     		    "1"
#define C_S_NUM     		    "010"
#elif defined EP
#define C_S_VER     		    "1"
#define C_S_NUM     		    "003"
#else
#define C_S_VER     		    "1"
#define C_S_NUM     		    "001"
#endif

#define THISMODEL		    "AFG PRO"
const char* rscu_version[3]={
    C_S_VER,
    C_S_NUM,
    THISMODEL
};


#define D_MACHINE_MODEL  		52
#define D_CONSOLE_TYPE  		1
#define D_BRAND_ID  			HORIZON
#ifdef TM
#define D_MACHINE_TYPE  		TREADMILL  
#define D_DRIVE_TYPE			DC_LCB_20_275HP_JIS
#define D_MAX_USER_WEIGHT_LBS		325
#define D_MIN_USER_WEIGHT_LBS		50
#define D_MAX_USER_WEIGHT_KG		148
#define D_MIN_USER_WEIGHT_KG		23
#else 
#define D_MACHINE_TYPE  		ELLIPTICAL
#define D_DRIVE_TYPE			EP_0B_JIS
#define D_MAX_USER_WEIGHT_LBS		300  
#define D_MIN_USER_WEIGHT_LBS		50
#define D_MAX_USER_WEIGHT_KG		136  
#define D_MIN_USER_WEIGHT_KG		23
#endif
#define D_LANGUAGE		        ENGLISH_US
#define D_UNIT					STANDARD


#define D_DISPLAY_MODE			0
#define D_VERSION_MAJOR			1
#define D_VERSION_MINOR			1
#define D_VERSION_REVISION		1

#define D_MAX_SPEED_MPHX10		120
#define D_MIN_SPEED_MPHX10		5
#define D_MAX_SPEED_KPHX10		200
#define D_MIN_SPEED_KPHX10		8
#define D_MAX_RPM_MPHX10		4101
#define D_MIN_RPM_MPHX10		170
#define D_MAX_RPM_KPHX10		4248	
#define D_MIN_RPM_KPHX10		170

#ifdef TM
#define D_MAX_INCLINEX10		120
#define D_MIN_INCLINEX10		0
#define D_MAX_INCLINE_ADC		1000
#define D_MIN_INCLINE_ADC		30
#else
#define D_MAX_INCLINEX10		100 //100% REF TT5.0 of 20 //20
#define D_MIN_INCLINEX10		0
#define D_MAX_INCLINE_ADC		3715  // REF TT5.0 //1600
#define D_MIN_INCLINE_ADC		915  // REF TT5.0 //40
#endif

#ifdef EP
#define D_MAX_RESISTANCE		10
#define D_MIN_RESISTANCE		1
#else
#define D_MAX_RESISTANCE		20
#define D_MIN_RESISTANCE		1
#endif
#define D_FORCE_UPDATE			0
#define D_FIRMWARE_AVAILANLE	0
#define D_DEMO_MODE_ON			0
#define D_ENERGY_SAVER_ON		1
#define D_BUZZER_ON			    0
#define D_RTC_PRESET			0
#define D_24HOURS_FORMAT		0
#define D_ACCUMULATED_AGING_HOURS	0
#define DEFAULT_VOLUME			9
#define DEFAULT_BRIGHTNESS		50
#define D_MAX_AGE			100
#define DEFAULT_AGE			30
#define D_MIN_AGE			10
#define DEFAULT_WEIGHT_STANDARD		15000
#define DEFAULT_WEIGHT_METRIC		6800
#define	D_TARGETHR				80

#define D_CSAFE_ID			2
#define D_CSAFE_MANU_FACTURE_ID		31
#define D_CSAFE_MODE_ID			0

#define D_CLUB_ID			0
#define D_AUTO_LOGIN			0

#define D_AP_MODE_ON			1

#define D_RF_CHANNEL			0
#define D_RF_ADDRESS			0x01

#define D_DEFAULT_ERP_WAITTIME          900 //15minutes

#define D_DEFAULT_DEMO_WAITTIME         300 // 5 minutes

#define WIFISSID     "MATRIX XR"
//#define WIFISSID     "XR-HMZ"
#define DEFAULT_TIME_ZONE "America/Chicago"
#define DEFAULT_MACHINEUUID "00000000-0000-0000-0000-000000000000"
#if ( WIFI_DAPI_VERSION == 9 )
 #define DEFAULT_SERIAL_NUMBER  "000000000000000"
 #define DEFAULT_FRAME_NUMBER  "000000000000000"
#else
 #define DEFAULT_SERIAL_NUMBER  "000000000000000000000000000000000000"
 #define DEFAULT_FRAME_NUMBER "000000000000000000000000000000000000"
#endif


//#define JOHNSON_WIFI_SERVERIP            JOHNSON_WIFI_SERVERIP_STAGING
#define JOHNSON_WIFI_SERVERIP            JOHNSON_WIFI_SERVERIP_QA

const MODEL_DEFAULTS Models_Default[AVAIABLE_MACHINES_TOTAL] = 
{
    {125, 5,    200, 8,    4351, 173,   4327, 173,    150, 0,    1000, 34,     20,1,     TREADMILL,   DC_LCB_30_325HP_JIS,            1, TM_TF30,    MATRIX_RETIAL,},
    {125, 5,    200, 8,    4234, 169,   4210, 169,    150, 0,    1000, 34,     20,1,     TREADMILL,   DC_LCB_30_325HP_JIS,            2, TM_TF50,    MATRIX_RETIAL,},
    {125, 5,    200, 8,    4234, 169,   4210, 169,    150, 0,    1000, 34,     20,1,     TREADMILL,   DC_LCB_30_325HP_JIS,            3, TM_T50,     MATRIX_RETIAL,},

    {125, 5,    200, 8,    4100, 160,   4200, 160,    100, 0,    900, 30,     20,1,     BIKE,        DIGITAL_ECB,               3, BK_R30,     MATRIX_RETIAL,},
    {125, 5,    200, 8,    4100, 160,   4200, 160,    100, 0,    900, 30,     20,1,     BIKE,        DIGITAL_ECB,               1, BK_U30,     MATRIX_RETIAL,},
    {125, 5,    200, 8,    4100, 160,   4200, 160,    100, 0,    900, 30,     20,1,     ELLIPTICAL,  DIGITAL_ECB,               1, EP_SE30,    MATRIX_RETIAL,},
    {125, 5,    200, 8,    4100, 160,   4200, 160,    100, 0,    900, 30,     30,1,     BIKE,        BIKE_0C_JIS,           4, BK_R50I,    MATRIX_RETIAL,},
    {125, 5,    200, 8,    4100, 160,   4200, 160,    100, 0,    900, 30,     30,1,     BIKE,        BIKE_0C_JIS,           2, BK_U50I,    MATRIX_RETIAL,},
    {125, 5,    200, 8,    4100, 160,   4200, 160,    100, 0,    900, 30,     30,1,     ELLIPTICAL,  BIKE_0C_JIS,           2, EP_SE50I,   MATRIX_RETIAL,},
    {125, 5,    200, 8,    4100, 160,   4200, 160,    100, 0,    1575, 34,     20,1,     ASCENT,      EP_0B_JIS,             1, AST_A30,    MATRIX_RETIAL,},
    {125, 5,    200, 8,    4100, 160,   4200, 160,    100, 0,    1575, 34,     30,1,     ASCENT,      INDUCTOR_BRAKE_INCLINE,2, AST_A50I,   MATRIX_RETIAL,},
    {125, 5,    200, 8,    4916, 120,   4947, 120,    150, 0,    195,  20,     20,1,     TREADMILL,   MCB_DELTA,             4, TM_T70_C2,     MATRIX_RETIAL,},//c2
    {125, 5,    200, 8,    4916, 120,   4947, 120,    150, 0,    195,  20,     20,1,     TREADMILL,   MCB_DELTA_COMMOCIAL,           4, TM_T70_C3,     MATRIX_RETIAL,},//c3
    {125, 5,    200, 8,    3375, 90,   3510, 90,    150, 0,    195,  20,     20,1,     TREADMILL,   MCB_TOPTEK,             4, TM_T70_C1,     MATRIX_RETIAL,},//c1
};

//const MODEL_DEFAULTS Models_Default[AVAIABLE_MACHINES_TOTAL] = 
//{
//    {125, 5,    200, 8,    4351, 173,   4327, 173,    150, 0,    1100, 34,     20,1,     TREADMILL,   DC_LCB_JIS,            1, TM_TF30,    MATRIX_RETIAL,},
//    {125, 5,    200, 8,    4234, 169,   4210, 169,    150, 0,    1100, 34,     20,1,     TREADMILL,   DC_LCB_JIS,            2, TM_TF50,    MATRIX_RETIAL,},
//    {125, 5,    200, 8,    4234, 169,   4210, 169,    150, 0,    1100, 34,     20,1,     TREADMILL,   DC_LCB_JIS,            3, TM_T50,     MATRIX_RETIAL,},
//
//    {125, 5,    200, 8,    4100, 160,   4200, 160,    150, 0,    1000, 30,     20,1,     BIKE,        ECB_JIS,               3, BK_R30,     MATRIX_RETIAL,},
//    {125, 5,    200, 8,    4100, 160,   4200, 160,    150, 0,    1000, 30,     20,1,     BIKE,        ECB_JIS,               1, BK_U30,     MATRIX_RETIAL,},
//    {125, 5,    200, 8,    4100, 160,   4200, 160,    150, 0,    1000, 30,     20,1,     ELLIPTICAL,  ECB_JIS,               1, EP_SE30,    MATRIX_RETIAL,},
//    
//    {125, 5,    200, 8,    4100, 160,   4200, 160,    150, 0,    1000, 30,     30,1,     BIKE,        BIKE_0C_JIS,           4, BK_R50I,    MATRIX_RETIAL,},
//    {125, 5,    200, 8,    4100, 160,   4200, 160,    150, 0,    1000, 30,     30,1,     BIKE,        BIKE_0C_JIS,           2, BK_U50I,    MATRIX_RETIAL,},
//    {125, 5,    200, 8,    4100, 160,   4200, 160,    150, 0,    1000, 30,     30,1,     ELLIPTICAL,  BIKE_0C_JIS,           2, EP_SE50I,   MATRIX_RETIAL,},
//    {125, 5,    200, 8,    4916, 120,   4750, 120,    150, 0,    205,  20,     20,1,     TREADMILL,   MCB_DELTA,             4, TM_T70,     MATRIX_RETIAL,},//c2
//    {125, 5,    200, 8,    4100, 160,   4200, 160,    150, 0,    1675, 34,     20,1,     ASCENT,      EP_0B_JIS,             1, AST_A30,    MATRIX_RETIAL,},
//    {125, 5,    200, 8,    4100, 160,   4200, 160,    150, 0,    1675, 34,     30,1,     ASCENT,      INDUCTOR_BRAKE_INCLINE,2, AST_A50I,   MATRIX_RETIAL,},
//};
#define AVAILABLE_LANGUAGES 8
const UINT8 AvailableLanguage[AVAILABLE_LANGUAGES] = {ENGLISH_US,FRENCH,GERMANY,DUTCH,SPANISH,ITALIAN,PORTUGUESE,VIETNAMESE};

//old stop not prior
//const unsigned char Table[8][8]= 
//{
//     //    D1                D2                D3               D4            D5               D6          D7            D8  
//     KB_ARROW_DOWN,      KB_RF,            KB_START,        KB_NUMBER1,   KB_NUMBER3,      KB_NUMBER2,  KB_NONE,     KB_NONE,//C1
//     KB_ARROW_UP,        KB_CHANGE_DISPLAY,KB_SPEED_FAST,   KB_NUMBER5,   KB_SPEED_SLOW,   KB_NUMBER6,  KB_NUMBER4,  KB_NONE,//C2
//     KB_STOP,            KB_INCLINE_DOWN,  KB_WIFI,         KB_NUMBER8,   KB_NUMBER9,      KB_NONE,     KB_NUMBER7,  KB_NONE,//C3
//     KB_NONE,            KB_INCLINE_UP,    KB_START,        KB_NUMBER0,   KB_STOP,         KB_ENTER,    KB_BACK,     KB_NONE,//C4
//     
//     KB_NONE,	      KB_NONE,	        KB_NONE,         KB_NONE,	KB_NONE,	KB_NONE,     KB_NONE,	  KB_NONE,//C5
//     KB_NONE,	      KB_NONE,	        KB_NONE,         KB_NONE,	KB_NONE,	KB_NONE,     KB_NONE,	  KB_NONE,//C6
//     KB_NONE,	      KB_NONE,	        KB_NONE,	 KB_NONE,	KB_NONE,	KB_NONE,     KB_NONE,	  KB_NONE,//C7
//     KB_NONE,	      KB_NONE,	        KB_NONE,	 KB_NONE,	KB_NONE,	KB_NONE,     KB_NONE,	  KB_NONE,//C8
//};
////new stop is prior
#ifdef TM
const unsigned char Table[8][8]= 
{
     //D1              D2              D3               D4              D5                D6            D7                  D8  
     KB_INCLINE_0,     KB_INCLINE_1,   KB_NONE,          KB_NONE,        KB_SPEED_12,     KB_SPEED_10,  KB_SPEED_8,         KB_NONE,//C1
     KB_INCLINE_6,     KB_INCLINE_4,   KB_INCLINE_2,     KB_NONE,        KB_SPEED_4,      KB_SPEED_5,   KB_SPEED_6,         KB_NONE,//C2
     KB_INCLINE_8,     KB_INCLINE_10,  KB_INCLINE_12,    KB_NONE,        KB_NONE,         KB_SPEED_3,   KB_SPEED_2,         KB_NONE,//C3
     KB_NONE,          KB_NONE,        KB_NONE,          KB_NONE,        KB_NONE,         KB_NONE,      KB_NONE,            KB_NONE,//C4

     KB_MANUAL,	       KB_WEIGHT_LOSS, KB_FAN,           KB_ENTER,	     KB_BLUETOOTH,	  KB_DISTANCE,  KB_CALORIES,        KB_STOP,//C5
     KB_INTERVALS,     KB_NONE,        KB_INCLINE_DOWN,  KB_INCLINE_UP,	 KB_SPEED_FAST,   KB_SPEED_SLOW,KB_NONE,            KB_NONE,//C6
     KB_NONE,	       KB_NONE,        KB_NONE,          KB_START ,      KB_NONE,         KB_NONE,      KB_CHANGE_DISPLAY,	KB_NONE,//C7
     KB_NONE,	       KB_NONE,        KB_NONE,          KB_NONE,	     KB_NONE,	      KB_NONE,      KB_NONE,	        KB_NONE,//C8
};
#elif defined EP
const unsigned char Table[8][8]= 
{
     //D1                D2              D3               D4                //D5                 D6                D7                    D8  
     KB_NONE,            KB_NONE,        KB_NONE,         KB_NONE,          KB_NONE,             KB_NONE,         KB_NONE,              KB_NONE,//C1
     KB_NONE,            KB_NONE,        KB_NONE,         KB_NONE,          KB_NONE,             KB_NONE,         KB_NONE,              KB_NONE,//C2
     KB_NONE,            KB_NONE,        KB_NONE,         KB_NONE,          KB_NONE,             KB_NONE,         KB_NONE,              KB_NONE,//C3
     KB_NONE,            KB_NONE,        KB_NONE,         KB_NONE,          KB_NONE,             KB_NONE,         KB_NONE,              KB_NONE,//C4

     KB_NONE,	           KB_NONE,        KB_NONE,         KB_STOP,	      KB_CALORIES,         KB_INTERVALS,    KB_LEVEL_6,           KB_NONE,//C5
     KB_LEVEL_3,         KB_LEVEL_4,     KB_LEVEL_5,      KB_WEIGHT_LOSS,   KB_CHANGE_DISPLAY,   KB_LEVEL_9,      KB_LEVEL_7,           KB_NONE,//C6
     KB_LEVEL_1,         KB_LEVEL_2,     KB_MANUAL,       KB_DISTANCE,      KB_LEVEL_8,          KB_LEVEL_10,     KB_RESISTANCE_UP,     KB_NONE,//C7
     KB_RESISTANCE_DOWN, KB_START,       KB_ENTER,        KB_BLUETOOTH,     KB_NONE,             KB_NONE,         KB_NONE,	          KB_NONE,//C8
};
#else 
const unsigned char Table[8][8]= 
{ 
     //D1                 D2                 D3                  D4                 D5                   D6                  D7                       D8  
     KB_NONE,            KB_NONE,           KB_NONE,            KB_NONE,           KB_NONE,             KB_NONE,            KB_NONE,                 KB_NONE,    //C1
     KB_NONE,            KB_NONE,           KB_NONE,            KB_NONE,           KB_NONE,             KB_NONE,            KB_NONE,                 KB_NONE,    //C2
     KB_NONE,            KB_NONE,           KB_NONE,            KB_NONE,           KB_NONE,             KB_NONE,            KB_NONE,                 KB_NONE,    //C3
     KB_NONE,            KB_NONE,           KB_NONE,            KB_NONE,           KB_NONE,             KB_NONE,            KB_NONE,                 KB_NONE,    //C4

     KB_INCLINE_DOWN,    KB_INCLINE_UP,     KB_INCLINE_4,      KB_INCLINE_8,     KB_INTERVALS,        KB_CALORIES,        KB_CHANGE_DISPLAY,       KB_NONE,    //C5
     KB_INCLINE_12,      KB_INCLINE_16,     KB_INCLINE_20,     KB_WEIGHT_LOSS,    KB_LEVEL_20,         KB_LEVEL_16,        KB_LEVEL_12,             KB_LEVEL_8, //C6
     KB_MANUAL,          KB_DISTANCE,       KB_NONE,            KB_ENTER,          KB_LEVEL_4,          KB_NONE,            KB_NONE,	         KB_NONE,    //C7
     KB_NONE,            KB_START,          KB_BLUETOOTH,       KB_STOP,           KB_RESISTANCE_DOWN,  KB_RESISTANCE_UP,   KB_NONE,	         KB_NONE,    //C8

//EP KEY
//     KB_NONE,	           KB_NONE,            KB_NONE,            KB_STOP,          KB_CALORIES,	      KB_INTERVALS,       KB_LEVEL_20,             KB_NONE,    //C5
//     KB_INCLINE_12,      KB_INCLINE_16,      KB_INCLINE_20,     KB_WEIGHT_LOSS,   KB_CHANGE_DISPLAY,   KB_LEVEL_8,         KB_LEVEL_16,             KB_NONE,    //C6
//     KB_INCLINE_4,      KB_INCLINE_8,      KB_MANUAL,          KB_DISTANCE,      KB_LEVEL_12,          KB_LEVEL_4,        KB_ARROW_UP/*KB_RESISTANCE_UP*/,        KB_NONE,    //C7
//     KB_ARROW_DOWN/*KB_RESISTANCE_DOWN*/, KB_START,           KB_ENTER,           KB_BLUETOOTH,     KB_NONE,	      KB_NONE,            KB_NONE,	         KB_NONE,    //C8
};
#endif
KEY_COMBO TM_COMboTable[5] = {{KB_INCLINE_UP,KB_SPEED_SLOW,KB_ENG_Backup},{KB_SPEED_2,KB_SPEED_3,KB_RTC_Backup},{KB_SPEED_FAST,KB_STOP,KB_LUBEBELT_Backup},{KB_RESISTANCE_UP,KB_STOP,KB_RF},{KB_INCLINE_UP,KB_STOP,KB_RF}}; 
// KEY_COMBO EP_COMboTable[5] = {{KB_SPEED_FAST,KB_SPEED_SLOW,KB_ENG_Backup},{KB_SPEED_2,KB_SPEED_3,KB_RTC_Backup},{KB_SPEED_FAST,KB_STOP,KB_LUBEBELT_Backup},{KB_RESISTANCE_UP,KB_STOP,KB_RF},{KB_INCLINE_UP,KB_STOP,KB_RF}};
KEY_COMBO EP_COMboTable[5] = {{KB_RESISTANCE_UP,KB_RESISTANCE_DOWN,KB_ENG_Backup},{KB_SPEED_2,KB_SPEED_3,KB_RTC_Backup},{KB_SPEED_FAST,KB_STOP,KB_LUBEBELT_Backup},{KB_RESISTANCE_UP,KB_STOP,KB_RF},{KB_INCLINE_UP,KB_STOP,KB_RF}};
KEY_COMBO AE_COMboTable[5] = {{KB_INCLINE_UP,KB_RESISTANCE_DOWN,KB_ENG_Backup},{KB_SPEED_2,KB_SPEED_3,KB_RTC_Backup},{KB_SPEED_FAST,KB_STOP,KB_LUBEBELT_Backup},{KB_RESISTANCE_UP,KB_STOP,KB_RF},{KB_INCLINE_UP,KB_STOP,KB_RF}};
KEY_HOLD HOLDTable[11] = {{KB_STOP,KB_HOLD_3S,KB_RESET},{KB_ENG_Backup,KB_HOLD_3S,KB_ENG},{KB_LUBEBELT_Backup,KB_HOLD_3S,KB_STOP_FAST},\
	{KB_RF,KB_HOLD_3S,KB_RF_SYNC},{KB_WIFI,KB_HOLD_3S,KB_WIFI_AP},\
	{KB_RTC_Backup,KB_HOLD_3S,KB_RTC},{KB_NUMBER9,KB_HOLD_10S,KB_Hold_9},\
    {KB_SPEED_FAST,KB_HOLD_1S,KB_SPEED_LONGPRESS},{KB_QUICK_SPD_FAST,KB_HOLD_1S,KB_SPEED_LONGPRESS},{KB_CHANGE_DISPLAY,KB_HOLD_5S,KB_SCAN},{KB_BLUETOOTH,KB_HOLD_5S,KB_BT_PAIR}};


const SystemSettingType SystemSettingArray[SETTING_STRING_END]=
{
    { MAX_MACHINE_UUID , DEFAULT_MACHINEUUID},//SETTING_DEFAULT_MACHINE_UUID
    { MAX_FRAME_SERIAL_NUMBER , DEFAULT_SERIAL_NUMBER},//SETTING_DEFAULT_SERIAL_NUMBER
    { MAX_CONSOLE_SERIAL_NUMBER,DEFAULT_FRAME_NUMBER},//SETTING_DEFAULT_FRAME_NUMBER
    { MAX_TIME_ZONE,DEFAULT_TIME_ZONE},//SETTING_DEFAULT_TIME_ZONE
    { MAX_MAIRO_BOX,JOHNSON_WIFI_SERVERIP_PRODUCTION},//SETTING_MARIOX_PRODUCTION_ADDRESS
    { MAX_MAIRO_BOX, JOHNSON_WIFI_SERVERIP_QA},//SETTING_MARIOX_QA_ADDRESS
    { MAX_MAIRO_BOX,JOHNSON_WIFI_SERVERIP_STAGING},//SETTING_MARIOX_STAGING_ADDRESS    
    { 30, WIFISSID}//SETTING_DEFAULT_SSID
};






/************main.c π”√*************/
const UINT16 Factory_Default[FACTORY_DEFAULT_END]=
{
    D_MACHINE_TYPE  		,
    D_MACHINE_MODEL  		,
    D_CONSOLE_TYPE          ,
    D_BRAND_ID  			,
    D_DRIVE_TYPE			,
    D_LANGUAGE				,
    D_UNIT					,
    D_MAX_USER_WEIGHT_LBS		,
    D_MIN_USER_WEIGHT_LBS		,
    D_MAX_USER_WEIGHT_KG		,
    D_MIN_USER_WEIGHT_KG		,
    
    D_DISPLAY_MODE			,
    D_VERSION_MAJOR			,
    D_VERSION_MINOR			,
    D_VERSION_REVISION		,
    
    D_MAX_SPEED_MPHX10		,
    D_MIN_SPEED_MPHX10		,
    D_MAX_SPEED_KPHX10		,
    D_MIN_SPEED_KPHX10		,
    D_MAX_RPM_MPHX10		,
    D_MIN_RPM_MPHX10		,
    D_MAX_RPM_KPHX10		,	
    D_MIN_RPM_KPHX10		,
    
    D_MAX_INCLINEX10		,
    D_MIN_INCLINEX10		,
    D_MAX_INCLINE_ADC		,
    D_MIN_INCLINE_ADC		,
    
    D_MAX_RESISTANCE		,
    D_MIN_RESISTANCE		,
    
    D_FORCE_UPDATE			,
    D_FIRMWARE_AVAILANLE		,
    D_DEMO_MODE_ON			,
    D_ENERGY_SAVER_ON		,
    D_BUZZER_ON			,
    D_RTC_PRESET			,
    D_24HOURS_FORMAT		,
    D_ACCUMULATED_AGING_HOURS	,
    DEFAULT_VOLUME			,
    DEFAULT_BRIGHTNESS		,
    DEFAULT_AGE			,
    DEFAULT_WEIGHT_STANDARD		,
    DEFAULT_WEIGHT_METRIC		,
    
    
    D_CSAFE_ID			,
    D_CSAFE_MANU_FACTURE_ID		,
    D_CSAFE_MODE_ID			,
    
    D_CLUB_ID			,
    D_AUTO_LOGIN			,
    
    D_AP_MODE_ON			,
    
    D_RF_CHANNEL			,
    D_RF_ADDRESS			,
    D_DEFAULT_ERP_WAITTIME              ,
    D_DEFAULT_DEMO_WAITTIME             ,
	D_MAX_AGE			,
	D_MIN_AGE          ,
	D_TARGETHR		
};


#endif
