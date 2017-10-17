#include <string.h>
#include "Global_Config.h"
#include "LED.h"
#include "Events_Handler.h"

extern FRAME_STAGE Frame_Stage;

#define COM_NUMBER 8
#define C_14_SGE_STRING_MAX  70
#define C_14_SEG_MAX     24
#define C_14_SEG_COUNT_MAX 255
#if 1
typedef enum
{
    COM8,
    COM7,
    COM6,
    COM5,
    COM4,
    COM3,
    COM2,
    COM1,
    COMEND,
    VCOM1,
    VCOM2,
    VCOM3
    
}COMType;
typedef struct
{
    UCHAR seg1:1;
    UCHAR seg2:1;
    UCHAR seg3:1;
    UCHAR seg4:1;
    UCHAR seg5:1;
    UCHAR seg6:1;
    UCHAR seg7:1;
    UCHAR seg8:1;
    UCHAR seg9:1;
    UCHAR seg10:1;
    UCHAR seg11:1;
    UCHAR seg12:1;
    UCHAR seg13:1;
    UCHAR seg14:1;
    UCHAR seg15:1;
    UCHAR seg16:1;
    UCHAR seg17:1;
    UCHAR seg18:1;
    UCHAR seg19:1;
    UCHAR seg20:1;
    UCHAR seg21:1;
    UCHAR seg22:1;
    UCHAR seg23:1;
    UCHAR seg24:1;
    UCHAR seg25:1;
    UCHAR seg26:1;
    UCHAR seg27:1;
    UCHAR seg28:1;
    UCHAR seg29:1;
    UCHAR comA0:1;
    UCHAR comA1:1;
    UCHAR comA2:1;
}seg32byte;
typedef union{
 seg32byte bitsType;
UCHAR abyData[4];
}abyLedStruct;
static abyLedStruct aby_led_data1[COMEND];


const char aby_Number_Code[24]=
{
 // 0    1     2   3    4    5    6    7    8    9
    0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,
    //OFF  ON    -  t   o    p     e    n   d     -
    0x00,0xFF,0x40,0x1E,0x3A,0x73,0x79,0x54,0x56,0x02,
    //f  i     l      g
    0x8E,0x20,0x38,0x6F
};


void LED_Flag_10ms_Int(void)
{
}
void LED_1ms_ISR(void)
{
    //return;
    static UCHAR sComIndex = 0;
    static char by_Count;
    aby_led_data1[sComIndex].abyData[3] &= 0x1F;
    aby_led_data1[sComIndex].abyData[3] |= (sComIndex<<5);
    TLC59282_Process_SPI(aby_led_data1[sComIndex].abyData,4);
    sComIndex++;
    sComIndex = sComIndex%COMEND;
    by_Count++;
    if(by_Count >= 10)//10ms
    {
        by_Count = 0;
        LED_Flag_10ms_Int();
    }
}

void LED_Light(LedType ledIndex,LED_STATUS ledstatus)
{
#ifdef TM
    switch(ledIndex)
    {
        case LED1_PRO_MANUAL:
        {
            aby_led_data1[COM3].bitsType.seg19 = ledstatus;
        }
        break;
        case LED2_PRO_WEIGHT_LOSS:
        {
            aby_led_data1[COM3].bitsType.seg20 = ledstatus;
        }
        break;
        case LED3_PRO_INTERVALS:
        {
            aby_led_data1[COM3].bitsType.seg21 = ledstatus;
        }
        break;
        case LED5_TIME:
        {
            aby_led_data1[COM3].bitsType.seg23 = ledstatus;
        }
        break;
        case LED6_CALORIES:
        {
            aby_led_data1[COM3].bitsType.seg24 = ledstatus;
        }
        break;
        case LED7_SPEED:
        {
            aby_led_data1[COM4].bitsType.seg19 = ledstatus;
        }
        break;
        case LED8_INCLINE:
        {
            aby_led_data1[COM4].bitsType.seg20 = ledstatus;
        }
        break;
        case LED9_DISTANCE:
        {
            aby_led_data1[COM4].bitsType.seg21 = ledstatus;
        }
        break;
        case LED10_HRT:
        {
            aby_led_data1[COM1].bitsType.seg1 = ledstatus;
        }
        break;
        case LED11_FAN:
        {
            aby_led_data1[COM2].bitsType.seg2 = ledstatus;
        }
        break;
        case LED12_BT:
        {
            aby_led_data1[COM3].bitsType.seg3 = ledstatus;
        }
        break;
        case LED13_PRO_DISTANCE:
        {
            aby_led_data1[COM4].bitsType.seg4 = ledstatus;
        }
        break;
        case LED14_PRO_CALORIES:
        {
            aby_led_data1[COM5].bitsType.seg5 = ledstatus;
        }
        break;
        case LED15_CHANGE_DISPLAY:
        {
            aby_led_data1[COM6].bitsType.seg6 = ledstatus;
        }
        break;
        case TIME_COM1:
        {
            if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
            {
                aby_led_data1[COM4].bitsType.seg17 = ledstatus;
            }
            else
            {
                aby_led_data1[COM4].bitsType.seg9 = ledstatus;
            }
        }
        break;
        case TIME_COM5:
        {
            aby_led_data1[COM4].bitsType.seg18 = ledstatus;
        }
        break;
        case LED_HRT_ICON:
        {
            aby_led_data1[COM8].bitsType.seg26 = ledstatus;
        }
        break;
    }
#else
    switch(ledIndex)
    {
        case LED1_RPM_EP:
        {
            aby_led_data1[COM3].bitsType.seg19 = ledstatus;
        }
        break;
        case LED2_DISTANCE_EP:
        {
            aby_led_data1[COM3].bitsType.seg20 = ledstatus;
        }
        break;
        case LED3_HRT_EP:
        {
            aby_led_data1[COM3].bitsType.seg21 = ledstatus;
        }
        break;
        case LED5_CHANGE_DISPLAY_EP:
        {
            aby_led_data1[COM3].bitsType.seg23 = ledstatus;
        }
        break;
        case LED6_CALORIES_EP:
        {
            aby_led_data1[COM3].bitsType.seg24 = ledstatus;
        }
        break;
        case LED7_PRO_CALORIES_EP:
        {
            aby_led_data1[COM4].bitsType.seg19 = ledstatus;
        }
        break;
        case LED8_RESISTANCE_EP:
        {
            aby_led_data1[COM4].bitsType.seg20 = ledstatus;
        }
        break;
        case LED9_PRO_WEIGHT_LOSS_EP:
        {
            aby_led_data1[COM4].bitsType.seg21 = ledstatus;
        }
        break;
        case LED10_TIME_EP:
        {
            aby_led_data1[COM1].bitsType.seg1 = ledstatus;
        }
        break;
//        case LED11_FAN:
//        {
//            aby_led_data1[COM2].bitsType.seg2 = ledstatus;
//        }
//        break;
        case LED11_PRO_MANUAL_EP:
        {
            aby_led_data1[COM2].bitsType.seg2 = ledstatus;
        }
        break;
        case LED12_PRO_DISTANCE_EP:
        {
            aby_led_data1[COM3].bitsType.seg3 = ledstatus;
        }
        break;
        case LED13_BT_EP:
        {
            aby_led_data1[COM4].bitsType.seg4 = ledstatus;
        }
        break;
        case LED14_PRO_INTERVALS_EP:
        {
            aby_led_data1[COM5].bitsType.seg5 = ledstatus;
        }
        break;
        case TIME_COM1_EP:
        {
            if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
            {
                aby_led_data1[COM4].bitsType.seg17 = ledstatus;
            }
            else
            {
                aby_led_data1[COM4].bitsType.seg9 = ledstatus;
            }
        }
        break;
        case TIME_COM5_EP:
        {
            aby_led_data1[COM4].bitsType.seg18 = ledstatus;
        }
        break;
        case LED_HRT_ICON_EP:
        {
            aby_led_data1[COM8].bitsType.seg26 = ledstatus;
        }
        break;
    }
#endif
}

void LEDCom(COMType comIndex,UCHAR data, UCHAR type)   //zsf
{
    UCHAR temp;
    
    if (1 == type)
    {
    temp = aby_Number_Code[data&0x7f];
    if(data &0x80)
    {
        temp = temp|0x80;
    }
    }
    else if (0 == type)
    {
        temp = data;
    }
    
    switch(comIndex)
    {
        case COM1:
        {
            if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
            {
                aby_led_data1[comIndex].abyData[1] =  temp;
            }
            else
            {
                aby_led_data1[comIndex].bitsType.seg14 =temp&0x01;
                aby_led_data1[comIndex].bitsType.seg16  = (temp&0x02)>>1;
                aby_led_data1[comIndex].bitsType.seg10 = (temp&0x04)>>2;
                aby_led_data1[comIndex].bitsType.seg12  = (temp&0x08)>>3;
                aby_led_data1[comIndex].bitsType.seg13 = (temp&0x10)>>4;
                aby_led_data1[comIndex].bitsType.seg15  = (temp&0x20)>>5;
                aby_led_data1[comIndex].bitsType.seg8 = (temp&0x40)>>6;
                aby_led_data1[comIndex].bitsType.seg11 =(temp&0x80)>>7;
            }
        }
        break;
        case VCOM1:
        {
            aby_led_data1[COM1].bitsType.seg18 =temp&0x01;
            aby_led_data1[COM1].bitsType.seg4  = (temp&0x02)>>1;
            aby_led_data1[COM1].bitsType.seg5 = (temp&0x04)>>2;
            aby_led_data1[COM1].bitsType.seg6  = (temp&0x08)>>3;
            aby_led_data1[COM1].bitsType.seg7 = (temp&0x10)>>4;
            aby_led_data1[COM1].bitsType.seg26  = (temp&0x20)>>5;
            aby_led_data1[COM1].bitsType.seg27 = (temp&0x40)>>6;
            aby_led_data1[COM1].bitsType.seg25 =(temp&0x80)>>7;
        }
        break;
        case COM2:
        {
            if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
            {
                aby_led_data1[comIndex].abyData[1] =  temp;
            }
            else
            {
                aby_led_data1[comIndex].bitsType.seg14 =temp&0x01;
                aby_led_data1[comIndex].bitsType.seg16  = (temp&0x02)>>1;
                aby_led_data1[comIndex].bitsType.seg10 = (temp&0x04)>>2;
                aby_led_data1[comIndex].bitsType.seg12  = (temp&0x08)>>3;
                aby_led_data1[comIndex].bitsType.seg13 = (temp&0x10)>>4;
                aby_led_data1[comIndex].bitsType.seg15  = (temp&0x20)>>5;
                aby_led_data1[comIndex].bitsType.seg8 = (temp&0x40)>>6;
                aby_led_data1[comIndex].bitsType.seg11 =(temp&0x80)>>7;
            }
        }
        break;
        case VCOM2:
        {
            aby_led_data1[COM2].bitsType.seg18 = temp&0x01;
            aby_led_data1[COM2].bitsType.seg4  = (temp&0x02)>>1;
            aby_led_data1[COM2].bitsType.seg5 = (temp&0x04)>>2;
            aby_led_data1[COM2].bitsType.seg6  = (temp&0x08)>>3;
            aby_led_data1[COM2].bitsType.seg7 = (temp&0x10)>>4;
            aby_led_data1[COM2].bitsType.seg26  = (temp&0x20)>>5;
            aby_led_data1[COM2].bitsType.seg27 = (temp&0x40)>>6;
            aby_led_data1[COM2].bitsType.seg25 =(temp&0x80)>>7;
        }
        break;
        case COM3:
        {
            if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
            {
                aby_led_data1[comIndex].abyData[1] =  temp;
            }
            else
            {
                aby_led_data1[comIndex].bitsType.seg14 =temp&0x01;
                aby_led_data1[comIndex].bitsType.seg16  = (temp&0x02)>>1;
                aby_led_data1[comIndex].bitsType.seg10 = (temp&0x04)>>2;
                aby_led_data1[comIndex].bitsType.seg12  = (temp&0x08)>>3;
                aby_led_data1[comIndex].bitsType.seg13 = (temp&0x10)>>4;
                aby_led_data1[comIndex].bitsType.seg15  = (temp&0x20)>>5;
                aby_led_data1[comIndex].bitsType.seg8 = (temp&0x40)>>6;
                aby_led_data1[comIndex].bitsType.seg11 =(temp&0x80)>>7;
            }
        }
        break;
        case VCOM3:
        {
            aby_led_data1[COM3].bitsType.seg18 = temp&0x01;
            aby_led_data1[COM3].bitsType.seg4  = (temp&0x02)>>1;
            aby_led_data1[COM3].bitsType.seg5 = (temp&0x04)>>2;
            aby_led_data1[COM3].bitsType.seg6  = (temp&0x08)>>3;
            aby_led_data1[COM3].bitsType.seg7 = (temp&0x10)>>4;
            aby_led_data1[COM3].bitsType.seg26  = (temp&0x20)>>5;
            aby_led_data1[COM3].bitsType.seg27 = (temp&0x40)>>6;
            aby_led_data1[COM3].bitsType.seg25 =(temp&0x80)>>7;
        }
        break;
        case COM4:
        {
            if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
            {
                aby_led_data1[comIndex].abyData[1] =  temp;
            }
            else
            {
                aby_led_data1[comIndex].bitsType.seg14 =temp&0x01;
                aby_led_data1[comIndex].bitsType.seg16  = (temp&0x02)>>1;
                aby_led_data1[comIndex].bitsType.seg10 = (temp&0x04)>>2;
                aby_led_data1[comIndex].bitsType.seg12  = (temp&0x08)>>3;
                aby_led_data1[comIndex].bitsType.seg13 = (temp&0x10)>>4;
                aby_led_data1[comIndex].bitsType.seg15  = (temp&0x20)>>5;
                aby_led_data1[comIndex].bitsType.seg8 = (temp&0x40)>>6;
                aby_led_data1[comIndex].bitsType.seg11 =(temp&0x80)>>7;
            }
        }
        break;
        case COM5:
        {
            aby_led_data1[comIndex].abyData[2] &=  0x03;
            aby_led_data1[comIndex].abyData[3] &=  0xFC;
            
            aby_led_data1[comIndex].abyData[2] |=  (temp&0x3F)<<2;
            aby_led_data1[comIndex].abyData[3] |=  (temp&0xC0)>>6;
        }
        break;
        case COM6:
        {
            aby_led_data1[comIndex].abyData[2] &=  0x03;
            aby_led_data1[comIndex].abyData[3] &=  0xFC;
            aby_led_data1[comIndex].abyData[2] |=  (temp&0x3F)<<2;
            aby_led_data1[comIndex].abyData[3] |=  (temp&0xC0)>>6;
        }
        break;
        case COM7:
        {
            aby_led_data1[comIndex].abyData[2] &=  0x03;
            aby_led_data1[comIndex].abyData[3] &=  0xFC;
            aby_led_data1[comIndex].abyData[2] |=  (temp&0x3F)<<2;
            aby_led_data1[comIndex].abyData[3] |=  (temp&0xC0)>>6;
        }
        break;
        case COM8:
        {
            aby_led_data1[comIndex].abyData[2] &=  0x03;
            aby_led_data1[comIndex].abyData[3] &=  0xFC;
            aby_led_data1[comIndex].abyData[2] |=  (temp&0x3F)<<2;
            aby_led_data1[comIndex].abyData[3] |=  (temp&0xC0)>>6;
        }
        break;
    }
}

void LED_Window_Left(unsigned char by_Address,const char *str)
{
    UCHAR lby_data;
    for(char i = 0;i < 4;i++)
    {
        if(by_Address == 1)
        {
            LED_Light(TIME_COM1,LED_ON);
        }
        else
        {
            LED_Light(TIME_COM1,LED_OFF);
        }
        
        //zsf
        if(*(str+i) == 'E')
            lby_data = 16;
        else if(*(str+i) == 'n')
            lby_data = 17;
        else if(*(str+i) == '-')
            lby_data = 12;
        else if(*(str+i) == 'g')
            lby_data = 23;
        
        else if(*(str+i) != ' ')
            lby_data = *(str+i)-'0';
        else
            lby_data = 10;
        LEDCom(COM1-i,lby_data, 1);
    }
}
void LED_Window_Right(unsigned char by_Address,const char *str)
{
    UCHAR lby_data;
    for(char i = 0;i < 4;i++)
    {
        if(*(str+i) == '-')   //zsf
            lby_data = 12;
        else if(*(str+i) != ' ')
            lby_data = *(str+i)-'0';
        else
            lby_data = 10;

        if(by_Address == 1 && i == 1)
        {
            lby_data |= 0x80;
        }
        
        LEDCom(COM5-i,lby_data, 1);
    }
}

void LED_Window_Middle(unsigned char by_Address,const char *str)
{
    UCHAR lby_data;
    for(char i = 0;i < 3;i++)
    {
        if(*(str+i) >= '0' && *(str+i) <= '9')
            lby_data = *(str+i)-'0';
        else if(*(str+i) == 'L')
            lby_data = 22;
        else if(*(str+i) == '-')   //zsf
            lby_data = 12;
         
        else
            lby_data = 10;

        if(by_Address == 1 && i == 1)
        {
            lby_data |= 0x80;
        }
        
        LEDCom(VCOM1+i,lby_data,1);
    }
}

void LED_String(unsigned char x,unsigned char y,unsigned char Z,const char *str)
{
    if(x == 2 && y == 0)
    {
        LED_Window_Left(Z,str);//Time,Cal
    }
    else if(x == 2 && y == 1)
    {
        LED_Window_Middle(Z,str);//speed,incline
    }
    else if(x == 2 && y == 2)
    {
        LED_Window_Right(Z,str);//distance,hrt
    }
}

void LED_Regist_Functions(void)
{
    Hal_Regist_ISR(MS_ISR,LED_CTRL_ISR_IDX,LED_1ms_ISR);
}



  
void LED_Demo_Processs(void)                          //zsf
{
    static signed char w_DemoData=0;
    static unsigned char by_Dir=0;
    static unsigned char lby_data = 0;
    
    LED_Light(TIME_COM1,LED_OFF);
    
    if(by_Dir==0)
    {
         lby_data += (1<<w_DemoData); 
             
         for(unsigned char i = 0; i < 4; i++)
         { 
             LEDCom(COM1-i, lby_data, 0);
         }
         for(unsigned char i = 0; i < 3; i++)
         { 
             LEDCom(VCOM1+i, lby_data, 0);
         }
         for(unsigned char i = 0; i < 4; i++)
         { 
             LEDCom(COM5-i, lby_data, 0);
         }

        w_DemoData++;
        if(w_DemoData>=7)
        {
            w_DemoData=0;
            by_Dir=1;
        }
    }
    else
    {
         lby_data = 0xFF>>w_DemoData;
         for(unsigned char i = 0; i < 4; i++)
         { 
             LEDCom(COM1-i, lby_data, 0);
         }
         for(unsigned char i = 0; i < 3; i++)
         { 
             LEDCom(VCOM1+i, lby_data, 0);
         }
         for(unsigned char i = 0; i < 4; i++)
         { 
             LEDCom(COM5-i, lby_data, 0);
         }
             
        w_DemoData++;
        if(w_DemoData>8)
        {
            w_DemoData=0;
            by_Dir=0;
        }
    }
}

void LED_Intial_Data(void)
{
    memset(aby_led_data1,0,sizeof(aby_led_data1));
}

void LED_Set_All(char by_Mode)
{
    if(by_Mode)
    {
        memset(aby_led_data1,0xffff,sizeof(aby_led_data1));
    }
    else
    {
        memset(aby_led_data1,0,sizeof(aby_led_data1));
    }
}
//void LED_SET_DOT(char by_Data)
//{
//}

void LED_14_SEG_Dot(signed char by_Address, char by_Onoff)
{
}

#else




enum
{
    Str_0=0,
    Str_1,
    Str_2,
    Str_3,
    Str_4,
    Str_5,
    Str_6,
    Str_7,
    Str_8,
    Str_9,
    Str_DOT,
    Str_DUNHAO,
    Str_BACKSLASH,
    Str_SLASH,
    Str_VERTICAL_LINE,
    Str_PLUS_SIGN,
    Str_MINUS_SIGN,
    Str_A,
    Str_B,
    Str_C,
    Str_D,
    Str_E,
    Str_F,
    Str_G,
    Str_H,
    Str_I,
    Str_J,
    Str_K,
    Str_L,
    Str_M,
    Str_N,
    Str_O,
    Str_P,
    Str_Q,
    Str_R,
    Str_S,
    Str_T,
    Str_U,
    Str_V,
    Str_W,
    Str_X,
    Str_Y,
    Str_Z,
    Str_OFF,
    Str_ALL_ON,
}Seg14Index;

typedef  union
{
    unsigned short _Word;
    struct{
        unsigned char Lbytes;
        unsigned char Hbytes;
    }_Bytes;
    struct{
        unsigned short Bit0:1;
        unsigned short Bit1:1;
        unsigned short Bit2:1;
        unsigned short Bit3:1;
        unsigned short Bit4:1;
        unsigned short Bit5:1;
        unsigned short Bit6:1;
        unsigned short Bit7:1;
        unsigned short Bit8:1;
        unsigned short Bit9:1;
        unsigned short Bit10:1;
        unsigned short Bit11:1;
        unsigned short Bit12:1;
        unsigned short Bit13:1;
        unsigned short Bit14:1;
        unsigned short Bit15:1;
    }_Bits;
}Data_INT16;




const unsigned short aw_14_SEG_Code[]=
{
    0x003F,	//*0*-0	Str_0,
    0x0006,	//*1*-1	Str_1,
    0x045B,	//*2*-2	Str_2,
    0x044F,	//*3*-3	Str_3,
    0x0466,	//*4*-4	Str_4,
    0x046D,	//*5*-5	Str_5,
    0x047D,	//*6*-6	Str_6,
    0x0007,	//*7*-7	Str_7,
    0x047F,	//*8*-8	Str_8,
    0x046F,	//*9*-9	Str_9,
    0x4000,	//*.*-10	Str_DOT,
    0x0800,	//*¡¢*-11	Str_DUNHAO,
    0x2200,	//*/*-12	Str_BACKSLASH,
    0x0880,	//*\*-13	Str_SLASH,
    0x1100,	//*|*-14	Str_VERTICAL_LINE,
    0x1540,	//*+*-15	Str_PLUS_SIGN,
    0x0440,	//*-*-16	Str_MINUS_SIGN,
    0x0477,	//*A*-17	Str_A,
    0x150F,	//*B*-18	Str_B,
    0x0039,	//*C*-19	Str_C,
    0x110F,	//*D*-20	Str_D,
    0x0479,	//*E*-21	Str_E,
    0x0471,	//*F*-22	Str_F,
    0x043D,	//*G*-23	Str_G,
    0x0476,	//*H*-24	Str_H,
    0x1109,	//*I*-25	Str_I,
    0x001E,	//*J*-26	Str_J,
    0x0A70,	//*K*-27	Str_K,
    0x0038,	//*L*-28	Str_L,
    0x02B6,	//*M*-29	Str_M,
    0x08B6,	//*N*-30	Str_N,
    0x003F,	//*O*-31	Str_O,
    0x0473,	//*P*-32	Str_P,
    0x083F,	//*Q*-33	Str_Q,
    0x0C73,	//*R*-34	Str_R,
    0x046D,	//*S*-35	Str_S,
    0x1101,	//*T*-36	Str_T,
    0x003E,	//*U*-37	Str_U,
    0x2230,	//*V*-38	Str_V,
    0x2836,	//*W*-39	Str_W,
    0x2A80,	//*X*-40	Str_X,
    0x1280,	//*Y*-41	Str_Y,
    0x2209,	//*Z*-42	Str_Z,
    0x0000,	//*OFF*-43	Str_OFF,
    0x3FFF,	//*ALL_ON*-44	Str_ALL_ON,
};
static char aby_LED_Data[COM_NUMBER][4];



__no_init static unsigned short aw_14_SEG_String[C_14_SGE_STRING_MAX];
__no_init static unsigned short aw_14_SEG_Program[C_14_SEG_MAX];
__no_init static unsigned short aw_14_SEG_Demo[C_14_SEG_MAX];

__no_init static unsigned char  by_14_SEG_End;
__no_init static unsigned char  by_14_SEG_Index;
__no_init static unsigned char  by_Dot;
__no_init static unsigned short by_14SEG_Move_Speed;
//__no_init static unsigned char  by_14_SEG_Count;
__no_init static char Str_OldBuff[50];

__no_init static
struct {
    unsigned S14_STR_MOVE : 1;
    unsigned S14_PAGE_MOVE : 1;
    unsigned SHOW_MODE: 2;
    unsigned LED1:1;
}Led;
static unsigned char LED_AscciiConvert(unsigned char by_Dat);
void LED_Clear_string(void)
{
    memset(Str_OldBuff,0,sizeof(Str_OldBuff));
}
void LED_Intial_Data(void)
{
    memset(aw_14_SEG_String,0,sizeof(aw_14_SEG_String));
    memset(aw_14_SEG_Program,0,sizeof(aw_14_SEG_Program));
    memset(aw_14_SEG_Demo,0,sizeof(aw_14_SEG_Demo));
    memset(aby_LED_Data,0,sizeof(aby_LED_Data));
}
void LED_Set_All(char by_Mode)
{
    Led.SHOW_MODE = 0;
    if(by_Mode)
    {
        Led.LED1=1;
        memset(aw_14_SEG_String,0xffff,sizeof(aw_14_SEG_String));
        memset(aw_14_SEG_Program,0xffff,sizeof(aw_14_SEG_Program));
        memset(aw_14_SEG_Demo,0xffff,sizeof(aw_14_SEG_Demo));
        memset(aby_LED_Data,0xff,sizeof(aby_LED_Data));
    }
    else
    {
        Led.LED1=0;
        memset(aw_14_SEG_String,0,sizeof(aw_14_SEG_String));
        memset(aw_14_SEG_Program,0,sizeof(aw_14_SEG_Program));
        memset(aw_14_SEG_Demo,0,sizeof(aw_14_SEG_Demo));
        memset(aby_LED_Data,0,sizeof(aby_LED_Data));
    }
}
static unsigned char LED_AscciiConvert(unsigned char by_Dat)
{
    unsigned char by_Out = 0;
    if(by_Dat >= '0' && by_Dat <= '9')
    {
        by_Out = by_Dat-'0';
    }
    else if(by_Dat >= 'A' && by_Dat <= 'Z')
    {
        by_Out = (by_Dat-'A')+Str_A;
    }
    else if(by_Dat >= 'a' && by_Dat <= 'z')
    {
        by_Out = (by_Dat-'a')+Str_A;
    }
    else if(by_Dat == '.')
    {
        by_Out = Str_DOT;
    }
    else if(by_Dat == '/')
    {
        by_Out = Str_BACKSLASH;
    }
    else if(by_Dat == ' ')
    {
        by_Out = Str_OFF;
    }
    else if(by_Dat == '|')
    {
        by_Out = Str_VERTICAL_LINE;
    }
    else if(by_Dat == '+')
    {
        by_Out = Str_PLUS_SIGN;
    }
    else if(by_Dat == '-')
    {
        by_Out = Str_MINUS_SIGN;
    }
    return by_Out;
}
void LED_Flag_10ms_Int(void)
{
    static unsigned short by_14SEG_Move_Time = 0;
    
    if(by_14SEG_Move_Time >= by_14SEG_Move_Speed)
    {
        by_14SEG_Move_Time = 0;
        if(Led.S14_PAGE_MOVE)
        {
            if(by_14_SEG_Index < by_14_SEG_End)
            {
                by_14_SEG_Index += 16;
            }
            else
            {
                by_14_SEG_Index = 0;
//                if(by_14_SEG_Count < C_14_SEG_COUNT_MAX)
//                {
//                    ++by_14_SEG_Count;
//                }
            }
        }
        if(Led.S14_STR_MOVE)
        {
            if(by_14_SEG_Index < by_14_SEG_End)
            {
                ++by_14_SEG_Index;
            }
            else
            {
                by_14_SEG_Index =0;
                
//                if(by_14_SEG_Count < C_14_SEG_COUNT_MAX)
//                {
//                    ++by_14_SEG_Count;
//                }
            }
        }
    }
    else by_14SEG_Move_Time++;
}

void LED_Set_Move_Mode(char by_Mode)
{
    switch(by_Mode)
    {
        case C_14SEG_PAGE:
        by_14_SEG_Index = 0;
        Led.S14_PAGE_MOVE = 1;
        Led.S14_STR_MOVE = 0;
        by_14SEG_Move_Speed = 300;
        break;
        case C_14SEG_STOP:
        by_14_SEG_Index = 0;
        Led.S14_PAGE_MOVE = 0;
        Led.S14_STR_MOVE = 0;
        break;
        case C_14SEG_STR:
        by_14_SEG_Index = 0;
        Led.S14_PAGE_MOVE = 0;
        Led.S14_STR_MOVE = 1;
        by_14SEG_Move_Speed = 25;
        break;
        default:
        break;
    }
}

void LED_14_SEG_Number(char by_Screen,const char *pby_Str,char by_Mode)
{
    char by_Index;
    char by_Out;
    char by_Zero=0;
    unsigned char by_Data;
//    unsigned short w_Buffer;
    
    for(by_Index = 0;by_Index < 4;by_Index++)
    {
        LED_14_SEG_Char(4,Str_OFF);
        LED_14_SEG_Char(8,Str_OFF);
        LED_14_SEG_Char(12,Str_OFF);
        
        if(by_Mode == DN_ALL_OFF)
        {
            switch(by_Screen)
            {
                case 1:
                LED_14_SEG_Char(by_Index,Str_OFF);
                break;
                case 2:
                LED_14_SEG_Char(by_Index+4,Str_OFF);
                break;
                case 3:
                LED_14_SEG_Char(by_Index+8,Str_OFF);
                break;
                case 4:
                LED_14_SEG_Char(by_Index+12,Str_OFF);
                break;
                default :
                break;
            }
            continue;
        }
        if(by_Mode==DN_THREE && by_Index==1)
        {
            by_Zero=1;
        }
        else if(by_Mode==DN_TWO && by_Index==2)
        {
            by_Zero=1;
        }
//        switch(by_Index)
//        {
//            case 0: w_Buffer=1000; break;
//            case 1: w_Buffer=100; break;
//            case 2: w_Buffer=10; break;
//            case 3: w_Buffer=1; break;
//        }
//        by_Out = w_Dat / w_Buffer;
//        w_Dat %= w_Buffer;
        by_Data = *(pby_Str+by_Index);
        if(by_Data >= '0' && by_Data <= '9')
        {
            by_Out = by_Data-'0';
        }
        else if(by_Data == ' ')
        {
            by_Out = 43;//OFF
        }
        
        if(by_Out||by_Zero||by_Index==3)
        {
            by_Zero=1;
            switch(by_Screen)
            {
                case 1:
                LED_14_SEG_Char(by_Index,by_Out);
                break;
                case 2:
                LED_14_SEG_Char(by_Index+4,by_Out);
                break;
                case 3:
                LED_14_SEG_Char(by_Index+8,by_Out);
                break;
                case 4:
                LED_14_SEG_Char(by_Index+12,by_Out);
                break;
                default :
                break;
            }
        }
        else
        {
            switch(by_Screen)
            {
                case 1:
                LED_14_SEG_Char(by_Index,Str_OFF);
                break;
                case 2:
                LED_14_SEG_Char(by_Index+4,Str_OFF);
                break;
                case 3:
                LED_14_SEG_Char(by_Index+8,Str_OFF);
                break;
                case 4:
                LED_14_SEG_Char(by_Index+12,Str_OFF);
                break;
                default :
                break;
            }
        }
    }
}
void LED_14_SEG_Char(signed char by_Address,signed char by_Dat)
{
    unsigned short w_Buffer;
    if(by_Address<0||by_Address>15) return;
    
    w_Buffer = aw_14_SEG_Code[by_Dat];
    aw_14_SEG_Program[by_Address] = w_Buffer;
    Led.SHOW_MODE = 0;
}
void LED_14_SEG_Dot(signed char by_Address, char by_Onoff)
{
    if(by_Address<0||by_Address>15) return;
    if(by_Onoff)
    {
        aw_14_SEG_Program[by_Address] |=0x4000;
    }
    else
    {
        aw_14_SEG_Program[by_Address] &=~0x4000;
    }
}

void LED_14_SEG_String(const char *pby_Str)
{
    char by_Dat=0;
    char by_Adr=0;
    unsigned short w_Buffer;
    char Str_Buff[50];
    char by_Address;
    char i = 0;
    
    by_Address = strlen(pby_Str);
    memset(Str_Buff,0,sizeof(Str_Buff));
    strcpy(Str_Buff,pby_Str);
    
    while(Str_Buff[i] != '\0')
    {
        if(Str_Buff[i] == '.')
            by_Address--;
        i++;
    }
    if(by_Address <= 16)
    {
        memset(Str_OldBuff,0,sizeof(Str_OldBuff));
        by_Address = (16-by_Address)/2;
        LED_Set_Move_Mode(C_14SEG_STOP);
    }
    else
    {
        if(strcmp(Str_Buff,Str_OldBuff) != 0)
        {
            memset(Str_OldBuff,0,sizeof(Str_OldBuff));
            strcpy(Str_OldBuff,Str_Buff);
        }
        else return;
        
        by_Address = 16;
        LED_Set_Move_Mode(C_14SEG_STR);
    }
    memset(aw_14_SEG_String,0,sizeof(aw_14_SEG_String));
    while(*pby_Str != 0)
    {
        by_Dat = *pby_Str++;
        if(by_Dat=='.')
        {
            w_Buffer=0x4000;
            if(by_Address)
                by_Address--;
        }
        else
        {
            by_Adr =  LED_AscciiConvert(by_Dat);
            w_Buffer = aw_14_SEG_Code[by_Adr];
        }
        aw_14_SEG_String[by_Address] |= w_Buffer;
        by_Address++;
    }
    by_14_SEG_Index = 0;
    by_14_SEG_End = by_Address;
    Led.SHOW_MODE = 1;
//    by_14_SEG_Count = 0;
}
void LED_Bit_Exchange(unsigned short *w_Buffer)
{
    Data_INT16 Temp_buff,Backup_buff;
    Backup_buff._Word=*w_Buffer;
    
    Temp_buff._Bits.Bit3=Backup_buff._Bits.Bit0;
    Temp_buff._Bits.Bit4=Backup_buff._Bits.Bit1;
    Temp_buff._Bits.Bit5=Backup_buff._Bits.Bit2;
    Temp_buff._Bits.Bit0=Backup_buff._Bits.Bit3;
    Temp_buff._Bits.Bit1=Backup_buff._Bits.Bit4;
    Temp_buff._Bits.Bit2=Backup_buff._Bits.Bit5;
    Temp_buff._Bits.Bit10=Backup_buff._Bits.Bit6;
    Temp_buff._Bits.Bit11=Backup_buff._Bits.Bit7;
    Temp_buff._Bits.Bit12=Backup_buff._Bits.Bit8;
    Temp_buff._Bits.Bit13=Backup_buff._Bits.Bit9;
    Temp_buff._Bits.Bit6=Backup_buff._Bits.Bit10;
    Temp_buff._Bits.Bit7=Backup_buff._Bits.Bit11;
    Temp_buff._Bits.Bit8=Backup_buff._Bits.Bit12;
    Temp_buff._Bits.Bit9=Backup_buff._Bits.Bit13;
    Temp_buff._Bits.Bit14=Backup_buff._Bits.Bit14;
    Temp_buff._Bits.Bit15=Backup_buff._Bits.Bit15;
    *w_Buffer=Temp_buff._Word;
}
void LED_14_SEG_View(signed char by_Adress,UCHAR w_Data)
{
	char by_Addr,by_Com;
	//Data_INT16 Temp_Data;
	UCHAR tempdata = 0;
	if(by_Adress<0 || by_Adress>24)
		return;
       tempdata = w_Data&0xFF;
	//Temp_Data._Word=w_Data& 0xff;
	//if(by_Adress==2||by_Adress==3||by_Adress==10||by_Adress==11)
	//	LED_Bit_Exchange(&Temp_Data._Word);
//    if(by_Adress==2)
//        by_Adress = 5;
//    else if(by_Adress==3)
//        by_Adress = 4;
//    else if(by_Adress==4||by_Adress==5)
//        by_Adress += 4;
//    else if(by_Adress==6||by_Adress==7)
//        by_Adress += 6;
//    else if(by_Adress==8)
//        by_Adress = 3;
//    else if(by_Adress==9)
//        by_Adress = 2;
//    else if(by_Adress==10||by_Adress==11)
//        by_Adress -= 4;
//    else if(by_Adress==12)
//        by_Adress = 11;
//    else if(by_Adress==13)
//        by_Adress = 10;
	switch(by_Adress)
	{
		case 0:
		by_Addr = 0;
		by_Com=0;
		break;
		case 1:
		by_Addr = 0;
		by_Com=1;
		break;
		case 2:
		by_Addr = 2;
		by_Com=1;
		break;
		case 3:
		by_Addr = 2;
		by_Com=0;
		break;
		case 4:
		by_Addr = 0;
		by_Com=2;
		break;
		case 5:
		by_Addr = 0;
		by_Com=3;
		break;
		case 6:
		by_Addr = 2;
		by_Com=2;
		break;
		case 7:
		by_Addr = 2;
		by_Com=3;
		break;
		case 8:
		by_Addr = 0;
		by_Com=4;
		break;
		case 9:
		by_Addr = 0;
		by_Com=5;
		break;
		case 10:
		by_Addr = 2;
		by_Com=5;
		break;
		case 11:
		by_Addr = 2;
		by_Com=4;
		break;
		case 12:
		by_Addr = 0;
		by_Com=6;
		break;
		case 13:
		by_Addr = 0;
		by_Com=7;
		break;
		case 14:
		by_Addr = 2;
		by_Com=6;
		break;
		case 15:
		by_Addr = 2;
		by_Com=7;
		break;
		default:
		break;
	}
	//if(Led.LED1)
		//Temp_Data._Bytes.Hbytes|=0x80;
	//aby_LED_Data[by_Com][by_Addr+1]=  Temp_Data._Bytes.Hbytes;
	//aby_LED_Data[by_Com][by_Addr]  =  Temp_Data._Bytes.Lbytes;
	aby_LED_Data[by_Com][by_Addr] = tempdata;
}
//void LED_Program_BL(char by_Program,char by_Data)
//{
//	aby_LED_Data[1][4] &= ~0x01;
//    aby_LED_Data[2][4] &= ~0x01;
//	aby_LED_Data[3][4] &= ~0x03;
//    aby_LED_Data[4][4] &= ~0x03;
//	aby_LED_Data[5][4] &= ~0x03;
//    aby_LED_Data[6][4] &= ~0x03;
//    
//	switch(by_Program)
//	{
//		case P_MANUAL:
//        if(by_Data != 0)
//            aby_LED_Data[1][4] |= 0x01;
//        else
//            aby_LED_Data[1][4] &= ~0x01;
//        break;
//		case P_DISTANCE:
//        if(by_Data != 0)
//            aby_LED_Data[6][4] |= 0x01;
//        else
//            aby_LED_Data[6][4] &= ~0x01;
//        break;
//		case P_FAT_BURN:
//        if(by_Data != 0)
//            aby_LED_Data[6][4]|= 0x02;
//        else
//            aby_LED_Data[6][4] &= ~0x02;
//        break;
//		case P_CALORIES:
//        if(by_Data != 0)
//            aby_LED_Data[3][4] |= 0x01;
//        else
//            aby_LED_Data[3][4] &= ~0x01;
//        break;
//		case P_HILL_CLIMB:
//        if(by_Data != 0)
//            aby_LED_Data[3][4] |= 0x02;
//        else
//            aby_LED_Data[3][4] &= ~0x02;
//        break;
//		case P_TGART_HEART_RATE:
//        if(by_Data != 0)
//            aby_LED_Data[4][4] |= 0x01;
//        else
//            aby_LED_Data[4][4] &= ~0x01;
//        break;
//		case P_MY_FIRST_5K:
//        if(by_Data != 0)
//            aby_LED_Data[4][4] |= 0x02;
//        else
//            aby_LED_Data[4][4] &= ~0x02;
//        break;
//		case P_CUSTOM:
//        if(by_Data != 0)
//            aby_LED_Data[5][4] |= 0x02;
//        else
//            aby_LED_Data[5][4] &= ~0x02;
//        break;       
//        case P_CUSTOM_HEART_RATE:
//        if(by_Data != 0)
//            aby_LED_Data[5][4] |= 0x01;
//        else
//            aby_LED_Data[5][4] &= ~0x01;
//        break;
//        case P_SPRINT:
//        if(by_Data != 0)
//            aby_LED_Data[2][4] |= 0x01;
//        else
//            aby_LED_Data[2][4] &= ~0x01;
//        break;
//		default:
//        break;
//	}
//}
//void LED_Clear_ProgramBL(void)
//{
//    aby_LED_Data[0][4] &= ~0x78;
//	aby_LED_Data[5][4] &= ~0x78;
//}

void LED_Program_BL1(const char *pby_Str,char by_Data)
{
    char by_ProgramNum = *pby_Str - '0';
    aby_LED_Data[0][4] &= ~0x78;
	aby_LED_Data[5][4] &= ~0x78;
	switch(by_ProgramNum)
	{
		case P_MANUAL:
        if(by_Data != 0)
            aby_LED_Data[0][4]|=0x08;
        else
            aby_LED_Data[0][4]&=~0x08;
        break;
		case P_DISTANCE:
        if(by_Data != 0)
            aby_LED_Data[0][4]|=0x10;
        else
            aby_LED_Data[0][4]&=~0x10;
        break;
		case P_CALORIES:
        if(by_Data != 0)
            aby_LED_Data[0][4]|=0x20;
        else
            aby_LED_Data[0][4]&=~0x20;
        break;
		case P_FAT_BURN:
        if(by_Data != 0)
            aby_LED_Data[0][4]|=0x40;
        else
            aby_LED_Data[0][4]&=~0x40;
        break;
		case P_HILL_CLIMB:
        if(by_Data != 0)
            aby_LED_Data[5][4]|=0x08;
        else
            aby_LED_Data[5][4]&=~0x08;
        break;
		case P_TGART_HEART_RATE:
        if(by_Data != 0)
            aby_LED_Data[5][4]|=0x10;
        else
            aby_LED_Data[5][4]&=~0x10;
        break;
		case P_MY_FIRST_5K:
        if(by_Data != 0)
            aby_LED_Data[5][4]|=0x20;
        else
            aby_LED_Data[5][4]&=~0x20;
        break;
		case P_CUSTOM:
        if(by_Data != 0)
            aby_LED_Data[5][4]|=0x40;
        else
            aby_LED_Data[5][4]&=~0x40;
        break;
		default:
        break;
	}
}
void LED_Light(char by_Mode,char by_On)
{
    if(by_On)
    {
        switch(by_Mode)
        {
            case DP_LED1:
            Led.LED1=1;
            aby_LED_Data[5][1] |= 0x80;
            break;
            case DP_LED2:
            aby_LED_Data[5][4] |= 0x04;
            break;
            case DP_LED3:
            aby_LED_Data[5][4] |= 0x01;
            break;
            case DP_LED4:
            aby_LED_Data[5][4] |= 0x02;
            break;
            default:
            break;
        }
    }
    else
    {
        switch(by_Mode)
        {
            case DP_LED1:
            Led.LED1=0;
            aby_LED_Data[5][1] &= ~0x80;
            break;
            case DP_LED2:
            aby_LED_Data[5][4] &= ~0x04;
            break;
            case DP_LED3:
            aby_LED_Data[5][4] &= ~0x01;
            break;
            case DP_LED4:
            aby_LED_Data[5][4] &= ~0x02;
            break;
            default:
            break;
        }
    }
}
void LED_Updata(char by_Mode)
{
    char by_Loop;
    UCHAR w_Buffer;
    if(by_Mode)
    {
        for(by_Loop=0; by_Loop < C_14_SEG_MAX;by_Loop++)
        {
            if(Led.SHOW_MODE==0)
            {
                w_Buffer = aby_Number_Code[by_Loop];
                LED_14_SEG_View(by_Loop,w_Buffer);
            }
            else if(Led.SHOW_MODE==1)
            {
                if((by_14_SEG_Index + by_Loop) < by_14_SEG_End)
                {
                    w_Buffer = aby_Number_Code[by_14_SEG_Index + by_Loop];
                }
                else
                {
                    w_Buffer = 0;
                }
                LED_14_SEG_View(by_Loop,w_Buffer);
            }
            else if(Led.SHOW_MODE==2)
            {
                w_Buffer = aby_Number_Code[by_Loop];
                LED_14_SEG_View(by_Loop,w_Buffer);
            }
        }
    }
}
void LED_1ms_ISR(void)
{
    static UCHAR sComIndex = 0;
    static char by_Count;
    aby_LED_Data[sComIndex][3] &=0x1F;
    aby_LED_Data[sComIndex][3] |=(sComIndex<<5);
    TLC59282_Process_SPI(aby_LED_Data[sComIndex],4);
    sComIndex++;
    sComIndex = sComIndex%COM_NUMBER;
    by_Count++;
    if(by_Count >= 10)//10ms
    {
        by_Count = 0;
        LED_Flag_10ms_Int();
    }
    /*
    static union
    {
        struct
        {
            unsigned char Bit0_4:5;
            unsigned char Bit5_7:3;
        }Bits;
        unsigned char Bytes;
    }Scan={0};
    Scan.Bits.Bit0_4=aby_LED_Data[Scan.Bits.Bit5_7][5];
    memset(aby_LED_Data[0],'0',11);
    aby_LED_Data[Scan.Bits.Bit5_7][5]=(~Scan.Bits.Bit5_7)<<5|Scan.Bits.Bit0_4;
    TLC59282_Process_SPI(aby_LED_Data[0],4);
    Scan.Bits.Bit5_7++;
    Scan.Bits.Bit5_7 %= COM_NUMBER;
    */
}
void LED_Demo_Processs(void)
{
    static signed char w_DemoData=0;
    static unsigned char by_Dir=0;
    Led.SHOW_MODE=2;
    if(by_Dir==0)
    {
        for(char by_Loop=0; by_Loop < C_14_SEG_MAX;by_Loop++)
        {
            aw_14_SEG_Demo[by_Loop]|=1<<w_DemoData;
        }
        w_DemoData++;
        if(w_DemoData>=15)
        {
            w_DemoData=0;
            by_Dir=1;
        }
    }
    else
    {
        for(char by_Loop=0; by_Loop < C_14_SEG_MAX;by_Loop++)
        {
            aw_14_SEG_Demo[by_Loop]=0xFFFF>>w_DemoData;
        }
        w_DemoData++;
        if(w_DemoData>16)
        {
            w_DemoData=0;
            by_Dir=0;
        }
    }
}
//void LED_SET_DOT(char by_Data)
//{
//    by_Dot = by_Data;
//}
void LED_Regist_Functions(void)
{
    Hal_Regist_ISR(MS_ISR,LED_CTRL_ISR_IDX,LED_1ms_ISR);
}

void LED_String(unsigned char x,unsigned char y,unsigned char Z,const char *str)
{
    if(x==1&&y==1)//string
    {       
        LED_14_SEG_String(str);
    }
    else if(x==2&&y==0)//time/count
    {
        LED_14_SEG_Number(1,str,DN_THREE);
        if(by_Dot != 0)
        {
            LED_14_SEG_Dot(1,1);
            LED_14_SEG_Dot(2,1);
        }
        else
        {
            LED_14_SEG_Dot(1,0);
            LED_14_SEG_Dot(2,0);
        }
    }
    else if(x==2&&y==1)//speed/incline
    {
        LED_14_SEG_Number(2,str,DN_TWO);
    }
    else if(x==2&&y==2)//distace/pace
    {
        LED_14_SEG_Number(3,str,DN_THREE);
        if(by_Dot == 0)
        {
            LED_14_SEG_Dot(9,0);
            LED_14_SEG_Dot(10,0);
        }
        if(by_Dot == 1)
        {
            LED_14_SEG_Dot(10,1);
        }
        else if(by_Dot == 2)
        {
            LED_14_SEG_Dot(9,1);
        }
    }
    else if(x==2&&y==3)//calories/heart
    {        
        LED_14_SEG_Number(4,str,DN_NORMAL);
//        LED_14_SEG_Dot(15,0);
    }
    else if(x==2&&y==4)
    {
        LED_Program_BL1(str,Z);
    }
}
#endif
