#ifndef __HT16CXX_H__
#define __HT16CXX_H__

#include "HAL_Lib.h"

/***************************************************************************//**
* @brief
*   HT16CXX_Initial_IO 
*   ��ʼ��IO,I2C ,����оƬ
* @note
*   
*
* @param[in]��none
* @param[out]��none
******************************************************************************/
void HT16CXX_Initial_IO(void);

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
void HT16CXX_Updata_Data(UINT8 *pby_Lcd_Buffer,char IC_Count);

/***************************************************************************//**
* @brief
*   HT16CXX_Updata_Data2---->reeserved 
*
* @note
*   1���ú����������Ϊ�ṹ��HT_ShowData���͵ı���Disdata���ñ�������9���ֽڵ���
*   �ݳ��Ⱥ�9��buffer���ݵ�ַ��Ϣ��
*   2��Ҫ��Disdata.Displaydata[0]~Disdata.Displaydata[8]�ֱ�ΪIC0~IC8����ʾ���ݣ�
*   ͬʱҪ��д��Ӧ���ݵĳ�����Ϣ����Disdata.Length[0]~Disdata.Length[8]�С�
*   3�����û�����ݣ�����д0��bufferд��NULL��
*   4��display.c�и�����Ҫ����ȫ�ֱ���buffer���������������
*   // ����
*   UINT8 Dis_BufferIC1[120];
*   UINT8 Dis_BufferIC2[60];
*   UINT8 Dis_BufferIC1[90];
*   UINT8 Dis_BufferIC1[30];
*   
*   //��ֵ
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
*   //����
*   HAL_16XX_I2C_Write_Command1(ShowData_Buffer);
*
* @param[in]��HT_ShowData Disdata
* @param[out]��none
******************************************************************************/
void HT16CXX_Updata_Data2( HT_ShowData Disdata);


#endif
