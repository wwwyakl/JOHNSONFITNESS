#include "UtilitiesErp.h"


#define ERP_TIMEOUT_COUNTER 500
__no_init static int by_Times1s;
__no_init static char by_number;
__no_init unsigned int w_Time_ERP;
__no_init static bool ERP_1S_FLAG;
__no_init static bool ERP_100MS_FLAG;
__no_init static UINT16 w_Timer;
__no_init static bool ERP_TimeOut_Flag;
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
	ERP_STATE_TOTAL
}
ERP_STATE;

__no_init ERP_STATE Erp_State ;

void ERP_Initial_IO(void)
{
	//todo 
	//如ERP灯的初始化，时钟的初始化
	
}
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
    ERP_TimeOut_Flag = 0 ;
    ERP_TimeOut_Counter = 0;
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
	if(by_Times100ms >= 100)
	{
		by_Times100ms = 0;
		ERP_100MS_FLAG = 1;
	}
	else
	{
		by_Times100ms++;
	}
	if(by_Times1s >= 1000)
	{
		by_Times1s = 0;
		ERP_1S_FLAG = 1;
	}
	else
	{
		by_Times1s++;
	}
    if(ERP_TimeOut_Flag)
    {
        if(++ERP_TimeOut_Counter>=ERP_TIMEOUT_COUNTER)
          ERP_TimeOut_Counter = ERP_TIMEOUT_COUNTER; 
    }
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


void ERP_Clear_time(void)
{
	w_Time_ERP = 0;
    Erp_State = ERP_IDLE;
}


void ERP_Set_time(unsigned int ERP_TIME)
{
	if(ERP_TIME >= SystemConfig_Get_Int(ERP_WAIT_MIN_UINT16)*10)
	{
		ERP_TIME = SystemConfig_Get_Int(ERP_WAIT_MIN_UINT16)*10;
	}
	w_Time_ERP = ERP_TIME;
	//TODO 保存还是不保存
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
ERP_EVENT ERP_Process(UINT16 vk)
{
  FRAME_STAGE stage = (FRAME_STAGE)(vk&0xFF00);
  UINT8 realKey = vk&0x00ff;
  
  ERP_EVENT Erp_Event = ERP_NO_EVENT;
  
  if(stage == FRAME_STAGE_ENG && w_Time_ERP>SystemConfig_Get_Int(DEMO_WAIT_MIN_UINT16)*10)
  {
    stage = FRAME_STAGE_USER;
  }
  if(stage == FRAME_STAGE_PRGRUN || stage == FRAME_STAGE_AGINGTEST ||stage == FRAME_STAGE_ENG ||\
    stage == FRAME_STAGE_MODEL)
  {    
    w_Time_ERP = 0;
    Erp_State = ERP_IDLE;
    Erp_Event = ERP_NO_EVENT;
    return(Erp_Event);
  }
  
  switch(Erp_State)
  {	
  case ERP_IDLE:
    {
      if(realKey!=VK_NULL)
      {
        w_Time_ERP = 0;
      }
      if(!ERP_100MS_FLAG) break;
      ERP_100MS_FLAG = 0;
      if((w_Time_ERP >= 900*10) &&\
        (SystemConfig_Get_Char(ENERGY_SAVER_FLAG_UCHAR)))// && !USBH_DeviceConnected()) 
      {
        ERP_1S_FLAG = 0;
        by_Times1s = 0;
        Erp_State = ERP_COUNTING;
        Erp_Event = (ERP_EVENT)by_number;  
      }
      else if(w_Time_ERP >= SystemConfig_Get_Int(DEMO_WAIT_MIN_UINT16)*10 )
      {
        Erp_State = ERP_DEMO;
        Erp_Event = ENTER_DEMO; 
      }
      else
      {
        w_Time_ERP++;
        Erp_Event = ERP_NO_EVENT;
      }  
    }
    break;
  case ERP_DEMO:
    {
      if(realKey!=VK_NULL)
      {
        w_Time_ERP = 0;
        ERP_100MS_FLAG = 0 ;
        Erp_State = ERP_IDLE ;
        Erp_Event = ERP_QUIT_DEMO;
      }
      if(!ERP_100MS_FLAG) break;
      ERP_100MS_FLAG = 0;
      
      if((w_Time_ERP >= 900*10) &&\
        (SystemConfig_Get_Char(ENERGY_SAVER_FLAG_UCHAR)))// && !USBH_DeviceConnected()) 
      {
        ERP_1S_FLAG = 0;
        by_Times1s = 0;
        Erp_State = ERP_COUNTING;
        Erp_Event = (ERP_EVENT)by_number;  
      }
      else
      {
        w_Time_ERP++;
        Erp_Event = ERP_NO_EVENT;
      }
    }
    break;
  case ERP_COUNTING:
    {
      if(realKey!=VK_NULL)
      {
        ERP_1S_FLAG = 0 ;
        ERP_Initial_Data();
        Erp_Event = ERP_QUIT_ERPCOUNT;
      }
      if(!ERP_1S_FLAG) break;
      ERP_1S_FLAG = 0;
      
      by_Times1s = 0;
      ERP_1S_FLAG = 0;
      if(by_number != 1 )
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
      w_Time_ERP = 0;
      by_Times1s = 0;
      ERP_1S_FLAG = 0;
      Erp_State = ERP_IDLE;
      Fan_Initial_Data();    //ZSF
      Display_Updata_Data(true);    //ZSF
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
	w_Time_ERP = 900*10;
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
    ERP_TimeOut_Flag = 1;

	LCB_Erp();
    do
    {
#ifdef WDT_ENABLE
        HAL_Clear_WatchDog();
#endif        
        result = LCB_Process();
        if(ERP_TimeOut_Counter >= ERP_TIMEOUT_COUNTER)
        {
            result = 1;
        }
    }
    while(result == 0);
    ERP_TimeOut_Flag = 0;
    ERP_TimeOut_Counter = 0;
    
	if(result == 1)
	{
          HAL_ERP_Low_POWER_Config(KEYMODE_I2C,1);
//          HAL_BOR_Enable_ExtPower(Power_Channel_ERP,1);
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
            CodingsWitch_1ms_Int();
            if(KB_Get_Switch_Value() != 0xAA)
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
	Hal_Regist_ISR(MS_ISR,ERP_ISR_IDX,ERP_1ms_Int);
	HAL_ERP_Regist_LETIMER_ISR(ERP_TIMER_COUNT,ERP_LETIMER_10ms_Int);
}
