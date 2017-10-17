/*------------------------------------------------------------------------------
                     ( Dot Matrix Window )
      
         012345 -----> Scanning direction
       ---------------------------------------------
     0 | oooooooooooooooooooooooo  ...  oooooooooo | 
     1 | oooooooooooooooooooooooo  ...  oooooooooo |
     2 | oooooooooooooooooooooooo  ...  oooooooooo |
     3 | oooooooooooooooooooooooo  ...  oooooooooo |
     . |                           ...
     7 | oooooooooooooooooooooooo  ...  oooooooooo |
     | ---------------------------------------------
     v
  Scanning direction

每一行(直)的数据的缓存器 ==> DotMatrix_Window[]
------------------------------------------------------------------------------*/
#include "Show_Functions.h"
#include "LetterCodeTable.h"
//#include "wifi_api.h"
#include "Show.h"

#define _Message_Keep   0 // 定字符串
#define _Message_Loop   1 // 卷动字符串
#define _Message_NotYet 2 // 无动作

union
{
  struct
  {
      unsigned char StopShow : 1;
      unsigned char Show_Start : 1;
      unsigned char DataFilling : 1;
      unsigned char DataUpdate : 1;
      unsigned char Flag4 : 1;
      unsigned char Flag5 : 1;
      unsigned char Flag6 : 1;
      unsigned char Flag7 : 1;
  }Flag;
  unsigned char all;
}Show_State;

unsigned short DotMatrix_Window[_StringWindow_Width + 10] = {0}; // The buffers which place hardware data
unsigned char WindowString[_StringWindow_Letters_Max + 2] = {0}; // The buffers which place the letters
unsigned char Pre_Motion = 0;
unsigned char Pre_StringLength = 0;
unsigned char ChangeWindow = 0;
unsigned short Show_DelayTimer = 0;
unsigned short DotMatrixIntervalTimer = 0;
unsigned short StopShowTimer = 0;
unsigned short CurrentLetterCount = 0;
unsigned char DotMovedNumber_String = 0;
unsigned char DotMovedNumber_Small[16] = {0};
unsigned char CurrentShowType = 0;
unsigned long SetTimeOut = 0;
unsigned char Window_ShiftDots = Window_Shift_Dots;
unsigned char StringWindow_NowWidth = _StringWindow_Width;
unsigned char StringWindow_NowLetters = _StringWindow_Letters_Max;

/*******************************************************************************
* Function Name  : Show_Set_StringWindow_Width()
* Description    : Set the letters displayed
* Input          : Mode, 0:use all, 1:Copper Test  
* Output         : None
* Return         : None
*******************************************************************************/
void Show_Set_StringWindow_Width(unsigned char by_Mode)
{
    Show_Clear_Window(0);
    switch(by_Mode)
    {
        default:
            StringWindow_NowWidth = _StringWindow_Width;
            StringWindow_NowLetters = _StringWindow_Letters_Max;
            break;
        case _WindowLetters_CooperTest:    
            StringWindow_NowWidth = _FitTestStringWindow_Width;
            StringWindow_NowLetters = _FitTestStringWindow_Letters_Max;
            break;
    }
    Show_Initial();
}
/*******************************************************************************
* Function Name  : Show_Initial()
* Description    : Initial Parameters
* Input          : None  
* Output         : None
* Return         : None
*******************************************************************************/
void Show_Initial(void)
{
    unsigned char i = 0;
    
    Pre_Motion = 0;
    Pre_StringLength = 0;
    ChangeWindow = 0;
    Show_DelayTimer = 0;
    DotMatrixIntervalTimer = 0;
    Show_Clear_Window(1);
    for(i = 0; i < StringWindow_NowLetters; i++)
        WindowString[i] = 0;
    CurrentLetterCount = 0;
    DotMovedNumber_String = 0;
    for(i = 0; i < 16; i++)
        DotMovedNumber_Small[i] = 0;
    StopShowTimer = 0;
    Show_State.all = 0;
    Window_ShiftDots = Window_Shift_Dots;
}
/*******************************************************************************
* Function Name  : Show_Clear_Window()
* Description    : Clear the hardware buffers(DotMatrix_Window[]).
* Input          : None  
* Output         : None
* Return         : None
*******************************************************************************/
void Show_Clear_Window(unsigned char by_Mode)
{
    unsigned char i;
    switch(by_Mode)
    {
        default:
            for(i = 0; i < StringWindow_NowWidth; i++)
                DotMatrix_Window[i] = 0;
            break;
        case 0:    
            for(i = 0; i < _StringWindow_Width; i++)
                DotMatrix_Window[i] = 0;
            break;
    }
}

/*******************************************************************************
* Function Name  : Show_Timer()
* Description    : 1ms Timer. Please place in timer interrupt
* Input          : None  
* Output         : None
* Return         : None
*******************************************************************************/
void Show_Timer(void)
{// 1ms
    if(Show_DelayTimer <= 60000)
        Show_DelayTimer++; 
    if(DotMatrixIntervalTimer <= DotMatrix_DotTime_)
        DotMatrixIntervalTimer++;
    
    if(Show_State.Flag.StopShow == 1)
    {
        if(StopShowTimer <= 60000)
            StopShowTimer++;
    }
    else
        StopShowTimer = 0;
    
}
/*******************************************************************************
* Function Name  : Led_String()
* Description    : Show the string.
* Input          : 1.*String : 对应字符串 
*                  2. StringNumber : 字符串编号
*                  3. Motion : 字符串动作
* Output         : None
* Return         : 0=单字符串 / 1=跑马灯
*******************************************************************************/
unsigned char Led_String(unsigned char *String, unsigned char StringNumber, unsigned char Motion)
{
    unsigned char by_Out=_Message_Keep;
    unsigned char StringLength = 0;
    //unsigned char NumberMoved = 0, j = 0;
    unsigned char i = 0, WindowLetterNumber = 0;
    static unsigned char Pre_StringNumber = 0;
    static unsigned char *PreStringPointer ;

    StringWindow_NowLetters = StringNumber;
    StringNumber = 0;
    
    if(Motion == Show_Blank)
    {
        Show_Initial();
        Pre_StringNumber = 0;
        return _Message_Keep;
    }
    // Calculate the length of string
    while(*(String+StringLength) != 0)
    {
        StringLength += 1;
    }
    
    if(Motion == Show_Auto)
    {// 大于窗口可以显示的字数时，以转动的方式显示
        //Motion = ((StringLength != StringWindow_NowLetters)?Show_DownToUp:Show_Fixed);
        Motion = ((StringLength > StringWindow_NowLetters)?Show_RightToLeft:Show_MiddleFixed);
    }
    if(Motion != Show_Fixed && Motion != Show_MiddleFixed)
    {
        by_Out = _Message_Loop;
    }
    
    if(Motion == Show_Fixed || Motion == Show_MiddleFixed)
    {
        if(StringLength == StringWindow_NowLetters)
        {
            for(i = 0; i < StringWindow_NowLetters; i++)
            {
                if((*(String+i)) != WindowString[i])
                {
                    Show_State.Flag.DataUpdate = 1;
                    break;
                }
            }
            i = 0;
        }
    }
    
    
       
    // Check the previous data
    if(Motion != Pre_Motion || StringLength != Pre_StringLength || StringNumber != Pre_StringNumber ||\
     PreStringPointer != String || Show_State.Flag.DataUpdate == 1)
    {
        Window_ShiftDots = Window_Shift_Dots;
        Show_DelayTimer = 0;
        DotMatrixIntervalTimer = 0;
        StopShowTimer = 0;
        Pre_Motion = Motion;
        Pre_StringLength = StringLength;
        Pre_StringNumber = StringNumber;
        PreStringPointer = String;
        CurrentLetterCount = 0;
        for(i = 0; i < StringWindow_NowLetters; i++)
            WindowString[i] = 0;
        ChangeWindow = 0;
        DotMovedNumber_String = 0;
        Show_State.all = 0x00;
        Show_Clear_Window(1);
        Show_State.Flag.DataUpdate = 1;
        Show_State.Flag.Show_Start = 1;
    }

    if(ChangeWindow == 0)
    {
        ChangeWindow = 1;
        Window_ShiftDots = Window_Shift_Dots;
        switch(Motion)
        {
            case Show_RightToLeft:
                 if(CurrentLetterCount < StringLength)
                 {
                     if(CurrentLetterCount == 0)
                     {
                        for(i = 0; i < StringWindow_NowLetters; i++)
                           WindowString[i] = 0x00;
                     }
                     else
                     {
                        for(i = 0; i < StringWindow_NowLetters; i++)
                          WindowString[i] = WindowString[i + 1];
                     }
                     WindowString[i] = *(String + CurrentLetterCount);
                     CurrentLetterCount += 1;
                 }
                 else
                 {
                    for(i = 0; i < StringWindow_NowLetters; i++)
                          WindowString[i] = WindowString[i + 1];
                    WindowString[i] = 0x00;
                    CurrentLetterCount += 1;
                 }
                 break;
            case Show_UpToDown:
            case Show_DownToUp:  
                 // Search the whole word
                 i = 0;
                 while(i < (StringWindow_NowLetters + 1))
                 {
                     if(*(String + CurrentLetterCount + i) == 0)
                     {
                         WindowLetterNumber = i;
                         break;
                     }
                     else if(*(String + CurrentLetterCount + i) == ' ')  
                     {
                         if(i >= StringWindow_NowLetters)
                         {
                            WindowLetterNumber = i;
                         }
                         else
                            WindowLetterNumber = i + 1;
                     }
                     i++;
                 }
                 // Fill the string to buffer.
                 for(i = 0; i < StringWindow_NowLetters; i++)
                 {
                     if(i < WindowLetterNumber)
                        WindowString[i] = *(String + CurrentLetterCount + i);
                     else
                        WindowString[i] = ' ';     
                 }
                 CurrentLetterCount += WindowLetterNumber;
                 // Adjust the string to the center
                 for(i = WindowLetterNumber; i < StringWindow_NowLetters; i++)
                 {
                    WindowString[i] = 0;
                 }
                 Window_ShiftDots += (StringWindow_NowLetters - WindowLetterNumber)*Letter_Width/2;  
                 if(WindowString[WindowLetterNumber - 1] == ' ')
                 {
                      WindowString[WindowLetterNumber - 1] = 0x00;
                      Window_ShiftDots += Letter_Width >> 1;
                 }
                 if(WindowString[0] == ' ')
                 {
                    for(i=1;i<StringWindow_NowLetters; i++)
                        WindowString[i-1] = WindowString[i];
                    WindowString[StringWindow_NowLetters - 1] = 0x00;
                    Window_ShiftDots += Letter_Width >> 1;
                 }
                 break;     
            default:
                 if(StringLength <= StringWindow_NowLetters)
                    WindowLetterNumber = StringLength;
                 else
                    WindowLetterNumber = StringWindow_NowLetters;
                 
                 // Fill the string to buffer.
                 for(i = 0; i < StringWindow_NowLetters; i++)
                 {
                     if(i < WindowLetterNumber)
                        WindowString[i] = *(String + CurrentLetterCount + i);
                     else
                        WindowString[i] = ' ';     
                 }
                 CurrentLetterCount += WindowLetterNumber;
                 if(Motion == Show_MiddleFixed)
                 {// Adjust the string to the center
                      if(WindowLetterNumber < StringWindow_NowLetters)
                          Window_ShiftDots += ((StringWindow_NowLetters - WindowLetterNumber)*Letter_Width/2);  
                 }
                 break; 
        }
    }
    
    if(ChangeWindow == 1)
    {
        switch(DotMatrix_SetString(WindowString, Motion))
        {
            case 1:
                ChangeWindow = 0;
                if(Motion == Show_Fixed || Motion == Show_MiddleFixed)
                {
                    Show_State.Flag.Show_Start = 0;
                    CurrentLetterCount = 0;
                }
                else if(Motion == Show_RightToLeft)
                {
                    if(CurrentLetterCount >= (StringLength + StringWindow_NowLetters + 1))
                    {
                        CurrentLetterCount = 0;
                        Show_State.Flag.Show_Start = 0;
                    }
                }
                else
                {
                    if(CurrentLetterCount >= StringLength)
                    {
                        CurrentLetterCount = 0;
                        Show_State.Flag.Show_Start = 0;
                    }
                }
                
                break;
            case 2:
                if(Motion == Show_UpToDown || Motion == Show_DownToUp)
                    Show_State.Flag.StopShow = 1;
                break;
            default:
                break;
        }
    }
  return by_Out;
}

/*******************************************************************************
* Function Name  : DotMatrix_TableSearch()
* Description    : Search the letter code
* Input          : Letter
* Output         : None
* Return         : Letter code
*******************************************************************************/
unsigned char DotMatrix_TableSearch(unsigned char SearchLetter)
{
     //  char by_Out = 127;// 空格
     
     //  if(SearchLetter >= 'A' && SearchLetter <= 'Z') 
     //       by_Out = SearchLetter - 'A' + 10;
     //  else if(SearchLetter >= 'a' && SearchLetter <= 'z') 
     //       by_Out = SearchLetter - 'a' + 36;
     //  else if(SearchLetter >= '0' && SearchLetter <= '9') 
     //       by_Out = SearchLetter - '0';
     //  else if(SearchLetter >= 160 && SearchLetter <= 215) 
     //       by_Out = SearchLetter - 160 + 62;
     //  else if(SearchLetter == 216) by_Out = 128;
     //  else if(SearchLetter == 218) by_Out = 129;
     //  else if(SearchLetter == '"') by_Out = 117;
     //  else if(SearchLetter == '*') by_Out = 118;
     //  else if(SearchLetter == '`') by_Out = 119;
     //  else if(SearchLetter == '.') by_Out = 120;
     //  else if(SearchLetter == ',') by_Out = 121;
     //  else if(SearchLetter == ':') by_Out = 122;
     //  else if(SearchLetter == '/') by_Out = 123;
     //  else if(SearchLetter == '!') by_Out = 124;
     //  else if(SearchLetter == '%') by_Out = 125;
     //  else if(SearchLetter == '-') by_Out = 126;
     //  else if(SearchLetter == '[') by_Out = 136;
     //  else if(SearchLetter == ']') by_Out = 137;
     char by_Out = SearchLetter;
     if(SearchLetter >= ' ' && SearchLetter <= 'z') by_Out = SearchLetter-' ';
     if(SearchLetter >= 'a' && SearchLetter <= 'z') by_Out = (SearchLetter-'a')+('A'-' ');
     
     return by_Out;
}
/*******************************************************************************
* Function Name  : DotMatrix_SetString()
* Description    : Place the data to the hareware buffer(DotMatrix_Window[]).
* Input          : string and motion
* Output         : None
* Return         : Letter code
*******************************************************************************/
unsigned char DotMatrix_SetString(unsigned char *String, unsigned char Motion)
{
    unsigned char i = 0, j = 0;
    unsigned long DotMask = 0;
    unsigned char Rtn = 0;
    unsigned short RemainingDot = 0;
    unsigned short DotCheck = 0;
    
    if(Show_State.Flag.StopShow == 1)
    {
        DotMatrixIntervalTimer = 0;
        if(StopShowTimer > (DotMatrix_StopTime - DotMatrix_DotTime_))
          Show_State.Flag.StopShow = 0;
    }
    else
    {
        if(DotMatrixIntervalTimer >= DotMatrix_DotTime_ || Show_State.Flag.DataUpdate == 1)
        {
            Show_State.Flag.DataFilling = 1;
            Show_State.Flag.DataUpdate = 0;
            DotMatrixIntervalTimer = 0;    
            switch(Motion)
            {
                default:
                case Show_Fixed:
                case Show_MiddleFixed:
                    Rtn = 1;
                    break;
                case Show_UpToDown:
                    DotMovedNumber_String += 1;
                    if(DotMovedNumber_String <= (Letter_Height + 1))
                    {
                        if(Letter_Height >= 32)
                            DotMask = 0xFFFFFFFF;
                        else
                            DotMask =  ((1 << Letter_Height) - 1);
                        
                        DotMask -= (1 << (Letter_Height + 1 - DotMovedNumber_String)) - 1;
                    } 
                    else   
                    {
                        DotMask = (1 << ((Letter_Height + 1) - (DotMovedNumber_String - Letter_Height) )) - 1;
                    }
                    
                    if(DotMovedNumber_String == (Letter_Height + 1))
                        Rtn = 2;
                    else if(DotMovedNumber_String >= (Letter_Height * 2 + 1))
                        Rtn = 1;
                    break;
                case Show_DownToUp:    
                    DotMovedNumber_String += 1;
                    if(DotMovedNumber_String <= (Letter_Height + 1))
                        DotMask = (1 << (DotMovedNumber_String - 1)) - 1;
                    else   
                    {
                        if(Letter_Height >= 32)
                            DotMask = 0xFFFFFFFF;
                        else
                            DotMask =  ((1 << Letter_Height) - 1);
                        DotMask -= (1 << (DotMovedNumber_String - 1 - Letter_Height)) - 1;
                    }
                    
                    if(DotMovedNumber_String == (Letter_Height + 1))
                        Rtn = 2;
                    else if(DotMovedNumber_String >= (Letter_Height * 2 + 1))
                        Rtn = 1;
                    break;
                case Show_RightToLeft:
                    RemainingDot = StringWindow_NowLetters * Letter_Width;
                    if(RemainingDot < StringWindow_NowWidth)
                        RemainingDot = StringWindow_NowWidth - RemainingDot;
                    else
                        RemainingDot = 0;
                    
                    DotMovedNumber_String += 1;
                    if(DotMovedNumber_String >= Letter_Width)
                        Rtn = 1;
                    break;     
            }
            if(Motion == Show_RightToLeft)
            {
                for(i = 0; i< RemainingDot; i++)
                  DotMatrix_Window[i] = DotMatrix_Window[i + 1];
            }
            
            // 填窗口显示数据
            for(i = 0; i < StringWindow_NowLetters; i++)
            {       
                for(j = 0; j < Letter_Width; j++)
                {
                    switch(Motion)
                    {
                        default:
                        case Show_Fixed:
                        case Show_MiddleFixed:  
                            DotCheck = i*Letter_Width + j + Window_ShiftDots;
                            if(DotCheck < StringWindow_NowWidth)
                            {
                                DotMatrix_Window[DotCheck] = nAsciiDotTable[DotMatrix_TableSearch(*(String + i))][j];
                            }
                            break;
                        case Show_UpToDown:
                            DotCheck = i*Letter_Width + j + Window_ShiftDots;
                            if(DotCheck < StringWindow_NowWidth)
                            {
                                    if(DotMovedNumber_String <= Letter_Height + 1)
                                        DotMatrix_Window[DotCheck] = ((nAsciiDotTable[DotMatrix_TableSearch(*(String + i))][j]) & DotMask) >> (Letter_Height + 1 - DotMovedNumber_String);
                                    else
                                        DotMatrix_Window[DotCheck] = ((nAsciiDotTable[DotMatrix_TableSearch(*(String + i))][j]) & DotMask) << (DotMovedNumber_String - Letter_Height - 1);
                            }
                            break;
                        case Show_DownToUp:
                            DotCheck = i*Letter_Width + j + Window_ShiftDots;
                            if(DotCheck < StringWindow_NowWidth)
                            {
                                    if(DotMovedNumber_String <= Letter_Height + 1)
                                        DotMatrix_Window[DotCheck] = ((nAsciiDotTable[DotMatrix_TableSearch(*(String + i))][j]) & DotMask) << (Letter_Height + 1 - DotMovedNumber_String);
                                    else
                                        DotMatrix_Window[DotCheck] = ((nAsciiDotTable[DotMatrix_TableSearch(*(String + i))][j]) & DotMask) >> (DotMovedNumber_String - Letter_Height - 1);
                            }
                            break;
                        case Show_RightToLeft:
                                if((j + DotMovedNumber_String) < Letter_Width)
                                    DotMatrix_Window[i*Letter_Width + j + RemainingDot] = nAsciiDotTable[DotMatrix_TableSearch(*(String + i))][j + DotMovedNumber_String];
                                else
                                    DotMatrix_Window[i*Letter_Width + j + RemainingDot] = nAsciiDotTable[DotMatrix_TableSearch(*(String + i + 1))][j + DotMovedNumber_String - Letter_Width];
                            break;     
                    }
                }
            }
            
            if(Rtn == 1)
            {
                DotMovedNumber_String = 0;
            }
            Show_State.Flag.DataFilling = 0;
        }
    }
    return Rtn;
}

/*******************************************************************************
* Function Name  : Show_Message_End()
* Description    : Place the data to the hareware buffer(DotMatrix_Window[]).
* Input          : None
* Output         : None
* Return         : 0:Showing
                   1:Showing End
*******************************************************************************/
unsigned char Led_Get_String_End(void)
{
  return (Show_State.Flag.Show_Start == 1 ? 0 : 1);
}

void Led_Set_Reset(void)
{
    Show_State.Flag.Show_Start = 1;
}
void Led_Set_Strig_End(void)
{
    Show_State.Flag.Show_Start = 0;
}
/*******************************************************************************
* Function Name  : DotMatrix_Number()
* Description    : Set the small window for showing the number.(以区段式的方式来显示数字
* Input          : GroupNumber : 显示组编号, 用于区别timer
                 : BeginningDot : 显示起始点
                 : DigitNumber :  位总数(整数 + 小数)
                 : DecimalPoint :  小数位数
                 : IsTime : 1:Yes, 0:Number
                 : LetterType : 8x5, 8x4
                 : NumberData :  显示的数字
                 : Motion :  显示的动作
* Output         : None
* Return         : None
*******************************************************************************/
//void DotMatrix_Number(unsigned char GroupNumber, unsigned short BeginningDot, unsigned char DigitNumber,
//                      unsigned char IsTime, unsigned char DecimalPoint, unsigned LetterType,
//                      unsigned long NumberData, unsigned char Motion)
//{    
//    unsigned long Integer = 0; // 整数
//    unsigned long DecimalFraction = 0; // 小数
//    unsigned short i, j, Digits, Digits_2;
//    unsigned long Dat = 1, Dat_2 = 0;
//    unsigned short CurrentDot = BeginningDot;
//    unsigned char LetterWidth = 0;
//    unsigned short NumberWindow_Width;
//    
//    //
//    Digital_Command_Proce_wifi();
//    //
//    if(DotMatrixIntervalTimer >= DotMatrix_DotTime_)
//    {
//        DotMatrixIntervalTimer = 0;
//        SetTimeOut = 0xFFFFFFFF;
//    }
//    
//    if((SetTimeOut&(1<<GroupNumber)) != 0 || Motion == Show_Fixed)
//    {
//        SetTimeOut &= (0xFFFFFFFF - (1<<GroupNumber));
//        
//        switch(LetterType)
//        {
//            case Letter_8x4:
//                LetterWidth = LetterWidth_8x4;
//                NumberWindow_Width = DigitNumber * LetterWidth_8x4;
//                break;
//            default:
//            case Letter_8x5:
//                LetterWidth = LetterWidth_8x5;
//                NumberWindow_Width = DigitNumber * LetterWidth_8x5;
//                break;  
//        }
//        
//        Show_State.Flag.DataFilling = 1;
//        // Limit the digits
//        if(DigitNumber > 9) // 最多9位数
//          DigitNumber = 9;
//        if(DecimalPoint >= DigitNumber) // 最多小数点7位数
//          DecimalPoint = DigitNumber - 1;
//        
//        // Get the integer part
//        if(IsTime == 1)
//        {
//            Integer = NumberData/60;
//            DecimalFraction = NumberData % 60;
//            DecimalPoint = 2;
//        }
//        else
//        {
//            for(i = 0; i < DigitNumber; i++)
//                Dat *= 10;
//            NumberData = NumberData % Dat; 
//            Dat = 1;
//            Integer = NumberData;
//            for(i = 0; i < DecimalPoint; i++)
//            {
//                Integer /= 10;
//                Dat *= 10;
//            }
//            // Get the decimal fraction
//            DecimalFraction = NumberData % Dat;
//        }
//        //----------------------------------------------------------------------
//        // Fill the dot data to hardware
//        Dat_2 = Integer;
//        for(Digits_2 = 0;; Digits_2++)
//        {
//            if((Dat_2/10) == 0)
//            {
//                Digits_2 += 1;
//                break;
//            }
//            Dat_2 /= 10;
//        }
//        // Display the integer
//        Digits = DigitNumber - DecimalPoint;
//        for(i = 0; i < (Digits - Digits_2); i++)
//        {
//            for(j = 0; j < LetterWidth; j++)
//                DotMatrix_Window[i*LetterWidth+j+CurrentDot] = 0x00;
//        }
//        CurrentDot += (Digits - Digits_2)*LetterWidth;
//        for(i = Digits_2; i > 0 ; i--)
//        {
//            DotMatrix_SetLetter((CurrentDot + (i - 1)*LetterWidth), LetterType, (Integer%10));
//            Integer /= 10;
//        }  
//        CurrentDot += (Digits_2 * LetterWidth);
//       
//        if(IsTime == 1)
//        {// 冒号   
//            DotMatrix_SetLetter(CurrentDot, LetterType, ((LetterType == Letter_8x4)?15:122));
//            CurrentDot += LetterWidth;
//            NumberWindow_Width += LetterWidth;
//        }
//        else
//        { // 小数点
//            if(DecimalPoint != 0)
//            {
//                DotMatrix_SetLetter(CurrentDot, LetterType, ((LetterType == Letter_8x4)?16:120));
//                CurrentDot += LetterWidth;
//                NumberWindow_Width += LetterWidth;  
//            }
//        }
//        // 小数 
//        for(i = DecimalPoint; i > 0 ; i--)
//        {
//            Dat = 1;
//            for(j = 0; j < (i - 1); j++)
//                Dat *= 10;
//            
//            DotMatrix_SetLetter(CurrentDot, LetterType, (DecimalFraction/Dat));
//            DecimalFraction %= Dat;
//            CurrentDot += LetterWidth;
//        }  
//              
//        switch(Motion)
//        {
//            case Show_Fixed:
//            case Show_MiddleFixed:  
//                 break;
//            case Show_UpToDown:
//                 for(i = BeginningDot; i < (NumberWindow_Width + BeginningDot); i++)
//                 {
//                    if(DotMovedNumber_Small[GroupNumber] <= Letter_Height)
//                        DotMatrix_Window[i] >>= (Letter_Height - DotMovedNumber_Small[GroupNumber]);
//                    else
//                        DotMatrix_Window[i] <<= (DotMovedNumber_Small[GroupNumber] - Letter_Height);
//                 }
//                 DotMovedNumber_Small[GroupNumber]++;
//                 if(DotMovedNumber_Small[GroupNumber] >= (Letter_Height*2 + 1)) //(Letter_Height * 2 + 1))
//                    DotMovedNumber_Small[GroupNumber] = 0 ; //0;
//                 break;
//            case Show_DownToUp:
//                 for(i = BeginningDot; i < (NumberWindow_Width + BeginningDot); i++)
//                 {
//                    if(DotMovedNumber_Small[GroupNumber] <= Letter_Height)
//                        DotMatrix_Window[i] <<= (Letter_Height - DotMovedNumber_Small[GroupNumber]);
//                    else
//                        DotMatrix_Window[i] >>= (DotMovedNumber_Small[GroupNumber] - Letter_Height);
//                 }
//                 DotMovedNumber_Small[GroupNumber]++;
//                 if(DotMovedNumber_Small[GroupNumber] >= (Letter_Height*2 + 1)) //(Letter_Height * 2 + 1))
//                    DotMovedNumber_Small[GroupNumber] = 0; //0;
//                 break;
//            case Show_RightToLeft:
//                 if(DotMovedNumber_Small[GroupNumber] < NumberWindow_Width)
//                 {
//                     for(i = NumberWindow_Width; i > DotMovedNumber_Small[GroupNumber] ; i--)
//                       DotMatrix_Window[i - 1] = 0x00;
//                     for(i = DotMovedNumber_Small[GroupNumber]; i > 0; i--)
//                     {
//                        DotMatrix_Window[NumberWindow_Width - 1 - (DotMovedNumber_Small[GroupNumber] - i)] = DotMatrix_Window[i - 1];
//                        DotMatrix_Window[i - 1] = 0x00;
//                     }
//                 }
//                 else if(DotMovedNumber_Small[GroupNumber] > NumberWindow_Width)
//                 {
//                     for(i = 0; i < (DotMovedNumber_Small[GroupNumber] - NumberWindow_Width) ; i++)
//                       DotMatrix_Window[i] = 0x00;
//                     for(i = 0; i < (NumberWindow_Width - (DotMovedNumber_Small[GroupNumber] - NumberWindow_Width)); i++)
//                     {
//                        DotMatrix_Window[i] = DotMatrix_Window[i + (DotMovedNumber_Small[GroupNumber] - NumberWindow_Width)];
//                        DotMatrix_Window[i + (DotMovedNumber_Small[GroupNumber] - NumberWindow_Width)] = 0x00;
//                     }
//                 }
//
//                 DotMovedNumber_Small[GroupNumber]++;  
//                 if(DotMovedNumber_Small[GroupNumber] >= (NumberWindow_Width*2+1))
//                    DotMovedNumber_Small[GroupNumber] = 0;
//                 break; 
//            default:     
//            case Show_Blank:
//                 for(i = BeginningDot; i < (NumberWindow_Width + BeginningDot); i++)
//                   DotMatrix_Window[i] = 0x00;  
//                 break;
//        }
//        
//        Show_State.Flag.DataFilling = 0;
//    }
//}
void DotMatrix_SetLetter(unsigned char Position, unsigned char LetterType, unsigned char Data)
{
    unsigned char i = 0;
    
    switch(LetterType)
    {
        case Letter_8x4 : // 8 x 4
            for(i = 0; i < LetterWidth_8x4; i++)
            {
                DotMatrix_Window[Position + i] = SmallLetterTable[Data][i];
            }
            break;
        default: // 8 x 5
        case Letter_8x5:
            for(i = 0; i < LetterWidth_8x5; i++)
            {
                DotMatrix_Window[Position + i] = LetterTable[Data][i];
            }
            break; 
    }
}

unsigned char Show_DataFilling_End(void)
{
    return (Show_State.Flag.DataFilling == 1 ? 0 : 1);
}

/*******************************************************************************
// 
*******************************************************************************/
void Led_Number(unsigned char GroupNumber, unsigned long NumberData, unsigned char Motion)
{
//    NumberInfo SelWindows[_8x5_Windows_Number] = 
//    {
//        {(0*LetterWidth_8x5)  + Window_Shift_Dots, 4,0,0},    // 1  ->左 XXXX
//        {(12*LetterWidth_8x5) + Window_Shift_Dots, 4,0,0},    // 2  ->右 XXXX
//        {(6*LetterWidth_8x5)  + Window_Shift_Dots, 4,1,0},    // 3  ->中 XX:XX
//        {(0*LetterWidth_8x5)  + Window_Shift_Dots, 3,0,1},    // 4  ->左 XX.x
//        {(11*LetterWidth_8x5) + Window_Shift_Dots, 4,0,1},    // 5  ->右 XXX.x
//        {(11*LetterWidth_8x5) + Window_Shift_Dots, 4,1,0},    // 6  ->右 XX:XX
//        {(5*LetterWidth_8x5)  + Window_Shift_Dots, 4,0,0},    // 7  ->中 XXXX
//        {(12*LetterWidth_8x5) + Window_Shift_Dots, 3,0,2},    // 8  ->右 X.xx
//        {(11*LetterWidth_8x5) + Window_Shift_Dots, 5,0,0},    // 9  ->右 XXXXX
//        {(10*LetterWidth_8x5) + Window_Shift_Dots, 6,0,0},    // 10 ->右 XXXXXX
//        {(8*LetterWidth_8x5)  + Window_Shift_Dots, 7,0,1},    // 11 ->右 XXXXXX.X
//        {(7*LetterWidth_8x5)  + Window_Shift_Dots, 2,0,0},    // 12 ->中 XX
//        {(14*LetterWidth_8x5) + Window_Shift_Dots, 2,0,0},    // 13 ->右 XX 
//        {(4*LetterWidth_8x5)  + Window_Shift_Dots, 9,0,0},    // 14 ->中 XXXXXXXXX
//        {(11*LetterWidth_8x5) + Window_Shift_Dots, 3,0,0}     // 15 ->中偏右 XXX 
//    };
    
//    DotMatrix_Number(GroupNumber-1, SelWindows[GroupNumber-1].w_BeginningDot, SelWindows[GroupNumber-1].w_Digits, 
//    SelWindows[GroupNumber-1].w_IsTime, SelWindows[GroupNumber-1].w_DecimalPoint, Letter_8x5, NumberData, Motion);
}

//========================================================================================================================
//====>设定数值显示位置设定 1.第几排 2.数值 (运动显示值用 4x8字库)
//=============================================================================================
void Led_NumberSite_4x8(unsigned char by_Seg,unsigned char by_Dat)
{
   unsigned char i;  
   if(by_Seg > (_StringWindow_Width - LetterWidth_8x4 + 1)) 
      by_Seg = (_StringWindow_Width - LetterWidth_8x4 + 1);//==>避免超出显示范围
   for(i=0;i < LetterWidth_8x4;i++)
   {
      DotMatrix_Window[by_Seg-1+i]=SmallLetterTable[by_Dat][i];
   }

}
//========================================================================================================================
//====>设定数值显示位置设定 1.第几排 2.数值 (5x8字库)
//=============================================================================================
void Led_NumberSite(unsigned char by_Seg,unsigned char by_Dat)
{
   unsigned char i;
   if(by_Seg > (_StringWindow_Width - LetterWidth_8x5)) 
      by_Seg = (_StringWindow_Width - LetterWidth_8x5);//==>避免超出显示范围
   for(i=0;i < LetterWidth_8x5;i++)
   {
      DotMatrix_Window[by_Seg-1+i] = LetterTable[by_Dat][i];
   }
}












