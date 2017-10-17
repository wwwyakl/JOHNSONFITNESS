#ifndef _UtilitiesFan_H_
#define _UtilitiesFan_H_

#include "HAL_Lib.h"

typedef struct
{
	uint8_t LowPwm;
	uint8_t MidPwm;
	uint8_t HighPwm;
	uint8_t Fan_Level_Number;
}FAN_PWM;

void Fan_Initial_Data(FAN_PWM Fan_Pwm);
unsigned char Fan_Get_Level(void);
void Fan_Set_Level(UCHAR by_Dat);
void Fan_Increase_Level(void);
void Fan_Decrease_Level(void);

#endif
