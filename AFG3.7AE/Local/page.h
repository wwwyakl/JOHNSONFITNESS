#ifndef _page_h_
#define _page_h_

#include "Display.h"
#include "Strings.h"

#define WIN_TOTAL                  20


//---------------------------data structures------------------------------------
//--!!! 请不要动--------
typedef enum 
{
	Show_None=0,
	Show_Speed = 1,
	Show_Incline,
	Show_Resistance,	
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

extern __no_init Win_Items g_wndLeftTopTextBox;//0,0
extern __no_init Win_Items g_wndLeftTopNumerical;//0,8
extern __no_init Win_Items g_wndRightTopTextBox;//0,1
extern __no_init Win_Items g_wndRightTopNumerical;//0,10
extern __no_init Win_Items g_wndLeftBottomTextBox;//0,4
extern __no_init Win_Items g_wndLeftBottomNumerical;//0,9
extern __no_init Win_Items g_wndRightBottomTextBox;//0,5
extern __no_init Win_Items g_wndRightBottomNumerical;//0,11
extern __no_init Win_Items g_wndMiddleBottomTextBox;//
extern __no_init Win_Items g_wndBottomTextBox;//
extern __no_init Win_Items g_wndMiddleMatrixBox;//0,6
extern __no_init Win_Items g_wndMiddleNumerical;//
extern __no_init Win_Items g_wndAFGLOGOIcon;
extern __no_init Win_Items g_wndTimeIcon;
extern __no_init Win_Items g_wndDistanceIcon;
extern __no_init Win_Items g_wndCaloriesIcon;
extern __no_init Win_Items g_wndInclineIcon;//0,4
extern __no_init Win_Items g_wndLapsIcon;//0,5
extern __no_init Win_Items g_wndLEDTextBox;//1,1
extern __no_init Win_Items g_wndLED1TextBox;//2,0
extern __no_init Win_Items g_wndLED2TextBox;//2,1
extern __no_init Win_Items g_wndLED3TextBox;//2,2
extern __no_init Win_Items g_wndLED4TextBox;//2,3
extern __no_init Win_Items g_wndLapsCircleIcon;//0,12

// step 2: enumnate all pages per software def
typedef enum 
{
    /*1*/   PAGE_INITAL = 0,
//    /*2*/   PAGE_VERSIONS,
//           PAGE_AOTO_MODELS,
            PAGE_AGINTEST,
    /*3*/   PAGE_SELECTUSER,
            PAGE_PROGRAM,  
    /*89*/  PAGE_ERP_COUNTDOWN,//显示Number9~0,
    
    /*31*/  PAGE_WORK_TM1,
    /*32*/  PAGE_WORK_TM2,
    /*35*/  PAGE_WORK_BK1,
    /*36*/  PAGE_WORK_BK2,
    /*33*/  PAGE_WORK_AS1,
    /*34*/  PAGE_WORK_AS2,
    
            PAGE_SAFETYKE_OFF,
            PAGE_ENG0_0_DisplayTest,
    /*72*/  PAGE_TOTAL
}VR_PAGE;

extern VR_PAGE  nCurrentPage;
extern Matrix_Type martix_type;



//---------------------------Customized windows---------------------------------
void Page_Init_Data(void);
void ShowPage(VR_PAGE p, UINT8 isDefault);
void Page_Push_OverlappedWindow(Win_Items* pSrcWnd, Win_Items *pWnd);
Win_Items* Page_Pop_OverlappedWindow(Win_Items* pSrcWnd, UINT8* cnt);
bool  Page_Reset_Modal_Window(Win_Items* pSrcWnd);
bool Page_Get_Modal_State(void);
void Page_Reset_All_ModalWnds(void);

#endif
