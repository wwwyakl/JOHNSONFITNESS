#ifndef __INCLINE_H
#define __INCLINE_H

#include "HAL_Lib.h"

#define  w_InclineADC_MAX	 3715 // 4096 12bit  
#define  w_InclineADC_MIN	 915  // 
#define  by_Incline_MAX	 100   // 

extern  char  by_Incline_old;
extern  char  by_Incline;
extern unsigned int w_Incline_ADC_Step;

void Incline_Initial_Data(void);
void Incline_ADC_Read_1ms_Int(void);
void Incline_Cul_ADC(void);
void Incline_Set_ADC_MAX(unsigned int w_Dat);
void Incline_Set_ADC_MIN(unsigned int w_Dat);
unsigned int Incline_Get_ADC_MAX(void);
unsigned int Incline_Get_ADC_MIN(void);
unsigned int Incline_Get_ADC(void);
unsigned int Incline_Get_ADC_Target(void);
unsigned char Incline_Set_Up(void);
unsigned char Incline_Set_Down(void);
void Incline_Set_Stop(void);
void Incline_ADC_Converter(char by_Dat);
char Incline_Proce(void);
char Incline_Check_Move(void);
unsigned char Incline_Get_Incline(void);
char Incline_Get_Base(void);
void Incline_Set_Incline(unsigned char by_Dat);
void Incline_Updata_Incline(void);
void Incline_Add_Incline(void);
void Incline_Dec_Incline(void);
void Incline_Flag_100ms_Int(void);
//void Incline_Set_Base(char by_Dat);
unsigned char Incline_Get_Percent(void);
void Incline_Set_Base(unsigned char by_Dat);
void Incline_Add_Base(void);
void Incline_Dec_Base(void);
void Incline_Set_MAX(unsigned char by_Dat);
unsigned char Incline_Get_MAX(void);
unsigned char Incline_Get_Program(unsigned char by_Dat);
void Incline_Set_Zero(void);
void Incline_Input_Enable(unsigned char by_Dat);
void Incline_Power_Check_ADC_Read_Int(void);
unsigned char Incline_Get_Power_Check_ADC(void);

void ADC_Initial_Data(void);
void ADC_Process_1ms_Int(void);
unsigned int ADC_Get_Incline_ADC(void);

#endif
