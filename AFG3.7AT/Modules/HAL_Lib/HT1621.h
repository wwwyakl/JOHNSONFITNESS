#ifndef __HT1621_H__
#define __HT1621_H__

#include "HAL_IO_define.h"

void HT1621_Initial_IO(void);
void HT1621_Updata_Data(char *pby_Lcd_Buffer,char by_1621_Count);
//void HT1621_Initial_Command_First(char by_IC);
//void HT1621_Initial_Command(char by_IC);

#endif	//__HT1621_H__
