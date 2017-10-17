/***********************************/
#include "Incline.h"
/**********************************/

#define C_INCLINE_DELAY     10  //0.1sec
#define C_INCLINE_OFFSET     25 //9  //6


__no_init static struct {
    unsigned INCLINE_ON :   1;
    unsigned S100MS :       1;
    unsigned ENABLE :       1;
}b;

__no_init static int Incline_TargetADC_Value;
__no_init static int Incline_CurrentADC_Value;
__no_init  unsigned int w_Incline_ADC_Step;
__no_init static unsigned char by_ADC_Index;
//__no_init static char  INCLINE_Dir;
__no_init  char  by_Incline_old;
__no_init  char  by_Incline;
extern FRAME_STAGE Frame_Stage;
extern UINT16 Eng1_Set_InclineADC;
unsigned int incline_dir_up=0;
unsigned int incline_dir_down=0;


void Incline_Input_Enable(unsigned char by_Dat)
{
    if(by_Dat)
    {
        b.ENABLE = 1;
    }
    else
    {
        b.ENABLE = 0;
    }
}

/*
void Incline_Set_MAX(unsigned char by_Dat)
{
    by_Incline_MAX = by_Dat;
}

unsigned char Incline_Get_MAX(void)
{
    return(by_Incline_MAX);
}
*/

void Incline_Initial_Data(void)
{
    by_Incline_old = 2 ;
    Incline_Cul_ADC();
    by_Incline = 0 ;
    Incline_Set_Incline(0);
//    Incline_Updata_Incline();
}



void Incline_Cul_ADC(void)
{
    unsigned int w_Dat;
    
//    w_Dat = (w_ADC_MAX - w_ADC_MIN) * 10;
//    w_Dat /= (by_Incline_MAX);
      w_Dat = (w_InclineADC_MAX - w_InclineADC_MIN) ;
      w_Dat /= by_Incline_MAX;
      w_Incline_ADC_Step = w_Dat;
}

//unsigned int Incline_Dir_Flag=0;
void  Incline_Updata_Incline(void)
{
  static unsigned char by_Up_Delay = 0,by_Down_Delay = 0;
  
  //    Incline_ADC_Converter(Incline_Get_Incline());
  //    b.INCLINE_ON = 1;  

  if(Frame_Stage != FRAME_STAGE_ENG )
  {
//    if( by_Incline_old != by_Incline )
//    {
  if((Incline_CurrentADC_Value <= Incline_TargetADC_Value + C_INCLINE_OFFSET*2)&& ( Incline_CurrentADC_Value + C_INCLINE_OFFSET*2 >= Incline_TargetADC_Value ))
      {
        HAL_Incline_Set_Up(0);
        incline_dir_up=0;
        incline_dir_down=0;
//         by_Incline_old = by_Incline;
      }
      else if(Incline_CurrentADC_Value + C_INCLINE_OFFSET <= Incline_TargetADC_Value)
      {
        HAL_Incline_Set_Up(1);
        incline_dir_up++;    
      }
      else if(Incline_CurrentADC_Value >= Incline_TargetADC_Value + C_INCLINE_OFFSET)
      {
        HAL_Incline_Set_Down(1);
        incline_dir_down++;
      }
//    }
  }
  else
  {
    if((Incline_CurrentADC_Value <= Eng1_Set_InclineADC + C_INCLINE_OFFSET*2)&& ( Incline_CurrentADC_Value + C_INCLINE_OFFSET*2 >= Eng1_Set_InclineADC ))
      {
        HAL_Incline_Set_Up(0);
        incline_dir_up=0;
        incline_dir_down=0;
      }
      else if(Incline_CurrentADC_Value + C_INCLINE_OFFSET <= Eng1_Set_InclineADC)
      {
        HAL_Incline_Set_Up(1);
        incline_dir_up++;    
      }
      else if(Incline_CurrentADC_Value >= Eng1_Set_InclineADC + C_INCLINE_OFFSET)
      {
        HAL_Incline_Set_Down(1);
        incline_dir_down++;
      }
  }
}

void Incline_MotorDir_Set_Test(unsigned char by_Dir)
{
  switch( by_Dir )
  {
  case 1:
    {
        HAL_Incline_Set_Up(1);
        incline_dir_up++;
    }
    break;
  case 2:
    {
        HAL_Incline_Set_Down(1);
        incline_dir_down++;
    }
    break;
  default:
    {
        HAL_Incline_Set_Up(0);
        incline_dir_up = 0;
        incline_dir_down = 0;
    }
    break;
  }
}

void Incline_Set_Zero(void)
{
    by_Incline_old = 3 ;
    by_Incline = 0 ;
//    i_ADC_Target = w_ADC_MIN;
    Incline_TargetADC_Value = w_InclineADC_MIN;
//    b.INCLINE_ON = 1;
}


void Incline_Set_Incline(unsigned char by_Dat)
{
//  if(!b.ENABLE) return;
    if(by_Dat >= by_Incline_MAX)
  {
    by_Dat = by_Incline_MAX;
  }
  else if(by_Dat <= 0)
  {
    by_Dat = 0;
  }
  by_Incline = by_Dat;
  Incline_ADC_Converter(by_Incline);
//  b.INCLINE_ON = 1;
}

unsigned char Incline_Set_Up(void)
{
    if(Incline_CurrentADC_Value < (950 - C_INCLINE_OFFSET))
    {
        //HW_IO_Clear_UP();
        //HW_IO_Set_DOWN();
        HW_Incline_Set_Up(1);
        return (0);
    }
    else
    {
        //Incline_Set_Stop();
        HW_Incline_Set_Up(0);
        return(1);
    }
}

unsigned char Incline_Set_Down(void)
{
    if(Incline_CurrentADC_Value > (50 + C_INCLINE_OFFSET))
    {
        //HW_IO_Clear_DOWN();
        //HW_IO_Set_UP();
        HW_Incline_Set_Down(1);
        return(0);
    }
    else
    {
        //Incline_Set_Stop();
        HW_Incline_Set_Down(0);
        return(1);
    }
}

void Incline_Set_Stop(void)
{
    //HW_IO_Set_DOWN();
    //HW_IO_Set_UP();
    HW_Incline_Set_Down(0);
}

void Incline_Flag_100ms_Int(void)
{
    b.S100MS = 1;
}

void Incline_ADC_Converter(char by_Dat)
{
    unsigned int w_Dat;
    if(by_Dat <= 0)
    {
      Incline_TargetADC_Value = w_InclineADC_MIN;
    }
    if(by_Dat >= by_Incline_MAX)
    {
      Incline_TargetADC_Value = w_InclineADC_MAX;
    }
    else
    {
        w_Dat = w_Incline_ADC_Step * by_Dat;
//        w_Dat /= 10;
        w_Dat += w_InclineADC_MIN;
        Incline_TargetADC_Value = w_Dat;
    }
}

char Incline_Proce_AE(unsigned int i_incline)
{    
    Incline_CurrentADC_Value=i_incline;
    Incline_Updata_Incline(); 
}

//ADCÍ¨µÀÇÐ»»º¯Êý





