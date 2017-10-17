#include "Display.h"
#include "stdio.h"
#include "string.h"
#include "HT16CXX.h"
#include "jisdef.h"

#include "Show_Functions.h"

#define BIT0                (0x00000001)
#define BIT1                (0x00000002)
#define BIT2                (0x00000004)
#define BIT3                (0x00000008)
#define BIT4                (0x00000010)
#define BIT5                (0x00000020)
#define BIT6                (0x00000040)
#define BIT7                (0x00000080)
#define BIT8                (0x00000100)
#define BIT9                (0x00000200)
#define BITA                (0x00000400)
#define BITB                (0x00000800)
#define BITC                (0x00001000)
#define BITD                (0x00002000)
#define BITE                (0x00004000)
#define BITF                (0x00008000)

#if defined(_4_16C24_Display_)

#define C_16C24_COUNT    		4
#define LCD_BUF_LEN     			120*C_16C24_COUNT
#define C_STRING_MAX  			150//250

void Display_Bottom_Matrix(char Motion,const char *pby_Str);

__no_init static unsigned char aby_LCD_Buffer[LCD_BUF_LEN]; //16C24
__no_init static unsigned char  Bottom_Matrix[50];
__no_init static union
{
     struct
     {
          //---------------------------------------16C24 16bit*60 ----------------------------------------------
          struct {
               UINT16  	 Left_Bottom_Matrix_L       :8;
               UINT16  	 Left_Bottom_Segment        :3;
               UINT16  	 bit0_4                     :5;
          }IC1_Mat[31];	
          struct {
               UINT16  	 Left_Bottom_Matrix_R        :8;
               UINT16  	 Middle_Matrix1        :8;
          }IC1_Mat1[28];	
          struct
          {
               UINT16  	 Bit0_15                          :16;
          }IC1_Null[1];//Null
          //---------------------------------------16C24 16bit*60 ----------------------------------------------
          struct {
               UINT16  	 Right_Bottom_Matrix_R         :8;
               UINT16  	 Right_Bottom_Segment        :3;
               UINT16  	 bit0_4                     :5;
          }IC2_Mat[31];	
          struct {
               UINT16  	 Right_Bottom_Matrix_L        :8;
               UINT16  	 Middle_Matrix2        :8;
          }IC2_Mat1[28];	
          struct
          {
               UINT16  	 Bit0_15                          :16;
          }IC2_Null[1];//Null
          //---------------------------------------16C24 16bit*60 ----------------------------------------------
          struct {
               UINT16  	 bit11_15                  :5;
               UINT16  	 Left_Above_Segment        :3;
               UINT16  	 Left_Above_Matrix_L         :8;
          }IC3_Mat[23];//HRㄛRF	
          struct {
               UINT16  	 Middle_Matrix3          :8;
               UINT16  	 Left_Above_Matrix_R                :8;
          }IC3_Mat1[18];	
          struct
          {
               UINT16  	 bit3_15                  :12;
               UINT16  	 Left_Middle_Segment               :4;
          }IC3_Seg[7];	
          struct
          {
               UINT16  	 Bit0_15                          :16;
          }IC3_Null[12];//Null
          //---------------------------------------16C24 16bit*60 ----------------------------------------------
          struct {
               UINT16  	 bit11_15                  :5;
               UINT16  	 Right_Above_Segment        :3;
               UINT16  	 Right_Above_Matrix_R         :8;
          }IC4_Mat[24];//wifiㄛFan
          struct {
               UINT16  	 Middle_Matrix4          :8;
               UINT16  	 Right_Above_Matrix_L                :8;
          }IC4_Mat1[17];	
          struct {
               UINT16  	 bit3_15                  :12;
               UINT16  	 Right_Middle_Segment               :4;
          }IC4_Seg[6];	
          struct
          {
               UINT16  	 Bit0_15                          :16;
          }IC4_Null[13];//Null
     }HT;
     //-----------------------4pcs HT16XX------------------------------//
     struct
     {
          UINT16  	 Com9_16       :8;
          UINT16  	 Com1_8        :8;
     }LcdTotal_Buffer[60+60+60+60];
     
}LCD;

const char nAscii7Seg3[12][3]=
{
     {0x03,0x05,0x03,}, // -0-48
     {0x00,0x00,0x03,},  // -1-
     {0x02,0x07,0x01,},  // -2-
     {0x00,0x07,0x03,},  // -3-
     {0x01,0x02,0x03,},  // -4-
     {0x01,0x07,0x02,},  // -5-
     {0x03,0x07,0x02,},  // -6-
     {0x00,0x01,0x03,},  // -7-
     {0x03,0x07,0x03,},  // -8-
     {0x01,0x07,0x03,},   // -9-
     
     {0x00,0x00,0x00,}, // -OFF-
     {0x03,0x07,0x03,}, // -ON-
};

const char nAscii7Seg3T[12][3]=
{
     {0x05,0x09,0x06,}, // -0-48
     {0x00,0x00,0x06,},  // -1-
     {0x04,0x0B,0x02,},  // -2-
     {0x00,0x0B,0x06,},  // -3-
     {0x01,0x02,0x06,},  // -4-
     {0x01,0x0B,0x04,},  // -5-
     {0x05,0x0B,0x04,},  // -6-
     {0x00,0x01,0x06,},  // -7-
     {0x05,0x0B,0x06,},  // -8-
     {0x01,0x0B,0x06,},   // -9-
     
     {0x00,0x00,0x00,}, // -OFF-
     {0x05,0x0B,0x06,}, // -ON-
};
const char nAscii7Seg3x[12][3]=
{
     {0x03,0x05,0x03,}, // -0-48
     {0x00,0x00,0x03,},  // -1-
     {0x02,0x07,0x01,},  // -2-
     {0x00,0x07,0x03,},  // -3-
     {0x01,0x02,0x03,},  // -4-
     {0x01,0x07,0x02,},  // -5-
     {0x03,0x07,0x02,},  // -6-
     {0x00,0x01,0x03,},  // -7-
     {0x07,0x07,0x07,},  // -8-
     {0x01,0x07,0x03,},   // -9-
     
     {0x00,0x00,0x00,}, // -OFF-
     {0x07,0x07,0x03,}, // -ON-
};

const char nAscii7Seg4[12][4]=
{
     {0x03, 0x00, 0x05, 0x03,}, // -0-48
     {0x00, 0x00, 0x00, 0x03,},  // -1-
     {0x01, 0x00, 0x0f, 0x02,},  // -2-
     {0x00, 0x00, 0x0f, 0x03,},  // -3-
     {0x02, 0x00, 0x02, 0x03,},  // -4-
     {0x02, 0x00, 0x0f, 0x01,},  // -5-
     {0x03, 0x00, 0x0f, 0x01,},  // -6-
     {0x00, 0x00, 0x04, 0x03,},  // -7-
     {0x03, 0x00, 0x0f, 0x03,},  // -8-
     {0x02, 0x00, 0x0f, 0x03,},   // -9-
     
     {0x00, 0x00, 0x00, 0x00,}, // -OFF-
     {0x03, 0x00, 0x0f, 0x03,}, // -ON-
};

const char nAscii7Seg5[12][5]=
{
     {0x07,0x00,0x05,0x00,0x07,}, // -0-48
     {0x00,0x00,0x00,0x00,0x07,},  // -1-
     {0x02,0x00,0x0f,0x00,0x01,},  // -2-
     {0x00,0x00,0x0f,0x00,0x07,},  // -3-
     {0x05,0x00,0x02,0x00,0x07,},  // -4-
     {0x05,0x00,0x0f,0x00,0x02,},  // -5-
     {0x07,0x00,0x0f,0x00,0x02,},  // -6-
     {0x00,0x00,0x01,0x00,0x07,},  // -7-
     {0x03,0x00,0x0f,0x00,0x03,},  // -8-
     {0x05,0x00,0x0f,0x00,0x07,},   // -9-
     
     {0x00,0x00,0x00,0x00,0x00,}, // -OFF-
     {0x03,0x00,0x0f,0x00,0x03,}, // -ON-
};
const char Demo7Seg3[2][3]=
{
     {0x01,0x03,0x01,},
     {0x02,0x06,0x02,},
};
const char Demo7Seg3T[2][3]=
{
     {0x01,0x03,0x02,},
     {0x04,0x0A,0x04,},
};
const char Demo7Seg4[2][4]=
{
     {0x01,0x00,0x03,0x01,},
     {0x02,0x00,0x06,0x02,},
};
const char Demo7Seg5[2][5]=
{
     {0x01,0x00,0x03,0x00,0x01,},
     {0x02,0x00,0x06,0x00,0x02,},
};
const char Demo7Seg6[2][6]=
{
     {0x01,0x00,0x03,0x00,0x00,0x01,},
     {0x02,0x00,0x06,0x00,0x00,0x02,},
};
const char Demo7Seg7[2][7]=
{
     {0x01,0x00,0x00,0x03,0x00,0x00,0x01,},
     {0x02,0x00,0x00,0x06,0x00,0x00,0x02,},
};
const char Demo7Seg7x[2][7]=
{
     {0x01,0x00,0x00,0x00,0x03,0x00,0x01,},
     {0x02,0x00,0x00,0x00,0x06,0x00,0x02,},
};
const char Demo7Seg8[2][8]=
{
     {0x01,0x00,0x00,0x00,0x03,0x00,0x00,0x01,},
     {0x02,0x00,0x00,0x00,0x06,0x00,0x00,0x02,},
};
const char nAscii7Seg6[12][6]=
{
     {0x03,0x00,0x05,0x00,0x00,0x03,}, // -0-48
     {0x00,0x00,0x00,0x00,0x00,0x03,},  // -1-
     {0x02,0x00,0x07,0x00,0x00,0x01,},  // -2-
     {0x00,0x00,0x07,0x00,0x00,0x03,},  // -3-
     {0x01,0x00,0x02,0x00,0x00,0x03,},  // -4-
     {0x01,0x00,0x07,0x00,0x00,0x02,},  // -5-
     {0x03,0x00,0x07,0x00,0x00,0x02,},  // -6-
     {0x00,0x00,0x01,0x00,0x00,0x03,},  // -7-
     {0x03,0x00,0x07,0x00,0x00,0x03,},  // -8-
     {0x01,0x00,0x07,0x00,0x00,0x03,},   // -9-
     
     {0x00,0x00,0x00,0x00,0x00,0x00,}, // -OFF-
     {0x03,0x00,0x07,0x00,0x00,0x03,}, // -ON-
};

const char nAscii7Seg7[12][7]=
{
     {0x03, 0x00,0x00, 0x05, 0x00,0x00, 0x03,}, // -0-48
     {0x00, 0x00,0x00, 0x00, 0x00,0x00, 0x03,},  // -1-
     {0x01, 0x00,0x00, 0x07, 0x00,0x00, 0x02,},  // -2-
     {0x00, 0x00,0x00, 0x07, 0x00,0x00, 0x07,},  // -3-
     {0x02, 0x00,0x00, 0x02, 0x00,0x00, 0x03,},  // -4-
     {0x02, 0x00,0x00, 0x07, 0x00,0x00, 0x01,},  // -5-
     {0x03, 0x00,0x00, 0x07, 0x00,0x00, 0x01,},  // -6-
     {0x00, 0x00,0x00, 0x04, 0x00,0x00, 0x03,},  // -7-
     {0x03, 0x00,0x00, 0x07, 0x00,0x00, 0x03,},  // -8-
     {0x02, 0x00,0x00, 0x07, 0x00,0x00, 0x03,},   // -9-
     
     {0x00, 0x00,0x00, 0x00, 0x00,0x00, 0x00,}, // -OFF-
     {0x03, 0x00,0x00, 0x07, 0x00,0x00, 0x03,}, // -ON-
};

const char nAscii7Seg7x[12][7]=
{
     {0x03, 0x00,0x00, 0x00, 0x05,0x00, 0x03,}, // -0-48
     {0x00, 0x00,0x00, 0x00, 0x00,0x00, 0x03,},  // -1-
     {0x01, 0x00,0x00, 0x00, 0x07,0x00, 0x02,},  // -2-
     {0x00, 0x00,0x00, 0x00, 0x07,0x00, 0x03,},  // -3-
     {0x02, 0x00,0x00, 0x00, 0x02,0x00, 0x03,},  // -4-
     {0x02, 0x00,0x00, 0x00, 0x07,0x00, 0x01,},  // -5-
     {0x03, 0x00,0x00, 0x00, 0x07,0x00, 0x01,},  // -6-
     {0x00, 0x00,0x00, 0x00, 0x04,0x00, 0x03,},  // -7-
     {0x03, 0x00,0x00, 0x00, 0x07,0x00, 0x03,},  // -8-
     {0x02, 0x00,0x00, 0x00, 0x07,0x00, 0x03,},   // -9-
     
     {0x00, 0x00,0x00, 0x00, 0x00,0x00, 0x00,}, // -OFF-
     {0x03, 0x00,0x00, 0x00, 0x07,0x00, 0x03,}, // -ON-
};

const char nAscii7Seg8[12][8]=
{
     {0x03, 0x00,0x00, 0x00, 0x05, 0x00,0x00, 0x03,}, // -0-48
     {0x00, 0x00,0x00, 0x00, 0x00, 0x00,0x00, 0x03,},  // -1-
     {0x01, 0x00,0x00, 0x00, 0x0f, 0x00,0x00, 0x02,},  // -2-
     {0x00, 0x00,0x00, 0x00, 0x0f, 0x00,0x00, 0x03,},  // -3-
     {0x02, 0x00,0x00, 0x00, 0x02, 0x00,0x00, 0x03,},  // -4-
     {0x02, 0x00,0x00, 0x00, 0x0f, 0x00,0x00, 0x01,},  // -5-
     {0x03, 0x00,0x00, 0x00, 0x0f, 0x00,0x00, 0x01,},  // -6-
     {0x00, 0x00,0x00, 0x00, 0x04, 0x00,0x00, 0x03,},  // -7-
     {0x03, 0x00,0x00, 0x00, 0x0f, 0x00,0x00, 0x03,},  // -8-
     {0x02, 0x00,0x00, 0x00, 0x0f, 0x00,0x00, 0x03,},   // -9-
     
     {0x00, 0x00,0x00, 0x00, 0x00, 0x00,0x00, 0x00,}, // -OFF-
     {0x03, 0x00,0x00, 0x00, 0x0f, 0x00,0x00, 0x03,}, // -ON-
};

const char nAscii7Seg8x[12][8]=
{
     {0x03, 0x00,0x00, 0x05, 0x00, 0x00,0x00, 0x03,}, // -0-48
     {0x00, 0x00,0x00, 0x00, 0x00, 0x00,0x00, 0x03,},  // -1-
     {0x02, 0x00,0x00, 0x07, 0x00, 0x00,0x00, 0x01,},  // -2-
     {0x00, 0x00,0x00, 0x07, 0x00, 0x00,0x00, 0x03,},  // -3-
     {0x01, 0x00,0x00, 0x02, 0x00, 0x00,0x00, 0x03,},  // -4-
     {0x01, 0x00,0x00, 0x07, 0x00, 0x00,0x00, 0x02,},  // -5-
     {0x03, 0x00,0x00, 0x07, 0x00, 0x00,0x00, 0x02,},  // -6-
     {0x00, 0x00,0x00, 0x01, 0x00, 0x00,0x00, 0x03,},  // -7-
     {0x03, 0x00,0x00, 0x07, 0x00, 0x00,0x00, 0x03,},  // -8-
     {0x01, 0x00,0x00, 0x07, 0x00, 0x00,0x00, 0x03,},   // -9-
     
     {0x00, 0x00,0x00, 0x00, 0x00, 0x00,0x00, 0x00,}, // -OFF-
     {0x03, 0x00,0x00, 0x07, 0x00, 0x00,0x00, 0x03,}, // -ON-
};
const unsigned int nAsciiLandmarkIcon[23] =           // ASCII
{
    0x0000,0x0001,0x0003,0x0007,0x000f,0x001f,0x003f,0x007f,0x00ff,0x01ff,
    0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
    0x0003,0x000f,0X007c,0x1fc8,
    0x0fff,0xffff,  
};
const unsigned int nAsciiMatrixIcon[35] =           // ASCII
{
    0x0000,0x3ffc,0x2004,0x2004,0x3ffc,0x0000,//ICON
    0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,//HI8
    0x1ff8,0x3ffc,0x300c,0x300c,0x3ffc,0x1ff8,0x0000,//LO8
    0x030c,0x0498,0x04b0,0x0360,0x00d8,0x01a4,0x0324,0x0618,//'%'
    0x0000,0x0000,0x0000,
};
const unsigned int nAsciiMatrixNum[11][10] =           // ASCII
{
    {0x3ffc,0x2004,0x2004,0x3ffc,0x1ff8,0x3ffc,0x300c,0x300c,0x3ffc,0x1ff8,}, // -0-
    {0x3ffc,0x200c,0x200c,0x3ffc,0x0000,0x0000,0x1c00,0x3ffc,0x3ffc,0x0000,}, // -1-
    {0x3ffc,0x201c,0x201c,0x3ffc,0x187c,0x38fc,0x31cc,0x338c,0x3f0c,0x1e0c,},//2
    {0x3ffc,0x203c,0x203c,0x3ffc,0x1818,0x399c,0x318c,0x318c,0x3ffc,0x1e78,},// -3-
    {0x3ffc,0x207c,0x207c,0x3ffc,0x3fc0,0x3fc0,0x00c0,0x00c0,0x3ffc,0x3ffc,}, // -4-
    {0x3ffc,0x20fc,0x20fc,0x3ffc,0x3f98,0x3f9c,0x318c,0x318c,0x31fc,0x30f8,}, // -5-
    {0x3ffc,0x21fc,0x21fc,0x3ffc,0x1ff8,0x3ffc,0x318c,0x318c,0x39fc,0x18f8,}, // -6-
    {0x3ffc,0x23fc,0x23fc,0x3ffc,0x3004,0x301c,0x3078,0x31e0,0x3f80,0x3e00,}, // -7-
    {0x3ffc,0x27fc,0x27fc,0x3ffc,0x1e78,0x3ffc,0x318c,0x318c,0x3ffc,0x1e78,}, // -8-
    {0x3ffc,0x2ffc,0x2ffc,0x3ffc,0x1f18,0x3f9c,0x318c,0x318c,0x3ffc,0x1ff8,}, // -9-
    {0x3ffc,0x3ffc,0x3ffc,0x3ffc,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,}, // -10-
};
const unsigned int nAsciiMatrix[25][5] =           // ASCII
{
     {0x0000,0x0000,0x0000,0x0000,0x0000,}, // -0-  48
     
     {0x0001,0x0001,0x0001,0x0001,0x0001,}, // -1-
     {0x0003,0x0003,0x0003,0x0003,0x0003,}, // -1-
     
     {0x0007,0x0007,0x0007,0x0007,0x0007,}, // -2-
     
     {0x000f,0x000f,0x000f,0x000f,0x000f,}, // -3-
     
     {0x001f,0x001f,0x001f,0x001f,0x001f,}, // -4-
     
     {0x003f,0x003f,0x003f,0x003f,0x003f,}, // -5-
     
     {0x007f,0x007f,0x007f,0x007f,0x007f,}, // -6-
     
     {0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,}, // -7-
     
     {0x01ff,0x01ff,0x01ff,0x01ff,0x01ff,}, // -8-
     
     
     {0x0000,0x0000,0x0000,0x0000,0x0000,}, // -:-
     
     {0x0000,0x0000,0x0000,0x0000,0x0000,}, // -;-
     
     {0x0000,0x0000,0x0000,0x0000,0x0000,}, // -<-
     
     {0x0000,0x0000,0x0000,0x0000,0x0000,}, // -=-
     
     {0x0000,0x0000,0x0000,0x0000,0x0000,}, // ->-
     
     {0x0000,0x0000,0x0000,0x0000,0x0000,}, // -?-
     
     {0x0000,0x0000,0x0000,0x0000,0x0000,}, // -@-
     {0x03ff,0x03ff,0x03ff,0x03ff,0x03ff,}, // -10-
     
     {0x07ff,0x07ff,0x07ff,0x07ff,0x07ff,}, // -A-
     
     {0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,}, // -B-
     
     {0x1fff,0x1fff,0x1fff,0x1fff,0x1fff,}, // -C-
     
     {0x3fff,0x3fff,0x3fff,0x3fff,0x3fff,}, // -D-
     
     {0x7fff,0x7fff,0x7fff,0x7fff,0x7fff,}, // -E-
     
     {0xffff,0xffff,0xffff,0xffff,0xffff,}, // -F-
     
     {0x0000,0x0000,0x0000,0x0000,0x0000,} ,// -0ff-
};

const char nAsciiDot[100][5] =              // ASCII
{
     {0x00,0x00,0x00,0x00,0x00,}, // - -32
     
     {0x00,0x00,0xBE,0x00,0x00,}, // -!-
     
     {0x00,0x0E,0x00,0x0E,0x00,}, // -"-
     
     {0x28,0xFE,0x28,0xFE,0x00,}, // -#-
     
     {0x48,0x5C,0xF6,0x54,0x00,}, // -$-
     
     {0x46,0x26,0x10,0xC8,0xc4,}, // -%-
     
     {0x6C,0x92,0xAC,0x40,0x00,}, // -&-
     
     {0x00,0x08,0x06,0x02,0x00,}, // -'-
     
     {0x00,0x38,0x44,0x82,0x00,}, // -(-
     
     {0x00,0x82,0x44,0x38,0x00,}, // -)-
     
     {0x44,0x28,0xFE,0x28,0x00,}, // -*-
     
     {0x10,0x10,0xFE,0x10,0x00,}, // -+-
     
     {0x80,0x60,0x20,0x00,0x00,}, // -,-
     
     {0x10,0x10,0x10,0x10,0x00,}, // ---
     
     {0x80,0x00,0x00,0x00,0x00,}, // -.-	    //{0x00,0xC0,0xC0,0x00,0x00,}, // -.-
     
     {0x40,0x20,0x10,0x08,0x00,}, // -/-
     
     
     
     
     {0x7e,0x81,0x81,0x7e,0x00,}, // -0-48
     
     {0x82,0xff,0x80,0x00,0x00,},  // -1-
     
     {0xC6,0xA1,0x91,0x8e,0x00,},  // -2-
     
     {0x42,0x89,0x89,0x76,0x00,},  // -3-
     
     {0x0f,0x08,0x08,0xFf,0x00,},  // -4-
     
     {0x47,0x89,0x89,0x71,0x00,},  // -5-
     
     {0x7e,0x89,0x89,0x72,0x00,},  // -6-
     
     {0x01,0xf1,0x09,0x07,0x00,},  // -7-
     
     {0x76,0x89,0x89,0x76,0x00,},  // -8-
     
     {0x4e,0x91,0x91,0x7e,0x00,},   // -9-
     
     
     
     
     {0x00,0x6C,0x6C,0x00,0x00,}, // -:-
     
     {0x80,0x6C,0x6C,0x00,0x00,}, // -;-
     
     {0x10,0x28,0x44,0x82,0x00,}, // -<-
     
     {0x28,0x28,0x28,0x28,0x00,}, // -=-
     
     {0x00,0x82,0x44,0x28,0x00,}, // ->-
     
     {0x04,0x02,0xB2,0x0A,0x00,}, // -?-
     
     {0x7C,0x82,0xBA,0xAA,0x00,}, // -@-
     
     
     
     
     {0xFe,0x11,0x11,0xfe,0x00,}, // -A-65
     
     {0xFf,0x89,0x89,0x76,0x00,}, // -B-
     
     {0x7e,0x81,0x81,0x42,0x00,}, // -C-
     
     {0xFf,0x81,0x81,0x7e,0x00,}, // -D-
     
     {0xFf,0x89,0x89,0x81,0x00,}, // -E-
     
     {0xFf,0x09,0x09,0x01,0x00,}, // -F-
     
     {0x7e,0x81,0x91,0x72,0x00,}, // -G-
     
     {0xFf,0x08,0x08,0xff,0x00,}, // -H-
     
     {0x81,0xff,0x81,0x00,0x00,}, // -I-
     
     {0x41,0x81,0x7f,0x01,0x00,}, // -J-
     
     {0xFf,0x08,0x14,0xe3,0x00,}, // -K-
     
     {0xFf,0x80,0x80,0x80,0x00,}, // -L-
     
     {0xFf,0x0c,0x30,0x0c,0xff,}, // -M-
     
     {0xFf,0x08,0x10,0xff,0x00,}, // -N-
     
     {0x7e,0x81,0x81,0x7e,0x00,}, // -O-
     
     {0xFf,0x11,0x11,0x0e,0x00,}, // -P-
     
     {0x7e,0xa1,0xc1,0xfe,0x00,}, // -Q-
     
     {0xFf,0x11,0x11,0xee,0x00,}, // -R-
     
     {0x46,0x89,0x89,0x72,0x00,}, // -S-
     
     {0x01,0x01,0xFf,0x01,0x01,}, // -T-
     
     {0x7f,0x80,0x80,0x7f,0x00,}, // -U-
     
     {0xff,0x40,0x20,0x1f,0x00,}, // -V-
     
     {0xFf,0x30,0x0c,0x30,0xff,}, // -W-
     
     {0xe7,0x18,0x18,0xe7,0x00,}, // -X-
     
     {0x47,0x88,0x90,0x7f,0x00,}, // -Y-
     
     {0xe1,0x91,0x89,0x87,0x00,}, // -Z-90
     
     
     
     {0xFE,0xFE,0x82,0x82,0x00,}, // -[-
     
     {0x04,0x08,0x10,0x20,0x00,}, // -\-
     
     {0x00,0x82,0x82,0xFE,0x00,}, // -]-
     
     {0x08,0x04,0xFE,0x04,0x00,}, // -^-
     
     {0x00,0x00,0x00,0x00,0x00,}, // -_-//{0x10,0x38,0x54,0x10,0x10,}, // -_-
     
     {0x00,0x00,0x02,0x04,0x00,}, // -`-
     
     
     
     {0x48,0xA8,0xA8,0x70,0x00,}, // -a-97
     
     {0xFE,0x50,0x88,0x88,0x00,}, // -b-
     
     {0x70,0x88,0x88,0x88,0x00,}, // -c-
     
     {0x70,0x88,0x88,0x50,0x00,}, // -d-
     
     {0x70,0xA8,0xA8,0xA8,0x00,}, // -e-
     
     {0x10,0xFC,0x12,0x12,0x00,}, // -f-
     
     {0x30,0x48,0x48,0x48,0x00,}, // -g-
     
     {0xFE,0x10,0x08,0x08,0x00,}, // -h-
     
     {0x00,0x00,0xF2,0x00,0x00,}, // -i-
     
     {0x00,0x00,0x10,0xF2,0x00,}, // -j-
     
     {0xFE,0x20,0x50,0x88,0x00,}, // -k-
     
     {0x00,0x82,0xFE,0x80,0x00,}, // -l-
     
     {0xF0,0x08,0xF0,0x08,0x00,}, // -m-
     
     {0x08,0xF0,0x08,0x08,0x00,}, // -n-
     
     {0x70,0x88,0x88,0x88,0x00,}, // -o-
     
     {0xF8,0x48,0x48,0x48,0x00,}, // -p-
     
     {0x30,0x48,0x48,0x48,0x00,}, // -q-
     
     {0x08,0xF0,0x08,0x08,0x00,}, // -r-
     
     {0x90,0xA8,0xA8,0xA8,0x00,}, // -s-
     
     {0x08,0x7E,0x88,0x88,0x00,}, // -t-
     
     {0x78,0x80,0x80,0x78,0x00,}, // -u-
     
     {0x38,0x40,0x80,0x40,0x00,}, // -v-
     
     {0x78,0x80,0x78,0x80,0x00,}, // -w-
     
     {0x88,0x50,0x20,0x50,0x00,}, // -x-
     
     {0x38,0x40,0x40,0x20,0x00,}, // -y-
     
     {0x88,0xC8,0xA8,0x98,0x00,},  // -z-122
     
     
     
     
     {0x10,0x6C,0x82,0x00,0x00,}, // -{-
     
     {0x00,0x00,0xEE,0x00,0x00,}, // -|-
     
     {0x00,0x00,0x82,0x6C,0x00,}, // -}-
     
     {0x10,0x08,0x10,0x20,0x00,}, // -~-
     
     {0xAA,0x54,0xAA,0x54,0x00,}  // --
     
     
};

void Display_Initial_Data(void)
{
     memset(aby_LCD_Buffer,0x0000,sizeof(aby_LCD_Buffer));
     memset(LCD.LcdTotal_Buffer,0x0000,sizeof(LCD.LcdTotal_Buffer));
     memset(Bottom_Matrix,0,sizeof(Bottom_Matrix));
     Show_Initial();
}

void Display_Set_All(char by_Dat)		
{
     if(by_Dat)
     {
          memset(aby_LCD_Buffer,0xff,sizeof(aby_LCD_Buffer));
          memset(LCD.LcdTotal_Buffer,0xffff,sizeof(LCD.LcdTotal_Buffer));
     }
     else
     {
          memset(aby_LCD_Buffer,0x00,sizeof(aby_LCD_Buffer));
          memset(LCD.LcdTotal_Buffer,0x000,sizeof(LCD.LcdTotal_Buffer));
     }
}

void Display_Updata_Data(char by_Mode)
{
     for( unsigned int i = 0;i < 60*4;i++)
     {
          aby_LCD_Buffer[i*2+0] =  LCD.LcdTotal_Buffer[i].Com1_8;
          aby_LCD_Buffer[i*2+1] =  LCD.LcdTotal_Buffer[i].Com9_16;
     }	
     //memset(aby_LCD_Buffer,0xff,480);
     HT16CXX_Updata_Data(&aby_LCD_Buffer[0],4);
}

void Display_Time_Segment(char by_Address,const char *pby_Str)//Time
{
     char by_Out,by_Loop,by_Loop2;
     char by_String_Loop =0;
     char by_Str_Index = 0;
     char by_Dat;
     unsigned char w_Buffer;
     unsigned char  Time_Segment_Buffer[12];
     
     for(by_Loop = 0;by_Loop < 12;by_Loop++,by_Loop2--)
     {
          Time_Segment_Buffer[by_Loop] = 0x00;
     }
     
     if(sizeof(pby_Str)>4) return;
     
     for(    ;((by_Dat = *(pby_Str+(by_Str_Index++))) != '\0');   )
     {		
          if(by_Dat >= '0' && by_Dat <= '9') by_Out=by_Dat-'0';
          else if(by_Dat == ' ')
          {
               by_Out = 10;//OFF
          }
          for(by_Loop = 0;by_Loop < 3;by_Loop++)
          {
               Time_Segment_Buffer[by_String_Loop] = (unsigned int)nAscii7Seg3T[by_Out&0x7f][by_Loop];
               ++by_String_Loop;
          }
     }
     //       by_Loop2=5;
     for(by_Loop = 0;by_Loop < 6;by_Loop++,by_Loop2--)
     {
          LCD.HT.IC3_Seg[by_Loop].Left_Middle_Segment = Time_Segment_Buffer[by_Loop];
     }
     
     by_Loop2=11;
     for(by_Loop = 0;by_Loop < 6;by_Loop++,by_Loop2--)
     {
          LCD.HT.IC4_Seg[by_Loop].Right_Middle_Segment = Time_Segment_Buffer[by_Loop2];
     }
     
     if(by_Address==0)
     {
          LCD.HT.IC3_Seg[6].Left_Middle_Segment = 0x00;
     }
     else if(by_Address==1)//---.---
     {
          LCD.HT.IC3_Seg[6].Left_Middle_Segment = 0x08;
     }
     else if(by_Address==2)//---:---
     {
          LCD.HT.IC3_Seg[6].Left_Middle_Segment = 0x04;
     }
     else
     {
          LCD.HT.IC3_Seg[6].Left_Middle_Segment = 0x00;
     }
     for(by_Loop = 0;by_Loop < 7 ;by_Loop++)
     {
          w_Buffer =  LCD.HT.IC3_Seg[by_Loop].Left_Middle_Segment;
          LCD.LcdTotal_Buffer[by_Loop+161].Com1_8 = w_Buffer;
     }	
     for(by_Loop = 0;by_Loop < 6 ;by_Loop++)
     {
          w_Buffer =  LCD.HT.IC4_Seg[by_Loop].Right_Middle_Segment;
          LCD.LcdTotal_Buffer[by_Loop+221].Com1_8 = w_Buffer;
     }	
}

void Display_Left_Bottom_Segment(char by_Address,const char *pby_Str)
{
     char by_Out,by_Loop;
     char by_String_Loop =0;
     char by_Dat;
     unsigned char w_Buffer,w_Buffer2;
     for(by_Loop = 0;by_Loop < 31 ;by_Loop++)
     {
          LCD.HT.IC1_Mat[by_Loop].Left_Bottom_Segment = 0x00;
     }
     
     if(sizeof(pby_Str)>4) return;
     
     if((by_Dat = *(pby_Str)) != '\0')
     {
          if(by_Dat >= '0' && by_Dat <= '9') by_Out=by_Dat-'0';
          else if(by_Dat == ' ')
          {
               by_Out = 10;//OFF
          }
          for(by_Loop = 0;by_Loop < 4;by_Loop++)
          {
               LCD.HT.IC1_Mat[by_String_Loop].Left_Bottom_Segment = (unsigned int)nAscii7Seg4[by_Out&0x7f][by_Loop];//SEG1,3,5,7..驅動
               ++by_String_Loop;
          }
     }
     if((by_Dat = *(pby_Str + 1)) != '\0')
     {
          by_String_Loop = 5;
          if(by_Dat >= '0' && by_Dat <= '9') by_Out=by_Dat-'0';
          else if(by_Dat == ' ')
          {
               by_Out = 10;//OFF
          }
          for(by_Loop = 0;by_Loop < 8;by_Loop++)
          {
               LCD.HT.IC1_Mat[by_String_Loop].Left_Bottom_Segment = (unsigned int)nAscii7Seg8[by_Out&0x7f][by_Loop];//SEG1,3,5,7..驅動
               by_String_Loop++;
          }
     }
     if((by_Dat = *(pby_Str + 2)) != '\0')
     {
          by_String_Loop = 14;
          if(by_Dat >= '0' && by_Dat <= '9') by_Out=by_Dat-'0';
          else if(by_Dat == ' ')
          {
               by_Out = 10;//OFF
          }
          for(by_Loop = 0;by_Loop < 8;by_Loop++)
          {
               LCD.HT.IC1_Mat[by_String_Loop].Left_Bottom_Segment = (unsigned int)nAscii7Seg8[by_Out&0x7f][by_Loop];
               by_String_Loop++;
          }
     }
     if((by_Dat = *(pby_Str + 3)) != '\0')
     {
          by_String_Loop = 22;
          if(by_Dat >= '0' && by_Dat <= '9') by_Out=by_Dat-'0';
          else if(by_Dat == ' ')
          {
               by_Out = 10;//OFF
          }
          for(by_Loop = 0;by_Loop < 7;by_Loop++)
          {
               LCD.HT.IC1_Mat[by_String_Loop].Left_Bottom_Segment = (unsigned int)nAscii7Seg7[by_Out&0x7f][by_Loop];
               by_String_Loop++;
          }
     }
     
     if(by_Address==0)
          LCD.HT.IC1_Mat[13].Left_Bottom_Segment = 0x00;
     else if(by_Address==1)//---.---
          LCD.HT.IC1_Mat[13].Left_Bottom_Segment = 0x01;
     else if(by_Address==2)//---:---
          LCD.HT.IC1_Mat[13].Left_Bottom_Segment = 0x02;
     else
          LCD.HT.IC1_Mat[13].Left_Bottom_Segment = 0x00;
     
     for(by_Loop = 0;by_Loop < 31 ;by_Loop++)
     {
          w_Buffer =  LCD.HT.IC1_Mat[by_Loop].Left_Bottom_Segment ;
          w_Buffer2 =0;
          if(w_Buffer & BIT0) w_Buffer2=w_Buffer2|0x80;else ;
          if(w_Buffer & BIT1) w_Buffer2=w_Buffer2|0x40;else ;
          if(w_Buffer & BIT2) w_Buffer2=w_Buffer2|0x20;else ;
          if(w_Buffer & BIT3) w_Buffer2=w_Buffer2|0x10;else ;
          if(w_Buffer & BIT4) w_Buffer2=w_Buffer2|0x08;else ;
          if(w_Buffer & BIT5) w_Buffer2=w_Buffer2|0x04;else ;
          if(w_Buffer & BIT6) w_Buffer2=w_Buffer2|0x02;else ;
          if(w_Buffer & BIT7) w_Buffer2=w_Buffer2|0x01;else ;
          LCD.LcdTotal_Buffer[by_Loop].Com1_8 = w_Buffer2;
     }	
     
     
}

void Display_Right_Bottom_Segment(char by_Address,const char *pby_Str)
{
     char by_Out,by_Loop;//,by_Loop2;
     char by_String_Loop =0;
     char by_Dat;
     unsigned char w_Buffer,w_Buffer2;
     if(sizeof(pby_Str)>4) return;
     
     by_String_Loop = 29;
     if((by_Dat = *(pby_Str)) != '\0')
     {
          by_String_Loop = 29;
          if(by_Dat >= '0' && by_Dat <= '9') by_Out=by_Dat-'0';
          else if(by_Dat == ' ')
          {
               by_Out = 10;//OFF
          }
          for(by_Loop = 0;by_Loop < 7;by_Loop++)
          {
               LCD.HT.IC2_Mat[by_String_Loop].Right_Bottom_Segment = (unsigned int)nAscii7Seg7[by_Out&0x7f][by_Loop];//SEG1,3,5,7..驅動
               by_String_Loop--;
          }
     }
     
     if((by_Dat = *(pby_Str + 1)) != '\0')
     {
          by_String_Loop = 21;
          if(by_Dat >= '0' && by_Dat <= '9') by_Out=by_Dat-'0';
          else if(by_Dat == ' ')
          {
               by_Out = 10;//OFF
          }
          for(by_Loop = 0;by_Loop < 7;by_Loop++)
          {
               LCD.HT.IC2_Mat[by_String_Loop].Right_Bottom_Segment = (unsigned int)nAscii7Seg7[by_Out&0x7f][by_Loop];//SEG1,3,5,7..驅動
               by_String_Loop--;
          }
     }
     if((by_Dat = *(pby_Str + 2)) != '\0')
     {
          by_String_Loop = 13;
          if(by_Dat >= '0' && by_Dat <= '9') by_Out=by_Dat-'0';
          else if(by_Dat == ' ')
          {
               by_Out = 10;//OFF
          }
          for(by_Loop = 0;by_Loop < 7;by_Loop++)
          {
               LCD.HT.IC2_Mat[by_String_Loop].Right_Bottom_Segment = (unsigned int)nAscii7Seg7[by_Out&0x7f][by_Loop];
               by_String_Loop--;
          }
     }
     if((by_Dat = *(pby_Str + 3)) != '\0')
     {
          by_String_Loop = 6;
          if(by_Dat >= '0' && by_Dat <= '9') by_Out=by_Dat-'0';
          else if(by_Dat == ' ')
          {
               by_Out = 10;//OFF
          }
          for(by_Loop = 0;by_Loop < 7;by_Loop++)
          {
               LCD.HT.IC2_Mat[by_String_Loop].Right_Bottom_Segment = (unsigned int)nAscii7Seg7x[by_Out&0x7f][by_Loop];
               by_String_Loop--;
          }
     }
     
     if(by_Address==0)
          LCD.HT.IC2_Mat[14].Right_Bottom_Segment = 0x00;
     else if(by_Address==1)//---.---
          LCD.HT.IC2_Mat[14].Right_Bottom_Segment = 0x01;
     else if(by_Address==2)//---:---
          LCD.HT.IC2_Mat[14].Right_Bottom_Segment = 0x02;
     else
          LCD.HT.IC2_Mat[14].Right_Bottom_Segment = 0x00;
//     by_Loop2 = 0;
     for(by_Loop = 0;by_Loop < 30 ;by_Loop++)
     {
          w_Buffer =  LCD.HT.IC2_Mat[by_Loop].Right_Bottom_Segment;
          w_Buffer2 =0;
          if(w_Buffer & BIT0) w_Buffer2=w_Buffer2|0x80;else ;
          if(w_Buffer & BIT1) w_Buffer2=w_Buffer2|0x40;else ;
          if(w_Buffer & BIT2) w_Buffer2=w_Buffer2|0x20;else ;
          if(w_Buffer & BIT3) w_Buffer2=w_Buffer2|0x10;else ;
          if(w_Buffer & BIT4) w_Buffer2=w_Buffer2|0x08;else ;
          if(w_Buffer & BIT5) w_Buffer2=w_Buffer2|0x04;else ;
          if(w_Buffer & BIT6) w_Buffer2=w_Buffer2|0x02;else ;
          if(w_Buffer & BIT7) w_Buffer2=w_Buffer2|0x01;else ;
          LCD.LcdTotal_Buffer[by_Loop+60].Com1_8 = w_Buffer2;
     }	
}

void Display_Left_Above_Segment(char by_Address,const char *pby_Str)
{
     char by_Out,by_Loop;
     char by_String_Loop =0;
     char by_Dat;
     unsigned char w_Buffer;
     for(by_Loop = 0;by_Loop < 23 ;by_Loop++)
     {
          LCD.HT.IC3_Mat[by_Loop].Left_Above_Segment =0x00;
     }
     
     if(sizeof(pby_Str)>4) return;
     
     if((by_Dat = *(pby_Str)) != '\0')
     {
          if(by_Dat >= '0' && by_Dat <= '9') by_Out=by_Dat-'0';
          else if(by_Dat == ' ')
          {
               by_Out = 10;//OFF
          }
          for(by_Loop = 0;by_Loop < 5;by_Loop++)
          {
               LCD.HT.IC3_Mat[by_String_Loop].Left_Above_Segment = (unsigned int)nAscii7Seg5[by_Out&0x7f][by_Loop];//SEG1,3,5,7..驅動
               ++by_String_Loop;
          }
     }
     if((by_Dat = *(pby_Str + 1)) != '\0')
     {
          by_String_Loop = 6;
          if(by_Dat >= '0' && by_Dat <= '9') by_Out=by_Dat-'0';
          else if(by_Dat == ' ')
          {
               by_Out = 10;//OFF
          }
          for(by_Loop = 0;by_Loop < 5;by_Loop++)
          {
               LCD.HT.IC3_Mat[by_String_Loop].Left_Above_Segment = (unsigned int)nAscii7Seg5[by_Out&0x7f][by_Loop];//SEG1,3,5,7..驅動
               by_String_Loop++;
          }
     }
     if((by_Dat = *(pby_Str + 2)) != '\0')
     {
          by_String_Loop = 14;
          if(by_Dat >= '0' && by_Dat <= '9') by_Out=by_Dat-'0';
          else if(by_Dat == ' ')
          {
               by_Out = 10;//OFF
          }
          for(by_Loop = 0;by_Loop < 5;by_Loop++)
          {
               LCD.HT.IC3_Mat[by_String_Loop].Left_Above_Segment = (unsigned int)nAscii7Seg5[by_Out&0x7f][by_Loop];//SEG1,3,5,7..驅動
               by_String_Loop++;
          }
     }
     if((by_Dat = *(pby_Str + 3)) != '\0')
     {
          by_String_Loop = 19;
          if(by_Dat >= '0' && by_Dat <= '9') by_Out=by_Dat-'0';
          else if(by_Dat == ' ')
          {
               by_Out = 10;//OFF
          }
          for(by_Loop = 0;by_Loop < 3;by_Loop++)
          {
               LCD.HT.IC3_Mat[by_String_Loop].Left_Above_Segment = (unsigned int)nAscii7Seg3x[by_Out&0x7f][by_Loop];//SEG1,3,5,7..驅動
               by_String_Loop++;
          }
     }
     
     if(by_Address==0)
     {//SEG12苤杅萸ㄛ19ㄩHRㄛ23ㄩRF
          LCD.HT.IC3_Mat[12].Left_Above_Segment = 0x00;
          LCD.HT.IC3_Mat[19].Left_Above_Segment = 0xfb&LCD.HT.IC3_Mat[19].Left_Above_Segment;//bit11
     }
     else if(by_Address==1)//---.---
          LCD.HT.IC3_Mat[12].Left_Above_Segment = 0x02;//
     else if(by_Address==2)//---:---
          LCD.HT.IC3_Mat[12].Left_Above_Segment = 0x04;
     else if(by_Address==3)
          LCD.HT.IC3_Mat[19].Left_Above_Segment = 0x01|LCD.HT.IC3_Mat[19].Left_Above_Segment;
     else if(by_Address==4)
          LCD.HT.IC3_Mat[22].Left_Above_Segment = 0x07;//RF
     else
          LCD.HT.IC3_Mat[12].Left_Above_Segment = 0x00;
     
     for(by_Loop = 0;by_Loop < 23 ;by_Loop++)
     {
          w_Buffer =  LCD.HT.IC3_Mat[by_Loop].Left_Above_Segment;
          LCD.LcdTotal_Buffer[by_Loop+120].Com9_16 = w_Buffer;
     }	
}


void Display_Right_Above_Segment(char by_Address,const char *pby_Str)
{
     char by_Out,by_Loop;
     char by_String_Loop =0;
     char by_Dat;
     unsigned char w_Buffer;
     if(sizeof(pby_Str)>4) return;
     
     by_String_Loop = 21;
     if((by_Dat = *(pby_Str)) != '\0')
     {
          if(by_Dat >= '0' && by_Dat <= '9') by_Out=by_Dat-'0';
          else if(by_Dat == ' ')
          {
               by_Out = 10;//OFF
          }
          for(by_Loop = 0;by_Loop < 3;by_Loop++)
          {
               LCD.HT.IC4_Mat[by_String_Loop].Right_Above_Segment = (unsigned int)nAscii7Seg3[by_Out&0x7f][by_Loop];//SEG1,3,5,7..驅動
               by_String_Loop--;
          }
     }
     if((by_Dat = *(pby_Str + 1)) != '\0')
     {
          by_String_Loop = 18;
          if(by_Dat >= '0' && by_Dat <= '9') by_Out=by_Dat-'0';
          else if(by_Dat == ' ')
          {
               by_Out = 10;//OFF
          }
          for(by_Loop = 0;by_Loop < 3;by_Loop++)
          {
               LCD.HT.IC4_Mat[by_String_Loop].Right_Above_Segment = (unsigned int)nAscii7Seg3[by_Out&0x7f][by_Loop];//SEG1,3,5,7..驅動
               by_String_Loop--;
          }
     }
     if((by_Dat = *(pby_Str + 2)) != '\0')
     {
          by_String_Loop = 14;
          if(by_Dat >= '0' && by_Dat <= '9') by_Out=by_Dat-'0';
          else if(by_Dat == ' ')
          {
               by_Out = 10;//OFF
          }
          for(by_Loop = 0;by_Loop < 8;by_Loop++)
          {
               LCD.HT.IC4_Mat[by_String_Loop].Right_Above_Segment = (unsigned int)nAscii7Seg8x[by_Out&0x7f][by_Loop];//SEG1,3,5,7..驅動
               by_String_Loop--;
          }
     }
     if((by_Dat = *(pby_Str + 3)) != '\0')
     {
          by_String_Loop = 5;
          if(by_Dat >= '0' && by_Dat <= '9') by_Out=by_Dat-'0';
          else if(by_Dat == ' ')
          {
               by_Out = 10;//OFF
          }
          for(by_Loop = 0;by_Loop < 6;by_Loop++)
          {
               LCD.HT.IC4_Mat[by_String_Loop].Right_Above_Segment = (unsigned int)nAscii7Seg6[by_Out&0x7f][by_Loop];//SEG1,3,5,7..驅動
               by_String_Loop--;
          }
     }
     
     if(by_Address==0)
          LCD.HT.IC4_Mat[15].Right_Above_Segment = 0x00|LCD.HT.IC4_Mat[15].Right_Above_Segment;
     else if(by_Address==1)//---.---
          LCD.HT.IC4_Mat[15].Right_Above_Segment = 0x02|LCD.HT.IC4_Mat[15].Right_Above_Segment;
     else if(by_Address==2)//---:---
          LCD.HT.IC4_Mat[15].Right_Above_Segment = 0x01|LCD.HT.IC4_Mat[15].Right_Above_Segment;
     else
          LCD.HT.IC4_Mat[15].Right_Above_Segment = 0x00|LCD.HT.IC4_Mat[15].Right_Above_Segment;
     
     
     for(by_Loop = 0;by_Loop < 24 ;by_Loop++)
     {
          w_Buffer =  LCD.HT.IC4_Mat[by_Loop].Right_Above_Segment;
          LCD.LcdTotal_Buffer[by_Loop+180].Com9_16 = w_Buffer;
     }	
}

void Display_Left_Above_Matrix(char by_Address,const char *pby_Str)// 8*41 Matrix
{
     unsigned int by_Dotmax_Loop = 0;
     char by_Loop = 0;
     char by_Str_Index = 0;
     char by_Dat,by_Adr;
     unsigned char w_Buffer;//,w_Buffer2;
     
     for(by_Loop = 0;by_Loop < 41 ;by_Loop++)
     {
          LCD.HT.IC3_Mat[by_Loop].Left_Above_Matrix_L = 0x00;
     }
     while(1)
     {
          by_Dat = *(pby_Str+by_Str_Index);
          if(by_Dat >= ' ' && by_Dat <= 'z') by_Adr = by_Dat-' ';
          if(by_Dat >= 'a' && by_Dat <= 'z') by_Adr = (by_Dat-'a')+('A'-' ');
          
          if(by_Dat)
          {
               for(by_Loop = 0;by_Loop < 5 ;by_Loop++,by_Dotmax_Loop++/*,by_Demo_End_Top++*/)
               {//(by_Dat == 'i' && by_Loop ==1) ||(by_Dat == 'I' && by_Loop ==1) ||
                    if(by_Dotmax_Loop >= C_STRING_MAX) return;
                    w_Buffer = nAsciiDot[by_Adr][by_Loop];//>>1;
                    //if((by_Dat == ' ' && by_Loop ==1) || (by_Dat != ' ' && w_Buffer == 0)) break;
                    if((by_Dat == ' ' && by_Loop ==2) ||(by_Dat == '.' && by_Loop ==1)||\
                         (by_Dat == 'i' && by_Loop == 3) ||(by_Dat == 'I' && by_Loop == 3) ||(by_Dat == '1' && by_Loop ==3) ) break;//  11.1   99.9
					
                    if(((by_Dat >= '0' && by_Dat <= '9')||\
                         (by_Dat >= 'a' && by_Dat <= 'l')||(by_Dat >= 'n' && by_Dat <= 'v')||(by_Dat >= 'x' && by_Dat <= 'z')||\
                              (by_Dat >= 'A' && by_Dat <= 'L')||(by_Dat >= 'N' && by_Dat <= 'V')||(by_Dat >= 'X' && by_Dat <= 'Z'))&& by_Loop ==4 &&(by_Dat != 'T' && by_Dat != 't'))break;
                    
                    if(by_Dotmax_Loop < 41)
                         LCD.HT.IC3_Mat[by_Dotmax_Loop].Left_Above_Matrix_L  = w_Buffer;
               }
               if(by_Dotmax_Loop < 41)
                    LCD.HT.IC3_Mat[by_Dotmax_Loop++].Left_Above_Matrix_L  = 0;
//               by_Demo_End_Top++;						//max
               ++by_Str_Index;
          }
          else
          {
               for(;by_Dotmax_Loop < C_STRING_MAX;by_Dotmax_Loop++)
               {
                    //aby_String_Demo_Top[by_Dotmax_Loop] = 0;
               }
               if(by_Dotmax_Loop >= C_STRING_MAX) return;
          }
     }
}

void Display_Right_Above_Matrix(char by_Address,const char *pby_Str)// 8*41 Matrix
{
     unsigned int by_Dotmax_Loop = 0;
     char by_Loop = 0;
     char by_Loop2 = 0;
     char by_Str_Index = 0;
     char by_Dat,by_Adr;
     char by_Null_Count = 0;
     unsigned char w_Buffer=0;
     unsigned char  Right_Above_Matrix_Buffer[60];
     unsigned char Right_Above_Matrix[60];
     unsigned char Right_Above_String[60];
     
     unsigned char StringLength = strlen(pby_Str);     
     
     memset(Right_Above_Matrix,0x000,sizeof(Right_Above_Matrix));
     memset(Right_Above_Matrix_Buffer,0x000,sizeof(Right_Above_Matrix_Buffer));
     memset(Right_Above_String,0x000,sizeof(Right_Above_String));
       
//     if(StringLength <= 8)
//          WindowLetterNumber = StringLength;
//     else
//          WindowLetterNumber = 8;
//     
//     // Fill the string to buffer.
//     for(i = 0; i < 8; i++)//max letter number
//     {
//          if(i >= WindowLetterNumber)//mile
//          {
//               Right_Above_String[i] = *(pby_Str /*+ CurrentLetterCount*/ + j);
//               j++;
//          }
//          else
//               Right_Above_String[i] = ' ';     
//     }
     while(1)
     {
          by_Dat = *(pby_Str+by_Str_Index);
          //          by_Dat = Right_Above_String[by_Str_Index];
          
          if(by_Dat >= ' ' && by_Dat <= 'z') by_Adr = by_Dat-' ';
          if(by_Dat >= 'a' && by_Dat <= 'z') by_Adr = (by_Dat-'a')+('A'-' ');
          
          if(by_Dat)
          {
               for(by_Loop = 0;by_Loop < 5 ;by_Loop++,by_Dotmax_Loop++/*,by_Demo_End_Top++*/)
               {//
                    if(by_Dotmax_Loop >= C_STRING_MAX) return;
                    w_Buffer = nAsciiDot[by_Adr][by_Loop];//>>1;
                    //if((by_Dat == ' ' && by_Loop ==1) || (by_Dat != ' ' && w_Buffer == 0)) break;
                    if((by_Dat == ' ' && by_Loop ==2) ||(by_Dat == '.' && by_Loop ==1)||\
                         (by_Dat == 'i' && by_Loop ==3) ||(by_Dat == 'I' && by_Loop ==3) ||(by_Dat == '1' && by_Loop ==3) ) break;//  11.1   99.9
                    if(((by_Dat >= '0' && by_Dat <= '9')||\
                         (by_Dat >= 'a' && by_Dat <= 'l')||(by_Dat >= 'n' && by_Dat <= 'v')||(by_Dat >= 'x' && by_Dat <= 'z')||\
                              (by_Dat >= 'A' && by_Dat <= 'L')||(by_Dat >= 'N' && by_Dat <= 'V')||(by_Dat >= 'X' && by_Dat <= 'Z'))&& by_Loop ==4 &&(by_Dat != 'T' && by_Dat != 't'))break;//				w_Buffer2 =0;
                    
                    
                    if(by_Dotmax_Loop < 41)Right_Above_Matrix_Buffer[by_Dotmax_Loop] = w_Buffer;
               }
               if(by_Dotmax_Loop < 41)Right_Above_Matrix_Buffer[by_Dotmax_Loop++] = 0;
//               by_Demo_End_Top++;						//max
               ++by_Str_Index;
          }
          else
          {
               for(;by_Dotmax_Loop < C_STRING_MAX;by_Dotmax_Loop++)
               {
                    //                  aby_String_Demo_Top[by_Dotmax_Loop] = 0;
               }
               if(by_Dotmax_Loop >= C_STRING_MAX)
               {	
                    by_Loop2 = 40;
                    while(Right_Above_Matrix_Buffer[by_Loop2] == '\0')
                    {
                         by_Loop2--;
                         by_Null_Count++;
                    }
                    memcpy(&Right_Above_Matrix[by_Null_Count],Right_Above_Matrix_Buffer,41-by_Null_Count);
                    by_Loop2 = 40;
                    for(by_Loop = 0;by_Loop < 41 ;by_Loop++,by_Loop2--)
                    {
                         LCD.HT.IC4_Mat[by_Loop].Right_Above_Matrix_R = Right_Above_Matrix[by_Loop2];
                    }
                    return;
               }
          }
     }
     
}
void Display_Bottom_Midle_Matrix(char by_Address,const char *pby_Str)//8*118
{
	unsigned int by_Dotmax_Loop = 35;
	char by_Loop = 0;
	char by_Str_Index = 0;
	char by_Dat,by_Adr;
	unsigned char w_Buffer;
	static unsigned char  Bottom_Matrix[118];
	
	memset(Bottom_Matrix,0x000,sizeof(Bottom_Matrix));
	
	while(1)
	{
		by_Dat = *(pby_Str+by_Str_Index);
		if(by_Dat >= ' ' && by_Dat <= 'z') by_Adr = by_Dat-' ';
		if(by_Dat >= 'a' && by_Dat <= 'z') by_Adr = (by_Dat-'a')+('A'-' ');
		
		if(by_Dat)
		{
			for(by_Loop = 0;by_Loop < 5 ;by_Loop++,by_Dotmax_Loop++)//by_Demo_End_Top++)
			{
				if(by_Dotmax_Loop >= C_STRING_MAX) return;
				w_Buffer = nAsciiDot[by_Adr][by_Loop];//>>1;
//				if((by_Dat == ' ' && by_Loop ==2) ||(by_Dat == '.' && by_Loop ==1) ||(by_Dat == '1' && by_Loop ==4) ) break;//  11.1   99.9
				if((by_Dat == ' ' && by_Loop ==2) ||(by_Dat == '.' && by_Loop ==1)||\
                         (by_Dat == 'i' && by_Loop ==3) ||(by_Dat == 'I' && by_Loop ==3) ||(by_Dat == '1' && by_Loop ==3) ) break;//  11.1   99.9
                    if(((by_Dat >= '0' && by_Dat <= '9')||\
                         (by_Dat >= 'a' && by_Dat <= 'l')||(by_Dat >= 'n' && by_Dat <= 'v')||(by_Dat >= 'x' && by_Dat <= 'z')||\
                              (by_Dat >= 'A' && by_Dat <= 'L')||(by_Dat >= 'N' && by_Dat <= 'V')||(by_Dat >= 'X' && by_Dat <= 'Z'))&& by_Loop ==4 &&(by_Dat != 'T' && by_Dat != 't'))break;
				
				if(by_Dotmax_Loop < 118)Bottom_Matrix[by_Dotmax_Loop]= w_Buffer;
			}
			if(by_Dotmax_Loop < 118)Bottom_Matrix[by_Dotmax_Loop++]= 0;
					
//			by_Demo_End_Top++;						//max
			++by_Str_Index;
		}
		else
		{
			for(;by_Dotmax_Loop < C_STRING_MAX;by_Dotmax_Loop++)
			{
				//aby_String_Demo_Top[by_Dotmax_Loop] = 0;
			}
			if(by_Dotmax_Loop >= C_STRING_MAX) 
            {
                for(by_Loop = 35;by_Loop < 59 ;by_Loop++)
                {
                    LCD.HT.IC1_Mat[by_Loop].Left_Bottom_Matrix_L = Bottom_Matrix[by_Loop];
                }
                for(by_Loop = 35;by_Loop < 59 ;by_Loop++)
                {
                    LCD.HT.IC2_Mat[by_Loop].Right_Bottom_Matrix_R = Bottom_Matrix[117-by_Loop];
                }
                return;
            }
		}
	}
}

void Display_Bottom_LedString(void)
{
	Led_String(Bottom_Matrix,_StringWindow_Letters_Max,Show_Auto);//Show_Auto);//center reversal
     
     for(int by_Loop = 0;by_Loop < 59 ;by_Loop++)
     {
          LCD.HT.IC1_Mat[by_Loop].Left_Bottom_Matrix_L = DotMatrix_Window[by_Loop];
     }
     int by_Loop2=58;
     for(int by_Loop = 0;by_Loop < 59 ;by_Loop++,by_Loop2--)
     {
          LCD.HT.IC2_Mat[by_Loop].Right_Bottom_Matrix_R = DotMatrix_Window[by_Loop2+59];
     }
}	 
void Display_Bottom_Matrix(char Motion,const char *pby_Str)//8*118
{
     unsigned char StringLength = strlen(pby_Str);
     memset(Bottom_Matrix,0x000,sizeof(Bottom_Matrix));
   //*  
     for(int i = 0; i < StringLength; i++)
     {
          Bottom_Matrix[i]= *(pby_Str+i);
     }
		 
//Show_UpToDown
//Show_Auto     
     Display_Bottom_LedString();
}

void Display_Bottom_Left_Matrix(char by_Address,const char *pby_Str)//8*118
{
     unsigned int by_Dotmax_Loop = 0;
     char by_Loop = 0;
//     char by_Loop2 = 0;
     char by_Str_Index = 0;
     char by_Dat,by_Adr;
     unsigned char w_Buffer;
     unsigned char  Left_Bottom_Matrix[59];
     
     memset(Left_Bottom_Matrix,0x000,sizeof(Left_Bottom_Matrix));

     while(1)
     {
          by_Dat = *(pby_Str+by_Str_Index);
          if(by_Dat >= ' ' && by_Dat <= 'z') by_Adr = by_Dat-' ';
          if(by_Dat >= 'a' && by_Dat <= 'z') by_Adr = (by_Dat-'a')+('A'-' ');
          
          if(by_Dat)
          {
               for(by_Loop = 0;by_Loop < 5 ;by_Loop++,by_Dotmax_Loop++/*,by_Demo_End_Top++*/)
               {//
                    if(by_Dotmax_Loop >= C_STRING_MAX) return;
                    w_Buffer = nAsciiDot[by_Adr][by_Loop];//>>1;
              //if((by_Dat == ' ' && by_Loop ==1) || (by_Dat != ' ' && w_Buffer == 0)) break;
                    if((by_Dat == ' ' && by_Loop ==2) ||(by_Dat == '.' && by_Loop ==1)||\
                         (by_Dat == 'i' && by_Loop ==3) ||(by_Dat == 'I' && by_Loop ==3) ||(by_Dat == '1' && by_Loop ==3) ) break;//  11.1   99.9
                    if(((by_Dat >= '0' && by_Dat <= '9')||\
                         (by_Dat >= 'a' && by_Dat <= 'l')||(by_Dat >= 'n' && by_Dat <= 'v')||(by_Dat >= 'x' && by_Dat <= 'z')||\
                              (by_Dat >= 'A' && by_Dat <= 'L')||(by_Dat >= 'N' && by_Dat <= 'V')||(by_Dat >= 'X' && by_Dat <= 'Z'))&& by_Loop ==4 && (by_Dat != 'T' && by_Dat != 't'))break;
                    
                    if(by_Dotmax_Loop <= 59)Left_Bottom_Matrix[by_Dotmax_Loop]= w_Buffer;
               }
               if(by_Dotmax_Loop <= 59)Left_Bottom_Matrix[by_Dotmax_Loop++]= 0;		
//               by_Demo_End_Top++;						//max
               ++by_Str_Index;
          }
          else
          {
               for(;by_Dotmax_Loop < C_STRING_MAX;by_Dotmax_Loop++)
               {
                    //aby_String_Demo_Top[by_Dotmax_Loop] = 0;
               }
               if(by_Dotmax_Loop >= C_STRING_MAX)
               {
                    for(by_Loop = 0;by_Loop < 33 ;by_Loop++)
                    {
                         LCD.HT.IC1_Mat[by_Loop].Left_Bottom_Matrix_L = Left_Bottom_Matrix[by_Loop];
                    }		
                    return;
               }
          }
     }
}

void Display_Bottom_Right_Matrix(char by_Address,const char *pby_Str)//8*118
{
     unsigned int by_Dotmax_Loop = 0;
     char by_Loop = 0;
     char by_Loop2 = 0;
     char by_Null_Count = 0;
     char by_Str_Index = 0;
     char by_Dat,by_Adr;
     unsigned char w_Buffer;
     unsigned char  Bottom_Matrix_R[59] = {0};
     unsigned char Bottom_MatrixBuff[59] = {0};
     
     memset(Bottom_Matrix_R,0x000,sizeof(Bottom_Matrix_R));
     memset(Bottom_MatrixBuff,0x000,sizeof(Bottom_MatrixBuff));
     
     while(1)
     {
          by_Dat = *(pby_Str+by_Str_Index);
          if(by_Dat >= ' ' && by_Dat <= 'z') by_Adr = by_Dat-' ';
          if(by_Dat >= 'a' && by_Dat <= 'z') by_Adr = (by_Dat-'a')+('A'-' ');
          
          if(by_Dat)
          {
               for(by_Loop = 0;by_Loop < 5 ;by_Loop++,by_Dotmax_Loop++/*,by_Demo_End_Top++*/)
               {//(by_Dat == 'i' && by_Loop ==1) ||(by_Dat == 'I' && by_Loop ==1) ||
                    if(by_Dotmax_Loop >= C_STRING_MAX) return;
                    w_Buffer = nAsciiDot[by_Adr][by_Loop];//>>1;
              //if((by_Dat == ' ' && by_Loop ==1) || (by_Dat != ' ' && w_Buffer == 0)) break;
                    if((by_Dat == ' ' && by_Loop ==2) ||(by_Dat == '.' && by_Loop ==1)||\
                         (by_Dat == 'i' && by_Loop ==3) ||(by_Dat == 'I' && by_Loop ==3) ||(by_Dat == '1' && by_Loop ==3) ) break;//  11.1   99.9
                    if(((by_Dat >= '0' && by_Dat <= '9')||\
                         (by_Dat >= 'a' && by_Dat <= 'l')||(by_Dat >= 'n' && by_Dat <= 'v')||(by_Dat >= 'x' && by_Dat <= 'z')||\
                              (by_Dat >= 'A' && by_Dat <= 'L')||(by_Dat >= 'N' && by_Dat <= 'V')||(by_Dat >= 'X' && by_Dat <= 'Z'))&& by_Loop ==4 &&(by_Dat != 'T' && by_Dat != 't'))break;
                    
                    if(by_Dotmax_Loop <= 59)Bottom_MatrixBuff[by_Dotmax_Loop]= w_Buffer;
               }
               if(by_Dotmax_Loop <= 59)Bottom_MatrixBuff[by_Dotmax_Loop++]= 0;				
//               by_Demo_End_Top++;						//max
               ++by_Str_Index;
          }
          else
          {
               for(;by_Dotmax_Loop < C_STRING_MAX;by_Dotmax_Loop++)
               {
                    //aby_String_Demo_Top[by_Dotmax_Loop] = 0;
               }
               
               if(by_Dotmax_Loop >= C_STRING_MAX)
               {
                    by_Loop2=58;
                    while(Bottom_MatrixBuff[by_Loop2] == '\0')
                    {
                         by_Loop2--;
                         by_Null_Count++;
                    }
                    memcpy(&Bottom_Matrix_R[by_Null_Count],Bottom_MatrixBuff,59-by_Null_Count);
                    by_Loop2=58;
                    for(by_Loop = 0;by_Loop < 33 ;by_Loop++,by_Loop2--)
                    {
                         LCD.HT.IC2_Mat[by_Loop].Right_Bottom_Matrix_R = Bottom_Matrix_R[by_Loop2];
                    }
                    return;
               }
          }
     }
}

void Display_Middle_Matrix(char by_Address,const char *pby_Str)//16*35 Matrix
{
    unsigned int by_Dotmax_Loop = 0;
    char by_Loop = 0;
    char by_Loop2 = 0;
    char by_Str_Index = 0;
    char by_Dat,by_Adr;
    unsigned int w_Buffer,w_Buffer2;
    unsigned int  Middle_Matrix[35];
    
    memset(Middle_Matrix,0x000,sizeof(Middle_Matrix));
    
    while(1)
    {
        by_Dat = *(pby_Str+by_Str_Index);
        
        if(by_Dat >= '0' && by_Dat <= 'z') by_Adr = by_Dat-'0';
        if(by_Dat >= 'a' && by_Dat <= 'z') by_Adr = (by_Dat-'a')+('A'-'0');
        
        if(by_Dat)
        {
            if(by_Address == 2)
            {
//                for(by_Dotmax_Loop ;by_Dotmax_Loop < 35 ;by_Dotmax_Loop++/*,by_Demo_End_Top++*/)
                {
                    if(by_Dotmax_Loop >= C_STRING_MAX) return;
                    w_Buffer = nAsciiLandmarkIcon[by_Adr];//>>1;
                    
                    w_Buffer2 =0;
                    if(w_Buffer & BIT0) w_Buffer2=w_Buffer2|0x80;else ;
                    if(w_Buffer & BIT1) w_Buffer2=w_Buffer2|0x40;else ;
                    if(w_Buffer & BIT2) w_Buffer2=w_Buffer2|0x20;else ;
                    if(w_Buffer & BIT3) w_Buffer2=w_Buffer2|0x10;else ;
                    if(w_Buffer & BIT4) w_Buffer2=w_Buffer2|0x08;else ;
                    if(w_Buffer & BIT5) w_Buffer2=w_Buffer2|0x04;else ;
                    if(w_Buffer & BIT6) w_Buffer2=w_Buffer2|0x02;else ;
                    if(w_Buffer & BIT7) w_Buffer2=w_Buffer2|0x01;else ;
                    if(w_Buffer & BIT8) w_Buffer2=w_Buffer2|0x8000;else ;
                    if(w_Buffer & BIT9) w_Buffer2=w_Buffer2|0x4000;else ;
                    if(w_Buffer & BITA) w_Buffer2=w_Buffer2|0x2000;else ;
                    if(w_Buffer & BITB) w_Buffer2=w_Buffer2|0x1000;else ;
                    if(w_Buffer & BITC) w_Buffer2=w_Buffer2|0x0800;else ;
                    if(w_Buffer & BITD) w_Buffer2=w_Buffer2|0x0400;else ;
                    if(w_Buffer & BITE) w_Buffer2=w_Buffer2|0x0200;else ;
                    if(w_Buffer & BITF) w_Buffer2=w_Buffer2|0x0100;else ;
                    //				
                    Middle_Matrix[by_Dotmax_Loop]= w_Buffer2;/*w_Buffer2;*/
                    by_Dotmax_Loop++;
                }
            }
            else if(by_Address == 3)
            {
                if(by_Dotmax_Loop >= C_STRING_MAX) return;
                w_Buffer = nAsciiMatrixIcon[by_Dotmax_Loop];
                w_Buffer2 =0;
                if(w_Buffer & BIT0) w_Buffer2=w_Buffer2|0x80;else ;
                if(w_Buffer & BIT1) w_Buffer2=w_Buffer2|0x40;else ;
                if(w_Buffer & BIT2) w_Buffer2=w_Buffer2|0x20;else ;
                if(w_Buffer & BIT3) w_Buffer2=w_Buffer2|0x10;else ;
                if(w_Buffer & BIT4) w_Buffer2=w_Buffer2|0x08;else ;
                if(w_Buffer & BIT5) w_Buffer2=w_Buffer2|0x04;else ;
                if(w_Buffer & BIT6) w_Buffer2=w_Buffer2|0x02;else ;
                if(w_Buffer & BIT7) w_Buffer2=w_Buffer2|0x01;else ;
                if(w_Buffer & BIT8) w_Buffer2=w_Buffer2|0x8000;else ;
                if(w_Buffer & BIT9) w_Buffer2=w_Buffer2|0x4000;else ;
                if(w_Buffer & BITA) w_Buffer2=w_Buffer2|0x2000;else ;
                if(w_Buffer & BITB) w_Buffer2=w_Buffer2|0x1000;else ;
                if(w_Buffer & BITC) w_Buffer2=w_Buffer2|0x0800;else ;
                if(w_Buffer & BITD) w_Buffer2=w_Buffer2|0x0400;else ;
                if(w_Buffer & BITE) w_Buffer2=w_Buffer2|0x0200;else ;
                if(w_Buffer & BITF) w_Buffer2=w_Buffer2|0x0100;else ;
                //				
                Middle_Matrix[by_Dotmax_Loop]= w_Buffer2;/*w_Buffer2;*/
                by_Dotmax_Loop++;
            }
            else if(by_Address == 4)
            {
                if(by_Dotmax_Loop >= C_STRING_MAX) return;
                w_Buffer = 0;
                if((by_Dotmax_Loop >= 1)&&(by_Dotmax_Loop<=4))
                    w_Buffer = nAsciiMatrixNum[by_Adr][by_Dotmax_Loop-1];
                if((by_Dotmax_Loop >= 10)&&(by_Dotmax_Loop<=15))
                    w_Buffer = nAsciiMatrixNum[by_Adr][by_Dotmax_Loop-6];
                if((by_Dotmax_Loop >= 17)&&(by_Dotmax_Loop<=22))
                    w_Buffer = nAsciiMatrixNum[by_Adr][by_Dotmax_Loop-13];
                if((by_Dotmax_Loop >= 24)&&(by_Dotmax_Loop<=31))
                    w_Buffer = nAsciiMatrixIcon[by_Dotmax_Loop];
                
                w_Buffer2 =0;
                if(w_Buffer & BIT0) w_Buffer2=w_Buffer2|0x80;else ;
                if(w_Buffer & BIT1) w_Buffer2=w_Buffer2|0x40;else ;
                if(w_Buffer & BIT2) w_Buffer2=w_Buffer2|0x20;else ;
                if(w_Buffer & BIT3) w_Buffer2=w_Buffer2|0x10;else ;
                if(w_Buffer & BIT4) w_Buffer2=w_Buffer2|0x08;else ;
                if(w_Buffer & BIT5) w_Buffer2=w_Buffer2|0x04;else ;
                if(w_Buffer & BIT6) w_Buffer2=w_Buffer2|0x02;else ;
                if(w_Buffer & BIT7) w_Buffer2=w_Buffer2|0x01;else ;
                if(w_Buffer & BIT8) w_Buffer2=w_Buffer2|0x8000;else ;
                if(w_Buffer & BIT9) w_Buffer2=w_Buffer2|0x4000;else ;
                if(w_Buffer & BITA) w_Buffer2=w_Buffer2|0x2000;else ;
                if(w_Buffer & BITB) w_Buffer2=w_Buffer2|0x1000;else ;
                if(w_Buffer & BITC) w_Buffer2=w_Buffer2|0x0800;else ;
                if(w_Buffer & BITD) w_Buffer2=w_Buffer2|0x0400;else ;
                if(w_Buffer & BITE) w_Buffer2=w_Buffer2|0x0200;else ;
                if(w_Buffer & BITF) w_Buffer2=w_Buffer2|0x0100;else ;
                //				
                Middle_Matrix[by_Dotmax_Loop]= w_Buffer2;/*w_Buffer2;*/
                by_Dotmax_Loop++;
            }
            else
            {
                for(by_Loop = 0;by_Loop < 3 ;by_Loop++,by_Dotmax_Loop++/*,by_Demo_End_Top++*/)
                {
                    if(by_Dotmax_Loop >= C_STRING_MAX) return;
                    w_Buffer = nAsciiMatrix[by_Adr][by_Loop];//>>1;
                    
                    w_Buffer2 =0;
                    if(w_Buffer & BIT0) w_Buffer2=w_Buffer2|0x80;else ;
                    if(w_Buffer & BIT1) w_Buffer2=w_Buffer2|0x40;else ;
                    if(w_Buffer & BIT2) w_Buffer2=w_Buffer2|0x20;else ;
                    if(w_Buffer & BIT3) w_Buffer2=w_Buffer2|0x10;else ;
                    if(w_Buffer & BIT4) w_Buffer2=w_Buffer2|0x08;else ;
                    if(w_Buffer & BIT5) w_Buffer2=w_Buffer2|0x04;else ;
                    if(w_Buffer & BIT6) w_Buffer2=w_Buffer2|0x02;else ;
                    if(w_Buffer & BIT7) w_Buffer2=w_Buffer2|0x01;else ;
                    if(w_Buffer & BIT8) w_Buffer2=w_Buffer2|0x8000;else ;
                    if(w_Buffer & BIT9) w_Buffer2=w_Buffer2|0x4000;else ;
                    if(w_Buffer & BITA) w_Buffer2=w_Buffer2|0x2000;else ;
                    if(w_Buffer & BITB) w_Buffer2=w_Buffer2|0x1000;else ;
                    if(w_Buffer & BITC) w_Buffer2=w_Buffer2|0x0800;else ;
                    if(w_Buffer & BITD) w_Buffer2=w_Buffer2|0x0400;else ;
                    if(w_Buffer & BITE) w_Buffer2=w_Buffer2|0x0200;else ;
                    if(w_Buffer & BITF) w_Buffer2=w_Buffer2|0x0100;else ;
                    //				
                    if(by_Dotmax_Loop < 35)Middle_Matrix[by_Dotmax_Loop]= w_Buffer2;/*w_Buffer2;*/
                }
            }
            //IC1
            for(by_Loop = 31,by_Loop2= 0;by_Loop < 57;by_Loop++)
            {
                if(by_Loop%3)
                {
                    LCD.HT.IC1_Mat1[by_Loop-31].Middle_Matrix1 = Middle_Matrix[by_Loop2] & 0xFF;
                    by_Loop2++;
                }
            }
            //IC3
            for(by_Loop = 0;by_Loop < 18 ;by_Loop++)
            {
                LCD.HT.IC3_Mat1[by_Loop].Middle_Matrix3 = (Middle_Matrix[by_Loop]>>8) & 0xFF;
            }
            //IC2
            for(by_Loop = 31,by_Loop2=34;by_Loop < 56;by_Loop++)
            {
                if(by_Loop%3)
                {
                    LCD.HT.IC2_Mat1[by_Loop-31].Middle_Matrix2 = Middle_Matrix[by_Loop2] & 0xFF;
                    by_Loop2--;
                }
            }
            //IC4
            for(by_Loop = 0,by_Loop2=34;by_Loop < 17 ;by_Loop++,by_Loop2--)
            {
                LCD.HT.IC4_Mat1[by_Loop].Middle_Matrix4 = (Middle_Matrix[by_Loop2]>>8) & 0xFF;
            }
            //               by_Demo_End_Top++;						//max
            ++by_Str_Index;
        }
        else
        {
            for(;by_Dotmax_Loop < C_STRING_MAX;by_Dotmax_Loop++)
            {
                //aby_String_Demo_Top[by_Dotmax_Loop] = 0;
            }
            if(by_Dotmax_Loop >= C_STRING_MAX) return;
        }
    }
}
void Display_Total_Matrix(int by_x,int by_y)//use by eng0 display test
{
     if(by_x<41)LCD.HT.IC3_Mat[by_x].Left_Above_Matrix_L = by_y;
     if(by_x<41)LCD.HT.IC4_Mat[by_x].Right_Above_Matrix_R = by_y;
     
     if(by_x<18)LCD.HT.IC1_Mat1[by_x].Middle_Matrix1 = by_y;
     if(by_x<17)LCD.HT.IC2_Mat1[by_x].Middle_Matrix2 = by_y;
     if(by_x<18)LCD.HT.IC3_Mat1[by_x].Middle_Matrix3 = by_y;
     if(by_x<17)LCD.HT.IC4_Mat1[by_x].Middle_Matrix4 = by_y;
     
     if(by_x<59)LCD.HT.IC1_Mat[by_x].Left_Bottom_Matrix_L = by_y;
     if(by_x<59)LCD.HT.IC2_Mat[by_x].Right_Bottom_Matrix_R = by_y;
}


void Display_HEART_ICON(char type,const char *pby_Str)
{
    if(type)
	{
		LCD.LcdTotal_Buffer[139].Com9_16 |= 0x04;
		//LCD.HT.IC3_Mat[19].Left_Above_Segment = 0x07|LCD.HT.IC3_Mat[19].Left_Above_Segment;
	}
	else
	{
		LCD.LcdTotal_Buffer[139].Com9_16 &= ~0x04;
		//LCD.HT.IC3_Mat[19].Left_Above_Segment = 0xf8&LCD.HT.IC3_Mat[19].Left_Above_Segment;
	}  
}
void Display_RF_ICON(char type,const char *pby_Str)
{
     if(type)
     {
          //LCD.HT.IC3_Mat[22].Left_Above_Segment = 0x07;
          LCD.LcdTotal_Buffer[142].Com9_16 = 0x07;
     }
     else
     {
          //LCD.HT.IC3_Mat[22].Left_Above_Segment = 0x00;
          LCD.LcdTotal_Buffer[142].Com9_16 = 0x00;
     }
}

void DisplayString(unsigned char x,unsigned char y,unsigned char z,const char *str)
{
     unsigned char by_Loop,wifi_strength = 0;
     unsigned char w_Buffer = 0;
     if(x==1&&y==0)
          Display_Left_Above_Matrix(z,str);//8*41dot
     else if(x==41&&y==0)
          Display_Right_Above_Matrix(z,str);//8*40dot
     else if(x==0&&y==1)
          Display_Left_Above_Segment(z,str);//4seg
     else if(x==4&&y==1)
          Display_Time_Segment(z,str);//time
     else if(x==8&&y==1)
          Display_Right_Above_Segment(z,str);//4seg
     else if(x==12&&y==1)
          Display_Left_Bottom_Segment(z,str);//4seg
     else if(x==16&&y==1)
          Display_Right_Bottom_Segment(z,str);//4seg
     else if(x==0&&y==2)
          Display_Middle_Matrix(z,str);//16*35dot
     else if(x==0&&y==3)
          Display_Bottom_Matrix(z,str);//8*119dot
     else if(x==0&&y==6)
          Display_Bottom_Left_Matrix(z,str);//icon
//          Display_Bottom_Matrix(z,str);//8*119dot
     else if(x==0&&y==7)
          Display_Bottom_Right_Matrix(z,str);//icon
//          Display_Bottom_Matrix(z,str);//8*119dot
     else if(x==0&&y==8)
         Display_Bottom_Midle_Matrix(z,str);
     else if(x==0&&y==4)//RF
     {
          //Display_RF_ICON(z,str);//icon
          if(z)
          {
               LCD.LcdTotal_Buffer[142].Com9_16 = 0x07;
          }
          else
          {
               LCD.LcdTotal_Buffer[142].Com9_16 = 0x00;
          }
     }
     else if(x==0&&y==5)//wifi
     {
          wifi_strength = z;
          if(wifi_strength == 0)
          {
               LCD.HT.IC4_Mat[22].Right_Above_Segment = 0x00;
               LCD.HT.IC4_Mat[23].Right_Above_Segment = 0x00;
          }
          else if(wifi_strength == 1)
          {
               LCD.HT.IC4_Mat[22].Right_Above_Segment = 0x02;
               LCD.HT.IC4_Mat[23].Right_Above_Segment = 0x00;
          }
          else if(wifi_strength == 2)
          {
               LCD.HT.IC4_Mat[22].Right_Above_Segment = 0x02;
               LCD.HT.IC4_Mat[23].Right_Above_Segment = 0x02;
          }
          else if(wifi_strength == 3)
          {
               LCD.HT.IC4_Mat[22].Right_Above_Segment = 0x02;
               LCD.HT.IC4_Mat[23].Right_Above_Segment = 0x03;
          }
          else if(wifi_strength == 4)
          {
               LCD.HT.IC4_Mat[22].Right_Above_Segment = 0x03;
               LCD.HT.IC4_Mat[23].Right_Above_Segment = 0x03;
          }
          for(by_Loop = 22;by_Loop < 24 ;by_Loop++)
          {
               w_Buffer =  LCD.HT.IC4_Mat[by_Loop].Right_Above_Segment;
               LCD.LcdTotal_Buffer[by_Loop+180].Com9_16 = w_Buffer;
          }	
     }
     else
     {
          //    while(1);//     Display_Set_All(0);
     }
     
}

const  char demo1[8][8] ={ 
  {0xff,0x81,0x81,0x81,0x81,0x81,0x81,0xff},
  {0x00,0x7e,0x42,0x42,0x42,0x42,0x7e,0x00},   
  {0x00,0x00,0x3c,0x24,0x24,0x3c,0x00,0x00},
  {0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00},
  //{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00},
  {0x00,0x00,0x3c,0x24,0x24,0x3c,0x00,0x00},
  {0x00,0x7e,0x42,0x42,0x42,0x42,0x7e,0x00}, 
  {0xff,0x81,0x81,0x81,0x81,0x81,0x81,0xff},
};

//if(Timer_Counter(T_LOOP+2,4)) Display_DEMO_Process();
void Display_DEMO_Process(void)
{ 
    unsigned char by_Loop1,by_Loop2,y=0,y1=0;
    static unsigned char x=0,change_show=0;
     unsigned char w_Buffer,w_Buffer2,by_Loop;
    //  if(!b.DEMO_ON)return;
    //*
    Display_Set_All(0);
    //  if(change_show==0)
    {
        for(by_Loop1=0;by_Loop1 <8/*LED_COM*/;by_Loop1++)
        {         
            for(by_Loop2=0;by_Loop2<(/*LED_BUF_LEN*/5) ;by_Loop2++)	   
            {
                LCD.HT.IC3_Mat[by_Loop1+by_Loop2*8].Left_Above_Matrix_L = demo1[x][y];
            }
            
            for(by_Loop2=0;by_Loop2<(/*LED_BUF_LEN*/5) ;by_Loop2++)	   
            {
                LCD.HT.IC4_Mat[by_Loop1+by_Loop2*8].Right_Above_Matrix_R = demo1[x][y];
            }
            for(by_Loop2=0;by_Loop2<(/*LED_BUF_LEN*/7) ;by_Loop2++)	   
            {               
                LCD.HT.IC1_Mat[by_Loop1+by_Loop2*8+2].Left_Bottom_Matrix_L = demo1[x][y];
            }
            for(by_Loop2=0;by_Loop2<(/*LED_BUF_LEN*/7) ;by_Loop2++)	   
            {                
                LCD.HT.IC2_Mat[by_Loop1+by_Loop2*8+2].Right_Bottom_Matrix_R = demo1[x][y];
            }
            
            for(by_Loop2 = 0;by_Loop2 < 2;by_Loop2++)
            {
                LCD.HT.IC3_Mat1[by_Loop1+by_Loop2*8+1].Middle_Matrix3 = demo1[x][y];             
            }
            for(by_Loop2 = 0;by_Loop2 < 2;by_Loop2++)
            {
                LCD.HT.IC4_Mat1[by_Loop1+by_Loop2*8+1].Middle_Matrix4 = demo1[x][y];             
            }
            if(++y>7) y=0;
        }
        
        for(by_Loop2 = 0;by_Loop2 < 2;by_Loop2++)
        {
            for(by_Loop1 = 1;by_Loop1 < 13;by_Loop1++)
            {
                by_Loop = by_Loop1+by_Loop2*12+1;
                if(by_Loop%3)
                {
                    LCD.HT.IC1_Mat1[by_Loop-1].Middle_Matrix1 = demo1[x][y1];
                    if(++y1>7) y1=0;
                }  
            }
        }
        for(by_Loop2 = 0;by_Loop2 < 2;by_Loop2++)
        {
            for(by_Loop1 = 1;by_Loop1 < 13;by_Loop1++)
            {
                by_Loop = by_Loop1+by_Loop2*12+1;
                if(by_Loop%3)
                {
                    LCD.HT.IC2_Mat1[by_Loop-1].Middle_Matrix2 = demo1[x][y1];
                    if(++y1>7) y1=0;
                }  
            }
        }
        if(++x>7) 
        {
            x=0;  
        }
        //    
        //88:88
        //左上
        for(by_Loop = 0;by_Loop < 5;by_Loop++)
            LCD.HT.IC3_Mat[by_Loop].Left_Above_Segment = Demo7Seg5[change_show][by_Loop];
        for(by_Loop = 0;by_Loop < 5;by_Loop++)
            LCD.HT.IC3_Mat[by_Loop+6].Left_Above_Segment = Demo7Seg5[change_show][by_Loop];
        for(by_Loop = 0;by_Loop < 5;by_Loop++)
            LCD.HT.IC3_Mat[by_Loop+14].Left_Above_Segment = Demo7Seg5[change_show][by_Loop];
        for(by_Loop = 0;by_Loop < 3;by_Loop++)
            LCD.HT.IC3_Mat[by_Loop+19].Left_Above_Segment = Demo7Seg3[change_show][by_Loop];
        //左下
        for(by_Loop = 0;by_Loop < 5;by_Loop++)
            LCD.HT.IC1_Mat[by_Loop].Left_Bottom_Segment = Demo7Seg4[change_show][by_Loop];
        for(by_Loop = 0;by_Loop < 8;by_Loop++)
            LCD.HT.IC1_Mat[by_Loop+5].Left_Bottom_Segment = Demo7Seg8[change_show][by_Loop];
        for(by_Loop = 0;by_Loop < 8;by_Loop++)
            LCD.HT.IC1_Mat[by_Loop+14].Left_Bottom_Segment = Demo7Seg8[change_show][by_Loop];
        for(by_Loop = 0;by_Loop < 7;by_Loop++)
            LCD.HT.IC1_Mat[by_Loop+22].Left_Bottom_Segment = Demo7Seg7[change_show][by_Loop];
        //右上
        for(by_Loop = 0;by_Loop < 3;by_Loop++)
            LCD.HT.IC4_Mat[by_Loop+19].Right_Above_Segment = Demo7Seg3[change_show][by_Loop];
        for(by_Loop = 0;by_Loop < 3;by_Loop++)
            LCD.HT.IC4_Mat[by_Loop+16].Right_Above_Segment = Demo7Seg3[change_show][by_Loop];
        for(by_Loop = 0;by_Loop < 8;by_Loop++)
            LCD.HT.IC4_Mat[by_Loop+7].Right_Above_Segment = Demo7Seg8[change_show][by_Loop];
        for(by_Loop = 0,by_Loop1=5;by_Loop < 6;by_Loop++,by_Loop1--)
            LCD.HT.IC4_Mat[by_Loop1].Right_Above_Segment = Demo7Seg6[change_show][by_Loop];
        //右下
        for(by_Loop = 0;by_Loop < 7;by_Loop++)
            LCD.HT.IC2_Mat[by_Loop+23].Right_Bottom_Segment = Demo7Seg7[change_show][by_Loop];
        for(by_Loop = 0;by_Loop < 7;by_Loop++)
            LCD.HT.IC2_Mat[by_Loop+15].Right_Bottom_Segment = Demo7Seg7[change_show][by_Loop];
        for(by_Loop = 0;by_Loop < 7;by_Loop++)
            LCD.HT.IC2_Mat[by_Loop+7].Right_Bottom_Segment = Demo7Seg7[change_show][by_Loop];
        for(by_Loop = 0,by_Loop1=6;by_Loop < 7;by_Loop++,by_Loop1--)
            LCD.HT.IC2_Mat[by_Loop1].Right_Bottom_Segment = Demo7Seg7x[change_show][by_Loop];
        //中?
        for(by_Loop = 0;by_Loop < 3;by_Loop++)
            LCD.HT.IC3_Seg[by_Loop].Left_Middle_Segment = Demo7Seg3T[change_show][by_Loop];
        for(by_Loop = 0;by_Loop < 3;by_Loop++)
            LCD.HT.IC3_Seg[by_Loop+3].Left_Middle_Segment = Demo7Seg3T[change_show][by_Loop];
        for(by_Loop = 0,by_Loop1=2;by_Loop < 3;by_Loop++,by_Loop1--)
            LCD.HT.IC4_Seg[by_Loop1].Right_Middle_Segment = Demo7Seg3T[change_show][by_Loop];
        for(by_Loop = 0,by_Loop1=5;by_Loop < 3;by_Loop++,by_Loop1--)
            LCD.HT.IC4_Seg[by_Loop1].Right_Middle_Segment = Demo7Seg3T[change_show][by_Loop];
        
        if(change_show == 0)
        {
            change_show++;
        }
        else
        {              
            change_show = 0;
        }
       
        for(by_Loop = 0;by_Loop < 23 ;by_Loop++)
        {
            w_Buffer =  LCD.HT.IC3_Mat[by_Loop].Left_Above_Segment;
            LCD.LcdTotal_Buffer[by_Loop+120].Com9_16 = w_Buffer;
        }
        for(by_Loop = 0;by_Loop < 31 ;by_Loop++)
        {
            w_Buffer =  LCD.HT.IC1_Mat[by_Loop].Left_Bottom_Segment ;
            w_Buffer2 =0;
            if(w_Buffer & BIT0) w_Buffer2=w_Buffer2|0x80;else ;
            if(w_Buffer & BIT1) w_Buffer2=w_Buffer2|0x40;else ;
            if(w_Buffer & BIT2) w_Buffer2=w_Buffer2|0x20;else ;
            if(w_Buffer & BIT3) w_Buffer2=w_Buffer2|0x10;else ;
            if(w_Buffer & BIT4) w_Buffer2=w_Buffer2|0x08;else ;
            if(w_Buffer & BIT5) w_Buffer2=w_Buffer2|0x04;else ;
            if(w_Buffer & BIT6) w_Buffer2=w_Buffer2|0x02;else ;
            if(w_Buffer & BIT7) w_Buffer2=w_Buffer2|0x01;else ;
            LCD.LcdTotal_Buffer[by_Loop].Com1_8 = w_Buffer2;
        }
        for(by_Loop = 0;by_Loop < 24 ;by_Loop++)
        {
            w_Buffer =  LCD.HT.IC4_Mat[by_Loop].Right_Above_Segment;
            LCD.LcdTotal_Buffer[by_Loop+180].Com9_16 = w_Buffer;
        }	
        for(by_Loop = 0;by_Loop < 30 ;by_Loop++)
        {
            w_Buffer =  LCD.HT.IC2_Mat[by_Loop].Right_Bottom_Segment;
            w_Buffer2 =0;
            if(w_Buffer & BIT0) w_Buffer2=w_Buffer2|0x80;else ;
            if(w_Buffer & BIT1) w_Buffer2=w_Buffer2|0x40;else ;
            if(w_Buffer & BIT2) w_Buffer2=w_Buffer2|0x20;else ;
            if(w_Buffer & BIT3) w_Buffer2=w_Buffer2|0x10;else ;
            if(w_Buffer & BIT4) w_Buffer2=w_Buffer2|0x08;else ;
            if(w_Buffer & BIT5) w_Buffer2=w_Buffer2|0x04;else ;
            if(w_Buffer & BIT6) w_Buffer2=w_Buffer2|0x02;else ;
            if(w_Buffer & BIT7) w_Buffer2=w_Buffer2|0x01;else ;
            LCD.LcdTotal_Buffer[by_Loop+60].Com1_8 = w_Buffer2;
        }	
        for(by_Loop = 0;by_Loop < 6 ;by_Loop++)
        {
            w_Buffer =  LCD.HT.IC3_Seg[by_Loop].Left_Middle_Segment;
            LCD.LcdTotal_Buffer[by_Loop+161].Com1_8 = w_Buffer;
        }	
        for(by_Loop = 0;by_Loop < 6 ;by_Loop++)
        {
            w_Buffer =  LCD.HT.IC4_Seg[by_Loop].Right_Middle_Segment;
            LCD.LcdTotal_Buffer[by_Loop+221].Com1_8 = w_Buffer;
        }	
    }
    
}


#endif

