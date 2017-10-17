#ifndef _page_h_
#define _page_h_

#include "Display.h"
#include "Strings.h"
#include "wifi_interface.h"
#include "Display.h"
#include "Midware_Heart.h"



#define WIN_TOTAL                  16


//---------------------------data structures------------------------------------
//--!!! 请不要动--------
typedef enum 
{
	Show_None=0,
	Show_Speed = 1,
	Show_Incline,
	Show_Resistance,	
    Show_Hight,
    Show_Run_Hight,
	TypeTotal
}Matrix_Type;

typedef enum 
{
    modal_wnd_start=0,
	wnd_calories_title = modal_wnd_start,
    wnd_calories_data,
    wnd_tatal
}Modal_Wnd;

typedef enum 
{
    MODAL_INDEX_START=0,
    MODAL_INDEX_LEFT_BOTTOM,
    MODAL_INDEX_RIGHT_BOTTOM,
    MODAL_INDEX_WHOLE_BOTTOM,
    MODAL_INDEX_LEFT_RIGHT,
    MODAL_INDEX_TOTAL
}MODAL_INDEX;

#pragma pack( push, 1 )
typedef struct Win_Item
{
  UINT8 _x,_y,_z;
  STRINGID _string_id;
  UINT8 _blinkCount;
  UINT8 _activetime;
  void (*pfShow)(void  *p);   
  
  UINT8 _isHidden;
  Modal_Wnd* _PreWndid;
  UINT8 _blinkDelayCount;
}Win_Items;

typedef struct
{
    Win_Items *pWnd;
    Win_Items defParam; 
}window_def;

typedef struct tag_page
{
  UINT8 nWindowCount;
  UINT8 _isAnimated;
  UINT8 _waitSecondsX2;
  window_def  const *pItem[WIN_TOTAL];
}PAGES;

#pragma pack(pop)


//---------------------------Customized windows---------------------------------
//@brief: 请按照您的仪表显示窗口配置您所需的所有窗口全局变量
//STEP1: enumnate all windows per software def & overlay design

extern Win_Items g_wndLeftTopTextBox;
extern Win_Items g_wndLeftTopNumerical;
extern Win_Items g_wndRightTopTextBox;
extern Win_Items g_wndRightTopNumerical;
extern Win_Items g_wndLeftBottomTextBox;
extern Win_Items g_wndMiddleBottomTextBox;
extern Win_Items g_wndLeftBottomNumerical;
extern Win_Items g_wndRightBottomTextBox;
extern Win_Items g_wndRightBottomNumerical;
extern Win_Items g_wndBottomTextBox;
extern Win_Items g_wndMiddleMatrixBox;
extern Win_Items g_wndMiddleNumerical;
extern Win_Items g_wndRFIcon;
extern Win_Items g_wndWiFiIcon;
extern Win_Items g_wndFanIcon;

// step 2: enumnate all pages per software def
typedef enum 
{
	PAGE_INITAL = 0,
	PAGE_VERSIONS,
	PAGE_AUTO_MODELS,
	PAGE_SELECTUSER,
	PAGE_PROGRAM,  
	PAGE_ERP_COUNTDOWN,//??ê?Number9~0,
	PAGE_WORK_TM1,
	PAGE_WORK_TM2,
	PAGE_WORK_AS1,
	PAGE_WORK_AS2,
	PAGE_WORK_BK1,
	PAGE_WORK_BK2,
	PAGE_WORK_AS2_A30, 
	PAGE_WORK_BK2_EUR30,
	PAGE_WORK_CLIMBMILL1,
	PAGE_WORK_CLIMBMILL2,
	PAGE_ENG0_0_DisplayTest,
	PAGE_SAFETYKEY_OFF,
	PAGE_UPDATE_INFO,
    PAGE_ERRORCODE_INFO,
	PAGE_TOTAL
}VR_PAGE;

extern VR_PAGE  nCurrentPage;
extern Matrix_Type martix_type;



//---------------------------Customized windows---------------------------------
void Page_Init_Timer(void);
void Page_Timer_Process(void);

void Page_Init_Data(void);
void ShowPage(VR_PAGE p, UINT8 isDefault);
void Page_Push_OverlappedWindow(Win_Items* pSrcWnd, Win_Items *pWnd);
Win_Items* Page_Pop_OverlappedWindow(Win_Items* pSrcWnd, UINT8* cnt);
bool  Page_Reset_Modal_Window(Win_Items* pSrcWnd);
bool Page_Get_Modal_State(void);
void Page_Reset_All_ModalWnds(void);

#endif
