/************************************************************************************/
#include "UtilitiesBeep.h"

/************************************************************************************/
void Beep_1ms_ISR(void);


__no_init static struct
{
	unsigned Beep_Active: 1;
	unsigned Beep_Dir: 1;
	unsigned Beep_ON: 1;
} beep;

volatile static UCHAR by_Beep_Count = 0;
__no_init static UINT16 w_Beep_ON,w_Beep_OFF;
__no_init static UCHAR by_Beep_Time;//volatile 
__no_init static UINT16 w_Time;


void Beep_Initial_IO(void)
{
	HAL_Beep_Initial_IO();
	HAL_SystemTicker_Alloc_ISR(Beep_1ms_ISR);
}


/**************************************************************************//**
 * @brief
 *   初始化数据，注册中断
 *
 * @note
 *   此函数功能为初始化BEEP所使用的数据，以及注册所需要的中断。此中断为1ms中断。
 *****************************************************************************/

void Beep_Initial_Data(void)
{
	w_Time = 0;
	beep.Beep_ON=1;
	beep.Beep_Active = 0;
}

/**************************************************************************//**
 * @brief
 *   检测BEEP是否已响完
 *
 * @note
 *   此功能多用于在要刷新将要显示的内容时判断BEEP是否响完
 *
 * @return
 *   返回true or false.
 *****************************************************************************/

UCHAR Beep_Get_OK(void)
{
	if(beep.Beep_Active)
		return(0);
	else
		return(1);
}


/**************************************************************************//**
 * @brief
 *   设置BEEP几声，BEEP响持续的时间和BEEP灭持续的时间.
 *
 * @note
 *   此功能用于设置BEEP次数，响和灭的时间。次数为2n+1时，响的次数为n+1，灭为n。
 *   次数为2n数，响的次数为n，灭为n。
 *
 * @param[in] 
 *   输入为响的次数，响的持续时间和灭的持续时间。
 *****************************************************************************/

void Beep_Set_Beeps(UCHAR times, UCHAR onPeriod, UCHAR offPeriod)
{
    if(beep.Beep_Active)
    {
        if(by_Beep_Time>times )return;
    }
   
        if(beep.Beep_ON)
        {
            w_Beep_ON = onPeriod*50;
            w_Beep_OFF = offPeriod*50;
            by_Beep_Time = times;
            beep.Beep_Active=1;
            w_Time=0;
            by_Beep_Count=0;
        }
        else
        {
            beep.Beep_Active=0;
        }
   
}

/**************************************************************************//**
 * @brief
 *   中断函数，中断打开后1ms执行一次。
 *
 * @note
 *   此函数需在初始化数据时注册，否则不执行。
 *
 *****************************************************************************/
void Beep_1ms_ISR(void)
{
	if(beep.Beep_Active)
	{
		if(by_Beep_Count < by_Beep_Time)
		{
			if(by_Beep_Count & 0x01)
			{
				if(w_Time == 0)
				{
					beep.Beep_Dir=0;
					HAL_Beep_Set_Beep(0);  
				}
				if(w_Time >= w_Beep_OFF)
				{
					w_Time=0;
					++by_Beep_Count;
				}
				else
					++w_Time;
			}
			else
			{
				if(w_Time == 0)
				{
					beep.Beep_Dir=1;
					HAL_Beep_Set_Beep(1);  
				}
				if(w_Time >= w_Beep_ON)
				{
					w_Time=0;
					++by_Beep_Count;
				}
				else ++w_Time;
			}
		}
		else
		{
			by_Beep_Count=0;
			w_Time=0;
			beep.Beep_Active=0;
			beep.Beep_Dir=1;
		}
	}
	else 
	{
		HAL_Beep_Set_Beep(0);    
	}
}

/**************************************************************************//**
 * @brief
 *   开关BEEP。
 *
 * @note
 *   此功能用于开关BEEP。 
 *
 * @param[in]
 *   输入true or false.
 *****************************************************************************/

void Beep_Set_Enable(bool isEnabled)
{
	if(isEnabled)
		beep.Beep_ON=1;
	else
		beep.Beep_ON=0;
}
