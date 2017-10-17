#include  "ECB.h"

/***************************************************************************/
#define C_MIN_RESISTANCE    0
#define C_RESISTANCE_OFFSET     29 //6


__no_init static UCHAR   by_Resistance;
__no_init static int     RES_CurrentVR_Value;
__no_init static int    RES_TargetVR_Value;
__no_init static char   ECB_Dir;
__no_init static UCHAR   by_Resistance_Old;
__no_init static char   ECB_Pause;
__no_init static unsigned int w_ECB_ADC_Step;
__no_init  char   C_MAX_RESISTANCE;
static unsigned int res_dir_up=0;
static unsigned int res_dir_down=0;
__no_init static struct {
    unsigned INCLINE_ON :   1;
    unsigned S100MS :       1;
    unsigned ENABLE :       1;
}b_ECB;

void ECB_Initial_Data(void)
{
  by_Resistance_Old = 21;//
  by_Resistance = C_MIN_RESISTANCE;
  RES_TargetVR_Value  = RES_VR_Bottom;
  
#ifdef EP
    C_MAX_RESISTANCE = 9; //10-1
#else
    C_MAX_RESISTANCE = 19;//20-1
#endif
    
   ECB_Cul_ADC(); 
   ECB_Pause = 0;
}

/*------------------------------------------
Function Contents: 设置ECB阻力i_offset值
--------------------------------------------
Use Regional : main.C
--------------------------------------------
Input:  Name       Type    Min     Max
1   by_Data        UCHAR    0   by_Resistance_Max
--------------------------------------------
Output:  N/A
--------------------------------------------*/

void ECB_Set_Pause(void)
{
    ECB_Pause = by_Resistance_Old;
    ECB_Set_Resistance(ECB_Pause);
}

void ECB_Cul_ADC(void)
{
    unsigned int w_Dat;
    
    w_Dat = (RES_VR_TOP - RES_VR_Bottom) ;
    w_Dat /= C_MAX_RESISTANCE ;// (C_MAX_RESISTANCE-1);
    w_ECB_ADC_Step = w_Dat;
}

void ECB_ADC_Converter(char by_Dat)
{
    unsigned int w_Dat;
    if(by_Dat <= C_MIN_RESISTANCE + 1 )
    {
        RES_TargetVR_Value = RES_VR_Bottom;
    }
    if(by_Dat >= C_MAX_RESISTANCE + 1 )
    {
        RES_TargetVR_Value = RES_VR_TOP;
    }
    else
    {
        w_Dat = w_ECB_ADC_Step * (by_Dat-1);
//        w_Dat /= 10;
        w_Dat += RES_VR_Bottom;
        RES_TargetVR_Value = w_Dat;
    }
}

void ECB_Set_Resistance(UCHAR by_Data)
{
//   if(by_Resistance_Old != by_Resistance)
//    {
//      by_Resistance_Old = by_Resistance;
//    }
    if(by_Data >= C_MAX_RESISTANCE+1)
    {
        by_Data = C_MAX_RESISTANCE+1;
    }
    else if(by_Data <= C_MIN_RESISTANCE+1)
    {
        by_Data = C_MIN_RESISTANCE+1;
    }
    by_Resistance = by_Data;    
    ECB_ADC_Converter(by_Resistance);
}


/*------------------------------------------
Function Contents:ECB零点设置
--------------------------------------------
Use Regional : main.c
--------------------------------------------
Input:  N/A
--------------------------------------------
Output:  N/A
--------------------------------------------*/

void ECB_Set_Zero(void)
{ 
  by_Resistance_Old = 0;
  by_Resistance = C_MIN_RESISTANCE+1; //C_MIN_RESISTANCE
  RES_TargetVR_Value = RES_VR_Bottom;  
//  HAL_ECB_DOWN();  
}

UINT16 ECB_Get_VR2_ADC(void)
{
    return RES_CurrentVR_Value;
}

//UINT16 tempData[100]={0};
//UCHAR tempindex = 0;
void ECB_VR2_ADC_IntHandle(UINT ulADC)
{
    RES_CurrentVR_Value = ulADC;
//    if(tempindex>=100)
//      tempindex = 0;
//    tempData[tempindex++] = VR2_Value;    
//    VR2_Value >>=1;
//    VR2_Value <<=1;
    ECB_Data_Updata();
}

//unsigned int  Ecb_Dir_Flag=0;
void ECB_Data_Updata(void)
{
  //  if(by_Resistance_Old != by_Resistance)
   if( ( RES_CurrentVR_Value <= RES_TargetVR_Value + C_RESISTANCE_OFFSET*2 ) && ( RES_TargetVR_Value <= RES_CurrentVR_Value + C_RESISTANCE_OFFSET*2 ) )
    {
      HAL_ECB_NONE();
      res_dir_up=0;
      res_dir_down=0;
       //     by_Resistance_Old = by_Resistance ;
    }
    else if( RES_CurrentVR_Value + C_RESISTANCE_OFFSET <= RES_TargetVR_Value)
    {
      HAL_ECB_UP();
      res_dir_up++;
    }
    else if( RES_CurrentVR_Value >= RES_TargetVR_Value + C_RESISTANCE_OFFSET )
    {
      HAL_ECB_DOWN();
      res_dir_down++;
    }
}

void ECB_MotorDir_Set_Test(unsigned char by_Dir)
{
  switch(by_Dir)
  {
  case 1:
    {
      HAL_ECB_UP();
      res_dir_up++;
    }
    break;
  case 2:
    {
      HAL_ECB_DOWN();
      res_dir_down++;
    }
    break;
  default:
    {
      HAL_ECB_NONE();
      res_dir_up=0;
      res_dir_down=0;
    }
    break;
  }
}


void ECB_Flag_100ms_Int(void)
{ 
  static unsigned char by_Counter = 0;
  
  if(by_Counter >= 10)
  {
    by_Counter = 0;
    b_ECB.S100MS = 1;
  }
  else
    by_Counter++;
}