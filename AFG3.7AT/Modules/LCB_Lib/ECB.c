#include  "ECB.h"
//#include  "ECB_Table.h"
// #include "Information.h"  
// #include "Program.h"
/***************************************************************************/
//#define C_MAX_RESISTANCE  19
#define C_MIN_RESISTANCE  0

#define ECB_NONE	0
#define ECB_UP		1
#define ECB_DOWN        2

__no_init static char 	by_Base;
__no_init static INT8  	i_Offset;//偏移值
__no_init static UCHAR 	by_Resistance;
__no_init static int   	VR2_Value;
__no_init static int    TargetVR2;
__no_init static char   ECB_Dir;
__no_init static char   by_Resistance_Old;
__no_init static char   C_MAX_RESISTANCE;
__no_init static char   ECB_Icon;
__no_init static char   ECB_Pause;
//__no_init static char   ECB_start;

void ECB_Initial_Data(void)
{
	by_Base  = 0;
    i_Offset = 0;	//阻力偏移值为零
    by_Resistance = C_MIN_RESISTANCE;
	TargetVR2  = VR2_Bottom;
	ECB_Dir    = ECB_NONE;
	
        
//        if(Information_Get_Data(IM_MACHINE_NAME)==1)
        {
//           C_MAX_RESISTANCE = 19;
          C_MAX_RESISTANCE = 9; 
          ECB_Icon = 0;//ecb
          by_Resistance_Old = 31;//sean
        }
//         else
  /*      
        {
          C_MAX_RESISTANCE = 29;
          ECB_Icon = 1;//电磁铁
          by_Resistance_Old = 31;
        }
  */      
        ECB_Pause = 0;
        
//        ECB_start = 1;
}

char ECB_Get_Ecb_Icon(void)
{//0 ecb  1 电磁铁
  return ECB_Icon;
}

/*------------------------------------------
Function Contents: 设定ECB BASE值
--------------------------------------------
Use Regional : main.C
--------------------------------------------
Input:  Name       Type    Min     Max
1   by_Data        UCHAR    0   by_Resistance_Max
--------------------------------------------
Output:  N/A
--------------------------------------------*/
void ECB_Set_Base(UCHAR by_Data)
{
    if(by_Data >C_MAX_RESISTANCE)
    {
        by_Base =C_MAX_RESISTANCE;//最大阻力
    }
    by_Base = by_Data;
    by_Resistance = by_Base + i_Offset;//加上偏移值，上电后初始化为零
    if(by_Resistance > C_MAX_RESISTANCE)
    {
        by_Resistance = C_MAX_RESISTANCE;
    }
    if((by_Base + i_Offset) < C_MIN_RESISTANCE)
    {
        by_Resistance = C_MIN_RESISTANCE;//偏移值可能为负数，但是最小阻力就是零
    }
}

void ECB_Add_Base(void)
{
    if(by_Base < C_MAX_RESISTANCE)
    {
        ++by_Base;
    }
    by_Resistance = by_Base+i_Offset;
    if(by_Resistance > C_MAX_RESISTANCE)
    {
        by_Resistance = C_MAX_RESISTANCE;
    }
    if((by_Base + i_Offset) < C_MIN_RESISTANCE)
    {
        by_Resistance = C_MIN_RESISTANCE;
    }
}

void ECB_Dec_Base(void)
{
    if(by_Base > 0)
    {
        --by_Base;
    }

    by_Resistance = by_Base + i_Offset;
    if(by_Resistance > C_MAX_RESISTANCE)
    {
        by_Resistance = C_MAX_RESISTANCE;
    }
    if((by_Base + i_Offset) < C_MIN_RESISTANCE)
    {
        by_Resistance = C_MIN_RESISTANCE;
    }
}

UCHAR ECB_Get_Base(void)
{
    return(by_Base);
}

void ECB_Initial_Offset(void)
{
  	i_Offset=0;
}

signed char ECB_Get_Offset(void)
{
    return(i_Offset);
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
    ECB_Pause = by_Resistance;
    ECB_Set_Resistance(0);
}

void ECB_Set_Start(void)
{
    
    ECB_Set_Resistance(ECB_Pause);
    ECB_Pause = 0;
}

void ECB_Set_Resistance(UCHAR by_Data)
{/*   
    if(by_Data > C_MAX_RESISTANCE)
    {
        by_Data = C_MAX_RESISTANCE;
    }
    by_Resistance = by_Data;
    i_Offset = by_Resistance - by_Base;
 */
   
    if((by_Data-1) > C_MAX_RESISTANCE)
    {
        by_Data = C_MAX_RESISTANCE+1;
    }
    /*
    else
    {
      if((by_Data-1) < C_MIN_RESISTANCE)
      {
        
      }
    }
    */
    by_Resistance = by_Data-1;
    
}

void ECB_Set_Terrian_Resistance(signed int by_Dat)
{
    if((by_Dat+by_Resistance)>C_MAX_RESISTANCE)
    {
      by_Resistance = C_MAX_RESISTANCE;
      i_Offset = by_Resistance - by_Base;
      return;
    }
    if((by_Dat+by_Resistance)<0)
    {
      by_Resistance = 0;
      i_Offset = 0 - by_Base;
      return;
    }
    
    by_Resistance = by_Dat+by_Resistance;
    i_Offset = by_Dat+i_Offset;
    return;
}

void ECB_Add_Resistance(void)
{
    if(C_MAX_RESISTANCE > (i_Offset + by_Base))
    {
        if((i_Offset + by_Base) < C_MIN_RESISTANCE)
        {
            by_Resistance = C_MIN_RESISTANCE;
            i_Offset = by_Resistance - by_Base;
        }
        ++i_Offset;
        by_Resistance = i_Offset+by_Base;
    }
}

void ECB_Dec_Resistance(void)
{
    if(C_MIN_RESISTANCE < (i_Offset + by_Base))
    {
        if((i_Offset + by_Base) > C_MAX_RESISTANCE)
        {
            by_Resistance = C_MAX_RESISTANCE;
            i_Offset = by_Resistance - by_Base;
        }
        --i_Offset;
        by_Resistance =(UINT8)i_Offset+by_Base;
    }
}

UCHAR ECB_Get_Resistance(void)
{
    UCHAR by_Out;
    if((by_Base + i_Offset) > C_MAX_RESISTANCE)		by_Out = C_MAX_RESISTANCE;
    else if((by_Base+i_Offset)<C_MIN_RESISTANCE)	by_Out = C_MIN_RESISTANCE;
    else											by_Out = by_Base+i_Offset;
    return(by_Out);
}

UCHAR ECB_Get_Max_Resistance(void)
{
//    UCHAR by_Out;
//    if((by_Base + i_Offset) > C_MAX_RESISTANCE)		by_Out = C_MAX_RESISTANCE;
//    else if((by_Base+i_Offset)<C_MIN_RESISTANCE)	by_Out = C_MIN_RESISTANCE;
//    else											by_Out = by_Base+i_Offset;
    return(C_MAX_RESISTANCE);
}

UCHAR ECB_Get_Res_Show(void)
{
    UCHAR by_Out;
    if((by_Base + i_Offset) > C_MAX_RESISTANCE)		by_Out = C_MAX_RESISTANCE;
    else if((by_Base+i_Offset)<C_MIN_RESISTANCE)	by_Out = C_MIN_RESISTANCE;
    else											by_Out = by_Base+i_Offset;
    return(by_Out+1);
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
  
//  if(ECB_Icon)
  /*
  {
    HW_EMS_Set_Level(0); 
  }
  */
//  else
  {
	ECB_Dir = ECB_DOWN;
	TargetVR2 = VR2_Bottom;
	by_Resistance = C_MIN_RESISTANCE;
        
        HAL_ECB_DOWN();
  }
}

UINT16 ECB_Get_VR2_ADC(void)
{
    return VR2_Value;
}

void ECB_VR2_ADC_IntHandle(UINT ulADC)
{
  //  if(ECB_start)
  {
    VR2_Value = ulADC;
    switch(ECB_Dir)
    {
    case ECB_UP:
      if(VR2_Value>TargetVR2)
      {
        ECB_Dir = ECB_NONE;
        HAL_ECB_NONE();
      }
      break;
    case ECB_DOWN:
      if(VR2_Value<TargetVR2)
      {
        ECB_Dir = ECB_NONE;
        HAL_ECB_NONE();
      }
      break;
    case ECB_NONE:
      ECB_Dir = ECB_NONE;
      HAL_ECB_NONE();
      break;
    }
  }
}


void ECB_Data_Updata(void)
{
  ///////////////ECB///////////////////
  //  ECB_start = 1;
//  if(ECB_Icon==0)
  {
    if(by_Resistance_Old != by_Resistance)
    {
      by_Resistance_Old = by_Resistance;
      TargetVR2 = (VR2_TOP-VR2_Bottom)/C_MAX_RESISTANCE*by_Resistance + VR2_Bottom;
      if(VR2_Value < TargetVR2)
      {
        ECB_Dir = ECB_UP;
        HAL_ECB_UP();
      }
      else if(VR2_Value > TargetVR2)
      {
        ECB_Dir = ECB_DOWN;
        HAL_ECB_DOWN();
      }
      else if(VR2_Value == TargetVR2)
      {
        ECB_Dir = ECB_NONE;
        HAL_ECB_NONE();
      }
    }
  }
//  else
 /* 
  {
    if(  (Program_Get_Mode()==P_CONSTANT_WATT)
       ||(Program_Get_Mode()==P_2ND_WATT_PROGRAM)
         ||(Program_Get_Mode()==P_FITNESS_TEST)
           )return;
    
    ///////////////EMS///////////////////
    HW_EMS_Set_Level(by_Resistance); 
  }
  */
}

void ECB_Set_Test(char by_Dir,int by_Target)
{
  switch(by_Dir)
  {
  case 0:
    if(TargetVR2-by_Target>=VR2_Bottom)
      TargetVR2 -=by_Target;
    else
      TargetVR2=100;
    ECB_Dir = ECB_DOWN;
    HW_ECB_DOWN();
    break;
  case 1:
    if(TargetVR2+by_Target<=VR2_TOP)
     TargetVR2 +=by_Target;
    else
      TargetVR2=2500;
    ECB_Dir = ECB_UP;
    HW_ECB_UP();
    break;
  case 2:
    TargetVR2 =VR2_Value;
    ECB_Dir = ECB_NONE;
    HW_ECB_NONE(); 
    break;
  default:break;
  }
}

UINT16 ECB_Get_Target_ADC(void)
{
    return TargetVR2;
}