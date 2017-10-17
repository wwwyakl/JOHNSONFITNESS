/*
 * Function: Beep_hardware_Control
 *
 * Description: Beep_Sound
 *
 * New Design: Version: S001
 *             By:
 *             Date: 2004/12/3
 *
 * Use Region:
 *
 * Modify : Version: S003
 *          By:     Zhao Xiaodong
 *          Date:   2008.11.20
 *          Content:    HardWare Function
 *          Interrupt :
*/

/************************************************************************************/
#include "HT16CXX.h"
/************************************************************************************/


/***********************************/
#define SYS_ENABLE         0X01
#define LCD_ON             0x03
#define RC_256K            0x18
#define BIAS3              0x29
/***********************************/

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

void HT1621_Updata_Data_One(char by_Index,char *pby_Buf);
void HT1621_Write_Command(char by_IC, char by_Dat);
void HT1621_Initial_Command(char by_IC);

void HT1621_Initial_Command_First(char by_IC)
{
    HT1621_Write_Command(by_IC,0);
    HT1621_Write_Command(by_IC,SYS_ENABLE);
    HT1621_Write_Command(by_IC,LCD_ON);
    HT1621_Write_Command(by_IC,RC_256K);	//Default Value is RC_256K
    HT1621_Write_Command(by_IC,BIAS3);
}
/*------------------------------------------
Function Contents: 虫1621﹍て(ず场ㄧ计)
--------------------------------------------
Use Regional : HT1621.c
--------------------------------------------
Input:  Name    Type     Min     Max
  1  by_IC      char      0       2
--------------------------------------------
Output:  N/A
--------------------------------------------*/
void HT1621_Initial_Command(char by_IC)
{
    HT1621_Write_Command(by_IC,SYS_ENABLE);
    HT1621_Write_Command(by_IC,LCD_ON);
    HT1621_Write_Command(by_IC,RC_256K);	//Default Value is RC_256K
    HT1621_Write_Command(by_IC,BIAS3);
}

/*------------------------------------------
Function Contents: 计沮穝ㄧ计
--------------------------------------------
Use Regional : Display.c
--------------------------------------------
Input:  Name            Type     Min   Max
  1  pby_Lcd_Buffer     char *    0    oxffff
  2  by_1621_Count      char      0     2
--------------------------------------------
Output:  N/A
--------------------------------------------*/
void HT1621_Updata_Data(char *pby_Lcd_Buffer,char by_1621_Count)     //*************
{
    char by_IC_Index;
    char by_Flag = by_1621_Count&0x80;
    by_1621_Count = by_1621_Count&0x7F;
    for(by_IC_Index = 0;by_IC_Index < by_1621_Count;by_IC_Index++)
    {
        HT1621_Updata_Data_One(by_IC_Index+by_Flag,pby_Lcd_Buffer);
    }
}

/*------------------------------------------
Function Contents: 虫1621计沮穝ㄧ计(ず场ㄧ计)
--------------------------------------------
Use Regional : HT1621.c
--------------------------------------------
Input:  Name        Type     Min   Max
  1  pby_Buf        char *    0    oxffff
  2  by_IC          char      0     2
--------------------------------------------
Output:  N/A
--------------------------------------------*/
void HT1621_Updata_Data_One(char by_IC,char *pby_Buf)
{
    char by_Out,by_Index = 0,by_Loop,by_MAX;

    if(by_IC&0x80)
		HT1621_Initial_Command(by_IC&0x7F);
    by_IC &= 0x7f;
    HW_HT162x_Set_High_CS(by_IC,0);
    
    HW_HT162x_Set_High_Data(1);
    HW_HT162x_Set_High_WR(0);
    HW_HT162x_Set_High_WR(1);

    HW_HT162x_Set_High_Data(0);
    HW_HT162x_Set_High_WR(0);
    HW_HT162x_Set_High_WR(1);

    HW_HT162x_Set_High_Data(1);
    HW_HT162x_Set_High_WR(0);
    HW_HT162x_Set_High_WR(1);

    
    by_MAX = 7;
    
	
    for(by_Loop=0; by_Loop<by_MAX; by_Loop++) //FROM THE FIRST ADDRESS
    {
        HW_HT162x_Set_High_Data(0);
        HW_HT162x_Set_High_WR(0);
        HW_HT162x_Set_High_WR(1);
    }
   for(by_Index=0;by_Index<64;by_Index++)
    {
      by_Out = pby_Buf[by_IC*64+by_Index];//
       for(by_Loop = 0;by_Loop < 8; by_Loop++)
       {
          if (by_Out & BIT7)
          {
              HW_HT162x_Set_High_Data(1);
          }
          else
          {
              HW_HT162x_Set_High_Data(0);
          }
          HW_HT162x_Set_High_WR(0);
          HW_HT162x_Set_High_WR(1);
          by_Out = by_Out << 1;
       }
    }
    HW_HT162x_Set_High_CS(by_IC,1);
}

/*------------------------------------------
Function Contents: 1621糶ㄧ计(ず场ㄧ计)
--------------------------------------------
Use Regional : HT1621.c
--------------------------------------------
Input:  Name        Type     Min   Max
  1  by_Dat         char     0     255
  2  by_IC          char      0     2
--------------------------------------------
Output:  N/A
--------------------------------------------*/
void HT1621_Write_Command(char by_IC ,char by_Dat)
{
    char by_Loop;
    char by_Out;

    HW_HT162x_Set_High_CS(by_IC,0);

    HW_HT162x_Set_High_Data(1);  // 1
    HW_HT162x_Set_High_WR(0);
    HW_HT162x_Set_High_WR(1);

    HW_HT162x_Set_High_Data(0); // 0
    HW_HT162x_Set_High_WR(0);
    HW_HT162x_Set_High_WR(1);

    HW_HT162x_Set_High_Data(0); //0
    HW_HT162x_Set_High_WR(0);
    HW_HT162x_Set_High_WR(1);

    by_Out = by_Dat;
    for(by_Loop=0; by_Loop<8; by_Loop++)
    {
        if(by_Out & BIT7)
        {
            HW_HT162x_Set_High_Data(1);
        }
        else
        {
            HW_HT162x_Set_High_Data(0);
        }
        HW_HT162x_Set_High_WR(0);
        HW_HT162x_Set_High_WR(1);
        by_Out = by_Out << 1;
    }
    HW_HT162x_Set_High_Data(0);
    HW_HT162x_Set_High_WR(0);
    HW_HT162x_Set_High_WR(1);

    HW_HT162x_Set_High_CS(by_IC,1);
}



