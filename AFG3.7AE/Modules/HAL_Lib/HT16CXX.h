#ifndef __HT16CXX_H__
#define __HT16CXX_H__

#include "HAL_Lib.h"

/***************************************************************************//**
* @brief
*   HT16CXX_Initial_IO 
*   初始化IO,I2C ,配置芯片
* @note
*   
*
* @param[in]：none
* @param[out]：none
******************************************************************************/
void HT16CXX_Initial_IO(void);

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
void HT16CXX_Updata_Data(UINT8 *pby_Lcd_Buffer,char IC_Count);

/***************************************************************************//**
* @brief
*   HT16CXX_Updata_Data2---->reeserved 
*
* @note
*   1、该函数传入参数为结构体HT_ShowData类型的变量Disdata，该变量包含9个字节的数
*   据长度和9个buffer数据地址信息。
*   2、要求Disdata.Displaydata[0]~Disdata.Displaydata[8]分别为IC0~IC8的显示数据，
*   同时要填写对应数据的长度信息放在Disdata.Length[0]~Disdata.Length[8]中。
*   3、如果没有数据，长度写0，buffer写“NULL”
*   4、display.c中根据需要定义全局变量buffer，数量因需而定：
*   // 定义
*   UINT8 Dis_BufferIC1[120];
*   UINT8 Dis_BufferIC2[60];
*   UINT8 Dis_BufferIC1[90];
*   UINT8 Dis_BufferIC1[30];
*   
*   //赋值
*   ShowData_Buffer.Length[0] = sizeof(Dis_BufferIC1) ;
*   ShowData_Buffer.Displaydata[0] = Dis_BufferIC1;
*    
*   ShowData_Buffer.Length[1] = sizeof(Dis_BufferIC2)  ;
*   ShowData_Buffer.Displaydata[1] = Dis_BufferIC2;  
*    
*   ShowData_Buffer.Length[2] = sizeof(Dis_BufferIC3) ;
*   ShowData_Buffer.Displaydata[2] = Dis_BufferIC1;
*    
*   ShowData_Buffer.Length[3] = sizeof(Dis_BufferIC4) ;
*   ShowData_Buffer.Displaydata[3] = Dis_BufferIC1;
*
*   ShowData_Buffer.Length[4] = 0 ;
*   ShowData_Buffer.Displaydata[3] = NULL;
*    
*   ShowData_Buffer.Length[5] = 0 ;
*   ShowData_Buffer.Displaydata[3] = NULL;
*    
*   ShowData_Buffer.Length[6] = 0 ;
*   ShowData_Buffer.Displaydata[3] = NULL;
*   
*   ShowData_Buffer.Length[7] = 0 ;
*   ShowData_Buffer.Displaydata[3] = NULL;
*    
*   ShowData_Buffer.Length[8] = 0 ;
*   ShowData_Buffer.Displaydata[3] = NULL;
*
*   //调用
*   HAL_16XX_I2C_Write_Command1(ShowData_Buffer);
*
* @param[in]：HT_ShowData Disdata
* @param[out]：none
******************************************************************************/
void HT16CXX_Updata_Data2( HT_ShowData Disdata);


#endif
