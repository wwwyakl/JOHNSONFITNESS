//------------------------------------------------------------------------------
//  Description:  CC1100-CC2500.c function declarations
//
//  Demo Application for MSP430/CC1100-2500 Interface Code Library v1.0
//
//  K. Quiring
//  Texas Instruments, Inc.
//  July 2006
//  IAR Embedded Workbench v3.41
//------------------------------------------------------------------------------

#ifndef _CC1100_CC2500_H_
#define _CC1100_CC2500_H_


void writeRFSettings(void);
void RFSendPacket(char *, char);
char RFReceivePacket(char *rxBuffer, char *length);
#endif