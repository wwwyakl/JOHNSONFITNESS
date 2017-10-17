#include "UtilitiesAMP.h"

unsigned char volarray[] = {0,20,40,60,80,100};

__no_init static unsigned char by_Current_Level;

void AMP_PROCESS(void)
{
    if(HAL_AMP_Check_AUX())
    {
        HAL_Set_IO_Low(Audio_Source1_PORT, Audio_Source1_PIN);
    }
    else
    {
        HAL_Set_IO_High(Audio_Source1_PORT, Audio_Source1_PIN);
    }
//    if(wlt2564GetAmpConnectStatus()==1)
//    {
//        if(HAL_AMP_Check_AUX()==0)
//        {
//            HAL_Set_IO_High(Audio_Source1_PORT, Audio_Source1_PIN);
//        }
//        else
//        {
//            HAL_Set_IO_Low(Audio_Source1_PORT, Audio_Source1_PIN);
//        }
//    }
//    else
//    {
//        if(HAL_AMP_Check_AUX())
//        {
//            HAL_Set_IO_Low(Audio_Source1_PORT, Audio_Source1_PIN);
//        }
//        else
//        {
//            HAL_Set_IO_High(Audio_Source1_PORT, Audio_Source1_PIN);
//     
//        }
//    }
}

void AMP_Initial_Data(void)
{
	by_Current_Level = 2;
	HAL_AMP_Set_Duty(volarray[by_Current_Level]);
}

UCHAR AMP_Get_Level(void)
{
	return(by_Current_Level);
}

void AMP_Increase_Level(void)
{	
	if( by_Current_Level > 0 )
	{
		by_Current_Level -- ;
	}
	else
	{
		by_Current_Level = 4;
	}
	HAL_AMP_Set_Duty( volarray[by_Current_Level]);
}

void AMP_Decrease_Level(void)
{
	if( by_Current_Level < 4 )
	{
		by_Current_Level ++;
	}
	else 
	{
		by_Current_Level = 0;
	}
	HAL_AMP_Set_Duty( volarray[by_Current_Level]);
}

