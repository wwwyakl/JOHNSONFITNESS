/***************************************************************************//**
 * @file usbconfig.h
 * @brief USB protocol stack library, application supplied configuration options.
 * @author Energy Micro AS
 * @version 1.2.2
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2012 Energy Micro AS, http://www.energymicro.com</b>
 *******************************************************************************
 *
 * This source code is the property of Energy Micro AS. The source and compiled
 * code may only be used on Energy Micro "EFM32" microcontrollers.
 *
 * This copyright notice may not be removed from the source code nor changed.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Energy Micro AS has no
 * obligation to support this Software. Energy Micro AS is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Energy Micro AS will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/
#ifndef __USBCONFIG_H
#define __USBCONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#define USB_HOST            /* Compile stack for host mode. */

/****************************************************************************
**                                                                         **
** Specify number of host channels used (in addition to EP0).              **
**                                                                         **
*****************************************************************************/
#define NUM_HC_USED 2       /* Not counting default control ep which  */
                            /* is assigned to host channels 0 and 1   */


/****************************************************************************
**                                                                         **
** Specify Over current port and pin and polarity                          **
**                                                                         **
*****************************************************************************/
#define USB_VBUSOVRCUR_PORT       gpioPortA
#define USB_VBUSOVRCUR_PIN        14
#define USB_VBUSOVRCUR_POLARITY   USB_VBUSOVRCUR_POLARITY_LOW


#ifndef NDEBUG
/* Debug USB API functions (illegal input parameters etc.)  */
//#define DEBUG_USB_API       /* Uncomment to turn on  */

/*
 * Some utility functions in the API needs printf. These
 * functions have "print" in their name. This macro enables
 * these functions.
 */
//#define USB_USE_PRINTF      /* Uncomment to enable   */
#endif
  
#ifdef __cplusplus
}
#endif

#endif /* __USBCONFIG_H */
