#ifndef _LED_H_
#define _LED_H_
#include "HAL_Lib.h"
#include "TLC59282.h"

typedef enum
{
    LED1_PRO_MANUAL,//
    LED2_PRO_WEIGHT_LOSS,
    LED3_PRO_INTERVALS,
    LED4,
    LED5_TIME,
    LED6_CALORIES,
    LED7_SPEED,
    LED8_INCLINE,
    LED9_DISTANCE,
    LED10_HRT,
    LED11_FAN,
    LED12_BT,
    LED13_PRO_DISTANCE,
    LED14_PRO_CALORIES,
    LED15_CHANGE_DISPLAY,
    TIME_COM1,
    TIME_COM5,
    LED_HRT_ICON,
    LEDEND,
}LedType;

typedef enum
{
    LED1_RPM_EP,
    LED2_DISTANCE_EP,
    LED3_HRT_EP,
    LED4_EP,
    LED5_CHANGE_DISPLAY_EP,
    LED6_CALORIES_EP,
    LED7_PRO_CALORIES_EP,
    LED8_RESISTANCE_EP,
    LED9_PRO_WEIGHT_LOSS_EP,
    LED10_TIME_EP,
    LED11_PRO_MANUAL_EP,
    LED12_PRO_DISTANCE_EP,
    LED13_BT_EP,
    LED14_PRO_INTERVALS_EP,
    LED15_EP,
    TIME_COM1_EP,
    TIME_COM5_EP,
    LED_HRT_ICON_EP,
    LEDEND_EP,
}EP_LedType;

typedef enum
{
    LED_OFF,
    LED_ON,
}LED_STATUS;

//#define P_MANUAL        	     			        1
//#define P_DISTANCE     	     			        	2
//#define P_FAT_BURN     	     			        	3
//#define P_CALORIES									4
//#define P_HILL_CLIMB								5
//#define P_TGART_HEART_RATE							6
//#define P_MY_FIRST_5K								7
//#define P_CUSTOM								    8
//#define P_CUSTOM_HEART_RATE							9
//#define P_SPRINT									10

//#define DP_LED1 1
//#define DP_LED2 2
//#define DP_LED3 3
//#define DP_LED4 4
//#define DP_LED7 5
//#define DP_LED8 6
//#define DP_LED9 7
//#define DP_LED10 8
//#define DP_LED11 9
//#define DP_LED12 10
//#define DP_LED13 11
//#define DP_LED14 12
//#define DP_LED15 13
//#define DP_LED16 14

//#define C_DOT_STOP 0
//#define C_DOT_PAGE 1
//#define C_DOT_STR  2
//#define C_14SEG_STOP 3
//#define C_14SEG_PAGE 4
//#define C_14SEG_STR  5

//#define DB_ALL        0xff
//#define DB_TIME       1
//#define DB_INCLINE    2
//#define DB_DISTANCE   3
//#define DB_SPEED      4
//#define DB_CALORIES   6
//#define DB_PULSE      7
//#define DB_WEIGHT     9
//#define DB_LEVEL      10
//#define DB_PROGRAM    11
//#define DB_HEART    12
//#define DB_RESISTANCE    13

//#define DN_ALL_OFF  0xff
//#define DN_NORMAL   0x00
//#define DN_THREE    0x01
//#define DN_TWO      0x02
//void LED_Clear_string(void);
void LED_Intial_Data(void);
void LED_Set_All(char by_Mode);
//void LED_Set_Move_Speed(unsigned short w_Move_Speed);
//void LED_14_SEG_Dot(signed char by_Address, char by_Onoff);
//void LED_14_SEG_String(const char *pby_Str);
//void LED_14_SEG_Number(char by_Screen,const char *pby_Str,char by_Mode);
//void LED_14_SEG_Char(signed char by_Address,signed char by_Dat);
//void LED_Bit_Exchange(unsigned short *w_Buffer);
//void LED_14_SEG_View(signed char by_Adress,unsigned short w_Data);
//void LED_Set_Move_Mode(char by_Mode);
//unsigned char LED_Get_14SEG_OK_Count(void);
//void LED_Scan_1ms_Int(void);
//void LED_Updata(char by_Mode);
void LED_Light(LedType ledIndex,LED_STATUS ledstatus);
void LED_Demo_Processs(void);
//void LED_Program_BL(char by_Program,char by_Data);
//void LED_Program_BL1(const char *pby_Str,char by_Data);
//void LED_Byte_Exchange(unsigned short *w_Buffer);
void LED_Regist_Functions(void);
void LED_String(unsigned char x,unsigned char y,unsigned char Z,const char *str);
//void LED_Clear_ProgramBL(void);
#endif








































