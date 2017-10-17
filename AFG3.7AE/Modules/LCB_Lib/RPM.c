/*
* Function: Get rpm
*
* Description: 通過定時器及中斷計算rpm值,增加ENG8相關功能函數
*
* New Design: Version: S001
*             By: Spirit
*             Date: 2004/12/4
*
* Use Region:  EP/Bike Rpm
*
* Modify : Version: S002
*          By:         wangjm
*          Date:       2007.9.30
*          Content:
*          Total ROM Size:  242
*          Total RAM Size:  11
*          Interrupt :     TACAP/TBCAP,1ms Interrupt
*/

/************************************************************************/
#include "RPM.h"
/************************************************************************/


#define C_Rpm_TIMEBASE      60000//60060//61440               
#define C_Rpm_MAX           220  
#define C_Rpm_LOW_LIMIT     C_Rpm_TIMEBASE/250  
#define C_Rpm_HIGH_LIMIT    C_Rpm_TIMEBASE/15   
#define C_Rpm_CLAER_TIME    1                   


__no_init static unsigned int by_Rpm,by_Check,by_Rpm_Auto;
__no_init UINT16 w_Rpm_Counter,w_Rpm_Data;

__no_init static struct 
{
    unsigned Rpm_OK   : 1;
}Rpm;


void RPM_Initial_Data(void)   
{
    by_Rpm = 0;
    w_Rpm_Counter = C_Rpm_HIGH_LIMIT;
    w_Rpm_Data = C_Rpm_HIGH_LIMIT;
}

void RPM_Calculate_Counter_Int(void)     
{
    if(w_Rpm_Counter > C_Rpm_HIGH_LIMIT) 
    {
        w_Rpm_Data = w_Rpm_Counter;
        w_Rpm_Counter = 0;
        Rpm.Rpm_OK = 1;                    
    }
    else
    {
        w_Rpm_Counter++;
    }
}



void RPM_Reload_Int(void)
{
    if(w_Rpm_Counter >= C_Rpm_LOW_LIMIT) 
    {
        w_Rpm_Data = w_Rpm_Counter;
        Rpm.Rpm_OK = 1;
        w_Rpm_Counter = 0;
    }
}

//UCHAR RPM_IO_Check_RPM(void)
//{
//    if(HW_RPM_Get_IO()) return 1;
//    else return 0;
//}

UCHAR RPM_Decoder(void)
{
    static UCHAR by_Frist_Time = 0;
    UCHAR b_Out;
    b_Out = 0; 
    
    if(by_Check)
    {
        return (1); 
    }
    if(Rpm.Rpm_OK)
    {
       Rpm.Rpm_OK = 0;
       b_Out = 1;
       if(w_Rpm_Data > C_Rpm_HIGH_LIMIT )
       {
          by_Frist_Time = 0;
          by_Rpm = 0;
       }
       else
       {
          if(by_Frist_Time < C_Rpm_CLAER_TIME)   
          {
             ++by_Frist_Time;
          }
          else
          {
             by_Rpm = C_Rpm_TIMEBASE*10/w_Rpm_Data+1;
             by_Rpm /= 10;
          }
       }
    }
    return (b_Out);
}


UCHAR RPM_Get_RPM(void)
{
    if(by_Check)
    {
        return (RPM_Get_AutoRPM()); 
    }
    else
    {
       if(by_Rpm > C_Rpm_MAX)
       {
          return(C_Rpm_MAX);
       }
       else
       {
          return(by_Rpm);
       }
    }
}

void RPM_Add(void)
{
    if(by_Rpm_Auto < 220)
    {
        by_Rpm_Auto += 5;
    }
}

void RPM_Set_Auto(unsigned int set)
{
    if(set <= 220)
    {
        by_Rpm_Auto = set;
    }
}

void RPM_DEC(void)
{
    if(by_Rpm_Auto > 5)
    {
        by_Rpm_Auto -= 5;
    }
    else
    {
        by_Rpm_Auto = 0;
    }
}

UCHAR RPM_Get_AutoRPM(void)
{
    if(by_Rpm_Auto > 220)
    {
        by_Rpm_Auto = 220;
        return(by_Rpm_Auto);
    }
    else
    {
        return(by_Rpm_Auto);
    }
}

void RPM_AUTO_Check(UCHAR by_Mode)
{
    if(by_Mode)
    {
        by_Check = 1;
    } 
    else
    {
        by_Check = 0;
    }
}

unsigned char RPM_Get_AUTO_Check(void)
{
  return by_Check;
}

void RPM_AutoRPM_Clear(void)
{
    by_Rpm_Auto = 0;
}


