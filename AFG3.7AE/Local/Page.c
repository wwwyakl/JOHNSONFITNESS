#include "page.h"
#include "Events_Handler.h"
#include "Programs.h"
#include "main.h"
#include <stdio.h>
#include <string.h>

#define MODAL_WIN_TOAL                          3
#define MAX_OVERLAPPED_WNDS   	                5
//#define TM
//#define EP
__no_init VR_PAGE  nCurrentPage;
__no_init Matrix_Type martix_type;
typedef struct 
{
    UINT8 _OverlappedWinCnt;
    Win_Items _OverlappedWindows[MODAL_WIN_TOAL]; 
}OVERLAPPINGWIN;

typedef struct
{
    Win_Items* _ActiveWnd;
    OVERLAPPINGWIN _OverlappedWndGroup;
}OVERLAPPINGWINGROUP;

__no_init static OVERLAPPINGWINGROUP g_OverlappedWindows[MAX_OVERLAPPED_WNDS];
__no_init static UINT8 g_OverlappedWndIndex;

//virtual windows -----------BY BRIAN WANG 8.12.2015-----
__no_init Win_Items g_wndLeftTopTextBox;//0,0
__no_init Win_Items g_wndLeftTopNumerical;//0,8
__no_init Win_Items g_wndRightTopTextBox;//0,1
__no_init Win_Items g_wndRightTopNumerical;//0,10
__no_init Win_Items g_wndLeftBottomTextBox;//0,4
__no_init Win_Items g_wndLeftBottomNumerical;//0,9
__no_init Win_Items g_wndRightBottomTextBox;//0,5
__no_init Win_Items g_wndRightBottomNumerical;//0,11
__no_init Win_Items g_wndMiddleBottomTextBox;//
__no_init Win_Items g_wndBottomTextBox;//0,13
__no_init Win_Items g_wndMiddleMatrixBox;//0,6
__no_init Win_Items g_wndMiddleNumerical;//
__no_init Win_Items g_wndRpmIcon;
__no_init Win_Items g_wndTimeIcon;
__no_init Win_Items g_wndDistanceIcon;
__no_init Win_Items g_wndCaloriesIcon;
__no_init Win_Items g_wndAFGLOGOIcon;
__no_init Win_Items g_wndInclineIcon;//0,2
__no_init Win_Items g_wndLapsIcon;//0,3
__no_init Win_Items g_wndLapsCircleIcon;//0,12
__no_init Win_Items g_wndLEDTextBox;//1,1
__no_init Win_Items g_wndLED1TextBox;//2,0
__no_init Win_Items g_wndLED2TextBox;//2,1
__no_init Win_Items g_wndLED3TextBox;//2,2
__no_init Win_Items g_wndLED4TextBox;//2,3
__no_init Win_Items g_wndLED5TextBox;//2,4


//Define all possible window default values
window_def const   INITIAL_TITLE_WND      = {&g_wndBottomTextBox, {0,13,0,STR_INITIAL_ID,0,0,ShowCommonTextBox,0,NULL,0}};

//use for auto model
//window_def const   AUTO_MODEL_WND         = {&g_wndBottomTextBox, {1,1,0,STR_TF50_ID,0,0,ShowCommonTextBox,0,NULL,0}};//Bottom_Matrix


//use for select user page
window_def const   SELECTUSER_TITLE_WND   = {&g_wndBottomTextBox, {0,13,0,STR_SELECTUSER_ID,0,0,ShowBottomTextBox,0,NULL,0}};//Bottom_Matrix
//window_def const   SELECTUSER_SUB1_WND    = {&g_wndRightTopTextBox, {1,1,0,STR_MALE_ID,0,0,NULL,0,NULL,0}};
//window_def const   SELECTUSER_SUB2_WND    = {&g_wndRightTopNumerical, {1,1,0,NULL,0,0,NULL,0,NULL,0}};
//window_def const   LEFT_TOP_TXT_WND       = {&g_wndLeftTopTextBox, {1,0,0,STR_MALE_ID,0,1,0,NULL}};
//window_def const   RIGHT_TOP_TXT_WND      = {&g_wndRightTopNumerical, {8,1,0,NULL,0,1,0,NULL}};

// for eng
//window_def const   ENG_TITLE_DISPLAY          = {&g_wndBottomTextBox,{0,3,NULL, STR_TITLE_DISPLAY_ID,0,0,ShowCommonTextBox,0,NULL,0}};

//matrix
window_def const   MATRIX_DATA_WND        = {&g_wndMiddleMatrixBox,{0,6,0, NULL,0,0,ShowMatrixData,0,NULL,0}};//Middle_Matrix

//titles
window_def const   AFGLOGO_TITLE_WND      = {&g_wndAFGLOGOIcon, {0,7,1, NULL,0,0,ShowAFGLOGOTitle,0,NULL,0}};//LeftTopTextBox
window_def const   LCD_RPM_DATA_WND       = {&g_wndRpmIcon, {0,14,0, NULL,0,0,ShowRpmData,0,NULL,0}};
//WWW window_def const   LCD_TIME_DATA_WND      = {&g_wndTimeIcon, {0,15,0, NULL,0,0,ShowLcdElapsedTimeData,0,NULL,0}};
window_def const   LCD_TIME_DATA_WND      = {&g_wndTimeIcon, {0,15,0, NULL,0,0,ShowTimeData,0,NULL,0}};//WWW
window_def const   LCD_Distance_DATA_WND  = {&g_wndDistanceIcon, {0,16,0, NULL,0,0,ShowLcdDistanceData,0,NULL,0}};
window_def const   LCD_Calories_DATA_WND  = {&g_wndCaloriesIcon, {0,17,0, NULL,0,0,ShowLcdCaloriesData,0,NULL,0}};
window_def const   CAL_TITLE_WND          = {&g_wndLeftBottomTextBox, {0,4,NULL, STR_CALORIES_TITLE_ID,0,0,ShowCaloriesTitle,0,NULL,0}};//LeftTopTextBox
window_def const   CALPERHOUR_DATA_LCDWND = {&g_wndLeftBottomNumerical,{0,9,0, NULL,0,0,Show_Calories_PerHour,0,NULL,0}};

window_def const   LAPSCIRCLE_WND = {&g_wndLapsCircleIcon,{0,12,0, NULL,0xaa,0,ShowLapsCircle,0,NULL,0}};

window_def const   HEART_TITLE_WND        = {&g_wndRightBottomTextBox, {0,5,5, STR_HR_TITLE_ID,0,0,ShowHeartTitle,0,NULL,0}};//LeftTopTextBox

window_def const   WATTS_TITLE_WND        = {&g_wndLeftTopTextBox, {0,0,1, STR_WATTS_TITLE_ID,0,0,ShowWattsTitle,0,NULL,0}};//RightTopTextBox
window_def const   PACE_TITLE_WND         = {&g_wndRightTopTextBox, {0,1,3, STR_PACE_TITLE_ID,0,0,ShowPaceTitle,0,NULL,0}};//RightTopTextBox

//window_def const   MILE_TITLE_WND         = {&g_wndRightTopTextBox,{41,0,NULL,STR_METRIC_ID,0,0,ShowMileTitle,0,NULL,0}};//Right_Above_Matrix
//window_def const   DIS_TITLE_WND          = {&g_wndRightTopTextBox,{41,0,NULL,STR_MILE_TITLE_ID,0,0,ShowDistanceTitle,0,NULL,0}};//Right_Above_Matrix

//window_def const   METS_TITLE_WND         = {&g_wndLeftBottomTextBox,{0,6,NULL, STR_METS_TITLE_ID,0,0,ShowMetsTitle,0,NULL,0}};//LeftBottomTextBox
//window_def const   RPM_TITLE_WND          = {&g_wndRightBottomTextBox,{0,7,NULL, STR_RPM_TITLE_ID,0,0,ShowRpmTitle,0,NULL,0}};//LeftBottomTextBox
//window_def const   INCLINE_TITLE_WND      = {&g_wndLeftBottomTextBox,{0,6,NULL, STR_INCLINE_TITLE_ID,0,0,ShowInclineTitle,0,NULL,0}};//LeftBottomTextBox
//window_def const   SPEED_TITLE_WND        = {&g_wndRightBottomTextBox,{0,7,NULL, STR_SPEED_TITLE_ID,0,0,ShowSpeedTitle,0,NULL,0}};//LeftBottomTextBox
//window_def const   RES_TITLE_WND          = {&g_wndRightBottomTextBox,{0,7,NULL, STR_RESISTANCE_TITLE_ID,0,0,ShowResistanceTitle,0,NULL,0}};//LeftBottomTextBox
//window_def const   BK_RPM_TITLE_WND       = {&g_wndLeftBottomTextBox,{0,6,NULL, STR_RPM_TITLE_ID,0,0,ShowRpmTitle,0,NULL,0}};//LeftBottomTextBox
//window_def const   BK_INCLINE_TITLE_WND   = {&g_wndLeftBottomTextBox,{0,7,NULL, STR_INCLINE_TITLE_ID,0,0,ShowInclineTitle,0,NULL,0}};//RightBottomTextBox for BK

//data
window_def const   SET_TIME_WND           = {&g_wndLEDTextBox,{1,1,0, NULL,0,0,ShowSetTime,0,NULL,0}};
//WWW window_def const   TIME_DATA_WND          = {&g_wndLED1TextBox,{2,0,1, NULL,0,0,ShowElapsedTimeData,0,NULL,0}};//MiddleNumerical
window_def const   TIME_DATA_WND          = {&g_wndLED1TextBox,{2,0,1, NULL,0,0,ShowTimeData,0,NULL,0}};//MiddleNumerical   //WWW
window_def const   COUNTTIME_DATA_WND     = {&g_wndLED1TextBox,{2,0,0, NULL,0,0,ShowCountTimeData,0,NULL,0}};//MiddleNumerical

window_def const   CAL_DATA_WND           = {&g_wndLED1TextBox,{2,0,0, NULL,0,0,ShowCaloriesData,0,NULL,0}};//LeftTopNumerical
window_def const   HEART_DATA_WND         = {&g_wndLED3TextBox,{2,2,0, NULL,0,0,ShowHeartData,0,NULL,0}};
window_def const   HEART_DATA_LCDWND      = {&g_wndRightBottomNumerical,{0,11,0, NULL,0,0,ShowHeartData,0,NULL,0}};
//window_def const   TARGET_HEART_DATA_WND  = {&g_wndLeftTopNumerical,{0,1,0, NULL,0,0,ShowTargetHeartData,0,NULL,0}};//LeftTopNumerical

window_def const   WATTS_DATA_LCDWND      = {&g_wndLeftTopNumerical,{0,8,0, NULL,0,0,ShowWattsData,0,NULL,0}};
window_def const   DIS_DATA_WND           = {&g_wndLED3TextBox,{2,2,1, NULL,0,0,ShowDistanceData,0,NULL,0}};//RightTopNumerical
window_def const   PACE_DATA_WND          = {&g_wndLED3TextBox,{2,2,0, NULL,0,0,ShowPaceData,0,NULL,0}};//RightTopNumerical
window_def const   PACE_DATA_LCDWND       = {&g_wndRightTopNumerical,{0,10,0, NULL,0,0,ShowPace_LcdData,0,NULL,0}};//RightTopNumerical

//window_def const   SET_INCLINE_WND        = {&g_wndLeftBottomNumerical,{1,1,0, NULL,0,0,ShowSetIncline,0,NULL,0}};
window_def const   INCLINE_DATA_WND       = {&g_wndLED2TextBox,{2,1,1, NULL,0,0,ShowInclineData,0,NULL,0}};
window_def const   INCLINE_DATA_LCDWND    = {&g_wndInclineIcon,{0,2,1, NULL,0,0,ShowIncline_LCDData,0,NULL,0}};

//window_def const   METS_DATA_WND          = {&g_wndLeftBottomNumerical,{12,1,0, NULL,0,0,ShowMetsData,0,NULL,0}};//LeftBottomNumerical
//window_def const   RPM_DATA_WND           = {&g_wndLeftBottomNumerical,{12,1,0, NULL,0,0,ShowRpmData,0,NULL,0}};//LeftBottomNumerical
//window_def const   AS_RPM_DATA_WND        = {&g_wndRightBottomNumerical,{16,1,0, NULL,0,0,ShowRpmData,0,NULL,0}};//LeftBottomNumerical
//window_def const   PROGRAMBL_DATA_WND       = {&g_wndLED5TextBox,{2,4,0, NULL,0,0,ShowProgramBLData,0,NULL,0}};//RightBottomNumerical
window_def const   SPEED_DATA_LCDWND         = {&g_wndRightTopNumerical,{0,10,0, NULL,0,0,ShowLcdSpeedData,0,NULL,0}};//RightBottomNumerical

window_def const   SPEED_DATA_WND         = {&g_wndLED2TextBox,{2,1,1, NULL,0,0,ShowSpeedData,0,NULL,0}};//RightBottomNumerical
window_def const   RPM_DATA_WND         = {&g_wndLED2TextBox,{2,1,1, NULL,0,0,ShowRpmData,0,NULL,0}};//RightBottomNumerical
window_def const   RES_DATA_WND           = {&g_wndLED2TextBox,{2,1,0, NULL,0,0,ShowResistanceData,0,NULL,0}};//RightBottomNumerical
//window_def const   LEVEL_DATA_WND         = {&g_wndRightBottomNumerical,{16,1,0, NULL,0,0,ShowLevelData,0,NULL,0}};//RightBottomNumerical
//window_def const   AGE_DATA_WND           = {&g_wndRightBottomNumerical,{16,1,0, NULL,0,0,ShowAgeData,0,NULL,0}};
//window_def const   WEIGHT_DATA_WND        = {&g_wndRightBottomNumerical,{1,1,0, NULL,0,0,ShowWeightData,0,NULL,0}};
//window_def const   WEIGHT_UNIT_WND        = {&g_wndRightTopTextBox,{1,0,NULL, STR_LBS_ID,0,0,ShowWeightUnit,0,NULL,0}};
//window_def const   TARGET_WATTS_WND       = {&g_wndRightTopNumerical,{8,1,0, NULL,0,0,ShowTargetWattsData,0,NULL,0}};//RightTopNumerical

window_def const   BOTTOM_TEXT_WND        = {&g_wndBottomTextBox, {0,13,0,STR_NULL_ID,0,0,ShowBottomTextBox,1,NULL,0}};
//window_def const   MIDDLE_INFO_WND       = {&g_wndMiddleBottomTextBox,{0,8,NULL, STR_NULL_ID,0,0,ShowBottomTextBox,1,NULL,0}};//LeftBottomTextBox

window_def const   SAFEKEY_TEXT_WND       = {&g_wndBottomTextBox, {0,13,0,STR_SAFEKEY_OFF_ID,0,0,ShowCommonTextBox,0,NULL,0}};
window_def const   SAFEKEY_DATA1_WND       = {&g_wndLED1TextBox,{2,0,0, NULL,0,0,ShowSafekeyOffLedNumber,0,NULL,0}};    //zsf
window_def const   SAFEKEY_DATA2_WND       = {&g_wndLED2TextBox,{2,1,0, NULL,0,0,ShowSafekeyOffLedNumber,0,NULL,0}};    //zsf
window_def const   SAFEKEY_DATA3_WND       = {&g_wndLED3TextBox,{2,2,0, NULL,0,0,ShowSafekeyOffLedNumber,0,NULL,0}};    //zsf
//RF icon
//window_def const   RF_DATA_WND            = {&g_wndRFIcon,{0,4,0, NULL,0,0,ShowRFICON,0,NULL,0}};//RFIcon
//window_def const   WIFI_DATA_WND          = {&g_wndWiFiIcon,{0,5,0, NULL,0,0,ShowWifiIcon,0,NULL,0}};//WIFIIcon



// Define all pages with default window settings
PAGES const pages[PAGE_TOTAL] = {
    /*1*/{1, 0, 0,&INITIAL_TITLE_WND},//&RF_DATA_WND,&WIFI_DATA_WND},//initial
//    /*2*/{1, 0, 0,&MILE_TITLE_WND},//&RF_DATA_WND,&WIFI_DATA_WND},//versions
//    /*3*/ {1, 0, 0,&INITIAL_TITLE_WND},//&RF_DATA_WND,&WIFI_DATA_WND},//PAGE_AOTO_MODELS
         {0, 0, 0,},//AGINGTEST
#ifdef TM
    /*4*/{9, 0, 0,&SELECTUSER_TITLE_WND,&INCLINE_DATA_LCDWND,&TIME_DATA_WND,&HEART_DATA_LCDWND,&MATRIX_DATA_WND,&SPEED_DATA_LCDWND,&SPEED_DATA_WND,&DIS_DATA_WND,&LAPSCIRCLE_WND,},//select user
    /*5*/{9, 0, 0,&BOTTOM_TEXT_WND,&LAPSCIRCLE_WND,&INCLINE_DATA_LCDWND,&HEART_DATA_LCDWND,&MATRIX_DATA_WND,&SPEED_DATA_LCDWND,&TIME_DATA_WND,&SPEED_DATA_WND,&DIS_DATA_WND,},//PAGE_PROGRAM
#elif  (defined AE)
    /*4*/{9, 0, 0,&RPM_DATA_WND,&SELECTUSER_TITLE_WND,&INCLINE_DATA_LCDWND,&TIME_DATA_WND,&HEART_DATA_LCDWND,&MATRIX_DATA_WND,&SPEED_DATA_LCDWND,&DIS_DATA_WND,&LAPSCIRCLE_WND,},//select user
    /*5*/{9, 0, 0,&RPM_DATA_WND,&BOTTOM_TEXT_WND,&LAPSCIRCLE_WND,&INCLINE_DATA_LCDWND,&HEART_DATA_LCDWND,&MATRIX_DATA_WND,&SPEED_DATA_LCDWND,&TIME_DATA_WND,&DIS_DATA_WND,},//PAGE_PROGRAM
#else
    /*4*/{8, 0, 0,&RPM_DATA_WND,&SELECTUSER_TITLE_WND,&TIME_DATA_WND,&HEART_DATA_LCDWND,&MATRIX_DATA_WND,&SPEED_DATA_LCDWND,&DIS_DATA_WND,&LAPSCIRCLE_WND,},//select user
    /*5*/{8, 0, 0,&RPM_DATA_WND,&BOTTOM_TEXT_WND,&LAPSCIRCLE_WND,&HEART_DATA_LCDWND,&MATRIX_DATA_WND,&SPEED_DATA_LCDWND,&TIME_DATA_WND,&DIS_DATA_WND,},//PAGE_PROGRAM
#endif
    /*6*/{0, 0, 0,},//DEMO,ERP
    
    /*7*/{11, 0, 0,&TIME_DATA_WND,&DIS_DATA_WND,&SPEED_DATA_WND,&LCD_TIME_DATA_WND,&INCLINE_DATA_LCDWND,&HEART_DATA_LCDWND,&MATRIX_DATA_WND,&LCD_Distance_DATA_WND,&LCD_Calories_DATA_WND,&SPEED_DATA_LCDWND,&LAPSCIRCLE_WND,},//work tm1
        
    /*8*/{11, 0, 0,&CAL_DATA_WND,&HEART_DATA_WND,&INCLINE_DATA_WND,&LCD_TIME_DATA_WND,&INCLINE_DATA_LCDWND,&HEART_DATA_LCDWND,&MATRIX_DATA_WND,&LCD_Distance_DATA_WND,&LCD_Calories_DATA_WND,&SPEED_DATA_LCDWND,&LAPSCIRCLE_WND,},//work tm2
            
    /*9*/{11, 0, 0,&SPEED_DATA_LCDWND,&RPM_DATA_WND,&LCD_Calories_DATA_WND,&LCD_Distance_DATA_WND,&LCD_TIME_DATA_WND,&TIME_DATA_WND,&DIS_DATA_WND,&HEART_DATA_LCDWND,&LAPSCIRCLE_WND,&MATRIX_DATA_WND,&BOTTOM_TEXT_WND},//work EP1
        
    /*10*/{13, 0, 0,&LCD_TIME_DATA_WND,&CAL_DATA_WND,&SPEED_DATA_LCDWND,&RPM_DATA_WND,&LCD_Calories_DATA_WND,&LCD_Distance_DATA_WND,&RES_DATA_WND,&HEART_DATA_WND,&HEART_TITLE_WND,&HEART_DATA_LCDWND,&LAPSCIRCLE_WND,&MATRIX_DATA_WND,&BOTTOM_TEXT_WND},//work EP2
 {0, 0, 0,},//AGINGTEST
  {0, 0, 0,},//AGINGTEST
     /*14*/ {4, 0, 0,&SAFEKEY_TEXT_WND,&SAFEKEY_DATA1_WND,&SAFEKEY_DATA2_WND,&SAFEKEY_DATA3_WND},//     PAGE_ENG0_0_DisplayTest,     //zsf
};

void Page_Init_Data(void)
{
    nCurrentPage = PAGE_INITAL;
	if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8) == BIKE)	
    	martix_type =   Show_Resistance;
	else
    	martix_type =   Show_Speed;	
	
    g_wndLEDTextBox._x = 1;
    g_wndLEDTextBox._y = 1;
    g_wndLEDTextBox._z = NULL;
    g_wndLEDTextBox._string_id = STR_NULL_ID;
    g_wndLEDTextBox._blinkCount = 0 ;
    g_wndLEDTextBox._activetime = 0 ;
    g_wndLEDTextBox.pfShow = NULL ;
    g_wndLEDTextBox._isHidden = 0 ;
    g_wndLEDTextBox._blinkDelayCount = 0 ;
    
    g_wndLED1TextBox._x = 2;
    g_wndLED1TextBox._y = 0;
    g_wndLED1TextBox._z = 1;
    g_wndLED1TextBox._string_id = STR_NULL_ID;
    g_wndLED1TextBox._blinkCount = 0 ;
    g_wndLED1TextBox._activetime = 0 ;
    g_wndLED1TextBox.pfShow = NULL ;
    g_wndLED1TextBox._isHidden = 0 ;
    g_wndLED1TextBox._blinkDelayCount = 0 ;
    
    g_wndLED2TextBox._x = 2;
    g_wndLED2TextBox._y = 1;
    g_wndLED2TextBox._z = 1;
    g_wndLED2TextBox._string_id = STR_NULL_ID;
    g_wndLED2TextBox._blinkCount = 0 ;
    g_wndLED2TextBox._activetime = 0 ;
    g_wndLED2TextBox.pfShow = NULL ;
    g_wndLED2TextBox._isHidden = 0 ;
    g_wndLED2TextBox._blinkDelayCount = 0 ;
    
    g_wndLED3TextBox._x = 2;
    g_wndLED3TextBox._y = 2;
    g_wndLED3TextBox._z = NULL;
    g_wndLED3TextBox._string_id = STR_NULL_ID;
    g_wndLED3TextBox._blinkCount = 0 ;
    g_wndLED3TextBox._activetime = 0 ;
    g_wndLED3TextBox.pfShow = NULL ;
    g_wndLED3TextBox._isHidden = 0 ;
    g_wndLED3TextBox._blinkDelayCount = 0 ;
    
    g_wndLED4TextBox._x = 2;
    g_wndLED4TextBox._y = 3;
    g_wndLED4TextBox._z = NULL;
    g_wndLED4TextBox._string_id = STR_NULL_ID;
    g_wndLED4TextBox._blinkCount = 0 ;
    g_wndLED4TextBox._activetime = 0 ;
    g_wndLED4TextBox.pfShow = NULL ;
    g_wndLED4TextBox._isHidden = 0 ;
    g_wndLED4TextBox._blinkDelayCount = 0 ;
    
    g_wndLED5TextBox._x = 2;
    g_wndLED5TextBox._y = 4;
    g_wndLED5TextBox._z = NULL;
    g_wndLED5TextBox._string_id = STR_NULL_ID;
    g_wndLED5TextBox._blinkCount = 0 ;
    g_wndLED5TextBox._activetime = 0 ;
    g_wndLED5TextBox.pfShow = NULL ;
    g_wndLED5TextBox._isHidden = 0 ;
    g_wndLED5TextBox._blinkDelayCount = 0 ;
    
    g_wndLeftTopTextBox._x = 0;
    g_wndLeftTopTextBox._y = 0;
    g_wndLeftTopTextBox._z = 1;
    g_wndLeftTopTextBox._string_id = STR_NULL_ID;
    g_wndLeftTopTextBox._blinkCount = 0 ;
    g_wndLeftTopTextBox._activetime = 0 ;
    g_wndLeftTopTextBox.pfShow = NULL ;
    g_wndLeftTopTextBox._isHidden = 0 ;
    g_wndLeftTopTextBox._blinkDelayCount = 0 ;
    
    g_wndLeftTopNumerical._x = 0;
    g_wndLeftTopNumerical._y = 8;
    g_wndLeftTopNumerical._z = 0;
    g_wndLeftTopNumerical._string_id = STR_NULL_ID;
    g_wndLeftTopNumerical._blinkCount = 0 ;
    g_wndLeftTopNumerical._activetime = 0 ;
    g_wndLeftTopNumerical.pfShow = NULL ;
    g_wndLeftTopNumerical._isHidden = 0 ;
    g_wndLeftTopNumerical._blinkDelayCount = 0 ;
    
    g_wndRightTopTextBox._x = 0;
    g_wndRightTopTextBox._y = 1;
    g_wndRightTopTextBox._z = 3;
    g_wndRightTopTextBox._string_id = STR_NULL_ID;
    g_wndRightTopTextBox._blinkCount = 0 ;
    g_wndRightTopTextBox._activetime = 0 ;
    g_wndRightTopTextBox.pfShow = NULL ;
    g_wndRightTopTextBox._isHidden = 0 ;
    g_wndRightTopTextBox._blinkDelayCount = 0 ;

    g_wndRightTopNumerical._x = 0;
    g_wndRightTopNumerical._y = 10;
    g_wndRightTopNumerical._z = 0;
    g_wndRightTopNumerical._string_id = STR_NULL_ID;
    g_wndRightTopNumerical._blinkCount = 0 ;
    g_wndRightTopNumerical._activetime = 0 ;
    g_wndRightTopNumerical.pfShow = NULL ;
    g_wndRightTopNumerical._isHidden = 0 ;
    g_wndRightTopNumerical._blinkDelayCount = 0 ;

    g_wndLeftBottomTextBox._x = 0;
    g_wndLeftBottomTextBox._y = 4;
    g_wndLeftBottomTextBox._z = NULL;
    g_wndLeftBottomTextBox._string_id = STR_NULL_ID;
    g_wndLeftBottomTextBox._blinkCount = 0 ;
    g_wndLeftBottomTextBox._activetime = 0 ;
    g_wndLeftBottomTextBox.pfShow = NULL ;
    g_wndLeftBottomTextBox._isHidden = 0 ;
    g_wndLeftBottomTextBox._blinkDelayCount = 0 ;
    
    g_wndLeftBottomNumerical._x = 0;
    g_wndLeftBottomNumerical._y = 9;
    g_wndLeftBottomNumerical._z = NULL;
    g_wndLeftBottomNumerical._string_id = STR_NULL_ID;
    g_wndLeftBottomNumerical._blinkCount = 0 ;
    g_wndLeftBottomNumerical._activetime = 0 ;
    g_wndLeftBottomNumerical.pfShow = NULL ;
    g_wndLeftBottomNumerical._isHidden = 0 ;
    g_wndLeftBottomNumerical._blinkDelayCount = 0 ;
    
    g_wndRightBottomTextBox._x = 0;
    g_wndRightBottomTextBox._y = 5;
    g_wndRightBottomTextBox._z = NULL;
    g_wndRightBottomTextBox._string_id = STR_NULL_ID;
    g_wndRightBottomTextBox._blinkCount = 0 ;
    g_wndRightBottomTextBox._activetime = 0 ;
    g_wndRightBottomTextBox.pfShow = NULL ;
    g_wndRightBottomTextBox._isHidden = 0 ;
    g_wndRightBottomTextBox._blinkDelayCount = 0 ;
    
    
    g_wndRightBottomNumerical._x = 0;
    g_wndRightBottomNumerical._y = 11;
    g_wndRightBottomNumerical._z = NULL;
    g_wndRightBottomNumerical._string_id = STR_NULL_ID;
    g_wndRightBottomNumerical._blinkCount = 0 ;
    g_wndRightBottomNumerical._activetime = 0 ;
    g_wndRightBottomNumerical.pfShow = NULL ;
    g_wndRightBottomNumerical._isHidden = 0 ;
    g_wndRightBottomNumerical._blinkDelayCount = 0 ;
    
    g_wndBottomTextBox._x = 0;
    g_wndBottomTextBox._y = 13;
    g_wndBottomTextBox._z = 1;
    g_wndBottomTextBox._string_id = STR_NULL_ID;
    g_wndBottomTextBox._blinkCount = 0;
    g_wndBottomTextBox._activetime = 0;
    g_wndBottomTextBox.pfShow = NULL;
    g_wndBottomTextBox._isHidden = 0;
    g_wndBottomTextBox._blinkDelayCount = 0;
	Page_Reset_All_ModalWnds();
}


void Page_Push_OverlappedWindow(Win_Items* pSrcWnd, Win_Items *pWnd)
{
    EFM_ASSERT(pWnd != NULL && pSrcWnd != NULL);
    bool isWndExist = false;
    for(UINT i = 0; i < MAX_OVERLAPPED_WNDS; i++)
    {
    	if(g_OverlappedWindows[i]._ActiveWnd == pSrcWnd)
    	{
	    if(g_OverlappedWindows[i]._OverlappedWndGroup._OverlappedWinCnt < MODAL_WIN_TOAL)
    {
                memcpy(&g_OverlappedWindows[i]._OverlappedWndGroup._OverlappedWindows[g_OverlappedWindows[i]._OverlappedWndGroup._OverlappedWinCnt],pWnd, sizeof(Win_Items));
                g_OverlappedWindows[i]._OverlappedWndGroup._OverlappedWinCnt++;
    }
	    isWndExist = true;
	    break;
}
    }
    if(isWndExist == false)
    {
    	if(g_OverlappedWndIndex < MAX_OVERLAPPED_WNDS)
{
            g_OverlappedWindows[g_OverlappedWndIndex]._ActiveWnd = pSrcWnd;
            memcpy(&g_OverlappedWindows[g_OverlappedWndIndex]._OverlappedWndGroup._OverlappedWindows[0], pWnd,sizeof(Win_Items));
            g_OverlappedWindows[g_OverlappedWndIndex]._OverlappedWndGroup._OverlappedWinCnt = 1;
            g_OverlappedWndIndex++;
    	}
    }
}

Win_Items* Page_Pop_OverlappedWindow(Win_Items* pSrcWnd, UINT8* cnt)
{
    EFM_ASSERT(pSrcWnd != NULL && cnt != NULL);
    for(UINT8 i = 0; i < MAX_OVERLAPPED_WNDS; i++)
    
    {
        if(g_OverlappedWindows[i]._ActiveWnd == pSrcWnd)
        {
            *cnt = g_OverlappedWindows[i]._OverlappedWndGroup._OverlappedWinCnt;
            return (g_OverlappedWindows[i]._OverlappedWndGroup._OverlappedWindows);
        }
    }
    *cnt = 0;
    return 0;
        }

bool Page_Get_Modal_State(void)
        {
    return( g_OverlappedWndIndex > 0);
        }

bool  Page_Reset_Modal_Window(Win_Items* pSrcWnd)
        {
    EFM_ASSERT(pSrcWnd != NULL);
    bool isWndExist = false;
//    for(UINT i = 0; i < g_OverlappedWndIndex; i++)
    for(UINT i = 0; i < MAX_OVERLAPPED_WNDS; i++)
    {
    	if(g_OverlappedWindows[i]._ActiveWnd == pSrcWnd)
    	{
            memset(&(g_OverlappedWindows[i]._OverlappedWndGroup),0, sizeof(OVERLAPPINGWIN));
            g_OverlappedWindows[i]._ActiveWnd = 0;
            g_OverlappedWndIndex--;
            isWndExist = true;
        break;
    	}
    }
    return isWndExist;
}

void Page_Reset_All_ModalWnds(void)
{
    g_OverlappedWndIndex = 0;
    for(UINT8 i = 0; i < MAX_OVERLAPPED_WNDS; i++){
    	memset(&g_OverlappedWindows[i],0, sizeof(OVERLAPPINGWINGROUP));
    }
}  

void ShowPage(VR_PAGE p, UINT8 isDefault)
{
    nCurrentPage  = p; 
        
    for(int i=0;i<(pages[nCurrentPage].nWindowCount);i++)
    {
        if(pages[nCurrentPage].pItem[i]->pWnd!=NULL && isDefault != 0)
        {
			pages[nCurrentPage].pItem[i]->pWnd->_x = pages[nCurrentPage].pItem[i]->defParam._x;
			pages[nCurrentPage].pItem[i]->pWnd->_y = pages[nCurrentPage].pItem[i]->defParam._y;
			pages[nCurrentPage].pItem[i]->pWnd->_z = pages[nCurrentPage].pItem[i]->defParam._z;
			pages[nCurrentPage].pItem[i]->pWnd->_string_id = pages[nCurrentPage].pItem[i]->defParam._string_id;
			pages[nCurrentPage].pItem[i]->pWnd->_blinkCount= pages[nCurrentPage].pItem[i]->defParam._blinkCount;	
			pages[nCurrentPage].pItem[i]->pWnd->_activetime= pages[nCurrentPage].pItem[i]->defParam._activetime;	
			pages[nCurrentPage].pItem[i]->pWnd->_isHidden = pages[nCurrentPage].pItem[i]->defParam._isHidden;
			pages[nCurrentPage].pItem[i]->pWnd->_blinkDelayCount= pages[nCurrentPage].pItem[i]->defParam._blinkDelayCount;
			pages[nCurrentPage].pItem[i]->pWnd->pfShow = pages[nCurrentPage].pItem[i]->defParam.pfShow;
        }
        if(pages[nCurrentPage].pItem[i]->pWnd->pfShow != NULL &&  pages[nCurrentPage].pItem[i]->pWnd->_isHidden==0)
			pages[nCurrentPage].pItem[i]->pWnd->pfShow((pages[nCurrentPage].pItem[i]->pWnd));
    }
}
