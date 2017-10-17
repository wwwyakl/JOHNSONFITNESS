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
#define ADDRESS_HT1624              0x7A
#define ADDRESS_HT1621              0x70

#define CMD_DRIVE_MODE_cmd          0x82    //����ģʽ
#define Setting_Duty_Bias           0x0C    //duty 1/16��bias 1/5

#define CMD_SYSTEM_MODE_cmd         0x84    //ϵͳģʽ
#define Setting_Osc_Lcd_On          0x03    //Internal Osc on,LCD  on
#define Setting_Osc_Lcd_Off         0x00    //Internal Osc off,LCD  off

#define CMD_FRAME_FREQ_cmd          0x86    //֡Ƶ��ģʽ
#define Setting_Frame_Freq          0x00    //80Hz

#define CMD_BLINK_FREQ_cmd          0x88    //��˸Ƶ��ģʽ
#define Setting_Blink_Freq          0x00    //disable blink

/*Setting_IVA_Voltage������Ϊ0x30~0x3F������*/
#define CMD_IVA_cmd                 0x8A    //�ڲ���ѹ����(IVA) ��������
#define Setting_IVA_Voltage         0x20    //disable blink

#define CMD_WRIATE_DIS_DATA         0x80    //д��ʾ��������
#define CMD_DIS_START_ADDR          0x00    // д���ݵ���ʼ��ַ��0x00


#define MAX_CHIP_COUNT 4
#define PGE_BYTE_COUNT 120

/***************************************************************************//**
* @brief
*   HAL_HT16XX_Select_Chip
*   ��ʾоƬƬѡ
* @note
*   1��cs3~cs6ѡ��ÿһ��оƬ���磺
*      cs3~cs6:0001-->ѡ���һ��оƬ��IC1~IC4����
*      cs3~cs6:0010-->ѡ��ڶ���оƬ��IC5~IC8����
*   2��cs1~cs2ѡ��ǰһ��оƬ�е�ĳһ��оƬ���磺
*      cs1~cs2��00-->��ǰһ��оƬ�е�һ��
*      cs1~cs2��10-->��ǰһ��оƬ�е�����
*   3����ѡ���8��IC��������cs3~cs6��0010��cs1~cs2��11
* @param[in]��none
* @param[out]��none
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
*   ����HAL_16XX_I2C_Write_CMD����ʼ������оƬ
*
* @param[in]��none
* @param[out]��none
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
*   ��ʼ��CS output
* @note
*
*
* @param[in]��none
* @param[out]��none
******************************************************************************/
static void HAL_HT16CXX_Init_IO_Ports(void)
{
    /* cs3:ʹ�ܣ�����Ч  */
    HAL_Set_IO_Output(HT16XX_CS3_PORT,HT16XX_CS3_PIN);	
    HAL_Set_IO_High(HT16XX_CS3_PORT,HT16XX_CS3_PIN);	

    /* cs1��cs2��Ƭѡ  */
    HAL_Set_IO_Output(HT16XX_CS1_PORT,HT16XX_CS1_PIN);	
    HAL_Set_IO_Low(HT16XX_CS1_PORT,HT16XX_CS1_PIN);	

    HAL_Set_IO_Output(HT16XX_CS2_PORT,HT16XX_CS2_PIN);
    HAL_Set_IO_Low(HT16XX_CS2_PORT,HT16XX_CS2_PIN);
}

/***************************************************************************//**
* @brief
*   HAL_HT16XX_Inital
*   ��ʼ��IO,I2C ,����оƬ
* @note
*
*
* @param[in]��none
* @param[out]��none
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
*   1���ú����������Ϊ��һ��buffer��IC������buffer�а�������Ҫˢ�µ�ÿ��IC������
*   2��IC������������Ҫ�ӵ�ַ0x00��ʼ��
*   3����ʱĬ��IC�����ݳ���Ϊ60*16=120byte��
*
* @param[in]��UINT8 *pby_Lcd_Buffer,char IC_Count
* @param[out]��none
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
*   ����HAL_16XX_I2C_Write_CMD����ʼ������оƬ
*
* @param[in]��none
* @param[out]��none
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