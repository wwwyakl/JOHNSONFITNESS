/**************************************************************************//**
 * @file
 * @brief EFM32GG_DK3750, Touch panel driver
 * @author Energy Micro AS
 * @version 2.0.1
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2012 Energy Micro AS, http://www.energymicro.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 * 4. The source and compiled code may only be used on Energy Micro "EFM32"
 *    microcontrollers and "EFR4" radios.
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
 *****************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "efm32.h"
#include "em_adc.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "touchcfg.h"
#include "touch.h"
#include "calibrate.h"
#include "dvk.h"
#ifndef TOUCH_WITHOUT_STORE
#include "i2cdrv.h"
#include "eeprom.h"
#endif

#define CALIBRATION_MAGIC_NUMBER 0xCA71B4A7
#define CALIBRATION_EEPROM_OFFSET 0x80

/** touch driver states */
typedef enum
{ TOUCH_INIT,
  TOUCH_CHECK_PRESS,
  TOUCH_MEASURE_X,
  TOUCH_MEASURE_Y
} TOUCH_State_TypeDef;

static volatile TOUCH_State_TypeDef touch_state = TOUCH_INIT;
static ADC_InitSingle_TypeDef sInit = ADC_INITSINGLE_DEFAULT;
volatile TOUCH_Pos_TypeDef newpos;
volatile TOUCH_Pos_TypeDef current_pos;
static TOUCH_Pos_TypeDef pos;

static   void (*upcall)(TOUCH_Pos_TypeDef *) = 0;

uint32_t touch_ignore_move;

/* Calibration table */
MATRIX calibrationMatrix = TOUCH_INIT_CALIBRATION;

#ifndef TOUCH_WITHOUT_STORE
/* calculate simple checksum over POINT table */
static uint32_t touch_CountChecksum(uint32_t magic, uint32_t *data, uint32_t len)
{ unsigned long checksum = magic; 

  while(len--)
  { if(checksum&0x80000000)
    { checksum<<=1;
      checksum |= 1;
    } else checksum<<=1;
    checksum += *data;
    data++;
  }
  return(checksum);
}

/* function loads calibration table from EEPROM, validate it and if OK uses it */
static void touch_LoadCalibration(void)
{ I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT;
  uint32_t temp, checksum;
  int count;
  MATRIX new_matrix;
  
  /* Initialize I2C driver, using standard rate. Devices on DK itself */
  /* supports fast mode, but in case some slower devices are added on */
  /* prototype board, we use standard mode. */
  I2CDRV_Init(&i2cInit);
 
  
  count =  EEPROM_Read(I2C0, EEPROM_DVK_ADDR, CALIBRATION_EEPROM_OFFSET, (uint8_t*)&temp, sizeof(temp));
  count += EEPROM_Read(I2C0, EEPROM_DVK_ADDR, CALIBRATION_EEPROM_OFFSET+4, (uint8_t*)&new_matrix, sizeof(new_matrix));
  if( count == sizeof(new_matrix)+4)
  { if( temp == CALIBRATION_MAGIC_NUMBER )
    { checksum = touch_CountChecksum(temp, (uint32_t*)&new_matrix, sizeof(new_matrix)/4);
      count = EEPROM_Read(I2C0, EEPROM_DVK_ADDR, CALIBRATION_EEPROM_OFFSET+4+sizeof(new_matrix), (uint8_t*)&temp, sizeof(temp));
      if(temp==checksum)
      { /* looks like calibration table is valid */
        ADC_IntDisable(ADC0, ADC_IF_SINGLE); /* we need to disable ADC interrupt to avoid current_pos structure update for a while */
        memcpy(&calibrationMatrix, &new_matrix, sizeof(calibrationMatrix));
        ADC_IntEnable(ADC0, ADC_IF_SINGLE);
      }
    }
  } 
}

/* function stores calibration table together with magic number and checksum */
static void touch_StoreCalibration(void)
{ int count;
  uint32_t temp = CALIBRATION_MAGIC_NUMBER, checksum;
  
  checksum = touch_CountChecksum(CALIBRATION_MAGIC_NUMBER, (uint32_t*)&calibrationMatrix, sizeof(calibrationMatrix)/4);
  count = EEPROM_Write(I2C0, EEPROM_DVK_ADDR, CALIBRATION_EEPROM_OFFSET, (uint8_t*)&temp, sizeof(temp));
  count += EEPROM_Write(I2C0, EEPROM_DVK_ADDR, CALIBRATION_EEPROM_OFFSET+4, (uint8_t*)&calibrationMatrix, sizeof(calibrationMatrix));
  count += EEPROM_Write(I2C0, EEPROM_DVK_ADDR, CALIBRATION_EEPROM_OFFSET+4+sizeof(calibrationMatrix), (uint8_t*)&checksum, sizeof(checksum));
}
#endif

/***************************************************************************//**
 * @brief Convert ADC readings into XY position 
 * @param pointer to position structure
 ******************************************************************************/
void TOUCH_RecalculatePosition(volatile TOUCH_Pos_TypeDef *pos)
{ POINT old_pos, new_pos;

  if(pos->pen)
  {
    old_pos.x = pos->adcx;
    old_pos.y = pos->adcy;
    if(getDisplayPoint(&new_pos, &old_pos,&calibrationMatrix)==OK)
    { if(new_pos.x>=0) pos->x = new_pos.x;
      else pos->x = 0;
      if(new_pos.y>=0) pos->y = new_pos.y;
      else pos->y = 0;
    }
  } else
  {
    pos->x = 0;
    pos->y = 0;
  }
}

/***************************************************************************//**
 * @brief function that calls registered upcall
 ******************************************************************************/
void TOUCH_CallUpcall(void)
{
  if(upcall)
    upcall((TOUCH_Pos_TypeDef*)&current_pos);
}

/***************************************************************************//**
 * @brief Check if cursor state changed (down or move)
 ******************************************************************************/
int TOUCH_StateChanged(void)
{ int result = 0;
  int diff, a, b;
  
  if(newpos.pen && !current_pos.pen) result=1; /* pen down */
  a = current_pos.x;
  b = newpos.x;
  diff = a - b;
  if(abs(diff) > touch_ignore_move) result = 1; /* move in X axis */
  a = current_pos.y;
  b = newpos.y;
  diff = a - b;
  if(abs(diff) > touch_ignore_move) result = 1; /* move in Y axis */
  return result;
}

/***************************************************************************//**
 * @brief interrupt handler is executed with frequency ~28Hz when panel is not pressed
 * and with frequency ~140Hz when panel is pressed - this will give ~50 readings per second
 ******************************************************************************/
void ADC0_IRQHandler(void)
{ 
  switch(touch_state)
  {
    case TOUCH_INIT: /* enter this state if touch panel is not pressed */
                     touch_state = TOUCH_CHECK_PRESS;
                     GPIO_PinModeSet(TOUCH_Y1, gpioModePushPull, 1);
                     GPIO_PinModeSet(TOUCH_Y2, gpioModePushPull, 1);
                     GPIO_PinModeSet(TOUCH_X1, gpioModePushPull, 0);
                     GPIO_PinModeSet(TOUCH_X2, gpioModeInput, 0);
                     sInit.input = ADC_Y;
                     sInit.reference = adcRefVDD;
                     sInit.resolution = adcResOVS;
                     sInit.acqTime = adcAcqTime128; /* used to slow down */
                     ADC_InitSingle(ADC0, &sInit);
                     break;
    case TOUCH_CHECK_PRESS: /* periodically checks if touch panel is pressed */
                     if( ADC_DataSingleGet(ADC0) > TOUCH_PRESS_LIMIT ) 
                     {
                       touch_state = TOUCH_MEASURE_Y;
                       GPIO_PinModeSet(TOUCH_X1, gpioModePushPull, 1);
                       GPIO_PinModeSet(TOUCH_X2, gpioModePushPull, 0);
                       GPIO_PinModeSet(TOUCH_Y1, gpioModeInput, 0);
                       GPIO_PinModeSet(TOUCH_Y2, gpioModeInput, 0);
                       sInit.input = ADC_X;
                       sInit.acqTime = adcAcqTime16; /* pressed, so speed-up */
                       ADC_InitSingle(ADC0, &sInit);
                       current_pos.pen = newpos.pen;
                       TOUCH_RecalculatePosition(&newpos);
                       if(newpos.pen)
                       { int call_upcall = TOUCH_StateChanged(); 
                         if(call_upcall)
                         {
                            current_pos.x = newpos.x;
                            current_pos.y = newpos.y;
                         }
                         current_pos.adcx = newpos.adcx; 
                         current_pos.adcy = newpos.adcy; 
                         current_pos.pen = 1;
                         if(call_upcall) TOUCH_CallUpcall();
                       }
                       newpos.pen = 1;
                     } else 
                     { touch_state = TOUCH_INIT;
                       newpos.pen = 0;
                       current_pos.pen = 0;
                       TOUCH_CallUpcall();
                     }
                     break;
    case TOUCH_MEASURE_Y: /* touch panel pressed, measure Y position */
                     newpos.adcy = (ADC_DataSingleGet(ADC0)+31)>>6; /* reduce ADC resolution to 10-bits */
                     GPIO_PinModeSet(TOUCH_Y1, gpioModePushPull, 0);/* to avoid overflow in calibration routines */
                     GPIO_PinModeSet(TOUCH_Y2, gpioModePushPull, 1);
                     GPIO_PinModeSet(TOUCH_X1, gpioModeInput, 0);
                     GPIO_PinModeSet(TOUCH_X2, gpioModeInput, 0);
                     sInit.input = ADC_Y;
                     ADC_InitSingle(ADC0, &sInit);
                     touch_state = TOUCH_MEASURE_X;
                     break;
    case TOUCH_MEASURE_X: /* touch panel pressed, measure X position */
                     newpos.adcx = (ADC_DataSingleGet(ADC0)+31)>>6;
                     GPIO_PinModeSet(TOUCH_Y1, gpioModePushPull, 1);
                     GPIO_PinModeSet(TOUCH_Y2, gpioModePushPull, 1);
                     GPIO_PinModeSet(TOUCH_X1, gpioModePushPull, 0);
                     GPIO_PinModeSet(TOUCH_X2, gpioModeInput, 0);
                     sInit.input = ADC_Y;
                     ADC_InitSingle(ADC0, &sInit);
                     touch_state = TOUCH_CHECK_PRESS;
                     break;
    default: touch_state = TOUCH_INIT;
  }
  ADC_IntClear(ADC0, ADC_IF_SINGLE);
  ADC_Start(ADC0, adcStartSingle);
}

/***************************************************************************//**
 * @brief Initialize touch panel driver
 ******************************************************************************/
void TOUCH_Init(TOUCH_Config_TypeDef *config)
{ ADC_Init_TypeDef init = ADC_INIT_DEFAULT;
  
#ifndef TOUCH_WITHOUT_STORE
  touch_LoadCalibration();
#endif
  CMU_ClockEnable( cmuClock_ADC0, true);
  ADC_IntDisable(ADC0, _ADC_IF_MASK);
  init.prescale = ADC_PrescaleCalc(config->frequency, 0);
  touch_ignore_move = config->ignore;
  init.ovsRateSel = config->oversampling;
  ADC_Init(ADC0, &init);
  DVK_ENABLE_ADC;
  sInit.input = ADC_Y;
  sInit.reference = adcRefVDD;
  sInit.resolution = adcResOVS;
  ADC_InitSingle(ADC0, &sInit);
  ADC_IntClear(ADC0, _ADC_IF_MASK);
  touch_state = TOUCH_INIT;
  NVIC_ClearPendingIRQ(ADC0_IRQn);
  NVIC_EnableIRQ(ADC0_IRQn);  
  ADC_IntEnable(ADC0, ADC_IF_SINGLE);
  ADC_Start(ADC0, adcStartSingle);
}

/***************************************************************************//**
 * @brief Returns current touch position and state
 ******************************************************************************/
TOUCH_Pos_TypeDef *TOUCH_GetPos(void)
{ 
  ADC_IntDisable(ADC0, ADC_IF_SINGLE); /* we need to disable ADC interrupt to avoid current_pos structure update for a while */
  pos.pen = current_pos.pen;
  pos.x = current_pos.x;
  pos.y = current_pos.y;
  pos.adcx = current_pos.adcx;
  pos.adcy = current_pos.adcy;
  ADC_IntEnable(ADC0, ADC_IF_SINGLE);

  return &pos;
}

/***************************************************************************//**
 * @brief Register upcall which will be call every position or state change
 * @param pointer to upcall function
 ******************************************************************************/
void TOUCH_RegisterUpcall(TOUCH_Upcall_TypeDef *new_upcall)
{
  upcall = new_upcall;
}


/***************************************************************************//**
 * @brief Set calibration table
 * @param table of display points
 * @param table of adc values reflecting display points
 ******************************************************************************/
int TOUCH_CalibrationTable(POINT * displayPtr, POINT * screenPtr)
{ int result;
	
  result = setCalibrationMatrix(displayPtr, screenPtr, &calibrationMatrix);
#ifndef TOUCH_WITHOUT_STORE
  if(result==OK)
		touch_StoreCalibration();
#endif
	return(result);
}
