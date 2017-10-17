#include "UtilitiesFan.h"
#ifdef TM
#define FAN_LEVEL_ONE
#else
#undef FAN_LEVEL_ONE
#endif

__no_init uint8_t FAN_Speed[4];
__no_init static uint8_t by_Current_Level;
static uint8_t Fan_Level_Number;


void Fan_Initial_IO(void)
{
	HAL_FAN_Initial_IO();
    HAL_FAN_Set_Duty(FAN_Speed[by_Current_Level]);
}


/**************************************************************************//**
* @brief
*   初始化数据函数。
*
* @note
*   此函数功能为初始化数据，并将风扇等级分为2级或4级。
*
* @param[in] 
*   输入为风扇参数的结构体。
*****************************************************************************/

void Fan_Initial_Data(FAN_PWM Fan_Pwm)
{		
	uint8_t PWM_BUFFER = 0;	
	Fan_Level_Number = Fan_Pwm.Fan_Level_Number;
	by_Current_Level = 0;
    HAL_FAN_Set_Duty(0);
	
	if(Fan_Pwm.LowPwm <= Fan_Pwm.MidPwm)
	{
		if(Fan_Pwm.MidPwm <= Fan_Pwm.HighPwm)
		{
			;		
		}
		else
		{
			if(Fan_Pwm.LowPwm <= Fan_Pwm.HighPwm)
			{
				PWM_BUFFER = Fan_Pwm.MidPwm;
				Fan_Pwm.MidPwm = Fan_Pwm.HighPwm;
				Fan_Pwm.HighPwm = PWM_BUFFER;
			}
			else
			{			
				PWM_BUFFER = Fan_Pwm.LowPwm;
				Fan_Pwm.LowPwm = Fan_Pwm.HighPwm;
				Fan_Pwm.HighPwm = PWM_BUFFER;
			}
		}
	}
	else
	{
		if(Fan_Pwm.MidPwm > Fan_Pwm.HighPwm)
		{
			PWM_BUFFER = Fan_Pwm.LowPwm;
			Fan_Pwm.LowPwm = Fan_Pwm.HighPwm;
			Fan_Pwm.HighPwm = PWM_BUFFER;		
		}
		else
		{
			if(Fan_Pwm.LowPwm > Fan_Pwm.HighPwm)
			{
				PWM_BUFFER = Fan_Pwm.LowPwm;
				Fan_Pwm.LowPwm = Fan_Pwm.MidPwm;
				Fan_Pwm.MidPwm = PWM_BUFFER;
				
				PWM_BUFFER = Fan_Pwm.MidPwm;
				Fan_Pwm.MidPwm = Fan_Pwm.HighPwm;
				Fan_Pwm.HighPwm = PWM_BUFFER;
			}
			else
			{
				PWM_BUFFER = Fan_Pwm.LowPwm;
				Fan_Pwm.LowPwm = Fan_Pwm.MidPwm;
				Fan_Pwm.MidPwm = PWM_BUFFER;
			}
		}
	}
	
	FAN_Speed[0] = 0;
	FAN_Speed[1] = Fan_Pwm.LowPwm;
	FAN_Speed[2] = Fan_Pwm.MidPwm;
	FAN_Speed[3] = Fan_Pwm.HighPwm;	
}

/**************************************************************************//**
* @brief
*   得到风扇等级函数。
*
* @note
*   此函数功能为返回风扇等级。
*
* @return
*   返回值是风扇等级。
*****************************************************************************/

unsigned char Fan_Get_Level(void)
{
	return(by_Current_Level);
}

/**************************************************************************//**
* @brief
*   设置风扇等级函数。
*
* @note
*   此函数功能为设置风扇等级。
*
* @param[in] 
*   输入值是风扇等级。
*****************************************************************************/

void Fan_Set_Level(unsigned char by_Dat)
{
	if(by_Dat <= 3)
	{
		by_Current_Level = by_Dat;
		HAL_FAN_Set_Duty(FAN_Speed[by_Dat]);
	}
	else
	{
		by_Current_Level = 3;
		HAL_FAN_Set_Duty(FAN_Speed[3]);
	}
}

/**************************************************************************//**
* @brief
*   改变风扇等级函数。
*
* @note
*   此函数功能为增加风扇等级。
*
*****************************************************************************/

void Fan_Increase_Level(void)
{	
//	if(Fan_Level_Number == 2)
//	{
//		if(by_Current_Level == 3)
//		{
//			by_Current_Level = 2;
//		}
//		else
//			by_Current_Level++;
//	}
//	else
//	{
//		if(by_Current_Level == 3)
//		{
//			by_Current_Level = 0;
//		}
//		else
//			by_Current_Level++;
//	}
    HAL_FAN_Set_Duty(FAN_Speed[by_Current_Level]);
}

/**************************************************************************//**
* @brief
*   改变风扇等级函数。
*
* @note
*   此函数功能为减少风扇等级。
*
*****************************************************************************/

void Fan_Decrease_Level(void)
{
	if(Fan_Level_Number == 2)
	{
		if(by_Current_Level == 3)
		{
			by_Current_Level = 2;
		}
		else
		{
			by_Current_Level = 3;
		}
	}
	else
	{
		if(by_Current_Level == 0)
		{
			by_Current_Level = 3;
		}
		else
			by_Current_Level--;
	}
    HAL_FAN_Set_Duty(FAN_Speed[by_Current_Level]);
}
#ifdef FRAMEWORK2_0
VR_KEY UTL_Fan_Set_Virtualkey(VR_KEY vk)
{
	VR_KEY ret = vk;
	if(vk == VK_FAN)
	{
		Fan_Increase_Level();
		ret = VK_PROCESSED ;
	}
	return ret ;
}
#else
void UTL_Fan_Set_Virtualkey(UINT16 vk)
{
	UINT8 realKey = vk&0x00ff;
    UINT16 _stage = vk&0xff00;    
	if(realKey == VK_FAN && _stage!= FRAME_STAGE_ENG && _stage!= FRAME_STAGE_AGINGTEST)
	{
#ifdef FAN_LEVEL_ONE
       if(by_Current_Level == 0)
       {
           by_Current_Level = 2;
           Show_LEDLight(LED11_FAN,LED_ON);    //ZSF
       }
       else
       {
           by_Current_Level = 0;
           Show_LEDLight(LED11_FAN,LED_OFF);    //ZSF
       }
#endif  
		Fan_Increase_Level();
	}
}
#endif
