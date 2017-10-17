#ifndef _UtilitiesFan_H_
#define _UtilitiesFan_H_

#include "HAL_Lib.h"
#include "LED.h"
#include "Global_Config.h"

typedef struct
{
	uint8_t LowPwm;
	uint8_t MidPwm;
	uint8_t HighPwm;
	uint8_t Fan_Level_Number;
}FAN_PWM;
#ifdef FRAMEWORK2_0
VR_KEY UTL_Fan_Set_Virtualkey(VR_KEY vk);
#else
void UTL_Fan_Set_Virtualkey(UINT16 vk);
#endif
void Fan_Initial_Data(FAN_PWM Fan_Pwm);
unsigned char Fan_Get_Level(void);
void Fan_Set_Level(UCHAR by_Dat);
void Fan_Increase_Level(void);
void Fan_Decrease_Level(void);
void Fan_Initial_IO(void);

#endif
