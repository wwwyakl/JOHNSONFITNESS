/***************************************************************************//**
* @file
* @brief HAL(Hardware Abstraction Layer) Library
* @author 
* @version 1.0.0
*
******************************************************************************/

#ifndef _HAL_LIB_H_
#define _HAL_LIB_H_
//EP
#define VR2_TOP	2120//2500 //4096 12bit  2786  
#define VR2_Bottom	 210//100                        
/***************************************************************************//**
* @Header files
*   
******************************************************************************/
#include "efm32lg380f256.h"
#include "efm32.h"
#include "em_int.h"
#include "em_msc.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "em_letimer.h"
#include "em_leuart.h"
#include "em_adc.h"
#include "em_wdog.h"
#include "em_i2c.h"
#include "em_usb.h"
#include "em_emu.h"
#include "em_timer.h"
#include "dmactrl.h"
#include "em_dma.h"
#include "em_assert.h"
#include "em_dac.h"
#include "em_prs.h"
#include "Global_Config.h"

#include "JIStypes.h"
#include "HAL_IO_define.h"

//#define TM
//#define EP

#define HAL_VISION_CODE "1.0.1"
/***************************************************************************//**
* @ Micro difine
*   
******************************************************************************/
/*WDT config relatived*/
#define DEBUG
//#define OUTPUT_DEBUG
/*RF module data transmit and receive with DMA + hardware SPI */
#define DMA_SPI
#undef DMA_SPI



typedef enum tag_PIN_INT_IDX
{
    PIN_INT_I2C=0,
    PIN_INT_HR,
    PIN_INT_POLAR,
    PIN_INT_GDO0,
    PIN_INT_GDO2, 
    ALL_INTIDXS
}PIN_INT_IDX;
/***************************************************************************//**
* @ enum define,������ UART_TX��UART_RX��GPIO_ODD��GPIO_EVEN�ȵ��ж�ע������
*
******************************************************************************/

typedef enum tag_INTERRUPT_ISR
{
  MS_ISR = 0,
  UART_TX_ISR,
  UART_RX_ISR,
  GPIO_ODD_ISR,
  GPIO_EVEN_ISR,
  UART0_TX_ISR,
  UART0_RX_ISR,
  UART1_TX_ISR,
  UART1_RX_ISR,
  ALL_ISRS
}INTERRUPT_ISR;

/***************************************************************************//**
* @ enum define,Ϊ��������ģ����Ҫ��1Ms ISR��ע�ắ���ṩ����
*
******************************************************************************/
typedef enum tag_1MSISR_IDX
{

    BEEP_ISR_IDX = 0,
	ERP_ISR_IDX,
    HAL_KeyMatrix_1ms,
    HAL_KeyMatrix_20ms,
    HAL_KeyI2C_1ms,
	Heart_ISR1_IDX,
	Heart_ISR2_IDX,
	Heart_ISR3_IDX,
	Heart_ISR4_IDX,
	Keyboard_ISR_IDX,
    TIMER_ISR_IDX,
    LED_CTRL_ISR_IDX,
    MOTOR_CTRL_ISR_IDX,
    LASER_SCAN_ISR_IDX,
    SDCARD_ISR_IDX,
    KEY_SCAN_IDX,
    WORK_TIMER_IDX,
    LCB_ISR_IDX,
    Err_ISR_IDX,
    DATA_ISR_IDX,
    RF_1Ms_TimeOut_ISR,
    WIFI_ISR_IDX,
    BOR_ISR_IDX,
    Keypad_ISR_IDX,
    PRG_ISR_IDX,
    ENG_ISR_IDX,
    BURN_ISR_IDX,
    BT_ISR_IDX,
    RTC_ISR_IDX,
    KB_Switch_IDX,
    DISPLAY_CTRL_ISR_IDX,
	//EP
    RPM_Calculate_ISR_IDX,
    GPIO_ODD_IRQ_ISR_IDX,
    HAL_VR2_Get_CrrntChk_ISR_IDX,
    ADC0_IRQHandler_ISR_IDX,
    
    ALL_IDXS
}MSISR_IDX;
///***************************************************************************//**
//* @ enum define,Ϊ����ODD�жϺ�����ע���ṩ����
//*
//******************************************************************************/
//typedef enum tag_ODDINTISR_IDX
//{
//    I2C_INT_PF9=0,
//	HR_INT_PB1,
//    RF_GDO0_INT,
//    ALL_ODDIDXS
//}ODD_INTISR_IDX;
///***************************************************************************//**
//* @ enum define,Ϊ����EVEN�жϺ�����ע���ṩ����
//*
//******************************************************************************/
//typedef enum tag_EVENINTISR_IDX
//{
//    POLOR_INT_PB2=0,
//    RF_GDO2_INT,
//    ALL_EVENIDXS
//}EVEN_INTISR_IDX;

/***************************************************************************//**
* @ enum define,Ϊ����EVEN�жϺ�����ע���ṩ����
*
******************************************************************************/
typedef enum tag_LETIMER_IDX
{
    ERP_TIMER_COUNT=0,
    ALL_LETIMERIDXS
}LETIMER_IDX;

/***************************************************************************//**
* @ enum define,for power channel:  
*   Power_Channel_A:�󲿷������Դ
*   Power_Channel_B��reserved
*   Power_Channel_C��Power_Button_LED_PORT��reserved
*   Power_Channel_ERP��ERP Light��LED��
******************************************************************************/
typedef enum tag_PWR_CHANNEL{
    Power_Channel_A=1,
	Power_Channel_B,
    Power_Channel_C,
    Power_Channel_ERP,
    ALL_CHANNELS
}PWR_CHANNEL;
/***************************************************************************//**
* @ enum define,for Maxtrix key and I2C key 
*
******************************************************************************/
typedef enum tag_PHYKEYMODE{
   KEYMODE_I2C= 0,
   KEYMODE_IO,
   KEYMODE_ALL
}PHYKEYMODE;
/***************************************************************************//**
* @ enum define,for �����ڲ������ⲿʱ�Ӵ�IO��� 
*
******************************************************************************/
typedef enum tag_CLOCK_OUTPUT_MODE{
   INTERNAL_OUTPUT= 0,
   EXTERNAL_OUTPUT,
   OUTMODE_ALL
}CLOCK_OUTPUT_MODE;

/***************************************************************************//**
* @ enum define,for �����ڲ������ⲿʱ�Ӵ�IO��� 
*
******************************************************************************/
//typedef enum tag_TIMER1_CC_CHL
//{
//    TIMER1_CC_CH_HR=1,
//    TIMER1_CC_CH_POLAR,
//    DEFAULT_CH
//}TIMER1_CC_CHL;
/***************************************************************************//**
* @ enum define,for �����ڲ������ⲿʱ�Ӵ�IO��� 
*
******************************************************************************/
typedef enum tag_AMP_SOURCE_CHANNEL{
   AMP_SOURCE_CHANNEL1= 0,
   AMP_SOURCE_CHANNEL2,
   AMP_SOURCE_CHANNEL3,
   AMP_SOURCE_CHANNEL_ALL
}AMP_SOURCE_CHANNEL;

/***************************************************************************//**
* @ typedef struct,DMA��ʼ�������ݽṹͼ���Ͷ��壬���������У�
*   char channel��ͨ��
*   DMA_CfgDescr_TypeDef    DescrCfg���������ã�����������
                            DescrCfg.dstInc��Ŀ���ַ������
*                           DescrCfg.srcInc������Դ��ַ����
*                           DescrCfg.size��ÿ�δ��͵������ֽ���
*                           DescrCfg.hprot������Arbitrate��Ҫ������ֽ���
*   UINT16 size��DMA������ֽڳ���
*   DMA_FuncPtr_TypeDef callback���ص�������ָ��
*   UINT32  dataselect��DMA�ź�Դ˵��
*   void*  datasrc������Դ��ַ
*   void*  datadst��Ŀ���ַ
******************************************************************************/
typedef struct STRUCT_DMA_PARA{
    char channel;
    DMA_CfgDescr_TypeDef DescrCfg;
    UINT16 size;
    DMA_FuncPtr_TypeDef callback;
    UINT32  dataselect;
    void*  datasrc;
    void*  datadst;       
}DMA_PARA;

typedef struct
{
    UINT8 Length[9];
    UINT8 *Displaydata[9];     
}HT_ShowData;
/***************************************************************************//**
* @ GLOBAL FUNCTIONS
*   
******************************************************************************/
void HAL_Set_Stop_position(UINT16 stop_position);
/***************************************************************************//**
* @brief
*   ���IO�ж�Flag    
*
* @note
*  
*
* @param[in] location
*   UINT w_Pin
* @param[out]��none
******************************************************************************/
void HAL_Clear_IO_IntFlag(UINT w_Pin);

/***************************************************************************//**
* @brief
*    ��ȡIO �ж�Flag
*
* @note
*  
*
* @param[in] location
*   UINT w_Pin
* @param[out]��
*   (UCHAR)by_Stat
******************************************************************************/
UINT HAL_Get_IO_IntFlag(UINT w_Pin);

/***************************************************************************//**
* @brief
*   ����IO��Ϊinput
*
* @note
*  
* @param[in] location
*   GPIO_Port_TypeDef by_Port
*   UINT w_Pin
* @param[out]��none
******************************************************************************/
void HAL_Set_IO_Input(GPIO_Port_TypeDef by_Port, UINT w_Pin);

/***************************************************************************//**
* @brief
*    ����IO������͵�ƽ
*
* @note
*  
* @param[in] location
*   GPIO_Port_TypeDef by_Port
*   UINT w_Pin 
* @param[out]��none
******************************************************************************/
void HAL_Set_IO_Low(GPIO_Port_TypeDef by_Port, UINT w_Pin);

/***************************************************************************//**
* @brief
*    ����IO��Ϊ���
*
* @note
*
* @param[in] location
*   GPIO_Port_TypeDef by_Port
*   UINT w_Pin
* @param[out]��none
******************************************************************************/
void HAL_Set_IO_Output(GPIO_Port_TypeDef by_Port, UINT w_Pin);

/***************************************************************************//**
* @brief
*    ����IO������ߵ�ƽ
*
* @note
*  
* @param[in] location
*   GPIO_Port_TypeDef by_Port
*   UINT w_Pin 
* @param[out]��none
******************************************************************************/
void HAL_Set_IO_High(GPIO_Port_TypeDef by_Port, UINT w_Pin);

/***************************************************************************//**
* @brief
*   HAL_Enable_IO_PullUp
*
* @note
*  
*
* @param[in] location
*   UINT w_Pin
* @param[out]��none
******************************************************************************/
void HAL_Enable_IO_PullUp(GPIO_Port_TypeDef by_Port, UINT w_Pin);

/***************************************************************************//**
* @brief
*  ����IO��IntMode
*
* @note
*   ���ô˺���Ĭ��INT��enable�ģ� GPIO_IntConfigԭ�����£�
*   GPIO_IntConfig(GPIO_Port_TypeDef port,
*                   unsigned int pin,
*                   bool risingEdge,
*                   bool fallingEdge,
*                   bool enable)
* @param[in]
*   GPIO_Port_TypeDef by_Port
*   UINT w_Pin 
*   UCHAR by_Mode�����������غ��½���
*   by_Enable:ʹ���ж�
* @param[out]��none
******************************************************************************/
void HAL_Set_IO_IntMode(GPIO_Port_TypeDef by_Port, UINT w_Pin, UCHAR by_Mode,UCHAR by_Enable);
/***************************************************************************//**
* @brief
*    ʹ��WDT
*
* @note
*  
*
* @param[in]��none
* @param[out]��none  
******************************************************************************/
void HAL_Enable_WatchDog(void);

/***************************************************************************//**
* @brief
*    ����WDT����
*
* @note
*  
*
* @param[in]��none
* @param[out]��none 
******************************************************************************/
void HAL_Clear_WatchDog(void);

/***************************************************************************//**
* @brief
*    ��ֹWDT
*
* @note
*  
*
* @param[in]��none
* @param[out]��none 
******************************************************************************/
void HAL_Disable_WatchDog(void);

/***************************************************************************//**
* @brief
*    ���帴λ
*
* @note
*  
*
* @param[in]��none
* @param[out]��none   
******************************************************************************/
void HAL_Reset_System(void);

/***************************************************************************//**
* @brief
*    ϵͳ��λ��ʱ������
*
* @note
*  
*
* @param[in]��none
* @param[out]��none   
******************************************************************************/
void HAL_Config_Clock_Active(void);

/***************************************************************************//**
* @brief
*    ����ERP mode�µ�ʱ�ӣ��ص�ERPʱ����Ҫ��
*
* @note
*  
*
* @param[in]��none
* @param[out]��none 
******************************************************************************/
void HAL_Config_Clock_Standby(void);

/***************************************************************************//**
* @brief
*    enable interrrupt
*
* @note
*  
*
* @param[in]��none
* @param[out]��none 
******************************************************************************/
__STATIC_INLINE void HAL_Enable_MasterInterrupt(void)
{
  __enable_irq();
}

/***************************************************************************//**
* @brief
*    disable interrrupt
*
* @note
*  
*
* @param[in]��none
* @param[out]��none 
******************************************************************************/
__STATIC_INLINE void HAL_Disable_MasterInterrupt(void)
{
  __disable_irq();
}

/***************************************************************************//**
* @brief
*   �ϵ���Դ����IO�ڳ�ʼ��
*
* @note
*   ��ʼ�����ʱ�Դ���ERP״̬���������ﲢû�д����еĵ�Դ     
*
* @param[in]��none
* @param[out]��none 
******************************************************************************/
void HAL_BOR_Initial_IO(void);

/***************************************************************************//**
* @brief
*    ��ȡ��Դ������IO��״̬
*
* @note
*  
*
* @param[in] None
* @param[out]����bool��pinstatus 
******************************************************************************/
bool HAL_BOR_Get_Enable(void);

/***************************************************************************//**
* @brief
*    �������衢Power_Button��ERP��Դ
*
* @note
*  @ enum define,for power channel:  
*   Power_Channel_A:�����Դ
*   Power_Channel_B��reserved
*   Power_Channel_C��Power_Button_LED_PORT��reserved
*   Power_Channel_ERP��ERP Light��LED��
*
* @param[in]: UCHAR channel,bool isEnable
* @param[out]:none   
******************************************************************************/
void HAL_BOR_Enable_ExtPower(PWR_CHANNEL channel,bool isEnable);

/***************************************************************************//**
* @brief
*  This is a Serial Wire Output based console.
*
* @note
*  1. Set up the Serial Wire Output in your application.
   You can use the function below for this.
   NOTE: This funtion is different than the one used
   in the profiler!
*  2. To print a single character use the CMSIS function
   ITM_SendChar(char c).
*
* @param[in] :none
* @param[out]:none
******************************************************************************/
void setupSWOForPrint(void);

/***************************************************************************//**
* @brief
*    HAL_Sys_Clock_Output_Enable
*
* @note
*  ����CLOCK��PA1��PA2�����pa1�����Ƶ��pa2�����Ƶ
*  ע����ú���ʱ��ֹ�ú������������ø����
* @param[in]��CLOCK_OUTPUT_MODE mode��ѡ������ڲ�clock�����ⲿclock
* @param[out]��none 
******************************************************************************/
void HAL_Sys_Clock_Output_Enable(CLOCK_OUTPUT_MODE mode);

/***************************************************************************//**
* @brief
*    4.On Line Debug
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
//void HAL_Debug_Initial_IO(void);
//void HAL_Debug_Printf(*fmt,va_list);

/***************************************************************************//**
* @brief
*    �ջ����������ԣ����IO��������
*
* @note
*  
*
* @param[in] :none
* @param[out]:none
******************************************************************************/
void HAL_LFT_Initial_IO(void);

/***************************************************************************//**
* @brief
*    ��ȡ�ջ����������ԣ�IO��״̬
*
* @note
*    �ߵ�ƽ��ʾ�ջ���δ��·  
*    �͵�ƽ��ʾ��¼�ڴ��ڶ�·״̬
* @param[in] :none
* @param[out]:(bool)pin status
******************************************************************************/
bool HAL_LFT_Get_Enable(void);

/***************************************************************************//**
* @brief
*   RTC��CLK��RST IO��ʼ��������Ϊ���
*
* @note
*  
*
* @param[in] :none
* @param[out]:none
******************************************************************************/
void HAL_RTC_Initial_IO(void);

/***************************************************************************//**
* @brief
*    RTC��RST�ܽ������ƽ����
*
* @note
*  
*
* @param[in] :(bool) isHighLevel
* @param[out]:none
******************************************************************************/
void HAL_RTC_Set_RST(bool isHighLevel);

/***************************************************************************//**
* @brief
*    RTC��CLK�ܽ������ƽ����
*
* @note
*  
*
* @param[in] :(bool) isHighLevel
* @param[out]:none 
******************************************************************************/
void HAL_RTC_Set_CLK(bool isHighLevel);

/***************************************************************************//**
* @brief
*    RTC��Data�ܽ������ƽ����
*
* @note
*  
*
* @param[in] :(bool) isHighLevel
* @param[out]:none 
******************************************************************************/
void HAL_RTC_Set_DAT(bool isHighLevel);

/***************************************************************************//**
* @brief
*    ��ȡRTC��Data�ܽ�����״̬
*
* @note
*  
*
* @param[in] :none
* @param[out]:(bool) pin status 
******************************************************************************/
bool HAL_RTC_Get_Data(void);

/***************************************************************************//**
* @brief
*   ��ʼ��������IO����I2C������I2Cģ��
*
* @note
*   ����ʱҪ����ö������ΪPHYKEYMODE�Ĳ���  mode��
*   KEYMODE_I2C= 0, ---->I2C����
*   KEYMODE_IO      ---->IO/Matrix ����
*   �磺HAL_Key_Initial_IO��KEYMODE_I2C����
* @param[in]: PHYKEYMODE mode
* @param[out]:none
******************************************************************************/
void HAL_Key_Initial_IO(PHYKEYMODE mode);

/***************************************************************************//**
* @brief
*   ��ȡ����ֵ���˴��õ��ļ�ֵ��Ҫ����
*@note
*   1.����ʱҪ����ö������ΪPHYKEYMODE�Ĳ���  mode��
*   KEYMODE_I2C= 0, ---->I2C����
*   KEYMODE_IO      ---->IO/Matrix ����
*   2.����ֵΪ1��32bit��ֵ����ʽΪ0xK1K2K3K4������K1��K2��K3��K4�ֱ�Ϊһ��8bit
*   ������
*   K1�������һ������ֵ���ڵ���
*   K2�������һ������ֵ���ڵ���
*   K3������ڶ�������ֵ���ڵ���
*   K4������ڶ�������ֵ���ڵ���
*   ���û�а������£��ð����ķ���ֵΪAA������2���������£���Ϊ��Ч������AAAA��
*   ���ֻ��һ���������£�����ֵΪ K1K2AA��
*   ����������������£�����K1K2K3K4��
*   ���û�а������߶���2������ͬ�°��£��򷵻�AAAAA;
*
*   UtilityKeyboard�еĺ���ͨ�����øú����õ���ֵ����ͨ������ֵ���õ���Ӧ��
*   ����ֵ,�磺
*   UINT32 KeyValue = HAL_Key_Get_Value��KEYMODE_I2C����
*   �磺KeyValue = 0x34AA---->˵��ֻ��һ���������£�Ϊ3��4�е��Ǹ�����������ֵ��
*   �ɵó������Ķ��壻
*   �磺KeyValue = 0x3456---->˵���������������£�Ϊ3��4�С�5��6�е�������������
*   ����ֵ����Եõ���������ϼ���
*   �磺KeyValue = 0xAAAA---->˵���ް������»��ߴ���2���İ��������¡�
*
* @param[in]: PHYKEYMODE mode
* @param[out]��UINT32 key_value  
******************************************************************************/
UINT32 HAL_Key_Get_Value(PHYKEYMODE mode);

/***************************************************************************//**
* @brief
*   I2C�����ͷű�־λ
*
* @note
*   I2C�����ṩ�İ����ͷŵı�־λ
*   ����ֵΪ1�������ͷ�
*   ����ֵΪ0������Ϊδ�ͷ�
*   һ�����ڵ���⵽�����ͷ�ʱ�����Զ԰������¼�ʱ�ļĴ�������
* @param[in]��none
* @param[out]��bool status
******************************************************************************/
bool HAL_Key_I2C_Get_KeyReleaseflag(void);

/***************************************************************************//**
* @brief
*    HAL_FAN_Initial_IO
*
* @note
*    ��ʼ��Fan IO����͵�ƽ������ռ�ձ�Ϊ0
*
* @param[in]��none
* @param[out]��none
******************************************************************************/
void HAL_FAN_Initial_IO(void);

/***************************************************************************//**
* @brief    
*   HAL_FAN_Set_Duty
*
* @note
*   ����ֱ����������Ҫ��ռ�ձȣ�����Ҫ���÷��ȵ�ռ�ձ�Ϊ80%��Input����Ϊ80�����£�     
* HAL_FAN_Set_Duty(80);
* @param[in]:UINT8 DutyValue
* @param[out]��none
******************************************************************************/
void HAL_FAN_Set_Duty(UCHAR DutyValue);
//
///***************************************************************************//**
//* @brief
//*   ��ʼ��timer1 capture�жϣ�����HR��polar�ļ�� 
//*
//* @note
//*  
//*
//* @param[in] none
//* @param[out] none 
//******************************************************************************/
//void HAL_Capture_Initial_IO();
/***************************************************************************//**
* @brief
*    HAL_HR_Initial_IO
*       
* @note
*   Init HR & POLOR IOs & interrupt
*
* @param[in] none
* @param[out]��none 
******************************************************************************/
void HAL_HR_Initial_IO(void);
UCHAR Hal_Heart_Get_Heart_Level(void);
UCHAR Hal_Heart_Get_Polar_Level(void);
///***************************************************************************//**
//* @brief
//*   HAL_HR_Get_HGP
//*   Get HR pin state
//* @note
//*  
//*
//* @param[in] none
//* @param[out]��pin status 
//******************************************************************************/
//bool HAL_HR_Get_HGP(void);
//
///***************************************************************************//**
//* @brief
//*    HAL_HR_Get_Polar
//*
//* @note
//*   Get POLOR pin state
//*
//* @param[in] none
//* @param[out]��pin status 
//******************************************************************************/
//bool HAL_HR_Get_Polar(void);
//
///***************************************************************************//**
//* @brief
//*   HAL_HR_Get_HGP_EXI
//*   Get HR intflag
//* @note
//*   
//*
//* @param[in] none
//* @param[out]��HGP pin int flag
//******************************************************************************/
//bool HAL_HR_Get_HGP_EXI(void);
//
///***************************************************************************//**
//* @brief
//*   HAL_HR_Get_Polar_EXI
//*   Get polar intflag
//* @note
//*  
//*
//* @param[in] none
//* @param[out]��polor pin int flag
//******************************************************************************/
//UCHAR HAL_HR_Get_Polar_EXI(void);
//
///***************************************************************************//**
//* @brief
//*    HAL_HRT_Clear_HR_EXI
//*
//* @note
//*  
//*
//* @param[in]: none
//* @param[out]��none
//******************************************************************************/
//void HAL_HRT_Clear_HR_EXI(void);
//
///***************************************************************************//**
//* @brief
//*   HAL_HRT_Clear_Polar_EXI 
//*
//* @note
//*  
//*
//* @param[in]: none
//* @param[out]��none  
//******************************************************************************/
//void HAL_HRT_Clear_Polar_EXI(void);

/***************************************************************************//**
* @brief
*    HAL_WIFI_Initial_IO
*
* @note
*   InitUART1
*
* @param[in]: none
* @param[out]��none 
******************************************************************************/
void HAL_WIFI_Initial_IO(void);

/***************************************************************************//**
* @brief
*    HAL_WIFI_Set_RST
*    set WF_RST_PORT high level or loe level
* @note
*  
*
* @param[in] bool isHighLevel
* @param[out]��none 
******************************************************************************/
void HAL_WIFI_Set_RST(bool isHighLevel);

/***************************************************************************//**
* @brief
*   HAL_WIFI_Get_RDY 
*   Get WF_Ready_PIN status
* @note
*  
*
* @param[in]��none
* @param[out]: WIFI ready pin status
******************************************************************************/
UINT8 HAL_WIFI_Get_RDY(void);

/***************************************************************************//**
* @brief
*  HAL_WIFI_Get_INT
*
* @note
*   Get WF_INT_PORT status
*
* @param[in]��none
* @param[out]: WF_INT_PORT status
******************************************************************************/
bool HAL_WIFI_Get_INT(void);

/***************************************************************************//**
* @brief
*   HAL_WIFI_Enable_UARTI
*   set wifi uart int enabled or disabled
* @note
*  
*
* @param[in]:bool isEnabled
* @param[out]: none   
******************************************************************************/
void HAL_WIFI_Enable_UARTI(bool isEnabled);

/***************************************************************************//**
* @brief
*    HAL_WIFI_SendData
*
* @note
*  
*
* @param[in] UCHAR by_Dat
* @param[out]: none  
******************************************************************************/
void HAL_WIFI_SendData(UCHAR Data);
/***************************************************************************//**
* @brief
*    HAL_WIFI_Get_RX_Select
*
* @note
*   DMA�ź�Դ˵��
*
* @param[in]:bool isEnabled)
* @param[out]: UINT32    
******************************************************************************/
UINT32 HAL_WIFI_Get_RX_Select(void);
/***************************************************************************//**
* @brief
*    HAL_WIFI_Get_RX_DataSrc
*
* @note
*   DMA��ȡRX������Դ������ֵΪ����Դ�ĵ�ַ������Ϊ(void*)
*
* @param[in]:none
* @param[out]: void*    
******************************************************************************/
void* HAL_WIFI_Get_RX_DataSrc(void);
/***************************************************************************//**
* @brief
*    HAL_WIFI_Get_RX_Select
*
* @note
*   DMA�ź�Դ˵��
*
* @param[in]:bool isEnabled)
* @param[out]: UINT32    
******************************************************************************/
UINT32 HAL_WIFI_Get_TX_Select(void);
/***************************************************************************//**
* @brief
*    HAL_WIFI_Get_TX_DataSrc
*
* @note
*   DMA��ȡTX������Դ������ֵΪ����Դ�ĵ�ַ������Ϊ(void*)
*
* @param[in]:none
* @param[out]: void*   
******************************************************************************/
void* HAL_WIFI_Get_TX_DataSrc(void);

UCHAR HAL_WIFI_GetData(void);
/***************************************************************************//**
* @brief
*   11.RF function
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
#ifdef DMA_SPI
void HAL_DMA_SPI_Master_Setup(void);
void HAL_DMA_SPI_Transfer(uint8_t *txBuffer,int bytes);
void HAL_DMA_Receive(UINT8 *rxBuffer,int bytes);
bool HAL_DMA_Get_transferComplete(void);
bool HAL_DMA_Get_receiveComplete(void);
void spiDmaTransfer(uint8_t *txBuffer, uint8_t *rxBuffer,  int bytes);
void HAL_DMA_Clear_RXBuffer(UINT8 Length);
void HAL_DMA_Set_RXBuffer_Length(int rxLength);
void spiDmaTransfer(uint8_t *txBuffer, uint8_t *rxBuffer,  int bytes);
#endif
/***************************************************************************//**
* @brief:HAL_RF_Initial_IO
*    ����SPI��IO���������״̬
*   
* @note
*   ���������DMA_SPI��DMA+Ӳ��SPIʵ��RF���ܣ�������Ӳ��SPIҲ���г�ʼ����
*   ��δ���壬��Ĭ��ʹ��ģ��SPI��
*
* @param[in] none
* @param[out]: none    
******************************************************************************/
void HAL_RF_Initial_IO(void);

/***************************************************************************//**
* @brief:HAL_RF_Get_GDO0_Pin_Input_State
*    Get RF_GDO0 pin state
*   
* @note
*
* @param[in] none
* @param[out]: RF_GDO0 pin state    
******************************************************************************/
bool HAL_RF_Get_GDO0_Pin_Input_State(void);

/***************************************************************************//**
* @brief:HAL_RF_Get_GDO2_Pin_Input_State
*    Get RF_GDO2 pin state
*   
* @note
*
* @param[in] none
* @param[out]: RF_GDO2 pin state    
******************************************************************************/
bool HAL_RF_Get_GDO2_Pin_Input_State(void);

/***************************************************************************//**
* @brief:HAL_RF_Config_GDO0
*    ����RF_GDO0���ж�
*    isEnable �����Ƿ���ж�
* @note
*
* @param[in] bool isEnable
* @param[out]: none    
******************************************************************************/
void HAL_RF_Config_GDO0(bool isEnable);

/***************************************************************************//**
* @brief:HAL_RF_Config_GDO2
*    ����RF_GDO2���ж�
*    isEnable �����Ƿ���ж�
* @note
*
* @param[in] bool isEnable
* @param[out]: none    
******************************************************************************/
void HAL_RF_Config_GDO2(bool isEnable);

/***************************************************************************//**
* @brief:HAL_RF_Set_SPI_BITBANG_SIMO
*    set SIMO high or low
*
* @note
*
* @param[in] bool isHigh
* @param[out]: none    
******************************************************************************/
void HAL_RF_Set_SPI_BITBANG_SIMO(bool isHigh);

/***************************************************************************//**
* @brief:HAL_RF_Set_SPI_BITBANG_UCLK
*    set CLK high or low
*
* @note
*
* @param[in] bool isHigh
* @param[out]: none    
******************************************************************************/
void HAL_RF_Set_SPI_BITBANG_UCLK(bool isHigh);

/***************************************************************************//**
* @brief:HAL_RF_Get_SPI_BITBANG_SOMI
*   get MISO pin status
*
* @note
*
* @param[in] none
* @param[out]: MISO pin status    
******************************************************************************/
bool HAL_RF_Get_SPI_BITBANG_SOMI(void);

//unsigned char HAL_RF_Get_SPI_BITBANG_SOMI_BIT(void);
/***************************************************************************//**
* @brief:HAL_RF_Set_SPI_BITBANG_CS
*    set cs high or low
*
* @note
*
* @param[in] bool isHigh
* @param[out]: none    
******************************************************************************/
void HAL_RF_Set_SPI_BITBANG_CS(bool isHigh);

/***************************************************************************//**
* @brief:HAL_RF_Delay
*    delay used in RF
*
* @note
*
* @param[in] bool isHigh
* @param[out]: none    
******************************************************************************/
void HAL_RF_Delay(UINT16 cycles);

/***************************************************************************//**
* @brief:HAL_RF_Clear_INT_Flag
*   clear GDO0 or GDO2 int flag
*   mode= 0 :clear GDO0 int flag
*   mode= 1 :clear GDO2 int flag
*
* @note
*
* @param[in] bool mode
* @param[out]: none    
******************************************************************************/
void HAL_RF_Clear_INT_Flag(bool mode);

/***************************************************************************//**
* @brief:HAL_RF_Clear_INT_Flag
*   get GDO0 or GDO2 int flag
*   mode= 0 :get GDO0 int flag
*   mode= 1 :get GDO2 int flag
*
* @note
*
* @param[in] bool mode
* @param[out]: bool int flag    
******************************************************************************/
bool HAL_RF_Get_IO_IntFlag(bool mode);

/***************************************************************************//**
* @brief
*   HAL_SafetyKey_Initial_IO
*   set saftykey input
* @note
*  
*
* @param[in]:none
* @param[out]: none
******************************************************************************/
void HAL_SafetyKey_Initial_IO(void);

/***************************************************************************//**
* @brief
*   HAL_SafetyKey_Get_Attached 
*   get SafetyKey pin status
* @note
*  
*
* @param[in] :none
* @param[out]: safety key pin status  
******************************************************************************/
bool HAL_SafetyKey_Get_Attached(void);

/***************************************************************************//**
* @brief
*    HAL_DLCB_Delay
*
* @note
*   1���˴�loop�ڼ���ҪClear WDT
*   2���˴���ʱ����i����ֵ������1ms�ж��н��еݼ�������delay�������ǲ����ݼ���
*   ���
*
* @param[in]:UINT16 i
* @param[out]:none
******************************************************************************/
void HAL_DLCB_Delay(UINT16 i);


/***************************************************************************//**
* @brief
*    HAL_DLCB_1ms_Int
*
* @note
*   ��1ms�ж��ж�HAL_DLCB_Delay������ֵ���еݼ�
*
* @param[in]:none
* @param[out]:none
******************************************************************************/
void HAL_DLCB_1ms_Int(void);

/***************************************************************************//**
* @brief
*    HAL_DLCB_Initial_IO
*    DLCB���õ�UART���г�ʼ��
* @note
*  
*
* @param[in]:none
* @param[out]:none
******************************************************************************/
void HAL_DLCB_Initial_IO(void);

/***************************************************************************//**
* @brief
*   HAL_DLCB_SendData 
*   ��UART����һ���ֽ�����
* @note
*  
*
* @param[in]:UCHAR Data
* @param[out]:none
******************************************************************************/
void HAL_DLCB_SendData(UCHAR Data);

/***************************************************************************//**
* @brief
*   �ر�TX���ж� 
*
* @note
*  
*
* @param[in]:none
* @param[out]:none 
******************************************************************************/
void HAL_DLCB_SetToRx(void);

/***************************************************************************//**
* @brief
*    HAL_DLCB_StartTx
*    enable tx   
* @note
*  
*
* @param[in]:none
* @param[out]:none  
******************************************************************************/
void HAL_DLCB_StartTx(void);

/***************************************************************************//**
* @brief
*    HAL_DLCB_WakeUp_LCB
*
* @note
*   
*
* @param[in]:none
* @param[out]:none  
******************************************************************************/
void HAL_DLCB_WakeUp_LCB(void);

/***************************************************************************//**
* @brief
*   HAL_DLCB_Get_Rx
*
* @note
*   ��ֹ UART_Rx_IRQ-->����UART_485DE_PIN ��� �͵�ƽ-->���� UART_RX_PIN ����״̬
*   -->get UART_RX_PIN state������״ֵ̬
*
* @param[in]:none
* @param[out]:none    
******************************************************************************/
bool HAL_DLCB_Get_Rx(void);

/***************************************************************************//**
* @brief
*    HAL_DLCB_GetData
*
* @note
*  
*
* @param[in]:none
* @param[out]:SART_Rx(USART1);   
******************************************************************************/
UCHAR HAL_DLCB_GetData(void);

/***************************************************************************//**
* @brief
*    HAL_DLCB_Erp
*   DisableIRQ UART_Rx_IRQ ;
* @note
*  
*
* @param[in]:none
* @param[out]:none   
******************************************************************************/
void HAL_DLCB_Erp(void);

/***************************************************************************//**
* @brief
*   HAL_Beep_Initial_IO
*
* @note
*   ����beep��Timer 
*
* @param[in]:none
* @param[out]:none   
******************************************************************************/
void HAL_Beep_Initial_IO(void);

/***************************************************************************//**
* @brief
*   HAL_Beep_Set_Beep
*   ����beep IO ��PWMʹ��
* @note
*  
*
* @param[in]:bool isEnabled
* @param[out]:none    
******************************************************************************/
void HAL_Beep_Set_Beep(bool isEnabled);

/***************************************************************************//**
* @brief
*   HAL_Beep_Get_IOLevel
*   Get BEEP_PIN status  
* @note
*  
*
* @param[in]:none
* @param[out]:BEEP_PORT status  
******************************************************************************/
UCHAR HAL_Beep_Get_IOLevel(void);

/***************************************************************************//**
* @brief
*    HAL_AMP_Initial_IO
*
* @note
*   1������Audio_Source1��2/3��PWM��MUTE��IOΪ�����Check_In pinΪ����
*   2�����ó�ʼ��AMPռ�ձȣ��ɲ�������
*
* @param[in] location
* @param[out] location  
******************************************************************************/
void HAL_AMP_Initial_IO(UCHAR DutyValue);

/***************************************************************************//**
* @brief
*   HAL_AMP_Select_Source 
*       AMP_SOURCE_CHANNEL1= 0,
*       AMP_SOURCE_CHANNEL2,
*       AMP_SOURCE_CHANNEL3,
* @note
*   Ĭ��ÿ��״̬��ֻ�ܴ�һ��source
*
* @param[in]��AMP_SOURCE_CHANNEL channel
* @param[out]�� none
******************************************************************************/
void HAL_AMP_Select_Source(AMP_SOURCE_CHANNEL channel);

/***************************************************************************//**
* @brief
*   HAL_AMP_Check_AUX 
*   ��ȡVolume_CHECK_IN_PIN �ܽ�״̬
* @note
*  
*
* @param[in]��none
* @param[out]����ȡVolume_CHECK_IN_PIN pin status   
******************************************************************************/
bool HAL_AMP_Check_AUX(void);

/***************************************************************************//**
* @brief
*   HAL_AMP_Set_Mute 
*   ����mute�ܽŸߵ͵�ƽ
* @note
*
* @param[in]��bool isMuted
* @param[out]��none  
******************************************************************************/
void HAL_AMP_Set_Mute(bool isMuted);

/***************************************************************************//**
* @brief
*  HAL_AMP_Set_Duty  
*   ����AMP��ռ�ձȣ��磬����ռ�ձ�Ϊ75%����HAL_AMP_Set_Duty(75);
* @note
*  
*
* @param[in]��UCHAR DutyValue
* @param[out]�� none 
******************************************************************************/
void HAL_AMP_Set_Duty(UCHAR DutyValue);

/***************************************************************************//**
* @brief
*   HAL_16XX_I2C_Write_CMD 
*   ����дchip configuration����
* @note
*   ע���ַ����ͬ�ͺ�оƬ�ĵ�ַ��һ����
*   16C21-->0x70
*   16C22-->
*   16C23-->
*   16C24-->0x7A
*   
* @param[in]��UINT8 Cmd,UINT8 Settings
* @param[out]��none  
******************************************************************************/
void HAL_16CXX_I2C_Write_CMD(UINT8 Chip_Type,UINT8 Cmd,UINT8 Settings);


void HAL_16CXX_I2C_Write_ShowData(UINT8 Chip_Type,UINT8 Dis_Cmd,UINT8 Dis_Address,UINT8 *pby_Lcd_Buffer,UINT8 Length);
/***************************************************************************//**
* @brief
*   HAL_16XX_Init_I2C 
*   ��ʼ��I2Cģ��
* @note
*   
*
* @param[in]��none
* @param[out]��none
******************************************************************************/
void HAL_16CXX_Init_I2C(void);



/***************************************************************************//**
* @brief
*   ����16XX��IO״̬ 
*
* @note
*  
*
* @param[in]��none
* @param[out]��none   
******************************************************************************/
void HAL_HT16XX_Initial_IO(void);
void HAL_HT16XX_Set_High_CLK(bool isHigh);

/***************************************************************************//**
* @brief
*   ����WRΪ�ߵ�ƽ���ߵ͵�ƽ   
*
* @note
*  
* @param[in]��bool isHigh
* @param[out]��none   
******************************************************************************/
void HAL_HT16XX_Set_High_WR(bool isHigh);

/***************************************************************************//**
* @brief
*   ����Data0Ϊ�ߵ�ƽ���ߵ͵�ƽ    
*
* @note
*  
*
* @param[in]��bool isHigh
* @param[out]��none   
******************************************************************************/
void HAL_HT162x_Initial_IO(void);
void HAL_HT16XX_Set_High_Data0(bool isHigh);

/***************************************************************************//**
* @brief
*   ����Data1Ϊ�ߵ�ƽ���ߵ͵�ƽ    
*
* @note
*  
*
* @param[in]��bool isHigh
* @param[out]��none   
******************************************************************************/
void HAL_HT16XX_Set_High_Data1(bool isHigh);

/***************************************************************************//**
* @brief
*   ����Data2Ϊ�ߵ�ƽ���ߵ͵�ƽ    
*
* @note
*  
*
* @param[in]��bool isHigh
* @param[out]��none   
******************************************************************************/
void HAL_HT16XX_Set_High_Data2(bool isHigh);

/***************************************************************************//**
* @brief
*   ����Data3Ϊ�ߵ�ƽ���ߵ͵�ƽ    
*
* @note
*  
*
* @param[in]��bool isHigh
* @param[out]��none   
******************************************************************************/
void HAL_HT16XX_Set_High_Data3(bool isHigh);

/***************************************************************************//**
* @brief
*   HAL_HT16XX_Set_High_CS
*   set CS�����ƽ
* @note
*   isHigh =1 ʱ������csΪ�͵�ƽ
*   isHigh =0 ʱ������csΪ�ߵ�ƽ
*
* @param[in]��UCHAR by_IC, bool isHigh
* @param[out]��none 
******************************************************************************/
void HAL_HT16XX_Set_High_CS(UCHAR by_IC, bool isHigh);

/***************************************************************************//**
* @brief
*   HAL_LCD_Backlight_Initial_IO 
*   ����LCD_BACKLIGHT_PINΪ�ߵ�ƽ
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void HAL_TLC59282_Initial_IO(void);
void HAL_Switch_Initial_IO(void);
bool HAL_SwitchA_Pin_Input_State(void);
bool HAL_SwitchB_Pin_Input_State(void);
void HAL_LCD_Backlight_Initial_IO(void);
void HAL_HT162x_Set_High_CS(UCHAR by_IC, UCHAR by_Dat);
/***************************************************************************//**
* @brief
*   HAL_USB_LED_Initial_IO
*
* @note
*  reserved
*
* @param[in] none
* @param[out] none
******************************************************************************/
void HAL_USB_LED_Initial_IO(void);

/***************************************************************************//**
* @brief
*   HAL_USB_LED_Initial_IO
*
* @note
*  reserved
*
* @param[in] none
* @param[out] none
******************************************************************************/
void HAL_USB_LED_Control(UCHAR LED,UCHAR ON);
void HAL_USB_Set_EN_PORT(bool isHighLevel);
void HAL_USB_Set_DM_PORT(bool isHighLevel);
void HAL_USB_Set_DP_PORT(bool isHighLevel);
void HAL_USB_Set_CTRL_PORT(bool isHighLevel);
UCHAR HAL_USB_Get_DETECT_Enable(void);
UCHAR HAL_USB_Get_Over_Enable(void);

/***************************************************************************//**
* @brief
*  19.External SPI Memory  
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
//void HAL_SPIFlash_Initial_IO(void);
//void HAL_SPIFlash_Enable(bool isHighLevel);
//UINT32 HAL_SPIFlash_Get_LowestAddress(void);
//UINT32 HAL_SPIFlash_Get_HighestAddress(void);
//void HAL_SPIFlash_Set_SCK(BOOL isHighLevel);
//void HAL_SPIFlash_Set_MOSI(BOOL isHighLevel);
//BOOL HAL_SPIFlash_Get_MISO(void);
//void HAL_SPIFlash_Single_Write(UINT32 addr, BYTE data);
//BYTE HAL_SPIFlash_Single_Read(UINT32 addr);
//void HAL_SPIFlash_Burst_Write(UINT32 addr, PCHAR *pSrc, UINT32 len);
//bool HAL_SPIFlash_Burst_Read(UINT32 addr, PCHAR *pDest, UINT32 len);

/***************************************************************************//**
* @brief
*    HAL_FLASH_Get_Page_Size
*    ��ȡMCUһ��page��size
* @note
*  
*
* @param[in] none
* @param[out] FlashPageSize 
******************************************************************************/
UINT32 HAL_FLASH_Get_Page_Size(void);

/***************************************************************************//**
* @brief
*   HAL_FLASH_Get_LowestAddress 
*
* @note
*  
*
* @param[in] none
* @param[out] LowestAddress 
******************************************************************************/
UINT32 HAL_FLASH_Get_LowestAddress(void);

/***************************************************************************//**
* @brief
*  HAL_FLASH_Get_HighestAddress  
*
* @note
*  
*
* @param[in] none
* @param[out] HighestAddress 
******************************************************************************/
UINT32 HAL_FLASH_Get_HighestAddress(void);
/***************************************************************************//**
* @brief
*    HAL_FLASH_Burst_Write
*
* @note
*   1�������д��flash������׵�ַ����д��������Լ����ȣ�д������ɺ��Ƿ��
*   �жϣ�
*   2��eg��HAL_FLASH_Burst_Write( FLASH_ADDRESS, (PCHAR*)flashBuffer, 2048 ,0);
*
* @param[in] UINT32 addr, PCHAR *pSrc, UINT32 len,bool enableInt
* @param[out] RIGHT or ERROR   
******************************************************************************/
bool  HAL_FLASH_Burst_Write(UINT32 addr, UINT8 *pSrc, UINT16 len, bool enableInt);


/***************************************************************************//**
* @brief
*    HAL_FLASH_Burst_Read
*
* @note
*  
*
* @param[in] (UINT32 addr, UINT32 *pDest, UINT32  len)
* @param[out] RIGHT or ERROR   
******************************************************************************/
bool HAL_FLASH_Burst_Read(UINT32 addr, UINT8 *pDest, UINT16 len);

/***************************************************************************//**
* @brief
*    HAL_FLASH_Erase_Page
*
* @note
*   ����һ��������ҳ�档
*
* @param[in] UINT32 addr,,bool enableInt
* @param[out] RIGHT or ERROR   
******************************************************************************/
bool HAL_FLASH_Erase_Page(UINT32 addr, bool enableInt);
/***************************************************************************//**
* @brief
*   HAL_BlueTooth_Initial_IO
*   reserved
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
//void HAL_BlueTooth_Initial_IO(void);
//void HAL_Bluetooth_EnableTx(UCHAR by_Dat);
//void HAL_Bluetooth_SendData(UCHAR by_Dat);

/***************************************************************************//**
* @brief
* 22.TV Port
*   reserved
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
//void HAL_TV_PORT_Initial_IO(void);
//void HAL_TV_PORT_UART_Enable_UARTI(bool isEnabled);
//void HAL_TV_PORT_UART_SendData(UCHAR by_Dat);

/***************************************************************************//**
* @brief
* 23.c_safe
*   reserved
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
//void HAL_CSafe_Initial_IO(void);
//void HAL_CSafe_Set_CTS(bool isHighLevel);
//void HAL_CSafe_UART_Enable_UARTI(bool isEnabled);
//void HAL_CSafe_UART_SendData(UCHAR by_Dat);

/***************************************************************************//**
* @brief
* 24.ALCB
*   reserved
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
//void HAL_ALCB_Initial_IO(void);
//void HAL_ALCB_Get_InclineZero(void);
//void HAL_ALCB_Set_InclineUp(void);
//void HAL_ALCB_Set_InclineDown(void);
//void HAL_ALCB_Get_InclineCount(void); //level
//void HAL_ALCB_Eanble_Relay(bool isEnabled);
//void HAL_ALCB_Set_PWM(UINT16 value);
//void HAL_ALCB_Get_RPM(void);  //level

/***************************************************************************//**
* @brief
* 25.I2C Reserved
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
//void HAL_I2C_Reserved_Initial_IO(void);
//UINT32 HAL_I2C_Reserved_Get_Value(void);

/***************************************************************************//**
* @brief
*    HAL_Time_Delay
*   �������ʱ
* @note
*
* @param[in]:unsigned long  Delay_Value
* @param[out]:none   
******************************************************************************/
void HAL_Time_Delay(unsigned long  ulVal);

/***************************************************************************//**
* @brief
*    HAL_ERP_Low_POWER_Config
*
* @note
*  1������ERP֮ǰ���жϡ��ж������������ж����á�ʱ�ӡ�LETIMER���������
*  2����ڵ�����������KeyMode������I2C����IO��������ͬ�İ�������ERP֮ǰIO���ж�
*   ���ò�һ����iSEnableLETimer�����Ƿ���Ҫ��LETIMER�Թ�����
*
* @param[in]:PHYKEYMODE KeyMode,bool iSEnableLETimer 
* @param[out]:none
******************************************************************************/
void HAL_ERP_Low_POWER_Config(PHYKEYMODE KeyMode,bool iSEnableLETimer);

/***************************************************************************//**
* @brief
*    HAL_ERP_Enter_Standby_Mode
*
* @note
*   EFM32ϵ�е�Ƭ��Ϊ����EM2ģʽ
*
* @param[in] none
* @param[out]:none
******************************************************************************/
void HAL_ERP_Enter_Standby_Mode(void);

/***************************************************************************//**
* @brief
*   HAL_ERP_Get_Reset_Flag   
*
* @note
*   1����EM2״̬�£����жϻ��Ѻ�Reset_From_ERP_Flag����ְλ0x055AA�����ϲ��ȡ��
*   �������ж��Ƿ�Ϊ�������Ѳ�ִ�и�λ������
*
* @param[in] none
* @param[out]:Reset_From_ERP_Flag
******************************************************************************/
UINT16 HAL_ERP_Get_Reset_Flag(void);

/***************************************************************************//**
* @brief
*   HAL_ERP_Regist_LETIMER_ISR
*
* @note
*   LETIMER��
*   �磺Hal_ERP_Regist_LETIMER_ISR(ERP_TIMER_COUNT,LETIMER_Count);
*
* @param[in]��LETIMER_IDX index, PF pf
* @param[out]��none 
******************************************************************************/
void HAL_ERP_Regist_LETIMER_ISR(LETIMER_IDX index, PF pf);
/***************************************************************************//**
* @brief
*    ע��1ms�ڲ��жϣ�ÿ�ν���1ms�жϺ�������ע��ĺ���1��
*
* @note
*   ����˵����
*   1�����������������Beep_1ms_ISR������Ҫÿ1ms���жϵ���һ�Σ����������HAL.h
*      ��tag_1MSISR_IDX�����Ӻ���Beep_1ms_ISR������BEEP_ISR_IDX����beep��ʼ��ʱ��
*      ���µ��ã�Hal_Regist_ISR(MS_ISR,BEEP_ISR_IDX,Beep_1ms_ISR);
*   2������ע��������Digital_Cmd_Tx_Int����SysTick_Handlerƽ�е��жϳ�������Ҫ
*      ����HAL.h��tag_INTERRUPT_ISR�����Ӻ���Digital_Cmd_Tx_Int��UART_TX_ISR����
*      �������µ��ã� Hal_Regist_ISR(UART_RX_ISR, 0, Digital_Cmd_Rx_Int);
* @param[in]:INTERRUPT_ISR isr, MSISR_IDX index, PF pf
* @param[out]:none
******************************************************************************/
void Hal_Regist_ISR(INTERRUPT_ISR isr, MSISR_IDX index, PF pf);
///***************************************************************************//**
//* @brief
//*    Hal_Regist_ODDINT_ISR
//*
//* @note
//*   ODDINT�ж�ע�ắ�����磺Hal_Regist_ODDINT_ISR(I2C_INT_PF9,HAL_Key_I2C_IntISR) 
//*
//* @param[in]��ODD_INTISR_IDX index��PF pf
//* @param[out]��none
//******************************************************************************/
//void Hal_Regist_ODDINT_ISR(ODD_INTISR_IDX index, PF pf);
///***************************************************************************//**
//* @brief
//*   Hal_Regist_EVENINT_ISR
//*
//* @note
//*   ENENINT�ж�ע�ắ����
//*   �磺Hal_Regist_EVENINT_ISR(POLOR_INT_PB2,Heart_Calculate_Polar_Pulse_Int);
//*
//* @param[in]��(EVEN_INTISR_IDX index, PF pf)
//* @param[out]��none 
//******************************************************************************/
//void Hal_Regist_EVENINT_ISR(EVEN_INTISR_IDX index, PF pf);

/***************************************************************************//**
* @brief
*   HAL_INT_Regist_Pin_INT
*
* @note
*   PIN INT�ж�ע�ắ����
*   HAL_INT_Regist_Pin_INT(PIN_INT_GDO0,RF_GDO0_Int_ISR);
*   HAL_INT_Regist_Pin_INT(PIN_INT_GDO2,RF_GDO2_Int_ISR);
*   ����ö�����ͣ�EVEN_INTISR_IDX������ PIN_INT_GDO2��HAL������ܽŶ��壬����
*   �Զ�����ע����ж���
* @param[in]��PIN_INT_IDX Int_Index,PF pf
* @param[out]��none 
******************************************************************************/
void HAL_INT_Regist_Pin_INT(PIN_INT_IDX Int_Index,PF pf);

//
///***************************************************************************//**
//* @brief
//*    HAL_Clear_Timer1_CC_Flag
//*
//* @note
//*   clear Timer1_CC int Flag
//*
//* @param[in] TIMER1_CC_CHL chl
//* @param[out] none
//******************************************************************************/
//void HAL_Clear_Timer1_CC_Flag(TIMER1_CC_CHL chl);
//
///***************************************************************************//**
//* @brief
//*    HAL_Get_Timer1_CCChanel
//*
//* @note
//*   Get timer1 CC��ͨ��
//*
//* @param[in] none
//* @param[out] TIMER1_CC_CHL chl
//******************************************************************************/
//TIMER1_CC_CHL HAL_Get_Timer1_CCChanel(void);
//
///***************************************************************************//**
//* @brief
//*    ע�� timer1��capture�ж�
//*
//* @note
//*   ODD�жϷ����ӳ���
//*
//* @param[in]��none
//* @param[out]��none 
//******************************************************************************/
//void HAL_Regist_Timer1_ISR(PF pf);

/***************************************************************************//**
* @brief
*   HAL_DMA_ActivateBasic   
*
* @note
*   DMA���ͻ��������activate����������У�
*   char channel��DMAͨ��
*   void* datasrc������Դ�ĵ�ַ��ָ�����ͱ�����
*   void* datadst������ת�Ƶ�Ŀ���ַ��ָ�����ͱ�����  
*   UINT16 size�����ݳ���
*   bool isSend���Ƿ���DMA���͵��ã�����Ƿ��ͣ���Ҫ�ж���һ�η����Ƿ����
*
* @param[in] char channel,void* datasrc, void* datadst,\
    UINT16 size,bool isSend
* @param[out]:none
******************************************************************************/
void HAL_DMA_ActivateBasic(char channel,void* datasrc, void* datadst,\
    UINT16 size,bool isSend);

/***************************************************************************//**
* @brief
*   HAL_DMA_Setup_Dma   
*
* @note
*   ��ʼ��DMA����Ҫ������dmaControlBlock������ѡ��Ĭ�ϵ�����
*
* @param[in] none
* @param[out]:none
******************************************************************************/
void HAL_DMA_Setup_Dma(void);

/***************************************************************************//**
* @brief
*   HAL_DMA_Malloc_Channel   
*
* @note
*   ΪDMA�Զ�����ͨ��������ͨ��0���ַ�����󣬹ʵõ���ͨ��ֵ��СΪ1��ʵ��ʹ��ʱ
*   DMA��ͨ��Ϊ�񣨵õ�ֵ-1��
*
* @param[in] none
* @param[out]:ͨ��ֵ
******************************************************************************/
char HAL_DMA_Malloc_Channel(char channel);

/***************************************************************************//**
* @brief
*   HAL_DMA_Setup_Channel   
*
* @note
*   ����DMAͨ���ľ��������������Ͷ���DMA_PARA��
*
* @param[in] DMA_PARA cfg
* @param[out]:none
******************************************************************************/
void HAL_DMA_Setup_Channel( DMA_PARA cfg,bool isRx );


#ifdef OUTPUT_DEBUG

/***************************************************************************//**
* @brief
*   HAL_Monitor_Init_Uart   
*
* @note
*   ��ʼ��LEUART1
*
* @param[in] none
* @param[out]:none
******************************************************************************/
void HAL_Monitor_Init_Uart(void);

/***************************************************************************//**
* @brief
*   HAL_Monitor_Send_Char   
*
* @note
*   ����һ���Լ�����
*
* @param[in] UINT8 Data
* @param[out]:none
******************************************************************************/
void HAL_Monitor_Send_Char(UINT8 Data);

/***************************************************************************//**
* @brief
*   HAL_Monitor_Send_String   
*
* @note
*   ���͹涨���ȣ�length����string
*
* @param[in]��UCHAR *string,UCHAR length
* @param[out]:none
******************************************************************************/
void HAL_Monitor_Send_String(UINT8 *string,UINT8 length);

#endif

/***************************************************************************//**
* @brief
*   HAL_Get_Vision_Code   
*
* @note
*   ��vision code����ָ���ĵ�ַ��,�����س���
*
* @param[in]��none
* @param[out]:UINT8 strlen(VISION_CODE)
******************************************************************************/
UINT8 HAL_Get_Vision_Code(UINT8 *buffer);
void HAL_BT_Enable_UARTI(bool isEnabled);
void HAL_BT_SendData(UCHAR Data);
UCHAR HAL_BT_GetData(void);
void HAL_BT_Initial_IO(void);
//EP
UINT HAL_RPM_Get_EXIFlg(void);
void HAL_RPM_Clear_EXIFlg(void);
void HAL_VR2_Get_CrrntChk_IO(void);
/******************************************************************************/
#endif