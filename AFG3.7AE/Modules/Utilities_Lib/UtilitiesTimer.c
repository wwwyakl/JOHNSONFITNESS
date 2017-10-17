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
 *   初始化数据，注册中断
 *
 * @note
 *   此函数功能为初始化数据，以及注册所需要的中断。此中断为1ms中断。
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
 *   中断函数，中断打开后1ms执行一次。
 *
 * @note
 *   此函数需在初始化数据时注册，否则不执行。
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
 *   定时器功能函数。
 *
 * @note
 *   此函数功能为得到未使用的通道值，选择未使用的通道，如果使用TimerFlag置1，反之，置0。
 *
 * @return
 *   输出为未使用的通道值。
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
 *   定时器功能函数。
 *
 * @note
 *   此函数功能为定时，定时时间达到后，返回1。
 *
 * @return
 *   输出为1或0。
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
*   注册中断，方便main统一调用
*
* @note
*   每个模块初注册中断函数均与模块名对应，以便main统一初始化，不会遗漏
*****************************************************************************/

void Timer_Regist_Functions(void)
{
	Hal_Regist_ISR(MS_ISR,TIMER_ISR_IDX,Timer_Set_Flag_100ms_Int);
}