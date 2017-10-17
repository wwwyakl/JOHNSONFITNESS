#ifndef __SHOW_FUNCTION_H__
#define __SHOW_FUNCTION_H__


typedef struct
{
    unsigned char w_BeginningDot;
    unsigned char w_Digits;
    unsigned char w_IsTime; 
    unsigned char w_DecimalPoint;
}NumberInfo;

// Word size (unit : Dot)
#define LetterWidth_8x5  6 
#define LetterWidth_8x4  5 // for Workout Info
//
#define   RealLetterHeight    8// Real Letter height (Don't change)
#define   Letter_Width        LetterWidth_8x4//LetterWidth_8x5
#define   Letter_Height       RealLetterHeight

// Window width (unit : Dot)
#define   _StringWindow_Width     118
#define   _FitTestStringWindow_Width      (_StringWindow_Width - 25) 

// The max number of the Letters displayed on the window. (unit : Letters)
#define   _StringWindow_Letters_Max  (_StringWindow_Width/Letter_Width)
#define   _FitTestStringWindow_Letters_Max      (_FitTestStringWindow_Width/Letter_Width)

// 
#define   _WindowLetters_All   0
#define   _WindowLetters_CooperTest 1

// Shift Dot
#define  Window_Shift_Dots  ((_StringWindow_Width - _StringWindow_Letters_Max*Letter_Width)>>1)  // 不能超过2点

// The string motion
#define   Show_Fixed        0   // Fix the string
#define   Show_UpToDown     1   // Scroll from up to down
#define   Show_DownToUp     2   // Scroll from down to up
#define   Show_RightToLeft  3   // Move from Right to Left
#define   Show_Blank        4   // Blank
#define   Show_Auto         5   // 自动调整转动状态
#define   Show_MiddleFixed  6   // 自动将字符串固定中间 // 用于显示字符串

// Letter Type
#define   Letter_8x5   0
#define   Letter_8x4   1
#define   IsNumber_    0
#define   IsTime_      1
//
#define   DotMatrix_DotTime_        (32+96)/4
#define   DotMatrix_StopTime        1500

// The position and digits of the shown Led Number
#define _8x5_Windows_Number  15
#define _8x4_Windows_Number  10
#define _Show_Left_4Int       1 // 1->左 XXXX
#define _Show_Right_4Int      2 // 2->右 XXXX
#define _Show_Middle_Time     3 // 3->中 XX:XX
#define _Show_Left_2Int1Dec   4 // 4->左 XX.x
#define _Show_Right_3Int1Dec  5 // 5->右 XXX.x
#define _Show_Right_Time      6 // 6->右 XX:XX
#define _Show_Middle_4Int     7 // 7->中 XXXX
#define _Show_Right_1Int2Dec  8 // 8->右 X.xx
#define _Show_Right_5Int      9 // 9->右 XXXXX
#define _Show_Right_6Int      10 //10->右 XXXXXX
#define _Show_Right_6Int1Dec  11 //11->右 XXXXXX.X
#define _Show_Middle_2Int     12 //12->中 XX
#define _Show_Right_2Int      13 //13->右 XX 
#define _Show_Middle_9Int     14 //14->中 XXXXXXXXX

void Show_Regist_Functions(void);
void Show_Set_StringWindow_Width(unsigned char by_Mode);
void Show_Initial(void);
void Show_Timer(void);
void Show_Clear_Window(unsigned char by_Mode);
unsigned char DotMatrix_TableSearch(unsigned char SearchLetter);
unsigned char DotMatrix_SetString(unsigned char *String, unsigned char Motion);
unsigned char Led_String(unsigned char *String, unsigned char StringNumber, unsigned char Motion);
unsigned char Led_Get_String_End(void);
void Led_Set_Reset(void);
void Led_Set_Strig_End(void);
void DotMatrix_Number(unsigned char GroupNumber, unsigned short BeginningDot, unsigned char DigitNumber,
                      unsigned char IsTime, unsigned char DecimalPoint, unsigned LetterType,
                      unsigned long NumberData, unsigned char Motion);
void DotMatrix_SetLetter(unsigned char Position, unsigned char LetterType, unsigned char Data);
unsigned char Show_DataFilling_End(void);
void Led_Number(unsigned char GroupNumber, unsigned long NumberData, unsigned char Motion);
//void Led_Number_4x8(unsigned char GroupNumber, unsigned long NumberData, unsigned char Motion);
void Led_NumberSite_4x8(unsigned char by_Seg, unsigned char by_Dat);
void Led_NumberSite(unsigned char by_Seg, unsigned char by_Dat);
//
extern unsigned short DotMatrix_Window[_StringWindow_Width + 10];

#endif /* __SHOW_FUNCTION_H__ */
