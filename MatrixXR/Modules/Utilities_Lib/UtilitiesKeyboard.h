#ifndef _UtilitiesKeyboard_H_
#define _UtilitiesKeyboard_H_

#include "HAL_Lib.h"
#include "UtilitiesBeep.h"





#define KB_NONE           0xAA
#define KB_NA             0xff
#define KB_HOLD_10S       1000
#define KB_HOLD_5S        500
#define KB_HOLD_3S        300
#define KB_HOLD_1S        100
#define KB_LOCK           0xEE//add by ckm

#define KB_OFFSET        0x80

#define KB_NUMBER0              1
#define KB_NUMBER1              2
#define KB_NUMBER2              3
#define KB_NUMBER3              4
#define KB_NUMBER4              5
#define KB_NUMBER5              6
#define KB_NUMBER6              7
#define KB_NUMBER7              8
#define KB_NUMBER8              9
#define KB_NUMBER9              10

#define	K_TV_Power				11
#define	K_TV_Last				12
#define	K_TV_VO_UP				13
#define	K_TV_VO_Down			14
#define	K_TV_CH_UP				15
#define	K_TV_CH_Down			16
#define	K_Back				    17
#define	K_TV_CC					18
#define	K_TV_Mute				19
#define	K_MASK					20

#define	KB_INCLINE_DOWN			21
#define	KB_INCLINE_UP			22
#define	KB_SPEED_SLOW 			23
#define	KB_SPEED_FAST			24
#define	KB_RESISTANCE_DOWN		25
#define	KB_RESISTANCE_UP		26
#define	KB_START				27
#define	KB_STOP					28
#define	KB_ENTER				29
#define	KB_BACK					30
#define	KB_FAN					31
#define	KB_CHANGE_DISPLAY		32
#define	KB_ARROW_UP				33
#define	KB_ARROW_DOWN			34
#define	KB_VOL_UP				35
#define	KB_VOL_DOWN				36

#define	KB_WIFI					37
#define	KB_RF					38
#define	KB_BLUETOOTH			39
#define	KB_SPLIT				40
#define	KB_SELECT_GOAL			41
#define	KB_COOL_DOWN			42
#define	KB_ERP					43


#define	KB_IPOD_VOLUM_INC		44
#define	KB_IPOD_VOLUM_DEC		45
#define	KB_IPOD_PLAYPAUSE		46
#define	KB_IPOD_PREVIOUS		47
#define	KB_IPOD_NEXT			48

#define	KB_SET_SPEED			49
#define	KB_SET_INCLINE			50
#define	KB_SET_RESISTANCE		51

#define KB_SLOW_UP          	52
#define KB_LUBEBELTRESET			53
#define KB_ENG_Backup				54
#define KB_RTC_Backup				55
#define KB_LUBEBELT_Backup				56
#define KB_Combo_4				57
#define KB_Combo_5				58
#define KB_Hold_9               59
#define KB_Hold_0              60
#define KB_RELEASED              61

#define KB_RESET         		KB_STOP+KB_OFFSET
#define KB_SCAN         		KB_ENTER+KB_OFFSET
#define KB_ENG          		KB_SLOW_UP+KB_OFFSET
#define KB_SAVE         		KB_START+KB_OFFSET
#define KB_CLEAR	            KB_START+KB_OFFSET	
#define	KB_CUSTOMRST			KB_ENTER+KB_OFFSET	
#define	KB_WIFI_AP				KB_WIFI+KB_OFFSET	
#define	KB_BT_PAIR				KB_BLUETOOTH+KB_OFFSET	

#define KB_LUBERESET      		KB_SPEED_FAST + KB_STOP+KB_OFFSET//?
#define	KB_RTC					KB_NUMBER1+KB_NUMBER2+KB_OFFSET	
#define	KB_RF_SYNC				KB_RESISTANCE_UP + KB_STOP+KB_OFFSET	
#define KB_SPEED_LONGPRESS      KB_SPEED_FAST+KB_OFFSET  
#define KB_RESISTANCE_LONGPRESS KB_RESISTANCE_UP+KB_OFFSET

typedef struct 
{
	UINT8  row;
	UINT8  col;
}KEY_DIM;

typedef struct
{
	UINT8  code1;
	UINT16 holdTime;
	UINT8  code;
}KEY_HOLD;

typedef struct 
{
	UINT8  code1;
	UINT8  code2;
	UINT8  code;
}KEY_COMBO;
void Keyboard_Initial_IO(void);
void KeyBoard_Set_KeyComboTable(KEY_COMBO *pComboTable,UINT8 column);
void KeyBoard_Set_KeyholdTable(KEY_HOLD *pHoldTable,UINT8 column);
void KeyBoard_Set_TrueTable(const unsigned char *pTable, UINT8 rows, UINT8 columns);
char KeyBoard_Get_Code(char mode);
void Keyboard_Initial_Data(void);

#endif
