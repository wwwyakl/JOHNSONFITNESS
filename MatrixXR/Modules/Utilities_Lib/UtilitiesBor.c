#include "UtilitiesBor.h"

#define	C_CheckTime 3

__no_init static UINT32 w_passcode;


void BOR_Initial_Data(void)
{
	
}

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


void BOR_Process_1ms_Int(void)
{
	static unsigned char by_Timesflag = 0;
	if(++by_Timesflag >= C_CheckTime)
	{          
		by_Timesflag = 0;
		BOR_Check_Process();
	}
}

void BOR_Initial_IO(void)
{
	HAL_BOR_Initial_IO();
	HAL_SystemTicker_Alloc_ISR(BOR_Process_1ms_Int);
}
/**************************************************************************//**
 * @brief
 *   �����⹦�ܺ�����
 *
 * @note
 *   �˺�������Ϊ����ʱ�ӳ�3�룬��ϵͳ��λ���Ա���һЩֵ�ͽ���һЩ��Ҫ������
 *
 *****************************************************************************/
//12V��8V 10ms��3.3��2.5 2.5ms��3.3��0 40ms��
void BOR_Check_Process(void)
{//��12V��8V���������
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
        	HAL_16CXX_Set_LCD_OFF();
			w_passcode = 0;  

			UINT16 _delay = 1000;
			while(_delay--);
			HAL_Reset_System();
		}
	}
}
