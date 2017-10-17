/***************************************************************************//**
* @file
* @brief HAL(Hardware Abstraction Layer) Library
* @author 
* @version 1.0.0
*******************************************************************************
*
******************************************************************************/

#include "HAL_Lib.h"


#ifdef BSP_CORTEX_EFM32

#ifdef EFM32LG380F256

/*******************************************************************************
*******************************   DEFINES   ***********************************
******************************************************************************/

/** Bit definition */
#ifndef BIT0
#define BIT0                    (0x00000001)
#define BIT1                    (0x00000002)
#define BIT2                    (0x00000004)
#define BIT3                    (0x00000008)
#define BIT4                    (0x00000010)
#define BIT5                    (0x00000020)
#define BIT6                    (0x00000040)
#define BIT7                    (0x00000080)
#define BIT8                    (0x00000100)
#define BIT9                    (0x00000200)
#define BITA                    (0x00000400)
#define BITB                    (0x00000800)
#define BITC                    (0x00001000)
#define BITD                    (0x00002000)
#define BITE                    (0x00004000)
#define BITF                    (0x00008000)
#endif

//typedef enum tag_INTERRUPT_ISR
//{
//	INT_ISR_START = 0,
//	UART_TX_ISR = INT_ISR_START,
//	UART_RX_ISR,
//	UART1_TX_ISR,
//	UART1_RX_ISR,
//	GPIO_ODD_ISR,
//	GPIO_EVEN_ISR,
//	ALL_ISRS
//}INTERRUPT_ISR;


/** Internal Functions */
static UCHAR HAL_Get_IO(GPIO_Port_TypeDef by_Port, UINT w_Pin);
static void HAL_Enable_IO_Int(UINT w_Pin);
static void HAL_Key_I2C_IntISR(void);
static void HAL_Key_Matrix_Scan(UINT8 index);
static UCHAR HAL_Key_Matrix_Get_Port(void);
static void HAL_Key_Matrix_1msScan_Int(void);
static UINT32 HAL_Key_Matrix_Get_Value(void);
static UINT32 HAL_Key_I2C_Get_Value(void);
__no_init static UINT16 Stop_Position;
__no_init UINT8 I2C_Key_First;
/***************************************************************************//**
* @brief
*    获取IO电平值
*
* @note
*  
*
* @param[in] location
*   GPIO_Port_TypeDef by_Port
*   UINT w_Pin 
* @param[out]：none
******************************************************************************/
static UCHAR HAL_Get_IO(GPIO_Port_TypeDef by_Port, UINT w_Pin)
{
    int data;
    if (w_Pin < 8)
    {
        data= (GPIO->P[by_Port].MODEL & (0xF << (w_Pin * 4)))>>(w_Pin * 4);
    }
    else
    {
        data= (GPIO->P[by_Port].MODEH & (0xF << ((w_Pin - 8) * 4)))>>((w_Pin - 8) * 4);
    }
    
    if(data<4)
        return( GPIO_PinInGet(by_Port,w_Pin) );
    else
        return( GPIO_PinOutGet(by_Port,w_Pin) );
}
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
void HAL_Set_IO_IntMode(GPIO_Port_TypeDef by_Port, UINT w_Pin, UCHAR by_Mode,UCHAR by_Enable)
{
    if(by_Mode)
        GPIO_IntConfig(by_Port, w_Pin, true, false, by_Enable);
    else
        GPIO_IntConfig(by_Port, w_Pin, false, true, by_Enable);
}
/***************************************************************************//**
* @brief
*   使能IO中断    
*
* @note
*  
*
* @param[in] location
*   UINT w_Pin
* @param[out]：none
******************************************************************************/
static void HAL_Enable_IO_Int(UINT w_Pin)
{
    GPIO_IntEnable(1<<w_Pin);
}
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
UINT HAL_Get_IO_IntFlag(UINT w_Pin)
{
    UINT16 by_Stat = 0;
    by_Stat = GPIO_IntGet()&(1<<w_Pin);
    return ((UINT)by_Stat);
}
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
void HAL_Clear_IO_IntFlag(UINT w_Pin)
{
    GPIO_IntClear(1<<w_Pin);
}
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
void HAL_Set_IO_Output(GPIO_Port_TypeDef by_Port, UINT w_Pin)
{
    GPIO_PinModeSet(by_Port, w_Pin, gpioModePushPull, 0);
}
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
void HAL_Set_IO_Input(GPIO_Port_TypeDef by_Port, UINT w_Pin)
{
    GPIO_PinModeSet(by_Port, w_Pin, gpioModeInput, 1);
}
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
void HAL_Set_IO_High(GPIO_Port_TypeDef by_Port, UINT w_Pin)
{
    GPIO_PinOutSet(by_Port,w_Pin);
}
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
void HAL_Set_IO_Low(GPIO_Port_TypeDef by_Port, UINT w_Pin)
{
    GPIO_PinOutClear(by_Port,w_Pin);
}
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
void HAL_Enable_IO_PullUp(GPIO_Port_TypeDef by_Port, UINT w_Pin)
{
    GPIO_PinModeSet((GPIO_Port_TypeDef)by_Port, w_Pin, gpioModeInputPull, 1);
}

//1.system hardware
/***************************************************************************//**
* @brief
*    WDT初始化，配置时钟为Ultra low frequency (1 kHz)&2049 clock periods，超时方
*   案为约2s
*
* @note
*   在HAL_Config_Clock_Active中被调用（非debug模式）
*
* @param[in]：None 
* @param[out]：None   
******************************************************************************/
static void HAL_WDT_Initial_IO(void)
{
    /* Defining the watchdog initialization data */
    WDOG_Init_TypeDef init =
    {
        .enable     = true,                 					/* Start watchdog when init done */
        .debugRun   = false,                				/* WDOG not counting during debug halt */
        .em2Run     = false,                				/* WDOG counting when in EM2 */
        .em3Run     = false,                				/* WDOG counting when in EM3 */
        .em4Block   = false,                				/* EM4 can be entered */
        .swoscBlock = false,               				/* Do not block disabling LFRCO/LFXO in CMU */
        .lock       = false,                					/* Do not lock WDOG configuration (if locked, reset needed to unlock) */
        .clkSel     = wdogClkSelLFRCO,     	/* Select LFRCO (32.768Hz) clock */
        .perSel     = wdogPeriod_64k,       		/* Set the watchdog period to 16K clock periods (ie ~0.5 second)*/
    };

    WDOG_Init(&init); 								/* Initializing watchdog with choosen settings */
    while (WDOG->SYNCBUSY & WDOG_SYNCBUSY_CTRL);
    WDOG_Feed();
}
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
void HAL_Enable_WatchDog(void)
{
    WDOG_Enable(1);
}
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
void HAL_Clear_WatchDog(void)
{
    WDOG_Feed();
}
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
void HAL_Disable_WatchDog(void)
{
    WDOG_Enable(0);
}
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
void HAL_Reset_System(void)
{
    NVIC_SystemReset();
}
/***************************************************************************//**
* @brief
*    系统上电后时钟配置
*
* @note
*  
*
* @param[in]：none
* @param[out]：none   
******************************************************************************/
void HAL_Config_Clock_Active(void)
{
  CHIP_Init();
  
  MSC->READCTRL = (MSC->READCTRL & ~_MSC_READCTRL_MODE_MASK) | MSC_READCTRL_MODE_WS2;
  
  SystemHFXOClockSet(48000000);
  
  SystemCoreClockUpdate();
  
  CMU->HFCORECLKDIV |= CMU_HFCORECLKDIV_HFCORECLKLEDIV;
  CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
  
  setupSWOForPrint();

  /*enable cmuOsc_LFRCO & cmuOsc_ULFRCO */
  CMU_OscillatorEnable(cmuOsc_LFRCO, true, true);
  CMU_OscillatorEnable(cmuOsc_ULFRCO, true, true);
  CMU_ClockEnable(cmuClock_HFPER, true);
  CMU_ClockEnable(cmuClock_GPIO, true);
  CMU_ClockEnable(cmuClock_CORELE, true);
  CMU_ClockEnable(cmuClock_TIMER1, true);
  CMU_ClockEnable(cmuClock_TIMER2, true);
  CMU_ClockEnable(cmuClock_ADC0, true);
  CMU_ClockEnable(cmuClock_DMA, true);
  CMU_ClockEnable(cmuClock_USB, true);
  CMU_ClockEnable(cmuClock_USART0, true);
  CMU_ClockEnable(cmuClock_USART1, true);
  CMU_ClockEnable(cmuClock_UART1, true);
  CMU_ClockEnable(cmuClock_I2C0, true);
  CMU_ClockEnable(cmuClock_I2C1, true);
  CMU_ClockEnable(cmuClock_USART2, true);
  CMU_ClockEnable(cmuClock_DAC0, true);
  CMU_ClockEnable(cmuClock_TIMER0, true);
  CMU_ClockEnable(cmuClock_PRS, true);
  CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_CORELEDIV2);
  CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_CORELEDIV2);
  
  CMU_ClockEnable(cmuClock_LEUART0, true);
#ifdef OUTPUT_DEBUG
  CMU_ClockEnable(cmuClock_LEUART1, true);
#endif
  CMU_ClockEnable(cmuClock_LETIMER0, true);
  CMU_ClockDivSet(cmuClock_LETIMER0, cmuClkDiv_64);
#ifdef WDT_ENABLE
  HAL_WDT_Initial_IO();
#endif 
  SysTick_Config(SystemCoreClock / 1000);
}
/***************************************************************************//**
* @brief
*    配置ERP mode下的时钟
*
* @note
*  
*
* @param[in]：none
* @param[out]：none 
******************************************************************************/
void HAL_Config_Clock_Standby(void)
{
    //clock disabled list
    CMU_ClockEnable(cmuClock_TIMER1, false);
    CMU_ClockEnable(cmuClock_TIMER2, false);
    CMU_ClockEnable(cmuClock_DMA, false);
    CMU_ClockEnable(cmuClock_USB, false);
    CMU_ClockEnable(cmuClock_USART0, false);
    CMU_ClockEnable(cmuClock_USART2, false);
    CMU_ClockEnable(cmuClock_DAC0, false);
    CMU_ClockEnable(cmuClock_LETIMER0, true);
    CMU_ClockEnable(cmuClock_ADC0, false);
    CMU_ClockEnable(cmuClock_LEUART0, false);
    /*for wifi,for RS485*/
    CMU_ClockEnable(cmuClock_USART1, false);
}
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
void HAL_Sys_Clock_Output_Enable(CLOCK_OUTPUT_MODE mode)
{
    CMU_ClockEnable(cmuClock_GPIO,true); //使能GPIO模块
    GPIO_PinModeSet(gpioPortA, 1, gpioModePushPull, 1); //将相应的GPIO口设置成输出，低频,PD8 替换PA1
    GPIO_PinModeSet(gpioPortA, 2, gpioModePushPull, 1); //将相应的GPIO口设置成输出，高频
    
    if(mode==INTERNAL_OUTPUT)
    {
        CMU->CTRL |= CMU_CTRL_CLKOUTSEL0_HFRCO + CMU_CTRL_CLKOUTSEL1_LFRCO;//CMU_CTRL_CLKOUTSEL0_HFXO; //配置为HFRCO输出
        CMU->ROUTE = CMU_ROUTE_CLKOUT0PEN | CMU_ROUTE_CLKOUT1PEN | CMU_ROUTE_LOCATION_LOC0;
    }
    else if(mode==EXTERNAL_OUTPUT)
    {
        CMU->CTRL |= CMU_CTRL_CLKOUTSEL0_HFXO + CMU_CTRL_CLKOUTSEL1_LFXO;
        CMU->ROUTE = CMU_ROUTE_CLKOUT0PEN | CMU_ROUTE_CLKOUT1PEN | CMU_ROUTE_LOCATION_LOC0;
    }
}
//2.Power management
/***************************************************************************//**
* @brief
*   上电后电源控制IO口初始化
*
* @note
*   初始化完成时仍处于ERP状态     
*
* @param[in]：none
* @param[out]：none 
******************************************************************************/
void HAL_BOR_Initial_IO(void)
{
    /*power fail detect config*/
    HAL_Set_IO_Input(Power_Fail_PORT, Power_Fail_PIN);
    
    /*close ERP Light*/
    HAL_Set_IO_Output(ERP_LED_PORT,ERP_LED_PIN);
    HAL_Set_IO_High(ERP_LED_PORT,ERP_LED_PIN);
    
    /*close EN1*/
    HAL_Set_IO_Output(POWER_EN1_PORT, POWER_EN1_PIN);
    HAL_Set_IO_Low(POWER_EN1_PORT, POWER_EN1_PIN);
    
    /*close EN2*/
    HAL_Set_IO_Output(POWER_EN2_PORT, POWER_EN2_PIN);
    HAL_Set_IO_Low(POWER_EN2_PORT, POWER_EN2_PIN);
    
    /*close Power_Button_LED_PORT */
    HAL_Set_IO_Output(Power_Button_LED_PORT, Power_Button_LED_PIN);
    HAL_Set_IO_Low(Power_Button_LED_PORT, Power_Button_LED_PIN);
}

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
bool HAL_BOR_Get_Enable(void)
{
    return (HAL_Get_IO(Power_Fail_PORT, Power_Fail_PIN));
}

/***************************************************************************//**
* @brief
*    控制外设、Power_Button、ERP电源
*
* @note
*  
*
* @param[in]: UCHAR channel,bool isEnable
* @param[out]:none   
******************************************************************************/
void HAL_BOR_Enable_ExtPower(PWR_CHANNEL channel,bool isEnable)
{
	EFM_ASSERT(channel<ALL_CHANNELS);

	switch(channel)
	{
		case Power_Channel_A:
		{
			if(isEnable)
			{
				HAL_Set_IO_High(POWER_EN1_PORT, POWER_EN1_PIN);
			}
			else
			{
				HAL_Set_IO_Low(POWER_EN1_PORT, POWER_EN1_PIN);
			}
		}
		break;
		case Power_Channel_B:
		{
			if(isEnable)
			{
				HAL_Set_IO_High(POWER_EN2_PORT, POWER_EN2_PIN);
			}
			else
			{
				HAL_Set_IO_Low(POWER_EN2_PORT, POWER_EN2_PIN);
			}
		}
		break;
		case Power_Channel_C:
		{
			if(isEnable)
			{
				HAL_Set_IO_High(Power_Button_LED_PORT, Power_Button_LED_PIN);
			}
			else
			{
				HAL_Set_IO_Low(Power_Button_LED_PORT, Power_Button_LED_PIN);
			}
		}
		break;          
		case Power_Channel_ERP:
		{
			/* ERP LED */
			if(isEnable)
			{
				HAL_Set_IO_Low(ERP_LED_PORT, ERP_LED_PIN);
			}
			else
			{
				HAL_Set_IO_High(ERP_LED_PORT, ERP_LED_PIN);
			}
		}
		break;            
		default:
		break;   
		}
}

//3.SWO Debug
/***************************************************************************//**
* @brief
*  This is a Serial Wire Output based console.
*
* @note
*  1. Set up the Serial Wire Output in your application.
   You can use the function below for this.
   NOTE: This funtion is different than the one used
   in the profiler!
2. To print a single character use the CMSIS function
   ITM_SendChar(char c).
*
* @param[in] :none
* @param[out]:none
******************************************************************************/
void setupSWOForPrint(void)
{
  /* Enable GPIO clock. */
  CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_GPIO;

  /* Enable Serial wire output pin */
  GPIO->ROUTE |= GPIO_ROUTE_SWOPEN;

#if defined(_EFM32_GIANT_FAMILY) || defined(_EFM32_LEOPARD_FAMILY) || defined(_EFM32_WONDER_FAMILY)
  /* Set location 0 */
  GPIO->ROUTE = (GPIO->ROUTE & ~(_GPIO_ROUTE_SWLOCATION_MASK)) | GPIO_ROUTE_SWLOCATION_LOC0;

  /* Enable output on pin - GPIO Port F, Pin 2 */
  GPIO->P[5].MODEL &= ~(_GPIO_P_MODEL_MODE2_MASK);
  GPIO->P[5].MODEL |= GPIO_P_MODEL_MODE2_PUSHPULL;
#else
  /* Set location 1 */
  GPIO->ROUTE = (GPIO->ROUTE & ~(_GPIO_ROUTE_SWLOCATION_MASK)) |GPIO_ROUTE_SWLOCATION_LOC1;
  /* Enable output on pin */
  GPIO->P[2].MODEH &= ~(_GPIO_P_MODEH_MODE15_MASK);
  GPIO->P[2].MODEH |= GPIO_P_MODEH_MODE15_PUSHPULL;
#endif

  /* Enable debug clock AUXHFRCO */
  CMU->OSCENCMD = CMU_OSCENCMD_AUXHFRCOEN;

  /* Wait until clock is ready */
  while (!(CMU->STATUS & CMU_STATUS_AUXHFRCORDY));

  /* Enable trace in core debug */
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  ITM->LAR  = 0xC5ACCE55;
  ITM->TER  = 0x0;
  ITM->TCR  = 0x0;
  TPI->SPPR = 2;
  TPI->ACPR = 0xf;
  ITM->TPR  = 0x0;
  DWT->CTRL = 0x400003FE;
  ITM->TCR  = 0x0001000D;
  TPI->FFCR = 0x00000100;
  ITM->TER  = 0x1;
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
//4.On Line Debug
//void HAL_Debug_Initial_IO(void)
//{
//
//}
//
//void HAL_Debug_Printf(*fmt,va_list)
//{
//
//}

//5.Lifetime testing
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
void HAL_LFT_Initial_IO(void)
{
    HAL_Set_IO_Input(BURN_PORT, BURN_PIN);
}

/***************************************************************************//**
* @brief
*    获取烧机（寿命测试）IO口状态
*
* @note
*  
*
* @param[in] :none
* @param[out]:(bool)pin status
******************************************************************************/
bool HAL_LFT_Get_Enable(void)
{
    return(HAL_Get_IO(BURN_PORT, BURN_PIN));
}

//6.RTC function
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
void HAL_RTC_Initial_IO(void)
{
    HAL_Set_IO_Output(HT1381_CLK_PORT, HT1381_CLK_PIN);
    HAL_Set_IO_Output(HT1381_RST_PORT, HT1381_RST_PIN);
}
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
void HAL_RTC_Set_RST(bool isHighLevel)
{
    if(isHighLevel != 0)
    {
        HAL_Set_IO_High(HT1381_RST_PORT, HT1381_RST_PIN);
    }
    else
    {
        HAL_Set_IO_Low(HT1381_RST_PORT, HT1381_RST_PIN);
    }
}

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
void HAL_RTC_Set_CLK(bool isHighLevel)
{
    if(isHighLevel != 0)
    {
        HAL_Set_IO_High(HT1381_CLK_PORT, HT1381_CLK_PIN);
    }
    else
    {
        HAL_Set_IO_Low(HT1381_CLK_PORT, HT1381_CLK_PIN);
    }
}
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
void HAL_RTC_Set_DAT(bool isHighLevel)
{
    HAL_Set_IO_Output(HT1381_DAT_PORT, HT1381_DAT_PIN);
    if(isHighLevel != 0)
    {
        HAL_Set_IO_High(HT1381_DAT_PORT, HT1381_DAT_PIN);
    }
    else
    {
        HAL_Set_IO_Low(HT1381_DAT_PORT, HT1381_DAT_PIN);
    }
}
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
bool  HAL_RTC_Get_Data(void)
{
    HAL_Set_IO_Input(HT1381_DAT_PORT, HT1381_DAT_PIN);
    return(HAL_Get_IO(HT1381_DAT_PORT, HT1381_DAT_PIN));
}

//7.key fuction

#define	KB_NA	0xFF
#define	K_C0_D0	0x07
#define	K_C0_D1	0x0F
#define	K_C0_D2	0x17
#define	K_C0_D3	0x1F
#define	K_C0_D4	0x27
#define	K_C0_D5	0x2F
#define	K_C0_D6	0x37
#define	K_C0_D7	0x3F
#define	K_C1_D0	0x06
#define	K_C1_D1	0x0E
#define	K_C1_D2	0x16
#define	K_C1_D3	0x1E
#define	K_C1_D4	0x26
#define	K_C1_D5	0x2E
#define	K_C1_D6	0x36
#define	K_C1_D7	0x3E
#define	K_C2_D0	0x05
#define	K_C2_D1	0x0D
#define	K_C2_D2	0x15
#define	K_C2_D3	0x1D
#define	K_C2_D4	0x25
#define	K_C2_D5	0x2D
#define	K_C2_D6	0x35
#define	K_C2_D7	0x3D
#define	K_C3_D0	0x04
#define	K_C3_D1	0x0C
#define	K_C3_D2	0x14
#define	K_C3_D3	0x1C
#define	K_C3_D4	0x24
#define	K_C3_D5	0x2C
#define	K_C3_D6	0x34
#define	K_C3_D7	0x3C
#define	K_C4_D0	0x03
#define	K_C4_D1	0x0B
#define	K_C4_D2	0x13
#define	K_C4_D3	0x1B
#define	K_C4_D4	0x23
#define	K_C4_D5	0x2B
#define	K_C4_D6	0x33
#define	K_C4_D7	0x3B
#define	K_C5_D0	0x02
#define	K_C5_D1	0x0A
#define	K_C5_D2	0x12
#define	K_C5_D3	0x1A
#define	K_C5_D4	0x22
#define	K_C5_D5	0x2A
#define	K_C5_D6	0x32
#define	K_C5_D7	0x3A
#define	K_C6_D0	0x01
#define	K_C6_D1	0x09
#define	K_C6_D2	0x11
#define	K_C6_D3	0x19
#define	K_C6_D4	0x21
#define	K_C6_D5	0x29
#define	K_C6_D6	0x31
#define	K_C6_D7	0x39
#define	K_C7_D0	0x00
#define	K_C7_D1	0x08
#define	K_C7_D2	0x10
#define	K_C7_D3	0x18
#define	K_C7_D4	0x20
#define	K_C7_D5	0x28
#define	K_C7_D6	0x30
#define	K_C7_D7	0x38
/** for key matrix */
#define                         C_MEM_SIZE  8
static char by_Index = 0;
static char aby_KEY_Buffer[C_MEM_SIZE];

/** for key I2C Data */
unsigned char KB_Flag;
static bool Key_Releaseflag = 1;
__no_init unsigned char trans_data[10];
static I2C_TransferReturn_TypeDef trans_return_value;   
#if 1//def __PETER_TEST__
static UINT16 gCurrentKeyNumber;
static UINT16 gLastKeyNumber;
static UINT16 gValidKeyNumber;
static UINT16 gKeyCounter;
#endif

/***************************************************************************//**
* @brief
*    初始化按键的IO或者I2C按键的I2C模块
*
* @note
*  
*
* @param[in]: PHYKEYMODE mode
* @param[out]:none
******************************************************************************/
void HAL_Key_Initial_IO(PHYKEYMODE mode)
{
    EFM_ASSERT(mode < KEYMODE_ALL);

    if(mode==KEYMODE_IO)
    {
        HAL_Set_IO_Input(KEYBOARD_SCAN0_PORT, KEYBOARD_SCAN0_PIN);
        HAL_Set_IO_Input(KEYBOARD_SCAN1_PORT, KEYBOARD_SCAN1_PIN);
        HAL_Set_IO_Input(KEYBOARD_SCAN2_PORT, KEYBOARD_SCAN2_PIN);
        HAL_Set_IO_Input(KEYBOARD_SCAN3_PORT, KEYBOARD_SCAN3_PIN);
        HAL_Set_IO_Input(KEYBOARD_SCAN4_PORT, KEYBOARD_SCAN4_PIN);
        HAL_Set_IO_Input(KEYBOARD_SCAN5_PORT, KEYBOARD_SCAN5_PIN);
        HAL_Set_IO_Input(KEYBOARD_SCAN6_PORT, KEYBOARD_SCAN6_PIN);
        HAL_Set_IO_Input(KEYBOARD_SCAN7_PORT, KEYBOARD_SCAN7_PIN);
        
        HAL_Set_IO_High(KEYBOARD_SCAN0_PORT, KEYBOARD_SCAN0_PIN);
        HAL_Set_IO_High(KEYBOARD_SCAN1_PORT, KEYBOARD_SCAN1_PIN);
        HAL_Set_IO_High(KEYBOARD_SCAN2_PORT, KEYBOARD_SCAN2_PIN);
        HAL_Set_IO_High(KEYBOARD_SCAN3_PORT, KEYBOARD_SCAN3_PIN);
        HAL_Set_IO_High(KEYBOARD_SCAN4_PORT, KEYBOARD_SCAN4_PIN);
        HAL_Set_IO_High(KEYBOARD_SCAN5_PORT, KEYBOARD_SCAN5_PIN);
        HAL_Set_IO_High(KEYBOARD_SCAN6_PORT, KEYBOARD_SCAN6_PIN);
        HAL_Set_IO_High(KEYBOARD_SCAN7_PORT, KEYBOARD_SCAN7_PIN);
        
        HAL_Set_IO_Input(KEYBOARD_KeyIn0_PORT, KEYBOARD_KeyIn0_PIN);
        HAL_Set_IO_Input(KEYBOARD_KeyIn1_PORT, KEYBOARD_KeyIn1_PIN);
        HAL_Set_IO_Input(KEYBOARD_KeyIn2_PORT, KEYBOARD_KeyIn2_PIN);
        HAL_Set_IO_Input(KEYBOARD_KeyIn3_PORT, KEYBOARD_KeyIn3_PIN);
        HAL_Set_IO_Input(KEYBOARD_KeyIn4_PORT, KEYBOARD_KeyIn4_PIN);
        HAL_Set_IO_Input(KEYBOARD_KeyIn5_PORT, KEYBOARD_KeyIn5_PIN);
        HAL_Set_IO_Input(KEYBOARD_KeyIn6_PORT, KEYBOARD_KeyIn6_PIN);
        HAL_Set_IO_Input(KEYBOARD_KeyIn7_PORT, KEYBOARD_KeyIn7_PIN);
        
        /* rigist 1ms scan int,I2C do not use  */
		HAL_SystemTicker_Alloc_ISR(HAL_Key_Matrix_1msScan_Int);
	}
    else
    {
        /* GPIO Extern input Interrupt config  */
        NVIC_EnableIRQ(GPIO_ODD_IRQn);
        GPIO_PinModeSet(I2CKEY_INT_PORT, I2CKEY_INT_PIN, gpioModeInput, 1);
        GPIO_IntConfig(I2CKEY_INT_PORT,I2CKEY_INT_PIN,false,true,true);
        
        /* i2c gpio config  */
        GPIO_PinModeSet(KEY_SDA_PORT, KEY_SDA_PIN, gpioModeWiredAndPullUpFilter, 1);
        GPIO_PinModeSet(KEY_SCL_PORT, KEY_SCL_PIN, gpioModeWiredAndPullUpFilter, 1);
		
        GPIO_PinModeSet(I2CKEY_RST_PORT, I2CKEY_RST_PIN,gpioModePushPull,1);
        for(char i=0;i<50;i++);  
        GPIO_PinOutClear(I2CKEY_RST_PORT, I2CKEY_RST_PIN);
        for(char i=0;i<50;i++);
        GPIO_PinOutSet(I2CKEY_RST_PORT, I2CKEY_RST_PIN);
        for(char i=0;i<50;i++);
        
        /* enable scl & sda pin function  */
        I2C0->ROUTE = I2C_ROUTE_LOCATION_LOC0 + I2C_ROUTE_SDAPEN + I2C_ROUTE_SCLPEN;  
        
        I2C_Init_TypeDef i2c_para;
        i2c_para.enable = 1;
        i2c_para.master = 1;
        i2c_para.refFreq = 0;
        i2c_para.freq = I2C_FREQ_FAST_MAX;
        i2c_para.clhr = i2cClockHLRAsymetric;
        
        I2C_TransferSeq_TypeDef i2c_data;  
        i2c_data.addr = 0xB0;
        i2c_data.flags = I2C_FLAG_WRITE_WRITE;
        i2c_data.buf[0].data = &trans_data[0];
        i2c_data.buf[0].len = 1;
        i2c_data.buf[1].data = &trans_data[1];
        i2c_data.buf[1].len = 1;
        
        trans_data[0] = 0x08;
        trans_data[1] = 0x34;
        
        /* wait power supply stability */
        for(int i = 0;i <0x4FFF;i++);  
        
        /* init i2c module  */
        I2C_Init(I2C0,&i2c_para);
        trans_return_value = I2C_TransferInit(I2C0,&i2c_data);  
        
        while (trans_return_value == i2cTransferInProgress)
        {
            trans_return_value = I2C_Transfer(I2C0);
        }
        #if 1//def __PETER_TEST__
        gCurrentKeyNumber = 0;
            gLastKeyNumber = 0;
            gValidKeyNumber = 0;
            gKeyCounter = 600;
        #endif
 #if 0       
        /* read config value  used for debug  */
        i2c_data.flags = I2C_FLAG_WRITE_READ;
        i2c_data.buf[1].data = &trans_data[3];  
        trans_return_value = I2C_TransferInit(I2C0,&i2c_data);   
        while (trans_return_value == i2cTransferInProgress)
        {
            trans_return_value = I2C_Transfer(I2C0);
        }
#endif        
        /* rigist I2C ODDINT  */   
        HAL_PININT_Alloc_ISR(HAL_Key_I2C_IntISR); 
		I2C_Key_First = 1;		
		
		Stop_Position = 0;
    }
}
/***************************************************************************//**
* @brief
*    获取按键检测的值，分IO key和I2C key两种
*
* @note
*  
*
* @param[in]: PHYKEYMODE mode
* @param[out]：UINT32 key_value  
******************************************************************************/
UINT32 HAL_Key_Get_Value(PHYKEYMODE mode)
{
    EFM_ASSERT(mode < KEYMODE_ALL);

    if(mode == KEYMODE_IO)
    {
        return(HAL_Key_Matrix_Get_Value());
    }
    else
    {
        return(HAL_Key_I2C_Get_Value());
    }
}
/***************************************************************************//**
* @brief
*    I2C按键释放标志位
*
* @note
*  
*
* @param[in]：none
* @param[out]：bool status
******************************************************************************/
bool HAL_Key_I2C_Get_KeyReleaseflag(void)
{
    return(Key_Releaseflag);
}
/***************************************************************************//**
* @brief
*    I2C按键用的Pin中断 
*
* @note
*  
*
* @param[in]：none
* @param[out]：none
******************************************************************************/
static UINT32 keyi2c = 0; 
static UINT8 keycheckcnt = 0;
void HAL_Key_I2C_Release(void);

#if 1//def __PETER_TEST__
UINT16 getCurrentKeyNumber(void)
{
    return gCurrentKeyNumber;
}
UINT16 getValidKeyNumber(void)
{
    return gValidKeyNumber;
}
UINT16 getLastKeyNumber(void)
{
    return gLastKeyNumber;
}
#endif
UINT32 Hal_Get_KeyValue(void)
{
	UINT32 ret ;
       if((keycheckcnt >90)&&(Key_Releaseflag==0))
	{
            keyi2c = 0xAAAAAAAA;
		HAL_Key_I2C_Release( );
	}
	ret = keyi2c;
	return ret;
}
static void HAL_Key_I2C_IntISR(void)
{
	
    if(HAL_Get_IO_IntFlag(I2CKEY_INT_PIN))
    {
        extern void Keyboard_Initial_Data(void);
        extern bool Key_Processflag;
        #if 1//def __PETER_TEST__
        gCurrentKeyNumber ++;
        #endif
        HAL_Clear_IO_IntFlag(I2CKEY_INT_PIN);
	   keycheckcnt = 0 ;
		if(HAL_Get_IO(I2CKEY_INT_PORT, I2CKEY_INT_PIN) ) return ;		
    #if 1//def __PETER_TEST__
        gValidKeyNumber ++;
    #endif
        UINT32 keyv=   HAL_Key_Get_Value(KEYMODE_I2C);
        if(keyv != 0XAAAAAAAA )
        {
        	if(keyi2c!=keyv)
    		{
				Keyboard_Initial_Data();
			}
            keyi2c =keyv;		    
        }
        /*每个中断要求清零自己的中断标志*/
    }
}

/***************************************************************************//**
* @brief
*    矩阵按键的扫描程序
*
* @note
*  
*
* @param[in] ：index
* @param[out]：none 
******************************************************************************/
static void HAL_Key_Matrix_Scan(UINT8 index)
{
    switch(by_Index)
    {
        case 0:
        {
            HAL_Set_IO_Input(KEYBOARD_SCAN7_PORT, KEYBOARD_SCAN7_PIN);
            HAL_Set_IO_Output(KEYBOARD_SCAN0_PORT, KEYBOARD_SCAN0_PIN);
            HAL_Set_IO_High(KEYBOARD_SCAN0_PORT, KEYBOARD_SCAN0_PIN);
            break;
        }
        case 1:
        {
            HAL_Set_IO_Input(KEYBOARD_SCAN0_PORT, KEYBOARD_SCAN0_PIN);
            HAL_Set_IO_Output(KEYBOARD_SCAN1_PORT, KEYBOARD_SCAN1_PIN);
            HAL_Set_IO_High(KEYBOARD_SCAN1_PORT, KEYBOARD_SCAN1_PIN);
            break;
        }
        case 2:
        {
            HAL_Set_IO_Input(KEYBOARD_SCAN1_PORT, KEYBOARD_SCAN1_PIN);
            HAL_Set_IO_Output(KEYBOARD_SCAN2_PORT, KEYBOARD_SCAN2_PIN);
            HAL_Set_IO_High(KEYBOARD_SCAN2_PORT, KEYBOARD_SCAN2_PIN);
            break;
        }
        case 3:
        {
            HAL_Set_IO_Input(KEYBOARD_SCAN2_PORT, KEYBOARD_SCAN2_PIN);
            HAL_Set_IO_Output(KEYBOARD_SCAN3_PORT, KEYBOARD_SCAN3_PIN);
            HAL_Set_IO_High(KEYBOARD_SCAN3_PORT, KEYBOARD_SCAN3_PIN);
            break;
        }
        case 4:
        {
            HAL_Set_IO_Input(KEYBOARD_SCAN3_PORT, KEYBOARD_SCAN3_PIN);
            HAL_Set_IO_Output(KEYBOARD_SCAN4_PORT, KEYBOARD_SCAN4_PIN);
            HAL_Set_IO_High(KEYBOARD_SCAN4_PORT, KEYBOARD_SCAN4_PIN);
            break;
        }
        case 5:
        {
            HAL_Set_IO_Input(KEYBOARD_SCAN4_PORT, KEYBOARD_SCAN4_PIN);
            HAL_Set_IO_Output(KEYBOARD_SCAN5_PORT, KEYBOARD_SCAN5_PIN);
            HAL_Set_IO_High(KEYBOARD_SCAN5_PORT, KEYBOARD_SCAN5_PIN);
            break;
        }
        case 6:
        {
            HAL_Set_IO_Input(KEYBOARD_SCAN5_PORT, KEYBOARD_SCAN5_PIN);
            HAL_Set_IO_Output(KEYBOARD_SCAN6_PORT, KEYBOARD_SCAN6_PIN);
            HAL_Set_IO_High(KEYBOARD_SCAN6_PORT, KEYBOARD_SCAN6_PIN);
            break;
        }
        case 7:
        {
            HAL_Set_IO_Input(KEYBOARD_SCAN6_PORT, KEYBOARD_SCAN6_PIN);
            HAL_Set_IO_Output(KEYBOARD_SCAN7_PORT, KEYBOARD_SCAN7_PIN);
            HAL_Set_IO_High(KEYBOARD_SCAN7_PORT, KEYBOARD_SCAN7_PIN);
            break;
        }
        default:break;
    }
}
/***************************************************************************//**
* @brief
*    矩阵键盘的按键扫描程序
*
* @note
*
* @param[in]：none
* @param[out]：按键检测值
******************************************************************************/
static UCHAR HAL_Key_Matrix_Get_Port(void)
{
	UINT by_Out = 0;
    
	by_Out |= (HAL_Get_IO(KEYBOARD_KeyIn0_PORT, KEYBOARD_KeyIn0_PIN) << 0);
	by_Out |= (HAL_Get_IO(KEYBOARD_KeyIn1_PORT, KEYBOARD_KeyIn1_PIN) << 1);
	by_Out |= (HAL_Get_IO(KEYBOARD_KeyIn2_PORT, KEYBOARD_KeyIn2_PIN) << 2);
	by_Out |= (HAL_Get_IO(KEYBOARD_KeyIn3_PORT, KEYBOARD_KeyIn3_PIN) << 3);
	by_Out |= (HAL_Get_IO(KEYBOARD_KeyIn4_PORT, KEYBOARD_KeyIn4_PIN) << 4);
	by_Out |= (HAL_Get_IO(KEYBOARD_KeyIn5_PORT, KEYBOARD_KeyIn5_PIN) << 5);
	by_Out |= (HAL_Get_IO(KEYBOARD_KeyIn6_PORT, KEYBOARD_KeyIn6_PIN) << 6);
	by_Out |= (HAL_Get_IO(KEYBOARD_KeyIn7_PORT, KEYBOARD_KeyIn7_PIN) << 7);
    
	return (by_Out);
}
/***************************************************************************//**
* @brief
*    矩阵键盘的1ms扫描中断
*
* @note
*    内含常用的20ms中断
*
* @param[in]：none
* @param[out]：none
******************************************************************************/
static void HAL_Key_Matrix_1msScan_Int(void)
{
    static UINT8 Counter20MS=0;
    
	if(++Counter20MS>=20)
	{
        Counter20MS = 0 ;
        KB_Flag = 1;
	}
    
    aby_KEY_Buffer[by_Index++] = HAL_Key_Matrix_Get_Port();
    
    if(by_Index >= C_MEM_SIZE)	by_Index = 0;
    
    HAL_Key_Matrix_Scan(by_Index);
}
/***************************************************************************//**
* @brief
*    矩阵键盘数据处理并返回
*
* @note
*  
*
* @param[in]：none
* @param[out]：UINT32 value
******************************************************************************/
static UINT32 HAL_Key_Matrix_Get_Value(void)
{
	unsigned char  Key_Matrix_Array[2][2];
    
	unsigned char  by_Count = 0 ;
    
	UINT32 ReturnValue;
	
	if(!KB_Flag)
		return(0xAAAAAAAA);
	KB_Flag = 0;
    
	if(aby_KEY_Buffer[0] & BIT0) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 0;++by_Count; }
	if(aby_KEY_Buffer[0] & BIT1) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 1;++by_Count; }
	if(aby_KEY_Buffer[0] & BIT2) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 2;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[0] & BIT3) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 3;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[0] & BIT4) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 4;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[0] & BIT5) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 5;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[0] & BIT6) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 6;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[0] & BIT7) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 7;if(++by_Count>=3) return(0xAAAAAAAA);}
    
	if(aby_KEY_Buffer[1] & BIT0) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 0;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[1] & BIT1) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 1;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[1] & BIT2) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 2;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[1] & BIT3) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 3;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[1] & BIT4) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 4;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[1] & BIT5) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 5;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[1] & BIT6) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 6;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[1] & BIT7) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 7;if(++by_Count>=3) return(0xAAAAAAAA);}
    
	if(aby_KEY_Buffer[2] & BIT0) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 0;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[2] & BIT1) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 1;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[2] & BIT2) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 2;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[2] & BIT3) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 3;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[2] & BIT4) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 4;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[2] & BIT5) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 5;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[2] & BIT6) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 6;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[2] & BIT7) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 7;if(++by_Count>=3) return(0xAAAAAAAA);}
    
	if(aby_KEY_Buffer[3] & BIT0) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 0;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[3] & BIT1) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 1;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[3] & BIT2) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 2;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[3] & BIT3) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 3;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[3] & BIT4) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 4;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[3] & BIT5) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 5;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[3] & BIT6) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 6;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[3] & BIT7) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 7;if(++by_Count>=3) return(0xAAAAAAAA);}
    
	if(aby_KEY_Buffer[4] & BIT0) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 0;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[4] & BIT1) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 1;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[4] & BIT2) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 2;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[4] & BIT3) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 3;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[4] & BIT4) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 4;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[4] & BIT5) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 5;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[4] & BIT6) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 6;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[4] & BIT7) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 7;if(++by_Count>=3) return(0xAAAAAAAA);}
    
	if(aby_KEY_Buffer[5] & BIT0) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 0;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[5] & BIT1) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 1;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[5] & BIT2) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 2;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[5] & BIT3) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 3;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[5] & BIT4) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 4;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[5] & BIT5) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 5;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[5] & BIT6) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 6;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[5] & BIT7) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 7;if(++by_Count>=3) return(0xAAAAAAAA);}
    
	if(aby_KEY_Buffer[6] & BIT0) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 0;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[6] & BIT1) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 1;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[6] & BIT2) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 2;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[6] & BIT3) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 3;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[6] & BIT4) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 4;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[6] & BIT5) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 5;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[6] & BIT6) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 6;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[6] & BIT7) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 7;if(++by_Count>=3) return(0xAAAAAAAA);}
    
	if(aby_KEY_Buffer[7] & BIT0) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 0;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[7] & BIT1) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 1;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[7] & BIT2) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 2;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[7] & BIT3) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 3;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[7] & BIT4) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 4;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[7] & BIT5) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 5;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[7] & BIT6) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 6;if(++by_Count>=3) return(0xAAAAAAAA);}
	if(aby_KEY_Buffer[7] & BIT7) { Key_Matrix_Array[by_Count][0] = 0;Key_Matrix_Array[by_Count][1] = 7;if(++by_Count>=3) return(0xAAAAAAAA);}
    
    
	ReturnValue = 0x00;
	ReturnValue = ((UINT32)Key_Matrix_Array[0][0])<<24;
	ReturnValue |= ((UINT32)(Key_Matrix_Array[0][1]))<<16;
	ReturnValue |= ((UINT32)(Key_Matrix_Array[1][0]))<<8;
	ReturnValue |= ((UINT32)Key_Matrix_Array[1][1]);
    
	return( ReturnValue );
}

const static unsigned char array_Table[8][8] = 
{
	0x07,0x0F,0x17,0x1F,0x27,0x2F,0x37,0x3F,
	0x06,0x0E,0x16,0x1E,0x26,0x2E,0x36,0x3E,
	0x05,0x0D,0x15,0x1D,0x25,0x2D,0x35,0x3D,
	0x04,0x0C,0x14,0x1C,0x24,0x2C,0x34,0x3C,
	0x03,0x0B,0x13,0x1B,0x23,0x2B,0x33,0x3B,
	0x02,0x0A,0x12,0x1A,0x22,0x2A,0x32,0x3A,
	0x01,0x09,0x11,0x19,0x21,0x29,0x31,0x39,
	0x00,0x08,0x10,0x18,0x20,0x28,0x30,0x38,
};

#define	K_C2_D0	0x05
#define	K_C2_D1	0x0D
#define	K_C2_D2	0x15
#define	K_C2_D3	0x1D
#define	K_C2_D4	0x25
#define	K_C2_D5	0x2D
#define	K_C2_D6	0x35
#define	K_C2_D7	0x3D
#define	K_C3_D0	0x04
#define	K_C3_D1	0x0C
#define	K_C3_D2	0x14
#define	K_C3_D3	0x1C
#define	K_C3_D4	0x24
#define	K_C3_D5	0x2C
#define	K_C3_D6	0x34
#define	K_C3_D7	0x3C
#define	K_C4_D0	0x03
#define	K_C4_D1	0x0B
#define	K_C4_D2	0x13
#define	K_C4_D3	0x1B
#define	K_C4_D4	0x23
#define	K_C4_D5	0x2B
#define	K_C4_D6	0x33
#define	K_C4_D7	0x3B
#define	K_C5_D0	0x02
#define	K_C5_D1	0x0A
#define	K_C5_D2	0x12
#define	K_C5_D3	0x1A
#define	K_C5_D4	0x22
#define	K_C5_D5	0x2A
#define	K_C5_D6	0x32
#define	K_C5_D7	0x3A
#define	K_C6_D0	0x01
#define	K_C6_D1	0x09
#define	K_C6_D2	0x11
#define	K_C6_D3	0x19
#define	K_C6_D4	0x21
#define	K_C6_D5	0x29
#define	K_C6_D6	0x31
#define	K_C6_D7	0x39
#define	K_C7_D0	0x00
#define	K_C7_D1	0x08
#define	K_C7_D2	0x10
#define	K_C7_D3	0x18
#define	K_C7_D4	0x20
#define	K_C7_D5	0x28
#define	K_C7_D6	0x30
#define	K_C7_D7	0x38

void HAL_Set_Stop_position(UINT16 stop_position)
{
	Stop_Position = stop_position;
}
/***************************************************************************//**
* @brief
*    I2C键盘数据处理并返回
*
* @note
*  
*
* @param[in]：none
* @param[out]：UINT32 value
******************************************************************************/
void HAL_Key_I2C_Release(void)
{
	Key_Releaseflag = 1 ;
    #if 1//def __PETER_TEST__
        gKeyCounter = 0;
    #endif
    gLastKeyNumber ++;
	keyi2c = 0;
}
static UINT32 HAL_Key_I2C_Get_Value(void)
{  
    unsigned char i=0,Key_Count = 0;  
	
    //static unsigned char trans_data[10];
    
    static I2C_TransferReturn_TypeDef trans_return_value;
    
    I2C_TransferSeq_TypeDef i2c_data;  
    
 	unsigned char  Key_I2C_Array[2][2];
    
	UINT32 I2C_Returned_Value = 0 ;
    UINT32 I2C_Stop_Value = 0 ;
	UINT8 data[3];


	data[1] = trans_data[1];
	data[2] = trans_data[2];

		
    i2c_data.addr = 0xB0;
    i2c_data.flags = I2C_FLAG_WRITE_READ;
    i2c_data.buf[0].data = &trans_data[0];
    i2c_data.buf[0].len = 1;
    i2c_data.buf[1].len = 1;    
    trans_data[0] = 0x10;   
    
    trans_data[1] = 0x00 ;
    trans_data[2] = 0x00 ;
		
    for(i = 1;i<4;i++)
    {
        i2c_data.buf[1].data = &trans_data[i];  
        trans_return_value = I2C_TransferInit(I2C0,&i2c_data);  
        while (trans_return_value == i2cTransferInProgress)
        {
            trans_return_value = I2C_Transfer(I2C0);
        }
        if(!(trans_data[i] & 0x80))
            break;
    }
    Key_Count = i;
    
    /* key release ,do not car  */
    Key_Releaseflag = 0 ;
    #if 0
    if( (trans_data[1] || trans_data[2]) == 0 )
    {
        return(0XAAAAAAAA);
    }
	
	if((data[1]&0x40) &&( !(trans_data[1]&0x40) ))
	{
		trans_data[1]  =0;
	}
	if((data[2]&0x40)  &&( !(trans_data[2]&0x40) ))
	{
		trans_data[2]  =0;
	}
	#endif
	if((!(trans_data[1] & 0x40))&& !((trans_data[2] & 0x40))) 
    {
		HAL_Key_I2C_Release( );
        return(0XAAAAAAAA);		
    }
    
	char row = (char)(Stop_Position>>8);
	char column = (char)(Stop_Position&0x00FF);
	
	if(Key_Count > 2)
	{
		for(unsigned char j = 0;j<10;j++)
		{
			if((trans_data[j]& 0x3F) == array_Table[row][column])
			{
				I2C_Stop_Value = ((UINT32)(row<<24));
				I2C_Stop_Value |= ((UINT32)(column<<16));
				I2C_Stop_Value |= ((UINT32)(0xAA<<8));
				I2C_Stop_Value |= ((UINT32)(0x00));
				return I2C_Stop_Value;
			}
		}
		Key_Count = 0;
	}
	
	i=0 ;
    Key_I2C_Array[0][0] = 0xAA ;
	Key_I2C_Array[0][1] = 0xAA ;
    Key_I2C_Array[1][0] = 0xAA ;
	Key_I2C_Array[1][1] = 0xAA ;    
    
	while(i<Key_Count )
	{
		switch(trans_data[i+1] & 0x3F)
		{
		    case K_C0_D0 : {Key_I2C_Array[i][0] = 0;Key_I2C_Array[i][1] = 0;break;} 
		    case K_C0_D1 : {Key_I2C_Array[i][0] = 0;Key_I2C_Array[i][1] = 1;break;} 
		    case K_C0_D2 : {Key_I2C_Array[i][0] = 0;Key_I2C_Array[i][1] = 2;break;} 
		    case K_C0_D3 : {Key_I2C_Array[i][0] = 0;Key_I2C_Array[i][1] = 3;break;} 
		    case K_C0_D4 : {Key_I2C_Array[i][0] = 0;Key_I2C_Array[i][1] = 4;break;} 
		    case K_C0_D5 : {Key_I2C_Array[i][0] = 0;Key_I2C_Array[i][1] = 5;break;} 
		    case K_C0_D6 : {Key_I2C_Array[i][0] = 0;Key_I2C_Array[i][1] = 6;break;} 
		    case K_C0_D7 : {Key_I2C_Array[i][0] = 0;Key_I2C_Array[i][1] = 7;break;} 
		    
		    case K_C1_D0 : {Key_I2C_Array[i][0] = 1;Key_I2C_Array[i][1] = 0;break;} 
		    case K_C1_D1 : {Key_I2C_Array[i][0] = 1;Key_I2C_Array[i][1] = 1;break;} 
		    case K_C1_D2 : {Key_I2C_Array[i][0] = 1;Key_I2C_Array[i][1] = 2;break;} 
		    case K_C1_D3 : {Key_I2C_Array[i][0] = 1;Key_I2C_Array[i][1] = 3;break;} 
		    case K_C1_D4 : {Key_I2C_Array[i][0] = 1;Key_I2C_Array[i][1] = 4;break;} 
		    case K_C1_D5 : {Key_I2C_Array[i][0] = 1;Key_I2C_Array[i][1] = 5;break;} 
		    case K_C1_D6 : {Key_I2C_Array[i][0] = 1;Key_I2C_Array[i][1] = 6;break;} 
		    case K_C1_D7 : {Key_I2C_Array[i][0] = 1;Key_I2C_Array[i][1] = 7;break;} 
		    
		    case K_C2_D0 : {Key_I2C_Array[i][0] = 2;Key_I2C_Array[i][1] = 0;break;} 
		    case K_C2_D1 : {Key_I2C_Array[i][0] = 2;Key_I2C_Array[i][1] = 1;break;} 
		    case K_C2_D2 : {Key_I2C_Array[i][0] = 2;Key_I2C_Array[i][1] = 2;break;} 
		    case K_C2_D3 : {Key_I2C_Array[i][0] = 2;Key_I2C_Array[i][1] = 3;break;} 
		    case K_C2_D4 : {Key_I2C_Array[i][0] = 2;Key_I2C_Array[i][1] = 4;break;} 
		    case K_C2_D5 : {Key_I2C_Array[i][0] = 2;Key_I2C_Array[i][1] = 5;break;} 
		    case K_C2_D6 : {Key_I2C_Array[i][0] = 2;Key_I2C_Array[i][1] = 6;break;} 
		    case K_C2_D7 : {Key_I2C_Array[i][0] = 2;Key_I2C_Array[i][1] = 7;break;} 
		    
		    case K_C3_D0 : {Key_I2C_Array[i][0] = 3;Key_I2C_Array[i][1] = 0;break;} 
		    case K_C3_D1 : {Key_I2C_Array[i][0] = 3;Key_I2C_Array[i][1] = 1;break;} 
		    case K_C3_D2 : {Key_I2C_Array[i][0] = 3;Key_I2C_Array[i][1] = 2;break;} 
		    case K_C3_D3 : {Key_I2C_Array[i][0] = 3;Key_I2C_Array[i][1] = 3;break;} 
		    case K_C3_D4 : {Key_I2C_Array[i][0] = 3;Key_I2C_Array[i][1] = 4;break;} 
		    case K_C3_D5 : {Key_I2C_Array[i][0] = 3;Key_I2C_Array[i][1] = 5;break;} 
		    case K_C3_D6 : {Key_I2C_Array[i][0] = 3;Key_I2C_Array[i][1] = 6;break;} 
		    case K_C3_D7 : {Key_I2C_Array[i][0] = 3;Key_I2C_Array[i][1] = 7;break;} 
		    
		    case K_C4_D0 : {Key_I2C_Array[i][0] = 4;Key_I2C_Array[i][1] = 0;break;} 
		    case K_C4_D1 : {Key_I2C_Array[i][0] = 4;Key_I2C_Array[i][1] = 1;break;} 
		    case K_C4_D2 : {Key_I2C_Array[i][0] = 4;Key_I2C_Array[i][1] = 2;break;} 
		    case K_C4_D3 : {Key_I2C_Array[i][0] = 4;Key_I2C_Array[i][1] = 3;break;} 
		    case K_C4_D4 : {Key_I2C_Array[i][0] = 4;Key_I2C_Array[i][1] = 4;break;} 
		    case K_C4_D5 : {Key_I2C_Array[i][0] = 4;Key_I2C_Array[i][1] = 5;break;} 
		    case K_C4_D6 : {Key_I2C_Array[i][0] = 4;Key_I2C_Array[i][1] = 6;break;} 
		    case K_C4_D7 : {Key_I2C_Array[i][0] = 4;Key_I2C_Array[i][1] = 7;break;} 
		    
		    case K_C5_D0 : {Key_I2C_Array[i][0] = 5;Key_I2C_Array[i][1] = 0;break;} 
		    case K_C5_D1 : {Key_I2C_Array[i][0] = 5;Key_I2C_Array[i][1] = 1;break;}  
		    case K_C5_D2 : {Key_I2C_Array[i][0] = 5;Key_I2C_Array[i][1] = 2;break;}  
		    case K_C5_D3 : {Key_I2C_Array[i][0] = 5;Key_I2C_Array[i][1] = 3;break;}  
		    case K_C5_D4 : {Key_I2C_Array[i][0] = 5;Key_I2C_Array[i][1] = 4;break;}  
		    case K_C5_D5 : {Key_I2C_Array[i][0] = 5;Key_I2C_Array[i][1] = 5;break;}  
		    case K_C5_D6 : {Key_I2C_Array[i][0] = 5;Key_I2C_Array[i][1] = 6;break;}  
		    case K_C5_D7 : {Key_I2C_Array[i][0] = 5;Key_I2C_Array[i][1] = 7;break;} 
		    
		    case K_C6_D0 : {Key_I2C_Array[i][0] = 6;Key_I2C_Array[i][1] = 0;break;} 
		    case K_C6_D1 : {Key_I2C_Array[i][0] = 6;Key_I2C_Array[i][1] = 1;break;} 
		    case K_C6_D2 : {Key_I2C_Array[i][0] = 6;Key_I2C_Array[i][1] = 2;break;} 
		    case K_C6_D3 : {Key_I2C_Array[i][0] = 6;Key_I2C_Array[i][1] = 3;break;} 
		    case K_C6_D4 : {Key_I2C_Array[i][0] = 6;Key_I2C_Array[i][1] = 4;break;}  
		    case K_C6_D5 : {Key_I2C_Array[i][0] = 6;Key_I2C_Array[i][1] = 5;break;}  
		    case K_C6_D6 : {Key_I2C_Array[i][0] = 6;Key_I2C_Array[i][1] = 6;break;}  
		    case K_C6_D7 : {Key_I2C_Array[i][0] = 6;Key_I2C_Array[i][1] = 7;break;} 
		    
		    case K_C7_D0 : {Key_I2C_Array[i][0] = 7;Key_I2C_Array[i][1] = 0;break;}  
		    case K_C7_D1 : {Key_I2C_Array[i][0] = 7;Key_I2C_Array[i][1] = 1;break;}  
		    case K_C7_D2 : {Key_I2C_Array[i][0] = 7;Key_I2C_Array[i][1] = 2;break;}  
		    case K_C7_D3 : {Key_I2C_Array[i][0] = 7;Key_I2C_Array[i][1] = 3;break;}  
		    case K_C7_D4 : {Key_I2C_Array[i][0] = 7;Key_I2C_Array[i][1] = 4;break;}  
		    case K_C7_D5 : {Key_I2C_Array[i][0] = 7;Key_I2C_Array[i][1] = 5;break;}  
		    case K_C7_D6 : {Key_I2C_Array[i][0] = 7;Key_I2C_Array[i][1] = 6;break;}  
		    case K_C7_D7 : {Key_I2C_Array[i][0] = 7;Key_I2C_Array[i][1] = 7;break;}  
            
		    default:break;
        }
		i++	;	
	}
    I2C_Returned_Value = 0;
	I2C_Returned_Value = ((UINT32)Key_I2C_Array[0][0])<<24;
	I2C_Returned_Value |= ((UINT32)Key_I2C_Array[0][1])<<16;
	I2C_Returned_Value |= ((UINT32)Key_I2C_Array[1][0])<<8;
	I2C_Returned_Value |= ((UINT32)Key_I2C_Array[1][1]);
    
	return(I2C_Returned_Value );
}

//8.fan level

#define PWM_TIMER	                TIMER2

#define PWM_TIMER_TOP	            26666
#define FAN_TIMER                   TIMER2
#define FAN_CCP                     1
#define FAN_TIMER_ROUTE_CCPEN       TIMER_ROUTE_CC1PEN
#define PWM_TIMER_LOCATION          TIMER_ROUTE_LOCATION_LOC0

/***************************************************************************//**
* @ enum define,for PWM channel 
*   CHANNEL_MOTOR---->cc0--->USE in ALCB
*   CHANNEL_FAN  ---->cc1--->USE in FAN
*   CHANNEL_AMP  ---->cc3--->USE in VOLUME control
******************************************************************************/
typedef enum tag_PWM_CC_CHANNEL{
   CHANNEL_MOTOR= 0,
   CHANNEL_FAN,
   CHANNEL_AMP,
   CHANNEL_ALL
}PWM_CC_CHANNEL;
/***************************************************************************//**
* @brief
*    HAL_PWM_Initial
*
* @note
*    INIT PWM
*    CHANNEL_MOTOR---->cc0--->USE in ALCB
*    CHANNEL_FAN  ---->cc1--->USE in FAN
*    CHANNEL_AMP  ---->cc3--->USE in VOLUME control
* @param[in]：PWM_CC_CHANNEL channel
* @param[out]：none
******************************************************************************/
void HAL_PWM_Initial(PWM_CC_CHANNEL channel )
{
  EFM_ASSERT(channel < CHANNEL_ALL);

  /* Select CC channel parameters */
  TIMER_InitCC_TypeDef timerCCInit =
  {
    .eventCtrl = timerEventEveryEdge,
    .edge = timerEdgeBoth,
    .prsSel = timerPRSSELCh0,
    .cufoa = timerOutputActionNone,
    .cofoa = timerOutputActionNone,
    .cmoa = timerOutputActionToggle,
    .mode = timerCCModePWM,
    .filter = false,
    .prsInput = false,
    .coist = false,
    .outInvert = false,
  };
  switch(channel)
  {
      case CHANNEL_MOTOR:
        TIMER_InitCC(TIMER0, 0, &timerCCInit);
        TIMER_CompareBufSet(TIMER0, 0, 0);
        break;
      case CHANNEL_FAN: 
        TIMER_InitCC(TIMER0, 1, &timerCCInit);
        TIMER_CompareBufSet(TIMER0, 1, 26666);
        break;
      case CHANNEL_AMP:
        TIMER_InitCC(TIMER0, 2, &timerCCInit);
        TIMER_CompareBufSet(TIMER0, 2, 0);
        break;
      default:break;
  }
  TIMER_TopSet(TIMER0, 26667);

  /* Select timer parameters */
  TIMER_Init_TypeDef timerInit =
  {
    .enable = true,
    .debugRun = true,
    .prescale = timerPrescale8,
    .clkSel = timerClkSelHFPerClk,
    .fallAction = timerInputActionNone,
    .riseAction = timerInputActionNone,
    .mode = timerModeUp,
    .dmaClrAct = false,
    .quadModeX4 = false,
    .oneShot = false,
    .sync = false,
  };
  
  TIMER_Init(TIMER0, &timerInit);
  
  TIMER0->ROUTE |=  TIMER_ROUTE_LOCATION_LOC2;
}

/***************************************************************************//**
* @brief
*    HAL_FAN_Initial_IO
*
* @note
*    INIT PWM
*
* @param[in]：none
* @param[out]：none
******************************************************************************/
void HAL_FAN_Initial_IO(void)
{
    HAL_Set_IO_Low(FAN_PORT, FAN_PIN);
    HAL_Set_IO_Output(FAN_PORT, FAN_PIN);
    
    HAL_PWM_Initial(CHANNEL_FAN);

    HAL_FAN_Set_Duty(0);   
}
/***************************************************************************//**
* @brief    
*    HAL_FAN_Set_Level
*
* @note
*  
*
* @param[in]:UINT8 level
* @param[out]：none
******************************************************************************/
void HAL_FAN_Set_Duty(UCHAR DutyValue)
{
    UINT32  TargetPWM = 0;
    
    if( DutyValue > 100 ) 
    {
        TargetPWM =  0;
    }
    else
    {
        TargetPWM = PWM_TIMER_TOP*DutyValue/100;
    }
    
    if( TargetPWM == 0 )
    {
        FAN_TIMER->CC[FAN_CCP].CCVB = 0;
        FAN_TIMER->ROUTE &= ~(FAN_TIMER_ROUTE_CCPEN);
    }
    else
    {
        FAN_TIMER->CC[FAN_CCP].CCVB = (UINT16)(TargetPWM);
        FAN_TIMER->ROUTE |=  (FAN_TIMER_ROUTE_CCPEN);
    }    
}

//9.HR Function
/** INT Edge */
#define INT_FALLING_EDGE	    0
#define INT_RISING_EDGE		    1
#define INT_BOTH_EDGES          2
#define INT_LOW_LEVEL           3
#define INT_HIGH_LEVEL          4

/***************************************************************************//**
* @brief
*   初始化timer1 capture中断，用于HR和polar的检测 
*
* @note
*  
*
* @param[in] none
* @param[out] none 
******************************************************************************/
/***************************************************************************//**
* @brief
*    HAL_HR_Initial_IO
*       
* @note
*   Init IOs & interrupt
*
* @param[in] none
* @param[out]：none 
******************************************************************************/
void HAL_HR_Initial_IO(void)
{
    HAL_Set_IO_Input(HEART_PORT, HEART_PIN);

    GPIO_PinModeSet(HEART_PORT, HEART_PIN, gpioModeInput, 1);
    GPIO_IntConfig(HEART_PORT, HEART_PIN, true, true, true);
    
    GPIO_IntEnable(0x0001 << HEART_PIN);
    NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
    NVIC_EnableIRQ(GPIO_ODD_IRQn);


    
    HAL_Set_IO_Input(POLAR_PORT, POLAR_PIN);
    
#if 0
    HAL_Set_IO_IntMode(HEART_PORT, HEART_PIN, INT_FALLING_EDGE,1);
    HAL_Set_IO_IntMode(POLAR_PORT, POLAR_PIN, INT_FALLING_EDGE,1);
    
    HAL_Enable_IO_Int(HEART_PIN);
    HAL_Enable_IO_Int(POLAR_PIN);
    
    NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
    NVIC_EnableIRQ(GPIO_EVEN_IRQn);
    NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
    NVIC_EnableIRQ(GPIO_ODD_IRQn);
#endif
}

UCHAR Hal_Heart_Get_Heart_Level(void)
{
    return(HAL_Get_IO(HEART_PORT, HEART_PIN));
}

UCHAR Hal_Heart_Get_Polar_Level(void)
{
    return(HAL_Get_IO(POLAR_PORT, POLAR_PIN));
}

///***************************************************************************//**
//* @brief
//*    HAL_HR_Get_HGP
//*
//* @note
//*  
//*
//* @param[in] none
//* @param[out]：pin status 
//******************************************************************************/
//bool HAL_HR_Get_HGP(void)
//{
//    return(HAL_Get_IO(HEART_PORT, HEART_PIN));
//}
//
///***************************************************************************//**
//* @brief
//*    HAL_HR_Get_Polar
//*
//* @note
//*  
//*
//* @param[in] none
//* @param[out]：pin status 
//******************************************************************************/
//bool HAL_HR_Get_Polar(void)
//{
//    return(HAL_Get_IO(POLAR_PORT, POLAR_PIN));
//}
///***************************************************************************//**
//* @brief
//*    HAL_HR_Get_HGP_EXI
//*
//* @note
//*  
//*
//* @param[in] none
//* @param[out]：HGP pin int flag
//******************************************************************************/
//bool HAL_HR_Get_HGP_EXI(void)
//{
//    return(HAL_Get_IO_IntFlag(HEART_PIN));
//}
///***************************************************************************//**
//* @brief
//*    HAL_HR_Get_Polar_EXI
//*
//* @note
//*  
//*
//* @param[in] none
//* @param[out]：polor pin int flag
//******************************************************************************/
//UCHAR HAL_HR_Get_Polar_EXI(void)
//{
//    return(HAL_Get_IO_IntFlag(POLAR_PIN));
//}
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
//void HAL_HRT_Clear_HR_EXI(void)
//{
//    HAL_Clear_IO_IntFlag(HEART_PIN);
//}
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
//void HAL_HRT_Clear_Polar_EXI(void)
//{
//    HAL_Clear_IO_IntFlag(POLAR_PIN);
//}

//10.wifi interface
/***************************************************************************//**
* @brief
*    HAL_WIFI_Initial_IO
*
* @note
*  
*
* @param[in]: none
* @param[out]：none 
******************************************************************************/
void HAL_WIFI_Initial_IO(void)
{
    USART_InitAsync_TypeDef UART_init = USART_INITASYNC_DEFAULT;
    UART_init.baudrate = 115200;
    USART_InitAsync(DIGITAL_WIFI_UART, &UART_init);
    DIGITAL_WIFI_UART->IFC = _UART_IFC_MASK;
    NVIC_ClearPendingIRQ(DIGITAL_WIFI_UART_Rx_IRQ);
    NVIC_ClearPendingIRQ(DIGITAL_WIFI_UART_Tx_IRQ);
    DIGITAL_WIFI_UART->IEN = UART_IEN_RXDATAV;
    GPIO_PinModeSet(WF_UART_TX_PORT, WF_UART_TX_PIN, gpioModePushPull, 1); //tx
    GPIO_PinModeSet(WF_UART_RX_PORT, WF_UART_RX_PIN, gpioModeInput, 1); //rx    
    DIGITAL_WIFI_UART->ROUTE |= DIGITAL_WIFI_UART_ROUTE_EN | DIGITAL_WIFI_UART_ROUTE_LOCATION;    
    //DIGITAL_WIFI_UART->TRIGCTRL

    NVIC_EnableIRQ(DIGITAL_WIFI_UART_Rx_IRQ);
    // HAL_WIFI_Enable_UARTI(0);       

    HAL_Set_IO_Output(WF_RST_PORT, WF_RST_PIN);
    //HAL_Set_IO_Input(WF_RST_PORT, WF_RST_PIN); 

    HAL_Set_IO_Input(WF_INT_PORT,WF_INT_PIN);
    HAL_Set_IO_Input(WF_Ready_PORT,WF_Ready_PIN);
    
}
/***************************************************************************//**
* @brief
*    HAL_WIFI_Set_RST
*
* @note
*  
*
* @param[in] bool isHighLevel
* @param[out]：none 
******************************************************************************/
void HAL_WIFI_Set_RST(bool isHighLevel)
{
    if(isHighLevel)
    {
        HAL_Set_IO_High(WF_RST_PORT, WF_RST_PIN);
    }
    else
    {
        HAL_Set_IO_Low(WF_RST_PORT, WF_RST_PIN);
    }
}
/***************************************************************************//**
* @brief
*   HAL_WIFI_Get_RDY 
*
* @note
*  
*
* @param[in]：none
* @param[out]: WIFI ready pin status
******************************************************************************/
UINT8 HAL_WIFI_Get_RDY(void)
{
    UCHAR ch = HAL_Get_IO(WF_Ready_PORT, WF_Ready_PIN);
    return ch;
}
/***************************************************************************//**
* @brief
*    HAL_WIFI_Get_INT
*
* @note
*  
*
* @param[in]：none
* @param[out]: WIFI RST pin status
******************************************************************************/
bool HAL_WIFI_Get_INT(void)
{
    return HAL_Get_IO(WF_INT_PORT, WF_INT_PIN);
}
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
void HAL_WIFI_SendData(UCHAR Data)
{
    USART_Tx(DIGITAL_WIFI_UART, Data);
}
/***************************************************************************//**
* @brief
*    HAL_WIFI_Enable_UARTI
*
* @note
*  
*
* @param[in]:bool isEnabled)
* @param[out]: none   
******************************************************************************/
void HAL_WIFI_Enable_UARTI(bool isEnabled)
{
   UINT16 TimeOut = 0;
    if(isEnabled == 0)
    {
        while(!(DIGITAL_WIFI_UART->STATUS & UART_STATUS_TXC) && TimeOut < 10000)
        {
#ifdef WDT_ENABLE
            HAL_Clear_WatchDog();
#endif   
            TimeOut++;
        }
        NVIC_ClearPendingIRQ(DIGITAL_WIFI_UART_Tx_IRQ);
        NVIC_DisableIRQ(DIGITAL_WIFI_UART_Tx_IRQ);
        USART_IntClear(DIGITAL_WIFI_UART, UART_IEN_TXC);
        USART_IntDisable(DIGITAL_WIFI_UART, UART_IEN_TXC);
    }
    else
    {
        NVIC_EnableIRQ (DIGITAL_WIFI_UART_Tx_IRQ);
        USART_IntSet   (DIGITAL_WIFI_UART, UART_IFS_TXC);
        USART_IntEnable(DIGITAL_WIFI_UART, UART_IEN_TXC);
    }
}
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
UINT32 HAL_WIFI_Get_RX_Select(void)
{
    return DIGITAL_WIFI_DMA_RX_SELECT;
}
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
void* HAL_WIFI_Get_RX_DataSrc(void)
{
    return (void*)&(DIGITAL_WIFI_UART->RXDATA);
}
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
UINT32 HAL_WIFI_Get_TX_Select(void)
{
    return DIGITAL_WIFI_DMA_TX_SELECT;
}
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
void* HAL_WIFI_Get_TX_DataSrc(void)
{
    return (void*)&(DIGITAL_WIFI_UART->TXDATA);
}

UCHAR HAL_WIFI_GetData(void)
{
    return USART_Rx(UART1);
}
//11.RF function
//
#define RISING_EDGE             0
#define FALLING_EDGE            1

#ifdef DMA_SPI
//definition for DMA_SPI_MASTER
#define DMA_CHANNEL_TX   0
#define DMA_CHANNEL_RX   1
#define DMA_CHANNELS     2

#define DMACTRL_ALIGNMENT   256
#define DMACTRL_CH_CNT      8
#pragma data_alignment=DMACTRL_ALIGNMENT
DMA_DESCRIPTOR_TypeDef dmaControlBlock[DMACTRL_CH_CNT * 2];

/* DMA Callback structure */
DMA_CB_TypeDef spiTranseferCallback;
DMA_CB_TypeDef spiReceiveCallback;

volatile bool rxComplete = 1 ;
volatile bool txComplete = 1 ;
volatile char spiRxDataIndex = 0 ;
volatile int RxBuffLength = 1 ;
#define SPI_TRANSFER_SIZE 34

volatile char spiRxData[SPI_TRANSFER_SIZE];
/***************************************************************************//**
* @brief :HAL_DMA_Set_RXBuffer_Length
*    
*
* @note
*  
*
* @param[in]:rxLength
*   
******************************************************************************/
void HAL_DMA_Set_RXBuffer_Length(int rxLength)
{
    RxBuffLength = rxLength;
}
/***************************************************************************//**
* @brief :callback when HAL_DMA_TransferComplete
*    
*
* @note
*  
*
* @param[in]:
*   
******************************************************************************/
void HAL_DMA_TransferComplete(unsigned int channel, bool primary, void *user)
{
    txComplete = 1;
}
/***************************************************************************//**
* @brief :callback when HAL_DMA_ReceiveComplete
*    
*
* @note
*  
*
* @param[in]:
*   
******************************************************************************/
extern char Receivebuffer[];
void HAL_DMA_ReceiveComplete(unsigned int channel, bool primary, void *user)
{
    rxComplete = 1;
    
    DMA_ActivateBasic(DMA_CHANNEL_RX,
                      true,
                      false,
                      (void *)Receivebuffer,
                      (void *)&(USART2->RXDATA),
                      RxBuffLength-1); 
}
/***************************************************************************//**
* @brief :Get RxComplete or txComplete
*    
*
* @note
*  
*
* @param[in]:
*   
******************************************************************************/
bool HAL_DMA_Get_transferComplete(void)
{
    return(txComplete);
}
bool HAL_DMA_Get_receiveComplete(void)
{
    return(rxComplete);
}
/***************************************************************************//**
* @brief:HAL_DMA_SPI_Master_Setup
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void HAL_DMA_SPI_Master_Setup(void)
{
  /* Initialization structs */
  DMA_Init_TypeDef        dmaInit;
  DMA_CfgChannel_TypeDef  rxChnlCfg;
  DMA_CfgDescr_TypeDef    rxDescrCfg;
  DMA_CfgChannel_TypeDef  txChnlCfg;
  DMA_CfgDescr_TypeDef    txDescrCfg;
  
  /* Initializing the DMA */
  dmaInit.hprot        = 0;
  dmaInit.controlBlock = dmaControlBlock;
  DMA_Init(&dmaInit);
  
  /* Setup call-back function */  
  spiTranseferCallback.cbFunc= HAL_DMA_TransferComplete;
  spiTranseferCallback.userPtr=NULL;
  
  spiReceiveCallback.cbFunc  = HAL_DMA_ReceiveComplete;//transferComplete    ;
  spiReceiveCallback.userPtr = NULL;
  
  /*** Setting up RX DMA ***/

  /* Setting up channel */
  rxChnlCfg.highPri   = false;
  rxChnlCfg.enableInt = true;//true;
  rxChnlCfg.select    = DMAREQ_USART2_RXDATAV;
  rxChnlCfg.cb        = &spiReceiveCallback;
  DMA_CfgChannel(DMA_CHANNEL_RX, &rxChnlCfg);

  /* Setting up channel descriptor */
  rxDescrCfg.dstInc  = dmaDataInc1;
  rxDescrCfg.srcInc  = dmaDataIncNone;
  rxDescrCfg.size    = dmaDataSize1;
  rxDescrCfg.arbRate = dmaArbitrate1;
  rxDescrCfg.hprot   = 0;
  DMA_CfgDescr(DMA_CHANNEL_RX, true, &rxDescrCfg);
  
  /* Activate RX channel */
//  DMA_ActivateBasic(DMA_CHANNEL_RX,
//                      true,
//                      false,
//                      (void *)spiRxData,
//                      (void *)&(USART2->RXDATA),
//                      SPI_TRANSFER_SIZE - 1); 
  
  /*** Setting up TX DMA ***/

  /* Setting up channel */
  txChnlCfg.highPri   = false;
  txChnlCfg.enableInt = true;
  txChnlCfg.select    = DMAREQ_USART2_TXBL;
  txChnlCfg.cb        = &spiTranseferCallback;
  DMA_CfgChannel(DMA_CHANNEL_TX, &txChnlCfg);

  /* Setting up channel descriptor */
  txDescrCfg.dstInc  = dmaDataIncNone;
  txDescrCfg.srcInc  = dmaDataInc1;
  txDescrCfg.size    = dmaDataSize1;
  txDescrCfg.arbRate = dmaArbitrate1;
  txDescrCfg.hprot   = 0;
  DMA_CfgDescr(DMA_CHANNEL_TX, true, &txDescrCfg);
}
/***************************************************************************//**
* @brief:HAL_DMA_SPI_Transfer
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void HAL_DMA_SPI_Transfer(UINT8 *txBuffer,int bytes)
{ 
    /* Setting flag to indicate that TX is in progress
    * will be cleared by call-back function */
    txComplete = 0;
    /* Clear TX regsiters */
    // USART2->CMD = USART_CMD_CLEARTX;
    
    /* Activate TX channel */
    DMA_ActivateBasic(DMA_CHANNEL_TX,
                      true,
                      false,
                      (void *)&(USART2->TXDATA),
                      txBuffer,
                      bytes-1); 
}
/***************************************************************************//**
* @brief:HAL_DMA_Receive
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void HAL_DMA_Receive(UINT8 *rxBuffer,int bytes)
{
 /* Only activate RX DMA if a receive buffer is specified */  
  if (rxBuffer != NULL)
  {
    /* Setting flag to indicate that RX is in progress
     * will be cleared by call-back function */
    rxComplete = 0;
    /* Clear RX regsiters */
   // USART2->CMD = USART_CMD_CLEARRX;
    
    /* Activate RX channel */
    DMA_ActivateBasic(DMA_CHANNEL_RX,
                      true,
                      false,
                      rxBuffer,
                      (void *)&(USART2->RXDATA),
                      bytes - 1); 
  }
}
/***************************************************************************//**
* @brief:spiDmaTransfer ,Rx and Tx
*    
*
* @note
*  
*
* @param[in] reserved
*   
******************************************************************************/
void spiDmaTransfer(uint8_t *txBuffer, uint8_t *rxBuffer,  int bytes)
{ 
  /* Only activate RX DMA if a receive buffer is specified */  
  if (rxBuffer != NULL)
  {
    /* Setting flag to indicate that RX is in progress
     * will be cleared by call-back function */
    txComplete = 0;
    
    /* Clear RX regsiters */
   // USART1->CMD = USART_CMD_CLEARRX;
    
    /* Activate RX channel */
    DMA_ActivateBasic(DMA_CHANNEL_RX,
                      true,
                      false,
                      rxBuffer,
                      (void *)&(USART1->RXDATA),
                      bytes - 1); 
  }
  /* Setting flag to indicate that TX is in progress
   * will be cleared by call-back function */
  rxComplete = true;
  
  /* Clear TX regsiters */
  //USART1->CMD = USART_CMD_CLEARTX;
  
  /* Activate TX channel */
  DMA_ActivateBasic(DMA_CHANNEL_TX,
                    true,
                    false,
                    (void *)&(USART1->TXDATA),
                    txBuffer,
                    bytes - 1); 
}
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
void HAL_RF_Initial_IO(void)
{
#ifdef DMA_SPI   
  USART_InitSync_TypeDef usartInit = USART_INITSYNC_DEFAULT;  
  
  /* Initialize SPI */
  usartInit.databits = usartDatabits8;
  //usartInit.baudrate = 115200;
  usartInit.baudrate = 115200;
  //usartInit.baudrate = 96000;
  usartInit.msbf = true;//senf most bit first
  USART_InitSync(USART2, &usartInit);
  
  /* Turn on automatic Chip Select control */
  USART2->CTRL |= USART_CTRL_AUTOCS;
  
  /* Enable SPI transmit and receive */
  USART_Enable(USART2, usartEnable);
#endif
    //Config GDO0
    HAL_Enable_IO_PullUp(TI_CC_GDO0_PORT,TI_CC_GDO0_PIN);
    HAL_Set_IO_Input(TI_CC_GDO0_PORT,TI_CC_GDO0_PIN);
    
    //Config GDO2
    HAL_Enable_IO_PullUp(TI_CC_GDO2_PORT,TI_CC_GDO2_PIN);
    HAL_Set_IO_Input(TI_CC_GDO2_PORT,TI_CC_GDO2_PIN);
    
    //Config CS
    HAL_Enable_IO_PullUp(TI_CC_CSn_PORT,TI_CC_CSn_PIN);
    HAL_Set_IO_Output(TI_CC_CSn_PORT,TI_CC_CSn_PIN);
    HAL_Set_IO_High(TI_CC_CSn_PORT,TI_CC_CSn_PIN);
    
    //Config SIMO
    HAL_Enable_IO_PullUp(TI_CC_SPI_BITBANG_SIMO_PORT,TI_CC_SPI_BITBANG_SIMO_PIN);
    HAL_Set_IO_Output(TI_CC_SPI_BITBANG_SIMO_PORT,TI_CC_SPI_BITBANG_SIMO_PIN);
    HAL_Set_IO_High(TI_CC_SPI_BITBANG_SIMO_PORT,TI_CC_SPI_BITBANG_SIMO_PIN);
    
    //Config SOMI
    HAL_Enable_IO_PullUp(TI_CC_SPI_BITBANG_SOMI_PORT,TI_CC_SPI_BITBANG_SOMI_PIN);
    HAL_Set_IO_Input(TI_CC_SPI_BITBANG_SOMI_PORT,TI_CC_SPI_BITBANG_SOMI_PIN);
    
    //Config UCLK
    HAL_Enable_IO_PullUp(TI_CC_SPI_BITBANG_UCLK_PORT,TI_CC_SPI_BITBANG_UCLK_PIN);
    HAL_Set_IO_Output(TI_CC_SPI_BITBANG_UCLK_PORT,TI_CC_SPI_BITBANG_UCLK_PIN);
    HAL_Set_IO_High(TI_CC_SPI_BITBANG_UCLK_PORT,TI_CC_SPI_BITBANG_UCLK_PIN);
  #ifdef DMA_SPI  
  /* Enable routing for SPI pins from USART to location 1 */
  USART2->ROUTE = USART_ROUTE_TXPEN | 
                  USART_ROUTE_RXPEN | 
                  USART_ROUTE_CSPEN | 
                  USART_ROUTE_CLKPEN | 
                  USART_ROUTE_LOCATION_LOC0;
#endif
}

/***************************************************************************//**
* @brief:HAL_RF_Get_GDO0_Pin_Input_State
*    Get RF_GDO0 pin state
*   
* @note
*
* @param[in] none
* @param[out]: RF_GDO0 pin state    
******************************************************************************/
bool HAL_RF_Get_GDO0_Pin_Input_State(void)
{
    return (HAL_Get_IO(TI_CC_GDO0_PORT, TI_CC_GDO0_PIN));
}

/***************************************************************************//**
* @brief:HAL_RF_Get_GDO2_Pin_Input_State
*    Get RF_GDO2 pin state
*   
* @note
*
* @param[in] none
* @param[out]: RF_GDO2 pin state    
******************************************************************************/
bool HAL_RF_Get_GDO2_Pin_Input_State(void)
{
    return (HAL_Get_IO(TI_CC_GDO2_PORT, TI_CC_GDO2_PIN));
}

/***************************************************************************//**
* @brief:HAL_RF_Config_GDO0
*    配置RF_GDO0的中断
*    isEnable 决定是否打开中断
* @note
*
* @param[in] isEnable
* @param[out]: RF_GDO2 pin state    
******************************************************************************/
void HAL_RF_Config_GDO0(bool isEnable)
{
    //Config GDO0
    HAL_Set_IO_Input(TI_CC_GDO0_PORT,TI_CC_GDO0_PIN);
    if(isEnable)
    {
      HAL_Clear_IO_IntFlag(TI_CC_GDO0_PIN);
      HAL_Set_IO_IntMode(TI_CC_GDO0_PORT,TI_CC_GDO0_PIN,FALLING_EDGE,1);
      NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
      NVIC_EnableIRQ(GPIO_EVEN_IRQn);
    }
    else
    {
      HAL_Set_IO_IntMode(TI_CC_GDO0_PORT,TI_CC_GDO0_PIN,FALLING_EDGE,0);
    }
}

/***************************************************************************//**
* @brief:HAL_RF_Config_GDO2
*    配置RF_GDO2的中断
*    isEnable 决定是否打开中断
* @note
*
* @param[in] bool isEnable
* @param[out]: none    
******************************************************************************/
void HAL_RF_Config_GDO2(bool isEnable)
{
    //Config GDO2
    HAL_Set_IO_Input(TI_CC_GDO2_PORT,TI_CC_GDO2_PIN);
    if(isEnable)
    {
      HAL_Clear_IO_IntFlag(TI_CC_GDO2_PIN);
      HAL_Set_IO_IntMode(TI_CC_GDO2_PORT,TI_CC_GDO2_PIN,RISING_EDGE,1);
      NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
      NVIC_EnableIRQ(GPIO_ODD_IRQn);
    }
    else
    {
      HAL_Set_IO_IntMode(TI_CC_GDO2_PORT,TI_CC_GDO2_PIN,RISING_EDGE,0);
    }
}

/***************************************************************************//**
* @brief:HAL_RF_Set_SPI_BITBANG_SIMO
*    set SIMO high or low
*
* @note
*
* @param[in] bool isHigh
* @param[out]: none    
******************************************************************************/
void HAL_RF_Set_SPI_BITBANG_SIMO(bool isHigh)
{
    if(isHigh)
    {
        HAL_Set_IO_High(TI_CC_SPI_BITBANG_SIMO_PORT, TI_CC_SPI_BITBANG_SIMO_PIN); 
    }
    else
    {
        HAL_Set_IO_Low(TI_CC_SPI_BITBANG_SIMO_PORT, TI_CC_SPI_BITBANG_SIMO_PIN); 
    }
}

/***************************************************************************//**
* @brief:HAL_RF_Set_SPI_BITBANG_UCLK
*    set CLK high or low
*
* @note
*
* @param[in] bool isHigh
* @param[out]: none    
******************************************************************************/
void HAL_RF_Set_SPI_BITBANG_UCLK(bool isHigh)
{
    if(isHigh)
    {
        HAL_Set_IO_High(TI_CC_SPI_BITBANG_UCLK_PORT, TI_CC_SPI_BITBANG_UCLK_PIN);
    }
    else
    {
        HAL_Set_IO_Low(TI_CC_SPI_BITBANG_UCLK_PORT, TI_CC_SPI_BITBANG_UCLK_PIN);
    }
}

/***************************************************************************//**
* @brief:HAL_RF_Get_SPI_BITBANG_SOMI
*   get MISO pin status
*
* @note
*
* @param[in] none
* @param[out]: MISO pin status    
******************************************************************************/
bool HAL_RF_Get_SPI_BITBANG_SOMI(void)
{
    return (HAL_Get_IO(TI_CC_SPI_BITBANG_SOMI_PORT, TI_CC_SPI_BITBANG_SOMI_PIN));
}

/***************************************************************************//**
* @brief:HAL_RF_Set_SPI_BITBANG_CS
*    set cs high or low
*
* @note
*
* @param[in] bool isHigh
* @param[out]: none    
******************************************************************************/
void HAL_RF_Set_SPI_BITBANG_CS(bool isHigh)
{
    if(isHigh)
    {
        HAL_Set_IO_High(TI_CC_CSn_PORT, TI_CC_CSn_PIN);
    }
    else
    {
        HAL_Set_IO_Low(TI_CC_CSn_PORT, TI_CC_CSn_PIN);
    }
}

/***************************************************************************//**
* @brief:HAL_RF_Delay
*    delay used in RF
*
* @note
*
* @param[in] bool isHigh
* @param[out]: none    
******************************************************************************/
void HAL_RF_Delay(UINT16 cycles)
{
    cycles = cycles * 6;
    while(cycles)
    {
#ifdef WDT_ENABLE
        HAL_Clear_WatchDog();
#endif   
        cycles--;
    }
}

void HAL_SPI_Flash_Initial(void)
{
  GPIO_PinModeSet(CS_PORT,CS_PIN,gpioModePushPull,0);
  GPIO_PinOutSet(CS_PORT,CS_PIN);
  
  GPIO_PinModeSet(CLK_PORT,CLK_PIN,gpioModePushPull,0);
  GPIO_PinOutSet(CLK_PORT,CLK_PIN);
  
  GPIO_PinModeSet(DO_PORT,DO_PIN,gpioModeInputPull,1);
  
  GPIO_PinModeSet(DI_PORT,DI_PIN,gpioModePushPull,0);
  GPIO_PinOutSet(DI_PORT,DI_PIN);
}

void HAL_SPI_Flash_CS(bool isHigh)
{
  if(isHigh)
  {
    GPIO_PinOutSet(CS_PORT, CS_PIN); 
  }
  else
  {
    GPIO_PinOutClear(CS_PORT, CS_PIN); 
  }
}

void HAL_SPI_Flash_CLK(bool isHigh)
{
  if(isHigh)
  {
    GPIO_PinOutSet(CLK_PORT, CLK_PIN);
  }
  else
  {
    GPIO_PinOutClear(CLK_PORT, CLK_PIN);
  }
}
void HAL_SPI_Flash_DI(bool isHigh)
{
  if(isHigh)
  {
    GPIO_PinOutSet(DI_PORT, DI_PIN);
  }
  else
  {
    GPIO_PinOutClear(DI_PORT, DI_PIN);
  }
}

bool HAL_SPI_Flash_DO(void)
{
  return (HAL_Get_IO(DO_PORT, DO_PIN));
}

//actually not used
//unsigned char HAL_RF_Get_SPI_BITBANG_SOMI_BIT(void)
//{
//    return ((0x01 << TI_CC_SPI_BITBANG_SOMI_PIN));
//}
//
//void HAL_RF_Config_GDO0_INT_Mode(bool isEnable)
//{
//    if(isEnable)
//    {
//        HAL_Set_IO_IntMode(TI_CC_GDO0_PORT,TI_CC_GDO0_PIN,1,1);
//    }
//    else
//    {
//        HAL_Set_IO_IntMode(TI_CC_GDO0_PORT,TI_CC_GDO0_PIN,1,0); 
//    }
//}
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
void HAL_RF_Clear_INT_Flag(bool mode)
{
    if(mode)
    {
        HAL_Clear_IO_IntFlag(TI_CC_GDO2_PIN);
    }
    else
    {
        HAL_Clear_IO_IntFlag(TI_CC_GDO0_PIN);
    }
}
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
bool HAL_RF_Get_IO_IntFlag(bool mode)
{
    if(mode)
    {
        return(HAL_Get_IO_IntFlag(TI_CC_GDO2_PIN));
    }
    else
    {
        return(HAL_Get_IO_IntFlag(TI_CC_GDO0_PIN));
    }
}
//12.Safe check 
/***************************************************************************//**
* @brief
*    HAL_SafetyKey_Initial_IO
*
* @note
*  
*
* @param[in]:none
* @param[out]: none
******************************************************************************/
void HAL_SafetyKey_Initial_IO(void)
{
    HAL_Set_IO_Input(SAFETYKEY_CHECK_PORT, SAFETYKEY_CHECK_PIN);
}
/***************************************************************************//**
* @brief
*   HAL_SafetyKey_Get_Attached 
*
* @note
*  
*
* @param[in] none
* @param[out]: safety key pin status  
******************************************************************************/
bool HAL_SafetyKey_Get_Attached(void)
{
    return (HAL_Get_IO(SAFETYKEY_CHECK_PORT, SAFETYKEY_CHECK_PIN));
}

//13.Digital LCB fuction
__no_init volatile static UINT16 w_DelayCount;
/***************************************************************************//**
* @brief
*    HAL_DLCB_Delay
*
* @note
*   此处loop期间需要Clear WDT
*
* @param[in]:UINT16 i
* @param[out]:none
******************************************************************************/
void HAL_DLCB_Delay(UINT16 i) //unit:1ms
{
    w_DelayCount = i;
    while(w_DelayCount > 0)
    {
#ifdef WDT_ENABLE
        while(WDOG->SYNCBUSY & WDOG_SYNCBUSY_CTRL);
        HAL_Clear_WatchDog();
        while(WDOG->SYNCBUSY & WDOG_SYNCBUSY_CMD);
#endif    
    }
}

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
void HAL_DLCB_1ms_Int(void) 
{
    if(w_DelayCount > 0) w_DelayCount--;
}
/***************************************************************************//**
* @brief
*    HAL_DLCB_Initial_IO
*
* @note
*  
*
* @param[in]:none
* @param[out]:none
******************************************************************************/
void HAL_DLCB_Initial_IO(void)
{
    USART_InitAsync_TypeDef UART_init = USART_INITASYNC_DEFAULT;
    UART_init.baudrate = UART_BAUD;
    USART_InitAsync(UART_PORT, &UART_init);
    UART_PORT->IFC = _USART_IFC_MASK;
    NVIC_ClearPendingIRQ(UART_Rx_IRQ);
    NVIC_ClearPendingIRQ(UART_Tx_IRQ);
    NVIC_EnableIRQ(UART_Rx_IRQ);
    UART_PORT->IEN = USART_IEN_RXDATAV;
    /* tx  */
    GPIO_PinModeSet(UART_TX_PORT, UART_TX_PIN, gpioModePushPull, 1);
    
    /*  rx */
    GPIO_PinModeSet(UART_RX_PORT, UART_RX_PIN, gpioModeInput, 1); 
    
    /*  Ctrl */
    GPIO_PinModeSet(UART_485DE_PORT, UART_485DE_PIN, gpioModePushPull, 0); 
    
    UART_PORT->ROUTE |= UART_ROUTE_EN | USART_ROUTE_LOCATION_LOC1;
    
    HAL_DLCB_SetToRx();
    
    w_DelayCount = 0;
}
/***************************************************************************//**
* @brief
*   HAL_DLCB_SendData 
*
* @note
*  
*
* @param[in]:UCHAR Data
* @param[out]:none
******************************************************************************/
void HAL_DLCB_SendData(UCHAR Data)
{
    USART_Tx(UART_PORT, Data);
}
/***************************************************************************//**
* @brief
*   HAL_DLCB_SetToRx 
*
* @note
*  
*
* @param[in]:none
* @param[out]:none 
******************************************************************************/
void HAL_DLCB_SetToRx(void)
{
    UINT16 TimeOut = 0;
    while(!(UART_PORT->STATUS & USART_STATUS_TXC) && TimeOut < 10000)
    {
#ifdef WDT_ENABLE
        HAL_Clear_WatchDog();
#endif   
        TimeOut++;
    }
    NVIC_ClearPendingIRQ(UART_Tx_IRQ);
    NVIC_DisableIRQ(UART_Tx_IRQ);
    USART_IntClear(UART_PORT, USART_IEN_TXC);
    USART_IntDisable(UART_PORT, USART_IEN_TXC);
    HAL_Set_IO_Low(UART_485DE_PORT, UART_485DE_PIN);
}
/***************************************************************************//**
* @brief
*    HAL_DLCB_StartTx
*
* @note
*  
*
* @param[in]:none
* @param[out]:none  
******************************************************************************/
void HAL_DLCB_StartTx(void)
{
    HAL_Set_IO_High(UART_485DE_PORT, UART_485DE_PIN);
    NVIC_EnableIRQ(UART_Tx_IRQ);
    USART_IntEnable(UART_PORT, USART_IEN_TXC);
    USART_IntSet(UART_PORT, USART_IFS_TXC);
}
/***************************************************************************//**
* @brief
*    HAL_DLCB_WakeUp_LCB
*
* @note
*  设置UART_485DE_PIN输出高后，UART_TX_PIN发出500ms低电平，再发出50ms高电平，再
*  进行HAL_DLCB_Initial_IO初始化操作
*
* @param[in]:none
* @param[out]:none  
******************************************************************************/
void HAL_DLCB_WakeUp_LCB(void)
{
	UART_PORT->ROUTE = 0;
		
    HAL_Set_IO_Output(UART_485DE_PORT, UART_485DE_PIN);
    HAL_Set_IO_High(UART_485DE_PORT, UART_485DE_PIN);
	
    HAL_Set_IO_Output(UART_TX_PORT, UART_TX_PIN);
    HAL_Set_IO_Low(UART_TX_PORT, UART_TX_PIN);
    HAL_DLCB_Delay(1000);
    HAL_Set_IO_High(UART_TX_PORT, UART_TX_PIN);
    
    HAL_DLCB_Delay(50);
    HAL_DLCB_Initial_IO();
}

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
bool HAL_DLCB_Get_Rx(void)
{
    NVIC_DisableIRQ(UART_Rx_IRQ);
    HAL_Set_IO_Output(UART_485DE_PORT,UART_485DE_PIN);
    HAL_Set_IO_Low(UART_485DE_PORT,UART_485DE_PIN);
    HAL_Set_IO_Input(UART_RX_PORT,UART_RX_PIN);
    
    return HAL_Get_IO(UART_RX_PORT, UART_RX_PIN);
}
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
UCHAR HAL_DLCB_GetData(void)
{
    return USART_Rx(USART1);
}
/***************************************************************************//**
* @brief
*    HAL_DLCB_Erp
*
* @note
*  
*
* @param[in]:none
* @param[out]:none   
******************************************************************************/
void HAL_DLCB_Erp(void)
{
    NVIC_DisableIRQ(UART_Rx_IRQ);
}

//14.beep function
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
void HAL_Beep_Initial_IO(void)
{
    GPIO_PinModeSet(BEEP_PORT, BEEP_PIN, gpioModePushPull, 0);
    
    LETIMER_CompareSet(BEEP_TIMER, BEEP_TIMER_COMP0, CMU_ClockFreqGet(cmuClock_LETIMER0) / 2000);
    LETIMER_CompareSet(BEEP_TIMER, BEEP_TIMER_COMP1, (CMU_ClockFreqGet(cmuClock_LETIMER0) / 2000) >> 1);
    
    LETIMER_RepeatSet(BEEP_TIMER, BEEP_TIMER_COMP0, 0x01);
    LETIMER_RepeatSet(BEEP_TIMER, BEEP_TIMER_COMP1, 0x01);
    
    BEEP_TIMER->ROUTE = BEEP_TIMER_ROUTE_LOCATION;
    
    /* Set configurations for LETIMER 0 */
    const LETIMER_Init_TypeDef letimerInit =
    {
        .enable         = true,                   /* Start counting when init completed. */
        .debugRun       = false,                  /* Counter shall not keep running during debug halt. */
        .rtcComp0Enable = false,                  /* Don't start counting on RTC COMP0 match. */
        .rtcComp1Enable = false,                  /* Don't start counting on RTC COMP1 match. */
        .comp0Top       = true,                   /* Load COMP0 register into CNT when counter underflows. COMP0 is used as TOP */
        .bufTop         = false,                  /* Don't load COMP1 into COMP0 when REP0 reaches 0. */
        .out0Pol        = 0,                      /* Idle value for output 0. */
        .out1Pol        = 0,                      /* Idle value for output 1. */
        .ufoa0          = letimerUFOAPwm,         /* PWM output on output 0 */
        .ufoa1          = letimerUFOANone,        /* Pulse output on output 1*/
        .repMode        = letimerRepeatFree       /* Count until stopped */
    };
    
    /* Initialize LETIMER */
    LETIMER_Init(BEEP_TIMER, &letimerInit);
}
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
void HAL_Beep_Set_Beep(bool isEnabled)
{
    if(isEnabled)
    {
        BEEP_TIMER->ROUTE |= BEEP_TIMER_ROUTE_EN;
    }
    else
    {
        BEEP_TIMER->ROUTE &= ~BEEP_TIMER_ROUTE_EN;
        HAL_Set_IO_Low(BEEP_PORT, BEEP_PIN);
    }
}
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
UCHAR HAL_Beep_Get_IOLevel(void)
{
    return(HAL_Get_IO(BEEP_PORT, BEEP_PIN));
}

//15.AMP fuction



/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
#define AMP_SOURCE_BLUETOOTH 0
#define AMP_SOURCE_CSSAFE 1
#define AMP_SOURCE_TV_PORT 2
#define AMP_SOURCE_RESERVE 3

#define Volume_PWM_TIMER                PWM_TIMER
#define Volume_PWM_CCP                  2
#define Volume_TIMER_ROUTE_CCPEN        TIMER_ROUTE_CC2PEN
#define Volume_PWM_VALUE		3616

#define Volume_PWM_TIMER                  PWM_TIMER
#define Volume_PWM_CCP                    2
#define Volume_TIMER_ROUTE_CCPEN          TIMER_ROUTE_CC2PEN
#define Volume_PWM_VALUE		  3616

/***************************************************************************//**
* @brief
*    HAL_AMP_Initial_IO
*
* @note
*   1、设置Audio_Source1、2/3、PWM、MUTE等IO为输出，Check_In pin为输入
*   2、设置初始的AMP占空比，由参数传入
*
* @param[in]：UCHAR DutyValue
* @param[out]：none  
******************************************************************************/
void HAL_AMP_Initial_IO(UCHAR DutyValue)
{
    HAL_Set_IO_Output(Audio_Source1_PORT, Audio_Source1_PIN);
    HAL_Set_IO_Output(Audio_Source2_PORT, Audio_Source2_PIN);
    HAL_Set_IO_Output(Audio_Source3_PORT, Audio_Source3_PIN);
    HAL_Set_IO_Output(Volume_PWM_PORT, Volume_PWM_PIN);
    HAL_Set_IO_Output(Volume_MUTE_PORT, Volume_MUTE_PIN);
    HAL_Set_IO_Input(Volume_CHECK_IN_PORT, Volume_CHECK_IN_PIN);
    
    HAL_AMP_Set_Duty(DutyValue);
}
/***************************************************************************//**
* @brief
*   HAL_AMP_Set_Mute 
*   设置mute管脚高低电平
* @note
*
* @param[in]：bool isMuted
* @param[out]：none  
******************************************************************************/
void HAL_AMP_Set_Mute(bool isMuted)
{
    if(isMuted)
    {
        HAL_Set_IO_High(Volume_MUTE_PORT, Volume_MUTE_PIN);
    }
    else
    {
        HAL_Set_IO_Low(Volume_MUTE_PORT, Volume_MUTE_PIN);
    }
}
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
void HAL_AMP_Set_Duty(UCHAR DutyValue)
{
    UINT32  TargetPWM = 0;
    
    if( DutyValue > 100 ) 
    {
        TargetPWM =  0;
    }
    else
    {
        TargetPWM = PWM_TIMER_TOP*100/DutyValue;
    }
    if( TargetPWM == 0 )
    {
        Volume_PWM_TIMER->CC[Volume_PWM_CCP].CCVB = 0;
        Volume_PWM_TIMER->ROUTE &= ~(Volume_TIMER_ROUTE_CCPEN);
    }
    else
    {
        Volume_PWM_TIMER->CC[Volume_PWM_CCP].CCVB = (UINT16)(TargetPWM);
        Volume_PWM_TIMER->ROUTE |=  (Volume_TIMER_ROUTE_CCPEN);
    }  
}
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
void HAL_AMP_Select_Source(AMP_SOURCE_CHANNEL channel)
{
    EFM_ASSERT(channel<AMP_SOURCE_CHANNEL_ALL);
    switch(channel)
    {
        case AMP_SOURCE_CHANNEL1: 
            HAL_Set_IO_Low(Audio_Source2_PORT, Audio_Source2_PIN);
            HAL_Set_IO_Low(Audio_Source3_PORT, Audio_Source3_PIN);
            HAL_Set_IO_High(Audio_Source1_PORT, Audio_Source1_PIN);
            break;
         case AMP_SOURCE_CHANNEL2:
            HAL_Set_IO_Low(Audio_Source1_PORT, Audio_Source1_PIN);
            HAL_Set_IO_Low(Audio_Source3_PORT, Audio_Source3_PIN);
            HAL_Set_IO_High(Audio_Source2_PORT, Audio_Source2_PIN);
            break;
         case AMP_SOURCE_CHANNEL3:
            HAL_Set_IO_Low(Audio_Source1_PORT, Audio_Source1_PIN);
            HAL_Set_IO_Low(Audio_Source2_PORT, Audio_Source2_PIN);
            HAL_Set_IO_High(Audio_Source3_PORT, Audio_Source3_PIN);
            break;
        default:break;                         
    }
}
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
bool HAL_AMP_Check_AUX(void)
{
    return(HAL_Get_IO(Volume_CHECK_IN_PORT, Volume_CHECK_IN_PIN));
}

//16.LCD driver
/***************************************************************************//**
* @brief
*   HAL_16XX_Init_I2C 
*   3?¨o???￥I2C??ê?¨|
* @note
*   
*
* @param[in]?êonone
* @param[out]?êonone
******************************************************************************/
void HAL_16CXX_Init_I2C(void)
{    
    //UINT8 trans_data[25];
    
    /* i2c gpio config  */
    GPIO_PinModeSet(HT16XX_I2C_DATA_PORT, HT16XX_I2C_DATA_PIN, gpioModeWiredAndPullUpFilter, 1);
    GPIO_PinModeSet(HT16XX_I2C_CLK_PORT, HT16XX_I2C_CLK_PIN, gpioModeWiredAndPullUpFilter, 1);
    
    /* enable scl & sda pin function  */
    I2C1->ROUTE = I2C_ROUTE_LOCATION_LOC2 + I2C_ROUTE_SDAPEN + I2C_ROUTE_SCLPEN;  
    
    I2C_Init_TypeDef i2c_para;
    i2c_para.enable = 1;
    i2c_para.master = 1;
    i2c_para.refFreq = 0;
    i2c_para.freq = I2C_FREQ_FAST_MAX;//392500;//I2C_FREQ_STANDARD_MAX;//I2C_FREQ_FAST_MAX;//392500
    i2c_para.clhr = i2cClockHLRAsymetric;//i2cClockHLRStandard;//i2cClockHLRAsymetric;
    
    I2C_Init(I2C1,&i2c_para);
    
//    //for test   
//    HAL_HT16CXX_Select_Chip(4);
//    I2C_TransferSeq_TypeDef i2c_data;  
//    i2c_data.addr = ADDRESS_HT1624;
//    i2c_data.flags = I2C_FLAG_WRITE_WRITE;
//    i2c_data.buf[0].data = &trans_data[0];
//    i2c_data.buf[0].len = 2;
//    i2c_data.buf[1].data = &trans_data[2];
//    i2c_data.buf[1].len = 10;
//    
//    trans_data[0] = 0x80;
//    trans_data[1] = 0x00;
//    
//    trans_data[2] = 0x01;
//    trans_data[3] = 0x02;
//    trans_data[4] = 0x03;
//    trans_data[5] = 0x04;
//    trans_data[6] = 0x85;
//    trans_data[7] = 0x06;
//    trans_data[8] = 0x87;
//    trans_data[9] = 0x08;
//    trans_data[10] = 0x89;
//    trans_data[11] = 0x0a;
//    
//    //wait power supply stability
//    for(int i = 0;i <0x4FFF;i++);  
//
//    trans_return_value = I2C_TransferInit(I2C1,&i2c_data);  
//    
//    while (trans_return_value == i2cTransferInProgress)
//    {
//        trans_return_value = I2C_Transfer(I2C1);
//    }
//    //send 10 data bytes
//    
//    //read config value  used for debug
//    i2c_data.flags = I2C_FLAG_WRITE_READ;
//    i2c_data.buf[1].data = &trans_data[12];  
//    trans_return_value = I2C_TransferInit(I2C1,&i2c_data);   
//    while (trans_return_value == i2cTransferInProgress)
//    {
//        trans_return_value = I2C_Transfer(I2C1);
//    }
//    asm("nop");
}
/***************************************************************************//**
* @brief
*   HAL_16XX_I2C_Write_CMD 
*   ¨??¨?¨2D??chip configuration|ì??¨??
* @note
*   ?á?é¨°a|ì???¤?êo2?¨a?D¨ao?D???|ì?|ì???¤2?¨°??¨′?ê?
*   16C21-->0x70
*   16C22-->
*   16C23-->
*   16C24-->0x7A
*   
* @param[in]?êoUINT8 Cmd,UINT8 Settings
* @param[out]?êonone  
******************************************************************************/
void HAL_16CXX_I2C_Write_CMD(UINT8 Chip_Type,UINT8 Cmd,UINT8 Settings)
{
    UINT8 Cmd_Settings[2];
    
    I2C_TransferSeq_TypeDef i2c_data;  
    i2c_data.addr = Chip_Type;//ADDRESS_HT1624;
    i2c_data.flags = I2C_FLAG_WRITE_WRITE;
    Cmd_Settings[0] = Cmd;
    Cmd_Settings[1] = Settings;
    
    i2c_data.buf[0].data = &Cmd_Settings[0];
    i2c_data.buf[0].len = 1;
    i2c_data.buf[1].data = &Cmd_Settings[1];
    i2c_data.buf[1].len = 1;
    
    trans_return_value = I2C_TransferInit(I2C1,&i2c_data);  
    
    while (trans_return_value == i2cTransferInProgress)
    {
        trans_return_value = I2C_Transfer(I2C1);
    }
}
/***************************************************************************//**
* @brief
*   HAL_16XX_I2C_Write_CMD 
*   ¨??¨?¨2D??chip configuration|ì??¨??
* @note
*   ?á?é¨°a|ì???¤?êo2?¨a?D¨ao?D???|ì?|ì???¤2?¨°??¨′?ê?
*   16C21-->0x70
*   16C22-->
*   16C23-->
*   16C24-->0x7A
*   
* @param[in]?êoUINT8 Cmd,UINT8 Settings
* @param[out]?êonone  
******************************************************************************/
void HAL_16CXX_I2C_Write_ShowData(UINT8 Chip_Type,UINT8 Dis_Cmd,UINT8 Dis_Address,UINT8 *pby_Lcd_Buffer,UINT8 Length)
{
    UINT8 Cmd_Data[2]={Dis_Cmd,Dis_Address};
    
    I2C_TransferSeq_TypeDef i2c_data;  
    i2c_data.addr = Chip_Type;
    i2c_data.flags = I2C_FLAG_WRITE_WRITE;
    i2c_data.buf[0].data = Cmd_Data;
    i2c_data.buf[0].len = 2;
    i2c_data.buf[1].data = pby_Lcd_Buffer; //&(pby_Lcd_Buffer[i*120]);
    i2c_data.buf[1].len = Length;//Disdata.Length[i];
    
    trans_return_value = I2C_TransferInit(I2C1,&i2c_data);  
    while (trans_return_value == i2cTransferInProgress)
    {
        trans_return_value = I2C_Transfer(I2C1);
    }
            //read config value  used for debug
//        i2c_data.flags = I2C_FLAG_WRITE_READ;
//        
//        i2c_data.buf[1].data = &TEST_RECEIVE1[0]; 
//        
//        trans_return_value = I2C_TransferInit(I2C1,&i2c_data);   
//        while (trans_return_value == i2cTransferInProgress)
//        {
//            trans_return_value = I2C_Transfer(I2C1);
//        }
//        asm("nop");
}
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
void HAL_HT16XX_Initial_IO(void)
{
    HAL_Set_IO_Output(HT16XX_DAT0_PORT,HT16XX_DAT0_PIN);
    HAL_Set_IO_Output(HT16XX_DAT1_PORT,HT16XX_DAT1_PIN);
    HAL_Set_IO_Output(HT16XX_DAT3_PORT,HT16XX_DAT3_PIN);
    HAL_Set_IO_Output(HT16XX_CLK_PORT,HT16XX_CLK_PIN);
    HAL_Set_IO_Output(HT16XX_CS1_PORT,HT16XX_CS1_PIN);	
    HAL_Set_IO_Output(HT16XX_CS2_PORT,HT16XX_CS2_PIN);
    HAL_Set_IO_Output(HT16XX_CS3_PORT,HT16XX_CS3_PIN);	
}
/***************************************************************************//**
* @brief
*   设置WR为高电平或者低电平   
*
* @note
*  
* @param[in]：bool isHigh
* @param[out]：none   
******************************************************************************/
void HAL_HT16XX_Set_High_WR(bool isHigh)
{
    if(isHigh)
    {
        HAL_Set_IO_Low(HT16XX_CLK_PORT, HT16XX_CLK_PIN);
    }
    else
    {
        HAL_Set_IO_High(HT16XX_CLK_PORT, HT16XX_CLK_PIN);
    }
}
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
void HAL_HT16XX_Set_High_Data0(bool isHigh)
{
    if(isHigh)
    {
        HAL_Set_IO_Low(HT16XX_DAT0_PORT,HT16XX_DAT0_PIN);
    }
    else
    {
        HAL_Set_IO_High(HT16XX_DAT0_PORT,HT16XX_DAT0_PIN);
    }
}
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
void HAL_HT16XX_Set_High_Data1(bool isHigh)
{
    if(isHigh)
    {
        HAL_Set_IO_Low(HT16XX_DAT1_PORT,HT16XX_DAT1_PIN);
    }
    else
    {
        HAL_Set_IO_High(HT16XX_DAT1_PORT,HT16XX_DAT1_PIN);
    }
}
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
void HAL_HT16XX_Set_High_Data3(bool isHigh)
{
    if(isHigh)
    {
        HAL_Set_IO_Low(HT16XX_DAT3_PORT,HT16XX_DAT3_PIN);
    }
    else
    {
        HAL_Set_IO_High(HT16XX_DAT3_PORT,HT16XX_DAT3_PIN);
    }
}
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
void HAL_HT16XX_Set_High_CS(UCHAR by_IC, bool isHigh)
{
    switch(by_IC)
    {
        case 0:
        {
            if(isHigh != 0)
            {
                HAL_Set_IO_Low(HT16XX_CS1_PORT, HT16XX_CS1_PIN);
            }
            else
            {
                HAL_Set_IO_High(HT16XX_CS1_PORT, HT16XX_CS1_PIN);
            }
            break;
        }
        case 1:
        {
            if(isHigh != 0)
            {
                HAL_Set_IO_Low(HT16XX_CS2_PORT, HT16XX_CS2_PIN);
            }
            else
            {
                HAL_Set_IO_High(HT16XX_CS2_PORT, HT16XX_CS2_PIN);
            }
            break;
        }
        case 2:
        {
            if(isHigh != 0)
            {
                HAL_Set_IO_Low(HT16XX_CS3_PORT, HT16XX_CS3_PIN);
            }
            else
            {
                HAL_Set_IO_High(HT16XX_CS3_PORT, HT16XX_CS3_PIN);
            }
            break;
        }
        default:break;
    }
}

//17.LED driver
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
void HAL_LCD_Backlight_Initial_IO(void)
{
    HAL_Set_IO_Output(LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_PIN);
    HAL_Set_IO_High(LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_PIN);
}

//18.USB 
/** usb */
#define LED_GREEN               0
#define LED_RED                 1
#define LED_ORANGE              2

#define LED_ON                  1
#define LED_OFF                 0
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void HAL_USB_LED_Initial_IO(void)
{
    HAL_Set_IO_Output(USB_LED_RED_PORT,USB_LED_RED_PIN);
    HAL_Set_IO_High(USB_LED_RED_PORT,USB_LED_RED_PIN);
    
    HAL_Set_IO_Output(USB_LED_GREEN_PORT,USB_LED_GREEN_PIN);
    HAL_Set_IO_High(USB_LED_GREEN_PORT,USB_LED_GREEN_PIN);	
    
    HAL_Set_IO_Output(USB_EN_PORT,USB_EN_PIN);
    HAL_Set_IO_High(USB_EN_PORT,USB_EN_PIN);	
    
    HAL_Set_IO_Output(USB_OVER_PORT,USB_OVER_PIN);
    HAL_Set_IO_High(USB_OVER_PORT,USB_OVER_PIN);	
    
    HAL_Set_IO_Output(USB_DM_PORT,USB_DM_PIN);
    HAL_Set_IO_High(USB_DM_PORT,USB_DM_PIN);	
    
    HAL_Set_IO_Output(USB_DP_PORT,USB_DP_PIN);
    HAL_Set_IO_High(USB_DP_PORT,USB_DP_PIN);	
    
    HAL_Set_IO_Output(USB_CRTL_PORT,USB_CRTL_PIN);
    HAL_Set_IO_High(USB_CRTL_PORT,USB_CRTL_PIN);	
    
    HAL_Set_IO_Input(USB_DETECT_PORT, USB_DETECT_PIN);
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void HAL_USB_LED_Control(UCHAR LED,UCHAR ON)
{
    switch(LED)
    {
        case LED_RED:
        {
            if(ON)
            {
                HAL_Set_IO_Low(USB_LED_RED_PORT,USB_LED_RED_PIN);
            }
            else
            {
                HAL_Set_IO_High(USB_LED_RED_PORT,USB_LED_RED_PIN);
            }
            break;
        }
        case LED_GREEN:
        {
            if(ON)
            {
                HAL_Set_IO_Low(USB_LED_GREEN_PORT,USB_LED_GREEN_PIN);
            }
            else
            {
                HAL_Set_IO_High(USB_LED_GREEN_PORT,USB_LED_GREEN_PIN);
            }
            break;
        }
        case LED_ORANGE:
        {
            if(ON)
            {
                HAL_Set_IO_Low(USB_LED_GREEN_PORT,USB_LED_GREEN_PIN);
                HAL_Set_IO_Low(USB_LED_RED_PORT,USB_LED_RED_PIN);
            }
            else
            {
                HAL_Set_IO_High(USB_LED_GREEN_PORT,USB_LED_GREEN_PIN);
                HAL_Set_IO_High(USB_LED_RED_PORT,USB_LED_RED_PIN);
            }
            break;
        }
        default:break;
    }
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void HAL_USB_Set_EN_PORT(bool isHighLevel)
{
    if(isHighLevel != 0)
    {
        HAL_Set_IO_High(USB_EN_PORT, USB_EN_PIN);
    }
    else
    {
        HAL_Set_IO_Low(USB_EN_PORT, USB_EN_PIN);
    }
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void HAL_USB_Set_DM_PORT(bool isHighLevel)
{
    if(isHighLevel != 0)
    {
        HAL_Set_IO_High(USB_DM_PORT, USB_DM_PIN);
    }
    else
    {
        HAL_Set_IO_Low(USB_DM_PORT, USB_DM_PIN);
    }
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void HAL_USB_Set_DP_PORT(bool isHighLevel)
{
    if(isHighLevel != 0)
    {
        HAL_Set_IO_High(USB_DP_PORT, USB_DP_PIN);
    }
    else
    {
        HAL_Set_IO_Low(USB_DP_PORT, USB_DP_PIN);
    }
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void HAL_USB_Set_CTRL_PORT(bool isHighLevel)
{
    if(isHighLevel != 0)
    {
        HAL_Set_IO_High(USB_CRTL_PORT, USB_CRTL_PIN);
    }
    else
    {
        HAL_Set_IO_Low(USB_CRTL_PORT, USB_CRTL_PIN);
    }
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
UCHAR HAL_USB_Get_DETECT_Enable(void)
{
    return(HAL_Get_IO(USB_DETECT_PORT, USB_DETECT_PIN));
}

UCHAR HAL_USB_Get_Over_Enable(void)
{
    return(HAL_Get_IO(USB_OVER_PORT, USB_OVER_PIN));
}

//19.External SPI Memory
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
//void HAL_SPIFlash_Initial_IO(void)
//{
//    HAL_Set_IO_Output(EEPROM_SK_PORT,EEPROM_SK_PIN);
//    HAL_Set_IO_High(EEPROM_SK_PORT,USB_EN_PIN);	
//    
//    HAL_Set_IO_Output(EEPROM_D0_PORT,EEPROM_D0_PIN);
//    HAL_Set_IO_High(EEPROM_D0_PORT,EEPROM_D0_PIN);	
//    
//    HAL_Set_IO_Output(EEPROM_CS_PORT,EEPROM_CS_PIN);
//    HAL_Set_IO_High(EEPROM_CS_PORT,EEPROM_CS_PIN);	
//    
//    HAL_Set_IO_Input(EEPROM_DI_PORT, EEPROM_DI_PIN);
//}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
//void HAL_SPIFlash_Enable(bool isHighLevel)
//{
//    if(isHighLevel != 0)
//    {
//        HAL_Set_IO_High(EEPROM_CS_PORT,EEPROM_CS_PIN);
//    }
//    else
//    {
//        HAL_Set_IO_Low(EEPROM_CS_PORT,EEPROM_CS_PIN);
//    }
//}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
//UINT32 HAL_SPIFlash_Get_LowestAddress(void)
//{
//    
//}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
//UINT32 HAL_SPIFlash_Get_HighestAddress(void)
//{
//    
//}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
//void HAL_SPIFlash_Single_Write(UINT32 addr, BYTE data)
//{
//    
//}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
//BYTE HAL_SPIFlash_Single_Read(UINT32 addr)
//{
//    
//}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
//void HAL_SPIFlash_Burst_Write(UINT32 addr, PCHAR *pSrc, UINT32 len)
//{
//    
//}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
//bool HAL_SPIFlash_Burst_Read(UINT32 addr, PCHAR *pDest, UINT32 len)
//{
//    
//}

//20.internal Flash
/** EFM32LG380F256 */
#define PROGRAMSIZE             1024
#define FLASH_PAGE_SIZE         2048
#define ERROR                   0
#define RIGHT                   1

#define FLAH_LOWEST_ADDRESS     0x38000
#define FLAH_HIGHEST_ADDRESS    0x3FFFF

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
UINT32 HAL_FLASH_Get_LowestAddress(void)
{
    UINT32 LowestAddress=FLAH_LOWEST_ADDRESS;
    return (LowestAddress);
}
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
UINT32 HAL_FLASH_Get_HighestAddress(void)
{
    UINT32 HighestAddress=FLAH_HIGHEST_ADDRESS;
    return (HighestAddress);
}
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
UINT32 HAL_FLASH_Get_Page_Size(void)
{
    UINT32 FlashSize=FLASH_PAGE_SIZE;
    return (FlashSize);
}
/***************************************************************************//**
* @brief
*    HAL_FLASH_Burst_Write
*
* @note
*   1?￠′?è?′yD′è?flash??óòμ?ê×μ??·￡?′yD′è?μ?êy?Yò??°3¤?è￡?D′2ù×÷íê3éoó￡?ê?·?′ò?a
*   ?D??￡?
*   2?￠eg￡oHAL_FLASH_Burst_Write( FLASH_ADDRESS, (PCHAR*)flashBuffer, 2048 ,0);
*
* @param[in] UINT32 addr, PCHAR *pSrc, UINT32 len,bool enableInt
* @param[out] RIGHT or ERROR   
******************************************************************************/
bool  HAL_FLASH_Burst_Write(UINT32 addr, UINT8 *pSrc, UINT16 len, bool enableInt)
{
    UINT32 w_adress = addr;
    bool result = false;
    
    if(w_adress < 0x38000) return result;
    if(len & 0x03 != 0  || len == 0) return result;
    
    HAL_Disable_MasterInterrupt();
#ifdef WDT_ENABLE
    HAL_Clear_WatchDog();
    while(WDOG->SYNCBUSY & WDOG_SYNCBUSY_CMD);
    WDOG_Enable(false);
    while (WDOG->SYNCBUSY & WDOG_SYNCBUSY_CTRL);
#endif 
    
    MSC_Init();
    
    if((MSC_WriteWord((uint32_t *)w_adress, pSrc, len)) != mscReturnOk)
    {
        return ERROR;
    }
 

    MSC_Deinit();
    #ifdef WDT_ENABLE  
    WDOG_Enable(true);
    while (WDOG->SYNCBUSY & WDOG_SYNCBUSY_CTRL);
    HAL_Clear_WatchDog();
#endif
    if(enableInt)
    {
       HAL_Enable_MasterInterrupt();
    }
    return RIGHT;
}

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
bool HAL_FLASH_Burst_Read(UINT32 addr, UINT8 *pDest, UINT16 len)
{
    memcpy(pDest, (UINT8 *)addr, len);
    return (RIGHT);
}
/***************************************************************************//**
* @brief
*    HAL_FLASH_Segment_Write
*   ??óú?3D??μ·±D′è?μ?êy?Y￡?￡??é?úflash?D?a±ùò?????óò￡?ó?à′?-?·D′è?￡???′?D′1ì?¨
*   μ?3¤?è￡???′?D′μ?ê±oòμ??·×??ˉà??ó￡?D′?ú?ù?a??óò?ò??ê￡??μ?μ??·????D?óú?ùD′μ?êy
*   ?Y3¤?èê±￡??éò?2á3y??????óò￡???D??aê?D′è??￡?aD?2ù×÷óéé?2?μ?systemconfigíê3é?￡
*
* @note
*   1?￠ê?è?μ?2?êyóD￡o??2á3yflag￡?D′êy?Yμ???±êflash?eê?μ??·￡?′yD′è?μ?êy?Y?°3¤?è￡?
*   D′íêoóê?·?′ò?a?D??
*
* @param[in] UINT8 EraseFlag,UINT32 addr, PCHAR *pSrc, UINT32 len,bool enableInt
* @param[out] RIGHT or ERROR   
******************************************************************************/
bool HAL_FLASH_Erase_Page(UINT32 addr, bool enableInt)
{
    bool result = false;
    UINT32 w_address = addr;
        
    if(w_address < 0x38000 || w_address & 0xFF != 0) return result;

     HAL_Disable_MasterInterrupt();
     
#ifdef WDT_ENABLE
    HAL_Clear_WatchDog();
    while(WDOG->SYNCBUSY & WDOG_SYNCBUSY_CMD);
    WDOG_Enable(false);
    while (WDOG->SYNCBUSY & WDOG_SYNCBUSY_CTRL);
#endif 
    
    MSC_Init();
    
    /* clean befor write  */
    if(MSC_ErasePage((uint32_t *)w_address) != mscReturnOk)
    {
        result = false;
        return result;
    }
    
    result = true;
    MSC_Deinit();
#ifdef WDT_ENABLE  
    WDOG_Enable(true);
    while (WDOG->SYNCBUSY & WDOG_SYNCBUSY_CTRL);
    HAL_Clear_WatchDog();
#endif 
    if(enableInt)
    {
        HAL_Enable_MasterInterrupt();
    }
    return result;
}

//21.BlueTooth
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
//void HAL_BlueTooth_Initial_IO(void)
//{
//    USART_InitAsync_TypeDef UART_init = USART_INITASYNC_DEFAULT;
//    UART_init.baudrate = 115200;
//    USART_InitAsync(BLUETOOTH_UART, &UART_init);
//    BLUETOOTH_UART->IFC = _USART_IFC_MASK;
//    NVIC_ClearPendingIRQ(BLUETOOTH_UART_Rx_IRQ);
//    NVIC_ClearPendingIRQ(BLUETOOTH_UART_Tx_IRQ);
//    NVIC_EnableIRQ(BLUETOOTH_UART_Rx_IRQ);
//    BLUETOOTH_UART->IEN = USART_IEN_RXDATAV;
//    /* tx  */
//    GPIO_PinModeSet(BlueTooth_TX_PORT,BlueTooth_TX_PIN,gpioModePushPull,1);
//    
//    /* rx  */
//    GPIO_PinModeSet(BlueTooth_RX_PORT,BlueTooth_RX_PIN,gpioModeInput,1);
//    BLUETOOTH_UART->ROUTE |= BLUETOOTH_UART_ROUTE_EN | BLUETOOTH_UART_ROUTE_LOCATION;
//}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
//void HAL_Bluetooth_EnableTx(UCHAR by_Dat)
//{
//    UINT16 TimeOut = 0;
//    
//    if(by_Dat == 0)
//    {
//        while(!(BLUETOOTH_UART->STATUS & USART_STATUS_TXC) && TimeOut < 10000)
//        {
//            TimeOut++;
//        }
//        NVIC_ClearPendingIRQ(BLUETOOTH_UART_Tx_IRQ);
//        NVIC_DisableIRQ(BLUETOOTH_UART_Tx_IRQ);
//        USART_IntClear(BLUETOOTH_UART, USART_IEN_TXC);
//        USART_IntDisable(BLUETOOTH_UART,USART_IEN_TXC);
//    }
//    else
//    {
//        NVIC_EnableIRQ(BLUETOOTH_UART_Tx_IRQ);
//        USART_IntEnable(BLUETOOTH_UART,USART_IEN_TXC);
//        USART_IntSet(BLUETOOTH_UART,USART_IFS_TXC);
//    }
//}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
//void HAL_Bluetooth_SendData(UCHAR by_Dat)
//{
//    USART_Tx(BLUETOOTH_UART, by_Dat);
//}

//22.TV Port
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
//void HAL_TV_PORT_Initial_IO(void)
//{
//    USART_InitAsync_TypeDef UART_init = USART_INITASYNC_DEFAULT;
//    UART_init.baudrate = 115200;
//    USART_InitAsync(TV_PORT_UART, &UART_init);
//    TV_PORT_UART->IFC = _UART_IFC_MASK;
//    NVIC_ClearPendingIRQ(TV_PORT_UART_Rx_IRQ);
//    NVIC_ClearPendingIRQ(TV_PORT_UART_Tx_IRQ);
//    NVIC_EnableIRQ(TV_PORT_UART_Rx_IRQ);
//    TV_PORT_UART->IEN = UART_IEN_RXDATAV;
//    
//    GPIO_PinModeSet(TV_PORT_UART_TX_PORT, TV_PORT_UART_TX_PIN, gpioModePushPull, 1);
//    
//    GPIO_PinModeSet(TV_PORT_UART_RX_PORT, WF_UART_RX_PIN, gpioModeInput, 1);
//    
//    
//    TV_PORT_UART->ROUTE |= TV_PORT_UART_ROUTE_EN | TV_PORT_UART_ROUTE_LOCATION;
//    
//    USART_Enable(TV_PORT_UART, usartEnable);
//    
//    HAL_TV_PORT_UART_Enable_UARTI(0);
//}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
//void HAL_TV_PORT_UART_SendData(UCHAR by_Dat)
//{
//    USART_Tx(TV_PORT_UART, by_Dat);
//}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
//void HAL_TV_PORT_UART_Enable_UARTI(bool isEnabled)
//{
//    UINT16 TimeOut = 0;
//    if(isEnabled == 0)
//    {
//        while(!(TV_PORT_UART->STATUS & UART_STATUS_TXC) && TimeOut < 10000)
//        {
//            TimeOut++;
//        }
//        NVIC_ClearPendingIRQ(TV_PORT_UART_Tx_IRQ);
//        NVIC_DisableIRQ(TV_PORT_UART_Tx_IRQ);
//        USART_IntClear(TV_PORT_UART, UART_IEN_TXC);
//        USART_IntDisable(TV_PORT_UART, UART_IEN_TXC);
//    }
//    else
//    {
//        NVIC_EnableIRQ (TV_PORT_UART_Tx_IRQ);
//        USART_IntSet   (TV_PORT_UART, UART_IFS_TXC);
//        USART_IntEnable(TV_PORT_UART, UART_IEN_TXC);
//    }
//}

//23.c_safe
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
//void HAL_CSafe_Initial_IO(void)
//{
//    USART_InitAsync_TypeDef UART_init = USART_INITASYNC_DEFAULT;
//    UART_init.baudrate = 9600;
//    USART_InitAsync(CSafe_UART, &UART_init);
//    CSafe_UART->IFC = _USART_IFC_MASK;
//    NVIC_ClearPendingIRQ(CSafe_UART_Rx_IRQ);
//    NVIC_ClearPendingIRQ(CSafe_UART_Tx_IRQ);
//    NVIC_EnableIRQ(CSafe_UART_Rx_IRQ);
//    CSafe_UART->IEN = USART_IEN_RXDATAV;
//    GPIO_PinModeSet(CSafe_TX_PORT,CSafe_TX_BIT,gpioModePushPull,1);
//    GPIO_PinModeSet(CSafe_RX_PORT,CSafe_RX_BIT,gpioModeInput,1);
//    GPIO_PinModeSet(CSafe_CTS_PORT,CSafe_CTS_BIT,gpioModePushPull,0);
//    
//    CSafe_UART->ROUTE |= CSafe_UART_ROUTE_EN | USART_ROUTE_LOCATION_LOC2;
//}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
//void HAL_CSafe_Set_CTS(bool isHighLevel)
//{
//    if(isHighLevel)
//    {
//        HAL_Set_IO_High(CSafe_CTS_PORT,CSafe_CTS_BIT);
//    }
//    else
//    {
//        HAL_Set_IO_Low(CSafe_CTS_PORT,CSafe_CTS_BIT);
//    }
//}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
//void HAL_CSafe_UART_Enable_UARTI(bool isEnabled)
//{
//    UINT16 TimeOut = 0;
//    
//    if(isEnabled == 0)
//    {
//        while(!(CSafe_UART->STATUS & USART_STATUS_TXC) && TimeOut < 10000)
//        {
//            TimeOut++;
//        }
//        NVIC_ClearPendingIRQ(CSafe_UART_Tx_IRQ);
//        NVIC_DisableIRQ(CSafe_UART_Tx_IRQ);
//        USART_IntClear(CSafe_UART, USART_IEN_TXC);
//        USART_IntDisable(CSafe_UART,USART_IEN_TXC);
//    }
//    else
//    {
//        NVIC_EnableIRQ(CSafe_UART_Tx_IRQ);
//        USART_IntEnable(CSafe_UART,USART_IEN_TXC);
//        USART_IntSet(CSafe_UART,USART_IFS_TXC);
//    }
//}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
//void HAL_CSafe_UART_SendData(UCHAR by_Dat)
//{
//    USART_Tx(CSafe_UART, by_Dat);
//}

//24.ALCB 
///***************************************************************************//**
//* @brief
//*    
//*
//* @note
//*  
//*
//* @param[in] location
//*   
//******************************************************************************/
//void HAL_ALCB_Initial_IO(void)
//{
//    
//}
///***************************************************************************//**
//* @brief
//*    
//*
//* @note
//*  
//*
//* @param[in] location
//*   
//******************************************************************************/
//void HAL_ALCB_Get_InclineZero(void)
//{
//    
//}
///***************************************************************************//**
//* @brief
//*    
//*
//* @note
//*  
//*
//* @param[in] location
//*   
//******************************************************************************/
//void HAL_ALCB_Set_InclineUp(void)
//{
//    
//}
///***************************************************************************//**
//* @brief
//*    
//*
//* @note
//*  
//*
//* @param[in] location
//*   
//******************************************************************************/
//void HAL_ALCB_Set_InclineDown(void)
//{
//    
//}
///***************************************************************************//**
//* @brief
//*    
//*
//* @note
//*  
//*
//* @param[in] location
//*   
//******************************************************************************/
//void HAL_ALCB_Get_InclineCount(void) //level
//{
//    
//}
///***************************************************************************//**
//* @brief
//*    
//*
//* @note
//*  
//*
//* @param[in] location
//*   
//******************************************************************************/
//void HAL_ALCB_Eanble_Relay(bool isEnabled)
//{
//    
//}
///***************************************************************************//**
//* @brief
//*    
//*
//* @note
//*  
//*
//* @param[in] location
//*   
//******************************************************************************/
//void HAL_ALCB_Set_PWM(UINT16 value)
//{
//    
//}
///***************************************************************************//**
//* @brief
//*    
//*
//* @note
//*  
//*
//* @param[in] location
//*   
//******************************************************************************/
//void HAL_ALCB_Get_RPM(void)  //level
//{
//    
//}
//25.I2C Reserved
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
//void HAL_I2C_Reserved_Initial_IO(void)
//{  
//    /* i2c gpio config  */
//    GPIO_PinModeSet(I2C_SDA_PORT, I2C_SDA_PIN, gpioModeWiredAndPullUpFilter, 1);
//    GPIO_PinModeSet(I2C_SCL_PORT, I2C_SCL_PIN, gpioModeWiredAndPullUpFilter, 1);
//
//    /* enable scl & sda pin function  */
//    I2C0->ROUTE = I2C_ROUTE_LOCATION_LOC2 + I2C_ROUTE_SDAPEN + I2C_ROUTE_SCLPEN;  
//}
///***************************************************************************//**
//* @brief
//*    
//*
//* @note
//*  
//*
//* @param[in] location
//*   
//******************************************************************************/
////UINT32 HAL_I2C_Reserved_Get_Value(void)
////{
////    
////}


//26.ERP 
__no_init uint32_t vectorTable[38+16]@ 0x20000000;

__no_init UINT16 Reset_From_ERP_Flag ;
/***************************************************************************//**
* @brief
*    HAL_Time_Delay
*   简单软件延时
* @note
*
* @param[in]:unsigned long  Delay_Value
* @param[out]:none   
******************************************************************************/
void  HAL_Time_Delay(unsigned long  Delay_Value)
{
    while ( --Delay_Value  !=  0 )
    {
#ifdef WDT_ENABLE
       HAL_Clear_WatchDog();
#endif   
    }
}
/***************************************************************************//**
* @brief
*    HAL_Time_Delay
*   简单软件延时
* @note
*
* @param[in]:unsigned long  Delay_Value
* @param[out]:none   
******************************************************************************/
void  HAL_ERP_Time_Delay(unsigned long  Delay_Value)
{
    while ( --Delay_Value  );
}

/***************************************************************************//**
* @brief
*   HAL_ERP_Set_Scan_Port_High 
*
* @note
*  
*
* @param[in]:none 
* @param[out]:none
******************************************************************************/
static void HAL_ERP_Set_Scan_Port_High(void)
{
    GPIO_PinModeSet(KEYBOARD_SCAN0_PORT, KEYBOARD_SCAN0_PIN, gpioModePushPull, 1);
    GPIO_PinModeSet(KEYBOARD_SCAN1_PORT, KEYBOARD_SCAN1_PIN, gpioModePushPull, 1);
    GPIO_PinModeSet(KEYBOARD_SCAN2_PORT, KEYBOARD_SCAN2_PIN, gpioModePushPull, 1);
    GPIO_PinModeSet(KEYBOARD_SCAN3_PORT, KEYBOARD_SCAN3_PIN, gpioModePushPull, 1);
    GPIO_PinModeSet(KEYBOARD_SCAN4_PORT, KEYBOARD_SCAN4_PIN, gpioModePushPull, 1);
    GPIO_PinModeSet(KEYBOARD_SCAN5_PORT, KEYBOARD_SCAN5_PIN, gpioModePushPull, 1);
    GPIO_PinModeSet(KEYBOARD_SCAN6_PORT, KEYBOARD_SCAN6_PIN, gpioModePushPull, 1);
    GPIO_PinModeSet(KEYBOARD_SCAN7_PORT, KEYBOARD_SCAN7_PIN, gpioModePushPull, 1);
}
/***************************************************************************//**
* @brief
*    HAL_ERP_GPIO_EVEN_Inthandle
*
* @note
*  
*
* @param[in]:unsigned char PinNumber,bool Clear_Int
* @param[out]:ulState & (0x0001 << PinNumber)  
******************************************************************************/
static void HAL_ERP_GPIO_EVEN_Inthandle(void)//偶数中断
{
    unsigned long ulState = GPIO_IntGet();
    
    if(ulState & (0x0001 << SAFETYKEY_CHECK_PIN))
    {
        Reset_From_ERP_Flag = 0x55AA ;
    }
	GPIO_IntClear(ulState);	
}
/***************************************************************************//**
* @brief
*    Eup_GPIO_ODD_Inthandle
*
* @note
*  
*
* @param[in]:unsigned char PinNumber,bool Clear_Int
* @param[out]:ulState & (0x0001 << PinNumber)  
******************************************************************************/
static void HAL_ERP_GPIO_ODD_Inthandle(void)//奇数中断
{
    unsigned long ulState = GPIO_IntGet();

    if(ulState & (0x0001 << I2CKEY_INT_PIN))
	{
        Reset_From_ERP_Flag = 0x55AA ;
    }   
    GPIO_IntClear(ulState);
}
/***************************************************************************//**
* @brief
*   HAL_Eup_LETIMER0_IRQHandler 
*
* @note
*  
*
* @param[in]:none 
* @param[out]:none
******************************************************************************/
#define MAX_LETIMER_ISR	3
__no_init UINT8 LETimer_ISR_Index ;
PF LETimerISR[MAX_LETIMER_ISR];

bool HAL_LETimer_Alloc_ISR(PF pf)
{
	bool ret = 0;
	if(pf == NULL) return ret ;
	for( UINT8 i = 0; i<LETimer_ISR_Index;i++)
	{
		if(pf == LETimerISR[i]) return 1 ;
	}
	if(LETimer_ISR_Index>=MAX_LETIMER_ISR) return ret ;
	
	LETimerISR[LETimer_ISR_Index] = pf ;
	LETimer_ISR_Index++;
	ret = 1;
	return ret;
}

void HAL_ERP_LETIMER0_IRQHandler(void)
{
    LETIMER0->IFC = LETIMER_IFC_COMP0;
	for(UINT8 i = 0; i < LETimer_ISR_Index; i++)
	{
		if(LETimerISR[i] != NULL)
			(*LETimerISR[i])();
	}
}

static void HAL_ERP_SOME_IO_IN(void)
{
	//AMP
	GPIO_PinModeSet(Volume_CHECK_IN_PORT, Volume_CHECK_IN_PIN, gpioModeInput, 0);
    GPIO_PinModeSet(Volume_PWM_PORT, Volume_PWM_PIN, gpioModeInput, 0);
    GPIO_PinModeSet(Volume_MUTE_PORT, Volume_MUTE_PIN, gpioModeInput, 0);
	GPIO_PinModeSet(AUDIO_DAC_PORT, AUDIO_DAC_PIN, gpioModeInput, 0);
	GPIO_PinModeSet(Audio_Source1_PORT, Audio_Source1_PIN, gpioModeInput, 0);
    GPIO_PinModeSet(Audio_Source2_PORT, Audio_Source2_PIN, gpioModeInput, 0);
    GPIO_PinModeSet(Audio_Source3_PORT, Audio_Source3_PIN, gpioModeInput, 0);
	
	//RF
	GPIO_PinModeSet(TI_CC_GDO0_PORT, TI_CC_GDO0_PIN, gpioModeInput, 0);
    GPIO_PinModeSet(TI_CC_GDO2_PORT, TI_CC_GDO2_PIN, gpioModeInput, 0);
    GPIO_PinModeSet(TI_CC_CSn_PORT, TI_CC_CSn_PIN, gpioModeInput, 0);
    GPIO_PinModeSet(TI_CC_SPI_BITBANG_SIMO_PORT, TI_CC_SPI_BITBANG_SIMO_PIN, gpioModeInput, 0);
    GPIO_PinModeSet(TI_CC_SPI_BITBANG_SOMI_PORT, TI_CC_SPI_BITBANG_SOMI_PIN, gpioModeInput, 0);
    GPIO_PinModeSet(TI_CC_SPI_BITBANG_UCLK_PORT, TI_CC_SPI_BITBANG_UCLK_PIN, gpioModeInput, 0);
	
	//hr
	GPIO_PinModeSet(HEART_PORT, HEART_PIN, gpioModeInput, 0);
    GPIO_PinModeSet(POLAR_PORT, POLAR_PIN, gpioModeInput, 0);
	
	//wifi
	GPIO_PinModeSet(WF_RST_PORT, WF_RST_PIN, gpioModeInput, 0);
    GPIO_PinModeSet(WF_INT_PORT, WF_INT_PIN, gpioModeInput, 0);
    GPIO_PinModeSet(WF_Ready_PORT, WF_Ready_PIN, gpioModeInput, 0);
    GPIO_PinModeSet(WF_UART_TX_PORT, WF_UART_TX_PIN, gpioModeInput, 0);
    GPIO_PinModeSet(WF_UART_RX_PORT, WF_UART_RX_PIN, gpioModeInput, 0);
    NVIC_DisableIRQ(DIGITAL_WIFI_UART_Rx_IRQ);
    NVIC_DisableIRQ(DIGITAL_WIFI_UART_Tx_IRQ);
    DIGITAL_WIFI_UART->ROUTE |= ~DIGITAL_WIFI_UART_ROUTE_EN;
	
	//lcb
	GPIO_PinModeSet(UART_RX_PORT, UART_RX_PIN, gpioModeInput, 0);
    GPIO_PinModeSet(UART_TX_PORT, UART_TX_PIN, gpioModeInput, 0);
    GPIO_PinModeSet(UART_485DE_PORT, UART_485DE_PIN, gpioModeInput, 0);
    NVIC_DisableIRQ(UART_Rx_IRQ);
    NVIC_DisableIRQ(UART_Tx_IRQ);
    UART_PORT->ROUTE |= ~UART_ROUTE_EN; 
	
	//key
	GPIO_PinModeSet(KEY_SDA_PORT, KEY_SDA_PIN, gpioModeInput, 0);
	GPIO_PinModeSet(KEY_SCL_PORT, KEY_SCL_PIN, gpioModeInput, 0);
	GPIO_PinModeSet(I2CKEY_INT_PORT, I2CKEY_INT_PIN, gpioModeInput, 0);
	GPIO_PinModeSet(I2CKEY_RST_PORT, I2CKEY_RST_PIN, gpioModeInput, 0);
	
	//usb
	GPIO_PinModeSet(USB_EN_PORT, USB_EN_PIN, gpioModeInput, 0);
	GPIO_PinModeSet(USB_OVER_PORT, USB_OVER_PIN, gpioModeInput, 0);
	GPIO_PinModeSet(USB_LED_RED_PORT, USB_LED_RED_PIN, gpioModeInput, 0);
	GPIO_PinModeSet(USB_LED_GREEN_PORT, USB_LED_GREEN_PIN, gpioModeInput, 0);
	GPIO_PinModeSet(USB_DM_PORT, USB_DM_PIN, gpioModeInput, 0);
	GPIO_PinModeSet(USB_DP_PORT, USB_DP_PIN, gpioModeInput, 0);
	GPIO_PinModeSet(USB_CRTL_PORT, USB_CRTL_PIN, gpioModeInput, 0);
	GPIO_PinModeSet(USB_DETECT_PORT, USB_DETECT_PIN, gpioModeInput, 0);
	
	//DISPLAY
	GPIO_PinModeSet(HT16XX_I2C_DATA_PORT, HT16XX_I2C_DATA_PIN, gpioModeInput, 0);
	GPIO_PinModeSet(HT16XX_I2C_CLK_PORT, HT16XX_I2C_CLK_PIN, gpioModeInput, 0);
	GPIO_PinModeSet(HT16XX_CS1_PORT, HT16XX_CS1_PIN, gpioModeInput, 0);
	GPIO_PinModeSet(HT16XX_CS2_PORT, HT16XX_CS2_PIN, gpioModeInput, 0);
	GPIO_PinModeSet(HT16XX_CS3_PORT, HT16XX_CS3_PIN, gpioModeInput, 0);
	GPIO_PinModeSet(HT16XX_CS4_PORT, HT16XX_CS4_PIN, gpioModeInput, 0);
	GPIO_PinModeSet(HT16XX_CS5_PORT, HT16XX_CS5_PIN, gpioModeInput, 0);
	GPIO_PinModeSet(HT16XX_CS6_PORT, HT16XX_CS6_PIN, gpioModeInput, 0);
	
	//csafe
	GPIO_PinModeSet(CSafe_TX_PORT, CSafe_TX_BIT, gpioModeInput, 0);
    GPIO_PinModeSet(CSafe_RX_PORT, CSafe_RX_BIT, gpioModeInput, 0);
    GPIO_PinModeSet(CSafe_CTS_PORT, CSafe_CTS_BIT, gpioModeInput, 0);
    NVIC_DisableIRQ(CSafe_UART_Rx_IRQ);
    NVIC_DisableIRQ(CSafe_UART_Tx_IRQ);
    CSafe_UART->ROUTE |= ~CSafe_UART_ROUTE_EN; 
	
	//tv_port
	GPIO_PinModeSet(TV_PORT_UART_TX_PORT, TV_PORT_UART_TX_PIN, gpioModeInput, 0);
    GPIO_PinModeSet(TV_PORT_UART_RX_PORT, TV_PORT_UART_RX_PIN, gpioModeInput, 0);
    NVIC_DisableIRQ(TV_PORT_UART_Rx_IRQ);
    NVIC_DisableIRQ(TV_PORT_UART_Tx_IRQ);
    TV_PORT_UART->ROUTE |= ~TV_PORT_UART_ROUTE_EN; 
	
	//
	GPIO_PinModeSet(FAN_PORT, FAN_PIN, gpioModeInput, 0);
	GPIO_PinModeSet(BURN_PORT, BURN_PIN, gpioModeInput, 0);
}
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
void HAL_ERP_Low_POWER_Config(PHYKEYMODE KeyMode,bool iSEnableLETimer)
{  
    EFM_ASSERT(KeyMode<KEYMODE_ALL);
    /* stop USB and USB timer  */
    /*USBH_Stop();*/
    TIMER_Reset(TIMER1);
    TIMER_IntClear(TIMER1, 0xFFFFFFFF);
    TIMER_Enable(TIMER1, false);
    
    /*disable SysTick*/
    SysTick->CTRL = 0;
    
    /* disable interrupt  */
    HAL_Disable_MasterInterrupt();
    
#ifdef WDT_ENABLE
    HAL_Clear_WatchDog();
    while(WDOG->SYNCBUSY & WDOG_SYNCBUSY_CMD);
    HAL_Disable_WatchDog();
#endif
    
    /* disable and clear irq flag*/
    NVIC_DisableIRQ(DIGITAL_WIFI_UART_Rx_IRQ);
    NVIC_DisableIRQ(DIGITAL_WIFI_UART_Tx_IRQ);
    NVIC_DisableIRQ(ADC0_IRQn);
    NVIC_ClearPendingIRQ(DIGITAL_WIFI_UART_Rx_IRQ);
    NVIC_ClearPendingIRQ(DIGITAL_WIFI_UART_Tx_IRQ);
    NVIC_ClearPendingIRQ(ADC0_IRQn);
    NVIC_DisableIRQ(TIMER1_IRQn);
    NVIC_ClearPendingIRQ(TIMER1_IRQn);
    ADC_IntClear(ADC0, _ADC_IF_MASK);
    USART_IntClear(DIGITAL_WIFI_UART, _USART_IF_MASK);
    
    HAL_ERP_Time_Delay(50000);
    
    HAL_ERP_SOME_IO_IN();	
    
    if(KeyMode == KEYMODE_IO )
    {
        
        HAL_ERP_Set_Scan_Port_High(); 
    }
    
    HAL_Config_Clock_Standby();
    
    /* generate Vector  */
    unsigned long *Point = (unsigned long *)SCB->VTOR;
    for(unsigned char i = 0 ; i < 38+16; i++)
    {
        vectorTable[i] = *(Point + i);
    }
    vectorTable[GPIO_EVEN_IRQn+16] = (uint32_t)HAL_ERP_GPIO_EVEN_Inthandle;//HAL_ERP_Get_GPIO_EVEN_Int_PinStatus;
    vectorTable[GPIO_ODD_IRQn+16] = (uint32_t)HAL_ERP_GPIO_ODD_Inthandle;
    vectorTable[LETIMER0_IRQn+16] = (uint32_t)HAL_ERP_LETIMER0_IRQHandler;
    
    GPIO->EXTIRISE = 0x0000;
    GPIO->EXTIFALL = 0x0000;
    GPIO->IEN = 0x0000;
    
    NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
    NVIC_EnableIRQ(GPIO_EVEN_IRQn);
    NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
    NVIC_EnableIRQ(GPIO_ODD_IRQn);
    
    /* Clear all interrupts set. */
    SCB->VTOR = (uint32_t)vectorTable;
    
    
    if(KeyMode == KEYMODE_I2C )
    {
       GPIO_IntConfig(I2CKEY_INT_PORT, I2CKEY_INT_PIN, true, true, true);
    }
    else
    {
        GPIO_IntConfig(KEYBOARD_KeyIn0_PORT, KEYBOARD_KeyIn0_PIN, true, true, true);
        GPIO_IntConfig(KEYBOARD_KeyIn1_PORT, KEYBOARD_KeyIn1_PIN, true, true, true);
        GPIO_IntConfig(KEYBOARD_KeyIn2_PORT, KEYBOARD_KeyIn2_PIN, true, true, true);
        GPIO_IntConfig(KEYBOARD_KeyIn3_PORT, KEYBOARD_KeyIn3_PIN, true, true, true);
        GPIO_IntConfig(KEYBOARD_KeyIn4_PORT, KEYBOARD_KeyIn4_PIN, true, true, true);
        GPIO_IntConfig(KEYBOARD_KeyIn5_PORT, KEYBOARD_KeyIn5_PIN, true, true, true);
        GPIO_IntConfig(KEYBOARD_KeyIn6_PORT, KEYBOARD_KeyIn6_PIN, true, true, true);
        GPIO_IntConfig(KEYBOARD_KeyIn7_PORT, KEYBOARD_KeyIn7_PIN, true, true, true);
    }
   
    GPIO_IntConfig(SAFETYKEY_CHECK_PORT, SAFETYKEY_CHECK_PIN, true, true, true);   
    //GPIO_IntConfig(UART_RX_PORT, UART_RX_PIN, true, true, true);   
    
    if( iSEnableLETimer )
    {
        CMU_ClockEnable(cmuClock_LETIMER0, false);
        CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFRCO);
        CMU_ClockDivSet(cmuClock_LETIMER0, cmuClkDiv_1);
        CMU_ClockEnable(cmuClock_LETIMER0, true);
        /* Set initial compare values for COMP0 */
        LETIMER_CompareSet(LETIMER0, 0, 32768/100);//0.02
        /* Set configurations for LETIMER 0 */
        const LETIMER_Init_TypeDef letimerInit =
        {
            /* Don't start counting when init completed - only with RTC compare match */
            .enable = true,
            /* Counter shall not keep running during debug halt. */
            .debugRun = false,
            /* Start counting on RTC COMP0 match. */
            .rtcComp0Enable = false, 
            /* Don't start counting on RTC COMP1 match. */
            .rtcComp1Enable = false, 
            /* Load COMP0 register into CNT when counter underflows. COMP is used as TOP */
            .comp0Top = true, 
            /* Don't load COMP1 into COMP0 when REP0 reaches 0. */
            .bufTop = false, 
            /* Idle value for output 0. */
            .out0Pol = 0,
            /* Idle value for output 1. */
            .out1Pol = 0, 
            /* Pulse output on output 0 */
            .ufoa0 = letimerUFOANone, 
            /* No output on output 1*/
            .ufoa1 = letimerUFOANone, 
            /*Free mode*/
            .repMode = letimerRepeatFree, 
        };
        /* Initialize LETIMER */
        LETIMER_Init(LETIMER0, &letimerInit);
        LETIMER_Enable(LETIMER0, true);
        LETIMER0->IEN = LETIMER_IEN_COMP0;
        NVIC_EnableIRQ(LETIMER0_IRQn);
    }
    
    //enable watch dog
/*    
#ifdef WDT_ENABLE
    HAL_Enable_WatchDog();
    while(WDOG->SYNCBUSY & WDOG_SYNCBUSY_CTRL);
    HAL_Clear_WatchDog();
    while(WDOG->SYNCBUSY & WDOG_SYNCBUSY_CMD);
#endif    
 */  
    HAL_Enable_MasterInterrupt();
    Reset_From_ERP_Flag = 0 ;
}
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
void HAL_ERP_Enter_Standby_Mode(void)
{
    EMU_EnterEM2(false);
}
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
UINT16 HAL_ERP_Get_Reset_Flag(void)
{
    return(Reset_From_ERP_Flag);
}

#pragma data_alignment=512
static DMA_DESCRIPTOR_TypeDef dmaControlBlock[DMA_CHAN_COUNT * 2];
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
    UINT16 size,bool isSend)
{
    if(isSend)
    {
        while(DMA_ChannelEnabled(channel-1));
    }
    DMA_ActivateBasic(channel-1,
                      true,
                      false,
                      (datadst),
                      (datasrc),
                      (size-1)>size?1:((size-1)==0?1:(size-1)));
    
}

char DmaChannel[DMA_CHAN_COUNT+1]={0};
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
void HAL_DMA_Setup_Dma(void)
{
    DMA_Init_TypeDef         dmaInit;
    
    dmaInit.hprot        = 0;
    dmaInit.controlBlock = dmaControlBlock;
    DMA_Init(&dmaInit);
    //memset(DmaChannel,0,DMA_CHAN_COUNT);
}

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
char HAL_DMA_Malloc_Channel(char channel)
{
    if((channel < DMA_CHAN_COUNT)&&(channel != 0))
    {
        if(DmaChannel[channel] == 1)
        {
            return channel;
        }
    }
    for(int i=1; i< DMA_CHAN_COUNT+1; i++)
    {
        if(DmaChannel[i] == 0)
        {
            DmaChannel[i] = 1;
            return i;
        }
    }
    return DMA_CHAN_COUNT;
}

DMA_CB_TypeDef Func_cb[DMA_CHAN_COUNT]; 
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
void HAL_DMA_Setup_Channel( DMA_PARA cfg,bool isRx )
{
    DMA_CfgChannel_TypeDef   ChnlCfg;
    DMA_CfgDescr_TypeDef     DescrCfg;
    HAL_DMA_Setup_Dma();
    Func_cb[cfg.channel-1].cbFunc  = cfg.callback;
    Func_cb[cfg.channel-1].userPtr = NULL;
    
    ChnlCfg.highPri   = false;
    
    if(Func_cb[cfg.channel-1].cbFunc == NULL )
        ChnlCfg.enableInt = false;
    else
        ChnlCfg.enableInt = true;
    
    ChnlCfg.select    = cfg.dataselect;
    ChnlCfg.cb        = &(Func_cb[cfg.channel-1]);
    DMA_CfgChannel(cfg.channel-1, &ChnlCfg);
    
    //  memset(&DescrCfg,0,sizeof(DMA_CfgDescr_TypeDef));
    //  memcpy(&DescrCfg,&(cfg.DescrCfg));
    
    DescrCfg.dstInc  = cfg.DescrCfg.dstInc;
    DescrCfg.srcInc  = cfg.DescrCfg.srcInc;
    DescrCfg.size    = cfg.DescrCfg.size;
    DescrCfg.arbRate = cfg.DescrCfg.arbRate;
    DescrCfg.hprot   = cfg.DescrCfg.hprot;
    DMA_CfgDescr(cfg.channel-1, true, &DescrCfg);

    if(isRx)
    {
        DMA_ActivateBasic(cfg.channel-1,
                      true,
                      false,
                      cfg.datadst,
                      cfg.datasrc,
                      (cfg.size-1)>cfg.size?1:((cfg.size-1)==0?1:(cfg.size-1))); 
    }

}
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
void HAL_Monitor_Init_Uart(void)
{
    /* Defining the LEUART1 initialization data */
    LEUART_Init_TypeDef leuart1Init =LEUART_INIT_DEFAULT;
    leuart1Init.baudrate = 115200;
    LEUART_Reset(LEUART1);
    LEUART_Init(LEUART1, &leuart1Init);
    
    /* Route LEUART1 TX pin to DMA location 0 */
    LEUART1->ROUTE = DIGITAL_MONITOR_UART_ROUTE_EN |
        DIGITAL_MONITOR_UART_ROUTE_LOCATION;
    
    /* Enable GPIO for LEUART1. TX is on C6 */
    GPIO_PinModeSet(MONITOR_LEUART_TX_PORT,
                    MONITOR_LEUART_TX_PIN,
                    gpioModePushPull,
                    1);     
}
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
void HAL_Monitor_Send_Char(UINT8 Data)
{
    LEUART_Tx(DIGITAL_MONITOR_UART,Data);
}
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
void HAL_Monitor_Send_String(UINT8 *string,UINT8 length)
{
    for(UCHAR i=0;i<length;i++)
        LEUART_Tx(DIGITAL_MONITOR_UART,string[i]);
}
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
UINT8 HAL_Get_Vision_Code(UINT8 *buffer)
{
    UINT8 string[8] = HAL_VISION_CODE;
    memcpy(buffer,string,sizeof(string));
    return(strlen(HAL_VISION_CODE));
}


/** interrupt list */

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
PF pfISR[ALL_ISRS];
void HAL_WiFi_Regist_ISR(PF txFunc,PF rxFunc)
{
	pfISR[UART1_TX_ISR] = txFunc ;
	pfISR[UART1_RX_ISR] = rxFunc ;
}

void HAL_Digital_Regist_ISR(PF txFunc,PF rxFunc)
{
	pfISR[UART_TX_ISR] = txFunc ;
	pfISR[UART_RX_ISR] = rxFunc ;	
}



#define MAX_SYSTICKER_ISR	20
__no_init UINT8 SysTicker_ISR_Index ;
PF SysTickerISR[MAX_SYSTICKER_ISR];

bool HAL_SystemTicker_Alloc_ISR(PF pf)
{
	bool ret = 0;
	if(pf == NULL) return ret ;
	for( UINT8 i = 0; i<SysTicker_ISR_Index;i++)
	{
		if(pf == SysTickerISR[i]) return 1 ;
	}
	if(SysTicker_ISR_Index>=MAX_SYSTICKER_ISR) return ret ;
	
	SysTickerISR[SysTicker_ISR_Index] = pf ;
	SysTicker_ISR_Index++;
	ret = 1;
	return ret;
}

extern UINT32  TestTimer;

void SysTick_Handler(void)
{
	TestTimer++;
	if(keycheckcnt<100)
	{
		keycheckcnt++;
	}
  #ifdef __PETER_TEST__
   if( gKeyCounter<200)
    {
        gKeyCounter++;
        if(gKeyCounter == 200)
        {
            gCurrentKeyNumber = 0;            
            gValidKeyNumber = 0;
        }
    }
   #endif
	for(UINT8 i = 0; i < SysTicker_ISR_Index; i++)
	{
		if(SysTickerISR[i] != NULL)
			(*SysTickerISR[i])();
	}
}
/***************************************************************************//**
* @brief
*    HAL_INT_Regist_Pin_INT
*
* @note
*   IO中断注册程序 ，调用者不需要区分奇偶中断
*
* @param[in]：PIN_INT_IDX Int_Index,PF pf
* @param[out]：none 
******************************************************************************/

#define MAX_PININT_ISR	10
__no_init UINT8 PinInt_ISR_Index ;
PF PININTISR[MAX_PININT_ISR];

UINT8 HAL_PININT_Alloc_ISR(PF pf)
{
	UINT ret = 0;
	if(pf == NULL) return ret ;
	for( UINT8 i = 0; i<PinInt_ISR_Index;i++)
	{
		if(pf == PININTISR[i]) return 1 ;
	}
	if(PinInt_ISR_Index>=MAX_PININT_ISR) return ret ;
	PININTISR[PinInt_ISR_Index] = pf ;
	PinInt_ISR_Index++;
	ret = 1;
	return ret;
}

void HAL_ISR_Pin_INT(void)
{
	for(UINT8 i = 0; i < PinInt_ISR_Index; i++)
	{
		if(PININTISR[i]!= NULL)
			(*PININTISR[i])();
	}
}

void HAL_Init_ISR(void)
{
	SysTicker_ISR_Index = 0;
	PinInt_ISR_Index = 0 ;
	LETimer_ISR_Index = 0 ;
	memset(SysTickerISR,0,sizeof(SysTickerISR));
	memset(PININTISR,0,sizeof(PININTISR));
	memset(LETimerISR,0,sizeof(LETimerISR));
}



/***************************************************************************//**
* @brief
*    GPIO_ODD_IRQHandler
*
* @note
*   ODD中断服务子程序
*
* @param[in]：none
* @param[out]：none 
******************************************************************************/
void GPIO_ODD_IRQHandler(void)
{ 
    HAL_ISR_Pin_INT();
}
/***************************************************************************//**
* @brief
*    GPIO_ODD_IRQHandler
*
* @note
*  EVEN中断服务子程序
*
* @param[in]：none
* @param[out]：none
******************************************************************************/
void GPIO_EVEN_IRQHandler(void)
{
    HAL_ISR_Pin_INT();
}
/***************************************************************************//**
* @brief
*   USART1_RX_IRQHandler 
*
* @note
*  
*
* @param[in]：none
* @param[out]：none
******************************************************************************/
void USART1_RX_IRQHandler(void)
{
	unsigned long state;

	state = USART_IntGet(USART1);

	/* Reset all pending interupts */
	USART_IntClear(USART1, ~_UART_IFC_RESETVALUE); 

	if (state & UART_IF_RXDATAV)
	{
		if(pfISR[UART_RX_ISR] != NULL)
			(*pfISR[UART_RX_ISR])();
	}
}
/***************************************************************************//**
* @brief
*   USART1_TX_IRQHandler 
*
* @note
*  
* @param[in]：none
* @param[out]：none
******************************************************************************/
void USART1_TX_IRQHandler(void)
{
	unsigned long state;

	state = USART_IntGet(USART1);

	USART_IntClear(USART1, ~_UART_IFC_RESETVALUE);/* Reset all pending interupts */

	if (state & UART_IF_TXC)
	{
		if(pfISR[UART_TX_ISR] != NULL)
			(*pfISR[UART_TX_ISR])();
	}
}

/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void UART1_RX_IRQHandler(void)
{
	unsigned long state;
	state = USART_IntGet(UART1);
	/* Reset all pending interupts */
	USART_IntClear(UART1, ~_UART_IFC_RESETVALUE);

	if (state & UART_IF_RXDATAV)
	{
		if(pfISR[UART1_RX_ISR] != NULL)
			pfISR[UART1_RX_ISR]();
	}
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void UART1_TX_IRQHandler(void)
{
	unsigned long state;
	state = USART_IntGet(UART1);
	USART_IntClear(UART1, ~_UART_IFC_RESETVALUE);

	if (state & UART_IF_TXC)
	{
		if(pfISR[UART1_TX_ISR] != NULL)
			pfISR[UART1_TX_ISR]();
	}
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void HardFault_Handler(void)
{
    EFM_ASSERT(0);
    HAL_Reset_System();
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void MemManage_Handler(void)
{
    EFM_ASSERT(0);
    HAL_Reset_System();
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void BusFault_Handler(void)
{
    EFM_ASSERT(0);
    HAL_Reset_System();
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void UsageFault_Handler(void)
{
    EFM_ASSERT(0);
    HAL_Reset_System();
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void SVC_Handler(void)
{
    EFM_ASSERT(0);
    HAL_Reset_System();
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void DebugMon_Handler(void)
{
    EFM_ASSERT(0);
    HAL_Reset_System();
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void PendSV_Handler(void)
{
    EFM_ASSERT(0);
    HAL_Reset_System();
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
//void TIMER0_IRQHandler(void)
//{
//    EFM_ASSERT(0);
//    HAL_Reset_System();
//}
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
//TIMER1_CC_CHL HAL_Get_Timer1_CCChanel(void)
//{
//    UINT16 Return_CHL=0;
//    if(TIMER1->IF & TIMER_IF_CC1)
//    {
//        Return_CHL = TIMER1_CC_CH_HR;
//    }
//    else if(TIMER1->IF & TIMER_IF_CC2)
//    {
//        Return_CHL = TIMER1_CC_CH_POLAR;
//    }
//    else
//    {
//        Return_CHL = DEFAULT_CH;
//    }
//    return(Return_CHL);
//}

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
//void HAL_Clear_Timer1_CC_Flag(TIMER1_CC_CHL chl)
//{
//    if( chl==TIMER1_CC_CH_HR )
//    {
//        TIMER1->IFC = TIMER_IFC_CC1;
//    }
//    else if( chl==TIMER1_CC_CH_POLAR )
//    {
//        TIMER1->IFC = TIMER_IFC_CC2;
//    }
//    else
//    {
//        TIMER1->IFC = TIMER1->IF;
//    }
//}

/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void ACMP0_IRQHandler(void)
{
    EFM_ASSERT(0);
    HAL_Reset_System();
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void ADC0_IRQHandler(void)
{
    EFM_ASSERT(0);
    HAL_Reset_System();
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void DAC0_IRQHandler(void)
{
    EFM_ASSERT(0);
    HAL_Reset_System();
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void I2C0_IRQHandler(void)
{
    EFM_ASSERT(0);
    HAL_Reset_System();
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void TIMER2_IRQHandler(void)
{
    EFM_ASSERT(0);
    HAL_Reset_System();
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void USART2_RX_IRQHandler(void)
{
    EFM_ASSERT(0);
    HAL_Reset_System();
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void USART2_TX_IRQHandler(void)
{
    EFM_ASSERT(0);
    HAL_Reset_System();
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void UART0_RX_IRQHandler(void)
{
    EFM_ASSERT(0);
    HAL_Reset_System();
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void UART0_TX_IRQHandler(void)
{
    EFM_ASSERT(0);
    HAL_Reset_System();
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void LEUART0_IRQHandler(void)
{
    EFM_ASSERT(0);
    HAL_Reset_System();
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void LEUART1_IRQHandler(void)
{
    EFM_ASSERT(0);
    HAL_Reset_System();
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void LETIMER0_IRQHandler(void)
{
    EFM_ASSERT(0);
    HAL_Reset_System();
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void PCNT0_IRQHandler(void)
{
    EFM_ASSERT(0);
    HAL_Reset_System();
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void PCNT1_IRQHandler(void)
{
    EFM_ASSERT(0);
    HAL_Reset_System();
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void PCNT2_IRQHandler(void)
{
    EFM_ASSERT(0);
    HAL_Reset_System();
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void RTC_IRQHandler(void)
{
    EFM_ASSERT(0);
    HAL_Reset_System();
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void CMU_IRQHandler(void)
{
    EFM_ASSERT(0);
    HAL_Reset_System();
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void VCMP_IRQHandler(void)
{
    EFM_ASSERT(0);
    HAL_Reset_System();
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void LCD_IRQHandler(void)
{
    EFM_ASSERT(0);
    HAL_Reset_System();
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void MSC_IRQHandler(void)
{
    EFM_ASSERT(0);
    HAL_Reset_System();
}
/***************************************************************************//**
* @brief
*    
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void AES_IRQHandler(void)
{
    EFM_ASSERT(0);
    HAL_Reset_System();
}

#endif

//end of HAL.c

#else 

#error Please defined a BSP platform

#endif
