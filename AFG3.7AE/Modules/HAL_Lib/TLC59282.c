#include "TLC59282.h"
#include "HAL_Lib.h"

void TLC59282_Process_SPI(char *pby_TLC59282Buffer,char by_5026_Count)
{
	char by_Loop,by_Number,by_Dat;
    
    HAL_Set_IO_High(TLC59282_OE_PORT,TLC59282_OE_PIN);
    
	for(by_Loop=0;by_Loop<by_5026_Count*2;by_Loop++)
	{
		by_Dat = *(pby_TLC59282Buffer + by_5026_Count*2 - by_Loop - 1);
		for(by_Number=0;by_Number<8;by_Number++)
		{
			if(by_Dat & (0x80 >> by_Number))
			{
                HAL_Set_IO_High(TLC59282_DAT_PORT,TLC59282_DAT_PIN);
			}
			else
			{
				HAL_Set_IO_Low(TLC59282_DAT_PORT,TLC59282_DAT_PIN);
			}
            HAL_Set_IO_Low(TLC59282_CLK_PORT,TLC59282_CLK_PIN);
			HAL_Set_IO_High(TLC59282_CLK_PORT,TLC59282_CLK_PIN);
		}
	}
    HAL_Set_IO_Low(TLC59282_LE_PORT,TLC59282_LE_PIN);
    HAL_Set_IO_High(TLC59282_LE_PORT,TLC59282_LE_PIN);
    HAL_Set_IO_High(TLC59282_LE_PORT,TLC59282_LE_PIN);
    HAL_Set_IO_Low(TLC59282_LE_PORT,TLC59282_LE_PIN);
	HAL_Set_IO_Low(TLC59282_OE_PORT,TLC59282_OE_PIN);
}
