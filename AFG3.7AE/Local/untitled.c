typedef enum
{
 WIN_LOCA_START=0;
 WIN_LOCA_NONE=WIN_LOCA_START;
 WIN_TOP_LEFT_TEXT;
 WIN_TOP_LEFT_MUNBER;
 WIN_TOP_RIGHT_TEXT;
 WIN_TOP_RIGHT_MUNBER;
 WIN_MIDDLE_MUNBER;
 WIN_MIDDLE_MATRIX;
 WIN_BOTTOM_LEFT_TEXT;
 WIN_BOTTOM_LEFT_MUNBER; 
 WIN_BOTTOM_RIGHT_TEXT;
 WIN_BOTTOM_RIGHT_MUNBER; 
 WIN_BOTTOM_TEXT;
 WIN_RF_ICON;
 WIN_WIFI_ICON;
 WIN_HEART_ICON;
 WIN_FAN_ICON;
 WIN_TATAL;
}WIN_LOCATION;

typedef struct tag_Modalpara
{
    
    WIN_LOCATION _loction1st;
    WIN_LOCATION _loction2nd;
    
    void (*pfShow1st)(void  *p);
    void (*pfShow2nd)(void  *p);
}MODEL_DEFAULTS;


typedef struct 
{
    UINT8 _OverlappedWinCnt;
    Win_Items* _OverlappedWindows[WIN_TOTAL]; 
}OVERLAPPINGWIN;

typedef struct
{
    Win_Items* _ActiveWnd;
    OVERLAPPINGWIN _OverlappedWnds[3];
}OVERLAPPINGWINGROUP;

__no_init static OVERLAPPINGWINGROUP g_OverlappedWindows;



g_wndRightBottomNumerical

Set_Wnd_Para(g_wndRightBottomNumerical);

void Set_Wnd_Para(Win_Items* p)
{
    p->_isHidden = 0;
    
}