#include "UtilitiesErrorcode.h"
#include "LCB.h"

#define SAFETYKEY_DELAY_TIMEOUT 300

bool Check_20ms_Safe_Flag1;
bool Check_20ms_Safe_Flag2;
__no_init bool ClassLevel;
//bool Check_Statu=0;
__no_init ERR_EVENT Err_event;
__no_init UINT8 SafetyKey_DelayStatus;
__no_init UINT16 SafetyKey_DelayCnt;
__no_init ERR_EVENT Err_event_backup;
__no_init UINT16 ErrorSkipTime;
void Err_1ms_Int();
bool Speed_IO_Check_Safe_AlwaysOff(void);
bool Speed_IO_Check_Safe_AlwaysOn(void);
//UINT16 error_code_debug=0;
UINT16 g_ErrorCode = 0;
UINT16 g_LastErrorCode = 0;
UINT8 g_ErrorCode_Count = 0;
bool g_ErrorCode_Event = 0;
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
typedef struct
{
	UINT8  Code1;
	UINT16 Code2;
}C50_ERROR_CODE;
static const TM_ERROR_CODE TM_Table[13] = 
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
    {OVER_SPEED,		0x02A1},
	
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

static const C50_ERROR_CODE C50_Table[16] = 
{
	{MANUAL_SWITCH,		        0x02C0},//CLASS C
	{DC_OVER_CURRENT,			0x02BF},//CLASS C
	{CONTROL_FREQUENCY,	 	    0x02C4},//CLASS C
	{CONTROL_DISCONNECTION,	    0x02C6},//CLASS C
	{IR_FREQUENCY,	 	        0x02C5},//CLASS C
	{IR_DISCONNECTION,		    0x02C7},//CLASS C
	{ELECTR_OVER_CURRENT,		0x01AC},//CLASS C
	{ELECTR_DISCONNECTION,		0x01AF},//CLASS C
	{VOER_SPEED,	 		    0x02C1},//CLASS C
	{SPEED_SENSOR,			    0x02A0},//CLASS C
    {COMMUNICATION,			    0x04A0},//CLASS C
    {MACHINE_TYPE_ERROR,	    0x02AB},//CLASS C
    {C50_RESISTANCE_TYPE,	    0x02B4},//CLASS C
	{POSITION_SENCOR,			0x044B},//CLASS B
    {COMMAND_ERROR,			    0x0441},//CLASS B
	{FUN_ERR,		            0x0249},//CLASS B
};
typedef enum
{
    ERROR_NONE,
    ERROR_NO_SPM,
    ERROR_IRSENSOR,
    ERROR_CONTROLZONE,
    ERR0R_UCB_EMERG,
    ERROR_HANDRAIL_EMERG,
    ERROR_NONE_EVENT,
    ERROR_END
}Errorcode;
typedef enum
{
    UTL_IDLE = 0,   
	Safekey_ON,
    Safekey_OFF,
    UTL_ERROR_NO_SPM,
    UTL_ERROR_IRSENSOR,
    UTL_ERROR_CONTROLZONE,
    UTL_ERR0R_UCB_EMERG,
    UTL_ERROR_HANDRAIL_EMERG,
    UTL_ERROR_NONE,
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
__no_init static char g_ClearBootPassword[LENGTH_OF_PASSWORD];
__no_init static UINT8 g_ClearBootIndex;
__no_init UINT16 ClearBootCnt  ;
/**************************************************************************//**
* @brief
*   初始化端口，方便main统一调用
*
* @note
*   每个模块初始化I/O函数均与模块名对应，以便main统一初始化，不会遗漏
*****************************************************************************/

void Err_Initial_IO(void)
{	
	HAL_SafetyKey_Initial_IO();
	HAL_SystemTicker_Alloc_ISR(Err_1ms_Int);
}

void ErrClearENGPassword(void)
{
	ClearBootCnt = 0 ;
	g_ClearBootIndex = 0 ;
	memset(g_ClearBootPassword,'\0',LENGTH_OF_PASSWORD);
}
/**************************************************************************//**
* @brief
*   初始化数据
*
* @note
*   此函数功能为初始化数据
*****************************************************************************/

void Err_Initial_Data(void)
{
	Err_state = UTL_IDLE;	
	Err_event = S_NO_EVENT;
    ClassLevel = 1;
	Check_20ms_Safe_Flag1 = 0;
	Check_20ms_Safe_Flag2 = 0;	
    SafetyKey_DelayStatus = 0 ;
    SafetyKey_DelayCnt = 0;
    Err_event_backup = Err_event;
	ErrClearENGPassword();
}

/**************************************************************************//**
* @brief
*   中断函数，中断打开后1ms执行一次。
*
* @note
*   此函数功能用于计时。此中断为1ms中断。
*   
*****************************************************************************/
void Err_1ms_Int()
{
	static unsigned char by_20ms = 0;
    ErrorSkipTime++;
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
	if(Err_state == Safekey_OFF)
	{
		if(++ClearBootCnt>=USER2ENG_SPACETIME)
		{
			ClearBootCnt = 0 ;
			ErrClearENGPassword();
		}
	}
}

/**************************************************************************//**
* @brief
*   状态维护函数。
*
* @note
*   此函数功能为判断发生了什么事件，改变自身状态
*    ，并把发生的事件返回。
*   
*****************************************************************************/
VR_KEY UTL_ERR_Set_VK(VR_KEY vk)
{
	VR_KEY ret = vk;
	if(ret== VK_PROCESSED ||ret == VK_NULL || ret== VK_CLEAR_BOOT ) return ret ;
	if((Err_state == Safekey_OFF || g_ErrorCode == ERR0R_UCB_EMERG || g_ErrorCode ==ERROR_HANDRAIL_EMERG) )//此处是在提示错误时可以按2009重新选机种
	{
		switch(vk)
		{
			case VK_ENTER:
			case VK_ENTER_CHANGEDIS:
			{
				if( strcmp(g_ClearBootPassword, CLEARBOOT_PASSWORD) ==0)
                    ret = VK_CLEAR_BOOT;	
				else
                    ret = VK_PROCESSED;
				ErrClearENGPassword();
		    }
			break;
			case VK_NUM0:
		    case VK_NUM1:
		    case VK_NUM2:
		    case VK_NUM3:
		    case VK_NUM4:
		    case VK_NUM5:
		    case VK_NUM6:
		    case VK_NUM7:
		    case VK_NUM8:
		    case VK_NUM9:
			{
				ClearBootCnt = 0 ;
				if( g_ClearBootIndex< (LENGTH_OF_PASSWORD-1) )
					g_ClearBootPassword[g_ClearBootIndex++] = (vk-VK_NUM0)+'0';
				else
					ErrClearENGPassword();
				ret = VK_PROCESSED;
			}
			break;
			default:
			{
				ErrClearENGPassword();
				ret = VK_PROCESSED;
			}
		}
	}
	return ret;
}
ERR_EVENT ERR_Process(ReturnEventHandler flag)
{
    UINT16 w_ErrorCode=0;
	ERR_EVENT ret = S_NO_EVENT;
//    UINT16 _tempErrorCode = 0;
	UINT16 Error = 0;
	//new2 c50暂与其他的机种区分开来，判断错误的方式不同。
	if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==CLIMB_MILL)
	{
        Error = LCB_Get_Data(G_READ_STATUS);
//        error_code_debug = Error ;
        if(0x0300&Error)
        {
            if(g_ErrorCode != ERR0R_UCB_EMERG)
            {
                g_ErrorCode = ERR0R_UCB_EMERG;
                Err_state = UTL_ERR0R_UCB_EMERG;
                g_ErrorCode_Count = 5;
                g_ErrorCode_Event = 1;
            }
        }
        else if(0x0c00&Error)
        {
            if(g_ErrorCode != ERROR_HANDRAIL_EMERG)
            {
                g_ErrorCode = ERROR_HANDRAIL_EMERG;
                Err_state = UTL_ERROR_HANDRAIL_EMERG;
                g_ErrorCode_Count = 5;
                g_ErrorCode_Event = 1;
            }
        }
        else if(0x00c0&Error)
        {
            if(g_ErrorCode != ERROR_CONTROLZONE)
            {
                if(g_ErrorCode_Count == 5)
                {
                    g_ErrorCode_Count = 0;   
                    Err_state = UTL_ERROR_NONE;
                }
                else
                {
                    g_ErrorCode = ERROR_CONTROLZONE;
                    Err_state = UTL_ERROR_CONTROLZONE;
                    //                    g_ErrorCode_Count ++;
                    g_ErrorCode_Event = 1;						
                }
            }
        }
        else if(0x0018&Error)
        {
            if(g_ErrorCode != ERROR_IRSENSOR)
            {
                g_ErrorCode = ERROR_IRSENSOR;
                Err_state = UTL_ERROR_IRSENSOR;
                //                    g_ErrorCode_Count ++;
                g_ErrorCode_Event = 1;		        	
            }
        }
        else if(0x1000&Error)
        {
            if(g_ErrorCode != ERROR_NO_SPM)
            {
                g_ErrorCode = ERROR_NO_SPM;
                Err_state = UTL_ERROR_NO_SPM;
                //                    g_ErrorCode_Count ++;
                g_ErrorCode_Event = 1;		        	
            }
        }
        else 
        {
            if(g_ErrorCode_Event == 1)
            {
                //                g_ErrorCode = ERROR_NONE_EVENT;
                Err_state = UTL_ERROR_NONE;
            }
        }
        
		if(flag == RET_REFRESH_DISPLAY )
		{
			if(Err_event_backup!= S_NO_EVENT)
			{
				flag = RET_PROCESS_DISPLAY;
			}
			return Err_event_backup;
		}
        if(0!=Error)
        {
            LCB_Send_Cmd(DS_SET_STATUS,0xA5);
        }
        switch(Err_state)
		{
			case UTL_IDLE://下控无传感器错误时的错误代码获取
			{
                Error = LCB_Dectect_Error();
//                Error = 0x0441;
		        if(Error != 0)
                {
                    for(UCHAR i = 0 ;i < 16 ; i++)
                    {
                        if(C50_Table[i].Code2 == Error)
                        {
                            w_ErrorCode = C50_Table[i].Code2;
                            
                            if(i>12)
                            {
                                ClassLevel = 0;//class a&b
                            }
                            else
                                ClassLevel = 1;//class c
                            break;
                        }
                    }
                }
//                if( g_ErrorCode!=0)
                if( g_ErrorCode>6)
                {
                    ret = ERR_PROCESSED;
                    if(ClassLevel == 0)
                    {
                        if(ErrorSkipTime>=5000)
                        {
                            ErrorSkipTime=0;
                            ret = S_ERROR_RELEASED;	
                            g_LastErrorCode = g_ErrorCode;
                            g_ErrorCode = 0;
                            Err_state = UTL_IDLE;
                        }
                    }
                }
                if(w_ErrorCode != 0)//有错误码产生
                {
                    if(ClassLevel == 1)//CLASS C
                    {
                        if((w_ErrorCode != g_ErrorCode)&&(w_ErrorCode != g_LastErrorCode))
                        {               
                            g_ErrorCode = w_ErrorCode;
                            Err_state = UTL_IDLE;
                            ret = S_ERROR_EVENT;	
        //                        Err_state = UTL_STATE_ERROR;
                            LCB_Initial();
                            Program_Set_VirtualKey(VK_SAFETY_KEY_OFF);	
                            RF_Set_VirtualKey(VK_SAFETY_KEY_OFF);	
                            UTL_ERP_Set_VK(VK_WARMUP);
                            LCB_Send_Cmd(DS_SET_STATUS,0x01);
                            LCB_Send_Cmd(DS_SET_SPM,0);
                        }
                    }
                    else    //CLASS A&B
                    {
                        if(SystemConfig_Get_Char(SHOW_ERROR_FLAG_UCHAR)==1)
                        {
                            if((w_ErrorCode != g_ErrorCode)&&(w_ErrorCode != g_LastErrorCode))
                            {    
                                ErrorSkipTime=0;
                                g_ErrorCode = w_ErrorCode;
                                Err_state = UTL_IDLE;
                                ret = S_ERROR_EVENT;	
                                LCB_Initial();
                                Program_Set_VirtualKey(VK_SAFETY_KEY_OFF);	
                                RF_Set_VirtualKey(VK_SAFETY_KEY_OFF);	
                                UTL_ERP_Set_VK(VK_WARMUP);
                                LCB_Send_Cmd(DS_SET_STATUS,0x01);
                                LCB_Send_Cmd(DS_SET_SPM,0);
                            }
                        }
                    }
                }
                
//                if(w_ErrorCode != 0)
//                {
//                    ret = ERR_PROCESSED;
//                    if(w_ErrorCode != g_ErrorCode)
//                    {               
//                        g_ErrorCode = w_ErrorCode;
//                        Err_state = UTL_IDLE;
//                        ret = S_ERROR_EVENT;	
//    //                        Err_state = UTL_STATE_ERROR;
//                        LCB_Initial();
//                    Program_Set_VirtualKey(VK_SAFETY_KEY_OFF);	
//                    RF_Set_VirtualKey(VK_SAFETY_KEY_OFF);	
//                    UTL_ERP_Set_VK(VK_WARMUP);
//                    }
//                }
//                else
//                {
//                    if( g_ErrorCode!=0 && ClassLevel == 0)
//                    {
//                        if(ErrorSkipTime>=30000)
//                        {
//                            ErrorSkipTime=0;
//                            ret = S_ERROR_RELEASED;	
//                            g_ErrorCode = 0;
////                            Err_state = UTL_IDLE;
//                            Err_state = UTL_IDLE;
//                        }
//                    }
//                }
			}
			break;
			case UTL_ERROR_NONE:
			{
				if(g_ErrorCode_Event == 1)
	        	{
	        		g_ErrorCode_Event = 0;
					Err_state = UTL_IDLE;
                    if((g_ErrorCode>=ERROR_NO_SPM)&&(g_ErrorCode<=ERROR_CONTROLZONE))
                    {
//                        g_ErrorCode = ERROR_NONE_EVENT;
                        g_ErrorCode = ERROR_NONE;
                        Program_Set_VirtualKey(VK_IRSENSOR_ERROR_RELEASED);
                        RF_Set_VirtualKey(VK_IRSENSOR_ERROR_RELEASED);
                    }
                    if((g_ErrorCode==ERR0R_UCB_EMERG)||(g_ErrorCode==ERROR_HANDRAIL_EMERG))
                    {
//                        g_ErrorCode = ERROR_NONE_EVENT;
                        g_ErrorCode = ERROR_NONE;
                        ret = Safekey_off_to_on;
                        Program_Set_VirtualKey(VK_UCBEMERG_ERROR_RELEASED);
                        RF_Set_VirtualKey(VK_UCBEMERG_ERROR_RELEASED);
                    }
				}
			}
			break;
			case UTL_ERROR_IRSENSOR:
			{
                Err_state = UTL_IDLE;
                ret = ERR_EVT_IRSENEOR;
                LCB_Send_Cmd(DS_SET_STATUS,0x01);
                LCB_Send_Cmd(DS_SET_SPM,0);
                LCB_Send_Cmd(DS_SET_STATUS,0xA5);
                Program_Set_VirtualKey(VK_IRSENSOR_ERROR);	
                //                RF_Set_VirtualKey(VK_IRSENSOR_ERROR);	
                UTL_ERP_Set_VK(VK_WARMUP);
			}
			break;
			case UTL_ERROR_CONTROLZONE:
			{
				Err_state = UTL_IDLE;
                ret = ERR_EVT_CONTROL_ZONE;
                LCB_Send_Cmd(DS_SET_STATUS,0x01);
                LCB_Send_Cmd(DS_SET_SPM,0);
                LCB_Send_Cmd(DS_SET_STATUS,0xA5);
                Program_Set_VirtualKey(VK_IRSENSOR_ERROR);
                //                RF_Set_VirtualKey(VK_IRSENSOR_ERROR);	
                UTL_ERP_Set_VK(VK_WARMUP);
			}
			break;
			case UTL_ERROR_HANDRAIL_EMERG:
			{
				Err_state = UTL_IDLE;
                ret = ERR_EVT_HANDRAIL_EMERGENCY;
                LCB_Send_Cmd(DS_SET_STATUS,0x01);
                LCB_Send_Cmd(DS_SET_SPM,0);
                LCB_Send_Cmd(DS_SET_STATUS,0xA5);
                Program_Set_VirtualKey(VK_UCBEMERG_ERROR);
                //                RF_Set_VirtualKey(VK_IRSENSOR_ERROR);	
                UTL_ERP_Set_VK(VK_WARMUP);                
			}
			break;
			case UTL_ERR0R_UCB_EMERG:
			{
				Err_state = UTL_IDLE;
                ret = ERR_EVT_UCB_EMERGENCY;
                LCB_Send_Cmd(DS_SET_STATUS,0x01);
                LCB_Send_Cmd(DS_SET_SPM,0);
                LCB_Send_Cmd(DS_SET_STATUS,0xA5);
                Program_Set_VirtualKey(VK_UCBEMERG_ERROR);
                //                RF_Set_VirtualKey(VK_IRSENSOR_ERROR);	
                UTL_ERP_Set_VK(VK_WARMUP); 
			}
			break;
            case UTL_ERROR_NO_SPM:
            {
                Err_state = UTL_IDLE;
                ret = ERR_EVT_NO_SPM;
                LCB_Send_Cmd(DS_SET_STATUS,0x01);
                LCB_Send_Cmd(DS_SET_SPM,0);
                Program_Set_VirtualKey(VK_IRSENSOR_ERROR);
                //                RF_Set_VirtualKey(VK_IRSENSOR_ERROR);	
                UTL_ERP_Set_VK(VK_WARMUP); 
            }
            break;
			default:
			break;
            
		}
	}
	else
	{
        Error = LCB_Dectect_Error();
//        if(Program_Get_Status()==PS_RUN)
//        Error = 0X02A1;
		if(Error != 0)
		{
			if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
			{
				for(UCHAR i = 0 ;i < 13 ; i++)
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

		if(flag < RET_PROCESS_DISPLAY)
		{
			if((Speed_IO_Check_Safe_AlwaysOff() || w_ErrorCode == SAFEKEY)&&(Err_state == UTL_IDLE)&& (SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL))
			{
				UTL_ERP_Set_VK(VK_WARMUP);	
			}
			if((Speed_IO_Check_Safe_AlwaysOn() )&&(Err_state == Safekey_OFF)&& (SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL))
			{
				UTL_ERP_Set_VK(VK_WARMUP);	
			}
			return ret;
		}

		if(flag == RET_REFRESH_DISPLAY )
		{
			if(Err_event_backup!= S_NO_EVENT)
			{
				flag = RET_PROCESS_DISPLAY;
			}
			return Err_event_backup;
		}


		switch(Err_state)
		{
			case UTL_IDLE:
			{//事件是安全开关移除，条件是原先开关存在，（I/O检测为拔掉或下控传错误）
                if((Speed_IO_Check_Safe_AlwaysOff() || w_ErrorCode == SAFEKEY)&& (SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL) )
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
                    ret = Safekey_on_to_off;
                }   
                }
			      else
                    {
                    Err_state = Safekey_OFF;
                    ret = Safekey_on_to_off;					
                }
                    */
                    Err_state = Safekey_OFF;
                    ret = Safekey_on_to_off;
                    LCB_Initial();
                    Program_Set_VirtualKey(VK_SAFETY_KEY_OFF);	
                    RF_Set_VirtualKey(VK_SAFETY_KEY_OFF);	
                    UTL_ERP_Set_VK(VK_WARMUP);
                    break;
                }
                switch(w_ErrorCode)
                {
                    case NO_INCODE:
                    {
                        Err_state = UTL_IDLE;
                        ret = S_NO_INCODE;
                        break;
                    }
                    
                    case MOS_FAULT:
                    {
                        Err_state = UTL_IDLE;
                        ret = S_MOS_FAULT;
                        break;
                    }
                    
                    case SPEED_FAST:
                    {
                        Err_state = UTL_IDLE;
                        ret = S_SPEED_FAST;
                        break;
                    }
                    
                    case SPEED_LOW:
                    {
                        Err_state = UTL_IDLE;
                        ret = S_SPEED_LOW;
                        break;
                    }
                    
                    case OVER_CURRENT:
                    {
                        Err_state = UTL_IDLE;
                        ret = S_OVER_CURRENT;
                        break;
                    }
                    
                    case INC_NOCOUNT:
                    {
                        Err_state = UTL_IDLE;
                        ret = S_INC_NOCOUNT;
                        break;
                    }
                    
                    case MCB_TEMP_OVER:
                    {
                        Err_state = UTL_IDLE;
                        ret = S_MCB_TEMP_OVER;
                        break;
                    }
                    
                    case DIGTIAL_TIMEOUT:
                    {
                        Err_state = UTL_IDLE;
                        ret = S_DIGTIAL_TIMEOUT;
                        break;
                    }	
                    
                    case DIGTIAL_RX_ERROR:
                    {
                        Err_state = UTL_IDLE;
                        ret = S_DIGTIAL_RX_ERROR;
                        break;
                    }
                    
                    case FALSH_WRITE:
                    {
                        Err_state = UTL_IDLE;
                        ret = S_FALSH_WRITE;
                        break;
                    }
                    
                    case FALSH_READ:
                    {
                        Err_state = UTL_IDLE;
                        ret = S_FALSH_READ;
                        break;
                    }
                    case OVER_SPEED:
                    {
                        Err_state = UTL_IDLE;
                        Program_Set_VirtualKey(VK_SPEEDOVER);
                        break;
                    }
                    default:
                    break;
                }
                break;
			}	
			break;
			case Safekey_OFF:
			{//事件是安全开关插上，条件是原状态为Safekey_OFF，I/O检测为插上且下控无开关错误）
                ret = ERR_PROCESSED;
                if(Speed_IO_Check_Safe_AlwaysOn())
                {
                    LCB_Send_Cmd(DS_INITIAL,0);
                    LCB_Send_Cmd(DS_WORK_STATUS,0);
                    LCB_Send_Cmd(DS_WORK_STATUS,0x01);
                    LCB_Send_Cmd(DS_MOTOR_HP,0x5678);
                    if(w_ErrorCode != SAFEKEY)
                    {
                        SafetyKey_DelayCnt = 0 ;
                        SafetyKey_DelayStatus = 0 ;
                        Err_state = UTL_IDLE;
                        ret = Safekey_off_to_on;
                    }
                }
			}
			break;
			default:
			break;
        }	
	}
	if((ret!= ERR_PROCESSED)&&(ret!= S_NO_EVENT))
	{
		Err_event_backup = ret;
	}
	return ret;
}

/**************************************************************************
* @brief
*   检测安全开关400ms内是否一直处于拔掉状态。
*   
* @return
*   返回1即拔除，0即不是一直拔出。
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
*   检测安全开关100ms内是否一直处于存在状态。
*   
* @return
*   返回1即一直存在，0即不是一直存在。。
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
void Clearg_ErrorCode()
{
    g_ErrorCode = 0;
}
UINT16 UTL_Get_ErrorCode(void)
{
	return g_ErrorCode ;
}
bool UTL_Get_ErrorLevel(void)
{
  return ClassLevel;
}


