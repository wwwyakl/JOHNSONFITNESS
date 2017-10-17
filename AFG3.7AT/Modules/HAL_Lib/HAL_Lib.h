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
* @ enum define,类似于 UART_TX、UART_RX、GPIO_ODD、GPIO_EVEN等的中断注册索引
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
* @ enum define,为各个功能模块需要在1Ms ISR里注册函数提供索引
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
//* @ enum define,为各个ODD中断函数需注册提供索引
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
//* @ enum define,为各个EVEN中断函数需注册提供索引
//*
//******************************************************************************/
//typedef enum tag_EVENINTISR_IDX
//{
//    POLOR_INT_PB2=0,
//    RF_GDO2_INT,
//    ALL_EVENIDXS
//}EVEN_INTISR_IDX;

/***************************************************************************//**
* @ enum define,为各个EVEN中断函数需注册提供索引
*
******************************************************************************/
typedef enum tag_LETIMER_IDX
{
    ERP_TIMER_COUNT=0,
    ALL_LETIMERIDXS
}LETIMER_IDX;

/***************************************************************************//**
* @ enum define,for power channel:  
*   Power_Channel_A:大部分外设电源
*   Power_Channel_B：reserved
*   Power_Channel_C：Power_Button_LED_PORT，reserved
*   Power_Channel_ERP：ERP Light（LED）
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
* @ enum define,for 设置内部或者外部时钟从IO输出 
*
******************************************************************************/
typedef enum tag_CLOCK_OUTPUT_MODE{
   INTERNAL_OUTPUT= 0,
   EXTERNAL_OUTPUT,
   OUTMODE_ALL
}CLOCK_OUTPUT_MODE;

/***************************************************************************//**
* @ enum define,for 设置内部或者外部时钟从IO输出 
*
******************************************************************************/
//typedef enum tag_TIMER1_CC_CHL
//{
//    TIMER1_CC_CH_HR=1,
//    TIMER1_CC_CH_POLAR,
//    DEFAULT_CH
//}TIMER1_CC_CHL;
/***************************************************************************//**
* @ enum define,for 设置内部或者外部时钟从IO输出 
*
******************************************************************************/
typedef enum tag_AMP_SOURCE_CHANNEL{
   AMP_SOURCE_CHANNEL1= 0,
   AMP_SOURCE_CHANNEL2,
   AMP_SOURCE_CHANNEL3,
   AMP_SOURCE_CHANNEL_ALL
}AMP_SOURCE_CHANNEL;

/***************************************************************************//**
* @ typedef struct,DMA初始化的数据结构图类型定义，包含数据有：
*   char channel：通道
*   DMA_CfgDescr_TypeDef    DescrCfg：参数配置，包含参数：
                            DescrCfg.dstInc：目标地址步进；
*                           DescrCfg.srcInc：数据源地址步进
*                           DescrCfg.size：每次传送的数据字节数
*                           DescrCfg.hprot：进行Arbitrate需要传输的字节数
*   UINT16 size：DMA传输的字节长度
*   DMA_FuncPtr_TypeDef callback：回调函数的指针
*   UINT32  dataselect：DMA信号源说明
*   void*  datasrc：数据源地址
*   void*  datadst：目标地址
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
*   清除IO中断Flag    
*
* @note
*  
*
* @param[in] location
*   UINT w_Pin
* @param[out]：none
******************************************************************************/
void HAL_Clear_IO_IntFlag(UINT w_Pin);

/***************************************************************************//**
* @brief
*    获取IO 中断Flag
*
* @note
*  
*
* @param[in] location
*   UINT w_Pin
* @param[out]：
*   (UCHAR)by_Stat
******************************************************************************/
UINT HAL_Get_IO_IntFlag(UINT w_Pin);

/***************************************************************************//**
* @brief
*   设置IO口为input
*
* @note
*  
* @param[in] location
*   GPIO_Port_TypeDef by_Port
*   UINT w_Pin
* @param[out]：none
******************************************************************************/
void HAL_Set_IO_Input(GPIO_Port_TypeDef by_Port, UINT w_Pin);

/***************************************************************************//**
* @brief
*    设置IO口输出低电平
*
* @note
*  
* @param[in] location
*   GPIO_Port_TypeDef by_Port
*   UINT w_Pin 
* @param[out]：none
******************************************************************************/
void HAL_Set_IO_Low(GPIO_Port_TypeDef by_Port, UINT w_Pin);

/***************************************************************************//**
* @brief
*    设置IO口为输出
*
* @note
*
* @param[in] location
*   GPIO_Port_TypeDef by_Port
*   UINT w_Pin
* @param[out]：none
******************************************************************************/
void HAL_Set_IO_Output(GPIO_Port_TypeDef by_Port, UINT w_Pin);

/***************************************************************************//**
* @brief
*    设置IO口输出高电平
*
* @note
*  
* @param[in] location
*   GPIO_Port_TypeDef by_Port
*   UINT w_Pin 
* @param[out]：none
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
* @param[out]：none
******************************************************************************/
void HAL_Enable_IO_PullUp(GPIO_Port_TypeDef by_Port, UINT w_Pin);

/***************************************************************************//**
* @brief
*  设置IO的IntMode
*
* @note
*   调用此函数默认INT是enable的， GPIO_IntConfig原型如下：
*   GPIO_IntConfig(GPIO_Port_TypeDef port,
*                   unsigned int pin,
*                   bool risingEdge,
*                   bool fallingEdge,
*                   bool enable)
* @param[in]
*   GPIO_Port_TypeDef by_Port
*   UINT w_Pin 
*   UCHAR by_Mode：控制上升沿和下降沿
*   by_Enable:使能中断
* @param[out]：none
******************************************************************************/
void HAL_Set_IO_IntMode(GPIO_Port_TypeDef by_Port, UINT w_Pin, UCHAR by_Mode,UCHAR by_Enable);
/***************************************************************************//**
* @brief
*    使能WDT
*
* @note
*  
*
* @param[in]：none
* @param[out]：none  
******************************************************************************/
void HAL_Enable_WatchDog(void);

/***************************************************************************//**
* @brief
*    清零WDT计数
*
* @note
*  
*
* @param[in]：none
* @param[out]：none 
******************************************************************************/
void HAL_Clear_WatchDog(void);

/***************************************************************************//**
* @brief
*    禁止WDT
*
* @note
*  
*
* @param[in]：none
* @param[out]：none 
******************************************************************************/
void HAL_Disable_WatchDog(void);

/***************************************************************************//**
* @brief
*    软体复位
*
* @note
*  
*
* @param[in]：none
* @param[out]：none   
******************************************************************************/
void HAL_Reset_System(void);

/***************************************************************************//**
* @brief
*    系统复位后时钟配置
*
* @note
*  
*
* @param[in]：none
* @param[out]：none   
******************************************************************************/
void HAL_Config_Clock_Active(void);

/***************************************************************************//**
* @brief
*    配置ERP mode下的时钟，关掉ERP时不需要的
*
* @note
*  
*
* @param[in]：none
* @param[out]：none 
******************************************************************************/
void HAL_Config_Clock_Standby(void);

/***************************************************************************//**
* @brief
*    enable interrrupt
*
* @note
*  
*
* @param[in]：none
* @param[out]：none 
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
* @param[in]：none
* @param[out]：none 
******************************************************************************/
__STATIC_INLINE void HAL_Disable_MasterInterrupt(void)
{
  __disable_irq();
}

/***************************************************************************//**
* @brief
*   上电后电源控制IO口初始化
*
* @note
*   初始化完成时仍处于ERP状态，所以这里并没有打开所有的电源     
*
* @param[in]：none
* @param[out]：none 
******************************************************************************/
void HAL_BOR_Initial_IO(void);

/***************************************************************************//**
* @brief
*    获取电源掉电检测IO口状态
*
* @note
*  
*
* @param[in] None
* @param[out]：（bool）pinstatus 
******************************************************************************/
bool HAL_BOR_Get_Enable(void);

/***************************************************************************//**
* @brief
*    控制外设、Power_Button、ERP电源
*
* @note
*  @ enum define,for power channel:  
*   Power_Channel_A:外设电源
*   Power_Channel_B：reserved
*   Power_Channel_C：Power_Button_LED_PORT，reserved
*   Power_Channel_ERP：ERP Light（LED）
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
*  配置CLOCK从PA1或PA2输出，pa1输出低频，pa2输出高频
*  注意调用函数时防止该函数被其他配置给冲掉
* @param[in]：CLOCK_OUTPUT_MODE mode：选择输出内部clock还是外部clock
* @param[out]：none 
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
*    烧机（寿命测试）检测IO输入配置
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
*    获取烧机（寿命测试）IO口状态
*
* @note
*    高电平表示烧机口未短路  
*    低电平表示烧录口处于短路状态
* @param[in] :none
* @param[out]:(bool)pin status
******************************************************************************/
bool HAL_LFT_Get_Enable(void);

/***************************************************************************//**
* @brief
*   RTC的CLK和RST IO初始化，设置为输出
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
*    RTC的RST管脚输出电平设置
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
*    RTC的CLK管脚输出电平设置
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
*    RTC的Data管脚输出电平设置
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
*    获取RTC的Data管脚输入状态
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
*   初始化按键的IO或者I2C按键的I2C模块
*
* @note
*   调用时要加上枚举类型为PHYKEYMODE的参数  mode，
*   KEYMODE_I2C= 0, ---->I2C按键
*   KEYMODE_IO      ---->IO/Matrix 按键
*   如：HAL_Key_Initial_IO（KEYMODE_I2C）；
* @param[in]: PHYKEYMODE mode
* @param[out]:none
******************************************************************************/
void HAL_Key_Initial_IO(PHYKEYMODE mode);

/***************************************************************************//**
* @brief
*   获取按键值，此处得到的键值需要解码
*@note
*   1.调用时要加上枚举类型为PHYKEYMODE的参数  mode，
*   KEYMODE_I2C= 0, ---->I2C按键
*   KEYMODE_IO      ---->IO/Matrix 按键
*   2.返回值为1个32bit的值，格式为0xK1K2K3K4，其中K1、K2、K3、K4分别为一个8bit
*   的数据
*   K1：代表第一个按键值所在的行
*   K2：代表第一个按键值所在的列
*   K3：代表第二个按键值所在的行
*   K4：代表第二个按键值所在的列
*   如果没有按键按下，该按键的返回值为AA：超过2个按键按下，认为无效，返回AAAA；
*   如果只有一个按键按下，返回值为 K1K2AA；
*   如果有两个按键按下，返回K1K2K3K4；
*   如果没有按键或者多于2个按键同事按下，则返回AAAAA;
*
*   UtilityKeyboard中的函数通过调用该函数得到键值后，再通过查真值表，得到相应的
*   按键值,如：
*   UINT32 KeyValue = HAL_Key_Get_Value（KEYMODE_I2C）；
*   如：KeyValue = 0x34AA---->说明只有一个按键按下，为3行4列的那个按键，查真值表
*   可得出按键的定义；
*   如：KeyValue = 0x3456---->说明有两个按键按下，为3行4列、5行6列的两个按键，查
*   看真值表可以得到按键或组合键的
*   如：KeyValue = 0xAAAA---->说明无按键按下或者大于2个的按键被按下。
*
* @param[in]: PHYKEYMODE mode
* @param[out]：UINT32 key_value  
******************************************************************************/
UINT32 HAL_Key_Get_Value(PHYKEYMODE mode);

/***************************************************************************//**
* @brief
*   I2C按键释放标志位
*
* @note
*   I2C按键提供的按键释放的标志位
*   返回值为1：按键释放
*   返回值为0：按键为未释放
*   一般用于当检测到按键释放时，可以对按键按下计时的寄存器清零
* @param[in]：none
* @param[out]：bool status
******************************************************************************/
bool HAL_Key_I2C_Get_KeyReleaseflag(void);

/***************************************************************************//**
* @brief
*    HAL_FAN_Initial_IO
*
* @note
*    初始化Fan IO输出低电平，设置占空比为0
*
* @param[in]：none
* @param[out]：none
******************************************************************************/
void HAL_FAN_Initial_IO(void);

/***************************************************************************//**
* @brief    
*   HAL_FAN_Set_Duty
*
* @note
*   可以直接设置所需要的占空比，如需要设置风扇的占空比为80%，Input参数为80，如下：     
* HAL_FAN_Set_Duty(80);
* @param[in]:UINT8 DutyValue
* @param[out]：none
******************************************************************************/
void HAL_FAN_Set_Duty(UCHAR DutyValue);
//
///***************************************************************************//**
//* @brief
//*   初始化timer1 capture中断，用于HR和polar的检测 
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
* @param[out]：none 
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
//* @param[out]：pin status 
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
//* @param[out]：pin status 
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
//* @param[out]：HGP pin int flag
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
//* @param[out]：polor pin int flag
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
//* @param[out]：none
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
//* @param[out]：none  
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
* @param[out]：none 
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
* @param[out]：none 
******************************************************************************/
void HAL_WIFI_Set_RST(bool isHighLevel);

/***************************************************************************//**
* @brief
*   HAL_WIFI_Get_RDY 
*   Get WF_Ready_PIN status
* @note
*  
*
* @param[in]：none
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
* @param[in]：none
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
*   DMA信号源说明
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
*   DMA获取RX的数据源，返回值为数据源的地址，类型为(void*)
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
*   DMA信号源说明
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
*   DMA获取TX的数据源，返回值为数据源的地址，类型为(void*)
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
*    配置SPI的IO口输入输出状态
*   
* @note
*   如果定义了DMA_SPI（DMA+硬件SPI实现RF功能），则会对硬件SPI也进行初始化；
*   若未定义，则默认使用模拟SPI。
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
*    配置RF_GDO0的中断
*    isEnable 决定是否打开中断
* @note
*
* @param[in] bool isEnable
* @param[out]: none    
******************************************************************************/
void HAL_RF_Config_GDO0(bool isEnable);

/***************************************************************************//**
* @brief:HAL_RF_Config_GDO2
*    配置RF_GDO2的中断
*    isEnable 决定是否打开中断
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
*   1、此处loop期间需要Clear WDT
*   2、此处延时参数i的数值，会在1ms中断中进行递减处理，该delay函数内是不做递减处
*   理的
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
*   在1ms中断中对HAL_DLCB_Delay给的数值进行递减
*
* @param[in]:none
* @param[out]:none
******************************************************************************/
void HAL_DLCB_1ms_Int(void);

/***************************************************************************//**
* @brief
*    HAL_DLCB_Initial_IO
*    DLCB所用的UART进行初始化
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
*   用UART发送一个字节数据
* @note
*  
*
* @param[in]:UCHAR Data
* @param[out]:none
******************************************************************************/
void HAL_DLCB_SendData(UCHAR Data);

/***************************************************************************//**
* @brief
*   关闭TX和中断 
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
*   禁止 UART_Rx_IRQ-->设置UART_485DE_PIN 输出 低电平-->设置 UART_RX_PIN 输入状态
*   -->get UART_RX_PIN state并返回状态值
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
*   设置beep的Timer 
*
* @param[in]:none
* @param[out]:none   
******************************************************************************/
void HAL_Beep_Initial_IO(void);

/***************************************************************************//**
* @brief
*   HAL_Beep_Set_Beep
*   设置beep IO 的PWM使能
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
*   1、设置Audio_Source1、2/3、PWM、MUTE等IO为输出，Check_In pin为输入
*   2、设置初始的AMP占空比，由参数传入
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
*   默认每次状态下只能打开一个source
*
* @param[in]：AMP_SOURCE_CHANNEL channel
* @param[out]： none
******************************************************************************/
void HAL_AMP_Select_Source(AMP_SOURCE_CHANNEL channel);

/***************************************************************************//**
* @brief
*   HAL_AMP_Check_AUX 
*   获取Volume_CHECK_IN_PIN 管脚状态
* @note
*  
*
* @param[in]：none
* @param[out]：获取Volume_CHECK_IN_PIN pin status   
******************************************************************************/
bool HAL_AMP_Check_AUX(void);

/***************************************************************************//**
* @brief
*   HAL_AMP_Set_Mute 
*   设置mute管脚高低电平
* @note
*
* @param[in]：bool isMuted
* @param[out]：none  
******************************************************************************/
void HAL_AMP_Set_Mute(bool isMuted);

/***************************************************************************//**
* @brief
*  HAL_AMP_Set_Duty  
*   设置AMP的占空比，如，设置占空比为75%，则：HAL_AMP_Set_Duty(75);
* @note
*  
*
* @param[in]：UCHAR DutyValue
* @param[out]： none 
******************************************************************************/
void HAL_AMP_Set_Duty(UCHAR DutyValue);

/***************************************************************************//**
* @brief
*   HAL_16XX_I2C_Write_CMD 
*   用于写chip configuration调用
* @note
*   注意地址：不同型号芯片的地址不一样，
*   16C21-->0x70
*   16C22-->
*   16C23-->
*   16C24-->0x7A
*   
* @param[in]：UINT8 Cmd,UINT8 Settings
* @param[out]：none  
******************************************************************************/
void HAL_16CXX_I2C_Write_CMD(UINT8 Chip_Type,UINT8 Cmd,UINT8 Settings);


void HAL_16CXX_I2C_Write_ShowData(UINT8 Chip_Type,UINT8 Dis_Cmd,UINT8 Dis_Address,UINT8 *pby_Lcd_Buffer,UINT8 Length);
/***************************************************************************//**
* @brief
*   HAL_16XX_Init_I2C 
*   初始化I2C模块
* @note
*   
*
* @param[in]：none
* @param[out]：none
******************************************************************************/
void HAL_16CXX_Init_I2C(void);



/***************************************************************************//**
* @brief
*   设置16XX的IO状态 
*
* @note
*  
*
* @param[in]：none
* @param[out]：none   
******************************************************************************/
void HAL_HT16XX_Initial_IO(void);
void HAL_HT16XX_Set_High_CLK(bool isHigh);

/***************************************************************************//**
* @brief
*   设置WR为高电平或者低电平   
*
* @note
*  
* @param[in]：bool isHigh
* @param[out]：none   
******************************************************************************/
void HAL_HT16XX_Set_High_WR(bool isHigh);

/***************************************************************************//**
* @brief
*   设置Data0为高电平或者低电平    
*
* @note
*  
*
* @param[in]：bool isHigh
* @param[out]：none   
******************************************************************************/
void HAL_HT162x_Initial_IO(void);
void HAL_HT16XX_Set_High_Data0(bool isHigh);

/***************************************************************************//**
* @brief
*   设置Data1为高电平或者低电平    
*
* @note
*  
*
* @param[in]：bool isHigh
* @param[out]：none   
******************************************************************************/
void HAL_HT16XX_Set_High_Data1(bool isHigh);

/***************************************************************************//**
* @brief
*   设置Data2为高电平或者低电平    
*
* @note
*  
*
* @param[in]：bool isHigh
* @param[out]：none   
******************************************************************************/
void HAL_HT16XX_Set_High_Data2(bool isHigh);

/***************************************************************************//**
* @brief
*   设置Data3为高电平或者低电平    
*
* @note
*  
*
* @param[in]：bool isHigh
* @param[out]：none   
******************************************************************************/
void HAL_HT16XX_Set_High_Data3(bool isHigh);

/***************************************************************************//**
* @brief
*   HAL_HT16XX_Set_High_CS
*   set CS及其电平
* @note
*   isHigh =1 时，设置cs为低电平
*   isHigh =0 时，设置cs为高电平
*
* @param[in]：UCHAR by_IC, bool isHigh
* @param[out]：none 
******************************************************************************/
void HAL_HT16XX_Set_High_CS(UCHAR by_IC, bool isHigh);

/***************************************************************************//**
* @brief
*   HAL_LCD_Backlight_Initial_IO 
*   设置LCD_BACKLIGHT_PIN为高电平
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
*    获取MCU一个page的size
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
*   1、传入待写入flash区域的首地址，待写入的数据以及长度，写操作完成后，是否打开
*   中断；
*   2、eg：HAL_FLASH_Burst_Write( FLASH_ADDRESS, (PCHAR*)flashBuffer, 2048 ,0);
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
*   擦除一个完整的页面。
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
*   简单软件延时
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
*  1、进入ERP之前的中断、中断向量、唤醒中断配置、时钟、LETIMER等相关配置
*  2、入口的两个参数：KeyMode决定是I2C还是IO按键，不同的按键进入ERP之前IO及中断
*   配置不一样；iSEnableLETimer决定是否需要打开LETIMER以供他用
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
*   EFM32系列单片机为进入EM2模式
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
*   1、在EM2状态下，被中断唤醒后Reset_From_ERP_Flag将被职位0x055AA，由上层读取该
*   参数来判断是否为正常唤醒并执行复位动作。
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
*   LETIMER，
*   如：Hal_ERP_Regist_LETIMER_ISR(ERP_TIMER_COUNT,LETIMER_Count);
*
* @param[in]：LETIMER_IDX index, PF pf
* @param[out]：none 
******************************************************************************/
void HAL_ERP_Regist_LETIMER_ISR(LETIMER_IDX index, PF pf);
/***************************************************************************//**
* @brief
*    注册1ms内部中断，每次进入1ms中断后会调用所注册的函数1次
*
* @note
*   调用说明：
*   1、如蜂鸣器处理中有Beep_1ms_ISR函数需要每1ms被中断调用一次，则可以先在HAL.h
*      的tag_1MSISR_IDX中增加函数Beep_1ms_ISR的索引BEEP_ISR_IDX，在beep初始化时，
*      如下调用：Hal_Regist_ISR(MS_ISR,BEEP_ISR_IDX,Beep_1ms_ISR);
*   2、如需注册类似于Digital_Cmd_Tx_Int的与SysTick_Handler平行的中断程序，则需要
*      先在HAL.h的tag_INTERRUPT_ISR中增加函数Digital_Cmd_Tx_Int的UART_TX_ISR的索
*      引，如下调用： Hal_Regist_ISR(UART_RX_ISR, 0, Digital_Cmd_Rx_Int);
* @param[in]:INTERRUPT_ISR isr, MSISR_IDX index, PF pf
* @param[out]:none
******************************************************************************/
void Hal_Regist_ISR(INTERRUPT_ISR isr, MSISR_IDX index, PF pf);
///***************************************************************************//**
//* @brief
//*    Hal_Regist_ODDINT_ISR
//*
//* @note
//*   ODDINT中断注册函数，如：Hal_Regist_ODDINT_ISR(I2C_INT_PF9,HAL_Key_I2C_IntISR) 
//*
//* @param[in]：ODD_INTISR_IDX index，PF pf
//* @param[out]：none
//******************************************************************************/
//void Hal_Regist_ODDINT_ISR(ODD_INTISR_IDX index, PF pf);
///***************************************************************************//**
//* @brief
//*   Hal_Regist_EVENINT_ISR
//*
//* @note
//*   ENENINT中断注册函数，
//*   如：Hal_Regist_EVENINT_ISR(POLOR_INT_PB2,Heart_Calculate_Polar_Pulse_Int);
//*
//* @param[in]：(EVEN_INTISR_IDX index, PF pf)
//* @param[out]：none 
//******************************************************************************/
//void Hal_Regist_EVENINT_ISR(EVEN_INTISR_IDX index, PF pf);

/***************************************************************************//**
* @brief
*   HAL_INT_Regist_Pin_INT
*
* @note
*   PIN INT中断注册函数，
*   HAL_INT_Regist_Pin_INT(PIN_INT_GDO0,RF_GDO0_Int_ISR);
*   HAL_INT_Regist_Pin_INT(PIN_INT_GDO2,RF_GDO2_Int_ISR);
*   传入枚举类型（EVEN_INTISR_IDX）参数 PIN_INT_GDO2后，HAL根据其管脚定义，将其
*   自动分配注册进中断中
* @param[in]：PIN_INT_IDX Int_Index,PF pf
* @param[out]：none 
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
//*   Get timer1 CC的通道
//*
//* @param[in] none
//* @param[out] TIMER1_CC_CHL chl
//******************************************************************************/
//TIMER1_CC_CHL HAL_Get_Timer1_CCChanel(void);
//
///***************************************************************************//**
//* @brief
//*    注册 timer1的capture中断
//*
//* @note
//*   ODD中断服务子程序
//*
//* @param[in]：none
//* @param[out]：none 
//******************************************************************************/
//void HAL_Regist_Timer1_ISR(PF pf);

/***************************************************************************//**
* @brief
*   HAL_DMA_ActivateBasic   
*
* @note
*   DMA发送或接收重新activate，传入参数有：
*   char channel：DMA通道
*   void* datasrc：数据源的地址（指针类型变量）
*   void* datadst：数据转移的目标地址（指针类型变量）  
*   UINT16 size：数据长度
*   bool isSend：是否是DMA发送调用，如果是发送，需要判断上一次发送是否结束
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
*   初始化DMA，主要是配置dmaControlBlock，其他选择默认的配置
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
*   为DMA自动分配通道，避免通道0出现分配错误，故得到的通道值最小为1；实际使用时
*   DMA的通道为获（得的值-1）
*
* @param[in] none
* @param[out]:通道值
******************************************************************************/
char HAL_DMA_Malloc_Channel(char channel);

/***************************************************************************//**
* @brief
*   HAL_DMA_Setup_Channel   
*
* @note
*   设置DMA通道的具体参数，详见类型定义DMA_PARA，
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
*   初始化LEUART1
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
*   发送一个自己数据
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
*   发送规定长度（length）的string
*
* @param[in]：UCHAR *string,UCHAR length
* @param[out]:none
******************************************************************************/
void HAL_Monitor_Send_String(UINT8 *string,UINT8 length);

#endif

/***************************************************************************//**
* @brief
*   HAL_Get_Vision_Code   
*
* @note
*   将vision code放在指定的地址上,并返回长度
*
* @param[in]：none
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