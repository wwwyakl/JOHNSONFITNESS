#include "UtilitiesErrorcode.h"
#include "LCB.h"
#include "programs.h"

#define SAFETYKEY_DELAY_TIMEOUT 300

bool Check_20ms_Safe_Flag1;
bool Check_20ms_Safe_Flag2;
__no_init ERR_EVENT Err_event;
__no_init UINT8 SafetyKey_DelayStatus;
__no_init UINT16 SafetyKey_DelayCnt;

void Err_1ms_Int(void);
bool Speed_IO_Check_Safe_AlwaysOff(void);
bool Speed_IO_Check_Safe_AlwaysOn(void);
extern void  Bluetooth_Set_VirtualKey(UINT16 vk);

typedef struct
{
	UINT8  Code1;
	UINT16 Code2;
}TM_ERROR_CODE;

typedef struct
{
	UINT8  Code1;
	UINT16 Code2;
}EP_ERROR_CODE;

static const TM_ERROR_CODE TM_Table[12] = 
{
	{SAFEKEY,			0x02B2},
	{NO_INCODE,			0x00A1},
	{MOS_FAULT,			0x02A8},
	{SPEED_FAST,		0x0042},
	{SPEED_LOW,			0x0041},	
	{OVER_CURRENT,		0x0144},
	{INC_NOCOUNT,		0x0140},
	{MCB_TEMP_OVER,		0x024C},
	{DIGTIAL_TIMEOUT,	0x0440},
	{DIGTIAL_RX_ERROR,	0x0443},
	{FALSH_WRITE,		0x02B9},	
	{FALSH_READ,		0x02BA},
	
}; 

static const EP_ERROR_CODE EP_Table[13] = 
{
	{CURRENT_OVER,		0x0149},
	{TMEP_HIGH,			0x0242},
	{VOLTAGE_LOW,	 	0x0240},
	{INCLINE_NO_COUNT,	0x0140},
	{ECB_NO_COUNT,	 	0x0244},
	{ECB_ZERO_OEPN,		0x0245},
	{CONNECT_FAIL,		0x0440},
	{NO_RPM,			0x00A1},
	{RX_TX_ERROR,		0x0443},
	{RESISTANCE_OPEN,	0x01AF},
	{NVFLASH_R,	 		0x02ba},
	{NVFLASH_W,			0x02b9},
	{TEMP_ERR,			0x024c},
};

typedef enum
{
    UTL_IDLE = 0,   
	Safekey_ON,
    Safekey_OFF,
	UTL_NO_INCODE,
	UTL_MOS_FAULT,
	UTL_SPEED_FAST,
	UTL_SPEED_LOW,
	UTL_OVER_CURRENT,
	UTL_INC_NOCOUNT,
	UTL_MCB_TEMP_OVER,
	UTL_DIGTIAL_TIMEOUT,
	UTL_DIGTIAL_RX_ERROR,
	UTL_FALSH_WRITE,
	UTL_FALSH_READ,
	UTL_STATE_TOTAL
}ERR_STATE;

__no_init ERR_STATE Err_state;

UINT8 UTL_GetErrState(void)
{
  return(UINT8) Err_state ;
}
/**************************************************************************//**
* @brief
*   ��ʼ���˿ڣ�����mainͳһ����
*
* @note
*   ÿ��ģ���ʼ��I/O��������ģ������Ӧ���Ա�mainͳһ��ʼ����������©
*****************************************************************************/

void Err_Initial_IO(void)
{	
	HAL_SafetyKey_Initial_IO();	
}


/**************************************************************************//**
* @brief
*   ��ʼ������
*
* @note
*   �˺�������Ϊ��ʼ������
*****************************************************************************/

void Err_Initial_Data(void)
{
	Err_state = UTL_IDLE;	
	Err_event = S_NO_EVENT;
	Check_20ms_Safe_Flag1 = 0;
	Check_20ms_Safe_Flag2 = 0;	
    SafetyKey_DelayStatus = 0 ;
    SafetyKey_DelayCnt = 0;
}

/**************************************************************************//**
* @brief
*   �жϺ������жϴ򿪺�1msִ��һ�Ρ�
*
* @note
*   �˺����������ڼ�ʱ�����ж�Ϊ1ms�жϡ�
*   
*****************************************************************************/
void Err_1ms_Int(void)
{
	static unsigned char by_20ms = 0;
	if(by_20ms >= 10)
	{
		by_20ms = 0;		
		Check_20ms_Safe_Flag1 = 1;
		Check_20ms_Safe_Flag2 = 1;
	}
	else 
	{
		by_20ms++;
	}
    if( SafetyKey_DelayStatus )
    {
        if(++SafetyKey_DelayCnt>=SAFETYKEY_DELAY_TIMEOUT)
            SafetyKey_DelayCnt = SAFETYKEY_DELAY_TIMEOUT+1;
    }
}

/**************************************************************************//**
* @brief
*   ״̬ά��������
*
* @note
*   �˺�������Ϊ�жϷ�����ʲô�¼����ı�����״̬
*    �����ѷ������¼����ء�
*   
*****************************************************************************/

ERR_EVENT ERR_Process(UINT16 vk)
{
  FRAME_STAGE stage = (FRAME_STAGE)(vk&0xFF00);
  //    UINT8 realKey = (UINT8)(vk&0x00ff);
  UINT16 w_ErrorCode = 0;
  ERR_EVENT Err_event_l = S_NO_EVENT;
  LCB_Process();
  
  if(stage == FRAME_STAGE_MODEL || stage == FRAME_STAGE_AGINGTEST )
  {    
    return(Err_event_l);
  }
  
  UINT16 Error = LCB_Dectect_Error();
  
  if(Error != 0)
  {
    if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
    {
      for(UCHAR i = 0 ;i < 12 ; i++)
      {
        if(TM_Table[i].Code2 == Error)
          w_ErrorCode = TM_Table[i].Code1;
      }
    }
    else if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==ELLIPTICAL)
    {
      for(UCHAR i = 0 ;i < 13 ; i++)
      {
        if(EP_Table[i].Code2 == Error)
          w_ErrorCode = EP_Table[i].Code1;
      }		
    }
  }
  
  switch(Err_state)
  {
  case UTL_IDLE:
    {//�¼��ǰ�ȫ�����Ƴ���������ԭ�ȿ��ش��ڣ���I/O���Ϊ�ε����¿ش�����
      if((Speed_IO_Check_Safe_AlwaysOff() || w_ErrorCode == SAFEKEY))
      {
        /*
        if(w_ErrorCode == SAFEKEY )
        {
        if( !SafetyKey_DelayStatus )
        {
        SafetyKey_DelayStatus = 1 ;
        SafetyKey_DelayCnt = 0 ;
      }
                    else if(SafetyKey_DelayCnt >= SAFETYKEY_DELAY_TIMEOUT )
        {
        Err_state = Safekey_OFF;
        Err_event = Safekey_on_to_off;
        Bluetooth_Set_VirtualKey(VK_SAFEKEYOFF);
        //                        Program_Set_VirtualKey(VK_SAFEKEYOFF);
      }   
      }
                else
        {
        Err_state = Safekey_OFF;
        Err_event = Safekey_on_to_off;
        Bluetooth_Set_VirtualKey(VK_SAFEKEYOFF);
        Bluetooth_Set_VirtualKey(VK_RESET);
        //                    Program_Set_VirtualKey(VK_SAFEKEYOFF);
      }*/
//#ifdef TM
        Err_state = Safekey_OFF;
//#else
       // Err_state = UTL_IDLE;
//#endif
        Err_event_l = Safekey_on_to_off;
        Bluetooth_Set_VirtualKey(VK_SAFEKEYOFF);
        Bluetooth_Set_VirtualKey(VK_RESET);
        UTL_Set_Virtual_Key(VK_SAFEKEYOFF);    //zsf
        if(Program_Get_Status() == PS_WAIT_3S)
          Program_Set_VirtualKey(VK_SAFEKEYOFF);
        return Err_event_l;
      }
      
      switch(w_ErrorCode)
      {
      case NO_INCODE:
        {
          Err_state = UTL_IDLE;
          Err_event_l = S_NO_INCODE;
          break;
        }
        
      case MOS_FAULT:
        {
          Err_state = UTL_IDLE;
          Err_event_l = S_MOS_FAULT;
          break;
        }
        
      case SPEED_FAST:
        {
          Err_state = UTL_IDLE;
          Err_event_l = S_SPEED_FAST;
          break;
        }
        
      case SPEED_LOW:
        {
          Err_state = UTL_IDLE;
          Err_event_l = S_SPEED_LOW;
          break;
        }
        
      case OVER_CURRENT:
        {
          Err_state = UTL_IDLE;
          Err_event_l = S_OVER_CURRENT;
          break;
        }
        
      case INC_NOCOUNT:
        {
          Err_state = UTL_IDLE;
          Err_event_l = S_INC_NOCOUNT;
          break;
        }
        
      case MCB_TEMP_OVER:
        {
          Err_state = UTL_IDLE;
          Err_event_l = S_MCB_TEMP_OVER;
          break;
        }
        
      case DIGTIAL_TIMEOUT:
        {
          Err_state = UTL_IDLE;
          Err_event_l = S_DIGTIAL_TIMEOUT;
          break;
        }	
        
      case DIGTIAL_RX_ERROR:
        {
          Err_state = UTL_IDLE;
          Err_event_l = S_DIGTIAL_RX_ERROR;
          break;
        }
        
      case FALSH_WRITE:
        {
          Err_state = UTL_IDLE;
          Err_event_l = S_FALSH_WRITE;
          break;
        }
        
      case FALSH_READ:
        {
          Err_state = UTL_IDLE;
          Err_event_l = S_FALSH_READ;
          break;
        }
        
      default:
        break;
      }
      break;
    }	
    
  case Safekey_OFF:
    {//�¼��ǰ�ȫ���ز��ϣ�������ԭ״̬ΪSafekey_OFF��I/O���Ϊ�������¿��޿��ش���
      if(Speed_IO_Check_Safe_AlwaysOn())
      {
        LCB_Send_Cmd(DS_INITIAL,0);
        LCB_Send_Cmd(DS_WORK_STATUS,0);
        LCB_Send_Cmd(DS_WORK_STATUS,0x01);
        LCB_Send_Cmd(DS_MOTOR_HP,0x5678);
        if(w_ErrorCode != SAFEKEY)
        {
          SafetyKey_DelayCnt = 0;
          SafetyKey_DelayStatus = 0;
          Err_state = UTL_IDLE;
          Err_event_l = Safekey_off_to_on;
          Bluetooth_Set_VirtualKey(VK_SAFEKEYON);
          Program_Set_VirtualKey(VK_SAFEKEYON);
        }
      }
      break;
    }
    
  default:
    break;
  }		
  return Err_event_l;
}

/**************************************************************************
* @brief
*   ��ⰲȫ����400ms���Ƿ�һֱ���ڰε�״̬��
*   
* @return
*   ����1���γ���0������һֱ�γ���
*****************************************************************************/
bool Speed_IO_Check_Safe_AlwaysOff(void)
{
	static unsigned char Safe_Key_400ms_Counter = 0 ;
	bool Temp = 0;
	if(Check_20ms_Safe_Flag1)
	{
		Check_20ms_Safe_Flag1 = 0;
		if(HAL_SafetyKey_Get_Attached())
		{
			if(++Safe_Key_400ms_Counter >= 5)
			{
				Safe_Key_400ms_Counter = 0;
				Temp = 1 ;
			}
		}
		else
		{
			Safe_Key_400ms_Counter = 0;
			Temp = 0 ;
		}
	}
	return (Temp) ;
}

/**************************************************************************//**
* @brief
*   ��ⰲȫ����100ms���Ƿ�һֱ���ڴ���״̬��
*   
* @return
*   ����1��һֱ���ڣ�0������һֱ���ڡ���
*****************************************************************************/
bool Speed_IO_Check_Safe_AlwaysOn(void)
{
	static unsigned char Safe_Key_100ms_Counter = 0 ;
	bool Temp = 0;
	if(Check_20ms_Safe_Flag2)
	{
		Check_20ms_Safe_Flag2 = 0;
		if(!HAL_SafetyKey_Get_Attached())
		{
			if(++Safe_Key_100ms_Counter >= 50)
			{
				Safe_Key_100ms_Counter = 0;
				Temp = 1 ;
			}
		}
		else
		{
			Safe_Key_100ms_Counter = 0;
			Temp = 0 ;
		}
	}
	return (Temp) ;
}

/**************************************************************************//**
* @brief
*   ע���жϣ�����mainͳһ����
*
* @note
*   ÿ��ģ���ע���жϺ�������ģ������Ӧ���Ա�mainͳһ��ʼ����������©
*****************************************************************************/

void Err_Regist_Functions(void)
{
	Hal_Regist_ISR(MS_ISR,Err_ISR_IDX,Err_1ms_Int);
}




