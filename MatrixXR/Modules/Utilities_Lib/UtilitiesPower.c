#include "UtilitiesPower.h"

__no_init static UINT32 w_passcode;

/**************************************************************************//**
 * @brief
 *   掉电检测功能函数。
 *
 * @note
 *   此函数功能为初上电时设置passcode，用于判断是否为初上电。
 *
 * @param[in]
 *   输入为要设定的passcode。
 *****************************************************************************/

void BOR_Set_Passcode(UINT32 passcode)
{
	w_passcode=passcode;
}

/**************************************************************************//**
 * @brief
 *   掉电检测功能函数。
 *
 * @note
 *   此函数功能为得到passcode，用于判断是否为初上电。
 *
 * @return
 *   输出为已设定的passcode。
 *****************************************************************************/

UINT32 BOR_Get_Passcode(void)
{
	return (w_passcode);
}

/**************************************************************************//**
 * @brief
 *   掉电检测功能函数。
 *
 * @note
 *   此函数功能为掉电时延迟3秒，将系统复位，以保存一些值和进行一些必要操作。
 *
 *****************************************************************************/

void BOR_Check_Process(void)
{
	static UCHAR by_Counter = 3;
	if(HAL_BOR_Get_Enable())
	{
		by_Counter = 3;
		return;
	}
	else
	{
		--by_Counter;
		if(by_Counter == 0)
		{
			w_passcode = 0;    
			for(int by_Loop1 = 0;by_Loop1 < 200;by_Loop1++)
			{
				for(int by_Loop2 = 0;by_Loop2 < 65535;by_Loop2++)
				{
					w_passcode = 0;
				}
			}
			HAL_Reset_System();
		}
	}
}