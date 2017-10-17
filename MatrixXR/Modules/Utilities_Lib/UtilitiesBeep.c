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
 *   ��ʼ�����ݣ�ע���ж�
 *
 * @note
 *   �˺�������Ϊ��ʼ��BEEP��ʹ�õ����ݣ��Լ�ע������Ҫ���жϡ����ж�Ϊ1ms�жϡ�
 *****************************************************************************/

void Beep_Initial_Data(void)
{
	w_Time = 0;
	beep.Beep_ON=1;
	beep.Beep_Active = 0;
}

/**************************************************************************//**
 * @brief
 *   ���BEEP�Ƿ�������
 *
 * @note
 *   �˹��ܶ�������Ҫˢ�½�Ҫ��ʾ������ʱ�ж�BEEP�Ƿ�����
 *
 * @return
 *   ����true or false.
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
 *   ����BEEP������BEEP�������ʱ���BEEP�������ʱ��.
 *
 * @note
 *   �˹�����������BEEP������������ʱ�䡣����Ϊ2n+1ʱ����Ĵ���Ϊn+1����Ϊn��
 *   ����Ϊ2n������Ĵ���Ϊn����Ϊn��
 *
 * @param[in] 
 *   ����Ϊ��Ĵ�������ĳ���ʱ�����ĳ���ʱ�䡣
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
 *   �жϺ������жϴ򿪺�1msִ��һ�Ρ�
 *
 * @note
 *   �˺������ڳ�ʼ������ʱע�ᣬ����ִ�С�
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
 *   ����BEEP��
 *
 * @note
 *   �˹������ڿ���BEEP�� 
 *
 * @param[in]
 *   ����true or false.
 *****************************************************************************/

void Beep_Set_Enable(bool isEnabled)
{
	if(isEnabled)
		beep.Beep_ON=1;
	else
		beep.Beep_ON=0;
}
