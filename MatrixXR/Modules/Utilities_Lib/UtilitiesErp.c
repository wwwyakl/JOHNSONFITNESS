#include "UtilitiesErp.h"


#define ERP_TIMEOUT_COUNTER 50

__no_init static char by_number;
__no_init unsigned int w_Time_ERP;
__no_init static bool ERP_1S_FLAG;
__no_init static bool ERP_QuickIn_FLAG;
__no_init static UINT8 ERP_Refresh_Counter;
__no_init static UINT16 w_Timer;
__no_init static UINT16 ERP_TimeOut_Counter;


void ERP_Mode(void);
void ERP_1ms_Int(void);
void ERP_LETIMER_10ms_Int(void);


typedef enum
{
	ERP_IDLE,
	ERP_DEMO,
	ERP_COUNTING,
	ERP_MODE,
	ERP_END,
	ERP_STATE_TOTAL
}
ERP_STATE;

__no_init ERP_STATE Erp_State ;


/**************************************************************************//**
* @brief
*   初始化数据，注册中断。
*
* @note
*   此函数功能为初始化ERP所使用的数据，以及注册所需要的中断。此中断为1ms中断。
*****************************************************************************/

void ERP_Initial_Data(void)
{
	w_Timer = 0;
	by_number = 10;
	w_Time_ERP = 0;
    ERP_TimeOut_Counter = 0;
	ERP_QuickIn_FLAG = 0 ;
    Erp_State = ERP_IDLE ;
}

/**************************************************************************//**
* @brief
*   中断函数，中断打开后1ms执行一次。
*
* @note
*   此函数功能为每100ms标志位置1，用于计时。此中断为1ms中断。
*   
*****************************************************************************/
void ERP_1ms_Int(void)
{
	static unsigned char by_Times100ms = 0;	
	static UCHAR by_times1s = 0;
	if(by_Times100ms < 100)
	{
		by_Times100ms++;
		return;
	}
	by_Times100ms = 0;

	w_Time_ERP++;
	ERP_Refresh_Counter ++;
	if(w_Time_ERP>= SystemConfig_Get_Int(DEMO_WAIT_MIN_UINT16)*10 )
	{
		if(Erp_State == ERP_IDLE)
		{
			Erp_State = ERP_DEMO;
			ERP_Refresh_Counter = 3;
		}
	}
    if((w_Time_ERP >= SystemConfig_Get_Int(ERP_WAIT_MIN_UINT16)*10) 
		&& (SystemConfig_Get_Char(ENERGY_SAVER_FLAG_UCHAR)||ERP_QuickIn_FLAG )
		) 
    	{
			if((Erp_State == ERP_IDLE)||((Erp_State == ERP_DEMO)))
			{
	  			Erp_State = ERP_COUNTING;
				by_times1s = 10;
			}
    	}

	if(by_times1s >= 10)
	{
		by_times1s = 0;
		ERP_1S_FLAG = 1;
	}
	else
	{
		by_times1s++;
	}
    if(++ERP_TimeOut_Counter>=ERP_TIMEOUT_COUNTER)
      	ERP_TimeOut_Counter = ERP_TIMEOUT_COUNTER; 
}

/**************************************************************************//**
* @brief
*   中断函数，中断打开后10ms执行一次。
*
* @note
*   此函数功能为每10ms标志位置1，用于计时。此中断为LETIMER中断。
*   
*****************************************************************************/
void ERP_LETIMER_10ms_Int(void)
{
	if(w_Timer < 0xFFFF)
		w_Timer++;
}

/**************************************************************************//**
* @brief
*   ERP阶段判断。
*
* @note
*	 此函数功能为判断是否满足进入ERP的条件，输出进入ERP时的倒计时数字，并进行进入ERP的一些处理。
*
* @param[in]
*   输入为工作状态和具体按键.
*
* @return
*   返回ERP_EVENT。
*****************************************************************************/
VR_KEY UTL_ERP_Set_VK(VR_KEY vk)
{
	VR_KEY ret = vk;
	
	if(vk != VK_NULL )
	{
		if(Erp_State>ERP_IDLE )
		{
			ret = VK_PROCESSED;
			Erp_State = ERP_END;
		}
		else
		{
			w_Time_ERP = 0;
		}

	}
	return ret;
}

ERP_EVENT ERP_Process(ReturnEventHandler flag)
{ 
	ERP_EVENT Erp_Event = ERP_NO_EVENT;

	if(flag< RET_PROCESS_DISPLAY) 
		return Erp_Event;
	if(Erp_State > ERP_IDLE)
	{
		Erp_Event = ERP_PROCESSED;
	}
	
	switch(Erp_State)
	{	
		case ERP_END:
		{
			ERP_Initial_Data( );
			Erp_Event = ERP_QUIT_DEMO;
		}
		break;
			
		case ERP_DEMO:
		{
			if(ERP_Refresh_Counter >10)
			{
				ERP_Refresh_Counter = 0;
				Erp_Event = REFRESH_DEMO;
			}
		}
        break;
		case ERP_COUNTING:
		{
            if(!ERP_1S_FLAG) break;
            ERP_1S_FLAG = 0;
            if(by_number>0 )
            {
                by_number--;
                Erp_Event = (ERP_EVENT)by_number;
            }
            else
            {
                Erp_State = ERP_MODE;
                Erp_Event = Enter_ERP;
            }
		}
        break;
		case ERP_MODE:
		{
            ERP_Mode();
		}
		break;
		default :
		break;
	}
	return Erp_Event;
}

/**************************************************************************//**
* @brief
*   快速进入ERP。
*
* @note
*	 此函数功能为将倒计时时间计满，从而快速进行以后的动作。
*
*****************************************************************************/
void ERP_Quick_In(void)
{
	Erp_State = ERP_IDLE ;
	w_Time_ERP = SystemConfig_Get_Int(ERP_WAIT_MIN_UINT16)*10;
	ERP_QuickIn_FLAG = 1;
	by_number = 6 ;
}

/**************************************************************************//**
* @brief
*   进入ERP后的一些操作。
*
* @note
*   此功能为进入ERP后的开ERP灯，关外设电源及进入超低功耗的一些操作及ERP唤醒后的关
&	 ERP灯，开电源，系统复位等操作。
*
*****************************************************************************/


void ERP_Mode(void)
{
    UINT8 result = 0;
    ERP_TimeOut_Counter = 0;

	LCB_Erp();
    do
    {
#ifdef WDT_ENABLE
        HAL_Clear_WatchDog();
#endif        
        result = LCB_Process();
		if(ERP_TimeOut_Counter >= ERP_TIMEOUT_COUNTER)
			result = 1;
    }while(result == 0); 
    ERP_TimeOut_Counter = 0;
    
	if(result == 1)
	{
		HAL_ERP_Low_POWER_Config(KEYMODE_I2C,1);
	    HAL_BOR_Enable_ExtPower(Power_Channel_ERP,1);
        HAL_BOR_Enable_ExtPower(Power_Channel_A,0);
		while(1)
		{
			HAL_ERP_Enter_Standby_Mode();
			w_Timer = 0;
			while(!HAL_DLCB_Get_Rx())   
			{
				if(w_Timer >= 10)
				{
					BOR_Set_Passcode(0);
					NVIC_SystemReset();                                             
				}
			}
			
			if(HAL_ERP_Get_Reset_Flag() == 0x55AA)
			{
				BOR_Set_Passcode(0);
				NVIC_SystemReset(); 
			}
			
		}	
	}
	else if(result == 2)
	{
		BOR_Set_Passcode(0);
		NVIC_SystemReset(); 
	}
}

/**************************************************************************//**
* @brief
*   注册中断，方便main统一调用
*
* @note
*   每个模块初注册中断函数均与模块名对应，以便main统一初始化，不会遗漏
*****************************************************************************/

void Erp_Regist_Functions(void)
{
	HAL_SystemTicker_Alloc_ISR(ERP_1ms_Int);
	HAL_LETimer_Alloc_ISR(ERP_LETIMER_10ms_Int);
}
