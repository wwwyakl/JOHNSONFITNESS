#include "UtilitiesPower.h"

__no_init static UINT32 w_passcode;

/**************************************************************************//**
 * @brief
 *   �����⹦�ܺ�����
 *
 * @note
 *   �˺�������Ϊ���ϵ�ʱ����passcode�������ж��Ƿ�Ϊ���ϵ硣
 *
 * @param[in]
 *   ����ΪҪ�趨��passcode��
 *****************************************************************************/

void BOR_Set_Passcode(UINT32 passcode)
{
	w_passcode=passcode;
}

/**************************************************************************//**
 * @brief
 *   �����⹦�ܺ�����
 *
 * @note
 *   �˺�������Ϊ�õ�passcode�������ж��Ƿ�Ϊ���ϵ硣
 *
 * @return
 *   ���Ϊ���趨��passcode��
 *****************************************************************************/

UINT32 BOR_Get_Passcode(void)
{
	return (w_passcode);
}

/**************************************************************************//**
 * @brief
 *   �����⹦�ܺ�����
 *
 * @note
 *   �˺�������Ϊ����ʱ�ӳ�3�룬��ϵͳ��λ���Ա���һЩֵ�ͽ���һЩ��Ҫ������
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