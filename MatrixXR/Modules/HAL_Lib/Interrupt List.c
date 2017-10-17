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

extern PF pfISR[ALL_ISRS];
extern PF pfMsISR[ALL_IDXS];
extern PF pfODDINTISR[ALL_ODDIDXS];
extern PF pfEVENINTISR[ALL_EVENIDXS];

/***************************************************************************//**
* @brief
*    SysTick_Handler
*
* @note
*   1ms中断ISR
*
* @param[in]：none
* @param[out]：none 
******************************************************************************/
void SysTick_Handler(void)
{
    for(UINT8 i = 0; i < ALL_IDXS; i++)
    {
        if(pfMsISR[i] != NULL)
            (*pfMsISR[i])();
    }
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
    for(UINT8 i = 0; i < ALL_ODDIDXS; i++)
    {
        if(pfODDINTISR[i] != NULL)
            (*pfODDINTISR[i])();
    }
    GPIO_IntClear(0xAAAA);
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
    for(UINT8 i = 0; i < ALL_EVENIDXS; i++)
    {
        if(pfEVENINTISR[i] != NULL)
            (*pfEVENINTISR[i])();
    }
    GPIO_IntClear(0x5555);
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
        (*pfISR[UART_TX_ISR])();
    }
}
/***************************************************************************//**
* @brief
*    USART0_RX_IRQHandler
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void USART0_RX_IRQHandler(void)
{
#ifdef __WIFI_FUNC__
    unsigned long state;
    state = USART_IntGet(USART0);
    /* Reset all pending interupts */
    USART_IntClear(USART0, ~_USART_IFC_RESETVALUE);
    
    if (state & USART_IF_RXDATAV)
    {
        Digital_Recv_Int_wifi(USART_Rx(USART0));
    }
#else
    HAL_Reset_System();
#endif
}
/***************************************************************************//**
* @brief
*    USART0_TX_IRQHandler
*
* @note
*  
*
* @param[in] location
*   
******************************************************************************/
void USART0_TX_IRQHandler(void)//WIFI
{
#ifdef __WIFI_FUNC__
    unsigned long state;
    state = USART_IntGet(USART0);
    /* Reset all pending interupts */
    USART_IntClear(USART0, ~_USART_IFC_RESETVALUE);
    
    if (state & USART_IF_TXC)
    {
        //Digital_Send_FIFO_wifi_Workout();
        Digital_Send_FIFO_wifi();
    }
#else
    HAL_Reset_System();
#endif
    
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
void TIMER0_IRQHandler(void)
{
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
void ACMP0_IRQHandler(void)
{
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
    HAL_Reset_System();
}
#endif

//end of HAL.c

#else 

#error Please defined a BSP platform

#endif
