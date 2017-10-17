#include "UtilitiesBurntest.h"
#include "SystemConfig.h"

typedef enum
{
	S_AGINGOFF = 0,
	S_AGINGON,
	S_TOTAL
}BURNTEST_STATE;
__no_init static bool aby_Check[5];
__no_init UINT32 TestTime_Sec;
__no_init UINT16 Timer1sCounter;
__no_init BURNTEST_STATE BurnTest_State ;


VR_KEY UTL_BurnTest_Set_VK(VR_KEY vk)
{
	VR_KEY ret = vk;
	if(BurnTest_State ==S_AGINGON )
	{
		ret = VK_PROCESSED;
	}
	return ret;
}



void BurnTest_1ms_ISR(void)
{
	if(BurnTest_State==S_AGINGON)
	{
		if(++Timer1sCounter>=1000)
		{
			Timer1sCounter = 0 ;
			TestTime_Sec++;
		}
	}
}

void BurnTest_Initial_IO(void)
{
	HAL_LFT_Initial_IO();
	HAL_SystemTicker_Alloc_ISR(BurnTest_1ms_ISR);
}

void BurnTest_Initial_Data(void)
{
	BurnTest_Clear_MEM();
	TestTime_Sec = 0 ;
	Timer1sCounter = 0 ;
	BurnTest_State = S_AGINGOFF;
}

BURNTEST_EVENT BurnTest_Process(ReturnEventHandler flag)
{
	BURNTEST_EVENT BurnTest_Event = E_NO_EVENT;

	if(flag<RET_PROCESS_DISPLAY) return BurnTest_Event;

	unsigned char by_count = 0;
    

	BurnTest_Save_MEM_FIFO(HAL_LFT_Get_Enable());
	switch(BurnTest_State)
	{
		case S_AGINGOFF:
		{
			for(UCHAR i = 0;i < 5;i++)
			{
				if(aby_Check[i] == AGINGON)
				{
					by_count++;
				}
			}
			if(by_count == 5)
			{
				BurnTest_State = S_AGINGON;
				BurnTest_Event = E_AGINGON;
				Timer1sCounter = 0 ;
				TestTime_Sec =  0;
			}
			else
			{
				BurnTest_Event = E_NO_EVENT;
			}
			break;
		}
		
		case S_AGINGON:
		{
			for(UCHAR i = 0;i < 5;i++)
			{
				if(aby_Check[i] == AGINGOFF)
				{
					by_count++;
				}
			}
			if(by_count == 5)
			{
				UINT16 AccAgingHours =  SystemConfig_Get_Int(ACCUMULATED_AGING_HOURS_UINT16);
				AccAgingHours += (TestTime_Sec/3600);
				SystemConfigProcess(SVK_ACCAGING_HOURS,&AccAgingHours,sizeof(AccAgingHours));
                	SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
				ERP_Initial_Data();					
				BurnTest_State = S_AGINGOFF;
				BurnTest_Event = E_AGINGOFF;
			}
			else
			{
				BurnTest_Event = E_PROCESSED;
			}
			break;
		}
		
		default:
			
		break;
	}
	return BurnTest_Event;
}

void BurnTest_Save_MEM_FIFO(bool by_Dat)
{
    UCHAR by_Loop;
    for(by_Loop = 4 ; by_Loop > 0; by_Loop--)
    {
        aby_Check[by_Loop] = aby_Check[by_Loop - 1];
    }
    aby_Check[0] = by_Dat;
}

void BurnTest_Clear_MEM(void)
{
    UCHAR by_Loop;
    for(by_Loop = 0; by_Loop < 5; by_Loop++)
    {
        aby_Check[by_Loop] = AGINGOFF;
    }
}

