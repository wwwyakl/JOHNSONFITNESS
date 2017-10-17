#ifndef __PROGRAM_INNER_H_
#define __PROGRAM_INNER_H_
#include "Engmode.h"

#define ENG_NONE_KEY_EVENT  0
#define ENG_ENTER_KEY_EVENT 1
#define ENG_BACK_KEY_EVENT   2
#define ENG_RESET_KEY_EVENT   3

//#define	NO_EVENT         0x0000
//#define	DATA_1S_EVENT    0x0001
//#define	SPEED_EVENT      0x0003
//#define	INCLINE_EVENT    0x0005
//#define	RESISTANCE_EVENT 0x0009
//#define SPEED_INCLINE_EVENT 0x0007
//#define RESISTANCE_INCLINE_EVENT 0x000d
//
//#define	WARM_UP_EVENT    0x0041
//#define	COOLDOWN_EVENT   0x0081
//#define	COMPLETE_EVENT   0x0101
//#define	RESET_EVENT      0x0201

typedef struct
{
	UCHAR Engid;
	void (*Initial)(void);
	ENG_SETTING_ENUM (*Setup)(ENGKeyEvts);
	UCHAR (*Run)(UCHAR);
}struEngmode;

void Engmode_Initial_Inner_Data(void);
void Engmode_Add_Array(struEngmode pro);

UINT8 Engmode_Get_KeyEvent(void);
void Engmode_Set_Event(ENG_EVENT pro_event);

bool Engmode_Get_DataEventFlag(void);
#endif
