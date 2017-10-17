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
	//��ERP�Ƶĳ�ʼ����ʱ�ӵĳ�ʼ��
	
}
/**************************************************************************//**
* @brief
*   ��ʼ�����ݣ�ע���жϡ�
*
* @note
*   �˺�������Ϊ��ʼ��ERP��ʹ�õ����ݣ��Լ�ע������Ҫ���жϡ����ж�Ϊ1ms�жϡ�
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
*   �жϺ������жϴ򿪺�1msִ��һ�Ρ�
*
* @note
*   �˺�������Ϊÿ100ms��־λ��1�����ڼ�ʱ�����ж�Ϊ1ms�жϡ�
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
*   �жϺ������жϴ򿪺�10msִ��һ�Ρ�
*
* @note
*   �˺�������Ϊÿ10ms��־λ��1�����ڼ�ʱ�����ж�ΪLETIMER�жϡ�
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
	//TODO ���滹�ǲ�����
}
/**************************************************************************//**
* @brief
*   ERP�׶��жϡ�
*
* @note
*	 �˺�������Ϊ�ж��Ƿ��������ERP���������������ERPʱ�ĵ���ʱ���֣������н���ERP��һЩ����
*
* @param[in]
*   ����Ϊ����״̬�;��尴��.
*
* @return
*   ����ERP_EVENT��
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
*   ���ٽ���ERP��
*
* @note
*	 �˺�������Ϊ������ʱʱ��������Ӷ����ٽ����Ժ�Ķ�����
*
*****************************************************************************/
void ERP_Quick_In(void)
{
	w_Time_ERP = 900*10;
}

/**************************************************************************//**
* @brief
*   ����ERP���һЩ������
*
* @note
*   �˹���Ϊ����ERP��Ŀ�ERP�ƣ��������Դ�����볬�͹��ĵ�һЩ������ERP���Ѻ�Ĺ�
&	 ERP�ƣ�����Դ��ϵͳ��λ�Ȳ�����
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
*   ע���жϣ�����mainͳһ����
*
* @note
*   ÿ��ģ���ע���жϺ�������ģ������Ӧ���Ա�mainͳһ��ʼ����������©
*****************************************************************************/

void Erp_Regist_Functions(void)
{
	Hal_Regist_ISR(MS_ISR,ERP_ISR_IDX,ERP_1ms_Int);
	HAL_ERP_Regist_LETIMER_ISR(ERP_TIMER_COUNT,ERP_LETIMER_10ms_Int);
}
