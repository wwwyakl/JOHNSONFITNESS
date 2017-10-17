
#ifndef _HAL_IO_DEFINE_H_
#define _HAL_IO_DEFINE_H_


#include "efm32.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "em_letimer.h"
#include "em_leuart.h"
#include "em_adc.h"
#include "em_wdog.h"
#include "em_i2c.h"
#include "jisdef.h"

#define DMA_SPI
#undef DMA_SPI
//---------------------------Heart&Polar Port--------------------//
#define HEART_PORT			        gpioPortB
#define HEART_PIN			        1
#define POLAR_PORT			        gpioPortB
#define POLAR_PIN			        2
//---------------------------Power_MGR Port--------------------//
#define ERP_LED_PORT		        gpioPortE
#define ERP_LED_PIN		            7
#define Power_Button_LED_PORT		gpioPortB
#define Power_Button_LED_PIN		12
#define POWER_EN1_PORT		        gpioPortD
#define POWER_EN1_PIN		        5
#define POWER_EN2_PORT		        gpioPortD
#define POWER_EN2_PIN		        9
#define Power_Fail_PORT		        gpioPortA
#define Power_Fail_PIN		        15

//---------------------------Fan Port----------------------//
#define FAN_PORT		            gpioPortA
#define FAN_PIN		                9

//---------------------------BURN Port----------------------//
#define BURN_PORT			        gpioPortB
#define BURN_PIN			        3
//---------------------------RTC Port--------------------//
#define HT1381_CLK_PORT			    gpioPortE
#define HT1381_CLK_PIN			    1

#define HT1381_DAT_PORT			    gpioPortE
#define HT1381_DAT_PIN			    0

#define HT1381_RST_PORT			    gpioPortE
#define HT1381_RST_PIN			    6
//---------------------------I2C_KEY Port--------------------//
#define KEY_SDA_PORT		        gpioPortA
#define KEY_SDA_PIN			        0

#define KEY_SCL_PORT		        gpioPortA
#define KEY_SCL_PIN			        1

#define I2CKEY_INT_PORT		        gpioPortF
#define I2CKEY_INT_PIN		        9

#define I2CKEY_RST_PORT		        gpioPortE
#define I2CKEY_RST_PIN		        2

//reserved
#define KEYBOARD_SCAN0_PORT		gpioPortC 
#define KEYBOARD_SCAN0_PIN		11
#define KEYBOARD_SCAN1_PORT		gpioPortC
#define KEYBOARD_SCAN1_PIN		11
#define KEYBOARD_SCAN2_PORT		gpioPortC
#define KEYBOARD_SCAN2_PIN		11
#define KEYBOARD_SCAN3_PORT		gpioPortC
#define KEYBOARD_SCAN3_PIN		11
#define KEYBOARD_SCAN4_PORT		gpioPortC
#define KEYBOARD_SCAN4_PIN		11
#define KEYBOARD_SCAN5_PORT		gpioPortC
#define KEYBOARD_SCAN5_PIN		11
#define KEYBOARD_SCAN6_PORT		gpioPortC
#define KEYBOARD_SCAN6_PIN		11
#define KEYBOARD_SCAN7_PORT		gpioPortC
#define KEYBOARD_SCAN7_PIN		11

#define KEYBOARD_KeyIn0_PORT	        gpioPortC
#define KEYBOARD_KeyIn0_PIN		11
#define KEYBOARD_KeyIn1_PORT	        gpioPortC
#define KEYBOARD_KeyIn1_PIN		11
#define KEYBOARD_KeyIn2_PORT	        gpioPortC
#define KEYBOARD_KeyIn2_PIN		11
#define KEYBOARD_KeyIn3_PORT	        gpioPortC
#define KEYBOARD_KeyIn3_PIN		11
#define KEYBOARD_KeyIn4_PORT	        gpioPortC
#define KEYBOARD_KeyIn4_PIN		11
#define KEYBOARD_KeyIn5_PORT	        gpioPortC
#define KEYBOARD_KeyIn5_PIN		11
#define KEYBOARD_KeyIn6_PORT	        gpioPortC
#define KEYBOARD_KeyIn6_PIN		11
#define KEYBOARD_KeyIn7_PORT	        gpioPortC
#define KEYBOARD_KeyIn7_PIN		11
//-------------------------Digital LCB----------------------
#define UART_TX_PORT                    gpioPortD
#define UART_TX_PIN                     0

#define UART_RX_PORT                    gpioPortD
#define UART_RX_PIN                     1

#define UART_485DE_PORT                 gpioPortD
#define UART_485DE_PIN                  2

#define UART_BAUD                       9600
#define UART_PORT                       USART1
#define UART_ROUTE_EN                   USART_ROUTE_TXPEN | USART_ROUTE_RXPEN
#define DIGITAL_UART_ROUTE_LOCATION     USART_ROUTE_LOCATION_LOC1

#define UART_Rx_IRQ                     USART1_RX_IRQn
#define UART_Tx_IRQ                     USART1_TX_IRQn
#define DIGITAL_UART_STATUS             USART1->STATUS
#define DIGITAL_UART_UARTTXC            USART_STATUS_TXC
///---------------------------WiFi Module Port--------------------//
#define WF_UART_TX_PORT                 gpioPortB
#define WF_UART_TX_PIN                  9

#define WF_UART_RX_PORT                 gpioPortB
#define WF_UART_RX_PIN                  10

#define WF_Ready_PORT			gpioPortA
#define WF_Ready_PIN		        12

#define WF_INT_PORT                gpioPortA//link?
#define WF_INT_PIN                 13

#define WF_RST_PORT			gpioPortA
#define WF_RST_PIN		        14

#define DIGITAL_WIFI_UART                   UART1
#define DIGITAL_WIFI_UART_ROUTE_EN          UART_ROUTE_TXPEN | UART_ROUTE_RXPEN
#define DIGITAL_WIFI_UART_ROUTE_LOCATION    UART_ROUTE_LOCATION_LOC2

#define DIGITAL_WIFI_DMA_TX_SELECT  DMAREQ_UART1_TXBL
#define DIGITAL_WIFI_DMA_RX_SELECT  DMAREQ_UART1_RXDATAV


#define DIGITAL_WIFI_UART_Rx_IRQ            UART1_RX_IRQn
#define DIGITAL_WIFI_UART_Tx_IRQ            UART1_TX_IRQn
#define DIGITAL_WIFI_UART_STATUS            UART1->STATUS
#define DIGITAL_WIFI_UART_UARTTXC           UART_STATUS_TXC
//---------------------------RF Port----------------------//
#define TI_CC_GDO0_PORT                 gpioPortA
#define TI_CC_GDO0_PIN                  6

#define TI_CC_GDO2_PORT                 gpioPortA
#define TI_CC_GDO2_PIN                  5

#define TI_CC_CSn_PORT                  gpioPortC
#define TI_CC_CSn_PIN                   5

#define TI_CC_SPI_BITBANG_SIMO_PORT     gpioPortC
#define TI_CC_SPI_BITBANG_SIMO_PIN      2

#define TI_CC_SPI_BITBANG_SOMI_PORT     gpioPortC
#define TI_CC_SPI_BITBANG_SOMI_PIN      3

#define TI_CC_SPI_BITBANG_UCLK_PORT     gpioPortC
#define TI_CC_SPI_BITBANG_UCLK_PIN      4

#define CS_PORT gpioPortA
#define CS_PIN   11
#define CLK_PORT gpioPortC
#define CLK_PIN   4
#define DI_PORT gpioPortC
#define DI_PIN   2
#define DO_PORT gpioPortC
#define DO_PIN   3
//---------------------------Safe key Port----------------------//
#define SAFETYKEY_CHECK_PORT		gpioPortC
#define SAFETYKEY_CHECK_PIN		8
//---------------------------Beeper Port--------------------//
#define BEEP_PORT			gpioPortD
#define BEEP_PIN			6

#define BEEP_TIMER                      LETIMER0
#define BEEP_TIMER_COMP0                0
#define BEEP_TIMER_COMP1                1

#define BEEP_TIMER_ROUTE_EN             LETIMER_ROUTE_OUT0PEN
#define BEEP_TIMER_ROUTE_LOCATION       LETIMER_ROUTE_LOCATION_LOC0
//---------------------------Audio Port--------------------//
#define Volume_PWM_PORT		            gpioPortA
#define Volume_PWM_PIN		            10
#define AUDIO_DAC_PORT                  gpioPortB
#define AUDIO_DAC_PIN                   11
#define Volume_MUTE_PORT                gpioPortE
#define Volume_MUTE_PIN                 9
#define Volume_CHECK_IN_PORT            gpioPortE
#define Volume_CHECK_IN_PIN             8
#define Audio_Source1_PORT              gpioPortD
#define Audio_Source1_PIN               10
#define Audio_Source2_PORT              gpioPortD
#define Audio_Source2_PIN               7
#define Audio_Source3_PORT              gpioPortD
#define Audio_Source3_PIN               8


//---------------------------LCD_Driver Port--------------------//
#define HT16XX_DAT0_PORT		gpioPortE
#define HT16XX_DAT0_PIN			0

#define HT16XX_DAT1_PORT		gpioPortF
#define HT16XX_DAT1_PIN			12

#define HT16XX_DAT3_PORT		gpioPortB
#define HT16XX_DAT3_PIN			12

#define HT16XX_CLK_PORT		        gpioPortE
#define HT16XX_CLK_PIN			1

//#define HT16XX_CS1_PORT			gpioPortE
//#define HT16XX_CS1_PIN			3
//
//#define HT16XX_CS2_PORT			gpioPortE
//#define HT16XX_CS2_PIN			4
//
//#define HT16XX_CS3_PORT			gpioPortE
//#define HT16XX_CS3_PIN			5

#define HT16XX_I2C_DATA_PORT    gpioPortE
#define HT16XX_I2C_DATA_PIN     0

#define HT16XX_I2C_CLK_PORT     gpioPortE
#define HT16XX_I2C_CLK_PIN      1

#define HT16XX_CS1_PORT			gpioPortF
#define HT16XX_CS1_PIN			12

#define HT16XX_CS2_PORT			gpioPortE
#define HT16XX_CS2_PIN			3

#define HT16XX_CS3_PORT			gpioPortE
#define HT16XX_CS3_PIN			4

#define HT16XX_CS4_PORT			gpioPortE
#define HT16XX_CS4_PIN			5

#define HT16XX_CS5_PORT			gpioPortE
#define HT16XX_CS5_PIN			6

#define HT16XX_CS6_PORT			gpioPortA
#define HT16XX_CS6_PIN			11
//---------------------------LED_Driver Port--------------------//
#define  LCD_BACKLIGHT_PORT             gpioPortA
#define  LCD_BACKLIGHT_PIN               2
//---------------------------USB_Port---------------------------//
#define USB_EN_PORT		        gpioPortF
#define USB_EN_PIN			5
#define USB_OVER_PORT		        gpioPortF
#define USB_OVER_PIN			6

#define USB_LED_RED_PORT	        gpioPortF
#define USB_LED_RED_PIN		        7

#define USB_LED_GREEN_PORT	        gpioPortF
#define USB_LED_GREEN_PIN	        8

#define USB_DM_PORT		        gpioPortF
#define USB_DM_PIN		        10

#define USB_DP_PORT		        gpioPortF
#define USB_DP_PIN		        11

//#define USBVBUS_PORT		        gpioPortF
//#define USBVBUS_PIN		        8

#define USB_CRTL_PORT		        gpioPortD
#define USB_CRTL_PIN		        11

#define USB_DETECT_PORT		        gpioPortD
#define USB_DETECT_PIN		        12
//------------------------External SPI Memory-------------------//
#define EEPROM_SK_PORT		        gpioPortC
#define EEPROM_SK_PIN		        0
#define EEPROM_DI_PORT		        gpioPortC
#define EEPROM_DI_PIN		        2
#define EEPROM_D0_PORT		        gpioPortC
#define EEPROM_D0_PIN		        3
#define EEPROM_CS_PORT		        gpioPortE
#define EEPROM_CS_PIN		        5
//------------------------BlueTooth-------------------//
#define BlueTooth_TX_PORT               gpioPortA
#define BlueTooth_TX_PIN                5
#define BlueTooth_RX_PORT               gpioPortA
#define BlueTooth_RX_PIN                6
#define BlueTooth_RST_PORT              gpioPortB
#define BlueTooth_RST_PIN               4
#define BlueTooth_WAKEUP_PORT           gpioPortB
#define BlueTooth_WAKEUP_PIN            5
#define BlueTooth_MODE1_PORT            gpioPortB
#define BlueTooth_MODE1_PIN             6
#define BlueTooth_MODE2_PORT            gpioPortB
#define BlueTooth_MODE2_PIN             7
#define BlueTooth_MODE3_PORT            gpioPortB
#define BlueTooth_MODE3_PIN             8

#define BLUETOOTH_UART                  USART2
#define BLUETOOTH_UART_ROUTE_EN	        USART_ROUTE_TXPEN | USART_ROUTE_RXPEN
#define BLUETOOTH_UART_ROUTE_LOCATION   USART_ROUTE_LOCATION_LOC0

#define BLUETOOTH_UART_Rx_IRQ           USART2_RX_IRQn
#define BLUETOOTH_UART_Tx_IRQ           USART2_TX_IRQn
#define BLUETOOTH_UART_STATUS           USART2->STATUS
#define BLUETOOTH_UART_UARTTXC          USART_STATUS_TXC
//------------------------TV PORT-------------------//
#define TV_PORT_UART_TX_PORT            gpioPortA
#define TV_PORT_UART_TX_PIN             3
#define TV_PORT_UART_RX_PORT            gpioPortA
#define TV_PORT_UART_RX_PIN             4

#define TV_PORT_UART                    UART0
#define TV_PORT_UART_ROUTE_EN           UART_ROUTE_TXPEN | UART_ROUTE_RXPEN
#define TV_PORT_UART_ROUTE_LOCATION     UART_ROUTE_LOCATION_LOC2

#define TV_PORT_UART_Rx_IRQ             UART0_RX_IRQn
#define TV_PORT_UART_Tx_IRQ             UART0_TX_IRQn
#define TV_PORT_UART_UART_STATUS        UART0->STATUS
#define TV_PORT_UART_UARTTXC            UART_STATUS_TXC
//------------------------TV PORT-------------------//

//------------------------I2C PORT------------------//
#define I2C_SDA_PORT		        gpioPortC
#define I2C_SDA_PIN			6

#define I2C_SCL_PORT		        gpioPortC
#define I2C_SCL_PIN			7
//------------------------I2C PORT------------------//

//----------------------C_Safe----------------------//
#define CSafe_TX_PORT                 gpioPortC
#define CSafe_TX_BIT                  9

#define CSafe_RX_PORT                 gpioPortC
#define CSafe_RX_BIT                  10

#define CSafe_CTS_PORT                gpioPortC
#define CSafe_CTS_BIT                 11

#define CSafe_UART                 USART0
#define CSafe_UART_ROUTE_EN        USART_ROUTE_TXPEN | USART_ROUTE_RXPEN
#define CSafe_UART_ROUTE_LOCATION  USART_ROUTE_LOCATION_LOC2//US0 #2

#define CSafe_UART_Rx_IRQ          USART0_RX_IRQn
#define CSafe_UART_Tx_IRQ          USART0_TX_IRQn
#define CSafe_UART_STATUS          USART0->STATUS
#define CSafe_UART_UARTTXC         USART_STATUS_TXC
//----------------------C_Safe----------------------//

//---------------------------ERP Port----------------------//
#define ERP_POWER_OutPut()         GPIO_PinModeSet(POWER_EN1_PORT, POWER_EN1_PIN, gpioModePushPull,0);
#define ERP_POWER_on()             GPIO_PinOutSet(POWER_EN1_PORT, POWER_EN1_PIN);
#define ERP_POWER_off()            GPIO_PinOutClear(POWER_EN1_PORT, POWER_EN1_PIN);

#define EUP_LED_RED_PORT		   gpioPortD
#define EUP_LED_RED_PIN			   9
#define ERP_LED_OutPut() 	       GPIO_PinModeSet(EUP_LED_RED_PORT, EUP_LED_RED_PIN, gpioModePushPull,0);
#define ERP_LED_ON() 	           GPIO_PinOutSet(EUP_LED_RED_PORT, EUP_LED_RED_PIN);
#define ERP_LED_OFF() 	           GPIO_PinOutClear(EUP_LED_RED_PORT, EUP_LED_RED_PIN);
//---------------------------ERP Port----------------------//

#define MONITOR_LEUART_TX_PORT      gpioPortC
#define MONITOR_LEUART_TX_PIN       6
#define MONITOR_LEUART_RX_PORT      gpioPortC
#define MONITOR_LEUART_RX_PIN       7

#define DIGITAL_MONITOR_UART                   LEUART1
#define DIGITAL_MONITOR_UART_ROUTE_EN          LEUART_ROUTE_TXPEN | LEUART_ROUTE_RXPEN
#define DIGITAL_MONITOR_UART_ROUTE_LOCATION    LEUART_ROUTE_LOCATION_LOC0
//-----------------------MONITOR UART Port------//
#endif