#include "page.h"
#include "Events_Handler.h"
#include "Programs.h"
#include "main.h"
#include <stdio.h>
#include <string.h>

#define MODAL_WIN_TOAL                          3
#define MAX_OVERLAPPED_WNDS   	                10


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
__no_init Win_Items g_wndLeftTopTextBox;//1,0
__no_init Win_Items g_wndLeftTopNumerical;//0,1
__no_init Win_Items g_wndRightTopTextBox;//41,0
__no_init Win_Items g_wndRightTopNumerical;//8,1
__no_init Win_Items g_wndLeftBottomTextBox;//0,6
__no_init Win_Items g_wndLeftBottomNumerical;//12,1
__no_init Win_Items g_wndRightBottomTextBox;//0,7
__no_init Win_Items g_wndMiddleBottomTextBox;//0,8
__no_init Win_Items g_wndRightBottomNumerical;//16,1
__no_init Win_Items g_wndBottomTextBox;//0,3
__no_init Win_Items g_wndMiddleMatrixBox;//0,2
__no_init Win_Items g_wndMiddleNumerical;//4,1
__no_init Win_Items g_wndRFIcon;//0,4
__no_init Win_Items g_wndWiFiIcon;//0,5
__no_init Win_Items g_wndFanIcon;

__no_init     UINT8 BLINK_DISPLAY_CHANNEL ;
__no_init     UINT8 STATIC_DISPLAY_CHANNEL;
__no_init     UINT8 MODAL_WINDOW_CHANNEL;
__no_init     UINT8 REFRESH_WINDOW_CHANNEL;
__no_init static UCHAR Timer_500msHigh_Level;

//Define all possible window default values
window_def const   INITIAL_TITLE_WND      = {&g_wndBottomTextBox, {0,3,0,STR_INITIAL_ID,0,0,ShowCommonTextBox,0,NULL,0}};

//use for auto model
window_def const   AUTO_MODEL_WND         = {&g_wndBottomTextBox, {0,3,0,STR_TF50_ID,0,0,ShowCommonTextBox,0,NULL,0}};//Bottom_Matrix


//use for select user page
window_def const   SELECTUSER_TITLE_WND   = {&g_wndBottomTextBox, {0,3,0,STR_SELECT_USER_ID,0,0,ShowCommonTextBox,0,NULL,0}};//Bottom_Matrix
window_def const   SELECTUSER_SUB1_WND    = {&g_wndRightTopTextBox, {41,0,0,STR_MALE_ID,0,0,NULL,0,NULL,0}};
window_def const   SELECTUSER_SUB2_WND    = {&g_wndRightTopNumerical, {8,1,0,NULL,0,0,NULL,0,NULL,0}};
window_def const   LEFT_TOP_TXT_WND       = {&g_wndLeftTopTextBox, {1,0,0,STR_MALE_ID,0,1,0,NULL}};
window_def const   RIGHT_TOP_TXT_WND      = {&g_wndRightTopNumerical, {8,1,0,NULL,0,1,0,NULL}};

// for eng
window_def const   ENG_TITLE_DISPLAY          = {&g_wndBottomTextBox,{0,3,NULL, STR_DISPLAY_TEST_ID,0,0,ShowCommonTextBox,0,NULL,0}};

//matrix
window_def const   MATRIX_DATA_WND        = {&g_wndMiddleMatrixBox,{0,2,0, NULL,0,0,ShowMatrixData,0,NULL,0}};//Middle_Matrix

//titles
window_def const   CAL_TITLE_WND          = {&g_wndLeftTopTextBox, {1,0,NULL, STR_CALORIES_ID,0,0,ShowCaloriesTitle,0,NULL,0}};//LeftTopTextBox
window_def const   HEART_TITLE_WND        = {&g_wndLeftTopTextBox, {1,0,NULL, STR_HR_ID,0,0,ShowHeartTitle,0,NULL,0}};//LeftTopTextBox

window_def const   WATTS_TITLE_WND        = {&g_wndRightTopTextBox, {41,0,NULL, STR_WATTS_ID,0,0,ShowWattsTitle,0,NULL,0}};//RightTopTextBox
window_def const   PACE_TITLE_WND         = {&g_wndRightTopTextBox, {41,0,NULL, STR_PACE_ID,0,0,ShowPaceTitle,0,NULL,0}};//RightTopTextBox

window_def const   MILE_TITLE_WND         = {&g_wndRightTopTextBox,{41,0,NULL,STR_METRIC_ID,0,0,ShowMileTitle,0,NULL,0}};//Right_Above_Matrix
window_def const   DIS_TITLE_WND          = {&g_wndRightTopTextBox,{41,0,NULL,STR_MILES_ID,0,0,ShowDistanceTitle,0,NULL,0}};//Right_Above_Matrix

window_def const   METS_TITLE_WND         = {&g_wndLeftBottomTextBox,{0,6,NULL, STR_METS_ID,0,0,ShowMetsTitle,0,NULL,0}};//LeftBottomTextBox
window_def const   RPM_TITLE_WND          = {&g_wndRightBottomTextBox,{0,7,NULL, STR_RPM_ID,0,0,ShowRpmTitle,0,NULL,0}};//LeftBottomTextBox
window_def const   INCLINE_TITLE_WND      = {&g_wndLeftBottomTextBox,{0,6,NULL, STR_INCLINE_ID,0,0,ShowInclineTitle,0,NULL,0}};//LeftBottomTextBox
window_def const   SPEED_TITLE_WND        = {&g_wndRightBottomTextBox,{0,7,NULL, STR_SPEED_ID,0,0,ShowSpeedTitle,0,NULL,0}};//LeftBottomTextBox
window_def const   RES_TITLE_WND          = {&g_wndRightBottomTextBox,{0,7,NULL, STR_RESISTANCE_ID,0,0,ShowResistanceTitle,0,NULL,0}};//LeftBottomTextBox
window_def const   BK_RPM_TITLE_WND       = {&g_wndLeftBottomTextBox,{0,6,NULL, STR_RPM_ID,0,0,ShowRpmTitle,0,NULL,0}};//LeftBottomTextBox
window_def const   LEVEL_TITLE_WND        = {&g_wndLeftBottomTextBox,{0,6,NULL, STR_LEVEL_ID,0,0,ShowLevelTitle,0,NULL,0}};//LeftBottomTextBox
window_def const   STAIR_TITLE_WND        = {&g_wndRightBottomTextBox,{0,7,NULL, STR_STAIRS_ID,0,0,ShowStairsTitle,0,NULL,0}};//g_wndRightBottomTextBox
window_def const   CLIMBMILL_METS_TITLE_WND = {&g_wndRightTopTextBox,{41,0,NULL, STR_METS_ID,0,0,ShowMetsTitle,0,NULL,0}};//LeftBottomTextBox
window_def const   CLIMBMILL_SPM_TITLE_WND = {&g_wndLeftBottomTextBox,{0,6,NULL, STR_SPM_ID,0,0,ShowSPMTitle,0,NULL,0}};//LeftBottomTextBox
window_def const   CLIMBMILL_HEIGHT_TITLE_WND = {&g_wndRightBottomTextBox,{0,7,NULL, STR_HEIGHT_ID,0,0,ShowHeightTitle,0,NULL,0}};//LeftBottomTextBox
//data
window_def const   TIME_DATA_WND          = {&g_wndMiddleNumerical,{4,1,2, NULL,0,0,ShowTimeData,0,NULL,0}};//MiddleNumerical

window_def const   CAL_DATA_WND           = {&g_wndLeftTopNumerical,{0,1,0, NULL,0,0,ShowCaloriesData,0,NULL,0}};//LeftTopNumerical
window_def const   HEART_DATA_WND         = {&g_wndLeftTopNumerical,{0,1,0, NULL,0,0,ShowHeartData,0,NULL,0}};//LeftTopNumerical
window_def const   TARGET_HEART_DATA_WND  = {&g_wndLeftTopNumerical,{0,1,0, NULL,0,0,ShowTargetHeartData,0,NULL,0}};//LeftTopNumerical

window_def const   WATTS_DATA_WND         = {&g_wndRightTopNumerical,{8,1,0, NULL,0,0,ShowWattsData,0,NULL,0}};//RightTopNumerical
window_def const   DIS_DATA_WND           = {&g_wndRightTopNumerical,{8,1,1, NULL,0,0,ShowDistanceData,0,NULL,0}};//RightTopNumerical
window_def const   PACE_DATA_WND          = {&g_wndRightTopNumerical,{8,1,1, NULL,0,0,ShowPaceData,0,NULL,0}};//RightTopNumerical

window_def const   INCLINE_DATA_WND       = {&g_wndLeftBottomNumerical,{12,1,0, NULL,0,0,ShowInclineData,0,NULL,0}};//LeftBottomNumerical
window_def const   METS_DATA_WND          = {&g_wndLeftBottomNumerical,{12,1,0, NULL,0,0,ShowMetsData,0,NULL,0}};//LeftBottomNumerical
window_def const   CLIMBMILL_METS_DATA_WND = {&g_wndRightTopNumerical,{8,1,0, NULL,0,0,ShowMetsData,0,NULL,0}};//LeftBottomNumerical
window_def const   CLIMBMILL_SPM_DATA_WND = {&g_wndLeftBottomNumerical,{12,1,0, NULL,0,0,ShowSPMData,0,NULL,0}};//LeftBottomNumerical
window_def const   CLIMBMILL_HEIGHT_DATA_WND = {&g_wndRightBottomNumerical,{16,1,0, NULL,0,0,ShowHeightData,0,NULL,0}};//LeftBottomNumerical

window_def const   RPM_DATA_WND           = {&g_wndLeftBottomNumerical,{12,1,0, NULL,0,0,ShowRpmData,0,NULL,0}};//LeftBottomNumerical
window_def const   AS_RPM_DATA_WND        = {&g_wndRightBottomNumerical,{16,1,0, NULL,0,0,ShowRpmData,0,NULL,0}};//LeftBottomNumerical

window_def const   SPEED_DATA_WND         = {&g_wndRightBottomNumerical,{16,1,1, NULL,0,0,ShowSpeedData,0,NULL,0}};//RightBottomNumerical
window_def const   RES_DATA_WND           = {&g_wndRightBottomNumerical,{16,1,0, NULL,0,0,ShowResistanceData,0,NULL,0}};//RightBottomNumerical
window_def const   LEVEL_DATA_WND         = {&g_wndRightBottomNumerical,{16,1,0, NULL,0,0,ShowLevelData,0,NULL,0}};//RightBottomNumerical
window_def const   CLIMBMILL_LEVEL_DATA_WND = {&g_wndLeftBottomNumerical,{12,1,0, NULL,0,0,ShowResistanceData,0,NULL,0}};//RightBottomNumerical
window_def const   CLIMBMILL_STAIRS_DATA_WND = {&g_wndRightBottomNumerical,{16,1,0, NULL,0,0,ShowStairsData,0,NULL,0}};//RightBottomNumerical
window_def const   AGE_DATA_WND           = {&g_wndRightBottomNumerical,{16,1,0, NULL,0,0,ShowAgeData,0,NULL,0}};
window_def const   WEIGHT_DATA_WND        = {&g_wndRightBottomNumerical,{8,1,0, NULL,0,0,ShowWeightData,0,NULL,0}};
window_def const   WEIGHT_UNIT_WND        = {&g_wndRightTopTextBox,{41,0,NULL, STR_LBS_ID,0,0,ShowWeightUnit,0,NULL,0}};
window_def const   TARGET_WATTS_WND       = {&g_wndRightTopNumerical,{8,1,0, NULL,0,0,ShowTargetWattsData,0,NULL,0}};//RightTopNumerical

window_def const   BOTTOM_TEXT_WND        = {&g_wndBottomTextBox, {0,3,0,STR_NULL_ID,0,0,ShowBottomTextBox,1,NULL,0}};
window_def const   MIDDLE_INFO_WND        = {&g_wndMiddleBottomTextBox,{0,8,NULL, STR_NULL_ID,0,0,ShowBottomTextBox,1,NULL,0}};//LeftBottomTextBox
window_def const   SAFEKEY_TEXT_WND       = {&g_wndBottomTextBox, {0,3,0,STR_SAFETY_KEY_OFF_ID,0,0,ShowBottomTextBox,0,NULL,0}};
window_def const   ERRORCODE_INFO_WND       = {&g_wndBottomTextBox, {0,3,0,NULL,0,0,ShowErrorCode,0,NULL,0}};
window_def const   UPDATE_INFO_WND    = {&g_wndBottomTextBox, {0,3,0,STR_SOFTWARE_UPDATE_PRESS_ENTER_ID,0,0,ShowBottomTextBox,0,NULL,0}};
//RF icon
window_def const   RF_DATA_WND            = {&g_wndRFIcon,{0,4,0, NULL,0,0,ShowRFICON,0,NULL,0}};//RFIcon
window_def const   WIFI_DATA_WND          = {&g_wndWiFiIcon,{0,5,0, NULL,0,0,ShowWifiIcon,0,NULL,0}};//WIFIIcon



// Define all pages with default window settings
PAGES const pages[PAGE_TOTAL] = {
    /*1*/{3, 0, 0,&INITIAL_TITLE_WND,&RF_DATA_WND,&WIFI_DATA_WND},//initial
    /*2*/{3, 0, 0,&MILE_TITLE_WND,&RF_DATA_WND,&WIFI_DATA_WND},//versions
    /*3*/ {3, 0, 0,&INITIAL_TITLE_WND,&RF_DATA_WND,&WIFI_DATA_WND},//PAGE_AUTO_MODELS
    /*4*/{5, 0, 0,&SELECTUSER_TITLE_WND,&SELECTUSER_SUB1_WND,&SELECTUSER_SUB2_WND,&RF_DATA_WND,&WIFI_DATA_WND,},//select user
    /*5*/{14, 0, 0,&CAL_TITLE_WND,&MILE_TITLE_WND,&INCLINE_TITLE_WND,&SPEED_TITLE_WND,&TIME_DATA_WND,&CAL_DATA_WND,&DIS_DATA_WND,&INCLINE_DATA_WND, &SPEED_DATA_WND,&MATRIX_DATA_WND,&BOTTOM_TEXT_WND,&MIDDLE_INFO_WND,&RF_DATA_WND,&WIFI_DATA_WND},//PAGE_PROGRAM
    /*6*/{2, 0, 0,&WIFI_DATA_WND,&RF_DATA_WND},//PAGE_ERP_COUNTDOWN,//??¨o?Number9~0, 
    
    /*7*/{14, 0, 0,&CAL_TITLE_WND,&MILE_TITLE_WND,&INCLINE_TITLE_WND,&SPEED_TITLE_WND,&TIME_DATA_WND,&CAL_DATA_WND,&DIS_DATA_WND,&INCLINE_DATA_WND, &SPEED_DATA_WND,&MATRIX_DATA_WND,&BOTTOM_TEXT_WND,&MIDDLE_INFO_WND,&RF_DATA_WND,&WIFI_DATA_WND},//work tm1
        
    /*8*/{14, 0, 0,&HEART_TITLE_WND, &PACE_TITLE_WND, &INCLINE_TITLE_WND, &SPEED_TITLE_WND, &TIME_DATA_WND, &HEART_DATA_WND, &PACE_DATA_WND, &INCLINE_DATA_WND, &SPEED_DATA_WND,&MATRIX_DATA_WND,&BOTTOM_TEXT_WND,&MIDDLE_INFO_WND,&RF_DATA_WND,&WIFI_DATA_WND},//work tm2
            
    /*9*/{14, 0, 0,&CAL_TITLE_WND, &MILE_TITLE_WND, &INCLINE_TITLE_WND, &RES_TITLE_WND, &TIME_DATA_WND, &CAL_DATA_WND, &DIS_DATA_WND, &INCLINE_DATA_WND, &RES_DATA_WND,&MATRIX_DATA_WND,&BOTTOM_TEXT_WND,&MIDDLE_INFO_WND,&RF_DATA_WND,&WIFI_DATA_WND},//work as1
                
    /*10*/{14, 0, 0,&HEART_TITLE_WND, &WATTS_TITLE_WND, &METS_TITLE_WND, &RPM_TITLE_WND, &TIME_DATA_WND, &HEART_DATA_WND, &WATTS_DATA_WND, &METS_DATA_WND, &AS_RPM_DATA_WND,&MATRIX_DATA_WND,&BOTTOM_TEXT_WND,&MIDDLE_INFO_WND,&RF_DATA_WND,&WIFI_DATA_WND},//work as2
                    
    /*11*/{14, 0, 0,&CAL_TITLE_WND, &MILE_TITLE_WND, &BK_RPM_TITLE_WND, &RES_TITLE_WND, &TIME_DATA_WND, &CAL_DATA_WND, &DIS_DATA_WND, &RPM_DATA_WND, &RES_DATA_WND,&MATRIX_DATA_WND,&BOTTOM_TEXT_WND,&MIDDLE_INFO_WND,&RF_DATA_WND,&WIFI_DATA_WND},//work bk1
                        
     /*12*/{14, 0, 0,&HEART_TITLE_WND, &WATTS_TITLE_WND, &METS_TITLE_WND, &SPEED_TITLE_WND, &TIME_DATA_WND, &HEART_DATA_WND, &WATTS_DATA_WND, &METS_DATA_WND, &SPEED_DATA_WND,&MATRIX_DATA_WND,&BOTTOM_TEXT_WND,&MIDDLE_INFO_WND,&RF_DATA_WND,&WIFI_DATA_WND},//work bk2

	/*13*/{14, 0, 0,&HEART_TITLE_WND, &PACE_TITLE_WND, &METS_TITLE_WND, &RPM_TITLE_WND, &TIME_DATA_WND, &HEART_DATA_WND, &PACE_DATA_WND, &METS_DATA_WND, &AS_RPM_DATA_WND,&MATRIX_DATA_WND,&BOTTOM_TEXT_WND,&MIDDLE_INFO_WND,&RF_DATA_WND,&WIFI_DATA_WND},//work as2_E30

     /*14*/{14, 0, 0,&HEART_TITLE_WND, &PACE_TITLE_WND, &METS_TITLE_WND, &SPEED_TITLE_WND, &TIME_DATA_WND, &HEART_DATA_WND, &PACE_DATA_WND, &METS_DATA_WND, &SPEED_DATA_WND,&MATRIX_DATA_WND,&BOTTOM_TEXT_WND,&MIDDLE_INFO_WND,&RF_DATA_WND,&WIFI_DATA_WND},//work bk2

     /*15*/{14, 0, 0,&CAL_TITLE_WND, &WATTS_TITLE_WND, &LEVEL_TITLE_WND, &STAIR_TITLE_WND, &TIME_DATA_WND,&CAL_DATA_WND, &WATTS_DATA_WND, &CLIMBMILL_LEVEL_DATA_WND, &CLIMBMILL_STAIRS_DATA_WND,&MATRIX_DATA_WND,&BOTTOM_TEXT_WND,&MIDDLE_INFO_WND,&RF_DATA_WND,&WIFI_DATA_WND},//PAGE_WORK_CLIMBMILL1

     /*16*/{14, 0, 0,&HEART_TITLE_WND, &CLIMBMILL_METS_TITLE_WND, &CLIMBMILL_SPM_TITLE_WND, &CLIMBMILL_HEIGHT_TITLE_WND, &TIME_DATA_WND, &HEART_DATA_WND,&CLIMBMILL_METS_DATA_WND,  &CLIMBMILL_SPM_DATA_WND, &CLIMBMILL_HEIGHT_DATA_WND, &MATRIX_DATA_WND,&BOTTOM_TEXT_WND, &MIDDLE_INFO_WND, &RF_DATA_WND,&WIFI_DATA_WND},//PAGE_WORK_CLIMBMILL2

     /*17*/{3, 0, 0,&ENG_TITLE_DISPLAY,&RF_DATA_WND,&WIFI_DATA_WND},//                            

     /*18*/ {3, 0, 0,&SAFEKEY_TEXT_WND,&RF_DATA_WND,&WIFI_DATA_WND},//     
     /*19*/ {3, 0, 0,&UPDATE_INFO_WND,&RF_DATA_WND,&WIFI_DATA_WND},//    
     /*20*/ {3, 0, 0,&ERRORCODE_INFO_WND,&RF_DATA_WND,&WIFI_DATA_WND},//
};


void Page_Init_Data(void)
{
    //nCurrentPage = PAGE_INITAL;
	martix_type = (Matrix_Type)(Program_Get_DefaultDotMatrix());	
    g_wndLeftTopTextBox._x = 1;
    g_wndLeftTopTextBox._y = 0;
    g_wndLeftTopTextBox._z = NULL;
    g_wndLeftTopTextBox._string_id = NULL;
    g_wndLeftTopTextBox._blinkCount = 0 ;
    g_wndLeftTopTextBox._activetime = 0 ;
    g_wndLeftTopTextBox.pfShow = NULL ;
    g_wndLeftTopTextBox._isHidden = 0 ;
    g_wndLeftTopTextBox._blinkDelayCount = 0 ;
    
    g_wndLeftTopNumerical._x = 0;
    g_wndLeftTopNumerical._y = 1;
    g_wndLeftTopNumerical._string_id = NULL;
    g_wndLeftTopNumerical._blinkCount = 0 ;
    g_wndLeftTopNumerical._activetime = 0 ;
    g_wndLeftTopNumerical.pfShow = NULL ;
    g_wndLeftTopNumerical._isHidden = 0 ;
    g_wndLeftTopNumerical._blinkDelayCount = 0 ;
    
    g_wndRightTopTextBox._x = 41;
    g_wndRightTopTextBox._y = 0;
    g_wndRightTopTextBox._z = NULL;
    g_wndLeftTopNumerical._string_id = NULL;
    g_wndRightTopTextBox._blinkCount = 0 ;
    g_wndRightTopTextBox._activetime = 0 ;
    g_wndRightTopTextBox.pfShow = NULL ;
    g_wndRightTopTextBox._isHidden = 0 ;
    g_wndRightTopTextBox._blinkDelayCount = 0 ;
    
    g_wndRightTopNumerical._x = 8;
    g_wndRightTopNumerical._y = 1;
    g_wndRightTopNumerical._string_id = NULL;
    g_wndRightTopNumerical._z = 0;
    g_wndRightTopNumerical._blinkCount = 0 ;
    g_wndRightTopNumerical._activetime = 0 ;
    g_wndRightTopNumerical.pfShow = NULL ;
    g_wndRightTopNumerical._isHidden = 0 ;
    g_wndRightTopNumerical._blinkDelayCount = 0 ;
    
    g_wndLeftBottomTextBox._x = 0;
    g_wndLeftBottomTextBox._y = 6;
    g_wndLeftBottomTextBox._z = NULL;
    g_wndLeftBottomTextBox._string_id = NULL;
    g_wndLeftBottomTextBox._blinkCount = 0 ;
    g_wndLeftBottomTextBox._activetime = 0 ;
    g_wndLeftBottomTextBox.pfShow = NULL ;
    g_wndLeftBottomTextBox._isHidden = 0 ;
    g_wndLeftBottomTextBox._blinkDelayCount = 0 ;
    
    g_wndLeftBottomNumerical._x = 12;
    g_wndLeftBottomNumerical._y = 1;
    g_wndLeftBottomNumerical._z = NULL;
    g_wndLeftBottomNumerical._string_id = NULL;
    g_wndLeftBottomNumerical._blinkCount = 0 ;
    g_wndLeftBottomNumerical._activetime = 0 ;
    g_wndLeftBottomNumerical.pfShow = NULL ;
    g_wndLeftBottomNumerical._isHidden = 0 ;
    g_wndLeftBottomNumerical._blinkDelayCount = 0 ;
    
    g_wndRightBottomTextBox._x = 0;
    g_wndRightBottomTextBox._y = 7;
    g_wndRightBottomTextBox._z = NULL;
    g_wndRightBottomTextBox._string_id = NULL;
    g_wndRightBottomTextBox._blinkCount = 0 ;
    g_wndRightBottomTextBox._activetime = 0 ;
    g_wndRightBottomTextBox.pfShow = NULL ;
    g_wndRightBottomTextBox._isHidden = 0 ;
    g_wndRightBottomTextBox._blinkDelayCount = 0 ;

    g_wndMiddleBottomTextBox._x = 0;
    g_wndMiddleBottomTextBox._y = 8;
    g_wndMiddleBottomTextBox._z = NULL;
    g_wndMiddleBottomTextBox._string_id = STR_NULL_ID;
    g_wndMiddleBottomTextBox._blinkCount = 0 ;
    g_wndMiddleBottomTextBox._activetime = 0 ;
    g_wndMiddleBottomTextBox.pfShow = ShowBottomTextBox ;
    g_wndMiddleBottomTextBox._isHidden = 1 ;
    g_wndMiddleBottomTextBox._blinkDelayCount = 0 ;
			 
    g_wndRightBottomNumerical._x = 16;
    g_wndRightBottomNumerical._y = 1;
    g_wndRightBottomNumerical._z = NULL;
    g_wndRightBottomNumerical._string_id = NULL;
    g_wndRightBottomNumerical._blinkCount = 0 ;
    g_wndRightBottomNumerical._activetime = 0 ;
    g_wndRightBottomNumerical.pfShow = NULL ;
    g_wndRightBottomNumerical._isHidden = 0 ;
    g_wndRightBottomNumerical._blinkDelayCount = 0 ;
    
    g_wndBottomTextBox._x = 0;
    g_wndBottomTextBox._y = 3;
    g_wndBottomTextBox._z = NULL;
    g_wndBottomTextBox._string_id = NULL;
    g_wndBottomTextBox._blinkCount = 0 ;
    g_wndBottomTextBox._activetime = 0 ;
    g_wndBottomTextBox.pfShow = NULL ;
    g_wndBottomTextBox._isHidden = 0 ;
    g_wndBottomTextBox._blinkDelayCount = 0 ;

    g_wndMiddleMatrixBox._x = 0;
    g_wndMiddleMatrixBox._y = 2;
    g_wndMiddleMatrixBox._z = NULL;
    g_wndMiddleMatrixBox._string_id = NULL;
    g_wndMiddleMatrixBox._blinkCount = 0 ;
    g_wndMiddleMatrixBox._activetime = 0 ;
    g_wndMiddleMatrixBox.pfShow = NULL ;
    g_wndMiddleMatrixBox._isHidden = 0 ;
    g_wndMiddleMatrixBox._blinkDelayCount = 0 ;
    
    g_wndMiddleNumerical._x = 4;
    g_wndMiddleNumerical._y = 1;
    g_wndMiddleNumerical._z = NULL;
    g_wndMiddleNumerical._string_id = NULL;
    g_wndMiddleNumerical._blinkCount = 0 ;
    g_wndMiddleNumerical._activetime = 0 ;
    g_wndMiddleNumerical.pfShow = NULL ;
    g_wndMiddleNumerical._isHidden = 0 ;
    g_wndMiddleNumerical._blinkDelayCount = 0 ;
    
    g_wndRFIcon._x = 0;
    g_wndRFIcon._y = 4;
    g_wndRFIcon._z = NULL;
    g_wndRFIcon._string_id = NULL;
    g_wndRFIcon._blinkCount = 0 ;
    g_wndRFIcon._activetime = 0 ;
    g_wndRFIcon.pfShow = ShowRFICON ;
    g_wndRFIcon._isHidden = 0 ;
    g_wndRFIcon._blinkDelayCount = 0 ;
    
    g_wndWiFiIcon._x = 0;
    g_wndWiFiIcon._y = 5;
    g_wndWiFiIcon._z = NULL;
    g_wndWiFiIcon._string_id = NULL;
	if(WIFIIsAPMode())
	{
    	g_wndWiFiIcon._blinkCount = 0xAA ;
	}
    g_wndWiFiIcon._activetime = 0 ;
    g_wndWiFiIcon.pfShow = ShowWifiIcon ;
    g_wndWiFiIcon._isHidden = 0 ;
    g_wndWiFiIcon._blinkDelayCount = 0 ;
    
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
        
    for(int i=0;i<(pages[nCurrentPage].nWindowCount-2);i++)
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


void Event_Modal_Timer_Handler(void)
{
    for(UINT16 i = 0; i < pages[nCurrentPage].nWindowCount; i++)
    {
        if( pages[nCurrentPage].pItem[i]->pWnd != NULL&&\
            pages[nCurrentPage].pItem[i]->pWnd->_isHidden!=1)
        {
            if(pages[nCurrentPage].pItem[i]->pWnd->_activetime >1 &&\
                pages[nCurrentPage].pItem[i]->pWnd->_activetime!=0xAA) 
            {
                pages[nCurrentPage].pItem[i]->pWnd->_activetime--;
            }
            else if(pages[nCurrentPage].pItem[i]->pWnd->_activetime==1)
            {
				pages[nCurrentPage].pItem[i]->pWnd->_activetime = 0 ;
				pages[nCurrentPage].pItem[i]->pWnd->_isHidden = 1;
				Win_Items *pPopedWnds;
				UINT8 cnt ;
				pPopedWnds = Page_Pop_OverlappedWindow(pages[nCurrentPage].pItem[i]->pWnd, &cnt);
                if(pPopedWnds)
                {
                    memcpy(pages[nCurrentPage].pItem[i]->pWnd,pPopedWnds,sizeof(Win_Items));//2016.02.24修改，把pop出的数据copy到page的位置
                    for(UINT8 i = 0;i < cnt; i++)
                    {
                        pPopedWnds[i]._isHidden = 0;
                        if(pPopedWnds[i].pfShow)
                            pPopedWnds[i].pfShow(&pPopedWnds[i]);
                    }
                }
				Page_Reset_Modal_Window(pages[nCurrentPage].pItem[i]->pWnd);
			}
		}
	}
}
void Event_Static_Timer_Handler(void)
{
	for(UINT16 i = 0; i < pages[nCurrentPage].nWindowCount; i++)
	{
        if( pages[nCurrentPage].pItem[i]->pWnd != NULL&&\
            pages[nCurrentPage].pItem[i]->pWnd->_isHidden!=1)
        {
            if(pages[nCurrentPage].pItem[i]->pWnd->_blinkDelayCount >1 &&\
                pages[nCurrentPage].pItem[i]->pWnd->_blinkDelayCount!=0xAA) 
            {
                pages[nCurrentPage].pItem[i]->pWnd->_blinkDelayCount--;
            }
            else if(pages[nCurrentPage].pItem[i]->pWnd->_blinkDelayCount==1)
            {
                pages[nCurrentPage].pItem[i]->pWnd->_blinkDelayCount = 0 ;
            }
        }
    }  
}



void Events_Blink_Timer_Handler(void)
{
    Timer_500msHigh_Level = !Timer_500msHigh_Level;
    
    for(UINT16 i = 0; i < pages[nCurrentPage].nWindowCount; i++)
    {
        if( pages[nCurrentPage].pItem[i]->pWnd != NULL&&\
            pages[nCurrentPage].pItem[i]->pWnd->_isHidden!=1)
        {
            if(pages[nCurrentPage].pItem[i]->pWnd->_blinkDelayCount==0 ||\
                pages[nCurrentPage].pItem[i]->pWnd->_blinkDelayCount ==0xAA) 
                {
                    //pages[nCurrentPage].pItem[i]->pWnd->_blinkCount-=2;
                if(pages[nCurrentPage].pItem[i]->pWnd->_blinkCount>2)
                {
                    if(pages[nCurrentPage].pItem[i]->pWnd->_blinkDelayCount==0 && Timer_500msHigh_Level)
                    {
                        pages[nCurrentPage].pItem[i]->pWnd->_blinkDelayCount = 0xAA;
                    }
                    if(pages[nCurrentPage].pItem[i]->pWnd->_blinkDelayCount == 0xAA)
                        pages[nCurrentPage].pItem[i]->pWnd->_blinkCount --;
                    
                    if(pages[nCurrentPage].pItem[i]->pWnd->_blinkCount ==0xA8 )//?a0xAAê±ò??±éá??
                        pages[nCurrentPage].pItem[i]->pWnd->_blinkCount = 0xAA;
                    
                    pages[nCurrentPage].pItem[i]->pWnd->pfShow(pages[nCurrentPage].pItem[i]->pWnd);
                }
                else if(pages[nCurrentPage].pItem[i]->pWnd->_blinkCount==2)
                {
                    pages[nCurrentPage].pItem[i]->pWnd->_blinkCount = 0 ;
					if(pages[nCurrentPage].pItem[i]->pWnd->_isHidden == 0xAA )
	            	{
	            		pages[nCurrentPage].pItem[i]->pWnd->_isHidden = 0 ;
                        extern Matrix_Type g_martix_modal_type;
						martix_type = g_martix_modal_type;
					}					
                }
            }
        }
    }
}

void Event_Refresh_Timer_Handler(void)
{
	 extern MAIN_STAGE Main_Stage ;
	if(g_wndBottomTextBox._isHidden == 0)
	{
		Display_Bottom_LedString();
	}
	if( Main_Stage == STAGE_PRG_RUN )
	{
		ShowPage(nCurrentPage , 0);
	}
}


void Page_Init_Timer(void)
{
	 BLINK_DISPLAY_CHANNEL = Timer_Malloc_Channels(1);
     STATIC_DISPLAY_CHANNEL = Timer_Malloc_Channels(1);  
     MODAL_WINDOW_CHANNEL = Timer_Malloc_Channels(1);
     REFRESH_WINDOW_CHANNEL = Timer_Malloc_Channels(1);		
}

void Page_Timer_Process(void)
{
     //Timer for blink ctr
	if(Timer_Counter(BLINK_DISPLAY_CHANNEL,5))
	{
		Events_Blink_Timer_Handler();
	}
	//Timer for static display ctr
	if(Timer_Counter(STATIC_DISPLAY_CHANNEL,10))
	{
		Event_Static_Timer_Handler();
	}
	//Timer for modal Wnd ctr
	if(Timer_Counter(MODAL_WINDOW_CHANNEL,10))
	{
		Event_Modal_Timer_Handler();
	}
	//Timer for display refresh ctr
	if(Timer_Counter(REFRESH_WINDOW_CHANNEL,2))
	{
		Event_Refresh_Timer_Handler();
	}
}
