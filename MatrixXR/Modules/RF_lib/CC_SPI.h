//------------------------------------------------------------------------------
//  Description:  Header file for TI_CC_spi.c
//
//  MSP430/CC1100-2500 Interface Code Library v1.1
//
//  W. Goh
//  Texas Instruments, Inc.
//  December 2009
//  IAR Embedded Workbench v4.20
//------------------------------------------------------------------------------
// Change Log:
//------------------------------------------------------------------------------
// Version:  1.1
// Comments: Fixed function bugs
//           Added support for 5xx
//
// Version:  1.00
// Comments: Initial Release Version
//------------------------------------------------------------------------------
#ifndef _CC_SPI_H_
#define _CC_SPI_H_

void CC_SPI_Setup_SPI(void);
void CC_SPI_Write_Reg(char addr, char value);
void CC_SPI_Write_Burst_Reg(char addr, char *buffer, char count);
char CC_SPI_Read_Reg(char addr);
void CC_SPI_Read_Burst_Reg(char addr, char *buffer, char count);
char CC_SPI_Read_Status(char addr);
void CC_SPI_Strobe(char strobe);
void CC_PowerupResetCCxxxx(void);



#endif



