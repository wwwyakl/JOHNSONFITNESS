#include "UtilitiesTimer.h"
#include "Show_Functions.h"

#define T_CHANNEL_MAX 32
#define FLAG_VALUE 0XFFFF

typedef struct
{
	UCHAR TimerFlag;
	UINT16 aw_Timer_Data;
}TimerData;
__no_init static UINT16 w_Timer_Flag;
__no_init static TimerData aw_Timer_Data[T_CHANNEL_MAX];

/**************************************************************************//**
 * @brief
 *   ��ʼ�����ݣ�ע���ж�
 *
 * @note
 *   �˺�������Ϊ��ʼ�����ݣ��Լ�ע������Ҫ���жϡ����ж�Ϊ1ms�жϡ�
 *****************************************************************************/

void Timer_Initial_Data(void)
{
	UCHAR by_Loop;
	w_Timer_Flag=0;
	for(by_Loop=0;by_Loop < T_CHANNEL_MAX;by_Loop++)
	{
		aw_Timer_Data[by_Loop].aw_Timer_Data=0;
		aw_Timer_Data[by_Loop].TimerFlag = 0;
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
void Timer_Set_Flag_100ms_Int(void)
{
	static unsigned char i = 0;
	if(i >= 100)
	{
		i = 0;
		w_Timer_Flag = FLAG_VALUE;
	}
	else
	{
		i++;
	}
    Show_Timer();
}

/**************************************************************************//**
 * @brief
 *   ��ʱ�����ܺ�����
 *
 * @note
 *   �˺�������Ϊ�õ�δʹ�õ�ͨ��ֵ��ѡ��δʹ�õ�ͨ�������ʹ��TimerFlag��1����֮����0��
 *
 * @return
 *   ���Ϊδʹ�õ�ͨ��ֵ��
 *****************************************************************************/

unsigned char Timer_Clear(void)
{
	UCHAR by_Loop = 0;
	
	for( by_Loop = 0; by_Loop < T_CHANNEL_MAX; by_Loop++)
	{
		if( aw_Timer_Data[by_Loop].TimerFlag == 0 )
		{
			aw_Timer_Data[by_Loop].aw_Timer_Data = 0;
			aw_Timer_Data[by_Loop].TimerFlag = 1;
			return by_Loop ;
		}
	}
	return by_Loop ;	
}

/**************************************************************************//**
 * @brief
 *   ��ʱ�����ܺ�����
 *
 * @note
 *   �˺�������Ϊ��ʱ����ʱʱ��ﵽ�󣬷���1��
 *
 * @return
 *   ���Ϊ1��0��
 *****************************************************************************/

unsigned char Timer_Counter(UCHAR channel, UINT16 w_Period)
{
	UCHAR b_Out = 0;
	UCHAR by_Channel;
	UINT32 w_Mask;  
	
	by_Channel = channel & 0x7f;
              if ( by_Channel <= 31 )   
              {
	aw_Timer_Data[by_Channel].TimerFlag = 1;
              }
              else       
                return 0 ;
	w_Mask = 0x01 << by_Channel;
	if((w_Timer_Flag & w_Mask)==0) return(0);
	w_Timer_Flag &= ~w_Mask;
	
	if(aw_Timer_Data[by_Channel].aw_Timer_Data <= w_Period) ++aw_Timer_Data[by_Channel].aw_Timer_Data;
	if(aw_Timer_Data[by_Channel].aw_Timer_Data == w_Period) b_Out=1;
	if(aw_Timer_Data[by_Channel].aw_Timer_Data >= w_Period)
	{	/*******for auto clear "T_LOOP"**************/
		if(channel & 0x80) aw_Timer_Data[by_Channel].aw_Timer_Data=0;
	}
	return(b_Out);
}

/**************************************************************************//**
* @brief
*   ע���жϣ�����mainͳһ����
*
* @note
*   ÿ��ģ���ע���жϺ�������ģ������Ӧ���Ա�mainͳһ��ʼ����������©
*****************************************************************************/

void Timer_Regist_Functions(void)
{
	Hal_Regist_ISR(MS_ISR,TIMER_ISR_IDX,Timer_Set_Flag_100ms_Int);
}