#if defined(_4_16C24_Display_)

#ifndef _Display_H
#define _Display_H

#include "JISdef.h"

void Display_Bottom_LedString(void);
void Display_DEMO_Process(void);
void Display_Initial_Data(void);
void Display_Set_All(char by_Dat);
void Display_Update_Data(char by_Mode);
void Display_Updata_Data(char by_Mode);
void Display_Time_Segment(char by_Address,const char *pby_Str);//Time
void Display_Left_Above_Segment(char by_Address,const char *pby_Str);//Calories Heart
void Display_Right_Above_Segment(char by_Address,const char *pby_Str);
void Display_Left_Bottom_Segment(char by_Address,const char *pby_Str);//Incline Rpm Mets
void Display_Right_Bottom_Segment(char by_Address,const char *pby_Str);//Mph Speed Resistance Rpm
void Display_Left_Above_Matrix(char by_Address,const char *pby_Str);// 8*41 Matrix
void Display_Right_Above_Matrix(char by_Address,const char *pby_Str);// 8*41 Matrix
void Display_Middle_Matrix(char by_Address,/*const*/ char *pby_Str);//16*35 Matrix
void Display_Bottom_Matrix(char by_Address,const char *pby_Str);//8*119
void Display_Bottom_Midle_Matrix(char by_Address,const char *pby_Str);//8*118
void Display_Total_Matrix(int by_x,int by_y);//use by eng0 display test
void DisplayString(unsigned char x,unsigned char y,unsigned char z,const char *str);
void Display_RF_ICON(char type,const char *pby_Str);

void Display_HEART_ICON(char type,const char *pby_Str);
#endif

#elif defined(_6_16C24_Display_)

#ifndef _Display_H
#define _Display_H

#endif

#endif