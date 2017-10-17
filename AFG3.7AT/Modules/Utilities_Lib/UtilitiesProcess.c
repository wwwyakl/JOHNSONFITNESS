#include "UtilitiesProcess.h"
#include "UtilitiesAMP.h"

__no_init static UTL_EVENT Utl_Event;
__no_init  FRAME_STAGE g_CurrentStage;
__no_init UINT16 g_vk_Temp;

#define Major     		    "1"//"1"
#define Minor     		    "0"//"013"
#define Revision		    "001"      //boot	  Information.h


const char* utl_version[3]=
{
    Major,
	Minor,
	Revision
};


void UTL_Set_Virtual_Key(UINT16 vk)
{
//    FRAME_STAGE stage = (FRAME_STAGE)(vk&0xFF00);
    UINT8 realKey = vk&0x00ff;
    
    g_vk_Temp = vk;
    if( realKey == VK_DEMO_ERP_WAKEUP)
    {
        ERP_Initial_Data();
    }
    if(realKey == VK_SAFEKEYOFF)
    {
        ERP_Initial_Data();
        LCB_Initial();
    }
    
}
/**************************************************************************//**
 * @brief
 *   ��������UTL���̵ĺ���
 *
 * @note
 *   �˺������ϸ�UTLģ��Ľ��̣�����ֵΪ��ģ�鷢�����¼�
 *
 * @return
 *   ����ֵΪ��UTLģ����¼�.
 *****************************************************************************/

UTL_EVENT UTL_Process(void)
{
  static UINT8 _btHrflag = 0 ;
  
  Utl_Event.erp_event = ERP_Process(g_vk_Temp);
  Utl_Event.err_event = ERR_Process(g_vk_Temp);
  Utl_Event.burn_event = BurnTest_Process(g_vk_Temp);
  Utl_Event.keypad_event = KEYPAD_Get_5S();
  
  if(Bluetooth_Get_HeartData()>10)
  {
    extern UINT8 BTHRTimerChannel ;
    if(Timer_Counter(BTHRTimerChannel+0x80,5))
    {
      if(_btHrflag == 0) 
      {
        if((Program_Get_Status()>=PS_PAUSED) && (Program_Get_Status()<=PS_COOLDOWN) )
          _btHrflag = 1;
      }
      else
        _btHrflag = 0;
    }
  }
  else
  {
    if((Program_Get_Status()>=PS_PAUSED) && (Program_Get_Status()<=PS_COOLDOWN) )
      _btHrflag = Midware_Heart_Get_Blink();
    else
      _btHrflag = 0;
  }
  Utl_Event.heart_event = _btHrflag;
  AMP_PROCESS();
  return Utl_Event;
}

/**************************************************************************//**
 * @brief
 *   ����ģ��汾�ź���
 *
 * @note
 *   �˺�������ֵ��һ����ַ�����˵�ַ����һ��������������Ԫ�ؼ�Ϊ�汾��
 *
 * @return
 *   ����ֵΪ��ַ.
 *****************************************************************************/

char* UTL_Get_Version(void)
{
	static char retVersion[8]={0};
    UINT8 strLength;
    strcpy(retVersion, utl_version[0] );
    strLength = strlen(utl_version[0]);
    strcpy((retVersion+strLength),utl_version[1]);
    strLength += strlen(utl_version[1]);
    strcpy((retVersion+strLength),utl_version[2]);
    return retVersion;	
}