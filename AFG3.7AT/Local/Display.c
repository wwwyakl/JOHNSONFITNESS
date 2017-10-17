#include "HT1621.h"
#include "Display.h"
#include "LED.h"
#include "stdio.h"
#include "string.h"
//#include "Information.h"
#include "Global_Config.h"
#include "Events_Handler.h"
extern FRAME_STAGE Frame_Stage;
#define C_1622_COUNT    		0
#define C_1625_COUNT    		1
#define LCD_BUF_LEN     		(32*C_1622_COUNT+64*C_1625_COUNT)

#define C_DOTMAX_X_MAX  		10
#define LED_BUF_LEN     		14
#define LED_COM         		8

#define C_STRING_MAX  			400


#define C_EIGHT_MAX         	14
#define C_LCD_MAX           	16
#define C_CAPTION_MAX         	39
#define C_LED_MEM_MAX        	48
#define C_LCD_MEM_MAX        	32

#define DEMO_WIN_COUNT  		64*2
#define DEMO_WIN3_COUNT 		48*2
#define LCD_14SEG_LENGTH   	    13
#define SEG14_STR_STARTPOSITION_DIFF 	1
#define SEG14_STR_STARTPOSITION 	( LCD_14SEG_LENGTH - SEG14_STR_STARTPOSITION_DIFF)

__no_init static char aby_LCD_Seg14_String[50];//max 45-byte char
__no_init static unsigned int by_seg14_Move_Point;//Point of start
__no_init static unsigned int by_seg14_Moved_Lap_count;//Ok count
__no_init static bool by_14SEG_Move_Mode;
__no_init static bool by_14SEG_Move_Mode_Refresh;


//static char aby_LED_Data_Out[LED_COM][LED_BUF_LEN]={0};
//static char aby_LED_Buffer[LED_COM][LED_BUF_LEN]={0};

//__no_init static char by_Dotmax_Blink;
//__no_init static unsigned int aw_Dotmax_Program[2][C_DOTMAX_X_MAX];//add by ckm 110510
__no_init static char aby_LCD_Buffer[LCD_BUF_LEN]; //1625
//__no_init static unsigned int aby_String_Demo_Top[C_STRING_MAX];
//__no_init static unsigned int  by_Demo_End_Top;
//__no_init static unsigned int by_Demo_OK_Count_Top;
//__no_init static unsigned int by_Demo_Index_Top;
//__no_init static unsigned char aby_Circle[8];
//__no_init static unsigned int aby_Test_Buffer[64];
unsigned int gLapCircle;
//__no_init static char by_LCD_Move_Speed_Top;
//__no_init static char by_LCD_Move_Speed_Mid;

//__no_init static char by_LED_Blink_Incline;
//__no_init static char by_LED_Blink_Speed;
//__no_init static char by_Blink_Time;
//static char sprintfBuf[16];

#define D_SMALL_LCD     1
#define D_BIG_LCD       2
#define D_DEFAULT_MODE  D_SMALL_LCD

#if D_DEFAULT_MODE == D_SMALL_LCD
__no_init static union
{
//    struct
//    {
//        //---------------------------------------HT1622A   32BYTE----------------------------------------------
//        struct {
//            unsigned  	 Bit7        :1;//NONE
//            unsigned  	 Bit1_6     :6;
//            unsigned  	 Bit0        :1;
//        }Matrix_Up[10];							
//        struct {
//            unsigned  	 Bit7        :1;//NONE
//            unsigned  	 Bit1_6     :6;
//            unsigned  	 Bit0        :1;
//        }Matrix_Down[10];	
//        struct {
//            unsigned  	 Bit7        :1;//NONE
//            unsigned  	 Bit3_6      :4;
//            unsigned  	 Bit2        :1;
//            unsigned  	 Bit1        :1;
//            unsigned  	 Bit0        :1;
//        }Incline[4];	
//        struct {
//            unsigned  	 Bit7        :1;//NONE
//            unsigned  	 Bit3_6      :4;
//            unsigned  	 Bit2        :1;
//            unsigned  	 Bit1        :1;
//            unsigned  	 Bit0        :1;
//        }Laps[4];		
//        unsigned 	 	NULL_BufferB[4];//       	     		10  +  10   +  4  +  4  +  4 = 32
//        //-------------------------------------HT1622B1   32BYTE---------------------------------------
//        struct {
//            unsigned  	 Bit7        :1;
//            unsigned  	 Bit0_6      :7;	
//        }Left[8*2];      	  		
//        struct {
//            unsigned  	 Bit7        :1;
//            unsigned  	 Bit0_6      :7;
//        }Watts[4];												
//        struct {
//            unsigned  	 Bit7        :1;
//            unsigned  	 Bit0_6      :7;
//        }Calories[4];						
//        unsigned 	 	NULL_BufferA[8];//  				     		16  +  4   +  4  +  8 = 32
//        //----------------------------------------HT1622B2   32BYTE------------------------------------------
//        struct {
//            unsigned  	 Bit7        :1;
//            unsigned  	 Bit0_6      :7;	
//        }Right[8*2];							              	  		
//        struct {
//            unsigned  	 Bit7        :1;
//            unsigned  	 Bit0_6      :7;
//        }Pace[4];							 			 	 					
//        struct {
//            unsigned  	 Bit7        :1;
//            unsigned  	 Bit0_6      :7;
//        }Heart_Rate[4];								
//        unsigned 	 NULL_BufferC[8];//				        16  +  4 +  4  +  8 = 32BYTE
//        //----------------------------------------HT1625C   64BYTE----------------------------------------
//        struct {
//            unsigned  	 Bit7        :1;
//            unsigned  	 Bit0_6      :7;
//        }Bottom[30*2];						// 37 38 -  - -65 66	60BYTE	
//        unsigned   	 	NULL_BufferD[4];		//					60BYTE  +  4BYTE  = 64
//        
//    }HT;
    struct
    {
        //---------------------------------------HT1622A   32BYTE----------------------------------------------
        struct {
            unsigned  	 Bit0_7        :8;//NONE
        }Matrix_Up_1[2];							
        struct {
            unsigned  	 Bit4_7      :4;
            unsigned  	 Bit3        :1;
            unsigned  	 Bit2        :1;
            unsigned  	 Bit1        :1;
            unsigned  	 Bit0        :1;
        }Incline[4];	
        struct {
            unsigned  	 Bit7      :1;
            unsigned  	 Bit6      :1;
            unsigned  	 Bit5      :1;
            unsigned  	 Bit4      :1;
            unsigned  	 Bit3      :1;
            unsigned  	 Bit2      :1;
            unsigned  	 Bit1      :1;
            unsigned  	 Bit0      :1;
        }Lap_X_1[1];	
        struct {
            unsigned  	 Bit0_7     :8;
        }Matrix_Up_2[8];
        struct {
            unsigned  	 Bit7      :1;
            unsigned  	 Bit6      :1;
            unsigned  	 Bit5      :1;
            unsigned  	 Bit4      :1;
            unsigned  	 Bit3      :1;
            unsigned  	 Bit2      :1;
            unsigned  	 Bit1      :1;
            unsigned  	 Bit0      :1;
        }Lap_X_2[2];
        struct {
            unsigned  	 Bit0_7     :8;
        }Matrix_Up_3[8];
        struct {
            unsigned  	 Bit7      :1;
            unsigned  	 Bit6      :1;
            unsigned  	 Bit5      :1;
            unsigned  	 Bit4      :1;
            unsigned  	 Bit3      :1;
            unsigned  	 Bit2      :1;
            unsigned  	 Bit1      :1;
            unsigned  	 Bit0      :1;
        }Lap_X_3[1];
        struct {
            unsigned  	 Bit4_7      :4;
            unsigned  	 Bit3        :1;
            unsigned  	 Bit2        :1;
            unsigned  	 Bit1        :1;
            unsigned  	 Bit0        :1;
        }Speed[4];
        struct {
            unsigned  	 Bit0_7     :8;
        }Matrix_Up_4[2];
        struct {
                        unsigned  	 Bit7        :1;
            unsigned  	 Bit1_6     :7;

        }HeartRt[3];
        struct {
                       
            unsigned  	 Bit1_7     :7;
             unsigned  	 Bit0        :1;

        }LED14Segment_1[22];
        struct {
            unsigned  	 Bit7      :1;
            unsigned  	 Bit6      :1;
            unsigned  	 Bit5      :1;
            unsigned  	 Bit4      :1;
            unsigned  	 Bit3      :1;
            unsigned  	 Bit2      :1;
            unsigned  	 Bit1      :1;
            unsigned  	 Bit0      :1;
        }Lap_X_4[1];
        struct {
                       
            unsigned  	 Bit1_7     :7;
            unsigned  	 Bit0        :1;

        }LED14Segment_2[4];
        struct {
            unsigned  	 Bit7      :1;
            unsigned  	 Bit6      :1;
            unsigned  	 Bit5      :1;
            unsigned  	 Bit4      :1;
            unsigned  	 Bit3      :1;
            unsigned  	 Bit2      :1;
            unsigned  	 Bit1      :1;
            unsigned  	 Bit0      :1;
        }Lap_X_5[1];
        struct {
            unsigned  	 Bit2_7        :6;
            unsigned  	 Bit1        :1;
            unsigned  	 Bit0        :1;
        }Fan[1];
    }HT;
    unsigned  		LcdTotal_Buffer[64];         		//TOTAL BUFFER   160
    
}LCD;

#elif D_DEFAULT_MODE == D_BIG_LCD
__no_init static union
{
    struct
    {
        //---------------------------------------HT1622A   32BYTE----------------------------------------------
        struct {
            unsigned  	 Bit7        :1;
            unsigned  	 Bit1_6     :6;
            unsigned  	 Bit0        :1;
        }Matrix_Up[10];							
        struct {
            unsigned  	 Bit7        :1;
            unsigned  	 Bit1_6     :6;
            unsigned  	 Bit0        :1;
        }Matrix_Down[10];	
        struct {
            unsigned  	 Bit7        :1;
            unsigned  	 Bit6        :1;
            unsigned  	 Bit5        :1;
            unsigned  	 Bit4        :1;
            unsigned  	 Bit3        :1;
            unsigned  	 Bit2        :1;
            unsigned  	 Bit1        :1;
            unsigned  	 Bit0        :1;
        }Heart_100M[2];	
        unsigned 	 	NULL_BufferB[10];//       	     		10BYTE  +  10BYTE  +  12BYTE
        //-------------------------------------HT1622B1   32BYTE---------------------------------------
        struct {
            unsigned  	 Bit7        :1;
            unsigned  	 Bit0_6      :7;	
        }Left[8*2];						         	  		
        struct {
            unsigned  	 Bit7        :1;
            unsigned  	 Bit0_6      :7;
        }Watts[4];							
        struct {
            unsigned  	 Bit7        :1;
            unsigned  	 Bit0_6      :7;
        }Incline[2];							
        struct {
            unsigned  	 Bit7        :1;
            unsigned  	 Bit0_6      :7;
        }Calories[4];						
        unsigned 	 	NULL_BufferA[6];//  				     		16  +  4  +  2  +  4  +  6 = 32
        //----------------------------------------HT1622B2   32BYTE------------------------------------------
        struct {
            unsigned  	 Bit7        :1;
            unsigned  	 Bit0_6      :7;	
        }Right[8*2];							              	  		
        struct {
            unsigned  	 Bit7        :1;
            unsigned  	 Bit0_6      :7;
        }Pace[4];							 			
        struct {
            unsigned  	 Bit7        :1;
            unsigned  	 Bit0_6      :7;
        }Laps[2];				 					
        struct {
            unsigned  	 Bit7        :1;
            unsigned  	 Bit0_6      :7;
        }Heart_Rate[4];								
        unsigned 	 NULL_BufferC[6];//				        16  +  4  +  2 +  4  +  6 = 32BYTE
        /*unsigned 		 DP10           			:1;
        unsigned 		 DP11           			:1;
        unsigned 		 DP12           			:1;
        unsigned 		 DP13           			:1;
        unsigned 		 COL3           :1;					
        unsigned 		 DP14           :1;
        unsigned 		 DP15           :1;
        unsigned 		 DP16           :1;
        unsigned 		 DP17           			:1;
        unsigned 		 COL4          			:1;
        unsigned 		 BC2           			:1; 	
        unsigned 		 Heart          			:1;
        unsigned 		 BitC1           	:1;
        unsigned 		 BitC2         	:1;					
        unsigned 		 BitC3           	:1;
        unsigned 		 BitC4           	:1;*/
        //----------------------------------------HT1625C   64BYTE----------------------------------------
        struct {
            unsigned  	 Bit7        :1;
            unsigned  	 Bit0_6      :7;
        }Bottom[30*2];						// 37 38 -  - -65 66	60BYTE	
        unsigned   	 	NULL_BufferD[4];		//					60BYTE  +  4BYTE  = 64
        
    }HT;
    
    unsigned  		LcdTotal_Buffer[64];         		//TOTAL BUFFER   160
    
}LCD;

#endif


const char nAscii7Seg[24]=
{
    0x77,// 0
    0x24,// 1
    0x5d,// 2
    0x6d,// 3
    0x2e,// 4
    0x6b,// 5
    0x7b,// 6
    0x25,// 7
    0x7f,// 8
    0x6f,// 9
    
    0x00,//OFF
    0x7F,//ON
    
};
const char nAsciiFan[24]=
{
    0x09,// T13
    0x18,// T14
    0x28,// T15
    0x0C,// T16
    0x0A,// T17
    0x08,// T18
    
    0x00,//OFF
    0x3F,//ON
    
};
const char nAscii7Seg2[24][2]=
{
    {0x05,0x0F,}, // -0-48
    {0x00,0x06,},  // -1-
    {0x06,0x0B,},  // -2-
    {0x02,0x0f,},  // -3-
    {0x03,0x06,},  // -4-
    {0x03,0x0d,},  // -5-
    {0x07,0x0d,},  // -6-
    {0x00,0x07,},  // -7-
    {0x07,0x0F,},  // -8-
    {0x03,0x0F,},   // -9-
    
    {0x00,0x00,}, // -OFF-
    {0x07,0x0F,}, // -ON-
    
};

const char nAscii14Seg[100][2] =              // ASCII
{
//    {0x7f,0x7f,}, // - -32
//    
//    {0x00,0x00,}, // -!-
//    
//    {0x00,0x00,}, // -"-
//    
//    {0x00,0x00,}, // -#-
//    
//    {0x00,0x00,}, // -$-
//    
//    {0x00,0x00,}, // -%-
//    
//    {0x00,0x00,}, // -&-
//    
//    {0x00,0x00,}, // -'-
//    
//    {0x00,0x00,}, // -(-
//    
//    {0x00,0x00,}, // -)-
//    
//    {0x00,0x00,}, // -*-
//    
//    {0x00,0x00,}, // -+-
//    
//    {0x00,0x00,}, // -,-
//    
//    {0x10,0x04,}, // ---
//    
//    {0x00,0x02,}, // -.-	
//    
//    {0x04,0x10,}, // -/-
//    
//    
    {0x63,0x41,}, // -0-48
    
    {0x00,0x41,},  // -1-
    
    {0x69,0x09,},  // -2-
    
    {0x49,0x49,},  // -3-
    
    {0x0a,0x49,},  // -4-
    
    {0x4b,0x48,},  // -5-
    
    {0x6b,0x48,},  // -6-
    
    {0x01,0x41,},  // -7-
    
    {0x6b,0x49,},  // -8-
    
    {0x4b,0x49,},   // -9-
    
    
    {0x00,0x00,}, // -:-
    
    {0x00,0x00,}, // -;-
    
    {0x00,0x00,}, // -<-
    
    {0x00,0x00,}, // -=-
    
    {0x00,0x00,}, // ->-
    
    {0x00,0x00,}, // -?-
    
    {0x00,0x00,}, // -@-
    
    
    {0x2B,0x49,}, // -A-65
    
    {0x41,0x6D,}, // -B-
    
    {0x63,0x00,}, // -C-
    
    {0x41,0x65,},// -D-
    
    {0x6B,0x08,}, // -E-
    
    {0x2B,0x08,}, // -F-70
    
    {0x63,0x48,}, // -G-
    
    {0x2A,0x49,}, // -H-
    
    {0x41,0x24,}, // -I-
    
    {0x60,0x41,}, // -J-
    
    {0x2A,0x12,}, // -K-75
    
    {0x62,0x00,}, // -L-
    
    {0x26,0x43,}, // -M-
    
    {0x26,0x51,}, // -N-
    
    {0x63,0x41,}, // -O-
    
    {0x2B,0x09,}, // -P-80
    
    {0x63,0x51,}, // -Q-
    
    {0x2B,0x19,}, // -R-?
    
    {0x4B,0x48,}, // -S-
    
    {0x01,0x24,}, // -T-
    
    {0x62,0x41,}, // -U-85
    
    {0x32,0x02,}, // -V-
    
    {0x32,0x51,}, // -W-
    
    {0x14,0x12,}, // -X-
    
    {0x04,0x22,}, // -Y-
    
    {0x51,0x02,}, // -Z-90
    
    
    {0x00,0x00,}, // -[-
    
    {0x00,0x00,}, // -\-
    
    {0x00,0x00,}, // -]-
    
    {0x00,0x00,}, // -^-
    
 //   {0x00,0x00,}, // -_-
    {0x40,0x00,}, // -_-   
    
    {0x00,0x00,}, // -`-   
};

const char nAsciiDotMax2[20][4] =              // DotMax
{
    {0x00,0x00,0x00,0x00}, // OFF
    
    {0x00,0x00,0x00,0x00}, // 1
    
    {0x00,0x00,0x00,0x00}, // 2
    
    {0x80,0x00,0x80,0x00}, // 3
    
    {0xC0,0x00,0xc0,0x00}, // 4
    
    {0xE0,0x00,0xe0,0x00}, // 5
    
    {0xF0,0x00,0xf0,0x00}, // 6
    
    {0xF8,0x00,0xf8,0x00}, // 7
    
    {0xFC,0x00,0x00,0x00}, // 8
    
    {0xFE,0x00,0x00,0x00}, // 9
    
    {0xFF,0x00,0x00,0x00}, // 10
    
    {0x00,0x00,0x00,0x00}, // 11
    
    {0x3F,0x3F,0x00,0x00}, // 12
    
    {0x00,0x00,0x00,0x00}, //
    
    {0x00,0x00,0x00,0x00} //
};


//const char aby_Number_Code[24]=
//{
//    // 0    1     2       3      4      5      6       7     8     9
//    0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,
//    //OFF  ON    -    t     o      p       e      n        d     -
//    0x00,0xFF,0x40,0x1E,0x3A,0x73,0x79,0x54,0x56,0x02,
//    //f       i       l      g
//    0x8E,0x20,0x38,0x6F
//};


//__no_init static char by_Move_Speed_Top;
__no_init static
struct
{
    unsigned STR_MOVE_Top:1;
    unsigned PAGE_MOVE_Top:1;
    
    unsigned STR_MOVE_Mid:1;
    unsigned PAGE_MOVE_Mid:1;
    
    unsigned String_Flag : 1;
    unsigned String_OK : 1;
    unsigned String_Loop: 1;
    unsigned String_Start: 1;
    unsigned M100MS : 1;
    unsigned UPDATA : 1;
    unsigned BLINK : 1;
    unsigned TIME_SEC: 1;
    
    unsigned BLINK_INCLINE : 1;
    unsigned BLINK_CALORIES : 1;
    unsigned BLINK_WATTS : 1;
    unsigned BLINK_DISTANCE : 1;
    unsigned BLINK_PACE : 1;
    unsigned BLINK_HEART : 1;
    unsigned BLINK_SPEED : 1;
    unsigned BLINK_BOTTOM:1;
    
    unsigned DEMO_Top:1;
    unsigned DEMO_Mid:1;
    
    unsigned DEMO_ON:1;
    unsigned DEMO_100MS:1;
    unsigned DEMO_STATE:1;
    unsigned DEMO_14SEG:1;
    unsigned DEMO_LCD:1;
    unsigned DEMO_Tip:1;//for example Pace,Time etc
    
    unsigned LCD_Update:1;
    
    unsigned Matrix_blink: 1;
    unsigned PROGRAM_MODE : 1;
}b;
void Display_Initial_Data(void)
{
	by_14SEG_Move_Mode = 0;
	by_seg14_Move_Point = 0;
	by_seg14_Moved_Lap_count = 0;
	by_14SEG_Move_Mode_Refresh = 0 ;

//    char by_Loop, by_Loop1,by_Loop2;
//    unsigned int by_Dotmax_Loop = 0;
    
    ////////add for show matrix flash/////07_10_29
//    b.Matrix_blink=1;
    
//    by_LED_Blink_Incline = by_LED_Blink_Incline;
//    by_LED_Blink_Speed = by_LED_Blink_Speed;
//    by_Demo_Index_Top = 0;
//    by_Demo_End_Top = 0;
//    
//    b.STR_MOVE_Top = 0;
//    b.PAGE_MOVE_Top = 0;
//    b.STR_MOVE_Mid = 0;
//    b.PAGE_MOVE_Mid = 0;
//    by_Move_Speed_Top = 0;
//    //by_Move_Speed_Mid = 0;
//    by_LCD_Move_Speed_Top = 0;
//    by_LCD_Move_Speed_Mid = 0;
//    by_Demo_OK_Count_Top = 0;
//    //  by_Demo_OK_Count_Mid = 0;
//    
//    b.DEMO_Top = 0;
//    b.DEMO_Mid = 0;
//    
//    by_Blink_Time = 0;
//    
//    b.DEMO_ON = 0;
//    b.DEMO_LCD = 0;
//    b.DEMO_100MS = 0;
//    b.DEMO_14SEG = 0;
//    b.DEMO_Tip = 0;
//    
//    b.BLINK_INCLINE = 0;
//    b.BLINK_CALORIES = 0;
//    b.BLINK_WATTS = 0;
//    b.BLINK_DISTANCE = 0;
//    b.BLINK_PACE = 0;
//    b.BLINK_SPEED = 0;
//    b.BLINK_BOTTOM = 0;
//    
//    b.LCD_Update = 1;
//    b.PROGRAM_MODE = 0;
//    ///////////   incline    speed  ///////////////////////
//    for(by_Loop = 0;by_Loop < C_DOTMAX_X_MAX;by_Loop++)
//    {
//        aw_Dotmax_Program[0][by_Loop] = 0;
//        aw_Dotmax_Program[1][by_Loop] = 0;       
//    }
    
//    for(by_Loop1 = 0;by_Loop1 < 8;by_Loop1++)
//    {
//        for(by_Loop2 = 0;by_Loop2 < LED_BUF_LEN;by_Loop2++)
//        {
//            aby_LED_Data_Out[by_Loop1][by_Loop2] = aby_LED_Buffer[by_Loop1][by_Loop2] = 0;
//        }
//    }
    
//    for(by_Dotmax_Loop = 0;by_Dotmax_Loop < C_STRING_MAX;by_Dotmax_Loop++)
//    {aby_String_Demo_Top[by_Dotmax_Loop] = 0;}
//    memset(aby_Test_Buffer,0xFFFF,sizeof(aby_Test_Buffer));

    /****************************************************/
    for(int by_Dat = 0;by_Dat < LCD_BUF_LEN;by_Dat++)
    {
        aby_LCD_Buffer[by_Dat] = 0;
        LCD.LcdTotal_Buffer[by_Dat]= 0;
    }
    HAL_HT162x_Initial_IO();
    LED_Intial_Data();
}

//void Display_Flag_100ms_Int(void)
//{
//    static char aby_Time = 0;
//    static char by_Delay_Top=0;
//    static char by_Delay_Mid=0;
//    static char by_LCD_Update=0;
//    b.M100MS = 1;
//    b.DEMO_100MS = 1;
//    if(by_Delay_Top > by_LCD_Move_Speed_Top)
//    {
//        by_Delay_Top=0;
//        b.String_Flag=1;
//    }
//    else
//    {
//        ++by_Delay_Top;
//    }
//    if(by_Delay_Mid > by_LCD_Move_Speed_Mid)
//    {
//        by_Delay_Mid=0;
//        b.String_Flag=1;
//    }
//    else
//    {
//        ++by_Delay_Mid;
//    }
//    if(by_Blink_Time >= 5)//300ms
//    {
//        by_Blink_Time = 0;
//        b.BLINK = ~b.BLINK;
//    }
//    else ++by_Blink_Time;
//    
//    if(aby_Time > 5)//500ms
//    {
//        b.TIME_SEC = ~b.TIME_SEC;
//        aby_Time = 0;
//    }
//    else ++aby_Time;
//    if(by_LCD_Update > 5)
//    {
//        b.LCD_Update = 1;
//        by_LCD_Update = 0;
//    }
//    else ++by_LCD_Update;
//}

//void Display_Set_Blink_Counter(unsigned char by_Data)
//{
//    by_Blink_Time = by_Data;
//    b.BLINK = 0;
//}
//void Display_Set_Blink(char by_Dat)
//{
//  if(by_Dat == DB_ALL)
//  {
//    b.BLINK_INCLINE = 1;
//    b.BLINK_CALORIES = 1;
//    b.BLINK_WATTS = 1;
//    b.BLINK_DISTANCE = 1;
//    b.BLINK_PACE = 1;
////    b.BLINK_PULSE = 1;
//    b.BLINK_SPEED = 1;
//    b.BLINK_BOTTOM = 1;
//  }
//  if(by_Dat == DB_INCLINE)   b.BLINK_INCLINE = 1;
//  if(by_Dat == DB_CALORIES)  b.BLINK_CALORIES = 1;
//  if(by_Dat == DB_WATTS)      b.BLINK_WATTS = 1;
////  if(by_Dat == DB_DISTANCE)  b.BLINK_DISTANCE = 1;
//  if(by_Dat == DB_PACE)      b.BLINK_PACE = 1;
////  if(by_Dat == DB_PULSE)     b.BLINK_PULSE = 1;
//  if(by_Dat == DB_SPEED)     b.BLINK_SPEED = 1;
//  if(by_Dat == DB_BOTTOM)    b.BLINK_BOTTOM = 1;
//}

//void Display_Clear_Blink(char by_Dat)
//{
//  if(by_Dat == DB_ALL)
//  {
//    b.BLINK_INCLINE = 0;
//    b.BLINK_CALORIES = 0;
//    b.BLINK_WATTS = 0;
//    b.BLINK_DISTANCE = 0;
//    b.BLINK_PACE = 0;
////    b.BLINK_PULSE = 0;
//    b.BLINK_SPEED = 0;
//    b.BLINK_BOTTOM = 0;
//  }
//  if(by_Dat == DB_INCLINE)   b.BLINK_INCLINE = 0;
//  if(by_Dat == DB_CALORIES)  b.BLINK_CALORIES = 0;
//  if(by_Dat == DB_WATTS)      b.BLINK_WATTS = 0;
////  if(by_Dat == DB_DISTANCE)  b.BLINK_DISTANCE = 0;
//  if(by_Dat == DB_PACE)      b.BLINK_PACE = 0;
////  if(by_Dat == DB_PULSE)     b.BLINK_PULSE = 0;
////  if(by_Dat == DB_SPEED)     b.BLINK_SPEED = 0;
//  if(by_Dat == DB_BOTTOM)    b.BLINK_BOTTOM = 0;
//}
//
//char Display_Get_Blink(char by_Dat)
//{
//  if(by_Dat == DB_INCLINE)          return(b.BLINK_INCLINE&b.BLINK);
//  else if(by_Dat == DB_HEART)    	return(b.BLINK_HEART&b.BLINK);
//  else if(by_Dat == DB_WATTS)       return(b.BLINK_WATTS&b.BLINK);
////  else if(by_Dat == DB_PACE)    return(b.BLINK_DISTANCE&b.BLINK);
//  else if(by_Dat == DB_PACE)        return(b.BLINK_PACE&b.BLINK);
////  else if(by_Dat == DB_PULSE)       return(b.BLINK_PULSE&b.BLINK);
//  else if(by_Dat == DB_CALORIES)    return(b.BLINK_CALORIES&b.BLINK);
//  else                              return(b.BLINK_BOTTOM&b.BLINK);
//}
void Display_Set_All(char by_Dat)		//by ckm
{
    if(by_Dat)
    {
        memset(LCD.LcdTotal_Buffer,0xFF,sizeof(LCD.LcdTotal_Buffer));
    }
    else
    {
        memset(LCD.LcdTotal_Buffer,0x00,sizeof(LCD.LcdTotal_Buffer));
    }
}

void Display_Updata_Data(char by_Mode)
{
    HAL_HT162x_Initial_IO();
	
	if(by_14SEG_Move_Mode && by_14SEG_Move_Mode_Refresh == 1)
    {
       char by_String_Loop = 3;
       char by_Count=0;
	   char *bys_String_Loop=&by_String_Loop;
	   char *bys_Count=&by_Count;
        by_14SEG_Move_Mode_Refresh = 0;
        for(int i=0;i<LCD_14SEG_LENGTH;i++)
        {
            LCD_Seg14_Char(bys_Count,bys_String_Loop,&aby_LCD_Seg14_String[by_seg14_Move_Point+i]/*(p_Str+i)*/);
        }
    }
    //show LED
    extern UINT8 appStart;    //zsf
    extern UINT16 appProId;    //zsf
    
    if ((Frame_Stage == FRAME_STAGE_PRGSETUP) || (Frame_Stage == FRAME_STAGE_PRGRUN))
    {
        if (appStart == 0)
        {
            Show_ProgramLED(Program_Get_ProgramID());    //zsf
        }
        else
        {
            if (appProId == 0)
                Show_ProgramLED(appProId);    //zsf
            else
                Show_ProgramLED(10);    //zsf
        }
    }
    
    if (Fan_Get_Level())//ZSF
    {
        Show_LEDLight(LED11_FAN,LED_ON);    
    }
    Display_FanIcon(Fan_Get_Level());
    for( unsigned int i = 0;i < LCD_BUF_LEN;i++) 
        aby_LCD_Buffer[i] =  LCD.LcdTotal_Buffer[i];
    HAL_HT162x_Initial_IO();
    HT1621_Updata_Data(&aby_LCD_Buffer[0],1);// ht1622  ht1625 ht1625
}
////////////////////////////////////////////////////////////////////////////
//void Display_Time(char by_Address,const char *pby_Str)// left up
//{
//  char by_Out;
//  char by_String_Loop =0;
//  char by_Str_Index = 0;
//  char by_Dat;
//  for(    ;((by_Dat = *(pby_Str+(by_Str_Index++))) != '\0');   )//End Char
//  {		
//    if(by_Dat >= '0' && by_Dat <= '9') by_Out=by_Dat-'0';
//    else if(by_Dat == ' ')  by_Out = 10;//OFF
//    LCD.HT.Time[by_String_Loop].Bit0_6 = (unsigned int)nAscii7Seg[by_Out&0x7f];    ++by_String_Loop;
//#if D_DEFAULT_MODE == D_SMALL_LCD
//    LCD.HT.Time[2].Bit7 =   0;//COL1 OFF
//    LCD.HT.Time[1].Bit7 =   0;//DP2  OFF
//    LCD.HT.Time[3].Bit7 =   0;//DP3  OFF
//    if(by_Dat == ' ')LCD.HT.Time[2].Bit7 =   0;     else  LCD.HT.Time[2].Bit7 =   1;//COL1
//#elif D_DEFAULT_MODE == D_BIG_LCD
//    LCD.HT.Time[2].Bit7 =   0;//COL1 OFF
//    LCD.HT.Time[1].Bit7 =   0;//DP2  OFF
//    LCD.HT.Time[3].Bit7 =   0;//DP3  OFF
//    if(by_Dat == ' ')LCD.HT.Time[2].Bit7 =   0;     else  LCD.HT.Time[2].Bit7 =   1;//COL1
//#endif
//  }
//  Display_Left(1,"TIME");
//}
void Display_Watte_Segment(char by_Address,const char *pby_Str)// left up
{
    char by_Out;
    char by_String_Loop =9;
    char by_Str_Index = 0;
    char by_Dat;
    for(    ;((by_Dat = *(pby_Str+(by_Str_Index++))) != '\0');   )//End Char
    {		
        if(by_Dat >= '0' && by_Dat <= '9') 
            by_Out=by_Dat-'0';
        else if(by_Dat == ' ')  
            by_Out = 11;//OFF
        for(char by_Loop = 0;by_Loop < 2;by_Loop++)
        {
            LCD.HT.LED14Segment_1[by_String_Loop].Bit1_7 = (unsigned int)nAscii14Seg[by_Out&0x7f][by_Loop];
            --by_String_Loop;
        }
#if D_DEFAULT_MODE == D_SMALL_LCD
        LCD.HT.LED14Segment_1[7].Bit0 =   1;//WATT ICON ON 
        if(by_Address ==2)//COL1 OFF
        {
//            if(by_Dat == ' ')
//                LCD.HT.LED14Segment_1[2].Bit7 =   0;
//            else  
//                LCD.HT.LED14Segment_1[2].Bit7 =   0;//COL1
        }
#elif D_DEFAULT_MODE == D_BIG_LCD
        LCD.HT.Watts[2].Bit7 = 0;//COL1 OFF
        LCD.HT.Watts[1].Bit7 = 0;
        LCD.HT.Watts[3].Bit7 = 0;
        if(by_Address ==2)//COL1 OFF
        {
            if(by_Dat == ' ')LCD.HT.Watts[2].Bit7 = 0;
            else  LCD.HT.Watts[2].Bit7 = 0;//COL1
        }	
#endif
    }
}
void Display_RPM(char by_Address,const char *pby_Str)
{
    char by_Out;
    char by_String_Loop =19;
    char by_Str_Index = 0;
    char by_Dat;
    for(    ;((by_Dat = *(pby_Str+(by_Str_Index++))) != '\0');   )//End Char
    {		
        if(by_Dat >= '0' && by_Dat <= '9') 
            by_Out=by_Dat-'0';
        else if(by_Dat == ' ')  
            by_Out = 10;//OFF
        for(char by_Loop = 0;by_Loop < 2;by_Loop++)
        {
            LCD.HT.LED14Segment_1[by_String_Loop].Bit1_7 = (unsigned int)nAscii14Seg[by_Out&0x7f][by_Loop];	  
            --by_String_Loop;
        }	  
        LCD.HT.LED14Segment_1[17].Bit0 =   1;//RPM ICON 
        if(by_Address ==2)//COL1 OFF
        {
            //            if(by_Dat == ' ')
            //                LCD.HT.LED14Segment_1[2].Bit7 =   0;
            //            else  
            //                LCD.HT.LED14Segment_1[2].Bit7 =   0;//COL1
        }
    }
}
void Display_LCD_TIME_DATA(char by_Address,const char *pby_Str)
{
    char by_Out;
    char by_String_Loop =3;
    char by_Str_Index = 0;
    char by_Dat;
    char by_Count=0;
    for(    ;((by_Dat = *(pby_Str+(by_Str_Index++))) != '\0');   )//End Char
    {		
        if(by_Dat >= '0' && by_Dat <= '9') 
            by_Out=by_Dat-'0';
        else if(by_Dat == ' ')  
            by_Out = 10;//OFF
        for(char by_Loop = 0;by_Loop < 2;by_Loop++)
        {
            by_Count++;
            if(by_Count<5)
                LCD.HT.LED14Segment_2[by_String_Loop].Bit1_7 = (unsigned int)nAscii14Seg[by_Out&0x7f][by_Loop];	  
            else
                LCD.HT.LED14Segment_1[by_String_Loop].Bit1_7 = (unsigned int)nAscii14Seg[by_Out&0x7f][by_Loop];
            --by_String_Loop;
            if(by_Count==4)
            {
                by_String_Loop = 21;
            }
        }	
        LCD.HT.Lap_X_4[0].Bit4 = 1;//POINT ICON
        LCD.HT.LED14Segment_2[3].Bit0 = 1;//TIME ICON
    }
}
void Display_LCD_Distance_DATA(char by_Address,const char *pby_Str)
{
    char by_Out;
    char by_String_Loop =15;
    char by_Str_Index = 0;
    char by_Dat;
    for(    ;((by_Dat = *(pby_Str+(by_Str_Index++))) != '\0');   )//End Char
    {		
        if(by_Dat >= '0' && by_Dat <= '9') 
            by_Out=by_Dat-'0';
        else if(by_Dat == ' ')  
            by_Out = 10;//OFF
        for(char by_Loop = 0;by_Loop < 2;by_Loop++)
        {
            LCD.HT.LED14Segment_1[by_String_Loop].Bit1_7 = (unsigned int)nAscii14Seg[by_Out&0x7f][by_Loop];
            --by_String_Loop;
        }	
    }
    LCD.HT.LED14Segment_1[13].Bit0 = 1;//DISTANCE ICON
    if(by_Address == 2)
    {
//        LCD.HT.LED14Segment_1[14].Bit0 = 1;//6P POINT ICON
        LCD.HT.LED14Segment_1[12].Bit0 = 1;//7P POINT ICON
        LCD.HT.LED14Segment_1[10].Bit0 = 0;//8P POINT ICON
    }
    if(by_Address == 1)
    {
//        LCD.HT.LED14Segment_1[14].Bit0 = 0;//6P POINT ICON
        LCD.HT.LED14Segment_1[12].Bit0 = 0;//7P POINT ICON
        LCD.HT.LED14Segment_1[10].Bit0 = 1;//8P POINT ICON
    }
    if(by_Address == 0)
    {
//        LCD.HT.LED14Segment_1[14].Bit0 = 0;//6P POINT ICON
        LCD.HT.LED14Segment_1[12].Bit0 = 0;//7P POINT ICON
        LCD.HT.LED14Segment_1[10].Bit0 = 0;//8P POINT ICON
    }
}

void Display_LCD_Calories_DATA(char by_Address,const char *pby_Str)
{
    char by_Out;
    char by_String_Loop = 5;
    char by_Str_Index = 0;
    char by_Dat;
    for(    ;((by_Dat = *(pby_Str+(by_Str_Index++))) != '\0');   )//End Char
    {		
        if(by_Dat >= '0' && by_Dat <= '9') 
            by_Out=by_Dat-'0';
        else if(by_Dat == ' ')  
            by_Out = 10;//OFF
        for(char by_Loop = 0;by_Loop < 2;by_Loop++)
        {
            LCD.HT.LED14Segment_1[by_String_Loop].Bit1_7 = (unsigned int)nAscii14Seg[by_Out&0x7f][by_Loop];
            --by_String_Loop;
        }	
        LCD.HT.LED14Segment_1[0].Bit0 = 1;//CALORIES ICON
    }
}

void Display_HEART_ICON(bool b_HeartIconFlag)
{
//#if D_DEFAULT_MODE == D_SMALL_LCD
    LCD.HT.HeartRt[1].Bit7 = b_HeartIconFlag;//HEART
//#elif D_DEFAULT_MODE == D_BIG_LCD
//    LCD.HT.Heart_100M[0].Bit6 = b_HeartIconFlag;//HEART
//#endif
    
}
////////////////////////////////////////////////////////////////////////////

void Display_Incline(char by_Address,const char *pby_Str)// 9
{
    char by_Out;
    char by_String_Loop =0;
    char by_Str_Index = 0;
    char by_Dat;
    char by_Flag = 1;
    
#if D_DEFAULT_MODE == D_SMALL_LCD
    char by_BC1 = 0;
    for(    ;((by_Dat = *(pby_Str+(by_Str_Index++))) != '\0');   )//End Char
    {		
        if(by_Dat >= '0' && by_Dat <= '9') by_Out=by_Dat-'0';
        if(by_Dat == ' ')  by_Out = 10;					//OFF
        
        if(by_Flag)
        {
            by_Flag = 0;
            if(by_Dat == ' ')by_BC1 = 0;       
            else  by_BC1 = 1;
        }
        else 			
        {
            for(char by_Loop = 0;by_Loop < 2;by_Loop++)
            {
                LCD.HT.Incline[by_String_Loop].Bit4_7 = (unsigned int)nAscii7Seg2[by_Out&0x7f][by_Loop];	  
                ++by_String_Loop;
            }	
        }
        if(by_BC1) LCD.HT.Incline[0].Bit4_7 |= 0x08;//BC1
        LCD.HT.Incline[1].Bit3 = 1;//incline icon
        if(!by_Address)
            LCD.HT.Incline[2].Bit4_7 |= 0x00;     
        else  
            LCD.HT.Incline[2].Bit4_7 |= 0x08;//DP9
    }
#elif D_DEFAULT_MODE == D_BIG_LCD
    for(    ;((by_Dat = *(pby_Str+(by_Str_Index++))) != '\0');   )//End Char
    {		
        if(by_Dat >= '0' && by_Dat <= '9') by_Out=by_Dat-'0';
        if(by_Dat == ' ')  by_Out = 10;					//OFF
        
        if(by_Flag)
        {
            by_Flag = 0;
            if(by_Dat == ' ')LCD.HT.Incline[0].Bit7 =   0;     else  LCD.HT.Incline[0].Bit7 =   1;//BC1
        }
        else 			
        {
            LCD.HT.Incline[by_String_Loop].Bit0_6 = (unsigned int)nAscii7Seg[by_Out&0x7f];
            if(by_String_Loop < 1)
                ++by_String_Loop;
        }
        if(by_Dat == ' ')LCD.HT.Incline[1].Bit7 = 0;     else  LCD.HT.Incline[1].Bit7 = 1;//?	
    }
    if(!by_Address)//zero
    {
        LCD.HT.Left[1].Bit7 =   0;//INCLINE ICON
        LCD.HT.Left[2].Bit7 =   0;//INCLINE
    }
    else
    {
        LCD.HT.Left[1].Bit7 =   1;//INCLINE ICON
        LCD.HT.Left[2].Bit7 =   1;//INCLINE
    }
#endif
}
void Display_Speed(char by_Address,const char *pby_Str)// 9
{
    char by_Out;
    char by_String_Loop =0;
    char by_Str_Index = 0;
    char by_Dat;
    char by_Flag = 1;

    char by_BC1 = 0;
    for(    ;((by_Dat = *(pby_Str+(by_Str_Index++))) != '\0');   )//End Char
    {		
        if(by_Dat >= '0' && by_Dat <= '9') by_Out=by_Dat-'0';
        if(by_Dat == ' ')  by_Out = 10;					//OFF
        
        if(by_Flag)
        {
            by_Flag = 0;
            if(by_Dat == ' ')
                by_BC1 = 0;       
            else  
                by_BC1 = 1;
        }
        else 			
        {
            for(char by_Loop = 0;by_Loop < 2;by_Loop++)
            {
                LCD.HT.Speed[by_String_Loop].Bit4_7 = (unsigned int)nAscii7Seg2[by_Out&0x7f][by_Loop];	  
                ++by_String_Loop;
            }	
        }
        if(by_BC1) 
        {
            LCD.HT.Speed[0].Bit4_7 |= 0x08;//BC1     
        }
    }
	if( by_Address == 1)
	{
		LCD.HT.Speed[2].Bit4_7|= 0x08;//17P POINT ICON
	}
    LCD.HT.Speed[1].Bit3 = 1;//SPEED icon
}

void Display_Laps_Circle(void)//by_Address=1 LAPS   by_Address=2 SPLIT
//void Display_Laps(char by_Address,char pby_Str)
{
    LCD.HT.Lap_X_1[0].Bit2 =   (gLapCircle>>19) & 0x01;//X1
    LCD.HT.Lap_X_1[0].Bit5 =   (gLapCircle>>18) & 0x01;//X2
    LCD.HT.Lap_X_1[0].Bit6 =   (gLapCircle>>17) & 0x01;//X3
    LCD.HT.Lap_X_1[0].Bit7 =   (gLapCircle>>16) & 0x01;//X4
    LCD.HT.Lap_X_2[0].Bit5 =   (gLapCircle>>15) & 0x01;//X5
    LCD.HT.Lap_X_2[0].Bit4 =   (gLapCircle>>14) & 0x01;//X6
    LCD.HT.Lap_X_2[0].Bit3 =   (gLapCircle>>13) & 0x01;//X7
    LCD.HT.Lap_X_2[0].Bit2 =   (gLapCircle>>12) & 0x01;//X8
    LCD.HT.Lap_X_2[1].Bit2 =   (gLapCircle>>11) & 0x01;//X9
    LCD.HT.Lap_X_2[1].Bit3 =   (gLapCircle>>10) & 0x01;//X10
    LCD.HT.Lap_X_2[1].Bit4 =   (gLapCircle>>9) & 0x01;//X11
    LCD.HT.Lap_X_2[1].Bit5 =   (gLapCircle>>8) & 0x01;//X12
    LCD.HT.Lap_X_3[0].Bit7 =   (gLapCircle>>7) & 0x01;//X13
    LCD.HT.Lap_X_3[0].Bit6 =   (gLapCircle>>6) & 0x01;//X14
    LCD.HT.Lap_X_3[0].Bit5 =   (gLapCircle>>5) & 0x01;//X15
    LCD.HT.Lap_X_3[0].Bit2 =   (gLapCircle>>4) & 0x01;//X16
    LCD.HT.Lap_X_4[0].Bit6 =   (gLapCircle>>3) & 0x01;//X17
    LCD.HT.Lap_X_4[0].Bit5 =   (gLapCircle>>2) & 0x01;//X18
    LCD.HT.Lap_X_4[0].Bit3 =   (gLapCircle>>1) & 0x01;//X19
    LCD.HT.Lap_X_4[0].Bit2 =   (gLapCircle>>0) & 0x01;//X20
    LCD.HT.LED14Segment_1[4].Bit0 =   (gLapCircle>>31) & 0x01;//X21
    LCD.HT.LED14Segment_1[5].Bit0 =   (gLapCircle>>30) & 0x01;//X22
    LCD.HT.LED14Segment_1[6].Bit0 =   (gLapCircle>>29) & 0x01;//X23
    LCD.HT.LED14Segment_1[8].Bit0 =   (gLapCircle>>28) & 0x01;//X24
    LCD.HT.LED14Segment_1[21].Bit0 =   (gLapCircle>>27) & 0x01;//X25
    LCD.HT.Lap_X_4[0].Bit0 =   (gLapCircle>>26) & 0x01;//X26
    LCD.HT.LED14Segment_2[0].Bit0 =   (gLapCircle>>25) & 0x01;//X27
    LCD.HT.LED14Segment_2[1].Bit0 =   (gLapCircle>>24) & 0x01;//X28
    LCD.HT.Lap_X_5[0].Bit1 =   (gLapCircle>>23) & 0x01;//X29
    LCD.HT.Lap_X_5[0].Bit2 =   (gLapCircle>>22) & 0x01;//X30
    LCD.HT.Lap_X_5[0].Bit3 =   (gLapCircle>>21) & 0x01;//X31
    LCD.HT.Lap_X_5[0].Bit4 =   (gLapCircle>>20) & 0x01;//X32
    
    
    LCD.HT.Lap_X_1[0].Bit3 =   1;//X33
    LCD.HT.Lap_X_1[0].Bit4 =   1;//X34
    LCD.HT.Lap_X_2[0].Bit6 =   1;//X35
    LCD.HT.Lap_X_2[0].Bit7 =   1;//X36
    LCD.HT.Lap_X_2[1].Bit6 =   1;//X37
    LCD.HT.Lap_X_2[1].Bit7 =   1;//X38
    LCD.HT.Lap_X_3[0].Bit3 =   1;//X39
    LCD.HT.Lap_X_3[0].Bit4 =   1;//X40
    LCD.HT.HeartRt[2].Bit7 =   1;//X41
    LCD.HT.LED14Segment_1[9].Bit0 =   1;//X42
    LCD.HT.LED14Segment_1[3].Bit0 =   1;//X43
    LCD.HT.LED14Segment_2[2].Bit0 =   1;//X44
    LCD.HT.LED14Segment_1[20].Bit0 =   1;//X45
    LCD.HT.Lap_X_5[0].Bit5 =   1;//X46
    LCD.HT.Lap_X_5[0].Bit6 =   1;//X47
    LCD.HT.Lap_X_4[0].Bit7 =   1;//X48
}

void Display_Bottom_Text(char by_Address,const char *pby_Str)// 7
{
  char by_Out;
  char by_String_Loop = 3;
  char by_Str_Index = 0;
  char by_Dat;
  char by_Count=0;
  char lpby_Str[LCD_14SEG_LENGTH];
  LCD.HT.Lap_X_4[0].Bit4 = 0;//TIME POINT ICON
  LCD.HT.LED14Segment_2[3].Bit0 = 0;//TIME ICON
  LCD.HT.LED14Segment_1[13].Bit0 = 0;//DISTANCE ICON
  LCD.HT.LED14Segment_1[12].Bit0 = 0;//DISTANCE 7P POINT ICON
  LCD.HT.LED14Segment_1[10].Bit0 = 0;//DISTANCE 8P POINT ICON
  LCD.HT.LED14Segment_1[0].Bit0 = 0;//CALORIES ICON
  int lenth = strlen(pby_Str);
  char *par_Str=lpby_Str;
  char *bys_String_Loop=&by_String_Loop;
  char *bys_Count=&by_Count;
  
  memset(aby_LCD_Seg14_String,0,sizeof(aby_LCD_Seg14_String));
  if(lenth <= LCD_14SEG_LENGTH)
  {
    by_14SEG_Move_Mode = 0;
    for(char i = 0;i < LCD_14SEG_LENGTH;i++)
    {
      lpby_Str[i] = ' ';
      if(*(pby_Str+i) != '\0')
      {
        by_Str_Index++;
      }
    }
    by_Str_Index = (LCD_14SEG_LENGTH-by_Str_Index)/2;
    for(char i = 0;i < LCD_14SEG_LENGTH;i++)//End Char
    {		
      if(*(pby_Str+i) != '\0')
        lpby_Str[by_Str_Index+i] = pby_Str[i];        
      *(par_Str+i) = lpby_Str[i];
      LCD_Seg14_Char(bys_Count,bys_String_Loop,(par_Str+i));
    }  
  }
  else
  {
    for(int i=0;i<LCD_14SEG_LENGTH;i++)
    {
      aby_LCD_Seg14_String[i] = ' ';
    }
    by_14SEG_Move_Mode = 1;
    by_seg14_Move_Point = 0;    //zsf
    by_seg14_Moved_Lap_count = 0;
    int count = 0;
    while(1)
    {
      aby_LCD_Seg14_String[SEG14_STR_STARTPOSITION+count] = *(pby_Str);
      pby_Str++;
      count++;
      if(*(pby_Str) == 0)
      {
        return;
      }
    }
  }
  
}


// attend
 void LCD_Seg14_Char(char* bys_Count,char* bys_String_Loop,char* p_Str)
 {
   char by_Out;
   char by_String_Loop=*bys_String_Loop;
   char by_Str_Index = 0;
   char by_Dat=*p_Str;
   char by_Count=*bys_Count;
   
   if(by_Dat >= '0' && by_Dat <= '9') 
     by_Out=by_Dat-'0';
   else if(by_Dat >= 'A' && by_Dat <= 'Z')
     by_Out = by_Dat-'0';
   else if(by_Dat >= 'a' && by_Dat <= 'z')
     by_Out = (by_Dat-'a')+('A'-'0');
      else if(by_Dat == ' ')  
        by_Out = 10;//OFF  
   else if(by_Dat == '.')
     by_Out = 11;//OFF
   else if(by_Dat == '_')   
      by_Out = 47; 
   else
     by_Out = 10;
   for(char by_Loop = 0;by_Loop < 2;by_Loop++)
   {
     by_Count++;
     (*bys_Count)++;
     if(by_Dat == '.')
     {
       LCD.HT.LED14Segment_1[12].Bit0 = 1;//6P
     }
     if(by_Count<5)
       LCD.HT.LED14Segment_2[by_String_Loop].Bit1_7 = (unsigned int)nAscii14Seg[by_Out&0x7f][by_Loop];	 
     else
       LCD.HT.LED14Segment_1[by_String_Loop].Bit1_7 = (unsigned int)nAscii14Seg[by_Out&0x7f][by_Loop];
     --by_String_Loop;
     --(*bys_String_Loop);
     if(by_Count==4)
     {
       by_String_Loop = 21;
       *bys_String_Loop=21;
     }
     
   }   
   
   
 }

static void Display_1ms_ISR(void)
{
/*
    static int MatrixMoveSpeed200ms = 0;
    MatrixMoveSpeed200ms++;
    if(MatrixMoveSpeed200ms>=200)
    {//Matrix_Move
        by_Matrix_Move_Point++;
        if(by_Matrix_Move_Point >= by_Matrix_String_end)
        {
            by_Matrix_Move_Point = 0;
        }
        MatrixMoveSpeed200ms = 0;
        by_Matrix_Move_Mode_Refresh = 1;
    }
*/    
    //seg14_move
    if(by_14SEG_Move_Mode==1)
    {
        static int seg14_MoveSpeed = 0;
        seg14_MoveSpeed++;
        if(seg14_MoveSpeed>=300)
        {
            by_seg14_Move_Point++;
            if(by_seg14_Move_Point>=50)
            {
                by_seg14_Move_Point = 0;
            }
            if(aby_LCD_Seg14_String[by_seg14_Move_Point] == 0)//+SEG14_STR_STARTPOSITION
            {
                by_seg14_Move_Point = 0;
                by_seg14_Moved_Lap_count++;
            }
            seg14_MoveSpeed = 0;
            by_14SEG_Move_Mode_Refresh = 1;
        }
    }
}

 void Display_Regist_1msISR(void)
 {
   Hal_Regist_ISR(MS_ISR,DISPLAY_CTRL_ISR_IDX,Display_1ms_ISR);
 }


////attend


void Display_Bottom_Left(char by_Address,const char *pby_Str)// 7
{
//    unsigned int by_Dotmax_Loop = by_Address*2;
//    char by_Str_Index = 0;
//    char by_Dat,by_Adr;
//    while(1)
//    {
//        by_Dat = *(pby_Str+by_Str_Index);					//NEXT Ascii Char
//        if(by_Dat >= ' ' && by_Dat <= 'z') by_Adr = by_Dat-' ';
//        if(by_Dat >= 'a' && by_Dat <= 'z') by_Adr = (by_Dat-'a')+('A'-' ');
//        
//        if(by_Dat)
//        {
//            for(char by_Loop = 0;by_Loop < 2;by_Loop++,by_Dotmax_Loop++)
//            {
//                LCD.HT.Bottom[by_Dotmax_Loop].Bit0_6  = nAscii14Seg[by_Adr][by_Loop];
//                if(by_Dotmax_Loop >= 29) return;//
//            }
//            ++by_Str_Index;		//NEXT Ascii Char
//        }
//        else					//space
//        {
//            for(;by_Dotmax_Loop < 30;by_Dotmax_Loop++)
//            {
//                LCD.HT.Bottom[by_Dotmax_Loop].Bit0_6 = 0;
//            }
//            if(by_Dotmax_Loop >= 29) return;//
//        }
//    }
}
void Display_HeartRt(char by_Address,const char *pby_Str)// 7
{
    char by_Out;
    char by_String_Loop =2;
    char by_Str_Index = 0;
    char by_Dat;
    for(    ;((by_Dat = *(pby_Str+(by_Str_Index++)+1)) != '\0');   )//End Char
    {		
        if(by_Dat >= '0' && by_Dat <= '9') 
            by_Out=by_Dat-'0';
        else if(by_Dat == ' ')  
            by_Out = 10;//OFF
        LCD.HT.HeartRt[by_String_Loop].Bit1_6 = (unsigned int)nAscii7Seg[by_Out&0x7f]; 
        if(by_String_Loop > 0)
        --by_String_Loop;
#if D_DEFAULT_MODE == D_SMALL_LCD
        LCD.HT.HeartRt[0].Bit7 =   1; //HEART ICON
//        if(!by_Address)
//            LCD.HT.HeartRt[1].Bit7 = 0;
//        else  
//            LCD.HT.HeartRt[1].Bit7 =   1;//HEART
#elif D_DEFAULT_MODE == D_BIG_LCD
        LCD.HT.Heart_Rate[2].Bit0 =   0; //DP16
//        if(!by_Address)
//            LCD.HT.Heart_100M[0].Bit6 = 0;
//        else  
//            LCD.HT.Heart_100M[0].Bit6 = 1;//HEART
#endif
    }
}
void Display_FanIcon (char by_Address)
{
//    char by_Out;
//    char by_String_Loop =0;
//    char by_Str_Index = 0;
//    char by_Dat;
//    char by_Flag = 1;
    if(by_Address != 0)
        LCD.HT.Fan[0].Bit2_7 = 0x3f;
    else
    {
        if(Frame_Stage != FRAME_STAGE_ENG  && Frame_Stage != FRAME_STAGE_AGINGTEST)
        {    
            LCD.HT.Fan[0].Bit2_7 = 0;
        }
    }
//    char by_BC1 = 0;
//    for(    ;((by_Dat = *(pby_Str+(by_Str_Index++))) != '\0');   )//End Char
//    {		
//        if(by_Dat >= '0' && by_Dat <= '9') 
//        {
//            by_Out=by_Dat-'0';
//            if(by_Out==0)
//            {
//                LCD.HT.Fan[0].Bit0 = 0;
//                LCD.HT.Fan[0].Bit1 = 0;
//                LCD.HT.Fan[0].Bit2_7 = nAsciiFan[6];
//                LCD.HT.Lap_X_5[0].Bit0 = 0;
//            }
//            if(by_Out==1)
//            {
//                LCD.HT.Fan[0].Bit1 = 1;
//                LCD.HT.Fan[0].Bit2_7 = nAsciiFan[7];
//            }
//            if(by_Out==2)
//            {
//                LCD.HT.Fan[0].Bit0 = 1;
//                LCD.HT.Fan[0].Bit1 = 1;
//                LCD.HT.Fan[0].Bit2_7 = nAsciiFan[7];
//            }
//            if(by_Out==3)
//            {
//                LCD.HT.Fan[0].Bit0 = 1;
//                LCD.HT.Fan[0].Bit1 = 1;
//                LCD.HT.Lap_X_5[0].Bit0 = 1;
//                LCD.HT.Fan[0].Bit2_7 = nAsciiFan[7];
//            }
//        }
////        if(by_Dat == ' ')  //OFF
////        {
////            LCD.HT.Fan[0].Bit0 = 0;
////            LCD.HT.Fan[0].Bit1 = 0;
////			LCD.HT.Fan[0].Bit2_7 = nAsciiFan[6];
////            LCD.HT.Lap_X_5[0].Bit0 = 0;
////        }
//    }
}
//////////////////////////////////////////////
//void Display_WarmUp(char by_Mode)
//{
//#if D_DEFAULT_MODE == D_SMALL_LCD
//    
//#elif D_DEFAULT_MODE == D_BIG_LCD
//    if(by_Mode)
//    {
//        LCD.HT.Matrix_Up[0].Bit7 =   1;//WarmUp
//        LCD.HT.Matrix_Up[9].Bit0 =   0;//CoolDown
//    }
//    else       LCD.HT.Matrix_Up[0].Bit7 =   0;//WarmUp
//#endif
//}
//void Display_CoolDown(char by_Mode)
//{
//#if D_DEFAULT_MODE == D_SMALL_LCD
//    
//#elif D_DEFAULT_MODE == D_BIG_LCD
//    if(by_Mode)
//    {
//        LCD.HT.Matrix_Up[0].Bit7 =   0;//WarmUp
//        LCD.HT.Matrix_Up[9].Bit0 =   1;//CoolDown
//    }
//    else       LCD.HT.Matrix_Up[9].Bit0 =   0;//CoolDown
//#endif
//}
//////////////////////////////////////////
void Display_AFG_LOGO(char by_Mode)
{
#if D_DEFAULT_MODE == D_SMALL_LCD
    
#elif D_DEFAULT_MODE == D_BIG_LCD
    if(by_Mode)
        LCD.HT.Matrix_Up[2].Bit0 = 1;//AFG LOGO
    else       
        LCD.HT.Matrix_Up[2].Bit0 = 0;//AFG LOGO
#endif
}
void Display_PersonalBest(char by_Mode)
{
#if D_DEFAULT_MODE == D_SMALL_LCD
    
#elif D_DEFAULT_MODE == D_BIG_LCD
    if(by_Mode)
    {
        LCD.HT.Matrix_Up[4].Bit0 =   1;//PersonalBest
        LCD.HT.Matrix_Up[7].Bit0 =   0;//Last5Workouts
        LCD.HT.Matrix_Up[5].Bit0 =   0;//WorkoutTotals
        LCD.HT.Matrix_Up[8].Bit0 =   0;//WorkoutStats
    }
    else       LCD.HT.Matrix_Up[4].Bit0 =   0;//PersonalBest
#endif
}
void Display_Last5Workouts(char by_Mode)
{
#if D_DEFAULT_MODE == D_SMALL_LCD
    
#elif D_DEFAULT_MODE == D_BIG_LCD
    if(by_Mode)
    {
        LCD.HT.Matrix_Up[4].Bit0 =   0;//PersonalBest
        LCD.HT.Matrix_Up[5].Bit0 =   1;//Last5Workouts
        LCD.HT.Matrix_Up[7].Bit0 =   0;//WorkoutTotals
        //LCD.HT.Matrix_Up[8].Bit0 =   0;//WorkoutStats
    }
    else       LCD.HT.Matrix_Up[5].Bit0 =   0;//Last5Workouts
#endif
}
void Display_WorkoutTotals(char by_Mode)
{
#if D_DEFAULT_MODE == D_SMALL_LCD
    
#elif D_DEFAULT_MODE == D_BIG_LCD
    if(by_Mode)
    {
        LCD.HT.Matrix_Up[4].Bit0 =   0;//PersonalBest
        LCD.HT.Matrix_Up[5].Bit0 =   0;//Last5Workouts
        LCD.HT.Matrix_Up[7].Bit0 =   1;//WorkoutTotals
        LCD.HT.Matrix_Up[8].Bit0 =   0;//WorkoutStats
    }
    else       LCD.HT.Matrix_Up[7].Bit0 =   0;//WorkoutTotals
#endif
}
void Display_WorkoutStats(char by_Mode)
{
#if D_DEFAULT_MODE == D_SMALL_LCD
    
#elif D_DEFAULT_MODE == D_BIG_LCD
    if(by_Mode)
    {
        LCD.HT.Matrix_Up[4].Bit0 =   0;//PersonalBest
        //LCD.HT.Matrix_Up[5].Bit0 =   0;//Last5Workouts
        LCD.HT.Matrix_Up[7].Bit0 =   0;//WorkoutTotals
        LCD.HT.Matrix_Up[8].Bit0 =   1;//WorkoutStats
    }
    else       LCD.HT.Matrix_Up[8].Bit0 =   0;//WorkoutStats
#endif
}
////////////////////////////////////////////////////////////////////
void Display_Decode_To_Lcd(void)
{
//#if D_DEFAULT_MODE == D_SMALL_LCD
//    //Loop_Circle
//    LCD.HT.Laps[2].Bit0 		= aby_Circle[0];//50M
//    LCD.HT.Laps[2].Bit2 		= aby_Circle[1];//100M
//    LCD.HT.Matrix_Up[8].Bit0 	= aby_Circle[2];//150M
//    LCD.HT.Matrix_Up[4].Bit0 	= aby_Circle[3];//200M
//    LCD.HT.Matrix_Up[0].Bit0 	= aby_Circle[4];//250M
//    LCD.HT.Matrix_Down[0].Bit0  = aby_Circle[5];//300M
//    LCD.HT.Matrix_Down[4].Bit0  = aby_Circle[6];//350M
//    LCD.HT.Matrix_Down[9].Bit0  = aby_Circle[7];//400M
//    
//#elif D_DEFAULT_MODE == D_BIG_LCD
//    //Loop_Circle
//    LCD.HT.Heart_100M[0].Bit5 = aby_Circle[0];//50M
//    LCD.HT.Heart_100M[0].Bit1 = aby_Circle[1];//100M
//    LCD.HT.Matrix_Up[7].Bit7    = aby_Circle[2];//150M
//    LCD.HT.Matrix_Up[4].Bit7    = aby_Circle[3];//200M
//    LCD.HT.Matrix_Up[1].Bit7    = aby_Circle[4];//250M
//    LCD.HT.Matrix_Down[1].Bit7 = aby_Circle[5];//300M
//    LCD.HT.Matrix_Down[4].Bit7 = aby_Circle[6];//350M
//    LCD.HT.Matrix_Down[7].Bit7 = aby_Circle[7];//400M
//    
//#endif
}

//unsigned char *Display_Get_Loop_Circle_Point(void)
//{
//    return(aby_Circle);
//}

void Display_Matrix(char by_Index,char by_Dat)// 11
{
//    for(char by_Dotmax_Loop=0;by_Dotmax_Loop < 10;by_Dotmax_Loop++ )
//    {
//        LCD.HT.Matrix_Up[by_Dotmax_Loop].Bit1_6 = nAsciiDotMax2[0][0];	
//        LCD.HT.Matrix_Down[by_Dotmax_Loop].Bit1_6 = nAsciiDotMax2[0][1];
//    }
}
void Display_Middle_Matrix(char by_Address,const char *pby_Str)//12*10
{
    char by_Dat,by_Adr;
    char by_Str_Index = 0;
    LCD.HT.Matrix_Up_1[0].Bit0_7 = 0x00;
    LCD.HT.Matrix_Up_1[1].Bit0_7 = 0x00;
    LCD.HT.Matrix_Up_4[0].Bit0_7 = 0x00;
    LCD.HT.Matrix_Up_4[1].Bit0_7 = 0x00;
    while(1)
    {
        by_Dat = *(pby_Str+by_Str_Index);
        
        if(by_Dat >= '0' && by_Dat <= '9') by_Adr = by_Dat-'0';
        if(by_Dat >= 'A' && by_Dat <= 'Z') by_Adr = (by_Dat-'A')+10;
        
        if(by_Dat)
        {
            //             if(by_Str_Index >= 0 && by_Str_Index <= 1)
            //             {
            //                 LCD.HT.Matrix_Up_1[by_Str_Index].Bit0_7 = nAsciiDotMax2[by_Adr][0];
            ////                 LCD.HT.Matrix_Up_2[by_Str_Index].Bit0_7 = nAsciiDotMax2[by_Adr][1];
            ////                 LCD.HT.Matrix_Up_3[by_Str_Index].Bit0_7 = nAsciiDotMax2[by_Adr][2];   
            ////                 LCD.HT.Matrix_Up_4[by_Str_Index].Bit0_7 = nAsciiDotMax2[by_Adr][3];
            //             }
            //             else
            //             {
            //                 if(by_Str_Index >= 2 && by_Str_Index <= 8)
            //                 {
            //                     LCD.HT.Matrix_Up_1[by_Str_Index].Bit0_7 = nAsciiDotMax2[by_Adr][0];
            //                     LCD.HT.Matrix_Up_2[by_Str_Index].Bit0_7 = nAsciiDotMax2[by_Adr][1];
            //                     LCD.HT.Matrix_Up_3[by_Str_Index].Bit0_7 = nAsciiDotMax2[by_Adr][1];   
            //                     LCD.HT.Matrix_Up_4[by_Str_Index].Bit0_7 = nAsciiDotMax2[by_Adr][3];
            //                 }
            //             }
            switch(by_Str_Index)
            {
                case 0:
                    if(by_Adr==0)
                    {
                        LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x00;
                        LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                        LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                        LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x00;
                    }
                    if(by_Adr==1)
                    {
                        LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x80;
                        LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                        LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                        LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x00;
                    }
                    if(by_Adr>=2)
                    {
                        LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x80;
                        LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x80;
                        LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                        LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x00;
                    }
                break;
                case 1:
                if(by_Adr==0)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x00;
                }
                if(by_Adr==1)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x40;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x00;
                }
                if(by_Adr>=2)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x40;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x40;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x00;
                }                 
                break;
                case 2:
                if(by_Adr==0)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x00;
                }
                if(by_Adr==1)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x20;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x00;
                }
                if(by_Adr>=2)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x20;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x20;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x00;
                }
                break;
                case 3:
                if(by_Adr==0)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x00;
                }
                if(by_Adr==1)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x10;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x00;
                }
                if(by_Adr>=2)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x10;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x10;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x00;
                }
                break;
                case 4:
                if(by_Adr==0)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x00;
                }
                if(by_Adr==1)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x08;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x00;
                }
                if(by_Adr>=2)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x08;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x08;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x00;
                }
                break;
                case 5:
                if(by_Adr==0)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x00;
                }
                if(by_Adr==1)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x04;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x00;
                }
                if(by_Adr>=2)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x04;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x04;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x00;
                }
                break;
                case 6:
                if(by_Adr==0)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x00;
                }
                if(by_Adr==1)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x02;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x00;
                }
                if(by_Adr>=2)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x02;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x02;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x00;
                }
                break;
                case 7:
                if(by_Adr==0)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x00;
                }
                if(by_Adr==1)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x01;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x00;
                }
                if(by_Adr>=2)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x01;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x01;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x00;
                }
                break;
                case 8:
                if(by_Adr==0)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x00;
                }
                if(by_Adr==1)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x01;
                }
                if(by_Adr>=2)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x01;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x01;
                }
                break;
                case 9:
                if(by_Adr==0)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x00;
                }
                if(by_Adr==1)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x02;
                }
                if(by_Adr>=2)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x02;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x02;
                }
                break;
                case 10:
                if(by_Adr==0)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x00;
                }
                if(by_Adr==1)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x04;
                }
                if(by_Adr>=2)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x04;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x04;
                }
                break;
                case 11:
                if(by_Adr==0)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x00;
                }
                if(by_Adr==1)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x08;
                }
                if(by_Adr>=2)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x08;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x08;
                }
                break;
                case 12:
                if(by_Adr==0)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x00;
                }
                if(by_Adr==1)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x10;
                }
                if(by_Adr>=2)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x10;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x10;
                }
                break;
                case 13:
                if(by_Adr==0)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x00;
                }
                if(by_Adr==1)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x20;
                }
                if(by_Adr>=2)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x20;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x20;
                }
                break;
                case 14:
                if(by_Adr==0)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x00;
                }
                if(by_Adr==1)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x40;
                }
                if(by_Adr>=2)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x40;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x40;
                }
                break;
                case 15:
                if(by_Adr==0)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x00;
                }
                if(by_Adr==1)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x80;
                }
                if(by_Adr>=2)
                {
                    LCD.HT.Matrix_Up_1[0].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_1[1].Bit0_7 |= 0x00;
                    LCD.HT.Matrix_Up_4[0].Bit0_7 |= 0x80;
                    LCD.HT.Matrix_Up_4[1].Bit0_7 |= 0x80;
                }
                break;
                default:
                break;
            }
            if(by_Str_Index<8)
            {
                LCD.HT.Matrix_Up_2[by_Str_Index].Bit0_7 = nAsciiDotMax2[by_Adr][0];
                LCD.HT.Matrix_Up_3[by_Str_Index].Bit0_7 = nAsciiDotMax2[by_Adr][1];
            }
            else
            {
                LCD.HT.Matrix_Up_2[by_Str_Index%8].Bit0_7 |= nAsciiDotMax2[by_Adr][1];
                LCD.HT.Matrix_Up_3[by_Str_Index%8].Bit0_7 |= nAsciiDotMax2[by_Adr][0];
            }
            by_Str_Index++;
            if(by_Str_Index >= 16)
                return;
            
        }
        else
        {
            if(by_Str_Index >= 16)return;
        }
    }
}
//
//void Display_Set_Dotmax_BottomWork(char by_Mode)//for ?? mode
//{
//    char by_Loop;
//    unsigned int w_Buffer;
//    Display_Matrix(0,0);
//    
//    if(by_Mode)//for setup mode
//    {
//        for(by_Loop = 0;by_Loop < C_DOTMAX_X_MAX;by_Loop++)//10
//        {
//            if(b.PROGRAM_MODE)  w_Buffer = aw_Dotmax_Program[1][by_Loop]; 		//incline
//            else w_Buffer = aw_Dotmax_Program[0][by_Loop];				//speed
//            
//            Display_Set_Dotmax_X_Bar(by_Loop,w_Buffer);			        // "0"   --- by_Loop---   "9"
//        }
//    }
//    else//for work mode
//    {
//        for(by_Loop = 0;by_Loop < C_DOTMAX_X_MAX;by_Loop++)//10
//        {
//            if(b.PROGRAM_MODE)  w_Buffer = aw_Dotmax_Program[1][by_Loop]; 		//incline
//            else w_Buffer = aw_Dotmax_Program[0][by_Loop];				//speed
//            // blink here .attention !
//            if( by_Dotmax_Blink && b.BLINK && (by_Dotmax_Blink-1) == by_Loop)  w_Buffer = 0;
//            Display_Set_Dotmax_X_Bar(by_Loop,w_Buffer);			        // "0"   --- by_Loop---   "9"
//        }//  update dotmatrix data ended here
//        // the following is to update 14 segment (mi zi xing )data
//    }
//}

//static void Display_Set_Dotmax_X_Bar(char by_Address,unsigned int w_Dat)        //x=by_Adderss; y=by_Dat
//{
//    unsigned int by_Dotmax_Loop = 0;
//    char by_Dat,by_Adr;
//    //  unsigned int w_Buffer;
//    
//    by_Dat = w_Dat;
//    
//    while(by_Address)
//    {
//        ++by_Dotmax_Loop;
//        --by_Address;
//    }
//    
//    by_Adr = by_Dat;
//    
//    //  if(by_Dat)
//    {
//        for(  ;by_Dotmax_Loop < 10 ;by_Dotmax_Loop++ )
//        {
//            LCD.HT.Matrix_Up[by_Dotmax_Loop].Bit1_6 = nAsciiDotMax2[by_Adr][0];	
//            LCD.HT.Matrix_Down[by_Dotmax_Loop].Bit1_6 = nAsciiDotMax2[by_Adr][1];
//        }
//    }
//}

//void Display_Set_Dotmax_ProgramWork(char by_Index,char by_Dat)
//{
//    char by_Adr;
//    unsigned int w_Mask;
//    
//    if(by_Dat > 12) by_Dat = 12;      			//10*12
//    
//    by_Adr = by_Index & 0x7f;           		//address  " 12 segment "
//    
//    w_Mask =  by_Dat;                                   //add by ckm 110511
//    
//    if(by_Index & 0x80)
//        aw_Dotmax_Program[1][by_Adr] = w_Mask;			// incline " 12 segment "
//    else
//        aw_Dotmax_Program[0][by_Adr] = w_Mask;			//speed " 12 segment "
//}

//void Display_Set_Moving_StringTop(char by_Address,const char *pby_Str)
//{
//  char by_Out;
//  char by_String_Loop;
//  char by_Str_Index = 0;
//  char by_Dat;
//  //char by_Local_Addr;
//
//  by_String_Loop=0;
//  //by_Local_Addr = by_Address&0x7F;
//
//  while((by_Dat = *(pby_Str+(by_Str_Index++))) != '\0')
//  {		
//    if(by_Dat >= '0' && by_Dat <= '9') by_Out=by_Dat-'0';
//    //else if(by_Dat >= 'A' && by_Dat <= 'Z') by_Out=by_Dat-'A'+10;
//    //else if(by_Dat >= 'a' && by_Dat <= 'z') by_Out=by_Dat-'a'+10;
//    else if(by_Dat == ' ')  by_Out = 10;
//    else if(by_Dat == 'L')  by_Out = 22;
//    else if(by_Dat == 'P')  by_Out = 15;
//    else if(by_Dat == '-')  by_Out = 12;
//    else if(by_Dat == 'E')  by_Out = 16;
//    else if(by_Dat == 'n')  by_Out = 17;
//    else if(by_Dat == 'g')  by_Out = 23;
//    else if(by_Dat == 'd')  by_Out = 18;
//    else by_Out = 10;//space
//    //if(by_Dat == ' ' && by_String_Loop ==0) by_Out = 10;
//    aby_LED_Buffer[by_String_Loop][12] = aby_Number_Code[by_Out&0x7f];
//
//    if(by_Dat == ':')
//    {
//      aby_LED_Buffer[1][12] = 0x80|aby_LED_Buffer[1][12];//---bigLED_dot_SEG2---
//      aby_LED_Buffer[2][12] = 0x80|aby_LED_Buffer[2][12];//---bigLED_dot_SEG3---
//    }
//    if(by_Dat == '.')
//    {
//      aby_LED_Buffer[1][12] = 0x80|aby_LED_Buffer[1][12];//---bigLED_dot_SEG2---
//      //aby_LED_Buffer[2][12] = 0x80|aby_LED_Buffer[2][12];//---bigLED_dot_SEG3---
//    }
//
//    ++by_String_Loop;
//  }
//}

//void Display_Show_State(char by_Dat)//show in
//{
//  if(by_Dat == 7)//for display PCB_ LED7 state  ''incline''  "distance"  "speed"
//  {
//    aby_LED_Buffer[4][10] = 0x01;
//  }
//  else if(by_Dat == 8)//for display PCB_ LED8 state  ''calories''  "pace"  "heat rate"
//  {
//    aby_LED_Buffer[4][10] = 0x02;
//  }
//  else
//  {
//    aby_LED_Buffer[4][10] = 0x00;
//  }
//
//}

//void Display_Set_Moving_String_Bottom(char by_Address,const char *pby_Str)
//{
//  unsigned int by_Dotmax_Loop = 0;
//  char by_Loop = 0;
//  char by_Str_Index = 0;
//  char by_Dat,by_Adr;
//  unsigned int w_Buffer;
//
//  //char *p_Demo_Index;
//  //char *p_Demo_OK_Count;
//  //char *p_Demo_End;
//  //unsigned char *pby_String_Demo;
//
//  by_Demo_Index_Top = 0;
//  by_Demo_OK_Count_Top = 0;
//  by_Demo_End_Top = 0;
//  //m.PROGRAM = DM_DEMO;
//
//  if(by_Address&0x80)
//  {
//    //b.PAGE_MOVE_Top = 0;
//    b.STR_MOVE_Top = 1;
//    //by_Address = 80;//by ckm debug   ???	
//    by_Move_Speed_Top = 5;//by ckm debug    ???
//    // if(by_Dat&0x80)
//    //	{
//    //  by_Move_Speed_Top = by_Dat&0x7f;
//    //}
//    //	else
//    //	{
//    // by_Move_Speed_Top = by_Dat;
//    // }
//  }
//  else
//  {
//    b.STR_MOVE_Top = 0;
//    //b.PAGE_MOVE_Top = 10;
//  }
//
//  by_Address = by_Address&0x7F;
//
//  // p_Demo_Index = &by_Demo_Index_Top;
//  // p_Demo_OK_Count = &by_Demo_OK_Count_Top;
//  // p_Demo_End = &by_Demo_End_Top;
//  // pby_String_Demo = aby_String_Demo_Top;
//
//  //*p_Demo_Index = 0;
//  // *p_Demo_OK_Count = 0;
//  // *p_Demo_End = 0;
//
//  while(by_Address)
//  {
//    // aby_String_Demo_Top[by_Dotmax_Loop] = 0;
//    ++by_Demo_End_Top;
//    ++by_Dotmax_Loop;
//    --by_Address;
//  }
//  while(1)
//  {
//    by_Dat = *(pby_Str+by_Str_Index);
//    //if(by_Dat >= '0' && by_Dat <= '9') by_Adr = by_Dat-'0';
//    //if(by_Dat >= 'A' && by_Dat <= 'Z') by_Adr = (by_Dat-'A')+10;
//    //if(by_Dat >= 'a' && by_Dat <= 'z') by_Adr = (by_Dat-'a')+10;
//    //if(by_Dat == ' ') by_Adr = DSLC_SPACE;
//    if(by_Dat >= ' ' && by_Dat <= 'z') by_Adr = by_Dat-' ';
//    if(by_Dat >= 'a' && by_Dat <= 'z') by_Adr = (by_Dat-'a')+('A'-' ');
//
//    if(by_Dat)
//    {
//      for(by_Loop = 0;by_Loop < 5 ;by_Loop++,by_Dotmax_Loop++,by_Demo_End_Top++)
//      {
//        if(by_Dotmax_Loop >= C_STRING_MAX) return;
//        w_Buffer = nAsciiDot[by_Adr][by_Loop];//>>1;
//        //if((by_Dat == ' ' && by_Loop ==1) || (by_Dat != ' ' && w_Buffer == 0)) break;
//        if((by_Dat == ' ' && by_Loop ==2) ||(by_Dat == '.' && by_Loop ==1) ||(by_Dat == '1' && by_Loop ==4) ) break;//  11.1   99.9
//
//        aby_String_Demo_Top[by_Dotmax_Loop] = w_Buffer;
//      }
//      aby_String_Demo_Top[by_Dotmax_Loop++] = 0;
//      by_Demo_End_Top++;						//max
//      ++by_Str_Index;
//    }
//    else
//    {
//      for(;by_Dotmax_Loop < C_STRING_MAX;by_Dotmax_Loop++)
//      {
//        //aby_String_Demo_Top[by_Dotmax_Loop] = 0;
//      }
//      if(by_Dotmax_Loop >= C_STRING_MAX) return;
//    }
//  }
//}

//void Display_Flag_10ms_Int(void)//debug by ckm
//{
//    static char by_Move_Time_Top = 0;
//    // static char by_Move_Time_Mid = 0;
//    
//    if(by_Move_Time_Top >= by_Move_Speed_Top)
//    {
//        by_Move_Time_Top = 0;
//        
//        
//        if(b.STR_MOVE_Top)
//        {
//            if(by_Demo_Index_Top < by_Demo_End_Top)
//                ++by_Demo_Index_Top;
//            
//            else
//            {
//                by_Demo_Index_Top =0;
//                
//                if(by_Demo_OK_Count_Top < C_STRING_MAX)//degbug by ckm  255
//                    ++by_Demo_OK_Count_Top;
//            }
//            
//            // b.UPDATA = 1;
//        }
//    }
//    else ++by_Move_Time_Top;//by_Move_Speed_Top
//}
//char Display_Get_STR_MOVE_Top(void)//debug by ckm 110504
//{
//    return(b.STR_MOVE_Top);
//}

//char Display_Get_Move_OK_Count(char by_Mode)//debug by ckm 110504
//{
//    if(by_Mode&0x80)
//    {
//        return(by_Demo_OK_Count_Top);//by_Demo_OK_Count_Mid
//    }
//    else
//    {
//        return(by_Demo_OK_Count_Top);
//    }
//}
//void Display_Number_Init(void)//led show init
//{
//aby_LED_Buffer[0][1] = aby_Number_Code[21];
//aby_LED_Buffer[1][1] = aby_Number_Code[17];
//aby_LED_Buffer[2][1] = aby_Number_Code[21];
//aby_LED_Buffer[3][1] = aby_Number_Code[13];
// Display_Set_Time_Dot(0);
//}

//void Display_Number_Safe(void)
//{
//Display_Set_Moving_StringTop(2|0x00,"----");
//  aby_LED_Buffer[0][12] = 0x40;
//  aby_LED_Buffer[1][12] = 0x40;
//  aby_LED_Buffer[2][12] = 0x40;
//  aby_LED_Buffer[3][12] = 0x40;
//}
//void Display_Number_L(char by_Level)//show in the bottom lcd
//{
//}
//void Display_Show_NA(void)
//{
//Display_Set_LCD_Char(33,23);
//Display_Set_LCD_Char(34,38);
//Display_Set_LCD_Char(35,10);
//}
/********************************/
//clear the three line top mid and bottom
//by_Index:CL_TOP,CL_MID,CL_BOT
/********************************/
//void Display_Clear_LCD(unsigned char by_Index)
//{
//}
//void Display_Set_Word_Windows(char by_Index,UINT16 w_Data,char by_Dot)
//{
//}
/********************************/
//show AM/PM
/********************************/
//void Display_RTC_AM_PM(char by_Mode,char by_Data)
//{
//  if(by_Mode)
//  {
//    if(by_Data)
//    {
//      //Display_Set_LCD_Char(34,25);
//      //Display_Set_LCD_Char(35,22);
//    }
//    else
//    {
//      //Display_Set_LCD_Char(34,10);
//      // Display_Set_LCD_Char(35,22);
//    }
//  }
//  else
//  {
//    //Display_Set_LCD_Char(34,DSLC_SPACE);
//    //Display_Set_LCD_Char(35,DSLC_SPACE);
//  }
//}


//void Display_Show_Pace(UINT16 w_Pace)
//{
//    unsigned char by_BIT = 0;
//    if(w_Pace == DSW_ALL_OFF)
//    {
//        Display_Watts(2|0x00,"    ");
//    }
//    else
//    {
//        //w_Pace = 60000/Speed_Get_Speed();
//        //if(w_Pace==65535) w_Pace=0;
//        //else
//        //{
//        if(w_Pace>=10000)      //08_1_10
//        {
//            w_Pace /= 100;
//            by_BIT=3;//DP3  ON
//        }
//        else  by_BIT=2;//DP2  ON
//        //}
//        sprintf(sprintfBuf,"%2u.%2u",(UINT16)w_Pace/100,(UINT16)w_Pace%100);
//        Display_Watts(by_BIT|0x00,sprintfBuf);
//    }
//}

//void Display_Set_Move_Speed(char by_Dat)
//{
//    if(by_Dat&0x80)
//    {
//        by_Move_Speed_Top = by_Dat&0x7f;
//    }
//    else
//    {
//        by_Move_Speed_Top = by_Dat;
//    }
//}

//void Display_Set_Caption(unsigned char by_Addr,unsigned char by_Action)
//{
//}
char Display_Get_Demo(void)
{
    if( b.DEMO_ON )
    {
        return(1);
    }
    else
    {
        return(0);
    }
}

void Display_Set_Demo(char by_Data)
{
    if(by_Data)
    {
        b.DEMO_ON = 1;
    }
    else b.DEMO_ON = 0;
}

void Display_DEMO_Process(void)
{
    LED_Demo_Processs();
}

void Display_Set_Matrix_blink_Flag(char by_Matrix_blink) //////////add 07_10_29//////
{
    if(by_Matrix_blink) b.Matrix_blink=1;
    else b.Matrix_blink=0;
}

char Display_Get_Matrix_blink_Flag(void) //////////add 07_10_29//////
{
    return(b.Matrix_blink);
}

unsigned int *Display_Get_HRDisplay_DotmaxBuffer_Pointer(void)//07_12_2 add
{
    return(0);
    //  return(aw_Dotmax_Program_HRDisplay);
}
//
//void Display_Set_Dotmax_Blink(char by_Dat)
//{
//    by_Dotmax_Blink = by_Dat;
//}

//void Display_Set_Dotmax_Program(char by_Index,char by_Dat)
//{
//    char by_Adr;
//    unsigned int w_Mask;
//    
//    if(by_Dat > 12) by_Dat = 12;      			//12 high
//    
//    by_Adr = by_Index & 0x7f;           		//address  " 14 segment "
//    
//    w_Mask =  by_Dat;                                   //add by ckm 110511
//    
//    if(by_Index & 0x80)
//        aw_Dotmax_Program[1][by_Adr] = w_Mask;			// incline " 10 segment "
//    else
//        aw_Dotmax_Program[0][by_Adr] = w_Mask;			//speed " 10 segment "
//}

void Display_Set_Dotmax_Program_Mode(char by_Mode)
{
    //b.PROGRAM = 1;
    if(by_Mode) b.PROGRAM_MODE = DP_INCLINE;
    else b.PROGRAM_MODE= DP_SPEED;
}

void DisplayString(unsigned char x,unsigned char y,unsigned char z,const char *str)
{
    LED_String(x,y,z,str);
//    if(x==0 && y==0)
//        Display_Left(z,str);//WATTS
    
//    else if(x==0 && y==1)
//        Display_Right(z,str);//PACE
    if(x==0 && y==2)
    {
        Display_Incline(z,str);//Incline
    }
//    else if(x==0 && y==3)
//        Display_Laps(z,str);
    else if(x==0 && y==4)
        Display_Bottom_Left(z,str);//Calories/HR
//    else if(x==0 && y==5)
//        Display_Heart(z,str);//Heart
    else if(x==0 && y==6)
//        Display_Middle_Matrix(z,str);
        Display_Middle_Matrix(z,str);
//    else if(x==0 && y==7)
//        Display_AFG_LOGO(z);
    else if(x==0 && y==8)
//        Display_LeftUp_Segment(z,str);//watts
        Display_Watte_Segment(z,str);//WATTS
//    else if(x==0 && y==9)
//        Display_LeftDown_Segment(z,str);//Calories/HR
    else if(x==0 && y==10)
        Display_Speed(z,str);//Speed
//        Display_Right(z,str);//PACE
    else if(x==0 && y==11)
    {
//        Display_RightDown_Segment(z,str);//Heart
        Display_HeartRt(z,str);//Heart DATA
    }
    else if(x==0 && y==12)
    {
        Display_Laps_Circle();
    }
    else if(x==0 && y==13)
    {
        Display_Bottom_Text(z,str);//BOTTOM TEXT
    }
    else if(x==0 && y==14)
    {
        Display_RPM(z,str);//RPM
    }
    else if(x==0 && y==15)
    {
        Display_LCD_TIME_DATA(z,str);//time
    }
    else if(x==0 && y==16)
    {
        Display_LCD_Distance_DATA(z,str);//distance
    }
    else if(x==0 && y==17)
    {
        Display_LCD_Calories_DATA(z,str);//Calories
    }
}

void SetLapCircle(unsigned int LapCircle)
{
	gLapCircle = LapCircle;
}

