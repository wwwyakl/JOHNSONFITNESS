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
//
//#define BIT0                (0x00000001)
//#define BIT1                (0x00000002)
//#define BIT2                (0x00000004)
//#define BIT3                (0x00000008)
//#define BIT4                (0x00000010)
//#define BIT5                (0x00000020)
//#define BIT6                (0x00000040)
//#define BIT7                (0x00000080)
//#define BIT8                (0x00000100)
//#define BIT9                (0x00000200)
//#define BITA                (0x00000400)
//#define BITB                (0x00000800)
//#define BITC                (0x00001000)
//#define BITD                (0x00002000)
//#define BITE                (0x00004000)
//#define BITF                (0x00008000)
/*chip configuration define*/
/* 1624 */
/*-----------------------------
Setting_Duty_Bias setting bits
-------------bit3-----bit1
------------duty1----duty0
1/4 duty---->0         0
1/8 duty---->0         1
1/16 duty--->1         X

-------------bit2-----bit0
------------Bias1----Bias0
1/3 duty---->0         0
1/4 duty---->0         1
1/5 duty---->1         X
----------------------------- */

#if defined(_4_16C24_Display_)

#define ADDRESS_HT1624              0x7A
#define ADDRESS_HT1621              0x70

#define CMD_DRIVE_MODE_cmd          0x82    //驱动模式
#define Setting_Duty_Bias           0x0C    //duty 1/16，bias 1/5

#define CMD_SYSTEM_MODE_cmd         0x84    //系统模式
#define Setting_Osc_Lcd_On          0x03    //Internal Osc on,LCD  on
#define Setting_Osc_Lcd_Off         0x00    //Internal Osc off,LCD  off

#define CMD_FRAME_FREQ_cmd          0x86    //帧频率模式
#define Setting_Frame_Freq          0x00    //80Hz

#define CMD_BLINK_FREQ_cmd          0x88    //闪烁频率模式
#define Setting_Blink_Freq          0x00    //disable blink

/*Setting_IVA_Voltage可设置为0x30~0x3F逐级增加*/
#define CMD_IVA_cmd                 0x8A    //内部电压调整(IVA) 设置命令
#define Setting_IVA_Voltage         0x20    //disable blink

#define CMD_WRIATE_DIS_DATA         0x80    //写显示数据命令
#define CMD_DIS_START_ADDR          0x00    // 写数据的起始地址，0x00


#define MAX_CHIP_COUNT 4
#define PGE_BYTE_COUNT 120

/***************************************************************************//**
* @brief
*   HAL_HT16XX_Select_Chip
*   显示芯片片选
* @note
*   1、cs3~cs6选择每一组芯片，如：
*      cs3~cs6:0001-->选择第一组芯片（IC1~IC4）；
*      cs3~cs6:0010-->选择第二组芯片（IC5~IC8）；
*   2、cs1~cs2选择当前一组芯片中的某一颗芯片，如：
*      cs1~cs2：00-->当前一组芯片中第一个
*      cs1~cs2：10-->当前一组芯片中第三个
*   3、如选择第8个IC，则配置cs3~cs6：0010，cs1~cs2：11
* @param[in]：none
* @param[out]：none
******************************************************************************/
static void HAL_HT16CXX_Select_Chip(UINT8 number)
{
    switch(number)
    {
        case 1:
            HAL_Set_IO_Low(HT16XX_CS3_PORT,HT16XX_CS3_PIN);	
            asm("nop");
            asm("nop");
            HAL_Set_IO_Low(HT16XX_CS1_PORT,HT16XX_CS1_PIN);	
            HAL_Set_IO_Low(HT16XX_CS2_PORT,HT16XX_CS2_PIN);	
            asm("nop");
            asm("nop");
            break;
        case 2:
            HAL_Set_IO_Low(HT16XX_CS3_PORT,HT16XX_CS3_PIN);	
            asm("nop");
            asm("nop");
            HAL_Set_IO_High(HT16XX_CS1_PORT,HT16XX_CS1_PIN);	
            HAL_Set_IO_Low(HT16XX_CS2_PORT,HT16XX_CS2_PIN);	
            asm("nop");
            asm("nop");
            break;
        case 3:
            HAL_Set_IO_Low(HT16XX_CS3_PORT,HT16XX_CS3_PIN);	
            asm("nop");
            asm("nop");
            HAL_Set_IO_Low(HT16XX_CS1_PORT,HT16XX_CS1_PIN);	
            HAL_Set_IO_High(HT16XX_CS2_PORT,HT16XX_CS2_PIN);	
            asm("nop");
            asm("nop");
            break;
        case 4:
            HAL_Set_IO_Low(HT16XX_CS3_PORT,HT16XX_CS3_PIN);	
            asm("nop");
            asm("nop");
            HAL_Set_IO_High(HT16XX_CS1_PORT,HT16XX_CS1_PIN);	
            HAL_Set_IO_High(HT16XX_CS2_PORT,HT16XX_CS2_PIN);	
            asm("nop");
            asm("nop");
            break;
        default:
            HAL_Set_IO_High(HT16XX_CS3_PORT,HT16XX_CS3_PIN);	
            HAL_Set_IO_High(HT16XX_CS1_PORT,HT16XX_CS1_PIN);	
            HAL_Set_IO_High(HT16XX_CS2_PORT,HT16XX_CS2_PIN);	
            break;
    }
}
/***************************************************************************//**
* @brief
*   HAL_16XX_I2C_Config_Chip
* @note
*   调用HAL_16XX_I2C_Write_CMD来初始化配置芯片
*
* @param[in]：none
* @param[out]：none
******************************************************************************/
static void HAL_16CXX_I2C_Config_Chip(void)
{
    UINT8 i = 0 ;
    for(i=1;i<=MAX_CHIP_COUNT;i++)
    {
        HAL_HT16CXX_Select_Chip(i);

        HAL_16CXX_I2C_Write_CMD(ADDRESS_HT1624,CMD_DRIVE_MODE_cmd,Setting_Duty_Bias);
        HAL_16CXX_I2C_Write_CMD(ADDRESS_HT1624,CMD_FRAME_FREQ_cmd,Setting_Frame_Freq);
        HAL_16CXX_I2C_Write_CMD(ADDRESS_HT1624,CMD_IVA_cmd,Setting_IVA_Voltage);
        HAL_16CXX_I2C_Write_CMD(ADDRESS_HT1624,CMD_BLINK_FREQ_cmd,Setting_Blink_Freq);
        HAL_16CXX_I2C_Write_CMD(ADDRESS_HT1624,CMD_SYSTEM_MODE_cmd,Setting_Osc_Lcd_Off);
    }
}
/***************************************************************************//**
* @brief
*   HAL_HT16XX_Init_IO_Ports
*   初始化CS output
* @note
*
*
* @param[in]：none
* @param[out]：none
******************************************************************************/
static void HAL_HT16CXX_Init_IO_Ports(void)
{
    /* cs3:使能，低有效  */
    HAL_Set_IO_Output(HT16XX_CS3_PORT,HT16XX_CS3_PIN);	
    HAL_Set_IO_High(HT16XX_CS3_PORT,HT16XX_CS3_PIN);	

    /* cs1：cs2：片选  */
    HAL_Set_IO_Output(HT16XX_CS1_PORT,HT16XX_CS1_PIN);	
    HAL_Set_IO_Low(HT16XX_CS1_PORT,HT16XX_CS1_PIN);	

    HAL_Set_IO_Output(HT16XX_CS2_PORT,HT16XX_CS2_PIN);
    HAL_Set_IO_Low(HT16XX_CS2_PORT,HT16XX_CS2_PIN);

    /* cs4  */
//    HAL_Set_IO_Output(HT16XX_CS4_PORT,HT16XX_CS4_PIN);	
//
//    /* cs5  */
//    HAL_Set_IO_Output(HT16XX_CS5_PORT,HT16XX_CS5_PIN);
//
//    /* cs6  */
//    HAL_Set_IO_Output(HT16XX_CS6_PORT,HT16XX_CS6_PIN);	
}

/***************************************************************************//**
* @brief
*   HAL_HT16XX_Inital
*   初始化IO,I2C ,配置芯片
* @note
*
*
* @param[in]：none
* @param[out]：none
******************************************************************************/
void HT16CXX_Initial_IO(void)
{
    HAL_HT16CXX_Init_IO_Ports();
    HAL_16CXX_Init_I2C();
    HAL_16CXX_I2C_Config_Chip();
}
/***************************************************************************//**
* @brief
*   HT16CXX_Updata_Data
*
* @note
*   1、该函数传入参数为结一个buffer和IC数量，buffer中包含所需要刷新的每个IC的数据
*   2、IC的数据内容需要从地址0x00开始。
*   3、暂时默认IC的数据长度为60*16=120byte，
*
* @param[in]：UINT8 *pby_Lcd_Buffer,char IC_Count
* @param[out]：none
******************************************************************************/
//UINT8 TEST_RECEIVE1[150];
void HT16CXX_Updata_Data(UINT8 *pby_Lcd_Buffer,char IC_Count)
{
  UINT8 i = 0;
  
  for(i=0;i<IC_Count;i++)
  {
    HAL_HT16CXX_Select_Chip(i+1);
    HAL_16CXX_I2C_Write_CMD(ADDRESS_HT1624,CMD_DRIVE_MODE_cmd,Setting_Duty_Bias);
    HAL_16CXX_I2C_Write_CMD(ADDRESS_HT1624,CMD_FRAME_FREQ_cmd,Setting_Frame_Freq);
    HAL_16CXX_I2C_Write_CMD(ADDRESS_HT1624,CMD_IVA_cmd,Setting_IVA_Voltage);
    HAL_16CXX_I2C_Write_CMD(ADDRESS_HT1624,CMD_BLINK_FREQ_cmd,Setting_Blink_Freq);
    HAL_16CXX_I2C_Write_ShowData(ADDRESS_HT1624,CMD_WRIATE_DIS_DATA,\
      CMD_DIS_START_ADDR,pby_Lcd_Buffer+i*PGE_BYTE_COUNT,PGE_BYTE_COUNT);
    HAL_16CXX_I2C_Write_CMD(ADDRESS_HT1624,CMD_SYSTEM_MODE_cmd,Setting_Osc_Lcd_On);
  }
}
/***************************************************************************//**
* @brief
*   HAL_16XX_I2C_Config_Chip
* @note
*   调用HAL_16XX_I2C_Write_CMD来初始化配置芯片
*
* @param[in]：none
* @param[out]：none
******************************************************************************/
void HAL_16CXX_Set_LCD_OFF(void)
{
    UINT8 i = 0 ;
    for(i=1;i<=MAX_CHIP_COUNT;i++)
    {
        HAL_HT16CXX_Select_Chip(i);
        HAL_16CXX_I2C_Write_CMD(ADDRESS_HT1624,CMD_SYSTEM_MODE_cmd,Setting_Osc_Lcd_Off);
    }
}

//// below reserved
//void HT16CXX_Updata_Data2( HT_ShowData Disdata)
//{
//    UINT8 Cmd_Data[2]={CMD_WRIATE_DIS_DATA,CMD_DIS_START_ADDR};
//    UINT8 i = 1;
//    for(i=1;i<MAX_CHIP_COUNT;i++)
//    {
//        if(Disdata.Length[i] == 0) break;
//        HAL_HT16CXX_Select_Chip(i);
//
//        I2C_TransferSeq_TypeDef i2c_data;
//        i2c_data.addr = ADDRESS_HT1624;
//        i2c_data.flags = I2C_FLAG_WRITE_WRITE;
//
//        i2c_data.buf[0].data = Cmd_Data;
//        i2c_data.buf[0].len = 2;
//        i2c_data.buf[1].data = Disdata.Displaydata[i];
//        i2c_data.buf[1].len = Disdata.Length[i];
//
//        trans_return_value = I2C_TransferInit(I2C1,&i2c_data);
//
//        while (trans_return_value == i2cTransferInProgress)
//        {
//            trans_return_value = I2C_Transfer(I2C1);
//        }
//        //read config value  used for debug
////        i2c_data.flags = I2C_FLAG_WRITE_READ;
////
////        if(i==0)
////            i2c_data.buf[1].data = &TEST_RECEIVE1[0];
////        else
////            i2c_data.buf[1].data = &TEST_RECEIVE2[0];
////
////        trans_return_value = I2C_TransferInit(I2C1,&i2c_data);
////        while (trans_return_value == i2cTransferInProgress)
////        {
////            trans_return_value = I2C_Transfer(I2C1);
////        }
////        asm("nop");
//    }
//}

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
//
//#define BIT0                (0x00000001)
//#define BIT1                (0x00000002)
//#define BIT2                (0x00000004)
//#define BIT3                (0x00000008)
//#define BIT4                (0x00000010)
//#define BIT5                (0x00000020)
//#define BIT6                (0x00000040)
//#define BIT7                (0x00000080)
//#define BIT8                (0x00000100)
//#define BIT9                (0x00000200)
//#define BITA                (0x00000400)
//#define BITB                (0x00000800)
//#define BITC                (0x00001000)
//#define BITD                (0x00002000)
//#define BITE                (0x00004000)
//#define BITF                (0x00008000)
/*chip configuration define*/
/* 1624 */
/*-----------------------------
Setting_Duty_Bias setting bits
-------------bit3-----bit1
------------duty1----duty0
1/4 duty---->0         0
1/8 duty---->0         1
1/16 duty--->1         X

-------------bit2-----bit0
------------Bias1----Bias0
1/3 duty---->0         0
1/4 duty---->0         1
1/5 duty---->1         X
----------------------------- */

#elif defined(_6_16C24_Display_)

#define ADDRESS_HT1624              0x7A
#define ADDRESS_HT1621              0x70

#define CMD_DRIVE_MODE_cmd          0x82    //驱动模式
#define Setting_Duty_Bias           0x0C    //duty 1/16，bias 1/5

#define CMD_SYSTEM_MODE_cmd         0x84    //系统模式
#define Setting_Osc_Lcd_On          0x03    //Internal Osc on,LCD  on
#define Setting_Osc_Lcd_Off         0x00    //Internal Osc off,LCD  off

#define CMD_FRAME_FREQ_cmd          0x86    //帧频率模式
#define Setting_Frame_Freq          0x00    //80Hz

#define CMD_BLINK_FREQ_cmd          0x88    //闪烁频率模式
#define Setting_Blink_Freq          0x00    //disable blink

/*Setting_IVA_Voltage可设置为0x30~0x3F逐级增加*/
#define CMD_IVA_cmd                 0x8A    //内部电压调整(IVA) 设置命令
#define Setting_IVA_Voltage         0x20    //disable blink

#define CMD_WRIATE_DIS_DATA         0x80    //写显示数据命令
#define CMD_DIS_START_ADDR          0x00    // 写数据的起始地址，0x00


#define MAX_CHIP_COUNT 6
#define PGE_BYTE_COUNT 120

/***************************************************************************//**
* @brief
*   HAL_HT16XX_Select_Chip
*   显示芯片片选
* @note
*   1、cs3~cs6选择每一组芯片，如：
*      cs3~cs6:0001-->选择第一组芯片（IC1~IC4）；
*      cs3~cs6:0010-->选择第二组芯片（IC5~IC8）；
*   2、cs1~cs2选择当前一组芯片中的某一颗芯片，如：
*      cs1~cs2：00-->当前一组芯片中第一个
*      cs1~cs2：10-->当前一组芯片中第三个
*   3、如选择第8个IC，则配置cs3~cs6：0010，cs1~cs2：11
* @param[in]：none
* @param[out]：none
******************************************************************************/
static void HAL_HT16CXX_Select_Chip(UINT8 number)
{
    switch(number)
    {
        case 1:
		{
			HAL_Set_IO_High(HT16XX_CS4_PORT,HT16XX_CS4_PIN);
			HAL_Set_IO_Low(HT16XX_CS3_PORT,HT16XX_CS3_PIN);	
			asm("nop");
			asm("nop");
			HAL_Set_IO_Low(HT16XX_CS1_PORT,HT16XX_CS1_PIN);	
			HAL_Set_IO_Low(HT16XX_CS2_PORT,HT16XX_CS2_PIN);	
			asm("nop");
			asm("nop");
			break;
		}
        case 2:
		{
			HAL_Set_IO_High(HT16XX_CS4_PORT,HT16XX_CS4_PIN);
			HAL_Set_IO_Low(HT16XX_CS3_PORT,HT16XX_CS3_PIN);	
			asm("nop");
			asm("nop");
			HAL_Set_IO_High(HT16XX_CS1_PORT,HT16XX_CS1_PIN);	
			HAL_Set_IO_Low(HT16XX_CS2_PORT,HT16XX_CS2_PIN);	
			asm("nop");
			asm("nop");
			break;
		}
        case 3:
		{
			HAL_Set_IO_High(HT16XX_CS4_PORT,HT16XX_CS4_PIN);
			HAL_Set_IO_Low(HT16XX_CS3_PORT,HT16XX_CS3_PIN);	
			asm("nop");
			asm("nop");
			HAL_Set_IO_Low(HT16XX_CS1_PORT,HT16XX_CS1_PIN);	
			HAL_Set_IO_High(HT16XX_CS2_PORT,HT16XX_CS2_PIN);	
			asm("nop");
			asm("nop");
			break;
		}
        case 4:
		{
			HAL_Set_IO_High(HT16XX_CS4_PORT,HT16XX_CS4_PIN);
			HAL_Set_IO_Low(HT16XX_CS3_PORT,HT16XX_CS3_PIN);	
			asm("nop");
			asm("nop");
			HAL_Set_IO_High(HT16XX_CS1_PORT,HT16XX_CS1_PIN);	
			HAL_Set_IO_High(HT16XX_CS2_PORT,HT16XX_CS2_PIN);	
			asm("nop");
			asm("nop");
			break;
		}
		case 5:
		{
			HAL_Set_IO_High(HT16XX_CS3_PORT,HT16XX_CS3_PIN);	
			HAL_Set_IO_Low(HT16XX_CS4_PORT,HT16XX_CS4_PIN);
			asm("nop");
			asm("nop");
			HAL_Set_IO_High(HT16XX_CS1_PORT,HT16XX_CS1_PIN);	
			HAL_Set_IO_High(HT16XX_CS2_PORT,HT16XX_CS2_PIN);	
			asm("nop");
			asm("nop");
			break;
		}
		case 6:
		{
			HAL_Set_IO_High(HT16XX_CS3_PORT,HT16XX_CS3_PIN);	
			HAL_Set_IO_Low(HT16XX_CS4_PORT,HT16XX_CS4_PIN);
			asm("nop");
			asm("nop");
			HAL_Set_IO_Low(HT16XX_CS1_PORT,HT16XX_CS1_PIN);	
			HAL_Set_IO_High(HT16XX_CS2_PORT,HT16XX_CS2_PIN);	
			asm("nop");
			asm("nop");
			break;
		}
        default:
		{
			HAL_Set_IO_High(HT16XX_CS3_PORT,HT16XX_CS3_PIN);	
			HAL_Set_IO_High(HT16XX_CS4_PORT,HT16XX_CS4_PIN);
			HAL_Set_IO_High(HT16XX_CS1_PORT,HT16XX_CS1_PIN);	
			HAL_Set_IO_High(HT16XX_CS2_PORT,HT16XX_CS2_PIN);	
			break;
		}
    }
}
/***************************************************************************//**
* @brief
*   HAL_16XX_I2C_Config_Chip
* @note
*   调用HAL_16XX_I2C_Write_CMD来初始化配置芯片
*
* @param[in]：none
* @param[out]：none
******************************************************************************/
static void HAL_16CXX_I2C_Config_Chip(void)
{
    UINT8 i = 0 ;
    for(i=1;i<=MAX_CHIP_COUNT;i++)
    {
        HAL_HT16CXX_Select_Chip(i);

        HAL_16CXX_I2C_Write_CMD(ADDRESS_HT1624,CMD_DRIVE_MODE_cmd,Setting_Duty_Bias);
        HAL_16CXX_I2C_Write_CMD(ADDRESS_HT1624,CMD_FRAME_FREQ_cmd,Setting_Frame_Freq);
        HAL_16CXX_I2C_Write_CMD(ADDRESS_HT1624,CMD_IVA_cmd,Setting_IVA_Voltage);
        HAL_16CXX_I2C_Write_CMD(ADDRESS_HT1624,CMD_BLINK_FREQ_cmd,Setting_Blink_Freq);
        HAL_16CXX_I2C_Write_CMD(ADDRESS_HT1624,CMD_SYSTEM_MODE_cmd,Setting_Osc_Lcd_Off);
    }
}
/***************************************************************************//**
* @brief
*   HAL_HT16XX_Init_IO_Ports
*   初始化CS output
* @note
*
*
* @param[in]：none
* @param[out]：none
******************************************************************************/
static void HAL_HT16CXX_Init_IO_Ports(void)
{
	    /* cs4  */
    HAL_Set_IO_Output(HT16XX_CS4_PORT,HT16XX_CS4_PIN);	
	HAL_Set_IO_High(HT16XX_CS4_PORT,HT16XX_CS4_PIN);	
	
    /* cs3:使能，低有效  */
    HAL_Set_IO_Output(HT16XX_CS3_PORT,HT16XX_CS3_PIN);	
    HAL_Set_IO_High(HT16XX_CS3_PORT,HT16XX_CS3_PIN);	

    /* cs1：cs2：片选  */
    HAL_Set_IO_Output(HT16XX_CS1_PORT,HT16XX_CS1_PIN);	
    HAL_Set_IO_Low(HT16XX_CS1_PORT,HT16XX_CS1_PIN);	

    HAL_Set_IO_Output(HT16XX_CS2_PORT,HT16XX_CS2_PIN);
    HAL_Set_IO_Low(HT16XX_CS2_PORT,HT16XX_CS2_PIN);



    /* cs4  */
//    HAL_Set_IO_Output(HT16XX_CS4_PORT,HT16XX_CS4_PIN);	
//
//    /* cs5  */
//    HAL_Set_IO_Output(HT16XX_CS5_PORT,HT16XX_CS5_PIN);
//
//    /* cs6  */
//    HAL_Set_IO_Output(HT16XX_CS6_PORT,HT16XX_CS6_PIN);	
}

/***************************************************************************//**
* @brief
*   HAL_HT16XX_Inital
*   初始化IO,I2C ,配置芯片
* @note
*
*
* @param[in]：none
* @param[out]：none
******************************************************************************/
void HT16CXX_Initial_IO(void)
{
    HAL_HT16CXX_Init_IO_Ports();
    HAL_16CXX_Init_I2C();
    HAL_16CXX_I2C_Config_Chip();
}
/***************************************************************************//**
* @brief
*   HT16CXX_Updata_Data
*
* @note
*   1、该函数传入参数为结一个buffer和IC数量，buffer中包含所需要刷新的每个IC的数据
*   2、IC的数据内容需要从地址0x00开始。
*   3、暂时默认IC的数据长度为60*16=120byte，
*
* @param[in]：UINT8 *pby_Lcd_Buffer,char IC_Count
* @param[out]：none
******************************************************************************/
//UINT8 TEST_RECEIVE1[150];
void HT16CXX_Updata_Data(UINT8 *pby_Lcd_Buffer,char IC_Count)
{
  UINT8 i = 0;
  
  for(i=0;i<IC_Count;i++)
  {
    HAL_HT16CXX_Select_Chip(i+1);
    HAL_16CXX_I2C_Write_CMD(ADDRESS_HT1624,CMD_DRIVE_MODE_cmd,Setting_Duty_Bias);
    HAL_16CXX_I2C_Write_CMD(ADDRESS_HT1624,CMD_FRAME_FREQ_cmd,Setting_Frame_Freq);
    HAL_16CXX_I2C_Write_CMD(ADDRESS_HT1624,CMD_IVA_cmd,Setting_IVA_Voltage);
    HAL_16CXX_I2C_Write_CMD(ADDRESS_HT1624,CMD_BLINK_FREQ_cmd,Setting_Blink_Freq);
    HAL_16CXX_I2C_Write_ShowData(ADDRESS_HT1624,CMD_WRIATE_DIS_DATA,\
    CMD_DIS_START_ADDR,pby_Lcd_Buffer+i*PGE_BYTE_COUNT,PGE_BYTE_COUNT);
    HAL_16CXX_I2C_Write_CMD(ADDRESS_HT1624,CMD_SYSTEM_MODE_cmd,Setting_Osc_Lcd_On);
  }
}

//// below reserved
//void HT16CXX_Updata_Data2( HT_ShowData Disdata)
//{
//    UINT8 Cmd_Data[2]={CMD_WRIATE_DIS_DATA,CMD_DIS_START_ADDR};
//    UINT8 i = 1;
//    for(i=1;i<MAX_CHIP_COUNT;i++)
//    {
//        if(Disdata.Length[i] == 0) break;
//        HAL_HT16CXX_Select_Chip(i);
//
//        I2C_TransferSeq_TypeDef i2c_data;
//        i2c_data.addr = ADDRESS_HT1624;
//        i2c_data.flags = I2C_FLAG_WRITE_WRITE;
//
//        i2c_data.buf[0].data = Cmd_Data;
//        i2c_data.buf[0].len = 2;
//        i2c_data.buf[1].data = Disdata.Displaydata[i];
//        i2c_data.buf[1].len = Disdata.Length[i];
//
//        trans_return_value = I2C_TransferInit(I2C1,&i2c_data);
//
//        while (trans_return_value == i2cTransferInProgress)
//        {
//            trans_return_value = I2C_Transfer(I2C1);
//        }
//        //read config value  used for debug
////        i2c_data.flags = I2C_FLAG_WRITE_READ;
////
////        if(i==0)
////            i2c_data.buf[1].data = &TEST_RECEIVE1[0];
////        else
////            i2c_data.buf[1].data = &TEST_RECEIVE2[0];
////
////        trans_return_value = I2C_TransferInit(I2C1,&i2c_data);
////        while (trans_return_value == i2cTransferInProgress)
////        {
////            trans_return_value = I2C_Transfer(I2C1);
////        }
////        asm("nop");
//    }
//}

#endif