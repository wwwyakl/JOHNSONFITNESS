#ifndef __SPI_Flash_HAL_H__
#define __SPI_Flash_HAL_H__
#include "em_gpio.h"

typedef enum
{ Bit_RESET = 0,
  Bit_SET
}BitAction;
#define CS_PORT gpioPortA
#define CS_PIN   11
#define CLK_PORT gpioPortC
#define CLK_PIN   4
#define DI_PORT gpioPortC
#define DI_PIN   2
#define DO_PORT gpioPortC
#define DO_PIN   3


void HAL_SPI_Flash_Initial(void);
void HAL_SPI_Flash_CS(bool isHigh);

void HAL_SPI_Flash_CLK(bool isHigh);
void HAL_SPI_Flash_DI(bool isHigh);
bool HAL_SPI_Flash_DO(void);
#endif