#include "UtilitiesBor.h"

#define	C_CheckTime 3

__no_init static UINT32 w_passcode;

void BOR_Initial_IO(void)
{
	HAL_BOR_Initial_IO();
}


void BOR_Initial_Data(void)
{
	
}

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


void BOR_Process_1ms_Int(void)
{
	static unsigned char by_Timesflag = 0;
	if(++by_Timesflag >= C_CheckTime)
	{          
		by_Timesflag = 0;
		BOR_Check_Process();
	}
}


/**************************************************************************//**
 * @brief
 *   掉电检测功能函数。
 *
 * @note
 *   此函数功能为掉电时延迟3秒，将系统复位，以保存一些值和进行一些必要操作。
 *
 *****************************************************************************/
//12V到8V 10ms；3.3到2.5 2.5ms；3.3到0 40ms；
void BOR_Check_Process(void)
{//从12V到8V进入掉电检测
	static UCHAR by_Counter = 3;
	if(HAL_BOR_Get_Enable())
	{
		by_Counter = 5;
		return;
	}
	else
	{
		--by_Counter;
		if(by_Counter == 0)
		{
			HAL_Disable_MasterInterrupt();
//        	HAL_16CXX_Set_LCD_OFF();
			w_passcode = 0;  

			UINT32 _delay = 120000;
			while(_delay--);
			HAL_Reset_System();
		}
	}
}


/**************************************************************************//**
* @brief
*   注册中断，方便main统一调用
*
* @note
*   每个模块初注册中断函数均与模块名对应，以便main统一初始化，不会遗漏
*****************************************************************************/

void Bor_Regist_Functions(void)
{
	Hal_Regist_ISR(MS_ISR,BOR_ISR_IDX,BOR_Process_1ms_Int);
}
