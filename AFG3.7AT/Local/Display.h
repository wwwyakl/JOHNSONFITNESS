#ifndef _DISPLAY_H
#define _DISPLAY_H
//#include "Information.h"//need D_DEFAULT_MODE to choose display windows
#include "JIStypes.h"

#define DSE_ALL_ON  0xff
#define DSE_ALL_OFF 0xfe

//LED show index
#define DC_SP    10
#define DC_ALL   11
#define DC_S     12
#define DC_T     13
#define DC_O     14
#define DC_P     15
#define DC_E     16
#define DC_N     17
#define DC_D     18
#define DC_SAFE  19
#define DC_F     20
#define DC_I     21
#define DC_L     22
#define DC_G     23


#define LCD_MID_ALL 1
#define LCD_TOP_ALL 2
#define LCD_ALL_ALL 3

#define C_STOP 0
#define C_PAGE 1
#define C_STR  2

//Display_Set_Windows

#define DP_SPEED     0
#define DP_INCLINE   1

#define DSW_ALL_ON     0xffff
#define DSW_ALL_OFF    0xfffe


#define DSL_ALL_ON 0xffff
#define DSL_ALL_OFF 0xfffe

//clear LCD used
#define CL_TOP  0
#define CL_MID  1
#define CL_BOT  2

//Display_Set_LCD_Char
#define DSLC_DEC       36//'-'
#define DSLC_LOW       37//
#define DSLC_DIV       38//'/'
#define DSLC_SPACE     63//' '
#define DSLC_ALL_ON    40//
#define DSLC_ALL_OFF   39//
#define DSLC_UP        41//
#define DSLC_DOWN      42//''
#define DSLC_DIR       43//'\'
#define DSLC_IS        44//'''
#define DSLC_SB_L      45//'[' SB = square bracket
#define DSLC_SB_R      45//']' SB = square bracket
#define DSLC_ADD       47//'+'
#define DSLC_ARROW     48//'->'
#define DSLC_DOT       49//'.'

//Display_Set_Blink(
//#define DB_INCLINE    1
//#define DB_CALORIES   2
//#define DB_HEART       3
//#define DB_PACE       4
//#define DB_WATTS      5
//#define DB_PULSE      6
//#define DB_SPEED      7
//#define DB_BOTTOM     8


#define DB_2_SEGMENT       0
#define DB_4_SEGMENT       1
#define DB_3_SEGMENT       2
#define DB_PACE_3_SEGMENT  3
#define DB_YEAR_4_SEGMENT  4
#define DB_DATE_2_SEGMENT  5
#define DB_FORWARD_2_SEGMENT   6
#define DB_BACKWARD_4_SEGMENT  7
#define DB_MIN_2_SEGMENT       8
#define DB_MID_FW_4_SEGMENT    9
#define DB_MID_BW_4_SEGMENT    10
#define DB_MID_LAST_5_WORK     11


#define  DB_ALL        0xff

#define  DP_NUMB      0
#define  DP_WORD      1

//caption LCD
#define  DL_TIME          1
#define  DL_DISTANCE      2
#define  DL_CALORIES      3
#define  DL_PACE          4
#define  DL_ELE_GAIN      5
#define  DL_TOP_1         6
#define  DL_TOP_2         7
#define  DL_TOP_3         8
#define  DL_TOP_MID       9
#define  DL_MID_1         10
#define  DL_MID_2         11
#define  DL_MID_3         12
#define  DL_MID_4         13
#define  DL_MID_5         14
#define  DL_MID_BOTT      15
//caption LED
#define  DC_CALORIES      16
#define  DC_INCLINE       17
#define  DC_SPEED         18
#define  DC_HEART         19
#define  DC_USB_RED       20
#define  DC_USB_GREEN     21
#define  DC_USB_ORANGE    22

#define BIT0                     (0x00000001)
#define BIT1                     (0x00000002)
#define BIT2                     (0x00000004)
#define BIT3                     (0x00000008)
#define BIT4                     (0x00000010)
#define BIT5                     (0x00000020)
#define BIT6                     (0x00000040)
#define BIT7                     (0x00000080)
#define BIT8                     (0x00000100)
#define BIT9                     (0x00000200)
#define BIT10                    (0x00000400)
#define BIT11                    (0x00000800)
#define BIT12                    (0x00001000)
#define BIT13                    (0x00002000)
#define BIT14                    (0x00004000)
#define BIT15                    (0x00008000)
#define BIT16                    (0x00010000)
#define BIT17                    (0x00020000)
#define BIT18                    (0x00040000)
#define BIT19                    (0x00080000)
#define BIT20                    (0x00100000)
#define BIT21                    (0x00200000)
#define BIT22                    (0x00400000)
#define BIT23                    (0x00800000)
#define BIT24                    (0x01000000)
#define BIT25                    (0x02000000)
#define BIT26                    (0x04000000)
#define BIT27                    (0x08000000)
#define BIT28                    (0x10000000)
#define BIT29                    (0x20000000)
#define BIT30                    (0x40000000)
#define BIT31                    (0x80000000)

void Display_Initial_Data(void);
//void LCD_Set_All_On(void);
//void LCD_Set_All_Off(void);
//void Display_Flag_100ms_Int(void);
void Display_Set_All(char by_Dat);
//void Display_Set_LCD_Move_Speed(char by_Data);
//void Display_Set_LCD_Char(char by_Index,char by_Dat);
void Display_Updata_Data(char by_Mode);
//void Display_Number_F(char by_Mode);
//void Display_Number_Init(void);
//void Display_Show_Px(char by_Dat);
//void Display_Number_L(char by_Dat);
//void Display_Number_Safe(void);
//void Display_Number_Eng(char by_Dat);
//void Display_Set_Blink(char by_Dat);
//void Display_Clear_Blink(char by_Dat);
//char Display_Get_Move_OK_Count(char by_Mode);
//void Display_Flag_10ms_Int(void);
//char Display_Get_Blink(char by_Dat);
//void Display_Set_Move_Speed(char by_Dat);
void Display_Set_Windows_All(char by_Show);
//void Display_Set_LED_Blink(char by_Mode,char by_Dat);
//void Display_Clear_LED_Blink(void);
//void Display_Set_Distance_Dot(char by_Show);
//void Display_Set_Heart_Dot(char by_Show);
//void Display_Set_Dot_All(char by_Show);
//void Display_Set_User_LED(char by_Show);
//void Display_Set_Moving_String_Bottom(char by_Address,const char *pby_Str);//by ckm
//void Display_Set_Moving_StringTop(char by_Address,const char *pby_Str);//by ckm
//void Display_Set_Windows_Mode(char by_Mode);
void Display_Set_Caption(unsigned char by_Addr,unsigned char by_Action);
void Display_Show_Pace(UINT16 w_Pace);
//void Display_Show_Target_Pace(UINT16 w_Pace);
//void Display_Show_Quick_Key(UINT16 w_Data);
//void Display_Set_Word_Windows(char by_Index,UINT16 w_Data,char by_Dot);
//void Display_Clear_LCD(unsigned char by_Index);
//void Display_Set_Blink_Counter(unsigned char by_Data);
//void Display_RTC_AM_PM(char by_Mode,char by_Data);
//void Display_Set_Char_Buffer(unsigned char by_Start,const char *pby_Str);
//void Display_Show_NA(void);
void Display_Set_Demo(char by_Data);
void Display_DEMO_Process(void);
//void Display_Scan_Int(void);
void Display_Show_Bottom(char sr[7]);
//void Display_Show_State(char by_Dat);
//char Display_Get_STR_MOVE_Top(void);
//void Display_Set_Dotmax_Program(char by_Index,char by_Dat);
//void Display_Set_Matrix_blink_Flag(char by_Matrix_blink);
//char Display_Get_Matrix_blink_Flag(void);
//unsigned int *Display_Get_HRDisplay_DotmaxBuffer_Pointer(void);
//void Display_Set_Dotmax_Blink(char by_Dat);
//void Display_Set_Dotmax_Bottom(char by_Mode);
//static void Display_Set_Dotmax_X_Bar(char by_Address,unsigned int w_Dat);
void Display_Set_Dotmax_Program_Mode(char by_Mode);
//void Display_Time(char by_Address,const char *pby_Str);
//void Display_Distance(char by_Address,const char *pby_Str);
void Display_Left(char by_Address,const char *pby_Str);
void Display_Right(char by_Address,const char *pby_Str);
void Display_LeftDown_Segment(char by_Address,const char *pby_Str);
//void Display_Speed(char by_Address,const char *pby_Str);
//void Display_Logo(char by_Address,const char *pby_Str);
void Display_Incline(char by_Address,const char *pby_Str);
void Display_Laps_Circle(void);
//void Display_Laps(char by_Address,char pby_Str);
void Display_Bottom_Text(char by_Address,const char *pby_Str);
void Display_Matrix(char by_Index,char by_Dat);
void Display_RightDown_Segment(char by_Address,const char *pby_Str);
//static void Display_Set_Dotmax_X_Bar(char by_Address,unsigned int w_Dat);
//static void Display_Set_Dotmax_X_Bar3(char by_Address,unsigned int w_Dat);
//void Display_Matrix(char by_Index,char by_Dat);
//void Display_SpeedSet(char by_Address,const char *pby_Str);
//void Display_Level(char by_Address,const char *pby_Str);
//void Display_Weight(char by_Address,const char *pby_Str);
unsigned char *Display_Get_Loop_Circle_Point(void);
void Display_Decode_To_Lcd(void);
char Display_Get_Demo(void);
//void Display_Set_Dotmax_BottomWork(char by_Mode);
//void Display_Set_Dotmax_ProgramWork(char by_Index,char by_Dat);
//void Display_Segment(char by_Address,const char *pby_Str);
//void Display_WarmUp(char by_Mode);
//void Display_CoolDown(char by_Mode);

void Display_AFG_LOGO(char by_Mode);
//void Display_PersonalBest(char by_Mode);
void Display_Last5Workouts(char by_Mode);
void Display_WorkoutTotals(char by_Mode);
void Display_WorkoutStats(char by_Mode);
void Display_Middle_Matrix(char by_Address,const char *pby_Str);
//void Display_Incline_Custom(char by_Address,const char *pby_Str);
void Display_LeftUp_Segment(char by_Address,const char *pby_Str);
void Display_RightUp_Segment(char by_Address,const char *pby_Str);
//void Display_PaceSplit(char by_Address,const char *pby_Str);
void Display_Left_Down(char by_Address,const char *pby_Str);
void Display_Right_Down(char by_Address,const char *pby_Str);	
void DisplayString(unsigned char x,unsigned char y,unsigned char z,const char *str);
void SetLapCircle(unsigned int LapCircle);
void Display_HEART_ICON(bool b_HeartIconFlag);
void Display_FanIcon (char by_Address);
void Display_LCD_Calories_DATA(char by_Address,const char *pby_Str);
void Display_LCD_Distance_DATA(char by_Address,const char *pby_Str);
void Display_LCD_TIME_DATA(char by_Address,const char *pby_Str);
void Display_RPM(char by_Address,const char *pby_Str);

extern void LCD_Seg14_Char(char* bys_Count,char* bys_String_Loop,char* p_Str);
extern void Display_Regist_1msISR(void);

#endif
